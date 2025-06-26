#include <cstdlib>
#include <World.h>

using namespace Grics;

World::World(unsigned maxContacts, unsigned iterations)
    :
    resolver(iterations),
    maxContacts(maxContacts)
{
    contacts = new Contact[maxContacts];
    calculateIterations = (iterations == 0);
}

World::~World()
{
    delete[] contacts;
}

void World::startFrame()
{

    for (RigidBodies::iterator i = bodies.begin(); i != bodies.end();i++)
    {
        (*i)->clearAccumulators();
        (*i)->calculateDerivedData();
    }
}

World::RigidBodies& World::getRigidBodies()
{
    return bodies;
}

World::ContactGenerators& World::getContactGenerators()
{
    return contactGenerator;
}

unsigned World::generateContacts()
{
    unsigned limit = maxContacts;
    Contact* nextContact = contacts;

    for (ContactGenerators::iterator i = contactGenerator.begin(); i != contactGenerator.end();i++)
    {
        unsigned used = (*i)->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // We've run out of contacts to fill. This means we're missing
        // contacts.
        if (limit <= 0) break;
    }

    // Return the number of contacts used.
    return maxContacts - limit;
}

void World::runPhysics(real dt)
{
    // First apply the force generators
    //registry.updateForces(duration);

    for (RigidBodies::iterator i = bodies.begin(); i != bodies.end();i++)
    {
        (*i)->integrate(dt);

    }

    // Generate contacts
    unsigned usedContacts = generateContacts();

    // And process them
    if (calculateIterations) resolver.setIterations(usedContacts * 4);
    resolver.resolveContacts(contacts, usedContacts, dt);
}