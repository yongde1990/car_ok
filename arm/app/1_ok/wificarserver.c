#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <linux/tcp.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <poll.h>
#include <signal.h>

#include "config.h"

/* 与客户端相关部分 */
/* 小车方向 */
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define STOP 4

#define GETDATA 5

/* socket
 * bind
 * listen
 * accept
 * send/recv
 */

#define SERVERPORT 8980
#define BACKLOG 10

char *ip = "192.168.7.1";

typedef struct mesg_car{
	unsigned char distect;

	char temperature;
	unsigned char humidity;
} mesg;

mesg car_request;

int GlobalClientFd;

/* 与stm32 相关部分 */
#define ReadHDT11Data 		0xF0
#define CarBEFOR          		0x06
#define CarSTOP          		0x00
#define CarBACK          		0x09
#define CarTurnBeforeRight  	0x04
#define CarTurnBeforeLift   	0x02

#define CarTurnBackRight    	0x08
#define CarTurnBackLift     	0x01

#define BSP115200 115200
#define BSP9600     9600

#define MyUartDrivesName "/dev/UartArm9Stm32DataCharge"

Stm32SeniorData GetStmData;
unsigned char CmdToStm32;
int fd_uart_to_32;

/* 事件 */
#define QT_Request 1
#define CALL_Hoster 2

#define TH_FIRE 2048

int event = 0;

/* 异步读数据 */
void fasync_uart_read(int signum){
	int isendlen;
	read(fd_uart_to_32, &GetStmData, 8);
	//write(fd, &Data, 8);
	/* 解析数据; 并执行相应行为 */
	if(event == QT_Request){
		car_request.temperature = (char)GetStmData.TemperatureHigh;
		car_request.humidity = GetStmData.HumidityHigh;
		printf("temperature is %d\n", car_request.temperature);
		printf("humidity is %d\n", car_request.humidity);
		isendlen = send(GlobalClientFd, &car_request, sizeof(struct mesg_car), 0);
		if(isendlen <= 0){
			close(GlobalClientFd);
			printf("when send data, find close %d client, the link over\n",GlobalClientFd);
		}
		event = 0;
	}
	if(GetStmData.FireDataHigh > TH_FIRE){
		/* 用GSM 模块发送给主人，提醒有紧急事件发生 */
	}
}

/* 设置串口 */
int UartSet(void){
	int ret;
	fd_uart_to_32 = open(MyUartDrivesName, O_RDWR);  
	if(fd_uart_to_32 < 0){  
	    printf("Error: open /dev/uart1 error!/n");  
	    return 1;  
	} 
	ret = ioctl(fd_uart_to_32, BSP9600, 9600);
	memset(&GetStmData, 0, sizeof(struct SeniorData));
	return 0;
}

/* 异步机制设置 */
void FasyncSet(void){
	int oflag;
	signal(SIGIO, fasync_uart_read);
	fcntl(fd_uart_to_32, F_SETOWN,getpid());
	oflag = fcntl(fd_uart_to_32, F_GETFL); 
	fcntl(fd_uart_to_32, F_SETFL, oflag | FASYNC);
}

int main(int argc, char* argv[]){
	int iSocketServerFd;
	int iSocketClientFd;
	int iRet;
	int iAddLen;
	int iRecvLen;
	int iSendLen;
	int iClientNum = -1;
	int lenRequest;
	
	lenRequest = sizeof(struct mesg_car);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	//unsigned char 	ucRecvBuf[4];
	
	iSocketServerFd = socket(AF_INET, SOCK_STREAM, 0);
	printf("open a socket , socket fd = %d\n",iSocketServerFd);
	
	if(iSocketServerFd == -1){
		printf("open socket error!\n");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port	 = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	
#if 0
	if (0 == inet_aton(ip, &serveraddr.sin_addr))
 	{
		printf("invalid server_ip\n");
		return -1;
	}
#endif

	memset(serveraddr.sin_zero, 0, 8);

	iRet = bind(iSocketServerFd, (const struct sockaddr *)&serveraddr, sizeof(struct sockaddr));
	if (-1 == iRet){
		printf("bind error!\n");
		return -1;
	}
	printf("bind succeed!\n");
	
	iRet = listen(iSocketServerFd, BACKLOG);
	if (-1 == iRet){
		printf("listen error!\n");
		return -1;
	}
	printf("listen succeed!\n");

/* stm32 相关部分 */
	UartSet();
	
	FasyncSet();
	
	while(1){
		iAddLen = sizeof(struct sockaddr);
		printf("accept ready!\n");
		iSocketClientFd = accept(iSocketServerFd, (struct sockaddr *)&clientaddr, &iAddLen);	
		printf("accept ok!\n");
		if (-1 != iSocketClientFd){
			iClientNum++;
			/* 子进程返回值为0 */
			printf("%s client connet to host\n",inet_ntoa(clientaddr.sin_addr));
			if(!fork()){
				while (1)
				{
					/* 接收客户端发来的数据并显示出来 */
					//iRecvLen = recv(iSocketClientFd, ucRecvBuf, 1, 0);
					memset(&car_request, 0x0, lenRequest);
					iRecvLen = recv(iSocketClientFd, &car_request, lenRequest, 0);
					if (iRecvLen <= 0){
						close(iSocketClientFd);
						printf("when recive data, find close %d client, the link over\n",iSocketClientFd);
						return -1;
					}
					else{
						//ucRecvBuf[iRecvLen] = '\0';
						switch(car_request.distect){
							case UP:						
								printf("Get Msg From Client %d: %d is car up\n", iClientNum,car_request.distect);
								CmdToStm32 = CarBEFOR;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));								
								break;
							case LEFT:
								printf("Get Msg From Client %d: %d is car left\n", iClientNum,car_request.distect);
								CmdToStm32 = CarTurnBeforeLift;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));								
								break;						
							case RIGHT:
								printf("Get Msg From Client %d: %d is car right\n", iClientNum,car_request.distect);
								CmdToStm32 = CarTurnBeforeRight;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));
								break;
							case DOWN:						
								printf("Get Msg From Client %d: %d is car down\n", iClientNum,car_request.distect);
								CmdToStm32 = CarBACK;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));
								break;
							case STOP:
								printf("Get Msg From Client %d: %d is car stop\n", iClientNum,car_request.distect);
								CmdToStm32 = CarSTOP;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));
								break;
							case GETDATA:
								printf("Get Msg From Client %d: %d is car get data\n", iClientNum,car_request.distect);
								CmdToStm32 = ReadHDT11Data;
								write(fd_uart_to_32, &CmdToStm32, sizeof(CmdToStm32));
								GlobalClientFd = iSocketClientFd;
								event = QT_Request;
								#if 0
								car_request.temperature = 11;
								car_request.humidity = 88;
								iSendLen = send(iSocketClientFd, &car_request, lenRequest, 0);
								if(iSendLen <= 0){
									close(iSocketClientFd);
									return -1;
								}
								#endif
								
								break;
							default :
								printf("Get Msg From Client %d: is not undefine\n", iClientNum,car_request.distect);
								break;
						}
					}
				}			
			}
		}
	}

	close(fd_uart_to_32); 
	close(iSocketServerFd);
	return 0;
}

