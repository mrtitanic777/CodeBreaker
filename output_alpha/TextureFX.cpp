#include "stdafx.h"
// Auto-generated from TextureFX.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "TextureFX.h"
#include "Common.h"
#include "RenderEngine.h"
#include "Textures.h"

TextureFX::TextureFX()
	: m_field_1127_a(new unsigned char[1024])
	, m_field_1126_b(0)
	, m_field_1131_c(false)
	, m_field_1130_d(0)
	, m_field_1129_e(1)
	, m_field_1128_f(0)
{
}

TextureFX::TextureFX(int var1)
	: m_field_1127_a(new unsigned char[1024])
	, m_field_1126_b(0)
	, m_field_1131_c(false)
	, m_field_1130_d(0)
	, m_field_1129_e(1)
	, m_field_1128_f(0)
{
		m_field_1126_b = var1;
}

TextureFX::~TextureFX() {
	delete[] m_field_1127_a;
	m_field_1127_a = NULL;
}

void TextureFX::func_783_a() {
	// TODO: implement
}

void TextureFX::func_782_a(RenderEngine* var1) {
		if(m_field_1128_f == 0) {
   g_pd3dDevice->SetTexture(0, Textures::getById(var1->getTexture("/terrain.png")));
		} else if(m_field_1128_f == 1) {
   g_pd3dDevice->SetTexture(0, Textures::getById(var1->getTexture("/gui/items.png")));
		}

}
