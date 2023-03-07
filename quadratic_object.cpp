/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

#include "quadratic_object.h"
#include "transform.h"
#include <math.h>

using namespace std;

Quadratic::Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j)
{
//BEGIN_STAGE_TWO
    A = a;
    B = b;
    C = c;
    D = d;
    E = e;
    F = f;
    G = g;
    H = h;
    I = i;
    J = j;

	Q = Transform(A, B, C, D,
			          B, E, F, G,
			          C, F, H, i,
			          D, G, I, J);
//END_STAGE_TWO
  next = (Object *)0;
}

Hit *Quadratic::intersection(Ray ray)
{
//BEGIN_STAGE_TWO

    // Ray components.
    float px = ray.position.x;
    float py = ray.position.y;
    float pz = ray.position.z;

    float dx = ray.direction.x;
    float dy = ray.direction.y;
    float dz = ray.direction.z;
    

    float Aq = A*dx*dx + 2*B*dx*dy + 2*C*dx*dz + E*dy*dy + 2*F*dy*dz + H*dz*dz;
    float Bq = 2*(A*px*dx + B*(px*dy + dx*py) + C*(px*dz + dx*pz) + D*dx + E*py*dy + F*(py*dz + dy*pz) + G*dy + H*pz*dz + I*dz);
    float Cq = A*px*px +2*B*px*py + 2*C*px*pz +2*D*px + E*py*py + 2*F*py*pz +2*G*py + H*pz*pz + 2*I*pz + J;

    // if ray is tangent to the surface, return no intersection
    if (Aq == 0)
    {   
        return 0;
    }

    float disc = Bq*Bq - 4*Aq*Cq;
    if (disc <0)
    {
        return 0;
    }
    
    float t0 = (-Bq - sqrt(Bq*Bq - 4*Aq *Cq)) / ( 2*Aq);
    float t1 = (-Bq + sqrt(Bq*Bq - 4*Aq *Cq)) / ( 2*Aq);

    Hit* hit0 = new Hit();
	hit0->what = this;
	hit0->t = t0;
	hit0->entering = true;

	hit0->position = ray.position + hit0->t * ray.direction;
	hit0->normal.x = 2*(A*hit0->position.x + B*hit0->position.y + C*hit0->position.z + D);
	hit0->normal.y = 2*(B*hit0->position.x + E*hit0->position.y + F*hit0->position.z + G);
	hit0->normal.z = 2*(C*hit0->position.x + F*hit0->position.y + H*hit0->position.z + I);
	hit0->normal.normalise();
	if (hit0->normal.dot(ray.direction) > 0.0)
	{
		hit0->normal.negate();
	}

	Hit* hit1 = new Hit();
	hit1->what = this;
	hit1->t = t1;
	hit1->entering = false;

	hit1->position = ray.position + hit1->t * ray.direction;
	hit1->normal.x = 2*(A*hit1->position.x + B*hit1->position.y + C*hit1->position.z + D);
	hit1->normal.y = 2*(B*hit1->position.x + E*hit1->position.y + F*hit1->position.z + G);
	hit1->normal.z = 2*(C*hit1->position.x + F*hit1->position.y + H*hit1->position.z + I);
	hit1->normal.normalise();
	if (hit1->normal.dot(ray.direction) > 0.0)
	{
		hit1->normal.negate();
	}

	hit0->next = hit1;
	hit1->next = 0;

	return hit0;

//END_STAGE_TWO
    return 0;
}


void Quadratic::apply_transform(Transform& trans)
{
// BEGIN_STAGE_TWO
	Transform inv = trans.inverse();
	Transform invTransposed = trans.inverse().transpose();

    
    Q = inv*Q*invTransposed;
    
    A = Q.matrix[0][0];
    B = Q.matrix[0][1];
    C = Q.matrix[0][2];
    D = Q.matrix[0][3];
    E = Q.matrix[1][1];
    F = Q.matrix[1][2];
    G = Q.matrix[1][3];
    H = Q.matrix[2][2];
    I = Q.matrix[2][3];
    J = Q.matrix[3][3];
//END_STAGE_TWO
}
