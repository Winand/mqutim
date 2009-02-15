/****************************************************************************
** Meta object code from reading C++ file 'jJoinChat.h'
**
** Created: Sun Feb 15 15:01:20 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jJoinChat.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jJoinChat.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jJoinChat[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      77,   60,   10,   10, 0x0a,
     127,   10,   10,   10, 0x0a,
     160,   10,   10,   10, 0x0a,
     196,   10,   10,   10, 0x0a,
     222,   10,   10,   10, 0x08,
     246,   10,   10,   10, 0x08,
     275,  270,   10,   10, 0x08,
     317,  306,   10,   10, 0x08,
     344,   10,   10,   10, 0x08,
     361,  359,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jJoinChat[] = {
    "jJoinChat\0\0,,\0"
    "createConferenceRoom(QString,QString,QString)\0"
    "current,previous\0"
    "showConference(QListWidgetItem*,QListWidgetItem*)\0"
    "on_addConferenceButton_clicked()\0"
    "on_removeConferenceButton_clicked()\0"
    "on_searchButton_clicked()\0"
    "on_joinButton_clicked()\0on_saveButton_clicked()\0"
    "item\0joinBookmark(QListWidgetItem*)\0"
    "conference\0setConferenceRoom(QString)\0"
    "finishSearch()\0i\0changeRecent(int)\0"
};

const QMetaObject jJoinChat::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_jJoinChat,
      qt_meta_data_jJoinChat, 0 }
};

const QMetaObject *jJoinChat::metaObject() const
{
    return &staticMetaObject;
}

void *jJoinChat::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jJoinChat))
	return static_cast<void*>(const_cast< jJoinChat*>(this));
    return QWidget::qt_metacast(_clname);
}

int jJoinChat::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createConferenceRoom((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: showConference((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QListWidgetItem*(*)>(_a[2]))); break;
        case 2: on_addConferenceButton_clicked(); break;
        case 3: on_removeConferenceButton_clicked(); break;
        case 4: on_searchButton_clicked(); break;
        case 5: on_joinButton_clicked(); break;
        case 6: on_saveButton_clicked(); break;
        case 7: joinBookmark((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 8: setConferenceRoom((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: finishSearch(); break;
        case 10: changeRecent((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void jJoinChat::createConferenceRoom(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
