/****************************************************************************
** Meta object code from reading C++ file 'requestauthdialog.h'
**
** Created: Sat Feb 14 22:20:56 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../requestauthdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'requestauthdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_requestAuthDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_requestAuthDialog[] = {
    "requestAuthDialog\0"
};

const QMetaObject requestAuthDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_requestAuthDialog,
      qt_meta_data_requestAuthDialog, 0 }
};

const QMetaObject *requestAuthDialog::metaObject() const
{
    return &staticMetaObject;
}

void *requestAuthDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_requestAuthDialog))
	return static_cast<void*>(const_cast< requestAuthDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int requestAuthDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
