#include "track.hh"

#include <stdexcept>

Track::Track(const unsigned trackNumber) :
    _tNumber(trackNumber),
    _trackMutex(),
    _startEnd(initIntersects(trackNumber))
{}

unsigned Track::getNextIntersect(const unsigned leftIntersect) const
{
    if (_startEnd.second == leftIntersect) return _startEnd.first;
    else return _startEnd.second;
}

std::pair<bool, std::unique_lock<std::timed_mutex>>
    Track::tryToLockForHs()
{
    using namespace std::chrono_literals;

    std::unique_lock<std::timed_mutex> tLock(_trackMutex, 0.5s);

    if (tLock.owns_lock()) return {true, std::move(tLock)};
    else return {false, std::move(tLock)};
}

unsigned Track::getTNumber() const
{
    return _tNumber;
}

Track::interPair Track::initIntersects(const unsigned trackNumber)
{
    // Track has 2 distinct intersections connected to it. First is always
    // the starting point
    switch (trackNumber)
    {
        case 0: return {0, 2};
        case 1: return {0, 1};
        case 2: return {1, 2};
        case 3: return {3, 0};
        case 4: return {1, 3};
        case 5: return {2, 3};
        default: throw std::runtime_error("Invalid track number");
    }
}
