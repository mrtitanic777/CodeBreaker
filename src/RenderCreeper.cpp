#include "stdafx.h"
// Auto-generated from RenderCreeper.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderCreeper.h"
#include "Common.h"
#include "EntityLiving.h"
#include "EntityCreeper.h"
#include "Shaders.h"

RenderCreeper::RenderCreeper()
	: RenderLiving(new ModelCreeper()
{
}

RenderCreeper::~RenderCreeper() {
	// No dynamic resources
}

void RenderCreeper::preRenderCallback(EntityLiving* var1, float var2) {
		EntityCreeper var4 = (EntityCreeper*)var1;
		float var5 = var4.c(var2);
		var2 = 1.0F + sinf(var5 * 100.0F) * var5 * 0.01F;
		if(var5 < 0.0F) {
			var5 = 0.0F;
		}

		if(var5 > 1.0F) {
			var5 = 1.0F;
		}

		var5 *= var5;
		var5 *= var5;
		float var3 = (1.0F + var5 * 0.4F) * var2;
		var5 = (1.0F + var5 * 0.1F) / var2;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, var3, var5, var3);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

int RenderCreeper::getColorMultiplier(EntityLiving* var1, float var2, float var3) {
		EntityCreeper var4 = (EntityCreeper*)var1;
		float var5 = var4.c(var3);
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

			return var6 << 24 | 16711680 | '\uff00' | 255;
		}
}
