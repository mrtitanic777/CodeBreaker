#include "stdafx.h"
// Hand-ported from PlayerControllerSP.java (Minecraft Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
//
// Single-player controller: block breaking/placing progress + item use.
// Matches output_alpha/PlayerControllerSP.h exactly (obfuscated signatures).
//
// Why this is a handwritten override (the converted output_alpha/PlayerControllerSP.cpp
// was correct LOGIC but failed to compile): it pulled in EntityPlayer.h instead of the
// concrete EntityPlayerSP.h, so mc->thePlayer (EntityPlayerSP*) was an undefined type and
// every EntityPlayerSP*->EntityPlayer* upcast (passed to Block::func_225_a /
// onBlockClicked / ItemStack::func_1097_a) failed with C2664. It also forward-declared
// but never fully included GuiIngame and StepSound. The fix is purely the include set:
// give the compiler the full EntityPlayerSP / GuiIngame / StepSound definitions so the
// upcast and member access resolve. The body is unchanged from the faithful conversion.

#include "PlayerControllerSP.h"
#include "Common.h"
#include "PlayerController.h"
#include "Minecraft.h"
#include "World.h"
#include "Block.h"
#include "StepSound.h"
#include "ItemStack.h"
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"   // concrete type of mc->thePlayer; gives the upcast to EntityPlayer*
#include "InventoryPlayer.h"
#include "SoundManager.h"
#include "GuiIngame.h"
#include "RenderGlobal.h"

#include <math.h>

PlayerControllerSP::PlayerControllerSP()
	: m_field_1074_c(-1)
	, m_field_1073_d(-1)
	, m_field_1072_e(-1)
	, m_field_1071_f(0.0f)
	, m_field_1070_g(0.0f)
	, m_field_1069_h(0.0f)
	, m_field_1068_i(0)
{
}

PlayerControllerSP::PlayerControllerSP(Minecraft* var1)
	: PlayerController(var1)
	, m_field_1074_c(-1)
	, m_field_1073_d(-1)
	, m_field_1072_e(-1)
	, m_field_1071_f(0.0f)
	, m_field_1070_g(0.0f)
	, m_field_1069_h(0.0f)
	, m_field_1068_i(0)
{
}

PlayerControllerSP::~PlayerControllerSP() {
	// No dynamic resources
}

void PlayerControllerSP::flipPlayer(EntityPlayer* var1) {
	var1->rotationYaw = -180.0F;
}

bool PlayerControllerSP::sendBlockRemoved(int var1, int var2, int var3, int var4) {
	int var5 = mc->theWorld->getBlockId(var1, var2, var3);
	int var6 = mc->theWorld->getBlockMetadata(var1, var2, var3);
	bool var7 = PlayerController::sendBlockRemoved(var1, var2, var3, var4);
	ItemStack* var8 = mc->thePlayer->getCurrentEquippedItem();
	bool var9 = mc->thePlayer->canHarvestBlock(Block::blocksList[var5]);
	if(var8 != NULL) {
		var8->hitBlock(var5, var1, var2, var3);
		if(var8->stackSize == 0) {
			var8->func_1097_a(mc->thePlayer);
			mc->thePlayer->destroyCurrentEquippedItem();
		}
	}

	if(var7 && var9) {
		Block::blocksList[var5]->harvestBlock(mc->theWorld, var1, var2, var3, var6);
	}

	return var7;
}

void PlayerControllerSP::clickBlock(int var1, int var2, int var3, int var4) {
	int var5 = mc->theWorld->getBlockId(var1, var2, var3);
	if(var5 > 0 && m_field_1071_f == 0.0F) {
		Block::blocksList[var5]->onBlockClicked(mc->theWorld, var1, var2, var3, mc->thePlayer);
	}

	if(var5 > 0 && Block::blocksList[var5]->func_225_a(mc->thePlayer) >= 1.0F) {
		sendBlockRemoved(var1, var2, var3, var4);
	}

}

void PlayerControllerSP::func_6468_a() {
	m_field_1071_f = 0.0F;
	m_field_1068_i = 0;
}

void PlayerControllerSP::sendBlockRemoving(int var1, int var2, int var3, int var4) {
	if(m_field_1068_i > 0) {
		--m_field_1068_i;
	} else {
		if(var1 == m_field_1074_c && var2 == m_field_1073_d && var3 == m_field_1072_e) {
			int var5 = mc->theWorld->getBlockId(var1, var2, var3);
			if(var5 == 0) {
				return;
			}

			Block* var6 = Block::blocksList[var5];
			if(var6 == NULL) {   // PORT: guard unregistered block id before deref
				return;
			}
			m_field_1071_f += var6->func_225_a(mc->thePlayer);
			if(fmodf(m_field_1069_h, 4.0F) == 0.0F && var6->stepSound != NULL) {
				mc->m_sndManager->func_336_b(var6->stepSound->func_1145_d(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->func_1147_b() + 1.0F) / 8.0F, var6->stepSound->func_1144_c() * 0.5F);
			}

			++m_field_1069_h;
			if(m_field_1071_f >= 1.0F) {
				sendBlockRemoved(var1, var2, var3, var4);
				m_field_1071_f = 0.0F;
				m_field_1070_g = 0.0F;
				m_field_1069_h = 0.0F;
				m_field_1068_i = 5;
			}
		} else {
			m_field_1071_f = 0.0F;
			m_field_1070_g = 0.0F;
			m_field_1069_h = 0.0F;
			m_field_1074_c = var1;
			m_field_1073_d = var2;
			m_field_1072_e = var3;
		}

	}
}

void PlayerControllerSP::func_6467_a(float var1) {
	if(m_field_1071_f <= 0.0F) {
		mc->m_ingameGUI->m_field_6446_b = 0.0F;
		mc->m_field_6323_f->m_field_1450_i = 0.0F;
	} else {
		float var2 = m_field_1070_g + (m_field_1071_f - m_field_1070_g) * var1;
		mc->m_ingameGUI->m_field_6446_b = var2;
		mc->m_field_6323_f->m_field_1450_i = var2;
	}

}

float PlayerControllerSP::getBlockReachDistance() {
	return 4.0F;
}

void PlayerControllerSP::func_717_a(World* var1) {
	PlayerController::func_717_a(var1);
}

void PlayerControllerSP::func_6474_c() {
	m_field_1070_g = m_field_1071_f;
	mc->m_sndManager->func_4033_c();
}
