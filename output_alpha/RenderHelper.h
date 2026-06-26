#ifndef RENDERHELPER_H
#define RENDERHELPER_H

// Auto-generated from RenderHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

class RenderHelper {
public:
	RenderHelper();
	virtual ~RenderHelper();

	static void disableStandardItemLighting();
	static void enableStandardItemLighting();

public: // was private
	static float* /* PORT: was FloatBuffer */ s_field_1695_a;

	float* /* PORT: was FloatBuffer */ func_1157_a(double var0, double var2, double var4, double var6);
	float* /* PORT: was FloatBuffer */ func_1156_a(float var0, float var1, float var2, float var3);

};

#endif // RENDERHELPER_H