#include "valopallo.hpp"

void Valopallo::calcPos(float t)
{

	float t2 = t*speed + phase;
	t2 = t2 - (int)t2;

	pos = startpos * (1-t2) + endpos * t2;

	currentpos = t2;
}