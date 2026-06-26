#ifndef MATHHELPER_H
#define MATHHELPER_H

// Auto-generated from MathHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

class MathHelper {
public:
	MathHelper();
	virtual ~MathHelper();
	static void initStatics();  // run once at startup (was Java static {})

	static float sin(float var0);
	static float cos(float var0);
	static float sqrt_float(float var0);
	static float sqrt_double(double var0);
	static int floor_float(float var0);
	static int floor_double(double var0);
	static float abs(float var0);
	static double abs_max(double var0, double var2);
	static int bucketInt(int var0, int var1);

public: // was private
	static float* s_SIN_TABLE;

};

#endif // MATHHELPER_H