/****************************************************************************
** Meta object code from reading C++ file 'jSlotSignal.h'
**
** Created: Sun Feb 15 15:01:39 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jSlotSignal.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jSlotSignal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jSlotSignal[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      33,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   13,   12,   12, 0x0a,
      77,   72,   12,   12, 0x0a,
     135,  118,   12,   12, 0x0a,
     186,  183,   12,   12, 0x0a,
     233,  183,   12,   12, 0x0a,
     295,  280,   12,   12, 0x0a,
     353,  343,   12,   12, 0x0a,
     409,  394,   12,   12, 0x0a,
     460,  446,   12,   12, 0x0a,
     512,  497,   12,   12, 0x0a,
     607,  555,   12,   12, 0x0a,
     658,  555,   12,   12, 0x0a,
     777,  712,   12,   12, 0x0a,
     903,  839,   12,   12, 0x0a,
    1034,  972,   12,   12, 0x0a,
    1156, 1101,   12,   12, 0x0a,
    1284, 1227,   12,   12, 0x0a,
    1362, 1349,   12,   12, 0x0a,
    1420, 1399,   12,   12, 0x0a,
    1476, 1456,   12,   12, 0x0a,
    1521,  555,   12,   12, 0x0a,
    1625, 1582,   12,   12, 0x0a,
    1738, 1667,   12,   12, 0x0a,
    1854, 1805,   12,   12, 0x0a,
    1970, 1906,   12,   12, 0x0a,
    2070, 2047,   12,   12, 0x0a,
    2127, 2114,   12,   12, 0x0a,
    2185, 2169,   12,   12, 0x0a,
    2246, 2236,   12,   12, 0x0a,
    2302, 2288,   12,   12, 0x0a,
    2355, 2348,   12,   12, 0x0a,
    2380,   12,   12,   12, 0x0a,
    2399,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_jSlotSignal[] = {
    "jSlotSignal\0\0item,item_name\0"
    "addItemToContactList(TreeModelItem,QString)\0"
    "Item\0removeItemFromContactList(TreeModelItem)\0"
    "item,status,mass\0"
    "setContactItemStatus(TreeModelItem,QString,int)\0"
    ",,\0setClientCustomIcon(TreeModelItem,QString,int)\0"
    "setClientCustomText(TreeModelItem,QString,int)\0"
    "item,date,text\0"
    "addMessageFrom(TreeModelItem,QDateTime,QString)\0"
    "item,text\0addServiceMessage(TreeModelItem,QString)\0"
    "item,invisible\0setItemInvisible(TreeModelItem,bool)\0"
    "item,position\0messageDelievered(TreeModelItem,int)\0"
    "item,composing\0"
    "sendTypingNotification(TreeModelItem,bool)\0"
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
    "contact,name\0clientVersion(TreeModelItem,QString)\0"
    "account_name,message\0"
    "systemNotification(QString,QString)\0"
    "jid,vcard,avatarUrl\0"
    "s_onFetchVCard(QString,const VCard*,QString)\0"
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
    "text,jid,jabber_client\0"
    "createAcceptAuthDialog(QString,JID,Client*)\0"
    "item,message\0customNotification(TreeModelItem,QString)\0"
    "OldItem,NewItem\0"
    "moveItemInContactList(TreeModelItem,TreeModelItem)\0"
    "Item,name\0setContactItemName(TreeModelItem,QString)\0"
    "b_list,c_list\0"
    "bookmarksHandled(BookmarkList,ConferenceList)\0"
    "xml,in\0tagHandled(QString,bool)\0"
    "createXmlConsole()\0destroyXmlConsole()\0"
};

const QMetaObject jSlotSignal::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jSlotSignal,
      qt_meta_data_jSlotSignal, 0 }
};

const QMetaObject *jSlotSignal::metaObject() const
{
    return &staticMetaObject;
}

void *jSlotSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jSlotSignal))
	return static_cast<void*>(const_cast< jSlotSignal*>(this));
    return QObject::qt_metacast(_clname);
}

int jSlotSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addItemToContactList((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: removeItemFromContactList((*reinterpret_cast< TreeModelItem(*)>(_a[1]))); break;
        case 2: setContactItemStatus((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: setClientCustomIcon((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: setClientCustomText((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: addMessageFrom((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QDateTime(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: addServiceMessage((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: setItemInvisible((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: messageDelievered((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: sendTypingNotification((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: addConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 11: removeConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 12: renameConferenceItem((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 13: setConferenceItemStatus((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 14: setConferenceItemRole((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 15: addMessageToConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 16: conferenceClientVersion((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5]))); break;
        case 17: clientVersion((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 18: systemNotification((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 19: s_onFetchVCard((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const VCard*(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 20: changeOwnConferenceNickName((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 21: createConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 22: setConferenceItemIcon((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 23: setConferenceTopic((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 24: addSystemMessageToConference((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4])),(*reinterpret_cast< const QDateTime(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 25: createAcceptAuthDialog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< Client*(*)>(_a[3]))); break;
        case 26: customNotification((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 27: moveItemInContactList((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const TreeModelItem(*)>(_a[2]))); break;
        case 28: setContactItemName((*reinterpret_cast< const TreeModelItem(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 29: bookmarksHandled((*reinterpret_cast< const BookmarkList(*)>(_a[1])),(*reinterpret_cast< const ConferenceList(*)>(_a[2]))); break;
        case 30: tagHandled((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 31: createXmlConsole(); break;
        case 32: destroyXmlConsole(); break;
        }
        _id -= 33;
    }
    return _id;
}
