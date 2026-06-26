#ifndef MOVEMENTINPUTFROMOPTIONS_H
#define MOVEMENTINPUTFROMOPTIONS_H

// Auto-generated from MovementInputFromOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovementInput.h"

class EntityPlayer;
class GameSettings;

class MovementInputFromOptions : public MovementInput {
public:
	MovementInputFromOptions();
	MovementInputFromOptions(GameSettings* var1);
	virtual ~MovementInputFromOptions();

	virtual void func_796_a(int var1, bool var2);
	virtual void func_798_a();
	virtual void func_797_a(EntityPlayer* var1);

public: // was private
	bool* m_field_1179_f;
	GameSettings* m_field_1178_g;

public:  // array-length companions
	int m_field_1179_f_count;

};

#endif // MOVEMENTINPUTFROMOPTIONS_H