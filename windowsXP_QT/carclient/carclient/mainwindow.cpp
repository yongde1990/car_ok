#include "mainwindow.h"
#include "ui_mainwindow.h"

/* С������ */
#define UP 0
#define LEFT 1
#define RIGHT 2
#define DOWN 3
#define STOP 4

#define GETDATA 5

/* ���͵����� */

/*
 * С������
 * ��������
 */
struct mesg_car{
    unsigned char dirtect;

    char temperature;
    unsigned char humidity;
};

struct mesg_car request;

//char request = 'T';

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    carclient = new QTcpSocket;

    ui->tp_Number->setStyleSheet("background: rgb(127,127,95);border-color: rgb(255,255,0);");
    ui->sd_Number->setStyleSheet("background: rgb(127,127,95);border-color: rgb(255,255,0);");
    /* �����ź���۵Ĺ�ϵ �첽 */
    connect(carclient,SIGNAL(connected()),this,SLOT(socketsucceed()));
    connect(carclient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socketerror()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete carclient;
}

void MainWindow::on_connect_clicked()
{
     /* 1. ״̬��ʾ*/
     ui->statuslabel->setText(tr("connect..."));
     /* 2.���ӷ������� */
     carclient->connectToHost(ui->ipserveradd->text(),ui->port->text().toInt());
}

void MainWindow::socketsucceed(){
    /* 1. ״̬��ʾ*/
    ui->statuslabel->setText(tr("connect succeed"));
}

void MainWindow::socketerror(){
    /* 1. ״̬��ʾ*/
    ui->statuslabel->setText(tr("connect error"));
}

void MainWindow::on_disconnect_clicked()
{
    /* 1. ״̬��ʾ*/
    ui->statuslabel->setText(tr("disconnect..."));
    /* 2.���ӷ������� */
    carclient->abort();

    ui->statuslabel->setText(tr("disconnect succeed!\n"));
}

void MainWindow::on_send_clicked()
{
    //carclient->write((const char *)&request,sizeof(char));
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = GETDATA;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);

    carclient->read((char*)&request, sizeof(struct mesg_car));
    qDebug("get data from : temperature %d ; humidity %d",request.temperature,request.humidity);

    ui->tp_Number->display(request.temperature);
    ui->sd_Number->display(request.humidity);
}

void MainWindow::on_UP_Button_clicked()
{
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = UP;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);
}

void MainWindow::on_LEFT_Button_clicked()
{
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = LEFT;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);
}

void MainWindow::on_RIGHT_Button_clicked()
{
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = RIGHT;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);
}

void MainWindow::on_DOWN_Button_clicked()
{
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = DOWN;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);
}

void MainWindow::on_STOP_Button_clicked()
{
    /* �������� */
    memset(&request, 0x0, sizeof(struct mesg_car));
    request.dirtect = STOP;
    /* send request */
    carclient->write((const char*)&request, sizeof(struct mesg_car));
    qDebug("send to host %d\n",request.dirtect);
}
