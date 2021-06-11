#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OwlKeyLogLabel.h"
#include "OwlMouseLogLabel.h"
#include "OwlKeyHook.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void timerEvent ( QTimerEvent * event );
    void drawKeyLog();
    ~MainWindow();

signals:
    void setMouseHookEnable(bool enable);

private:
    Ui::MainWindow *ui;
    OwlKeyLogLabel *keyLogLbl;
    OwlMouseLogLabel *mouseLogLbl;
    OwlKeyHook keyHook;
    QThread *handleThread;
};

#endif // MAINWINDOW_H
