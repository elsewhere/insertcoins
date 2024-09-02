#ifndef _RIPPLE_HPP_
#define _RIPPLE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

class GridPoint
{
	public:

		float u, v;
};

//-------------------------------------------------------
//	Ripple-luokka
//-------------------------------------------------------

class Ripple : public Base
{
	friend class Base;

	public:

		Ripple();
		~Ripple();
		
		bool init(unsigned long s, unsigned long e);
		void draw();		

	private:
		
		Texture *texture;
		Texture *texture2;
		GridPoint *grid;
};

#endif