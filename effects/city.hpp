#ifndef _CITY_HPP_
#define _CITY_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class City : public Base
{
	public:

		City();
		~City();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		Spline *cam;
		Spline *tar;
		Spline *rot;

		int splinecount;
		Spline **paths;

		int cubecount;
		float *cubesizes;
		float *cubealphas;
		Vector *cubepos;
		Vector *cuberots;
		Vector *cuberots2;
		Vector *cubecols;


		void renderScene(float pos, float alpha);

};

#endif
