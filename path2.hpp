#ifndef _SPLINE_HPP_
#define _SPLINE_HPP_

#include "externs.hpp"
#include "lista2.hpp"

class Spline
{
public:
	Spline();
	~Spline();
	
	void destroy();
	void addpoint(Vector &point);

	void draw();
	Vector getValue(float t);

private:
	
	int count;
	ListaKoskaEngineEiToimi *points;

};



#endif