#ifndef TRACK_HH
#define TRACK_HH

#include <utility>
#include <mutex>

// Forward declaration
class Intersection;

class Track
{
public:

    // Constructor allows implicit construction from an integer for the same
    // reason as Intersection.
    Track(const unsigned trackNumber);

    // Returns the index of next Intersection in a Track.
    unsigned getNextIntersect(const unsigned leftIntersect) const;

    // Tries to lock the underlying _trackMutex for 0.5s. bool tells if the
    // lock was acquired or not.
    std::pair<bool, std::unique_lock<std::timed_mutex>> tryToLockForHs();

    // Returns the number
    unsigned getTNumber() const;

private: 
    // Number of the track. Also represents the index of the Track in
    // TrackSystem's array.
    const unsigned _tNumber;

    // Mutex for the locking the Track. Timed mutex is used to limit the
    // amount of loops getFreeTrack does.
    std::timed_mutex _trackMutex;

    // Type for the index pair.
    using interPair = std::pair<unsigned, unsigned>;

    // Indices of the Intersections in TrackSystem array
    const interPair _startEnd;

    // Initialises the InterPair for the Intersection
    static interPair initIntersects(const unsigned trackNumber);
};

#endif // TRACK_HH
