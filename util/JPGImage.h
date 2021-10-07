#pragma once
#include <minwindef.h>
#include <cstdio>
#include <pngconf.h>

class JPGImage
{
public:
	JPGImage();
	~JPGImage();

	BOOL LoadJPGImage(const char* filename);
	DWORD Blt(char* dest, int destX, int destY, DWORD screenWidth, DWORD screenHeight, DWORD pitch, DWORD bytePerPixel, int srcStartX, int srcStartY, int srcWidth, int srcHeight);
	void Destroy();

	inline DWORD getWidth() const;
	inline DWORD getHeight() const;
	inline char* GetRawImage();
	inline DWORD GetPixel(DWORD x, DWORD y) const;

private:
	DWORD mWidth;
	DWORD mHeight;
	char* mRawImage;
};

DWORD JPGImage::getWidth() const
{
	return mWidth;
}

DWORD JPGImage::getHeight() const
{
	return mHeight;
}

char* JPGImage::GetRawImage()
{
	return mRawImage;
}

DWORD JPGImage::GetPixel(DWORD x, DWORD y) const
{
	DWORD dwColor = *(DWORD*)(mRawImage + ((x + y * mWidth) * 4));
	return dwColor;
}
