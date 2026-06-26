#ifndef VEC3D_H
#define VEC3D_H

// Auto-generated from Vec3D.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>
#include <vector>

class Vec3D {
public:
	Vec3D();
	Vec3D(double var1, double var3, double var5);
	virtual ~Vec3D();

	double xCoord;
	double yCoord;
	double zCoord;

	static Vec3D* createVectorHelper(double var0, double var2, double var4);
	static void initialize();
	static Vec3D* createVector(double var0, double var2, double var4);
	virtual Vec3D* func_1262_a(Vec3D* var1);
	virtual Vec3D* normalize();
	virtual Vec3D* crossProduct(Vec3D* var1);
	virtual Vec3D* addVector(double var1, double var3, double var5);
	virtual double distanceTo(Vec3D* var1);
	virtual double squareDistanceTo(Vec3D* var1);
	virtual double squareDistanceTo(double var1, double var3, double var5);
	virtual double lengthVector();
	virtual Vec3D* getIntermediateWithXValue(Vec3D* var1, double var2);
	virtual Vec3D* getIntermediateWithYValue(Vec3D* var1, double var2);
	virtual Vec3D* getIntermediateWithZValue(Vec3D* var1, double var2);
	virtual std::string toString();
	virtual void rotateAroundX(float var1);
	virtual void rotateAroundY(float var1);

public: // was private
	static std::vector<void*> /* PORT: determine element type */ s_vectorList;
	static int s_nextVector;

	Vec3D* setComponents(double var1, double var3, double var5);

};

#endif // VEC3D_H