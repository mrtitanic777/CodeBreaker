#include "stdafx.h"
// Auto-generated from StepSound.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "StepSound.h"
#include "Common.h"
#include <string>

StepSound::StepSound()
	: m_field_1678_a("")
	, m_field_1677_b(0)
	, m_field_1679_c(0)
{
}

StepSound::StepSound(const std::string& var1, float var2, float var3)
	: m_field_1678_a("")
	, m_field_1677_b(0)
	, m_field_1679_c(0)
{
		m_field_1678_a = var1;
		m_field_1677_b = var2;
		m_field_1679_c = var3;
}

StepSound::~StepSound() {
	// No dynamic resources
}

float StepSound::func_1147_b() {
		return m_field_1677_b;
}

float StepSound::func_1144_c() {
		return m_field_1679_c;
}

std::string StepSound::func_1146_a() {
		return "step." + toStr(m_field_1678_a);
}

std::string StepSound::func_1145_d() {
		return "step." + toStr(m_field_1678_a);
}
