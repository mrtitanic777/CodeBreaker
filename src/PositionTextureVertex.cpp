#include "stdafx.h"
// Auto-generated from PositionTextureVertex.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PositionTextureVertex.h"
#include "Common.h"
#include "Vec3.h"
#include "Vertex.h"

PositionTextureVertex::PositionTextureVertex()
	: m_texturePositionX(0)
	, m_texturePositionY(0)
{
}

PositionTextureVertex::PositionTextureVertex(float var1, float var2, float var3, float var4, float var5)
	: m_texturePositionX(0)
	, m_texturePositionY(0)
{
  // PORT: constructor delegation  // was: this(new Vec3D(var1, var2, var3), var4, var5);
}

PositionTextureVertex::PositionTextureVertex(const PositionTextureVertex& var1, float var2, float var3)
	: m_texturePositionX(0)
	, m_texturePositionY(0)
{
		m_vector3.0f = var1.vector3.0f;
		m_texturePositionX = var2;
		m_texturePositionY = var3;
}

PositionTextureVertex::PositionTextureVertex(const Vec3D& var1, float var2, float var3)
	: m_texturePositionX(0)
	, m_texturePositionY(0)
{
		m_vector3.0f = var1;
		m_texturePositionX = var2;
		m_texturePositionY = var3;
}

PositionTextureVertex::~PositionTextureVertex() {
	// No dynamic resources
}

PositionTextureVertex PositionTextureVertex::setTexturePosition(float var1, float var2) {
		return PositionTextureVertex(this, var1, var2);
}
