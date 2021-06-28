
#include "material.h"
#include <algorithm>

class Mirror : public Material
{
public:
	Mirror(Vector3D kd/*, Vector3D ks, int shine*/);

	bool Mirror::hasSpecular() const;
	bool Mirror::hasTransmission() const;
	bool Mirror::hasScatter() const;
	bool Mirror::hasDiffuseOrGlossy() const;
	virtual Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;

private:
	Vector3D kd;
	Vector3D ks;
	int shine;
};

// Implement the function "getReflectance" here
//
// (...)
//