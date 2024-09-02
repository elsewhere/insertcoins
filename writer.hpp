#ifndef _WRITER_HPP_
#define _WRITER_HPP_

#include "externs.hpp"

class FontRect
{
public:
	static void getUV(int charnum, float *u1, float *v1, float *u2, float *v2);
	
};

class Writer
{
public:
	static void write2D(float x, float y, float size, char *string, Texture *font);


};

#endif