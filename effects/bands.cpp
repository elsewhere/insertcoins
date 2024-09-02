//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>
#include "bands.hpp"
#include "../primitives.hpp"
#include "../mathematics.hpp"

#define NUM_NAUHAT 500

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Bands::Bands()
{
	nauha = NULL;
}

Bands::~Bands()
{	
	if(nauha)
	{
		delete [] nauha;
		nauha = NULL;
	}

	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------
bool Bands::init(unsigned long s, unsigned long e)
{		
	unsigned int i;

	active = false;
	startTime = s;
	endTime = e;	
	lastTime = (float)s;
	deltaTime = lastTime - startTime;

	nauha = new Band[NUM_NAUHAT];
	if(!nauha)
	{
		return false;
	}

	flare = dmsGetTexture("particle.jpg");
	if(!flare)
	{
		dmsMsg("Couldn't find texture");
		return false;
	}	

	float var, var2, var3;

	for(i = 0; i < NUM_NAUHAT; i++)
	{
		nauha[i].start = Vector(0.0f, 0.0f,   0.0f);

		const int size = 200;

		var  = float(rand()%size - rand()%size);
		var2 = float(rand()%size - rand()%size);
		var3 = float(rand()%size - rand()%size);

		nauha[i].p1 = Vector(var, var2, var3);

		var  = float(rand()%size - rand()%size);
		var2 = float(rand()%size - rand()%size);
		var3 = float(rand()%size - rand()%size);

		nauha[i].p2 = Vector(var, var2, var3);

		var  = float(rand()%size - rand()%size);
		var2 = float(rand()%size - rand()%size);
		var3 = float(rand()%size - rand()%size);

		nauha[i].end = Vector(var, var2, var3);
		nauha[i].curve.set(nauha[i].start, nauha[i].p1, nauha[i].p2, nauha[i].end);

		nauha[i].alpha = 1.0f;
		nauha[i].speed = ((rand()%255) / 255.0f) + 10.0f + rand()%25;
		nauha[i].strips = 50;
		nauha[i].phase = (float)(rand()%endTime);//0.0f;

		nauha[i].time = (rand()%endTime) * 0.5f;
//		nauha[i].time = 0.0f;
		nauha[i].startTime = (float)startTime;
		nauha[i].endTime = (float)endTime;

		nauha[i].fadeInStart = 0.0f;
		nauha[i].fadeInEnd = 0.4f;
		nauha[i].fadeOutStart = 0.6f;
		nauha[i].fadeOutEnd = 1.0f;

		if(i % 2 == 0)
		{
			nauha[i].r = 1.0f;
			nauha[i].g = 0.5f;
			nauha[i].b = 0.25f;
		}
		else
		{
			nauha[i].r = 0.3f;
			nauha[i].g = 0.5f;
			nauha[i].b = 1.0f;
		}
	}
				
	return true;
}

void Bands::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.90f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos < fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);



#define TUNNEL_GLOW

#ifndef TUNNEL_GLOW
	renderScene();
#else
	glViewport(0, 0, 512, 512);
	renderScene();
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
	const float radiusmod = 0.017f;
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

	const float t1alku = 0.04f;
	const float t1loppu = 0.30f;
	const float t2alku = 0.34f;
	const float t2loppu = 0.60f;
	const float t3alku = 0.64f;
	const float t3loppu = 0.90f;
	const float t4alku = 0.75f;
	const float t4loppu = 0.95f;

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
	myDisable(MY_DEPTH_TEST);

	dmsWrite2D(0.7f, 0.7f, t1alpha, 1, "arcade.pcx",   "Code");
	dmsWrite2D(0.71f, 0.74f, t1alpha, 1, "arcade.pcx", "Preacher");
	dmsWrite2D(0.71f, 0.78f, t1alpha, 1, "arcade.pcx", "Vulture");
	dmsWrite2D(0.7f, 0.7f, t2alpha, 1, "arcade.pcx",   "Music");
	dmsWrite2D(0.71f, 0.74f, t2alpha, 1, "arcade.pcx", "Leopart");
	dmsWrite2D(0.71f, 0.78f, t2alpha, 1, "arcade.pcx", "7eebr");
	dmsWrite2D(0.7f, 0.7f, t3alpha, 1, "arcade.pcx",   "Design");
	dmsWrite2D(0.71f, 0.74f, t3alpha, 1, "arcade.pcx", "Preacher");
	dmsWrite2D(0.71f, 0.78f, t3alpha, 1, "arcade.pcx", "Napsa");
	dmsWrite2D(0.71f, 0.82f, t3alpha, 1, "arcade.pcx", "7eebr");
//	dmsWrite2D(0.25f, 0.4f, t1alpha, 1,  "arcade.pcx", "Traction and deMarche");
//	dmsWrite2D(0.23f, 0.45f, t2alpha, 1, "arcade.pcx", "       present       ");
//	dmsWrite2D(0.25f, 0.48f, t3alpha, 1, "arcade.pcx", "    Insert Coins     ");
//	dmsWrite2D(0.26f, 0.60f, t4alpha, 1, "arcade.pcx", "        for         ");
//	dmsWrite2D(0.25f, 0.65f, t4alpha, 1, "arcade.pcx", "      TUM 2005       ");





}

void Bands::renderScene()	
{			
	unsigned int i;
	float t, step, matrix[16];
	Vector v1, v2, v3, v4, x, y, z;
	Vector current;
	Vector cameraPosition;

	const float pos = (time - startTime) / (endTime - startTime);
	const float globalalpha = dmsCalcSaturate(pos, 0, 1, 15)*0.71f;

	const float camspeed = 0.4f;
	cameraPosition.x = (float)(80.0f * sin( camspeed*time * 0.0001f ) - 80.0f * cos( camspeed*time * 0.0001f ));
	cameraPosition.y = (float)(80.0f * cos( camspeed*time * 0.0005f ) - 80.0f * sin( camspeed*time * 0.0005f ));
	cameraPosition.z = (float)(50.0f * cos( camspeed*time * 0.0001f ) - 50.0f * sin( camspeed*time * 0.0001f ));
	cameraPosition.z += (float)pow(pos, 170)*1000;

	glLoadIdentity();
	const float gs = 30;
	const float gz = -20;//+(float)pow(pos, 8)*40;
	Vector g1 = Vector(-gs, -gs, gz);// - xd - yd;
	Vector g2 = Vector(gs, -gs, gz);// + xd - yd;
	Vector g3 = Vector(gs, gs, gz);// + xd + yd;
	Vector g4 = Vector(-gs, gs, gz);// -xd + yd;
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	glLineWidth(1);
	glColor4f(0.2f, 0.2f, 0.2f, globalalpha);
	glRotatef(-1-pos*10, 0, 0, 1);	
	Primitives::renderGrid(g1, g2, g3, g4, 60);

	glLoadIdentity ();
	gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z, 15, -10, 0, 0, 1, 0);
	glRotatef(100.0f * pos, -0.95f, 0.75f, 0.43f);

	// Get projection matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);	
	
	x = (Vector(matrix[0], matrix[4], matrix[8]))*3.0f;
	y = (Vector(matrix[1], matrix[5], matrix[9]))*3.0f;
	z = Vector(matrix[2], matrix[6], matrix[10]);
	
	myDisable(MY_LIGHTING);	
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);		
	myEnable(MY_LINE_SMOOTH);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glLineWidth(2.0f);		

	glBindTexture(GL_TEXTURE_2D, flare->getID());	
	for(i = 0; i < NUM_NAUHAT; i++)
	{				
		myDisable(MY_TEXTURE_2D);		

		// Draw band
		step = 1.0f / nauha[i].strips;
		nauha[i].phase = (nauha[i].time - nauha[i].startTime) / (nauha[i].endTime - nauha[i].startTime);

		if (nauha[i].phase >= nauha[i].fadeInStart && nauha[i].phase <= nauha[i].fadeInEnd)
			nauha[i].alpha = globalalpha * 1.0f * ((nauha[i].phase-nauha[i].fadeInStart) / (nauha[i].fadeInEnd-nauha[i].fadeInStart));
		if (nauha[i].phase >= nauha[i].fadeOutStart && nauha[i].phase <= nauha[i].fadeOutEnd)
			nauha[i].alpha *= globalalpha * (1.0f-((nauha[i].phase-nauha[i].fadeOutStart) / (nauha[i].fadeOutEnd-nauha[i].fadeOutStart)));

		
		glBegin(GL_LINE_STRIP);

			for(t = 0.0f; t < 1.0f + step; t += step)
			{
				current = nauha[i].curve.getValue(t * nauha[i].phase);

			glColor4f(nauha[i].r, nauha[i].g, nauha[i].b, nauha[i].alpha * 0.75f * (1-t)*globalalpha);
				glVertex3f(current.x, current.y, current.z);
			}	
	
		glEnd();

		Vector flarekohta = nauha[i].curve.getValue(0.5f*t*nauha[i].phase);
	
		// Draw billboarded flare
		float size = 2.0f;
		v1 = flarekohta + x + y;
		v2 = flarekohta - x + y;
		v3 = flarekohta + x - y;
		v4 = flarekohta - x - y;
		
		myEnable(MY_TEXTURE_2D);

		//glColor4f(nauha[i].r, nauha[i].g, nauha[i].b, nauha[i].alpha);
		glColor4f(1.0f, 1.0f, 1.0f, nauha[i].alpha);

		glBegin(GL_TRIANGLE_STRIP);
											
			glTexCoord2f(1, 1); glVertex3f(v1.x, v1.y, v1.z);
			glTexCoord2f(0, 1); glVertex3f(v2.x, v2.y, v2.z);
			glTexCoord2f(1, 0); glVertex3f(v3.x, v3.y, v3.z);
			glTexCoord2f(0, 0); glVertex3f(v4.x, v4.y, v4.z);

		glEnd();

		// Update
		nauha[i].time += (deltaTime * nauha[i].speed)*0.5f;
		if(nauha[i].time >= nauha[i].endTime) nauha[i].time = 0.0f;
	}
	
	deltaTime = time - lastTime;
	lastTime = time;
}

