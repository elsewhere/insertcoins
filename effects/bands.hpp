#ifndef _BANDS_HPP_
#define _BANDS_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

//-------------------------------------------------------
//	Bands-luokka
//-------------------------------------------------------

class Band
{
	public:

		CubicBezierCurve curve;
		Vector start, p1, p2, end;

		float alpha;
		float speed;
		float strips;
		float phase;

		float r, g, b;

		float fadeInStart, fadeInEnd;
		float fadeOutStart, fadeOutEnd;
		float startTime, endTime, time;
};

//-------------------------------------------------------
//	Bands-luokka
//-------------------------------------------------------

class Bands : public Base
{
	friend class Base;

	public:

		Bands();
		~Bands();
		
		bool init(unsigned long s, unsigned long e);
		void draw();
		
	private:

		void renderScene();

		Band *nauha;
		Texture *flare;
		float lastTime, deltaTime;
};

#endif