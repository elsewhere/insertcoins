#include <math.h>
#include "writer.hpp"

void FontRect::getUV(int charnum, float *u1, float *v1, float *u2, float *v2)
{
	//vaaka
	int pysty = (int)floor(charnum/8.0f);
	int vaaka = charnum - pysty*8;

	const float step = 1.0f/256.0f;

	*v1 = (pysty*8)*step;
	*v2 = (pysty*8+15)*step;

	*u1 = (vaaka*8)*step;
	*u2 = (vaaka*8+15)*step;
}

void Writer::write2D(float x, float y, float size, char *string, Texture *font)
{
	int pituus = strlen(string);
	if (pituus <= 0)
		return;

	glPushMatrix();
	dmsPerspective2D(1,1);
	myEnable(MY_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, font->getID());


	glBegin(GL_QUADS);
	for (int i=0;i<pituus;i++)
	{
		int merkki = toupper(string[i]);

		merkki = 34;
		float u1, u2, v1, v2;
		FontRect::getUV(merkki, &u1, &v1, &u2, &v2);

		const float step = size;
		Vector pos1 = Vector(x+i*size, y, 0);
		Vector pos2 = Vector(x+i*size+step, y, 0);
		Vector pos3 = Vector(x+i*size+step, y+step, 0);
		Vector pos4 = Vector(x+i*size, y+step, 0);

		glTexCoord2f(u1, v1);
		glVertex2fv((float *)&pos1);
		glTexCoord2f(u2, v1);
		glVertex2fv((float *)&pos2);
		glTexCoord2f(u2, v2);
		glVertex2fv((float *)&pos3);
		glTexCoord2f(u1, v2);
		glVertex2fv((float *)&pos4);
	}
	glEnd();
	dmsPerspective3D();
	glPopMatrix();


}