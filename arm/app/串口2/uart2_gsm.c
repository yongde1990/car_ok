#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/sched.h> 
#include <linux/miscdevice.h> 
#include <linux/delay.h> 
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/device.h>
#include <mach/gpio.h>

#include <linux/irq.h>
#include <linux/poll.h>

#define MY_DEVICE_NAME  "Uart2ForGSM"    //设备文件名
#define MY_CLASS_NAME  "UART2ForGSM"

#define NUM_DATA_R  8

unsigned char ucSendUartData[8];
/* 数据长度 */
#define DATALENGTH 30
unsigned char DataFromUser[DATALENGTH];
unsigned char DataToUser[50];


#define BSP115200 115200
#define BSP9600     9600

static struct class 		*uartdrv_class;
static struct device	*uartdrv_class_dev;

volatile unsigned long *UART_ULCON2 = NULL;
volatile unsigned long *UART_UCON2 = NULL;
volatile unsigned long *UART_URXH2 = NULL;
volatile unsigned long *UART_UTXH2 = NULL;
volatile unsigned long *UART_UBRDIV2 = NULL;
volatile unsigned long *UART_UTRSTAT2 = NULL;
volatile unsigned long *UART_UFCON2 = NULL; 
volatile unsigned long *UART_UFSTAT2 = NULL; 
volatile unsigned long *GPHCON = NULL; 

static volatile int UartReadFlage = 0;
static DECLARE_WAIT_QUEUE_HEAD(UartReadWaitQuene);

static struct fasync_struct *UartAsync;


/* 串口 中断机制 */
static irqreturn_t UartReadIrqHandler(int irq, void *dev_id){
	UartReadFlage = 1;
	wake_up_interruptible(&UartReadWaitQuene);
	kill_fasync (&UartAsync, SIGIO, POLL_IN);
	return IRQ_HANDLED;
}

/* 异步通知机制初始化 */
static int uart_drv_fasync (int fd, struct file *filp, int on){
	return fasync_helper (fd, filp, on, &UartAsync);
}

static int myuart_open(struct inode *inode, struct file * file) 
{ 
	int UartIrq;
	printk("uart 2 open\n");
	/* 设置串口2 为uart 模式，对应开发板中uart2 口 */
	*GPHCON &= ~((0x3<<(6*2))|(0x3<<(7*2))) ;
	*GPHCON |= ((0x2<<(6*2))|(0x2<<(7*2)));
	/* 设置寄存器 */
	*UART_UCON2 =  0x05;
	*UART_ULCON2 = 0x03;

	irq_set_irq_type(IRQ_S3CUART_RX2, IRQF_TRIGGER_FALLING);

	UartIrq = request_irq(IRQ_S3CUART_RX2, UartReadIrqHandler, IRQF_DISABLED, "uart2_read_irq", NULL);
	if (UartIrq)
	{
		printk("serial read irq not registered. Err:%d\n", UartIrq);
	}
	printk("serial read irq registered success.\n");

	//disable_irq(IRQ_S3CUART_RX1);
	//enable_irq(IRQ_S3CUART_RX1);
	return 0; 
} 
 
/* 从设备读取数据，上层的read函数会调用到这里，file是read的fd对应的结构, ppos是系统回调 */
static ssize_t myuart_read (struct file *file, char __user *buff, size_t count, loff_t *ppos){
	//unsigned char ucGetUartData[8];
	int i;
	wait_event_interruptible(UartReadWaitQuene, UartReadFlage != 0);
	UartReadFlage = 0;
	for(i = 0; i < count; i++)
		DataToUser[i] = readb(UART_URXH2);

	copy_to_user(buff, &DataToUser, count);
	return strlen(buff);/* 返回buff的长度 */
}
/* 向设备写入数据，上层的write函数会调用到这里，file是write的fd对应的结构, offp是系统回调 */
static ssize_t myuart_write (struct file *file, const char __user *buff, size_t count, loff_t *offp){ 
	unsigned char i = 0;
	//copy_from_user(ucSendUartData, buff, count);
	copy_from_user(DataFromUser, buff, count);
	for (i = 0 ; i < count; i++){
		writeb(DataFromUser[i], UART_UTXH2);
	}
	return strlen(buff);
} 

static long myuart_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
      switch(cmd){ 
        case BSP115200:{ 
            if(arg == 115200){ 
                *UART_UBRDIV2 = 26; 
            } 
        };
	break; 
 	case BSP9600:{ 
            if(arg == 9600) { 
                *UART_UBRDIV2 = 325; 
            } 
        };
	break; 
       default: 
	   	*UART_UBRDIV2 = 325;
      } 
	return 0;
} 

static int myuart_close(struct inode *inode, struct file *filp) 
{ 
	printk("uart 2 close\n");
	free_irq(IRQ_S3CUART_RX2,NULL);
	return 0; 
} 
 /* file_operations结构是建立驱动程序和设备编号的连接，内部是一组函数指针，每个打开的文件，也就是file结构，和一组函数关联，这些操作主要用来实现系统调用的 */
static struct file_operations uart_fops = { 
 
      .owner 		=  	THIS_MODULE, 
      .open 		=  	myuart_open, 
      .unlocked_ioctl =  	myuart_ioctl, 
      .read			=    	myuart_read, 
      .write		=   	myuart_write, 
      .release		= 	myuart_close ,
      .fasync	 	=  	uart_drv_fasync,
};

int major; 
 /* 初始化驱动模块,驱动程序要注册一个设备文件，并对其进行操作 */
static int uart_init(void){
	
	 /* 注册一个字符设备，并分配设备编号，major是设备号，name是驱动程序名称 fops是file_operations结构 */
      	major = register_chrdev(0, MY_DEVICE_NAME, &uart_fops); 
	if (major < 0)  { 
	        printk(MY_DEVICE_NAME " can't register major number\n"); 
	        return major; 
	 }
 	uartdrv_class = class_create(THIS_MODULE, MY_CLASS_NAME);
	uartdrv_class_dev = device_create(uartdrv_class, NULL, MKDEV(major, 0), NULL, MY_DEVICE_NAME); /* /dev/uart */
	UART_ULCON2 	= (volatile unsigned long *)ioremap(0x50008000, 40);
	UART_UCON2  	= (volatile unsigned long *)ioremap(0x50008004, 40);
	UART_UTRSTAT2 = (volatile unsigned long *)ioremap(0x50008010, 40); 	
	UART_URXH2  	= (volatile unsigned long *)ioremap(0x50008024, 40);
	UART_UTXH2 	= (volatile unsigned long *)ioremap(0x50008020, 40);
	UART_UBRDIV2 	= (volatile unsigned long *)ioremap(0x50008028, 40);
	GPHCON 		= (volatile unsigned long *)ioremap(0x56000070, 40);	
	return 0; 
} 

static void  uart_exit(void) 
{
	//disable_irq(IRQ_S3CUART_RX1);
	
	
	device_destroy(uartdrv_class,MKDEV(major, 0));
	class_destroy(uartdrv_class);
	/* 如果不使用该设备时释放编号 */
	unregister_chrdev(major, MY_DEVICE_NAME); 
	iounmap(GPHCON);
	iounmap(UART_ULCON2);
	iounmap(UART_UFCON2);
	iounmap(UART_UFSTAT2);
	iounmap(UART_UCON2);
	iounmap(UART_URXH2);
	iounmap(UART_UTXH2);
	iounmap(UART_UBRDIV2);
	iounmap(UART_UTRSTAT2);
	printk(" ***** exit uart! *****\n"); 
} 
module_init(uart_init); 
module_exit(uart_exit); 
MODULE_LICENSE("GPL");
