#include "Grid.h"

namespace Grics {

	unsigned int Grid::gridVAO = 0;
	unsigned int Grid::gridVBO = 0;

	Grid::Grid(int size)
	{
		/**
		* Holds the size of the gridlines from -size to +size
		*/
		gridSize = size;
		for (int i = -gridSize; i <= gridSize; ++i) {
			// Vertical lines (parallel to Z-axis)
			gridVertices.push_back((real)i); gridVertices.push_back(0.0f); gridVertices.push_back((float)-gridSize);
			gridVertices.push_back((real)i); gridVertices.push_back(0.0f); gridVertices.push_back((float)gridSize);

			// Horizontal lines (parallel to X-axis)
			gridVertices.push_back((real)-gridSize); gridVertices.push_back(0.0f); gridVertices.push_back((float)i);
			gridVertices.push_back((real)gridSize);  gridVertices.push_back(0.0f); gridVertices.push_back((float)i);
		}

		if(gridVAO == 0)
		{
			glGenVertexArrays(1, &gridVAO);
			glGenBuffers(1, &gridVBO);

			glBindVertexArray(gridVAO);
			glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
			glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

			glBindVertexArray(0);
		}
	}	
	Grid::~Grid()
	{

	}
	void Grid::drawGrid()
	{
		glBindVertexArray(gridVAO);
		glDrawArrays(GL_LINES, 0, (GLsizei)(gridVertices.size() / 3));
		glBindVertexArray(0);
	}
}