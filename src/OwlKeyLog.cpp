#include "OwlKeyLog.h"
#include <QDebug>
#include <QPainter>
#include <QKeySequence>
void key2modifer(Qt::Key &key)
{
    switch (key)
    {
    case Qt::Key_Shift:
        key = (Qt::Key)Qt::ShiftModifier;
        break;
    case Qt::Key_Control:
        key = (Qt::Key)Qt::ControlModifier;
        break;
    case Qt::Key_Meta:
        key = (Qt::Key)Qt::MetaModifier;
        break;
    case Qt::Key_Alt:
        key = (Qt::Key)Qt::AltModifier;
        break;
    default:
        break;
    }
}
void OwlKeyLog::addToHistory()
{
    KeyRecord kr;
    kr.keyStr = mkeyStr;
    kr.dispearTime = QTime::currentTime().addSecs(2);
    kr.key = mCurKey;
    kr.modifer = mModifiers;
    mHistory.push_back(kr);
}
void OwlKeyLog::keyPress(Qt::Key key)
{
    if(mCurKey){
        addToHistory();
    }
    key2modifer(key);
    if(key & Qt::KeyboardModifierMask){
        mModifiers |= key;
    }else{
        mCurKey = key;
    }
    genKeyStr();
}
void OwlKeyLog::keyRelease(Qt::Key key)
{
    key2modifer(key);
    if(key != mCurKey && (key & Qt::KeyboardModifierMask) == 0)return;
    if(mCurKey){
        addToHistory();
    }
    if(key & Qt::KeyboardModifierMask){
        mModifiers &= (~key);
    }else{
        mCurKey = 0;
    }
    genKeyStr();
}
void OwlKeyLog::genKeyStr()
{
    mkeyStr = QKeySequence(mCurKey + mModifiers).toString();
    if(Qt::MetaModifier & mModifiers){
        mkeyStr.replace("Meta", "Win");
    }
}
void OwlKeyLog::drawKeyLog(QPainter &painter, QPoint pos)
{
    int recordCount = refreshHistory();
    auto fm = painter.fontMetrics();
    int drawHeight = (fm.height() + mPadding * 2 + mDrawGap);
    QPoint drawPos(pos.x(), pos.y() - drawHeight * recordCount + mDrawGap - 1);
    for (const auto &kr : mHistory)
    {
        qreal opa = (QTime::currentTime().msecsTo(kr.dispearTime)) / 2000.0;
        DrawBoundingText(painter, drawPos, kr.keyStr, opa);
        drawPos.setY(drawPos.y() + drawHeight);
    }
    if(recordCount > mHistory.size())
    {
        DrawBoundingText(painter, drawPos, mkeyStr, 1);
    }
}
int OwlKeyLog::refreshHistory()
{    
    int recordCount = mHistory.size();
    while (recordCount > mMaxRecordCount){
        recordCount--;
        mHistory.erase(mHistory.begin());
    }
    while (!mHistory.isEmpty() && mHistory.begin()->dispearTime < QTime::currentTime())
    {
        recordCount--;
        mHistory.erase(mHistory.begin());
    }
    if(!mCurKey && !mModifiers)return recordCount;
    if(mCurKey
    || mHistory.isEmpty()
    || !(mHistory.last().modifer & mModifiers)){
        if(mHistory.size() == mMaxRecordCount)
        {
            mHistory.erase(mHistory.begin());
        }else{
            recordCount++;
        }
    }
    return recordCount;
}

void OwlKeyLog::DrawBoundingText(QPainter &painter, 
    const QPoint &pos, 
    const QString &str,
    qreal opacity)
{
    QFontMetrics fm = painter.fontMetrics();
    int width = fm.width(str);
    QRectF drawRect(pos.x(), pos.y(), width + mPadding * 2, fm.height() + mPadding * 2);
    painter.setOpacity(opacity);
    painter.setBrush(QBrush(qRgba(0, 0, 0, 150)));
    painter.setPen(QPen(Qt::white));
    painter.fillRect(drawRect, Qt::SolidPattern);
    painter.drawRect(drawRect);
    painter.drawText(drawRect, Qt::AlignCenter, str);
}