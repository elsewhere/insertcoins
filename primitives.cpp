#include "primitives.hpp"

void Primitives::player(Vector &position, Vector &rotation, float scale)
{
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
//	glRotatef(angle*360/(2*3.141592f), 0, 1, 0);
	glRotatef(360*rotation.x, 1, 0, 0);
	glRotatef(360*rotation.y, 0, 1, 0);
	glRotatef(360*rotation.z, 0, 0, 1);

	Vector v1 = Vector(0, 0, 0.08f)*scale;
	Vector v2 = Vector(0.05f, 0, -0.08f)*scale;
	Vector v3 = Vector(-0.05f, 0, -0.08f)*scale;

	myBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_TRIANGLES);
	glVertex3fv((float *)&v1);
	glVertex3fv((float *)&v2);
	glVertex3fv((float *)&v3);
	glEnd();

	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBegin(GL_LINE_LOOP);
	glVertex3fv((float *)&v1);
	glVertex3fv((float *)&v2);
	glVertex3fv((float *)&v3);
	glEnd();
	glPopMatrix();

}
void Primitives::fadeText(float x, float y, float scale, float alpha, float pos, float starttime, float endtime, float ramp,
				 		  char *fontti, char *teksti)
{
	if (pos < starttime || pos > endtime)
		return;

	float fadekohta = (pos-starttime) / (endtime-starttime);
	fadekohta = (float)sin(fadekohta*3.141592f); //ylös ja alas
	fadekohta = (float)pow(fadekohta, ramp);

	dmsWrite2D(x, y, alpha*fadekohta, scale, fontti, teksti);

//void dmsWrite2D(float x, float y, float a, float scale, char *fontName, char *msg);

}

void Primitives::textWriterLayers(float xpos, float ypos, float pos, float spread, float textsize, float starttime, float endtime, float alpha, char *teksti)
{
	const float drift = spread;

 	if (pos > starttime && pos < endtime)
	{
		const float textpos = (pos-starttime) / (endtime-starttime);
		float textalpha = (float)sin(textpos*3.141592f)*4.0f*alpha;
		if (textalpha > 1.0f) textalpha = 1.0f;

		dmsWrite2D(xpos, ypos, (textalpha*0.9f), textsize+textpos*drift,"luontofontti.pcx", teksti);
		dmsWrite2D(xpos, ypos, (textalpha*0.4f), (textsize+textpos*drift)*1.2f, "luontofontti.pcx",teksti);
		dmsWrite2D(xpos, ypos, (textalpha*0.2f), (textsize+textpos*drift)*1.5f, "luontofontti.pcx",teksti);
		dmsWrite2D(xpos, ypos, (textalpha*0.1f), (textsize+textpos*drift)*2.3f, "luontofontti.pcx",teksti);
	}
}



void Primitives::flatCube(float size)
{
	Vector upleftback = Vector( -size, -size, -size);
	Vector upleftfront = Vector(-size, -size, +size);
	Vector uprightback = Vector( size, -size, -size);
	Vector uprightfront = Vector(size, -size, +size);

	Vector downleftback = Vector( -size, size, -size);
	Vector downleftfront = Vector(-size, size, +size);
	Vector downrightback = Vector( size, size, -size);
	Vector downrightfront = Vector(size, size, +size);

	glBegin(GL_QUADS);

	//yläsivu
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);

	//alasivu
	glVertex3fv((float *)&downleftback);
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);

	//etusivu
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);

	//takasivu
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);

	//vasen sivu
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);

	//oikea sivu
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);

	glEnd();
}
void Primitives::texturedCube(float size, float tmul)
{
	Vector upleftback = Vector( -size, -size, -size);
	Vector upleftfront = Vector(-size, -size, +size);
	Vector uprightback = Vector( size, -size, -size);
	Vector uprightfront = Vector(size, -size, +size);

	Vector downleftback = Vector( -size, size, -size);
	Vector downleftfront = Vector(-size, size, +size);
	Vector downrightback = Vector( size, size, -size);
	Vector downrightfront = Vector(size, size, +size);

	glBegin(GL_QUADS);

	//yläsivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&upleftback);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&uprightback);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&uprightfront);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&upleftfront);

	//alasivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&downleftback);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&downrightback);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&downrightfront);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&downleftfront);

	//etusivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&upleftfront);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&uprightfront);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&downrightfront);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&downleftfront);

	//takasivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&upleftback);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&uprightback);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&downrightback);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&downleftback);

	//vasen sivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&upleftback);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&upleftfront);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&downleftfront);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&downleftback);

	//oikea sivu
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&uprightback);
	glTexCoord2f(tmul, 0);
	glVertex3fv((float *)&uprightfront);
	glTexCoord2f(tmul, tmul);
	glVertex3fv((float *)&downrightfront);
	glTexCoord2f(0, tmul);
	glVertex3fv((float *)&downrightback);

	glEnd();

}
void Primitives::renderProperGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, int slices)
{
	//p1----p2
	//|     |
	//|     |
	//|     |
	//p4----p3

	int x, y;
	Vector *grid = new Vector[slices*slices];

	Vector dy1 = (p4-p1)*(1.0f/slices);
	Vector dy2 = (p3-p2)*(1.0f/slices);

	Vector u1 = p1;
	Vector u2 = p2;

	for (y=0;y<slices;y++)
	{
		Vector dx = (u2-u1)*(1.0f/slices);
		Vector v = u1;
		for (x=0;x<slices;x++)
		{
			grid[x+y*slices] = v;
			v += dx;
		}
		u1 += dy1;
		u2 += dy2;
	}

	//gridi tehty

	glBegin(GL_LINES);
	for (y=0;y<slices-1;y++)
	{
		for (x=0;x<slices-1;x++)
		{
			Vector v1 = grid[x+y*slices];
			Vector v2 = grid[x+1+y*slices];
			Vector v3 = grid[x+(y+1)*slices];
			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v2);
			glVertex3fv((float *)&v1);
			glVertex3fv((float *)&v3);
		}
	}
	glEnd();

	delete [] grid;

}
void Primitives::renderGrid(Vector &p1, Vector &p2, Vector &p3, Vector &p4, int slices)
{
	//p1----p2
	//|     |
	//|     |
	//|     |
	//p4----p3
	int i;

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

void Primitives::wireCube(float size)
{
	if (size <= 0) return;
	Vector upleftback = Vector( -size, -size, -size);
	Vector upleftfront = Vector(-size, -size, +size);
	Vector uprightback = Vector( size, -size, -size);
	Vector uprightfront = Vector(size, -size, +size);

	Vector downleftback = Vector( -size, size, -size);
	Vector downleftfront = Vector(-size, size, +size);
	Vector downrightback = Vector( size, size, -size);
	Vector downrightfront = Vector(size, size, +size);
	
	glBegin(GL_LINES);

	//yläsivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//yläsivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//yläsivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//yläsivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);

	//alasivu vasen
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//alasivu oikea
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//alasivu etu
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//alasivu taka
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);

	//oikea sivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//oikea sivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//oikea sivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//oikea sivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);

	//vasen sivu ylä
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//vasen sivu ala
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//vasen sivu etu
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);
	//vasen sivu taka
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	//etusivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//etusivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//etusivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//etusivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);

	//takasivu ylä
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);
	//takasivu ala
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);
	//takasivu oikea
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);
	//takasivu vasen
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	glEnd();
}
void Primitives::wireBox(Vector &p1, Vector &p2)
{

	float xsize = p2.x - p1.x;
	float zsize = p2.z - p1.z;

//	if (size <= 0) return;

	//p1 = vasen takayläkulma
	//p2 = oikea etualakulma
	
	Vector upleftback = p1;
	Vector uprightback = p1 + Vector(xsize, 0, 0);
	Vector upleftfront = p1 + Vector(0, 0, zsize);
	Vector uprightfront = p1 + Vector(xsize, 0, zsize);

	Vector downrightfront = p2;
	Vector downleftfront = p2 - Vector(xsize, 0, 0);
	Vector downleftback = p2 - Vector(xsize, 0, zsize);
	Vector downrightback = p2 - Vector(0, 0, zsize);

/*
	Vector upleftback = Vector( -size, -size, -size);
	Vector upleftfront = Vector(-size, -size, +size);
	Vector uprightback = Vector( size, -size, -size);
	Vector uprightfront = Vector(size, -size, +size);

	Vector downleftback = Vector( -size, size, -size);
	Vector downleftfront = Vector(-size, size, +size);
	Vector downrightback = Vector( size, size, -size);
	Vector downrightfront = Vector(size, size, +size);
*/
	glBegin(GL_LINES);

	//yläsivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//yläsivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//yläsivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//yläsivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);

	//alasivu vasen
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//alasivu oikea
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//alasivu etu
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//alasivu taka
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);

	//oikea sivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&uprightback);
	//oikea sivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downrightback);
	//oikea sivu etu
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//oikea sivu taka
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);

	//vasen sivu ylä
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&upleftback);
	//vasen sivu ala
	glVertex3fv((float *)&downleftfront);
	glVertex3fv((float *)&downleftback);
	//vasen sivu etu
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);
	//vasen sivu taka
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	//etusivu ylä
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&upleftfront);
	//etusivu ala
	glVertex3fv((float *)&downrightfront);
	glVertex3fv((float *)&downleftfront);
	//etusivu oikea
	glVertex3fv((float *)&uprightfront);
	glVertex3fv((float *)&downrightfront);
	//etusivu vasen
	glVertex3fv((float *)&upleftfront);
	glVertex3fv((float *)&downleftfront);

	//takasivu ylä
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&upleftback);
	//takasivu ala
	glVertex3fv((float *)&downrightback);
	glVertex3fv((float *)&downleftback);
	//takasivu oikea
	glVertex3fv((float *)&uprightback);
	glVertex3fv((float *)&downrightback);
	//takasivu vasen
	glVertex3fv((float *)&upleftback);
	glVertex3fv((float *)&downleftback);

	glEnd();
}
void Primitives::billboard(Vector &kohta, Vector &x, Vector &y, Vector &z, float scale)
{
	glBegin(GL_QUADS);
	Vector v1 = kohta - x*scale - y*scale;
	Vector v2 = kohta + x*scale - y*scale;
	Vector v3 = kohta + x*scale + y*scale;
	Vector v4 = kohta - x*scale + y*scale;

	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v2);
	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v4);
	glEnd();
}

void Primitives::drawFlare(Vector *pos, float size)
{
	myEnable(MY_BLEND);
	myBlendFunc(GL_SRC_ALPHA, GL_ONE);
	myDisable(MY_DEPTH_TEST);
//	glDepthMask(GL_FALSE);

	Vector x, y, z;
	Mathematics::antiRotate(&x, &y, &z);

	Vector v1 = *pos - x*size - y*size;
	Vector v2 = *pos + x*size - y*size;
	Vector v3 = *pos + x*size + y*size;
	Vector v4 = *pos - x*size + y*size;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3fv((float *)&v1);
	glTexCoord2f(1, 0);
	glVertex3fv((float *)&v2);
	glTexCoord2f(1, 1);
	glVertex3fv((float *)&v3);
	glTexCoord2f(0, 1);
	glVertex3fv((float *)&v4);
	glEnd();

/*	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(pos->x-size, pos->y-size, pos->z);
	glTexCoord2f(1, 0);
	glVertex3f(pos->x+size, pos->y-size, pos->z);
	glTexCoord2f(1, 1);
	glVertex3f(pos->x+size, pos->y+size, pos->z);
	glTexCoord2f(0, 1);
	glVertex3f(pos->x-size, pos->y+size, pos->z);
	glEnd();

	myDisable(MY_BLEND);
	myEnable(MY_DEPTH_TEST);*/
//	glDepthMask(GL_TRUE);
}

void Primitives::circle(Vector &pos, float radius, float lines, float r, float g, float b, float alpha)
{
	int i;
	glBegin(GL_LINE_STRIP);
	glColor4f(r, g, b, alpha);

	for (i=0;i<lines+1;i++)
	{
		const float step = 2*3.141592f / (float)lines;
		glVertex3f(pos.x + radius*(float)cos(i*step), pos.y+radius*(float)sin(i*step), pos.z);
	}
	glEnd();
}
