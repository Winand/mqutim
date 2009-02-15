/****************************************************************************
** Meta object code from reading C++ file 'passworddialog.h'
**
** Created: Sat Feb 14 22:20:51 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../passworddialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'passworddialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_passwordDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      39,   34,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_passwordDialog[] = {
    "passwordDialog\0\0okEnable(QString)\0"
    "flag\0savePass(int)\0"
};

const QMetaObject passwordDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_passwordDialog,
      qt_meta_data_passwordDialog, 0 }
};

const QMetaObject *passwordDialog::metaObject() const
{
    return &staticMetaObject;
}

void *passwordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_passwordDialog))
	return static_cast<void*>(const_cast< passwordDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int passwordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: okEnable((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: savePass((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
