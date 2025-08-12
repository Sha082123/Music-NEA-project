/****************************************************************************
** Meta object code from reading C++ file 'part_manager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../part_objects/part_manager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'part_manager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12part_managerE_t {};
} // unnamed namespace

template <> constexpr inline auto part_manager::qt_create_metaobjectdata<qt_meta_tag_ZN12part_managerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "part_manager",
        "part_name_listChanged",
        "",
        "buffer_part_name_listChanged",
        "create_root_part",
        "part_name",
        "create_new_part",
        "delete_part",
        "index",
        "update_part",
        "new_name",
        "old_name",
        "update_part_staves",
        "QVariantList",
        "part_existence",
        "apply_part_actions",
        "set_current_part",
        "list_size",
        "staff_exists_in_part",
        "staff_n_value",
        "scan_part_directory",
        "source_path",
        "save_all",
        "clear_parts",
        "part_name_list",
        "buffer_part_name_list"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'part_name_listChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'buffer_part_name_listChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'create_root_part'
        QtMocHelpers::MethodData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'create_new_part'
        QtMocHelpers::MethodData<int(QString)>(6, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::QString, 5 },
        }}),
        // Method 'delete_part'
        QtMocHelpers::MethodData<int(int)>(7, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 8 },
        }}),
        // Method 'update_part'
        QtMocHelpers::MethodData<int(int, QString, QString)>(9, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 8 }, { QMetaType::QString, 10 }, { QMetaType::QString, 11 },
        }}),
        // Method 'update_part_staves'
        QtMocHelpers::MethodData<int(QVariantList, int)>(12, 2, QMC::AccessPublic, QMetaType::Int, {{
            { 0x80000000 | 13, 14 }, { QMetaType::Int, 8 },
        }}),
        // Method 'apply_part_actions'
        QtMocHelpers::MethodData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'set_current_part'
        QtMocHelpers::MethodData<void(int)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Method 'list_size'
        QtMocHelpers::MethodData<int()>(17, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'staff_exists_in_part'
        QtMocHelpers::MethodData<bool(int, QString)>(18, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::Int, 19 }, { QMetaType::QString, 5 },
        }}),
        // Method 'scan_part_directory'
        QtMocHelpers::MethodData<void(QString)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Method 'save_all'
        QtMocHelpers::MethodData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clear_parts'
        QtMocHelpers::MethodData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'part_name_list'
        QtMocHelpers::PropertyData<QStringList>(24, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 0),
        // property 'buffer_part_name_list'
        QtMocHelpers::PropertyData<QStringList>(25, QMetaType::QStringList, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 1),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<part_manager, qt_meta_tag_ZN12part_managerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject part_manager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12part_managerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12part_managerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12part_managerE_t>.metaTypes,
    nullptr
} };

void part_manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<part_manager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->part_name_listChanged(); break;
        case 1: _t->buffer_part_name_listChanged(); break;
        case 2: _t->create_root_part((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: { int _r = _t->create_new_part((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { int _r = _t->delete_part((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->update_part((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->update_part_staves((*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->apply_part_actions(); break;
        case 8: _t->set_current_part((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: { int _r = _t->list_size();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->staff_exists_in_part((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->scan_part_directory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->save_all(); break;
        case 13: _t->clear_parts(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (part_manager::*)()>(_a, &part_manager::part_name_listChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (part_manager::*)()>(_a, &part_manager::buffer_part_name_listChanged, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QStringList*>(_v) = _t->part_name_list(); break;
        case 1: *reinterpret_cast<QStringList*>(_v) = _t->buffer_part_name_list(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setpart_name_list(*reinterpret_cast<QStringList*>(_v)); break;
        case 1: _t->setbuffer_part_name_list(*reinterpret_cast<QStringList*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *part_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *part_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12part_managerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int part_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void part_manager::part_name_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void part_manager::buffer_part_name_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
