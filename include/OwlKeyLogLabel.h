#pragma once
#include <QWidget>
#include <OwlKeyLog.h>
#include <OwlKeyHook.h>
#ifndef BUILD_STATIC
# if defined(OWLKEYLOG_LIB)
#  define OWLKEYLOG_EXPORT Q_DECL_EXPORT
# else
#  define OWLKEYLOG_EXPORT Q_DECL_IMPORT
# endif
#else
# define OWLKEYLOG_EXPORT
#endif

class OWLKEYLOG_EXPORT OwlKeyLogLabel : public QWidget
{
    Q_OBJECT
private:
    /* data */
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent * event);
public:
    OwlKeyLogLabel(OwlKeyHook *keyhook = Q_NULLPTR, QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~OwlKeyLogLabel(){};
    void startPrint(int interval);
    void stopPrint();
    OwlKeyLog keyLog;
    int timeId = -1;
};

