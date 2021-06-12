#include "OwlKeyHook.h"
#include <windows.h>
#include <QDebug>
#include <QThread>
#include <QMetaType>
#include "WinKeyToQtTbl.h"
quint64 OwlKeyHook::__MouseHook = 0;
quint64 OwlKeyHook::__KeyboardHook = 0;
bool OwlKeyHook::__TrackMouseMove = false;
QSet<OwlKeyHook *> OwlKeyHook::__dPtrSet = QSet<OwlKeyHook *>();
LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI MyKeyBoardCallback(int nCode, WPARAM wParam, LPARAM lParam);
bool gMouseDown = false;
OwlKeyHook::OwlKeyHook(QObject *parent):QObject(parent)
{
    if(__dPtrSet.isEmpty()){
        qInfo() << ("First OwlKeyHook object.");
        qRegisterMetaType<Qt::Key>("Qt::Key");
        qRegisterMetaType<Qt::MouseButton>("Qt::MouseButton");
    }
    qInfo() << ("OwlKeyHook init");
    __dPtrSet.insert(this);
    connect(this, &OwlKeyHook::sigKeyHookEnable, this, &OwlKeyHook::__setKeyHookEnable);
    connect(this, &OwlKeyHook::sigMouseHookEnable, this, &OwlKeyHook::__setMouseHookEnable);
}

void OwlKeyHook::setMouseHookEnable(bool enable)
{
    emit sigMouseHookEnable(enable);
}

void OwlKeyHook::setKeyHookEnable(bool enable)
{
    emit sigKeyHookEnable(enable);
}

void OwlKeyHook::__setMouseHookEnable(bool enable)
{
    if(enable && !__MouseHook){
        __MouseHook = (quint64)SetWindowsHookEx(WH_MOUSE_LL, MyMouseCallback, NULL, 0);
        if(!__MouseHook){
            qCritical() << "Thread " << QThread::currentThreadId() << " set WH_MOUSE_LL hook fail!";
        }else{
            qCritical() << "Thread " << QThread::currentThreadId() << " set WH_MOUSE_LL hook success";
        }
    }
    else if(!enable && __MouseHook)
    {
        qInfo() << ("Unhook WH_KEYBOARD_LL");
        UnhookWindowsHookEx((HHOOK)__MouseHook);
        __MouseHook = 0;
    }
}
void OwlKeyHook::__setKeyHookEnable(bool enable)
{
    if(enable && !__KeyboardHook){
        __KeyboardHook = (quint64)SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyBoardCallback, NULL, 0);
        if(!__KeyboardHook){
            qInfo() << "Thread " << QThread::currentThreadId() << " set WH_KEYBOARD_LL hook fail!";
        }else{
            qInfo() << "Thread " << QThread::currentThreadId() << " set WH_KEYBOARD_LL hook success";
        }
    }
    else if(!enable && __KeyboardHook)
    {
        qInfo() << "Unhook WH_KEYBOARD_LL";
        UnhookWindowsHookEx((HHOOK)__KeyboardHook);
        __KeyboardHook = 0;
    }
}
OwlKeyHook::~OwlKeyHook()
{
    qInfo() << "OwlKeyHook uninit";
    __dPtrSet.remove(this);
    if (__dPtrSet.isEmpty())
    {
        qInfo() << "Unhook WindowsHookEx";
        if(__MouseHook)UnhookWindowsHookEx((HHOOK)__MouseHook);
        if(__KeyboardHook)UnhookWindowsHookEx((HHOOK)__KeyboardHook);
    }
}

void OwlKeyHook::__EmitSigMouseDown(Qt::MouseButton Button, QPoint pos)
{
    emit SigMouseDown(Button, pos);
}

void OwlKeyHook::__EmitSigMouseUp(Qt::MouseButton Button, QPoint pos)
{
    emit SigMouseUp(Button, pos);
}

void OwlKeyHook::__EmitSigWheelRoll(int val, QPoint pos)
{
    emit SigWheelRoll(val, pos);
}

void OwlKeyHook::__EmitSigKeyDown(Qt::Key k)
{
    emit SigKeyDown(k);
}
void OwlKeyHook::__EmitSigMouseMove(QPoint pos)
{
    emit SigMouseMove(pos);
}
void OwlKeyHook::__EmitSigKeyUp(Qt::Key k)
{
    emit SigKeyUp(k);
}

LRESULT WINAPI MyMouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT *pMouseStruct = (MSLLHOOKSTRUCT *)lParam; 
    if (nCode == 0 && pMouseStruct
        && (OwlKeyHook::__TrackMouseMove || wParam != WM_MOUSEMOVE || gMouseDown))
    { // we have information in wParam/lParam ? If yes, let's check it:
        Qt::MouseButton Button;
        QPoint pos(pMouseStruct->pt.x, pMouseStruct->pt.y);
        switch (wParam)
        {
            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
            Button = Qt::LeftButton;
            break;

            case WM_RBUTTONUP:
            case WM_RBUTTONDOWN:
            Button = Qt::RightButton;
            break;

            case WM_MBUTTONUP:
            case WM_MBUTTONDOWN:
            Button = Qt::MiddleButton;
            break;
        default:
            break;
        }
        switch (wParam)
        {
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            {
                gMouseDown = false;
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigMouseUp(Button, pos);
                }
                break;
            }
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            {
                gMouseDown = true;
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigMouseDown(Button, pos);
                }
            }
            break;
            
            case WM_MOUSEWHEEL:
            {
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigWheelRoll((INT16)((pMouseStruct->mouseData & 0xffff0000) >> 16), pos);
                }
            }
            break;
            case WM_MOUSEMOVE:
            {
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigMouseMove(pos);
                }
            }
            break;
        default:
            break;
        }
    }
    return CallNextHookEx((HHOOK)OwlKeyHook::__MouseHook, nCode, wParam, lParam);
}

LRESULT WINAPI MyKeyBoardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;
    if (nCode == 0 && pKeyStruct)
    {
        Qt::Key k = (Qt::Key)QwlKeyTbl[pKeyStruct->vkCode];
        switch (wParam)
        {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
                //printf("pKeyStruct->vkCode:0x%x\n", pKeyStruct->vkCode);
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigKeyDown(k);
                }
                break;
            }
            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                for (auto ptr : OwlKeyHook::__dPtrSet)
                {
                    ptr->__EmitSigKeyUp(k);
                }
                break;
            }
        }
    }
    return CallNextHookEx((HHOOK)OwlKeyHook::__KeyboardHook, nCode, wParam, lParam);
}