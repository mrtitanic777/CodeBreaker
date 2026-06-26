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

private:
	static float* /* PORT: was FloatBuffer */ s_colorBuffer;

	float* /* PORT: was FloatBuffer */ setColorBuffer(float var0, float var1, float var2, float var3);

};

#endif // RENDERHELPER_H