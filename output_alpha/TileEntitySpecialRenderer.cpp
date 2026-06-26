#include "stdafx.h"
// Auto-generated from TileEntitySpecialRenderer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TileEntitySpecialRenderer.h"
#include "Common.h"
#include "FontRenderer.h"
#include "TileEntity.h"
#include "TileEntityRenderer.h"
#include <string>
#include "RenderEngine.h"

TileEntitySpecialRenderer::TileEntitySpecialRenderer()
	: m_tileEntityRenderer(NULL)
{
}

TileEntitySpecialRenderer::~TileEntitySpecialRenderer() {
	m_tileEntityRenderer = NULL;
}

void TileEntitySpecialRenderer::bindTextureByName(const std::string& var1) {
		RenderEngine* var2 = m_tileEntityRenderer->m_renderEngine;
		var2->bindTexture(var2->getTexture(var1));
}

void TileEntitySpecialRenderer::setTileEntityRenderer(TileEntityRenderer* var1) {
		m_tileEntityRenderer = var1;
}

FontRenderer* TileEntitySpecialRenderer::getFontRenderer() {
		return m_tileEntityRenderer->getFontRenderer();
}
