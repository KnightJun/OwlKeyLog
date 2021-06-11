#include <OwlKeyLogLabel.h>
#include <QPainter>
OwlKeyLogLabel::OwlKeyLogLabel(OwlKeyHook *keyhook, QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
    this->resize(350, 150);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(15);
    this->setFont(font);
    this->startTimer(100);

    if(keyhook){
        connect(keyhook, &OwlKeyHook::SigKeyDown, &keyLog, &OwlKeyLog::keyPress);
        connect(keyhook, &OwlKeyHook::SigKeyUp, &keyLog, &OwlKeyLog::keyRelease);
    }
    if(parent == nullptr){
        setAttribute(Qt::WA_TransparentForMouseEvents,true);
        Qt::WindowFlags m_flags = windowFlags();
        setWindowFlags(m_flags | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setAttribute(Qt::WA_TranslucentBackground);
    }
}

void OwlKeyLogLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    keyLog.drawKeyLog(painter,  QPoint(0, this->height()));
    return;
}
void OwlKeyLogLabel::timerEvent(QTimerEvent * event)
{
    this->update();
}
void OwlKeyLogLabel::startPrint(int interval)
{
    if(timeId > -1){
        this->killTimer(timeId);
    }
    timeId = this->startTimer(interval);
}

void OwlKeyLogLabel::stopPrint()
{
    if(timeId > -1){
        this->killTimer(timeId);
        timeId = -1;
    }
}