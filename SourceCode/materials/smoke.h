
#include "material.h"
#include <algorithm>

class Smoke : public Material
{
public:
	Smoke(double density, Vector3D color);

	bool Smoke::hasSpecular() const;
	bool Smoke::hasTransmission() const;
	bool Smoke::hasDiffuseOrGlossy() const;
	bool Smoke::hasScatter() const;
	bool Smoke::hasPassed() const;
	virtual Vector3D Smoke::smokeColor() const;

private:
	Vector3D color; 
	double density;
};

// Implement the function "getReflectance" here
//
// (...)
//