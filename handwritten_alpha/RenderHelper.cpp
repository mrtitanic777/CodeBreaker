#include "stdafx.h"
// Auto-generated from RenderHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderHelper.h"
#include "Common.h"
#include "Vec3D.h"
#include "GLAllocation.h"

float* /* PORT: was FloatBuffer */ RenderHelper::s_field_1695_a = GLAllocation::createDirectFloatBuffer(16);

RenderHelper::RenderHelper()
{
}

RenderHelper::~RenderHelper() {
	// No dynamic resources
}

void RenderHelper::disableStandardItemLighting() {
  // Lighting is shader-based on Xbox 360 (disabled)
  // Light 0 disabled - handle in shader constants
  // Light 1 disabled - handle in shader constants
  // Color material is shader-based on Xbox 360 (disabled)
}

void RenderHelper::enableStandardItemLighting() {
  // Lighting is shader-based on Xbox 360 (enabled)
  // Light 0 enabled - handle in shader constants
  // Light 1 enabled - handle in shader constants
  // Color material is shader-based on Xbox 360 (enabled)
  // Color material is shader-based on Xbox 360
		float var0 = 0.4F;
		float var1 = 0.6F;
		float var2 = 0.0F;
		Vec3D* var3 = Vec3D::createVector((double)0.2F, 1.0f, (double)-0.7F)->normalize();
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
		var3 = Vec3D::createVector((double)-0.2F, 1.0f, (double)0.7F)->normalize();
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Lighting via shader constants on Xbox 360
  // Shade model is shader-based on Xbox 360
  // Light model ambient - handled by shader constants
}

float* /* PORT: was FloatBuffer */ RenderHelper::func_1157_a(double var0, double var2, double var4, double var6) {
		return func_1156_a((float)var0, (float)var2, (float)var4, (float)var6);
}

float* /* PORT: was FloatBuffer */ RenderHelper::func_1156_a(float var0, float var1, float var2, float var3) {
  memset(s_field_1695_a, 0, sizeof(float) * 16);
  s_field_1695_a[0] = var0; s_field_1695_a[1] = var1; s_field_1695_a[2] = var2; s_field_1695_a[3] = var3;
  // NIO flip() - no-op for C array
		return s_field_1695_a;
}
