#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Starfield.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"


const int greetingscount = 26;
const int greetingslength = 12;
const char greetings[greetingscount][greetingslength] = {

"High scores",
"-----------",
"1000000 ANS",
"0950000 ASD",
"0900000 CNS",
"0850000 DDV",
"0800000 DIV",
"0750000 FKT",
"0715517 JML",
"0700000 FLT",
"0650000 GLT",
"0600000 HDL",
"0550000 LAG",
"0500000 MTC",
"0450000 MFX",
"0400000 N.C",
"0350000 NMC",
"0300000 OTB",
"0250000 PDZ",
"0200000 PLT",
"0150000 PYR",
"0100000 STC",
"0095000 SGZ",
"0090000 STS",
"0085000 TPD",
"0080000 U.D",
};


void Starfield::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.002f;
	const float fadeoutstart = 0.98f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define STARFIELD_GLOW

#ifndef STARFIELD_GLOW
	renderScene(pos, alpha);
#else
	glViewport(0, 0, 512, 512);
	renderScene(pos, alpha);
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render0")->getID());
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render2")->getID()); //alkup. talteen
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);

	myDisable(MY_DEPTH_TEST);
//	myDisable(MY_DEPTH_MASK); //<--- miksi tästä tulee kummallinen bugitus?
	dmsPerspective2D(1,1);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	Texture *render1;
	Texture *render2;
	
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	float blendalpha = 0.32f;
	const float radiusmod = 0.00517f;
	const int count = 12 ;

	for (int i=0;i<count;i++)
	{
	    const float kohta = i / (float)count;
        const float radius = i * radiusmod;
        //piirrettävän tekstuurin paikan muutokset
        const float xdisplace = radius*(float)sin(kohta*4*3.141592f);
        const float ydisplace = radius*(float)cos(kohta*4*3.141592f);
        
        //zoomataan hieman, muuten näyttää rumalta
        const float zoom = 0.00;//5f;
        
        if (!(i%2))
        {
                render1 = dmsGetTexture("render0");
                render2 = dmsGetTexture("render1");
        }
        else
        {
                render1 = dmsGetTexture("render1");
                render2 = dmsGetTexture("render0");
        }

        glBindTexture(GL_TEXTURE_2D, render1->getID());
        glBegin(GL_QUADS);
        glColor4f(1, 1, 1, blendalpha);
        glTexCoord2f(zoom, zoom);
        glVertex2f(xdisplace, ydisplace);
        glTexCoord2f(1-zoom, zoom);
        glVertex2f(1+xdisplace, ydisplace);
        glTexCoord2f(1-zoom, 1-zoom);
        glVertex2f(1+xdisplace, 1+ydisplace); //, 1);
        glTexCoord2f(zoom, 1-zoom);
        glVertex2f(xdisplace, 1+ydisplace);
        glEnd();

        glBindTexture(GL_TEXTURE_2D, render2->getID());
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 512, 512, 0);
	}
	myDisable(MY_BLEND);
	glViewport(0, 0, dmsGetWindowWidth(), dmsGetWindowHeight());
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, dmsGetTexture("render2")->getID());
    glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(0, 1);
    glEnd();
	myEnable(MY_BLEND);

    glColor4f(1,1,1,1);
    glBindTexture(GL_TEXTURE_2D, render2->getID());
    glBegin(GL_QUADS);
    glColor4f(1,1,1,1);
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);
    glTexCoord2f(1, 0);
    glVertex2f(1, 0);
    glTexCoord2f(1, 1);
    glVertex2f(1, 1);
    glTexCoord2f(0, 1);
    glVertex2f(0, 1);
    glEnd();
	dmsPerspective3D();
#endif 

//void dmsWrite2D(float x, float y, float a, float scale, char *fontName, char *msg);

//	dmsWrite2D();

	dmsPerspective2D(1,1);
	const float a = 1-Mathematics::calcPosFloat(pos, 0.0f,0.03f);
	if (a > 0)
	{
		myDisable(MY_TEXTURE_2D);
		myDisable(MY_DEPTH_TEST);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1,1,1,a);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(1, 0);
		glVertex2f(1, 1);
		glVertex2f(0, 1);
		glEnd();
	}
	dmsPerspective3D();




}

void Starfield::renderScene(float pos, float alpha)
{
	int i;

	glLoadIdentity();
	glTranslatef(0, 0, -13.0f);

	glRotatef(pos*pos*360, 0.2f, 0.3f*(float)cos(pos*4*3.141592f), -0.11f);
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("particle.jpg")->getID());

	Vector *shape = shape2;

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	
	glLineWidth(2.0f);

	Vector p1 = Vector(0, 0,  10);
	Vector p2 = Vector(0, 0, -20);

	const float alusstart = 0.25f;
	const float alusend = 0.35f;
	const float aluspos = Mathematics::calcPosFloat(pos, alusstart, alusend);

	if (aluspos > 0.001f && aluspos < 0.999f)
	{
		const float a = Mathematics::calcSaturate(aluspos, 0, 1, 2);
		glColor4f(1.0f, 0.2f, 0.2f, alpha*a);
		Primitives::player(p1*(1-aluspos) + p2*aluspos, Vector(0.5f, 0, pos*13), 4);
	}
	
	for (i=0;i<starcount;i++)
	{
		stars[i].t = stars[i].speed*pos + stars[i].phase;
		stars[i].t -= (int)stars[i].t;
		float p = dmsCalcSaturate(stars[i].t, 0, 1, 1);

		float light_t = stars[i].lightspeed*pos + stars[i].lightphase;
		light_t -= (int)light_t;
		
		float lightval = dmsCalcSaturate(light_t, 0, 1, 1);
		p *= lightval;

		Vector starpos = stars[i].startpos * (1-stars[i].t) + stars[i].endpos * stars[i].t;

		float shape_t = 2*(float)cos(pos*2*3.141592f*8)*Mathematics::calcPosFloat(pos, 0.55f, 0.58f);
		if (shape_t < 0)
			shape_t = 0;
		if (shape_t > 1)
			shape_t = 1;

		Vector final = shape[i]*shape_t + starpos*(1-shape_t);

		if (!(i%23))
		{
			glLineWidth(2.5f);
			glColor4f(0.8f, 0.6f, 0.3f, p*alpha*2);
		}
		else
		{
			glLineWidth(1.5f);
			if (i%2)
				glColor4f(0.3f,0.4f,0.8f,p*alpha*2);
			else
				glColor4f(0.8f,0.4f,0.8f,p*alpha*2);
		}

		const float l = 0.1f + lightval*0.05f;
		Vector v1 = starpos + Vector(-l, 0, 0);
		Vector v2 = starpos + Vector(l, 0, 0);
		Vector v3 = starpos + Vector(0, -l, 0);
		Vector v4 = starpos + Vector(0, l, 0);
		Vector v5 = starpos + Vector(0, 0, -l);
		Vector v6 = starpos + Vector(0, 0, l);

		glBegin(GL_LINES);
		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
		glVertex3fv((float *)&v4);
		glVertex3fv((float *)&v5);
		glVertex3fv((float *)&v6);
		glEnd();



//		Primitives::drawFlare(&starpos, 0.2f+lightval*0.05f);
	}
}

Starfield::Starfield()
{	
	int i;

	starcount = 1000;
	stars = new Star[starcount];

	shape1 = new Vector[starcount];
	shape2 = new Vector[starcount];
	shape3 = new Vector[starcount];
	shape4 = new Vector[starcount];

	float d1 = -10.0f;
	float d2 =  10.0f;
	const float scale = 12.0f;

	for (i=0;i<starcount;i++)
	{
		Vector v = Vector(dmsRandFloat()-0.5f, dmsRandFloat()-0.5f, 0)*scale;
		stars[i].startpos = v + Vector(0, 0, d1);
		stars[i].endpos = v + Vector(0, 0, d2);

		stars[i].phase = dmsRandFloat();
		stars[i].speed = 12.5f + dmsRandFloat()*7;

		stars[i].lightphase = dmsRandFloat();
		stars[i].lightspeed = 1.0f + dmsRandFloat()*3;
	}

	for (i=0;i<starcount;i++)
	{
		const float kohta = i / (float)starcount;

		//vieteri

		const float s1radius = 1.5;//*(0.5f+0.9f*(float)sin(kohta*47));
		const float s1kierroksia = 7;
		const float s1kulma = kohta*s1kierroksia*2*3.141592f;

		Vector s1start = Vector(0, -5, 0);
		Vector s1stop  = Vector(0,  5, 0);
		shape1[i] = s1start*(1-kohta) + s1stop*(kohta) + Vector((float)sin(s1kulma), 0, (float)cos(s1kulma))*s1radius;

		//hymiö
		const float s2radius = 4.0f+dmsRandFloat()*0.3f;
		const float s2kulma = dmsRandFloat()*2*3.141592f;

		if (dmsRandFloat()>0.35f)
			shape2[i] = Vector((float)cos(s2kulma), 0, (float)sin(s2kulma))*s2radius;
		else
		{
			//suu 
			if (dmsRandFloat()>0.5f)
			{
				const float s2radius2 = s2radius - 0.9f;
				const float s2kulma2 = 4.0f + dmsRandFloat()*0.3f*2*3.141592f;
				shape2[i] = Vector((float)cos(s2kulma2), 0, (float)sin(s2kulma2))*s2radius2;
			}	
			//silmät
			else
			{
				const float s2radius3 = 0.7f;
				const float s2kulma3 = dmsRandFloat()*2*3.141592f;
				Vector eyepos;
				if (dmsRandFloat()>0.5f)
				{
					eyepos = Vector(1.5f, 0, 1.5f);
				}
				else
				{
					eyepos = Vector(-1.5f, 0, 1.5f);
				}
				shape2[i] = eyepos + Vector((float)sin(s2kulma3), 0, (float)cos(s2kulma3))*s2radius3;
			}
		}


	}
}

Starfield::~Starfield()
{
	delete [] stars;

	delete [] shape1;
	delete [] shape2;
	delete [] shape3;
	delete [] shape4;
}

bool Starfield::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

