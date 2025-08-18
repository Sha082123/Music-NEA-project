/****************************************************************************
** Meta object code from reading C++ file 'part_object.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../part_objects/part_object.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'part_object.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11part_objectE_t {};
} // unnamed namespace

template <> constexpr inline auto part_object::qt_create_metaobjectdata<qt_meta_tag_ZN11part_objectE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "part_object",
        "list_PNG_pathsChanged",
        "",
        "reh_y_coordsChanged",
        "break_listChanged",
        "part_listChanged",
        "savedChanged",
        "file_pathChanged",
        "openFile",
        "file_path",
        "mode",
        "update",
        "new_break_item",
        "input",
        "delete_break_item",
        "measure_number",
        "update_break_list",
        "id",
        "apply_breaks",
        "element_from_point",
        "QVariantList",
        "point",
        "page_number",
        "coordinates_from_measure",
        "save_file",
        "list_PNG_paths",
        "reh_y_coords",
        "break_list",
        "part_list",
        "saved"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'list_PNG_pathsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'reh_y_coordsChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'break_listChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'part_listChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'savedChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'file_pathChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'openFile'
        QtMocHelpers::MethodData<void(const QString &, int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::Int, 10 },
        }}),
        // Method 'update'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'new_break_item'
        QtMocHelpers::MethodData<int(QString)>(12, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 13 },
        }}),
        // Method 'delete_break_item'
        QtMocHelpers::MethodData<int(int)>(14, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 15 },
        }}),
        // Method 'update_break_list'
        QtMocHelpers::MethodData<int(QString, QString)>(16, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 17 }, { QMetaType::QString, 13 },
        }}),
        // Method 'apply_breaks'
        QtMocHelpers::MethodData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'element_from_point'
        QtMocHelpers::MethodData<QVariantList(const QPointF &, const int &)>(19, 2, QMC::AccessPublic, 0x80000000 | 20, {{
            { QMetaType::QPointF, 21 }, { QMetaType::Int, 22 },
        }}),
        // Method 'coordinates_from_measure'
        QtMocHelpers::MethodData<QVariantList(int)>(23, 2, QMC::AccessPublic, 0x80000000 | 20, {{
            { QMetaType::Int, 15 },
        }}),
        // Method 'save_file'
        QtMocHelpers::MethodData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'list_PNG_paths'
        QtMocHelpers::PropertyData<QStringList>(25, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 0),
        // property 'reh_y_coords'
        QtMocHelpers::PropertyData<QVariantList>(26, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet | QMC::Final, 1),
        // property 'break_list'
        QtMocHelpers::PropertyData<QVariantList>(27, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::StdCppSet | QMC::Final, 2),
        // property 'part_list'
        QtMocHelpers::PropertyData<QVariantList>(28, 0x80000000 | 20, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 3),
        // property 'saved'
        QtMocHelpers::PropertyData<QString>(29, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 4),
        // property 'file_path'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 5),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<part_object, qt_meta_tag_ZN11part_objectE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject part_object::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11part_objectE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11part_objectE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11part_objectE_t>.metaTypes,
    nullptr
} };

void part_object::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<part_object *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->list_PNG_pathsChanged(); break;
        case 1: _t->reh_y_coordsChanged(); break;
        case 2: _t->break_listChanged(); break;
        case 3: _t->part_listChanged(); break;
        case 4: _t->savedChanged(); break;
        case 5: _t->file_pathChanged(); break;
        case 6: _t->openFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->update(); break;
        case 8: { int _r = _t->new_break_item((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 9: { int _r = _t->delete_break_item((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { int _r = _t->update_break_list((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->apply_breaks(); break;
        case 12: { QVariantList _r = _t->element_from_point((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 13: { QVariantList _r = _t->coordinates_from_measure((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 14: _t->save_file(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::list_PNG_pathsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::reh_y_coordsChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::break_listChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::part_listChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::savedChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_object::*)()>(_a, &part_object::file_pathChanged, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QStringList*>(_v) = _t->list_PNG_paths(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->reh_y_coords(); break;
        case 2: *reinterpret_cast<QVariantList*>(_v) = _t->break_list(); break;
        case 3: *reinterpret_cast<QVariantList*>(_v) = _t->part_list(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->saved(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->file_path(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setlist_PNG_paths(*reinterpret_cast<QStringList*>(_v)); break;
        case 1: _t->setReh_y_coords(*reinterpret_cast<QVariantList*>(_v)); break;
        case 2: _t->setBreak_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 3: _t->setpart_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 4: _t->setsaved(*reinterpret_cast<QString*>(_v)); break;
        case 5: _t->setfile_path(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *part_object::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *part_object::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11part_objectE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int part_object::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void part_object::list_PNG_pathsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void part_object::reh_y_coordsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void part_object::break_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void part_object::part_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void part_object::savedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void part_object::file_pathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
