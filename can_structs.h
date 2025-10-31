#ifndef CAN_STRUCTS_H
#define CAN_STRUCTS_H

#include <QObject>
#include <QVector>
#include <stdint.h>
#include <QCanBusFrame>

//Now inherits from the built-in CAN frame class from Qt. This should be more future proof and easier to integrate with other code

struct CANFrame : public QCanBusFrame
{
public:
    int bus;
    bool isReceived; //did we receive this or send it?
    uint64_t timedelta;
    uint32_t frameCount; //used in overwrite mode

    friend bool operator<(const CANFrame& l, const CANFrame& r)
    {
        uint64_t lStamp = l.fullTimeStamp();
        uint64_t rStamp = r.fullTimeStamp();
        return lStamp < rStamp;
    }

    CANFrame()
    {
        setFrameId(0);
        bus = 0;
        setExtendedFrameFormat(false);
        setFrameType(QCanBusFrame::DataFrame);
        isReceived = true;
        timedelta = 0;
        frameCount = 1;
    }

    uint64_t fullTimeStamp() const
    {
        return static_cast<uint64_t>(this->timeStamp().seconds()) * 1'000'000ULL
             + static_cast<uint64_t>(this->timeStamp().microSeconds());
    }
};

class CANFltObserver
{
public:
    quint32 id;
    quint32 mask;
    QObject * observer; //used to target the specific object that setup this filter

    bool operator ==(const CANFltObserver &b) const
    {
        if ( (id == b.id) && (mask == b.mask) && (observer == b.observer) ) return true;

        return false;
    }
};

#endif // CAN_STRUCTS_H

