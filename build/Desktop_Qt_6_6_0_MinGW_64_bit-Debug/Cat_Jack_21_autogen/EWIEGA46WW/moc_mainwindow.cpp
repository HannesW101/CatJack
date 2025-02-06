/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "toggleMute",
    "",
    "addFrame",
    "closeFrame",
    "QFrame*",
    "frame",
    "playSound",
    "soundName",
    "startSinglePlayerGame",
    "handleAITurn_player1",
    "QLabel*",
    "player_card_1",
    "player_card_2",
    "player_score",
    "cardList",
    "handleAITurn_player3"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[34];
    char stringdata0[11];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[9];
    char stringdata4[11];
    char stringdata5[8];
    char stringdata6[6];
    char stringdata7[10];
    char stringdata8[10];
    char stringdata9[22];
    char stringdata10[21];
    char stringdata11[8];
    char stringdata12[14];
    char stringdata13[14];
    char stringdata14[13];
    char stringdata15[9];
    char stringdata16[21];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 10),  // "toggleMute"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 8),  // "addFrame"
        QT_MOC_LITERAL(32, 10),  // "closeFrame"
        QT_MOC_LITERAL(43, 7),  // "QFrame*"
        QT_MOC_LITERAL(51, 5),  // "frame"
        QT_MOC_LITERAL(57, 9),  // "playSound"
        QT_MOC_LITERAL(67, 9),  // "soundName"
        QT_MOC_LITERAL(77, 21),  // "startSinglePlayerGame"
        QT_MOC_LITERAL(99, 20),  // "handleAITurn_player1"
        QT_MOC_LITERAL(120, 7),  // "QLabel*"
        QT_MOC_LITERAL(128, 13),  // "player_card_1"
        QT_MOC_LITERAL(142, 13),  // "player_card_2"
        QT_MOC_LITERAL(156, 12),  // "player_score"
        QT_MOC_LITERAL(169, 8),  // "cardList"
        QT_MOC_LITERAL(178, 20)   // "handleAITurn_player3"
    },
    "MainWindow",
    "toggleMute",
    "",
    "addFrame",
    "closeFrame",
    "QFrame*",
    "frame",
    "playSound",
    "soundName",
    "startSinglePlayerGame",
    "handleAITurn_player1",
    "QLabel*",
    "player_card_1",
    "player_card_2",
    "player_score",
    "cardList",
    "handleAITurn_player3"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x08,    1 /* Private */,
       3,    0,   57,    2, 0x08,    2 /* Private */,
       4,    1,   58,    2, 0x08,    3 /* Private */,
       7,    1,   61,    2, 0x08,    5 /* Private */,
       9,    0,   64,    2, 0x08,    7 /* Private */,
      10,    4,   65,    2, 0x08,    8 /* Private */,
      16,    4,   74,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11, 0x80000000 | 11, QMetaType::QStringList,   12,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 11, 0x80000000 | 11, QMetaType::QStringList,   12,   13,   14,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'toggleMute'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'closeFrame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFrame *, std::false_type>,
        // method 'playSound'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'startSinglePlayerGame'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleAITurn_player1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        // method 'handleAITurn_player3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QLabel *, std::false_type>,
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->toggleMute(); break;
        case 1: _t->addFrame(); break;
        case 2: _t->closeFrame((*reinterpret_cast< std::add_pointer_t<QFrame*>>(_a[1]))); break;
        case 3: _t->playSound((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->startSinglePlayerGame(); break;
        case 5: _t->handleAITurn_player1((*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[4]))); break;
        case 6: _t->handleAITurn_player3((*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QLabel*>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QFrame* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLabel* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
            case 1:
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QLabel* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
