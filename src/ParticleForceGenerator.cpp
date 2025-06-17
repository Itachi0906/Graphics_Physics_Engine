#include "ParticleForceGenerator.h"

using namespace Grics;

void ParticleForceRegistry::updateForces(real dt)
{
	Registry::iterator i = registrations.begin();
	for (i; i != registrations.end();i++)
	{
		i->fg->updateForce(i->particle, dt);
	}
}

ParticleGravity::ParticleGravity(const Vector3& gravity)
{
	this->gravity = gravity;
}

void ParticleGravity::updateForce(Particle* particle, real dt)
{
	if (!particle->hasFiniteMass()) return;

	particle->addForce(gravity * particle->getMass());
}

ParticleDrag::ParticleDrag(real k1, real k2)
{
	this->k1 = k1;
	this->k2 = k2;
}

void ParticleDrag::updateForce(Particle* particle, real duration)
{
	Vector3 force;
	force = particle->getVelocity();

	real dragCoeff = force.magnitude();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	force.normalize();
	force *= -dragCoeff;
	particle->addForce(force);
}

ParticleSpring::ParticleSpring(Particle* other, real springConstant, real restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleSpring::updateForce(Particle* particle, real dt)
{
	Vector3 force;
	force = particle->getPosition();
	force -= other->getPosition();

	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* anchor, real springConstant, real restLength)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, real dt)
{
	Vector3 force;
	force = particle->getPosition();
	force -= *anchor;

	real magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	force.normalize();
	force *= -magnitude;
	particle->addForce(force);

}

ParticleBungee::ParticleBungee(Particle* other, real springConstant, real restLength)
{
	this->other = other;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void ParticleBungee::updateForce(Particle* particle, real dt)
{
	Vector3 force;
	force = particle->getPosition();
	force -= other->getPosition();

	real magnitude = force.magnitude();
	if (magnitude < restLength)
		return;
	magnitude = real_abs(magnitude - restLength);
	force *= springConstant;

	force.normalize();
	force *= -magnitude;
	particle->addForce(force);
}
