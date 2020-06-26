#pragma once
#ifndef CYLINDER_H
#define CYLINDER_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <math.h>

#include "definition.h"

class Cylinder {

public:
	Cylinder(int slices, int stacks, float topRadius = 1.0, float bottomRadius = 1.0, float height = 1.0);
	void init();
	void load();
	void render();
	virtual ~Cylinder();


private:
	std::vector<VertexColor> vertexC;
	/*std::vector<VertexLightColor> vertexLC;
	std::vector<VertexLightTexture> vertexLT;*/
	std::vector<GLuint> index;
	float ratio;
	int slices;
	int stacks;
	float topRadius, bottomRadius, height;
	//MODEL_MODE mode;

	GLuint VAO, VBO, EBO;
};

#endif // CYLINDER_H