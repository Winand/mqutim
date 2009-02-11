/****************************************************************************
** Meta object code from reading C++ file 'searchuser.h'
**
** Created: Mon Feb 9 20:47:23 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../searchuser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'searchuser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_searchUser[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      34,   32,   11,   11, 0x05,
      75,   71,   11,   11, 0x05,
     123,   11,   11,   11, 0x05,
     150,  147,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     193,   11,   11,   11, 0x08,
     221,   11,   11,   11, 0x08,
     246,   11,   11,   11, 0x08,
     272,   32,   11,   11, 0x08,
     326,   11,   11,   11, 0x08,
     381,   11,   11,   11, 0x08,
     406,   11,   11,   11, 0x08,
     435,   11,   11,   11, 0x08,
     468,   11,   11,   11, 0x08,
     497,   32,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_searchUser[] = {
    "searchUser\0\0findAskedUsers(int)\0,\0"
    "openChatWithFounded(QString,QString)\0"
    ",,,\0openInfoWindow(QString,QString,QString,QString)\0"
    "checkStatusFor(QString)\0,,\0"
    "addUserToContactList(QString,QString,bool)\0"
    "on_moreButton_toggled(bool)\0"
    "on_clearButton_clicked()\0"
    "on_searchButton_clicked()\0"
    "on_resultTreeWidget_itemClicked(QTreeWidgetItem*,int)\0"
    "on_resultTreeWidget_customContextMenuRequested(QPoint)\0"
    "addUserActionActivated()\0"
    "checkStatusActionActivated()\0"
    "userInformationActionActivated()\0"
    "sendMessageActionActivated()\0"
    "on_resultTreeWidget_itemDoubleClicked(QTreeWidgetItem*,int)\0"
};

const QMetaObject searchUser::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_searchUser,
      qt_meta_data_searchUser, 0 }
};

const QMetaObject *searchUser::metaObject() const
{
    return &staticMetaObject;
}

void *searchUser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_searchUser))
	return static_cast<void*>(const_cast< searchUser*>(this));
    return QWidget::qt_metacast(_clname);
}

int searchUser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: findAskedUsers((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: openChatWithFounded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: openInfoWindow((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: checkStatusFor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: addUserToContactList((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: on_moreButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: on_clearButton_clicked(); break;
        case 7: on_searchButton_clicked(); break;
        case 8: on_resultTreeWidget_itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: on_resultTreeWidget_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 10: addUserActionActivated(); break;
        case 11: checkStatusActionActivated(); break;
        case 12: userInformationActionActivated(); break;
        case 13: sendMessageActionActivated(); break;
        case 14: on_resultTreeWidget_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void searchUser::findAskedUsers(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void searchUser::openChatWithFounded(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void searchUser::openInfoWindow(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void searchUser::checkStatusFor(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void searchUser::addUserToContactList(const QString & _t1, const QString & _t2, bool _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
