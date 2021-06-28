
#include "material.h"
#include <algorithm>

class Transmissive : public Material
{
public:
	Transmissive(double n2, Vector3D kd/*, Vector3D ks, int shine*/);

	bool Transmissive::hasSpecular() const;
	bool Transmissive::hasTransmission() const;
	bool Transmissive::hasScatter() const;
	bool Transmissive::hasDiffuseOrGlossy() const;
	virtual double Transmissive::getIndexOfRefraction() const;
	virtual Vector3D Transmissive::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const;

private:
	Vector3D kd;
	Vector3D ks;
	int shine; 
	double n2;
};

// Implement the function "getReflectance" here
//
// (...)
//