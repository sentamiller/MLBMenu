#pragma once

#include "window.h"
#include <string>

class Rect
{
public:
	Rect(int x, int y, int w, int h, int r, int g, int b, int a);
	Rect(int x, int y, int w, int h, const std::string& imagePath);
	Rect(int x, int y, int w, int h, char* imgBuffer, size_t bufferSize);
	~Rect();

	inline int getX() const { return x; }
	inline int getY() const { return y; }
	inline int getW() const { return w; }
	inline int getH() const { return h; }

	void draw(bool focus = false) const;
	void shiftX(int shift);

	inline void printVals() { printf("Rect vals: %d %d %d %d", x, y, w, h); }
private:
	int w, h;
	int x, y;
	int r, g, b, a;
	SDL_Texture* texture = nullptr;

};

