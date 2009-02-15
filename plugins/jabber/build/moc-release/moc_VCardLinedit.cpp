/****************************************************************************
** Meta object code from reading C++ file 'VCardLinedit.h'
**
** Created: Sun Feb 15 15:01:49 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/VCardLinedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VCardLinedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_VCardLinedit[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_VCardLinedit[] = {
    "VCardLinedit\0\0readMode()\0"
};

const QMetaObject VCardLinedit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_VCardLinedit,
      qt_meta_data_VCardLinedit, 0 }
};

const QMetaObject *VCardLinedit::metaObject() const
{
    return &staticMetaObject;
}

void *VCardLinedit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VCardLinedit))
	return static_cast<void*>(const_cast< VCardLinedit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int VCardLinedit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readMode(); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void VCardLinedit::readMode()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
