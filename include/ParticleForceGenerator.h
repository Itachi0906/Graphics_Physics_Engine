#ifndef GRICS_PARTICLE_FORCE_GENERATOR_H
#define GRICS_PARTICLE_FORCE_GENERATOR_H

#include "Particle.h"

namespace Grics{
	class ParticleForceGenerator {
	public:
		virtual void updateForce(Particle* particle, real dt) = 0;
	};

	class ParticleForceRegistry {
	protected:
		/**
		* Keeps track of one force generator and the particle it
		* applies to.
		*/
		struct ParticleForceRegistration
		{
			Particle* particle;
			ParticleForceGenerator* fg;
		};
		/**
		* Holds the list of registrations.
		*/
		typedef std::vector<ParticleForceRegistration> Registry;
		Registry registrations;

	public:
		/**
		* Registers the given force generator to apply to the
		* given particle.
		*/
		void add(Particle* particle, ParticleForceGenerator* fg);
		/**
		* Removes the given registered pair from the registry.
		* If the pair is not registered, this method will have
		* no effect.
		*/	
		void remove(Particle* particle, ParticleForceGenerator* fg);
		/**
		* Clears all registrations from the registry. This will
		* not delete the particles or the force generators
		* themselves, just the records of their connection.
		*/
		void clear();
		/**
		* Calls all the force generators to update the forces of
		* their corresponding particles.
		*/
		void updateForces(real dt);
	};


	class ParticleGravity : public ParticleForceGenerator {
		/**
		* Holds the acceleration due to gravity 
		*/
		Vector3 gravity;

	public:
		/**
		* Creates the generator with given acceleration 
		*/
		ParticleGravity(const Vector3& gravity);

		/** 
		* Applies the gravitational force to the given particle.
		*/
		virtual void updateForce(Particle* particle, real dt);
	};

	
	/**
	* A force generator that applies a drag force. One instance
	* can be used for multiple particles.
	*/
	class ParticleDrag : public ParticleForceGenerator
	{
		/** Holds the velocity drag coefficient. */
		real k1;
		/** Holds the velocity squared drag coefficient. */
		real k2;
	public:
		/** Creates the generator with the given coefficients. */
		ParticleDrag(real k1, real k2);
		/** Applies the drag force to the given particle. */
		virtual void updateForce(Particle* particle, real duration);
	};

	/**
	* a force generator that applies a spring force
	*/
	class ParticleSpring :public ParticleForceGenerator {
		/**
		* Particle at the other end of the spring
		*/
		Particle* other;

		/**
		* Holds the spring constant
		*/
		real springConstant;

		/**
		* Holds the rest length of the spring
		*/
		real restLength;

	public:
		/** Creates a new spring with the given parameters*/
		ParticleSpring(Particle* other, real springConstant, real restLength);

		/**
		* Applies the spring force to the given particle
		*/
		virtual void updateForce(Particle* partilce, real dt);

	};

	/**
	* a force generator that applies a spring force,
	* where one end is attached to a fixed point in space
	*/
	class ParticleAnchoredSpring : public ParticleForceGenerator {
		/**
		* The location of the anchored end of the spring
		*/
		Vector3* anchor;

		/**
		* Holds the spring constant
		*/
		real springConstant;

		/**
		* Holds the rest length of the spring
		*/
		real restLength;


	public:
		/** Creates a new anchored spring with the given parameters*/
		ParticleAnchoredSpring(Vector3 *anchor, real springConstant, real restLength);

		/**
		* Applies the spring force to the given particle
		*/
		virtual void updateForce(Particle* partilce, real dt);
	};

	/**
	* A force generator that applies a force when extended 
	*/
	class ParticleBungee : public ParticleForceGenerator {
		/**
		* Particle at the other end of the spring
		*/
		Particle* other;
		/**
		* Holds the spring constant
		*/
		real springConstant;
		/**
		* Holds the rest length of the spring
		*/
		real restLength;

	public:
		/** Creates a new spring with the given parameters*/
		ParticleBungee(Particle* other, real springConstant, real restLength);

		/**
		* Applies the spring force to the given particle
		*/
		virtual void updateForce(Particle* partilce, real dt);
	};
}


#endif