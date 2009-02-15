/****************************************************************************
** Meta object code from reading C++ file 'activitydialog.h'
**
** Created: Sun Feb 15 15:01:00 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/activitydialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'activitydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ActivityDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      33,   16,   15,   15, 0x08,
     128,  102,   15,   15, 0x08,
     203,   16,   15,   15, 0x08,
     273,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ActivityDialog[] = {
    "ActivityDialog\0\0current,previous\0"
    "on_generalList_currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "current,previous,specific\0"
    "onGeneralListCurrentItemChanged(QListWidgetItem*,QListWidgetItem*,QStr"
    "ing)\0"
    "on_specificList_currentItemChanged(QListWidgetItem*,QListWidgetItem*)\0"
    "on_chooseButton_clicked()\0"
};

const QMetaObject ActivityDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ActivityDialog,
      qt_meta_data_ActivityDialog, 0 }
};

const QMetaObject *ActivityDialog::metaObject() const
{
    return &staticMetaObject;
}

void *ActivityDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ActivityDialog))
	return static_cast<void*>(const_cast< ActivityDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ActivityDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_generalList_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 1: onGeneralListCurrentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: on_specificList_currentItemChanged((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 3: on_chooseButton_clicked(); break;
        }
        _id -= 4;
    }
    return _id;
}
