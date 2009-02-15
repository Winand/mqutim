/****************************************************************************
** Meta object code from reading C++ file 'jSearchConference.h'
**
** Created: Sun Feb 15 15:01:31 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jSearchConference.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jSearchConference.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jSearchConference[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      48,   19,   18,   18, 0x05,
     103,   92,   18,   18, 0x05,
     129,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     144,   18,   18,   18, 0x08,
     170,   18,   18,   18, 0x08,
     195,   18,   18,   18, 0x08,
     242,  221,   18,   18, 0x08,
     321,  298,   18,   18, 0x08,
     372,  370,   18,   18, 0x08,
     409,   18,   18,   18, 0x08,
     457,  442,   18,   18, 0x08,
     501,  442,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jSearchConference[] = {
    "jSearchConference\0\0conference,nickname,password\0"
    "joinConferenceRoom(QString,QString,QString)\0"
    "conference\0selectConference(QString)\0"
    "finishSearch()\0on_searchButton_clicked()\0"
    "on_closeButton_clicked()\0"
    "on_selectButton_clicked()\0"
    "parent_item,confList\0"
    "setConferencesList(QTreeWidgetItem*,QList<QStringList>)\0"
    "parent_item,entityList\0"
    "setConferencesList(QTreeWidgetItem*,QStringList)\0"
    ",\0joinConference(QTreeWidgetItem*,int)\0"
    "joinConference(QTreeWidgetItem*)\0"
    "server,node,dh\0"
    "getDiscoItem(QString,QString,DiscoHandler*)\0"
    "getDiscoInfo(QString,QString,DiscoHandler*)\0"
};

const QMetaObject jSearchConference::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_jSearchConference,
      qt_meta_data_jSearchConference, 0 }
};

const QMetaObject *jSearchConference::metaObject() const
{
    return &staticMetaObject;
}

void *jSearchConference::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jSearchConference))
	return static_cast<void*>(const_cast< jSearchConference*>(this));
    return QWidget::qt_metacast(_clname);
}

int jSearchConference::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: joinConferenceRoom((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: selectConference((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: finishSearch(); break;
        case 3: on_searchButton_clicked(); break;
        case 4: on_closeButton_clicked(); break;
        case 5: on_selectButton_clicked(); break;
        case 6: setConferencesList((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QList<QStringList>(*)>(_a[2]))); break;
        case 7: setConferencesList((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 8: joinConference((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: joinConference((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 10: getDiscoItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        case 11: getDiscoInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< DiscoHandler*(*)>(_a[3]))); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void jSearchConference::joinConferenceRoom(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jSearchConference::selectConference(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jSearchConference::finishSearch()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
