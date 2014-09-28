#include <iostream>
#include "Scene.h"
#include "Line.h"
#include "RectangleTexture.h"
#include "CutRectangle.h"
//#include <gl/glew.h>
#include <glfw3.h>

static const int WIDTH = 768;
static const int HEIGHT = 1024;

static Scene* scene = NULL;
static CutRectangle* cutRect = NULL;
static RectangleTexture* backRectTexture = NULL;
static RectangleTexture* upLayer = NULL;

static bool isDown = false;

static RectangleTexture* tempTexture = NULL;
static RectangleTexture* finalTexture = NULL;
static bool isCut = false;

static void doCutAnimation() {
	tempTexture = cutRect->getCutTempTexture();
	scene->addObj(tempTexture, 25);
	finalTexture = cutRect->getCutFinalTexture();
	cutRect->doCutAnimation(tempTexture, finalTexture);

	isCut = true;

// 	scene->removeObj(backRectTexture);
// 	scene->removeObj(upLayer);
// 
// 	backRectTexture = finalTexture;
// 	upLayer = new RectangleTexture(0.0f, 0.0f, backRectTexture->getHalfW() * 2,
// 		backRectTexture->getHalfH() * 2, "");
// 
// 	scene->addObj(upLayer, 20);
// 	scene->addObj(backRectTexture, 10);
// 	cutRect->setBackRect(backRectTexture, upLayer);
// 
// 	scene->removeObj(tempTexture);
// 	if (tempTexture != NULL) {
// 		delete tempTexture;
// 		tempTexture = NULL;
// 
}

static void doCut() {
	//do when doCutAnimation animation finished
	scene->removeObj(backRectTexture);
	scene->removeObj(upLayer);

	backRectTexture = finalTexture;
	upLayer = new RectangleTexture(0.0f, 0.0f, backRectTexture->getHalfW() * 2,
		backRectTexture->getHalfH() * 2, "");

	scene->addObj(upLayer, 20);
	scene->addObj(backRectTexture, 10);
	cutRect->setBackRect(backRectTexture, upLayer);

	scene->removeObj(tempTexture);
	if (tempTexture != NULL) {
		delete tempTexture;
		tempTexture = NULL;
	}

	isCut = false;
}

static void doReset() {
	scene->removeObj(backRectTexture);
	scene->removeObj(upLayer);
	backRectTexture = new RectangleTexture("view1.png");
	upLayer = new RectangleTexture(0.0f, 0.0f, backRectTexture->getHalfW() * 2,
		backRectTexture->getHalfH() * 2, "");
	scene->addObj(upLayer, 20);
	scene->addObj(backRectTexture, 10);
	cutRect->setBackRect(backRectTexture, upLayer, false);
}

static void error_callback(int error, const char* description) {
	std::cout<<description<<stderr<<std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//key cut reset
	if(!isCut) {
		if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			doCutAnimation();
		} else if(key == GLFW_KEY_R && action == GLFW_PRESS) {
			doReset();
		}
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
	if(!isCut) {
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
				cutRect->touchDown(glx, gly);
				break;;
			case GLFW_RELEASE:
				isDown = false;
				cutRect->touchUp();
				break;
			default:
				break;
			}
		}
	}
}

static void cursor_position_callback(GLFWwindow* window, double cx, double cy) {
	if(isDown && !isCut) {
		//move
		float glx, gly;
		getGLCursorPos(cx, cy, &glx, &gly);
		cutRect->touchMove(glx, gly);
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
	backRectTexture = new RectangleTexture("view1.png");
	upLayer = new RectangleTexture(0.0f, 0.0f,
		backRectTexture->getHalfW() * 2, backRectTexture->getHalfH() * 2,
		"");
	cutRect = new CutRectangle(backRectTexture, upLayer);

	scene->addObj(cutRect, 30);
	scene->addObj(upLayer, 20);
	scene->addObj(backRectTexture, 10);

	scene->onSurfaceChanged(WIDTH, HEIGHT);
	scene->onSurfaceCreated();

	/* Loop until the user closes the window */
	while(!glfwWindowShouldClose(window)) {
		/* Render here */
		scene->onDrawFrame();

		if(isCut) {
			//cut animation finished
			if(!backRectTexture->isRunAnimation()) {
				doCut();
			}
		}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}