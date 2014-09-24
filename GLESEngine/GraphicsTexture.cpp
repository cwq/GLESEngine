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
	FILE *fp;  
	png_structp png_ptr;  
	png_infop info_ptr;  
	png_bytep* row_pointers;  
	const int PNG_BYTES_TO_CHECK = 8;
	char buf[PNG_BYTES_TO_CHECK];  
	int temp;  

	fp = fopen( path, "rb" );  
	if( fp == NULL ) {   
		LOGE("%s no exit", path);
		return false/* 你的返回值 */;  
	}  

	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );  
	info_ptr = png_create_info_struct( png_ptr );  

	setjmp( png_jmpbuf(png_ptr) );   
	/* 读取PNG_BYTES_TO_CHECK个字节的数据 */  
	temp = fread( buf, 1, PNG_BYTES_TO_CHECK, fp );  
	/* 若读到的数据并没有PNG_BYTES_TO_CHECK个字节 */  
	if( temp < PNG_BYTES_TO_CHECK ) {  
		fclose(fp);  
		png_destroy_read_struct( &png_ptr, &info_ptr, 0);  
		return false/* 你的返回值 */;  
	}  
	/* 检测数据是否为PNG的签名 */  
	temp = png_sig_cmp( (png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK );  
	/* 如果不是PNG的签名，则说明该文件不是PNG文件 */  
	if( temp != 0 ) {  
		fclose(fp);  
		png_destroy_read_struct( &png_ptr, &info_ptr, 0);  
		return false/* 你的返回值 */;  
	}  

	/* 复位文件指针 */  
	rewind( fp );  
	/* 开始读文件 */  
	png_init_io( png_ptr, fp );   
	/* 读取PNG图片信息 */  
	png_read_png( png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0 );  
	/* 获取图像的宽高 */  
	mWidth = png_get_image_width( png_ptr, info_ptr );  
	mHeight = png_get_image_height( png_ptr, info_ptr );
	png_destroy_read_struct( &png_ptr, &info_ptr, 0); 
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
    fread(png_header, 1, 8, file);
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


    /* **************************************************
     * The high-level read interface in libpng (http://www.libpng.org/pub/png/libpng-1.2.5-manual.html)
     * **************************************************
     */
       png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);
	   mWidth = png_get_image_width( png_ptr, info_ptr );  
	   mHeight = png_get_image_height( png_ptr, info_ptr );
	   mFormat = png_get_color_type( png_ptr, info_ptr );
       unsigned char* rgba = new unsigned char[mWidth * mHeight * 4];  //each pixel(RGBA) has 4 bytes
       png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);
	   LOGI("%i \n", png_get_rowbytes(png_ptr, info_ptr));

       //Original PNG pixel data stored from top-left corner, BUT OGLES Texture drawing is from bottom-left corner
//        int pos = 0;
//        for(int row = 0; row < mHeight; row++)
//        {
//            for(int col = 0; col < (4 * mWidth); col += 4)
//            {
//               rgba[pos++] = row_pointers[row][col];     // red
//               rgba[pos++] = row_pointers[row][col + 1]; // green
//               rgba[pos++] = row_pointers[row][col + 2]; // blue
//               rgba[pos++] = row_pointers[row][col + 3]; // alpha
//            }
//        }


       //unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing...
       int pos = (mWidth * mHeight * 4) - (4 * mWidth);
       for(int row = 0; row < mHeight; row++)
       {
          for(int col = 0; col < (4 * mWidth); col += 4)
          {
              rgba[pos++] = row_pointers[row][col];        // red
              rgba[pos++] = row_pointers[row][col + 1]; // green
              rgba[pos++] = row_pointers[row][col + 2]; // blue
              rgba[pos++] = row_pointers[row][col + 3]; // alpha
          }
          pos=(pos - (mWidth * 4)*2); //move the pointer back two rows
       }

	   bool lTransparency = false;
	   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		   lTransparency = true;
	   }

	   switch (mFormat) {
	   case PNG_COLOR_TYPE_PALETTE:
		   mFormat = lTransparency ? GL_RGBA : GL_RGB;
		   break;
	   case PNG_COLOR_TYPE_RGB:
		   mFormat = lTransparency ? GL_RGBA : GL_RGB;
		   break;
	   case PNG_COLOR_TYPE_RGBA:
		   mFormat = GL_RGBA;
		   break;
	   case PNG_COLOR_TYPE_GRAY:
		   mFormat = lTransparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
		   break;
	   case PNG_COLOR_TYPE_GA:
		   mFormat = GL_LUMINANCE_ALPHA;
		   break;
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
