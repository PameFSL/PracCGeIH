#include "Window.h"

//********* para modificar la posicion de la luz ********************
GLfloat LX = -2.0f;
GLfloat LY = 0.0f;
GLfloat LZ = 0.0f;
GLfloat L2X = 2.0f;
GLfloat L2Y = 0.0f;
GLfloat L2Z = 0.0f;
//***********************************


Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
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
	mainWindow = glfwCreateWindow(width, height, "Practica 8", NULL, NULL);

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
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		printf("se presiono la tecla: %s\n",key_name);
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_X, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LX -= 0.8f;
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_H, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LX += 0.8f;
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_T, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LY += 0.8f;
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_G, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LY -= 0.8f;
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_B, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LZ += 0.8f;
	}
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_V, 0);
		printf("se presiono la tecla: %s\n", key_name);
		LZ -= 0.8f;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}

	//**************PARA LA OTRA LUZ ***********
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_J, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2X -= 0.8f;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_L, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2X += 0.8f;
	}

	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_I, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2Y += 0.8f;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_K, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2Y -= 0.8f;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_O, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2Z += 0.8f;
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_P, 0);
		printf("se presiono la tecla: %s\n", key_name);
		L2Z -= 0.8f;
	}

}
GLfloat Window::getLX() { return LX; }
GLfloat Window::getLY() { return LY; }
GLfloat Window::getLZ() { return LZ; }
//***********LUZ 2***********
GLfloat Window::getL2X() { return L2X; }
GLfloat Window::getL2Y() { return L2Y; }
GLfloat Window::getL2Z() { return L2Z; }

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
