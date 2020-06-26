
/*
* Sphere.cpp
*Basado en el código creado por
*  Created on: 06/09/2016
*      Author: reynaldo Martell
*/
#include "Sphere.h"


Cylinder::Cylinder(int slices, int stacks, float topRadius, float bottomRadius, float height): 
	 slices(slices), stacks(stacks), topRadius(topRadius),VAO(0), VBO(0), EBO(0){
	vertexC.resize((slices + 1) * (stacks + 1) + 2 * (slices + 1) + 2);
	index.resize(slices * stacks * 2 * 3 + 2 * slices * 3);
}

Cylinder::~Cylinder() {

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

}



void Sphere::init() {
	float stackHeight = height / stacks;
	float radiusStep = (topRadius - bottomRadius) / stacks;
	int count = 0;
	vertexC.resize(((slices + 1) * (stacks + 1)));
	index.resize((slices * stacks + slices) * 6);
	for (int i = 0; i <= stacks; i++) {
		float y = -0.5f * height + i * stackHeight;
		float r = bottomRadius + i * radiusStep;
		float dTheta = float(2.0f * M_PI) / slices;
		for (int j = 0; j <= slices; j++) {
			float c = cos(j * dTheta);
			float s = sin(j * dTheta);
			vertexC[i * (slices + 1) + j].position = ratio
				* glm::vec3(c*s, c, s);
			vertexC[i * (slices + 1) + j].color = glm::sphericalRand(1.0);
		}
	}


	//top cap
	float y = 0.5f * height;
	float dTheta = float(2.0f * M_PI) / slices;

	for (int i = slices; i >= 0; i--) {
		float x = topRadius * cos(i * dTheta);
		float z = topRadius * sin(i * dTheta);
		//vertexC[count++] = Vertex(glm::vec3(x, y, z), glm::vec4(), glm::vec2(), glm::vec3(0, 0, z));
	}
	//vertexC[count++] = Vertex(glm::vec3(0, y, 0), glm::vec4(), glm::vec2(), glm::vec3(0, y, 0));
	//bottom cap
	y = -y;

	for (int i = 0; i <= slices; i++) {
		float x = bottomRadius * cos(i * dTheta);
		float z = bottomRadius * sin(i * dTheta);
		//vertexC[count++] = Vertex(glm::vec3(x, y, z), glm::vec4(), glm::vec2(), glm::vec3(0, 0, z));
	}
	//vertexC[count++] = Vertex(glm::vec3(0, y, 0), glm::vec4(), glm::vec2(), glm::vec3(0, y, 0));

	//fill indices array
	int ringVertexCount = slices + 1;
	int id = 0;
	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);

			index[id++] = (i * ringVertexCount + j);
			index[id++] = ((i + 1) * ringVertexCount + j + 1);
			index[id++] = (i * ringVertexCount + j + 1);
		}
	}

	//top cap
	int baseIndex = (slices + 1) * (stacks + 1);
	int centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}

	//bottom cap
	baseIndex = centerIndex + 1;
	centerIndex = baseIndex + (slices + 1);

	for (int i = 0; i < slices; i++) {
		index[id++] = centerIndex;
		index[id++] = baseIndex + i;
		index[id++] = baseIndex + i + 1;
	}
}

void Sphere::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;

		glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
			vertexC.data(),
			GL_STATIC_DRAW);
		stride = sizeof(vertexC[0]);
		offset1 = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Sphere::render() {

	glBindVertexArray(VAO);
	glDrawElements( GL_TRIANGLES, index.size(), GL_UNSIGNED_INT,
			(GLvoid*) (sizeof(GLuint) * 0));
	glBindVertexArray(0);

}

void Cylinder::load() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;

	glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
		vertexC.data(),
		GL_STATIC_DRAW);
	stride = sizeof(vertexC[0]);
	offset1 = 0;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Cylinder::render() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT,
		(GLvoid*)(sizeof(GLuint) * 0));
	glBindVertexArray(0);

}