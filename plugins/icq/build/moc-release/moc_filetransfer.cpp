/****************************************************************************
** Meta object code from reading C++ file 'filetransfer.h'
**
** Created: Mon Feb 9 20:47:03 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../filetransfer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filetransfer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FileTransfer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   14,   13,   13, 0x05,
      63,   13,   13,   13, 0x05,
      94,   13,   13,   13, 0x05,
     126,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
     160,  156,   13,   13, 0x08,
     219,   13,   13,   13, 0x08,
     243,   13,   13,   13, 0x08,
     268,  266,   13,   13, 0x08,
     303,   14,   13,   13, 0x08,
     356,  266,   13,   13, 0x08,
     400,  394,   13,   13, 0x08,
     465,   14,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FileTransfer[] = {
    "FileTransfer\0\0,,\0"
    "sendFile(QByteArray&,QByteArray&,QByteArray&)\0"
    "emitCancelSending(QByteArray&)\0"
    "sendRedirectToProxy(QByteArray)\0"
    "emitAcceptSending(QByteArray)\0,,,\0"
    "getRedirectToProxyData(QByteArray,QString,quint16,quint32)\0"
    "deleteFileWin(QObject*)\0deleteReqWin(QObject*)\0"
    ",\0cancelSending(QByteArray&,QString)\0"
    "sendingToPeerRequest(QByteArray,QString,QStringList)\0"
    "sendAcceptMessage(QByteArray,QString)\0"
    ",,,,,\0"
    "fileAccepted(QByteArray,QString,QString,quint32,quint16,quint32)\0"
    "sendRedirectToMineServer(QByteArray,QString,quint16)\0"
};

const QMetaObject FileTransfer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileTransfer,
      qt_meta_data_FileTransfer, 0 }
};

const QMetaObject *FileTransfer::metaObject() const
{
    return &staticMetaObject;
}

void *FileTransfer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileTransfer))
	return static_cast<void*>(const_cast< FileTransfer*>(this));
    return QObject::qt_metacast(_clname);
}

int FileTransfer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendFile((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        case 1: emitCancelSending((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: sendRedirectToProxy((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: emitAcceptSending((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 4: getRedirectToProxyData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4]))); break;
        case 5: deleteFileWin((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 6: deleteReqWin((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 7: cancelSending((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: sendingToPeerRequest((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 9: sendAcceptMessage((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: fileAccepted((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4])),(*reinterpret_cast< quint16(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6]))); break;
        case 11: sendRedirectToMineServer((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void FileTransfer::sendFile(QByteArray & _t1, QByteArray & _t2, QByteArray & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FileTransfer::emitCancelSending(QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FileTransfer::sendRedirectToProxy(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileTransfer::emitAcceptSending(const QByteArray & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
