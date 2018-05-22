#include "trackmarks.h"


Direction TrackMarks::getDirection() const
{
    return _direction;
}

void TrackMarks::setDirection(const Direction& direction)
{
    _direction = direction;
}

TrackMarks::TrackMarks()
{
    _meter = 0;
    _centimeter = 0;
    _direction = ForwardDirection;
}

TrackMarks::~TrackMarks()
{
}

void TrackMarks::addMeter(int meter)
{
    if (_direction == ForwardDirection) {
        _meter += meter;
    }
    else if (_direction == BackwardDirection) {
        _meter -= meter;
    }
    else {
        _meter += meter;
    }
}
