#ifndef MOVEMENTINPUT_H
#define MOVEMENTINPUT_H

// Auto-generated from MovementInput.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class EntityPlayer;

class MovementInput {
public:
	MovementInput();
	virtual ~MovementInput();

	float moveStrafe;
	float moveForward;
	bool field_1177_c;
	bool jump;
	bool sneak;

	virtual void updatePlayerMoveState(EntityPlayer* var1);
	virtual void resetKeyState();
	virtual void setKeyState(int var1, bool var2);

	static void initStatics();
};

#endif // MOVEMENTINPUT_H