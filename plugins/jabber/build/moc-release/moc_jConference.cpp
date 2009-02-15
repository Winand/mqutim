/****************************************************************************
** Meta object code from reading C++ file 'jConference.h'
**
** Created: Sun Feb 15 15:02:18 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/conference/jConference.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jConference.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jConference[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      65,   13,   12,   12, 0x05,
     116,   13,   12,   12, 0x05,
     235,  170,   12,   12, 0x05,
     361,  297,   12,   12, 0x05,
     492,  430,   12,   12, 0x05,
     614,  559,   12,   12, 0x05,
     742,  685,   12,   12, 0x05,
     807,   13,   12,   12, 0x05,
     911,  868,   12,   12, 0x05,
    1024,  953,   12,   12, 0x05,
    1140, 1091,   12,   12, 0x05,
    1256, 1192,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
    1333,   12,   12,   12, 0x0a,
    1349,   12,   12,   12, 0x0a,
    1396, 1372,   12,   12, 0x0a,
    1479, 1450,   12,   12, 0x0a,
    1527,   12,   12,   12, 0x0a,
    1538,   12,   12,   12, 0x0a,
    1548,   12,   12,   12, 0x0a,
    1565,   12,   12,   12, 0x0a,
    1586,   12,   12,   12, 0x0a,
    1631, 1605,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_jConference[] = {
    "jConference\0\0"
    "protocol_name,conference_name,account_name,nickname\0"
    "addConferenceItem(QString,QString,QString,QString)\0"
    "removeConferenceItem(QString,QString,QString,QString)\0"
    "protocol_name,conference_name,account_name,nickname,new_nickname\0"
    "renameConferenceItem(QString,QString,QString,QString,QString)\0"
    "protocol_name,conference_name,account_name,nickname,status,mass\0"
    "setConferenceItemStatus(QString,QString,QString,QString,QString,int)\0"
    "protocol_name,conference_name,account_name,nickname,role,mass\0"
    "setConferenceItemRole(QString,QString,QString,QString,QString,int)\0"
    "conference_name,account_name,from,message,date,history\0"
    "addMessageToConference(QString,QString,QString,QString,QDateTime,bool)\0"
    "protocol_name,conference_name,account_name,nickname,name\0"
    "conferenceClientVersion(QString,QString,QString,QString,QString)\0"
    "changeOwnConferenceNickName(QString,QString,QString,QString)\0"
    "protocol_name,conference_name,account_name\0"
    "createConference(QString,QString,QString)\0"
    "protocol_name,conference_name,account_name,nickname,icon_name,position\0"
    "setConferenceItemIcon(QString,QString,QString,QString,QString,int)\0"
    "protocol_name,conference_name,account_name,topic\0"
    "setConferenceTopic(QString,QString,QString,QString)\0"
    "protocol_name,conference_name,account_name,message,date,history\0"
    "addSystemMessageToConference(QString,QString,QString,QString,QDateTime"
    ",bool)\0"
    "joinGroupchat()\0destroyJoinGroupChat()\0"
    "host,room,nick,password\0"
    "createConferenceRoom(QString,QString,QString,QString)\0"
    "conference,nickname,password\0"
    "s_createConferenceRoom(QString,QString,QString)\0"
    "kickUser()\0banUser()\0setVisitorUser()\0"
    "setParticipantUser()\0setModeratorUser()\0"
    "room,from,reason,password\0"
    "s_conferenceInvite(JID,JID,QString,QString)\0"
};

const QMetaObject jConference::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jConference,
      qt_meta_data_jConference, 0 }
};

const QMetaObject *jConference::metaObject() const
{
    return &staticMetaObject;
}

void *jConference::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jConference))
	return static_cast<void*>(const_cast< jConference*>(this));
    if (!strcmp(_clname, "MUCRoomHandler"))
	return static_cast< MUCRoomHandler*>(const_cast< jConference*>(this));
    if (!strcmp(_clname, "DiscoHandler"))
	return static_cast< DiscoHandler*>(const_cast< jConference*>(this));
    if (!strcmp(_clname, "PresenceHandler"))
	return static_cast< PresenceHandler*>(const_cast< jConference*>(this));
    return QObject::qt_metacast(_clname);
}

int jConference::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: removeConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: renameConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 3: setConferenceItemStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 4: setConferenceItemRole((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 5: addMessageToConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 6: conferenceClientVersion((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 7: changeOwnConferenceNickName((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 8: createConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 9: setConferenceItemIcon((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 10: setConferenceTopic((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 11: addSystemMessageToConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 12: joinGroupchat(); break;
        case 13: destroyJoinGroupChat(); break;
        case 14: createConferenceRoom((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 15: s_createConferenceRoom((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 16: kickUser(); break;
        case 17: banUser(); break;
        case 18: setVisitorUser(); break;
        case 19: setParticipantUser(); break;
        case 20: setModeratorUser(); break;
        case 21: s_conferenceInvite((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        }
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void jConference::addConferenceItem(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void jConference::removeConferenceItem(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void jConference::renameConferenceItem(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void jConference::setConferenceItemStatus(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void jConference::setConferenceItemRole(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void jConference::addMessageToConference(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QDateTime & _t5, bool _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void jConference::conferenceClientVersion(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void jConference::changeOwnConferenceNickName(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void jConference::createConference(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void jConference::setConferenceItemIcon(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QString & _t5, int _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void jConference::setConferenceTopic(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void jConference::addSystemMessageToConference(const QString & _t1, const QString & _t2, const QString & _t3, const QString & _t4, const QDateTime & _t5, bool _t6)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
static const uint qt_meta_data_BookmarkAction[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BookmarkAction[] = {
    "BookmarkAction\0\0triggered()\0"
};

const QMetaObject BookmarkAction::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BookmarkAction,
      qt_meta_data_BookmarkAction, 0 }
};

const QMetaObject *BookmarkAction::metaObject() const
{
    return &staticMetaObject;
}

void *BookmarkAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BookmarkAction))
	return static_cast<void*>(const_cast< BookmarkAction*>(this));
    return QObject::qt_metacast(_clname);
}

int BookmarkAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: triggered(); break;
        }
        _id -= 1;
    }
    return _id;
}
