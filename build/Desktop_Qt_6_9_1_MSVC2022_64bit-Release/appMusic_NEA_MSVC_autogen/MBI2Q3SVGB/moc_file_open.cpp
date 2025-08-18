/****************************************************************************
** Meta object code from reading C++ file 'file_open.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../file_opening/file_open.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'file_open.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9file_openE_t {};
} // unnamed namespace

template <> constexpr inline auto file_open::qt_create_metaobjectdata<qt_meta_tag_ZN9file_openE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "file_open",
        "openNewFile",
        "",
        "target_dir",
        "openNewDirectory",
        "name_from_project_files",
        "file_path",
        "get_base_name",
        "get_current_dir",
        "isDir",
        "path",
        "delete_file_or_dir",
        "rename_file_or_dir",
        "new_name"
    };

    QtMocHelpers::UintData qt_methods {
        // Method 'openNewFile'
        QtMocHelpers::MethodData<QString(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'openNewDirectory'
        QtMocHelpers::MethodData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Method 'name_from_project_files'
        QtMocHelpers::MethodData<QString(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'get_base_name'
        QtMocHelpers::MethodData<QString(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'get_current_dir'
        QtMocHelpers::MethodData<QString()>(8, 2, QMC::AccessPublic, QMetaType::QString),
        // Method 'isDir'
        QtMocHelpers::MethodData<bool(QString)>(9, 2, QMC::AccessPublic, QMetaType::Bool, {{
            { QMetaType::QString, 10 },
        }}),
        // Method 'delete_file_or_dir'
        QtMocHelpers::MethodData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Method 'rename_file_or_dir'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<file_open, qt_meta_tag_ZN9file_openE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject file_open::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9file_openE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9file_openE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9file_openE_t>.metaTypes,
    nullptr
} };

void file_open::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<file_open *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { QString _r = _t->openNewFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->openNewDirectory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: { QString _r = _t->name_from_project_files((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 3: { QString _r = _t->get_base_name((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 4: { QString _r = _t->get_current_dir();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->isDir((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->delete_file_or_dir((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->rename_file_or_dir((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *file_open::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *file_open::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9file_openE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int file_open::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
