#pragma once

#include <cinttypes>
#include <QObject>

namespace scoreboard
{

constexpr uint16_t port = 9233;

class Protocol : public QObject
{
    Q_OBJECT
public:
    enum DisplayRole
    {
        No,
        A1,
        A2,
        B1,
        B2
    };
    Q_ENUM(DisplayRole)

    enum Event
    {
        PlusA,
        PlusB
    };
    Q_ENUM(Event)

    enum ServerRequest
    {
        InfoDisplayRole,
        AcceptEvent,
        SetScore // ScoreA ScoreB SetA SetB Servis
    };
    Q_ENUM(ServerRequest)

    enum ServerIndication
    {
        InfoScore, // ScoreA ScoreB SetA SetB Servis
        SetMainDisplayText,
        SetMainDisplayTextColor,
        SetMainDisplayBgColor,
        SetAuxDisplayText,
        SetAuxDisplayTextColor,
        SetAuxDisplayBgColor,
        PlaySound
    };
};

}
