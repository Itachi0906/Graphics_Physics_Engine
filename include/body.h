#pragma once
#ifndef GRICS_BODY_H
#define GRICS_BODY_H

#include "core.h"
#include <assert.h>

namespace Grics {
	class RigidBody {
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
		* Holds the angular velocity, or rotation, or the rigid body 
		* in world space. 
		*/
		Vector3 rotation;

		/** 
		* Holds the angular orientation of the rigid body in world space. 
		*/
		Quaternion orientation;

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

	};
}

#endif