#pragma once
/*
Creado por Reynaldo Martell
Computacion Grafica e Interaccion Humano Computadora
Fecha: 08/02/2018
*/

#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H
#include <iostream>
#include <vector>
#include <memory>
#define _USE_MATH_DEFINES
#include <cmath>
#include <glew.h>
#include "Shader.h"
#define GLM_FORCE_RADIANS
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtc/type_ptr.hpp>

class AbstractModel
{
public:
	enum TypeModel {
		BOX, SPHERE, CYLINDER, QUAD, TRIANGLE, NUM_MODELS = TRIANGLE + 1
	};

	class Vertex {
	public:
		Vertex() {
		}

		Vertex(glm::vec3 m_pos, glm::vec3 m_color, glm::vec2 m_tex, glm::vec3 m_normal) : m_pos(m_pos), m_color(m_color), m_tex(m_tex), m_normal(m_normal) {
		}

	public:
		glm::vec3 m_pos;
		glm::vec3 m_color;
		glm::vec2 m_tex;
		glm::vec3 m_normal;
	};

	class VertexColor {
	public:
		VertexColor() {
		}

		VertexColor(glm::vec3 m_pos, glm::vec3 m_color) : m_pos(m_pos), m_color(m_color) {
		}
	public:
		glm::vec3 m_pos;
		glm::vec3 m_color;
	};

protected:
	Shader1 * shader_ptr;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 color;
	glm::vec3 orientation;
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertexArray;
	std::vector<GLuint> index;
	TypeModel typeModel;
};

#endif // ABSTRACTMODEL_H