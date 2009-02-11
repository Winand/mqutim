/****************************************************************************
** Meta object code from reading C++ file 'icqlayer.h'
**
** Created: Mon Feb 9 20:47:09 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../icqlayer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'icqlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_IcqLayer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_IcqLayer[] = {
    "IcqLayer\0"
};

const QMetaObject IcqLayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_IcqLayer,
      qt_meta_data_IcqLayer, 0 }
};

const QMetaObject *IcqLayer::metaObject() const
{
    return &staticMetaObject;
}

void *IcqLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IcqLayer))
	return static_cast<void*>(const_cast< IcqLayer*>(this));
    if (!strcmp(_clname, "ProtocolInterface"))
	return static_cast< ProtocolInterface*>(const_cast< IcqLayer*>(this));
    if (!strcmp(_clname, "org.qutim.plugininterface/0.2"))
	return static_cast< qutim_sdk_0_2::PluginInterface*>(const_cast< IcqLayer*>(this));
    return QObject::qt_metacast(_clname);
}

int IcqLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
