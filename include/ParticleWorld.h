#pragma once

#ifndef GRICS_PHYSICS_WORLD_H
#define GRICS_PHYSICS_WORLD_H

#include "ParticleForceGenerator.h"
#include "ParticleContacts.h"
namespace Grics {
	/** 
	* Keeps track of a set of particles, and provides the means to * update them all. 
	*/
	class ParticleWorld {
	public:

		typedef std::vector<Particle*> Particles;
		typedef std::vector<ParticleContactGenerator*> ContactGenerators;
	protected:


		bool calculateIterations;
		

		ParticleForceRegistry registry;
		
		ParticleContactResolver resolver;

		
		Particles particles;

		ContactGenerators contactGenerators;

		/** 
		* Holds the list of contacts. 
		*/
		ParticleContact* contacts;

		/** 
		* Holds the maximum number of contacts allowed (i.e., the 
		* size of the contacts array). 
		*/
		unsigned maxContacts;

	public: 
		/**
		* Creates a new particle simulator that can handle up to the
		* given number of contacts per frame. You can also optionally
		* give a number of contact-resolution iterations to use. If you
		* don’t give a number of iterations, then twice the number of
		* contacts will be used.
		*/
		ParticleWorld(unsigned maxContacts, unsigned iterations = 0);

		/** 
		* Calls each of the registered contact generators to report 
		* their contacts. Returns the number of generated contacts. 
		*/
		unsigned generateContacts();
		/** 
		* Integrates all the particles in this world forward in time * by the given duration. 
		*/
		void integrate(real dt);
		/** 
		*Processes all the physics for the particle world. 
		*/
		void runPhysics(real dt);

		/**
		* Initializes the world for a simulation frame. This clears
		* the force accumulators for particles in the world. After
		* calling this, the particles can have their forces for this
		* frame added.
		*/
		void startFrame();

		/**
		 *  Returns the list of particles.
		 */
		Particles& getParticles();

		/**
		 * Returns the list of contact generators.
		 */
		ContactGenerators& getContactGenerators();

		/**
		 * Returns the force registry.
		 */
		ParticleForceRegistry& getForceRegistry();
	};



	class ParticleCubeContactGenerator : public Grics::ParticleContactGenerator {
		Grics::Particle* p1;
		Grics::Particle* p2;
	    real halfExtent; // Half the length of a cube edge (e.g., 0.5 for unit cube)

	public:
		ParticleCubeContactGenerator(Grics::Particle* p1, Grics::Particle* p2, real cubeSize);

		unsigned addContact(Grics::ParticleContact* contact, unsigned limit) const override;
	};

}

#endif