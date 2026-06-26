#ifndef KEYBINDING_H
#define KEYBINDING_H

// Auto-generated from KeyBinding.java
// Target: Xbox 360 / C++03 / DirectX 9

#include <string>

class KeyBinding {
public:
	KeyBinding();
	KeyBinding(const std::string& var1, int var2);
	virtual ~KeyBinding();

	std::string m_keyDescription;
	int m_keyCode;


};

#endif // KEYBINDING_H