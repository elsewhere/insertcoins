#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Asteroids.hpp"
#include "../mathematics.hpp"
#include "../primitives.hpp"

void Asteroids::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.01f;
	const float fadeoutstart = 0.98f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

#define ASTEROIDS_GLOW

#ifndef ASTEROIDS_GLOW
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
	
	float blendalpha = 0.21f;
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
//	dmsWrite2D(0, 0, 1, 5, "fontti.pcx", "joojoo");
}

Explosion::Explosion(Vector &paikka, int count, float length, float strength)
{
	int i;

	position = paikka;
	particlecount = count;
	time = length;
	alphascale = 1.0f / time;

	particles = new Vector[particlecount];
	directions = new Vector[particlecount];

	for (i=0;i<particlecount;i++)
	{
		float a1 = Mathematics::randFloat();
		float a2 = Mathematics::randFloat();
		float r = Mathematics::randFloat()*0.1f;
		particles[i] = Mathematics::sphereToCartesian(r, a1*3.141592f, a2*2*3.141592f);
		directions[i] = Mathematics::sphereToCartesian(r*(strength+1*Mathematics::randFloat()), a1*3.141592f, a2*2*3.141592f);
	}

}
bool Explosion::update(float dt)
{
	time -= dt;
	if (time < 0.0f)
		return false;

	int i;
	for (i=0;i<particlecount;i++)
	{
		particles[i] += directions[i]*dt;
	}
	return true;
}

void Explosion::draw(float alpha)
{
	int i;
	glPushMatrix();
	glPointSize(2);
	const float a = time*alphascale;
	glColor4f(1,0.8f,0.8f, alpha*a);
	
	glTranslatef(position.x, position.y, position.z);
	myEnable(MY_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glBegin(GL_POINTS);
	for (i=0;i<particlecount;i++)
	{
		glVertex3fv((float *)&particles[i]);
	}
	glEnd();
	glPopMatrix();
}

Explosion::~Explosion()
{
	delete [] particles;
	delete [] directions;
}
Asteroid::Asteroid(int xres, int yres, float koko, float speedmodifier, Vector &pos)
{
	int x, y;
	xdim = xres;
	ydim = yres;
	size = koko;
	vertices = new Vector[xres*yres];

	for (y=0;y<yres;y++)
	{
		for (x=0;x<xres;x++)
		{
			float a1 = x/(float)xres;
			float a2 = y/(float)yres;
			float r = size + (Mathematics::randFloat()-0.5f)*(size*0.5f);
			vertices[x+y*xdim] = Mathematics::sphereToCartesian(r, a1*3.141592f, a2*2*3.141592f);
		}
	}
	float k = Mathematics::randFloat()*2*3.141592f;
	float k2 = Mathematics::randFloat()*2*3.141592f;

	position = pos;
	direction = Vector((float)cos(k), 0, (float)sin(k)) * 8 * speedmodifier;
	rotspeed = Vector(Mathematics::randFloat()-0.5f, Mathematics::randFloat()-0.5f, Mathematics::randFloat()-0.5f)*(1+Mathematics::randFloat())*speedmodifier;
	rotation = Vector(0, 0, 0);
}

void Player::update(float kulma, float dt)
{
	static bool manual = false;
	const float turnspeed = 500;
	if(GetAsyncKeyState(VK_UP)) 
	{
		angle += turnspeed*dt;
		manual = true;
	}
	if(GetAsyncKeyState(VK_DOWN)) 
	{
		angle -= turnspeed*dt;
		manual = true;
	}

	if (manual == false)
	{
		angle = kulma;
	}
//	speed = 41;
	position += Vector((float)sin(angle), 0, (float)cos(angle))*speed*dt;
}

void Player::draw()
{
	Primitives::player(position, Vector(0, angle/(2*3.141592f), 0), 5);
}

void Asteroid::update(float dt)
{
	position += direction*dt;
	rotation += rotspeed*dt;
}

void Asteroid::draw()
{
	int x, y;
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(360*rotation.x, 1, 0, 0);
	glRotatef(360*rotation.y, 0, 1, 0);
	glRotatef(360*rotation.z, 0, 0, 1);
	glBegin(GL_LINES);
	for (y=0;y<ydim;y++)
	{
		for (x=0;x<xdim;x++)
		{
			Vector v1 = vertices[x+y*xdim];
			Vector v2 = vertices[((x+1)%xdim)+y*xdim];
			Vector v3 = vertices[x+((y+1)%ydim)*xdim];

			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v2);
			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v3);
		}
	}
	glEnd();




	glPopMatrix();
}

Asteroid::~Asteroid()
{
	if (vertices)
		delete [] vertices;
}

bool Ammo::update(float dt)
{
	position += direction*dt;
	energyleft -= dt;
	if (energyleft <= 0.0001f)
		return false;
	else
		return true;
}

void Ammo::draw(float alpha)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);

	Vector v1 = Vector(0, 0, 0);//position;
	Vector v2 = direction*-0.0002f;

	glColor4f(0.2f, 0.7f, 0.2f, alpha*energyleft*9);
	glLineWidth(2);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);

	glBegin(GL_LINES);
	glVertex3fv((float *)&v1);
	glVertex3fv((float *)&v2);
	glEnd();

	myDisable(MY_DEPTH_TEST);
	glColor4f(1, 1, 1, alpha*0.1f);
	glBegin(GL_LINES);
	glVertex3fv((float *)&v1);
	glVertex3fv((float *)&v2);
	glEnd();

	glPopMatrix();


}

 void Asteroids::renderScene(float pos, float alpha)
{
	int i, j;

	static float lastframe = 0.0f;
	float thisframe = pos;
	float dt = thisframe - lastframe;
	lastframe = thisframe;

	dt *= 0.2f;

	static float timesub = 0.0f;
	float suuntakulma = pos*15;
	const float ammolimit = 0.005f;//0.004f+0.007f*(float)(0.5f+0.25f*cos(pos*169)+0.25f*sin(pos*121));

//	(float x, float y, float a, float scale, char *fontName, char *msg);

	glLoadIdentity();
//	Vector add = Vector((float)cos(pos*5), (float)sin(pos*7)*3, (float)sin(pos*5));
//	Vector campos = Vector(0.01f, 35+15*(float)cos(pos*11), 0.01f) + add;
//	Vector camtarget = Vector(0, 0, 0) + add;
//	gluLookAt(campos.x, campos.y, campos.z, camtarget.x, camtarget.y, camtarget.z, 0, 1, 0);

	Vector c = cam->getValue(pos);
	Vector t = target->getValue(pos);
	Vector r = rot->getValue(pos);

	gluLookAt(c.x, c.y, c.z, t.x, t.y, t.z, r.x, r.y, r.z);

	const float gs = 250;
	const float gy = -10;
	const float gy2 = 30;
	Vector g1 = Vector(-gs, gy, -gs);
	Vector g2 = Vector( gs, gy, -gs);
	Vector g3 = Vector( gs, gy,  gs);
	Vector g4 = Vector(-gs, gy,  gs);
	Vector g5 = Vector(-gs, gy2, -gs);
	Vector g6 = Vector( gs, gy2, -gs);
	Vector g7 = Vector( gs, gy2,  gs);
	Vector g8 = Vector(-gs, gy2,  gs);
	myDisable(MY_TEXTURE_2D);
	glLineWidth(2);
	glColor4f(0.2f, 0.2f, 0.4f, alpha*0.5f);

	Primitives::renderGrid(g1, g2, g3, g4, 60);
//	Primitives::renderGrid(g5, g6, g7, g8, 60);

//	glTranslatef(0, 0, -17);

	myEnable(MY_BLEND);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	if ((pos-timesub) > ammolimit)
	{
		Matrix plrrot;
		plrrot.makeRotation(0, 0, p1.angle);
		timesub += ammolimit;
		addammo(p1.position, Vector((float)sin(p1.angle), 0, (float)cos(p1.angle))*1235, 0.15f);
	}

	for (i=0;i<maxammocount;i++)
	{
		if (ammoflags[i])
		{
			if (ammo[i].update(dt)==false)
			{
				ammoflags[i]=false;
			}
		}
	}

	for (i=0;i<maxasteroidcount;i++)
	{
		if (asteroidflags[i])
		{
			rocks[i]->update(dt*2);
		}
	}

	for (i=0;i<maxammocount;i++)
	{
		if (ammoflags[i]==false)
			continue;
		for (j=0;j<maxasteroidcount;j++)
		{
			if (asteroidflags[j]==false)
				continue;

			//molemmat aktiivisia
			float dist = (rocks[j]->position - ammo[i].position).length();
			if (dist < rocks[j]->size*1.6f)
			{
				ammoflags[i]=false;
				split(j);
				addexplosion(rocks[j]->position, 1000, 0.08f);
				continue;
			}
		}
	}

	processexplosions(dt*5);

	float fogcol[] = {0, 0, 0, 1};

	glFogi(GL_FOG_MODE, GL_LINEAR); //EXP, EXP2, LINEAR
	glFogfv(GL_FOG_COLOR, fogcol);
	glFogf(GL_FOG_DENSITY, 0.4f+0.4f*(float)cos(pos*10));
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 50);
	glFogf(GL_FOG_END, 70);
	myEnable(MY_FOG);

	
	p1.update(ship->getValue(pos).x, dt);
	glLineWidth(3);
	glColor4f(1.0f, 0.2f, 0.2f, alpha*0.8f);
	p1.draw();
	
	for (i=0;i<maxexplosioncount;i++)
	{
		if (explosionflags[i])
		{
			explosions[i]->draw(alpha);
		}
	}

	for (i=0;i<maxammocount;i++)
	{
		if (ammoflags[i])
		{
			ammo[i].draw(alpha);
		}
	}

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1);
	glColor4f(1.2f, 1.2f, 1.2f, alpha*0.3f);

	for (i=0;i<maxasteroidcount;i++)
	{
		if (asteroidflags[i])
		{
			rocks[i]->draw();
		}
	}
	myDisable(MY_FOG);

}

void Asteroids::split(int index)
{
	int i;

	if (asteroidflags[index]==false)
		return;

	Vector paikka = rocks[index]->position;
	float koko = rocks[index]->size;

	if (koko < 0.4f)
	{
		//pieni ja mitätön, tuhotaan koko paska
		delete rocks[index];
		asteroidflags[index] = false;
		return;
	}
	//splitataan pienemmiksi

	const float speedmod = 2.0f;
	delete rocks[index];
	rocks[index] = new Asteroid(8, 8, koko*0.7f*(0.8f+0.3f*Mathematics::randFloat()), speedmod, paikka);

	//etsitään paikka uudelle
	for (i=0;i<maxasteroidcount;i++)
	{
		if (asteroidflags[i]==false)
		{
			asteroidflags[i]=true;
			rocks[i] = new Asteroid(8, 8, koko*0.7f*(0.8f+0.3f*Mathematics::randFloat()), speedmod, paikka);
			return;
		}
	}
	//ei mahtunut, voivoi
}

void Asteroids::addammo(Vector &paikka, Vector &suunta, float energy)
{
	int i;
	for (i=0;i<maxammocount;i++)
	{
		if (ammoflags[i]==false)
		{
			ammoflags[i] = true;
			ammo[i].position = paikka;
			ammo[i].direction = suunta;
			ammo[i].energyleft = energy;
			return;
		}
	}
}

void Asteroids::addexplosion(Vector &paikka, float strength, float length)
{
	int i;
	for (i=0;i<maxexplosioncount;i++)
	{
		if (explosionflags[i]==false)
		{
			explosionflags[i] = true;
			explosions[i] = new Explosion(paikka, 250+rand()%100, length, strength);
			return;
		}
	}
}
void Asteroids::processexplosions(float dt)
{
	int i;
	for (i=0;i<maxexplosioncount;i++)
	{
		if (explosionflags[i])
		{
			if (explosions[i]->update(dt)==false)
			{
				explosionflags[i] = false;
				delete explosions[i];
			}
		}
	}
}

Asteroids::Asteroids()
{	
	int i;
	maxasteroidcount = 2000;
	const int asteroiditalussa = 200;
	rocks = new Asteroid*[maxasteroidcount];
	asteroidflags = new bool[maxasteroidcount];

	maxammocount = 1000;
	ammo = new Ammo[maxammocount];
	ammoflags = new bool[maxammocount];

	maxexplosioncount = 100;
	explosions = new Explosion*[maxexplosioncount];
	explosionflags = new bool[maxexplosioncount];

	for (i=0;i<maxammocount;i++)
	{
		ammoflags[i] = false;
	}
	for (i=0;i<maxasteroidcount;i++)
	{
		asteroidflags[i]=false;
	}
	for (i=0;i<maxexplosioncount;i++)
	{
		explosionflags[i] = false;
	}

	srand(160891);
	for (i=0;i<asteroiditalussa;i++)
	{
		const float radius = 4+156*Mathematics::randFloat();
		const float kulma = Mathematics::randFloat()*2*3.141592f;
		Vector paikka = Vector((float)sin(kulma), 0, (float)cos(kulma))*radius;
		switch(i%2)
		{
			case 0: rocks[i] = new Asteroid(8, 8, 2.0f, 1, paikka); break;
			case 1: rocks[i] = new Asteroid(8, 8, 1.2f, 1, paikka); break;
		}
		asteroidflags[i] = true;
	}

	p1.position = Vector(0, 0, 0);
	p1.angle = 0;
	p1.speed = 150;

	cam = new Spline();
	target = new Spline();
	rot = new Spline();

	cam->addpoint(Vector(35.1f, 30, 0.1f));
	cam->addpoint(Vector(25.1f, 15, 0.8f));
	cam->addpoint(Vector(15.1f, 8, 5.4f));
	cam->addpoint(Vector(11.1f, 5, 11.4f));
	cam->addpoint(Vector(8.1f, 14, 13.4f));
	cam->addpoint(Vector(6.1f, 19, 24.4f));
	cam->addpoint(Vector(-2.1f, 22, 32.4f));
	cam->addpoint(Vector(-5.1f, 4, 12.4f));
	cam->addpoint(Vector(-11.1f, -3, 2.4f));
	cam->addpoint(Vector(-17.1f, 6, -12.4f));
	cam->addpoint(Vector(-24.1f, 5, -17.4f));
	cam->addpoint(Vector(-17.1f, 4, -14.4f));
	cam->addpoint(Vector(-9.1f, 9, -11.4f));

	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));
	target->addpoint(Vector(0, 0, 0));

	rot->addpoint(Vector(0, 1, 0));
	rot->addpoint(Vector(1, 1, 0));
	rot->addpoint(Vector(0.2f, 1, 0.4f));
	rot->addpoint(Vector(0, 1, -0.2f));
	rot->addpoint(Vector(-0.1f, 1, 0));
	rot->addpoint(Vector(0.2f, 1, 0));
	rot->addpoint(Vector(0, 1, 0));
	rot->addpoint(Vector(0, 1, 1));

	ship = new Spline();

	ship->addpoint(Vector(0, 0, 0));
	ship->addpoint(Vector(3, 0, 0));
	ship->addpoint(Vector(7, 0, 0));
	ship->addpoint(Vector(1, 0, 0));
	ship->addpoint(Vector(-5, 0, 0));
	ship->addpoint(Vector(-8, 0, 0));
	ship->addpoint(Vector(1, 0, 0));
}

Asteroids::~Asteroids()
{
	int i;
	for (i=0;i<maxasteroidcount;i++)
	{
		if (asteroidflags[i])
			delete rocks[i];
	}
	delete [] rocks;
	for (i=0;i<maxexplosioncount;i++)
	{
		if (explosionflags[i])
			delete explosions[i];
	}
	delete [] explosions;
	delete [] asteroidflags;
	delete [] ammo;
	delete [] ammoflags;
}

bool Asteroids::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

