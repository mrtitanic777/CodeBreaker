#include "stdafx.h"
// Hand-written PORT override for RenderEngine.java
// ------------------------------------------------
// The Java RenderEngine is an AWT/NIO texture manager (BufferedImage, ImageIO,
// ThreadDownloadImageData web-skin download) with no clean C++03 transpilation.
// On Xbox 360 the int-handle texture registry already exists natively as the
// Textures class (D3DXCreateTextureFromFileExA, cached by path). This shim
// implements the converter-generated RenderEngine.h interface as a thin wrapper
// over Textures, so every converted caller — getTexture()/bindTexture()/etc. —
// keeps working unchanged. No reference-port code is pulled in.

#include "RenderEngine.h"
#include "Common.h"
#include "Textures.h"
#include "TextureFX.h"
#include "GameSettings.h"
#include <string>

// Map a Java resource path ("/terrain.png", "/gui/gui.png", "##...", "%%...")
// to the Xbox 360 content path ("game:\Content\terrain.png", ...).
static std::string toContentPath(const std::string& res) {
	const char* p = res.c_str();
	// Strip the "##" (column-unwrap) / "%%" (clamp) animation prefixes if present.
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
	: m_singleIntBuffer(NULL)
	, imageData(NULL)
	, m_urlToImageDataMap(NULL)
	, m_options(NULL)
	, m_clampTexture(false)
{
}

RenderEngine::RenderEngine(GameSettings* var1)
	: m_singleIntBuffer(NULL)
	, imageData(NULL)
	, m_urlToImageDataMap(NULL)
	, m_options(var1)
	, m_clampTexture(false)
{
}

RenderEngine::~RenderEngine() {
	m_urlToImageDataMap = NULL;
	m_options = NULL;
}

int RenderEngine::getTexture(const std::string& var1) {
	// Resolve to a cached int handle in the Textures registry. Textures::load
	// returns the existing id when the path was loaded before, so this also
	// serves the original m_textureMap caching role.
	return Textures::load(toContentPath(var1).c_str());
}

int RenderEngine::getTextureForDownloadableImage(const std::string& var1, const std::string& var2) {
	// No web skin download on Xbox 360 — always fall back to the local texture.
	(void)var1;
	return getTexture(var2);
}

ThreadDownloadImageData* RenderEngine::obtainImageData(const std::string& var1, ImageBufferDownload* var2) {
	// Image-download threads are not used on Xbox 360.
	(void)var1; (void)var2;
	return NULL;
}

void RenderEngine::releaseImageData(const std::string& var1) {
	(void)var1;
}

void RenderEngine::registerTextureFX(TextureFX* var1) {
	m_textureList.push_back((void*)var1);
	var1->onTick();
}

void RenderEngine::updateDynamicTextures() {
	bool anaglyph = (m_options != NULL && m_options->m_anaglyph);
	for (size_t i = 0; i < m_textureList.size(); ++i) {
		TextureFX* fx = (TextureFX*)m_textureList[i];
		fx->anaglyphEnabled = anaglyph;
		fx->onTick();
		// PORT: per-frame LockRect upload of fx->imageData into the atlas tile
		// (animated water/lava/fire) is deferred — cosmetic, not required to run.
	}
}

void RenderEngine::refreshTextures() {
	// PORT: device-lost texture reload is handled by the Textures registry; no-op.
}

void RenderEngine::bindTexture(int var0) {
	if (var0 >= 0) {
		g_pd3dDevice->SetTexture(0, Textures::getById(var0));
	}
}

// --- Unused private helpers (BufferedImage path) kept as no-ops for the header ---

void* RenderEngine::unwrapImageByColumns(void* var0) {
	(void)var0;
	return NULL;
}

void RenderEngine::setupTexture(void* var1, int var2) {
	(void)var1; (void)var2;
}
