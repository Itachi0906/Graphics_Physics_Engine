#pragma once
#ifndef GRICS_WORLD_H
#define GRICS_WORLD_H

#include "body.h"
#include "Contacts.h"
#include <vector>

namespace Grics {
    /**
     * The world represents an independent simulation of physics.  It
     * keeps track of a set of rigid bodies, and provides the means to
     * update them all.
     */
    class World
    {
    public:
        typedef std::vector<RigidBody* > RigidBodies;
        typedef std::vector<ContactGenerator*> ContactGenerators;
    private:
        // ... other World data as before ...
        /**
         * True if the world should calculate the number of iterations
         * to give the contact resolver at each frame.
         */
        bool calculateIterations;

        RigidBodies bodies;

        /**
         * Holds the resolver for sets of contacts.
         */
        ContactResolver resolver;

        ContactGenerators contactGenerator;

        /**
         * Holds an array of contacts, for filling by the contact
         * generators.
         */
        Contact* contacts;

        /**
         * Holds the maximum number of contacts allowed (i.e. the size
         * of the contacts array).
         */
        unsigned maxContacts;

    public:
        /**
         * Creates a new simulator that can handle up to the given
         * number of contacts per frame. You can also optionally give
         * a number of contact-resolution iterations to use. If you
         * don't give a number of iterations, then four times the
         * number of detected contacts will be used for each frame.
         */
        World(unsigned maxContacts, unsigned iterations = 0);
        ~World();

        /**
         * Calls each of the registered contact generators to report
         * their contacts. Returns the number of generated contacts.
         */
        unsigned generateContacts();

        /**
         * Processes all the physics for the world.
         */
        void runPhysics(real dt);

        /**
         * Initialises the world for a simulation frame. This clears
         * the force and torque accumulators for bodies in the
         * world. After calling this, the bodies can have their forces
         * and torques for this frame added.
         */
        void startFrame();

        RigidBodies& getRigidBodies();

        ContactGenerators& getContactGenerators();
    };
}

#endif