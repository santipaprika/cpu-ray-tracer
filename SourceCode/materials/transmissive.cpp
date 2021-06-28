#include "transmissive.h"

// Implement the Constructors here
Transmissive::Transmissive(double n2, Vector3D kd/*, Vector3D ks, int shine*/)
{
	this->kd = kd;
	this->n2 = n2;
	/*this->ks = ks;
	this->shine = shine;*/
}

bool Transmissive::hasSpecular() const
{
	// This material has specular component
	/*double cosA = dot(wo, n) / (wo.length()*n.length());
	return (1 + pow(nt,2)*(pow(cosA,2)-1)) < 0;*/
	return false;
}

bool Transmissive::hasTransmission() const
{
	/*double cosA = dot(wo, n) / (wo.length()*n.length());
	return (1 + pow(nt, 2)*(pow(cosA, 2) - 1)) >= 0;*/
	return true;
}

bool Transmissive::hasDiffuseOrGlossy() const
{
	return false;
}

bool Transmissive::hasScatter() const
{
	return false;
}

double Transmissive::getIndexOfRefraction() const
{
	return n2;
}

Vector3D Transmissive::getReflectance(const Vector3D &n, const Vector3D &wo, const Vector3D &wi) const //Habra qe cambiar esto respecto los espejos supongo o en el direct shader
{
	//Vector3D wr = n * 2 * dot(n, wi) - wi;
	//Vector3D r = kd*std::max(dot(wi,n),0.001) + ks * pow(std::max(dot(wo, wr), 0.0001), shine);
	Vector3D r = kd * dot(wi, n);
	return r;
}
