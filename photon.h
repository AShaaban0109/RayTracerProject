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

#pragma once
#include "ray.h"
#include "colour.h"
#include "vertex.h"

class Photon
{
public:
	Photon()
	{
	}

	enum Mode {
		PHOTON_DIRECT = 0,
		DIFFUSE_PHOTON_INDIRECT,
		SPECULAR_PHOTON_INDIRECT,
        PHOTON_ABSOREBED,
	};
	Ray path;
	Colour intensity;
    Mode mode;

    // functions
    Photon(Photon::Mode mode, Ray path, Colour intensity);
    
    float operator[] (int axis) const
    {
        if (axis ==0)
        {
            return path.position.x;
        }

        if (axis ==1)
        {
            return path.position.y;
        }

        return path.position.z;
    }

    bool operator == (const Photon& p) const {
        return path.position.x == p[0] && path.position.y == p[1] && path.position.z == p[2];
    }

    friend std::ostream& operator << (std::ostream& s, const Photon& p) {
    return s << '(' << p[0] << ", " << p[1] << ", " << p[2] << ')';
    }

};

