#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <glm/gtx/transform.hpp>

using namespace std;

struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::mat4 toWorld;
glm::mat4 translate;
glm::mat4 scaling;
glm::mat4 orbit;
glm::mat4 model;
glm::mat4 mvp;
glm::mat4 M;
glm::mat4 C_INV;
glm::mat4 P;
glm::mat4 D;
std::vector<glm::vec3> p_prime;
float locX = 0;
float locY = 0;
float locZ = 0;
float pointSize;
int enableEC1 = 0;
int enableEC2 = 0;

public:
	OBJObject(const char* filepath);
	float angle;
	bool counter = 0;
	void parse(const char* filepath);
	void draw();
	void spin(float deg);
	void update();
	void resize(float size);
	void resetPos();
	void orbitFunc(float ang);
	void pSize(float size);
	glm::mat4 returnModel();
	glm::mat4 returnOrbit();
	void move(float locX, float locY, float locZ);
	void clearBuffer();
	void drawPoint(int x, int y, float r, float g, float b, int size);
	void rasterize();
	void calculateMVP();
	void resizeCallback(GLFWwindow * window, int width, int height);
	void displayCallback(GLFWwindow* window);
	void setHeight(int height);
	void setWidth(int width);
	float* getPixels();
	glm::mat4 returnTranslate();
	glm::mat4 returnToWorld();
	glm::mat4 returnScaling();
	void setModel(glm::mat4 model);
	void setTranslate(glm::mat4 translate);
	void setScaling(glm::mat4 scaling);
	void setToWorld(glm::mat4 toWorld);
	void setOrbit(glm::mat4 orbit);
	void setWindow(int width, int height);
	void setPointSize(float pointSize);
	void setEnableOptimation1(int enable);
	void setEnableOptimation2(int enable);
	float resizePointEC1(float Z, float range);
	void resizeR();
};

#endif