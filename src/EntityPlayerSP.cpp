#include "stdafx.h"
// Auto-generated from EntityPlayerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayerSP.h"
#include "Common.h"
#include "Entity.h"
#include "IInventory.h"
#include "Minecraft.h"
#include "MovementInput.h"
#include "NBTTagCompound.h"
#include "Session.h"
#include "World.h"
#include <string>
#include "EntityPlayer.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "NBTTagList.h"
#include "Player.h"
#include "EffectRenderer.h"
#include "GuiChest.h"
#include "GuiCrafting.h"

EntityPlayerSP::EntityPlayerSP()
	: m_movementInput(NULL)
	, mc(NULL)
{
}

EntityPlayerSP::EntityPlayerSP(Minecraft* var1, World* var2, Session* var3)
	: EntityPlayer(var2)
	, m_movementInput(NULL)
	, mc(NULL)
{
		mc = var1;
		if(var3 != NULL) {
			skinUrl = "http://www.minecraft.net/skin/" + var3->username + ".png";
		}

}

EntityPlayerSP::~EntityPlayerSP() {
	m_movementInput = NULL;
	mc = NULL;
}

void EntityPlayerSP::updatePlayerActionState() {
		moveStrafing = m_movementInput->m_moveStrafe;
		moveForward = m_movementInput->moveForward;
		isJumping = m_movementInput->m_jump;
}

void EntityPlayerSP::onLivingUpdate() {
		m_movementInput->updatePlayerMoveState();
		EntityPlayer::onLivingUpdate();
}

void EntityPlayerSP::writeEntityToNBT(NBTTagCompound* var1) {
		var1->setInteger("Score", getScore);
		InventoryPlayer var10002 = inventory;
		NBTTagList* var2 = new NBTTagList();
		InventoryPlayer var5 = var10002;

		for(int var3 = 0; var3 < var5.m_mainInventory /* PORT: array length - use tracked size */; ++var3) {
			if(var5.m_mainInventory[var3] != NULL) {
				NBTTagCompound* var4 = new NBTTagCompound();
				var4->setByte("Slot", (unsigned char)var3);
				var5.m_mainInventory[var3].writeToNBT(var4);
				var2->setTag(var4);
			}
		}

		var1->setTag("Inventory", var2);
}

void EntityPlayerSP::readEntityFromNBT(NBTTagCompound* var1) {
		getScore = var1->getInteger("Score");
		NBTTagList* var6 = var1->getTagList("Inventory");
		NBTTagList var2 = var6;
		InventoryPlayer var7 = inventory;
		var7.m_mainInventory = new ItemStack[var7.getSizeInventory()];

		for(int var3 = 0; var3 < var2.tagCount(); ++var3) {
			NBTTagCompound* var4 = (NBTTagCompound*)var2.tagAt(var3);
			int var5 = var4->getByte("Slot") & 255;
			if(var5 >= 0 && var5 < var7.m_mainInventory /* PORT: array length - use tracked size */) {
				var7.m_mainInventory[var5] = ItemStack(var4);
			}
		}

}

const std::string& EntityPlayerSP::getEntityString() {
		return "LocalPlayer";
}

void EntityPlayerSP::displayGUIChest(IInventory* var1) {
		mc->displayGuiScreen(new GuiChest(inventory, var1));
}

void EntityPlayerSP::displayWorkbenchGUI() {
		mc->displayGuiScreen(new GuiCrafting(inventory));
}

void EntityPlayerSP::displayGUIInventory() {
		inventory.setInventorySlotContents(inventory.m_currentItem, (ItemStack)NULL);
}

void EntityPlayerSP::onItemPickup(Entity* var1) {
		mc->m_effectRenderer->addEffect(new EntityPickupFX(mc->theWorld, var1, this, -0.5F));
}
