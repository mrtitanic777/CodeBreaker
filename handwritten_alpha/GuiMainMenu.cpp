#include "stdafx.h"
// Hand-written PORT override for GuiMainMenu.java (Minecraft Alpha v1.2.6)
// ------------------------------------------------------------------------
// Started from the converter output (ConversionTool\output_alpha\GuiMainMenu.cpp, ~70 errors)
// and fixed to compile against Alpha's GuiMainMenu.h, using indev's working GuiMainMenu as the
// porting guide. Alpha's menu differs from indev (Singleplayer / Multiplayer / Mods & Texture
// Packs / Options), so the button + actionPerformed logic follows the converted Alpha source,
// not indev wholesale.
//
// Notable converter fixes:
//   * Tesselator::getInstance() returns a reference -> "Tesselator& var4".
//   * Java collection (.get/.size) splash loading + AWT/BufferedReader dropped; splash list is
//     hardcoded and picked at random via s_rand (no STFS file read on the 360).
//   * Calendar/Date holiday splash overrides dropped (unportable; // PORT:).
//   * m_logoEffects is LogoEffectRandomizer** -> allocated as a real [cols][rows] 2D array and
//     indexed [col][row], matching the Java 2D logoEffects.
//   * updateScreen / drawLogo loops had garbled converter index vars (var2/var3) -> fixed.
//   * Matrices PRE-multiplied (new * current), like the indev reference -- the converter emitted
//     post-multiply (current * new), which swings the rotated splash off-screen. See memory
//     [[post-vs-pre-multiply-bug]].

#include "GuiMainMenu.h"
#include "Common.h"
#include "GuiButton.h"
#include "LogoEffectRandomizer.h"
#include "Random.h"
#include <string>
#include <vector>
#include "Block.h"
#include "GuiScreen.h"
#include "RenderBlocks.h"
#include "ScaledResolution.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "GuiMultiplayer.h"
#include "GuiOptions.h"
#include "GuiSelectWorld.h"
#include "GuiTexturePacks.h"
#include "Minecraft.h"
#include "RenderEngine.h"
#include <cmath>

Random* GuiMainMenu::s_rand = new Random();

// PORT: the Alpha splash list. Java read these from a packaged "splashes.txt" via a BufferedReader;
// we bake the list and pick one at random (s_rand) per menu creation -- robust on the 360 (no STFS
// file dependency) and the same observable result.
static const char* SPLASH_TEXTS[] = {
	"Now with extra stuff!", "Awesome!", "100% pure!", "May contain nuts!",
	"Better than Prey!", "More polygons!", "Sexy!", "Limited edition!", "Made by Notch!",
	"Coming soon!", "Ezetable!", "Wow!", "Not on TV!", "Check it out!",
	"Holy cow, man!", "It's a game!", "Made in Sweden!", "Minecraft!", "Yaaay!",
	"Singleplayer!", "Keyboard compatible!", "Ingredients!", "Don't bother!",
	"Indie!", "GNU!", "Microscope!", "Bring me Ray!", "Read more books!",
	"Punching trees!", "Digging holes!", "Building stuff!", "Now with shaders!"
};
static const int SPLASH_COUNT = (int)(sizeof(SPLASH_TEXTS) / sizeof(SPLASH_TEXTS[0]));

GuiMainMenu::GuiMainMenu()
	: m_minecraftLogo(NULL)
	, m_logoEffects(NULL)
	, m_updateCounter(0.0f)
	, m_splashText("missingno")
	, m_minecraftLogo_count(0)
	, m_logoEffects_count(0)
{
	// PORT(array-literal field init): C++03 has no init-list array fill; alloc + fill here.
	m_minecraftLogo = new std::string[5];
	m_minecraftLogo_count = 5;
	m_minecraftLogo[0] = " *   * * *   * *** *** *** *** *** ***";
	m_minecraftLogo[1] = " ** ** * **  * *   *   * * * * *    * ";
	m_minecraftLogo[2] = " * * * * * * * **  *   **  *** **   * ";
	m_minecraftLogo[3] = " *   * * *  ** *   *   * * * * *    * ";
	m_minecraftLogo[4] = " *   * * *   * *** *** * * * * *    * ";

	// PORT: pick a random splash. (Java built a vector from splashes.txt then random-indexed it.)
	if (SPLASH_COUNT > 0) {
		m_splashText = SPLASH_TEXTS[s_rand->nextInt(SPLASH_COUNT)];
	}
}

GuiMainMenu::~GuiMainMenu() {
	delete[] m_minecraftLogo;
	m_minecraftLogo = NULL;

	if (m_logoEffects != NULL) {
		for (int c = 0; c < m_logoEffects_count; ++c) {
			delete[] m_logoEffects[c];
		}
		delete[] m_logoEffects;
		m_logoEffects = NULL;
		m_logoEffects_count = 0;
	}
}

void GuiMainMenu::updateScreen() {
	++m_updateCounter;
	if (m_logoEffects != NULL) {
		// m_logoEffects is [cols][rows] (cols = length of logo row 0, rows = m_minecraftLogo_count).
		for (int col = 0; col < m_logoEffects_count; ++col) {
			for (int row = 0; row < m_minecraftLogo_count; ++row) {
				m_logoEffects[col][row].func_875_a();
			}
		}
	}
}

void GuiMainMenu::keyTyped(char var1, int var2) {
	// TODO: implement
}

void GuiMainMenu::initGui() {
	// PORT: Java used Calendar/Date for holiday splash overrides (Happy birthday / Merry X-mas /
	// Happy new year). Calendar/Date are not portable to the 360 here -> dropped.

	controlList.clear();
	controlList.push_back(new GuiButton(1, width / 2 - 100, height / 4 + 48, "Singleplayer"));
	controlList.push_back(new GuiButton(2, width / 2 - 100, height / 4 + 72, "Multiplayer"));
	controlList.push_back(new GuiButton(3, width / 2 - 100, height / 4 + 96, "Mods and Texture Packs"));
	controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120 + 12, "Options..."));
	if (mc->m_field_6320_i == NULL) {
		controlList[1]->m_enabled = false;   // Multiplayer needs a session
	}
}

void GuiMainMenu::actionPerformed(GuiButton* var1) {
	if (var1->m_id == 0) {
		mc->displayGuiScreen(new GuiOptions(this, mc->m_gameSettings));
	}

	if (var1->m_id == 1) {
		mc->displayGuiScreen(new GuiSelectWorld(this));
	}

	if (var1->m_id == 2) {
		mc->displayGuiScreen(new GuiMultiplayer(this));
	}

	if (var1->m_id == 3) {
		mc->displayGuiScreen(new GuiTexturePacks(this));
	}
}

void GuiMainMenu::drawScreen(int var1, int var2, float var3) {
	drawDefaultBackground();
	Tesselator& var4 = Tesselator::getInstance();
	drawLogo(var3);
	g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/gui/logo.png")));
	Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
	var4.colorI(16777215);

	// GL stacks transforms as scale->rotate->translate applied to the vertex. The shader uses row
	// vectors (v * M), so matrices must be PRE-multiplied (new * current), not post-multiplied --
	// otherwise the rotate happens after the translate and swings the splash off-screen. See
	// memory [[post-vs-pre-multiply-bug]]. (Converter emitted current*new everywhere.)
	D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
	{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(this->width / 2 + 90), 70.0F, 0.0F);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
	{ D3DXMATRIX _r; D3DXVECTOR3 _axis(0.0F, 0.0F, 1.0F); D3DXMatrixRotationAxis(&_r, &_axis, (-20.0F) * DEG_TO_RAD);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

	// PORT: bob the splash using a HIGH-RESOLUTION wall clock. Java used currentTimeMillis()%1000;
	// GetTickCount() can be too coarse on the emulator (phase looks frozen frame to frame).
	float _splashPhase;
	{ LARGE_INTEGER _qc, _qf; QueryPerformanceCounter(&_qc); QueryPerformanceFrequency(&_qf);
	  double _sec = (_qf.QuadPart > 0) ? (double)_qc.QuadPart / (double)_qf.QuadPart : 0.0;
	  _splashPhase = (float)(_sec - (double)(long long)_sec); }   // fractional second 0..1
	float var5 = 1.8F - (float)fabs(sinf(_splashPhase * 3.14159265358979323846f * 2.0F) * 0.1F);
	var5 = var5 * 100.0F / (float)(fontRenderer->getStringWidth(m_splashText) + 32);
	{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var5, var5, var5);
	  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
	  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
	drawCenteredString(fontRenderer, m_splashText, 0, -8, 16776960);
	Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();

	drawString(fontRenderer, "Minecraft Alpha v1.2.6", 2, 2, 5263440);
	std::string var6 = "Copyright Mojang Specifications. Do not distribute.";
	drawString(fontRenderer, var6, width - fontRenderer->getStringWidth(var6) - 2, height - 10, 16777215);
	GuiScreen::drawScreen(var1, var2, var3);
}

void GuiMainMenu::drawLogo(float var1) {
	// Lazily build the [cols][rows] LogoEffectRandomizer 2D array. cols = width of logo row 0 (the
	// longest), rows = number of logo rows. Java: logoEffects[minecraftLogo[0].length()][minecraftLogo.length].
	if (m_logoEffects == NULL) {
		int cols = (int)m_minecraftLogo[0].length();
		int rows = m_minecraftLogo_count;
		m_logoEffects_count = cols;
		m_logoEffects = new LogoEffectRandomizer*[cols];
		for (int col = 0; col < cols; ++col) {
			m_logoEffects[col] = new LogoEffectRandomizer[rows];
			for (int row = 0; row < rows; ++row) {
				// LogoEffectRandomizer(GuiMainMenu*, int var2, int var3): Java passed (this, col, row).
				m_logoEffects[col][row] = LogoEffectRandomizer(this, col, row);
			}
		}
	}

	int var3;
	D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
	{ D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }

	ScaledResolution var14 = ScaledResolution(mc->m_displayWidth, mc->m_displayHeight);
	var3 = 120 * var14.m_scaleFactor;
	{ D3DXMATRIX _persp; D3DXMatrixPerspectiveFovLH(&_persp, D3DXToRadian(70.0F),
	    (float)((float)this->mc->m_displayWidth / (float)var3), 0.05F, 100.0F);
	  Shaders::setProjectionMatrix(_persp); Shaders::updateShaderConstants(); }
	{ D3DVIEWPORT9 _vp = { (DWORD)0, (DWORD)(this->mc->m_displayHeight - var3),
	    (DWORD)(this->mc->m_displayWidth), (DWORD)var3, 0.0f, 1.0f };
	  g_pd3dDevice->SetViewport(&_vp); }

	{ D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); Shaders::updateShaderConstants(); }
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// PORT: GL11.glCullFace(GL_BACK) -> handled by CULLMODE above.
	g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	RenderBlocks* var4 = new RenderBlocks();

	for (int var5 = 0; var5 < 3; ++var5) {
		D3DXMATRIX _passSaved = Shaders::getWorldMatrix();
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.4F, 0.6F, -13.0F);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

		if (var5 == 0) {
			g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
			{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, -0.4F, 0.0F);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.98F, 1.0F, 1.0F);
			  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
			  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		if (var5 == 1) {
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			g_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
		}

		if (var5 == 2) {
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, 1.0F, -1.0F, 1.0F);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _r; D3DXVECTOR3 _axis(1.0F, 0.0F, 0.0F); D3DXMatrixRotationAxis(&_r, &_axis, (15.0F) * DEG_TO_RAD);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, 0.89F, 1.0F, 0.4F);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t,
		    (float)(-(int)this->m_minecraftLogo[0].length()) * 0.5F,
		    (float)(-this->m_minecraftLogo_count) * 0.5F, 0.0F);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }

		g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/terrain.png")));
		if (var5 == 0) {
			g_pd3dDevice->SetTexture(0, Textures::getById(this->mc->m_renderEngine->getTexture("/title/black.png")));
		}

		for (int var6 = 0; var6 < m_minecraftLogo_count; ++var6) {
			for (int var7 = 0; var7 < (int)m_minecraftLogo[var6].length(); ++var7) {
				char var8 = m_minecraftLogo[var6].at(var7);
				if (var8 != 32) {
					D3DXMATRIX _glyphSaved = Shaders::getWorldMatrix();
					LogoEffectRandomizer* var9 = &m_logoEffects[var7][var6];
					float var10 = (float)(var9->m_field_1311_b + (var9->m_field_1312_a - var9->m_field_1311_b) * (double)var1);
					float var11 = 1.0F;
					float var12 = 1.0F;
					float var13 = 0.0F;
					if (var5 == 0) {
						var11 = var10 * 0.04F + 1.0F;
						var12 = 1.0F / var11;
						var10 = 0.0F;
					}

					{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var7, (float)var6, var10);
					  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
					  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
					{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, var11, var11, var11);
					  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
					  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
					{ D3DXMATRIX _r; D3DXVECTOR3 _axis(0.0F, 1.0F, 0.0F); D3DXMatrixRotationAxis(&_r, &_axis, (var13) * DEG_TO_RAD);
					  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
					  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
					var4->func_1238_a(Block::s_stone, var12);
					Shaders::setWorldMatrix(_glyphSaved); Shaders::updateShaderConstants();
				}
			}
		}

		Shaders::setWorldMatrix(_passSaved); Shaders::updateShaderConstants();
	}

	delete var4;

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
	// Restore the full-screen viewport + GUI cull mode for the rest of the menu.
	{ D3DVIEWPORT9 _vp = { (DWORD)0, (DWORD)0, (DWORD)(this->mc->m_displayWidth),
	    (DWORD)(this->mc->m_displayHeight), 0.0f, 1.0f };
	  g_pd3dDevice->SetViewport(&_vp); }
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

Random* GuiMainMenu::getRand() {
	return s_rand;
}
