/*! \file plane.cxx
 *
 * \author Nick Confrey
 * Simple representation of a geometric plane
 */

 #include "plane.hxx"
 #include "cs237.hxx"

 Plane::Plane()
 {
 	this->a = 0;
 	this->b = 0;
 	this->c = 0;
 	this->d = 0;
 }

 Plane::Plane(float a, float b, float c, float d)
 {
 	this->a = a;
 	this->b = b;
 	this->c = c;
 	this->d = d;
 }
/*

	Extract the view frustum from a projection 4x4 matrix
	Attributed to Gil Gribb and Klaus Hartmann

*/
 Plane ** Plane::extractPlanes(cs237::mat4x4f projTransform)
 {
 	Plane **p_planes = new Plane *[6];
 	for(int i = 0; i <=5; i++)
 		p_planes[i] = new Plane();
 	// Left clipping plane
	 p_planes[0]->a = projTransform[3][0] + projTransform[0][0];
	 p_planes[0]->b = projTransform[3][1] + projTransform[0][1];
	 p_planes[0]->c = projTransform[3][2] + projTransform[0][2];
	 p_planes[0]->d = projTransform[3][3] + projTransform[0][3];
	 // Right clipping plane
	 p_planes[1]->a = projTransform[3][0] - projTransform[0][0];
	 p_planes[1]->b = projTransform[3][1] - projTransform[0][1];
	 p_planes[1]->c = projTransform[3][2] - projTransform[0][2];
	 p_planes[1]->d = projTransform[3][3] - projTransform[0][3];
	 // Top clipping plane
	 p_planes[2]->a = projTransform[3][0] - projTransform[1][0];
	 p_planes[2]->b = projTransform[3][1] - projTransform[1][1];
	 p_planes[2]->c = projTransform[3][2] - projTransform[1][2];
	 p_planes[2]->d = projTransform[3][3] - projTransform[1][3];
	 // Bottom clipping plane
	 p_planes[3]->a = projTransform[3][0] + projTransform[1][0];
	 p_planes[3]->b = projTransform[3][1] + projTransform[1][1];
	 p_planes[3]->c = projTransform[3][2] + projTransform[1][2];
	 p_planes[3]->d = projTransform[3][3] + projTransform[1][3];
	 // Near clipping plane
	 p_planes[4]->a = projTransform[3][0] + projTransform[2][0];
	 p_planes[4]->b = projTransform[3][1] + projTransform[2][1];
	 p_planes[4]->c = projTransform[3][2] + projTransform[2][2];
	 p_planes[4]->d = projTransform[3][3] + projTransform[2][3];
	 // Far clipping plane
	 p_planes[5]->a = projTransform[3][0] - projTransform[2][0];
	 p_planes[5]->b = projTransform[3][1] - projTransform[2][1];
	 p_planes[5]->c = projTransform[3][2] - projTransform[2][2];
	 p_planes[5]->d = projTransform[3][3] - projTransform[2][3];

	 //NormalizePlane(p_planes[0]);
	 //NormalizePlane(p_planes[1]);
	 //NormalizePlane(p_planes[2]);
	 //NormalizePlane(p_planes[3]);
	 //NormalizePlane(p_planes[4]);
	 //NormalizePlane(p_planes[5]);

	 return p_planes;
 }

void Plane::NormalizePlane(Plane * plane)
{
 float mag;
 mag = sqrt(plane->a * plane->a + plane->b * plane->b + plane->c * plane->c);
 plane->a = plane->a / mag;
 plane->b = plane->b / mag;
 plane->c = plane->c / mag;
 plane->d = plane->d / mag;
}

int Plane::ClassifyPoint(const cs237::vec3d & pt)
{
 // float d;
 // d = this->a*pt.x + this->b*pt.y + this->c*pt.z + this->d;
 //printf("distance to plane %f\n", d);
 // if (d < 0) return OUTSIDE;
 // if (d > 0) return INSIDE;
 // return OUTSIDE;

	const cs237::vec3d * normal = new cs237::vec3d(this->a, this->b, this->c);
	float distance = cs237::__detail::dot( *normal, pt) + this->d;
	//printf("distance to plane %f\n", distance);
	if (distance < 0) return OUTSIDE;
	if (distance > 0) return INSIDE;
	return ON_PLANE;
}