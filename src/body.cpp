#include "body.h"

using namespace Grics;

/** 
* Inline function that creates a transform matrix from a position 
* and orientation.
*/
static inline void _calculateTransformMatrix(Matrix4 &transformMatrix, const Vector3 &position, const Quaternion &orientation)
{
	transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
	transformMatrix.data[1] = 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k; 
	transformMatrix.data[2] = 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j; 
	transformMatrix.data[3] = position.x;
	transformMatrix.data[4] = 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k; 
	transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k; 
	transformMatrix.data[6] = 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i; 
	transformMatrix.data[7] = position.y;
	transformMatrix.data[8] = 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j;
	transformMatrix.data[9] = 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i; 
	transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j; 
	transformMatrix.data[11] = position.z;
}

static inline void _checkInverseInertiaTensor(const Matrix3& inverseInertiaTensor)
{
	assert(inverseInertiaTensor.data[0] != 0 || inverseInertiaTensor.data[1] != 0 || inverseInertiaTensor.data[2] != 0);
	assert(inverseInertiaTensor.data[3] != 0 || inverseInertiaTensor.data[4] != 0 || inverseInertiaTensor.data[5] != 0);
	assert(inverseInertiaTensor.data[6] != 0 || inverseInertiaTensor.data[7] != 0 || inverseInertiaTensor.data[8] != 0);
}

/** 
* Internal function to do an inertia tensor transform by a quaternion.
*/
static inline void _transformInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat)
{
	real t4 = rotmat.data[0] * iitBody.data[0] + rotmat.data[1] * iitBody.data[3] + rotmat.data[2] * iitBody.data[6];
	real t9 = rotmat.data[0] * iitBody.data[1] + rotmat.data[1] * iitBody.data[4] + rotmat.data[2] * iitBody.data[7];
	real t14 = rotmat.data[0] * iitBody.data[2] + rotmat.data[1] * iitBody.data[5] + rotmat.data[2] * iitBody.data[8];
	real t28 = rotmat.data[4] * iitBody.data[0] + rotmat.data[5] * iitBody.data[3] + rotmat.data[6] * iitBody.data[6];
	real t33 = rotmat.data[4] * iitBody.data[1] + rotmat.data[5] * iitBody.data[4] + rotmat.data[6] * iitBody.data[7];
	real t38 = rotmat.data[4] * iitBody.data[2] + rotmat.data[5] * iitBody.data[5] + rotmat.data[6] * iitBody.data[8];
	real t52 = rotmat.data[8] * iitBody.data[0] + rotmat.data[9] * iitBody.data[3] + rotmat.data[10] * iitBody.data[6];
	real t57 = rotmat.data[8] * iitBody.data[1] + rotmat.data[9] * iitBody.data[4] + rotmat.data[10] * iitBody.data[7];
	real t62 = rotmat.data[8] * iitBody.data[2] + rotmat.data[9] * iitBody.data[5] + rotmat.data[10] * iitBody.data[8];

	iitWorld.data[0] = t4 * rotmat.data[0] + t9 * rotmat.data[1] + t14 * rotmat.data[2]; 
	iitWorld.data[1] = t4 * rotmat.data[4] + t9 * rotmat.data[5] + t14 * rotmat.data[6]; 
	iitWorld.data[2] = t4 * rotmat.data[8] + t9 * rotmat.data[9] + t14 * rotmat.data[10]; 
	iitWorld.data[3] = t28 * rotmat.data[0] + t33 * rotmat.data[1] + t38 * rotmat.data[2];
	iitWorld.data[4] = t28 * rotmat.data[4] + t33 * rotmat.data[5] + t38 * rotmat.data[6]; 
	iitWorld.data[5] = t28 * rotmat.data[8] + t33 * rotmat.data[9] + t38 * rotmat.data[10]; 
	iitWorld.data[6] = t52 * rotmat.data[0] + t57 * rotmat.data[1] + t62 * rotmat.data[2];
	iitWorld.data[7] = t52 * rotmat.data[4] + t57 * rotmat.data[5] + t62 * rotmat.data[6];
	iitWorld.data[8] = t52 * rotmat.data[8] + t57 * rotmat.data[9] + t62 * rotmat.data[10];
}

void RigidBody::calculateDerivedData() {
	orientation.normalize();

	// Calculate the transform matrix for the body.
	_calculateTransformMatrix(transformMatrix, position, orientation);

	_transformInertiaTensor(inverseInertiaTensorWorld,
		orientation,
		inverseInertiaTensor,
		transformMatrix);
}

void RigidBody::setInertiaTensor(Matrix3& inertiaTensor)
{
	inverseInertiaTensor.setInverse(inertiaTensor);
	_checkInverseInertiaTensor(inverseInertiaTensor);
}

void RigidBody::setInverseInertiaTensor(const Matrix3& inverseInertiaTensor)
{
	_checkInverseInertiaTensor(inverseInertiaTensor);
	RigidBody::inverseInertiaTensor = inverseInertiaTensor;
}

void RigidBody::setCanSleep(const bool canSleep)
{
	this->canSleep = canSleep;

	if (!canSleep && !isAwake) setAwake();
}

void Grics::RigidBody::setLinearDamping(const real linearDamping)
{
	this->linearDamping = linearDamping;
}

void Grics::RigidBody::setAngularDamping(const real angularDamping)
{
	this->angularDamping = angularDamping;
}

Vector3 RigidBody::getPointInLocalSpace(const Vector3& point) const
{
	return transformMatrix.transformInverse(point);
}

Vector3 RigidBody::getPointInWorldSpace(const Vector3& point) const
{
	return transformMatrix.transform(point);
}

Vector3 RigidBody::getDirectionInLocalSpace(const Vector3& direction) const
{
	return transformMatrix.transformInverseDirection(direction);
}

Vector3 RigidBody::getDirectionInWorldSpace(const Vector3& direction) const
{
	return transformMatrix.transformDirection(direction);
}

void RigidBody::addForce(const Vector3& force)
{
	forceAccum += force;
}

void RigidBody::integrate(real dt)
{
	if (!isAwake) return;
	//Calculate linear Acceleration from force inputs
	lastFrameAcceleration = acceleration;
	lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);

	//Calculate angular acceleration from torque inputs
	Vector3 angularAcceleration = inverseInertiaTensor.transform(torqueAccum);

	// Adjust velocities.
	// Update linear velocity from both acceleration and impulse. 
	velocity.addScaledVector(lastFrameAcceleration, dt);

	// Update angular velocity from both acceleration and impulse. 
	rotation.addScaledVector(angularAcceleration, dt);

	// Impose drag. 
	velocity *= real_pow(linearDamping, dt); 
	rotation *= real_pow(angularDamping, dt);

	// Adjust positions
   // Update linear position.
	position.addScaledVector(velocity, dt);
	
	// Update angular position.
	orientation.addScaledVector(rotation, dt);

	// Normalise the orientation, and update the matrices with the new
	// position and orientation
	calculateDerivedData();

	//Clear Accumulators
	clearAccumulators();

	// Update the kinetic energy store, and possibly put the body to
	// sleep.
	if (canSleep) {
		real currentMotion = velocity.scalarProduct(velocity) +
			rotation.scalarProduct(rotation);

		real bias = real_pow(0.5, dt);
		motion = bias * motion + (1 - bias) * currentMotion;

		if (motion < sleepEpsilon) setAwake(false);
		else if (motion > 10 * sleepEpsilon) motion = 10 * sleepEpsilon;
	}

}

void Grics::RigidBody::setAwake(const bool awake)
{
	if (awake) {
		isAwake = true;

		// Add a bit of motion to avoid it falling asleep immediately.
		motion = sleepEpsilon * 2.0f;
	}
	else {
		isAwake = false;
		velocity.clear();
		rotation.clear();
	}
}

void RigidBody::clearAccumulators()
{
	forceAccum.clear();
	torqueAccum.clear();
}

void RigidBody::addForceAtBodyPoint(const Vector3& force, const Vector3& point)
{
	Vector3 pt = getPointInWorldSpace(pt);
	addForceAtPoint(force,pt);
}

void RigidBody::addForceAtPoint(const Vector3& force,const Vector3& point)
{
	// Convert to coordinates relative to center of mass.
	Vector3 pt = point;
	pt -= position;

	forceAccum += force;
	torqueAccum += pt % force;

	isAwake = true;
}

void RigidBody::setPosition(const Vector3& position)
{
    this->position = position;
}
void RigidBody::setPosition(const real x, const real y, const real z)
{
    this->position = Vector3(x, y, z);
}
void RigidBody::setRotation(const Vector3& rotation)
{
    this->rotation = rotation;
}
void RigidBody::setRotation(const real x, const real y, const real z)
{
    this->rotation = Vector3(x, y, z);
}
void RigidBody::setOrientation(const Quaternion& q)
{
	this->orientation = q;
}
void RigidBody::setOrientation(const real r, const real i, const real j, const real k)
{
	this->orientation = Quaternion(r, i, j, k);
}
void RigidBody::setVelocity(const Vector3& velocity)
{
    this->velocity = velocity;
}
void RigidBody::setVelocity(const real x, const real y, const real z)
{
    this->velocity = Vector3(x, y, z);
}

void RigidBody::setAccleration(const Vector3& acceleration)
{
    this->acceleration = acceleration;
}
void RigidBody::setAcceleration(const real x, const real y, const real z)
{
    this->acceleration = Vector3(x, y, z);
}

void RigidBody::setMass(real mass)
{
    assert(mass != 0.0);

    this->inverseMass = ((real)1.0) / mass;
}

void RigidBody::setInverseMass(const real inverseMass)
{
    this->inverseMass = inverseMass;
}

Vector3 RigidBody::getPosition()
{
    return position;
}
void RigidBody::getPosition(Vector3* pos)
{
	*pos = this->position;
}
Vector3 RigidBody::getRotation()
{
    return rotation;
}
void RigidBody::getRotation(Vector3* rot)
{
	*rot = this->rotation;
}
Quaternion RigidBody::getOrientation()
{
	return orientation;
}
void RigidBody::getOrientation(Quaternion* q)
{
	*q = this->orientation;
}
Vector3 RigidBody::getVelocity()
{
    return velocity;
}
void RigidBody::getVelocity(Vector3* vel)
{
	*vel = this->velocity;
}
Vector3 RigidBody::getLastFrameAcceleration()
{
	return lastFrameAcceleration;
}
void RigidBody::getLastFrameAcceleration(Vector3* acc)
{
	*acc = this->lastFrameAcceleration;
}
Vector3 RigidBody::getAcceleration()
{
    return acceleration;
}

void RigidBody::getAcceleration(Vector3* acc)
{
	*acc = this->acceleration;
}

real RigidBody::getMass()
{
    if (inverseMass == 0)
        return REAL_MAX;
    else
        return ((real)1.0) / inverseMass;
}
real RigidBody::getInverseMass()
{
    return inverseMass;
}
void RigidBody::getInverseInertiaTensorWorld(Matrix3* inverseInertiaTensor)
{
	*inverseInertiaTensor = inverseInertiaTensorWorld;
}
Matrix3 RigidBody::getInverseInertiaTensorWorld()
{
	return inverseInertiaTensorWorld;
}
void RigidBody::getInverseInertiaTensor(Matrix3* inverseInertiaTensor)
{
	*inverseInertiaTensor = this->inverseInertiaTensor;
}
Matrix3 RigidBody::getInverseInertiaTensor()
{
	return inverseInertiaTensor;
}
void RigidBody::getInertiaTensor(Matrix3* inertiaTensor) const
{
	return inertiaTensor->setInverse(inverseInertiaTensor);
}
Matrix3 RigidBody::getInertiaTensor()
{
	Matrix3 it;
	getInertiaTensor(&it);
	return it;
}

void RigidBody::getInertiaTensorWorld(Matrix3* inertiaTensor) const
{
	inertiaTensor->setInverse(inverseInertiaTensorWorld);
}

Matrix3 RigidBody::getInertiaTensorWorld() const
{
	Matrix3 it;
	getInertiaTensorWorld(&it);
	return it;
}

void RigidBody::getTransform(Matrix4* transform) const
{
	*transform = this->transformMatrix;
}

void RigidBody::getTransform(real matrix[16]) const
{
	matrix[0] = (real)transformMatrix.data[0];
	matrix[1] = (real)transformMatrix.data[4];
	matrix[2] = (real)transformMatrix.data[8];
	matrix[3] = 0;

	matrix[4] = (real)transformMatrix.data[1];
	matrix[5] = (real)transformMatrix.data[5];
	matrix[6] = (real)transformMatrix.data[9];
	matrix[7] = 0;

	matrix[8] = (real)transformMatrix.data[2];
	matrix[9] = (real)transformMatrix.data[6];
	matrix[10] = (real)transformMatrix.data[10];
	matrix[11] = 0;

	matrix[12] = (real)transformMatrix.data[3];
	matrix[13] = (real)transformMatrix.data[7];
	matrix[14] = (real)transformMatrix.data[11];
	matrix[15] = 1;
}

Matrix4 RigidBody::getTransform() const
{
	return transformMatrix;
}

real RigidBody::getLinearDamping()
{
	return linearDamping;
}

real RigidBody::getAngularDamping()
{
	return angularDamping;
}

bool RigidBody::hasFiniteMass()
{
	return inverseMass > 0.0f;
}

void RigidBody::addVelocity(const Vector3& deltaVeloctiy)
{
	velocity += deltaVeloctiy;
}

void RigidBody::addRotation(const Vector3& deltaRotation)
{
	rotation = deltaRotation;
}


