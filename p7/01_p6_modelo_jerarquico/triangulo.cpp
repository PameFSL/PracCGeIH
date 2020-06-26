#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;  //declaramos 3 variables enteras

//Vertex Shader
//el codigo esta embebido dentro del codigo principal    

//veclaramos la version a utilizar , location=0 no es necesario
//vec 3 vector de 3 vertices
//gl_Position dibuja los vertices y renderiza , podemos cambiar el tamaño aqui
//gl_Position=vec4(pos.x*.2,pos.y*.1,pos.z*.4,1.0f);
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//manejamos color, iluminacion y sombreado a lo del vertex shader
//tiene la bandera out de salida 
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.8f,0.0f,1.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	//generamos los vertices v1,v2,v3,....
	// arreglo flotante, dinamico
	GLfloat vertices[] = {
		/*
		-1.0f, -1.0f,0.0f,   //v0
		1.0f,-1.0f, 0.0f,	 //v1
		0.0f,1.0f,0.0f		//v2
		*/	

		-0.9f, -0.5f,0.0f, //inferior izquierda  //1
		-0.8f, -0.5, 0.0,							//2
		-0.8f, 0.8, 0.0,  //superior derecho							//3

		-0.9f, -0.5f,0.0f,   //superior izquierdo							//2
		-0.8f, 0.8, 0.0f,
		-0.9f, 0.8f,0.0f,							//4

		-0.9f, 0.2, 0.0,
		-0.6f, 0.0, 0.0,
		-0.3f, 0.2, 0.0,			//6

		-0.3f, 0.2, 0.0,
		- 0.3f, 0.8, 0.0,
		-0.2f, 0.5, 0.0,

		/*-0.8f, 0.8, 0.0,
		-0.3f, 0.8, 0.0,
		-0.3f, 1, 0.0,*/


		-0.8f, 0.8, 0.0,
		-0.3f, 0.8, 0.0,						//5
		-0.6f, 1, 0.0,


		//++++++++++++++
		//1.0f,-1.0f,0.0f   //inf_derecha
		//0.5f,0.5f,-0.5f		// superior derecha
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO    crea arreglo de vertices 
	glBindVertexArray(VAO);//asignar VAO			Asigna vaslores al arreglo

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		//EMPEZAMOS DESDE EL PUNTO 0 Y CADA 3 LO PROCESAMOS 
		//gl false solo procesa las coordenadas de los vertices ya que puede haber más datos 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		//se habilita el array en la posicion 0 
		glEnableVertexAttribArray(0);
		//manda a la tarjeta los datos
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}

// recibe 3 parametros, programa, tipo shader
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader


{
	//Asignamos que es cada uno de los parametro recibidos 
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se compila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
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
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Crear tríangulo
	CrearTriangulo();
	CompileShaders();


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		// dibuja el triangulo
		glBindVertexArray(VAO);
		//posiciones de los triangulos
		glDrawArrays(GL_TRIANGLES,0,20);  //procesa de 3 en 3 por eso solo aumentamos el indice 
		// una vez que termina, libera espacio
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}