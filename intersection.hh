#ifndef INTERSECTION_HH
#define INTERSECTION_HH

#include <array>
#include <vector>

class Intersection
{
public:
    // The constructor is implicit to allow construction of Intersection from
    // an integer in TrackSystem. Otherwise one would have to call
    // Intersection(1) etc. which is just redundant.
    Intersection(const unsigned intersectNumber);

    unsigned getNumber() const;

    // The amount of Tracks connected to each Intersection
    static constexpr std::size_t TRACKS_IN_X = 3;

    // Gets adjacent track indices. The previously used track index is not
    // included, hence the -1.
    std::array<unsigned, TRACKS_IN_X - 1>
        getAdjacentIndxs(const unsigned prevTrackIndex) const;

private:
    // Stores the indices of the tracks of the intersection
    std::array<unsigned, TRACKS_IN_X> _trackIndxs;

    // Number of the Intersection and the index of the Intersection in
    // TrackSystem's array.
    unsigned _iNumber;
};

#endif // INTERSECTION_HH
