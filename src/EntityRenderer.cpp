#include "stdafx.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// Auto-generated from EntityRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityRenderer.h"
#include "Common.h"
#include "DecimalFormat.h"
#include "Entity.h"
#include "ItemRenderer.h"
#include "Minecraft.h"
#include "Random.h"
#include "Vec3D.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "EffectRenderer.h"
#include "EntityPlayerSP.h"
#include "Material.h"
#include "MathHelper.h"
#include "MovingObjectPosition.h"
#include "RenderBlocks.h"
#include "RenderGlobal.h"
#include "RenderHelper.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "Vec3.h"
#include "World.h"

EntityRenderer::EntityRenderer()
	: mc(NULL)
	, m_anaglyphEnable(false)
	, m_farPlaneDistance(0.0f)
	, m_itemRenderer(NULL)
	, m_rendererUpdateCount(0)
	, m_pointedEntity(NULL)
	, m_entityRendererInt1(0)
	, m_entityRendererInt2(0)
	, m_entityDecimalFormat(new DecimalFormat("0000"))
	, m_entityFloatBuffer(new float[16])
	, m_random(new Random())
	, m_fogColorBuffer(new float[16])
	, m_fogColorRed(0)
	, m_fogColorGreen(0)
	, m_fogColorBlue(0)
	, m_prevFogColor(0)
	, m_fogColor(0)
{
}

EntityRenderer::EntityRenderer(Minecraft* var1)
	: mc(NULL)
	, m_anaglyphEnable(false)
	, m_farPlaneDistance(0.0f)
	, m_itemRenderer(NULL)
	, m_rendererUpdateCount(0)
	, m_pointedEntity(NULL)
	, m_entityRendererInt1(0)
	, m_entityRendererInt2(0)
	, m_entityDecimalFormat(new DecimalFormat("0000"))
	, m_entityFloatBuffer(new float[16])
	, m_random(new Random())
	, m_fogColorBuffer(new float[16])
	, m_fogColorRed(0)
	, m_fogColorGreen(0)
	, m_fogColorBlue(0)
	, m_prevFogColor(0)
	, m_fogColor(0)
{
		mc = var1;
		m_itemRenderer = new ItemRenderer(var1);
}

EntityRenderer::~EntityRenderer() {
	mc = NULL;
	m_itemRenderer = NULL;
	m_pointedEntity = NULL;
	m_entityDecimalFormat = NULL;
	m_random = NULL;
}

void EntityRenderer::updateRenderer() {
		m_prevFogColor = m_fogColor;
		float var1 = mc->theWorld->getLightBrightness((int)mc->thePlayer->posX, (int)mc->thePlayer->posY, (int)mc->thePlayer->posZ);
		float var2 = (float)(3 - mc->m_options->m_renderDistance) / 3.0F;
		var1 = var1 * (1.0F - var2) + var2;
		m_fogColor += (var1 - m_fogColor) * 0.1F;
		++m_rendererUpdateCount;
		m_itemRenderer->updateEquippedItem();
		if(mc->m_D) {
			EntityRenderer var13 = this;
			EntityPlayerSP* var14 = mc->thePlayer;
			World* var3 = mc->theWorld;
			int var4 = (int)var14->posX;
			int var5 = (int)var14->posY;
			int var15 = (int)var14->posZ;

			for(int var6 = 0; var6 < 50; ++var6) {
				int var7 = var4 + var13.m_random->nextInt(9) - 4;
				int var8 = var15 + var13.m_random->nextInt(9) - 4;
				int var9 = var3->a(var7, var8);
				int var10 = var3->getBlockId(var7, var9 - 1, var8);
				if(var9 <= var5 + 4 && var9 >= var5 - 4) {
					float var11 = var13.m_random->nextFloat();
					float var12 = var13.m_random->nextFloat();
					if(var10 > 0) {
						var13.mc->m_effectRenderer->addEffect(new EntityRainFX(var3, (float)var7 + var11, (float)var9 + 0.1F - Block::blocksList[var10].minY, (float)var8 + var12));
					}
				}
			}
		}

}

Vec3D EntityRenderer::orientCamera(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		float var3 = var2->prevPosX + (var2->posX - var2->prevPosX) * var1;
		float var4 = var2->prevPosY + (var2->posY - var2->prevPosY) * var1;
		var1 = var2->prevPosZ + (var2->posZ - var2->prevPosZ) * var1;
		return new Vec3D(var3, var4, var1);
}

void EntityRenderer::hurtCameraEffect(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		float var3 = (float)var2.hurtTime - var1;
		if(var2.health <= 0) {
			var1 += (float)var2->deathTime;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (40.0F - 8000.0F / (var1 + 200.0F))*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		if(var3 >= 0.0F) {
			var3 /= (float)var2->m_maxHurtTime;
			var3 = sinf(var3 * var3 * var3 * var3 * 3.14159265358979323846f);
			var1 = var2->attackedAtYaw;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-var1)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (-var3 * 14.0F)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var1)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}
}

void EntityRenderer::setupViewBobbing(float var1) {
		EntityPlayerSP* var2 = mc->thePlayer;
		float var3 = var2->distanceWalkedModified - var2->prevDistanceWalkedModified;
		var3 = var2->distanceWalkedModified + var3 * var1;
		float var4 = var2->m_prevCameraYaw + (var2->m_cameraYaw - var2->m_prevCameraYaw) * var1;
		var1 = var2->m_prevCameraPitch + (var2->m_cameraPitch - var2->m_prevCameraPitch) * var1;
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, MathHelper::sin(var3 * (float)M_PI) * var4 * 0.5F, -fabsf(MathHelper::cos(var3 * (float)M_PI) * var4), 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (MathHelper::sin(var3 * (float)M_PI) * var4 * 3.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (fabsf(MathHelper::cos(var3 * (float)M_PI + 0.2F) * var4) * 5.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var1)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void EntityRenderer::updateCameraAndRender(float var1) {
  true /* PORT: always active on 360 */  // was: if(this.m_anaglyphEnable && !Display.isActive()) {
			mc->displayInGameMenu();
		}

  true /* PORT: always active on 360 */  // was: this.m_anaglyphEnable = Display.isActive();
		int var5;
		int var6;
		if(mc->m_inventoryScreen) {
   // PORT: XInput right stick delta X  // was: Mouse.getDX();
			unsigned char var2 = 0;
   // PORT: XInput right stick delta Y  // was: Mouse.getDY();
			unsigned char var3 = 0;
			mc->m_mouseHelper.ungrabMouseCursor();
			unsigned char var4 = 1;
			if(mc->m_options->m_invertMouse) {
				var4 = -1;
			}

			var5 = var2 + mc->m_mouseHelper.m_deltaX;
			var6 = var3 - mc->m_mouseHelper.m_deltaY;
			if(var2 != 0 || m_entityRendererInt1 != 0) {
				DbgPrintF("%s\n", "xxo: " + var2 + ", " + m_entityRendererInt1 + ": " + m_entityRendererInt1 + ", xo: " + var5);
			}

			if(m_entityRendererInt1 != 0) {
				m_entityRendererInt1 = 0;
			}

			if(m_entityRendererInt2 != 0) {
				m_entityRendererInt2 = 0;
			}

			if(var2 != 0) {
				m_entityRendererInt1 = var2;
			}

			if(var3 != 0) {
				m_entityRendererInt2 = var3;
			}

			float var10001 = (float)var5;
			float var11 = (float)(var6 * var4);
			float var9 = var10001;
			EntityPlayerSP* var7 = mc->thePlayer;
			float var13 = var7->rotationPitch;
			float var14 = var7->rotationYaw;
			var7->rotationYaw = (float)((double)var7->rotationYaw + (double)var9 * 0.15f);
			var7->rotationPitch = (float)((double)var7->rotationPitch - (double)var11 * 0.15f);
			if(var7->rotationPitch < -90.0F) {
				var7->rotationPitch = -90.0F;
			}

			if(var7->rotationPitch > 90.0F) {
				var7->rotationPitch = 90.0F;
			}

			var7->prevRotationPitch += var7->rotationPitch - var13;
			var7->prevRotationYaw += var7->rotationYaw - var14;
		}

		ScaledResolution var8 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
		int var10 = var8.getScaledWidth();
		int var12 = var8.getScaledHeight();
  // PORT: XInput right stick X  // was: var5 = Mouse.getX() * var10 / this.mc->displayWidth;
  // PORT: XInput right stick Y  // was: var6 = var12 - Mouse.getY() * var12 / this.mc->displayHeight - 1;
		if(mc->theWorld != NULL) {
			getMouseOver(var1);
			mc->m_t.renderGameOverlay(var1);
		} else {
   { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
     g_pd3dDevice->SetViewport(&_vp); }
   // Clear color set: (0.0F, 0.0F, 0.0F, 0.0F) � applied in next Clear() call
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
			setupOverlayRendering();
		}

		if(mc->currentScreen != NULL) {
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
			mc->currentScreen->drawScreen(var5, var6, var1);
		}

		Sleep(0);
  g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void EntityRenderer::grabLargeScreenshot() {
		mc->m_loadingScreen->displayProgressMessage("Grabbing large screenshot");
		File* var1 = new File(System.getProperty("user.home", "."));
		int var2 = 0;

		while(true) {
			File* var3 = new File(var1, "mc_map_" + m_entityDecimalFormat->format((long)var2) + ".png");
			if(!var3->exists()) {
				var3 = var3->getAbsoluteFile();
				mc->m_loadingScreen->displayLoadingString("Rendering");
				mc->m_loadingScreen->setLoadingProgress(0);

    { // was try
					int var19 = (mc->theWorld->width << 4) + (mc->theWorld /* PORT: array length - use tracked size */ << 4);
					var2 = (mc->theWorld->height << 4) + var19 / 2;
     // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage* var4 = new BufferedImage(var19, var2, 1);
					Graphics var5 = var4.getGraphics();
					int var6 = mc->m_displayWidth;
					int var7 = mc->m_displayHeight;
					int var8 = (var19 / var6 + 1) * (var2 / var7 + 1);
					int var9 = 0;

					for(int var10 = 0; var10 < var19; var10 += var6) {
						for(int var11 = 0; var11 < var2; var11 += var7) {
							++var9;
							mc->m_loadingScreen->setLoadingProgress(var9 * 100 / var8);
							int var10001 = var10 - var19 / 2;
							int var10002 = var11 - var2 / 2;
							float var12 = 0.0F;
							int var14 = var10002;
							int var13 = var10001;
							if(m_entityByteBuffer == NULL) {
								m_entityByteBuffer = new unsigned char[mc->m_displayWidth * mc->m_displayHeight << 2];
							}

							EntityPlayerSP* var15 = mc->thePlayer;
							World* var16 = mc->theWorld;
							RenderGlobal var17 = mc->m_renderGlobal;
       { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
         g_pd3dDevice->SetViewport(&_vp); }
							updateFogColor(0.0F);
       g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
       g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
							m_farPlaneDistance = (float)(512 >> (mc->m_options->m_renderDistance << 1));
       // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
       { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
       { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.0D), (float)((double)this->mc->m_displayWidth), (float)((double)this->mc->m_displayHeight), (float)(0.0D), (float)(10.0D), (float)(10000.0D));
         Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
       // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
       { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
       { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-var13), (float)(-var14), -5000.0F);
         D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
         Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
       { D3DXMATRIX _s; D3DXMatrixScaling(&_s, 16.0F, -16.0F, -16.0F);
         D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
         Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
							float var22 = 1.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = 0.5F;
							var22 = 1.0F;
							var22 = 0.0F;
							var22 = -1.0F;
							var22 = 1.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = 0.0F;
							var22 = -0.5F;
							var22 = 1.0F;
       memset(m_entityFloatBuffer, 0, sizeof(m_entityFloatBuffer));
       m_entityFloatBuffer[0] = 1.0F; m_entityFloatBuffer[1] = -0.5F; m_entityFloatBuffer[2] = 0.0F; m_entityFloatBuffer[3] = 0.0F;
       m_entityFloatBuffer[0] = 0.0F; m_entityFloatBuffer[1] = 1.0F; m_entityFloatBuffer[2] = -1.0F; m_entityFloatBuffer[3] = 0.0F;
       m_entityFloatBuffer[0] = 1.0F; m_entityFloatBuffer[1] = 0.5F; m_entityFloatBuffer[2] = 0.0F; m_entityFloatBuffer[3] = 0.0F;
       m_entityFloatBuffer[0] = 0.0F; m_entityFloatBuffer[1] = 0.0F; m_entityFloatBuffer[2] = 0.0F; m_entityFloatBuffer[3] = 1.0F;
       // NIO flip() � no-op for C array
       // PORT: multiply into Shaders world matrix
       { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (0.0F)*DEG_TO_RAD);
         D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
         Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
       { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-var16->width) / 2.0F, (float)(-var16.height) / 2.0F, (float);
         D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
         Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
							IsomCamera* var24 = new IsomCamera();
							mc->m_renderGlobal.clipRenderersByFrustrum(var24);
							mc->m_renderGlobal.updateRenderers(var15);
							setupFog();
       Shaders::setFogEnabled(true);
       // Fog mode = GL11.GL_LINEAR � handled by shader
							float var23 = (float)var16.height * 8.0F;
       // Fog start = 5000.0F - var23 � handled by shader
       // Fog end = 5000.0F + var23 * 8.0F � handled by shader
							RenderHelper::enableStandardItemLighting();
							var17.renderEntities(orientCamera(0.0F), var24, 0.0F);
							RenderHelper::disableStandardItemLighting();
       g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
							var17.sortAndRender(var15, 0);
							var17.oobGroundRenderer();
							if(var16->m_cloudHeight < var16.height) {
								var17.renderSky(0.0F);
							}

       g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
       g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
       g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
       g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
							var13 = var17.sortAndRender(var15, 1);
       g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
							if(var13 > 0) {
								var17.renderAllRenderLists();
							}

							if(var16->getGroundLevel() >= 0) {
								var17.oobWaterRenderer();
							}

       g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
       g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
       Shaders::setFogEnabled(false);
       memset(m_entityByteBuffer, 0, sizeof(m_entityByteBuffer));
       // Pixel store alignment � not needed on D3D9
       // PORT: D3D9 screenshot via GetRenderTargetData + LockRect
       // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage var21 = screenshotBuffer(m_entityByteBuffer, var6, var7);
							var5.drawImage(var21, var10, var11, (ImageObserver)NULL);
						}
					}

					var5.dispose();
					mc->m_loadingScreen->displayLoadingString("Saving as " + var3 /* PORT: toString */);
					mc->m_loadingScreen->setLoadingProgress(100);
     // PORT: FileOutputStream -> Xbox 360 file I/O  // was: FileOutputStream* var20 = new FileOutputStream(var3);
     // PORT: ImageIO -> D3D9 texture loading  // was: ImageIO.write(var4, "png", var20);
     // Removed Java stream/exception call: var20.close();
					return;
				} { DbgPrintF("CATCH Throwable\n"); // was catch(Throwable var18)
     // Removed Java stream/exception call: var18.printStackTrace();
					return;
				}
			}

			++var2;
		}
}

void* /* PORT: was BufferedImage */ EntityRenderer::screenshotBuffer(unsigned char* /* PORT: was ByteBuffer */ var0, int var1, int var2) {
  // buffer var0 position = 0
  // PORT: BufferedImage -> D3D9 texture loading  // was: BufferedImage* var3 = new BufferedImage(var1, var2, 1);
		int[] var4 = ((DataBufferInt)var3->getRaster().getDataBuffer()).getData();

		for(int var5 = 0; var5 < var1 * var2; ++var5) {
			int var6 = var0.get(var5 * 3) & 255;
			int var7 = var0.get(var5 * 3 + 1) & 255;
			int var8 = var0.get(var5 * 3 + 2) & 255;
			var4[var5] = var6 << 16 | var7 << 8 | var8;
		}

		return var3;
}

void EntityRenderer::getMouseOver(float var1) {
		EntityRenderer var7 = this;
		EntityPlayerSP* var9 = mc->thePlayer;
		float var3 = var9->prevRotationPitch + (var9->rotationPitch - var9->prevRotationPitch) * var1;
		float var10 = var9->prevRotationYaw + (var9->rotationYaw - var9->prevRotationYaw) * var1;
		Vec3D var11 = orientCamera(var1);
		float var12 = cosf(-var10 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
		float var13 = sinf(-var10 * (3.14159265358979323846f / 180.0F) - 3.14159265358979323846f);
		var10 = cosf(-var3 * (3.14159265358979323846f / 180.0F));
		float var14 = sinf(-var3 * (3.14159265358979323846f / 180.0F));
		float var4 = var13 * var10;
		float var6 = var12 * var10;
		float var15 = mc->m_playerController.getBlockReachDistance();
		Vec3D var2 = var11.addVector(var4 * var15, var14 * var15, var6 * var15);
		mc->m_objectMouseOver = mc->theWorld->rayTraceBlocks(var11, var2);
		float var5 = var15;
		var11 = orientCamera(var1);
		if(mc->m_objectMouseOver != NULL) {
			var5 = mc->m_objectMouseOver.m_hitVec.distance(var11);
		}

		if(mc->dynamic_cast<PlayerControllerCreative*>(playerController) != NULL) {
			var15 = 32.0F;
		} else {
			if(var5 > 3.0F) {
				var5 = 3.0F;
			}

			var15 = var5;
		}

		var2 = var11.addVector(var4 * var15, var14 * var15, var6 * var15);
		m_pointedEntity = NULL;
		std::vector<Entity> var16 = mc->theWorld->m_entityMap.a(var9, var9->boundingBox.addCoord(var4 * var15, var14 * var15, var6 * var15));
		float var17 = 0.0F;

		for(size_t var18 = 0; var18 < var16.size(); ++var18) {
			Entity var20 = (Entity*)var16[var18];
			if(var20.canBeCollidedWith()) {
				AxisAlignedBB var23 = var20.boundingBox.expand(0.1F, 0.1F, 0.1F);
				MovingObjectPosition var24 = var23.calculateIntercept(var11, var2);
				if(var24 != NULL) {
					var4 = var11.distance(var24.m_hitVec);
					if(var4 < var17 || var17 == 0.0F) {
						var7.m_pointedEntity = var20;
						var17 = var4;
					}
				}
			}
		}

		if(var7.m_pointedEntity != NULL && !(var7.mc->dynamic_cast<PlayerControllerCreative*>(playerController) != NULL)) {
			var7.mc->m_objectMouseOver = MovingObjectPosition(var7.m_pointedEntity);
		}

		for(int var19 = 0; var19 < 2; ++var19) {
			if(mc->m_options->m_anaglyph) {
				if(var19 == 0) {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x6);
				} else {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x1);
				}
			}

			EntityPlayerSP* var21 = mc->thePlayer;
			World* var25 = mc->theWorld;
			RenderGlobal var28 = mc->m_renderGlobal;
			EffectRenderer var27 = mc->m_effectRenderer;
   { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
     g_pd3dDevice->SetViewport(&_vp); }
			updateFogColor(var1);
   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			m_farPlaneDistance = (float)(512 >> (mc->m_options->m_renderDistance << 1));
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
			if(mc->m_options->m_anaglyph) {
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(-((var19 << 1) - 1)) * 0.07F, 0.0F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			EntityPlayerSP* var37 = mc->thePlayer;
			var13 = 70.0F;
			if(var37->isInsideOfMaterial()) {
				var13 = 60.0F;
			}

			if(var37.health <= 0) {
				var10 = (float)var37->deathTime + var1;
				var13 /= (1.0F - 500.0F / (var10 + 500.0F)) * 2.0F + 1.0F;
			}

   { D3DXMATRIX _persp; D3DXMatrixPerspectiveFovLH(&_persp, D3DXToRadian((float)(var13)), (float)((float)this->mc->m_displayWidth / (float)this->mc->m_displayHeight), (float)(0.05F), (float)(this->m_farPlaneDistance));
     Shaders::setProjectionMatrix(_persp); Shaders::updateShaderConstants(); }
   // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
			if(mc->m_options->m_anaglyph) {
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)((var19 << 1) - 1) * 0.1F, 0.0F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

			hurtCameraEffect(var1);
			if(mc->m_options->m_fancyGraphics) {
				setupViewBobbing(var1);
			}

			var37 = mc->thePlayer;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -0.1F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (var37->prevRotationPitch + (var37->rotationPitch - var37->prevRotationPitch) * var1)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var37->prevRotationYaw + (var37->rotationYaw - var37->prevRotationYaw) * var1)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var13 = var37->prevPosX + (var37->posX - var37->prevPosX) * var1;
			var10 = var37->prevPosY + (var37->posY - var37->prevPosY) * var1;
			var14 = var37->prevPosZ + (var37->posZ - var37->prevPosZ) * var1;
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, -var13, -var10, -var14);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			setupFog();
			var28.renderSky(var1);
			setupFog();
			Frustrum* var31 = new Frustrum(var21, m_farPlaneDistance, var1);
			mc->m_renderGlobal.clipRenderersByFrustrum(var31);
			mc->m_renderGlobal.updateRenderers(var21);
			setupFog();
   Shaders::setFogEnabled(true);
   g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
			RenderHelper::disableStandardItemLighting();
			var28.sortAndRender(var21, 0);
			int var8;
			int var34;
			int var38;
			int var39;
			int var42;
			if(var25->isSolid(var21->posX, var21->posY, var21->posZ, 0.1F)) {
				var8 = (int)var21->posX;
				int var33 = (int)var21->posY;
				var34 = (int)var21->posZ;
				RenderBlocks* var35 = new RenderBlocks(var25);

				for(var38 = var8 - 1; var38 <= var8 + 1; ++var38) {
					for(int var40 = var33 - 1; var40 <= var33 + 1; ++var40) {
						for(var39 = var34 - 1; var39 <= var34 + 1; ++var39) {
							var42 = var25->getBlockId(var38, var40, var39);
							if(var42 > 0) {
								var35->renderBlockAllFaces(Block::blocksList[var42], var38, var40, var39);
							}
						}
					}
				}
			}

			RenderHelper::enableStandardItemLighting();
			var28.renderEntities(orientCamera(var1), var31, var1);
			var27.renderLitParticles(var1);
			RenderHelper::disableStandardItemLighting();
			setupFog();
			var27.renderParticles(var21, var1);
			var28.oobGroundRenderer();
			if(mc->m_objectMouseOver != NULL && var21->isInsideOfMaterial()) {
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				var28.drawBlockBreaking(mc->m_objectMouseOver, 0, var21->m_inventory.getCurrentItem());
				var28.drawSelectionBox(mc->m_objectMouseOver, 0);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			}

   g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
   g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			setupFog();
			var28.oobWaterRenderer();
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
   g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
			var8 = var28.sortAndRender(var21, 1);
   g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
			if(mc->m_options->m_anaglyph) {
				if(var19 == 0) {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x6);
				} else {
     g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x1);
				}
			}

			if(var8 > 0) {
				var28.renderAllRenderLists();
			}

   g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
   g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
   g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			if(mc->m_objectMouseOver != NULL && !var21->isInsideOfMaterial()) {
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				var28.drawBlockBreaking(mc->m_objectMouseOver, 0, var21->m_inventory.getCurrentItem());
				var28.drawSelectionBox(mc->m_objectMouseOver, 0);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			}

   Shaders::setFogEnabled(false);
			if(mc->m_D) {
				float var32 = var1;
				var7 = this;
				var9 = mc->thePlayer;
				World* var22 = mc->theWorld;
				var34 = (int)var9->posX;
				int var36 = (int)var9->posY;
				var38 = (int)var9->posZ;
				Tesselator var41 = Tesselator::getInstance();
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Tesselator::getInstance().normal(0.0F, 1.0F, 0.0F);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/rain.png")));
				var39 = var34 - 5;

				while(true) {
					if(var39 > var34 + 5) {
      g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
      g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
						break;
					}

					for(int var26 = var38 - 5; var26 <= var38 + 5; ++var26) {
						int var30 = var22->a(var39, var26);
						int var29 = var36 - 5;
						var42 = var36 + 5;
						if(var29 < var30) {
							var29 = var30;
						}

						if(var42 < var30) {
							var42 = var30;
						}

						if(var29 != var42) {
							var5 = ((float)((var7.m_rendererUpdateCount + var39 * 3121 + var26 * 418711) % 32) + var32) / 32.0F;
							float var43 = (float)var39 + 0.5F - var9->posX;
							var17 = (float)var26 + 0.5F - var9->posZ;
							float var44 = sqrtf(var43 * var43 + var17 * var17) / 5.0F;
       Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, (1.0F - var44 * var44);
							var41.init();
							var41.vertexUV((float)var39, (float)var29, (float)var26, 0.0F, (float)var29 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)(var39 + 1), (float)var29, (float)(var26 + 1), 2.0F, (float)var29 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)(var39 + 1), (float)var42, (float)(var26 + 1), 2.0F, (float)var42 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)var39, (float)var42, (float)var26, 0.0F, (float)var42 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)var39, (float)var29, (float)(var26 + 1), 0.0F, (float)var29 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)(var39 + 1), (float)var29, (float)var26, 2.0F, (float)var29 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)(var39 + 1), (float)var42, (float)var26, 2.0F, (float)var42 * 2.0F / 8.0F + var5 * 2.0F);
							var41.vertexUV((float)var39, (float)var42, (float)(var26 + 1), 0.0F, (float)var42 * 2.0F / 8.0F + var5 * 2.0F);
							var41.flush();
						}
					}

					++var39;
				}
			}

   g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
   { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
			if(mc->m_options->m_anaglyph) {
    { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)((var19 << 1) - 1) * 0.1F, 0.0F, 0.0F);
      D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
      Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			}

   D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			hurtCameraEffect(var1);
			if(mc->m_options->m_fancyGraphics) {
				setupViewBobbing(var1);
			}

			m_itemRenderer->renderItemInFirstPerson(var1);
   Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			m_itemRenderer->renderOverlays(var1);
			hurtCameraEffect(var1);
			if(mc->m_options->m_fancyGraphics) {
				setupViewBobbing(var1);
			}

			if(!mc->m_options->m_anaglyph) {
				return;
			}
		}

  g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x7);
}

void EntityRenderer::setupOverlayRendering() {
		ScaledResolution var1 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
		int var2 = var1.getScaledWidth();
		int var3 = var1.getScaledHeight();
  g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(0.0D), (float)((double)var2), (float)(0.0D), (float)((double)var3), (float)(1000.0D), (float)(3000.0D));
    Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }
  // Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix
  { D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, -2000.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
}

void EntityRenderer::updateFogColor(float var1) {
		World* var2 = mc->theWorld;
		EntityPlayerSP* var3 = mc->thePlayer;
		float var4 = 1.0F / (float)(4 - mc->m_options->m_renderDistance);
		var4 = 1.0F - (float)powf((double)var4, 0.25f);
		Vec3D var5 = var2->getSkyColor(var1);
		float var6 = var5.m_xCoord;
		float var7 = var5.m_yCoord;
		float var13 = var5.m_zCoord;
		Vec3D var8 = var2->getFogColor(var1);
		m_fogColorRed = var8.m_xCoord;
		m_fogColorGreen = var8.m_yCoord;
		m_fogColorBlue = var8.m_zCoord;
		m_fogColorRed += (var6 - m_fogColorRed) * var4;
		m_fogColorGreen += (var7 - m_fogColorGreen) * var4;
		m_fogColorBlue += (var13 - m_fogColorBlue) * var4;
		Block var9 = Block::blocksList[var2->getBlockId((int)var3->posX, (int)(var3->posY + 0.12F), (int)var3->posZ)];
		if(var9 != NULL && var9.material != Material::s_air) {
			Material var10 = var9.material;
			if(var10 == Material::s_water) {
				m_fogColorRed = 0.02F;
				m_fogColorGreen = 0.02F;
				m_fogColorBlue = 0.2F;
			} else if(var10 == Material::s_lava) {
				m_fogColorRed = 0.6F;
				m_fogColorGreen = 0.1F;
				m_fogColorBlue = 0.0F;
			}
		}

		float var11 = m_prevFogColor + (m_fogColor - m_prevFogColor) * var1;
		m_fogColorRed *= var11;
		m_fogColorGreen *= var11;
		m_fogColorBlue *= var11;
		if(mc->m_options->m_anaglyph) {
			var1 = (m_fogColorRed * 30.0F + m_fogColorGreen * 59.0F + m_fogColorBlue * 11.0F) / 100.0F;
			var11 = (m_fogColorRed * 30.0F + m_fogColorGreen * 70.0F) / 100.0F;
			float var12 = (m_fogColorRed * 30.0F + m_fogColorBlue * 70.0F) / 100.0F;
			m_fogColorRed = var1;
			m_fogColorGreen = var11;
			m_fogColorBlue = var12;
		}

  // Clear color set: (this.fogColorRed, this.fogColorGreen, this.fogColorBlue, 0.0F) � applied in next Clear() call
}

void EntityRenderer::setupFog() {
		World* var1 = mc->theWorld;
		EntityPlayerSP* var2 = mc->thePlayer;
		int var10000 = 0 /* fog color handled by Shaders */;
		float var3 = 1.0F;
		float var6 = m_fogColorBlue;
		float var5 = m_fogColorGreen;
		float var4 = m_fogColorRed;
  memset(m_fogColorBuffer, 0, sizeof(m_fogColorBuffer));
  m_fogColorBuffer[0] = var4; m_fogColorBuffer[1] = var5; m_fogColorBuffer[2] = var6; m_fogColorBuffer[3] = 1.0F;
  // NIO flip() � no-op for C array
  // Fog color buffer � set via Shaders::setFogColor()
  Tesselator::getInstance().normal(0.0F, -1.0F, 0.0F);
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		Block var7 = Block::blocksList[var1->getBlockId((int)var2->posX, (int)(var2->posY + 0.12F), (int)var2->posZ)];
		if(var7 != NULL && var7.material->getIsLiquid()) {
			Material var8 = var7.material;
   // Fog mode = GL11.GL_EXP � handled by shader
			if(var8 == Material::s_water) {
    Shaders::setFogDensity(0.1F);
			} else if(var8 == Material::s_lava) {
    Shaders::setFogDensity(2.0F);
			}
		} else {
   // Fog mode = GL11.GL_LINEAR � handled by shader
   // Fog start = this.farPlaneDistance / 4.0F � handled by shader
   // Fog end = this.farPlaneDistance � handled by shader
		}

  // Color material is shader-based on Xbox 360 (enabled)
  // Color material is shader-based on Xbox 360
}
