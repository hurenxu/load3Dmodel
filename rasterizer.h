#include <iostream>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <glm/gtx/transform.hpp>

class rasterizer
{
private:
	glm::mat4 M;
	glm::mat4 C_INV;
	glm::mat4 P;
	glm::mat4 D;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> p_prime;
	std::vector<glm::vec3> normals;
	glm::mat4 mvp;
public:
	rasterizer(const char* filepath);
	const char *filepath;
	void loadData();
	void clearBuffer();
	void setModel(glm::mat4 model);
	void drawPoint(int x, int y, float r, float g, float b);
	void rasterize();
	void calculateMVP();
	void resizeCallback(GLFWwindow * window, int width, int height);
	void displayCallback(GLFWwindow * window);
};