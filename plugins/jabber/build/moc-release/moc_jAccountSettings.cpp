/****************************************************************************
** Meta object code from reading C++ file 'jAccountSettings.h'
**
** Created: Sun Feb 15 15:01:08 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/jAccountSettings.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jAccountSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_jAccountSettings[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      34,   17,   17,   17, 0x08,
      52,   17,   17,   17, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_jAccountSettings[] = {
    "jAccountSettings\0\0settingsSaved()\0"
    "okButtonClicked()\0applyButtonClicked()\0"
};

const QMetaObject jAccountSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_jAccountSettings,
      qt_meta_data_jAccountSettings, 0 }
};

const QMetaObject *jAccountSettings::metaObject() const
{
    return &staticMetaObject;
}

void *jAccountSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_jAccountSettings))
	return static_cast<void*>(const_cast< jAccountSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int jAccountSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: settingsSaved(); break;
        case 1: okButtonClicked(); break;
        case 2: applyButtonClicked(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void jAccountSettings::settingsSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
