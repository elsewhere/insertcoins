#ifndef _PATH_HPP_
#define _PATH_HPP_

#include "vector.hpp"
#include "hermitecurve.hpp"
#include "lista.hpp"

//--------------------------------------------------------------------------------------------
//  This code belongs to our library namespace: TRACTION_DEMOTRACTOR
//--------------------------------------------------------------------------------------------

namespace TRACTION_DEMOTRACTOR
{

	class __declspec(dllexport) Path
	{
	public:
		Path();
		~Path();
		
		void destroy();
		void addpoint(Vector &point);

		void draw();
		Vector getValue(float t);
		Vector getDerivative(float t); 

	private:
		
		int count;
		Lista *points;

	};
}

#endif