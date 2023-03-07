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

#include "area_light.h"
#include "photon.h"
#include <random>

AreaLight::AreaLight()
{
	Light();
}

AreaLight::AreaLight(Vertex centre, Vector up, float width, float height, Colour col)
{
	Light();

    this->centre = centre;
    up.normalise();
    this->up = up;
    this->width = width;
    this->height = height;
	this->intensity = col;

    Vertex pposition;
    Vector direction;

}

bool AreaLight::get_direction(Vertex &surface, Vector &dir)
{
	dir = up;

	return true;
}

void AreaLight::get_intensity(Vertex &surface, Colour &level)
{
	level = intensity;
}

// TODO change this so it is actually an area light not a point light. Will need to have random positions and different random directions.
// currently, this is the implementation given in Jensons paper.
void AreaLight::emitPhotons(int nGlobalPhotons, int nCausticPhotons, Photon* globalPhotons, Photon* causticPhotons){
    // generate random numbers between -1 and 1. Used for random position and direction of each photon.
    static std::default_random_engine e;
    static std::uniform_real_distribution<> gen(-1, 1); // range -1  to 1


    //  use rejection sampling to find diffuse photon direction. From Jensen Paper
    float x,y,z;
    for (size_t gPhotonsEmitted = 0; gPhotonsEmitted < nGlobalPhotons; gPhotonsEmitted++)
    {
        do {
            x = gen(e);
            y = gen(e);
            z = gen(e);
        } while ( x*x + y*y + z*z > 1 );
        
        Photon gPhoton;
        // TODO make this area light
        gPhoton.path.position.x =5;
        gPhoton.path.position.y =5;
        gPhoton.path.position.z =5;
        
        gPhoton.path.direction.x = x;
        gPhoton.path.direction.y = y;
        gPhoton.path.direction.z = z;
        gPhoton.path.direction.normalise();

        gPhoton.intensity.r = intensity.r /nGlobalPhotons;
        gPhoton.intensity.g = intensity.g /nGlobalPhotons;
        gPhoton.intensity.b = intensity.b /nGlobalPhotons;
        gPhoton.mode = Photon::PHOTON_DIRECT;
        globalPhotons[gPhotonsEmitted] = gPhoton;
        // tracePhoton(photon);
    }

    // TODO similar for caustics?
}
