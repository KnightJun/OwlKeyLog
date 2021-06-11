#pragma once
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QPainter>

#ifndef BUILD_STATIC
# if defined(OWLKEYLOG_LIB)
#  define OWLKEYLOG_EXPORT Q_DECL_EXPORT
# else
#  define OWLKEYLOG_EXPORT Q_DECL_IMPORT
# endif
#else
# define OWLKEYLOG_EXPORT
#endif

class OWLKEYLOG_EXPORT OwlKeyLog : public QObject
{
    Q_OBJECT
public:
    struct KeyRecord
    {
        QTime dispearTime;
        QString keyStr;
        int modifer;
        int key;
    };
    OwlKeyLog(QObject *parent = nullptr):QObject(parent){};
    void drawKeyLog(QPainter &painter, QPoint pos);
    void DrawBoundingText(QPainter &painter, const QPoint &pos, const QString &str,
    qreal opacity);
    ~OwlKeyLog(){};
    void keyPress(Qt::Key key);
    void keyRelease(Qt::Key key);
    
private:
    void genKeyStr();
    int mCurKey = 0;
    int mMaxRecordCount = 3;
    int mDrawGap = 10;
    int mPadding = 2;
    QString mkeyStr;
    quint32 mModifiers = 0;
    QList<KeyRecord> mHistory;
    void addToHistory();
    int refreshHistory();
};
