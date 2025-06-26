#pragma once
#ifndef GRICS_BODY_H
#define GRICS_BODY_H

#include "core.h"
#include <assert.h>

namespace Grics {
	class RigidBody {


		bool isAwake;

		bool canSleep;

		/** 
		* Holds the inverse of the mass of the rigid body. It is more 
		* useful to hold the inverse mass because integration is simpler, 
		* and because in real time simulation it is more useful to have 
		* bodies with infinite mass (immovable) than zero mass (completely 
		* unstable in numerical simulation).
		*/
		real inverseMass;
		
		/** 
		* Holds the linear position of the rigid body in world space. 
		*/
		Vector3 position;

		/** 
		* Holds the linear velocity of the rigid body in world space. 
		*/
		Vector3 velocity;


		/**
		* Holds the acceleration before the integration step
		*/
		Vector3 lastFrameAcceleration;
		/**
		* Holds the acceleration of the rigid body in world space 
		*/
		Vector3 acceleration;

		/** 
		* Holds the angular velocity, or rotation, or the rigid body 
		* in world space. 
		*/
		Vector3 rotation;

		/** 
		* Holds the angular orientation of the rigid body in world space. 
		*/
		Quaternion orientation;

		/**
		 * Holds the amount of damping applied to linear
		 * motion.  Damping is required to remove energy added
		 * through numerical instability in the integrator.
		 */
		real linearDamping;

		/**
		 * Holds the amount of damping applied to angular
		 * motion.  Damping is required to remove energy added
		 * through numerical instability in the integrator.
		 */
		real angularDamping;

		/**
		* Holds the accumulated force to be applied at the next
		* simulation iteration only. This value is zeroed at each
		* integration step.
		*/
		Vector3 forceAccum;

		/**
		* Holds the accumulated accumulated to be applied at the next
		* simulation iteration only. This value is zeroed at each
		* integration step.
		*/
		Vector3 torqueAccum;

		/** 
		* Holds a transform matrix for converting body space into world 
		* space and vice versa. This can be achieved by calling the 
		* getPointInSpace functions.
		*/
		Matrix4 transformMatrix;

		/** 
		* Holds the inverse of the body’s inertia tensor. The inertia 
		* tensor provided must not be degenerate (that would mean 
		* the body had zero inertia for spinning along one axis). 
		* As long as the tensor is finite, it will be invertible. 
		* The inverse tensor is used for similar reasons as those 
		* for the use of inverse mass. 
		* The inertia tensor, unlike the other variables that define * a rigid body, is given in body space. *
		*/
		Matrix3 inverseInertiaTensor;

		/**
		 * Holds the inverse inertia tensor of the body in world
		 * space. The inverse inertia tensor member is specified in
		 * the body's local space.
		 */
		Matrix3 inverseInertiaTensorWorld;

		real motion;


	public:
		/**
		* Calculates internal data from state data. This should be called
		* after the body’s state is altered directly (it is called
		* automatically during integration). If you change the body’s
		* state and then intend to integrate before querying any data
		* (such as the transform matrix), then you can omit this step.
		*/
		void calculateDerivedData();

		/**
		* Sets the inverse inertia tensor corresponding to the given inertia tensor
		*/
		void setInertiaTensor(Matrix3& inertiaTensor);

		/** 
		* Adds the given force to the center of mass of the rigid body. 
		* The force is expressed in world coordinates.
		*/
		void addForce(const Vector3& force);

		/**
		 * Integrates the rigid body forward in time by the given amount.
		 * This function uses a Newton-Euler integration method, which is a
		 * linear approximation to the correct integral. For this reason it
		 * may be inaccurate in some cases.
		 */
		void integrate(real dt);

		/**
		 * Returns true if the body is awake and responding to
		 * integration.
		 */
		bool getAwake() const
		{
			return isAwake;
		}

		void setAwake(const bool awake = true);

		/**
		* Clear all the accumulators 
		*/
		void clearAccumulators();

		/** 
		* Adds the given force to the given point on the rigid body. 
		* The direction of the force is given in world coordinates, 
		* but the application point is given in body space. This is 
		* useful for spring forces, or other forces fixed to the 
		* body. 
		*/
		void addForceAtBodyPoint(const Vector3& force, const Vector3& point);

		void addForceAtPoint(const Vector3& force, const Vector3& point);

		Vector3 getPointInLocalSpace(const Vector3& point) const;

		Vector3 getPointInWorldSpace(const Vector3& point) const;
		

		Vector3 getDirectionInLocalSpace(const Vector3& direction) const;
		

		Vector3 getDirectionInWorldSpace(const Vector3& direction) const;

		/* Setter methods */
		/**
		* Sets the position of this particle to the given vector
		*/
		void setPosition(const Vector3& position);
		void setPosition(const real x, const real y, const real z);
		
		/**
		* Sets the rotation of this particle to the given vector
		*/
		void setRotation(const Vector3& rotation);
		void setRotation(const real x, const real y, const real z);

		/**
		* Sets the orientation of this particle to the given quaternion
		*/
		void setOrientation(const Quaternion& q);
		void setOrientation(const real r, const real i, const real j, const real k);

		/**
		* Sets the velocity of this particle to the given vector
		*/
		void setVelocity(const Vector3& velocity);
		void setVelocity(const real x, const real y, const real z);

		/**
		* Sets the acceleration of this particle to the given vector
		*/
		void setAccleration(const Vector3& acceleration);
		void setAcceleration(const real x, const real y, const real z);

		/**
		* Sets the mass and inverse mass of this particle to the given value
		*/
		void setMass(const real mass);
		/**
		* Set Infinite mass of this particle
		*/
		void setInverseMass(const real inverseMass);
		
		void setInverseInertiaTensor(const Matrix3& inverseInertiaTensor);

		void setCanSleep(const bool canSleep);

		void setLinearDamping(const real linearDamping);

		void setAngularDamping(const real angularDamping);

		/*Getter methods*/
		/**
		* Returns the position of this particle
		*/
		Vector3 getPosition();
		void getPosition(Vector3 *pos);


		/**
		* Returns the rotation of this particle
		*/
		Vector3 getRotation();
		void getRotation(Vector3* rot);

		/**
		* Returns the rotation of this particle
		*/
		Quaternion getOrientation();
		void getOrientation(Quaternion* q);

		/**
		* Returns the velocity of this particle
		*/
		Vector3 getVelocity();
		void getVelocity(Vector3* vel);
		/**
		* Returns the last frame acceleration of this particle
		*/
		Vector3 getLastFrameAcceleration();
		void getLastFrameAcceleration(Vector3* acc);
		/**
		* Returns the acceleration of this particle
		*/
		Vector3 getAcceleration();
		void getAcceleration(Vector3* acc);
		/**
		* Returns the mass of this particle
		*/
		real getMass();
		/**
		* Returns the inverse Mass of this particle
		*/
		real getInverseMass();
		/**
		* Returns the inverse inertia tensor in world space 
		*/
		void getInverseInertiaTensorWorld(Matrix3* inverseInertiaTensor);
		Matrix3 getInverseInertiaTensorWorld();
		/**
		* Returns the inverse inertia tensor
		*/
		void getInverseInertiaTensor(Matrix3* inverseInertiaTensor);
		Matrix3 getInverseInertiaTensor();

		void getInertiaTensor(Matrix3* inertiaTensor) const;
		Matrix3 getInertiaTensor();

		void getInertiaTensorWorld(Matrix3* inertiaTensor) const;
		Matrix3 getInertiaTensorWorld() const;

		/**
		 * Fills the given matrix with a transformation representing
		 * the rigid body's position and orientation.
		 *
		 * @note Transforming a vector by this matrix turns it from
		 * the body's local space to world space.
		 */
		void getTransform(Matrix4* transform) const;

		/**
		 * Fills the given matrix data structure with a
		 * transformation representing the rigid body's position and
		 * orientation.
		 *
		 * @note Transforming a vector by this matrix turns it from
		 * the body's local space to world space.
		 */
		void getTransform(real matrix[16]) const;

		/**
		 * Gets a transformation representing the rigid body's
		 * position and orientation.
		 *
		 * @note Transforming a vector by this matrix turns it from
		 * the body's local space to world space.
		 */
		Matrix4 getTransform() const;

		real getLinearDamping();

		real getAngularDamping();

		/**
		* Returns whether the particle has infinte mass
		*/
		bool hasFiniteMass();

		void addVelocity(const Vector3& deltaVeloctiy);
		void addRotation(const Vector3& deltaRotation);
		

	};
}

#endif