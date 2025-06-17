#pragma once
#ifndef GRICS_PARTICLE_CONTACT_H
#define GRICS_PARTICLE_CONTACT_H

#include "Particle.h"

namespace Grics {
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

	protected:
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
}

#endif