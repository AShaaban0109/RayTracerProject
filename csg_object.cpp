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

/* CSG is an object that is built by Constructive Solid Geometry from two sub-objects.*/

using namespace std;

#include "csg_object.h"

CSG::CSG(CSG::Mode p_mode, Object* p_left, Object* p_right)
{
//BEGIN_STAGE_TWO
    mode = p_mode;
    object1 = p_left;
    object2 = p_right;
//END_STAGE_TWO
	next = (Object*)0;
}

// for Union this process is simple. We take the earliest (least t) hit when
// both are entering, and the furthest hit when both are leaving.
Hit* CSG::unionOp(Ray ray){
    Hit* result = 0;

    // call the intersection method of each of the child objects
    // this gives us a sorted list of hits for each object
    Hit* object1Hits = object1->intersection(ray);
    Hit* object2Hits = object2->intersection(ray);

    // if no intersection between ray and an object, we record hits for
    // just the other object. (if there is no intersection between
    //  that object either,  this will be returned as null.)
    if (object2Hits == 0)
    {
        return object1Hits;
    }
    
    if (object1Hits == 0)
    {
        return object2Hits;
    }
    
    if (object1Hits->entering == true && object2Hits->entering == true)
    {
        if (object1Hits->t < object2Hits->t)
        {
            result = object1Hits;
        }
        else
        {
            result = object2Hits;
        }
    }
    
    // find the last hit along the ray.
    while (object1Hits->next != 0)
    {
        object1Hits = object1Hits->next;
    }

    while (object2Hits->next != 0)
    {
        object2Hits = object2Hits->next;
    }
    

    if (object1Hits->entering == false && object2Hits->entering == false)
    {
        if (object1Hits->t < object2Hits->t)
        {
            result->next = object2Hits;
        }
        else
        {
            result->next = object1Hits;
        }
    }

    return result;
}


// for Intersection this process is simple. We take the latest (highest t) hit when
// both are entering, and the closest hit when both are leaving.
Hit* CSG::intersectionOp(Ray ray){
    Hit* result = 0;

    // call the intersection method of each of the child objects
    // this gives us a sorted list of hits for each object
    Hit* object1Hits = object1->intersection(ray);
    Hit* object2Hits = object2->intersection(ray);

    // if no intersection between ray and an object, it is not possible for there to be an intersection of two objects.
    // we return 0.
    // just the other object. (if there is no intersection between
    //  that object either,  this will be returned as null.)
    if (object2Hits == 0 || object1Hits == 0)
    {
        return 0;
    }
    
    if (object1Hits->entering == true && object2Hits->entering == true)
    {
        if (object1Hits->t < object2Hits->t)
        {
            result = object2Hits;
        }
        else
        {
            result = object1Hits;
        }
    }
    
    // find the last hit along the ray.
    while (object1Hits->next != 0)
    {
        object1Hits = object1Hits->next;
    }

    while (object2Hits->next != 0)
    {
        object2Hits = object2Hits->next;
    }
    

    if (object1Hits->entering == false && object2Hits->entering == false)
    {
        if (object1Hits->t < object2Hits->t)
        {
            result->next = object1Hits;
        }
        else
        {
            result->next = object2Hits;
        }
    }

    return result;
}


Hit* CSG::differenceOP(Ray ray){
    Hit* result = 0;

    // call the intersection method of each of the child objects
    // this gives us a sorted list of hits for each object
    Hit* object1Hits = object1->intersection(ray);
    Hit* object2Hits = object2->intersection(ray);

    // if we hit the object we want to subtract, return null
    if (object2Hits != 0)
    {
        return 0;
    }
    
    if (object1Hits == 0)
    {
        return 0;
    }
    result = object1Hits;


    return result;
}

Hit* CSG::intersection(Ray ray)
{
	Hit* result = 0;
// BEGIN_STAGE_TWO
    if (mode == CSG_UNION)
    {
        result = unionOp(ray);
    } else if (mode == CSG_INTER)
    {
        result = intersectionOp(ray);
    } else {
        result = differenceOP(ray);
    }
// END_STAGE_TWO
	return result;
}

void CSG::apply_transform(Transform& transform)
{
//BEGIN_STAGE_TWO
    object1->apply_transform(transform);
    object2->apply_transform(transform);
//END_STAGE_TWO
}
