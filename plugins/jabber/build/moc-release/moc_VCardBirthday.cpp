/****************************************************************************
** Meta object code from reading C++ file 'VCardBirthday.h'
**
** Created: Sun Feb 15 15:01:46 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/VCardBirthday.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VCardBirthday.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_VCardBirthday[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      27,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   14,   14,   14, 0x08,
      53,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_VCardBirthday[] = {
    "VCardBirthday\0\0mouseOver()\0mouseOut()\0"
    "setLabelEdit()\0setLabelRead()\0"
};

const QMetaObject VCardBirthday::staticMetaObject = {
    { &VCardEntry::staticMetaObject, qt_meta_stringdata_VCardBirthday,
      qt_meta_data_VCardBirthday, 0 }
};

const QMetaObject *VCardBirthday::metaObject() const
{
    return &staticMetaObject;
}

void *VCardBirthday::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VCardBirthday))
	return static_cast<void*>(const_cast< VCardBirthday*>(this));
    return VCardEntry::qt_metacast(_clname);
}

int VCardBirthday::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = VCardEntry::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: mouseOver(); break;
        case 1: mouseOut(); break;
        case 2: setLabelEdit(); break;
        case 3: setLabelRead(); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void VCardBirthday::mouseOver()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void VCardBirthday::mouseOut()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
