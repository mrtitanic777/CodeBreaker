#include "stdafx.h"
// Hand-written PORT override for RenderGlobal.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (output_alpha/RenderGlobal.cpp), which carries Alpha's
// actual world/chunk render logic but ~52 compile errors. Fixed the same way the WORKING
// indev RenderGlobal port solved them:
//   - GL immediate mode -> D3D9 (g_pd3dDevice) + Shaders world-matrix (pre-multiply) + Tesselator.
//   - Tesselator::getInstance() returns a REFERENCE; ".init()/.flush()" not "->".
//   - Java collection ".get/.size/.add" -> std::vector ops; element type is void* (store &obj).
//   - "(Type)x" pointer casts; Java static "A.b.c" -> "A::b->c" / "A::s_field".
//   - The per-chunk WorldRenderer arrays are arrays of OBJECTS (header type WorldRenderer*),
//     accessed with "." ; pointer locals take "&arr[i]".
//   - EntitySorter/RenderSorter compare void* (== WorldRenderer*); object arrays are sorted via
//     a small address-adapter functor; the void* vectors sort directly.
// This keeps Alpha's behavior (its sky/cloud/occlusion/sortAndRender logic is preserved) and uses
// indev only as the how-to-port reference. Compilation is the priority; pieces that can't port
// cleanly on this platform are marked "// PORT:".

#include "RenderGlobal.h"
#include "Common.h"
#include "SoundManager.h"
#include "AxisAlignedBB.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "ICamera.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "MovingObjectPosition.h"
#include "RenderBlocks.h"
#include "RenderEngine.h"
#include "RenderList.h"
#include "TileEntity.h"
#include "Vec3D.h"
#include "World.h"
#include "WorldProvider.h"
#include "WorldRenderer.h"
#include <algorithm>
#include <string>
#include <math.h>
#include "Block.h"
#include "BlockLeaves.h"
#include "EntityPlayerSP.h"
#include "GuiIngame.h"
#include "MathHelper.h"
#include "Random.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "TileEntityRenderer.h"
#include "EffectRenderer.h"
#include "EntityFX.h"
#include "EntityBubbleFX.h"
#include "EntityExplodeFX.h"
#include "EntityFlameFX.h"
#include "EntityLavaFX.h"
#include "EntityPortalFX.h"
#include "EntityReddustFX.h"
#include "EntitySlimeFX.h"
#include "EntitySmokeFX.h"
#include "EntitySorter.h"
#include "EntitySplashFX.h"
#include "GLAllocation.h"
#include "GameSettings.h"
#include "Item.h"
#include "RenderManager.h"
#include "RenderSorter.h"

// --------------------------------------------------------------------------------------------
// PORT: the per-chunk renderer arrays (m_field_1444_o / m_field_1445_n) are arrays of WorldRenderer
// OBJECTS (the header field type is WorldRenderer*). std::sort with EntitySorter needs to compare
// element ADDRESSES as void* (EntitySorter::compare casts void* -> WorldRenderer*). These adapters
// wrap the converted comparators so they sort an array of objects by element address, preserving the
// exact compare logic.
// --------------------------------------------------------------------------------------------
namespace {
	struct WRObjEntitySorter {
		EntitySorter s;
		WRObjEntitySorter(Entity* e) : s(e) {}
		bool operator()(WorldRenderer& a, WorldRenderer& b) {
			return s.compare((void*)&a, (void*)&b) < 0;
		}
	};
}

RenderGlobal::RenderGlobal()
	: worldObj(NULL)
	, m_renderEngine(NULL)
	, m_field_1445_n(NULL)
	, m_field_1444_o(NULL)
	, m_field_1443_p(0)
	, m_field_1442_q(0)
	, m_field_1441_r(0)
	, m_field_1440_s(0)
	, mc(NULL)
	, m_field_1438_u(NULL)
	, m_field_1437_v(NULL)
	, m_field_1436_w(false)
	, m_field_1435_x(0)
	, m_field_1434_y(0)
	, m_field_1433_z(0)
	, m_field_1432_A(0)
	, m_field_1431_B(0)
	, m_field_1430_C(0)
	, m_field_1429_D(0)
	, m_field_1428_E(0)
	, m_field_1427_F(0)
	, m_field_1426_G(0)
	, m_field_1425_H(-1)
	, m_field_1424_I(2)
	, m_field_1423_J(0)
	, m_field_1422_K(0)
	, m_field_1421_L(0)
	, m_field_1457_b(new int[50000])   // PORT: was new int[0xC350] -- 0xC350 == 50000; MSVC can't encode the char literal (C4566). Plain int[50000].
	, m_field_1456_c(GLAllocation::createDirectIntBuffer(64))
	, m_field_1420_M(0)
	, m_field_1419_N(0)
	, m_field_1418_O(0)
	, m_field_1417_P(0)
	, m_field_1416_Q(0)
	, m_field_1415_R()
	, m_field_1414_S(NULL)
	, m_field_1455_d(0)
	, m_field_1454_e(GLAllocation::generateDisplayLists(1))
	, m_field_1453_f(-9999.0f)
	, m_field_1452_g(-9999.0f)
	, m_field_1451_h(-9999.0f)
	, m_field_1450_i(0)
	, m_field_1449_j(0)
	, m_field_1445_n_count(0)
	, m_field_1444_o_count(0)
	, m_field_1457_b_count(50000)
	, m_field_1414_S_count(0)
{
	// PORT(array-literal field init): C++03 has no init-list array fill; alloc here.
	m_field_1414_S = new RenderList[4];
	m_field_1414_S_count = 4;
}

RenderGlobal::RenderGlobal(Minecraft* var1, RenderEngine* var2)
	: worldObj(NULL)
	, m_renderEngine(NULL)
	, m_field_1445_n(NULL)
	, m_field_1444_o(NULL)
	, m_field_1443_p(0)
	, m_field_1442_q(0)
	, m_field_1441_r(0)
	, m_field_1440_s(0)
	, mc(NULL)
	, m_field_1438_u(NULL)
	, m_field_1437_v(NULL)
	, m_field_1436_w(false)
	, m_field_1435_x(0)
	, m_field_1434_y(0)
	, m_field_1433_z(0)
	, m_field_1432_A(0)
	, m_field_1431_B(0)
	, m_field_1430_C(0)
	, m_field_1429_D(0)
	, m_field_1428_E(0)
	, m_field_1427_F(0)
	, m_field_1426_G(0)
	, m_field_1425_H(-1)
	, m_field_1424_I(2)
	, m_field_1423_J(0)
	, m_field_1422_K(0)
	, m_field_1421_L(0)
	, m_field_1457_b(new int[50000])   // PORT: see ctor above (0xC350 == 50000).
	, m_field_1456_c(GLAllocation::createDirectIntBuffer(64))
	, m_field_1420_M(0)
	, m_field_1419_N(0)
	, m_field_1418_O(0)
	, m_field_1417_P(0)
	, m_field_1416_Q(0)
	, m_field_1415_R()
	, m_field_1414_S(NULL)
	, m_field_1455_d(0)
	, m_field_1454_e(GLAllocation::generateDisplayLists(1))
	, m_field_1453_f(-9999.0f)
	, m_field_1452_g(-9999.0f)
	, m_field_1451_h(-9999.0f)
	, m_field_1450_i(0)
	, m_field_1449_j(0)
	, m_field_1445_n_count(0)
	, m_field_1444_o_count(0)
	, m_field_1457_b_count(50000)
	, m_field_1414_S_count(0)
{
	m_field_1414_S = new RenderList[4];
	m_field_1414_S_count = 4;
	mc = var1;
	m_renderEngine = var2;
	unsigned char var3 = 64;
	m_field_1440_s = GLAllocation::generateDisplayLists(var3 * var3 * var3 * 3);
	// PORT: GL occlusion not available on Xbox 360. // was: var1->func_6251_l()->checkARBOcclusion();
	m_field_1436_w = false;
	if(m_field_1436_w) {
		m_field_1437_v = GLAllocation::createDirectIntBuffer(var3 * var3 * var3);
		// PORT: D3D9 no occlusion queries on 360.
	}

	m_field_1434_y = GLAllocation::generateDisplayLists(3);
	// PORT(stars/sky lists): the converter emitted the star-field + horizon-plane "display list"
	// build here in the constructor. That mutated Shaders' WorldMatrix and flushed the Tesselator
	// OUTSIDE any BeginScene (drew nothing / risked an out-of-scene flush crash) and there is no
	// display-list cache to record into. The reference port builds the sky/stars lazily and draws
	// them inline in func_4142_a (renderSky). Nothing to build at construction time.
	m_field_1433_z = m_field_1434_y + 1;
	m_field_1432_A = m_field_1434_y + 2;
}

RenderGlobal::~RenderGlobal() {
	worldObj = NULL;
	m_renderEngine = NULL;
	delete[] m_field_1445_n;
	m_field_1445_n = NULL;
	delete[] m_field_1444_o;
	m_field_1444_o = NULL;
	mc = NULL;
	m_field_1438_u = NULL;
	delete[] m_field_1457_b;
	m_field_1457_b = NULL;
	delete[] m_field_1414_S;
	m_field_1414_S = NULL;
}

void RenderGlobal::func_950_f() {
	// PORT(stars): built lazily + drawn inline in func_4142_a; this prebuild is a no-op here
	// (no display-list cache; flushing outside BeginScene is invalid). See func_4142_a.
}

void RenderGlobal::func_946_a(World* var1) {
	if(worldObj != NULL) {
		worldObj->removeWorldAccess(this);
	}

	m_field_1453_f = -9999.0f;
	m_field_1452_g = -9999.0f;
	m_field_1451_h = -9999.0f;
	RenderManager::instance->func_852_a(var1);
	worldObj = var1;
	m_field_1438_u = new RenderBlocks(var1);
	if(var1 != NULL) {
		var1->addWorldAccess(this);
		func_958_a();
	}
}

void RenderGlobal::func_958_a() {
	Block::s_leaves->setGraphicsLevel(mc->m_gameSettings->m_fancyGraphics);
	m_field_1425_H = mc->m_gameSettings->m_renderDistance;
	int var1;
	if(m_field_1444_o != NULL) {
		for(var1 = 0; var1 < m_field_1443_p * m_field_1442_q * m_field_1441_r; ++var1) {
			m_field_1444_o[var1].func_1204_c();
		}
	}

	var1 = 64 << (3 - m_field_1425_H);
	if(var1 > 400) {
		var1 = 400;
	}

	m_field_1443_p = var1 / 16 + 1;
	m_field_1442_q = 8;
	m_field_1441_r = var1 / 16 + 1;

	delete[] m_field_1444_o;
	delete[] m_field_1445_n;
	int _total = m_field_1443_p * m_field_1442_q * m_field_1441_r;
	m_field_1444_o = new WorldRenderer[_total];
	m_field_1444_o_count = _total;
	m_field_1445_n = new WorldRenderer[_total];
	m_field_1445_n_count = _total;
	int var2 = 0;
	int var3 = 0;
	m_field_1431_B = 0;
	m_field_1430_C = 0;
	m_field_1429_D = 0;
	m_field_1428_E = m_field_1443_p;
	m_field_1427_F = m_field_1442_q;
	m_field_1426_G = m_field_1441_r;

	size_t var4;
	for(var4 = 0; var4 < m_field_1446_m.size(); ++var4) {
		((WorldRenderer*)m_field_1446_m[var4])->needsUpdate = false;
	}

	m_field_1446_m.clear();
	m_field_1458_a.clear();

	int vx, var5, var6;
	for(vx = 0; vx < m_field_1443_p; ++vx) {
		for(var5 = 0; var5 < m_field_1442_q; ++var5) {
			for(var6 = 0; var6 < m_field_1441_r; ++var6) {
				int _idx = (var6 * m_field_1442_q + var5) * m_field_1443_p + vx;
				m_field_1444_o[_idx] = WorldRenderer(worldObj, m_field_1458_a, vx * 16, var5 * 16, var6 * 16, 16, m_field_1440_s + var2);
				if(m_field_1436_w && m_field_1437_v != NULL) {
					m_field_1444_o[_idx].m_field_1732_z = m_field_1437_v[var3];
				}

				m_field_1444_o[_idx].m_field_1733_y = false;
				m_field_1444_o[_idx].m_field_1734_x = true;
				m_field_1444_o[_idx].m_field_1749_o = true;
				m_field_1444_o[_idx].m_field_1735_w = var3++;
				m_field_1444_o[_idx].MarkDirty();
				m_field_1445_n[_idx] = m_field_1444_o[_idx];
				m_field_1446_m.push_back((void*)&m_field_1444_o[_idx]);
				var2 += 3;
			}
		}
	}

	if(worldObj != NULL) {
		EntityPlayerSP* var7 = mc->thePlayer;
		func_956_b((int)floor(var7->posX), (int)floor(var7->posY), (int)floor(var7->posZ));
		std::sort(m_field_1445_n, m_field_1445_n + m_field_1445_n_count, WRObjEntitySorter((Entity*)var7));
	}

	m_field_1424_I = 2;
}

void RenderGlobal::func_951_a(Vec3D* var1, ICamera* var2, float var3) {
	if(m_field_1424_I > 0) {
		--m_field_1424_I;
	} else {
		TileEntityRenderer::instance->setRenderingContext(worldObj, m_renderEngine, mc->fontRenderer, mc->thePlayer, var3);
		RenderManager::instance->func_857_a(worldObj, m_renderEngine, mc->fontRenderer, mc->thePlayer, mc->m_gameSettings, var3);
		m_field_1423_J = 0;
		m_field_1422_K = 0;
		m_field_1421_L = 0;
		EntityPlayerSP* var4 = mc->thePlayer;
		RenderManager::s_field_1232_b = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var3;
		RenderManager::s_field_1231_c = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var3;
		RenderManager::s_field_1230_d = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var3;
		TileEntityRenderer::s_staticPlayerX = var4->lastTickPosX + (var4->posX - var4->lastTickPosX) * (double)var3;
		TileEntityRenderer::s_staticPlayerY = var4->lastTickPosY + (var4->posY - var4->lastTickPosY) * (double)var3;
		TileEntityRenderer::s_staticPlayerZ = var4->lastTickPosZ + (var4->posZ - var4->lastTickPosZ) * (double)var3;
		std::vector<void*> var5 = worldObj->func_658_i();
		m_field_1423_J = (int)var5.size();

		size_t var6;
		for(var6 = 0; var6 < var5.size(); ++var6) {
			Entity* var7 = (Entity*)var5[var6];
			if(var7->func_390_a(var1) && var2->func_342_a(var7->boundingBox) && (var7 != (Entity*)mc->thePlayer || mc->m_gameSettings->m_thirdPersonView)) {
				++m_field_1422_K;
				RenderManager::instance->func_854_a(var7, var3);
			}
		}

		for(var6 = 0; var6 < m_field_1458_a.size(); ++var6) {
			TileEntityRenderer::instance->renderTileEntity((TileEntity*)m_field_1458_a[var6], var3);
		}
	}
}

const std::string& RenderGlobal::func_953_b() {
	// PORT: header returns const std::string& -- back it with function-local static storage so the
	// reference outlives the call (the converter returned a dangling temporary).
	static std::string _s;
	_s = "C: " + toStr(m_field_1417_P) + "/" + toStr(m_field_1420_M) + ". F: " + toStr(m_field_1419_N) + ", O: " + toStr(m_field_1418_O) + ", E: " + toStr(m_field_1416_Q);
	return _s;
}

const std::string& RenderGlobal::func_957_c() {
	static std::string _s;
	_s = "E: " + toStr(m_field_1422_K) + "/" + toStr(m_field_1423_J) + ". B: " + toStr(m_field_1421_L) + ", I: " + toStr(m_field_1423_J - m_field_1421_L - m_field_1422_K);
	return _s;
}

void RenderGlobal::func_956_b(int var1, int var2, int var3) {
	var1 -= 8;
	var2 -= 8;
	var3 -= 8;
	m_field_1431_B = 2147483647;
	m_field_1430_C = 2147483647;
	m_field_1429_D = 2147483647;
	m_field_1428_E = (-2147483647 - 1);
	m_field_1427_F = (-2147483647 - 1);
	m_field_1426_G = (-2147483647 - 1);
	int var4 = m_field_1443_p * 16;
	int var5 = var4 / 2;

	for(int var6 = 0; var6 < m_field_1443_p; ++var6) {
		int var7 = var6 * 16;
		int var8 = var7 + var5 - var1;
		if(var8 < 0) {
			var8 -= var4 - 1;
		}

		var8 /= var4;
		var7 -= var8 * var4;
		if(var7 < m_field_1431_B) {
			m_field_1431_B = var7;
		}

		if(var7 > m_field_1428_E) {
			m_field_1428_E = var7;
		}

		for(int var9 = 0; var9 < m_field_1441_r; ++var9) {
			int var10 = var9 * 16;
			int var11 = var10 + var5 - var3;
			if(var11 < 0) {
				var11 -= var4 - 1;
			}

			var11 /= var4;
			var10 -= var11 * var4;
			if(var10 < m_field_1429_D) {
				m_field_1429_D = var10;
			}

			if(var10 > m_field_1426_G) {
				m_field_1426_G = var10;
			}

			for(int var12 = 0; var12 < m_field_1442_q; ++var12) {
				int var13 = var12 * 16;
				if(var13 < m_field_1430_C) {
					m_field_1430_C = var13;
				}

				if(var13 > m_field_1427_F) {
					m_field_1427_F = var13;
				}

				WorldRenderer* var14 = &m_field_1444_o[(var9 * m_field_1442_q + var12) * m_field_1443_p + var6];
				bool var15 = var14->needsUpdate;
				var14->func_1197_a(var7, var13, var10);
				if(!var15 && var14->needsUpdate) {
					m_field_1446_m.push_back((void*)var14);
				}
			}
		}
	}
}

int RenderGlobal::func_943_a(EntityPlayer* var1, int var2, double var3) {
	if(mc->m_gameSettings->m_renderDistance != m_field_1425_H) {
		func_958_a();
	}

	if(var2 == 0) {
		m_field_1420_M = 0;
		m_field_1419_N = 0;
		m_field_1418_O = 0;
		m_field_1417_P = 0;
		m_field_1416_Q = 0;
	}

	double var5 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * var3;
	double var7 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * var3;
	double var9 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * var3;
	double var11 = var1->posX - m_field_1453_f;
	double var13 = var1->posY - m_field_1452_g;
	double var15 = var1->posZ - m_field_1451_h;
	if(var11 * var11 + var13 * var13 + var15 * var15 > 16.0f) {
		m_field_1453_f = var1->posX;
		m_field_1452_g = var1->posY;
		m_field_1451_h = var1->posZ;
		func_956_b((int)floor(var1->posX), (int)floor(var1->posY), (int)floor(var1->posZ));
		std::sort(m_field_1445_n, m_field_1445_n + m_field_1445_n_count, WRObjEntitySorter((Entity*)var1));
	}

	unsigned char var17 = 0;
	int var33;
	if(m_field_1436_w && !mc->m_gameSettings->m_anaglyph && var2 == 0) {
		int var18 = 0;
		int var19 = 16;
		func_962_a(var18, var19);

		for(int var20 = var18; var20 < var19; ++var20) {
			m_field_1445_n[var20].m_field_1734_x = true;
		}

		var33 = var17 + func_952_a(var18, var19, var2, var3);

		do {
			int var34 = var19;
			var19 *= 2;
			if(var19 > m_field_1445_n_count) {
				var19 = m_field_1443_p * m_field_1442_q * m_field_1441_r;
			}

			g_pd3dDevice->SetTexture(0, NULL);
			// Lighting is shader-based on Xbox 360 (disabled)
			g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			Shaders::setFogEnabled(false);
			g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			func_962_a(var34, var19);
			D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			float var35 = 0.0F;
			float var21 = 0.0F;
			float var22 = 0.0F;

			for(int var23 = var34; var23 < var19; ++var23) {
				if(m_field_1445_n[var23].func_1196_e()) {
					m_field_1445_n[var23].m_field_1749_o = false;
				} else {
					if(!m_field_1445_n[var23].m_field_1749_o) {
						m_field_1445_n[var23].m_field_1734_x = true;
					}

					if(m_field_1445_n[var23].m_field_1749_o && !m_field_1445_n[var23].m_field_1733_y) {
						float var24 = sqrtf(m_field_1445_n[var23].func_1202_a((Entity*)var1));
						int var25 = (int)(1.0F + var24 / 128.0F);
						if(m_field_1435_x % var25 == var23 % var25) {
							WorldRenderer* var26 = &m_field_1445_n[var23];
							float var27 = (float)((double)var26->m_field_1755_i - var5);
							float var28 = (float)((double)var26->m_field_1754_j - var7);
							float var29 = (float)((double)var26->m_field_1753_k - var9);
							float var30 = var27 - var35;
							float var31 = var28 - var21;
							float var32 = var29 - var22;
							if(var30 != 0.0F || var31 != 0.0F || var32 != 0.0F) {
								D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var30, var31, var32);
								D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
								Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants();
								var35 += var30;
								var21 += var31;
								var22 += var32;
							}

							// PORT: D3D9 no occlusion queries on 360.
							m_field_1445_n[var23].func_1201_d();
							m_field_1445_n[var23].m_field_1733_y = true;
						}
					}
				}
			}

			Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
			g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			g_pd3dDevice->SetTexture(0, g_terrainTexture);
			g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			Shaders::setFogEnabled(true);
			var33 += func_952_a(var34, var19, var2, var3);
		} while(var19 < m_field_1443_p * m_field_1442_q * m_field_1441_r);
	} else {
		var33 = var17 + func_952_a(0, m_field_1445_n_count, var2, var3);
	}

	return var33;
}

void RenderGlobal::func_962_a(int var1, int var2) {
	for(int var3 = var1; var3 < var2; ++var3) {
		if(m_field_1445_n[var3].m_field_1733_y) {
			// PORT: occlusion query result not available on Xbox 360 -- treat as visible (no cull).
		}
	}
}

int RenderGlobal::func_952_a(int var1, int var2, int var3, double var4) {
	m_field_1415_R.clear();
	int var6 = 0;

	for(int var7 = var1; var7 < var2; ++var7) {
		if(var3 == 0) {
			++m_field_1420_M;
			if(m_field_1445_n[var7].m_field_1748_p[var3]) {
				++m_field_1416_Q;
			} else if(!m_field_1445_n[var7].m_field_1749_o) {
				++m_field_1419_N;
			} else if(m_field_1436_w && !m_field_1445_n[var7].m_field_1734_x) {
				++m_field_1418_O;
			} else {
				++m_field_1417_P;
			}
		}

		if(!m_field_1445_n[var7].m_field_1748_p[var3] && m_field_1445_n[var7].m_field_1749_o && m_field_1445_n[var7].m_field_1734_x) {
			int var8 = m_field_1445_n[var7].func_1200_a(var3);
			if(var8 >= 0) {
				m_field_1415_R.push_back((void*)&m_field_1445_n[var7]);
				++var6;
			}
		}
	}

	EntityPlayerSP* var19 = mc->thePlayer;
	double var20 = var19->lastTickPosX + (var19->posX - var19->lastTickPosX) * var4;
	double var10 = var19->lastTickPosY + (var19->posY - var19->lastTickPosY) * var4;
	double var12 = var19->lastTickPosZ + (var19->posZ - var19->lastTickPosZ) * var4;
	int var14 = 0;

	int var15;
	for(var15 = 0; var15 < 4; ++var15) {
		m_field_1414_S[var15].func_859_b();
	}

	for(var15 = 0; var15 < (int)m_field_1415_R.size(); ++var15) {
		WorldRenderer* var16 = (WorldRenderer*)m_field_1415_R[var15];
		int var17 = -1;

		for(int var18 = 0; var18 < var14; ++var18) {
			if(m_field_1414_S[var18].func_862_a(var16->m_field_1755_i, var16->m_field_1754_j, var16->m_field_1753_k)) {
				var17 = var18;
			}
		}

		if(var17 < 0) {
			var17 = var14++;
			m_field_1414_S[var17].func_861_a(var16->m_field_1755_i, var16->m_field_1754_j, var16->m_field_1753_k, var20, var10, var12);
		}

		m_field_1414_S[var17].func_858_a(var16->func_1200_a(var3));
	}

	func_944_a(var3, var4);
	return var6;
}

void RenderGlobal::func_944_a(int var1, double var2) {
	// PORT: GL display lists / RenderList replay aren't available on Xbox 360. The per-chunk geometry
	// is drawn from its cached vertex buffer (WorldRenderer::func_1200_a builds it; the chunk render
	// pass draws it). RenderList here just collected GL list ids to batch -- a no-op on this platform.
	for(int var4 = 0; var4 < 4; ++var4) {
		m_field_1414_S[var4].func_860_a();
	}
}

void RenderGlobal::func_945_d() {
	++m_field_1435_x;
}

void RenderGlobal::func_4142_a(float var1) {
	if(!mc->theWorld->m_worldProvider->m_field_4220_c) {
		g_pd3dDevice->SetTexture(0, NULL);
		Vec3D* var2 = worldObj->func_4079_a(mc->thePlayer, var1);
		float var3 = (float)var2->xCoord;
		float var4 = (float)var2->yCoord;
		float var5 = (float)var2->zCoord;
		float var7;
		float var8;
		if(mc->m_gameSettings->m_anaglyph) {
			float var6 = (var3 * 30.0F + var4 * 59.0F + var5 * 11.0F) / 100.0F;
			var7 = (var3 * 30.0F + var4 * 70.0F) / 100.0F;
			var8 = (var3 * 30.0F + var5 * 70.0F) / 100.0F;
			var3 = var6;
			var4 = var7;
			var5 = var8;
		}

		Tesselator& var14 = Tesselator::getInstance();
		g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// Sky-color cap plane (was a prebuilt GL list 'field_1433_z'). Build + flush inline.
		Shaders::setFogEnabled(true);
		var14.init();
		var14.color3f(var3, var4, var5);
		{
			float _y = 16.0F;
			unsigned char _sz = 64;
			int _cnt = 256 / _sz + 2;
			for(int _x = -_sz * _cnt; _x <= _sz * _cnt; _x += _sz) {
				for(int _z = -_sz * _cnt; _z <= _sz * _cnt; _z += _sz) {
					var14.vertex((float)(_x + 0), _y, (float)(_z + 0));
					var14.vertex((float)(_x + _sz), _y, (float)(_z + 0));
					var14.vertex((float)(_x + _sz), _y, (float)(_z + _sz));
					var14.vertex((float)(_x + 0), _y, (float)(_z + _sz));
				}
			}
		}
		var14.flush();

		Shaders::setFogEnabled(false);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		float* var15 = worldObj->m_worldProvider->func_4097_b(worldObj->getCelestialAngle(var1), var1);
		float var11;
		if(var15 != NULL) {
			g_pd3dDevice->SetTexture(0, NULL);
			D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			{ D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (90.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var8 = worldObj->getCelestialAngle(var1);
			{ D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (var8 > 0.5F ? 180.0F : 0.0F)*DEG_TO_RAD);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var14.init(6);
			var14.color4f(var15[0], var15[1], var15[2], var15[3]);
			var14.vertex(0.0f, 100.0f, 0.0f);
			unsigned char var9 = 16;
			var14.color4f(var15[0], var15[1], var15[2], 0.0F);

			for(int var10 = 0; var10 <= var9; ++var10) {
				var11 = (float)var10 * 3.14159265358979323846f * 2.0F / (float)var9;
				float var12 = sinf(var11);
				float var13 = cosf(var11);
				var14.vertex((float)(var12 * 120.0F), (float)(var13 * 120.0F), (float)(-var13 * 40.0F * var15[3]));
			}

			var14.flush();
			Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}

		g_pd3dDevice->SetTexture(0, g_terrainTexture);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		D3DXMATRIX _savedMatrix2 = Shaders::getWorldMatrix();
		var7 = 0.0F;
		var8 = 0.0F;
		float var16 = 0.0F;
		Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, var7, var8, var16);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (0.0F)*DEG_TO_RAD);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (this->worldObj->getCelestialAngle(var1) * 360.0F)*DEG_TO_RAD);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var17 = 30.0F;
		g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/terrain/sun.png")));
		var14.init();
		var14.vertexUV((float)(-var17), 100.0f, (float)(-var17), 0.0f, 0.0f);
		var14.vertexUV((float)var17, 100.0f, (float)(-var17), 1.0f, 0.0f);
		var14.vertexUV((float)var17, 100.0f, (float)var17, 1.0f, 1.0f);
		var14.vertexUV((float)(-var17), 100.0f, (float)var17, 0.0f, 1.0f);
		var14.flush();
		var17 = 20.0F;
		g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/terrain/moon.png")));
		var14.init();
		var14.vertexUV((float)(-var17), -100.0f, (float)var17, 1.0f, 1.0f);
		var14.vertexUV((float)var17, -100.0f, (float)var17, 0.0f, 1.0f);
		var14.vertexUV((float)var17, -100.0f, (float)(-var17), 0.0f, 0.0f);
		var14.vertexUV((float)(-var17), -100.0f, (float)(-var17), 1.0f, 0.0f);
		var14.flush();
		g_pd3dDevice->SetTexture(0, NULL);
		var11 = worldObj->func_679_f(var1);
		if(var11 > 0.0F) {
			// PORT(stars): the GL star-field list ('field_1434_y') has no display-list cache here.
			// Build the 1500 star quads once (same seed Random(10842L) + per-star rotation as Java)
			// and draw them inline while the WorldMatrix is still in celestial-rotated space (so the
			// GPU applies the spin for free, like the moon quad). Additive blend + ZWRITE off are set.
			static bool s_starsBuilt = false;
			static float s_starQuads[1500][4][3];
			static int s_starQuadCount = 0;
			if(!s_starsBuilt) {
				Random* sr = new Random(10842ULL);
				int _qi = 0;
				for(int _si = 0; _si < 1500; ++_si) {
					double _ax = (double)(sr->nextFloat() * 2.0F - 1.0F);
					double _ay = (double)(sr->nextFloat() * 2.0F - 1.0F);
					double _az = (double)(sr->nextFloat() * 2.0F - 1.0F);
					double _sz = (double)(0.25F + sr->nextFloat() * 0.25F);
					double _d = _ax * _ax + _ay * _ay + _az * _az;
					if(_d < 1.0 && _d > 0.01) {
						_d = 1.0 / sqrt(_d);
						_ax *= _d; _ay *= _d; _az *= _d;
						double _cx = _ax * 100.0; double _cy = _ay * 100.0; double _cz = _az * 100.0;
						double _b0 = atan2(_ax, _az);
						double _sb = sin(_b0); double _cb = cos(_b0);
						double _b1 = atan2(sqrt(_ax * _ax + _az * _az), _ay);
						double _sc = sin(_b1); double _cc = cos(_b1);
						double _rot = sr->nextDouble() * 3.14159265358979323846 * 2.0;
						double _sr2 = sin(_rot); double _cr2 = cos(_rot);
						for(int _c = 0; _c < 4; ++_c) {
							double _u0 = 0.0;
							double _u1 = (double)((_c & 2) - 1) * _sz;
							double _u2 = (double)((_c + 1 & 2) - 1) * _sz;
							double _v0 = _u1 * _cr2 - _u2 * _sr2;
							double _v1 = _u2 * _cr2 + _u1 * _sr2;
							double _w0 = _v0 * _sc + _u0 * _cc;
							double _w1 = _u0 * _sc - _v0 * _cc;
							double _x0 = _w1 * _sb - _v1 * _cb;
							double _z0 = _v1 * _sb + _w1 * _cb;
							s_starQuads[_qi][_c][0] = (float)(_cx + _x0);
							s_starQuads[_qi][_c][1] = (float)(_cy + _w0);
							s_starQuads[_qi][_c][2] = (float)(_cz + _z0);
						}
						++_qi;
					}
				}
				delete sr;
				s_starQuadCount = _qi;
				s_starsBuilt = true;
			}
			Tesselator::getInstance().color4f(var11, var11, var11, var11);
			var14.init();
			for(int _si = 0; _si < s_starQuadCount; ++_si) {
				var14.vertex(s_starQuads[_si][0][0], s_starQuads[_si][0][1], s_starQuads[_si][0][2]);
				var14.vertex(s_starQuads[_si][1][0], s_starQuads[_si][1][1], s_starQuads[_si][1][2]);
				var14.vertex(s_starQuads[_si][2][0], s_starQuads[_si][2][1], s_starQuads[_si][2][2]);
				var14.vertex(s_starQuads[_si][3][0], s_starQuads[_si][3][1], s_starQuads[_si][3][2]);
			}
			var14.flush();
		}

		Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		Shaders::setFogEnabled(true);
		Shaders::setWorldMatrix(_savedMatrix2); Shaders::updateShaderConstants();

		// Void/bottom cap plane (was prebuilt GL list 'field_1432_A'). Build + flush inline.
		Tesselator::getInstance().color3f(var3 * 0.2F + 0.04F, var4 * 0.2F + 0.04F, var5 * 0.6F + 0.1F);
		g_pd3dDevice->SetTexture(0, NULL);
		var14.init();
		var14.color3f(var3 * 0.2F + 0.04F, var4 * 0.2F + 0.04F, var5 * 0.6F + 0.1F);
		{
			float _y = -16.0F;
			unsigned char _sz = 64;
			int _cnt = 256 / _sz + 2;
			for(int _x = -_sz * _cnt; _x <= _sz * _cnt; _x += _sz) {
				for(int _z = -_sz * _cnt; _z <= _sz * _cnt; _z += _sz) {
					var14.vertex((float)(_x + _sz), _y, (float)(_z + 0));
					var14.vertex((float)(_x + 0), _y, (float)(_z + 0));
					var14.vertex((float)(_x + 0), _y, (float)(_z + _sz));
					var14.vertex((float)(_x + _sz), _y, (float)(_z + _sz));
				}
			}
		}
		var14.flush();
		Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetTexture(0, g_terrainTexture);
		g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void RenderGlobal::func_4141_b(float var1) {
	if(!mc->theWorld->m_worldProvider->m_field_4220_c) {
		if(mc->m_gameSettings->m_fancyGraphics) {
			func_6510_c(var1);
		} else {
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			float var2 = (float)(mc->thePlayer->lastTickPosY + (mc->thePlayer->posY - mc->thePlayer->lastTickPosY) * (double)var1);
			unsigned char var3 = 32;
			int var4 = 256 / var3;
			Tesselator& var5 = Tesselator::getInstance();
			g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/environment/clouds.png")));
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			Vec3D* var6 = worldObj->func_628_d(var1);
			float var7 = (float)var6->xCoord;
			float var8 = (float)var6->yCoord;
			float var9 = (float)var6->zCoord;
			float var10;
			if(mc->m_gameSettings->m_anaglyph) {
				var10 = (var7 * 30.0F + var8 * 59.0F + var9 * 11.0F) / 100.0F;
				float var11 = (var7 * 30.0F + var8 * 70.0F) / 100.0F;
				float var12 = (var7 * 30.0F + var9 * 70.0F) / 100.0F;
				var7 = var10;
				var8 = var11;
				var9 = var12;
			}

			var10 = 0.5F / 1024.0F;
			double var22 = mc->thePlayer->prevPosX + (mc->thePlayer->posX - mc->thePlayer->prevPosX) * (double)var1 + (double)(((float)m_field_1435_x + var1) * 0.03F);
			double var13 = mc->thePlayer->prevPosZ + (mc->thePlayer->posZ - mc->thePlayer->prevPosZ) * (double)var1;
			int var15 = (int)floor(var22 / 2048.0f);
			int var16 = (int)floor(var13 / 2048.0f);
			var22 -= (double)(var15 * 2048);
			var13 -= (double)(var16 * 2048);
			float var17 = 120.0F - var2 + 0.33F;
			float var18 = (float)(var22 * (double)var10);
			float var19 = (float)(var13 * (double)var10);
			var5.init();
			var5.color4f(var7, var8, var9, 0.8F);

			for(int var20 = -var3 * var4; var20 < var3 * var4; var20 += var3) {
				for(int var21 = -var3 * var4; var21 < var3 * var4; var21 += var3) {
					var5.vertexUV((float)(var20 + 0), var17, (float)(var21 + var3), (float)(var20 + 0) * var10 + var18, (float)(var21 + var3) * var10 + var19);
					var5.vertexUV((float)(var20 + var3), var17, (float)(var21 + var3), (float)(var20 + var3) * var10 + var18, (float)(var21 + var3) * var10 + var19);
					var5.vertexUV((float)(var20 + var3), var17, (float)(var21 + 0), (float)(var20 + var3) * var10 + var18, (float)(var21 + 0) * var10 + var19);
					var5.vertexUV((float)(var20 + 0), var17, (float)(var21 + 0), (float)(var20 + 0) * var10 + var18, (float)(var21 + 0) * var10 + var19);
				}
			}

			var5.flush();
			Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		}
	}
}

void RenderGlobal::func_6510_c(float var1) {
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	float var2 = (float)(mc->thePlayer->lastTickPosY + (mc->thePlayer->posY - mc->thePlayer->lastTickPosY) * (double)var1);
	Tesselator& var3 = Tesselator::getInstance();
	float var4 = 12.0F;
	float var5 = 4.0F;
	double var6 = (mc->thePlayer->prevPosX + (mc->thePlayer->posX - mc->thePlayer->prevPosX) * (double)var1 + (double)(((float)m_field_1435_x + var1) * 0.03F)) / (double)var4;
	double var8 = (mc->thePlayer->prevPosZ + (mc->thePlayer->posZ - mc->thePlayer->prevPosZ) * (double)var1) / (double)var4 + (double)0.33F;
	float var10 = 108.0F - var2 + 0.33F;
	int var11 = (int)floor(var6 / 2048.0f);
	int var12 = (int)floor(var8 / 2048.0f);
	var6 -= (double)(var11 * 2048);
	var8 -= (double)(var12 * 2048);
	g_pd3dDevice->SetTexture(0, Textures::getById(this->m_renderEngine->getTexture("/environment/clouds.png")));
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Vec3D* var13 = worldObj->func_628_d(var1);
	float var14 = (float)var13->xCoord;
	float var15 = (float)var13->yCoord;
	float var16 = (float)var13->zCoord;
	float var17;
	float var18;
	float var19;
	if(mc->m_gameSettings->m_anaglyph) {
		var17 = (var14 * 30.0F + var15 * 59.0F + var16 * 11.0F) / 100.0F;
		var18 = (var14 * 30.0F + var15 * 70.0F) / 100.0F;
		var19 = (var14 * 30.0F + var16 * 70.0F) / 100.0F;
		var14 = var17;
		var15 = var18;
		var16 = var19;
	}

	var17 = (float)(var6 * 0.0f);
	var18 = (float)(var8 * 0.0f);
	var19 = 0.00390625F;
	var17 = (float)(int)floor(var6) * var19;
	var18 = (float)(int)floor(var8) * var19;
	float var20 = (float)(var6 - (double)(int)floor(var6));
	float var21 = (float)(var8 - (double)(int)floor(var8));
	unsigned char var22 = 8;
	unsigned char var23 = 3;
	float var24 = 1.0F / 1024.0F;
	{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var4, 1.0F, var4);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

	for(int var25 = 0; var25 < 2; ++var25) {
		if(var25 == 0) {
			g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0);
		} else {
			g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
		}

		for(int var26 = -var23 + 1; var26 <= var23; ++var26) {
			for(int var27 = -var23 + 1; var27 <= var23; ++var27) {
				var3.init();
				float var28 = (float)(var26 * var22);
				float var29 = (float)(var27 * var22);
				float var30 = var28 - var20;
				float var31 = var29 - var21;
				if(var10 > -var5 - 1.0F) {
					var3.color4f(var14 * 0.7F, var15 * 0.7F, var16 * 0.7F, 0.8F);
					var3.normal(0.0F, -1.0F, 0.0F);
					var3.vertexUV((var30 + 0.0F), (var10 + 0.0F), (var31 + (float)var22), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
					var3.vertexUV((var30 + (float)var22), (var10 + 0.0F), (var31 + (float)var22), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
					var3.vertexUV((var30 + (float)var22), (var10 + 0.0F), (var31 + 0.0F), ((var28 + (float)var22) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
					var3.vertexUV((var30 + 0.0F), (var10 + 0.0F), (var31 + 0.0F), ((var28 + 0.0F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
				}

				if(var10 <= var5 + 1.0F) {
					var3.color4f(var14, var15, var16, 0.8F);
					var3.normal(0.0F, 1.0F, 0.0F);
					var3.vertexUV((var30 + 0.0F), (var10 + var5 - var24), (var31 + (float)var22), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
					var3.vertexUV((var30 + (float)var22), (var10 + var5 - var24), (var31 + (float)var22), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
					var3.vertexUV((var30 + (float)var22), (var10 + var5 - var24), (var31 + 0.0F), ((var28 + (float)var22) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
					var3.vertexUV((var30 + 0.0F), (var10 + var5 - var24), (var31 + 0.0F), ((var28 + 0.0F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
				}

				var3.color4f(var14 * 0.9F, var15 * 0.9F, var16 * 0.9F, 0.8F);
				int var32;
				if(var26 > -1) {
					var3.normal(-1.0F, 0.0F, 0.0F);

					for(var32 = 0; var32 < var22; ++var32) {
						var3.vertexUV((var30 + (float)var32 + 0.0F), (var10 + 0.0F), (var31 + (float)var22), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 0.0F), (var10 + var5), (var31 + (float)var22), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 0.0F), (var10 + var5), (var31 + 0.0F), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 0.0F), (var10 + 0.0F), (var31 + 0.0F), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
					}
				}

				if(var26 <= 1) {
					var3.normal(1.0F, 0.0F, 0.0F);

					for(var32 = 0; var32 < var22; ++var32) {
						var3.vertexUV((var30 + (float)var32 + 1.0F - var24), (var10 + 0.0F), (var31 + (float)var22), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 1.0F - var24), (var10 + var5), (var31 + (float)var22), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + (float)var22) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 1.0F - var24), (var10 + var5), (var31 + 0.0F), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
						var3.vertexUV((var30 + (float)var32 + 1.0F - var24), (var10 + 0.0F), (var31 + 0.0F), ((var28 + (float)var32 + 0.5F) * var19 + var17), ((var29 + 0.0F) * var19 + var18));
					}
				}

				var3.color4f(var14 * 0.8F, var15 * 0.8F, var16 * 0.8F, 0.8F);
				if(var27 > -1) {
					var3.normal(0.0F, 0.0F, -1.0F);

					for(var32 = 0; var32 < var22; ++var32) {
						var3.vertexUV((var30 + 0.0F), (var10 + var5), (var31 + (float)var32 + 0.0F), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + (float)var22), (var10 + var5), (var31 + (float)var32 + 0.0F), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + (float)var22), (var10 + 0.0F), (var31 + (float)var32 + 0.0F), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + 0.0F), (var10 + 0.0F), (var31 + (float)var32 + 0.0F), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
					}
				}

				if(var27 <= 1) {
					var3.normal(0.0F, 0.0F, 1.0F);

					for(var32 = 0; var32 < var22; ++var32) {
						var3.vertexUV((var30 + 0.0F), (var10 + var5), (var31 + (float)var32 + 1.0F - var24), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + (float)var22), (var10 + var5), (var31 + (float)var32 + 1.0F - var24), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + (float)var22), (var10 + 0.0F), (var31 + (float)var32 + 1.0F - var24), ((var28 + (float)var22) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
						var3.vertexUV((var30 + 0.0F), (var10 + 0.0F), (var31 + (float)var32 + 1.0F - var24), ((var28 + 0.0F) * var19 + var17), ((var29 + (float)var32 + 0.5F) * var19 + var18));
					}
				}

				var3.flush();
			}
		}
	}

	Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

bool RenderGlobal::func_948_a(EntityPlayer* var1, bool var2) {
	std::sort(m_field_1446_m.begin(), m_field_1446_m.end(), RenderSorter(var1));
	int var3 = (int)m_field_1446_m.size() - 1;
	int var4 = (int)m_field_1446_m.size();

	for(int var5 = 0; var5 < var4; ++var5) {
		WorldRenderer* var6 = (WorldRenderer*)m_field_1446_m[var3 - var5];
		if(!var2) {
			if(var6->func_1202_a((Entity*)var1) > 1024.0F) {
				if(var6->m_field_1749_o) {
					if(var5 >= 3) {
						return false;
					}
				} else if(var5 >= 1) {
					return false;
				}
			}
		} else if(!var6->m_field_1749_o) {
			continue;
		}

		var6->func_1198_a();
		std::vector<void*>::iterator _removeIt = std::find(m_field_1446_m.begin(), m_field_1446_m.end(), (void*)var6);
		if (_removeIt != m_field_1446_m.end()) m_field_1446_m.erase(_removeIt);
		var6->needsUpdate = false;
	}

	return m_field_1446_m.size() == 0;
}

void RenderGlobal::func_959_a(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5) {
	Tesselator& var6 = Tesselator::getInstance();
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, (MathHelper::sin((float)GetTickCount() / 100.0F) * 0.2F + 0.4F) * 0.5F);
	int var8;
	if(var3 == 0) {
		if(m_field_1450_i > 0.0F) {
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			int var7 = Textures::load("game:\\Content\\terrain.png");
			g_pd3dDevice->SetTexture(0, Textures::getById(var7));
			Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 0.5F);
			D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
			var8 = worldObj->getBlockId(var2->m_blockX, var2->m_blockY, var2->m_blockZ);
			Block* var9 = var8 > 0 ? Block::blocksList[var8] : NULL;
			g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			// PORT: Alpha's Shaders has no setAlphaRef; rely on D3DRS_ALPHATESTENABLE off + alpha blend for the crack overlay.
			// PORT: GL11.glPolygonOffset -- no polygon-offset on Xbox 360. Use ZWRITE off (overlay is
			// coplanar with the block face; ZFUNC LESSEQUAL draws it on top cleanly).
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			Shaders::setFogEnabled(false);
			Shaders::updateShaderConstants();
			var6.init();
			double var10 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var5;
			double var12 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var5;
			double var14 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var5;
			// PORT: Tesselator::setTranslationD doesn't exist on this platform. Java offset the verts by
			// -camera; here translate the world matrix instead (pre-multiply), restored below.
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)-var10, (float)-var12, (float)-var14);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			var6.disableColor();
			if(var9 == NULL) {
				var9 = Block::s_stone;
			}

			m_field_1438_u->renderBlockUsingTexture(var9, var2->m_blockX, var2->m_blockY, var2->m_blockZ, 240 + (int)(m_field_1450_i * 10.0F));
			var6.flush();
			g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			Shaders::setFogEnabled(true);
			Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		}
	} else if(var4 != NULL) {
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		float var16 = sinf((float)GetTickCount() / 100.0F) * 0.2F + 0.8F;
		Tesselator::getInstance().color4f(var16, var16, var16, MathHelper::sin((float)GetTickCount() / 200.0F) * 0.2F + 0.5F);
		var8 = Textures::load("game:\\Content\\terrain.png");
		g_pd3dDevice->SetTexture(0, Textures::getById(var8));
		int var17 = var2->m_blockX;
		int var18 = var2->m_blockY;
		int var11 = var2->m_blockZ;
		if(var2->m_sideHit == 0) {
			--var18;
		}

		if(var2->m_sideHit == 1) {
			++var18;
		}

		if(var2->m_sideHit == 2) {
			--var11;
		}

		if(var2->m_sideHit == 3) {
			++var11;
		}

		if(var2->m_sideHit == 4) {
			--var17;
		}

		if(var2->m_sideHit == 5) {
			++var17;
		}
		(void)var17; (void)var18; (void)var11;
	}

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void RenderGlobal::drawSelectionBox(EntityPlayer* var1, MovingObjectPosition* var2, int var3, ItemStack* var4, float var5) {
	if(var3 == 0 && var2->m_typeOfHit == 0) {
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		Tesselator::getInstance().color4f(0.0F, 0.0F, 0.0F, 0.4F);
		// Line width not supported on Xbox 360 GPU
		g_pd3dDevice->SetTexture(0, NULL);
		g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		float var6 = 0.002F;
		int var7 = worldObj->getBlockId(var2->m_blockX, var2->m_blockY, var2->m_blockZ);
		if(var7 > 0) {
			Block::blocksList[var7]->setBlockBoundsBasedOnState(worldObj, var2->m_blockX, var2->m_blockY, var2->m_blockZ);
			double var8 = var1->lastTickPosX + (var1->posX - var1->lastTickPosX) * (double)var5;
			double var10 = var1->lastTickPosY + (var1->posY - var1->lastTickPosY) * (double)var5;
			double var12 = var1->lastTickPosZ + (var1->posZ - var1->lastTickPosZ) * (double)var5;
			drawOutlinedBoundingBox(Block::blocksList[var7]->getSelectedBoundingBoxFromPool(worldObj, var2->m_blockX, var2->m_blockY, var2->m_blockZ).expands((double)var6, (double)var6, (double)var6).getOffsetBoundingBox(-var8, -var10, -var12));
		}

		g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		g_pd3dDevice->SetTexture(0, g_terrainTexture);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}
}

void RenderGlobal::drawOutlinedBoundingBox(AxisAlignedBB var1) {
	Tesselator& var2 = Tesselator::getInstance();
	var2.init(3);
	var2.color4f(0.0F, 0.0F, 0.0F, 0.4F);
	var2.vertex((float)var1.minX, (float)var1.minY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.minY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.minY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.minY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.minY, (float)var1.minZ);
	var2.flush();
	var2.init(3);
	var2.vertex((float)var1.minX, (float)var1.maxY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.maxY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.maxY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.maxY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.maxY, (float)var1.minZ);
	var2.flush();
	var2.init(1);
	var2.vertex((float)var1.minX, (float)var1.minY, (float)var1.minZ);
	var2.vertex((float)var1.minX, (float)var1.maxY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.minY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.maxY, (float)var1.minZ);
	var2.vertex((float)var1.maxX, (float)var1.minY, (float)var1.maxZ);
	var2.vertex((float)var1.maxX, (float)var1.maxY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.minY, (float)var1.maxZ);
	var2.vertex((float)var1.minX, (float)var1.maxY, (float)var1.maxZ);
	var2.flush();
}

void RenderGlobal::func_949_a(int var1, int var2, int var3, int var4, int var5, int var6) {
	int var7 = MathHelper::bucketInt(var1, 16);
	int var8 = MathHelper::bucketInt(var2, 16);
	int var9 = MathHelper::bucketInt(var3, 16);
	int var10 = MathHelper::bucketInt(var4, 16);
	int var11 = MathHelper::bucketInt(var5, 16);
	int var12 = MathHelper::bucketInt(var6, 16);

	for(int var13 = var7; var13 <= var10; ++var13) {
		int var14 = var13 % m_field_1443_p;
		if(var14 < 0) {
			var14 += m_field_1443_p;
		}

		for(int var15 = var8; var15 <= var11; ++var15) {
			int var16 = var15 % m_field_1442_q;
			if(var16 < 0) {
				var16 += m_field_1442_q;
			}

			for(int var17 = var9; var17 <= var12; ++var17) {
				int var18 = var17 % m_field_1441_r;
				if(var18 < 0) {
					var18 += m_field_1441_r;
				}

				int var19 = (var18 * m_field_1442_q + var16) * m_field_1443_p + var14;
				WorldRenderer* var20 = &m_field_1444_o[var19];
				if(!var20->needsUpdate) {
					m_field_1446_m.push_back((void*)var20);
				}

				var20->MarkDirty();
			}
		}
	}
}

void RenderGlobal::func_934_a(int var1, int var2, int var3) {
	func_949_a(var1 - 1, var2 - 1, var3 - 1, var1 + 1, var2 + 1, var3 + 1);
}

void RenderGlobal::func_937_b(int var1, int var2, int var3, int var4, int var5, int var6) {
	func_949_a(var1 - 1, var2 - 1, var3 - 1, var4 + 1, var5 + 1, var6 + 1);
}

void RenderGlobal::func_960_a(ICamera* var1, float var2) {
	for(int var3 = 0; var3 < m_field_1443_p * m_field_1442_q * m_field_1441_r; ++var3) {
		if(!m_field_1444_o[var3].func_1196_e() && (!m_field_1444_o[var3].m_field_1749_o || (var3 + m_field_1449_j & 15) == 0)) {
			m_field_1444_o[var3].func_1199_a(var1);
		}
	}

	++m_field_1449_j;
}

void RenderGlobal::playRecord(const std::string& var1, int var2, int var3, int var4) {
	if(!var1.empty()) {
		mc->m_ingameGUI->func_553_b("C418 - " + var1);
	}

	mc->m_sndManager->func_331_a(var1, (float)var2, (float)var3, (float)var4, 1.0F, 1.0F);
}

void RenderGlobal::playSound(const std::string& var1, double var2, double var4, double var6, float var8, float var9) {
	float var10 = 16.0F;
	if(var8 > 1.0F) {
		var10 *= var8;
	}

	if(mc->thePlayer->getDistanceSq(var2, var4, var6) < (double)(var10 * var10)) {
		mc->m_sndManager->func_336_b(var1, (float)var2, (float)var4, (float)var6, var8, var9);
	}
}

void RenderGlobal::spawnParticle(const std::string& var1, double var2, double var4, double var6, double var8, double var10, double var12) {
	double var14 = mc->thePlayer->posX - var2;
	double var16 = mc->thePlayer->posY - var4;
	double var18 = mc->thePlayer->posZ - var6;
	if(var14 * var14 + var16 * var16 + var18 * var18 <= 256.0f) {
		if(var1 == "bubble") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityBubbleFX(worldObj, var2, var4, var6, var8, var10, var12));
		} else if(var1 == "smoke") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntitySmokeFX(worldObj, var2, var4, var6));
		} else if(var1 == "portal") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityPortalFX(worldObj, var2, var4, var6, var8, var10, var12));
		} else if(var1 == "explode") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityExplodeFX(worldObj, var2, var4, var6, var8, var10, var12));
		} else if(var1 == "flame") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityFlameFX(worldObj, var2, var4, var6, var8, var10, var12));
		} else if(var1 == "lava") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityLavaFX(worldObj, var2, var4, var6));
		} else if(var1 == "splash") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntitySplashFX(worldObj, var2, var4, var6, var8, var10, var12));
		} else if(var1 == "largesmoke") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntitySmokeFX(worldObj, var2, var4, var6, 2.5F));
		} else if(var1 == "reddust") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntityReddustFX(worldObj, var2, var4, var6));
		} else if(var1 == "snowballpoof") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntitySlimeFX(worldObj, var2, var4, var6, Item::s_snowball));
		} else if(var1 == "slime") {
			mc->m_field_6321_h->func_1192_a((EntityFX*)new EntitySlimeFX(worldObj, var2, var4, var6, Item::s_slimeBall));
		}
	}
}

void RenderGlobal::obtainEntitySkin(Entity* var1) {
	// PORT: skin download (network) not used on Xbox 360
}

void RenderGlobal::releaseEntitySkin(Entity* var1) {
	// PORT: skin release (network) not used on Xbox 360
}

void RenderGlobal::func_936_e() {
	for(int var1 = 0; var1 < m_field_1443_p * m_field_1442_q * m_field_1441_r; ++var1) {
		if(m_field_1444_o[var1].m_field_1747_A) {
			if(!m_field_1444_o[var1].needsUpdate) {
				m_field_1446_m.push_back((void*)&m_field_1444_o[var1]);
			}

			m_field_1444_o[var1].MarkDirty();
		}
	}
}

void RenderGlobal::func_935_a(int var1, int var2, int var3, TileEntity* var4) {
	// PORT: TileEntity special-renderer registration -- not used on Xbox 360 (no special TileEntities
	// in this build). No-op.
}
