#ifndef _LABY_HPP_
#define _LABY_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class LabyEdge
{
public:
	LabyEdge() {};
	LabyEdge(Vector *p_v1, Vector *p_v2) { v1 = *p_v1; v2 = *p_v2; };
	Vector v1;
	Vector v2;
};

class Pacman
{
public:
	int posx;
	int posy;
	int dx, dy;

	int pointcount;
	Vector *points;
	float *suu;

};
class Laby : public Base
{
	public:

		Laby();
		~Laby();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		float scale;

		int pacmancount;
		Pacman *pacmen;
		
		int gridx;
		int gridy;
		int *grid;

		bool safeGet(int x, int y);
		int edgecount;
		LabyEdge *edges;
		int *edgeorder;

		int pointcount;
		bool *pointflags;
		Vector *points;
		
		Spline *kamera;
		Spline *viewpoint;
		Spline *rotationtrack;
		void renderScene(float pos, float alpha);

};

#endif
