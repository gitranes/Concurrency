#include "intersection.hh"

#include <stdexcept>
#include <algorithm>

Intersection::Intersection(const unsigned intersectNumber) :
    _iNumber(intersectNumber)
{
    // Intersection has 3 distinct tracks connected to it
    switch (_iNumber)
    {
        case 0: _trackIndxs = {0, 1, 3};
                break;
        case 1: _trackIndxs = {1, 2, 4};
                break;
        case 2: _trackIndxs = {0, 2, 5};
                break;
        case 3: _trackIndxs = {3, 4, 5};
                break;
        default: throw std::runtime_error("Invalid Intersect number");
    }
}

unsigned Intersection::getNumber() const
{
    return _iNumber;
}

std::array<unsigned, Intersection::TRACKS_IN_X - 1>
    Intersection::getAdjacentIndxs(const unsigned prevTrackIndex) const
{
    // Copy will have one less index.
    std::array<unsigned, TRACKS_IN_X - 1> adjacentIndices;

    // Copy all expect the prevTrackIndex. const_iterator is used for the
    // shared container, so this is thread-safe. Writes are performed only
    // on the thread-local adjacentIndices.
    std::copy_if(_trackIndxs.cbegin(), _trackIndxs.cend(),
                 adjacentIndices.begin(),
                 [prevTrackIndex](const unsigned indx)
    {
        return prevTrackIndex != indx;
    });

    return adjacentIndices;
}
