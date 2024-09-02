#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "City.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void City::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.70f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define CITY_GLOW

#ifndef CITY_GLOW
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
	const float radiusmod = 0.001f;
	const int count = 12;

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
	dmsPerspective2D(1,1);
	const float a = (1-Mathematics::calcPosFloat(pos, 0.8f,0.83f))*0.2f;
	if (a > 0 && pos > 0.8f && pos < 0.83f)
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

void City::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();

	Vector add = Vector(0, 0, -1)*1000*(float)pow(1-pos, 100);

	Vector c = cam->getValue(pos) + add;
	Vector t = tar->getValue(pos) + add;
	Vector r = rot->getValue(pos);

	gluLookAt(c.x, c.y, c.z, t.x, t.y, t.z, r.x, r.y, r.z);

	float fogcol[] = {0, 0, 0, 1};

	glFogi(GL_FOG_MODE, GL_LINEAR); //EXP, EXP2, LINEAR
	glFogfv(GL_FOG_COLOR, fogcol);
	glFogf(GL_FOG_DENSITY, 0.02f);//+0.4f*(float)cos(pos*10));
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 210);
	glFogf(GL_FOG_END, 250);
	myEnable(MY_FOG);

	const float gs = 250;
	const float gy = 0;
	Vector g1 = Vector(-gs, gy, -gs);
	Vector g2 = Vector( gs, gy, -gs);
	Vector g3 = Vector( gs, gy,  gs);
	Vector g4 = Vector(-gs, gy,  gs);
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_BLEND);
	myEnable(MY_LINE_SMOOTH);

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float groundcolor[] = {0.2f, 0.3f, 0.8f, alpha};
	glLineWidth(1);
	glColor4fv((float *)&groundcolor);//0.2f, 0.2f, 0.4f, alpha*1.0f);
	Primitives::renderProperGrid(g1, g2, g3, g4, 60);

	float buildingcolor[] = {0.3f, 0.4f, 0.9f, alpha*0.7f};
	glColor4fv((float *)&buildingcolor);//0.2f, 0.2f, 0.4f, alpha*1.0f);

	const int talocount = 5;
	Vector talot[talocount*2] = {
		Vector(-10, 90, 170),
		Vector( 15, 0,  150),
		Vector(  0, 110, 170),
		Vector( -25, 0,  150),
		Vector(-10, 50, 130),
		Vector( 10, 0,  150),
		Vector(-35, 40, 120),
		Vector(-10, 0,  140),
		Vector( 10,  70,  140),
		Vector( 25,  0,  120),
	};
	for (i=0;i<talocount;i++)
	{
		Primitives::wireBox(talot[i*2], talot[i*2+1]);
	}

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glLineWidth(1);
	for (i=0;i<cubecount;i++)
	{
		glPushMatrix();
		glTranslatef(cubepos[i].x, cubepos[i].y, cubepos[i].z);

		Vector cuberot = cuberots[i]*pos + cuberots2[i]*(1-pos);
		glRotatef(360*cuberot.x, 1, 0, 0);
		glRotatef(360*cuberot.y, 0, 1, 0);
		glRotatef(360*cuberot.z, 0, 0, 1);

		glColor4f(cubecols[i].x, cubecols[i].y, cubecols[i].z, cubealphas[i]*alpha);
//		glColor4f(1,1,1,1);
		Primitives::wireCube(cubesizes[i]);
		glPopMatrix();
	}
	
	Vector p1 = Vector(0, 0, -50);
	Vector p2 = Vector(0, 20, 440);

	const float alusstart = 0.80f;
	const float alusend = 1.0f;
	const float aluspos = Mathematics::calcPosFloat(pos, alusstart, alusend);

	myDisable(MY_FOG);
	if (aluspos > 0.001f && aluspos < 0.999f)
	{
		float a = aluspos*10;
		if (a > 1)
			a = 1;
		glColor4f(1.0f, 0.2f, 0.2f, alpha*a);
		Primitives::player(p1*(1-aluspos) + p2*aluspos, Vector(0.0f, 0, 0.1f*(float)sin(pos*8)), 50);
	}
	
	/*
	glLineWidth(2);
	float splinecolor[] = {0.8f, 0.8f, 0.3f, alpha*1.0f};
	glColor4fv((float *)&splinecolor);

	for (i=0;i<splinecount;i++)
	{
		const int nauhoja = 12;
		const float nauhanpituus = 0.04f;
		const float vali = (1-nauhoja*nauhanpituus)/nauhoja;
		
		for (int j=0;j<nauhoja;j++)
		{
			const float startkohta = j*nauhanpituus + j*vali;
			glBegin(GL_LINE_STRIP);

			for (float t=startkohta;t<startkohta+nauhanpituus;t += nauhanpituus/20.0f)
			{
				Vector v = paths[i]->getValue(t);
				glVertex3fv((float *)&v);
			}
			glEnd();
		}
	}
*/
}

City::City()
{	
	int i;

	cam = new Spline();
	tar = new Spline();
	rot = new Spline();

	cam->addpoint(Vector(0, 5, -100));
	cam->addpoint(Vector(0, 8, -80));
	cam->addpoint(Vector(5, 11, -70));

	tar->addpoint(Vector(0, -15, 0));
	tar->addpoint(Vector(0, 0, 0));
	tar->addpoint(Vector(0, 15, 0));
	tar->addpoint(Vector(0, 25, 0));

	rot->addpoint(Vector(0, 1, 0));
	rot->addpoint(Vector(0, 1, 0));
	rot->addpoint(Vector(-0.02f, 1, 0));
	rot->addpoint(Vector(-0.04f, 1, 0));

	cubecount = 26;
	cubesizes = new float[cubecount];
	cubealphas = new float[cubecount];
	cubepos = new Vector[cubecount];
	cuberots = new Vector[cubecount];
	cuberots2 = new Vector[cubecount];
	cubecols = new Vector[cubecount];

	srand(715517);
	for (i=0;i<cubecount;i++)
	{
		Matrix scale;
		scale.makeIdentity();

		scale.data[0][0] = 200;
		scale.data[1][1] = 60;
		scale.data[2][2] = 100;

		Vector startpos = Vector(0, 0, 40);
		cubesizes[i] = 2 + Mathematics::randFloat()*4;
		cubealphas[i] = 0.5f + Mathematics::randFloat()*0.3f;
		cubepos[i] = Vector(Mathematics::randFloat()-0.5f, 
							Mathematics::randFloat(), 
							Mathematics::randFloat())*scale + startpos;

		const float rotspeed = 0.5f;
		cuberots[i] = Vector(Mathematics::randFloat(),
							 Mathematics::randFloat(),
							 Mathematics::randFloat())*rotspeed;
		cuberots2[i] = Vector(Mathematics::randFloat(),
							 Mathematics::randFloat(),
							 Mathematics::randFloat())*rotspeed;

		Vector col = Vector(0, 0, 0);
		col.x += rand()%2;
		col.y += rand()%2;
		col.z += rand()%2;
		cubecols[i] = col;


	}

	splinecount = 15;
	paths = new Spline*[splinecount];

	for (i=0;i<splinecount;i++)
	{
		paths[i] = new Spline();

		Vector centerpoint = Vector(0, 110, 140);
		const float a = (i / (float)splinecount)*2*3.141592f;
		const float r1 = 5;
		const float r2 = 150;

		Vector p1 = centerpoint + Vector((float)sin(a), 0, (float)cos(a))*r1;
		Vector p2 = centerpoint + Vector((float)sin(a), 0, (float)cos(a))*r2;

		const int pointcount = 40;

		for (int j=0;j<pointcount;j++)
		{
			const float t = j / (float)pointcount;
			Vector p = p1 * t + p2 * (1-t);

			p += Vector(Mathematics::randFloat()-0.5f, 
						Mathematics::randFloat()-0.5f, 
						Mathematics::randFloat()-0.5f)*2;
			paths[i]->addpoint(p);
		}
	}
}

City::~City()
{
	int i;
	for (i=0;i<splinecount;i++)
	{
		delete paths[i];
	}
	delete [] paths;

	delete [] cubesizes;
	delete [] cubealphas;
	delete [] cubepos;
	delete [] cuberots;
	delete [] cuberots2;
	delete [] cubecols;
	
	delete cam;
	delete tar;
}

bool City::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

