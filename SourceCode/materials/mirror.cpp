#include "mirror.h"

// Implement the Constructors here
Mirror::Mirror(Vector3D kd/*, Vector3D ks, int shine*/)
{
	this->kd = kd;
	/*this->ks = ks;
	this->shine = shine;*/
}

bool Mirror::hasSpecular() const
{
	// This material has specular component
	return true;
}

bool Mirror::hasTransmission() const
{
	return false;
}

bool Mirror::hasScatter() const
{
	return false;
}

bool Mirror::hasDiffuseOrGlossy() const
{
	return false;
}

Vector3D Mirror::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const //Habra qe cambiar esto respecto los espejos supongo o en el direct shader
{
	Vector3D wr = n * 2 * dot(n, wi) - wi;
	Vector3D r = kd*std::max(dot(wi,n),0.001) + ks * pow(std::max(dot(wo, wr), 0.0001), shine);
	//Vector3D r = kd * dot(wi, n);
	return r;
}
