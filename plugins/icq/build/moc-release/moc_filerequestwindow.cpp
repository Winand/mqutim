/****************************************************************************
** Meta object code from reading C++ file 'filerequestwindow.h'
**
** Created: Sat Feb 14 22:20:31 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../filerequestwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filerequestwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_fileRequestWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      21,   19,   18,   18, 0x05,
      62,   56,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     127,   18,   18,   18, 0x08,
     154,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_fileRequestWindow[] = {
    "fileRequestWindow\0\0,\0"
    "cancelSending(QByteArray&,QString)\0"
    ",,,,,\0"
    "fileAccepted(QByteArray,QString,QString,quint32,quint16,quint32)\0"
    "on_declineButton_clicked()\0"
    "on_acceptButton_clicked()\0"
};

const QMetaObject fileRequestWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_fileRequestWindow,
      qt_meta_data_fileRequestWindow, 0 }
};

const QMetaObject *fileRequestWindow::metaObject() const
{
    return &staticMetaObject;
}

void *fileRequestWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fileRequestWindow))
	return static_cast<void*>(const_cast< fileRequestWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int fileRequestWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: cancelSending((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: fileAccepted((*reinterpret_cast< const QByteArray(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< quint32(*)>(_a[4])),(*reinterpret_cast< quint16(*)>(_a[5])),(*reinterpret_cast< quint32(*)>(_a[6]))); break;
        case 2: on_declineButton_clicked(); break;
        case 3: on_acceptButton_clicked(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void fileRequestWindow::cancelSending(QByteArray & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void fileRequestWindow::fileAccepted(const QByteArray & _t1, const QString & _t2, const QString & _t3, quint32 _t4, quint16 _t5, quint32 _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
