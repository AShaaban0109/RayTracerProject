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

/* This is the top level for the program you need to create for lab three and four.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"
#include "Quadratic_object.h"
#include "csg_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"
#include "area_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"
#include "global_material.h"
#include "compound_material.h"
#include "photon_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"
#include "dof_camera.h"

using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(Scene& scene)
{
	// The following transform allows 4D homogeneous coordinates to be transformed.It moves the supplied teapot model to somewhere visible.
	Transform * transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, 1.0f, -2.7f,
                                          0.0f, 1.0f, 0.0f, 5.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);
    
    Transform * scale2 = new Transform(2.0f, 0.0f, 0.0f, 0.0f,
                                       0.0f, 2.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 2.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f, 1.0f);

	//  Read in the teapot model.
	PolyMesh* pm = new PolyMesh("teapot_smaller.ply", true);
	pm->apply_transform(*transform);

    Plane* plane = new Plane(0.0, 1.0, 0.0, 1.0);
    Plane* plane2 = new Plane(-1.0, 0.0, 0.0, 1.5);
	Sphere* sphere = new Sphere(Vertex(-1.0f, -0.5f, 3.0f), 0.5f);  // left small
	Sphere* sphere2 = new Sphere(Vertex(0.4f, -0.5f, 2.0f), 0.5f);  // mid
	Sphere* sphere3 = new Sphere(Vertex(1.4f, 0.0f, 6.0f), 1.0f);  // left
	Sphere* sphere4 = new Sphere(Vertex(1.0f, 0.0f, 3.0f), 1.0f);  // big centre
	Sphere* sphere5 = new Sphere(Vertex(-0.1f, 0.0f, 5.0f), 0.4f);  // small centre behind
	Sphere* sphere6 = new Sphere(Vertex(0.0f, 0.0f, -3.0f), 0.4f);  // hidden

    Quadratic* quadraticSphere = new Quadratic(1,0,0,0,1,0,0,1,0,-0.25);  // big quadratic sqhere
    CSG * csgSpheres = new CSG(CSG::CSG_INTER, sphere, sphere2);

	// DirectionalLight* dl = new DirectionalLight(Vector(1.01f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));
    AreaLight * areaLight = new AreaLight(Vertex(5,5,5), Vector(1.01f, -1.0f, 1.0f) , 1.0, 1.0, Colour(1.0,1.0, 1.0));  // TODO change dir of this


    Phong* planeColour = new Phong(Colour(0.2f, 0.2f, 0.2f), Colour(0.5f, 0.5f, 0.5f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong* planeColour2 = new Phong(Colour(0.0f, 0.8f, 0.0f), Colour(0.0f, 0.0f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 40.f);  // TODO its cos light is not reaching
	Phong* bp1 = new Phong(Colour(0.0f, 0.0f, 0.0f), Colour(0.6f, 0.0f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);  // red
	Phong* bp2 = new Phong(Colour(0.0f, 0.0f, 0.2f), Colour(0.0f, 0.0f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 40.f);  // blue

    GlobalMaterial* gmaterial= new GlobalMaterial(&scene, Colour(0.5f, 0.5f, 0.5f), Colour(0.5f, 0.5f, 0.5f), 1.9);

    CompoundMaterial* cmaterial1 = new CompoundMaterial(2);
    cmaterial1->include_material(bp1);
    cmaterial1->include_material(gmaterial);

    CompoundMaterial* cmaterial2 = new CompoundMaterial(2);
    cmaterial2->include_material(bp2);
    cmaterial2->include_material(gmaterial);

    PhotonMaterial * photonMaterial1 = new PhotonMaterial(&scene, Colour(0.1f, 0.1f, 0.3f), Colour(0.2f, 0.2f, 0.2f), Colour(0.1, 0.1,0.1));
    PhotonMaterial * photonMaterial2 = new PhotonMaterial(&scene, Colour(0.3f, 0.1f, 0.1f), Colour(0.2f, 0.2f, 0.2f), Colour(0.1, 0.1,0.1));
    PhotonMaterial * photonMaterial3 = new PhotonMaterial(&scene, Colour(0.1f, 0.3f, 0.1f), Colour(0.2f, 0.2f, 0.2f), Colour(0.1, 0.1,0.1));
    

    quadraticSphere->apply_transform(*scale2);

	pm->set_material(bp1);
    quadraticSphere->set_material(bp1);
    csgSpheres->set_material(bp1);

    plane->set_material(photonMaterial1);
    plane2->set_material(photonMaterial2);
	sphere->set_material(photonMaterial2);
    sphere2->set_material(photonMaterial3);
    // plane->set_material(planeColour);
    // plane2->set_material(planeColour2);
	// sphere->set_material(bp2);
    // sphere2->set_material(bp1);
    sphere3->set_material(bp1);
    sphere4->set_material(cmaterial2);
    sphere5->set_material(bp1);
    sphere6->set_material(bp2);

    // scene.add_object(pm);
    // scene.add_object(quadraticSphere);
    // scene.add_object(csgSpheres);
	// scene.add_light(dl);
    scene.add_light(areaLight);

    scene.add_object(plane);
    scene.add_object(plane2);
	scene.add_object(sphere);
	scene.add_object(sphere2);
	// scene.add_object(sphere3);
    // scene.add_object(sphere4);
    // scene.add_object(sphere5);
    // scene.add_object(sphere6);
    
}


// This is the entry point function to the program.
int main(int argc, char *argv[])
{
	int width = 512;
	int height = 512;
    // int width = 100;
	// int height = 100;
	// Create a framebuffer
	FrameBuffer* fb = new FrameBuffer(width, height);
	
	// Create a scene with number of global and caustic photons
    // TODO make this not hardcoded and make it nicer
    int min = -5;
    int max = 5;
    Photon ps;
    ps.path.position.x = min;
    ps.path.position.y = min;
    ps.path.position.z = min;

    Photon pe;
    pe.path.position.x = max;
    pe.path.position.y = max;
    pe.path.position.z = max;
    KD::Tree<CORE> kdGlobal(ps, pe);

	Scene *scene = new Scene(500000,10000, &kdGlobal);
	
	// Setup the scene
	build_scene(*scene);
	
	// Declare a camera
	// Camera *camera = new SimpleCamera(0.5f);
    Vertex origin = Vertex(0.0f, 0.0f, 0.0f);
    Vector lookat = Vector (0.0f, 0.0f, 1.0f);
    Vector up = Vector (0.0f, 1.0f, 0.0f);
	Camera* camera = new FullCamera(0.5f, origin, lookat, up);
	
	// Camera generates rays for each pixel in the framebuffer and records colour + depth.
	camera->render(*scene,*fb);
	
	// Output the framebuffer colour and depth as two images
	fb->writeRGBFile((char *)"test.ppm");
	fb->writeDepthFile((char *)"depth.ppm");
	
	cerr << "\nDone.\n" << flush;
	
	// a debug check to make sure we don't leak hits.
	cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
	return 0;
}
