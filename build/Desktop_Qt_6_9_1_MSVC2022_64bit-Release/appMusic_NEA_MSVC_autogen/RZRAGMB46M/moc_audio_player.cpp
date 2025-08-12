/****************************************************************************
** Meta object code from reading C++ file 'audio_player.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../render/audio_player.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'audio_player.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12audio_playerE_t {};
} // unnamed namespace

template <> constexpr inline auto audio_player::qt_create_metaobjectdata<qt_meta_tag_ZN12audio_playerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "audio_player",
        "qml_positionChanged",
        "",
        "sample_position_changed",
        "size_t",
        "current_position",
        "total_msChanged",
        "decibelsChanged",
        "play_audio",
        "pause_audio",
        "refresh",
        "back_10s",
        "forward_10s",
        "skip_to_start",
        "skip_to_end",
        "set_position",
        "position",
        "set_volume",
        "track_index",
        "volume",
        "set_master_volume",
        "mute",
        "index",
        "unmute",
        "qml_position",
        "total_ms",
        "decibels"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'qml_positionChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sample_position_changed'
        QtMocHelpers::SignalData<void(size_t)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 5 },
        }}),
        // Signal 'total_msChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'decibelsChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'play_audio'
        QtMocHelpers::MethodData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'pause_audio'
        QtMocHelpers::MethodData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'refresh'
        QtMocHelpers::MethodData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'back_10s'
        QtMocHelpers::MethodData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'forward_10s'
        QtMocHelpers::MethodData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'skip_to_start'
        QtMocHelpers::MethodData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'skip_to_end'
        QtMocHelpers::MethodData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'set_position'
        QtMocHelpers::MethodData<void(const int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Method 'set_volume'
        QtMocHelpers::MethodData<void(int, float)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Float, 19 },
        }}),
        // Method 'set_master_volume'
        QtMocHelpers::MethodData<void(float)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 19 },
        }}),
        // Method 'mute'
        QtMocHelpers::MethodData<void(int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Method 'unmute'
        QtMocHelpers::MethodData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'qml_position'
        QtMocHelpers::PropertyData<size_t>(24, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 0),
        // property 'total_ms'
        QtMocHelpers::PropertyData<size_t>(25, 0x80000000 | 4, QMC::DefaultPropertyFlags | QMC::Writable | QMC::EnumOrFlag | QMC::Final, 2),
        // property 'decibels'
        QtMocHelpers::PropertyData<float>(26, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::Final, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<audio_player, qt_meta_tag_ZN12audio_playerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject audio_player::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12audio_playerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12audio_playerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12audio_playerE_t>.metaTypes,
    nullptr
} };

void audio_player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<audio_player *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->qml_positionChanged(); break;
        case 1: _t->sample_position_changed((*reinterpret_cast< std::add_pointer_t<size_t>>(_a[1]))); break;
        case 2: _t->total_msChanged(); break;
        case 3: _t->decibelsChanged(); break;
        case 4: _t->play_audio(); break;
        case 5: _t->pause_audio(); break;
        case 6: _t->refresh(); break;
        case 7: _t->back_10s(); break;
        case 8: _t->forward_10s(); break;
        case 9: _t->skip_to_start(); break;
        case 10: _t->skip_to_end(); break;
        case 11: _t->set_position((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->set_volume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 13: _t->set_master_volume((*reinterpret_cast< std::add_pointer_t<float>>(_a[1]))); break;
        case 14: _t->mute((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->unmute((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (audio_player::*)()>(_a, &audio_player::qml_positionChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (audio_player::*)(size_t )>(_a, &audio_player::sample_position_changed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (audio_player::*)()>(_a, &audio_player::total_msChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (audio_player::*)()>(_a, &audio_player::decibelsChanged, 3))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<size_t*>(_v) = _t->qml_position(); break;
        case 1: *reinterpret_cast<size_t*>(_v) = _t->total_ms(); break;
        case 2: *reinterpret_cast<float*>(_v) = _t->decibels(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setqml_position(*reinterpret_cast<size_t*>(_v)); break;
        case 1: _t->settotal_ms(*reinterpret_cast<size_t*>(_v)); break;
        case 2: _t->setdecibels(*reinterpret_cast<float*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *audio_player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *audio_player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12audio_playerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int audio_player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void audio_player::qml_positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void audio_player::sample_position_changed(size_t _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void audio_player::total_msChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void audio_player::decibelsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
