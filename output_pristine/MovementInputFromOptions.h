#ifndef MOVEMENTINPUTFROMOPTIONS_H
#define MOVEMENTINPUTFROMOPTIONS_H

// Auto-generated from MovementInputFromOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovementInput.h"

class GameSettings;

class MovementInputFromOptions : public MovementInput {
public:
	MovementInputFromOptions();
	MovementInputFromOptions(GameSettings* var1);
	virtual ~MovementInputFromOptions();

	virtual void checkKeyForMovementInput(int var1, bool var2);
	virtual void resetKeyState();
	virtual void updatePlayerMoveState();

private:
	bool* m_movementKeyStates;
	GameSettings* m_gameSettings;

public:  // array-length companions
	int m_movementKeyStates_count;

};

#endif // MOVEMENTINPUTFROMOPTIONS_H