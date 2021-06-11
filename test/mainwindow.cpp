#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <qkeysequence.h>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QKeyEvent>
#include <QThread>
void MainWindow::timerEvent(QTimerEvent * event)
{
    
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    printf("qt key:0x%X\n", event->key());
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    keyLogLbl = new OwlKeyLogLabel(&keyHook);
    mouseLogLbl  = new OwlMouseLogLabel(&keyHook, this->geometry());
    handleThread = new QThread(this);
    keyLogLbl->move(0, 0);
    keyLogLbl->show();
    qDebug() << "main thread:" << QThread::currentThreadId();
    keyHook.moveToThread(handleThread);
    handleThread->start();

    keyHook.setKeyHookEnable(true);
    keyHook.setMouseHookEnable(true);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete mouseLogLbl;
    delete keyLogLbl;
    handleThread->quit();
}
