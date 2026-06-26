#include "stdafx.h"
// Auto-generated from RenderList.java  (handwritten_alpha override)
// FIX: the converter stripped a Java `if(buffer.remaining()==0){...}` to a comment
// but left its closing brace dangling, producing a stray `}` that cascaded into
// ~10 syntax errors. Braces balanced here. The actual batched draw is stubbed
// (display-list batching -> // PORT below), matching the rest of the port.
// Target: Xbox 360 / C++03 / DirectX 9

#include "RenderList.h"
#include "Common.h"
#include "Shaders.h"
#include "GLAllocation.h"

RenderList::RenderList()
	: m_field_1242_a(0)
	, m_field_1241_b(0)
	, m_field_1240_c(0)
	, m_field_1239_d(0)
	, m_field_1238_e(0)
	, m_field_1237_f(0)
	, m_field_1236_g(GLAllocation::createDirectIntBuffer(65536))
	, m_field_1235_h(false)
	, m_field_1234_i(false)
{
}

RenderList::~RenderList() {
	// No dynamic resources
}

void RenderList::func_861_a(int var1, int var2, int var3, double var4, double var6, double var8) {
	m_field_1235_h = true;
	// NIO clear() -> reset position (no-op for our C array)
	m_field_1242_a = var1;
	m_field_1241_b = var2;
	m_field_1240_c = var3;
	m_field_1239_d = (float)var4;
	m_field_1238_e = (float)var6;
	m_field_1237_f = (float)var8;
}

bool RenderList::func_862_a(int var1, int var2, int var3) {
	return !m_field_1235_h ? false : var1 == m_field_1242_a && var2 == m_field_1241_b && var3 == m_field_1240_c;
}

void RenderList::func_858_a(int var1) {
	m_field_1236_g[0] = var1;
	// NIO put()/remaining()==0 flush -> no-op for C array; draw is stubbed in func_860_a
	func_860_a();
}

void RenderList::func_860_a() {
	if(m_field_1235_h) {
		if(!m_field_1234_i) {
			// NIO flip() -> no-op for C array
			m_field_1234_i = true;
		}

		// NIO remaining() -> no-op for C array
		D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
		{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)this->m_field_1242_a - this->m_field_1239_d, (float)this->m_field_1241_b - this->m_field_1238_e, (float)this->m_field_1240_c - this->m_field_1237_f);
		  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
		  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		// PORT: batch draw cached vertex buffers
		Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
	}
}

void RenderList::func_859_b() {
	m_field_1235_h = false;
	m_field_1234_i = false;
}
