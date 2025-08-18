/****************************************************************************
** Meta object code from reading C++ file 'track_manager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../track_objects/track_manager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'track_manager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13track_managerE_t {};
} // unnamed namespace

template <> constexpr inline auto track_manager::qt_create_metaobjectdata<qt_meta_tag_ZN13track_managerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "track_manager",
        "music_loadedChanged",
        "",
        "qml_track_listChanged",
        "mute_listChanged",
        "solo_listChanged",
        "volume_listChanged",
        "start_ms_listChanged",
        "end_ms_listChanged",
        "duration_listChanged",
        "time_before_start_listChanged",
        "prevent_muteChanged",
        "qml_sync_pointsChanged",
        "clear",
        "get_track_count",
        "scan_audio_directory",
        "directory",
        "toggle_mute",
        "index",
        "toggle_solo",
        "set_volume",
        "volume",
        "save_playback_states",
        "open_new_track",
        "delete_track",
        "delete_sync_point",
        "add_sync_point",
        "time",
        "measure",
        "beat",
        "apply_sync_points",
        "ms_to_time",
        "ms",
        "music_loaded",
        "qml_track_list",
        "QVariantList",
        "mute_list",
        "solo_list",
        "volume_list",
        "start_ms_list",
        "end_ms_list",
        "duration_list",
        "time_before_start_list",
        "prevent_mute",
        "qml_sync_points"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'music_loadedChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'qml_track_listChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'mute_listChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'solo_listChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'volume_listChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'start_ms_listChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'end_ms_listChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'duration_listChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'time_before_start_listChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'prevent_muteChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'qml_sync_pointsChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'clear'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'get_track_count'
        QtMocHelpers::MethodData<int()>(14, 2, QMC::AccessPublic, QMetaType::Int),
        // Method 'scan_audio_directory'
        QtMocHelpers::MethodData<void(QString)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 16 },
        }}),
        // Method 'toggle_mute'
        QtMocHelpers::MethodData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_solo'
        QtMocHelpers::MethodData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'set_volume'
        QtMocHelpers::MethodData<void(int, float)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Float, 21 },
        }}),
        // Method 'save_playback_states'
        QtMocHelpers::MethodData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'open_new_track'
        QtMocHelpers::MethodData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'delete_track'
        QtMocHelpers::MethodData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'delete_sync_point'
        QtMocHelpers::MethodData<int(int)>(25, 2, QMC::AccessPublic, QMetaType::Int, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'add_sync_point'
        QtMocHelpers::MethodData<void(int, int, float)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 27 }, { QMetaType::Int, 28 }, { QMetaType::Float, 29 },
        }}),
        // Method 'apply_sync_points'
        QtMocHelpers::MethodData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'ms_to_time'
        QtMocHelpers::MethodData<QString(int)>(31, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::Int, 32 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'music_loaded'
        QtMocHelpers::PropertyData<bool>(33, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 0),
        // property 'qml_track_list'
        QtMocHelpers::PropertyData<QVariantList>(34, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 1),
        // property 'mute_list'
        QtMocHelpers::PropertyData<QVariantList>(36, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 2),
        // property 'solo_list'
        QtMocHelpers::PropertyData<QVariantList>(37, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 3),
        // property 'volume_list'
        QtMocHelpers::PropertyData<QVariantList>(38, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 4),
        // property 'start_ms_list'
        QtMocHelpers::PropertyData<QVariantList>(39, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 5),
        // property 'end_ms_list'
        QtMocHelpers::PropertyData<QVariantList>(40, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 6),
        // property 'duration_list'
        QtMocHelpers::PropertyData<QVariantList>(41, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 7),
        // property 'time_before_start_list'
        QtMocHelpers::PropertyData<QVariantList>(42, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 8),
        // property 'prevent_mute'
        QtMocHelpers::PropertyData<bool>(43, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 9),
        // property 'qml_sync_points'
        QtMocHelpers::PropertyData<QVariantList>(44, 0x80000000 | 35, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 10),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<track_manager, qt_meta_tag_ZN13track_managerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject track_manager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13track_managerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13track_managerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13track_managerE_t>.metaTypes,
    nullptr
} };

void track_manager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<track_manager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->music_loadedChanged(); break;
        case 1: _t->qml_track_listChanged(); break;
        case 2: _t->mute_listChanged(); break;
        case 3: _t->solo_listChanged(); break;
        case 4: _t->volume_listChanged(); break;
        case 5: _t->start_ms_listChanged(); break;
        case 6: _t->end_ms_listChanged(); break;
        case 7: _t->duration_listChanged(); break;
        case 8: _t->time_before_start_listChanged(); break;
        case 9: _t->prevent_muteChanged(); break;
        case 10: _t->qml_sync_pointsChanged(); break;
        case 11: _t->clear(); break;
        case 12: { int _r = _t->get_track_count();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->scan_audio_directory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 14: _t->toggle_mute((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->toggle_solo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->set_volume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 17: _t->save_playback_states(); break;
        case 18: _t->open_new_track(); break;
        case 19: _t->delete_track((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 20: { int _r = _t->delete_sync_point((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 21: _t->add_sync_point((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[3]))); break;
        case 22: _t->apply_sync_points(); break;
        case 23: { QString _r = _t->ms_to_time((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::music_loadedChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::qml_track_listChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::mute_listChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::solo_listChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::volume_listChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::start_ms_listChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::end_ms_listChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::duration_listChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::time_before_start_listChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::prevent_muteChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (track_manager::*)()>(_a, &track_manager::qml_sync_pointsChanged, 10))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->music_loaded(); break;
        case 1: *reinterpret_cast<QVariantList*>(_v) = _t->qml_track_list(); break;
        case 2: *reinterpret_cast<QVariantList*>(_v) = _t->mute_list(); break;
        case 3: *reinterpret_cast<QVariantList*>(_v) = _t->solo_list(); break;
        case 4: *reinterpret_cast<QVariantList*>(_v) = _t->volume_list(); break;
        case 5: *reinterpret_cast<QVariantList*>(_v) = _t->start_ms_list(); break;
        case 6: *reinterpret_cast<QVariantList*>(_v) = _t->end_ms_list(); break;
        case 7: *reinterpret_cast<QVariantList*>(_v) = _t->duration_list(); break;
        case 8: *reinterpret_cast<QVariantList*>(_v) = _t->time_before_start_list(); break;
        case 9: *reinterpret_cast<bool*>(_v) = _t->prevent_mute(); break;
        case 10: *reinterpret_cast<QVariantList*>(_v) = _t->qml_sync_points(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setmusic_loaded(*reinterpret_cast<bool*>(_v)); break;
        case 1: _t->setqml_track_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 2: _t->setmute_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 3: _t->setmolo_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 4: _t->setvolume_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 5: _t->setstart_ms_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 6: _t->setend_ms_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 7: _t->setduration_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 8: _t->settime_before_start_list(*reinterpret_cast<QVariantList*>(_v)); break;
        case 9: _t->setprevent_mute(*reinterpret_cast<bool*>(_v)); break;
        case 10: _t->setqml_sync_points(*reinterpret_cast<QVariantList*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *track_manager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *track_manager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13track_managerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int track_manager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void track_manager::music_loadedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void track_manager::qml_track_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void track_manager::mute_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void track_manager::solo_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void track_manager::volume_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void track_manager::start_ms_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void track_manager::end_ms_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void track_manager::duration_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void track_manager::time_before_start_listChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void track_manager::prevent_muteChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void track_manager::qml_sync_pointsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
