#ifndef _3DRIPPLE_HPP_
#define _3DRIPPLE_HPP_

//-------------------------------------------------------
//	Headers
//-------------------------------------------------------

#include "../externs.hpp"

class GridPoint3D
{
	public:

		float u, v, height;
};

//-------------------------------------------------------
//	3DRipple-luokka
//-------------------------------------------------------

class Ripple3D : public Base
{
	friend class Base;

	public:

		Ripple3D();
		~Ripple3D();
		
		bool init(unsigned long s, unsigned long e);		
		void draw();
		void drawBg(float x, float y, float z);

	private:
		
		Texture *texture;
		Texture *texture2;
		GridPoint3D *grid;

		float syke;
};

#endif