/****************************************************************************
** Meta object code from reading C++ file 'jTransport.h'
**
** Created: Sun Feb 15 15:01:42 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jTransport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jTransport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jTransport[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      61,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jTransport[] = {
    "jTransport\0\0,\0"
    "registrationResult(QString,RegistrationResult)\0"
    "registerClicked()\0"
};

const QMetaObject jTransport::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_jTransport,
      qt_meta_data_jTransport, 0 }
};

const QMetaObject *jTransport::metaObject() const
{
    return &staticMetaObject;
}

void *jTransport::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jTransport))
	return static_cast<void*>(const_cast< jTransport*>(this));
    if (!strcmp(_clname, "RegistrationHandler"))
	return static_cast< RegistrationHandler*>(const_cast< jTransport*>(this));
    return QWidget::qt_metacast(_clname);
}

int jTransport::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: registrationResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RegistrationResult(*)>(_a[2]))); break;
        case 1: registerClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void jTransport::registrationResult(QString _t1, RegistrationResult _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
