/****************************************************************************
** Meta object code from reading C++ file 'VCardRole.h'
**
** Created: Sun Feb 15 15:01:52 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/VCardRole.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VCardRole.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_VCardRole[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VCardRole[] = {
    "VCardRole\0\0changeStatus()\0"
};

const QMetaObject VCardRole::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_VCardRole,
      qt_meta_data_VCardRole, 0 }
};

const QMetaObject *VCardRole::metaObject() const
{
    return &staticMetaObject;
}

void *VCardRole::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VCardRole))
	return static_cast<void*>(const_cast< VCardRole*>(this));
    return QLabel::qt_metacast(_clname);
}

int VCardRole::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeStatus(); break;
        }
        _id -= 1;
    }
    return _id;
}
