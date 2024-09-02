#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Pong.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Pong::draw()
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

#define PONG_GLOW

#ifndef PONG_GLOW
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
	
//	float blendalpha = 0.2f;
//	const float radiusmod = 0.0015f;
//	const int count = 16;

	float blendalpha = 0.25f;
	const float radiusmod = 0.0015f;
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
}

void PongGame::render(double dt, float aika, float alpha)
{
/*	glLoadIdentity();
	glTranslatef(0, 0, -17.0f);

//	glRotatef(pos*4000, 0.4f, 0.8f, 0.89f);
*/
	moveStuff((float)dt);

	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glPushMatrix();
	glTranslatef(gamepos.x, gamepos.y, gamepos.z);

	Vector r = rotation;
	glRotatef(360*r.x, 1, 0, 0);
	glRotatef(360*r.y, 0, 1, 0);
	glRotatef(360*r.z, 0, 0, 1);
	glLineWidth(2.0f);
	drawBat(bat1, aika, alpha);
	drawBat(bat2, aika, alpha);
	drawBall(ball, aika, alpha);

	glPopMatrix();
}

void PongGame::moveStuff(float dt)
{
 	ball += dir*ballspeed*dt;

	if (dir.y > 0) //ylämailaa kohti menossa
	{
		if (bat2.x < ball.x)
			bat2.x += (float)batspeed*dt;
		if (bat2.x > ball.x)
			bat2.x -= batspeed*dt;
	}
	if (dir.y < 0) //alamailaa kohti menossa
	{
		if (bat1.x < ball.x)
			bat1.x += batspeed*dt;
		if (bat1.x > ball.x)
			bat1.x -= batspeed*dt;
	}

	const float epsilon = 0.5f;
	//pallo

	//bat1 = ylämaila
	if (ball.y < bat1.y)
	{
		dir.y = -dir.y;
		if (rand()%2)
			dir.x = -dir.x;

		ball.y = bat1.y + epsilon;
	}
	if (ball.y > bat2.y)
	{
		dir.y = -dir.y;
		if (rand()%2)
			dir.x = -dir.x;

		ball.y = bat2.y - epsilon;
	}



}

void PongGame::drawBat(Vector &pos, float aika, float alpha)
{
	int i;
	const int pieces = 5;
	const float size = 0.2f;

//	glColor4f(0.3f, 0.5f, 0.75f, alpha*0.2f);
	glColor4f(0.1f, 0.3f, 0.75f, alpha*0.3f);
	for (i=0;i<pieces;i++)
	{
		glPushMatrix();
		Vector kohta = pos + Vector((i-pieces*0.5f)*size*2, 0, 0);
		glTranslatef(kohta.x, kohta.y, kohta.z);
		Primitives::wireCube(size);
		glPopMatrix();
	}
}

void PongGame::drawBall(Vector &pos, float aika, float alpha)
{
	const float size = 0.2f;

//	glColor4f(0.3f, 0.75f, 0.5f, alpha*0.35f);
	glColor4f(0.1f, 0.75f, 0.3f, alpha*0.45f);
	glPushMatrix();
	Vector kohta = pos + Vector(-size*0.5f, 0, 0);// + Vector((i-pieces*0.5f)*size*2, 0, 0);
	glTranslatef(kohta.x, kohta.y, kohta.z);
	glRotatef(aika*360*23, 0.3f*(float)cos(aika*25), 0.2f*(float)cos(aika*17), 0.6f*(float)sin(aika*35)-aika*0.5f);
	Primitives::wireCube(size);
	glPopMatrix();

}

PongGame::PongGame(Vector &pos)
{	
	bat1 = Vector(0, -4, 0);
	bat2 = Vector(0,  4, 0);
	ball = Vector(0, 0, 0);

	dir = Vector(1, 2, 0).normalize();
	gamepos = pos;

	batspeed = 100 + 150*dmsRandFloat();
	ballspeed = batspeed - 40*dmsRandFloat();

	batspeed *= 3;
	ballspeed *= 3;

	rotation = Vector(dmsRandFloat()-0.5f, dmsRandFloat()-0.5f, dmsRandFloat()-0.5f)*2;
}

void Pong::renderScene(float pos, float alpha)
{
	int i;

	glLoadIdentity();
	
	Vector campos = kamera->getValue(pos);//Vector(0, 0, -50.0f-pos*50);
	Vector camtarget = target->getValue(pos);//Vector(0, 00, 0);

	Vector add = Vector(0, -1, 0)*(float)pow(1-pos, 20)*25;
	campos += add;
	camtarget += add;


	gluLookAt(campos.x, campos.y, campos.z, camtarget.x, camtarget.y, camtarget.z, 0, 1, 0);

	const float size = 1240;
	const float y = 30;
	Vector c1 = Vector(-size, y, -size);
	Vector c2 = Vector(size, y, -size);
	Vector c3 = Vector(size, y, size);
	Vector c4 = Vector(-size, y, size);

	Vector c5 = Vector(-size, -y, -size);
	Vector c6 = Vector(size, -y, -size);
	Vector c7 = Vector(size, -y, size);
	Vector c8 = Vector(-size, -y, size);

	myDisable(MY_TEXTURE_2D);

	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	myEnable(MY_LINE_SMOOTH);
	glLineWidth(2.0f);
	glColor4f(0.9f, 0.4f, 0.3f, alpha*0.7f);
	Primitives::renderGrid(c1, c2, c3, c4, 50);
	Primitives::renderGrid(c5, c6, c7, c8, 50);

	glPushMatrix();
	glRotatef(pos*10, 0.3f, 0.6f, 0.9f); 

	double diff = pos - previousframe;
	previousframe = pos;
	for (i=0;i<gamecount;i++)
	{
		games[i]->render(diff, pos, alpha);
	}

	glPopMatrix();

}

Pong::Pong()
{
	int i;
	gamecount = 130;

	games = new PongGame*[gamecount];

	srand(16);
	for (i=0;i<gamecount;i++)
	{
		Matrix scale;
		scale.data[0][0] = 125;
		scale.data[1][1] = 25;
		scale.data[2][2] = 125;
		games[i] = new PongGame(Vector(dmsRandFloat()-0.5f, dmsRandFloat()-0.5f, dmsRandFloat()-0.5f)*scale);
	}

	previousframe = 0;

	kamera = new Spline();
	target = new Spline();
/*	kamera->addpoint(Vector(0, 0, -80));
	kamera->addpoint(Vector(0, 0, -60));
	kamera->addpoint(Vector(20, 3, -50));
	kamera->addpoint(Vector(30, 7, -30));
	kamera->addpoint(Vector(10, 1, 20));
	kamera->addpoint(Vector(-50, -5, 60));
*/
	kamera->addpoint(Vector(0, 0, -80));
	kamera->addpoint(Vector(0, 0, -60));
	kamera->addpoint(Vector(20, 3, -50));
	kamera->addpoint(Vector(30, 7, -30));
	kamera->addpoint(Vector(10, 1, 20));
	kamera->addpoint(Vector(-10, -4, 50));
	kamera->addpoint(Vector(-50, -7, 80));
	kamera->addpoint(Vector(-30, 2, 20));
	kamera->addpoint(Vector(15, 4, -20));
	kamera->addpoint(Vector(35, 8, 0));
	kamera->addpoint(Vector(40, 1, 10));
	kamera->addpoint(Vector(-50, -5, 60));
//	kamera->addpoint(Vector(50, -5, -60));
	kamera->addpoint(Vector(250, -5, -300));
	kamera->addpoint(Vector(250*5, -5, -300*5));

	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
//	target->addpoint(Vector(100, -5, -120));
	target->addpoint(Vector(300, -5, -360));
	target->addpoint(Vector(300*5, -5, -360*5));

}


Pong::~Pong()
{
	int i;
	for (i=0;i<gamecount;i++)
	{
		delete games[i];

	}
	delete [] games;
	delete kamera;
	delete target;


}

bool Pong::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

