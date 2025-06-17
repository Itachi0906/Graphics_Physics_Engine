#pragma once
#ifndef GRICS_CUBE_H
#define GRICS_CUBE_H
#include "Particle.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Grics {
	class Cube : public Particle {
    private:
        /*
        * Holds the shared Vertex Buffer and Vertex Array over all objects of this class
        */
        static unsigned int m_VBO, m_VAO;
        static bool initialized;
        void initCube();
    public : 

        Cube(real, Vector3, real mass = 1);
        Cube(Vector3, Vector3, real mass = 1);
        ~Cube();
        void draw(Shader&);
        
	};
}
#endif