/*
 * Sphere.h
 *Basado en el código creado por
 *  Created on: 06/09/2016
 *      Author: reynaldo Martell
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "definition.h"

class Sphere {
public:
	Sphere(float ratio, int slices, int stacks); //radio, resoluciones horizontales y verticales
	void init();
	void load();
	void render();
	virtual ~Sphere(); //desctructor de la esfera
private:
	std::vector<VertexColor> vertexC;
	std::vector<GLuint> index;
	float ratio;
	int slices;
	int stacks;


	GLuint VAO, VBO, EBO;
};

#endif /* SPHERE_H_ */







  


