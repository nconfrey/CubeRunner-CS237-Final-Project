/*! \file plane.hxx
 *
 * \author Nick Confrey
 */

#ifndef _PLANE_HXX_
#define _PLANE_HXX_

#include "cs237.hxx"
#include "camera.hxx"

class Plane {
	public:

		enum Halfspace
		{
		 OUTSIDE = -1,
		 ON_PLANE = 0,
		 INSIDE = 1,
		};

		Plane();
		Plane(float a, float b, float c, float d);

		Plane ** extractPlanes(Camera c, cs237::mat4x4f projTransform);
		void NormalizePlane();
		int ClassifyPoint(const cs237::vec3d & pt, cs237::mat4x4f ModelViewMatrix);

		float a;
		float b;
		float c;
		float d;
};

#endif // !_PLANE_HXX_