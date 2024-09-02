#ifndef _VALOPALLO_HPP_
#define _VALOPALLO_HPP_

#include "externs.hpp"

class Valopallo
{
public:
	void calcPos(float t);

	Vector startpos;
	Vector endpos;
	Vector pos;

	float size;
	float phase;
	float speed;

	float currentpos;

	int count;
	Vector *vertices;
};

#endif