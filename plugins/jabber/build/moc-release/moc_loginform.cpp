/****************************************************************************
** Meta object code from reading C++ file 'loginform.h'
**
** Created: Sun Feb 15 15:01:45 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/loginform.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loginform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_LoginForm[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      41,   30,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LoginForm[] = {
    "LoginForm\0\0btnRegisterClick()\0jid,result\0"
    "registrationResult(QString,RegistrationResult)\0"
};

const QMetaObject LoginForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LoginForm,
      qt_meta_data_LoginForm, 0 }
};

const QMetaObject *LoginForm::metaObject() const
{
    return &staticMetaObject;
}

void *LoginForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoginForm))
	return static_cast<void*>(const_cast< LoginForm*>(this));
    if (!strcmp(_clname, "ConnectionListener"))
	return static_cast< ConnectionListener*>(const_cast< LoginForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int LoginForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: btnRegisterClick(); break;
        case 1: registrationResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< RegistrationResult(*)>(_a[2]))); break;
        }
        _id -= 2;
    }
    return _id;
}
