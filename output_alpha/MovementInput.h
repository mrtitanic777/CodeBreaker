#ifndef MOVEMENTINPUT_H
#define MOVEMENTINPUT_H

// Auto-generated from MovementInput.java
// Target: Xbox 360 / C++03 / DirectX 9

class EntityPlayer;

class MovementInput {
public:
	MovementInput();
	virtual ~MovementInput();

	float m_field_1174_a;
	float m_field_1173_b;
	bool m_field_1177_c;
	bool m_field_1176_d;
	bool m_field_1175_e;

	virtual void func_797_a(EntityPlayer* var1);
	virtual void func_798_a();
	virtual void func_796_a(int var1, bool var2);

};

#endif // MOVEMENTINPUT_H