#include "stdafx.h"
// Auto-generated from RenderCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderCreeper.h"
#include "Common.h"
#include "EntityCreeper.h"
#include "EntityLiving.h"
#include "Shaders.h"
#include "ModelCreeper.h"

RenderCreeper::RenderCreeper()
	: RenderLiving(new ModelCreeper(), 0.5f)
{
}

RenderCreeper::~RenderCreeper() {
	// No dynamic resources
}

void RenderCreeper::a(EntityCreeper* var1, float var2) {
		float var4 = var1->func_440_b(var2);
		float var5 = 1.0F + sinf(var4 * 100.0F) * var4 * 0.01F;
		if(var4 < 0.0F) {
			var4 = 0.0F;
		}

		if(var4 > 1.0F) {
			var4 = 1.0F;
		}

		var4 *= var4;
		var4 *= var4;
		float var6 = (1.0F + var4 * 0.4F) * var5;
		float var7 = (1.0F + var4 * 0.1F) / var5;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var6, var7, var6);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

int RenderCreeper::a(EntityCreeper* var1, float var2, float var3) {
		float var5 = var1->func_440_b(var3);
		if((int)(var5 * 10.0F) % 2 == 0) {
			return 0;
		} else {
			int var6 = (int)(var5 * 0.2F * 255.0F);
			if(var6 < 0) {
				var6 = 0;
			}

			if(var6 > 255) {
				var6 = 255;
			}

			short var7 = 255;
			short var8 = 255;
			short var9 = 255;
			return var6 << 24 | var7 << 16 | var8 << 8 | var9;
		}
}

void RenderCreeper::func_6330_a(EntityLiving* var1, float var2) {
		a((EntityCreeper*)var1, var2);
}

int RenderCreeper::func_173_a(EntityLiving* var1, float var2, float var3) {
		return a((EntityCreeper*)var1, var2, var3);
}
