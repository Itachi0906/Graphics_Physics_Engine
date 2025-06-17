#include "Cube.h"

namespace Grics {
    /**
    * Holds the vertices of the cube (6 faces × 2 triangles × 3 vertices each)
    */
    static float vertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    unsigned int Cube::m_VAO = 0;
    unsigned int Cube::m_VBO = 0;
    bool Cube::initialized = false;

    Cube::Cube(real scale, Vector3 position, real mass)
	{
        if (mass == 0) {
            std::cerr << "Enter a Valid Mass" << std::endl;
        }

        if (!initialized)
        {
            initialized = true;
            initCube();
        }
 
        /**
        * Holds the edge length of this cube centered by the origin
        */

        setScale(Vector3(scale, scale, scale));
        setPosition(position);
        setMass(mass);
	}
    Cube::Cube(Vector3 scale, Vector3 position, real mass)
    {
        if (mass == 0) {
            std::cerr << "Enter a Valid Mass" << std::endl;
        }

        if (!initialized)
        {
            initialized = true;
            initCube();
        }

        /**
        * Holds the edge length of this cube centered by the origin
        */

        setScale(scale);
        setPosition(position);
        setMass(mass);
    }
	Cube::~Cube()
	{
        /**
        * Deletes the current Vertex Array 
        */
		//glDeleteVertexArrays(1, &m_VAO);
        /** 
        * Deletes the current Vertex Buffers
        */
		//glDeleteBuffers(1, &m_VBO);
	}
    
    glm::vec3 toGLM(const Vector3& v) {
        return glm::vec3(v.x, v.y, v.z);
    }


	void Cube::draw(Shader &shader)
	{
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, toGLM(position));
        model = glm::scale(model, toGLM(scale));

        shader.setMat4("model", model);

        /**
        * Binds VAO as the current Vertex Array
        */
        glBindVertexArray(m_VAO);
        /**
        * Tells OpenGL to how to draw the Geometry using the Vertex Buffer 
        */
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
	}
    
    void Cube::initCube() {
        /**
         * Creates 1 Vertex Array Object (VAO).
         * m_VAO: Stores all the state related to how
         * vertex attributes are configured (e.g., positions, normals, textures).
        */
        glGenVertexArrays(1, &m_VAO);

        /**
         * Creates 1 Vertex Buffer Object (VBO).
         * VBO: Stores actual vertex data
         * (like positions, colors, normals) in GPU memory.
         */
        glGenBuffers(1, &m_VBO);

        /**
         * Binds the VAO so that all the following vertex
         * attribute calls apply to this VAO.
         */
        glBindVertexArray(m_VAO);

        /**
         * Binds the VBO as the current "array buffer".
         */
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        /**
        * Uploads your cube vertex data to the currently bound VBO.
        * GL_ARRAY_BUFFER: Target type for vertex attributes.
        * sizeof(cubeVertices): Total byte size of the vertex array.
        * cubeVertices: The actual data.
        * GL_STATIC_DRAW: This data won't change (static) and will be used for drawing.
        */
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        /**
        * Tells OpenGL how to interpret the vertex data inside the VBO.
        * 0: The index of the attribute (location = 0 in vertex shader).
        * 3: Each vertex has 3 float values (x, y, z).
        * GL_FLOAT: The type of each component.
        * GL_FALSE: Don’t normalize the data (not mapping to 0–1).
        * 3 * sizeof(float): The stride — how many bytes between consecutive vertices.
        * (void*)0: Offset — where the position data begins (start of array)
        */
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        /**
        * Enables the vertex attribute at location 0.
        */
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
}