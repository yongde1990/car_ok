#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTcpSocket *carclient;

private:
    Ui::MainWindow *ui;

private slots:
    void on_STOP_Button_clicked();
    void on_DOWN_Button_clicked();
    void on_RIGHT_Button_clicked();
    void on_LEFT_Button_clicked();
    void on_UP_Button_clicked();
    void on_send_clicked();
    void on_disconnect_clicked();
    void on_connect_clicked();
    void socketsucceed();
    void socketerror();
};

#endif // MAINWINDOW_H
