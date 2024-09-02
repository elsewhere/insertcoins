#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Blendjobb.hpp"

void Blendjobb::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.97f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos > fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
/*	Primitives::textWriter(30, 450, pos, 20, 0.2f, 0.4f, alpha, "Blendjobb");

	Primitives::textWriter(280, 280, pos, 20, 0.45f, 0.70f, alpha, "Any similarity");
	Primitives::textWriter(280, 300, pos, 20, 0.47f, 0.72f, alpha, "to real demos,");
	Primitives::textWriter(280, 320, pos, 20, 0.49f, 0.74f, alpha, "whether living");
	Primitives::textWriter(280, 340, pos, 20, 0.51f, 0.76f, alpha, "or dead, is purely");
	Primitives::textWriter(280, 360, pos, 20, 0.53f, 0.78f, alpha, "coincidental");
*/
}


void Blendjobb::renderScene(float pos, float alpha)
{
	unsigned int i;

	myEnable(MY_TEXTURE_2D);

	glLoadIdentity();
	glTranslatef(-0.5f, 0, -2.6f);

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myEnable(MY_BLEND);
	myDisable(MY_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("particle.jpg")->getID());
	glDepthMask(1);
	Vector x, y, z;
	Mathematics::antiRotate(&x, &y, &z);

	float *musa = dmsGetSpectrum();
	float summa = 0;
	for (i=0;i<512;i++)
		summa += musa[i]*0.2f;

	for (i=0;i<(unsigned int)valopallocount;i++)
	{
		pallot[i].calcPos(pos);

		glColor4f(1,1,1,alpha*(summa+0.4f)*(float)sin(pallot[i].currentpos*3.141592f));
		Primitives::billboard(pallot[i].pos, x, y, z, 0.1f+summa*0.005f);
	}

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	myEnable(MY_FOG);

	Vector valo(1, 1, 0);
	Matrix rotate, rotate2;

	rotate.makeRotation(pos*3.141592f*1.5f+1.0f, pos*4, 0);
	rotate2.makeRotation(-pos*1, pos*2, 2.4153f*(float)sin(pos*16));
	valo *= rotate2;
	valo.normalize();
	T3DFace *faceptr = kappale->getFaceArray();
	T3DVertex *vertptr = kappale->getVertexArray();

	Texture *text1 = dmsGetTexture("particle.jpg");
	Texture *text2 = dmsGetTexture("visual11.pcx");//.jpg");

	for (i=0;i<kappale->getFaceCount();i++)
	{
/*		Vector v1 = (faceptr->a->getPosition())*rotate;
		Vector v2 = (faceptr->b->getPosition())*rotate;
		Vector v3 = (faceptr->c->getPosition())*rotate;
		Vector nor1 = (faceptr->a->getNormal())*rotate;
		Vector nor2 = (faceptr->b->getNormal())*rotate;
		Vector nor3 = (faceptr->c->getNormal())*rotate;
*/
		Vector v1 = vertptr[faceptr->a].position*rotate;
		Vector v2 = vertptr[faceptr->b].position*rotate;
		Vector v3 = vertptr[faceptr->c].position*rotate;

		Vector nor1 = vertptr[faceptr->a].normal*rotate;
		Vector nor2 = vertptr[faceptr->b].normal*rotate;
		Vector nor3 = vertptr[faceptr->c].normal*rotate;

		float tu1 = vertptr[faceptr->a].u;
		float tv1 = vertptr[faceptr->a].v;
		float tu2 = vertptr[faceptr->b].u;
		float tv2 = vertptr[faceptr->b].v;
		float tu3 = vertptr[faceptr->c].u;
		float tv3 = vertptr[faceptr->c].v;

		float color;
		const float maxlight = 2.5f;//0.5f;
		const float ambient = 0.5f;
 		myEnable(MY_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, text1->getID());
		glDepthMask(GL_FALSE);
		glBegin(GL_TRIANGLES);
			color = (ambient + nor1.dotProduct(valo)*maxlight)*alpha;
			glColor3f(color, color, color);
			glTexCoord2f(tu1, tv1);
			glVertex3f(v1.x, v1.y, v1.z);
			color = (ambient + nor2.dotProduct(valo)*maxlight)*alpha;
			glColor3f(color, color, color);
			glTexCoord2f(tu2, tv2);
			glVertex3f(v2.x, v2.y, v2.z);
			color = (ambient + nor3.dotProduct(valo)*maxlight)*alpha;
			glColor3f(color, color, color);
			glTexCoord2f(tu3, tv3);
			glVertex3f(v3.x, v3.y, v3.z);
		glEnd();
		glDepthMask(GL_TRUE);
		myEnable(MY_BLEND);

		glColor4f(alpha, alpha, alpha, 0.5f);
		glBindTexture(GL_TEXTURE_2D, text2->getID());
		glBegin(GL_TRIANGLES);
			glTexCoord2f(nor1.x, nor1.y);
			glVertex3f(v1.x, v1.y, v1.z);
			glTexCoord2f(nor2.x, nor2.y);
			glVertex3f(v2.x, v2.y, v2.z);
			glTexCoord2f(nor3.x, nor3.y);
			glVertex3f(v3.x, v3.y, v3.z);
		glEnd();

		myEnable(MY_DEPTH_TEST);
		myDisable(MY_BLEND);
		faceptr++;
	}
	myDisable(MY_FOG);
}

Blendjobb::Blendjobb()
{
	int i;
	kappale = dmsGetObject("flake.t3d");

	int count = kappale->getVertexCount();
	T3DVertex *vertices = kappale->getVertexArray();

	float maxdist = 0;
	for (i=0;i<count;i++)
	{
		float dist = vertices[i].position.length();
		if (dist > maxdist)
			maxdist = dist;
	}

	for (i=0;i<count;i++)
	{
		vertices[i].position *= 1.0f / maxdist;
	}

/*	float longest=0.0f;
	Vertex *vert = kappale.getVertexPointer();
	for (unsigned  int i=0;i<kappale.getVertices();i++)
	{
		Vector position = vert->getPosition();
		const float dist = (position.x*position.x+position.y*position.y+position.z*position.z);
		if (dist > longest)
			longest = dist;
		vert++;
	}
	longest = (float)sqrt(longest);
	vert = kappale.getVertexPointer();
	for (i=0;i<kappale.getVertices();i++)
	{
		//reduce to unit
		vert->position = vert->position * (1.0f / longest);
		vert++;
	}*/


	valopallocount = 2000;

	pallot = new Valopallo[valopallocount];

	for (i=0;i<valopallocount;i++)
	{
		const float xsize = 4
			;
		const float yrange = 3;
		const float zrange = 4;

		float y = (rand()%10000)/10000.0f - 0.5f;
		float z = (rand()%10000)/10000.0f - 1.5f;

		Vector v1 = Vector(-xsize, y*yrange, z*zrange);
		Vector v2 = Vector( xsize, y*yrange, z*zrange);

		pallot[i].startpos = v1;
		pallot[i].endpos = v2;
		pallot[i].size = 0.1f;
		pallot[i].phase = (rand()%10000)/10000.0f;
		pallot[i].speed = 1 + 1.5f*(rand()%10000)/10000.0f;
	}
}

Blendjobb::~Blendjobb()
{
	delete [] pallot;
}

bool Blendjobb::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

