#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Rings.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Rings::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.98f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

//#define RINGS_GLOW

#define RINGS_GLOW

#ifndef RINGS_GLOW
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
	
	float blendalpha = 0.2f + 0.00f*pos;
	const float radiusmod = 0.0015f;
	const int count = 15;

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

	const int linebreak = 39;
	const char teksti[] = "top secret mission... this briefing is " \
		                  "for your eyes only.                    " \
						  "                                       " \
						  "your task is to travel on this crt beam" \
						  "back to eighties computer graphics and " \
						  "retrieve the pure neon rgb colors and  " \
						  "synthpop we have abandoned in our      " \
						  "foolish quest for design and art.      " \
						  "                                       " \
						  "The mission will be dangerous so good  " \
						  "luck/";

	int len = strlen(teksti);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myDisable(MY_DEPTH_TEST);

	float cursor_x = 0.0f;
	float cursor_y = 0.1f;

	const float tekstialku = 0.2f;
	const float tekstiloppu = 0.7f;

	const float tekstikohta = Mathematics::calcPosFloat(pos, tekstialku, tekstiloppu);
	const int koko = (int)(tekstikohta*len);

	for (i=0;i<koko;i++)
	{
		if ((i%linebreak)==0)
		{
			cursor_x = 0.0125f;
			cursor_y += 0.04f;
		}
		char string[2];
		string[0] = teksti[i];
		string[1] = 0;

		dmsWrite2D(cursor_x, cursor_y, alpha, 1, "arcade.pcx", (char *)string);
		cursor_x += 0.025f; 


	}

//	dmsWrite2D(0, 0, alpha, 1, "arcade.pcx", (char *)teksti);





}

void Rings::renderScene(float pos, float alpha)
{
	int i;

//	glLoadIdentity();
//	glTranslatef(pos, 0, -7.0f);
//	glRotatef(pos*60, 0.1f, 0.4f, 0.2f);

	Vector c = cam->getValue(pos);
	Vector t = target->getValue(pos);
	Vector r = rotate->getValue(pos);

	glLoadIdentity();
	gluLookAt(c.x, c.y, c.z, t.x, t.y, t.z, r.x, r.y, r.z);

	renderTausta(alpha);

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_BLEND);

	glColor4f(1.0f, 0.2f, 0.2f, alpha*10.8f);
	Vector aluspos = Vector(-6, 0, 0) + Vector(1, 0, 0)*(float)pow(pos+0.1f, 40)*40;
	Primitives::player(aluspos, Vector(0, 0.25f, pos*10), 3);
		
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glLineWidth(3);
//	glBegin(GL_LINES);
	for (i=0;i<linecount;i++)
	{
		lines[i].draw(pos, alpha);
	}
//	glEnd();
	glLineWidth(1);

	myEnable(MY_DEPTH_TEST);
	for (i=0;i<ringcount;i++)
	{
		renkaat[i].draw(pos, alpha);
	}

}

void Rings::renderTausta(float alpha)
{
	const float size = 20;
	Vector v1 = Vector(-size, -size, -size);
	Vector v2 = Vector( size, -size, -size);
	Vector v3 = Vector( size, -size,  size);
	Vector v4 = Vector(-size, -size,  size);

	Vector v5 = Vector(-size, size, -size);
	Vector v6 = Vector( size, size, -size);
	Vector v7 = Vector( size, size,  size);
	Vector v8 = Vector(-size, size,  size);

	const float harmaa = 0.5f;
	glColor4f(harmaa, harmaa, harmaa, alpha*0.4f);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(2.0f);


	glPushMatrix();
	//ylä<->ala
	renderSivu(v1, v2, v3, v4, alpha);
	renderSivu(v5, v6, v7, v8, alpha);
	//etu<->taka
	renderSivu(v1, v2, v6, v5, alpha);
	renderSivu(v3, v4, v8, v7, alpha);
	//vasen<->oikea
	renderSivu(v1, v4, v8, v5, alpha);
	renderSivu(v2, v3, v7, v6, alpha);
	glPopMatrix();
	glPushMatrix();
	glScalef(1.1f, 1.1f, 1.1f);
	glColor4f(harmaa, harmaa, harmaa, alpha*0.25f);
	//ylä<->ala
	renderSivu(v1, v2, v3, v4, alpha);
	renderSivu(v5, v6, v7, v8, alpha);
	//etu<->taka
	renderSivu(v1, v2, v6, v5, alpha);
	renderSivu(v3, v4, v8, v7, alpha);
	//vasen<->oikea
	renderSivu(v1, v4, v8, v5, alpha);
	renderSivu(v2, v3, v7, v6, alpha);
	glPopMatrix();
}
void Rings::renderSivu(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha)
{
	//p1----p2
	//|     |
	//|     |
	//|     |
	//p4----p3

	int i;
	const int slices = 25;

	glBegin(GL_LINES);
	for (i=0;i<slices;i++)
	{
		float kohta = i/(float)slices;

		Vector v1 = p1*(1-kohta) + p2*kohta; 
		Vector v2 = p4*(1-kohta) + p3*kohta;

		Vector v3 = p1*(1-kohta) + p4*kohta;
		Vector v4 = p2*(1-kohta) + p3*kohta;

		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
		glVertex3fv((float *)&v3);
		glVertex3fv((float *)&v4);

	}
	glEnd();
}

void Ringline::draw(float pos, float alpha)
{
	const float linealpha = Mathematics::calcPosFloat(pos, starttime, endtime);
	const float maxalpha = 1.0f*linealpha;
	float t = speed*pos + phase;
	t -= (int)t;
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_DEPTH_MASK);
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor4f(color.x, color.y, color.z, alpha*Mathematics::calcSaturate(t, 0, 1, 1)*maxalpha*0.01f);
	glVertex3fv((float *)&v1);
	glColor4f(color.x, color.y, color.z, alpha*Mathematics::calcSaturate(t, 0, 1, 1)*maxalpha);
	glVertex3fv((float *)&v2);
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES);
	glColor4f(1,1,1,0);
	glVertex3fv((float *)&v1);
	glColor4f(1,1,1,alpha*linealpha);
	glVertex3fv((float *)&v2);
	glEnd();
}

void Ring::draw(float aika, float alpha)
{
	int i;

	float ringpos = Mathematics::calcPosFloat(aika, this->starttime, this->endtime);
	float startkulma = (this->speed * aika + this->phase)*2*3.141592f;
	float endkulma = (this->speed * aika + this->phase 
					  + (1-this->gap*(0.5f+0.5f*(float)cos(aika*18+this->phase))))*2*3.141592f;

	const float t = (float)pow(ringpos, 5);
	float rad = this->radius*1.5f*(1-t) + this->radius*t;

	float r1 = rad - this->width*0.5f;
	float r2 = rad + this->width*0.5f;

	const int strips = 45;

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_DEPTH_TEST);
	glColor4f(this->col.x, this->col.y, this->col.z, alpha*0.7f*t);
//	glColor4f(1,1,1,1);

	glBegin(GL_TRIANGLE_STRIP);
	for (i=0;i<strips;i++)
	{
		float t = i / (float)strips;

		float kulma = (1-t)*startkulma + t*endkulma;

		Vector v1 = this->pos + Vector(0, (float)cos(kulma), (float)sin(kulma))*r1;
		Vector v2 = this->pos + Vector(0, (float)cos(kulma), (float)sin(kulma))*r2;

		glVertex3fv((float *)&v1);
		glVertex3fv((float *)&v2);
	}
	glEnd();
}
Rings::Rings()
{	
	int i;
	ringcount = 150;
	renkaat = new Ring[ringcount];

	for (i=0;i<ringcount;i++)
	{
		const float xrange = 7;
		renkaat[i].pos = Vector(dmsRandFloat()-0.5f, 0, 0)*xrange;
		renkaat[i].col = Vector(0.9f+dmsRandFloat()*0.1f, 
								0.3f+dmsRandFloat()*0.0f,
								0.1f+dmsRandFloat()*0.00f);

		renkaat[i].width = 0.06f;
		renkaat[i].radius = 0.3f + 1.5f*(float)pow(dmsRandFloat()*dmsRandFloat(), 1.5f);
		renkaat[i].speed = 2*(1.0f + dmsRandFloat()*0.4f);
		renkaat[i].phase = dmsRandFloat();
		renkaat[i].gap = dmsRandFloat()*0.4f;

		renkaat[i].starttime = dmsRandFloat()*0.1f;
		renkaat[i].endtime = renkaat[i].starttime + 0.1f;
	}

	linecount = 200;
	lines = new Ringline[linecount];

	for (i=0;i<linecount;i++)
	{
		const float ringrad = 0.153f*dmsRandFloat();
		const float k = dmsRandFloat()*2*3.141592f;
		lines[i].phase = dmsRandFloat();
		lines[i].speed = 40 + 146*dmsRandFloat();
		lines[i].v1 = Vector(-1.5f-dmsRandFloat(), (float)sin(k)*ringrad, (float)cos(k)*ringrad);
		lines[i].v2 = Vector(150, (float)sin(k)*ringrad, (float)cos(k)*ringrad);
		lines[i].color = Vector(0.1f+dmsRandFloat()*0.1f, 
								0.3f+dmsRandFloat()*0.0f,
								0.9f+dmsRandFloat()*0.00f);

		lines[i].starttime = 0.49f+dmsRandFloat()*0.1f;
		lines[i].endtime = lines[i].starttime + 0.03f;
	}

	cam = new Spline();
	target = new Spline();
	rotate = new Spline();

	cam->addpoint(Vector(3, 0, -7));
	cam->addpoint(Vector(4, 2, -7));
	cam->addpoint(Vector(5, 0, -5));
	cam->addpoint(Vector(9, -1, -4));
	cam->addpoint(Vector(7, 2, 1));
	cam->addpoint(Vector(9, 2, 5));
	cam->addpoint(Vector(6, -1, 6));

	target->addpoint(Vector(-7, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(3, 0, 0));
	target->addpoint(Vector(5, 0, 0));

	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, -0.2f));
	rotate->addpoint(Vector(0, 1, -0.5f));


}
Rings::~Rings()
{
	delete [] renkaat;
	delete [] lines;

	delete cam;
	delete target;
	delete rotate;
}

bool Rings::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

