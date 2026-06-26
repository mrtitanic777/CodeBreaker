#include "stdafx.h"
// Auto-generated from PlayerControllerCreative.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerControllerCreative.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "Minecraft.h"
#include "MobSpawner.h"
#include "World.h"
#include "ItemStack.h"
#include "PlayerController.h"

PlayerControllerCreative::PlayerControllerCreative()
	: m_mobSpawner(NULL)
{
}

PlayerControllerCreative::PlayerControllerCreative(Minecraft* var1)
	: PlayerController(var1)
	, m_mobSpawner(NULL)
{
}

PlayerControllerCreative::~PlayerControllerCreative() {
	m_mobSpawner = NULL;
}

void PlayerControllerCreative::onRespawn(EntityPlayer* var1) {
		for(int var2 = 0; var2 < 9; ++var2) {
			if(var1->m_inventory.m_mainInventory[var2] == NULL) {
				mc->thePlayer->m_inventory.m_mainInventory[var2] = ItemStack(((Block)Session.registeredBlocksList[var2]).blockID);
			} else {
				mc->thePlayer->m_inventory.m_mainInventory[var2].stackSize = 1;
			}
		}

}

bool PlayerControllerCreative::shouldDrawHUD() {
		return false;
}

void PlayerControllerCreative::onWorldChange(World* var1) {
		PlayerController::onWorldChange(var1);
		var1->m_survivalWorld = false;
		m_mobSpawner = new MobSpawner(var1);
}

void PlayerControllerCreative::onUpdate() {
		m_mobSpawner->performSpawning();
}
