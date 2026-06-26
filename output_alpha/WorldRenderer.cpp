#include "stdafx.h"
// Hand-written PORT override for WorldRenderer.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Per-chunk vertex-buffer builder. WorldRenderer::func_1198_a (updateRenderer) re-meshes a
// chunk's blocks with RenderBlocks into the Tesselator, then bakes the quads into per-pass
// D3D9 vertex buffers (resident on the GPU); func_1201_d binds + DrawPrimitive's them.
//
// This is ported from the WORKING indev WorldRenderer (which solved the chunk-VB on Xbox 360:
// CreateVertexBuffer / Lock / captureTriangles / DrawPrimitive, with the GL occlusion-query and
// display-list calls stubbed as PORT no-ops), but it is shaped to the ALPHA WorldRenderer.h
// interface (func_XXXX names + m_field_XXXX fields) instead of indev's named fields.
//
// CONSTRAINT: only this .cpp may be written, and it must compile against the converted Alpha
// WorldRenderer.h. That header has NO vertex-buffer storage fields (it still models the GL
// display-list design with an int call-list id, m_field_1744_C). Rather than touch the header,
// the per-chunk baked VBs live in a translation-unit-local side table keyed by 'this'. Alpha's
// Tesselator also lacks indev's captureTriangles()/setTranslationD(); both are reproduced inline
// here (read getVertices()/getVertexCount(), convert quads->triangles, and fold the chunk
// translation straight into the baked vertex positions).

#include "Tesselator.h"   // MUST precede WorldRenderer.h: its `static Tesselator* s_field_1742_D` member
#include "WorldRenderer.h" // needs Tesselator known, and the converted header doesn't forward-declare it
#include "Common.h"
#include "Entity.h"
#include "ICamera.h"
#include "Tesselator.h"
#include "World.h"
#include "AxisAlignedBB.h"
#include "Block.h"
#include "Item.h"
#include "RenderBlocks.h"
#include "RenderItem.h"
#include "Shaders.h"
#include "TileEntity.h"
#include "ChunkCache.h"

#include <map>

Tesselator* WorldRenderer::s_field_1742_D = &Tesselator::getInstance();
int WorldRenderer::s_field_1762_b = 0;

// ---------------------------------------------------------------------------------------------
// PORT: per-chunk baked GPU vertex buffers. The Alpha header keeps the old GL display-list field
// (m_field_1744_C) and no VB storage, so the two per-pass triangle buffers for each WorldRenderer
// live here, keyed by the object address. Built once in func_1198_a, drawn each frame.
// ---------------------------------------------------------------------------------------------
namespace {
	struct ChunkVB {
		IDirect3DVertexBuffer9* vb[2];   // pass 0 = solid, pass 1 = alpha
		int triVerts[2];                 // triangle-vertex count baked into each VB
		int cap[2];                      // VB capacity in vertices
		ChunkVB() {
			vb[0] = vb[1] = NULL;
			triVerts[0] = triVerts[1] = 0;
			cap[0] = cap[1] = 0;
		}
	};

	std::map<const WorldRenderer*, ChunkVB> g_chunkVBs;

	ChunkVB& vbFor(const WorldRenderer* wr) {
		return g_chunkVBs[wr];
	}

	void releaseChunkVBs(const WorldRenderer* wr) {
		std::map<const WorldRenderer*, ChunkVB>::iterator it = g_chunkVBs.find(wr);
		if(it == g_chunkVBs.end()) {
			return;
		}
		ChunkVB& c = it->second;
		// PORT(rapid-fire crash guard from reference): never Release a VB still bound as stream 0.
		if(g_pd3dDevice) {
			g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);
		}
		for(int p = 0; p < 2; ++p) {
			if(c.vb[p]) { c.vb[p]->Release(); c.vb[p] = NULL; }
			c.triVerts[p] = 0;
			c.cap[p] = 0;
		}
		g_chunkVBs.erase(it);
	}

	// Reproduces indev Tesselator::captureTriangles + setTranslationD: convert the quads currently
	// held in the Tesselator into a triangle list written to 'dst', subtracting (tx,ty,tz) from each
	// position so the chunk is meshed in chunk-local space (the old GL setTranslationD offset). The
	// Tesselator's quad buffer is left as-is for the caller to re-init. Returns triangle-vertex count.
	int bakeTriangles(TesselatorVertex* dst, double tx, double ty, double tz) {
		const TesselatorVertex* src = WorldRenderer::s_field_1742_D->getVertices();
		int vcount = WorldRenderer::s_field_1742_D->getVertexCount();
		int quadCount = vcount / 4;
		int out = 0;
		for(int q = 0; q < quadCount; ++q) {
			int base = q * 4;
			static const int order[6] = { 0, 2, 1, 0, 3, 2 };
			for(int k = 0; k < 6; ++k) {
				TesselatorVertex v = src[base + order[k]];
				v.x -= (float)tx;
				v.y -= (float)ty;
				v.z -= (float)tz;
				dst[out++] = v;
			}
		}
		return out;
	}

	// Bake whatever quads the Tesselator currently holds into the chunk's pass-'pass' VB, growing it
	// if needed. Mirrors the indev updateRenderer VB-bake block. Leaves the Tesselator re-init'd.
	void bakePassIntoVB(const WorldRenderer* wr, int pass, double tx, double ty, double tz) {
		ChunkVB& c = vbFor(wr);
		Tesselator& t = *WorldRenderer::s_field_1742_D;
		int vc = t.getVertexCount();
		int need = (vc / 4) * 6 + 6;   // triangle verts + slack
		if(vc > 0) {
			if(c.vb[pass] == NULL || c.cap[pass] < need) {
				if(c.vb[pass]) { c.vb[pass]->Release(); c.vb[pass] = NULL; }
				if(SUCCEEDED(g_pd3dDevice->CreateVertexBuffer(need * sizeof(TesselatorVertex),
						D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &c.vb[pass], NULL))) {
					c.cap[pass] = need;
				} else {
					c.vb[pass] = NULL;
					c.cap[pass] = 0;
				}
			}
			if(c.vb[pass] != NULL) {
				void* p = NULL;
				if(SUCCEEDED(c.vb[pass]->Lock(0, 0, &p, 0)) && p != NULL) {
					c.triVerts[pass] = bakeTriangles((TesselatorVertex*)p, tx, ty, tz);
					c.vb[pass]->Unlock();
				} else {
					c.triVerts[pass] = 0;
				}
			} else {
				c.triVerts[pass] = 0;
			}
		} else {
			c.triVerts[pass] = 0;
		}
		t.init();   // clear the tesselator for the next pass
	}
}

WorldRenderer::WorldRenderer()
	: worldObj(NULL)
	, m_field_1744_C(-1)
	, m_field_1761_c(0)
	, m_field_1760_d(0)
	, m_field_1759_e(0)
	, m_field_1758_f(0)
	, m_field_1757_g(0)
	, m_field_1756_h(0)
	, m_field_1755_i(0)
	, m_field_1754_j(0)
	, m_field_1753_k(0)
	, m_field_1752_l(0)
	, m_field_1751_m(0)
	, m_field_1750_n(0)
	, m_field_1749_o(false)
	, m_field_1748_p(new bool[2])
	, m_field_1746_q(0)
	, m_field_1743_r(0)
	, m_field_1741_s(0)
	, m_field_1740_t(0)
	, needsUpdate(false)
	, m_hasField_1736_v(false)
	, m_field_1735_w(0)
	, m_field_1734_x(true)
	, m_field_1733_y(false)
	, m_field_1732_z(0)
	, m_field_1747_A(false)
	, m_field_1739_E(false)
	, m_field_1748_p_count(2)
{
	m_field_1748_p[0] = m_field_1748_p[1] = true;
}

WorldRenderer::WorldRenderer(World* var1, std::vector<void*> /* PORT: determine element type */ var2, int var3, int var4, int var5, int var6, int var7)
	: worldObj(NULL)
	, m_field_1744_C(-1)
	, m_field_1761_c(0)
	, m_field_1760_d(0)
	, m_field_1759_e(0)
	, m_field_1758_f(0)
	, m_field_1757_g(0)
	, m_field_1756_h(0)
	, m_field_1755_i(0)
	, m_field_1754_j(0)
	, m_field_1753_k(0)
	, m_field_1752_l(0)
	, m_field_1751_m(0)
	, m_field_1750_n(0)
	, m_field_1749_o(false)
	, m_field_1748_p(new bool[2])
	, m_field_1746_q(0)
	, m_field_1743_r(0)
	, m_field_1741_s(0)
	, m_field_1740_t(0)
	, needsUpdate(false)
	, m_hasField_1736_v(false)
	, m_field_1735_w(0)
	, m_field_1734_x(true)
	, m_field_1733_y(false)
	, m_field_1732_z(0)
	, m_field_1747_A(false)
	, m_field_1739_E(false)
	, m_field_1748_p_count(2)
{
	m_field_1748_p[0] = m_field_1748_p[1] = true;
	worldObj = var1;
	m_field_1737_F = var2;
	m_field_1758_f = m_field_1757_g = m_field_1756_h = var6;
	m_field_1740_t = sqrtf((float)(m_field_1758_f * m_field_1758_f + m_field_1757_g * m_field_1757_g + m_field_1756_h * m_field_1756_h)) / 2.0F;
	m_field_1744_C = var7;
	m_field_1761_c = -999;
	func_1197_a(var3, var4, var5);
	needsUpdate = false;
}

WorldRenderer::~WorldRenderer() {
	releaseChunkVBs(this);
	worldObj = NULL;
	delete[] m_field_1748_p;
	m_field_1748_p = NULL;
}

void WorldRenderer::func_1197_a(int var1, int var2, int var3) {
	if(var1 != m_field_1761_c || var2 != m_field_1760_d || var3 != m_field_1759_e) {
		func_1195_b();
		m_field_1761_c = var1;
		m_field_1760_d = var2;
		m_field_1759_e = var3;
		m_field_1746_q = var1 + m_field_1758_f / 2;
		m_field_1743_r = var2 + m_field_1757_g / 2;
		m_field_1741_s = var3 + m_field_1756_h / 2;
		m_field_1752_l = var1 & 1023;
		m_field_1751_m = var2;
		m_field_1750_n = var3 & 1023;
		m_field_1755_i = var1 - m_field_1752_l;
		m_field_1754_j = var2 - m_field_1751_m;
		m_field_1753_k = var3 - m_field_1750_n;
		float var4 = 2.0F;
		m_field_1736_v = AxisAlignedBB::getBoundingBox(
			(double)((float)var1 - var4), (double)((float)var2 - var4), (double)((float)var3 - var4),
			(double)((float)(var1 + m_field_1758_f) + var4), (double)((float)(var2 + m_field_1757_g) + var4), (double)((float)(var3 + m_field_1756_h) + var4));
		m_hasField_1736_v = true;
		// PORT: Java recorded the chunk's offset bounding box into GL display list var7 (glNewList /
		// renderOffsetAABB / glEndList) purely for occlusion-query drawing. Occlusion queries don't
		// exist on Xbox 360, so the recording is unused -- skip it (drawing the AABB immediately here
		// would emit spurious draws during setLevel, outside BeginScene).
		MarkDirty();
	}
}

void WorldRenderer::func_1203_g() {
	// PORT: translate the world matrix to this chunk's local origin. Kept for callers that still
	// position the chunk via the shader world matrix.
	D3DXMATRIX _t;
	D3DXMatrixTranslation(&_t, (float)this->m_field_1752_l, (float)this->m_field_1751_m, (float)this->m_field_1750_n);
	D3DXMATRIX _w;
	D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
	Shaders::setWorldMatrix(_w);
	Shaders::updateShaderConstants();
}

// updateRenderer: rebuild this chunk's geometry into per-pass GPU vertex buffers.
void WorldRenderer::func_1198_a() {
	if(!needsUpdate) {
		return;
	}
	++s_field_1762_b;

	int var1 = m_field_1761_c;
	int var2 = m_field_1760_d;
	int var3 = m_field_1759_e;
	int var4 = m_field_1761_c + m_field_1758_f;
	int var5 = m_field_1760_d + m_field_1757_g;
	int var6 = m_field_1759_e + m_field_1756_h;

	// PORT(rapid-fire crash guard from reference): unbind stream 0 before any VB Release in the bake.
	if(g_pd3dDevice) {
		g_pd3dDevice->SetStreamSource(0, NULL, 0, 0);
	}

	for(int var7 = 0; var7 < 2; ++var7) {
		m_field_1748_p[var7] = true;
	}

	// Special-renderer tile entities for this chunk. The Alpha header models these collections as
	// std::vector<void*>; rebuild m_field_1745_B (this chunk's TE list) from scratch each mesh and
	// publish it to m_field_1737_F (the global "has special renderer" set the RenderGlobal consumes).
	m_field_1737_F.clear();
	m_field_1745_B.clear();

	unsigned char var8 = 1;
	ChunkCache* var9 = new ChunkCache(worldObj, var1 - var8, var2 - var8, var3 - var8, var4 + var8, var5 + var8, var6 + var8);
	RenderBlocks* var10 = new RenderBlocks(var9);

	for(int var11 = 0; var11 < 2; ++var11) {
		bool var12 = false;   // more passes needed
		bool var13 = false;   // this pass produced geometry
		bool var14 = false;   // tesselator was init'd for this pass

		for(int var15 = var2; var15 < var5; ++var15) {
			for(int var16 = var3; var16 < var6; ++var16) {
				for(int var17 = var1; var17 < var4; ++var17) {
					int var18 = var9->getBlockId(var17, var15, var16);
					if(var18 > 0 && Block::blocksList[var18] != NULL) {   // PORT: guard unregistered id -> NULL virtual call
						if(!var14) {
							var14 = true;
							s_field_1742_D->init();
						}

						if(var11 == 0 && Block::s_isBlockContainer[var18]) {
							TileEntity* var23 = var9->getBlockTileEntity(var17, var15, var16);
							// PORT: TileEntityRenderer special-renderer registry isn't ported; collect
							// every tile entity so the existing renderTileEntities path can dispatch it.
							if(var23 != NULL) {
								m_field_1745_B.push_back((void*)var23);
							}
						}

						Block* var24 = Block::blocksList[var18];
						int var20 = var24->func_234_g();   // getRenderBlockPass
						if(var20 != var11) {
							var12 = true;
						} else {
							var13 |= var10->renderBlockByRenderType(var24, var17, var15, var16);
						}
					}
				}
			}
		}

		if(var14) {
			// PORT: bake the quads collected for this pass into the chunk's GPU VB (was glEndList /
			// Tesselator.draw). Fold the chunk-local translation (-posX,-posY,-posZ) into the baked
			// vertex positions -- this replaces the old Tesselator.setTranslationD offset.
			bakePassIntoVB(this, var11, (double)m_field_1761_c, (double)m_field_1760_d, (double)m_field_1759_e);
		} else {
			var13 = false;
		}

		if(var13) {
			m_field_1748_p[var11] = false;   // pass has drawable geometry -> don't skip it
		}

		if(!var12) {
			break;
		}
	}

	// Publish this chunk's special-renderer tile entities to the shared set.
	m_field_1737_F = m_field_1745_B;

	m_field_1747_A = false;
	m_field_1739_E = true;
	needsUpdate = false;

	delete var10;
	delete var9;
}

float WorldRenderer::func_1202_a(Entity* var1) {
	float var2 = (float)(var1->posX - (double)m_field_1746_q);
	float var3 = (float)(var1->posY - (double)m_field_1743_r);
	float var4 = (float)(var1->posZ - (double)m_field_1741_s);
	return var2 * var2 + var3 * var3 + var4 * var4;
}

void WorldRenderer::func_1195_b() {
	for(int var1 = 0; var1 < 2; ++var1) {
		m_field_1748_p[var1] = true;
	}

	m_field_1749_o = false;
	m_field_1739_E = false;
}

void WorldRenderer::func_1204_c() {
	func_1195_b();
	worldObj = NULL;
}

int WorldRenderer::func_1200_a(int var1) {
	// Return a non-negative "call list" id when this pass is in-frustum and has drawable geometry,
	// matching the Alpha contract the RenderGlobal/RenderList batcher checks (>= 0 == drawable). The
	// id (m_field_1744_C + pass) is preserved so any GL-list-id consumer keeps a stable handle.
	return !m_field_1749_o ? -1 : (!m_field_1748_p[var1] ? m_field_1744_C + var1 : -1);
}

void WorldRenderer::func_1199_a(ICamera* var1) {
	m_field_1749_o = var1->func_342_a(m_field_1736_v);
}

// Draw this chunk's baked vertex buffers (both passes). The caller (RenderGlobal) binds the
// textured shader, terrain texture, vertex declaration and WVP before the chunk loop.
void WorldRenderer::func_1201_d() {
	std::map<const WorldRenderer*, ChunkVB>::iterator it = g_chunkVBs.find(this);
	if(it == g_chunkVBs.end()) {
		return;
	}
	ChunkVB& c = it->second;
	for(int pass = 0; pass < 2; ++pass) {
		if(m_field_1748_p[pass]) {
			continue;   // pass skipped (no geometry)
		}
		if(c.vb[pass] == NULL || c.triVerts[pass] <= 0) {
			continue;
		}
		g_pd3dDevice->SetStreamSource(0, c.vb[pass], 0, sizeof(TesselatorVertex));
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, c.triVerts[pass] / 3);
	}
}

bool WorldRenderer::func_1196_e() {
	return !m_field_1739_E ? false : m_field_1748_p[0] && m_field_1748_p[1];
}

void WorldRenderer::MarkDirty() {
	needsUpdate = true;
}
