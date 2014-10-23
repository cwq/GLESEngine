#include <iostream>
#include "Scene.h"
#include "Line.h"
#include "RectangleTexture.h"
#include "CutRectangle.h"
#include "MoveAnimation.h"
#include "ComplexAnimation.h"
#include "RotateAnimation.h"
//#include <gl/glew.h>
#include <glfw3.h>

static const int WIDTH = 768;
static const int HEIGHT = 1024;

static std::string BACK_IMAGE1 = "view1.png";
static std::string BACK_IMAGE2 = "view2.png";
static std::string BACK_IMAGE3 = "first.png";
static std::string BACK_IMAGE4 = "second.png";
static std::string BACK_IMAGE5 = "tou.png";
static std::string BACK_IMAGE6 = "tga.tga";

static Scene* scene = NULL;

static bool isDown = false;

static RectangleTexture* texture1 = NULL;
static RectangleTexture* texture2 = NULL;
static RectangleTexture* texture3 = NULL;
static RectangleTexture* texture4 = NULL;
static RectangleTexture* texture5 = NULL;
static RectangleTexture* texture6 = NULL;

static void error_callback(int error, const char* description) {
	std::cout<<description<<stderr<<std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//key cut reset
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {

	} else if(key == GLFW_KEY_R && action == GLFW_PRESS) {

	}
}

static void getGLCursorPos(double cx, double cy, float *glx, float *gly) {
	if(HEIGHT > WIDTH) {
		*glx = (cx / WIDTH) * 2 - 1;
		*gly = -((cy / HEIGHT) * 2 - 1) * HEIGHT / WIDTH;
	} else {
		*glx = ((cx / WIDTH) * 2 - 1) * WIDTH / HEIGHT;
		*gly = -((cy / HEIGHT) * 2 - 1);
	}
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		//left mouse button
		switch (action)
		{
		case GLFW_PRESS:
			isDown = true;
			double cx, cy;
			glfwGetCursorPos(window, &cx, &cy);
			float glx, gly;
			getGLCursorPos(cx, cy, &glx, &gly);
			
			break;;
		case GLFW_RELEASE:
			isDown = false;
			
			break;
		default:
			break;
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double cx, double cy) {
	if(isDown) {
		//move
		float glx, gly;
		getGLCursorPos(cx, cy, &glx, &gly);
		
	}
}

int main() {
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	/* Initialize the library */
	if(!glfwInit()) {
		std::cout<<"glfwInit error"<<std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", NULL, NULL);
	if(!window) {
		glfwTerminate();
		std::cout<<"glfwCreateWindow error"<<std::endl;
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	/* in win32 to use opengl hight level must call glewInit */
	GLenum err = glewInit();
	if (GLEW_OK != err){
		std::cout<<"GLEW Error: "<<glewGetErrorString(err)<<std::endl;
		return -1;
	}

	scene = new Scene();

	float s = 0.9f;
	texture1 = new RectangleTexture(-0.5f * s, 0.5f * s, 1.0f * s, 1.0f * s, BACK_IMAGE1);
	texture2 = new RectangleTexture(0.5f * s, 0.75f * s, 1.0f * s, 0.5f * s, BACK_IMAGE2);
	texture3 = new RectangleTexture(0.5f * s, 0.25f * s, 1.0f * s, 0.5f * s, BACK_IMAGE3);
	texture4 = new RectangleTexture(-0.5f * s, -0.5f * s, 1.0f * s, 1.0f * s, BACK_IMAGE4);
	texture5 = new RectangleTexture(0.5f * s, -0.25f * s, 1.0f * s, 0.5f * s, BACK_IMAGE5);
	texture6 = new RectangleTexture(0.5f * s, -0.75f * s, 1.0f * s, 0.5f * s, BACK_IMAGE6);

	scene->addObj(texture1, 10);
	scene->addObj(texture2, 10);
	scene->addObj(texture3, 10);
	scene->addObj(texture4, 10);
	scene->addObj(texture5, 10);
	scene->addObj(texture6, 10);

	ComplexAnimation* animation1 = new ComplexAnimation(2);
	animation1->setRepeat(true);
	animation1->addAnimation(MoveAnimation::move(2, Point(-0.5f * s, 0.5f * s), Point(0.5f * s, 0.5f * s)));
	animation1->addAnimation(RotateAnimation::rotate(2, texture1->getRotateMatrix(), 360, 1, 1, 1));

	ComplexAnimation* animation2 = new ComplexAnimation(2);
	animation2->setRepeat(true);
	animation2->addAnimation(MoveAnimation::move(2, Point(0.5f * s, 0.75f * s), Point(-0.5f * s, 0.75f * s)));
	animation2->addAnimation(RotateAnimation::rotate(2, texture2->getRotateMatrix(), 360, 1, 1, 1));

	ComplexAnimation* animation3 = new ComplexAnimation(2);
	animation3->setRepeat(true);
	animation3->addAnimation(MoveAnimation::move(2, Point(0.5f * s, 0.25f * s), Point(-0.5f * s, 0.25f * s)));
	animation3->addAnimation(RotateAnimation::rotate(2, texture3->getRotateMatrix(), 360, 1, 1, 1));

	ComplexAnimation* animation4 = new ComplexAnimation(2);
	animation4->setRepeat(true);
	animation4->addAnimation(MoveAnimation::move(2, Point(-0.5f * s, -0.5f * s), Point(0.5f * s, -0.5f * s)));
	animation4->addAnimation(RotateAnimation::rotate(2, texture4->getRotateMatrix(), 360, 1, 1, 1));

	ComplexAnimation* animation5 = new ComplexAnimation(2);
	animation5->setRepeat(true);
	animation5->addAnimation(MoveAnimation::move(2, Point(0.5f * s, -0.25f * s), Point(-0.5f * s, -0.25f * s)));
	animation5->addAnimation(RotateAnimation::rotate(2, texture5->getRotateMatrix(), 360, 1, 1, 1));

	ComplexAnimation* animation6 = new ComplexAnimation(2);
	animation6->setRepeat(true);
	animation6->addAnimation(MoveAnimation::move(2, Point(0.5f * s, -0.75f * s), Point(-0.5f * s, -0.75f * s)));
	animation6->addAnimation(RotateAnimation::rotate(2, texture6->getRotateMatrix(), 360, 1, 1, 1));

	texture1->setAnimation(animation1);
	texture2->setAnimation(animation2);
	texture3->setAnimation(animation3);
	texture4->setAnimation(animation4);
	texture5->setAnimation(animation5);
	texture6->setAnimation(animation6);

	scene->onSurfaceChanged(WIDTH, HEIGHT);
	scene->onSurfaceCreated();

	/* Loop until the user closes the window */
	while(!glfwWindowShouldClose(window)) {
		/* Render here */
		scene->onDrawFrame();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}