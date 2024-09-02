#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Tetris.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Tetris::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.02f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define TETRIS_GLOW

#ifndef TETRIS_GLOW
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

void Tetris::renderScene(float pos, float alpha)
{
	int x, y;
	glLoadIdentity();

	static int framespassed = 0;
	const int framecount = 49;
	static float frametime = 0.0f;
	static float framestep = 1.0f / framecount;
	static float lastframe = 0.0f;
	static char arena2[20*8];

	static int piecenum = 0;
	float dt = pos - lastframe;
	lastframe = pos;

	frametime += dt;

	char xmovetable[framecount] = {
		0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,-1,-1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		1,-1, 1, 1, 0, 0, 0, 0, 0};

	static int piecex = 0;
	static int piecey = 0;

	if (frametime > framestep)
	{
		frametime -= framestep;
		framespassed++;

		piecey++;
		piecex += xmovetable[framespassed];

		if (framespassed == 14)
		{
			piecey = 0;
			piecex = 0;
			piecenum++;

			//yres = 20
			//xres = 8
			arena[5 + 6*8] = 1;
			arena[5 + 5*8] = 1;
			arena[6 + 6*8] = 1;
			arena[6 + 7*8] = 1;
		}
		if (framespassed == 17)
		{
			piecey = 0;
			piecex = 0;
			piecenum++;

			//kopioidaan areena2:een
			for (x=0;x<xres;x++)
			{
				for (y=0;y<yres-4;y++)
				{
					arena2[x+y*xres] = arena[x+(y+4)*xres];
					if (y < 4)
						arena[x+y*xres] = 1;
				}
			}
		}
		if (framespassed == 18)
		{
			for (x=0;x<xres;x++)
			{
				for (y=0;y<4;y++)
				{
					arena[x+y*xres] = 0;//arena[x+(y+4)*xres];
				}
			}
		}
		if (framespassed == 19)
		{
			for (x=0;x<xres;x++)
			{
				for (y=0;y<4;y++)
				{
					arena[x+y*xres] = 1;//arena[x+(y+4)*xres];
				}
			}
		}
		if (framespassed == 20)
		{
			for (x=0;x<xres;x++)
			{
				for (y=0;y<yres;y++)
				{
					arena[x+y*xres] = arena2[x+y*xres];
				}
			}
		}
		if (framespassed == 34)
		{
			piecex = 0;
			piecey = 0;
			piecenum++;
			arena[4+3*xres] = 1;
			arena[5+3*xres] = 1;
			arena[4+4*xres] = 1;
			arena[5+4*xres] = 1;
		}
		if (framespassed == 37)
		{
			piecenum++;
		}
	}

//	Vector campos = Vector(pos*4, 3-pos, -13.0f+pos*8);
//	Vector camtarget = Vector(0, 2, 0);

	Vector campos = camera->getValue(pos);
	Vector camtarget = target->getValue(pos);
	Vector camrot = rotate->getValue(pos);

	Vector add = Vector(0, 1, 0)*(float)pow(pos+0.02f, 280)*15;
	campos += add;
	camtarget += add*0.5f;

//	glTranslatef(0, 6, -7.0f);

	gluLookAt(campos.x, campos.y, campos.z, camtarget.x, camtarget.y, camtarget.z, camrot.x, camrot.y, camrot.z);

	myEnable(MY_LINE_SMOOTH);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);
	myDisable(MY_TEXTURE_2D);

	const float planesize = 120;
	Vector a1 = Vector(-planesize, 0, -planesize);
	Vector a2 = Vector(planesize, 0, -planesize);
	Vector a3 = Vector(planesize, 0, planesize);
	Vector a4 = Vector(-planesize, 0, planesize);

	Vector color = Vector(0.3f, 0.5f, 0.96f);
	glColor4f(color.x, color.y, color.z, alpha*0.5f);

	glLineWidth(1.0f);
	renderGrid(a1, a2, a3, a4, alpha);
	const float h = 10;
	Vector a5 = Vector(-planesize, h, -planesize);
	Vector a6 = Vector(planesize, h, -planesize);
	Vector a7 = Vector(planesize, h, planesize);
	Vector a8 = Vector(-planesize, h, planesize);
	renderGrid(a5, a6, a7, a8, alpha);



	glColor4f(1,1,1,alpha);
	const float koko = 25;
	const float greety = 2.5f;

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myDisable(MY_DEPTH_TEST);

	Vector pos1 = Vector(-koko, greety, -koko);
	Vector pos2 = Vector(koko, greety, -koko);
	Vector pos3 = Vector(koko, greety, koko);
	Vector pos4 = Vector(-koko, greety, koko);

//	char string1[] = "ANS ASD CNS DDV DIV FKT";
//	char string2[] = "JML FLT GLT HDL LAG MTC";
//	char string3[] = "MFX N.C NMC OTB PDZ PLT";
//	char string4[] = "PYR STC SGZ STS TPD U.D";
	char string1[] = "ANS ASD CNS DDV DIV FKT JML FLT GLT";
	char string2[] = "HDL LAG MTC MFX N.C NMC OTB PDZ PLT";
	char string3[] = "PYR STC SGZ STS TPD U.D";

	glPushMatrix();
	
	glPushMatrix();
//	glRotatef(180, 0, 1, 0);
	dmsWrite3D(pos1.x, pos1.y, pos1.z, alpha, "arcade.pcx", (char *)string1);
	glPopMatrix();
	//palikka
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	dmsWrite3D(-pos2.x, pos2.y, pos2.z, alpha, "arcade.pcx", (char *)string2);
	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	dmsWrite3D(-pos3.x, pos3.y, - pos3.z, alpha, "arcade.pcx", (char *)string3);
	glPopMatrix();


	glPushMatrix();
	glRotatef(90, 0, 1, 0);
//	dmsWrite3D(-2*pos4.x, pos4.y, pos4.z, alpha, "arcade.pcx", (char *)string4);
	glPopMatrix();
	
	glPopMatrix();
	myDisable(MY_DEPTH_TEST);
	myEnable(MY_BLEND);
	myDisable(MY_TEXTURE_2D);

	Matrix scale;
	scale.data[0][0] = 0.2f;
	scale.data[1][1] = 0.2f;
	scale.data[2][2] = 0.2f;
	Vector basepos = Vector(0, 2.5f, 0);

	Matrix edgescale;
	edgescale.data[0][0] = 9.5f;
	edgescale.data[1][1] = 1;
	edgescale.data[2][2] = 1.2f;//(4, 1, 1.2f);

	Vector uppos = Vector(-0.125f, 4.75f, 0);
	Vector downpos = Vector(-0.125f, 0.35f, 0);

	const float edge = 0.1f;
	Vector c1 = downpos + Vector(-edge, -edge, -edge)*edgescale;
	Vector c2 = downpos + Vector(edge, -edge, -edge)*edgescale;
	Vector c3 = downpos + Vector(edge, -edge, edge)*edgescale;
	Vector c4 = downpos + Vector(-edge, -edge, edge)*edgescale;

	Vector c5 = uppos + Vector(-edge, 0, -edge)*edgescale;//*scale;
	Vector c6 = uppos + Vector(edge, 0, -edge)*edgescale;//*scale;
	Vector c7 = uppos + Vector(edge, 0, edge)*edgescale;//*scale;
	Vector c8 = uppos + Vector(-edge, 0, edge)*edgescale  ;//*scale;

	glLineWidth(3);
//	glColor4f(color.x, color.y, color.z, alpha*1.0f);
	glColor4f(0.96f, 0.3f, 0.2f, alpha*1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float *)&c1);
	glVertex3fv((float *)&c2);
	glVertex3fv((float *)&c3);
	glVertex3fv((float *)&c4);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float *)&c5);
	glVertex3fv((float *)&c6);
	glVertex3fv((float *)&c7);
	glVertex3fv((float *)&c8);
	glEnd();
	glBegin(GL_LINES);
	glVertex3fv((float *)&c1);
	glVertex3fv((float *)&c5);
	glVertex3fv((float *)&c2);
	glVertex3fv((float *)&c6);
	glVertex3fv((float *)&c3);
	glVertex3fv((float *)&c7);
	glVertex3fv((float *)&c4);
	glVertex3fv((float *)&c8);
	glEnd();


	const float cubesize = 0.1f;
	Vector palikkapos = basepos + Vector(0 + (float)piecex, yres*0.5f - piecey, 0)*scale;
	glPushMatrix();
	glTranslatef(palikkapos.x, palikkapos.y, palikkapos.z);
//	glColor4f(1, 0, 0, 1);
	glLineWidth(2.0f);
	glColor4f(color.x, color.y, color.z, alpha*0.4f);
	if (piecenum==0)
	{
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(-cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
	}
	if (piecenum == 1)
	{
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
	}
	if (piecenum == 2)
	{
		Primitives::wireCube(cubesize);
		glTranslatef(cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(-cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, cubesize*2, 0);
	}
	if (piecenum == 3)
	{
		Primitives::wireCube(cubesize);
		glTranslatef(cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(-cubesize*4, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
	}
	if (piecenum == 4)
	{
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(0, -cubesize*2, 0);
		Primitives::wireCube(cubesize);
		glTranslatef(cubesize*2, 0, 0);
		Primitives::wireCube(cubesize);
	}

	glPopMatrix();

	glColor4f(color.x, color.y, color.z, alpha*0.4f);
	glLineWidth(2.0f);
	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			Vector pos = basepos + Vector(x - xres*0.5f, y - yres*0.5f, 0)*scale;

			if (arena[x+y*xres])
			{
				glPushMatrix();
				glTranslatef(pos.x, pos.y, pos.z);
				Primitives::wireCube(cubesize);
				glPopMatrix();


			}

		}
	}


}
void Tetris::renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, float alpha)
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

Tetris::Tetris()
{	
	int x, y;
	xres = 8;
	yres = 20;
	arena = new unsigned char [xres*yres];

	unsigned char adds[8] = {2, 1,5, 6, 3, 1, 2, 4};

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			if (x==2)
				arena[x+y*xres]=0;
			else
			{
				if (y > 3+adds[x])
					arena[x+y*xres] = 0;
				else
					arena[x+y*xres] = 1;

			}
		}
	}

	camera = new Spline();
	target = new Spline();
	rotate = new Spline();
//	Vector campos = Vector(pos*4, 3-pos, -13.0f+pos*8);
//	Vector camtarget = Vector(0, 2, 0);

	camera->addpoint(Vector(0, 3, -13));
	camera->addpoint(Vector(1, 3, -3));
	camera->addpoint(Vector(4, 2, 4));
	camera->addpoint(Vector(1, 1, 6));
	camera->addpoint(Vector(-4, 4, 1));
	camera->addpoint(Vector(-2, 2, -11));
	camera->addpoint(Vector(-1, 1, -4));

	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(0, 1, 0));
	rotate->addpoint(Vector(-0.2f, 1.2f, 0.2f));
	rotate->addpoint(Vector(0.2f, 0.8f, 0));
	rotate->addpoint(Vector(0.4f, 0.6f, 0));
	rotate->addpoint(Vector(0.1f, 0.9f, 0));
	rotate->addpoint(Vector(0, 1, 0));	

	const float height = 2.5f;
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
	target->addpoint(Vector(0, height, 0));
}

Tetris::~Tetris()
{
	delete [] arena;
	delete camera;
	delete target;
	delete rotate;
}

bool Tetris::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

