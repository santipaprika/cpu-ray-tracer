#include "material.h"

#include <iostream>

Material::Material()
{ }


double Material::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}

Vector3D Material::getDiffuseCoefficient() const
{
	std::cout << "Warning!" << "Calling this for a non diffuse material!" << std::endl;
	return Vector3D(-1);
}

bool Material::hasPassed() const
{
	std::cout << "Warning! Calling \"Material::hasPassed()\" for a non-scatter material"
		<< std::endl;

	return -1;
}

Vector3D Material::smokeColor() const
{
	std::cout << "Warning!" << "Calling this for a non-scatter material!" << std::endl;
	return Vector3D(-1);
}

Vector3D Material::getReflectance(const Vector3D & n, const Vector3D & wo, const Vector3D & wi) const
{
	std::cout << "Warning!" << "Calling this for a non-reflectance material!" << std::endl;
	return Vector3D(-1);
}