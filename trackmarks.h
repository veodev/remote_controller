#ifndef TRACKMARKS_H
#define TRACKMARKS_H
#include <QString>

enum Direction
{
    UnknownDirection,
    ForwardDirection = 1,
    BackwardDirection = -1
};

enum ViewCoordinate: char{
    KmPkM,
    KmCommaM,
    Hectometer,
    MeterOnly,
};

class TrackMarks
{
protected:
    int _centimeter;
    int _meter;
    Direction _direction;

public:
    TrackMarks();
    virtual ~TrackMarks();
    virtual QString getString() = 0;
    virtual QString getHmString() = 0;
    virtual QString getKmString() = 0;
    virtual QString getMeterString() = 0;
    virtual int getTotalMeter();
    virtual void addMeter(int meter);
    virtual void addCentimeter(int cm);
    virtual void syncPole(int a, int b) = 0;
    virtual void reset() = 0;
    Direction getDirection() const;
    void setDirection(const Direction& direction);
};

#endif  // TRACKMARKS_H
