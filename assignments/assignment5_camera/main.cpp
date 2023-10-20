#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <nb/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, nb::Camera* camera, nb::CameraControls* controls, float deltaTime);

//Projection will account for aspect ratio!
int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;
float ASPECT_RATIO = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];
nb::Camera camera;
nb::CameraControls cameraControls;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	// Camera default settings
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0, 0, 0);
	camera.fov = 60.0f;
	camera.aspectRatio = ASPECT_RATIO;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;
	camera.orthoSize = 6.0f;

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	float prevTime = 0; // Timestamp of previous frame
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Calculate deltaTime
		float time = (float)glfwGetTime(); // Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		moveCamera(window, &camera, &cameraControls, deltaTime);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

		//Set uniforms
		shader.use();

		// Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", camera.ProjectionMatrix() * camera.ViewMatrix() * cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			if (ImGui::Button("Reset Camera")) {
				camera.position = ew::Vec3(0, 0, 5);
				camera.target = ew::Vec3(0, 0, 0);
				camera.fov = 60.0f;
				camera.nearPlane = 0.1f;
				camera.farPlane = 100.0f;
				camera.orthographic = false;
				camera.orthoSize = 6.0f;
				cameraControls.yaw = 0;
				cameraControls.pitch = 0;
			};
			ImGui::DragFloat3("Camera Position", &camera.position.x, 0.5f);
			ImGui::DragFloat3("Camera Target", &camera.target.x, 0.5f);
			ImGui::DragFloat("FOV", &camera.fov, 1.0f);
			ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.5f);
			ImGui::DragFloat("Far Plane", &camera.farPlane, 0.5f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			ImGui::DragFloat("Height", &camera.orthoSize, 0.5f);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void moveCamera(GLFWwindow* window, nb::Camera* camera, nb::CameraControls* controls, float deltaTime) {
	// If right mouse is not held, release cursor and return early
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		// Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	// GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	// If we just started right clicking, set prevMouse values to current position (prevents bug where camera moves as soon as we click)
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	// Get mouse position delta for this frame and add to yaw and pitch
	controls->yaw += (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	controls->pitch -= (mouseY - controls->prevMouseY) * controls->mouseSensitivity;

	// Clamp pitch between (-89, 89)
	if (controls->pitch > 89)
		controls->pitch = 89;
	if (controls->pitch < -89)
		controls->pitch = -89;

	// Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	// Construct forward vector using yaw and pitch (convert to radians)
	// Construct right and up vector using forward and world up (0, 1, 0)
	ew::Vec3 forward = ew::Vec3(
		sin(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch)),
		sin(ew::Radians(controls->pitch)),
		-cos(ew::Radians(controls->yaw)) * cos(ew::Radians(controls->pitch)));
	ew::Vec3 right = ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0)));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	// Keyboard controls for moving along forward, back, right, left, up, and down
	if (glfwGetKey(window, GLFW_KEY_W))
		camera->position += forward * controls->moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S))
		camera->position -= forward * controls->moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A))
		camera->position -= right * controls->moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D))
		camera->position += right * controls->moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q))
		camera->position += up * controls->moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_E))
		camera->position -= up * controls->moveSpeed * deltaTime;

	// Set target to point in front of camera along its forward direction
	camera->target = camera->position + forward;
}
