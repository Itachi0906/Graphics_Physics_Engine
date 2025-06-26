#include "ParticleWorld.h"

using namespace Grics;

void ParticleWorld::startFrame()
{
	for (Particles::iterator i = particles.begin(); i != particles.end(); i++)
	{
		(*i)->clearAccumulator();
	}
}

ParticleWorld::Particles& ParticleWorld::getParticles()
{
	return particles;
}

ParticleWorld::ContactGenerators& ParticleWorld::getContactGenerators()
{
	return contactGenerators;
}

ParticleForceRegistry& ParticleWorld::getForceRegistry()
{
	return registry;
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

	for (ContactGenerators::iterator g = contactGenerators.begin();
		g != contactGenerators.end();
		g++)
	{
		unsigned used = (*g)->addContact(nextContact, limit);
		limit -= used;
		nextContact += used;

		// We've run out of contacts to fill. This means we're missing
		// contacts.
		if (limit <= 0) break;
	}

	return maxContacts - limit;
}

void ParticleWorld::integrate(real dt)
{
	for (Particles::iterator p = particles.begin();
		p != particles.end();
		p++)
	{
		// Remove all forces from the accumulator
		(*p)->integrate(dt);
	}
}

void ParticleWorld::runPhysics(real dt)
{
	registry.updateForces(dt);

	integrate(dt);

	unsigned usedContacts = generateContacts();

	if (usedContacts)
	{
		if (calculateIterations) resolver.setIterations(std::min(usedContacts * 2,(unsigned int)10));
		resolver.resolveContacts(contacts, usedContacts, dt);
	}
}

ParticleCubeContactGenerator::ParticleCubeContactGenerator(Grics::Particle* p1, Grics::Particle* p2, real cubeSize)
	: p1(p1), p2(p2), halfExtent(cubeSize / 2.0f) {
}

unsigned ParticleCubeContactGenerator::addContact(ParticleContact* contact, unsigned limit) const {
	if (limit == 0) return 0;

	Vector3 pos1 = p1->getPosition();
	Vector3 pos2 = p2->getPosition();

	Vector3 delta = pos1 - pos2;
	Vector3 overlap;

	overlap.x = halfExtent * 2 - std::abs(delta.x);
	overlap.y = halfExtent * 2 - std::abs(delta.y);
	overlap.z = halfExtent * 2 - std::abs(delta.z);

	if (overlap.x < 0 || overlap.y < 0 || overlap.z < 0)
		return 0; // No collision

	// Collision occurred — find axis of least penetration
	float minPenetration = overlap.x;
	Vector3 normal((delta.x > 0) ? 1 : -1, 0, 0);

	if (overlap.y < minPenetration) {
		minPenetration = overlap.y;
		normal = Vector3(0, (delta.y > 0) ? 1 : -1, 0);
	}

	if (overlap.z < minPenetration) {
		minPenetration = overlap.z;
		normal = Vector3(0, 0, (delta.z > 0) ? 1 : -1);
	}
	const real slop = 0.01f;

	//std::cout << "Contact generated! penetration = " << contact->penetration << "\n";

	contact->particle[0] = p1;
	contact->particle[1] = p2;
	contact->contactNormal = normal;
	contact->penetration = std::max(minPenetration - slop , (real)0.0f);
	contact->restitution = 1.0f; // Tunable bounce factor

	

	return 1;
}