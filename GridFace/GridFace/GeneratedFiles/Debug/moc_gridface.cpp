/****************************************************************************
** Meta object code from reading C++ file 'gridface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gridface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gridface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GridFace_t {
    QByteArrayData data[9];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GridFace_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GridFace_t qt_meta_stringdata_GridFace = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GridFace"
QT_MOC_LITERAL(1, 9, 23), // "on_btn_next_run_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 20), // "on_btn_train_clicked"
QT_MOC_LITERAL(4, 55, 23), // "on_btn_validate_clicked"
QT_MOC_LITERAL(5, 79, 22), // "on_btn_browser_clicked"
QT_MOC_LITERAL(6, 102, 28), // "on_btn_next_validate_clicked"
QT_MOC_LITERAL(7, 131, 9), // "run_image"
QT_MOC_LITERAL(8, 141, 14) // "validate_image"

    },
    "GridFace\0on_btn_next_run_clicked\0\0"
    "on_btn_train_clicked\0on_btn_validate_clicked\0"
    "on_btn_browser_clicked\0"
    "on_btn_next_validate_clicked\0run_image\0"
    "validate_image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GridFace[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GridFace::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GridFace *_t = static_cast<GridFace *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_next_run_clicked(); break;
        case 1: _t->on_btn_train_clicked(); break;
        case 2: _t->on_btn_validate_clicked(); break;
        case 3: _t->on_btn_browser_clicked(); break;
        case 4: _t->on_btn_next_validate_clicked(); break;
        case 5: _t->run_image(); break;
        case 6: _t->validate_image(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GridFace::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GridFace.data,
      qt_meta_data_GridFace,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GridFace::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GridFace::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GridFace.stringdata0))
        return static_cast<void*>(const_cast< GridFace*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GridFace::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
