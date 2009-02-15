/****************************************************************************
** Meta object code from reading C++ file 'icqaccount.h'
**
** Created: Sat Feb 14 22:20:37 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../icqaccount.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'icqaccount.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_icqAccount[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      34,   11,   11,   11, 0x05,
      55,   11,   11,   11, 0x05,
      87,   11,   11,   11, 0x05,
     103,   11,   11,   11, 0x05,
     121,   11,   11,   11, 0x05,
     142,   11,   11,   11, 0x05,
     162,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     182,   11,   11,   11, 0x0a,
     204,   11,   11,   11, 0x0a,
     230,   11,   11,   11, 0x08,
     242,   11,   11,   11, 0x08,
     278,  271,   11,   11, 0x08,
     314,   11,   11,   11, 0x08,
     333,   11,   11,   11, 0x08,
     361,  356,   11,   11, 0x08,
     419,   11,   11,   11, 0x08,
     446,  444,   11,   11, 0x08,
     463,   11,   11,   11, 0x08,
     486,   11,   11,   11, 0x08,
     505,   11,   11,   11, 0x08,
     524,   11,   11,   11, 0x08,
     546,   11,   11,   11, 0x08,
     569,   11,   11,   11, 0x08,
     590,   11,   11,   11, 0x08,
     617,   11,   11,   11, 0x08,
     642,   11,   11,   11, 0x08,
     666,  444,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_icqAccount[] = {
    "icqAccount\0\0changeSettingsApply()\0"
    "statusChanged(QIcon)\0"
    "changeStatusInTrayMenu(QString)\0"
    "getNewMessage()\0readAllMessages()\0"
    "addToEventList(bool)\0updateTrayToolTip()\0"
    "updateTranslation()\0onUpdateTranslation()\0"
    "onReloadGeneralSettings()\0setStatus()\0"
    "setStatusIcon(accountStatus)\0status\0"
    "onOscarStatusChanged(accountStatus)\0"
    "emitChangeStatus()\0systemMessage(QString)\0"
    ",,,,\0userMessage(QString,QString,QString,userMessageType,bool)\0"
    "networkSettingsChanged()\0f\0addToEvent(bool)\0"
    "updateStatusMenu(bool)\0setVisibleForAll()\0"
    "setVisibleForVis()\0setNotVisibleForInv()\0"
    "setVisibleForContact()\0setInvisibleForAll()\0"
    "deleteTrayWindow(QObject*)\0"
    "generalSettingsChanged()\0"
    "customStatusTriggered()\0accountConnected(bool)\0"
};

const QMetaObject icqAccount::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_icqAccount,
      qt_meta_data_icqAccount, 0 }
};

const QMetaObject *icqAccount::metaObject() const
{
    return &staticMetaObject;
}

void *icqAccount::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_icqAccount))
	return static_cast<void*>(const_cast< icqAccount*>(this));
    return QObject::qt_metacast(_clname);
}

int icqAccount::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeSettingsApply(); break;
        case 1: statusChanged((*reinterpret_cast< const QIcon(*)>(_a[1]))); break;
        case 2: changeStatusInTrayMenu((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: getNewMessage(); break;
        case 4: readAllMessages(); break;
        case 5: addToEventList((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: updateTrayToolTip(); break;
        case 7: updateTranslation(); break;
        case 8: onUpdateTranslation(); break;
        case 9: onReloadGeneralSettings(); break;
        case 10: setStatus(); break;
        case 11: setStatusIcon((*reinterpret_cast< accountStatus(*)>(_a[1]))); break;
        case 12: onOscarStatusChanged((*reinterpret_cast< accountStatus(*)>(_a[1]))); break;
        case 13: emitChangeStatus(); break;
        case 14: systemMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 15: userMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< userMessageType(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 16: networkSettingsChanged(); break;
        case 17: addToEvent((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: updateStatusMenu((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: setVisibleForAll(); break;
        case 20: setVisibleForVis(); break;
        case 21: setNotVisibleForInv(); break;
        case 22: setVisibleForContact(); break;
        case 23: setInvisibleForAll(); break;
        case 24: deleteTrayWindow((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 25: generalSettingsChanged(); break;
        case 26: customStatusTriggered(); break;
        case 27: accountConnected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void icqAccount::changeSettingsApply()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void icqAccount::statusChanged(const QIcon & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void icqAccount::changeStatusInTrayMenu(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void icqAccount::getNewMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void icqAccount::readAllMessages()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void icqAccount::addToEventList(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void icqAccount::updateTrayToolTip()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void icqAccount::updateTranslation()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}
