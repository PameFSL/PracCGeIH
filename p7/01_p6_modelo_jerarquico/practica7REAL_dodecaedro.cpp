/*
Semestre 2019-2
Práctica 7 Texturizado
Hecho por: Ing. José Roque Román Guadarrama. Usando librería stb_image.h 
*/
//para cargar imagen
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
float codo = 0.0f;

Texture T_ladrillos;
Texture T_asfalto;
Texture T_dado;
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
		//coordenadas de texturizado 
		//x	      y	  z	      s	     t
		-0.5f, -0.5f,0.5f,	0.0f,	0.0f,  //esquina inf:izq
		0.0f,-0.5f,0.5f,	1.0f,	0.0f, //esquina inf der
		0.5f,-0.5f, 0.0f,	1.0f,	1.0f,  //superior der
		0.0f,0.5f,0.0f,		0.0f,	1.0f	//sub izq
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}
//ejercicio 1 para hacer en clase, el cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// CARA 1
		0, 1, 2,
		3,2,0,
		0,3,4,

		58,57,61,
		57,60,61,
		60,57,59,

		9,8,5,
		8,11,5,
		5,10,11,

		12,13,15,
		15,12,16,
		16,14,12,

		18,14,20,
		20,14,19,
		19,21,20,
		//CARA AZUL CLARO 
		23,22,25,
		25,22,24,
		24,22,26,
		//CARA VERDE 
		27,28,29,
		29,27,31,
		30,31,27,
		//cara azul
		32,33,36,
		36,35,33,
		33,34,35,
		//cara rosa
		37,41,40,
		40,39,37,
		37,38,39,
		//cara amarilla
		44,45,46,
		46,43,44,
		44,42,43,
		//cara roja-rosa
		48,51,50,
		50,49,48,
		48,47,49,
		//cara morada 10
		52,53,56,
		56,55,53,
		53,54,55,
	
	};

GLfloat cubo_vertices[] = {
	//x      y       z      u	   v
	0.3f,  0.0f,  -0.6f,   0.18f, 0.36f,   //0   A
	0.11f, -0.38f, -0.6,  0.15f,  0.6f,   //1    E
	-.31f, -0.31f, -0.6f,  .04f, 0.6f,   //2    D
	-0.38f, 0.11f, -0.6f,  0.01f,0.36f,   //3	c
	0.0f,  0.3f,  -0.6f,  0.1f,	 0.24f,   //4    B
	//--------------------------------------
	0.03f,  0.52f,  -0.24f,   0.31f, 0.36f,   //5   F
	0.36f,  0.36f,  -0.02f,   0.36f, 0.62f,   //6   K
	0.52f,  0.03f,  -0.24f,   0.26f, 0.76f,   //7   J

	-0.38f, 0.11f, -0.6f,  0.16f,0.16f,   //8	C
	0.0f,  0.3f,  -0.6f,  0.2f,  0.36f,   //9   B
	-0.32f, 0.46f, -0.02f,  0.36f,0.16f,   //10	L
	-0.58f,  0.21f,  -0.24f,  0.26f,  0.02f,   //11  G
	//CARA MORADA 2
	0.3f,  0.0f,  -0.6f,   0.16f, 0.6f,     //12   A
	0.11f, -0.38f, -0.6,  0.06f,  0.76f,      //13   E
	0.52f,  0.03f,  -0.24f,   0.26f, 0.76f,  //14   J
	0.21f,  -0.58f,  -0.24f,  0.1f,  0.9f,   //15   I
	0.46f, -0.32f,   -0.02f,  0.22f, 0.9f,   //16	O
	//CARA VERDE FUERTE 5
	0.52f,  0.03f,  -0.24f,   0.26f, 0.76f,  //17   J
	0.46f, -0.32f,   -0.02f,  0.3f, 0.9f,   //18	O
	0.36f,  0.36f,  -0.02f,   0.36f, 0.6f,  //19   K
	0.27f, -0.22f,   0.34f,   0.41f, 0.9f,   //20	T
	0.2f,    0.2f,  0.34f,   0.45f, 0.76f,  //21   P
	//CARA AZUL 6
	0.03f,  0.52f,  -0.24f,   0.31f, 0.36f,  //22   F
	0.36f,  0.36f,  -0.02f,   0.36f, 0.6f,  //23   K
	-0.22f, 0.27f,   0.34f,   0.52f, 0.36f,   //24	Q
	0.2f,    0.2f,  0.34f,   0.48f, 0.6f,  //25   P
	-0.32f, 0.46f, -0.02f,  0.42f,0.22f,   //26	L

	//CARA VERDE, DIVISION 7
	-0.22f, 0.27f,   0.34f,   0.52f, 0.36f,   //27	Q
	0.2f,    0.2f,  0.34f,   0.48f, 0.6f,  //28   P
	0.27f, -0.22f,   0.34f,   0.58f, 0.76f,   //29	T
	-0.41f, -0.11f,  0.34f,   0.63f, 0.36f,  //30   R
	-0.11f, -0.41f,   0.34f,   0.68f, 0.6f,   //31	S

	//CARA AZUL FUERTE 8
	-0.32f, 0.46f, -0.02f,  0.595f,0.02f,   //32	L
	-0.22f, 0.27f,   0.34f,   0.55f, 0.22f,   //33	Q
	-0.41f, -0.11f,  0.34f,   0.64f, 0.36f,  //34  R
	-0.63f, -0.15f,  -0.02f,   0.75f, 0.22f,   //35	M
	-0.58f,  0.21f,  -0.24f,  0.705f,  0.02f,   //36  G

	//cara rosa
	-0.11f, -0.41f,   0.34f,   0.68f, 0.6f,   //37	S
	0.27f, -0.22f,   0.34f,   0.645f, 0.84f,   //38	T
	0.46f, -0.32f,   -0.02f,  0.73f, 0.9f,   //39	O
	0.21f,  -0.58f,  -0.24f,  0.83f,  0.84f,   //40   I
	-0.15f,  -0.63f,  -0.02f,  0.8f,  0.6f,   //41   N

	//cara amarilla 12
	-0.11f, -0.41f,   0.34f,   0.68f, 0.6f,   //42	S
	-0.15f,  -0.63f,  -0.02f,  0.8f,  0.6f,   //43   N
	-0.41f, -0.11f,  0.34f,   0.64f, 0.36f,  //44  R
	-0.63f, -0.15f,  -0.02f,   0.75f, 0.22f,   //45	M
	-0.47f, -0.47f,  -0.24f,   0.84f, 0.36f,   //46	H
	 //cara roja-rosa 9
	-0.15f,  -0.63f,  -0.02f,  0.8f,  0.6f,   //47   N
	-0.47f, -0.47f,  -0.24f,   0.84f, 0.36f,   //48	H
	0.21f,  -0.58f,  -0.24f,  0.885f,  0.76f,   //49  I
	0.11f, -0.38f, -0.6,      0.98f,  0.61f,   //50   E
	-.31f, -0.31f, -0.6f,     .96f, 0.38f,   //51    D
	//cara morada 10
	-0.38f, 0.11f, -0.6f,  0.9f,0.02f,   //52	c
	-.31f, -0.31f, -0.6f,  .93f, 0.21f,   //53   D
	-0.47f, -0.47f,  -0.24f,   0.84f, 0.36f,   //54	H
	-0.63f, -0.15f,  -0.02f,   0.75f, 0.22f,   //55	M
	-0.58f,  0.21f,  -0.24f,  0.79f,  0.02f,   //56  G

	0.0f,  0.3f,  -0.6f,   0.198f, 0.6f,   //57  b
	0.3f,  0.0f,  -0.6f,  0.165f, 0.36f,   //58    a
	0.03f,  0.52f,  -0.24f,   0.31f, 0.36f,   //59   F
	0.36f,  0.36f,  -0.02f,   0.36f, 0.62f,   //60  K
	0.52f,  0.03f,  -0.24f,   0.26f, 0.76f,   //61   J
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,420, 426);
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
	T_ladrillos = Texture("Textures/dirt.png");
	T_dado = Texture("Textures/dod.tga");
	T_ladrillos.LoadTexture();
	T_asfalto = Texture("Textures/ladrillos.tga");
	T_asfalto.LoadTexture();
	T_dado.LoadTexture();
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
		//ejercicio 1:
		glm::mat4 model(1.0); 

		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		T_ladrillos.UseTexture();
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//T_asfalto.UseTexture();
		T_dado.UseTexture();
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}