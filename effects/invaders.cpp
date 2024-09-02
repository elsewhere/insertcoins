#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Invaders.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"


void Invaders::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float starttime = 0.5f;
	const float endtime = 1.0f;
	const float t = Mathematics::calcPosFloat(pos, starttime, endtime);

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	alpha = dmsCalcSaturate(t, 0, 1, 18);

	renderScene(t, alpha);
}

void Invaders::renderScene(float pos, float alpha)
{

//	glRotatef(pos*1069, 0.2f, 0.4f, pos);
//	glTranslatef(0, 0, -19.0f + 2*(float)sin(pos*11)+(float)pow((pos-0.02f), 170)*1000);
//	glRotatef(-30, 1, pos*0.15f*(float)cos(pos*14), 0);

//	Vector k = Vector(0, 0, 0);
//	drawInvader(k, 0.1f, 2);

}

Invaders::Invaders()
{	
}


Invaders::~Invaders()
{
	delete [] images;
	delete [] grid;
}

bool Invaders::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

