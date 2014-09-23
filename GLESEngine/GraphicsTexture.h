#ifndef GRAPHICSTEXTURE_H
#define GRAPHICSTEXTURE_H

#include "Resource.h"

#include <stdint.h>
#include <GLES2/gl2.h>
#include <png.h>

typedef struct {
	uint8_t* pixelData;
	int imageWidth;
	int imageHeight;
}ImageInfo;



typedef struct {
	uint8_t* data;
	int size;
	int offset;
}ImageSource;

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
	static void callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize);
	const char* path;
	GLuint mTextureId;
	int32_t mWidth, mHeight;
	GLint mFormat;

};

#endif // !GRAPHICSTEXTURE_H
