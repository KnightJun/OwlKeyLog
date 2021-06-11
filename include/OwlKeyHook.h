#pragma once
#include <QObject>
#include <QSet>
#include <QPoint>

#ifndef BUILD_STATIC
# if defined(OWLKEYLOG_LIB)
#  define OWLKEYLOG_EXPORT Q_DECL_EXPORT
# else
#  define OWLKEYLOG_EXPORT Q_DECL_IMPORT
# endif
#else
# define OWLKEYLOG_EXPORT
#endif

class OWLKEYLOG_EXPORT OwlKeyHook : public QObject
{
    Q_OBJECT
public:
    OwlKeyHook(QObject *parent = nullptr);
    ~OwlKeyHook();

    static quint64 __MouseHook;
    static quint64 __KeyboardHook;
    
    void setKeyHookEnable(bool enable);
    void setMouseHookEnable(bool enable);
    void __setKeyHookEnable(bool enable);
    bool getKeyHookEnable(){return __KeyboardHook;};
    void __setMouseHookEnable(bool enable);
    bool getMouseHookEnable(){return __MouseHook;};
    void setTrackMouseMove(bool enable){__TrackMouseMove = enable;};
    void __EmitSigMouseMove(QPoint pos);
    void __EmitSigMouseDown(Qt::MouseButton Button, QPoint pos);
    void __EmitSigMouseUp(Qt::MouseButton Button, QPoint pos);
    void __EmitSigWheelRoll(int val, QPoint pos);
    void __EmitSigKeyDown(Qt::Key k);
    void __EmitSigKeyUp(Qt::Key k);
    static QSet<OwlKeyHook *> __dPtrSet;
    static bool __TrackMouseMove;

signals:
    void SigMouseMove(QPoint pos);
    void SigMouseDown(Qt::MouseButton Button, QPoint pos);
    void SigMouseUp(Qt::MouseButton Button, QPoint pos);
    void SigWheelRoll(int val, QPoint pos);
    void SigKeyDown(Qt::Key k);
    void SigKeyUp(Qt::Key k);

    void sigKeyHookEnable(bool enable);
    void sigMouseHookEnable(bool enable);

private:
};
