#pragma once
#ifndef GRICS_GRID_H
#define GRICS_GRID_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector"
#include "precision.h"

namespace Grics {
	class Grid {
	private:
		std::vector<real> gridVertices;
		int gridSize;
		static unsigned int gridVAO, gridVBO;
	public:
		Grid(int);
		~Grid();
		void drawGrid();
	};
}

#endif