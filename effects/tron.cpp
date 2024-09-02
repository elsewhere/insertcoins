#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Tron.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

const float starttimes[] = 
{
	0.00f, 
	0.25f, 
	0.50f, 
	0.75f
};

const float endtimes[] = 
{
	0.25f, 
	0.50f,
	0.75f,
	1.00f,
};


void Tron::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.02f;
	const float fadeoutstart = 0.98f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define TRON_GLOW

#ifndef TRON_GLOW
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
	
	float blendalpha = 0.20f;
	const float radiusmod = 0.0017f;
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
}

void Tron::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();

	Vector campos;
	Vector camtarget;

	if (pos >= starttimes[0] && pos < endtimes[0])
	{
		campos = cams[0]->getValue(Mathematics::calcPosFloat(pos, starttimes[0], endtimes[0]));
		camtarget = targets[0]->getValue(Mathematics::calcPosFloat(pos, starttimes[0], endtimes[0]));
	}
	if (pos >= starttimes[1] && pos < endtimes[1])
	{
		campos = cams[1]->getValue(Mathematics::calcPosFloat(pos, starttimes[1], endtimes[1]));
		camtarget = targets[1]->getValue(Mathematics::calcPosFloat(pos, starttimes[1], endtimes[1]));
	}

	if (pos >= starttimes[2] && pos < endtimes[2])
	{
		campos = cams[2]->getValue(Mathematics::calcPosFloat(pos, starttimes[2], endtimes[2]));
		camtarget = targets[2]->getValue(Mathematics::calcPosFloat(pos, starttimes[2], endtimes[2]));
	}
	if (pos >= starttimes[3] && pos <= endtimes[3])
	{
		campos = cams[3]->getValue(Mathematics::calcPosFloat(pos, starttimes[3], endtimes[3]));
		camtarget = targets[3]->getValue(Mathematics::calcPosFloat(pos, starttimes[3], endtimes[3]));
	}

	Vector add = Vector(0, -1, 0)*(float)pow(1-pos, 100)*15;
	Vector add2 = Vector(0, 1, 0)*(float)pow(pos, 100)*15;
	campos += add;
	camtarget += add;

	campos += add2;
	camtarget += add2;


//	glTranslatef(0, 6, -7.0f);

	gluLookAt(campos.x, campos.y, campos.z, camtarget.x, camtarget.y, camtarget.z, 0, 1, 0);

	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_BLEND);
	myDisable(MY_TEXTURE_2D);

	const float planesize = 150;
	Vector a1 = Vector(-planesize, 0, -planesize);
	Vector a2 = Vector(planesize, 0, -planesize);
	Vector a3 = Vector(planesize, 0, planesize);
	Vector a4 = Vector(-planesize, 0, planesize);

	Vector color = Vector(1,1,1);
	glColor4f(color.x, color.y, color.z, alpha*0.2f);

	glLineWidth(2.0f);
	Primitives::renderGrid(a1, a2, a3, a4, 70);

	for (i=0;i<cyclecount;i++)
	{
		cycles[i].draw(pos, alpha);
	}

	const float alusstart = 0.8f;
	const float alusstop = 0.95f;

	const float trailcolor[] = {1, 0.5f, 0, alpha};
	glColor4fv((float *)&trailcolor);

	if (pos >= alusstart && pos <= alusstop)
	{
		float p = Mathematics::calcPosFloat(pos, alusstart, alusstop);
		Vector aluspos = aluspath->getValue(p);
		Vector alusrot = alusrotate->getValue(p);

		glColor4f(1.0f, 0.2f, 0.2f, alpha);
		glPushMatrix();
		Primitives::player(aluspos, alusrot, 4);
		glPopMatrix();

		glLineWidth(4);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE);

		glColor4fv((float *)&trailcolor);
		glBegin(GL_LINES);
		for (float t=0.0f;t<p;t+=0.01f)
		{
			glColor4f(trailcolor[0], trailcolor[1], trailcolor[2], alpha*t);
			glVertex3fv((float *)&aluspath->getValue(t));
		}
		glEnd();
	}
	if (pos >= alusstop)
	{
		glBegin(GL_LINES);
		for (float t=0.0f;t<1.0f;t+=0.01f)
		{
			glColor4f(trailcolor[0], trailcolor[1], trailcolor[2], alpha*t);
			glVertex3fv((float *)&aluspath->getValue(t));
		}
		glEnd();


	}

}
void Tron::renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha)
{
	//p1----p2
	//|     |
	//|     |
	//|     |
	//p4----p3

	int i;
	const int slices = 70;

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

Tron::Tron()
{	
	int i, j;

	cyclecount = 4;
	cycles = new Cycle[cyclecount];

	srand(16);
	for (i=0;i<cyclecount;i++)
	{
		cycles[i].vertexcount = 50;
		cycles[i].vertices = new Vector[cycles[i].vertexcount];

		switch(i%4)
		{
			case 0: cycles[i].color = Vector(1, 0.1f, 0.1f); break;
			case 1: cycles[i].color = Vector(0.1f, 1, 0.1f); break;
			case 2: cycles[i].color = Vector(1, 0.1f, 1.0f); break;
			case 3: cycles[i].color = Vector(0.1f, 0.1f, 1); break;
		}
//		cycles[i].color = Vector(dmsRandFloat(), dmsRandFloat(), dmsRandFloat());
		cycles[i].current = 0;
		cycles[i].starttime = 0;//starttimes[i];
		cycles[i].endtime = 1;//endtimes[i];

		const float ypos = 1.0f;
		Matrix scale;
		scale.makeIdentity();

		scale.data[0][0] = 8;
		scale.data[1][1] = 1;
		scale.data[2][2] = 8;

		Vector point = Vector(((rand()%10000)/10000.0f)-0.5f, ypos, ((rand()%10000)/10000.0f)-0.5f);
		for (j=0;j<cycles[i].vertexcount;j++)
		{
			cycles[i].vertices[j] = point * scale;
			const float minstep = 0.1f;
			const float maxstep = 0.5f;

			Vector newpoint; 
			int maxiter = 20;

			const float step = minstep + ((rand()%10000)/10000.0f) *(maxstep-minstep);
			switch(rand()%4)
			{
				case 0: newpoint = point + Vector(step, 0, 0); break;
				case 1: newpoint = point - Vector(step, 0, 0); break;
				case 2: newpoint = point + Vector(0, 0, step); break;
				case 3: newpoint = point - Vector(0, 0, step); break;
			}
			point = newpoint;


		}
	}
//	campos = Vector(pos*4, 8-pos, -13.0f+pos*8);

	cams = new Spline*[4];
	cams[0] = new Spline();
	cams[1] = new Spline();
	cams[2] = new Spline();
	cams[3] = new Spline();

	cams[0]->addpoint(Vector(1, 8, -13));
	cams[0]->addpoint(Vector(3, 5, -7));
	cams[0]->addpoint(Vector(6, 6, -4));
	cams[0]->addpoint(Vector(9, 5, -6));

	cams[1]->addpoint(Vector(4, 8, -6));
	cams[1]->addpoint(Vector(-2, 7, -4));
	cams[1]->addpoint(Vector(-6, 4, -3));
	cams[1]->addpoint(Vector(-7, 3, -6));

	cams[2]->addpoint(Vector(9, 4, 6));
	cams[2]->addpoint(Vector(7, 5, 4));
	cams[2]->addpoint(Vector(5, 6, 2));
	cams[2]->addpoint(Vector(4, 7, 1));

	cams[3]->addpoint(Vector(4, 5, -3));
	cams[3]->addpoint(Vector(3, 10, -2));
	cams[3]->addpoint(Vector(2, 30, -2));
	cams[3]->addpoint(Vector(1, 50, -3));

	targets = new Spline*[4];
	targets[0] = new Spline();
	targets[1] = new Spline();
	targets[2] = new Spline();
	targets[3] = new Spline();

	targets[0]->addpoint(Vector(0, 2, 0));
	targets[1]->addpoint(Vector(0, 2, 0));
	targets[2]->addpoint(Vector(0, 2, 0));
	targets[3]->addpoint(Vector(0, 2, 0));

	aluspath = new Spline();
	alusrotate = new Spline();
/*
	aluspath->addpoint(Vector(10, 2, -10));
	aluspath->addpoint(Vector(6, 3, -6));
	aluspath->addpoint(Vector(2, 15, -3));
	aluspath->addpoint(Vector(-1, 60, 2));

	alusrotate->addpoint(Vector(0.02f, -0.15f, 0));
	alusrotate->addpoint(Vector(0.04f, -0.15f, 0));
	alusrotate->addpoint(Vector(0.15f, -0.10f, 0));
	alusrotate->addpoint(Vector(0.16f, -0.07f, 0));
*/
	aluspath->addpoint(Vector(0, 2, -15));
	aluspath->addpoint(Vector(0, 2, -9));
	aluspath->addpoint(Vector(0, 11, -1));
	aluspath->addpoint(Vector(0, 65, 3));

	alusrotate->addpoint(Vector(0, 0, 0));
	alusrotate->addpoint(Vector(0.1f, 0, 0));
	alusrotate->addpoint(Vector(0.2f, 0, 0));
	alusrotate->addpoint(Vector(0.3f, 0, 0));
	alusrotate->addpoint(Vector(0.34f, 0, 0));
}

void Cycle::draw(float pos, float alpha)
{
	int i;

	if (pos < starttime || pos > endtime)
		return;
	
	myDisable(MY_TEXTURE_2D);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);
	glLineWidth(4.0f);
	glColor4f(color.x, color.y, color.z, alpha*0.8f);

	const float kohta = Mathematics::calcPosFloat(pos, starttime, endtime) * vertexcount;
	const float frac = kohta - (int)kohta;

	int alaraja = (int)floor(kohta);
	int ylaraja = (int)ceil(kohta);

	glBegin(GL_LINE_STRIP);
	for (i=0;i<=alaraja;i++)
	{
		glVertex3fv((float *)&vertices[i]);
	}
	glEnd();

	if (ylaraja < vertexcount)
	{
		Vector target = vertices[alaraja]*(1-frac) + vertices[ylaraja]*(frac);
		glBegin(GL_LINES);
		glVertex3fv((float *)&vertices[alaraja]);
		glVertex3fv((float *)&target);
		glEnd();
	}

}

Tron::~Tron()
{
	int i;

	for (i=0;i<4;i++)
	{
		delete cams[i];
	}
	delete [] cams;
	for (i=0;i<4;i++)
	{
		delete targets[i];
	}
	delete [] targets;

	for (i=0;i<cyclecount;i++)
	{
		delete [] cycles[i].vertices;
	}
	delete [] cycles;

	delete aluspath;
	delete alusrotate;
}

bool Tron::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

