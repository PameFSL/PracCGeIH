/*
Semestre 2019-2
Práctica 10: Iluminación 2 Spotlight: Se agrega código para crear spotlights.
Texturizado: se agrega código para transparencia y blending.
Skybox: Se agrega Skybox como textura ligada a la cámara.
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
float rotapato;
bool avanza, patoadelante;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;

Skybox skybox;

Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
																			// right
																			//x		y		z		S		T
																			0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
																			0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
																			0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
																			0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
																			// back
																			-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
																			0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
																			0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
																			-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

																			// left
																			//x		y		z		S		T
																			-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
																			-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
																			-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
																			-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

																			// bottom
																			//x		y		z		S		T
																			-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
																			0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
																			0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
																			-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

																			//UP
																			//x		y		z		S		T
																			-0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
																			0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
																			0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
																			-0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo_1, ciclo_1_2, contador_1 = 0;
bool animacion = false;

//NEW// Keyframes
float posXmartillo = 2.0, posYmartillo = 2.0, posZmartillo = 0;
float movmartillo_x = 0.0f, movmartillo_y = 0.0f;
float giromartillo = 0;

#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movmartillo_x;		//Variable para PosicionX
	float movmartillo_y;		//Variable para PosicionY
	float movmartillo_xInc;		//Variable para IncrementoX
	float movmartillo_yInc;		//Variable para IncrementoY
	float giromartillo;
	float giromartilloInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 5;			//introducir datos
bool play_JC = false;
int play_JCIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].movmartillo_x = movmartillo_x;
	KeyFrame[FrameIndex].movmartillo_y = movmartillo_y;
	KeyFrame[FrameIndex].giromartillo;
	FrameIndex++;
}

void resetElements(void)
{
	movmartillo_x = KeyFrame[0].movmartillo_x;
	movmartillo_y = KeyFrame[0].movmartillo_y;
	giromartillo = KeyFrame[0].giromartillo;
}

void interpolation(void)
{
	KeyFrame[play_JCIndex].movmartillo_xInc = (KeyFrame[play_JCIndex + 1].movmartillo_x - KeyFrame[play_JCIndex].movmartillo_x) / i_max_steps;
	KeyFrame[play_JCIndex].movmartillo_yInc = (KeyFrame[play_JCIndex + 1].movmartillo_y - KeyFrame[play_JCIndex].movmartillo_y) / i_max_steps;
	KeyFrame[play_JCIndex].giromartilloInc = (KeyFrame[play_JCIndex + 1].giromartillo - KeyFrame[play_JCIndex].giromartillo) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play_JC)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			play_JCIndex++;
			printf("play_JCindex : %d\n", play_JCIndex);
			if (play_JCIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				play_JCIndex = 0;
				play_JC = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movmartillo_x += KeyFrame[play_JCIndex].movmartillo_xInc;
			movmartillo_y += KeyFrame[play_JCIndex].movmartillo_yInc;
			giromartillo += KeyFrame[play_JCIndex].giromartilloInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.3ds");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador_1 de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-4.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de helicoptero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0 - movCoche, 2.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");



	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 1.0f;
	avanza = 1;

	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movmartillo_x = 0.0f;
	KeyFrame[0].movmartillo_y = 0.0f;
	KeyFrame[0].giromartillo = 0;


	KeyFrame[1].movmartillo_x = 1.0f;
	KeyFrame[1].movmartillo_y = 2.0f;
	KeyFrame[1].giromartillo = 0;


	KeyFrame[2].movmartillo_x = 2.0f;
	KeyFrame[2].movmartillo_y = 0.0f;
	KeyFrame[2].giromartillo = 0;


	KeyFrame[3].movmartillo_x = 3.0f;
	KeyFrame[3].movmartillo_y = -2.0f;
	KeyFrame[3].giromartillo = 0;

	KeyFrame[4].movmartillo_x = 4.0f;
	KeyFrame[4].movmartillo_y = 0.0f;
	KeyFrame[4].giromartillo = 180.0f;

	//Agregar Kefyrame[5] para que el avión regrese al inicio



	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		//deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		if (avanza)
		{
			if (movCoche < 10.0f)
			{
				//				movCoche += movOffset*deltaTime;
				movCoche += movOffset;
				avanza = 1;
			}
			else
			{
				avanza = 0;
			}
		}
		else
		{
			if (movCoche > -10.0f)
			{
				movCoche -= movOffset * deltaTime;
			}
			else
			{
				avanza = 1;
			}


		}


		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, -1.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();
		spotLights[2].SetPos(glm::vec3(movCoche, -1.5f, 0.0f));

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXmartillo + movmartillo_x, posYmartillo + movmartillo_y, posZmartillo);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, giromartillo* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		spotLights[3].SetPos(posblackhawk);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.53f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion<1)
		{
			if (play_JC == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play_JC = true;
				play_JCIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play_JC = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movmartillo_x es: %f\n", movmartillo_x);
			//printf("movmartillo_y es: %f\n", movmartillo_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo_1 < 1)
		{
			//printf("movmartillo_x es: %f\n", movmartillo_x);
			movmartillo_x += 1.0f;
			printf("movmartillo_x es: %f\n", movmartillo_x);
			ciclo_1++;
			ciclo_1_2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo_1_2<1)
		{
			ciclo_1 = 0;
		}
	}

}