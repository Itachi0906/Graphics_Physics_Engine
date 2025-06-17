#include "ParticleContacts.h"

using namespace Grics;

void ParticleContact::resolve(real dt)
{
	resolveVelocity(dt);
	resolveInterpenetration(dt);
}

real ParticleContact::calculateSeparatingVelocity() const
{
	Vector3 relativeVelocity = particle[0]->getVelocity();
	if (particle[1]) relativeVelocity -= particle[1]->getVelocity();
	return relativeVelocity * contactNormal;
}

void ParticleContact::resolveVelocity(real dt)
{
	real separatingVelocity = calculateSeparatingVelocity();

	if (separatingVelocity > 0)
	{
		return;
	}

	real newSepVelocity = -separatingVelocity * restitution;

	real deltaVelocity = newSepVelocity - separatingVelocity;

	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	if (totalInverseMass <= 0) return;

	real impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerUnitMass = contactNormal * impulse;

	particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerUnitMass * particle[0]->getInverseMass());
	if (particle[1]) particle[1]->setVelocity(particle[1]->getVelocity() + impulsePerUnitMass * -particle[1]->getInverseMass());

}

void Grics::ParticleContact::resolveInterpenetration(real dt)
{
	if (penetration <= 0) return;

	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	if (totalInverseMass <= 0) return;

	Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	particle[0]->setPosition(particle[0]->getPosition() + movePerIMass * particle[0]->getInverseMass());
	if (particle[1]) particle[1]->setPosition(particle[1]->getPosition() + movePerIMass * particle[1]->getInverseMass());
}
