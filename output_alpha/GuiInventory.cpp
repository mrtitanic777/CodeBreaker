#include "stdafx.h"
// Hand-ported from GuiInventory.java (Alpha v1.2.6)
// Target: Xbox 360 / C++03 / DirectX 9
// Guided by MinecraftIndev\GuiInventory.cpp (D3D9 player-preview adaptation).

#include "GuiInventory.h"
#include "Common.h"
#include "CraftingInventoryPlayerCB.h"
#include "IInventory.h"
#include "InventoryCrafting.h"
#include "ItemStack.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "SlotCrafting.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "GuiContainer.h"
#include "InventoryCraftResult.h"
#include "Minecraft.h"
#include "Entity.h"
#include "EntityPlayerSP.h"
#include "RenderManager.h"
#include "SlotArmor.h"
#include "SlotInventory.h"

GuiInventory::GuiInventory()
	: m_field_977_j(NULL)
	, m_xSize_lo(0)
	, m_ySize_lo(0)
{
}

GuiInventory::GuiInventory(IInventory* var1, ItemStack** var2)
	: m_field_977_j(NULL)
	, m_xSize_lo(0)
	, m_ySize_lo(0)
{
		m_field_948_f = true;
		m_field_977_j = new CraftingInventoryPlayerCB(var2);
		// m_craftMatrix is an InventoryCrafting*; the Slot ctors take IInventory* (its base).
		inventorySlots.push_back(new SlotCrafting(this, (IInventory*)m_field_977_j->m_craftMatrix, m_field_977_j->m_craftResult, 0, 144, 36));

		int var3;
		int var4;
		for(var3 = 0; var3 < 2; ++var3) {
			for(var4 = 0; var4 < 2; ++var4) {
				inventorySlots.push_back(new SlotInventory(this, (IInventory*)m_field_977_j->m_craftMatrix, var4 + var3 * 2, 88 + var4 * 18, 26 + var3 * 18));
			}
		}

		for(var3 = 0; var3 < 4; ++var3) {
			inventorySlots.push_back(new SlotArmor(this, this, var1, var1->getSizeInventory() - 1 - var3, 8, 8 + var3 * 18, var3));
		}

		for(var3 = 0; var3 < 3; ++var3) {
			for(var4 = 0; var4 < 9; ++var4) {
				inventorySlots.push_back(new SlotInventory(this, var1, var4 + (var3 + 1) * 9, 8 + var4 * 18, 84 + var3 * 18));
			}
		}

		for(var3 = 0; var3 < 9; ++var3) {
			inventorySlots.push_back(new SlotInventory(this, var1, var3, 8 + var3 * 18, 142));
		}

}

GuiInventory::~GuiInventory() {
	m_field_977_j = NULL;
}

void GuiInventory::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString("Crafting", 86, 16, 4210752);
}

void GuiInventory::drawScreen(int var1, int var2, float var3) {
		GuiContainer::drawScreen(var1, var2, var3);
		m_xSize_lo = (float)var1;
		m_ySize_lo = (float)var2;
}

void GuiInventory::drawGuiContainerBackgroundLayer(float var1) {
		int var2 = Textures::load("game:\\Content\\gui\\inventory.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetTexture(0, Textures::getById(var2));
		int var3 = (width - m_xSize) / 2;
		int var4 = (height - m_ySize) / 2;
		drawTexturedModalRect(var3, var4, 0, 0, m_xSize, m_ySize);
  // Normalize / color-material are shader-based on Xbox 360 (no GL enable needed).
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  // PORT: the inventory 3D player preview needs DEPTH so its parts self-occlude (GUI base is Z off).
  // Don't issue a mid-pass full-surface Clear here (it drops the panel quad on Nexia360); the
  // screen path already cleared depth before drawScreen and the flat 2D GUI does not write depth,
  // so the buffer is clean. Just enable Z for Steve, then restore Z-off for the rest of the GUI.
  // All transforms below are PRE-multiply (transposed-WVP), matching the indev port.
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var3 + 51), (float)(var4 + 75), 50.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var5 = 30.0F;
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -var5, var5, var5);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXVECTOR3 _ax(0.0F,0.0F,1.0F); D3DXMatrixRotationAxis(&_r, &_ax, (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  // PORT: compensating Ry(180) (paired with NEGATED yaw below) so Steve faces forward in the
  // LH pipeline + the -X scale (matches the indev port). Textures stay X-mirrored -- known issue.
  { D3DXMATRIX _r; D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMatrixRotationAxis(&_r, &_ax, (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var6 = mc->thePlayer->m_field_735_n;
		float var7 = mc->thePlayer->rotationYaw;
		float var8 = mc->thePlayer->rotationPitch;
		float var9 = (float)(var3 + 51) - m_xSize_lo;
		float var10 = (float)(var4 + 75 - 50) - m_ySize_lo;
  { D3DXMATRIX _r; D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMatrixRotationAxis(&_r, &_ax, (135.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  { D3DXMATRIX _r; D3DXVECTOR3 _ax(0.0F,1.0F,0.0F); D3DXMatrixRotationAxis(&_r, &_ax, (-135.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXVECTOR3 _ax(1.0F,0.0F,0.0F); D3DXMatrixRotationAxis(&_r, &_ax, (-((float)atanf((float)(var10 / 40.0F))) * 20.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		// PORT: renderYawOffset(m_field_735_n)/yaw NEGATED to pair with the compensating Ry(180) above.
		mc->thePlayer->m_field_735_n = -((float)atanf((float)(var9 / 40.0F)) * 20.0F);
		mc->thePlayer->rotationYaw = -((float)atanf((float)(var9 / 40.0F)) * 40.0F);
		mc->thePlayer->rotationPitch = -((float)atanf((float)(var10 / 40.0F))) * 20.0F;
		RenderManager::instance->func_853_a((Entity*)mc->thePlayer, 0.0, 0.0, 0.0, 0.0F, 1.0F);
		mc->thePlayer->m_field_735_n = var6;
		mc->thePlayer->rotationYaw = var7;
		mc->thePlayer->rotationPitch = var8;
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  // restore flat 2D state for the rest of the inventory GUI (slots, items, text)
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		RenderHelper::disableStandardItemLighting();
}
