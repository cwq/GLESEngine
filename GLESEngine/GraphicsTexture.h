#ifndef GRAPHICSTEXTURE_H
#define GRAPHICSTEXTURE_H

#include "Resource.h"

#include <stdint.h>
#include <gl/glew.h>

class GraphicsTexture
{
public:
	GraphicsTexture(const char* pPath);
	~GraphicsTexture();

	int32_t getHeight();
	int32_t getWidth();

	int load();
	void unload();
	GLuint getTextureId();
	bool loadWidthHeight();

protected:
	uint8_t* loadImage();

private:
	const char* path;
	GLuint mTextureId;
	int32_t mWidth, mHeight;
	GLint mFormat;

};

#endif // !GRAPHICSTEXTURE_H
