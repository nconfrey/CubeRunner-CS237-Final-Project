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
 Plane ** Plane::extractPlanes(Camera c, cs237::mat4x4f projTransform)
 {
 	Plane **p_planes = new Plane *[6];
 	for(int i = 0; i <=5; i++)
 		p_planes[i] = new Plane();
 	// Left clipping plane
	 // p_planes[0]->a =  projTransform[3][0] + projTransform[0][0];
	 // p_planes[0]->b =  projTransform[3][1] + projTransform[0][1];
	 // p_planes[0]->c =  projTransform[3][2] + projTransform[0][2];
	 // p_planes[0]->d =  projTransform[3][3] + projTransform[0][3];
	 // // Right clipping plane
	 // p_planes[1]->a =  projTransform[3][0] - projTransform[0][0];
	 // p_planes[1]->b =  projTransform[3][1] - projTransform[0][1];
	 // p_planes[1]->c =  projTransform[3][2] - projTransform[0][2];
	 // p_planes[1]->d =  projTransform[3][3] - projTransform[0][3];
	 // // Top clipping plane
	 // p_planes[2]->a =  projTransform[3][0] - projTransform[1][0];
	 // p_planes[2]->b =  projTransform[3][1] - projTransform[1][1];
	 // p_planes[2]->c =  projTransform[3][2] - projTransform[1][2];
	 // p_planes[2]->d =  projTransform[3][3] - projTransform[1][3];
	 // // Bottom clipping plane
	 // p_planes[3]->a =  projTransform[3][0] + projTransform[1][0];
	 // p_planes[3]->b =  projTransform[3][1] + projTransform[1][1];
	 // p_planes[3]->c =  projTransform[3][2] + projTransform[1][2];
	 // p_planes[3]->d =  projTransform[3][3] + projTransform[1][3];
	 // // Near clipping plane
	 // p_planes[4]->a =  projTransform[3][0] + projTransform[2][0];
	 // p_planes[4]->b =  projTransform[3][1] + projTransform[2][1];
	 // p_planes[4]->c =  projTransform[3][2] + projTransform[2][2];
	 // p_planes[4]->d =  projTransform[3][3] + projTransform[2][3];
	 // // Far clipping plane
	 // p_planes[5]->a =  projTransform[3][0] - projTransform[2][0];
	 // p_planes[5]->b =  projTransform[3][1] - projTransform[2][1];
	 // p_planes[5]->c =  projTransform[3][2] - projTransform[2][2];
	 // p_planes[5]->d =  projTransform[3][3] - projTransform[2][3];

	 // p_planes[0]->NormalizePlane();
	 // p_planes[1]->NormalizePlane();
	 // p_planes[2]->NormalizePlane();
	 // p_planes[3]->NormalizePlane();
	 // p_planes[4]->NormalizePlane();
	 // p_planes[5]->NormalizePlane();

 	//THESE ARE IN VIEW SPACE
 	/*

	AKA camera space. that means to test against a point, we also need to put the point into camera space
	Right now, I think we can do this by multiplying by the modelview matrix

 	*/


 	float e = 1.0f/(tan(cs237::radians(c.fov())/2));
 	float a = c.aspect();
 	printf("a: [%f] e: [%f]\n",a,e);
 	//Near
 	p_planes[0]->a = 0;
 	p_planes[0]->b = 0;
 	p_planes[0]->c = -1.0f;
 	p_planes[0]->d = -(c.near());

 	//Far
 	p_planes[1]->a = 0;
 	p_planes[1]->b = 0;
 	p_planes[1]->c = 1.0f;
 	p_planes[1]->d = c.far();
 	//Left
 	p_planes[2]->a = e/(sqrt((e*e) + 1));
 	p_planes[2]->b = 0;
 	p_planes[2]->c = -1.0f/(sqrt((e*e) + 1));
 	p_planes[2]->d = cs237::__detail::length(c.position());
 	//Right
 	p_planes[3]->a = -e/(sqrt((e*e) + 1));
 	p_planes[3]->b = 0;
 	p_planes[3]->c = -1.0f/(sqrt((e*e) + 1));;
 	p_planes[3]->d = cs237::__detail::length(c.position());
 	//Bottom
 	p_planes[4]->a = 0;
 	p_planes[4]->b = e / (sqrt((e*e) + (a*a)));
 	p_planes[4]->c = -a / (sqrt((e*e) + (a*a)));
 	p_planes[4]->d = cs237::__detail::length(c.position());
 	//Top
 	p_planes[5]->a = 0;
 	p_planes[5]->b = -e / (sqrt((e*e) + (a*a)));
 	p_planes[5]->c = -a / (sqrt((e*e) + (a*a)));
 	p_planes[5]->d = cs237::__detail::length(c.position());

	 return p_planes;
 }

void Plane::NormalizePlane()
{
 float mag;
 mag = sqrt(this->a * this->a + this->b * this->b + this->c * this->c);
 this->a = this->a / mag;
 this->b = this->b / mag;
 this->c = this->c / mag;
 this->d = this->d / mag;
}

int Plane::ClassifyPoint(const cs237::vec3d & pt, cs237::mat4x4f ModelViewMatrix)
{
 // float d;
 // d = this->a*pt.x + this->b*pt.y + this->c*pt.z + this->d;
 //printf("distance to plane %f\n", d);
 // if (d < 0) return OUTSIDE;
 // if (d > 0) return INSIDE;
 // return OUTSIDE;

	//Transform the point into camera space??
	//cs237::vec4f *pt4 = new cs237::vec4f(pt.x, pt.y, pt.z, 1);
	//cs237::vec3f pt3 = cs237::vec3f(ModelViewMatrix * *pt4);
	//cs237::vec3d pt3d = cs237::vec3d((float)pt3.x, (float)pt3.y, (float)pt3.z);
	//printf("corner of the bounding box is at [%f][%f][%f]\n", pt3d.x, pt3d.y, pt3d.z);

	cs237::vec3d normal = cs237::vec3d(this->a, this->b, this->c);
	//normal = cs237::__detail::normalize(normal);
	float epsilon = 10;
	float distance = cs237::__detail::dot(normal, pt) + this->d;
	printf("distance to plane %f\n", distance - epsilon);
	if (distance + epsilon > 0 || distance - epsilon > 0) return INSIDE;
	if (distance + epsilon < 0 || distance - epsilon < 0) return OUTSIDE;
	
	return ON_PLANE;
}