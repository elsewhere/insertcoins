#ifndef _PONG_HPP_
#define _PONG_HPP_

#include <stdio.h>
#include "../externs.hpp"
#include "../path2.hpp"

class PongGame
{
public:

	PongGame(Vector &pos);

	Vector bat1;
	Vector bat2;
	Vector ball;
	Vector dir;
	Vector gamepos;

	Vector rotation;

	void render(double dt, float aika, float alpha);
	void drawBat(Vector &pos, float aika, float alpha);
	void drawBall(Vector &pos, float aika, float alpha);
	void moveStuff(float dt);

	float batspeed;
	float ballspeed;



};
class Pong : public Base
{
	public:

		Pong();
		~Pong();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		void renderScene(float pos, float alpha);
		int gamecount;
		PongGame **games;
		float previousframe;

		Spline *kamera;
		Spline *target;

/*		Vector bat1;
		Vector bat2;
		Vector ball;

		Vector dir;

		void drawBat(Vector &pos, float alpha);
		void drawBall(Vector &pos, float aika, float alpha);
		void moveStuff(double dt);
*/

};

#endif
