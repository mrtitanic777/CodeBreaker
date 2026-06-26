#include "stdafx.h"
// Hand-written PORT override for RenderEngine.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Java RenderEngine is an AWT/NIO texture manager (BufferedImage, ImageIO, web-skin
// download) with no clean C++03 transpilation. On Xbox 360 the int-handle texture
// registry already exists as the Textures class (D3DXCreateTextureFromFile, path-cached).
// This shim implements the converter-generated RenderEngine.h interface as a thin wrapper
// over Textures, so every converted caller (getTexture/bindTexture/registerTextureFX/...)
// keeps working. Modeled on the indev RenderEngine override, adapted to Alpha signatures.

#include "RenderEngine.h"
#include "Common.h"
#include "Textures.h"
#include "TextureFX.h"
#include "GameSettings.h"
#include <string>

bool RenderEngine::s_useMipmaps = false;

// Map a Java resource path ("/terrain.png", "##.../%%..." animation prefixes) to the
// Xbox 360 content path ("game:\\Content\\terrain.png").
static std::string toContentPath(const std::string& res) {
	const char* p = res.c_str();
	if (res.size() >= 2 && ((p[0] == '#' && p[1] == '#') || (p[0] == '%' && p[1] == '%'))) {
		p += 2;
	}
	std::string out = "game:\\Content";
	if (*p != '/' && *p != '\\') {
		out += '\\';
	}
	for (; *p; ++p) {
		out += (*p == '/') ? '\\' : *p;
	}
	return out;
}

RenderEngine::RenderEngine()
	: m_singleIntBuffer(NULL), imageData(NULL), m_urlToImageDataMap(NULL),
	  m_options(NULL), m_clampTexture(false), m_blurTexture(false), m_field_6527_k(NULL) {
}

RenderEngine::RenderEngine(TexturePackList* var1, GameSettings* var2)
	: m_singleIntBuffer(NULL), imageData(NULL), m_urlToImageDataMap(NULL),
	  m_options(var2), m_clampTexture(false), m_blurTexture(false), m_field_6527_k(var1) {
}

RenderEngine::~RenderEngine() {
	m_urlToImageDataMap = NULL;
	m_options = NULL;
	m_field_6527_k = NULL;
}

int RenderEngine::getTexture(const std::string& var1) {
	// Resolve to a cached int handle in the Textures registry (load returns the existing
	// id when the path was loaded before, so this also covers the m_textureMap cache role).
	return Textures::load(toContentPath(var1).c_str());
}

int RenderEngine::allocateAndSetupTexture(void* var1) {
	(void)var1;
	return -1;  // PORT: BufferedImage upload path unused on 360
}

void RenderEngine::setupTexture(void* var1, int var2) {
	(void)var1; (void)var2;
}

void RenderEngine::deleteTexture(int var1) {
	(void)var1;  // textures are owned/cached by the Textures registry
}

int RenderEngine::getTextureForDownloadableImage(const std::string& var1, const std::string& var2) {
	(void)var1;  // no web skin download on 360 -> local fallback
	return getTexture(var2);
}

ThreadDownloadImageData* RenderEngine::obtainImageData(const std::string& var1, ImageBuffer* var2) {
	(void)var1; (void)var2;
	return NULL;
}

void RenderEngine::releaseImageData(const std::string& var1) {
	(void)var1;
}

void RenderEngine::registerTextureFX(TextureFX* var1) {
	m_field_1604_f.push_back((void*)var1);
	var1->func_783_a();   // tick once on register
}

void RenderEngine::func_1067_a() {
	// Per-frame animated-texture tick (water/lava/fire). The per-tile LockRect upload into
	// the atlas is deferred (cosmetic); the tick keeps the FX state advancing.
	for (size_t i = 0; i < m_field_1604_f.size(); ++i) {
		((TextureFX*)m_field_1604_f[i])->func_783_a();
	}
}

void RenderEngine::refreshTextures() {
	// PORT: device-lost reload handled by the Textures registry; no-op.
}

void RenderEngine::bindTexture(int var1) {
	if (var1 >= 0) {
		g_pd3dDevice->SetTexture(0, Textures::getById(var1));
	}
}

void* RenderEngine::unwrapImageByColumns(void* var1) {
	(void)var1;
	return NULL;
}

int RenderEngine::averageColor(int var1, int var2) {
	(void)var2;
	return var1;  // PORT: mipmap colour blend unused (prebaked mip atlases)
}

int RenderEngine::weightedAverageColor(int var1, int var2) {
	(void)var2;
	return var1;
}

void* RenderEngine::readTextureImage(InputStream* var1) {
	(void)var1;
	return NULL;
}
