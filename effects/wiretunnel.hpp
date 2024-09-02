#ifndef _Wiretunnel_HPP_
#define _Wiretunnel_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Tunnelpart
{
public:
	Vector v1, v2;
	Vector v3, v4;
	float phase;
};

class Invader
{
public:
	Vector pos;
	Vector pos2;
	Vector startrot;
	Vector endrot;

	float speed;
	float phase;

	float starttime;
	float endtime;
	float diestart;
	float dieend;

	int xres;
	int yres;
	int type;

	Vector *points;
	Vector *rotates;
	unsigned char *image;

	void draw(float pos, float size);

};

class Wiretunnel : public Base
{
	public:

		Wiretunnel();
		~Wiretunnel();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		int partcount;
		Tunnelpart *parts;

		int invadercount;
		Invader *invaders;

		int imagecount;
		unsigned char **images;

		int xres, yres;
		bool *grid;

		void drawInvader(Vector &kohta, float pos, float size, int type);
		
		void renderScene(float pos, float alpha);

};

#endif
