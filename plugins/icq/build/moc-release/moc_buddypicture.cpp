/****************************************************************************
** Meta object code from reading C++ file 'buddypicture.h'
**
** Created: Mon Feb 9 20:46:54 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../buddypicture.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'buddypicture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_buddyPicture[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      34,   32,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   13,   13,   13, 0x08,
      88,   13,   13,   13, 0x08,
     109,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_buddyPicture[] = {
    "buddyPicture\0\0emptyAvatarList()\0,\0"
    "updateAvatar(QString,QByteArray)\0"
    "readDataFromSocket()\0socketDisconnected()\0"
    "socketConnected()\0"
};

const QMetaObject buddyPicture::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_buddyPicture,
      qt_meta_data_buddyPicture, 0 }
};

const QMetaObject *buddyPicture::metaObject() const
{
    return &staticMetaObject;
}

void *buddyPicture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_buddyPicture))
	return static_cast<void*>(const_cast< buddyPicture*>(this));
    return QObject::qt_metacast(_clname);
}

int buddyPicture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: emptyAvatarList(); break;
        case 1: updateAvatar((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 2: readDataFromSocket(); break;
        case 3: socketDisconnected(); break;
        case 4: socketConnected(); break;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void buddyPicture::emptyAvatarList()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void buddyPicture::updateAvatar(const QString & _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
