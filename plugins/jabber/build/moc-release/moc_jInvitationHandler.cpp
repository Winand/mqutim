/****************************************************************************
** Meta object code from reading C++ file 'jInvitationHandler.h'
**
** Created: Sun Feb 15 15:01:19 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jInvitationHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jInvitationHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jInvitationHandler[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      46,   20,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_jInvitationHandler[] = {
    "jInvitationHandler\0\0room,from,reason,password\0"
    "conferenceInvite(JID,JID,QString,QString)\0"
};

const QMetaObject jInvitationHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_jInvitationHandler,
      qt_meta_data_jInvitationHandler, 0 }
};

const QMetaObject *jInvitationHandler::metaObject() const
{
    return &staticMetaObject;
}

void *jInvitationHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jInvitationHandler))
	return static_cast<void*>(const_cast< jInvitationHandler*>(this));
    if (!strcmp(_clname, "MUCInvitationHandler"))
	return static_cast< MUCInvitationHandler*>(const_cast< jInvitationHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int jInvitationHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: conferenceInvite((*reinterpret_cast< const JID(*)>(_a[1])),(*reinterpret_cast< const JID(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void jInvitationHandler::conferenceInvite(const JID & _t1, const JID & _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
