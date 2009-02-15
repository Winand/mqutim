/****************************************************************************
** Meta object code from reading C++ file 'customstatusdialog.h'
**
** Created: Sun Feb 15 15:01:03 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/customstatusdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customstatusdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CustomStatusDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      37,   20,   19,   19, 0x08,
     103,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CustomStatusDialog[] = {
    "CustomStatusDialog\0\0current,previous\0"
    "on_iconList_currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "on_chooseButton_clicked()\0"
};

const QMetaObject CustomStatusDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CustomStatusDialog,
      qt_meta_data_CustomStatusDialog, 0 }
};

const QMetaObject *CustomStatusDialog::metaObject() const
{
    return &staticMetaObject;
}

void *CustomStatusDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CustomStatusDialog))
	return static_cast<void*>(const_cast< CustomStatusDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CustomStatusDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_iconList_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: on_chooseButton_clicked(); break;
        }
        _id -= 2;
    }
    return _id;
}
