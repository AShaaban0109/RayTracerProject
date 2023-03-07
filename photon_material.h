#pragma once

#include "material.h"
#include "environment.h"

class PhotonMaterial : public Material {
public:
    // probability for each type of reflection. This is used with russian roulete later.
	// Colour diffuseReflection;
	// Colour mirrorReflection;
    // Colour transmissionReflection;
    Environment* environment;
    // float maxDiffuseReflection, maxMirrorReflection, maxTransmissionReflection;

    PhotonMaterial(Environment* environment, Colour diffuseReflection, Colour mirrorReflection, Colour transmissionReflection);
//END_STAGE_ONE
	PhotonMaterial()
	{

	}

	Colour compute_once(Ray& viewer, Hit& hit, int recurse);
	Colour compute_per_light(Vector& viewer, Hit& hit, Vector& ldir);
    int russianRoulette();
    Vector diffuseReflect(Vector &normal);

};
