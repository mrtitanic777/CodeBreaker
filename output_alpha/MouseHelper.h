#ifndef MOUSEHELPER_H
#define MOUSEHELPER_H

// Auto-generated from MouseHelper.java
// Target: Xbox 360 / C++03 / DirectX 9

class Cursor;

class MouseHelper {
public:
	MouseHelper();
	MouseHelper(void* /* PORT: was AWT Component */ var1);
	virtual ~MouseHelper();

	int m_field_1114_a;
	int m_field_1113_b;

	virtual void func_774_a();
	virtual void func_773_b();
	virtual void func_772_c();

public: // was private
	void* /* PORT: was AWT Component */ m_field_1117_c;
	void* /* PORT: was LWJGL Cursor */ m_field_1116_d;
	int m_field_1115_e;

};

#endif // MOUSEHELPER_H