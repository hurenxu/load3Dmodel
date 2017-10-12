#include "OBJObject.h"

static int window_width = 640, window_height = 480;
static float two = 2.0;
static float * pixels = new float[window_width * window_height * 3];
// depth buffering and also called z buffering (comparing the depth of the object make sure the back is not
// overlapping the front)
static float * z_buffer = new float[window_width * window_height];

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	translate = glm::mat4(1.0f);
	scaling = glm::mat4(1.0f);
	this->angle = 0.0f;
	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float xn, yn, zn;  // vertex normal coordinates
	float r, g, b;  // vertex color
	int c1, c2;    // characters read from file
	int c3;
	errno_t res;

	res = fopen_s(&fp, filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) {std::cerr << "error loading file" << std::endl; exit(-1); }  // just in case the file can't be found or is corrupt
	for (int i = 0; (c1 = fgetc(fp)) != EOF && (c2 = fgetc(fp)) != EOF; i++)
	{
		//c1 = fgetc(fp);
		//c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf_s(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 'n')) 
		{
			c3 = fgetc(fp);
			if(c3 == ' ') 
			{
				fscanf_s(fp, "%f %f %f", &xn, &yn, &zn);
				normals.push_back(glm::vec3(xn, yn, zn));
			}
		}
		/**else if ((c1 == 'f') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
		}*/
		else {
			//break;
		}
	}
	// read normal data accordingly

	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	model = orbit * translate * toWorld * scaling;
	glMultMatrixf(&(model[0][0]));
	// set up the depth mode
	glEnable(GL_DEPTH_TEST);

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		float base = sqrt(normals[i].x * normals[i].x + normals[i].y * normals[i].y + normals[i].z * normals[i].z);
		glColor3f((normals[i].x / base + 1) / 2, (normals[i].y / base + 1) / 2, (normals[i].z / base + 1) / 2);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
	spin(1.0f);
	calculateMVP();
}

void OBJObject::spin(float deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the cube
	this->toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::orbitFunc(float ang)
{
	this->orbit = glm::rotate(glm::mat4(1.0f), ang / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f));
}

void OBJObject::resize(float size)
{
	glm::vec3 scale = glm::vec3(size, size, size);
	this->scaling = glm::scale(glm::mat4(1.0f), scale);
}

void OBJObject::pSize(float size)
{
	glPointSize(size);
}

void OBJObject::move(float locX, float locY, float locZ)
{
	this->locZ = locZ;
	this->locX = locX;
	this->locY = locY;
	this->translate = glm::translate(glm::mat4(1.0f), glm::vec3(this->locX, this->locY, this->locZ));
}

void OBJObject::resetPos() 
{
	this->locX = 0;
	this->locY = 0;
	this->locZ = 0;
	this->translate = glm::translate(glm::mat4(1.0f), glm::vec3(this->locX, this->locY, this->locZ));
}

glm::mat4 OBJObject::returnModel() 
{
	return model;
}

void OBJObject::setModel(glm::mat4 model)
{
	this->model = model;
}

void OBJObject::setTranslate(glm::mat4 translate) 
{
	this->translate = translate;
}

void OBJObject::setScaling(glm::mat4 scaling) 
{
	this->scaling = scaling;
}

void OBJObject::setToWorld(glm::mat4 toWorld) 
{
	this->toWorld = toWorld;
}

void OBJObject::setOrbit(glm::mat4 orbit) 
{
	this->orbit = orbit;
}

void OBJObject::setWindow(int width, int height) 
{
	window_width = width;
	window_height = height;
}

glm::mat4 OBJObject::returnOrbit()
{
	return orbit;
}

glm::mat4 OBJObject::returnTranslate()
{
	return translate;
}

glm::mat4 OBJObject::returnToWorld()
{
	return toWorld;
}

glm::mat4 OBJObject::returnScaling()
{
	return scaling;
}

// Clear frame buffer
void OBJObject::clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i < window_width*window_height; ++i)
	{
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
		z_buffer[i] = 0;
	}
}

// Draw a point into the frame buffer
void OBJObject::drawPoint(int x, int y, float r, float g, float b, int size)
{
	int offset = y * window_width * 3 + x * 3;
	int check = window_height * window_width * 3;
	if (size == 0) 
	{
		if (offset > check || offset < 0) {
		
		}
		else {
			pixels[offset] = r;
			pixels[offset + 1] = g;
			pixels[offset + 2] = b;
		}
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				int ps = i *window_width * 3 + j * 3;
				if (offset + ps > check || offset < 0) {
					break;
				}
				pixels[offset + ps] = r;
				pixels[offset + 1 + ps] = g;
				pixels[offset + 2 + ps] = b;
			}
		}
	}
}

float OBJObject::resizePointEC1(float Z, float range) 
{
	
	return ((float)(10.0 / Z) + range);
}

void OBJObject::rasterize()
{
	calculateMVP();
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
	for (int i = 0; i < vertices.size(); i++)
	{	
		auto normalized = glm::normalize(normals[i]);
		normalized = (normalized + glm::vec3(1.0f)) * 0.5f;
		glm::vec4 homogenousVertices(vertices[i], 1);
		glm::vec4 p(mvp * homogenousVertices);
		if (p.x / p.w >= 0 && p.x / p.w < window_width && p.y / p.w >= 0 && p.y / p.w < window_height && p.z > 0) {
			float s = pointSize;
			if (enableEC1 == 1) {
				s = resizePointEC1(p.z, pointSize);
				//std::cout << s << std::endl;
			}

			if (enableEC2 == 1) {
				int X = p.x / p.w;
				int Y = p.y / p.w;
				if (p.z < z_buffer[Y * window_width + X] || z_buffer[Y * window_width + X] == 0)
				{
					z_buffer[Y * window_width + X] = p.z;
					drawPoint(X, Y, normalized.x, normalized.y, normalized.z, s);
				}
			}
			else {
				drawPoint(p.x / p.w, p.y / p.w, normalized.x, normalized.y, normalized.z, s);
			}
		}
	}
}

void OBJObject::calculateMVP()
{
	model = orbit * translate * toWorld * scaling;
	M = model;
	C_INV = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	P = glm::perspective(glm::radians(60.0f), (float)window_width / (float)window_height, 1.0f, 1000.0f);
	float x0 = 0;
	float y0 = 0;
	float x1 = (float)window_width;
	float y1 = (float)window_height;
	D = glm::mat4(1.0f);
	D[0] = glm::vec4((x1-x0)/2.0, 0, 0, 0);
	D[1] = glm::vec4(0, (y1 - y0)/2.0, 0, 0);
	D[2] = glm::vec4(0, 0, 1.0/2.0, 0);
	D[3] = glm::vec4((x0+x1)/2.0, (y0+y1)/2.0, 1.0/2.0, 1);
	mvp = D * P * C_INV * M;
}

void OBJObject::resizeCallback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	delete[] pixels;
	pixels = new float[window_width * window_height * 3];
	calculateMVP();
}

void OBJObject::displayCallback(GLFWwindow* window)
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

void OBJObject::setHeight(int height) 
{
	window_height = height;
}

void OBJObject::setWidth(int width) 
{
	window_width = width;
}

void OBJObject::setPointSize(float pointSize)
{
	this->pointSize = pointSize;
}

float* OBJObject::getPixels() 
{
	return pixels;
}

void OBJObject::setEnableOptimation1(int enable) 
{
	enableEC1 = enable;
}

void OBJObject::setEnableOptimation2(int enable) 
{
	enableEC2 = enable;
}

void OBJObject::resizeR() {
	auto translateMat = glm::vec3(model[3][0], model[3][1], model[3][2]);
	translate =  glm::translate(glm::mat4(1.0f), translateMat);
}