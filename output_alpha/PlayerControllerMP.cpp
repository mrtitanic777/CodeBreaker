#include "stdafx.h"
// Auto-generated from PlayerControllerMP.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. Every addToSendQueue(new Packet...) call is
// stubbed (no-op) -- the local block/dig/place logic still runs via the
// PlayerController base, only the network notifications are dropped.

#include "PlayerControllerMP.h"
#include "Common.h"
#include "SoundManager.h"
#include "Entity.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Minecraft.h"
#include "NetClientHandler.h"
#include "World.h"
#include "StepSound.h"
#include "GuiIngame.h"
#include "RenderGlobal.h"
#include "InventoryPlayer.h"
#include "Block.h"
#include "PlayerController.h"
#include "EntityClientPlayerMP.h"

PlayerControllerMP::PlayerControllerMP()
	: m_field_9445_c(-1)
	, m_field_9444_d(-1)
	, m_field_9443_e(-1)
	, m_field_9442_f(0.0f)
	, m_field_1080_g(0.0f)
	, m_field_9441_h(0.0f)
	, m_field_9440_i(0)
	, m_field_9439_j(false)
	, m_field_9438_k(NULL)
	, m_field_1075_l(0)
{
}

PlayerControllerMP::PlayerControllerMP(Minecraft* var1, NetClientHandler* var2)
	: PlayerController(var1)
	, m_field_9445_c(-1)
	, m_field_9444_d(-1)
	, m_field_9443_e(-1)
	, m_field_9442_f(0.0f)
	, m_field_1080_g(0.0f)
	, m_field_9441_h(0.0f)
	, m_field_9440_i(0)
	, m_field_9439_j(false)
	, m_field_9438_k(NULL)
	, m_field_1075_l(0)
{
		m_field_9438_k = var2;
}

PlayerControllerMP::~PlayerControllerMP() {
	m_field_9438_k = NULL;
}

void PlayerControllerMP::flipPlayer(EntityPlayer* var1) {
		var1->rotationYaw = -180.0F;
}

bool PlayerControllerMP::sendBlockRemoved(int var1, int var2, int var3, int var4) {
		// PORT: 360 MP API TODO -- Packet14BlockDig(3,...) (no-op).
		int var5 = mc->theWorld->getBlockId(var1, var2, var3);
		mc->theWorld->getBlockMetadata(var1, var2, var3);
		bool var7 = PlayerController::sendBlockRemoved(var1, var2, var3, var4);
		ItemStack* var8 = mc->thePlayer->getCurrentEquippedItem();
		if(var8 != NULL) {
			var8->hitBlock(var5, var1, var2, var3);
			if(var8->stackSize == 0) {
				var8->func_1097_a(mc->thePlayer);
				mc->thePlayer->destroyCurrentEquippedItem();
			}
		}

		return var7;
}

void PlayerControllerMP::clickBlock(int var1, int var2, int var3, int var4) {
		m_field_9439_j = true;
		// PORT: 360 MP API TODO -- Packet14BlockDig(0,...) (no-op).
		int var5 = mc->theWorld->getBlockId(var1, var2, var3);
		if(var5 > 0 && m_field_9442_f == 0.0F) {
			Block::blocksList[var5]->onBlockClicked(mc->theWorld, var1, var2, var3, mc->thePlayer);
		}

		if(var5 > 0 && Block::blocksList[var5]->func_225_a(mc->thePlayer) >= 1.0F) {
			sendBlockRemoved(var1, var2, var3, var4);
		}

}

void PlayerControllerMP::func_6468_a() {
		if(m_field_9439_j) {
			m_field_9439_j = false;
			// PORT: 360 MP API TODO -- Packet14BlockDig(2,...) (no-op).
			m_field_9442_f = 0.0F;
			m_field_9440_i = 0;
		}
}

void PlayerControllerMP::sendBlockRemoving(int var1, int var2, int var3, int var4) {
		m_field_9439_j = true;
		func_730_e();
		// PORT: 360 MP API TODO -- Packet14BlockDig(1,...) (no-op).
		if(m_field_9440_i > 0) {
			--m_field_9440_i;
		} else {
			if(var1 == m_field_9445_c && var2 == m_field_9444_d && var3 == m_field_9443_e) {
				int var5 = mc->theWorld->getBlockId(var1, var2, var3);
				if(var5 == 0) {
					return;
				}

				Block* var6 = Block::blocksList[var5];
				m_field_9442_f += var6->func_225_a(mc->thePlayer);
				if(fmodf(m_field_9441_h, 4.0F) == 0.0F && var6 != NULL) {
					mc->m_sndManager->func_336_b(var6->stepSound->func_1145_d(), (float)var1 + 0.5F, (float)var2 + 0.5F, (float)var3 + 0.5F, (var6->stepSound->func_1147_b() + 1.0F) / 8.0F, var6->stepSound->func_1144_c() * 0.5F);
				}

				++m_field_9441_h;
				if(m_field_9442_f >= 1.0F) {
					sendBlockRemoved(var1, var2, var3, var4);
					m_field_9442_f = 0.0F;
					m_field_1080_g = 0.0F;
					m_field_9441_h = 0.0F;
					m_field_9440_i = 5;
				}
			} else {
				m_field_9442_f = 0.0F;
				m_field_1080_g = 0.0F;
				m_field_9441_h = 0.0F;
				m_field_9445_c = var1;
				m_field_9444_d = var2;
				m_field_9443_e = var3;
			}

		}
}

void PlayerControllerMP::func_6467_a(float var1) {
		if(m_field_9442_f <= 0.0F) {
			mc->m_ingameGUI->m_field_6446_b = 0.0F;
			mc->m_field_6323_f->m_field_1450_i = 0.0F;
		} else {
			float var2 = m_field_1080_g + (m_field_9442_f - m_field_1080_g) * var1;
			mc->m_ingameGUI->m_field_6446_b = var2;
			mc->m_field_6323_f->m_field_1450_i = var2;
		}

}

float PlayerControllerMP::getBlockReachDistance() {
		return 4.0F;
}

void PlayerControllerMP::func_717_a(World* var1) {
		PlayerController::func_717_a(var1);
}

void PlayerControllerMP::func_6474_c() {
		func_730_e();
		m_field_1080_g = m_field_9442_f;
		mc->m_sndManager->func_4033_c();
}

void PlayerControllerMP::func_730_e() {
		ItemStack* var1 = mc->thePlayer->m_inventory->getCurrentItem();
		int var2 = 0;
		if(var1 != NULL) {
			var2 = var1->itemID;
		}

		if(var2 != m_field_1075_l) {
			m_field_1075_l = var2;
			// PORT: 360 MP API TODO -- Packet16BlockItemSwitch (no-op).
		}

}

bool PlayerControllerMP::sendPlaceBlock(EntityPlayer* var1, World* var2, ItemStack* var3, int var4, int var5, int var6, int var7) {
		func_730_e();
		// PORT: 360 MP API TODO -- Packet15Place (no-op).
		return PlayerController::sendPlaceBlock(var1, var2, var3, var4, var5, var6, var7);
}

bool PlayerControllerMP::sendUseItem(EntityPlayer* var1, World* var2, ItemStack* var3) {
		func_730_e();
		// PORT: 360 MP API TODO -- Packet15Place (no-op).
		return PlayerController::sendUseItem(var1, var2, var3);
}

EntityPlayer* PlayerControllerMP::func_4087_b(World* var1) {
		return new EntityClientPlayerMP(mc, var1, mc->m_field_6320_i, m_field_9438_k);
}

void PlayerControllerMP::func_6472_b(EntityPlayer* var1, Entity* var2) {
		func_730_e();
		// PORT: 360 MP API TODO -- Packet7 use-entity (no-op).
		var1->attackTargetEntityWithCurrentItem(var2);
}

void PlayerControllerMP::func_6475_a(EntityPlayer* var1, Entity* var2) {
		func_730_e();
		// PORT: 360 MP API TODO -- Packet7 use-entity (no-op).
		var1->func_6415_a_(var2);
}
