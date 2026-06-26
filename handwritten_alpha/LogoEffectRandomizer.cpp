#include "stdafx.h"
// Auto-generated from LogoEffectRandomizer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "LogoEffectRandomizer.h"
#include "Common.h"
#include "GuiMainMenu.h"
#include "Random.h"

LogoEffectRandomizer::LogoEffectRandomizer()
	: m_field_1312_a(0)
	, m_field_1311_b(0)
	, m_field_1314_c(0)
	, m_mainMenu(NULL)
{
}

LogoEffectRandomizer::LogoEffectRandomizer(GuiMainMenu* var1, int var2, int var3)
	: m_field_1312_a(0)
	, m_field_1311_b(0)
	, m_field_1314_c(0)
	, m_mainMenu(NULL)
{
		m_mainMenu = var1;
		m_field_1312_a = m_field_1311_b = (double)(10 + var3) + m_mainMenu->getRand()->nextDouble() * 32.0f + (double)var2;
}

LogoEffectRandomizer::~LogoEffectRandomizer() {
	m_mainMenu = NULL;
}

void LogoEffectRandomizer::func_875_a() {
		m_field_1311_b = m_field_1312_a;
		if(m_field_1312_a > 0.0f) {
			m_field_1314_c -= 0.6f;
		}

		m_field_1312_a += m_field_1314_c;
		m_field_1314_c *= 0.9f;
		if(m_field_1312_a < 0.0f) {
			m_field_1312_a = 0.0f;
			m_field_1314_c = 0.0f;
		}

}
