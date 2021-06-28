#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <vector>
#include <algorithm>
#include <time.h>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "core/hemisphericalsampler.h"

#include "shapes/sphere.h"
#include "shapes/infiniteplane.h"
#include "shapes/triangle.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/normalshader.h"
#include "shaders/directshader.h"
#include "shaders/globalshader.h"

#include "materials/material.h"
#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/transmissive.h"
#include "materials/smoke.h"


void buildSceneCornellBox(Camera* &cam, Film* &film,
	std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
	/* **************************** */
	/* Declare and place the camera */
	/* **************************** */
	Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
	double fovDegrees = 60;
	double fovRadians = Utils::degreesToRadians(fovDegrees);
	cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

	/* ********* */
	/* Materials */
	/* ********* */
	Material *redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
	Material *greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
	Material *greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);
	Material *yellowDiffuse = new Phong(Vector3D(0.8, 0.8, 0.1), Vector3D(0, 0, 0), 100);
	Material *blueDiffuse = new Phong(Vector3D(0.3, 0.2, 0.7), Vector3D(0, 0, 0), 100);
	Material *transmissive = new Transmissive(1.1, Vector3D(1));
	Material *mirror = new Mirror(Vector3D(1, 0.9, 0.85));
	Material *red_smoke = new Smoke(0.2, Vector3D(1.5, 0, 0));
	Material *green_smoke = new Smoke(0.2, Vector3D(0, 1.5, 0));
	Material *blue_smoke = new Smoke(0.2, Vector3D(0, 0, 1.5));
	Material *white_smoke = new Smoke(0.2, Vector3D(0.5, 0.5, 0.5));
	Material *red_100 = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0.7, 0.7, 0.2), 100);

	/* ******* */
	/* Objects */
	/* ******* */
	objectsList = new std::vector<Shape*>;
	double offset = 3.0;
	Matrix4x4 idTransform;
	double randomX = (double(rand()) / RAND_MAX) * 2 - 1;
	double randomY = (double(rand()) / RAND_MAX) * 2 - 1;
	double randomZ = (double(rand()) / RAND_MAX) * 2 - 1;
	
	// Construct the Cornell Box
	Shape *leftPlan = new InfinitePlane(Vector3D(-offset, 0, 0), Vector3D(1, 0, 0), redDiffuse);
	Shape *rightPlan = new InfinitePlane(Vector3D(offset, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
	Shape *topPlan = new InfinitePlane(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
	Shape *bottomPlan = new InfinitePlane(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), yellowDiffuse);
	Shape *backPlan = new InfinitePlane(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), blueDiffuse);
	objectsList->push_back(leftPlan);
	objectsList->push_back(rightPlan);
	objectsList->push_back(topPlan);
	objectsList->push_back(bottomPlan);
	objectsList->push_back(backPlan);
	
	// Place the Triangles
	Shape *triangleLeft = new Triangle(Vector3D(-2.8, 1.8, 0), Vector3D(-2.8, 1.8, 9), Vector3D(-2.8, -1, 4.5), greenDiffuse);
	Shape *triangleRight = new Triangle(Vector3D(2.8, 1.8, 0), Vector3D(2.8, -1, 4.5), Vector3D(2.8, 1.8, 9), blueDiffuse);
	Shape *triangleBack = new Triangle(Vector3D(2.8, 1.8, 8.8), Vector3D(0, -1, 8.8), Vector3D(-2.8, 1.8, 8.8), red_100);
	objectsList->push_back(triangleLeft);
	objectsList->push_back(triangleRight);
	objectsList->push_back(triangleBack);

	// Place the Spheres inside the Cornell Box
	
	//OTHER SPHERES
	Matrix4x4 sphereTransform1;
	double radius = 1;
	sphereTransform1 = Matrix4x4::translate(Vector3D(-offset + radius, -offset + radius, 4.5));
	Shape *s1 = new Sphere(1.5, sphereTransform1, mirror);
	Matrix4x4 sphereTransform2;
	radius = 0.05;
	sphereTransform2 = Matrix4x4::translate(Vector3D(1.0, -1.2, 1));
	Shape *s2 = new Sphere(1, sphereTransform2, transmissive);
	
	//SMOKE SPHERES (this ones are not being pushed back to the object list)
	Matrix4x4 sphereTransform3;
	radius = 1;
	sphereTransform3 = Matrix4x4::translate(Vector3D(0 + randomX, 0 + randomY, 5 + randomZ));
	Shape *s3 = new Sphere(radius, sphereTransform3, white_smoke);
	
	Matrix4x4 sphereTransform3_1;
	radius = 1;
	sphereTransform3_1 = Matrix4x4::translate(Vector3D(-0.5 + randomX, 0 + randomY,5.5 + randomZ));
	Shape *s3_1 = new Sphere(radius, sphereTransform3_1, white_smoke);

	Matrix4x4 sphereTransform3_2;
	radius = 1;
	sphereTransform3_2 = Matrix4x4::translate(Vector3D(0.5 + randomX, 0 + randomY, 5 + randomZ));
	Shape *s3_2 = new Sphere(radius, sphereTransform3_2, white_smoke);
	
	Matrix4x4 sphereTransform4;
	radius = 0.75;
	sphereTransform4 = Matrix4x4::translate(Vector3D(0 + randomX, 0.75 + randomY, 5 + randomZ));
	Shape *s4 = new Sphere(radius, sphereTransform4, white_smoke);

	Matrix4x4 sphereTransform5;
	radius = 0.75;
	sphereTransform5 = Matrix4x4::translate(Vector3D(0 + randomX, -0.75 + randomY, 5 + randomZ));
	Shape *s5 = new Sphere(radius, sphereTransform5, white_smoke);
	
	Matrix4x4 sphereTransform6;
	radius = 0.5;
	sphereTransform6 = Matrix4x4::translate(Vector3D(-0.7 + randomX, 0.5 + randomY, 5 + randomZ));
	Shape *s6 = new Sphere(radius, sphereTransform6, white_smoke);

	Matrix4x4 sphereTransform8;
	radius = 1;
	sphereTransform8 = Matrix4x4::translate(Vector3D(0, -0.5, 3));
	Shape *s8 = new Sphere(radius, sphereTransform8, white_smoke);
	
	Matrix4x4 sphereTransform9;
	radius = 0.5;
	sphereTransform9 = Matrix4x4::translate(Vector3D(-0.3, -0.5, 2.2));
	Shape *s9 = new Sphere(radius, sphereTransform9, white_smoke);

	// This are the ones being pushed currently
	int num_layers = 5;
	Vector3D centroid = Vector3D(1, 0.5, 2.7);
	radius = 1;
	double density = 0.05;
	double dispersity = 1.5;
	for (int layer = 0; layer < num_layers; ++layer)
	{
		for (int sphere = 0; sphere < (layer + 1) * 3; ++sphere)
		{
			double randomX = (double(rand()) / RAND_MAX) * dispersity - dispersity / 2;
			double randomY = (double(rand()) / RAND_MAX) * dispersity - dispersity / 2;
			double randomZ = (double(rand()) / RAND_MAX) * dispersity - dispersity / 2;

			Matrix4x4 sphereTransform;
			sphereTransform = Matrix4x4::translate(centroid + Vector3D(randomX, randomY, randomZ));
			Sphere *s;
			s = new Sphere(radius, sphereTransform, white_smoke);

			objectsList->push_back(s);
		}
		radius /= 1.5;
	}

	Matrix4x4 sphereTransform7;
	radius = 0.5;
	sphereTransform7 = Matrix4x4::translate(Vector3D(0.5, 0.0, 3.3));
	Shape *s7 = new Sphere(radius, sphereTransform7, redDiffuse);

	objectsList->push_back(s7);
	objectsList->push_back(s1);
	objectsList->push_back(s2);
	/*objectsList->push_back(s3);
	objectsList->push_back(s3_1);
	objectsList->push_back(s3_2);
	objectsList->push_back(s4);
	objectsList->push_back(s5);
	objectsList->push_back(s6);
	objectsList->push_back(s7);
	objectsList->push_back(s8);
	objectsList->push_back(s9);*/

	/* ****** */
	/* Lights */
	/* ****** */
	lightSourceList = new std::vector<PointLightSource>;
	Vector3D lightPosition1 = Vector3D(0, offset - 1, 2 * offset);
	Vector3D lightPosition2 = Vector3D(0, offset - 1, 0);
	Vector3D lightPosition3 = Vector3D(0, offset - 1, offset);
	Vector3D intensity = Vector3D(10, 10, 10); // Radiant intensity (watts/sr)
	PointLightSource pointLS1(lightPosition1, intensity);
	PointLightSource pointLS2(lightPosition2, intensity);
	PointLightSource pointLS3(lightPosition3, intensity);
	lightSourceList->push_back(pointLS1);
	lightSourceList->push_back(pointLS2);
	lightSourceList->push_back(pointLS3);
}

void raytrace(Camera* &cam, Shader* &shader, Film* &film,
              std::vector<Shape*>* &objectsList, std::vector<PointLightSource>* &lightSourceList)
{
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for(size_t lin=0; lin<resY; lin++)
    {
        // Show progression
        if ( lin%(resY/sizeBar) == 0)
            std::cout << ".";

        // Inner loop invariant: we have rendered col columns
        for(size_t col=0; col<resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;

            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);

            // Compute ray color according to the used shader
            Vector3D pixelColor = shader->computeColor( cameraRay, *objectsList, *lightSourceList );

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RTIS - Ray Tracer for \"Imatge Sintetica\"" << separator << std::endl;

	srand(time(NULL));

    // Create an empty film
    Film *film;
    film = new Film(720, 576);


    // Declare the shader
    Vector3D bgColor(0.0, 0.0, 0.0); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);

    //Shader *shader = new IntersectionShader (intersectionColor, bgColor);
	//Shader *shader = new DepthShader(Vector3D(1, 0.4, 0.4), 6, bgColor);
	//Shader *shader = new NormalShader(bgColor);

	// SMOKE IMPLEMENTED FOR DIRECT AND GLOBAL SHADER

	//hader *shader = new DirectShader(bgColor);
	Shader *shader = new GlobalShader(bgColor);

    // Declare pointers to all the variables which describe the scene
    Camera *cam;
    std::vector<Shape*> *objectsList;
    std::vector<PointLightSource> *lightSourceList;

    // Build the scene
    //buildSceneSphere(cam, film, objectsList, lightSourceList);
	buildSceneCornellBox(cam, film, objectsList, lightSourceList);
    
	// Launch some rays!
    raytrace(cam, shader, film, objectsList, lightSourceList);

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();

    std::cout << "\n\n" << std::endl;
    return 0;
}
