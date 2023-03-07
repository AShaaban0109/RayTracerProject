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

#include "global_material.h"

#include <math.h>

GlobalMaterial::GlobalMaterial(Environment* p_env, Colour p_reflect_weight, Colour p_refract_weight, float p_ior)
{
//BEGIN_STAGE_TWO
    environment = p_env;
    reflect_weight = p_reflect_weight;
    refract_weight = p_refract_weight;
    ior = p_ior;
    kreflect;
//END_STAGE_TWO
}

//BEGIN_STAGE_TWO
Vector GlobalMaterial::calculateReflection(Vector& viewer, Vector& normal) {
    return viewer - 2 * viewer.dot(normal) * normal;
}

void fresnel(Vector& view, Vector& normal, float etai, float etat, float& kr){
	double cosi = -view.dot(normal);
    float r = etai/etat;
    float TIRCheck = 1.0-pow(r,2.0) * (1.0-pow(cosi,2.0));
    if (TIRCheck < 0)
    {
        kr =1;
        return;
    }

}

// If not TIR, refracts ray and returns true. Refracted ray is stored in refract_ray argument.
// view and hit.normal are normalised from before.
bool GlobalMaterial::refract_ray(Vector& view, Hit& hit, float ior, Vector& refract_ray){
    double r1, r2, r;  // index of refraction of initial medium, and subsequent medium.    
    // If we are entering, the initial medium is air (ior =1), and second medium is the material (ior = ior). 
	if (hit.entering)
	{
        r1 = 1;
        r2 = ior;
	} else
    {
        r1 = ior;
        r2 =1;
    }
    r = r1/r2;

	double cosi = -view.dot(hit.normal);
    float TIRCheck = 1.0- r * r * (1.0-cosi*cosi);
    
    // Frensel
    if (TIRCheck < 0)
    {
        kreflect =1;
        return false;
    }
    double cost = sqrt(TIRCheck);
    float rpar = ((1/r) * cosi - cost) / ((1/r)*cosi +cost);
    float rper = (cosi - (1/r) *cost) / (cosi + (1/r)*cost);
    kreflect = 0.5*(rpar*rpar + rper+rper);

	refract_ray = r*view + (r*cosi - cost) * hit.normal;
    return true;
}
//END_STAGE_TWO

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;

//BEGIN_STAGE_TWO
    //if we have reached the depth specified, end the recursion.
    if (recurse == 0)
    {
        return result;
    }

    kreflect = 1;   //reflection coefficient. Default to 1. 1 is the value of kr when TIR occurs. Means full reflection. 
    float depth;

    // get reflection colour
    Ray reflectionRay;
    Colour reflectionColour;
    reflectionRay.direction = calculateReflection(viewer.direction, hit.normal);
    reflectionRay.position = hit.position + 0.01 * reflectionRay.direction;
    environment->raytrace(reflectionRay, recurse -1, reflectionColour, depth);

    Ray refractionRay;
    Colour refractionColour;
    if (refract_ray(viewer.direction, hit, ior, refractionRay.direction))
    {
        refractionRay.position = hit.position + 0.01 * refractionRay.direction;
        environment->raytrace(refractionRay, recurse -1, refractionColour, depth);
    }

    result.r += reflect_weight.r * kreflect * reflectionColour.r + refract_weight.r * (1-kreflect) * refractionColour.r;
    result.g += reflect_weight.g * kreflect * reflectionColour.g + refract_weight.g * (1-kreflect) * refractionColour.g;
    result.b += reflect_weight.b * kreflect * reflectionColour.b + refract_weight.b * (1-kreflect) * refractionColour.b;
//END_STAGE_TWO
	return result;
}

Colour GlobalMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;

	result.r=0.0f;
	result.g=0.0f;
	result.b=0.0f;

	return result;
}

