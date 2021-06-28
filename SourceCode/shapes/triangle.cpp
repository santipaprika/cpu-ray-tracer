#include "triangle.h"

Triangle::Triangle(const Vector3D &p0_, const Vector3D &p1_, const Vector3D &p2_,
         Material *mat_) :
    Shape(Matrix4x4(), mat_),
    p0(p0_), p1(p1_), p2(p2_)
{ 
	v01 = p1_ - p0_;
	v02 = p2_ - p0_;
	v12 = p2_ - p1_;
	v20 = p0_ - p2_;
	nWorld = cross(v01, v02).normalized();
}

Vector3D Triangle::getNormalWorld() const
{
    return nWorld;
}

bool Triangle::rayIntersectP(const Ray &rayWorld) const
{
	// Compute the denominator of the tHit formula
	float denominator = dot(rayWorld.d, nWorld);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon)
		return false;

	// Effectivelly compute the intersection point
	float tHit = dot((p0 - rayWorld.o), nWorld) / denominator;

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < rayWorld.minT || tHit > rayWorld.maxT)
		return false;

	Vector3D its_point = rayWorld.o + rayWorld.d * tHit;
	Vector3D v0p = its_point - p0;
	Vector3D v1p = its_point - p1;
	Vector3D v2p = its_point - p2;

	if (dot(cross(v01, v0p).normalized(), nWorld) > 0)
	{
		if (dot(cross(v12, v1p).normalized(), nWorld) > 0)
		{
			if (dot(cross(v20, v2p).normalized(), nWorld) > 0)
			{
				return true;
			}
		}
	}

    return false;
}

bool Triangle::rayIntersect(const Ray &rayWorld, Intersection &its) const
{
	// Compute the denominator of the tHit formula
	float denominator = dot(rayWorld.d, nWorld);

	// If the denominator is very close to zero, then the ray and the
	// plane are almost parallel
	if (std::abs(denominator) < Epsilon)
		return false;

	// Effectivelly compute the intersection point
	float tHit = dot((p0 - rayWorld.o), nWorld) / denominator;

	// Is tHit outside the bounds of the ray segment we want to test intersecion with?
	if (tHit < rayWorld.minT || tHit > rayWorld.maxT)
		return false;

	Vector3D its_point = rayWorld.o + rayWorld.d * tHit;
	Vector3D v0p = (its_point - p0);
	Vector3D v1p = (its_point - p1);
	Vector3D v2p = (its_point - p2);

	if (dot(cross(v01, v0p).normalized(), nWorld) > 0)
	{
		if (dot(cross(v12, v1p).normalized(), nWorld) > 0)
		{
			if (dot(cross(v20, v2p).normalized(), nWorld) > 0)
			{
				// Fill the intersection details
				its.itsPoint = its_point;
				its.normal = nWorld;
				its.shape = this;

				// Update the ray maxT
				rayWorld.maxT = tHit;
				return true;
			}
		}
	}

	return false;
    
}


std::string Triangle::toString() const
{
    std::stringstream s;
    s << "\tInfinitePlan attributes: \n";

    s << "\t p0 (world coordinates): " << p0 << "\n";
    s << "\t normal (world coordinates): " << nWorld << "\n" << std::endl;

    return s.str();
}

std::ostream &operator<<(std::ostream &out, const Triangle &t)
{
    out << t.toString();
    return out;
}
