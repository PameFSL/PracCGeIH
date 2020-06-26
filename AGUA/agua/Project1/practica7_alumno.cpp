
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al còdigo
#include"Mesh_texturizado.h"
#include"Shader.h"
#include "Sphere.h"
#include"Window.h"
#include "Camera.h"
#include"Texture.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture T_dodecaedro;
Camera camera;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader_t.vert";
static const char* fShader = "shaders/shader_t.frag";
Sphere sp = Sphere(1, 20, 20);

void CreateObject()
{
	unsigned int indices[] = { 
		0,3,1,
		1,3,2,
		2,3,0,
		0,1,2
	};
	GLfloat vertices[] = {
		//X		Y		Z	  S		T
		-0.5f, -0.5f,  0.5f, 0.0f,	0.0f,
		0.0f,  -0.5f,  0.5f, 1.0f,	0.0f,
		0.5f,  -0.5f,  0.0f, 1.0f,	1.0f,
		0.0f,  0.5f,   0.0f, 0.0f,	1.0f
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
	
		0,1,2,
		2,1,4,
		2,4,3,
		9,5,6,// 2
		9,6,7,
		9,7,8,
		13,12,11,//3
		13,11,10,
		13,10,14,
		18,17,16,//4
		18,16,15,
		18,15,19,
		23,22,21, //5
		23,21,20,
		23,20,24,
		28,27,26,//6
		28,26,25,
		28,25,29,
		33,32,31,//7
		33,31,30,
		33,30,34,
		38,37,36, //8
		38,36,35,
		38,35,39,
		43,42,41,//9
		43,41,40,
		43,40,44,
		48,47,46, //10
		48,46,45,
		48,45,49,
		53,52,51,//11
		53,51,50,
		53,50,54,
		58,57,56, //12
		58,56,55,
		58,55,59,
	};

	GLfloat cubo_vertices[] = {
		
	0.25f,  -0.38f,  0.0f,	0.0f, 0.14f, //0
   -0.25f,  -0.38f,  0.0f,	0.0f, 0.014f, //1 
    0.4f,    0.1f,    0.0f,	0.0f,	0.014f, //2
   -0.01f,   0.39f,   0.0f,	0.0f,	0.14f, //3
   -0.41f,   0.09f,   0.0f,	0.0f,	0.23f, //4
   -0.25f,  -0.38f,  0.00f,	0.0f, 0.144f, //5 
	0.25f,  -0.38f,  0.00f,	0.0f,	0.013f, //6
	0.41f,  -0.59f,  0.43f,	0.0f,	0.216f, //7
	0.00f,  -0.72f,  0.69f,	0.0f, 0.144f, //8
   -0.40f,  -0.59f,  0.43f,	0.0f,	0.013f,	//9		
	0.25f, -0.38f,  0.00f,	0.0f,	0.013f,	//10
	0.40f,  0.10f,  0.00f,	0.0f,	0.144f,	//11
	0.65f,  0.18f,  0.43f,	0.0f,	0.216f,	//12
	0.65f, -0.25f,  0.69f,	0.695f, 0.144f,	//13
	0.40f, -0.59f,  0.43f,	0.741f,	0.013f,	//14	
	0.40f,  0.10f,  0.00f,	0.258f,	0.26f,  //15
	0.00f,  0.39f,  0.00f,	0.310f,	0.394f, //16
	0.00f,  0.65f,  0.43f,	0.17f,	0.468f, //17
	0.40f,  0.52f,  0.69f,	0.025f,	0.394f, //18
	0.65f,  0.18f,  0.43f, 0.073f,  0.26f, //19	
	0.00f,  0.39f,  0.00f,	0.588f,	0.26f,  //20
   -0.40f,  0.10f,  0.00f,	0.636f,	0.394f, //21
   -0.65f,  0.18f,  0.43f,	0.50f,	0.468f, //22
   -0.40f,  0.52f,  0.69f,	0.36f,	0.394f, //23
	0.00f,  0.65f,  0.43f, 0.411f, 0.26f,  //24 				
   -0.40f,  0.10f,  0.00f,	0.910f,	0.27f,  //25
   -0.25f, -0.38f,  0.00f,	0.96f,	0.400f, //26
   -0.40f, -0.59f,  0.43f,	0.82f,	0.478f, //27
   -0.65f, -0.25f,  0.69f,	0.68f,	0.354f, //28
   -0.65f,  0.18f,  0.43f, 0.735f, 0.27f,  //29			
	0.00f, -0.72f,  0.69f,	0.258f,	0.50f,  //30
	0.40f, -0.59f,  0.43f,	0.310f,	0.64f,  //31
	0.65f, -0.25f,  0.69f,	0.170f,	0.72f,  //32
	0.40f, -0.17f,  1.11f,	0.025f,	0.64f,  //33
	0.00f, -0.46f,  1.11f, 0.073f, 0.50f,  //34			
	0.65f, -0.25f,  0.69f,	0.588f,	0.50f,  //35
	0.65f,  0.18f,  0.43f,	0.636f,	0.64f,  //36
	0.40f,  0.52f,  0.69f,	0.50f,	0.72f,  //37
	0.25f,  0.31f,  1.11f,	0.36f,	0.64f,  //38
	0.40f, -0.17f,  1.11f, 0.411f, 0.50f,  //39			
	0.40f,  0.52f,  0.69f, 0.910f,	0.51f,  //40
	0.00f,  0.65f,  0.43f,	0.960,	0.64f,  //41
   -0.40f,  0.52f,  0.69f,	0.820f,	0.72f,  //42
   -0.25f,  0.31f,  1.11f,	0.680f,	0.64f,  //43
	0.25f,  0.31f,  1.11f, 0.735f, 0.51f,  //44			
   -0.40f,  0.52f,  0.69f,	0.258f,	0.760f, //45
   -0.65f,  0.18f,  0.43f,	0.310f,	0.897f, //46
   -0.65f, -0.25f,  0.69f,	0.170f,	0.981f, //47
   -0.40f, -0.17f,  1.11f,	0.025f,	0.897f, //48
   -0.25f,  0.31f,  1.11f, 0.073f, 0.760f, //49			
   -0.65f, -0.25f,  0.69f,	0.588f,	0.760f, //50
   -0.40f, -0.59f,  0.43f,	0.636f,	0.897f, //51
	0.00f, -0.72f,  0.69f,	0.500f,	0.981f, //52
	0.00f, -0.46f,  1.11f,	0.360f,	0.897f, //53
   -0.40f, -0.17f,  1.11f, 0.411f, 0.760f, //54				
   -0.40f, -0.17f,  1.11f,	0.910f,	0.760f, //55
	0.00f, -0.46f,  1.11f,	0.960f,	0.897f, //56
	0.40f, -0.17f,  1.11f,	0.820f,	0.981f, //57
	0.25f,  0.31f,  1.11f,	0.680f,	0.897f, //58
   -0.25f,  0.31f,  1.11f, 0.735f, 0.760f, //59

};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,300, 108);
meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreateObject();
	CrearCubo();
	CreateShaders();
	camera = Camera(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.3f);
	//para crear la textura
	
	T_dodecaedro = Texture("Textures/dodecaedro.tga");
	T_dodecaedro.LoadTexture();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLoaction();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();

		glm::mat4 model(1.0); 
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	
		T_dodecaedro.UseTexture();

		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}