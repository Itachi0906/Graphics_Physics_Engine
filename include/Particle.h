#pragma once
#ifndef GRICS_PARTICLE_H
#define GRICS_PARTICLE_H

#include "core.h"
#include "Mesh.h"
#include <vector>
#include <cassert>

namespace Grics {
    /**
     * A particle is the simplest object that can be simulated in this
     * physics engine
     */
    class Particle
    {
    private:
        Vector3 gravity = Vector3(0.0f,-9.81f,0.0f);

        /**
         * Holds the linear position of this particle in
         * world space
         */
        Vector3 position;

        /**
        * Holds the scale of this particle in world space
        */
        Vector3 scale;
        
        /**
        * Holds the rotation of this particle in world space
        */
        Vector3 rotation;

        /**
         * Holds the linear velocity of this particle in
         * world space
         */
        Vector3 velocity;

        /**
        * Holds the acceleration of this particle. This value
        * can be used to set acceleration due to gravity (its primary
        * use) or any other constant acceleration.
        */
        Vector3 acceleration;

        /**
         * Holds the amount of damping applied to linear
         * motion. Damping is required to remove energy added
         * through numerical instability in the integrator
         */
        real damping;


        /**
         * holds the inverse mass of the particle. It
         * is more useful to hold the inverse because
         * integration is simpler and because in real-time
         * simulation it is more useful to have objects with
         * infinite mass (immovable) than zero mass
         * (completely unstable in numerical calculations)
         */
        real inverseMass;

        /**
        * Holds the accumulated force to be applied at the next
        * simulation iteration only. This value is zeroed at each
        * integration step.
        */
        Vector3 forceAccum;

        void updateGeometry(Shader &shader);
    public:


        /** 
        * the force to apply 
        */
        void addForce(const Vector3& forceVector);

        /**
        * Integrates the particle forward in time by the given amount.
        * This function uses a Newton-Euler integration method, which is a
        * linear approximation of the correct integral. For this reason it
        * may be inaccurate in some cases.
        */
        void update(real dt);
        void clearAccumulator();
        void drawParticle(Mesh m,Mesh::shapeType shape,Shader &shader);
        
        /* Setter methods */
        /**
        * Sets the position of this particle to the given vector
        */
        void setPosition(const Vector3& position);
        void setPosition(const real x, const real y, const real z);
        /**
        * Sets the scale of this particle to the given vector
        */
        void setScale(const Vector3& scale);
        void setScale(const real x, const real y, const real z);
        /**
        * Sets the rotation of this particle to the given vector
        */
        void setRotation(const Vector3& rotation);
        void setRotation(const real x, const real y, const real z);
        /**
        * Sets the velocity of this particle to the given vector
        */
        void setVelocity(const Vector3& velocity);
        void setVelocity(const real x, const real y, const real z);
        /**
        * Sets the acceleration of this particle to the given vector
        */
        void setAccleration(const Vector3& acceleration);
        void setAcceleration(const real x, const real y, const real z);
        /**
        * Sets the Damping of this particle to the given value
        */
        void setDamping(const real damping);
        /**
        * Sets the mass and inverse mass of this particle to the given value
        */
        void setMass(const real mass);
        /**
        * Set Infinite mass of this particle
        */
        void setInverseMass(const real inverseMass);


        /*Getter methods*/
        /**
        * Returns the position of this particle
        */
        Vector3 getPosition();
        /**
        * Returns the scale of this particle
        */
        Vector3 getScale();
        /**
        * Returns the rotation of this particle
        */
        Vector3 getRotation();
        /**
        * Returns the velocity of this particle 
        */
        Vector3 getVelocity();
        /**
        * Returns the acceleration of this particle
        */
        Vector3 getAcceleration();
        /**
        * Returns the damping applied on this particle
        */
        real getDamping();
        /**
        * Returns the mass of this particle
        */
        real getMass();
        /**
        * Returns the inverse Mass of this particle
        */
        real getInverseMass();
        
        
        /**
        * Returns whether the particle has infinte mass
        */
        bool hasFiniteMass();
    };
}


#endif