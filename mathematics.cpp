#include "mathematics.hpp"

Vector Mathematics::sphereToCartesian(float radius, float phi, float theta)
{
	float sp = (float)sin(phi);
	float cp = (float)cos(phi);
	float st = (float)sin(theta);
	float ct = (float)cos(theta);
	Vector value = Vector(radius*sp*ct, radius*sp*st, radius*cp);
	return value;
}
Vector Mathematics::cartesianToSphere(float x, float y, float z)
{
	float rho = (float)sqrt(x*x+y*y+z*z);
	if (rho == 0.0f) 
		return Vector(0,0,0);
	float phi = (float)acos(z/rho);
	float theta = (float)acos(x/(rho*sin(phi)));
	Vector value = Vector(rho, phi, theta);
	return value;
}
float Mathematics::integrateSimpson(float startpoint, float endpoint, int steps, float(*function)(float))
{
	float val = 0.0f;
	float h = (endpoint-startpoint)/steps;
	val += function(startpoint);
	val += function(endpoint);

	for (int i=0;i<steps-1;i++)
	{
		if (!(i&1)) //parillinen
			val += 4*function(h+i*h);
		else
			val += 2*function(h+i*h);
	}
	val *= h/3.0f;
	return val;
}
void Mathematics::antiRotate(Vector *x, Vector *y, Vector *z)
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);	
	
	*x = (Vector(matrix[0], matrix[4], matrix[8]));
	*y = (Vector(matrix[1], matrix[5], matrix[9]));
	*z = Vector(matrix[2], matrix[6], matrix[10]);
}

float Mathematics::calcSaturate(float value, float limit1, float limit2, float multiply)
{
	float pos, arvo;
	if (value <= limit1)
		return 0;
	if (value >= limit2)
		return 0;
	else
		pos = (float)(value-limit1)/(limit2-limit1);

	arvo = (float)sin(pos*3.141592f)*multiply;
	if (arvo < 0) 
		arvo = 0;
	if (arvo > 1)
		arvo = 1;

	return arvo;
}

float Mathematics::calcPosFloat(float value, float limit1, float limit2)
{
	if (value <= limit1)
		return 0;
	if (value >= limit2)
		return 1;
	else
		return (float)(value-limit1)/(limit2-limit1);
}

float Mathematics::randFloat()
{
	return (float)((rand()%10000)/10000.0f);
}


