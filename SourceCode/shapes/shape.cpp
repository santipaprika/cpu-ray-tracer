#include "shape.h"

Shape::Shape(const Matrix4x4 &t_, Material *material_)
{
    objectToWorld = t_;
    objectToWorld.inverse(worldToObject);
    material = material_;
}

const Material& Shape::getMaterial() const
{
    return *material;
}

double Shape::getRadius() const
{
	std::cout << "Warning! Calling \"Shape::getRadius()\" for a non-Sphere Shape"
		<< std::endl;

	return -1;
}

const void Shape::translate(Vector3D trans) const
{
	//std::cout << objectToWorld.data[0][3] << " " << objectToWorld.data[1][3] << " " << objectToWorld.data[2][3] << " " << objectToWorld.data[3][3] << std::endl;
	Matrix4x4 *t_;
	t_ = &Matrix4x4::translate(trans);
	Matrix4x4 *ptr;
	Matrix4x4 *ptr2;
	ptr = (Matrix4x4*)(&objectToWorld);
	ptr2 = (Matrix4x4*)(&worldToObject);
	ptr->data[0][3] += trans.x;
	ptr->data[1][3] += trans.y;
	ptr->data[2][3] += trans.z;
	
	ptr->inverse(*ptr2);
}