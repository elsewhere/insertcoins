#ifndef _PRIMITIVES_HPP_
#define _PRIMITIVES_HPP_

#include "externs.hpp"
#include "mathematics.hpp"

class Primitives
{
public:
	static void player(Vector &position, Vector &rotation, float scale);
	static void wireCube(float size);
	static void flatCube(float size);
	static void wireBox(Vector &p1, Vector &p2);
	static void texturedCube(float size, float tmul);
	static void drawFlare(Vector *pos, float size);
	static void circle(Vector &pos, float radius, float lines, float r, float g, float b, float alpha);
	static void fadeText(float x, float y, float scale, float alpha, float pos, float starttime, float endtime, float ramp,
				 		  char *fontti, char *teksti);
	static void textWriterLayers(float xpos, float ypos, float pos, float spread, float textsize, float starttime, float endtime, float alpha, char *teksti);
	static void renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, int slices);
	static void renderProperGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, int slices);
	static void billboard(Vector &kohta, Vector &x, Vector &y, Vector &z, float scale);
};

#endif