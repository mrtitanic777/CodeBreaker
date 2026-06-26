#include "stdafx.h"
// Auto-generated from MovementInputFromOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovementInputFromOptions.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "GameSettings.h"

MovementInputFromOptions::MovementInputFromOptions()
	: m_field_1179_f(new bool[10])
	, m_field_1178_g(NULL)
{
}

MovementInputFromOptions::MovementInputFromOptions(GameSettings* var1)
	: m_field_1179_f(new bool[10])
	, m_field_1178_g(NULL)
{
		m_field_1178_g = var1;
}

MovementInputFromOptions::~MovementInputFromOptions() {
	delete[] m_field_1179_f;
	m_field_1179_f = NULL;
	m_field_1178_g = NULL;
}

void MovementInputFromOptions::func_796_a(int var1, bool var2) {
		unsigned char var3 = -1;
		if(var1 == m_field_1178_g->m_keyBindForward.m_keyCode) {
			var3 = 0;
		}

		if(var1 == m_field_1178_g->m_keyBindBack.m_keyCode) {
			var3 = 1;
		}

		if(var1 == m_field_1178_g->m_keyBindLeft.m_keyCode) {
			var3 = 2;
		}

		if(var1 == m_field_1178_g->m_keyBindRight.m_keyCode) {
			var3 = 3;
		}

		if(var1 == m_field_1178_g->m_keyBindJump.m_keyCode) {
			var3 = 4;
		}

		if(var1 == m_field_1178_g->m_keyBindSneak.m_keyCode) {
			var3 = 5;
		}

		if(var3 >= 0) {
			m_field_1179_f[var3] = var2;
		}

}

void MovementInputFromOptions::func_798_a() {
		for(int var1 = 0; var1 < 10; ++var1) {
			m_field_1179_f[var1] = false;
		}

}

void MovementInputFromOptions::func_797_a(EntityPlayer* var1) {
		m_field_1174_a = 0.0F;
		m_field_1173_b = 0.0F;
		if(m_field_1179_f[0]) {
			++m_field_1173_b;
		}

		if(m_field_1179_f[1]) {
			--m_field_1173_b;
		}

		if(m_field_1179_f[2]) {
			++m_field_1174_a;
		}

		if(m_field_1179_f[3]) {
			--m_field_1174_a;
		}

		m_field_1176_d = m_field_1179_f[4];
		m_field_1175_e = m_field_1179_f[5];
		if(m_field_1175_e) {
			m_field_1174_a = (float)((double)m_field_1174_a * 0.3f);
			m_field_1173_b = (float)((double)m_field_1173_b * 0.3f);
		}

}
