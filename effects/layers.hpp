#ifndef _LAYERS_HPP_
#define _LAYERS_HPP_

#include <stdio.h>
#include "../externs.hpp"

class Layers : public Base
{
	public:

		Layers();
		~Layers();

		bool init(unsigned long s, unsigned long e);
		void draw();

	private:
		T3D *kappale;
//		Object reunus;

		void renderScene(float pos, float alpha);
};


#endif