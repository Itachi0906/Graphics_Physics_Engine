#include "ParticleContacts.h"

using namespace Grics;

void ParticleContact::resolve(real dt)
{
	resolveVelocity(dt);
	resolveInterpenetration(dt);
	//std::cout << "Resolving: " << particle[0] << " vs " << particle[1] << '\n';
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

	Vector3 accCausedVelocity = particle[0]->getAcceleration();
	if (particle[1]) accCausedVelocity -= particle[1]->getAcceleration();
	real accCausedSepVelocity = accCausedVelocity * contactNormal * dt;

	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += restitution * accCausedSepVelocity;

		if (newSepVelocity < 0) newSepVelocity = 0;
	}

	real deltaVelocity = newSepVelocity - separatingVelocity;

	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	if (totalInverseMass <= 0) return;

	real impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerUnitMass = contactNormal * impulse;

	particle[0]->setVelocity(particle[0]->getVelocity() + impulsePerUnitMass * particle[0]->getInverseMass());
	if (particle[1]) particle[1]->setVelocity(particle[1]->getVelocity() - impulsePerUnitMass * particle[1]->getInverseMass());

}

void Grics::ParticleContact::resolveInterpenetration(real dt)
{

	if (penetration <= 0) return;

	real totalInverseMass = particle[0]->getInverseMass();
	if (particle[1]) totalInverseMass += particle[1]->getInverseMass();

	if (totalInverseMass <= 0) return;

	Vector3 movePerIMass = contactNormal * (-penetration / totalInverseMass);

	particle[0]->setPosition(particle[0]->getPosition() + movePerIMass * particle[0]->getInverseMass());
	if (particle[1]) particle[1]->setPosition(particle[1]->getPosition() - movePerIMass * particle[1]->getInverseMass());
}

ParticleContactResolver::ParticleContactResolver(unsigned iterations)
{
	this->iterations = iterations;
	this->iterationsUsed = 0;
}

void ParticleContactResolver::setIterations(unsigned iterations)
{
	this->iterations = iterations;
}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, real dt)
{
	iterationsUsed = 0;
	while (iterationsUsed < iterations)
	{
		real max = 0;
		unsigned maxIndex = numContacts;
		for (unsigned i = 0;i < numContacts;i++)
		{
			real sepVelocity = contactArray[i].calculateSeparatingVelocity();
			if (sepVelocity < max)
			{
				max = sepVelocity;
				maxIndex = i;
			}
		}

		const real velocityEpsilon = 0.01f;

		// Check if no contacts need resolving
		if (maxIndex == numContacts || real_abs(max) < velocityEpsilon)
			break;

		contactArray[maxIndex].resolve(dt);
		iterationsUsed++;
	}
}
