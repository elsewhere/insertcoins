#ifndef _BLENDJOBB_HPP_
#define _BLENDJOBB_HPP_

#include <stdio.h>

#include "../externs.hpp"
#include "../primitives.hpp"
#include "../valopallo.hpp"
#include "../mathematics.hpp"

class Blendjobb : public Base
{
	public:

		Blendjobb();
		~Blendjobb();

		bool init(unsigned long s, unsigned long e);
		void draw();
	private:
		int valopallocount;
		Valopallo *pallot;

		T3D *kappale;
		void renderScene(float pos, float alpha);
};

#endif