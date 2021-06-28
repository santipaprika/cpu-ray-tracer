#ifndef GLOBALSHADER_H
#define GLOBALSHADER_H

#include "shader.h"


#include <math.h>
#include <iostream>
#include <random>
#include <string>
#include <iterator>
#include <algorithm>

#define _USE_MATH_DEFINES

class GlobalShader : public Shader
{
public:
	Vector3D a_t = Vector3D(0.1);
	int n_directions = 30;
	int n_bounces = 2;
	GlobalShader() = delete;
	GlobalShader(Vector3D bgColor_);

	float loopScatter(const Ray & r, const std::vector<Shape*>& objList, double smooth, double radius, int currObj) const;

	float recursiveScatter(const Ray & r, const std::vector<Shape*>& objList, double smooth, double radius, int currObj, int depth) const;

	virtual Vector3D computeColor(const Ray &r,
							const std::vector<Shape*> &objList,
							const std::vector<PointLightSource> &lsList) const;

private:
	// Used to store the value of the maximum distance which can be visualized
	
};
#endif // GlobalSHADER_H