#include "stdafx.h"
// Auto-generated from GuiEditSign.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Converter fixes:
//   * "literal".indexOf(ch) is invalid C++ -> Common.h's javaIndexOf(string, char). The Java
//     allowed-char string also listed extended Latin/CP437 glyphs (⌂ etc.) that aren't
//     representable in this source code page (C4566) and aren't reachable from the 360 controller
//     keyboard -> the allowed set is the printable ASCII range (matches handwritten_alpha\GuiChat).
//   * TileEntityRenderer.instance.renderTileEntityAt(...) -> TileEntityRenderer::instance is a
//     static pointer -> TileEntityRenderer::instance->renderTileEntityAt(...).

#include "GuiEditSign.h"
#include "Common.h"
#include "GuiButton.h"
#include "TileEntitySign.h"
#include "TileEntityRenderer.h"
#include "Block.h"
#include "GuiScreen.h"
#include "Shaders.h"
#include "Minecraft.h"

// Printable ASCII characters allowed in sign text. Java's allowed-char string also listed the
// extended Latin/CP437 glyphs, but those are unreachable from the 360 controller keyboard and
// aren't representable in this source code page -> printable ASCII is the portable set.
static const char* SIGN_ALLOWED_CHARS =
	" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~";

GuiEditSign::GuiEditSign()
	: m_screenTitle("Edit sign message:")
	, m_entitySign(NULL)
	, m_updateCounter(0)
	, m_editLine(0)
{
}

GuiEditSign::GuiEditSign(TileEntitySign* var1)
	: m_screenTitle("Edit sign message:")
	, m_entitySign(NULL)
	, m_updateCounter(0)
	, m_editLine(0)
{
		m_entitySign = var1;
}

GuiEditSign::~GuiEditSign() {
	m_entitySign = NULL;
}

void GuiEditSign::initGui() {
  controlList.clear();
  // STUBBED: Keyboard.enableRepeatEvents(true);  // XInput replaces Keyboard
		controlList.push_back(new GuiButton(0, width / 2 - 100, height / 4 + 120, "Done"));
}

void GuiEditSign::onGuiClosed() {
  // STUBBED: Keyboard.enableRepeatEvents(false);  // XInput replaces Keyboard
}

void GuiEditSign::updateScreen() {
		++m_updateCounter;
}

void GuiEditSign::actionPerformed(GuiButton* var1) {
		if(var1->m_enabled) {
			if(var1->m_id == 0) {
				m_entitySign->onInventoryChanged();
				mc->displayGuiScreen(NULL);
			}

		}
}

void GuiEditSign::keyTyped(char var1, int var2) {
		if(var2 == 200) {
			m_editLine = m_editLine - 1 & 3;
		}

		if(var2 == 208 || var2 == 28) {
			m_editLine = m_editLine + 1 & 3;
		}

		if(var2 == 14 && m_entitySign->m_signText[m_editLine].length() > 0) {
			m_entitySign->m_signText[m_editLine] = m_entitySign->m_signText[m_editLine].substr(0, m_entitySign->m_signText[m_editLine].length() - 1);
		}

		if(javaIndexOf(SIGN_ALLOWED_CHARS, var1) >= 0 && m_entitySign->m_signText[m_editLine].length() < 15) {
			m_entitySign->m_signText[m_editLine] = m_entitySign->m_signText[m_editLine] + var1;
		}

}

void GuiEditSign::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		drawCenteredString(fontRenderer, m_screenTitle, width / 2, 40, 16777215);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(this->width / 2), (float)(this->height / 2), 50.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var4 = 93.75F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -var4, -var4, -var4);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		Block* var5 = m_entitySign->getBlockType();
		if(var5 == Block::s_signPost) {
			float var6 = (float)(m_entitySign->getBlockMetadata() * 360) / 16.0F;
   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var6)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 5.0F / 16.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		} else {
			int var8 = m_entitySign->getBlockMetadata();
			float var7 = 0.0F;
			if(var8 == 2) {
				var7 = 180.0F;
			}

			if(var8 == 4) {
				var7 = 90.0F;
			}

			if(var8 == 5) {
				var7 = -90.0F;
			}

   { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (var7)*DEG_TO_RAD);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 5.0F / 16.0F, 0.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		}

		if(m_updateCounter / 6 % 2 == 0) {
			m_entitySign->m_lineBeingEdited = m_editLine;
		}

		TileEntityRenderer::instance->renderTileEntityAt(m_entitySign, -0.5f, -0.75f, -0.5f, 0.0F);
		m_entitySign->m_lineBeingEdited = -1;
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		GuiScreen::drawScreen(var1, var2, var3);
}
