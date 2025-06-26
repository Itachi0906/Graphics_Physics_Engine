#pragma once
#ifndef GRICS_FORCE_GENERATOR_H
#define GRICS_FORCE_GENERATOR_H

#include "body.h"

namespace Grics {

	class ForceGenerator {
	public:
		virtual void updateForce(RigidBody* body, real dt) = 0;
	};

	/**
	* A force generator that applies a gravitational force. One instance 
	* can be used for multiple rigid bodies. 
	*/
	class Gravity : public ForceGenerator {
		/** Holds the acceleration due to gravity. */ 
		Vector3 gravity;
	public:
		/** Creates the generator with the given acceleration. */ 
		Gravity(const Vector3& gravity);
		/** Applies the gravitational force to the given rigid body. */ 
		virtual void updateForce(RigidBody* body, real dt);
	};


	/** 
	* A force generator that applies a Spring force. 
	*/
	class Spring : public ForceGenerator {
		/** The point of connection of the spring, in local coordinates. */
		Vector3 connectionPoint;

		/**
		* The point of connection of the spring to the other object,
		* in that object’s local coordinates.
		*/
		Vector3 otherConnectionPoint;

		/** The particle at the other end of the spring. */
		RigidBody* other;

		/** Holds the spring constant. */
		real springConstant;

		/** Holds the rest length of the spring. */
		real restLength;

	public:
		/** Creates a new spring with the given parameters. */
		Spring(const Vector3& localConnectionPt, RigidBody* other, const Vector3& otherConnectionPt, real springConstant, real restLength);

		/** Applies the spring force to the given particle. */
		virtual void updateForce(RigidBody* body, real dt);;
	};
}

#endif