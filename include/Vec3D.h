#ifndef VEC3D_H
#define VEC3D_H

// Auto-generated from Vec3D.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

struct Vec3D {
public:
	Vec3D();
	Vec3D(float var1, float var2, float var3);
	virtual ~Vec3D();

	float m_xCoord;
	float m_yCoord;
	float m_zCoord;

	virtual Vec3D subtract(const Vec3D& var1);
	virtual Vec3D normalize();
	virtual Vec3D addVector(float var1, float var2, float var3);
	virtual float distance(const Vec3D& var1);
	virtual float squareDistanceTo(const Vec3D& var1);
	virtual Vec3D getIntermediateWithXValue(const Vec3D& var1, float var2);
	virtual Vec3D getIntermediateWithYValue(const Vec3D& var1, float var2);
	virtual Vec3D getIntermediateWithZValue(const Vec3D& var1, float var2);
	virtual const std::string& toString();

};

#endif // VEC3D_H