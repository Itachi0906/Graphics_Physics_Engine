#pragma once
#ifndef GRICS_PARTICLE_CONTACT_H
#define GRICS_PARTICLE_CONTACT_H

#include "Particle.h"
#include <iostream>

namespace Grics {


	class ParticleContactResolver;


	/**
	* A contact represents two objects in contact (in this case
	* ParticleContact representing two particles). Resolving a
	* contact removes their interpenetration, and applies sufficient
	* impulse to keep them apart. Colliding bodies may also rebound
	* 
	* The contact has no callable functions, it just holds the
	* contact details. To resolve a set of contacts, use the particle
	* contact resolver class.
	*/
	class ParticleContact {

		friend class ParticleContactResolver;

	public:
		/**
		* Holds the particle inovolved in the contact. The second member of this 
		* can be null in case colliding with scenery
		*/
		Particle* particle[2];

		/**
		* Holds the normal restitution coefficient at the contact
		*/
		real restitution;

		/**
		* Holds the direction of the contact in world coordinates 
		*/
		Vector3 contactNormal;

		/**
		* Holds the depth of penetration at the contact
		*/
		real penetration;

	//protected:

		/**
		* Resolves this contact for both velocity and interpenetration
		*/
		void resolve(real dt);

		/**
		* Calculates the separating velocity at this contact
		*/
		real calculateSeparatingVelocity() const;
	private:

		/**
		* Handles the impulse calculations for this collission
		*/
		void resolveVelocity(real dt);

		/**
		* Handles the interpenetration resolution for this contact. 
		*/
		void resolveInterpenetration(real dt);
		 
	};


	class ParticleContactResolver 
	{
	protected:
		/**
		* Holds the number of iterations
		*/
		unsigned iterations;

		/**
		* This is a performance tracking value - we keep a record
		* of number of iterations used
		*/
		unsigned iterationsUsed;

	public:
		/**
		* Creates a new contact solver
		*/
		ParticleContactResolver(unsigned iterations);

		/** 
		* Sets the number of iterations that can be used. 
		*/
		void setIterations(unsigned iterations);


		/** 
		* Resolves a set of particle contacts for both penetration * and velocity.
		*/
		void resolveContacts(ParticleContact* contactArray, unsigned numContacts, real dt);
	};

	class ParticleContactGenerator {
	public:
		/** 
		* Fills the given contact structure with the generated 
		* contact. The contact pointer should point to the first 
		* available contact in a contact array, where limit is the 
		* maximum number of contacts in the array that can be written 
		* to. The method returns the number of contacts that have * been written.
		*/
		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const = 0;
	};
}

#endif