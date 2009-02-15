/****************************************************************************
** Meta object code from reading C++ file 'multiplesending.h'
**
** Created: Sat Feb 14 22:20:42 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../multiplesending.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiplesending.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_multipleSending[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      55,   53,   16,   16, 0x08,
     110,   16,   16,   16, 0x08,
     134,   16,   16,   16, 0x08,
     158,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_multipleSending[] = {
    "multipleSending\0\0sendMessageToContact(messageFormat)\0"
    ",\0on_contactListWidget_itemChanged(QTreeWidgetItem*,int)\0"
    "on_sendButton_clicked()\0on_stopButton_clicked()\0"
    "sendMessage()\0"
};

const QMetaObject multipleSending::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_multipleSending,
      qt_meta_data_multipleSending, 0 }
};

const QMetaObject *multipleSending::metaObject() const
{
    return &staticMetaObject;
}

void *multipleSending::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_multipleSending))
	return static_cast<void*>(const_cast< multipleSending*>(this));
    return QWidget::qt_metacast(_clname);
}

int multipleSending::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendMessageToContact((*reinterpret_cast< const messageFormat(*)>(_a[1]))); break;
        case 1: on_contactListWidget_itemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: on_sendButton_clicked(); break;
        case 3: on_stopButton_clicked(); break;
        case 4: sendMessage(); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void multipleSending::sendMessageToContact(const messageFormat & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
