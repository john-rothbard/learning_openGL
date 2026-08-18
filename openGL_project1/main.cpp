#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int randombitch();

//constants (duh)
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices1[] = {
	-0.5f,  0.0f, 0.0f,  // left 
	 0.5f,  0.0f, 0.0f,  // right
	-0.25f,  0.5f, 0.0f  // top left
};
float vertices2[] = {
	-0.5f, -0.5f, 0.0f,  // bottom left
	 0.0f, -0.5f, 0.0f,  // bottom middle
	 0.5f, 0.5f, 0.0f   // bottom right
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);


	//new shit
	unsigned int VAOS[2], VBOS[2];
	glGenBuffers(2, VBOS);

	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//setting up shader (vertex shader)
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//shader compilation error checking
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//setting up shader 2 (fragment shader)
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	//shader2 compilation error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//shader program (linking shaders)
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//shader program error checking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::attachment_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	//delete shaders after linking them to the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//HERE (LINKING VERTEX ATTRIBUTES)

	//vao shit
	glGenVertexArrays(2, VAOS);


	//ebo shit
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAOS[0]);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// 2.5. copy our index array in a element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//round 2
	glBindVertexArray(VAOS[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//callback functions. (whats a callback function?)
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	while (!glfwWindowShouldClose(window))
	{
		//inputs
		processInput(window);

		//rendering 
		glClearColor(0.9f, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOS[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAOS[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//events/buffer swap
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//clear allocated resources
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int randombitch()
{
	// 1. Obtain a random seed from the hardware
	std::random_device rd;

	// 2. Initialize the standard Mersenne Twister engine with the seed
	std::mt19937 gen(rd());

	// 3. Define the distribution range [inclusive, inclusive]
	std::uniform_int_distribution distrib(0, 1);
	// 4. Generate the random number

	return distrib(gen);
}