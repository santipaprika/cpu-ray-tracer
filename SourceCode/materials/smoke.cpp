#include "smoke.h"

// Implement the Constructors here
Smoke::Smoke(double density, Vector3D color)
{
	this->density = density;
	this->color = color;
}

bool Smoke::hasSpecular() const
{
	return false;
}

bool Smoke::hasTransmission() const
{
	return false;
}

bool Smoke::hasDiffuseOrGlossy() const
{
	return false;
}

bool Smoke::hasScatter() const
{
	return true;
}

bool Smoke::hasPassed() const
{
	return ( (double(rand()) / RAND_MAX) > this->density );
}

Vector3D Smoke::smokeColor() const
{
	return color;
}