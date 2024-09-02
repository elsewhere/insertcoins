#ifndef _MATHEMATICS_HPP_
#define _MATHEMATICS_HPP_

#include "externs.hpp"

class Mathematics
{
public:
	Mathematics() {}; 
	~Mathematics() {};

	//palauttaa Vectorn, jossa on halutut pallokoordinaatit karteesisessa koordinaatistossa
	//phi = [0, pii], theta = [0, 2*pii], radius = [0, inf]
	static Vector sphereToCartesian(float radius, float phi, float theta);
	//palauttaa Vectorn, jossa on halutut karteesiset koordinaatit pallokoordinaatistossa
	//v = [radius, phi, theta]. Ei huomioi, missä segmentissä ollaan joten muista tarkistaa
	//kulmat!
	static Vector cartesianToSphere(float x, float y, float z);

	//numeerinen integrointi Simpsonin menetelmällä
	static float integrateSimpson(float startpoint, float endpoint, int steps, float(*function)(float));
	static void antiRotate(Vector *x, Vector *y, Vector *z);
	static float calcSaturate(float value, float limit1, float limit2, float multiply);
	static float calcPosFloat(float value, float limit1, float limit2);
	static float randFloat();

};

#endif