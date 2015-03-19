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

#define MY_DEVICE_NAME  "Uart2ForGSM"    //�豸�ļ���
#define MY_CLASS_NAME  "UART2ForGSM"

#define NUM_DATA_R  8

unsigned char ucSendUartData[8];
/* ���ݳ��� */
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


/* ���� �жϻ��� */
static irqreturn_t UartReadIrqHandler(int irq, void *dev_id){
	UartReadFlage = 1;
	wake_up_interruptible(&UartReadWaitQuene);
	kill_fasync (&UartAsync, SIGIO, POLL_IN);
	return IRQ_HANDLED;
}

/* �첽֪ͨ���Ƴ�ʼ�� */
static int uart_drv_fasync (int fd, struct file *filp, int on){
	return fasync_helper (fd, filp, on, &UartAsync);
}

static int myuart_open(struct inode *inode, struct file * file) 
{ 
	int UartIrq;
	printk("uart 2 open\n");
	/* ���ô���2 Ϊuart ģʽ����Ӧ��������uart2 �� */
	*GPHCON &= ~((0x3<<(6*2))|(0x3<<(7*2))) ;
	*GPHCON |= ((0x2<<(6*2))|(0x2<<(7*2)));
	/* ���üĴ��� */
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
 
/* ���豸��ȡ���ݣ��ϲ��read��������õ����file��read��fd��Ӧ�Ľṹ, ppos��ϵͳ�ص� */
static ssize_t myuart_read (struct file *file, char __user *buff, size_t count, loff_t *ppos){
	//unsigned char ucGetUartData[8];
	int i;
	wait_event_interruptible(UartReadWaitQuene, UartReadFlage != 0);
	UartReadFlage = 0;
	for(i = 0; i < count; i++)
		DataToUser[i] = readb(UART_URXH2);

	copy_to_user(buff, &DataToUser, count);
	return strlen(buff);/* ����buff�ĳ��� */
}
/* ���豸д�����ݣ��ϲ��write��������õ����file��write��fd��Ӧ�Ľṹ, offp��ϵͳ�ص� */
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
 /* file_operations�ṹ�ǽ�������������豸��ŵ����ӣ��ڲ���һ�麯��ָ�룬ÿ���򿪵��ļ���Ҳ����file�ṹ����һ�麯����������Щ������Ҫ����ʵ��ϵͳ���õ� */
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
 /* ��ʼ������ģ��,��������Ҫע��һ���豸�ļ�����������в��� */
static int uart_init(void){
	
	 /* ע��һ���ַ��豸���������豸��ţ�major���豸�ţ�name�������������� fops��file_operations�ṹ */
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
	/* �����ʹ�ø��豸ʱ�ͷű�� */
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
