#include "stdafx.h"
// Auto-generated from RenderHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderHelper.h"
#include "Common.h"
#include "Vec3.h"

float* /* PORT: was FloatBuffer */ RenderHelper::s_colorBuffer = new float[16];

RenderHelper::RenderHelper()
{
}

RenderHelper::~RenderHelper() {
	// No dynamic resources
}

void RenderHelper::disableStandardItemLighting() {
  // Lighting is shader-based on Xbox 360 (disabled)
  // Light 0 disabled � handle in shader constants
  // Light 1 disabled � handle in shader constants
  // Color material is shader-based on Xbox 360 (disabled)
}

void RenderHelper::enableStandardItemLighting() {
  // Lighting is shader-based on Xbox 360 (enabled)
  // Light 0 enabled � handle in shader constants
  // Light 1 enabled � handle in shader constants
  // Color material is shader-based on Xbox 360 (enabled)
  // Color material is shader-based on Xbox 360
		Vec3D var0 = new Vec3D(0.3F, 1.0F, -0.7F);
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
		var0 = new Vec3D(-0.7F, 1.0F, 0.2F);
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Shade model is shader-based on Xbox 360
  // Light model ambient � handled by shader constants
}

float* /* PORT: was FloatBuffer */ RenderHelper::setColorBuffer(float var0, float var1, float var2, float var3) {
  memset(s_colorBuffer, 0, sizeof(s_colorBuffer));
  s_colorBuffer[0] = var0; s_colorBuffer[1] = var1; s_colorBuffer[2] = var2; s_colorBuffer[3] = var3;
  // NIO flip() � no-op for C array
		return s_colorBuffer;
}
