#include "stdafx.h"
// Hand-written PORT override for FontRenderer.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Java FontRenderer is built on AWT (BufferedImage/ImageIO scan of the font PNG for
// glyph widths), java.nio.IntBuffer and GL display lists (glNewList/glCallLists) -
// none of which survive the transpile. This shim keeps the converter-generated
// FontRenderer.h interface (the texture handle is m_field_1308_a, the display-list
// base is m_field_1310_c, the IntBuffer is m_buffer) so every converted caller is
// unchanged, and renders glyphs with the reference port's proven Xbox 360 technique:
// textured DrawPrimitiveUP quads with alpha-test, and HARDCODED default-font widths
// (texture LockRect is unreliable on the 360, so the Java pixel-scan is replaced by
// a fixed table). Adapted from the indev FontRenderer override; behaviour matched to
// Alpha's Java (color-code marker is char 167 '§', brightness levels 85/170).

#include "FontRenderer.h"
#include "Common.h"
#include "RenderEngine.h"
#include "Textures.h"
#include "Shaders.h"
#include <string>

// Java color-code marker: section sign '§' (167). Alpha uses this (not '&').
static const int COLOR_MARKER = 167;

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
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

// Alpha FontRenderer color formula (Java glColor3f path, brightness 85/170):
//   base = (idx>>3 & 1) * 85;  r = (idx>>2 & 1)*170 + base; ...  (idx 6 -> r += 85)
//   idx >= 16  ->  shadow: components /= 4.
static DWORD colorFromIndex(int idx, bool shadow) {
	int b = (idx >> 3 & 1) * 85;
	int r = (idx >> 2 & 1) * 170 + b;
	int g = (idx >> 1 & 1) * 170 + b;
	int bl = (idx >> 0 & 1) * 170 + b;
	if (idx == 6) {
		r += 85;
	}
	if (shadow) {
		r /= 4;
		g /= 4;
		bl /= 4;
	}
	return D3DCOLOR_ARGB(255, r, g, bl);
}

FontRenderer::FontRenderer()
	: m_field_1308_a(-1)
	, m_charWidth(new int[256])
	, m_field_1310_c(0)
	, m_buffer(NULL)
	, m_charWidth_count(256)
{
	initCharWidths(m_charWidth);
}

FontRenderer::FontRenderer(GameSettings* var1, const std::string& var2, RenderEngine* var3)
	: m_field_1308_a(-1)
	, m_charWidth(new int[256])
	, m_field_1310_c(0)
	, m_buffer(NULL)
	, m_charWidth_count(256)
{
	(void)var1;  // anaglyph option unused in this minimal port
	initCharWidths(m_charWidth);
	if (var3 != NULL) {
		m_field_1308_a = var3->getTexture(var2);  // -> int handle in the Textures registry
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
	if (var1.empty() || m_field_1308_a < 0 || g_pd3dDevice == NULL) {
		return;
	}

	IDirect3DTexture9* tex = Textures::getById(m_field_1308_a);
	if (tex == NULL) {
		return;
	}

	// Java shadow dimming: keep the high alpha byte, dim RGB by 4 (mask 16579836).
	int color = var4;
	if (var5) {
		int alphaBits = color & (int)0xFF000000;
		color = (color & 16579836) >> 2;
		color += alphaBits;
	}
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
	const std::string& s = var1;
	for (size_t i = 0; i < s.size(); ++i) {
		// Color codes: marker char 167 followed by a hex digit (Alpha-style).
		if ((unsigned char)s[i] == COLOR_MARKER && i + 1 < s.size()) {
			int ci = hexIndex(s[i + 1]);
			if (ci < 0) ci = 15;
			packed = colorFromIndex(ci + (var5 ? 16 : 0), false);
			++i;  // skip the digit too
			continue;
		}

		int idx = (unsigned char)s[i];
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
	const std::string& s = var1;
	for (size_t i = 0; i < s.size(); ++i) {
		// Java skips the marker AND its following code digit.
		if ((unsigned char)s[i] == COLOR_MARKER) {
			++i;
			continue;
		}
		width += m_charWidth[(unsigned char)s[i]];
	}
	return width;
}
