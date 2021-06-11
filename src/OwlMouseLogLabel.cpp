#include "OwlMouseLogLabel.h"
#include <QPainter>
#include <qDebug>
#include <QBitmap>
OwlMouseLogLabel::OwlMouseLogLabel(OwlKeyHook *keyhook, QRect limitRect, QWidget* parent, Qt::WindowFlags f)
{
    this->resize(mRadius*3, mRadius*3);
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    Qt::WindowFlags m_flags = windowFlags();
    setWindowFlags(m_flags | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    if(keyhook){
        connect(keyhook, &OwlKeyHook::SigMouseMove, this, &OwlMouseLogLabel::onMouseMove);
        connect(keyhook, &OwlKeyHook::SigMouseDown, this, &OwlMouseLogLabel::onMouseDown);
        connect(keyhook, &OwlKeyHook::SigMouseUp, this, &OwlMouseLogLabel::onMouseUp);
        connect(keyhook, &OwlKeyHook::SigWheelRoll, this, &OwlMouseLogLabel::onWheelRoll);
    }
    mLimitRect = limitRect;
    qDebug() << "mLimitRect: " << mLimitRect;
    QBitmap bitMap(width(),height()); // A bit map has the same size with current widget
    bitMap.fill();
    QPainter painter(&bitMap);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen); // Any color that is not QRgb(0,0,0) is right
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRoundedRect(bitMap.rect(),15,15); //设置圆角弧度
    setMask(bitMap);
}

void OwlMouseLogLabel::onMouseDown(Qt::MouseButton Button, QPoint pos)
{
    mPos = pos;
    if(Button == Qt::LeftButton) mLeftBtnPress = true;
    else if(Button == Qt::RightButton) mRightBtnPress = true;
    else if(Button == Qt::MidButton) mMidBtnPress = true;
    if(checkShow())update();
}

void OwlMouseLogLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    
    QSize tSize = this->size();
    qreal w = tSize.width() / 3.0;
    QRectF dRect(0, 0, tSize.width(), tSize.height());
    painter.fillRect(dRect, mBackColor);

    dRect.setWidth(w);
    if(mLeftBtnPress)painter.fillRect(dRect, mActColor);

    dRect.moveLeft(w);
    if(mMidBtnPress)painter.fillRect(dRect, mActColor);
    
    if(mWheelVal < 0){
        QRectF wheelRect(w, tSize.height() * 0.5, w, tSize.height() * 0.5);
        painter.fillRect(wheelRect, mWheelColor);
    } else if(mWheelVal > 0){
        QRectF wheelRect(w, 0, w, tSize.height() * 0.5);
        painter.fillRect(wheelRect, mWheelColor);
    }

    dRect.moveLeft(w * 2);
    if(mRightBtnPress)painter.fillRect(dRect, mActColor);

}

void OwlMouseLogLabel::onMouseUp(Qt::MouseButton Button, QPoint pos)
{
    mPos = pos;
    if(Button == Qt::LeftButton) mLeftBtnPress = false;
    else if(Button == Qt::RightButton) mRightBtnPress = false;
    else if(Button == Qt::MidButton) mMidBtnPress = false;
    if(checkShow())update();
}
bool OwlMouseLogLabel::checkShow()
{
    if(mLimitRect.isEmpty() || mLimitRect.contains(mPos))
    {
        if(mLeftBtnPress || mRightBtnPress || mMidBtnPress || mWheelVal){
            this->move(mPos.x() - this->width() / 2, mPos.y() - this->height() / 2);
            if(this->isVisible() == false) this->show();
            return true;
        }
    }
    if(this->isVisible())this->hide();
    return false;
}
void OwlMouseLogLabel::onMouseMove(QPoint pos)
{
    mPos = pos;
    checkShow();
}
void OwlMouseLogLabel::onWheelRoll(int val, QPoint pos)
{
    mPos = pos;
    mWheelVal = val;
    if(checkShow())update();
    this->raise();
    this->hideLater();
}
void OwlMouseLogLabel::hideLater()
{
    if(this->mTimeId > -1){
        this->killTimer(this->mTimeId);
    }
    this->mTimeId = this->startTimer(mHideDelayTime);
}
void OwlMouseLogLabel::timerEvent(QTimerEvent * event)
{
    mWheelVal = 0;
    update();
    if(!(mLeftBtnPress || mRightBtnPress || mMidBtnPress)){
        this->hide();
    }
}