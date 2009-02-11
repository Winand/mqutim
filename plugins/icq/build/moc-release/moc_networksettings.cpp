/****************************************************************************
** Meta object code from reading C++ file 'networksettings.h'
**
** Created: Mon Feb 9 20:47:29 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../settings/networksettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networksettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_networkSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      35,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   16,   16,   16, 0x08,
      72,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_networkSettings[] = {
    "networkSettings\0\0settingsChanged()\0"
    "settingsSaved()\0widgetStateChanged()\0"
    "proxyTypeChanged(int)\0"
};

const QMetaObject networkSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_networkSettings,
      qt_meta_data_networkSettings, 0 }
};

const QMetaObject *networkSettings::metaObject() const
{
    return &staticMetaObject;
}

void *networkSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_networkSettings))
	return static_cast<void*>(const_cast< networkSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int networkSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: settingsChanged(); break;
        case 1: settingsSaved(); break;
        case 2: widgetStateChanged(); break;
        case 3: proxyTypeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void networkSettings::settingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void networkSettings::settingsSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
