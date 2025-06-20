#include "Mesh.h"

using namespace Grics;

GLuint Mesh::VBO = 0, Mesh::VAO = 0, Mesh::EBO = 0;
bool Mesh::initialized = false;

void Mesh::initMesh()
{
    generateAllShapes();

    /**
    * Creates 1 Vertex Array Object (VAO).
    * m_VAO: Stores all the state related to how
    * vertex attributes are configured (e.g., positions, normals, textures).
    */
    glGenVertexArrays(1, &VAO);


    /**
     * Binds the VAO so that all the following vertex
     * attribute calls apply to this VAO.
     */
    glBindVertexArray(VAO);

    /**
     * Creates 1 Vertex Buffer Object (VBO).
     * VBO: Stores actual vertex data
     * (like positions, colors, normals) in GPU memory.
     */
    glGenBuffers(1, &VBO);

    /**
     * Binds the VBO as the current "array buffer".
     */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /**
    * Uploads your cube vertex data to the currently bound VBO.
    * GL_ARRAY_BUFFER: Target type for vertex attributes.
    * sizeof(vertices): Total byte size of the vertex array.
    * vertices: The actual data.
    * GL_STATIC_DRAW: This data won't change (static) and will be used for drawing.
    */
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(real), vertexBuffer.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(unsigned int), indexBuffer.data(), GL_STATIC_DRAW);


    /**
    * Enables the vertex attribute at location 0.
    */
    glEnableVertexAttribArray(0);

    /**
    * Tells OpenGL how to interpret the vertex data inside the VBO.
    * 0: The index of the attribute (location = 0 in vertex shader).
    * 3: Each vertex has 3 float values (x, y, z).
    * GL_FLOAT: The type of each component.
    * GL_FALSE: Don’t normalize the data (not mapping to 0–1).
    * 3 * sizeof(float): The stride — how many bytes between consecutive vertices.
    * (void*)0: Offset — where the position data begins (start of array)
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(real), (void*)0);
}

void Grics::Mesh::generateAllShapes()
{
    // Cube
    appendShape(generateCube(), generateCubeIndices(), shapeType::CUBE);
    // Plane
    appendShape(generatePlane(), generatePlaneIndices(), shapeType::PLANE);
    // Cone
    appendShape(generateCone(), generateConeIndices(), shapeType::CONE);
    // Cylinder
    appendShape(generateCylinder(), generateCylinderIndices(), shapeType::CYLINDER);
    // Sphere
    appendShape(generateSphere(), generateSphereIndices(), shapeType::SPHERE);
    // Grid
    appendShape(generateGrid(), generateGridIndices(), shapeType::GRID);
}

void Grics::Mesh::appendShape(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, shapeType type)
{
    GLuint baseVertex = static_cast<GLuint>(vertexBuffer.size() / 3);
    GLuint baseIndex = static_cast<GLuint>(indexBuffer.size());

    vertexBuffer.insert(vertexBuffer.end(), vertices.begin(), vertices.end());

    for (unsigned int idx : indices) {
        indexBuffer.push_back(baseVertex + idx);
    }

    GLuint count = static_cast<GLuint>(indices.size());
    shapeDrawData[type] = { count, baseIndex };
}

std::vector<real> Mesh::generateCube()
{
    return {
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f, -0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f
    };
}

std::vector<unsigned int> Mesh::generateCubeIndices()
{
    return {
        0,1,2, 2,3,0, // back
        4,5,6, 6,7,4, // front
        0,4,7, 7,3,0, // left
        1,5,6, 6,2,1, // right
        3,2,6, 6,7,3, // top
        0,1,5, 5,4,0  // bottom
    };
}

std::vector<real> Mesh::generateSphere()
{
    return std::vector<real>();
}

std::vector<unsigned int> Mesh::generateSphereIndices()
{
    return std::vector<unsigned int>();
}

std::vector<real> Mesh::generateCylinder()
{
    return std::vector<real>();
}

std::vector<unsigned int> Mesh::generateCylinderIndices()
{
    return std::vector<unsigned int>();
}

std::vector<real> Mesh::generateCone()
{
    return std::vector<real>();
}

std::vector<unsigned int> Mesh::generateConeIndices()
{
    return std::vector<unsigned int>();
}

std::vector<real> Mesh::generatePlane()
{
    return std::vector<real>();
}

std::vector<unsigned int> Mesh::generatePlaneIndices()
{
    return std::vector<unsigned int>();
}

std::vector<real> Mesh::generateGrid()
{
    return std::vector<real>();
}

std::vector<unsigned int> Mesh::generateGridIndices()
{
    return std::vector<unsigned int>();
}

Mesh::Mesh()
{
    if (!initialized)
    {
        initialized = true;
        initMesh();
    }
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::drawGeometry(shapeType geometryType)
{
    GLuint count = shapeDrawData[geometryType].first;
    GLuint offset = shapeDrawData[geometryType].second;
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(unsigned int)));
}
