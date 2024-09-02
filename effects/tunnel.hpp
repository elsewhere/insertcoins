#ifndef _TUNNEL_HPP_
#define _TUNNEL_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class Tbeam
{
public:
	Vector v1;
	Vector v2;

	float speed;
	float phase;

};

class Tpart
{
public:
	Vector v1, v2;
	Vector v3, v4;
	float phase;

};

class Tunnel : public Base
{
	public:

		Tunnel();
		~Tunnel();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int partcount;
		Tpart *parts;

		int beamcount;
		Tbeam *beams;

		Spline *camera;
		Spline *target;


		void renderScene(float pos, float alpha);

};

#endif
