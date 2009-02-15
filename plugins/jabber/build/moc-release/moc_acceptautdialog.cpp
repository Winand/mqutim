/****************************************************************************
** Meta object code from reading C++ file 'acceptautdialog.h'
**
** Created: Sun Feb 15 15:00:57 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/acceptautdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'acceptautdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_AcceptAuthDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      47,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AcceptAuthDialog[] = {
    "AcceptAuthDialog\0\0on_authorizeButton_clicked()\0"
    "on_denyButton_clicked()\0"
};

const QMetaObject AcceptAuthDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AcceptAuthDialog,
      qt_meta_data_AcceptAuthDialog, 0 }
};

const QMetaObject *AcceptAuthDialog::metaObject() const
{
    return &staticMetaObject;
}

void *AcceptAuthDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AcceptAuthDialog))
	return static_cast<void*>(const_cast< AcceptAuthDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int AcceptAuthDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_authorizeButton_clicked(); break;
        case 1: on_denyButton_clicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
