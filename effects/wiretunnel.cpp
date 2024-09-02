#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Wiretunnel.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

static unsigned char invader1[] = 
{
	0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 
	0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 
	0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0
};

static unsigned char invader2[] = 
{
	0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 
	1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
	1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
	0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
};

static unsigned char invader3[] = 
{
	0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 
	0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 
	0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 
	0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 
	0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 
	0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 
	0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0,


};

void Wiretunnel::draw()
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
//	const float radiusmod = 0.017f;
	const float radiusmod = 0.0017f;
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

void Invader::draw(float pos, float size)
{
	int x, y;

	myDisable(MY_TEXTURE_2D);

	const float invaderpos = Mathematics::calcPosFloat(pos, starttime, endtime);
	if (invaderpos < 0.001f || invaderpos > 0.999f)
		return;
	const float diepos = Mathematics::calcPosFloat(invaderpos, diestart, dieend);
	Vector kohta = this->pos*invaderpos + this->pos2*(1-invaderpos);

	const float alpha = Mathematics::calcSaturate(invaderpos, 0, 1, 4);
	glColor4f(0.6f,0.6f,0.6f,alpha*(1-diepos));

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_BLEND);
	glBegin(GL_QUADS);

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			if (image[x+(7-y)*xres])
			{
				Matrix rotate;
				Vector r = rotates[x+y*xres]*diepos;

				rotate.makeRotation(r.x, r.y, r.z);
				Vector k = kohta + Vector((float)x, (float)y, 0)*size + points[x+y*xres]*diepos;
				Vector v1 = k;
				Vector v2 = k + Vector(size, 0, 0)*rotate;
				Vector v3 = k + Vector(size, size, 0)*rotate;
				Vector v4 = k + Vector(0, size, 0)*rotate;

				glVertex3fv((float *)&v1);
				glVertex3fv((float *)&v2);
				glVertex3fv((float *)&v3);
				glVertex3fv((float *)&v4);
			}
		}
	}
	glEnd();

	//ja laseri

	const float epsilon = 0.01f;
	float lasert = Mathematics::calcSaturate(invaderpos, diestart-epsilon, diestart+epsilon, 5);
	Vector source = Vector(0, 0, 60);
	Vector target = kohta + Vector((float)size, (float)size, 0)*6;
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glLineWidth(5);
	glColor4f(0.9f, 0.6f, 0.3f, alpha*lasert*0.3f);
	glBegin(GL_LINES);
	glVertex3fv((float *)&source);
	glVertex3fv((float *)&target);
	glEnd();
	glLineWidth(1);
	glColor4f(1,1,1,alpha*lasert*0.6f);
	glBegin(GL_LINES);
	glVertex3fv((float *)&source);
	glVertex3fv((float *)&target);
	glEnd();
}

void Wiretunnel::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();

	const float kulma = pos*14;
	Vector disp = Vector((float)sin(kulma), (float)cos(kulma), 0)*0.8f;

	Vector campos = Vector(0, 0, -20.0f) + disp;
	glTranslatef(campos.x, campos.y, campos.z);
	glRotatef(pos*160, 0, 0, 1);

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_BLEND);

	myDisable(MY_DEPTH_TEST);

	glLineWidth(4.0f);

	Vector color1 = Vector(0.96f, 0.2f, 0.1f);
	Vector color2 = Vector(0.3f, 0.5f, 0.96f);

	float colort = Mathematics::calcPosFloat(pos, 0.50f, 0.51f);
	Vector c1 = color1*colort + color2*(1-colort);
	Vector c2 = color2*colort + color1*(1-colort);

	const float speed = 13.0f+Mathematics::calcPosFloat(pos, 0.85f, 1.0f)*1;
	glBegin(GL_LINES);
	for (i=0;i<partcount;i++)
	{
		float kohta = parts[i].phase + pos*speed;
		kohta -= (int)kohta;

		Vector p1 = parts[i].v1*kohta + parts[i].v3*(1-kohta);
		Vector p2 = parts[i].v2*kohta + parts[i].v4*(1-kohta);

		if (i%8)
			glColor4f(c1.x, c1.y, c1.z, alpha*0.8f*kohta);
		else
			glColor4f(c2.x, c2.y, c2.z, alpha*0.9f*kohta);

		glVertex3fv((float *)&p1);
		glVertex3fv((float *)&p2);
	}
	glEnd();

	glPushMatrix();
	glRotatef(90, 0, 0, -1);

	for (i=0;i<invadercount;i++)
	{
		invaders[i].draw(pos, 0.03f);
	}
	glPopMatrix();
	Vector p1 = Vector(1, 0, 20);
	Vector p2 = Vector(1, 0, -20);

	const float alusstart = 0.87f;
	const float alusend = 0.98f;
	const float aluspos = Mathematics::calcPosFloat(pos, alusstart, alusend);

	if (aluspos > 0.001f && aluspos < 0.999f)
	{
		const float a = Mathematics::calcSaturate(aluspos, 0, 1, 2);
		glLineWidth(2);
		glColor4f(1.0f, 0.2f, 0.2f, alpha*a);
		Primitives::player(p1*(1-aluspos) + p2*aluspos, Vector(0.5f, 0, 0.2f+pos*7), 14);
	}

}

Wiretunnel::Wiretunnel()
{	
	int i;

	partcount = 5000;
	parts = new Tunnelpart[partcount];

	const float depth = 20.0f;

	for (i=0;i<partcount;i++)
	{
		const float size = 2.0f;

		const float minl = 0.1f;
		const float maxl = 0.25f;

		const float len = minl + dmsRandFloat()*(maxl-minl);
		float start = dmsRandFloat();
		float end = start + len;

//		if (end > 1.0f)
//			end = 1.0f;

		start -= 0.5f;
		end -= 0.5f;

		start *= 5;
		end *= 5;
	
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
	imagecount = 3;
	images = new unsigned char*[imagecount];
	images[0] = invader1;
	images[1] = invader2;
	images[2] = invader3;

	yres = 5;
	xres = 11;
	grid = new bool[xres*yres];

	int x, y;
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			grid[x+y*xres] = true;
//			grid[x+y*xres] = (rand()%2)?true:false;
		}

	}

	invadercount = 5*11;
	invaders = new Invader[invadercount];
	for (i=0;i<invadercount;i++)
	{
		const float r = 1;
		const float a = dmsRandFloat()*2*3.141592f;

		float xvalue = (i%12)-5.5f;
		float yvalue = (dmsRandFloat()-0.5f)*5;//(int)(i/(float)invadercount) - 2.5f;
		float zvalue = dmsRandFloat()*14;

		invaders[i].pos = Vector(xvalue, yvalue, zvalue)*0.2f;
		invaders[i].pos2 = Vector(xvalue, yvalue, zvalue)*0.2f;
//		invaders[i].pos = Vector((float)cos(a),(float)sin(a), 0)*r;
//		invaders[i].pos2 = Vector((float)cos(a),(float)sin(a), 0)*r;
		invaders[i].pos.z += depth*1;
		invaders[i].phase = dmsRandFloat();

		invaders[i].xres = 12;
		invaders[i].yres = 8;
		invaders[i].points = new Vector[invaders[i].xres*invaders[i].yres];
		invaders[i].rotates = new Vector[invaders[i].xres*invaders[i].yres];
		invaders[i].type = i%3;

		invaders[i].starttime = 0.2f;
		invaders[i].endtime = 0.9f;
		invaders[i].diestart = 0.45f+dmsRandFloat()*0.4f;
		invaders[i].dieend = invaders[i].diestart + 0.2f+ dmsRandFloat()*0.1f;

		switch(invaders[i].type)
		{
			case 0: invaders[i].image = invader1; break; 
			case 1: invaders[i].image = invader2; break; 
			case 2: invaders[i].image = invader3; break; 
		}
		for (int y=0;y<invaders[i].yres;y++)
		{
			for (int x=0;x<invaders[i].xres;x++)
			{
				const float mindist = 0.2f;
				const float maxdist = 2.6f;
				const float a1 = dmsRandFloat();
				const float a2 = dmsRandFloat();
				const float r = mindist + (maxdist-mindist)*dmsRandFloat();
				invaders[i].points[x+y*xres] = Mathematics::sphereToCartesian(r, a1*3.141592f, a2*3.141592f*2);
				invaders[i].rotates[x+y*xres] = Vector(dmsRandFloat()-0.5f, 
													   dmsRandFloat()-0.5f, 
													   dmsRandFloat()-0.5f)*15;
			}
		}
	}
}

Wiretunnel::~Wiretunnel()
{
	int i;
	delete [] parts;

	for (i=0;i<invadercount;i++)
	{
		delete [] invaders[i].points;
	}
	delete [] invaders;

}

bool Wiretunnel::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

