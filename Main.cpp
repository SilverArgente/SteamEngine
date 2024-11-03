//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include "FluidSimulation.h"

// Define precision for fluid simulation
#define N 100
#define arr_size (N+2)*(N+2)*(N+2)
#define IX(i, j, k) ((k + (j * N+2)) + i * (N+2) * (N+2))
#define SWAP(x0, x) {float *tmp = x0; x0 = x; x = tmp;}

const unsigned int width = 800;
const unsigned int height = 800;
float size = 1;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
		(size * -0.5f), (size * 0.0f),  (size * 0.5f),     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		(size * -0.5f), (size * 0.0f), (size * -0.5f),     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 (size * 0.5f), (size * 0.0f), (size * -0.5f),     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
		 (size * 0.5f), (size * 0.0f),  (size * 0.5f),     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
		 (size * 0.0f), (size * 0.8f),  (size * 0.0f),     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

/*
void draw_dens(const FluidSimulation& sim, Shader& shaderProgram, float maxDensity, float cellSize) {
	// Use the shader program
	glUseProgram(shaderProgram.ID);

	// Iterate through each cell in the 3D grid
	for (int i = 1; i <= sim.n; i++) {
		for (int j = 1; j <= sim.n; j++) {
			for (int k = 1; k <= sim.n; k++) {
				// Get the density value for the current grid cell
				float density = sim.dens[IX(i, j, k)];

				// Calculate opacity and color intensity based on density
				float opacity = density / maxDensity;         // Normalized for transparency
				float colorIntensity = density / maxDensity;  // Normalized for color

				// Pass color and opacity to the shader as a uniform
				glUniform4f(glGetUniformLocation(shaderProgram.ID, "cellColor"),
					colorIntensity, colorIntensity, 1.0f - colorIntensity, opacity);

				// Compute the cube position in the grid
				glm::vec3 position = glm::vec3(i * cellSize, j * cellSize, k * cellSize);

				// Model transformation matrix for the cube position
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, position);
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

				// Bind VAO for the cube and render it
				cubeVAO.Bind();  // Assuming `cubeVAO` represents a pre-configured VAO for drawing a cube
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}
}*/




int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	/*
	* I'm doing this relative path thing in order to centralize all the resources into one folder and not
	* duplicate them between tutorial folders. You can just copy paste the resources from the 'Resources'
	* folder and then give a relative path from this folder to whatever resource you want to get to.
	* Also note that this requires C++17, so go to Project Properties, C/C++, Language, and select C++17
	*/
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "/Resources/YoutubeOpenGL 7 - Going 3D/";

	// Texture
	Texture brickTex((parentDir + texPath + "brick.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);

	// Original code from the tutorial
	/*Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	brickTex.texUnit(shaderProgram, "tex0", 0);*/



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	shaderProgram.Activate();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool drawTriangle = true;

	float size = 1.0f;
	float color[4] = { 0.8f, 0.3f, 0.02f, 1.0f };
	glUseProgram(shaderProgram.ID);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "size"), size);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "color"), color[0], color[1], color[2], color[3]);
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = (currentFrame - lastFrame) * 50;
		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Render the OpenGL content
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard && !io.WantCaptureMouse) {
			camera.Inputs(window, deltaTime);
		}

		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");


		// Bind texture and VAO, then draw
		brickTex.Bind();
		VAO1.Bind();
		if (drawTriangle) {
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		ImGui::Begin("Test");
		ImGui::Text("hello world");
		ImGui::Checkbox("Draw Triangle", &drawTriangle);
		ImGui::SliderFloat("Size", &size, 0.5f, 2.0f);
		ImGui::ColorEdit4("Color", color);
		ImGui::End();

		glUseProgram(shaderProgram.ID);
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "size"), size);
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "color"), color[0], color[1], color[2], color[3]);
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
		lastFrame = currentFrame;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	brickTex.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}