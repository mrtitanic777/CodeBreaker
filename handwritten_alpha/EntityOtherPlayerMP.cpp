#include "stdafx.h"
// Auto-generated from EntityOtherPlayerMP.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: SINGLEPLAYER 360 port. This is the remote-player avatar; in SP it is
// never spawned. Kept compiling for completeness; no networking here.

#include "EntityOtherPlayerMP.h"
#include "Common.h"
#include "Entity.h"
#include "World.h"
#include <string>
#include "EntityPlayer.h"
#include "Player.h"

EntityOtherPlayerMP::EntityOtherPlayerMP()
	: m_field_785_bg(0)
	, m_field_784_bh(0)
	, m_field_783_bi(0)
	, m_field_782_bj(0)
	, m_field_780_bk(0)
	, m_field_786_bl(0)
	, m_a(0.0f)
{
}

EntityOtherPlayerMP::EntityOtherPlayerMP(World* var1, const std::string& var2)
	: EntityPlayer(var1)
	, m_field_785_bg(0)
	, m_field_784_bh(0)
	, m_field_783_bi(0)
	, m_field_782_bj(0)
	, m_field_780_bk(0)
	, m_field_786_bl(0)
	, m_a(0.0f)
{
		m_field_771_i = var2;
		yOffset = 0.0F;
		m_field_9286_aZ = 0.0F;
		// PORT: Java was 'var2 != null && var2.length() > 0'; a std::string ref
		// can't be null, so just check for non-empty.
		if(var2.length() > 0) {
			skinUrl = "http://www.minecraft.net/skin/" + toStr(var2) + ".png";
			DbgPrintF("%s\n", ("Loading texture " + skinUrl).c_str());
		}

		m_field_9314_ba = true;
		m_field_619_ac = 10.0f;
}

EntityOtherPlayerMP::~EntityOtherPlayerMP() {
	// No dynamic resources
}

bool EntityOtherPlayerMP::canAttackEntity(Entity* var1, int var2) {
		return true;
}

void EntityOtherPlayerMP::setPositionAndRotation2(double var1, double var3, double var5, float var7, float var8, int var9) {
		yOffset = 0.0F;
		m_field_784_bh = var1;
		m_field_783_bi = var3;
		m_field_782_bj = var5;
		m_field_780_bk = (double)var7;
		m_field_786_bl = (double)var8;
		m_field_785_bg = var9;
}

void EntityOtherPlayerMP::onUpdate() {
		EntityPlayer::onUpdate();
		m_field_705_Q = m_field_704_R;
		double var1 = posX - prevPosX;
		double var3 = posZ - prevPosZ;
		float var5 = sqrt(var1 * var1 + var3 * var3) * 4.0F;
		if(var5 > 1.0F) {
			var5 = 1.0F;
		}

		m_field_704_R += (var5 - m_field_704_R) * 0.4F;
		m_field_703_S += m_field_704_R;
}

float EntityOtherPlayerMP::func_392_h_() {
		return 0.0F;
}

void EntityOtherPlayerMP::onLivingUpdate() {
		EntityPlayer::func_418_b_();
		if(m_field_785_bg > 0) {
			double var1 = posX + (m_field_784_bh - posX) / (double)m_field_785_bg;
			double var3 = posY + (m_field_783_bi - posY) / (double)m_field_785_bg;
			double var5 = posZ + (m_field_782_bj - posZ) / (double)m_field_785_bg;

			double var7;
			for(var7 = m_field_780_bk - (double)rotationYaw; var7 < -180.0f; var7 += 360.0f) {
			}

			while(var7 >= 180.0f) {
				var7 -= 360.0f;
			}

			rotationYaw = (float)((double)rotationYaw + var7 / (double)m_field_785_bg);
			rotationPitch = (float)((double)rotationPitch + (m_field_786_bl - (double)rotationPitch) / (double)m_field_785_bg);
			--m_field_785_bg;
			setPosition(var1, var3, var5);
			setRotation(rotationYaw, rotationPitch);
		}

		m_field_775_e = m_field_774_f;
		float var9 = sqrt(motionX * motionX + motionZ * motionZ);
		float var2 = (float)atanf(-motionY * (double)0.2F) * 15.0F;
		if(var9 > 0.1F) {
			var9 = 0.1F;
		}

		if(!onGround || health <= 0) {
			var9 = 0.0F;
		}

		if(onGround || health <= 0) {
			var2 = 0.0F;
		}

		m_field_774_f += (var9 - m_field_774_f) * 0.4F;
		m_field_9328_R += (var2 - m_field_9328_R) * 0.8F;
}

bool EntityOtherPlayerMP::func_381_o() {
		return m_field_12240_bw;
}
