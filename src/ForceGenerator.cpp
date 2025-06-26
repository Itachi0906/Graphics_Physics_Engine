#include "ForceGenerator.h"

using namespace Grics;

Gravity::Gravity(const Vector3& gravity)
{
	this->gravity = gravity;
}

void Gravity::updateForce(RigidBody* body, real dt)
{
	if (!body->hasFiniteMass())
		return;

	body->addForce(gravity * body->getMass());
}

Spring::Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength)
{
	this->connectionPoint = localConnectionPt;
	this->other = other;
	this->otherConnectionPoint = otherConnectionPt;
	this->springConstant = springConstant;
	this->restLength = restLength;
}

void Spring::updateForce(RigidBody* body, real dt)
{
	// Calculate the two ends in world space. 
	Vector3 lws = body->getPointInWorldSpace(connectionPoint); 
	Vector3 ows = other->getPointInWorldSpace(otherConnectionPoint);

	// Calculate the vector of the spring. 
	Vector3 force = lws - ows;

	// Calculate the magnitude of the force. 
	real magnitude = force.magnitude(); 
	magnitude = real_abs(magnitude - restLength); 
	magnitude *= springConstant;

	// Calculate the final force and apply it. 
	force.normalize(); 
	force *= -magnitude; 
	body->addForceAtPoint(force, lws);
}
