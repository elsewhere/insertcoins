#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Alku.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"
#include "../writer.hpp"

void Alku::draw()
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

void Alku::renderScene(float pos, float alpha)
{
	glLoadIdentity();
//	glTranslatef(0, 0, -7.0f);
//extern "C" __declspec(dllexport) void dmsWrite3D(float x, float y, float z, float a, char *fontName, char *msg);

//	dmsWrite2D( 0.0f, 0.0f, 1.0f, 1.0f, "arcade.pcx", "viesti");

	const float t1alku = 0.04f;
	const float t1loppu = 0.30f;
	const float t2alku = 0.34f;
	const float t2loppu = 0.60f;
	const float t3alku = 0.64f;
	const float t3loppu = 0.95f;
	const float t4alku = 0.75f;
	const float t4loppu = 0.95f;

	const float blink = (float)sin(pos*95)>0?1.0f:0.0f;

	const float t1pos = Mathematics::calcPosFloat(pos, t1alku, t1loppu);
	const float t1alpha = Mathematics::calcSaturate(t1pos, 0, 1, 1);
	const float t2pos = Mathematics::calcPosFloat(pos, t2alku, t2loppu);
	const float t2alpha = Mathematics::calcSaturate(t2pos, 0, 1, 1);
	const float t3pos = Mathematics::calcPosFloat(pos, t3alku, t3loppu);
	const float t3alpha = Mathematics::calcSaturate(t3pos, 0, 1, 1);
	const float t4pos = Mathematics::calcPosFloat(pos, t4alku, t4loppu);
	const float t4alpha = Mathematics::calcSaturate(t4pos, 0, 1, 1);

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	dmsWrite2D(0.25f, 0.4f, t1alpha, 1,  "arcade.pcx", "Traction and deMarche");
	dmsWrite2D(0.23f, 0.45f, t2alpha, 1, "arcade.pcx", "       present       ");
	dmsWrite2D(0.25f, 0.48f, t3alpha*blink, 1, "arcade.pcx", "    Insert Coins     ");
	dmsWrite2D(0.26f, 0.60f, t4alpha, 1, "arcade.pcx", "        for         ");
	dmsWrite2D(0.25f, 0.65f, t4alpha, 1, "arcade.pcx", "      TUM 2005       ");


/*
	Texture *font = dmsGetTexture("arcade.pcx");

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1,1,1,alpha);
	Writer::write2D(0, 0, 0.2f, "H6RC", font);
	myDisable(MY_TEXTURE_2D);
*/

}

Alku::Alku()
{	
}

Alku::~Alku()
{
}

bool Alku::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

