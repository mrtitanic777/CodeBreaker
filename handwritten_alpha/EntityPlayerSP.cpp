#include "stdafx.h"
// Auto-generated from EntityPlayerSP.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "EntityPlayerSP.h"
#include "Common.h"
#include "SoundManager.h"
#include "Entity.h"
#include "IInventory.h"
#include "Minecraft.h"
#include "MovementInput.h"
#include "NBTTagCompound.h"
#include "Session.h"
#include "TileEntityFurnace.h"
#include "TileEntitySign.h"
#include "World.h"
#include <string>
#include "EntityLiving.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "Player.h"
#include "EffectRenderer.h"
#include "EntityPickupFX.h"
#include "GuiChest.h"
#include "GuiCrafting.h"
#include "GuiEditSign.h"
#include "GuiFurnace.h"
#include "InventoryPlayer.h"

EntityPlayerSP::EntityPlayerSP()
	: m_field_787_a(NULL)
	, m_field_9373_b(20)
	, m_field_4134_c(0)
	, m_field_4133_d(0)
	, mc(NULL)
	, m_field_9374_bx(false)
{
}

EntityPlayerSP::EntityPlayerSP(Minecraft* var1, World* var2, Session* var3, int var4)
	: EntityPlayer(var2)
	, m_field_787_a(NULL)
	, m_field_9373_b(20)
	, m_field_4134_c(0)
	, m_field_4133_d(0)
	, mc(NULL)
	, m_field_9374_bx(false)
{
		mc = var1;
		m_dimension = var4;
		// PORT: Session::username is a std::string value (converter wrongly used m_inventory).
		if(var3 != NULL && !var3->username.empty() && var3->username.length() > 0) {
			skinUrl = "http://www.minecraft.net/skin/" + var3->username + ".png";
			DbgPrintF("%s\n", ("Loading texture " + skinUrl).c_str());
		}

		if(var3 != NULL) {
			m_field_771_i = var3->username;
		}
}

EntityPlayerSP::~EntityPlayerSP() {
	m_field_787_a = NULL;
	mc = NULL;
}

void EntityPlayerSP::func_418_b_() {
		EntityPlayer::func_418_b_();
		m_field_9342_ah = m_field_787_a->m_field_1174_a;
		m_field_9340_ai = m_field_787_a->m_field_1173_b;
		isJumping = m_field_787_a->m_field_1176_d;
}

void EntityPlayerSP::onLivingUpdate() {
		m_field_4133_d = m_field_4134_c;
		if(m_field_9374_bx) {
			if(m_field_4134_c == 0.0F) {
				mc->m_sndManager->func_337_a("portal.trigger", 1.0F, rand->nextFloat() * 0.4F + 0.8F);
			}

			m_field_4134_c += 0.0125F;
			if(m_field_4134_c >= 1.0F) {
				m_field_4134_c = 1.0F;
				m_field_9373_b = 10;
				mc->m_sndManager->func_337_a("portal.travel", 1.0F, rand->nextFloat() * 0.4F + 0.8F);
				mc->func_6237_k();
			}

			m_field_9374_bx = false;
		} else {
			if(m_field_4134_c > 0.0F) {
				m_field_4134_c -= 0.05F;
			}

			if(m_field_4134_c < 0.0F) {
				m_field_4134_c = 0.0F;
			}
		}

		if(m_field_9373_b > 0) {
			--m_field_9373_b;
		}

		m_field_787_a->func_797_a(this);
		if(m_field_787_a->m_field_1175_e && m_field_9287_aY < 0.2F) {
			m_field_9287_aY = 0.2F;
		}

		EntityPlayer::onLivingUpdate();
}

void EntityPlayerSP::func_458_k() {
		m_field_787_a->func_798_a();
}

void EntityPlayerSP::func_460_a(int var1, bool var2) {
		m_field_787_a->func_796_a(var1, var2);
}

void EntityPlayerSP::writeEntityToNBT(NBTTagCompound* var1) {
		EntityPlayer::writeEntityToNBT(var1);
		var1->setInteger("Score", m_score);
}

void EntityPlayerSP::readEntityFromNBT(NBTTagCompound* var1) {
		EntityPlayer::readEntityFromNBT(var1);
		m_score = var1->getInteger("Score");
}

void EntityPlayerSP::displayGUIChest(IInventory* var1) {
		mc->displayGuiScreen(new GuiChest(m_inventory, var1));
}

void EntityPlayerSP::displayGUIEditSign(TileEntitySign* var1) {
		mc->displayGuiScreen(new GuiEditSign(var1));
}

void EntityPlayerSP::displayWorkbenchGUI() {
		mc->displayGuiScreen(new GuiCrafting(m_inventory));
}

void EntityPlayerSP::displayGUIFurnace(TileEntityFurnace* var1) {
		mc->displayGuiScreen(new GuiFurnace(m_inventory, var1));
}

void EntityPlayerSP::func_443_a_(Entity* var1, int var2) {
		mc->m_field_6321_h->func_1192_a(new EntityPickupFX(mc->theWorld, var1, this, -0.5F));
}

int EntityPlayerSP::getPlayerArmorValue() {
		return m_inventory->getTotalArmorValue();
}

void EntityPlayerSP::func_6415_a_(Entity* var1) {
		if(!var1->interact(this)) {
			ItemStack* var2 = getCurrentEquippedItem();
			if(var2 != NULL && dynamic_cast<EntityLiving*>(var1) != NULL) {
				var2->useItemOnEntity((EntityLiving*)var1);
				if(var2->stackSize <= 0) {
					var2->func_1097_a(this);
					destroyCurrentEquippedItem();
				}
			}

		}
}

void EntityPlayerSP::sendChatMessage(const std::string& var1) {
	// TODO: implement
}

void EntityPlayerSP::func_6420_o() {
	// TODO: implement
}

bool EntityPlayerSP::func_381_o() {
		return m_field_787_a->m_field_1175_e;
}

void EntityPlayerSP::func_4039_q() {
		if(m_field_9373_b > 0) {
			m_field_9373_b = 10;
		} else {
			m_field_9374_bx = true;
		}
}

void EntityPlayerSP::setHealth(int var1) {
		int var2 = health - var1;
		if(var2 <= 0) {
			health = var1;
		} else {
			m_field_9346_af = var2;
			m_field_9335_K = health;
			m_field_9306_bj = m_field_9366_o;
			damageEntity(var2);
			hurtTime = m_field_9332_M = 10;
		}

}

void EntityPlayerSP::func_9367_r() {
		mc->respawn();
}
