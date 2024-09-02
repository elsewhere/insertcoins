#ifndef _LAYERS2_HPP_
#define _LAYERS2_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Layers2 : public Base
{
	public:

		Layers2();
		~Layers2();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		T3D *kappale;

		void renderScene(float pos, float alpha);
};


#endif