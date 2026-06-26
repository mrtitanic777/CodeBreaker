#include "stdafx.h"
// Auto-generated from TexturedQuad.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TexturedQuad.h"
#include "Common.h"
#include "PositionTexureVertex.h"
#include "Tesselator.h"
#include "Vec3D.h"

// Quad faces are always 4 vertices.
static const int TQ_VERTS = 4;

TexturedQuad::TexturedQuad()
	: m_field_1195_a(NULL)
	, m_field_1194_b(0)
	, m_field_1196_c(false)
	, m_field_1195_a_count(0)
{
}

TexturedQuad::TexturedQuad(PositionTexureVertex* var1)
	: m_field_1195_a(NULL)
	, m_field_1194_b(0)
	, m_field_1196_c(false)
	, m_field_1195_a_count(0)
{
		// var1 is a caller-local array — OWN a copy of the 4 verts so the dtor's
		// delete[] frees a real heap region, not the caller's stack.
		m_field_1195_a = new PositionTexureVertex[TQ_VERTS];
		m_field_1195_a_count = TQ_VERTS;
		for (int i = 0; i < TQ_VERTS; ++i) {
			m_field_1195_a[i] = var1[i];
		}
		m_field_1194_b = m_field_1195_a_count;
}

TexturedQuad::TexturedQuad(PositionTexureVertex* var1, int var2, int var3, int var4, int var5)
	: m_field_1195_a(NULL)
	, m_field_1194_b(0)
	, m_field_1196_c(false)
	, m_field_1195_a_count(0)
{
		// PORT: was this(var1) — own a copy of the 4 verts, then set their UVs.
		m_field_1195_a = new PositionTexureVertex[TQ_VERTS];
		m_field_1195_a_count = TQ_VERTS;
		for (int i = 0; i < TQ_VERTS; ++i) {
			m_field_1195_a[i] = var1[i];
		}
		m_field_1194_b = m_field_1195_a_count;

		float var6 = 0.0015625F;
		float var7 = 0.003125F;
		// setTexturePosition returns a heap PositionTexureVertex* — deref into the
		// slot (value copy) and free the temp to avoid leaking it.
		PositionTexureVertex* t;
		t = m_field_1195_a[0].setTexturePosition((float)var4 / 64.0F - var6, (float)var3 / 32.0F + var7); m_field_1195_a[0] = *t; delete t;
		t = m_field_1195_a[1].setTexturePosition((float)var2 / 64.0F + var6, (float)var3 / 32.0F + var7); m_field_1195_a[1] = *t; delete t;
		t = m_field_1195_a[2].setTexturePosition((float)var2 / 64.0F + var6, (float)var5 / 32.0F - var7); m_field_1195_a[2] = *t; delete t;
		t = m_field_1195_a[3].setTexturePosition((float)var4 / 64.0F - var6, (float)var5 / 32.0F - var7); m_field_1195_a[3] = *t; delete t;
}

TexturedQuad::TexturedQuad(const TexturedQuad& other)
	: m_field_1195_a(NULL)
	, m_field_1194_b(other.m_field_1194_b)
	, m_field_1196_c(other.m_field_1196_c)
	, m_field_1195_a_count(other.m_field_1195_a_count)
{
	if (other.m_field_1195_a != NULL && other.m_field_1195_a_count > 0) {
		m_field_1195_a = new PositionTexureVertex[other.m_field_1195_a_count];
		for (int i = 0; i < other.m_field_1195_a_count; ++i) {
			m_field_1195_a[i] = other.m_field_1195_a[i];
		}
	}
}

TexturedQuad& TexturedQuad::operator=(const TexturedQuad& other) {
	if (this != &other) {
		delete[] m_field_1195_a;
		m_field_1195_a = NULL;
		m_field_1194_b = other.m_field_1194_b;
		m_field_1196_c = other.m_field_1196_c;
		m_field_1195_a_count = other.m_field_1195_a_count;
		if (other.m_field_1195_a != NULL && other.m_field_1195_a_count > 0) {
			m_field_1195_a = new PositionTexureVertex[other.m_field_1195_a_count];
			for (int i = 0; i < other.m_field_1195_a_count; ++i) {
				m_field_1195_a[i] = other.m_field_1195_a[i];
			}
		}
	}
	return *this;
}

TexturedQuad::~TexturedQuad() {
	delete[] m_field_1195_a;
	m_field_1195_a = NULL;
}

void TexturedQuad::func_809_a() {
		// Reverse the vertex order (flip the quad).
		PositionTexureVertex* var1 = new PositionTexureVertex[m_field_1195_a_count];

		for(int var2 = 0; var2 < m_field_1195_a_count; ++var2) {
			var1[var2] = m_field_1195_a[m_field_1195_a_count - var2 - 1];
		}

		delete[] m_field_1195_a;
		m_field_1195_a = var1;
}

void TexturedQuad::func_808_a(Tesselator* var1, float var2) {
		Vec3D* var3 = m_field_1195_a[1].m_vector3D->func_1262_a(m_field_1195_a[0].m_vector3D);
		Vec3D* var4 = m_field_1195_a[1].m_vector3D->func_1262_a(m_field_1195_a[2].m_vector3D);
		Vec3D* var5 = var4->crossProduct(var3)->normalize();
		var1->init();
		if(m_field_1196_c) {
			var1->normal(-((float)var5->xCoord), -((float)var5->yCoord), -((float)var5->zCoord));
		} else {
			var1->normal((float)var5->xCoord, (float)var5->yCoord, (float)var5->zCoord);
		}

		for(int var6 = 0; var6 < 4; ++var6) {
			PositionTexureVertex* var7 = &m_field_1195_a[var6];
			var1->vertexUV((float)((float)var7->m_vector3D->xCoord * var2), (float)((float)var7->m_vector3D->yCoord * var2), (float)((float)var7->m_vector3D->zCoord * var2), (float)var7->m_texturePositionX, (float)var7->m_texturePositionY);
		}

		var1->flush();
}
