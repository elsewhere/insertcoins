#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Spark.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

/*
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
	Vector startrot;
	Vector endrot;
}


class SparkItem
{
public:
	SparkItem(int randseed, int itemcount);

	SparkParticle *particles;

	void draw(float alpha);
	int itemcount;
}

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
*/

SparkItem::SparkItem(int randseed, int itemcount)
{
	int i;
	this->itemcount = itemcount;
	srand(randseed);

	particles = new SparkParticle[this->itemcount];

	for (i=0;i<this->itemcount;i++)
	{
		const float maxrad = 2.9f*dmsRandFloat();
		const float minrad = 0.0f;

		const float maxspeed = 2.0f;
		const float minspeed = 0.7f;

		const float maxradiusspeed = 5.0f;
		const float minradiusspeed = 2.0f;

		const float rotmul = 0.4f;

		particles[i].startradius = minrad;
		particles[i].endradius = maxrad;

		particles[i].phase = dmsRandFloat();
		particles[i].radiusphase = dmsRandFloat();
		particles[i].endrot = Vector(dmsRandFloat(), dmsRandFloat(), dmsRandFloat())*rotmul;
		particles[i].startrot = Vector(dmsRandFloat(), dmsRandFloat(), dmsRandFloat())*rotmul;

		particles[i].speed = minspeed + dmsRandFloat()*(maxspeed-minspeed);
		particles[i].radiusspeed = minradiusspeed + dmsRandFloat()*(maxradiusspeed-minradiusspeed);
	}
}

void SparkItem::draw(float aika, float alpha)
{
	int i;

	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("particle.jpg")->getID());
	for (i=0;i<itemcount;i++)
	{
		float t = aika*particles[i].speed + particles[i].phase;
		t -= (int)t;
		t = 0.5f+0.5f*(float)sin(t*3.141592f);

		float t2 = aika*particles[i].radiusspeed + particles[i].radiusphase;
		t2 -= (int)t2;
		t2 = 0.5f+0.5f*(float)sin(t2*3.141592f);

		Vector rot = particles[i].startrot*(1-t) + particles[i].endrot*t;
		float radius = particles[i].startradius*(1-t2) + particles[i].endradius*t2;

		float kulma = t*2*3.141592f;

		Matrix rotate;
		Matrix rotscale;

		rotscale.makeIdentity();
		rotscale.data[0][0] = 2*3.141592f;
		rotscale.data[1][1] = 2*3.141592f;
		rotscale.data[2][2] = 2*3.141592f;
		rot *= rotscale;
		rotate.makeRotation(rot.x, rot.y, rot.z);

		Vector v = Vector((float)sin(kulma), 0, (float)cos(kulma))*rotate;
		v *= radius;

		const float a = (alpha-radius/1.3f)*0.3f;
		glColor4f(1,1,1,a);

		Primitives::drawFlare(&v, 0.12f);
	}

}

SparkItem::~SparkItem()
{
	delete [] particles;
}
void Spark::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
}

void Spark::renderScene(float pos, float alpha)
{
	glLoadIdentity();
	glTranslatef(0, 0, -7.0f);

	glPushMatrix();
	glTranslatef((float)cos(pos*16), 0, 0);
	glRotatef(pos*150, 0, 0, 0.74f);
	item->draw(pos, alpha);
	glPopMatrix();
}

Spark::Spark()
{	
	item = new SparkItem(1060, 500);
}

Spark::~Spark()
{
	delete item;
}

bool Spark::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

