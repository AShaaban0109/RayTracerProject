#include "photon.h"

Photon::Photon(Photon::Mode mode, Ray ray, Colour intensity)
{
    this->mode = mode;
    this->path = ray;
    this->intensity = intensity;
}