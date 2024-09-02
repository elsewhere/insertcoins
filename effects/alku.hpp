#ifndef _ALKU_HPP_
#define _ALKU_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Alku : public Base
{
	public:

		Alku();
		~Alku();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		void renderScene(float pos, float alpha);

};

#endif
