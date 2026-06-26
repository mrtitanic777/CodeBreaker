#include "stdafx.h"
// Hand-written PORT of RenderFish.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Real render code: the fishing bobber (a single particle-sheet quad,
// billboarded toward the camera) plus the fishing line drawn as a
// 16-segment Tesselator line strip from the player's hand to the bobber.
// Ported from the converter output (output_alpha/RenderFish.cpp), fixed up
// against Alpha's RenderFish.h and the working indev RenderArrow / the Alpha
// handwritten RenderPlayer sibling.
//
// Key fixes vs the raw conversion (~62 errors):
//   * Added the missing type includes (RenderManager, EntityPlayer,
//     GameSettings, Entity) -- the converter only forward-declared them, so
//     every '->m_field_...' was "use of undefined type".
//   * doRender called the non-existent m_a(); the real virtual is a().
//   * Tesselator::getInstance() returns a REFERENCE and MUST NOT be copied by
//     value (the vertex buffer is ~6 MB -> stack overflow). Bound by ref.
//   * Tesselator vertex / vertexUV take floats, not doubles -- args cast.
//   * Fixed the texture path escape ("\C" / "\p" -> "\\") and actually BIND
//     the bobber texture (converter only LOADED it, same bug class as the
//     arrow), then restore the terrain texture for the line pass.
//   * Matrix composition matches the Alpha RenderPlayer sibling
//     (post-multiply: world * local) for the billboard transforms.

#include "RenderFish.h"
#include "Common.h"
#include "Entity.h"
#include "EntityFish.h"
#include "EntityPlayer.h"
#include "RenderManager.h"
#include "GameSettings.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

RenderFish::RenderFish()
{
}

RenderFish::~RenderFish() {
	// No dynamic resources
}

void RenderFish::a(EntityFish* var1, double var2, double var4, double var6, float var8, float var9) {
	D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();

	// Move to the bobber's world position and shrink to particle scale.
	{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var2, (float)var4, (float)var6);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
	// Normalize is shader-based on Xbox 360 (was: glEnable(GL_RESCALE_NORMAL))
	{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.5F, 0.5F, 0.5F);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

	unsigned char var10 = 1;
	unsigned char var11 = 2;
	// Bind the particle sheet (the bobber sprite lives at tile 1,2).
	g_pd3dDevice->SetTexture(0, Textures::getById(Textures::load("game:\\Content\\particles.png")));
	Tesselator& var12 = Tesselator::getInstance();
	float var13 = (float)(var10 * 8 + 0) / 128.0F;
	float var14 = (float)(var10 * 8 + 8) / 128.0F;
	float var15 = (float)(var11 * 8 + 0) / 128.0F;
	float var16 = (float)(var11 * 8 + 8) / 128.0F;
	float var17 = 1.0F;
	float var18 = 0.5F;
	float var19 = 0.5F;

	// Billboard the bobber quad toward the camera (yaw then pitch).
	{ D3DXVECTOR3 _ax(0.0F, 1.0F, 0.0F); D3DXMATRIX _r;
	  D3DXMatrixRotationAxis(&_r, &_ax, (180.0F - this->renderManager->m_field_1225_i) * DEG_TO_RAD);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
	{ D3DXVECTOR3 _ax(1.0F, 0.0F, 0.0F); D3DXMATRIX _r;
	  D3DXMatrixRotationAxis(&_r, &_ax, (-this->renderManager->m_field_1224_j) * DEG_TO_RAD);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

	var12.init();
	var12.normal(0.0F, 1.0F, 0.0F);
	var12.vertexUV(0.0F - var18, 0.0F - var19, 0.0F, var13, var16);
	var12.vertexUV(var17 - var18, 0.0F - var19, 0.0F, var14, var16);
	var12.vertexUV(var17 - var18, 1.0F - var19, 0.0F, var14, var15);
	var12.vertexUV(0.0F - var18, 1.0F - var19, 0.0F, var13, var15);
	var12.flush();
	// Normalize is shader-based on Xbox 360 (was: glDisable(GL_RESCALE_NORMAL))
	Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();

	if(var1->m_field_4097_b != NULL) {
		EntityPlayer* var50 = var1->m_field_4097_b;
		float var20 = (var50->prevRotationYaw + (var50->rotationYaw - var50->prevRotationYaw) * var9) * 3.14159265358979323846f / 180.0F;
		float var21 = (var50->prevRotationPitch + (var50->rotationPitch - var50->prevRotationPitch) * var9) * 3.14159265358979323846f / 180.0F;
		double var22 = (double)sinf(var20);
		double var24 = (double)cosf(var20);
		double var26 = (double)sinf(var21);
		double var28 = (double)cosf(var21);
		double var30 = var50->prevPosX + (var50->posX - var50->prevPosX) * (double)var9 - var24 * 0.7 - var22 * 0.5 * var28;
		double var32 = var50->prevPosY + (var50->posY - var50->prevPosY) * (double)var9 - var26 * 0.5;
		double var34 = var50->prevPosZ + (var50->posZ - var50->prevPosZ) * (double)var9 - var22 * 0.7 + var24 * 0.5 * var28;
		if(renderManager->m_options->m_thirdPersonView) {
			var20 = (var50->m_field_734_o + (var50->m_field_735_n - var50->m_field_734_o) * var9) * 3.14159265358979323846f / 180.0F;
			var22 = (double)sinf(var20);
			var24 = (double)cosf(var20);
			var30 = var50->prevPosX + (var50->posX - var50->prevPosX) * (double)var9 - var24 * 0.35 - var22 * 0.85;
			var32 = var50->prevPosY + (var50->posY - var50->prevPosY) * (double)var9 - 0.45;
			var34 = var50->prevPosZ + (var50->posZ - var50->prevPosZ) * (double)var9 - var22 * 0.35 + var24 * 0.85;
		}

		double var36 = var1->prevPosX + (var1->posX - var1->prevPosX) * (double)var9;
		double var38 = var1->prevPosY + (var1->posY - var1->prevPosY) * (double)var9 + 0.25;
		double var40 = var1->prevPosZ + (var1->posZ - var1->prevPosZ) * (double)var9;
		double var42 = (double)((float)(var30 - var36));
		double var44 = (double)((float)(var32 - var38));
		double var46 = (double)((float)(var34 - var40));

		// Fishing line: untextured black 16-segment strip from hand to bobber.
		g_pd3dDevice->SetTexture(0, NULL);
		// Lighting is shader-based on Xbox 360 (disabled)
		var12.init(/* PORT: GL line-strip mode */ 3);
		var12.colorI(0);
		unsigned char var48 = 16;

		for(int var49 = 0; var49 <= var48; ++var49) {
			float var51 = (float)var49 / (float)var48;
			var12.vertex(
				(float)(var2 + var42 * (double)var51),
				(float)(var4 + var44 * (double)(var51 * var51 + var51) * 0.5 + 0.25),
				(float)(var6 + var46 * (double)var51));
		}

		var12.flush();
		// Lighting is shader-based on Xbox 360 (enabled)
		g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: restore active world texture */);
	}
}

void RenderFish::doRender(Entity* var1, double var2, double var4, double var6, float var8, float var9) {
	a((EntityFish*)var1, var2, var4, var6, var8, var9);
}
