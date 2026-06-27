#ifndef MOVEMENTINPUTFROMOPTIONS_H
#define MOVEMENTINPUTFROMOPTIONS_H

// Auto-generated from MovementInputFromOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "MovementInput.h"

class EntityPlayer;
class GameSettings;

class MovementInputFromOptions : public MovementInput {
public:
	MovementInputFromOptions(GameSettings* var1);
	virtual ~MovementInputFromOptions();

	JArray<bool > movementKeyState;
	GameSettings* gameSettings;

	virtual void setKeyState(int var1, bool var2);
	virtual void resetKeyState();
	virtual void updatePlayerMoveState(EntityPlayer* var1);

	static void initStatics();
};

#endif // MOVEMENTINPUTFROMOPTIONS_H