#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "Layers2.hpp"
#include "../primitives.hpp"

void Layers2::draw()
{
	const float pos = (time - startTime) / (endTime - startTime);
	float alpha = 1.0f;

	const float fadeinstart = 0.0f;
	const float fadeinstop = 0.1f;
	const float fadeoutstart = 0.97f;
	const float fadeoutstop = 1.0f;

	if (pos >= fadeinstart && pos <= fadeinstop)
		alpha *= (pos-fadeinstart) / (fadeinstop-fadeinstart);
	if (pos >= fadeoutstart && pos <= fadeoutstop)
		alpha *= 1-(pos-fadeoutstart) / (fadeoutstop-fadeoutstart);

	renderScene(pos, alpha);
}


void Layers2::renderScene(float pos, float alpha)
{
	int i;
	glLoadIdentity();

	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, dmsGetTexture("visual11.pcx")->getID());
	glColor3f(alpha*0.4f, alpha*0.4f, alpha*0.4f);
	glRotatef(pos*32, 1.0f, 0.7f, -0.5f);
	Primitives::texturedCube(1, 2);
	glDepthMask(GL_TRUE);

	glLoadIdentity();
	glTranslatef(-0.5f, -0.2f, -2.7f);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Matrix rotate;
//	rotate.makeRotation(pos*5, pos*2.3f, -pos*2.1f);
	rotate.makeRotation(pos*1.3f, pos*5.3f, -pos*1.4f);

	Vector valo(1,1,0.5f);
	Matrix valorotate;
	valorotate.makeRotation(pos*15, -pos*13, pos*7);
	valo *= valorotate;

	T3DFace *faceptr = kappale->getFaceArray();
	T3DVertex *vertptr = kappale->getVertexArray();

	Texture *text1 = dmsGetTexture("visual6.pcx");
	Texture *text2 = dmsGetTexture("visual2.pcx");
	Texture *text3 = dmsGetTexture("particle.jpg");

	for (i=0;i<(int)kappale->getFaceCount();i++)
	{
		//perusobjekti
		const float maxalpha = 0.7f*alpha;
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("visual2")->getID()); //visual2
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("leaves")->getID()); //visual2
		glBindTexture(GL_TEXTURE_2D, text1->getID());

		Vector v1 = vertptr[faceptr->a].position*rotate;
		Vector v2 = vertptr[faceptr->b].position*rotate;
		Vector v3 = vertptr[faceptr->c].position*rotate;
		Vector n1 = vertptr[faceptr->a].normal*rotate;
		Vector n2 = vertptr[faceptr->b].normal*rotate;
		Vector n3 = vertptr[faceptr->c].normal*rotate;

		float tu1 = vertptr[faceptr->a].u;
		float tv1 = vertptr[faceptr->a].v;
		float tu2 = vertptr[faceptr->b].u;
		float tv2 = vertptr[faceptr->b].v;
		float tu3 = vertptr[faceptr->c].u;
		float tv3 = vertptr[faceptr->c].v;

		float l1 = (faceptr->normal*rotate).dotProduct(valo);
		l1 *= l1; l1 *= l1;
		glBegin(GL_TRIANGLES);
		glColor3f(maxalpha*l1, maxalpha*l1, maxalpha*l1);
		glTexCoord2f(tu1, tv1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(tu2, tv2);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(tu3, tv3);
		glVertex3f(v3.x, v3.y, v3.z);
		glEnd();

		//multitexture envymap
		myEnable(MY_BLEND);
		myDisable(MY_DEPTH_TEST);

		glColor4f(1, 1, 1, alpha*0.2f);
		glBindTexture(GL_TEXTURE_2D, text2->getID());
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("envmap")->getID()); //visual2
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("leaves3")->getID()); //visual2
		glBegin(GL_TRIANGLES);
		glTexCoord2f(n1.x, n1.y);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(n2.x, n2.y);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(n3.x, n3.y);
		glVertex3f(v3.x, v3.y, v3.z);

		glEnd();
		myEnable(MY_DEPTH_TEST);
		
		glDepthMask(GL_FALSE);

		glBindTexture(GL_TEXTURE_2D, text3->getID());
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("leaves3")->getID()); //visual11
//		glBindTexture(GL_TEXTURE_2D, tManager->getTextureName("visual11")->getID()); //visual11
		const float phase = (float)i/(float)kappale->getFaceCount();
		const float size = 1.2f + 0.1f*(float)sin(pos*16);//+phase*6*3.141592f);
		v1 *= size; v2 *= size; v3 *= size;

		glBegin(GL_TRIANGLES);
		glColor4f(1,1,1,0.83f*alpha);
		glTexCoord2f(tu1, tv1);
		glVertex3f(v1.x, v1.y, v1.z);
		glTexCoord2f(tu2, tv2);
		glVertex3f(v2.x, v2.y, v2.z);
		glTexCoord2f(tu3, tv3);
		glVertex3f(v3.x, v3.y, v3.z);
		glEnd();
		glDepthMask(GL_TRUE);
		myDisable(MY_BLEND);
		faceptr++;
	}
	myDisable(MY_TEXTURE_2D);
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	myEnable(MY_LINE_SMOOTH);
	glDepthMask(GL_FALSE);

	Matrix reunusrotate;
	reunusrotate.makeRotation(pos*1.2f, -pos*0.9f, pos*0.07f);

	T3DVertex *vertexptr = vertptr;//reunus.getVertexPointer();
	glColor4f(1, 1, 1, 0.077f*alpha);
	glBegin(GL_LINE_STRIP);
	for (i=0;i<(int)kappale->getVertexCount();i++)
	{
		const float scale = 2.32f;
		Vector v = (vertexptr->position*reunusrotate)*scale;
		glColor4f(1, 1, 1, 0.047f*alpha);
		glVertex3fv((float *)&v);//v.x, v.y, v.z);
		vertexptr++;
	}
	glEnd();
	glDepthMask(GL_TRUE);

	myDisable(MY_BLEND);
	myEnable(MY_TEXTURE_2D);
}

Layers2::Layers2()
{
	int i;
	kappale = dmsGetObject("hedra.t3d");
	int count = kappale->getVertexCount();

	float maxdist = 0;
	T3DVertex *vertices = kappale->getVertexArray();
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

//	kappale.reduceToUnit();
//	reunus.reduceToUnit();
}

Layers2::~Layers2()
{
}

bool Layers2::init(unsigned long s, unsigned long e)
{
	startTime = s;
	endTime = e;
	return true;
}

