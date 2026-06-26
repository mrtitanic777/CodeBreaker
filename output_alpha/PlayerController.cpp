#include "stdafx.h"
// Hand-ported from PlayerController.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Base controller: block break/place, item use, player spawn + attack hooks.
// Matches output_alpha/PlayerController.h exactly (obfuscated signatures).
//
// Why this is a handwritten override (the converted output_alpha/PlayerController.cpp
// was correct LOGIC but failed to compile): it forward-declared but never fully
// included StepSound, InventoryPlayer and WorldProvider, so member access on
// stepSound (func_1146_a/b/c), m_inventory->m_mainInventory/m_currentItem, and
// m_worldProvider->m_field_4218_e all hit "use of undefined type". The fix is the
// include set; the bodies are unchanged from the faithful conversion.

#include "PlayerController.h"
#include "Common.h"
#include "SoundManager.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"   // concrete type of mc->thePlayer / func_4087_b return
#include "ItemStack.h"
#include "Minecraft.h"
#include "World.h"
#include "WorldProvider.h"    // World::m_worldProvider->m_field_4218_e
#include "Block.h"
#include "StepSound.h"        // Block::stepSound member access
#include "InventoryPlayer.h"  // EntityPlayer::m_inventory member access
#include "EffectRenderer.h"

#include <string>

PlayerController::PlayerController()
	: m_field_1064_b(false)
	, mc(NULL)
{
}

PlayerController::PlayerController(Minecraft* var1)
	: m_field_1064_b(false)
	, mc(NULL)
{
		mc = var1;
}

PlayerController::~PlayerController() {
	mc = NULL;
}

void PlayerController::func_717_a(World* var1) {
	// TODO: implement (base does nothing)
}

void PlayerController::clickBlock(int var1, int var2, int var3, int var4) {
		sendBlockRemoved(var1, var2, var3, var4);
}

bool PlayerController::sendBlockRemoved(int var1, int var2, int var3, int var4) {
		mc->m_field_6321_h->func_1186_a(var1, var2, var3);
		World* var5 = mc->theWorld;
		Block* var6 = Block::blocksList[var5->getBlockId(var1, var2, var3)];
		int var7 = var5->getBlockMetadata(var1, var2, var3);
		bool var8 = var5->setBlockWithNotify(var1, var2, var3, 0);
		if(var6 != NULL && var8) {
			mc->m_sndManager->func_336_b(var6->stepSound->func_1146_a(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->func_1147_b() + 1.0F) / 2.0F, var6->stepSound->func_1144_c() * 0.8F);
			var6->onBlockDestroyedByPlayer(var5, var1, var2, var3, var7);
		}

		return var8;
}

void PlayerController::sendBlockRemoving(int var1, int var2, int var3, int var4) {
	// TODO: implement (base does nothing)
}

void PlayerController::func_6468_a() {
	// TODO: implement (base does nothing)
}

void PlayerController::func_6467_a(float var1) {
	// TODO: implement (base does nothing)
}

float PlayerController::getBlockReachDistance() {
		return 5.0F;
}

bool PlayerController::sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3) {
		int var4 = var3->stackSize;
		ItemStack* var5 = var3->useItemRightClick(var2, var1);
		if(var5 != var3 || (var5 != NULL && var5->stackSize != var4)) {
			var1->m_inventory->m_mainInventory[var1->m_inventory->m_currentItem] = var5;
			if(var5->stackSize == 0) {
				var1->m_inventory->m_mainInventory[var1->m_inventory->m_currentItem] = NULL;
			}

			return true;
		} else {
			return false;
		}
}

void PlayerController::flipPlayer(EntityPlayer* var1) {
	// TODO: implement (base does nothing)
}

void PlayerController::func_6474_c() {
	// TODO: implement (base does nothing)
}

bool PlayerController::func_6469_d() {
		return true;
}

void PlayerController::func_6473_b(EntityPlayer* var1) {
	// TODO: implement (base does nothing)
}

bool PlayerController::sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7) {
		int var8 = var2->getBlockId(var4, var5, var6);
		return var8 > 0 && Block::blocksList[var8]->blockActivated(var2, var4, var5, var6, var1) ? true : (var3 == NULL ? false : var3->useItem(var1, var2, var4, var5, var6, var7));
}

EntityPlayer* PlayerController::func_4087_b(World* var1) {
		return new EntityPlayerSP(mc, var1, mc->m_field_6320_i, var1->m_worldProvider->m_field_4218_e);
}

void PlayerController::func_6475_a(EntityPlayer* var1, Entity* var2) {
		var1->func_6415_a_(var2);
}

void PlayerController::func_6472_b(EntityPlayer* var1, Entity* var2) {
		var1->attackTargetEntityWithCurrentItem(var2);
}
