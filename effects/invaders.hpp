#ifndef _INVADERS_HPP_
#define _INVADERS_HPP_

#include <stdio.h>
#include "../externs.hpp"


class Invaders : public Base
{
	public:

		Invaders();
		~Invaders();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:

		void renderScene(float pos, float alpha);

};

#endif
