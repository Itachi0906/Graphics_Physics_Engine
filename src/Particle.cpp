#include "Particle.h"
#include <iostream>

namespace Grics{
    void Particle::addForce(const Vector3 &forceVector)
    {
        forceAccum += forceVector;
    }

    void Particle::update(real dt)
    {
        assert(dt > 0.0);
        /**
        * Using the equation of motion s = u.t + 1/2.a.t^2
        */
        position.addScaledVector(velocity, dt);
        position.addScaledVector(acceleration, dt * dt * 0.5);

        Vector3 resultingAcc = acceleration;
        resultingAcc.addScaledVector(forceAccum, inverseMass);
        velocity.addScaledVector(resultingAcc, dt);

       // velocity *= real_pow(damping, dt);

        clearAccumulator();
    }
    
    void Particle::clearAccumulator()
    {
        forceAccum.clear();
    }
    void Particle::setPosition(const Vector3& position)
    {
        this->position = position;
    }
    void Particle::setPosition(const real x, const real y, const real z)
    {
        this->position = Vector3(x, y, z);
    }
    void Particle::setScale(const Vector3& scale)
    {
        this->scale = scale;
    }
    void Particle::setScale(const real x, const real y, const real z)
    {
        this->scale = Vector3(x, y, z);
    }
    void Particle::setRotation(const Vector3& rotation)
    {
        this->rotation = rotation;
    }
    void Particle::setRotation(const real x, const real y, const real z)
    {
        this->rotation = Vector3(x, y, z);
    }
    void Particle::setVelocity(const Vector3& velocity)
    {
        this->velocity = velocity;
    }
    void Particle::setVelocity(const real x, const real y, const real z)
    {
        this->velocity = Vector3(x, y, z);
    }
    void Particle::setAccleration(const Vector3& acceleration)
    {
        this->acceleration = acceleration;
    }
    void Particle::setAcceleration(const real x, const real y, const real z)
    {
        this->acceleration = Vector3(x, y, z);
    }
    void Particle::setDamping(real damping)
    {
        this->damping = damping;
    }
    void Particle::setMass(real mass)
    {
        assert(mass != 0.0);

        this->inverseMass = ((real)1.0)/mass;
    }

    void Particle::setInverseMass(const real inverseMass)
    {
        this->inverseMass = inverseMass;
    }

    Vector3 Particle::getPosition()
    {
        return position;
    }
    Vector3 Particle::getScale()
    {
        return scale;
    }
    Vector3 Particle::getRotation()
    {
        return rotation;
    }
    Vector3 Particle::getVelocity()
    {
        return velocity;
    }
    Vector3 Particle::getAcceleration()
    {
        return acceleration;
    }
    real Particle::getDamping()
    {
        return damping;
    }
    real Particle::getMass()
    {
        if (inverseMass == 0)
            return REAL_MAX;
        else
            return ((real)1.0) / inverseMass;
    }
    real Particle::getInverseMass()
    {
        return inverseMass;
    }
    bool Particle::hasFiniteMass()
    {
        if(inverseMass == 0.0f)
            return true;
        return false;
    }
}