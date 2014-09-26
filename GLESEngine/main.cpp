#include <iostream>
#include <esUtil.h>
#include "Scene.h"
#include "Line.h"
#include "RectangleTexture.h"
#include "CutRectangle.h"

static Scene* scene = NULL;
static CutRectangle* cutRect = NULL;
static RectangleTexture* backRectTexture = NULL;
static RectangleTexture* upLayer = NULL;

void Draw ( ESContext *esContext )
{
	scene->onDrawFrame();
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

int main() {
	std::cout<<"a"<<std::endl;
	
	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, "hello", 1024, 768, ES_WINDOW_RGB);

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

	esRegisterDrawFunc(&esContext, Draw);
	esMainLoop(&esContext);
	return 0;
}