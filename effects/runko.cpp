#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Runko.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Runko::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
}

void Runko::renderScene(float pos, float alpha)
{
	glLoadIdentity();
	glTranslatef(0, 0, -7.0f);
}

Runko::Runko()
{	
}

Runko::~Runko()
{
}

bool Runko::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

