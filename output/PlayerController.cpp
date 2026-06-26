#include "stdafx.h"
// Auto-generated from PlayerController.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "PlayerController.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "Minecraft.h"
#include "World.h"
#include <string>
#include "Block.h"
#include "SoundManager.h"
#include "StepSound.h"
#include "EffectRenderer.h"

PlayerController::PlayerController()
	: mc(NULL)
	, m_isInTestMode(false)
{
}

PlayerController::PlayerController(Minecraft* var1)
	: mc(NULL)
	, m_isInTestMode(false)
{
		mc = var1;
}

PlayerController::~PlayerController() {
	mc = NULL;
}

void PlayerController::onWorldChange(World* var1) {
	// TODO: implement
}

void PlayerController::clickBlock(int var1, int var2, int var3) {
		sendBlockRemoved(var1, var2, var3);
}

bool PlayerController::sendBlockRemoved(int var1, int var2, int var3) {
		mc->m_effectRenderer->addBlockDestroyEffects(var1, var2, var3);
		World* var4 = mc->theWorld;
		Block* var5 = Block::blocksList[var4->getBlockId(var1, var2, var3)];
		unsigned char var6 = var4->getBlockMetadata(var1, var2, var3);
		bool var7 = var4->setBlockWithNotify(var1, var2, var3, 0);
		if(var5 != NULL && var7) {
			SoundManager* var10000 = mc->m_sndManager;
			std::string var10001 = var5->stepSound->stepSoundDir();
			float var10002 = (float)var1 + 0.5F;
			float var10003 = (float)var2 + 0.5F;
			float var10004 = (float)var3 + 0.5F;
			StepSound* var8 = var5->stepSound;
			float var10005 = (var8->m_soundVolume + 1.0F) / 2.0F;
			var8 = var5->stepSound;
			var10000->playSound(var10001, var10002, var10003, var10004, var10005, var8->m_soundPitch * 0.8F);
			var5->onBlockDestroyedByPlayer(var4, var1, var2, var3, var6);
		}

		return var7;
}

void PlayerController::sendBlockRemoving(int var1, int var2, int var3, int var4) {
	// TODO: implement
}

void PlayerController::resetBlockRemoving() {
	// TODO: implement
}

void PlayerController::setPartialTime(float var1) {
	// TODO: implement
}

float PlayerController::getBlockReachDistance() {
		return 5.0F;
}

void PlayerController::onUpdate() {
	// TODO: implement
}

bool PlayerController::shouldDrawHUD() {
		return true;
}

void PlayerController::onRespawn(EntityPlayer* var1) {
	// TODO: implement
}
