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

class OWLKEYLOG_EXPORT OwlMouseLogLabel : public QWidget
{
    Q_OBJECT
private:
    void onMouseDown(Qt::MouseButton Button, QPoint pos);
    void onMouseUp(Qt::MouseButton Button, QPoint pos);
    void onWheelRoll(int val, QPoint pos);
    void onMouseMove(QPoint pos);
    void hideLater();
    /* data */
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent * event);
public:
    OwlMouseLogLabel(OwlKeyHook *keyhook = Q_NULLPTR, QRect limitRect = QRect(), 
        QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~OwlMouseLogLabel(){};

    bool checkShow();
    void setLimitRect(QRect limitRect){mLimitRect = limitRect;};
    int timeId = -1;
    const int mRadius = 8;
    QPoint mPos;
    bool mLeftBtnPress = false;
    bool mRightBtnPress = false;
    bool mMidBtnPress = false;
    int mWheelVal = 0;
    QColor mBackColor = QColor(255, 0, 0, 60);
    QColor mActColor = QColor(255, 0, 0, 180);
    QColor mWheelColor = QColor(0, 0, 255, 180);
    QRect mLimitRect;
    int mTimeId = -1;
    int mHideDelayTime = 300;
};

