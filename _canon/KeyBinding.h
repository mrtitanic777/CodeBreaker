#ifndef KEYBINDING_H
#define KEYBINDING_H

// Auto-generated from KeyBinding.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class KeyBinding {
public:
	KeyBinding(const JavaString& var1, int var2);
	virtual ~KeyBinding();

	JavaString keyDescription;
	int keyCode;

	static void initStatics();
};

#endif // KEYBINDING_H