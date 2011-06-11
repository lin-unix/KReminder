/****************************************************************************
** Meta object code from reading C++ file 'kreminderbuttonbox.h'
**
** Created: Fri Jun 10 21:35:02 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ButtonBox/kreminderbuttonbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'kreminderbuttonbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KReminderButtonBox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      28,   20,   19,   19, 0x08,
      56,   20,   19,   19, 0x08,
      80,   20,   19,   19, 0x08,
     106,   19,   19,   19, 0x08,
     113,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KReminderButtonBox[] = {
    "KReminderButtonBox\0\0checked\0"
    "setAddReminderToolTip(bool)\0"
    "setAddNoteToolTip(bool)\0"
    "setConfigureToolTip(bool)\0next()\0"
    "sendToMenu()\0"
};

const QMetaObject KReminderButtonBox::staticMetaObject = {
    { &KDialogButtonBox::staticMetaObject, qt_meta_stringdata_KReminderButtonBox,
      qt_meta_data_KReminderButtonBox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KReminderButtonBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KReminderButtonBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KReminderButtonBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KReminderButtonBox))
        return static_cast<void*>(const_cast< KReminderButtonBox*>(this));
    return KDialogButtonBox::qt_metacast(_clname);
}

int KReminderButtonBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KDialogButtonBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setAddReminderToolTip((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: setAddNoteToolTip((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setConfigureToolTip((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: next(); break;
        case 4: sendToMenu(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
