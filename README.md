# Concurrency
Concurrency course voluntary project. 

The project's assigment was to represent a train track system using C++'s concurrency.

## Problem description

![alt text](http://www.cs.tut.fi/~rinn/htyo/design/pics/railroad.png)

The picture contains a railroad network and the task is to design simulation software, which runs 3-5 trains in this network.

Fixed point in design is that each train (locomotive) will be modelled by a thread of execution. The main objective of the project is to synchronise and manage the trains in such a way that, during the simulation, no collision or deadlock can occur.

### Boundary conditions
- Each track section is ONE WAY at a time: when a train has started going into one direction, there can not be any other trains going in opposite direction (note: this allows multiple trains in the SAME direction).
- When train has started on a section it SHOULD run to next section/junction (in other words: no U-turn in the middle of track section)
- Trains do NOT have to have a specific schedule (their overall movements can be random in the whole track-network).
- The track exchange/switching locations can be "magic" in the sense that any number of trains can be on them (waiting), all trains can pass each other to any direction inside them.

## Short description of the solution

Every Track seen in the picture (part that's seperated by 2 intersections) has a mutex in the program. This ensures that only one Train (thread) can ride the Track at a time (in this solution multiple trains cannot move in the SAME direction). After reaching an Intersection the Train releases the Track lock and starts to seek a free mutex (free Track). If a lock is succesful the Train will travel the Track towards the next Intersection.
