#include "directshader.h"
#include "../core/utils.h"
#include "../lightsources/pointlightsource.h"

DirectShader::DirectShader( Vector3D bgColor_)
{
	this->bgColor = bgColor_;

}

void RGBtoHSVDir(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
	float fCMax = std::max(std::max(fR, fG), fB);
	float fCMin = std::min(std::min(fR, fG), fB);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == fR) {
			fH = 60 * (fmod(((fG - fB) / fDelta), 6));
		}
		else if (fCMax == fG) {
			fH = 60 * (((fB - fR) / fDelta) + 2);
		}
		else if (fCMax == fB) {
			fH = 60 * (((fR - fG) / fDelta) + 4);
		}

		if (fCMax > 0) {
			fS = fDelta / fCMax;
		}
		else {
			fS = 0;
		}

		fV = fCMax;
	}
	else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0) {
		fH = 360 + fH;
	}
}
void HSVtoRGBDir(float& fR, float& fG, float& fB, float& fH, float& fS, float& fV) {
	float fC = fV * fS; // Chroma
	float fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;
}

// Loop scatter computation
float DirectShader::loopScatter(const Ray &r, const std::vector<Shape*> &objList, double smooth, double radius, int currObj) const
{
	int num_it = 20;
	Intersection its;
	double min_t = r.minT;
	double total_scat_factor = 0;

	bool stop = false;
	double scat_factor = 0;

	int newObj = Utils::getClosestObject(r, objList, its);

	Vector3D p = its.itsPoint;
	Vector3D wo = (r.o - p).normalized();

	while (newObj != -1 && !stop)
	{
		Vector3D n = its.normal;

		if (!its.shape->getMaterial().hasScatter())
		{
			newObj = currObj; // if we find a different material item, we want to compute the smoke that we have until now and then leave, so we force it to enter the condition to compute smoke
		}
		if (currObj == newObj)
		{
			double t_size;
			// beggining and the end in N parts of size t_size
			t_size = (r.maxT - min_t) / (2 * radius);

			int subdiv = floor(t_size * smooth);	// number of subdivisions of size t_size needed to achieve the leaving point (floored)

			for (int i = 0; i < num_it; i++)
			{
				double scat_fact_iter = 0;
				for (int j = 0; j < subdiv; ++j)
				{
					if (objList.at(currObj)->getMaterial().hasPassed())			// use a binary probability distribution to determine if the ray has "passed" a subdivision
					{
						// if it passes all subdivisions, the ray follow its path and returns the color it would return normally.
					}
					else
					{
						scat_fact_iter += 1 / double(smooth);
					}
				}
				scat_factor += scat_fact_iter;
			}
			total_scat_factor += scat_factor / num_it;
			scat_factor = 0;
			r.minT = min_t + 0.0000001;
			r.maxT = INFINITY;

			newObj = Utils::getClosestObject(r, objList, its);	// when the scatter factor has been computed, go back to the starting point and find the next intersection, and decide what to do
			while (newObj != -1)
			{
				Vector3D n = its.normal;

				if (!its.shape->getMaterial().hasScatter())		// if we are in another material, leave this
				{
					stop = true;
					break;
				}
				else if (newObj != currObj && dot(wo, n) < 0)	// if we find a leaving sphere, ignore it
				{
					r.minT = r.maxT + 0.00000001;
					r.maxT = INFINITY;
					newObj = Utils::getClosestObject(r, objList, its);
				}
				else if (newObj != currObj && dot(wo, n) >= 0)	// if we find an entering sphere, set its beggining point and go back to the beggining of the loop
				{
					min_t = r.maxT;
					r.minT = min_t + 0.00000001;
					r.maxT = INFINITY;
					currObj = newObj;
					radius = its.shape->getRadius();
					break;
				}
				else
				{
					r.minT = r.maxT + 0.00000001;
					r.maxT = INFINITY;
					stop = true;
					break;
				}


			}

		}
		else if (currObj != newObj)	// if we find anoter object ignore it
		{
			r.minT = r.maxT + 0.00000001;
			r.maxT = INFINITY;
		}

		if (!stop)
			newObj = Utils::getClosestObject(r, objList, its);
	}

	return total_scat_factor;
}

// Recursive scatter computation
float DirectShader::recursiveScatter(const Ray &r, const std::vector<Shape*> &objList, double smooth, double radius, int currObj, int depth) const
{
	//float scat_factor = r.scatter_depth;
	double scat_factor = 0;
	int num_it = 30;
	double min_t = r.minT;
	double inside_scat_factor = 0;
	int newObj = -1;
	double start_t = 0;
	double end_t = 0;
	double aux_end = 0;
	if (depth == 2)
		int a = 1;
	while (true)
	{
		Intersection its;

		newObj = Utils::getClosestObject(r, objList, its);	// get closest object index

		if (newObj > -1)		// 1st intersection will be the leaving point of the "smoke" object
		{														// such that minT = current point dist,  maxT = leaving point dist
			Vector3D p = its.itsPoint;
			Vector3D n = its.normal;
			Vector3D wo = (r.o - p).normalized();

			if (!its.shape->getMaterial().hasScatter())
			{
				newObj = currObj;	// to ensure that it enters the equal object condition
				r.maxT -= 0.0000001;	// since afterward the r.min is set to r.max, we redefine it a bit before, so the next intersection it gets is the same of the non scatter object just found
			}
			if (dot(wo, n) >= 0 && currObj != newObj)	// if it finds a new sphere, set the new ray origin and end
			{
				r.minT = r.maxT + 0.00000001;
				r.maxT = INFINITY;
				double new_radius = its.shape->getRadius();		// then call this function again with the new object as current object and its respective radius
				inside_scat_factor += recursiveScatter(r, objList, smooth, new_radius, newObj, depth + 1);
				aux_end = r.maxT;	// this will be the maximum t found (the leaving point of the smoke or intersected spheres)
			}
			else if (currObj == newObj)
			{
				if (start_t != 0)		// if it has found any "leaving point" of a different sphere, set the end of this particular sphere
					end_t = r.maxT + 0.00000001;

				double t_size;
				t_size = (r.maxT - min_t) / (2 * radius);	// t_size will be in range [0,1], since it is normalized by the radius

				int subdiv = floor(t_size * smooth);	// number of subdivisions of size t_size needed to achieve the leaving point (floored)

				if (objList.at(currObj)->getMaterial().hasScatter())
				{
					for (int i = 0; i < num_it; i++)	// this loop iterats proportional times to the t_size previously obtained and also iterates
					{									// an amount of times to avoid noisy results.
						double scat_fact_iter = 0;
						for (int j = 0; j < subdiv; ++j)	// the more we iterate here, the more scatter effect we will obtain.
						{
							if (objList.at(currObj)->getMaterial().hasPassed())			// use a binary probability distribution to determine if the ray has "passed" a subdivision
							{
								// if it passes all subdivisions, the ray follow its path and returns the color it would return normally.
							}
							else
							{
								scat_fact_iter += 1 / double(smooth);
							}
						}
						scat_factor += scat_fact_iter;
					}
					scat_factor = scat_factor / num_it + inside_scat_factor;	// do the mean of the "repeated" iterations
					break;
				}
			}
			else		// if we are in a leaving point of a different sphere
			{
				if (start_t == 0)
					start_t = r.maxT - 0.00000001;

				r.minT = r.maxT + 0.00000001;
				r.maxT = INFINITY;
			}
		}
		else
			break;
	}

	if (start_t == 0)		// if we have not found any leaving point of a different sphere
	{
		r.minT = r.maxT + 0.00000001;		// when the ray leaves, we set the origin at the leaving point, and the next intersection distance "Unknown"
		r.maxT = INFINITY;		// since we still need to compute it
	}
	else
	{
		r.minT = start_t;	// if we have foudn a leaving point of a different sphere, set the beggining of the ray a bit before it in order to detect it as the closest intersection
		r.maxT = end_t;		// the same with end
	}
	if (aux_end < end_t)	// update to have the biggest finishing point
		aux_end = end_t;

	if (aux_end != 0 && aux_end < 50)
	{
		if (depth == 0)			// only if it is the first bounce of scatter set the begggining of the next ray to the end
		{
			r.minT = aux_end;
			r.maxT = INFINITY;
		}
		else
			r.maxT = aux_end;	// otherwise, update it to the previous recursiveScatter function
	}
	return scat_factor;		// this value is a factor [0,1] that indicates the amount of scatter that we will see

}

Vector3D DirectShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<PointLightSource> &lsList) const
{
	Intersection its;

	int currObj = Utils::getClosestObject(r, objList, its);
	if (currObj > -1)
	{
		
		Vector3D p = its.itsPoint;
		Vector3D n = its.normal;
		Vector3D wo = (r.o - p).normalized();
		Vector3D l_o;

		if (its.shape->getMaterial().hasSpecular())		//For mirrors
		{
			Vector3D wr = (n * 2 * dot(n, wo) - wo).normalized();
			Ray refl_ray = Ray(its.itsPoint, wr, r.depth);
			l_o += computeColor(refl_ray, objList, lsList);
		}
		else if (its.shape->getMaterial().hasTransmission())		//For transmissions
		{
			double n2 = its.shape->getMaterial().getIndexOfRefraction();
			double n1 = 1;
			double nt;


			if (dot(wo, n) >= 0)
			{
				nt = n2 / n1;
			}
			else
			{
				nt = n1 / n2;
				n = -n;
			}
			double cosA = dot(wo, n);
			double radicand = (1 + pow(nt, 2)*(pow(cosA, 2) - 1));
			if (radicand >= 0)	//Transmission: Refraction
			{
				double cosB = sqrt(radicand);
				Vector3D wt = (-wo * nt + n * (nt*cosA - cosB)).normalized();
				Ray refr_ray = Ray(its.itsPoint, wt, r.depth);
				l_o += computeColor(refr_ray, objList, lsList);
			}

			else	//Transmission: Reflection
			{
				Vector3D wr = (n * 2 * dot(n, wo) - wo).normalized();
				Ray refl_ray = Ray(its.itsPoint, wr, r.depth);
				l_o += computeColor(refl_ray, objList, lsList);
			}
			
		}
		else if (its.shape->getMaterial().hasDiffuseOrGlossy())
		{
			for (int s = 0; s < lsList.size(); s++) {

				//Ray r_light = Ray(its.itsPoint, Vector3D(lsList.at(s).getPosition() - its.itsPoint), 0Ui64, 0.0001, Vector3D(lsList.at(s).getPosition() - its.itsPoint).length());
				Vector3D origen = its.itsPoint;
				Vector3D direction = Vector3D(lsList.at(s).getPosition() - its.itsPoint).normalized();
				double max_t = Vector3D(lsList.at(s).getPosition() - its.itsPoint).length();
				Ray r_light = Ray(origen, direction, 0Ui64, 0.0001, max_t);

				if (!Utils::hasIntersection(r_light, objList))
				{
					Vector3D wi = (lsList.at(s).getPosition() - p).normalized();
					if (its.shape->getMaterial().hasSpecular())
					{
						Vector3D wr = (n * 2 * dot(n, wo) - wo);
						Ray refl_ray = Ray(its.itsPoint, wr, r.depth + 1);
						l_o += computeColor(refl_ray, objList, lsList);
					}
					else if (its.shape->getMaterial().hasDiffuseOrGlossy())
					{
						Vector3D l_i = lsList.at(s).getIntensity(p);
						Vector3D r = its.shape->getMaterial().getReflectance(n, wo, wi);
						l_o += Utils::multiplyPerCanal(l_i, r);
					}
				}
			}
		}
		else if (its.shape->getMaterial().hasScatter() && r.depth == 0)
		{
			double smooth = 30;
			double radius = its.shape->getRadius();
			Vector3D color;

			r.minT = r.maxT + 0.000000001;
			r.maxT = INFINITY;
			double scat_fact = 0;

			if (dot(its.normal, wo) >= 0)
			{
				color = its.shape->getMaterial().smokeColor();
				scat_fact = loopScatter(r, objList, smooth, radius, currObj);	// choose between loop or recursive algorithm
				//scat_fact = recursiveScatter(r, objList, smooth, radius, currObj, 0);
			}

			l_o = computeColor(r, objList, lsList);

			// VERSION 1 ("white smoke")
			//l_o += color * scat_fact;

			// VERSION 2 (achieve gray smoke, not working very well yet, pending to check for the presentation)
			float h;
			float s;
			float v;
			float r = l_o.x;
			float g = l_o.y;
			float b = l_o.z;
			RGBtoHSVDir(r, g, b, h, s, v);
			s = std::max(0.0, s - 0.4*scat_fact);
			//v = std::max((v*(1-scat_fact)),0.05);
			HSVtoRGBDir(r, g, b, h, s, v);
			l_o = Vector3D(r, g, b);

		}
		return l_o;
	}
	else
	{
		return bgColor;
	}
}