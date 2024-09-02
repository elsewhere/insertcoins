//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#ifdef _DEBUG
	#include <stdlib.h>
	#include "../mmgr.h"
#endif

#include <math.h>

#include "3dripple.hpp"

#define GRIDWIDTH 30
#define GRIDHEIGHT 30

//-------------------------------------------------------
//	Constructor & destructor
//-------------------------------------------------------

Ripple3D::Ripple3D()
{	
	texture = NULL;
	grid = NULL;
	syke = 0.0f;
}

Ripple3D::~Ripple3D()
{
	if(grid)
	{
		delete [] grid;
		grid = NULL;
	}

	release();
}


//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Ripple3D::init(unsigned long s, unsigned long e)
{		
	active = false;
	startTime = s;
	endTime = e;

	grid = new GridPoint3D[(GRIDWIDTH+1)*(GRIDHEIGHT+1)];
	
	texture = dmsGetTexture("visual2.pcx");
	texture2 = dmsGetTexture("flare.pcx");
	if(!texture || !texture2 || !grid)
	{
		dmsMsg("Ripple3D::init error: Couldn't get 3D-objects or textures\n");
		return false;
	}

	memset(grid, 0, (GRIDWIDTH)*(GRIDHEIGHT)*sizeof(GridPoint3D));

	float fogColor[4]= { 0.0f, 0.0f, 0.0f, 1.0f };

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.85f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 350.0f);
	glFogf(GL_FOG_END, 450.0f);

	myEnable(MY_LINE_SMOOTH);
	
	return true;
}

void Ripple3D::draw()
{
	int x, y, width, height;	
	float xStep, yStep;
	float u, v, u1, v1, u2, v2, u3, v3;
	float h, h1, h2, h3;
	GridPoint3D *ptr = grid;
	Vector position;
	float *EQ = NULL;

	const float pos = (time - startTime) / (endTime - startTime);

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	width = 400;
	height = 400;
	xStep = (float)width / 20.0f;
	yStep = (float)height / 20.0f;

	for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2)+1; y++)
	{
		for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2)+1; x++)
		{			
			float len =  (float)sqrt(x*x+y*y);
			float uStep = (1.0f / 20.0f);
			float vStep = (1.0f / 20.0f);			

			ptr->u = float(x*uStep*cos(len*0.75f+(time*0.001f))+0.5f);
			ptr->v = float(y*vStep*cos(len*0.75f+(time*0.001f))+0.5f);
			ptr->height = float(cos(len*0.75f+(time*0.001f))+0.5f);
			
			*ptr++;
		}
	}

	position = Vector(200*(float)cos(time * 0.01f * 3.1415926f / 128.0f)-200*(float)sin(time * 0.01f * 3.1415926f / 128.0f) + 200, 
					  100, 
					  200*(float)sin(time * 0.01f * 3.1415926f / 128.0f)+200*(float)cos(time * 0.01f * 3.1415926f / 128.0f) + 200);

	glLoadIdentity();
	gluLookAt(position.x, position.y, position.z, 200, 0, 200, 0, 1, 0.11);

	glTranslatef(100, 0, 0);
		
	myEnable(MY_FOG);
	//drawBg(800, 50, 800);
	
	myDisable(MY_BLEND);
	myDisable(MY_CULL_FACE);	
	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
	myEnable(MY_TEXTURE_2D);
		
	glBindTexture(GL_TEXTURE_2D, texture->getID());
	
	glPushMatrix();

	glBegin(GL_QUADS);

	ptr = grid;
	for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2); y++)
	{
		for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2); x++)
		{	
			float xpos = x*xStep + (float)width/2.0f;
			float ypos = y*yStep + (float)height/2.0f;

			u = ptr->u;
			v = ptr->v;
			h = ptr->height;
			u1 = (ptr+1)->u;
			v1 = (ptr+1)->v;
			h1 = (ptr+1)->height;
			u2 = (ptr+1+GRIDWIDTH+1)->u;
			v2 = (ptr+1+GRIDWIDTH+1)->v;
			h2 = (ptr+1+GRIDWIDTH+1)->height;
			u3 = (ptr+GRIDWIDTH+1)->u;
			v3 = (ptr+GRIDWIDTH+1)->v;
			h3 = (ptr+GRIDWIDTH+1)->height;

			(h  < 0.25f) ? h  = 0.25f : h  = h;
			(h1 < 0.25f) ? h1 = 0.25f : h1 = h1;
			(h2 < 0.25f) ? h2 = 0.25f : h2 = h2;
			(h3 < 0.25f) ? h3 = 0.25f : h3 = h3;

			glColor4f(h , h , h , h ); glTexCoord2f(u , v ); glVertex3f(xpos		,  h*15, ypos		);			
			glColor4f(h1, h1, h1, h1); glTexCoord2f(u1, v1); glVertex3f(xpos+xStep	, h1*15, ypos		);		
			glColor4f(h2, h2, h2, h2); glTexCoord2f(u2, v2); glVertex3f(xpos+xStep	, h2*15, ypos+yStep	);
			glColor4f(h3, h3, h3, h3); glTexCoord2f(u3, v3); glVertex3f(xpos		, h3*15, ypos+yStep	);

			*ptr++;
		}
		*ptr++;
	}

	glEnd();

	glPopMatrix();

	glPushMatrix();
	
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_CULL_FACE);
	myDisable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);	
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(1.0f, 0.75f, 0.35f, 0.45f);
	glTranslatef(0.0f, 12.0f, 0.0f);

	glBegin(GL_LINES);

	ptr = grid;
	for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2); y++)
	{
		for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2); x++)
		{	
			float xpos = x*xStep + (float)width/2.0f;
			float ypos = y*yStep + (float)height/2.0f;
			
			h = ptr->height;
			h1 = (ptr+1)->height;
			h2 = (ptr+1+GRIDWIDTH+1)->height;
			h3 = (ptr+GRIDWIDTH+1)->height;

			glVertex3f(xpos		 ,  h*15, ypos		);
			glVertex3f(xpos+xStep, h1*15, ypos		);
			glVertex3f(xpos+xStep, h2*15, ypos+yStep);
			glVertex3f(xpos		 , h3*15, ypos+yStep);

			glVertex3f(xpos		 ,  h*15, ypos		);
			glVertex3f(xpos		 , h3*15, ypos+yStep);
			glVertex3f(xpos+xStep, h1*15, ypos		);
			glVertex3f(xpos+xStep, h2*15, ypos+yStep);			
			
			*ptr++;
		}
		*ptr++;
	}

	glEnd();	

	glPopMatrix();

	EQ = dmsGetSpectrum();
	if(EQ)
	{
		int offset = 0;
		float matrix[16];
		Vector xx, yy, zz;
		Vector p1, p2, p3, p4, pos;

		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	
		xx = (Vector(matrix[0], matrix[4], matrix[8]))*4.0f;
		yy = (Vector(matrix[1], matrix[5], matrix[9]))*4.0f;
		zz = Vector(matrix[2], matrix[6], matrix[10]);

		// EQ Balls ;D
		glPushMatrix();

		glTranslatef(0.0f, 12.0f, 0.0f);
		
		myDisable(MY_CULL_FACE);
		myDisable(MY_DEPTH_MASK);		
		myDisable(MY_DEPTH_TEST);
		myEnable(MY_TEXTURE_2D);
		myEnable(MY_BLEND);
		myBlendFunc(GL_SRC_ALPHA, GL_ONE);
		
		glBindTexture(GL_TEXTURE_2D, texture2->getID());				
		
		ptr = grid;
		for(y = -(GRIDHEIGHT/2); y < (GRIDHEIGHT/2); y++)
		{
			float val = EQ[offset%512]*5.0f;
			glColor4f(val, val, val, val);
			
			offset+=3;			

			for(x = -(GRIDWIDTH/2); x < (GRIDWIDTH/2); x++)
			{	
				float xpos = x*xStep + (float)width/2.0f; 
				float ypos = y*yStep + (float)height/2.0f;
			
				h = ptr->height;

				pos = Vector(xpos, h*15, ypos);
				p1 = pos + xx + yy;
				p2 = pos - xx + yy;
				p3 = pos + xx - yy;
				p4 = pos - xx - yy;				

				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(1, 1); glVertex3f(p1.x, p1.y, p1.z);
					glTexCoord2f(0, 1); glVertex3f(p2.x, p2.y, p2.z);
					glTexCoord2f(1, 0); glVertex3f(p3.x, p3.y, p3.z);
					glTexCoord2f(0, 0); glVertex3f(p4.x, p4.y, p4.z);				
				glEnd();

				*ptr++;
			}
			*ptr++;
		}				

		glPopMatrix();
	}

	myEnable(MY_DEPTH_MASK);
	myEnable(MY_DEPTH_TEST);
}

void Ripple3D::drawBg(float x, float y, float z)
{
	myDisable(MY_TEXTURE_2D);
	myDisable(MY_DEPTH_MASK);
	myDisable(MY_DEPTH_TEST);
	myDisable(MY_BLEND);	

	glPushMatrix();
		
		glBegin(GL_QUADS);

			glVertex3f( x, y,  z);
			glVertex3f(-x, y,  z);
			glVertex3f(-x, 0, -z);
			glVertex3f( x, 0, -z);

			glVertex3f( x, -y,  z);
			glVertex3f(-x, -y,  z);
			glVertex3f(-x,  0, -z);
			glVertex3f( x,  0, -z);

		glEnd();

	glPopMatrix();	
}


