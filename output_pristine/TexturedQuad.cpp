#include "stdafx.h"
// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturedQuad.h"
#include "Common.h"

TexturedQuad::TexturedQuad()
	: m_vertexPositions(NULL)
{
}

TexturedQuad::TexturedQuad(PositionTextureVertex* var1)
	: m_vertexPositions(NULL)
{
		m_vertexPositions = var1;
}

TexturedQuad::TexturedQuad(PositionTextureVertex* var1, int var2, int var3, int var4, int var5)
	: m_vertexPositions(NULL)
{
  // PORT: constructor delegation  // was: this(var1);
  m_vertices = var1;
  m_vertexCount = 4; // PORT: inlined from delegation
		var1[0] = var1[0].setTexturePosition((float)var4 / 64.0F - 0.0015625F, (float)var3 / 32.0F + 0.003125F);
		var1[1] = var1[1].setTexturePosition((float)var2 / 64.0F + 0.0015625F, (float)var3 / 32.0F + 0.003125F);
		var1[2] = var1[2].setTexturePosition((float)var2 / 64.0F + 0.0015625F, (float)var5 / 32.0F - 0.003125F);
		var1[3] = var1[3].setTexturePosition((float)var4 / 64.0F - 0.0015625F, (float)var5 / 32.0F - 0.003125F);
}

TexturedQuad::~TexturedQuad() {
	delete[] m_vertexPositions;
	m_vertexPositions = NULL;
}
