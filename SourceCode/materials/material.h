#ifndef MATERIAL
#define MATERIAL

#include "../core/vector3d.h"

class Material
{
public:
    Material();

    virtual Vector3D getReflectance(const Vector3D &n, const Vector3D &wo,
                                    const Vector3D &wi) const;
    virtual bool hasSpecular() const = 0;
    virtual bool hasTransmission() const = 0;
    virtual bool hasDiffuseOrGlossy() const = 0;
	virtual bool hasScatter() const = 0;
	virtual bool hasPassed() const;
    virtual double getIndexOfRefraction() const;
	virtual Vector3D getDiffuseCoefficient() const;
	virtual Vector3D smokeColor() const;
};


#endif // MATERIAL
