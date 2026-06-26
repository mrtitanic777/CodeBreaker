#include "stdafx.h"
// Auto-generated from GuiContainer.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"
#include "Common.h"
#include "ItemStack.h"
#include "RenderItem.h"
#include "EntityPlayerSP.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "Slot.h"
#include "Tesselator.h"

RenderItem* GuiContainer::s_itemRenderer = new RenderItem();

GuiContainer::GuiContainer()
	: m_itemStack(NULL)
	, m_xSize(176)
	, m_ySize(166)
{
}

GuiContainer::~GuiContainer() {
	m_itemStack = NULL;
}

void GuiContainer::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		int var12 = (width - m_xSize) / 2;
		int var4 = (height - m_ySize) / 2;
		drawGuiContainerBackgroundLayer();
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var12, (float)var4, 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  // Normalize is shader-based on Xbox 360 (enabled)

		for(size_t var5 = 0; var5 < inventorySlots.size(); ++var5) {
			Slot var6 = (Slot)inventorySlots[var5];
			int var11 = var6.m_yPos;
			int var10 = var6.m_xPos;
			int var9 = var6.m_slotIndex;
			IInventory var8 = var6.m_inventory;
			ItemStack var14 = var8.getStackInSlot(var9);
			s_itemRenderer->renderItemIntoGUI(mc->m_renderEngine, var14, var10, var11);
			s_itemRenderer->renderItemOverlayIntoGUI(fontRenderer, var14, var10, var11);
			if(var6.isAtCursorPos(var1, var2)) {
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				int var7 = var6.m_xPos;
				int var13 = var6.m_yPos;
				drawGradientRect(var7, var13, var7 + 16, var13 + 16, -2130706433, -2130706433);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			}
		}

		if(m_itemStack != NULL) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 32.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			s_itemRenderer->renderItemIntoGUI(mc->m_renderEngine, m_itemStack, var1 - var12 - 8, var2 - var4 - 8);
			s_itemRenderer->renderItemOverlayIntoGUI(fontRenderer, m_itemStack, var1 - var12 - 8, var2 - var4 - 8);
		}

  // Normalize is shader-based on Xbox 360 (disabled)
		RenderHelper::disableStandardItemLighting();
  // Lighting is shader-based on Xbox 360 (disabled)
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		drawGuiContainerForegroundLayer();
  // Lighting is shader-based on Xbox 360 (enabled)
  g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
}

void GuiContainer::drawGuiContainerForegroundLayer() {
	// TODO: implement
}

void GuiContainer::drawSlotInventory(int var1, int var2, int var3) {
		if(var3 == 0 || var3 == 1) {
			int var6 = var2;
			int var4 = var1;
			GuiContainer var5 = this;
			int var7 = 0;

			Slot var10000;
			while(true) {
				if(var7 >= var5.inventorySlots.size()) {
					var10000 = NULL;
					break;
				}

				Slot var8 = (Slot)var5.inventorySlots[var7];
				if(var8.isAtCursorPos(var4, var6)) {
					var10000 = var8;
					break;
				}

				++var7;
			}

			Slot var11 = var10000;
			if(var11 != NULL) {
				ItemStack var12 = var11.m_inventory.getStackInSlot(var11.m_slotIndex);
				if(var12 == NULL && m_itemStack == NULL) {
					return;
				}

				if(var12 != NULL && m_itemStack == NULL) {
					var6 = var3 == 0 ? var12.stackSize : (var12.stackSize + 1) / 2;
					m_itemStack = var11.m_inventory.decrStackSize(var11.m_slotIndex, var6);
					if(var12.stackSize == 0) {
						var11.putStack((ItemStack)NULL);
					}

					var11.onPickupFromSlot();
				} else if(var12 == NULL && m_itemStack != NULL && var11.isItemValid()) {
					var6 = var3 == 0 ? m_itemStack->stackSize : 1;
					if(var6 > var11.m_inventory.getInventoryStackLimit()) {
						var6 = var11.m_inventory.getInventoryStackLimit();
					}

					var11.putStack(m_itemStack->splitStack(var6));
					if(m_itemStack->stackSize == 0) {
						m_itemStack = NULL;
					}
				} else {
					if(var12 == NULL || m_itemStack == NULL) {
						return;
					}

					ItemStack var9;
					if(!var11.isItemValid()) {
						if(var12.itemID == m_itemStack->itemID) {
							var9 = m_itemStack;
							if(var9.getItem().getItemStackLimit() > 1) {
								var6 = var12.stackSize;
								if(var6 > 0) {
									int var14 = var6 + m_itemStack->stackSize;
									var9 = m_itemStack;
									if(var14 <= var9.getItem().getItemStackLimit()) {
										m_itemStack->stackSize += var6;
										var12.splitStack(var6);
										if(var12.stackSize == 0) {
											var11.putStack((ItemStack)NULL);
										}

										var11.onPickupFromSlot();
										return;
									}
								}

								return;
							}
						}

						return;
					}

					if(var12.itemID != m_itemStack->itemID) {
						if(m_itemStack->stackSize > var11.m_inventory.getInventoryStackLimit()) {
							return;
						}

						var11.putStack(m_itemStack);
						m_itemStack = var12;
					} else {
						if(var12.itemID != m_itemStack->itemID) {
							return;
						}

						if(var3 == 0) {
							var6 = m_itemStack->stackSize;
							if(var6 > var11.m_inventory.getInventoryStackLimit() - var12.stackSize) {
								var6 = var11.m_inventory.getInventoryStackLimit() - var12.stackSize;
							}

							var9 = m_itemStack;
							if(var6 > var9.getItem().getItemStackLimit() - var12.stackSize) {
								var9 = m_itemStack;
								var6 = var9.getItem().getItemStackLimit() - var12.stackSize;
							}

							m_itemStack->splitStack(var6);
							if(m_itemStack->stackSize == 0) {
								m_itemStack = NULL;
							}

							var12.stackSize += var6;
						} else {
							if(var3 != 1) {
								return;
							}

							var6 = 1;
							if(1 > var11.m_inventory.getInventoryStackLimit() - var12.stackSize) {
								var6 = var11.m_inventory.getInventoryStackLimit() - var12.stackSize;
							}

							var9 = m_itemStack;
							if(var6 > var9.getItem().getItemStackLimit() - var12.stackSize) {
								var9 = m_itemStack;
								var6 = var9.getItem().getItemStackLimit() - var12.stackSize;
							}

							m_itemStack->splitStack(var6);
							if(m_itemStack->stackSize == 0) {
								m_itemStack = NULL;
							}

							var12.stackSize += var6;
						}
					}
				}
			} else if(m_itemStack != NULL) {
				int var13 = (width - m_xSize) / 2;
				var6 = (height - m_ySize) / 2;
				if(var1 < var13 || var2 < var6 || var1 >= var13 + m_xSize || var2 >= var6 + m_xSize) {
					EntityPlayerSP var10 = mc->thePlayer;
					if(var3 == 0) {
						var10.dropPlayerItem(m_itemStack);
						m_itemStack = NULL;
					}

					if(var3 == 1) {
						var10.dropPlayerItem(m_itemStack->splitStack(1));
						if(m_itemStack->stackSize == 0) {
							m_itemStack = NULL;
						}
					}
				}
			}
		}

}

void GuiContainer::keyTyped(char var1, int var2) {
		if(var2 == 1 || var2 == mc->m_options.m_keyBindInventory.m_keyCode) {
			mc->displayGuiScreen((GuiScreen)NULL);
		}

}

void GuiContainer::onGuiClosed() {
		if(m_itemStack != NULL) {
			mc->thePlayer->dropPlayerItem(m_itemStack);
		}

}

void GuiContainer::guiCraftingItemsCheck() {
	// TODO: implement
}

bool GuiContainer::doesGuiPauseGame() {
		return false;
}
