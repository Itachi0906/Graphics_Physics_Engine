#pragma once
#ifndef GRICS_MESH_H
#define GRICS_MESH_H

#include "Shader.h"
#include "core.h"
#include "precision.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>


namespace Grics {
	class Mesh {
	public:
		enum shapeType{
			CUBE,
			SPHERE,
			CYLINDER,
			CONE,
			PLANE,
			GRID
		};

		Mesh();
		~Mesh();
		void drawGeometry(shapeType, Shader& , const Vector3&, const Vector3&);
		
	private:
		static GLuint VBO, VAO, EBO;
		static bool initialized;
		std::unordered_map<shapeType, std::pair<GLuint, GLuint>> shapeDrawData; // {indexCount, indexOffset}
		std::vector < real > vertexBuffer;
		std::vector<unsigned int> indexBuffer;
		void initMesh();
		void generateAllShapes();
		void appendShape(const std::vector<real>& vertices, const std::vector<unsigned int>& indices, shapeType type);
		std::vector<real> generateCube();
		std::vector<unsigned int> generateCubeIndices();

		std::vector<real> generateSphere();
		std::vector<unsigned int> generateSphereIndices();

		std::vector<real> generateCylinder();
		std::vector<unsigned int> generateCylinderIndices();

		std::vector<real> generateCone();
		std::vector<unsigned int> generateConeIndices();

		std::vector<real> generatePlane();
		std::vector<unsigned int> generatePlaneIndices();

		std::vector<real> generateGrid();
		std::vector<unsigned int> generateGridIndices();
	};
}


#endif