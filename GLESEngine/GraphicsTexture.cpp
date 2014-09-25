#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphicsTexture.h"
#include "LogHelper.h"

GraphicsTexture::GraphicsTexture(const char* pPath)
	: path(pPath), mTextureId(0),
	mWidth(0), mHeight(0) {

}

GraphicsTexture::~GraphicsTexture() {

}

int32_t GraphicsTexture::getHeight() {
	return mHeight;
}

int32_t GraphicsTexture::getWidth() {
	return mWidth;
}

bool GraphicsTexture::loadWidthHeight() {
	char png_header[8];
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte color_type; 
	png_byte bit_depth;
	png_colorp palette; 

	/* open file and test for it being a png */
	FILE *file = fopen(path, "rb");
	if( file == NULL ) {   
		LOGE("%s no exit", path);
		return false/* 你的返回值 */;  
	}  
	if(fread(png_header, 1, 8, file) < 8)
	{
		LOGE("fread not 8 byte");
		fclose(file);
		return false;
	}
	if(png_sig_cmp((png_bytep)png_header, 0, 8))
	{
		LOGE("Not a PNG file...");
		fclose(file);
		return false;
	}
	/* initialise structures for reading a png file */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		LOGE("ReadPngFile: Failed to read the PNG file");
		fclose(file);
		return false;
	}
	//I/O initialisation methods
	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, 8);  //Required!!!

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	mWidth = png_get_image_width( png_ptr, info_ptr );  
	mHeight = png_get_image_height( png_ptr, info_ptr );
	png_destroy_read_struct( &png_ptr, &info_ptr, 0); 
	fclose(file);
	return true;
}

uint8_t* GraphicsTexture::loadImage() {
	char png_header[8];
    png_structp png_ptr;
    png_infop info_ptr;
    png_byte color_type; 
    png_byte bit_depth;
    png_colorp palette; 

    /* open file and test for it being a png */
    FILE *file = fopen(path, "rb");
	if( file == NULL ) {   
		LOGE("%s no exit", path);
		return NULL/* 你的返回值 */;  
	}  
    if(fread(png_header, 1, 8, file) < 8)
	{
		LOGE("fread not 8 byte");
		fclose(file);
		return NULL;
	}
    if(png_sig_cmp((png_bytep)png_header, 0, 8))
    {
        LOGE("Not a PNG file...");
        fclose(file);
		return NULL;
    }
    /* initialise structures for reading a png file */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        LOGE("ReadPngFile: Failed to read the PNG file");
        fclose(file);
		return NULL;
    }
    //I/O initialisation methods
    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);  //Required!!!

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	mWidth = png_get_image_width( png_ptr, info_ptr );  
	mHeight = png_get_image_height( png_ptr, info_ptr );
	mFormat = png_get_color_type( png_ptr, info_ptr );
	unsigned int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    unsigned char* rgba = new unsigned char[rowbytes * mHeight];  //each pixel(RGBA) has 4 bytes
    png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);

	for(int row = 0; row < mHeight; row++) {
		memcpy(rgba+(rowbytes*(mHeight-1-row)), row_pointers[row], rowbytes);
	}

	switch (mFormat) {
	case PNG_COLOR_TYPE_RGB:
		mFormat = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGBA:
		mFormat = GL_RGBA;
		break;
	default:
		LOGE("%i format not rgb or rgba", mFormat);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(file);
		return NULL;
	}

    //clean up after the read, and free any memory allocated
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(file);
    return rgba;
}

void GraphicsTexture::callback_read(png_structp png, png_bytep data, png_size_t length) {
	ImageSource* isource = (ImageSource*)png_get_io_ptr(png);

	if(isource->offset + length <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png,"pngReaderCallback failed");
	}
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

	delete[] lImageBuffer;

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
