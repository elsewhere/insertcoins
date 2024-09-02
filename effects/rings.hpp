#ifndef _RINGS_HPP_
#define _RINGS_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"


class Ring
{
public:
	Vector pos;
	Vector col;
	float radius;
	float width;

	float starttime;
	float endtime;

	float angle;
	float speed;
	float phase;
	float gap;

	void draw(float aika, float alpha);
};

class Ringline
{
public:
	Vector v1;
	Vector v2;
	Vector color;

	float phase;
	float speed;
	float starttime;
	float endtime;

	void draw(float pos, float alpha);



};

class Rings : public Base
{
	public:

		Rings();
		~Rings();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int ringcount;
		Ring *renkaat;

		int linecount;
		Ringline *lines;

		Spline *cam;
		Spline *target;
		Spline *rotate;

		void renderScene(float pos, float alpha);

		void renderTausta(float alpha);
		void renderSivu(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha);


};

#endif
