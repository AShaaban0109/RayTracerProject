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

#include "scene.h"

Scene::Scene()
{
	object_list = 0;
	light_list = 0;
}

bool Scene::shadowtrace(Ray ray, float limit)
{
	Object *objects = this->object_list;

	while (objects != 0)
	{
		Hit *hit = this->select_first(objects->intersection(ray));

		if (hit != 0)
		{
		  if ((hit->t > 0.00000001f) &&( hit->t < limit))
		    {
			  delete hit;
		      return true;
		    }
		  delete hit;
		}

		objects = objects->next;
	}

	return false;
}

Hit *Scene::trace(Ray ray)
{
	Hit *best_hit = 0;

	Object *objects = this->object_list;

	while (objects != 0)
	{
		Hit *hit = this->select_first(objects->intersection(ray));

		if (hit != 0)
		{
			if (best_hit == 0)
			{
				best_hit = hit;

			} else if (hit->t < best_hit->t)
			{
				delete best_hit;
				best_hit = hit;
			}
			else
			{
				delete hit;
			}
		}

		objects = objects->next;
	}

	return best_hit;
}

Hit* Scene::select_first(Hit* list)
{
	Hit* result = 0;

	while (list != 0)
	{
		if (list->t >= 0.0f)
		{
			result = list;
			list = list->next;
			break;
		}

		Hit* temp = list;
		list = list->next;
		delete temp;
	}

	while (list != 0)
	{
		Hit* temp = list;
		list = list->next;
		delete temp;
	}

	return result;
}

void Scene::raytrace(Ray ray, int recurse, Colour &colour, float &depth)
{
    // if at the start of the process, we trace the photons and record them in our photon map. 
    // this is an ugly way of doing it. It would be nicer to call a new function such as tracePhotons from the camera class itself.
    if (applyPhotonMapping && firstPass)
    { 

        // TODO handle multiple lights later
        Light * light = light_list;

        // fill array with global and caustic photons. This will be transformed into a kdtree later.
        // Photon globalPhotons[nGlobalPhotons];
        Photon* globalPhotons = (Photon*)malloc(nGlobalPhotons*sizeof(Photon));
        Photon causticPhotons[nCausticPhotons];
        light->emitPhotons(nGlobalPhotons, nCausticPhotons, globalPhotons, causticPhotons);
        traceGlobalPhotons(globalPhotons, nGlobalPhotons);
        // traceCausticPhotons(causticPhotons);  TODO
        cout << "Done first pass" << endl;
    }

        // this works
        // Photon p;
        // p.path.position.x = 1;
        // p.path.position.y = 1;
        // p.path.position.z = 1;
        // globalPhotonMap->insert(p);
        // Photon p2;
        // p2.path.position.x = 2;
        // p2.path.position.y = 2;
        // p2.path.position.z = 2;
        // cout << globalPhotonMap->nearest(p2);

  
  Object *objects = object_list;
  Light *lights = light_list;
	
  // a default colour if we hit nothing.
  colour.r = 0.0f;
  colour.g = 0.0f;
  colour.b = 0.0f;
  colour.a = 0.0f;
  depth = 0.0f;

  // first step, find the closest primitive
  Hit *best_hit = this->trace(ray);

  // if we found a primitive then compute the colour we should see
  if (best_hit != 0)
  {
	  depth = best_hit->t;
	  colour = colour + best_hit->what->material->compute_once(ray, *best_hit, recurse); // this will be the global components such as ambient or reflect/refract

	  // next, compute the light contribution for each light in the scene.
	  Light* light = light_list;
	  while (light != (Light*)0)
	  {
		  Vector viewer;
		  Vector ldir;

		  viewer = ray.position-best_hit->position;
		  viewer.normalise();

		  bool lit;
		  lit = light->get_direction(best_hit->position, ldir);

		  if (ldir.dot(best_hit->normal) > 0)
		  {
			  lit = false;//light is facing wrong way.
		  }

		  // Put the shadow check here, if lit==true and in shadow, set lit=false
//BEGIN_STAGE_ONE
		  if (lit)
		  {
			  Ray shadow_ray;

			  shadow_ray.direction = -ldir;

			  shadow_ray.position = best_hit->position + (0.0001f * shadow_ray.direction);

			  if (this->shadowtrace(shadow_ray, 1000000000.0f))
			  {
				  lit = false; //there's a shadow so no lighting, if realistically close
			  }
		  }
//END_STAGE_ONE

		  if (lit)
		  {
			  Colour intensity;
			  
			  light->get_intensity(best_hit->position, intensity);
			  
			  colour = colour + intensity * best_hit->what->material->compute_per_light(viewer, *best_hit, ldir); // this is the per light local contrib e.g. diffuse, specular

		  }

		  light = light->next;
	  }

    // second pass
    if (applyPhotonMapping)
    {
        colour += secondPass(best_hit);
    }

	  delete best_hit;
  } else
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
		colour.a = 1.0f;
	}
}

void Scene::add_object(Object *obj)
{
  obj->next = this->object_list;
  this->object_list = obj;
}

void Scene::add_light(Light *light)
{
  light->next = this->light_list;
  this->light_list = light;
}

void Scene::traceGlobalPhotons(Photon * globalPhotons, int nGlobalPhotons)
{
        for (size_t i = 0; i < nGlobalPhotons; i++)
        {
            if (globalPhotons[i].mode == Photon::PHOTON_ABSOREBED)
            {
                continue;
            }
            
            Hit * best_hit;
            // for the max number of recursions 
            for (size_t n = 0; n < 5; n++)
            {
                best_hit = this->trace(globalPhotons[i].path);
                if (best_hit == 0)
                {
                    globalPhotons[i].mode == Photon::PHOTON_ABSOREBED;
                    break;
                    // TODO change this to something like Photon gone.
                }
                
                float russianRoulette = best_hit->what->material->russianRoulette();
                // TODO change this to uses modes like CSG
                
                // 0 is diffuse mode. We store in photon map, and reflect in random direction with reduced power.
                if (russianRoulette == 0)
                {
                    globalPhotons[i].path.position = best_hit->position;
                    //indicent direction remains constant

                    // store in map.
                    globalPhotonMap->insert(globalPhotons[i]);

                    // change new direction, and power. TODO change power if needed idk
                    globalPhotons[i].path.direction =  best_hit->what->material->diffuseReflect(best_hit->normal);
                    globalPhotons[i].mode = Photon::DIFFUSE_PHOTON_INDIRECT;
                    // // power attenuation formula
                    globalPhotons[i].intensity.r = globalPhotons[i].intensity.r * best_hit->what->material->diffuseReflection.r / best_hit->what->material->maxDiffuseReflection;
                    globalPhotons[i].intensity.g = globalPhotons[i].intensity.g * best_hit->what->material->diffuseReflection.g/ best_hit->what->material->maxMirrorReflection;
                    globalPhotons[i].intensity.b = globalPhotons[i].intensity.b * best_hit->what->material->diffuseReflection.b/ best_hit->what->material->maxTransmissionReflection;
                }

                // TODO if rr == 1 and 2

                // abosrbed. Insert into map and continue to next photon.
                if (russianRoulette == 3)
                {
                    globalPhotons[i].mode = Photon::PHOTON_ABSOREBED;
                    break; //todo look at notes and check this
                    globalPhotons[i].path.position = best_hit->position;
                    globalPhotonMap->insert(globalPhotons[i]);
                    break;
                }
            
            // TODO
            // trace find hit
            // perform russian roulette with hit.material
                // ie. if if pd = 0.2, ps = 0.4
                    // then we have pabsorption = 0.4
                    // pdiffuse reflection = 0.2.
                        // we can do a random cosine direction from the intersection
                    //ps = 0.4
                        // we can split that into mirror reflection and trasmission. eg. pm = 0.1, pt = 0.3      
            // if absorption, photon stays the same. we change pos to where it landed, and dir as incident dir. same power.
            // if diffuse idk
            // if pm or pt send to a specular handler. 
        }
    }
    
    firstPass = false;
}

Colour Scene::secondPass(Hit * hit)
{
    Colour colour;
    // TODO change this to take parameter values for things like radius.
    // this photon is just used to get the position. Can be better if i understand KD tree imp // TODO
    Photon p;
    p.path.position.x = hit->position.x;
    p.path.position.y = hit->position.y;
    p.path.position.z = hit->position.z;
    std::vector<CORE::Item> nearestPhotonHits = globalPhotonMap->find(p, 1000, 1);

    if (nearestPhotonHits.size() >0)
    {
        Colour photons_c = Colour(0, 0, 0);
        for (size_t i = 0; i < nearestPhotonHits.size(); i++)
        {
            photons_c.r += nearestPhotonHits.at(i).intensity.r;
            photons_c.g += nearestPhotonHits.at(i).intensity.g;
            photons_c.b += nearestPhotonHits.at(i).intensity.b;
        }

        photons_c.r = photons_c.r / nearestPhotonHits.size();
        photons_c.g = photons_c.g / nearestPhotonHits.size();
        photons_c.b = photons_c.b / nearestPhotonHits.size();
        
        colour.r += photons_c.r;
        colour.g += photons_c.g;
        colour.b += photons_c.b;
    }
    return colour;
}

Scene::Scene(int nGlobalPhotons, int nCausticPhotons, KD::Tree<CORE> *kdGlobal)
{
    object_list = 0;
	light_list = 0;

    this->nGlobalPhotons = nGlobalPhotons;
    this->nCausticPhotons = nCausticPhotons;
    applyPhotonMapping = true;
    firstPass = true;
    globalPhotonMap = kdGlobal;
}
