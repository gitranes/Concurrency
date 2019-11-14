#ifndef TRACKSYSTEM_HH
#define TRACKSYSTEM_HH

#include "intersection.hh"
#include "track.hh"

#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include <random>
#include <atomic>


class TrackSystem
{
public:
    // Constructs trainAmount of threads running the train-method.
    explicit TrackSystem(const unsigned trainAmount);

    // Destructor sets the shutdown to true and joins the threads created
    // in the constructor.
    ~TrackSystem();

private:
    // Amount of intersections and Tracks in the system.
    static constexpr std::size_t _INTR_AMOUNT = 4;
    static constexpr std::size_t _TRACK_AMOUNT = 6;

    // Random number generation
    inline static std::random_device _rd{};
    inline static std::mt19937 _rEng{_rd()};

    // Fixed-size datastructures for the Tracks and Intersections
    const std::array<Intersection, _INTR_AMOUNT> _intersects;
    std::array<Track, _TRACK_AMOUNT> _tracks;

    // Stores the threads (trains).
    std::vector<std::thread> _trains;

    /*
     * Mutexes for mutual exclusion / synchronization:
     *
     *  _printMutex - Synchronises the std::cout usage between threads.
     *                std::cout in itself is thread-safe but the prints
     *                could interlace.
     *
     * _rEngMutex - Mutual exclusion for random engine usage as only one
     *              random engine is shared between instances.
     */
    std::mutex _printMutex;
    std::mutex _rEngMutex;

    // Informs the threads when to shutdown (return from train-method). Is set
    // to true by the destructor.
    std::atomic<bool> _shutdown;

    // The mutex fetching in getFreeTrack is the only reason these two methods
    // cannot be const. The amount of mutual exclusion needed between different
    // threads in train-method is very little as mutex is the only shared
    // variable that is modified, the rest are only read.

    // The method the thread runs in, simulating a train.
    void train(const unsigned startIndx, const unsigned trainNumber);

    // Seeks an available track from the Intersection's tracks.
    std::pair<std::unique_lock<std::timed_mutex>, const Track&>
        getFreeTrack(const unsigned interIndx, const unsigned prevTrackIndx);
};

#endif // TRACKSYSTEM_HH
