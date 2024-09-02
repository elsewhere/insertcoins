#ifndef _STARFIELD_HPP_
#define _STARFIELD_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Star
{
public:

	Vector startpos;
	Vector endpos;

	float t;
	float speed;
	float phase;

	float lightphase;
	float lightspeed;
};

class Starfield : public Base
{
	public:

		Starfield();
		~Starfield();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		int starcount;
		Star *stars;

		Vector *shape1;
		Vector *shape2;
		Vector *shape3;
		Vector *shape4;

		void renderScene(float pos, float alpha);

};

#endif
