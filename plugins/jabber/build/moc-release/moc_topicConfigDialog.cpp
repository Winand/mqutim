/****************************************************************************
** Meta object code from reading C++ file 'topicConfigDialog.h'
**
** Created: Sun Feb 15 15:02:23 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/conference/topicConfigDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'topicConfigDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_topicConfigDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_topicConfigDialog[] = {
    "topicConfigDialog\0"
};

const QMetaObject topicConfigDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_topicConfigDialog,
      qt_meta_data_topicConfigDialog, 0 }
};

const QMetaObject *topicConfigDialog::metaObject() const
{
    return &staticMetaObject;
}

void *topicConfigDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_topicConfigDialog))
	return static_cast<void*>(const_cast< topicConfigDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int topicConfigDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
