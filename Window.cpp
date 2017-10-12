#include "Window.h"

const char* window_title = "GLFW Starter Project";
//Cube cube(5.0f);
char* filepath = "C:\\Users\\reh01\\Desktop\\CSE 167\\CSE167StarterCode\\img\\bunny.obj";
OBJObject obj1("C:\\Users\\reh01\\Desktop\\CSE 167\\CSE167StarterCode\\img\\bunny.obj");
OBJObject obj2("C:\\Users\\reh01\\Desktop\\CSE 167\\CSE167StarterCode\\img\\bear.obj");
OBJObject obj3("C:\\Users\\reh01\\Desktop\\CSE 167\\CSE167StarterCode\\img\\dragon.obj");
OBJObject obj(filepath);

glm::mat4 toWorldObj;
glm::mat4 translateObj;
glm::mat4 scalingObj;
glm::mat4 orbitObj;
int Window::width;
int Window::height;
float sizeo = 1;
float psize = 1;
float mul = 2;
int small = 0;
int rev = 0;
float dis = 1;
float locX = 0.0;
float locY = 0.0;
float locZ = 0.0;
float locXSam = 0.0;
float locYSam = 0.0;
float locZSam = 0.0;
float ang = 0;
int mode = 0;

void Window::initialize_objects()
{
}

void Window::clean_up()
{
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

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

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	if (mode % 2 == 0) 
	{
		Window::width = width;
		Window::height = height;
		// Set the viewport size
		glViewport(0, 0, width, height);
		// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
		glMatrixMode(GL_PROJECTION);
		// Load the identity matrix
		glLoadIdentity();
		// Set the perspective of the projection viewing frustum
		gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
		// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
		glTranslatef(0, 0, -20);
	}
	else 
	{
		obj.resizeCallback(window, width, height);
	}
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	obj.update();
}

void Window::display_callback(GLFWwindow* window)
{
	if (mode % 2 == 0) 
	{
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set the matrix mode to GL_MODELVIEW
		glMatrixMode(GL_MODELVIEW);
		// Load the identity matrix
		glLoadIdentity();

		// Render objects
		//cube.draw();
		// Render objects
		obj.draw();
		// Gets events, including input such as keyboard and mouse or window resizing
		glfwPollEvents();
		// Swap buffers
		glfwSwapBuffers(window);
	}
	else 
	{
		obj.displayCallback(window);
	}	

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (key == GLFW_KEY_F1)
		{
			orbitObj = obj.returnOrbit();
			scalingObj = obj.returnScaling();
			translateObj = obj.returnTranslate();
			toWorldObj = obj.returnToWorld();
			obj = obj1;
			obj.setOrbit(orbitObj);
			obj.setScaling(scalingObj);
			obj.setTranslate(translateObj);
			obj.setToWorld(toWorldObj);
		}
		else if (key == GLFW_KEY_F2)
		{
			orbitObj = obj.returnOrbit();
			scalingObj = obj.returnScaling();
			translateObj = obj.returnTranslate();
			toWorldObj = obj.returnToWorld();
			obj = obj2;
			obj.setOrbit(orbitObj);
			obj.setScaling(scalingObj);
			obj.setTranslate(translateObj);
			obj.setToWorld(toWorldObj);
		}
		else if (key == GLFW_KEY_F3)
		{
			orbitObj = obj.returnOrbit();
			scalingObj = obj.returnScaling();
			translateObj = obj.returnTranslate();
			toWorldObj = obj.returnToWorld();
			obj = obj3;
			obj.setOrbit(orbitObj);
			obj.setScaling(scalingObj);
			obj.setTranslate(translateObj);
			obj.setToWorld(toWorldObj);
		}
		else if (key == GLFW_KEY_P)
		{
			if (small == 1) 
			{
				mul = 2.0;
				obj.pSize(psize * mul);
				psize = psize * mul;
				obj.setPointSize(psize);
				small = 0;
				rev = 0;
			}
			else
			{
				mul = 0.5;
				obj.pSize(psize * mul);
				psize = psize * mul;
				obj.setPointSize(psize);
			}
		}
		else if ((key == GLFW_KEY_RIGHT_SHIFT) || (key == GLFW_KEY_LEFT_SHIFT)) 
		{
			small = 1;
			rev = 1;
		}
		else if (key == GLFW_KEY_X)
		{
			if (rev == 1) 
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(dis, 0, 0, 1);
				locX = locX + mat[0];
				locY = locY + mat[1];
				locZ = locZ + mat[2];
				locXSam = locXSam + dis;
				obj.move(locX, locY, locZ);
				small = 0;
				rev = 0;
			}
			else 
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(dis, 0, 0, 1);
				locX = locX - mat[0];
				locY = locY - mat[1];
				locZ = locZ - mat[2];
				locXSam = locXSam - dis;
				obj.move(locX, locY, locZ);
			}
		}
		else if (key == GLFW_KEY_M)
		{

			if (rev == 1) 
			{
				small = 0;
				rev = 0;
			}
			mode++;
		}
		else if (key == GLFW_KEY_Y)
		{
			if (rev == 1)
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(0, dis, 0, 1);
				locX = locX + mat[0];
				locY = locY + mat[1];
				locZ = locZ + mat[2];
				locYSam = locYSam + dis;
				obj.move(locX, locY, locZ);
				small = 0;
				rev = 0;
			}
			else
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(0, dis, 0, 1);
				locX = locX - mat[0];
				locY = locY - mat[1];
				locZ = locZ - mat[2];
				locYSam = locYSam - dis;
				obj.move(locX, locY, locZ);
			}
		}
		else if (key == GLFW_KEY_Z)
		{
			if (rev == 1)
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(0, 0, dis, 1);
				locX = locX + mat[0];
				locY = locY + mat[1];
				locZ = locZ + mat[2];
				locZSam = locZSam + dis;
				obj.move(locX, locY, locZ);
				small = 0;
				rev = 0;
			}
			else
			{
				glm::mat4 orb = obj.returnOrbit();
				glm::vec4 mat = glm::inverse(orb) * glm::vec4(0, 0, dis, 1);
				locX = locX - mat[0];
				locY = locY - mat[1];
				locZ = locZ - mat[2];
				locZSam = locZSam - dis;
				obj.move(locX, locY, locZ);
			}
		}
		else if (key == GLFW_KEY_R) 
		{
			if (rev == 1) 
			{
				sizeo = 1;
				ang = 0;
				obj.resizeR();
				//obj.move(locXSam, locYSam, locZSam);
				obj.resize(sizeo);
				obj.orbitFunc(ang);
				small = 0;
				rev = 0;
			}
			else 
			{
				locX = 0;
				locY = 0;
				locZ = 0;
				obj.resetPos();
			}
		}
		else if (key == GLFW_KEY_O)
		{
			if (rev == 1)
			{
				ang = ang - 30;
				obj.orbitFunc(ang);
				small = 0;
				rev = 0;
			}
			else
			{
				ang = ang + 30;
				obj.orbitFunc(ang);
			}
		}
		else if (key == GLFW_KEY_S)
		{
			if (rev == 1)
			{
				mul = 2.0;
				obj.resize(sizeo * mul);
				sizeo = sizeo * mul;
				small = 0;
				rev = 0;
			}
			else
			{
				mul = 0.5;
				obj.resize(sizeo * mul);
				sizeo = sizeo * mul;
			}
		}
		else if (key == GLFW_KEY_F4)
		{
			// optimization which is the ec
			if (rev == 1)
			{
				obj.setEnableOptimation1((int)0);
				small = 0;
				rev = 0;
			}
			else 
			{
				obj.setEnableOptimation1((int)1);
			}
		}
		else if (key == GLFW_KEY_F5)
		{
			// optimization which is the ec
			if (rev == 1)
			{
				obj.setEnableOptimation2((int)0);
				small = 0;
				rev = 0;
			}
			else
			{
				obj.setEnableOptimation2((int)1);
			}
		}
	}
}
