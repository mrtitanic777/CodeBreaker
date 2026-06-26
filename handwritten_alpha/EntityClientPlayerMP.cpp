#include "stdafx.h"
// Auto-generated from EntityClientPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. All packet-sending bodies here are stubbed
// (no-op) -- multiplayer networking is redesigned later against the 360 API.
// NetClientHandler is a compile-only stub; the Packet* hierarchy is not built.

#include "EntityClientPlayerMP.h"
#include "Common.h"
#include "Entity.h"
#include "EntityItem.h"
#include "InventoryPlayer.h"
#include "Minecraft.h"
#include "NetClientHandler.h"
#include "Session.h"
#include "World.h"
#include <string>
#include "EntityPlayer.h"
#include "EntityPlayerSP.h"

EntityClientPlayerMP::EntityClientPlayerMP()
	: m_field_797_bg(NULL)
	, m_field_9380_bx(0)
	, m_field_9379_by(0)
	, m_field_9378_bz(0)
	, m_field_9377_bA(0)
	, m_field_9376_bB(0)
	, m_field_9385_bC(0)
	, m_field_9384_bD(0)
	, m_field_9383_bE(new InventoryPlayer((EntityPlayer*)this))
	, m_field_9382_bF(false)
	, m_field_9381_bG(false)
	, m_field_12242_bI(0)
{
}

EntityClientPlayerMP::EntityClientPlayerMP(Minecraft* var1, World* var2, Session* var3, NetClientHandler* var4)
	: EntityPlayerSP(var1, var2, var3, 0)
	, m_field_797_bg(NULL)
	, m_field_9380_bx(0)
	, m_field_9379_by(0)
	, m_field_9378_bz(0)
	, m_field_9377_bA(0)
	, m_field_9376_bB(0)
	, m_field_9385_bC(0)
	, m_field_9384_bD(0)
	, m_field_9383_bE(new InventoryPlayer((EntityPlayer*)this))
	, m_field_9382_bF(false)
	, m_field_9381_bG(false)
	, m_field_12242_bI(0)
{
		m_field_797_bg = var4;
}

EntityClientPlayerMP::~EntityClientPlayerMP() {
	m_field_797_bg = NULL;
	m_field_9383_bE = NULL;
}

bool EntityClientPlayerMP::canAttackEntity(Entity* var1, int var2) {
		return false;
}

void EntityClientPlayerMP::heal(int var1) {
	// TODO: implement
}

void EntityClientPlayerMP::onUpdate() {
		if(worldObj->blockExists((int)floor(posX), 64, (int)floor(posZ))) {
			EntityPlayerSP::onUpdate();
			func_4056_N();
		}
}

void EntityClientPlayerMP::func_6420_o() {
	// TODO: implement
}

void EntityClientPlayerMP::func_4056_N() {
		if(m_field_9380_bx++ == 20) {
			sendInventoryChanged();
			m_field_9380_bx = 0;
		}

		bool var1 = func_381_o();
		if(var1 != m_field_9381_bG) {
			// PORT: 360 MP API TODO -- arm-animation packet (no-op).
			m_field_9381_bG = var1;
		}

		double var2 = posX - m_field_9379_by;
		double var4 = boundingBox.minY - m_field_9378_bz;
		double var6 = posY - m_field_9377_bA;
		double var8 = posZ - m_field_9376_bB;
		double var10 = (double)(rotationYaw - m_field_9385_bC);
		double var12 = (double)(rotationPitch - m_field_9384_bD);
		bool var14 = var4 != 0.0f || var6 != 0.0f || var2 != 0.0f || var8 != 0.0f;
		bool var15 = var10 != 0.0f || var12 != 0.0f;
		// PORT: 360 MP API TODO -- player position/look move packets (no-op).
		if(m_ridingEntity != NULL) {
			var14 = false;
		} else if(var14 && var15) {
			m_field_12242_bI = 0;
		} else if(var14) {
			m_field_12242_bI = 0;
		} else if(var15) {
			m_field_12242_bI = 0;
		} else {
			if(m_field_9382_bF == onGround && m_field_12242_bI <= 20) {
				++m_field_12242_bI;
			} else {
				m_field_12242_bI = 0;
			}
		}

		m_field_9382_bF = onGround;
		if(var14) {
			m_field_9379_by = posX;
			m_field_9378_bz = boundingBox.minY;
			m_field_9377_bA = posY;
			m_field_9376_bB = posZ;
		}

		if(var15) {
			m_field_9385_bC = rotationYaw;
			m_field_9384_bD = rotationPitch;
		}

}

void EntityClientPlayerMP::sendInventoryChanged() {
		if(!m_inventory->compareInventory(m_field_9383_bE)) {
			// PORT: 360 MP API TODO -- player inventory packets (no-op).
			m_field_9383_bE = m_inventory->copyInventory();
		}

}

void EntityClientPlayerMP::joinEntityItemWithWorld(EntityItem* var1) {
		// PORT: 360 MP API TODO -- pickup-spawn packet (no-op). In SP the item is
		// just added to the world by the caller; nothing to send.
		(void)var1;
}

void EntityClientPlayerMP::sendChatMessage(const std::string& var1) {
		// PORT: 360 MP API TODO -- chat packet (no-op).
		(void)var1;
}

void EntityClientPlayerMP::func_457_w() {
		EntityPlayerSP::func_457_w();
		// PORT: 360 MP API TODO -- arm-animation packet (no-op).
}

void EntityClientPlayerMP::func_9367_r() {
		sendInventoryChanged();
		// PORT: 360 MP API TODO -- respawn packet (no-op).
}

void EntityClientPlayerMP::damageEntity(int var1) {
		health -= var1;
}
