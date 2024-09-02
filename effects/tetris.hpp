#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class Tetris : public Base
{
	public:

		Tetris();
		~Tetris();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		int xres;
		int yres;
		Spline *camera;
		Spline *target;
		Spline *rotate;
		unsigned char *arena;
		void renderScene(float pos, float alpha);
		void renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha);

};

#endif
