#include "stdafx.h"
// Hand-ported from PlayerControllerTest.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Creative/"test" controller: refills the hotbar, hides the survival HUD.
// Matches output_alpha/PlayerControllerTest.h exactly (obfuscated signatures).
// Indev's PlayerControllerCreative is the porting guide for the same behavior.
//
// Why this is a handwritten override (the converted output_alpha/PlayerControllerTest.cpp
// was correct LOGIC but failed to compile):
//   * it forward-declared but never included InventoryPlayer.h / EntityPlayerSP.h, so
//     mc->thePlayer (EntityPlayerSP*) and ->m_inventory->m_mainInventory were undefined.
//   * it referenced "Session.registeredBlocksList[var2]" — Session is a stripped Xbox
//     stub with no such list, and it was used as a type not an instance. Per the indev
//     PlayerControllerCreative port, the creative palette is stubbed: give the hotbar
//     slot the block whose id == var2 + 1 (stone, grass, dirt, ...). PORT below.

#include "PlayerControllerTest.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"   // concrete type of mc->thePlayer
#include "Minecraft.h"
#include "World.h"
#include "ItemStack.h"
#include "PlayerController.h"
#include "Block.h"
#include "InventoryPlayer.h"  // m_inventory->m_mainInventory member access

PlayerControllerTest::PlayerControllerTest()
{
}

PlayerControllerTest::PlayerControllerTest(Minecraft* var1)
	: PlayerController(var1)
{
		m_field_1064_b = true;
}

PlayerControllerTest::~PlayerControllerTest() {
	// No dynamic resources
}

void PlayerControllerTest::func_6473_b(EntityPlayer* var1) {
		for(int var2 = 0; var2 < 9; ++var2) {
			if(var1->m_inventory->m_mainInventory[var2] == NULL) {
				// PORT: creative palette was Session.registeredBlocksList[var2] (stripped on
				// Xbox); stub to the block whose id == var2 + 1, matching indev's port.
				mc->thePlayer->m_inventory->m_mainInventory[var2] = new ItemStack(var2 + 1);
			} else {
				mc->thePlayer->m_inventory->m_mainInventory[var2]->stackSize = 1;
			}
		}

}

bool PlayerControllerTest::func_6469_d() {
		return false;
}

void PlayerControllerTest::func_717_a(World* var1) {
		PlayerController::func_717_a(var1);
}

void PlayerControllerTest::func_6474_c() {
	// TODO: implement
}
