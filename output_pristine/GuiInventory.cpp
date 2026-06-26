#include "stdafx.h"
// Auto-generated from GuiInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiInventory.h"
#include "Common.h"
#include "IInventory.h"
#include "InventoryCrafting.h"
#include "CraftingManager.h"
#include "ItemStack.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "Slot.h"
#include "SlotCrafting.h"
#include "Tesselator.h"
#include "Textures.h"

GuiInventory::GuiInventory()
	: m_inventoryCrafting(new InventoryCrafting(*this, 2, 2))
	, m_iInventory(new InventoryCraftResult())
	, m_xSize_lo(0)
	, m_ySize_lo(0)
{
}

GuiInventory::GuiInventory(IInventory* var1)
	: m_inventoryCrafting(new InventoryCrafting(*this, 2, 2))
	, m_iInventory(new InventoryCraftResult())
	, m_xSize_lo(0)
	, m_ySize_lo(0)
{
		allowUserInput = true;
		inventorySlots.push_back(new SlotCrafting(*this, m_inventoryCrafting, m_iInventory, 0, 144, 36));

		int var2;
		int var3;
		for(var2 = 0; var2 < 2; ++var2) {
			for(var3 = 0; var3 < 2; ++var3) {
				inventorySlots.push_back(new Slot(*this, m_inventoryCrafting, var3 + (var2 << 1), 88 + var3 * 18, 26 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 4; ++var2) {
			inventorySlots.push_back(new Slot(*this, var1, var1->getSizeInventory() - 1 - var2, 8, 8 + var2 * 18));
		}

		for(var2 = 0; var2 < 3; ++var2) {
			for(var3 = 0; var3 < 9; ++var3) {
				inventorySlots.push_back(new Slot(*this, var1, var3 + (var2 + 1) * 9, 8 + var3 * 18, 84 + var2 * 18));
			}
		}

		for(var2 = 0; var2 < 9; ++var2) {
			inventorySlots.push_back(new Slot(*this, var1, var2, 8 + var2 * 18, 142));
		}

}

GuiInventory::~GuiInventory() {
	m_inventoryCrafting = NULL;
	m_iInventory = NULL;
}

void GuiInventory::onGuiClosed() {
		GuiContainer::onGuiClosed();

		for(int var1 = 0; var1 < m_inventoryCrafting->getSizeInventory(); ++var1) {
			ItemStack var2 = m_inventoryCrafting->getStackInSlot(var1);
			if(var2 != NULL) {
				mc->thePlayer->dropPlayerItem(var2);
			}
		}

}

void GuiInventory::guiCraftingItemsCheck() {
		int* var1 = new int[9];

		for(int var2 = 0; var2 < 3; ++var2) {
			for(int var3 = 0; var3 < 3; ++var3) {
				int var4 = -1;
				if(var2 < 2 && var3 < 2) {
					ItemStack var5 = m_inventoryCrafting->getStackInSlot(var2 + (var3 << 1));
					if(var5 != NULL) {
						var4 = var5.itemID;
					}
				}

				var1[var2 + var3 * 3] = var4;
			}
		}

		m_iInventory->setInventorySlotContents(0, CraftingManager::getInstance().findMatchingRecipe(var1));
}

void GuiInventory::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString("Crafting", 86, 16, 4210752);
}

void GuiInventory::drawScreen(int var1, int var2, float var3) {
		GuiContainer::drawScreen(var1, var2, var3);
		m_xSize_lo = (float)var1;
		m_ySize_lo = (float)var2;
}

void GuiInventory::drawGuiContainerBackgroundLayer() {
		int var1 = mc.Textures::load("game:\Content\gui\inventory.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		RenderEngine::bindTexture(var1);
		var1 = (width - m_xSize) / 2;
		int var2 = (height - m_ySize) / 2;
		drawTexturedModalRect(var1, var2, 0, 0, m_xSize, m_ySize);
  // Normalize is shader-based on Xbox 360 (enabled)
  // Color material is shader-based on Xbox 360 (enabled)
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)(var1 + 51), (float)(var2 + 75), 50.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _s; D3DXMatrixScaling(&_s, -30.0F, 30.0F, 30.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,0.0F,1.0F), (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		float var3 = mc->thePlayer->renderYawOffset;
		float var4 = mc->thePlayer->rotationYaw;
		float var5 = mc->thePlayer->rotationPitch;
		float var6 = (float)(var1 + 51) - m_xSize_lo;
		float var7 = (float)(var2 + 75 - 50) - m_ySize_lo;
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (135.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(0.0F,1.0F,0.0F), (-135.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (-((float)Math.atan((double)(var7 / 40.0F))) * 20.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		mc->thePlayer->renderYawOffset = (float)atanf((double)(var6 / 40.0F)) * 20.0F;
		mc->thePlayer->rotationYaw = (float)atanf((double)(var6 / 40.0F)) * 40.0F;
		mc->thePlayer->rotationPitch = -((float)atanf((double)(var7 / 40.0F))) * 20.0F;
		RenderManager.instance.a(mc->thePlayer, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F);
		mc->thePlayer->renderYawOffset = var3;
		mc->thePlayer->rotationYaw = var4;
		mc->thePlayer->rotationPitch = var5;
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
		RenderHelper::disableStandardItemLighting();
  // Normalize is shader-based on Xbox 360 (disabled)
}
