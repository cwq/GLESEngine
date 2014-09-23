#include <iostream>
#include "Scene.h"
#include "Line.h"
#include <esUtil.h>

Scene* scene = NULL;

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
	BaseObject* temp = new Line(Point(-0.8f, 0.8f), Point(0.8f, -1));
	scene->addObj(temp);

	scene->onSurfaceChanged(1024, 768);
	scene->onSurfaceCreated();

	esRegisterDrawFunc(&esContext, Draw);
	esMainLoop(&esContext);
	return 0;
}