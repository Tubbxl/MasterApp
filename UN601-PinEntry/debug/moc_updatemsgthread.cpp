/****************************************************************************
** Meta object code from reading C++ file 'updatemsgthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PinEntry/updatemsgthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'updatemsgthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UpdateMsgThread_t {
    QByteArrayData data[10];
    char stringdata[117];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpdateMsgThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpdateMsgThread_t qt_meta_stringdata_UpdateMsgThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "UpdateMsgThread"
QT_MOC_LITERAL(1, 16, 19), // "refreshPromptSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "itemid"
QT_MOC_LITERAL(4, 44, 9), // "strPrompt"
QT_MOC_LITERAL(5, 54, 14), // "itemCtrlSignal"
QT_MOC_LITERAL(6, 69, 4), // "mode"
QT_MOC_LITERAL(7, 74, 5), // "count"
QT_MOC_LITERAL(8, 80, 18), // "showPinEntrySignal"
QT_MOC_LITERAL(9, 99, 17) // "closePromptSignal"

    },
    "UpdateMsgThread\0refreshPromptSignal\0"
    "\0itemid\0strPrompt\0itemCtrlSignal\0mode\0"
    "count\0showPinEntrySignal\0closePromptSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpdateMsgThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       5,    2,   39,    2, 0x06 /* Public */,
       8,    1,   44,    2, 0x06 /* Public */,
       9,    0,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,

       0        // eod
};

void UpdateMsgThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UpdateMsgThread *_t = static_cast<UpdateMsgThread *>(_o);
        switch (_id) {
        case 0: _t->refreshPromptSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->itemCtrlSignal((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->showPinEntrySignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->closePromptSignal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (UpdateMsgThread::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateMsgThread::refreshPromptSignal)) {
                *result = 0;
            }
        }
        {
            typedef void (UpdateMsgThread::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateMsgThread::itemCtrlSignal)) {
                *result = 1;
            }
        }
        {
            typedef void (UpdateMsgThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateMsgThread::showPinEntrySignal)) {
                *result = 2;
            }
        }
        {
            typedef void (UpdateMsgThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&UpdateMsgThread::closePromptSignal)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject UpdateMsgThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_UpdateMsgThread.data,
      qt_meta_data_UpdateMsgThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UpdateMsgThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpdateMsgThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateMsgThread.stringdata))
        return static_cast<void*>(const_cast< UpdateMsgThread*>(this));
    return QThread::qt_metacast(_clname);
}

int UpdateMsgThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void UpdateMsgThread::refreshPromptSignal(int _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpdateMsgThread::itemCtrlSignal(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UpdateMsgThread::showPinEntrySignal(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UpdateMsgThread::closePromptSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
