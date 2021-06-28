#ifndef DIRECTSHADER_H
#define DIRECTSHADER_H

#include "shader.h"
#include <algorithm>

class DirectShader : public Shader
{
public:
	DirectShader() = delete;
	DirectShader(Vector3D bgColor_);

	float loopScatter(const Ray & r, const std::vector<Shape*>& objList, double smooth, double radius, int currObj) const;
	float recursiveScatter(const Ray & r, const std::vector<Shape*>& objList, double smooth, double radius, int currObj, int depth) const;

	virtual Vector3D computeColor(const Ray &r,
							const std::vector<Shape*> &objList,
							const std::vector<PointLightSource> &lsList) const;

private:
	// Used to store the value of the maximum distance which can be visualized
	
};
#endif // DIRECTSHADER_H