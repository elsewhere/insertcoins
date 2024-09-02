#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Tunnel.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Tunnel::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.99f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
//	if (pos >= fadeoutstart && pos <= fadeoutstop)
//		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define TUNNEL_GLOW

#ifndef TUNNEL_GLOW
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
	
	float blendalpha = 0.25f;
	const float radiusmod = 0.002f;
	const int count = 8;

	for (int i=0;i<count;i++)
	{
	    const float kohta = pos + i / (float)count;
        const float radius = i * radiusmod;
        //piirrettävän tekstuurin paikan muutokset
        const float xdisplace = radius*(float)sin(kohta*4*3.141592f);
        const float ydisplace = radius*(float)cos(kohta*4*3.141592f);
        
        //zoomataan hieman, muuten näyttää rumalta
        const float zoom = 0.005f;
        
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
}

void Tunnel::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();

	const float kulma = pos*14;
	Vector disp = Vector((float)sin(kulma), (float)cos(kulma), 0)*0.8f;
//	Vector campos = Vector(0, 0, -20.0f) + disp;
//	glTranslatef(campos.x, campos.y, campos.z);
//	glRotatef(pos*260, 0, 0, 1);

//	gluLookAt(10-20*pos, 0, 20, 0, 0, 0, 0, 1, 0);

	Vector campos = camera->getValue(pos) + disp;
	Vector camtarget = target->getValue(pos);
	Vector rotate = Vector(0, 1, (float)sin(pos*25));

	gluLookAt(campos.x, campos.y, campos.z, camtarget.x, camtarget.y, camtarget.z, rotate.x, rotate.y, rotate.z);

	const float gs = 50;
	const float gz = -20+(float)pow(pos, 8)*40;
	Vector g1 = Vector(-gs, -gs, gz);
	Vector g2 = Vector(gs, -gs, gz);
	Vector g3 = Vector(gs, gs, gz);
	Vector g4 = Vector(-gs, gs, gz);

	myDisable(MY_TEXTURE_2D);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_BLEND);
	glLineWidth(2);
	glColor4f(0.2f, 0.2f, 0.2f, alpha);
	Primitives::renderGrid(g1, g2, g3, g4, 60);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);

	myDisable(MY_DEPTH_TEST);

	for (i=0;i<beamcount;i++)
	{
		const float p = pos * beams[i].speed + beams[i].phase;
		const float t = p - (int)p;
		const float a = Mathematics::calcSaturate(t, 0, 1, 2);

		glLineWidth(7.0f);
		glBegin(GL_LINES);
		glColor4f(0,0, 0, 0);
		glVertex3fv((float *)&beams[i].v1);
		glColor4f(0.2f, 0.3f, 0.95f, alpha*a);
		glVertex3fv((float *)&beams[i].v2);
		glEnd();
		glLineWidth(3.0f);
		glBegin(GL_LINES);
		glColor4f(0,0, 0, 0);
		glVertex3fv((float *)&beams[i].v1);
		glColor4f(1,1,1,alpha*a);
		glVertex3fv((float *)&beams[i].v2);
		glEnd();
	}

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	glLineWidth(6.0f);
	Vector color1 = Vector(0.96f, 0.2f, 0.1f);
	Vector color2 = Vector(1,1,1);

	Vector c1 = color1;
	Vector c2 = color2;

	Matrix rot;
	glPushMatrix();
	glRotatef(pos*150, 0, 0, 1);
	glBegin(GL_LINES);
	for (i=0;i<partcount;i++)
	{
		const float speed = 15.0f + pos*10;
		float kohta = parts[i].phase + pos*speed;
		kohta -= (int)kohta;
	
		Vector p1 = parts[i].v1*kohta + parts[i].v3*(1-kohta);
		Vector p2 = parts[i].v2*kohta + parts[i].v4*(1-kohta);

		rot.makeRotation(0, 0, kohta*1*(float)cos(pos*17));

		p1 *= rot;
		p2 *= rot;

		if (i%8)
			glColor4f(c1.x, c1.y, c1.z, alpha*0.8f*kohta);
		else
			glColor4f(c2.x, c2.y, c2.z, alpha*0.8f*kohta);

		glVertex3fv((float *)&p1);
		glVertex3fv((float *)&p2);
	}
	glPopMatrix();
	glEnd();

}

Tunnel::Tunnel()
{	
	int i;

	partcount = 1000;
	parts = new Tpart[partcount];

	beamcount = 30;
	beams = new Tbeam[beamcount];

	const float depth = 20.0f;
	const float size = 2.0f;

	for (i=0;i<beamcount;i++)
	{
		const float maxrad = size*0.4f;
		const float k = dmsRandFloat()*2*3.141592f;
		const float r = dmsRandFloat()*maxrad;

		Vector v = Vector((float)cos(k), (float)sin(k), 0)*r;
		beams[i].v1 = v + Vector(0, 0, -depth);
		beams[i].v2 = v + Vector(0, 0, depth);

		beams[i].speed = 5 + dmsRandFloat()*14;
		beams[i].phase = dmsRandFloat();
	}

	for (i=0;i<partcount;i++)
	{

		const float minl = 0.1f;
		const float maxl = 0.25f;

		const float len = minl + dmsRandFloat()*(maxl-minl);
		float start = dmsRandFloat();
		float end = start + len;

//		if (end > 1.0f)
//			end = 1.0f;

		start -= 0.5f;
		end -= 0.5f;

		start *= 2;
		end *= 2;
	
		Vector v1, v2, v3, v4;

		switch(rand()%4)
		{
			//yläreuna
			case 0:
			{
				v1 = Vector(start, -size, depth);
				v2 = Vector(end, -size, depth);
				v3 = Vector(start, -size, 0);
				v4 = Vector(end, -size, 0);
			} break;
			//alareuna
			case 1: 
			{
				v1 = Vector(start, size, depth);
				v2 = Vector(end, size, depth);
				v3 = Vector(start, size, 0);
				v4 = Vector(end, size, 0);
			} break;
			//vasen
			case 2:
			{
				v1 = Vector(-size, start, depth);
				v2 = Vector(-size, end, depth);
				v3 = Vector(-size, start, 0);
				v4 = Vector(-size, end, 0);
			} break;
			//oikea
			case 3:
			{
				v1 = Vector(size, start, depth);
				v2 = Vector(size, end, depth);
				v3 = Vector(size, start, 0);
				v4 = Vector(size, end, 0);
			} break;
		}
		
		parts[i].phase = dmsRandFloat();
		parts[i].v1 = v1;
		parts[i].v2 = v2;
		parts[i].v3 = v3;
		parts[i].v4 = v4;
	}

	camera = new Spline();
	target = new Spline();

	Vector basepos = Vector(0, 0, 20);
	Vector targetpos = Vector(0, 0, 0);

	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(-3, 0, 0)+basepos);
	camera->addpoint(Vector(-2, -3, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(1, 5, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(-3, -3, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(3, 0, 0)+basepos);
	camera->addpoint(Vector(0, 2, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);
	camera->addpoint(Vector(0, 0, 0)+basepos);

	target->addpoint(Vector(0, 0, 0)+targetpos);
	target->addpoint(Vector(0, 0, 0)+targetpos);
	target->addpoint(Vector(0, 0, 0)+targetpos);
	target->addpoint(Vector(0, 0, 0)+targetpos);



}

Tunnel::~Tunnel()
{
	delete [] parts;
	delete [] beams;
}

bool Tunnel::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

