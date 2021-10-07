#include <png.h>
#include <stdlib.h>
#include <Windows.h>

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
	BOOL bResult = FALSE;
	FILE* fp;

	png_structp png;
	png_infop info;
	png_byte colorType;
	png_byte bitDepth;
	unsigned int y;
	png_bytep* rowPointers;

	errno_t err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		goto lb_return;
	}

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == nullptr)
	{
		goto lb_return;
	}

	info = png_create_info_struct(png);
	if (info == nullptr)
	{
		goto lb_return;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		goto lb_return;
	}

	png_init_io(png, fp);
	png_read_info(png, info);
	mWidth = png_get_image_width(png, info);
	mHeight = png_get_image_height(png, info);
	colorType = png_get_color_type(png, info);
	bitDepth = png_get_bit_depth(png, info);

	/* Read any color_type into 8bit depth, RGBA format. */
	/* See http://www.libpng.org/pub/png/libpng-manual.txt */
	if (bitDepth == 16)
	{
		png_set_strip_16(png);
	}

	if (colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png);
	}

	/* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
	if (colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
	{
		png_set_expand_gray_1_2_4_to_8(png);
	}

	if (png_get_valid(png, info, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png);
	}

	/* These color_type don't have an alpha channel then fill it with 0xff. */
	if (colorType == PNG_COLOR_TYPE_RGB ||
		colorType == PNG_COLOR_TYPE_GRAY ||
		colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	if (colorType == PNG_COLOR_TYPE_GRAY ||
		colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(png);
	}

	png_read_update_info(png, info);
	//rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * mHeight);
	rowPointers = new png_bytep[mHeight];

	for (y = 0; y < mHeight; y++)
	{
		size_t rowbytes = png_get_rowbytes(png, info);
		//rowPointers[y] = (png_byte*)malloc(rowbytes);
		rowPointers[y] = new png_byte[rowbytes];
	}

	png_read_image(png, rowPointers);
	{
		mRawImage = new char[mWidth * mHeight * 4];
		
		char* dest = mRawImage;

		for (int y = 0; y < (int)mHeight; y++)
		{
			png_bytep src = rowPointers[y];

			for (int x = 0; x < (int)mWidth; x++)
			{
				DWORD r = (DWORD)*src;
				DWORD g = (DWORD)*(src + 1);
				DWORD b = (DWORD)*(src + 2);
				DWORD a = (DWORD)*(src + 3);

				DWORD color = (a << 24) + (r << 16) + (g << 8) + (b);
				*(DWORD*)dest = color;

				src += 4;
				dest += 4;
			}
		}
	}

	for (int y = 0; y < mHeight; y++)
	{
		delete rowPointers[y];
	}
	delete[] rowPointers;

	bResult = TRUE;

lb_return:
	if (fp != nullptr)
	{
		fclose(fp);
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
