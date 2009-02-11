/****************************************************************************
** Meta object code from reading C++ file 'filetransferwindow.h'
**
** Created: Mon Feb 9 20:47:05 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../filetransferwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filetransferwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_fileTransferWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   20,   19,   19, 0x05,
      60,   57,   19,   19, 0x05,
     117,  113,   19,   19, 0x05,
     176,   20,   19,   19, 0x05,
     214,   57,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     267,   19,   19,   19, 0x08,
     293,   19,   19,   19, 0x08,
     317,   19,   19,   19, 0x08,
     335,   19,   19,   19, 0x08,
     352,   19,   19,   19, 0x08,
     367,   19,   19,   19, 0x08,
     390,   19,   19,   19, 0x08,
     411,   19,   19,   19, 0x08,
     426,   19,   19,   19, 0x08,
     446,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_fileTransferWindow[] = {
    "fileTransferWindow\0\0,\0"
    "cancelSending(QByteArray&,QString)\0"
    ",,\0sendingToPeerRequest(QByteArray,QString,QStringList)\0"
    ",,,\0getRedirectToProxyData(QByteArray,QString,quint16,quint32)\0"
    "sendAcceptMessage(QByteArray,QString)\0"
    "sendRedirectToMineServer(QByteArray,QString,quint16)\0"
    "on_cancelButton_clicked()\0"
    "on_openButton_clicked()\0socketConnected()\0"
    "readFromSocket()\0sendFileData()\0"
    "checkLocalConnection()\0sendTransferPacket()\0"
    "bytesWritten()\0slotNewConnection()\0"
    "updateProgress()\0"
};

const QMetaObject fileTransferWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_fileTransferWindow,
      qt_meta_data_fileTransferWindow, 0 }
};

const QMetaObject *fileTransferWindow::metaObject() const
{
    return &staticMetaObject;
}

void *fileTransferWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fileTransferWindow))
	return static_cast<void*>(const_cast< fileTransferWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int fileTransferWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cancelSending((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: sendingToPeerRequest((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 2: getRedirectToProxyData((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4]))); break;
        case 3: sendAcceptMessage((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: sendRedirectToMineServer((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< quint16(*)>(_a[3]))); break;
        case 5: on_cancelButton_clicked(); break;
        case 6: on_openButton_clicked(); break;
        case 7: socketConnected(); break;
        case 8: readFromSocket(); break;
        case 9: sendFileData(); break;
        case 10: checkLocalConnection(); break;
        case 11: sendTransferPacket(); break;
        case 12: bytesWritten(); break;
        case 13: slotNewConnection(); break;
        case 14: updateProgress(); break;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void fileTransferWindow::cancelSending(QByteArray & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void fileTransferWindow::sendingToPeerRequest(const QByteArray & _t1, const QString & _t2, const QStringList & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void fileTransferWindow::getRedirectToProxyData(const QByteArray & _t1, const QString & _t2, quint16 _t3, quint32 _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void fileTransferWindow::sendAcceptMessage(const QByteArray & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void fileTransferWindow::sendRedirectToMineServer(const QByteArray & _t1, const QString & _t2, quint16 _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
