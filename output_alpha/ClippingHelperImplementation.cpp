#include "stdafx.h"
// Auto-generated from ClippingHelperImplementation.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ClippingHelperImplementation.h"
#include "Shaders.h"
#include "Common.h"
#include "ClippingHelper.h"
#include "GLAllocation.h"

ClippingHelperImplementation* ClippingHelperImplementation::s_field_1694_e = new ClippingHelperImplementation();

ClippingHelperImplementation::ClippingHelperImplementation()
	: m_field_1693_f(GLAllocation::createDirectFloatBuffer(16))
	, m_field_1692_g(GLAllocation::createDirectFloatBuffer(16))
	, m_field_1691_h(GLAllocation::createDirectFloatBuffer(16))
{
	// PORT: base ClippingHelper leaves m_field_1688_a (the 6x4 frustum-plane matrix)
	// NULL. Allocate it here so func_1153_b can fill it and func_1152_a can read it.
	m_field_1688_a = new float*[6];
	for (int i = 0; i < 6; ++i) {
		m_field_1688_a[i] = new float[4];
	}
	m_field_1688_a_count = 6;
}

ClippingHelperImplementation::~ClippingHelperImplementation() {
	// m_field_1688_a is owned by the base dtor's delete[] (single-level); free the
	// inner rows we allocated here to avoid leaking them.
	if (m_field_1688_a != NULL) {
		for (int i = 0; i < 6; ++i) {
			delete[] m_field_1688_a[i];
			m_field_1688_a[i] = NULL;
		}
	}
}

ClippingHelper* ClippingHelperImplementation::func_1155_a() {
		s_field_1694_e->func_1153_b();
		return s_field_1694_e;
}

void ClippingHelperImplementation::func_1154_a(float** var1, int var2) {
		float var3 = sqrtf(var1[var2][0] * var1[var2][0] + var1[var2][1] * var1[var2][1] + var1[var2][2] * var1[var2][2]);
		var1[var2][0] /= var3;
		var1[var2][1] /= var3;
		var1[var2][2] /= var3;
		var1[var2][3] /= var3;
}

void ClippingHelperImplementation::func_1153_b() {
  // PORT: Java read GL_PROJECTION_MATRIX and GL_MODELVIEW_MATRIX into NIO
  // buffers, flip()'d them, then copied into the proj (m_field_1687_b) and
  // modelview (m_field_1690_c) float[16] arrays. On D3D9 fetch the matrices via
  // GetTransform. GL stored matrices column-major (flat[col*4+row]); copy the
  // D3DMATRIX (m[row][col]) in that same column-major flat order so the
  // multiply below stays faithful to the original frustum math.
  // Xbox 360 (Xenos) is fully shader-based: D3DDevice has NO GetTransform. The live
  // projection/view matrices are tracked in Shaders:: instead. D3DXMATRIX exposes
  // m[row][col] just like the old D3DMATRIX, so the column-major copy below is unchanged.
  const D3DXMATRIX& projM = Shaders::getProjectionMatrix();
  const D3DXMATRIX& viewM = Shaders::getViewMatrix();
  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      m_field_1687_b[c * 4 + r] = projM.m[r][c];  // projection
      m_field_1690_c[c * 4 + r] = viewM.m[r][c];  // modelview
    }
  }
		m_field_1689_d[0] = m_field_1690_c[0] * m_field_1687_b[0] + m_field_1690_c[1] * m_field_1687_b[4] + m_field_1690_c[2] * m_field_1687_b[8] + m_field_1690_c[3] * m_field_1687_b[12];
		m_field_1689_d[1] = m_field_1690_c[0] * m_field_1687_b[1] + m_field_1690_c[1] * m_field_1687_b[5] + m_field_1690_c[2] * m_field_1687_b[9] + m_field_1690_c[3] * m_field_1687_b[13];
		m_field_1689_d[2] = m_field_1690_c[0] * m_field_1687_b[2] + m_field_1690_c[1] * m_field_1687_b[6] + m_field_1690_c[2] * m_field_1687_b[10] + m_field_1690_c[3] * m_field_1687_b[14];
		m_field_1689_d[3] = m_field_1690_c[0] * m_field_1687_b[3] + m_field_1690_c[1] * m_field_1687_b[7] + m_field_1690_c[2] * m_field_1687_b[11] + m_field_1690_c[3] * m_field_1687_b[15];
		m_field_1689_d[4] = m_field_1690_c[4] * m_field_1687_b[0] + m_field_1690_c[5] * m_field_1687_b[4] + m_field_1690_c[6] * m_field_1687_b[8] + m_field_1690_c[7] * m_field_1687_b[12];
		m_field_1689_d[5] = m_field_1690_c[4] * m_field_1687_b[1] + m_field_1690_c[5] * m_field_1687_b[5] + m_field_1690_c[6] * m_field_1687_b[9] + m_field_1690_c[7] * m_field_1687_b[13];
		m_field_1689_d[6] = m_field_1690_c[4] * m_field_1687_b[2] + m_field_1690_c[5] * m_field_1687_b[6] + m_field_1690_c[6] * m_field_1687_b[10] + m_field_1690_c[7] * m_field_1687_b[14];
		m_field_1689_d[7] = m_field_1690_c[4] * m_field_1687_b[3] + m_field_1690_c[5] * m_field_1687_b[7] + m_field_1690_c[6] * m_field_1687_b[11] + m_field_1690_c[7] * m_field_1687_b[15];
		m_field_1689_d[8] = m_field_1690_c[8] * m_field_1687_b[0] + m_field_1690_c[9] * m_field_1687_b[4] + m_field_1690_c[10] * m_field_1687_b[8] + m_field_1690_c[11] * m_field_1687_b[12];
		m_field_1689_d[9] = m_field_1690_c[8] * m_field_1687_b[1] + m_field_1690_c[9] * m_field_1687_b[5] + m_field_1690_c[10] * m_field_1687_b[9] + m_field_1690_c[11] * m_field_1687_b[13];
		m_field_1689_d[10] = m_field_1690_c[8] * m_field_1687_b[2] + m_field_1690_c[9] * m_field_1687_b[6] + m_field_1690_c[10] * m_field_1687_b[10] + m_field_1690_c[11] * m_field_1687_b[14];
		m_field_1689_d[11] = m_field_1690_c[8] * m_field_1687_b[3] + m_field_1690_c[9] * m_field_1687_b[7] + m_field_1690_c[10] * m_field_1687_b[11] + m_field_1690_c[11] * m_field_1687_b[15];
		m_field_1689_d[12] = m_field_1690_c[12] * m_field_1687_b[0] + m_field_1690_c[13] * m_field_1687_b[4] + m_field_1690_c[14] * m_field_1687_b[8] + m_field_1690_c[15] * m_field_1687_b[12];
		m_field_1689_d[13] = m_field_1690_c[12] * m_field_1687_b[1] + m_field_1690_c[13] * m_field_1687_b[5] + m_field_1690_c[14] * m_field_1687_b[9] + m_field_1690_c[15] * m_field_1687_b[13];
		m_field_1689_d[14] = m_field_1690_c[12] * m_field_1687_b[2] + m_field_1690_c[13] * m_field_1687_b[6] + m_field_1690_c[14] * m_field_1687_b[10] + m_field_1690_c[15] * m_field_1687_b[14];
		m_field_1689_d[15] = m_field_1690_c[12] * m_field_1687_b[3] + m_field_1690_c[13] * m_field_1687_b[7] + m_field_1690_c[14] * m_field_1687_b[11] + m_field_1690_c[15] * m_field_1687_b[15];
		m_field_1688_a[0][0] = m_field_1689_d[3] - m_field_1689_d[0];
		m_field_1688_a[0][1] = m_field_1689_d[7] - m_field_1689_d[4];
		m_field_1688_a[0][2] = m_field_1689_d[11] - m_field_1689_d[8];
		m_field_1688_a[0][3] = m_field_1689_d[15] - m_field_1689_d[12];
		func_1154_a(m_field_1688_a, 0);
		m_field_1688_a[1][0] = m_field_1689_d[3] + m_field_1689_d[0];
		m_field_1688_a[1][1] = m_field_1689_d[7] + m_field_1689_d[4];
		m_field_1688_a[1][2] = m_field_1689_d[11] + m_field_1689_d[8];
		m_field_1688_a[1][3] = m_field_1689_d[15] + m_field_1689_d[12];
		func_1154_a(m_field_1688_a, 1);
		m_field_1688_a[2][0] = m_field_1689_d[3] + m_field_1689_d[1];
		m_field_1688_a[2][1] = m_field_1689_d[7] + m_field_1689_d[5];
		m_field_1688_a[2][2] = m_field_1689_d[11] + m_field_1689_d[9];
		m_field_1688_a[2][3] = m_field_1689_d[15] + m_field_1689_d[13];
		func_1154_a(m_field_1688_a, 2);
		m_field_1688_a[3][0] = m_field_1689_d[3] - m_field_1689_d[1];
		m_field_1688_a[3][1] = m_field_1689_d[7] - m_field_1689_d[5];
		m_field_1688_a[3][2] = m_field_1689_d[11] - m_field_1689_d[9];
		m_field_1688_a[3][3] = m_field_1689_d[15] - m_field_1689_d[13];
		func_1154_a(m_field_1688_a, 3);
		m_field_1688_a[4][0] = m_field_1689_d[3] - m_field_1689_d[2];
		m_field_1688_a[4][1] = m_field_1689_d[7] - m_field_1689_d[6];
		m_field_1688_a[4][2] = m_field_1689_d[11] - m_field_1689_d[10];
		m_field_1688_a[4][3] = m_field_1689_d[15] - m_field_1689_d[14];
		func_1154_a(m_field_1688_a, 4);
		m_field_1688_a[5][0] = m_field_1689_d[3] + m_field_1689_d[2];
		m_field_1688_a[5][1] = m_field_1689_d[7] + m_field_1689_d[6];
		m_field_1688_a[5][2] = m_field_1689_d[11] + m_field_1689_d[10];
		m_field_1688_a[5][3] = m_field_1689_d[15] + m_field_1689_d[14];
		func_1154_a(m_field_1688_a, 5);
}
