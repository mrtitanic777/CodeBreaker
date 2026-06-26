#include "stdafx.h"
// Auto-generated from PlayerControllerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerControllerSP.h"
#include "Common.h"
#include "Minecraft.h"
#include "MobSpawner.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "EntityPlayerSP.h"
#include "Item.h"
#include "ItemStack.h"
#include "PlayerController.h"
#include "SoundManager.h"
#include "StepSound.h"
#include "InventoryPlayer.h"
#include "RenderGlobal.h"

PlayerControllerSP::PlayerControllerSP()
	: m_curBlockX(-1)
	, m_curBlockY(-1)
	, m_curBlockZ(-1)
	, m_curBlockDamage(0.0f)
	, m_prevBlockDamage(0.0f)
	, m_blockDestroySoundCounter(0.0f)
	, m_blockHitWait(0)
	, m_mobSpawner(NULL)
{
}

PlayerControllerSP::PlayerControllerSP(Minecraft* var1)
	: PlayerController(var1)
	, m_curBlockX(-1)
	, m_curBlockY(-1)
	, m_curBlockZ(-1)
	, m_curBlockDamage(0.0f)
	, m_prevBlockDamage(0.0f)
	, m_blockDestroySoundCounter(0.0f)
	, m_blockHitWait(0)
	, m_mobSpawner(NULL)
{
}

PlayerControllerSP::~PlayerControllerSP() {
	m_mobSpawner = NULL;
}

bool PlayerControllerSP::sendBlockRemoved(int var1, int var2, int var3) {
		int var4 = mc->theWorld->getBlockId(var1, var2, var3);
		unsigned char var5 = mc->theWorld->getBlockMetadata(var1, var2, var3);
		bool var6 = PlayerController::sendBlockRemoved(var1, var2, var3);
		EntityPlayerSP* var7 = mc->thePlayer;
		ItemStack* var9 = var7->m_inventory->getCurrentItem();
		if(var9 != NULL) {
			Item::itemsList[var9->itemID]->onBlockDestroyed(var9);
			if(var9->stackSize == 0) {
				mc->thePlayer->displayGUIInventory();
			}
		}

		if(var6) {
			Block::blocksList[var4]->dropBlockAsItem(mc->theWorld, var1, var2, var3, var5);
		}

		return var6;
}

void PlayerControllerSP::clickBlock(int var1, int var2, int var3) {
		int var4 = mc->theWorld->getBlockId(var1, var2, var3);
		if(var4 > 0 && Block::blocksList[var4]->blockStrength(mc->thePlayer) >= 1.0F) {
			sendBlockRemoved(var1, var2, var3);
		}

}

void PlayerControllerSP::resetBlockRemoving() {
		m_curBlockDamage = 0.0F;
		m_blockHitWait = 0;
}

void PlayerControllerSP::sendBlockRemoving(int var1, int var2, int var3, int var4) {
		if(m_blockHitWait > 0) {
			--m_blockHitWait;
		} else {
			PlayerController::sendBlockRemoving(var1, var2, var3, var4);
			if(var1 == m_curBlockX && var2 == m_curBlockY && var3 == m_curBlockZ) {
				var4 = mc->theWorld->getBlockId(var1, var2, var3);
				if(var4 != 0) {
					Block* var6 = Block::blocksList[var4];
					m_curBlockDamage += var6->blockStrength(mc->thePlayer);
					if(fmodf(m_blockDestroySoundCounter, 4.0F) == 0.0F && var6 != NULL) {
						SoundManager* var10000 = mc->m_sndManager;
						std::string var10001 = var6->stepSound->stepSoundDir2();
						float var10002 = (float)var1 + 0.5F;
						float var10003 = (float)var2 + 0.5F;
						float var10004 = (float)var3 + 0.5F;
						StepSound* var5 = var6->stepSound;
						float var10005 = (var5->m_soundVolume + 1.0F) / 8.0F;
						var5 = var6->stepSound;
						var10000->playSound(var10001, var10002, var10003, var10004, var10005, var5->m_soundPitch * 0.5F);
					}

					++m_blockDestroySoundCounter;
					if(m_curBlockDamage >= 1.0F) {
						sendBlockRemoved(var1, var2, var3);
						m_curBlockDamage = 0.0F;
						m_prevBlockDamage = 0.0F;
						m_blockDestroySoundCounter = 0.0F;
						m_blockHitWait = 5;
					}

				}
			} else {
				m_curBlockDamage = 0.0F;
				m_prevBlockDamage = 0.0F;
				m_blockDestroySoundCounter = 0.0F;
				m_curBlockX = var1;
				m_curBlockY = var2;
				m_curBlockZ = var3;
			}
		}
}

void PlayerControllerSP::setPartialTime(float var1) {
		if(m_curBlockDamage <= 0.0F) {
			mc->m_renderGlobal->m_damagePartialTime = 0.0F;
		} else {
			var1 = m_prevBlockDamage + (m_curBlockDamage - m_prevBlockDamage) * var1;
			mc->m_renderGlobal->m_damagePartialTime = var1;
		}
}

float PlayerControllerSP::getBlockReachDistance() {
		return 4.0F;
}

void PlayerControllerSP::onWorldChange(World* var1) {
		PlayerController::onWorldChange(var1);
		m_mobSpawner = new MobSpawner(var1);
}

void PlayerControllerSP::onUpdate() {
		m_prevBlockDamage = m_curBlockDamage;
		m_mobSpawner->performSpawning();
}
