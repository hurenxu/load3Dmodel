#include "rasterizer.h"

static int window_width = 512, window_height = 512;
static float two = 2.0;
static float* pixels = new float[window_width * window_height * 3];

using namespace std;

struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

rasterizer::rasterizer(const char* filepath) 
{
	this->filepath = filepath;
	loadData();
}

void rasterizer::loadData()
{
	// point cloud parser goes here
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float xn, yn, zn;  // vertex normal coordinates
	float r, g, b;  // vertex color
	int c1, c2;    // characters read from file
	int c3;
	errno_t res;

	res = fopen_s(&fp, this->filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
	for (int i = 0; (c1 = fgetc(fp)) != EOF && (c2 = fgetc(fp)) != EOF; i++)
	{
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf_s(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			c3 = fgetc(fp);
			if (c3 == ' ')
			{
				fscanf_s(fp, "%f %f %f", &xn, &yn, &zn);
				normals.push_back(glm::vec3(xn, yn, zn));
			}
		}
	}
	// read normal data accordingly

	fclose(fp);   // make sure you don't forget to close the file when done
}

// Clear frame buffer
void rasterizer::clearBuffer()
{
	Color clearColor = { 0.0, 1.0, 0.0 };   // clear color: black
	for (int i = 0; i<window_width*window_height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
}

// Draw a point into the frame buffer
void rasterizer::drawPoint(int x, int y, float r, float g, float b)
{
	int offset = y*window_width * 3 + x * 3;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void rasterizer::rasterize()
{
	calculateMVP();
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
	for (int i = 0; i < vertices.size(); i++) 
	{
		float base = sqrt(normals[i].x * normals[i].x + normals[i].y * normals[i].y + normals[i].z * normals[i].z);
		float normX = (normals[i].x / base + 1) / two;
		float normY = (normals[i].y / base + 1) / two;
		float normZ = (normals[i].z / base + 1) / two;
		glm::vec4 homogenousVertices(vertices[i], 1);
		glm::vec3 p(mvp * homogenousVertices);
		//p_prime.push_back(p);
		drawPoint(p.x, p.y, normX, normY, normZ);
	}
}

void rasterizer::calculateMVP() 
{
	C_INV = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	P = glm::perspective(glm::radians(60.0f), (float)window_width / (float)window_height, 1.0f, 1000.0f);
	float x0 = 0;
	float y0 = 0;
	float x1 = window_width;
	float y1 = window_height;
	D = glm::mat4(1.0f);
	D[0] = glm::vec4((x1 - x0) / two, 0, 0, 0);
	D[1] = glm::vec4(0, (y1 - y0) / two, 0, 0);
	D[2] = glm::vec4(0, 0, 1.0 / 2.0, 0);
	D[3] = glm::vec4((x0 + x1) / two, (y0 + y1) / two, 1.0 / 2.0, 1);
	mvp = D * P * C_INV * M;
}

// Called whenever the window size changes
void rasterizer::resizeCallback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
	calculateMVP();
}

void rasterizer::displayCallback(GLFWwindow* window)
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void rasterizer::setModel(glm::mat4 model)
{
	this->M = model;
}
/**
int main(int argc, char** argv) {
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(window_height, window_height, "Rastizer", NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	loadData();

	// Set the error callback
	glfwSetErrorCallback(errorCallback);
	// Set the key callback
	glfwSetKeyCallback(window, keyCallback);
	// Set the window resize callback
	glfwSetWindowSizeCallback(window, resizeCallback);

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(window))
	{
		// Main render display callback. Rendering of objects is done here.
		displayCallback(window);
	}

	// Destroy the window
	glfwDestroyWindow(window);
	// Terminate GLFW
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
*/