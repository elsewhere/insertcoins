#ifndef _RUNKO_HPP_
#define _RUNKO_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Runko : public Base
{
	public:

		Runko();
		~Runko();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void renderScene(float pos, float alpha);

};

#endif
