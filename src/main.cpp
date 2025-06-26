#include <iostream>
#include "World.h"
#include "body.h"
#include "Contacts.h"
#include "CollideFine.h"



int main() {
    using namespace Grics;

    // Create a physics world
    World world(10, 10); // max 10 contacts, 10 iterations

    // Create a rigid body
    RigidBody* body = new RigidBody();
    body->setMass(1.0f);
    body->setPosition(0.0f, 10.0f, 0.0f); // Start above the ground
    body->setVelocity(0.0f, 0.0f, 0.0f);
    body->setAccleration(Vector3(0.0f, -9.8f, 0.0f));
    body->setCanSleep(false);
    body->setAwake(true);
    body->setLinearDamping(0.99);
    body->setAngularDamping(0.99);

    Matrix3 it(1,0,0,0,1,0,0,0,1);
    body->setInertiaTensor(it);
    body->calculateDerivedData();

    world.getRigidBodies().push_back(body);
    GroundContactGenerator groundGen(body,0);

    world.getContactGenerators().push_back(&groundGen);

    // Run simulation for 5 seconds
    const float dt = 0.02f;
    for (int step = 0; step < 250; ++step) {
        world.startFrame();
        world.runPhysics(dt);

        Vector3 pos = body->getPosition();
        std::cout << "Time: " << step * dt << "s, Pos: ("
            << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    }

    delete body;
    return 0;
}
