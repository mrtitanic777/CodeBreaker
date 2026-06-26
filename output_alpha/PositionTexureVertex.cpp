#include "stdafx.h"
// Auto-generated from PositionTexureVertex.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PositionTexureVertex.h"
#include "Common.h"
#include "Vec3D.h"
#include "Vertex.h"

PositionTexureVertex::PositionTexureVertex()
	: m_vector3D(NULL)
	, m_texturePositionX(0)
	, m_texturePositionY(0)
{
}

PositionTexureVertex::PositionTexureVertex(float var1, float var2, float var3, float var4, float var5)
	: m_vector3D(NULL)
	, m_texturePositionX(0)
	, m_texturePositionY(0)
{
  // PORT: constructor delegation  // was: this(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3), var4, var5);
}

PositionTexureVertex::PositionTexureVertex(PositionTexureVertex* var1, float var2, float var3)
	: m_vector3D(NULL)
	, m_texturePositionX(0)
	, m_texturePositionY(0)
{
		m_vector3D = var1->m_vector3D;
		m_texturePositionX = var2;
		m_texturePositionY = var3;
}

PositionTexureVertex::PositionTexureVertex(Vec3D* var1, float var2, float var3)
	: m_vector3D(NULL)
	, m_texturePositionX(0)
	, m_texturePositionY(0)
{
		m_vector3D = var1;
		m_texturePositionX = var2;
		m_texturePositionY = var3;
}

PositionTexureVertex::~PositionTexureVertex() {
	m_vector3D = NULL;
}

PositionTexureVertex* PositionTexureVertex::setTexturePosition(float var1, float var2) {
		return new PositionTexureVertex(this, var1, var2);
}
