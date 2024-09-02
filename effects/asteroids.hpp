#ifndef _ASTEROIDS_HPP_
#define _ASTEROIDS_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class Asteroid
{
public:

	Asteroid(int xres, int yres, float koko, float speedmodifier, Vector &pos);
	~Asteroid();

	void draw();
	void update(float dt);

	int xdim, ydim;
	float size;
	Vector *vertices;
	Vector position;
	Vector direction;
	Vector rotation;
	Vector rotspeed;
};

class Player
{
public:
	Vector position;
	float speed;
	float angle;

	void update(float kulma, float dt);
	void draw();
};

class Ammo
{
public:

	Vector position;
	Vector direction;
	float energyleft;

	bool update(float dt);
	void draw(float alpha);
};

class Explosion
{
public:
	Explosion(Vector &paikka, int count, float length, float strength);
	~Explosion();
	void draw(float alpha);
	bool update(float dt);

private:
	float time;
	float alphascale;
	int particlecount;
	Vector position;
	Vector *particles;
	Vector *directions;




};

class Asteroids : public Base
{
	public:

		Asteroids();
		~Asteroids();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void renderScene(float pos, float alpha);
		void addammo(Vector &paikka, Vector &suunta, float energy);
		void addexplosion(Vector &paikka, float strength, float length);
		void processexplosions(float dt);
		void split(int index);

		Player p1;
		int maxammocount;
		Ammo *ammo;
		bool *ammoflags;

		int maxasteroidcount;
		Asteroid **rocks;
		bool *asteroidflags;

		int maxexplosioncount;
		Explosion **explosions;
		bool *explosionflags;

		Spline *cam;
		Spline *target;
		Spline *rot;

		Spline *ship;

};

#endif
