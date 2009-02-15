/****************************************************************************
** Meta object code from reading C++ file 'jServiceDiscovery.h'
**
** Created: Sun Feb 15 15:01:36 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jServiceDiscovery.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jServiceDiscovery.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jServiceDiscovery[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      34,   19,   18,   18, 0x05,
      78,   19,   18,   18, 0x05,
     145,  122,   18,   18, 0x05,
     209,  188,   18,   18, 0x05,
     282,  259,   18,   18, 0x05,
     324,  259,   18,   18, 0x05,
     377,  373,   18,   18, 0x05,
     399,  259,   18,   18, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_jServiceDiscovery[] = {
    "jServiceDiscovery\0\0server,node,dh\0"
    "getDiscoInfo(QString,QString,DiscoHandler*)\0"
    "getDiscoItem(QString,QString,DiscoHandler*)\0"
    "parent_item,entityList\0"
    "finishSearch(QTreeWidgetItem*,QStringList)\0"
    "parent_item,confList\0"
    "finishSearch(QTreeWidgetItem*,QList<QStringList>)\0"
    "parent_item,disco_item\0"
    "finishSearch(QTreeWidgetItem*,jDiscoItem)\0"
    "finishSearch(QTreeWidgetItem*,QList<jDiscoItem>)\0"
    "jid\0finishSearch(QString)\0"
    "finishSelfSearch(QTreeWidgetItem*,jDiscoItem)\0"
};

const QMetaObject jServiceDiscovery::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jServiceDiscovery,
      qt_meta_data_jServiceDiscovery, 0 }
};

const QMetaObject *jServiceDiscovery::metaObject() const
{
    return &staticMetaObject;
}

void *jServiceDiscovery::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jServiceDiscovery))
	return static_cast<void*>(const_cast< jServiceDiscovery*>(this));
    if (!strcmp(_clname, "DiscoHandler"))
	return static_cast< DiscoHandler*>(const_cast< jServiceDiscovery*>(this));
    return QObject::qt_metacast(_clname);
}

int jServiceDiscovery::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: getDiscoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        case 1: getDiscoItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        case 2: finishSearch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 3: finishSearch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QList<QStringList>(*)>(_a[2]))); break;
        case 4: finishSearch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const jDiscoItem(*)>(_a[2]))); break;
        case 5: finishSearch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QList<jDiscoItem>(*)>(_a[2]))); break;
        case 6: finishSearch((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: finishSelfSearch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const jDiscoItem(*)>(_a[2]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void jServiceDiscovery::getDiscoInfo(const QString & _t1, const QString & _t2, DiscoHandler * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jServiceDiscovery::getDiscoItem(const QString & _t1, const QString & _t2, DiscoHandler * _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jServiceDiscovery::finishSearch(QTreeWidgetItem * _t1, const QStringList & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void jServiceDiscovery::finishSearch(QTreeWidgetItem * _t1, const QList<QStringList> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void jServiceDiscovery::finishSearch(QTreeWidgetItem * _t1, const jDiscoItem & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void jServiceDiscovery::finishSearch(QTreeWidgetItem * _t1, const QList<jDiscoItem> & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void jServiceDiscovery::finishSearch(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void jServiceDiscovery::finishSelfSearch(QTreeWidgetItem * _t1, const jDiscoItem & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
