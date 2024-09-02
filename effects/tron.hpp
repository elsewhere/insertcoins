#ifndef _TRON_HPP_
#define _TRON_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class Cycle
{
public:

	int vertexcount;
	int current;
	Vector *vertices;
	Vector color;

	float starttime;
	float endtime;

	void draw(float pos, float alpha);


};


class Tron : public Base
{
	public:

		Tron();
		~Tron();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		int cyclecount;
		Cycle *cycles;

		Spline **cams;
		Spline **targets;

		Spline *aluspath;
		Spline *alusrotate;

		void renderScene(float pos, float alpha);
		void renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha);

};

#endif
