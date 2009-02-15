/****************************************************************************
** Meta object code from reading C++ file 'contactsettings.h'
**
** Created: Sat Feb 14 22:21:04 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../settings/contactsettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contactsettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_ContactSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      35,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ContactSettings[] = {
    "ContactSettings\0\0settingsChanged()\0"
    "settingsSaved()\0widgetStateChanged()\0"
};

const QMetaObject ContactSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ContactSettings,
      qt_meta_data_ContactSettings, 0 }
};

const QMetaObject *ContactSettings::metaObject() const
{
    return &staticMetaObject;
}

void *ContactSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ContactSettings))
	return static_cast<void*>(const_cast< ContactSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int ContactSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: settingsChanged(); break;
        case 1: settingsSaved(); break;
        case 2: widgetStateChanged(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ContactSettings::settingsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ContactSettings::settingsSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
