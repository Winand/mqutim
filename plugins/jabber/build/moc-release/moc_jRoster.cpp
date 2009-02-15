/****************************************************************************
** Meta object code from reading C++ file 'jRoster.h'
**
** Created: Sun Feb 15 15:01:28 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jRoster.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jRoster.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jRoster[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      24,    9,    8,    8, 0x05,
      73,   68,    8,    8, 0x05,
     131,  114,    8,    8, 0x05,
     194,  179,    8,    8, 0x05,
     244,  231,    8,    8, 0x05,
     284,  281,    8,    8, 0x05,
     323,  281,    8,    8, 0x05,
     370,  368,    8,    8, 0x05,
     430,  414,    8,    8, 0x05,
     491,  481,    8,    8, 0x05,
     543,  533,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     584,    8,    8,    8, 0x0a,
     601,    8,    8,    8, 0x0a,
     618,    8,    8,    8, 0x0a,
     633,    8,    8,    8, 0x0a,
     660,    8,    8,    8, 0x0a,
     686,    8,    8,    8, 0x0a,
     715,    8,    8,    8, 0x0a,
     731,    8,    8,    8, 0x0a,
     746,    8,    8,    8, 0x0a,
     765,    8,    8,    8, 0x0a,
     786,    8,    8,    8, 0x0a,
     811,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_jRoster[] = {
    "jRoster\0\0item,item_name\0"
    "addItemToContactList(TreeModelItem,QString)\0"
    "Item\0removeItemFromContactList(TreeModelItem)\0"
    "item,status,mass\0"
    "setContactItemStatus(TreeModelItem,QString,int)\0"
    "item,invisible\0setItemInvisible(TreeModelItem,bool)\0"
    "contact,name\0clientVersion(TreeModelItem,QString)\0"
    ",,\0setItemIcon(TreeModelItem,QString,int)\0"
    "setItemCustomText(TreeModelItem,QString,int)\0"
    ",\0s_customNotification(TreeModelItem,QString)\0"
    "OldItem,NewItem\0"
    "moveItemInContactList(TreeModelItem,TreeModelItem)\0"
    "Item,name\0setContactItemName(TreeModelItem,QString)\0"
    "item,text\0addServiceMessage(TreeModelItem,QString)\0"
    "onRenameAction()\0onDeleteAction()\0"
    "onMoveAction()\0onSendSubscriptionAction()\0"
    "onAskSubscriptionAction()\0"
    "onRemoveSubscriptionAction()\0"
    "onSendMessage()\0onAddContact()\0"
    "onRegisterAction()\0onUnregisterAction()\0"
    "onLogInTransportAction()\0"
    "onLogOutTransportAction()\0"
};

const QMetaObject jRoster::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jRoster,
      qt_meta_data_jRoster, 0 }
};

const QMetaObject *jRoster::metaObject() const
{
    return &staticMetaObject;
}

void *jRoster::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jRoster))
	return static_cast<void*>(const_cast< jRoster*>(this));
    return QObject::qt_metacast(_clname);
}

int jRoster::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addItemToContactList((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: removeItemFromContactList((*reinterpret_cast< TreeModelItem(*)>(_a[1]))); break;
        case 2: setContactItemStatus((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: setItemInvisible((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: clientVersion((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: setItemIcon((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: setItemCustomText((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: s_customNotification((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: moveItemInContactList((*reinterpret_cast< TreeModelItem(*)>(_a[1])),(*reinterpret_cast< TreeModelItem(*)>(_a[2]))); break;
        case 9: setContactItemName((*reinterpret_cast< TreeModelItem(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 10: addServiceMessage((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: onRenameAction(); break;
        case 12: onDeleteAction(); break;
        case 13: onMoveAction(); break;
        case 14: onSendSubscriptionAction(); break;
        case 15: onAskSubscriptionAction(); break;
        case 16: onRemoveSubscriptionAction(); break;
        case 17: onSendMessage(); break;
        case 18: onAddContact(); break;
        case 19: onRegisterAction(); break;
        case 20: onUnregisterAction(); break;
        case 21: onLogInTransportAction(); break;
        case 22: onLogOutTransportAction(); break;
        }
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void jRoster::addItemToContactList(const TreeModelItem & _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jRoster::removeItemFromContactList(TreeModelItem _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jRoster::setContactItemStatus(const TreeModelItem & _t1, QString _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void jRoster::setItemInvisible(const TreeModelItem & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void jRoster::clientVersion(const TreeModelItem & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void jRoster::setItemIcon(const TreeModelItem & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void jRoster::setItemCustomText(const TreeModelItem & _t1, const QString & _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void jRoster::s_customNotification(const TreeModelItem & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void jRoster::moveItemInContactList(TreeModelItem _t1, TreeModelItem _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void jRoster::setContactItemName(TreeModelItem _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void jRoster::addServiceMessage(const TreeModelItem & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
