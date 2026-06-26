#ifndef MATHHELPER_H
#define MATHHELPER_H

// Auto-generated from MathHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

class MathHelper {
public:
	MathHelper();
	virtual ~MathHelper();

	static float sin(float var0);
	static float cos(float var0);
	static float sqrt_float(float var0);
	static int floor_float(float var0);
	static int floor_double(double var0);
	static float abs(float var0);

private:
	static float* s_SIN_TABLE;

};

#endif // MATHHELPER_H