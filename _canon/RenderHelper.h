#ifndef RENDERHELPER_H
#define RENDERHELPER_H

// Auto-generated from RenderHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class FloatBuffer;

class RenderHelper {
public:
	virtual ~RenderHelper();

	static FloatBuffer* s_colorBuffer;

	static void disableStandardItemLighting();
	static void enableStandardItemLighting();
	static FloatBuffer* setColorBufferDouble(double var0, double var2, double var4, double var6);
	static FloatBuffer* setColorBuffer(float var0, float var1, float var2, float var3);

	static void initStatics();
};

#endif // RENDERHELPER_H