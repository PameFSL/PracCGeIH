#include "Window.h"

bool tecla = true;
	//*********************
	GLuint uniformc1 = 0;
	GLfloat c2 = 0.0f;
	GLuint uniformc2 = 0;
	//glm::vec3 c2(0.0f);
	//***********************	

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{ //detecta las teclas que manejamos 
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "practica_3", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	//************************************
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this); //identificar si el usuario esta en la ventana correcta al momento de presionar teclas 
}

void Window::createCallbacks()
{
	//
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	//identificar que ventana esta abierta
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	//con la teclas scape sale 

	// INTRUCCIONES PARA EL MANEJO DE LAS TECLAS 
	//***********************
	if (action == GLFW_PRESS) {
		const char* key_name;
		switch (key) {
		case GLFW_KEY_ESCAPE://indica que fue la tecla scape
			glfwSetWindowShouldClose(window, GL_TRUE); //hace salir de la ventana
			tecla = false;
			break;
		case GLFW_KEY_D:
			//muestra nombre de la tecla 
			key_name = glfwGetKeyName(GLFW_KEY_D, 0);
			tecla = true;
			printf("se presiono la tecla: %s, %d\n", key_name, tecla);
			break;
		case GLFW_KEY_R:
			key_name = glfwGetKeyName(GLFW_KEY_R, 0);
			tecla = false;
			printf("se presiono la tecla: %s, %d\n", key_name, tecla);
			break;
		}
	}
	//***************

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			//mostro la tecla que presiono de forma alfanumerica 
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
