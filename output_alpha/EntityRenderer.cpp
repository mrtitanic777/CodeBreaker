#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRenderer.h"
#include "Common.h"
#include "GuiScreen.h"
#include "GuiIngame.h"
#include "MouseHelper.h"
#include "InventoryPlayer.h"
#include "RenderEngine.h"
#include "GameSettings.h"
#include "Entity.h"
#include "ItemRenderer.h"
#include "Minecraft.h"
#include "Random.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "EffectRenderer.h"
#include "EntityPlayerSP.h"
#include "Material.h"
#include "MathHelper.h"
#include "MovingObjectPosition.h"
#include "RenderGlobal.h"
#include "RenderHelper.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Vec3D.h"
#include "World.h"
#include "ClippingHelperImplementation.h"
#include "EntityRainFX.h"
#include "Frustrum.h"
#include "GLAllocation.h"
#include "PlayerControllerTest.h"
#include "WorldProvider.h"

EntityRenderer::EntityRenderer()
	: mc(NULL)
	, m_field_1387_i(0.0f)
	, m_field_1395_a(NULL)
	, m_field_1386_j(0)
	, m_field_1385_k(NULL)
	, m_field_1384_l(GetTickCount())
	, m_field_1383_m(new Random())
	, m_field_1392_d(GLAllocation::createDirectFloatBuffer(16))
	, m_field_4270_e(0)
	, m_field_4269_f(0)
	, m_field_4268_g(0)
	, m_field_1382_n(0)
	, m_field_1381_o(0)
{
}

EntityRenderer::EntityRenderer(Minecraft* var1)
	: mc(NULL)
	, m_field_1387_i(0.0f)
	, m_field_1395_a(NULL)
	, m_field_1386_j(0)
	, m_field_1385_k(NULL)
	, m_field_1384_l(GetTickCount())
	, m_field_1383_m(new Random())
	, m_field_1392_d(GLAllocation::createDirectFloatBuffer(16))
	, m_field_4270_e(0)
	, m_field_4269_f(0)
	, m_field_4268_g(0)
	, m_field_1382_n(0)
	, m_field_1381_o(0)
{
		mc = var1;
		m_field_1395_a = new ItemRenderer(var1);
}

EntityRenderer::~EntityRenderer() {
	mc = NULL;
	m_field_1395_a = NULL;
	m_field_1385_k = NULL;
	m_field_1383_m = NULL;
}

void EntityRenderer::func_911_a() {
		m_field_1382_n = m_field_1381_o;
		float var1 = mc->theWorld->getLightBrightness((int)floor(mc->thePlayer->posX), (int)floor(mc->thePlayer->posY), (int)floor(mc->thePlayer->posZ));
		float var2 = (float)(3 - mc->m_gameSettings->m_renderDistance) / 3.0F;
		float var3 = var1 * (1.0F - var2) + var2;
		m_field_1381_o += (var3 - m_field_1381_o) * 0.1F;
		++m_field_1386_j;
		m_field_1395_a->func_895_a();
		if(mc->m_field_6288_M) {
			func_916_c();
		}

}

void EntityRenderer::func_910_a(float var1) {
		if(mc->thePlayer != NULL) {
			double var2 = (double)mc->m_field_6327_b->getBlockReachDistance();
			mc->m_objectMouseOver = mc->thePlayer->rayTrace(var2, var1);
			double var4 = var2;
			Vec3D* var6 = mc->thePlayer->getPosition(var1);
			if(mc->m_objectMouseOver != NULL) {
				var4 = mc->m_objectMouseOver->m_hitVec->distanceTo(var6);
			}

			if(dynamic_cast<PlayerControllerTest*>(mc->m_field_6327_b) != NULL) {
				var2 = 32.0f;
			} else {
				if(var4 > 3.0f) {
					var4 = 3.0f;
				}

				var2 = var4;
			}

			Vec3D* var7 = mc->thePlayer->getLook(var1);
			Vec3D* var8 = var6->addVector(var7->xCoord * var2, var7->yCoord * var2, var7->zCoord * var2);
			m_field_1385_k = NULL;
			float var9 = 1.0F;
			std::vector<void*> var10 = mc->theWorld->getEntitiesWithinAABBExcludingEntity(mc->thePlayer, mc->thePlayer->boundingBox.addCoord(var7->xCoord * var2, var7->yCoord * var2, var7->zCoord * var2).expands((double)var9, (double)var9, (double)var9));
			double var11 = 0.0f;

			for(size_t var13 = 0; var13 < var10.size(); ++var13) {
				Entity* var14 = (Entity*)var10[var13];
				if(var14->canBeCollidedWith()) {
					float var15 = var14->func_4035_j_();
					AxisAlignedBB var16 = var14->boundingBox.expands((double)var15, (double)var15, (double)var15);
					MovingObjectPosition* var17 = var16.func_1169_a(var6, var8);
					if(var16.isVecInside(var6)) {
						if(0.0f < var11 || var11 == 0.0f) {
							m_field_1385_k = var14;
							var11 = 0.0f;
						}
					} else if(var17 != NULL) {
						double var18 = var6->distanceTo(var17->m_hitVec);
						if(var18 < var11 || var11 == 0.0f) {
							m_field_1385_k = var14;
							var11 = var18;
						}
					}
				}
			}

			if(m_field_1385_k != NULL && !(dynamic_cast<PlayerControllerTest*>(mc->m_field_6327_b) != NULL)) {
				mc->m_objectMouseOver = new MovingObjectPosition(m_field_1385_k);
			}

		}
}

float EntityRenderer::func_914_d(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		float var3 = 70.0F;
		if(var2->isInsideOfMaterial(Material::s_water)) {
			var3 = 60.0F;
		}

		if(var2->health <= 0) {
			float var4 = (float)var2->deathTime + var1;
			var3 /= (1.0F - 500.0F / (var4 + 500.0F)) * 2.0F + 1.0F;
		}

		return var3;
}

void EntityRenderer::hurtCameraEffect(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		float var3 = (float)var2->hurtTime - var1;
		float var4;
		if(var2->health <= 0) {
			var4 = (float)var2->deathTime + var1;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (40.0F - 8000.0F / (var4 + 200.0F))*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		if(var3 >= 0.0F) {
			var3 /= (float)var2->m_field_9332_M;
			var3 = sinf(var3 * var3 * var3 * var3 * 3.14159265358979323846f);
			var4 = var2->m_field_9331_N;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var4)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var3 * 14.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var4)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}
}

void EntityRenderer::func_917_f(float var1) {
		if(!mc->m_gameSettings->m_thirdPersonView) {
			EntityPlayerSP* var2 = mc->thePlayer;
			float var3 = var2->m_field_9290_aS - var2->m_field_9291_aR;
			float var4 = var2->m_field_9290_aS + var3 * var1;
			float var5 = var2->m_field_775_e + (var2->m_field_774_f - var2->m_field_775_e) * var1;
			float var6 = var2->m_field_9329_Q + (var2->m_field_9328_R - var2->m_field_9329_Q) * var1;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, MathHelper::sin(var4 * (float)M_PI) * var5 * 0.5F, -fabsf(MathHelper::cos(var4 * (float)M_PI) * var5), 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (MathHelper::sin(var4 * (float)M_PI) * var5 * 3.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (fabsf(MathHelper::cos(var4 * (float)M_PI + 0.2F) * var5) * 5.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var6)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}
}

void EntityRenderer::orientCamera(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		double var3 = var2->prevPosX + (var2->posX - var2->prevPosX) * (double)var1;
		double var5 = var2->prevPosY + (var2->posY - var2->prevPosY) * (double)var1;
		double var7 = var2->prevPosZ + (var2->posZ - var2->prevPosZ) * (double)var1;
		if(mc->m_gameSettings->m_thirdPersonView) {
			double var9 = 4.0f;
			float var11 = var2->rotationYaw;
			float var12 = var2->rotationPitch;
   // PORT: XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
    // PORT-BLOCKED: var12 += 180.0F;
    // PORT-BLOCKED: var9 += 2.0;
   // PORT-BLOCKED: }

			double var13 = (double)(-sinf(var11 / 180.0F * 3.14159265358979323846f) * cosf(var12 / 180.0F * 3.14159265358979323846f)) * var9;
			double var15 = (double)(cosf(var11 / 180.0F * 3.14159265358979323846f) * cosf(var12 / 180.0F * 3.14159265358979323846f)) * var9;
			double var17 = (double)(-sinf(var12 / 180.0F * 3.14159265358979323846f)) * var9;

			for(int var19 = 0; var19 < 8; ++var19) {
				float var20 = (float)((var19 & 1) * 2 - 1);
				float var21 = (float)((var19 >> 1 & 1) * 2 - 1);
				float var22 = (float)((var19 >> 2 & 1) * 2 - 1);
				var20 *= 0.1F;
				var21 *= 0.1F;
				var22 *= 0.1F;
				MovingObjectPosition* var23 = mc->theWorld->rayTraceBlocks(Vec3D::createVector(var3 + (double)var20, var5 + (double)var21, var7 + (double)var22), Vec3D::createVector(var3 - var13 + (double)var20 + (double)var22, var5 - var17 + (double)var21, var7 - var15 + (double)var22));
				if(var23 != NULL) {
					double var24 = var23->m_hitVec->distanceTo(Vec3D::createVector(var3, var5, var7));
					if(var24 < var9) {
						var9 = var24;
					}
				}
			}

   // PORT: XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
    // PORT-BLOCKED: GL11.glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
   // PORT-BLOCKED: }

   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var2->rotationPitch - var12)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var2->rotationYaw - var11)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, (float)(-var9));
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var11 - var2->rotationYaw)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var12 - var2->rotationPitch)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -0.1F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var2->prevRotationPitch + (var2->rotationPitch - var2->prevRotationPitch) * var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var2->prevRotationYaw + (var2->rotationYaw - var2->prevRotationYaw) * var1 + 180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void EntityRenderer::setupCameraTransform(float var1, int var2) {
		m_field_1387_i = (float)(256 >> mc->m_gameSettings->m_renderDistance);
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
		float var3 = 0.07F;
		if(mc->m_gameSettings->m_anaglyph) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-(var2 * 2 - 1)) * var3, 0.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  { D3DXMATRIX _persp; D3DXMatrixPerspectiveFovLH(&_persp, D3DXToRadian((float)(this->func_914_d(var1))), (float)((float)this->mc->m_displayWidth / (float)this->mc->m_displayHeight), (float)(0.05F), (float)(this->m_field_1387_i));
    Shaders::setProjectionMatrix(_persp); Shaders::updateShaderConstants(); }
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
		if(mc->m_gameSettings->m_anaglyph) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var2 * 2 - 1) * 0.1F, 0.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		hurtCameraEffect(var1);
		if(mc->m_gameSettings->m_viewBobbing) {
			func_917_f(var1);
		}

		float var4 = mc->thePlayer->m_field_4133_d + (mc->thePlayer->m_field_4134_c - mc->thePlayer->m_field_4133_d) * var1;
		if(var4 > 0.0F) {
			float var5 = 5.0F / (var4 * var4 + 5.0F) - var4 * 0.04F;
			var5 *= var5;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,1.0F), (var4 * var4 * 1500.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F / var5, 1.0F, 1.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,1.0F), (-var4 * var4 * 1500.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		orientCamera(var1);
}

void EntityRenderer::func_4135_b(float var1, int var2) {
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
		if(mc->m_gameSettings->m_anaglyph) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var2 * 2 - 1) * 0.1F, 0.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		hurtCameraEffect(var1);
		if(mc->m_gameSettings->m_viewBobbing) {
			func_917_f(var1);
		}

  // PORT: XInputGetState  // was: if(!this.mc->gameSettings.thirdPersonView && !Keyboard.isKeyDown(Keyboard.KEY_F1)) {
   // PORT-BLOCKED: this.field_1395_a.renderItemInFirstPerson(var1);
  // PORT-BLOCKED: }

  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		if(!mc->m_gameSettings->m_thirdPersonView) {
			m_field_1395_a->renderOverlays(var1);
			hurtCameraEffect(var1);
		}

		if(mc->m_gameSettings->m_viewBobbing) {
			func_917_f(var1);
		}

}

void EntityRenderer::func_4136_b(float var1) {
  if (false) { // PORT: LWJGL window state (Display) fixed on 360
			if(GetTickCount() - m_field_1384_l > 500L) {
				mc->func_6252_g();
			}
		} else {
			m_field_1384_l = GetTickCount();
		}

		if(mc->m_field_6289_L) {
			mc->m_mouseHelper->func_772_c();
			float var2 = mc->m_gameSettings->m_mouseSensitivity * 0.6F + 0.2F;
			float var3 = var2 * var2 * var2 * 8.0F;
			float var4 = (float)mc->m_mouseHelper->m_field_1114_a * var3;
			float var5 = (float)mc->m_mouseHelper->m_field_1113_b * var3;
			unsigned char var6 = 1;
			if(mc->m_gameSettings->m_invertMouse) {
				var6 = -1;
			}

			mc->thePlayer->func_346_d(var4, var5 * (float)var6);
		}

		if(!mc->m_field_6307_v) {
			ScaledResolution var7 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
			int var8 = var7.getScaledWidth();
			int var9 = var7.getScaledHeight();
   // PORT: XInput right stick X  // was: int var10 = Mouse.getX() * var8 / this.mc->displayWidth;
   int var10 = 0; // PORT: stub for blocked declaration
   // PORT: XInput right stick Y  // was: int var11 = var9 - Mouse.getY() * var9 / this.mc->displayHeight - 1;
   int var11 = 0; // PORT: stub for blocked declaration
			if(mc->theWorld != NULL) {
				func_4134_c(var1);
    // PORT: XInputGetState  // was: if(!Keyboard.isKeyDown(Keyboard.KEY_F1)) {
     // PORT-BLOCKED: this.mc->ingameGUI.renderGameOverlay(var1, this.mc->currentScreen != null, var10, var11);
    // PORT-BLOCKED: }
			} else {
    { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
      g_pd3dDevice->SetViewport(&_vp); }
    // Clear color set: (0.0F, 0.0F, 0.0F, 0.0F) — applied in next Clear() call
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
    // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
    { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
				func_905_b();
			}

			if(mc->currentScreen != NULL) {
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
    { static int _di = 0; if (_di < 3) { DbgPrintF("[REND] calling drawScreen curScr=%p\n", (void*)mc->currentScreen); _di++; } }
				mc->currentScreen->drawScreen(var10, var11, var1);
			}

		}
}

void EntityRenderer::func_4134_c(float var1) {
		func_910_a(var1);
		EntityPlayerSP* var2 = mc->thePlayer;
		RenderGlobal* var3 = mc->m_field_6323_f;
		EffectRenderer* var4 = mc->m_field_6321_h;
		double var5 = var2->lastTickPosX + (var2->posX - var2->lastTickPosX) * (double)var1;
		double var7 = var2->lastTickPosY + (var2->posY - var2->lastTickPosY) * (double)var1;
		double var9 = var2->lastTickPosZ + (var2->posZ - var2->lastTickPosZ) * (double)var1;

		for(int var11 = 0; var11 < 2; ++var11) {
			if(mc->m_gameSettings->m_anaglyph) {
				if(var11 == 0) {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x6);
				} else {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x1);
				}
			}

   { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
     g_pd3dDevice->SetViewport(&_vp); }
			updateFogColor(var1);
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			setupCameraTransform(var1, var11);
			ClippingHelperImplementation::func_1155_a();
			if(mc->m_gameSettings->m_renderDistance < 2) {
				func_4140_a(-1);
				var3->func_4142_a(var1);
			}

   Shaders::setFogEnabled(true);
			func_4140_a(1);
			Frustrum* var12 = new Frustrum();
			var12->func_343_a(var5, var7, var9);
			mc->m_field_6323_f->func_960_a(var12, var1);
			mc->m_field_6323_f->func_948_a(var2, false);
			func_4140_a(0);
   Shaders::setFogEnabled(true);
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
			RenderHelper::disableStandardItemLighting();
			var3->func_943_a(var2, 0, (double)var1);
			RenderHelper::enableStandardItemLighting();
			var3->func_951_a(var2->getPosition(var1), var12, var1);
			var4->func_1187_b(var2, var1);
			RenderHelper::disableStandardItemLighting();
			func_4140_a(0);
			var4->func_1189_a(var2, var1);
			if(mc->m_objectMouseOver != NULL && var2->isInsideOfMaterial(Material::s_water)) {
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				var3->func_959_a(var2, mc->m_objectMouseOver, 0, var2->m_inventory->getCurrentItem(), var1);
				var3->drawSelectionBox(var2, mc->m_objectMouseOver, 0, var2->m_inventory->getCurrentItem(), var1);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			}

   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			func_4140_a(0);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
			if(mc->m_gameSettings->m_fancyGraphics) {
    g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
				int var13 = var3->func_943_a(var2, 1, (double)var1);
    g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
				if(mc->m_gameSettings->m_anaglyph) {
					if(var11 == 0) {
      g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x6);
					} else {
      g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x1);
					}
				}

				if(var13 > 0) {
					var3->func_944_a(1, (double)var1);
				}
			} else {
				var3->func_943_a(var2, 1, (double)var1);
			}

   g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			if(mc->m_objectMouseOver != NULL && !var2->isInsideOfMaterial(Material::s_water)) {
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				var3->func_959_a(var2, mc->m_objectMouseOver, 0, var2->m_inventory->getCurrentItem(), var1);
				var3->drawSelectionBox(var2, mc->m_objectMouseOver, 0, var2->m_inventory->getCurrentItem(), var1);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			}

   Shaders::setFogEnabled(false);
			if(m_field_1385_k != NULL) {
			}

			func_4140_a(0);
   Shaders::setFogEnabled(true);
			var3->func_4141_b(var1);
   Shaders::setFogEnabled(false);
			func_4140_a(1);
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
			func_4135_b(var1, var11);
			if(!mc->m_gameSettings->m_anaglyph) {
				return;
			}
		}

  g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x7);
}

void EntityRenderer::func_916_c() {
		if(mc->m_gameSettings->m_fancyGraphics) {
			EntityPlayerSP* var1 = mc->thePlayer;
			World* var2 = mc->theWorld;
			int var3 = (int)floor(var1->posX);
			int var4 = (int)floor(var1->posY);
			int var5 = (int)floor(var1->posZ);
			unsigned char var6 = 16;

			for(int var7 = 0; var7 < 150; ++var7) {
				int var8 = var3 + m_field_1383_m->nextInt(var6) - m_field_1383_m->nextInt(var6);
				int var9 = var5 + m_field_1383_m->nextInt(var6) - m_field_1383_m->nextInt(var6);
				int var10 = var2->func_696_e(var8, var9);
				int var11 = var2->getBlockId(var8, var10 - 1, var9);
				if(var10 <= var4 + var6 && var10 >= var4 - var6) {
					float var12 = m_field_1383_m->nextFloat();
					float var13 = m_field_1383_m->nextFloat();
					if(var11 > 0) {
						mc->m_field_6321_h->func_1192_a(new EntityRainFX(var2, (double)((float)var8 + var12), (double)((float)var10 + 0.1F) - Block::blocksList[var11]->minY, (double)((float)var9 + var13)));
					}
				}
			}

		}
}

void EntityRenderer::func_905_b() {
		ScaledResolution var1 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
		int var2 = var1.getScaledWidth();
		int var3 = var1.getScaledHeight();
  { static int _oi = 0; if (_oi < 3) { DbgPrintF("[ORTHO] scaledW=%d scaledH=%d dw=%d dh=%d\n", var2, var3, mc->m_displayWidth, mc->m_displayHeight); _oi++; } }
  g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
  // PORT: 2D GUI render state — back-face cull OFF (the Y-flipped GUI ortho reverses quad
  // winding, so the inherited CULL_CW would cull every menu quad -> invisible blue screen)
  // and depth test OFF. Matches indev setupOverlayRendering; alpha's converter omitted both.
  g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  // PORT: D3D LH GUI ortho — Minecraft's glOrtho(0,w,h,0,1000,3000) is Y-FLIPPED (origin
  // top-left): pass bottom=h+0.5, top=0.5 (+0.5 half-pixel). And the view Z translate must
  // be +2000 (NOT -2000): D3D LH visible range is [+near,+far]=[1000,3000]; GUI quads at z=0
  // translated to +2000 land inside it. The original -2000 put everything behind the near
  // plane -> all GUI clipped -> blank blue screen. (Matches the working indev EntityRenderer.)
  { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.5), (float)((double)var2 + 0.5), (float)((double)var3 + 0.5), (float)(0.5), (float)(1000.0), (float)(3000.0));
    Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 2000.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void EntityRenderer::updateFogColor(float var1) {
		World* var2 = mc->theWorld;
		EntityPlayerSP* var3 = mc->thePlayer;
		float var4 = 1.0F / (float)(4 - mc->m_gameSettings->m_renderDistance);
		var4 = 1.0F - (float)powf((double)var4, 0.25f);
		Vec3D* var5 = var2->func_4079_a(mc->thePlayer, var1);
		float var6 = (float)var5->xCoord;
		float var7 = (float)var5->yCoord;
		float var8 = (float)var5->zCoord;
		Vec3D* var9 = var2->func_4082_d(var1);
		m_field_4270_e = (float)var9->xCoord;
		m_field_4269_f = (float)var9->yCoord;
		m_field_4268_g = (float)var9->zCoord;
		m_field_4270_e += (var6 - m_field_4270_e) * var4;
		m_field_4269_f += (var7 - m_field_4269_f) * var4;
		m_field_4268_g += (var8 - m_field_4268_g) * var4;
		if(var3->isInsideOfMaterial(Material::s_water)) {
			m_field_4270_e = 0.02F;
			m_field_4269_f = 0.02F;
			m_field_4268_g = 0.2F;
		} else if(var3->isInsideOfMaterial(Material::s_lava)) {
			m_field_4270_e = 0.6F;
			m_field_4269_f = 0.1F;
			m_field_4268_g = 0.0F;
		}

		float var10 = m_field_1382_n + (m_field_1381_o - m_field_1382_n) * var1;
		m_field_4270_e *= var10;
		m_field_4269_f *= var10;
		m_field_4268_g *= var10;
		if(mc->m_gameSettings->m_anaglyph) {
			float var11 = (m_field_4270_e * 30.0F + m_field_4269_f * 59.0F + m_field_4268_g * 11.0F) / 100.0F;
			float var12 = (m_field_4270_e * 30.0F + m_field_4269_f * 70.0F) / 100.0F;
			float var13 = (m_field_4270_e * 30.0F + m_field_4268_g * 70.0F) / 100.0F;
			m_field_4270_e = var11;
			m_field_4269_f = var12;
			m_field_4268_g = var13;
		}

  // Clear color set: (this.field_4270_e, this.field_4269_f, this.field_4268_g, 0.0F) — applied in next Clear() call
}

void EntityRenderer::func_4140_a(int var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
  // Fog color buffer — set via Shaders::setFogColor()
  Tesselator::getInstance().normal(0.0F, -1.0F, 0.0F);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		float var3;
		float var4;
		float var5;
		float var6;
		float var7;
		float var8;
		if(var2->isInsideOfMaterial(Material::s_water)) {
   // Fog mode = GL11.GL_EXP — handled by shader
   Shaders::setFogDensity(0.1F);
			var3 = 0.4F;
			var4 = 0.4F;
			var5 = 0.9F;
			if(mc->m_gameSettings->m_anaglyph) {
				var6 = (var3 * 30.0F + var4 * 59.0F + var5 * 11.0F) / 100.0F;
				var7 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
				var8 = (var3 * 30.0F + var5 * 70.0F) / 100.0F;
			}
		} else if(var2->isInsideOfMaterial(Material::s_lava)) {
   // Fog mode = GL11.GL_EXP — handled by shader
   Shaders::setFogDensity(2.0F);
			var3 = 0.4F;
			var4 = 0.3F;
			var5 = 0.3F;
			if(mc->m_gameSettings->m_anaglyph) {
				var6 = (var3 * 30.0F + var4 * 59.0F + var5 * 11.0F) / 100.0F;
				var7 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
				var8 = (var3 * 30.0F + var5 * 70.0F) / 100.0F;
			}
		} else {
   // Fog mode = GL11.GL_LINEAR — handled by shader
   // Fog start = this.field_1387_i * 0.25F — handled by shader
   // Fog end = this.field_1387_i — handled by shader
			if(var1 < 0) {
    // Fog start = 0.0F — handled by shader
    // Fog end = this.field_1387_i * 0.8F — handled by shader
			}

   // PORT: D3D9 caps check  // was: if(GLContext::getCapabilities().GL_NV_fog_distance) {
    // PORT-BLOCKED: GL11.glFogi(NVFogDistance.GL_FOG_DISTANCE_MODE_NV, NVFogDistance.GL_EYE_RADIAL_NV);
   // PORT-BLOCKED: }

			if(mc->theWorld->m_worldProvider->m_field_4220_c) {
    // Fog start = 0.0F — handled by shader
			}
		}

  // Color material is shader-based on Xbox 360 (enabled)
  // Color material is shader-based on Xbox 360
}

float* /* PORT: was FloatBuffer */ EntityRenderer::func_908_a(float var1, float var2, float var3, float var4) {
  memset(m_field_1392_d, 0, sizeof(float) * 16);  // PORT: was sizeof(m_field_1392_d) = pointer size (4), buffer is 16 floats
  m_field_1392_d[0] = var1; m_field_1392_d[1] = var2; m_field_1392_d[2] = var3; m_field_1392_d[3] = var4;
  // NIO flip() — no-op for C array
		return m_field_1392_d;
}
