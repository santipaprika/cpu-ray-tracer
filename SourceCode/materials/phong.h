
#include "material.h"
#include <algorithm>

class Phong : public Material
{
public:
	Phong(Vector3D kd, Vector3D ks, int shine);

	bool Phong::hasSpecular() const;
	bool Phong::hasTransmission() const;
	bool Phong::hasScatter() const;
	bool Phong::hasDiffuseOrGlossy() const;
	virtual Vector3D Phong::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;
	virtual Vector3D getDiffuseCoefficient() const;

private:
	Vector3D kd;
	Vector3D ks;
	int shine;
};

// Implement the function "getReflectance" here
//
// (...)
//