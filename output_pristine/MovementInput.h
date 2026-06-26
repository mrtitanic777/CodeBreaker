#ifndef MOVEMENTINPUT_H
#define MOVEMENTINPUT_H

// Auto-generated from MovementInput.java
// Target: Xbox 360 / C++03 / DirectX 9

class MovementInput {
public:
	MovementInput();
	virtual ~MovementInput();

	float m_moveStrafe;
	float moveForward;
	bool m_jump;

	virtual void updatePlayerMoveState();
	virtual void resetKeyState();
	virtual void checkKeyForMovementInput(int var1, bool var2);

};

#endif // MOVEMENTINPUT_H