#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <nb/shader.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);

Vertex vertices[4] = {
	// x     y    z    u    v
	{-1.0, -1.0, 0.0, 0.0, 0.0}, //bottom left
	{ 1.0, -1.0, 0.0, 1.0, 0.0}, // bottom right
	{ 1.0,  1.0, 0.0, 1.0, 1.0}, // top right
	{-1.0,  1.0, 0.0, 0.0, 1.0}, // top left
};

unsigned int indices[6] = {
	// triangle 1
	0, 1, 2,
	// triangle 2
	2, 3, 0
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

float sunColor[] = { 0.9, 0.75, 0.4 };
float skyTopColor[] = { 0.4, 0.4, 0.5 };
float skyBotColor[] = { 0.75, 0.55, 0.4 };;
float hillsColor[] = { 0.6, 0.7, 0.4 };
float sunSpeed = 1.0;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	// Initialize ImGUI DO NOT TOUCH
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	unsigned int vao = createVAO(vertices, 4, indices, 6);

	natalie::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time = (float)glfwGetTime();

		// Set uniforms
		shader.setVec3("sunColor", sunColor[0], sunColor[1], sunColor[2]);
		shader.setVec3("skyTopColor", skyTopColor[0], skyTopColor[1], skyTopColor[2]);
		shader.setVec3("skyBotColor", skyBotColor[0], skyBotColor[1], skyBotColor[2]);
		shader.setVec3("hillsColor", hillsColor[0], hillsColor[1], hillsColor[2]);
		// THIS SLIDER DOESN'T ACTUALLY DO ANYTHING AND I CAN'T FIGURE OUT HOW TO MAKE IT DO SOMETHING
		shader.setFloat("_Time", time);
		shader.setFloat("sunSpeed", sunSpeed);

		shader.setVec2("_Resolution", SCREEN_WIDTH, SCREEN_HEIGHT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		// Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sun Color", sunColor);
			ImGui::ColorEdit3("Top Sky Color", skyTopColor);
			ImGui::ColorEdit3("Bottom Sky Color", skyBotColor);
			ImGui::ColorEdit3("Hills Color", hillsColor);
			ImGui::SliderFloat("Time", &time, 0.0f, 10.0f);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 10.0f);
			ImGui::End();

			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

