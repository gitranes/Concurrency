#include "tracksystem.hh"

#include <functional>
#include <chrono>
#include <stdexcept>
#include <iostream>

TrackSystem::TrackSystem(const unsigned trainAmount) :

    // Intersects and Tracks are implicitly constructed from the integers.
    _intersects{0, 1, 2, 3},
    _tracks{0, 1, 2, 3, 4, 5},

    _trains(),
    _printMutex(),
    _shutdown(false)

{
    std::uniform_int_distribution<unsigned> distr(0, _INTR_AMOUNT - 1);

    // Initialise trainAmount of threads to run the train-method. The start
    // Intersection is chosen randomly.
    for (unsigned i = 0; i < trainAmount; ++i)
    {
        unsigned startIntr;
        {
            std::scoped_lock<std::mutex> rEngLock(_rEngMutex);
            startIntr = distr(_rEng);
        } // rEngLock

        _trains.emplace_back(
                    std::bind(&TrackSystem::train, this, startIntr, i));
    }
}

TrackSystem::~TrackSystem()
{
    // Writing std::atomic when it is being read is well-defined.
    _shutdown = true;

    {
        std::scoped_lock<std::mutex> printLock(_printMutex);
        std::cout << "\n***Waiting for threads to stop...***" << std::endl;
    } // printLock

    // Join threads
    for (std::thread &t : _trains)
    {
        t.join();
    }
}

std::pair<std::unique_lock<std::timed_mutex>, const Track&>
    TrackSystem::getFreeTrack(const unsigned interIndx,
                              const unsigned prevTrackIndx)
{
    const auto &trackIndices =
            _intersects[interIndx].getAdjacentIndxs(prevTrackIndx);
    std::uniform_int_distribution<unsigned>
            distr(0, Intersection::TRACKS_IN_X - 2);

    unsigned adjacentIndx;
    {
        // Start from a 0 or 1
        std::scoped_lock<std::mutex> rEngLock(_rEngMutex);
        adjacentIndx = distr(_rEng);
    } // rEngLock

    while (true)
    {
        // Get the actual Track index
        unsigned trackIndx = trackIndices[adjacentIndx];
        Track& track = _tracks[trackIndx];

        // Tries to lock track for 0.5s. trackLock is move-constructed.
        auto [ownsLock, trackLock] = track.tryToLockForHs();

        if (!ownsLock)
        {
            // lock failed. Try the other index. 0 -> 1, 1 -> 0.
            adjacentIndx = (adjacentIndx ? 0 : 1);
            continue;
        }
        else {
            return {std::move(trackLock), track};
        }
    }
}

void TrackSystem::train(const unsigned startIndx, const unsigned trainNumber)
{
    using lockAndPtr =
        std::pair<std::unique_lock<std::timed_mutex>, const Track&>;

    const Intersection* interS = &_intersects[startIndx];
    unsigned nextInterIndx;

    // First time, usedTrackIndx is unknown
    unsigned usedTrackIndx = Intersection::TRACKS_IN_X + 99;

    while (true)
    {
        // Read the atomic bool
        if (_shutdown) return;

        {
            unsigned currInterIndx = interS->getNumber();

            // Move construct the acquired lock and the ptr
            lockAndPtr pair(getFreeTrack(currInterIndx, usedTrackIndx));
            const Track& nextTrack = pair.second;

            usedTrackIndx = nextTrack.getTNumber();
            nextInterIndx = nextTrack.getNextIntersect(currInterIndx);

            {
                std::scoped_lock<std::mutex> printLock(_printMutex);
                std::cout << " Train number " << trainNumber
                          << " departed from \n \t Intersection " << currInterIndx
                          << " towards Intersection " << nextInterIndx
                          << " \n \t on track " << usedTrackIndx
                          << std::endl;
            } // printLock

            using namespace std::chrono_literals;

            // Simulate a moving train.
            std::this_thread::sleep_for(4s);
        } // pair.first lock

        // Train has arrived
        interS = &_intersects[nextInterIndx];
    }
}

