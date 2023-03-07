#include "photon_material.h"

#include <math.h>
#include <random>

// TODO
PhotonMaterial::PhotonMaterial(Environment* environment, Colour diffuseReflection, Colour mirrorReflection, Colour transmissionReflection)
{
//BEGIN_STAGE_ONE
    this->environment = environment;
    this->diffuseReflection = diffuseReflection;
    this->mirrorReflection = mirrorReflection;
    this->transmissionReflection = transmissionReflection;

    // TODO add a member for prob of diffuse reflection in general. look at one of the sites i have. Also change the part of rausion roulete in scene with this change.
    maxDiffuseReflection= max(diffuseReflection.r, diffuseReflection.g);
    maxDiffuseReflection = max(maxDiffuseReflection, diffuseReflection.b);

    maxMirrorReflection= max(mirrorReflection.r, mirrorReflection.g);
    maxMirrorReflection= max(maxMirrorReflection, mirrorReflection.b);

    maxTransmissionReflection= max(transmissionReflection.r, transmissionReflection.g);
    maxTransmissionReflection= max(maxTransmissionReflection, transmissionReflection.b);


//END_STAGE_ONE
}

// The compute_once() method supplies the ambient term.
Colour PhotonMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;
//BEGIN_STAGE_ONE
	// TODO here we do the k nearest rendering stuff. Actually i think its in compute per light.
//END_STAGE_ONE
	return result;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour PhotonMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;
//BEGIN_STAGE_ONE
        float diff;

        Vector tolight;
        Vector toviewer;

        result.r=0.0f;
        result.g=0.0f;
        result.b=0.0f;

        tolight = ldir;
        tolight.negate();

        toviewer = viewer;
        toviewer.negate();

        diff = hit.normal.dot(tolight);
        
        // Scene.raytrace() does this test, but let's keep it here in case that changes or we get called from elsewhere. 
        if (diff < 0.0f) // light is behind surface
        {
            return result;
        } 

        // diffuse

        result += diffuseReflection * diff;
//END_STAGE_ONE
	return result;
    // return Colour(0,0,0); // todo remove this later. just for testing.
}

// 0 diffuse. 1 mirror reflection, 2 transmission, 3 absorption
// TODO make this not use ints
int PhotonMaterial::russianRoulette()
{
     // generate random numbers between -1 and 1. Used for random position and direction of each photon.
    static std::default_random_engine e;
    static std::uniform_real_distribution<> gen(0, 1); // range -1  to 1
    float r = gen(e);

    
    if (r < maxDiffuseReflection)
    {
        return 0;
    }
    else if (r < (maxDiffuseReflection + maxMirrorReflection) )
    {
        return 1;
    }
    else if (r < (maxDiffuseReflection + maxMirrorReflection + maxTransmissionReflection))
    {
        return 2;
    }
    else
    {
        return 3;   
    }
}

// https://raytracing.github.io/books/RayTracingInOneWeekend.html
Vector PhotonMaterial::diffuseReflect(Vector & normal)
{       
    static std::default_random_engine e;
    static std::uniform_real_distribution<> gen(-1, 1); // range -1  to 1

    float x,y,z;
    do {
        x = gen(e);
        y = gen(e);
        z = gen(e);
    } while ( x*x + y*y + z*z >= 1 );

    Vector inUnitSphere = Vector(x, y, z);
    inUnitSphere.normalise();
    if (inUnitSphere.dot(normal) > 0.0)  // In the same hemisphere as the normal
    {
        return inUnitSphere;
    }
    else
    {
        return -inUnitSphere;
    }
}