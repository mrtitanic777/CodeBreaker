// Textures.ext.cpp — texture-registry methods the GL11->D3D9 emulator needs.  [Step 3]
// REFERENCE (Xbox-side; not built by j2x). ADD these to the old port's Textures class
// (../MinecraftAlpha/Textures.{h,cpp}) -- it already has load(path)/getById/bind/s_textures[128].
// The new RenderEngine (override) drives textures through glGenTextures/glTexImage2D/glTexSubImage2D,
// which the GL emulator forwards here. ARGB pixel data (Java BufferedImage.getRGB order = 0xAARRGGBB)
// maps 1:1 to D3DFMT_A8R8G8B8, so no swizzle on upload.
//
// Add to Textures.h:
//   static int  genId();                                  // reserve an empty slot, return its id
//   static void deleteId(int id);
//   static void uploadRGBA(int id, int w, int h, const void* argbPixels);          // (re)create + fill
//   static void uploadSubRGBA(int id, int x, int y, int w, int h, const void* argbPixels);

#include "stdafx.h"
#include "Textures.h"
#include "Common.h"          // g_pd3dDevice

// reserve the next free slot (id 0 is reserved = "no texture"); ids index s_textures[].
int Textures::genId() {
	for (int i = 1; i < MAX_TEXTURES; ++i) {
		if (s_textures[i] == NULL && s_paths[i][0] == 0) {
			s_paths[i][0] = 1;   // mark reserved (no path)
			if (i >= s_count) s_count = i + 1;
			return i;
		}
	}
	return 0;
}

void Textures::deleteId(int id) {
	if (id <= 0 || id >= MAX_TEXTURES) return;
	if (s_textures[id]) { s_textures[id]->Release(); s_textures[id] = NULL; }
	s_paths[id][0] = 0;
}

// Copy w*h pixels from GL byte order [R,G,B,A] (what RenderEngine::setupTexture packs for GL_RGBA)
// into a D3DFMT_A8R8G8B8 surface, whose little-endian byte layout is [B,G,R,A] -- so R and B must be
// SWAPPED or every texture renders with red/blue swapped (brown dirt -> blue). White (R=G=B) is
// unaffected, which is why the font still looked right.
static void copyRGBAtoBGRA(unsigned char* dst, int dstPitch, const unsigned char* src, int w, int h) {
	for (int row = 0; row < h; ++row) {
		const unsigned char* s = src + row * w * 4;
		unsigned char* d = dst + row * dstPitch;
		for (int x = 0; x < w; ++x) {
			d[x*4+0] = s[x*4+2];   // dst.B <- src.B
			d[x*4+1] = s[x*4+1];   // dst.G <- src.G
			d[x*4+2] = s[x*4+0];   // dst.R <- src.R
			d[x*4+3] = s[x*4+3];   // dst.A <- src.A
		}
	}
}

// (Re)create texture `id` as A8R8G8B8 w*h and fill from rgbaPixels (GL_RGBA byte order [R,G,B,A]).
// MANAGED pool so it survives device resets without manual restore. No mips (MC pixel art).
void Textures::uploadRGBA(int id, int w, int h, const void* rgbaPixels) {
	if (id <= 0 || id >= MAX_TEXTURES || w <= 0 || h <= 0) return;
	if (s_textures[id]) { s_textures[id]->Release(); s_textures[id] = NULL; }
	IDirect3DTexture9* tex = NULL;
	// Nexia360's D3D12 backend FAILS CreateTexture while a scene is open (the menu loads gui/title
	// textures on-demand mid-BeginScene -> NULL texture -> invisible menu). The game runs inside a
	// persistent BeginScene (Main.cpp / Display::swapBuffers), so bracket creation OUTSIDE the scene.
	g_pd3dDevice->EndScene();
	HRESULT _texhr = g_pd3dDevice->CreateTexture(w, h, 1, 0, D3DFMT_A8R8G8B8,
	                                             D3DPOOL_MANAGED, &tex, NULL);
	g_pd3dDevice->BeginScene();
	if (FAILED(_texhr) || !tex)
		return;
	if (rgbaPixels) {
		D3DLOCKED_RECT lr;
		if (SUCCEEDED(tex->LockRect(0, &lr, NULL, 0))) {
			copyRGBAtoBGRA((unsigned char*)lr.pBits, lr.Pitch, (const unsigned char*)rgbaPixels, w, h);
			tex->UnlockRect(0);
		}
	}
	s_textures[id] = tex;
	if (id >= s_count) s_count = id + 1;
}

// Update a sub-rectangle (used for the animated terrain/water/lava cells and the dynamic light map).
// rgbaPixels is GL_RGBA byte order [R,G,B,A]; swizzled to the A8R8G8B8 surface like uploadRGBA.
void Textures::uploadSubRGBA(int id, int x, int y, int w, int h, const void* rgbaPixels) {
	if (id <= 0 || id >= MAX_TEXTURES || !s_textures[id] || !rgbaPixels) return;
	RECT box; box.left = x; box.top = y; box.right = x + w; box.bottom = y + h;
	D3DLOCKED_RECT lr;
	if (SUCCEEDED(s_textures[id]->LockRect(0, &lr, &box, 0))) {
		copyRGBAtoBGRA((unsigned char*)lr.pBits, lr.Pitch, (const unsigned char*)rgbaPixels, w, h);
		s_textures[id]->UnlockRect(0);
	}
}
