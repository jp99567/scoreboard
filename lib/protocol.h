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
    enum class DisplayRole
    {
        No,
        L1,
        L2,
        R1,
        R2
    };
    Q_ENUM(DisplayRole)

    enum class Event
    {
        PlusA,
        PlusB
    };
    Q_ENUM(Event)

    enum class Servis
    {
        non,
        A,
        B
    };
    Q_ENUM(Servis)

    enum class ServerRequest
    {
        InfoDisplayRole,
        AcceptEvent,
        SetScore // ScoreA ScoreB SetA SetB Servis
    };
    Q_ENUM(ServerRequest)

    enum class ServerIndication
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
