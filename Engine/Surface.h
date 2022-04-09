#pragma once

#include "Colors.h"
#include <string>
#include "RectI.h"

class Surface 
{
public:
	Surface(const std::string filename);
	Surface(int width, int height);
	Surface(const Surface& s);
	~Surface();
	Surface& operator=(const Surface& s);
	void PutPixel(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;

private:
	int width;
	int height;
	Color* pixels;

};
