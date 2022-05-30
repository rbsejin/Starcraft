#include <lodepng.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include "JPGImage.h"

JPGImage::JPGImage()
	: mWidth(0)
	, mHeight(0)
	, mRawImage(nullptr)
{
}

JPGImage::~JPGImage()
{
	Destroy();
}

BOOL JPGImage::LoadJPGImage(const char* filename)
{
	//decode
	unsigned error;
	unsigned char* image = nullptr;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char* png = nullptr;
	size_t pngsize;

	error = lodepng_load_file(&png, &pngsize, filename);
	if (!error) error = lodepng_decode32(&image, &width, &height, png, pngsize);
	if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

	free(png);

	mWidth = (DWORD)width;
	mHeight = (DWORD)height;

	mRawImage = new char[mWidth * mHeight * 4];

	int w = mWidth;
	int h = mHeight;
	{
		int inputChannels = 3;
		int outputChannels = 3;

		unsigned char* src = image;
		char* dest = mRawImage;

		for (int y = 0; y < (int)mHeight; y++)
		{
			for (int x = 0; x < (int)mWidth; x++)
			{
				DWORD r = (DWORD)src[0];
				DWORD g = (DWORD)src[1];
				DWORD b = (DWORD)src[2];
				DWORD a = (DWORD)src[3];

				DWORD color = (a << 24) + (r << 16) + (g << 8) + (b);
				*(DWORD*)dest = color;

				src += 4;
				dest += 4;
			}
		}
	}

	free(image);

	if (error)
	{
		return FALSE;
	}

	return TRUE;
}

DWORD JPGImage::Blt(char* dest, int destX, int destY, DWORD screenWidth, DWORD screenHeight, DWORD pitch, DWORD bytePerPixel, int srcStartX, int srcStartY, int srcWidth, int srcHeight)
{
	return 0;
}

void JPGImage::Destroy()
{
	delete mRawImage;
	mRawImage = nullptr;
}
