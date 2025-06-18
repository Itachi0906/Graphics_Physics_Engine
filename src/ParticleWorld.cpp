#include "ParticleWorld.h"

using namespace Grics;

void ParticleWorld::startFrame()
{
	particleRegistration* reg = firstParticle;
	while (reg)
	{
		// Remove all the forces from the accumualator
		reg->particle->clearAccumulator();

		// Get to the next registration 
		reg = reg->next;
	}
}

Grics::ParticleWorld::ParticleWorld(unsigned maxContacts, unsigned iterations):
	resolver(iterations),
	maxContacts(maxContacts)
{
	contacts = new ParticleContact[maxContacts];
	calculateIterations = (iterations == 0);
}

unsigned ParticleWorld::generateContacts()
{
	unsigned limit = maxContacts;
	ParticleContact* nextContact = contacts;

	contactGenRegistration* reg = firstContactGen;
	while (reg)
	{
		unsigned used = reg->gen->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		if (limit <= 0) break;

		reg = reg->next;
	}

	return maxContacts - limit;
}

void ParticleWorld::integrate(real dt)
{
	particleRegistration* reg = firstParticle;
	while (reg)
	{
		reg->particle->update(dt);

		reg = reg->next;
	}
}

void ParticleWorld::runPhysics(real dt)
{
	registry.updateForces(dt);

	integrate(dt);

	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		if (calculateIterations) resolver.setIterations(usedContacts * 2);
		resolver.resolveContacts(contacts, usedContacts, dt);
	}
}
