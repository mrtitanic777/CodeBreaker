#include "stdafx.h"
// Auto-generated from KeyBinding.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "KeyBinding.h"
#include "Common.h"
#include <string>

KeyBinding::KeyBinding()
	: m_keyCode(0)
{
}

KeyBinding::KeyBinding(const std::string& var1, int var2)
	: m_keyCode(0)
{
		m_keyDescription = var1;
		m_keyCode = var2;
}

KeyBinding::~KeyBinding() {
	// No dynamic resources
}
