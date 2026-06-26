#include "stdafx.h"
// Auto-generated from MovementInputFromOptions.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "MovementInputFromOptions.h"
#include "Common.h"
#include "GameSettings.h"

MovementInputFromOptions::MovementInputFromOptions()
	: m_movementKeyStates(new bool[10])
	, m_gameSettings(NULL)
{
}

MovementInputFromOptions::MovementInputFromOptions(GameSettings* var1)
	: m_movementKeyStates(new bool[10])
	, m_gameSettings(NULL)
{
		m_gameSettings = var1;
}

MovementInputFromOptions::~MovementInputFromOptions() {
	delete[] m_movementKeyStates;
	m_movementKeyStates = NULL;
	m_gameSettings = NULL;
}

void MovementInputFromOptions::checkKeyForMovementInput(int var1, bool var2) {
		unsigned char var3 = -1;
		if(var1 == m_gameSettings->m_keyBindForward.m_keyCode) {
			var3 = 0;
		}

		if(var1 == m_gameSettings->m_keyBindBack.m_keyCode) {
			var3 = 1;
		}

		if(var1 == m_gameSettings->m_keyBindLeft.m_keyCode) {
			var3 = 2;
		}

		if(var1 == m_gameSettings->m_keyBindRight.m_keyCode) {
			var3 = 3;
		}

		if(var1 == m_gameSettings->m_keyBindJump.m_keyCode) {
			var3 = 4;
		}

		if(var3 >= 0) {
			m_movementKeyStates[var3] = var2;
		}

}

void MovementInputFromOptions::resetKeyState() {
		for(int var1 = 0; var1 < 10; ++var1) {
			m_movementKeyStates[var1] = false;
		}

}

void MovementInputFromOptions::updatePlayerMoveState() {
		m_moveStrafe = 0.0F;
		moveForward = 0.0F;
		if(m_movementKeyStates[0]) {
			--moveForward;
		}

		if(m_movementKeyStates[1]) {
			++moveForward;
		}

		if(m_movementKeyStates[2]) {
			--m_moveStrafe;
		}

		if(m_movementKeyStates[3]) {
			++m_moveStrafe;
		}

		m_jump = m_movementKeyStates[4];
}
