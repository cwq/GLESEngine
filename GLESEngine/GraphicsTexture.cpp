#include "GraphicsTexture.h"

#include <stdlib.h>
#include <string.h>
#include <IL/il.h>

#include "LogHelper.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GraphicsTexture::GraphicsTexture(const char* pPath)
	: path(pPath), mTextureId(0),
	mWidth(0), mHeight(0) {
	mFormat = GL_RGBA;
	ilInit();
	ILuint devilError = ilGetError();
	if(devilError != IL_NO_ERROR)
		LOGE("ilInit error: %i\n", devilError);
	//set lower-left is origin
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

GraphicsTexture::~GraphicsTexture() {
	ilShutDown();
}

int32_t GraphicsTexture::getHeight() {
	return mHeight;
}

int32_t GraphicsTexture::getWidth() {
	return mWidth;
}

bool GraphicsTexture::loadWidthHeight() {
	ILuint image = ilGenImage();
	ilBindImage(image);
	ilLoadImage(path);

	ILuint devilError = ilGetError();
	if(devilError != IL_NO_ERROR) {
		LOGE("ilLoadImage error: %i\n", devilError);
		return false;
	}
	
	mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	return true;
}

uint8_t* GraphicsTexture::loadImage() {
	/*
	ILuint image = ilGenImage();
	ilBindImage(image);
	ilLoadImage(path);

	ILuint devilError = ilGetError();
	if(devilError != IL_NO_ERROR) {
		LOGE("ilLoadImage error: %i\n", devilError);
		return NULL;
	}

	mWidth = ilGetInteger(IL_IMAGE_WIDTH);
	mHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	// Make sure the image is in RGBA format.
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	// This should always be four.
	int bpp = ilGetInteger(IL_IMAGE_BPP);
	if (bpp != 4) {
		LOGE("Could not convert image! bpp:%i\n", bpp);
		return NULL;
	}

	int imgSize = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
	uint8_t* lImageBuffer = new uint8_t[imgSize];
	memcpy(lImageBuffer, ilGetData(), imgSize);

	ilDeleteImage(image);
	*/

	int n;
	unsigned char* lImageBuffer = stbi_load(path, &mWidth, &mHeight, &n, 4);

	return lImageBuffer;
}

int GraphicsTexture::load() {
	uint8_t* lImageBuffer = loadImage();
	if (lImageBuffer == NULL) {
		return 0;
	}
	// Creates a new OpenGL texture.
	GLenum lErrorResult;
	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);
	// Set-up texture properties.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Loads image data into OpenGL.
	glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0,
		mFormat, GL_UNSIGNED_BYTE, lImageBuffer);

	//delete[] lImageBuffer;
	stbi_image_free(lImageBuffer);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (glGetError() != GL_NO_ERROR) {
		LOGE("Error loading texture into OpenGL.");
		unload();
		return 0;
	}
	return 1;
}

void GraphicsTexture::unload() {
	if (mTextureId != 0) {
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}
	mWidth = 0; mHeight = 0; mFormat = 0;
}

GLuint GraphicsTexture::getTextureId() {
	return mTextureId;
}
