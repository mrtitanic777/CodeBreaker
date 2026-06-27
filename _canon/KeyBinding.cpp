#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from KeyBinding.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "KeyBinding.h"

KeyBinding::KeyBinding(const JavaString& var1, int var2)
	: keyCode(0)
{
	this->keyDescription = var1;
	this->keyCode = var2;
}

KeyBinding::~KeyBinding()
{
}

void KeyBinding::initStatics()
{
}
