#include "depthshader.h"
#include "../core/utils.h"

DepthShader::DepthShader(Vector3D color_, double maxDist_, Vector3D bgColor_)
{
	this->color = color_;
	this->maxDist = maxDist_;
	this->bgColor = bgColor_;
}

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;
	if (Utils::getClosestIntersection(r, objList, its))
	{
		
		if ((its.itsPoint - r.o).length() >= this->maxDist)
			return this->bgColor;
		else
			//std::cout << "Obj dist: " << (its.itsPoint - r.o).length() << std::endl;
			//std::cout << "Max dist: " << this->maxDist << "\n\n";
			return this->color * (1.0 - (its.itsPoint - r.o).length() / maxDist);
	}
	else
		return bgColor;
}