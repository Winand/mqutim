/****************************************************************************
** Meta object code from reading C++ file 'customstatusdialog.h'
**
** Created: Mon Feb 9 20:46:58 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../customstatusdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'customstatusdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_customStatusDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      37,   20,   19,   19, 0x08,
     103,   19,   19,   19, 0x08,
     129,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_customStatusDialog[] = {
    "customStatusDialog\0\0current,previous\0"
    "on_iconList_currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "on_chooseButton_clicked()\0"
    "on_awayEdit_textChanged()\0"
};

const QMetaObject customStatusDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_customStatusDialog,
      qt_meta_data_customStatusDialog, 0 }
};

const QMetaObject *customStatusDialog::metaObject() const
{
    return &staticMetaObject;
}

void *customStatusDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_customStatusDialog))
	return static_cast<void*>(const_cast< customStatusDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int customStatusDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_iconList_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: on_chooseButton_clicked(); break;
        case 2: on_awayEdit_textChanged(); break;
        }
        _id -= 3;
    }
    return _id;
}