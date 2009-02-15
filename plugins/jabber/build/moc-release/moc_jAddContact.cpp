/****************************************************************************
** Meta object code from reading C++ file 'jAddContact.h'
**
** Created: Sun Feb 15 15:01:10 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jAddContact.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jAddContact.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jAddContact[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,
      58,   12,   12,   12, 0x05,
      83,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      96,   12,   12,   12, 0x08,
     119,   12,   12,   12, 0x08,
     145,   12,   12,   12, 0x08,
     169,   12,   12,   12, 0x08,
     198,  193,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jAddContact[] = {
    "jAddContact\0\0,,,\0"
    "addContact(QString,QString,QString,bool)\0"
    "showInformation(QString)\0searchUser()\0"
    "on_buttonAdd_clicked()\0on_buttonCancel_clicked()\0"
    "on_buttonInfo_clicked()\0on_buttonFind_clicked()\0"
    "text\0on_groupText_changed(QString)\0"
};

const QMetaObject jAddContact::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_jAddContact,
      qt_meta_data_jAddContact, 0 }
};

const QMetaObject *jAddContact::metaObject() const
{
    return &staticMetaObject;
}

void *jAddContact::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jAddContact))
	return static_cast<void*>(const_cast< jAddContact*>(this));
    return QWidget::qt_metacast(_clname);
}

int jAddContact::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addContact((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: showInformation((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: searchUser(); break;
        case 3: on_buttonAdd_clicked(); break;
        case 4: on_buttonCancel_clicked(); break;
        case 5: on_buttonInfo_clicked(); break;
        case 6: on_buttonFind_clicked(); break;
        case 7: on_groupText_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void jAddContact::addContact(const QString & _t1, const QString & _t2, const QString & _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jAddContact::showInformation(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jAddContact::searchUser()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
