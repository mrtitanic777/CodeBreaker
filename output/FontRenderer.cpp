#include "stdafx.h"
// Hand-written PORT override for FontRenderer.java
// ------------------------------------------------
// The converted FontRenderer is built on Java AWT (BufferedImage/ImageIO to scan
// the font PNG for glyph widths) and GL display lists — neither survives the
// transpile. This shim keeps the converter-generated FontRenderer.h interface
// (drawString / drawStringWithShadow / getStringWidth, int texture handle via
// m_fontTextureName) so every converted caller is unchanged, and renders glyphs
// with the reference port's proven Xbox 360 technique: textured DrawPrimitiveUP
// quads with alpha-test, and HARDCODED default-font widths (texture LockRect is
// unreliable on the 360, so the Java pixel-scan is replaced by a fixed table).

#include "FontRenderer.h"
#include "Common.h"
#include "RenderEngine.h"
#include "Textures.h"
#include "Shaders.h"
#include <string>
#include "GameSettings.h"

// Default Minecraft font character widths (proportional). Most glyphs are 6px
// (5px glyph + 1px spacing); the rest are the well-known narrow/wide exceptions.
static void initCharWidths(int* w) {
	for (int i = 0; i < 256; ++i) w[i] = 6;
	w[' ']  = 4; w['!'] = 2; w['"'] = 5; w['\''] = 3;
	w['(']  = 5; w[')'] = 5; w['*'] = 5; w[','] = 2;
	w['.']  = 2; w[':'] = 2; w[';'] = 2; w['<'] = 5;
	w['>']  = 5; w['@'] = 7; w['I'] = 4; w['['] = 4;
	w[']']  = 4; w['`'] = 3; w['f'] = 5; w['i'] = 2;
	w['k']  = 5; w['l'] = 3; w['t'] = 4; w['{'] = 4;
	w['|']  = 2; w['}'] = 4; w['~'] = 7;
}

// Color-code digit ('0'-'9','a'-'f') -> 0..15, or -1 if not a valid code.
static int hexIndex(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

// Java FontRenderer color formula: bits select B/G/R (0 or 191), bit 3 brightens.
static DWORD colorFromIndex(int idx, bool shadow) {
	int extra = (idx & 8) << 3;
	int b = (idx & 1) * 191 + extra;
	int g = ((idx & 2) >> 1) * 191 + extra;
	int r = ((idx & 4) >> 2) * 191 + extra;
	if (shadow) { r >>= 2; g >>= 2; b >>= 2; }
	return D3DCOLOR_ARGB(255, r, g, b);
}

FontRenderer::FontRenderer()
	: m_charWidth(new int[256])
	, m_fontTextureName(-1)
	, m_fontDisplayLists(0)
	, m_buffer(NULL)
	, m_charWidth_count(256)
{
	initCharWidths(m_charWidth);
}

FontRenderer::FontRenderer(GameSettings* var1, const std::string& var2, RenderEngine* var3)
	: m_charWidth(new int[256])
	, m_fontTextureName(-1)
	, m_fontDisplayLists(0)
	, m_buffer(NULL)
	, m_charWidth_count(256)
{
	(void)var1;  // anaglyph option unused in this minimal port
	initCharWidths(m_charWidth);
	if (var3 != NULL) {
		m_fontTextureName = var3->getTexture(var2);  // -> int handle in the Textures registry
	}
}

FontRenderer::~FontRenderer() {
	delete[] m_charWidth;
	m_charWidth = NULL;
}

void FontRenderer::drawStringWithShadow(const std::string& var1, int var2, int var3, int var4) {
	renderString(var1, var2 + 1, var3 + 1, var4, true);
	drawString(var1, var2, var3, var4);
}

void FontRenderer::drawString(const std::string& var1, int var2, int var3, int var4) {
	renderString(var1, var2, var3, var4, false);
}

void FontRenderer::renderString(const std::string& var1, int var2, int var3, int var4, bool var5) {
	if (var1.empty() || m_fontTextureName < 0 || g_pd3dDevice == NULL) {
		return;
	}

	IDirect3DTexture9* tex = Textures::getById(m_fontTextureName);
	if (tex == NULL) {
		return;
	}

	int color = var5 ? ((var4 & 0xFCFCFC) >> 2) : var4;
	DWORD packed = D3DCOLOR_ARGB(255, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

	DWORD oldAlphaBlend, oldAlphaTest;
	g_pd3dDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &oldAlphaBlend);
	g_pd3dDevice->GetRenderState(D3DRS_ALPHATESTENABLE, &oldAlphaTest);

	g_pd3dDevice->SetTexture(0, tex);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 128);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	Shaders::useTexturedShader();
	Shaders::updateShaderConstants();

	int xOffset = 0;
	const char* text = var1.c_str();
	for (const char* p = text; *p != '\0'; ++p) {
		char c = *p;
		if (c == '&' && *(p + 1) != '\0') {
			int ci = hexIndex(*(p + 1));
			if (ci >= 0) {
				packed = colorFromIndex(ci, var5);
				++p;
				continue;
			}
		}

		int idx = (unsigned char)c;
		int texX = (idx % 16) * 8;
		int texY = (idx / 16) * 8;
		float cw = (float)m_charWidth[idx] - 0.01f;

		float u0 = (float)texX / 128.0f;
		float u1 = ((float)texX + cw) / 128.0f;
		float v0 = (float)texY / 128.0f;
		float v1 = ((float)texY + 7.99f) / 128.0f;

		float sx0 = (float)(var2 + xOffset) - 0.5f;
		float sy0 = (float)var3 - 0.5f;
		float sx1 = sx0 + cw;
		float sy1 = sy0 + 7.99f;

		TesselatorVertex verts[4] = {
			{ sx0, sy1, 0.0f, packed, u0, v1 },
			{ sx1, sy1, 0.0f, packed, u1, v1 },
			{ sx1, sy0, 0.0f, packed, u1, v0 },
			{ sx0, sy0, 0.0f, packed, u0, v0 },
		};
		g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, verts, sizeof(TesselatorVertex));

		xOffset += m_charWidth[idx];
	}

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, oldAlphaBlend);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, oldAlphaTest);
}

int FontRenderer::getStringWidth(const std::string& var1) {
	if (var1.empty()) {
		return 0;
	}
	int width = 0;
	const char* text = var1.c_str();
	for (const char* p = text; *p != '\0'; ++p) {
		if (*p == '&' && *(p + 1) != '\0' && hexIndex(*(p + 1)) >= 0) {
			++p;
			continue;
		}
		width += m_charWidth[(unsigned char)*p];
	}
	return width;
}
