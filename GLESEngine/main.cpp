#include <iostream>
#define GLFW_INCLUDE_ES2
#include <glfw3.h>
#include "Scene.h"
#include "Line.h"
#include "RectangleTexture.h"
#include "CutRectangle.h"

static Scene* scene = NULL;
static CutRectangle* cutRect = NULL;
static RectangleTexture* backRectTexture = NULL;
static RectangleTexture* upLayer = NULL;

void doCut() {
	RectangleTexture* tempTexture = cutRect->getCutTempTexture();
	scene->addObj(tempTexture, 25);
	RectangleTexture* finalTexture = cutRect->getCutFinalTexture();
	cutRect->doCutAnimation(tempTexture, finalTexture);

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
}

void doReset() {
	scene->removeObj(backRectTexture);
	scene->removeObj(upLayer);
	backRectTexture = new RectangleTexture("view1.png");
	upLayer = new RectangleTexture(0.0f, 0.0f, backRectTexture->getHalfW() * 2,
		backRectTexture->getHalfH() * 2, "");
	scene->addObj(upLayer, 20);
	scene->addObj(backRectTexture, 10);
	cutRect->setBackRect(backRectTexture, upLayer, false);
}

int main() {
	std::cout<<"a"<<std::endl;
	
	GLFWwindow* window;
	/* Initialize the library */
	if(!glfwInit()) {
		std::cout<<"glfwInit error"<<std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 768, "Engine", NULL, NULL);
	if(!window) {
		glfwTerminate();
		std::cout<<"glfwCreateWindow error"<<std::endl;
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	scene = new Scene();
	backRectTexture = new RectangleTexture("view1.png");
	upLayer = new RectangleTexture(0.0f, 0.0f,
		backRectTexture->getHalfW() * 2, backRectTexture->getHalfH() * 2,
		"");
	cutRect = new CutRectangle(backRectTexture, upLayer);

	scene->addObj(cutRect, 30);
	scene->addObj(upLayer, 20);
	scene->addObj(backRectTexture, 10);

	scene->onSurfaceChanged(1024, 768);
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
	
	glfwTerminate();
	return 0;
}