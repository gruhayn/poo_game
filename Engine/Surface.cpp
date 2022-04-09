#include "Surface.h"
#include <assert.h> 
#include "ChiliWin.h"
#include <fstream>

Surface::Surface(const std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);
	BITMAPFILEHEADER bmFileHeader;

	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount==32);
	assert(bmInfoHeader.biCompression == BI_RGB);
	
	int yStart;
	int yEnd;
	int dy;

	width = bmInfoHeader.biWidth;

	if (bmInfoHeader.biHeight<0) {
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height-1;
		yEnd = -1;
		dy = -1;
	}

	pixels = new Color[width * height];

	file.seekg(bmFileHeader.bfOffBits);

	const int padding = (4 - (width * 3) % 4) % 4;

	for (int y = yStart; dy > 0 ? y < yEnd : y > yEnd; y+=dy)
	{
		for (int x = 0; x < width; x++)
		{
			if (bmInfoHeader.biBitCount == 24) {
				PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			}
			else if (bmInfoHeader.biBitCount == 32) {
				PutPixel(x, y, Color(file.get(), file.get(), file.get(), file.get()));
			}
		}

		if (bmInfoHeader.biBitCount != 32) {
			file.seekg(padding, std::ios::cur);
		}
	}

}

Surface::Surface(int width, int height)
	:
	width(width),
	height(height),
	pixels( new Color[width*height])
{

}

Surface::Surface(const Surface& s)
{
	*this = s;
}

Surface::~Surface()
{
	delete[] pixels;
	pixels = nullptr;
}

Surface& Surface::operator=(const Surface& s)
{
	width = s.GetWidth();
	height = s.GetHeight();

	delete[] pixels;
	pixels = new Color[width * height];

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			this->PutPixel(i, j, s.GetPixel(i, j));
		}
	}
	return *this;
}

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);

	pixels[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < width);
	assert(y < height);

	return pixels[y*width+x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return {0,width-1,0, height-1};
}
