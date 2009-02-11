/****************************************************************************
** Meta object code from reading C++ file 'userinformation.h'
**
** Created: Mon Feb 9 20:47:26 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../userinformation.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userinformation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_userInformation[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      44,   42,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      72,   16,   16,   16, 0x08,
      96,   16,   16,   16, 0x08,
     123,   16,   16,   16, 0x08,
     146,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_userInformation[] = {
    "userInformation\0\0requestUserInfo(QString)\0"
    ",\0saveOwnerInfo(bool,QString)\0"
    "on_saveButton_clicked()\0"
    "on_requestButton_clicked()\0"
    "on_addButton_clicked()\0on_removeButton_clicked()\0"
};

const QMetaObject userInformation::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_userInformation,
      qt_meta_data_userInformation, 0 }
};

const QMetaObject *userInformation::metaObject() const
{
    return &staticMetaObject;
}

void *userInformation::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_userInformation))
	return static_cast<void*>(const_cast< userInformation*>(this));
    return QWidget::qt_metacast(_clname);
}

int userInformation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: requestUserInfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: saveOwnerInfo((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: on_saveButton_clicked(); break;
        case 3: on_requestButton_clicked(); break;
        case 4: on_addButton_clicked(); break;
        case 5: on_removeButton_clicked(); break;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void userInformation::requestUserInfo(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void userInformation::saveOwnerInfo(bool _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
