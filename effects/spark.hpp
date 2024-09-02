#ifndef _SPARK_HPP_
#define _SPARK_HPP_

#include <stdio.h>
#include "../externs.hpp"

class SparkParticle
{
public:
	Vector pos;
	float speed;
	float phase;
	float t;

	float startradius;
	float endradius;

	float radiusspeed;
	float radiusphase;
	Vector startrot;
	Vector endrot;
};


class SparkItem
{
public:
	SparkItem(int randseed, int itemcount);
	~SparkItem();

	SparkParticle *particles;

	void draw(float aika, float alpha);
	int itemcount;
};

class Spark : public Base
{
	public:

		Spark();
		~Spark();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		SparkItem *item;
		void renderScene(float pos, float alpha);

};

#endif
