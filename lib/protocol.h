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
        non,
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

    enum class DisplayFeature
    {
        non = 0,
        PlaySound = 0x01,
        Feature1 = 0x02
    };
    Q_FLAG(DisplayFeature)
    Q_DECLARE_FLAGS(DisplayFeatures, DisplayFeature)

    enum class ServerRequest
    {
        InfoDisplayRole,
        InfoFeatures,
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

Q_DECLARE_OPERATORS_FOR_FLAGS(Protocol::DisplayFeatures)

}
