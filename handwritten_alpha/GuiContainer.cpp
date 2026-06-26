#include "stdafx.h"
// Auto-generated from GuiContainer.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiContainer.h"
#include "Common.h"
#include "RenderItem.h"
#include "Slot.h"
#include "SlotInventory.h"
#include "EntityPlayerSP.h"
#include "GuiScreen.h"
#include "InventoryPlayer.h"
#include "ItemStack.h"
#include "RenderHelper.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"
#include "GameSettings.h"
#include "IInventory.h"
#include "Minecraft.h"

RenderItem* GuiContainer::s_itemRenderer = new RenderItem();

GuiContainer::GuiContainer()
	: m_xSize(176)
	, m_ySize(166)
{
}

GuiContainer::~GuiContainer() {
	// No dynamic resources
}

void GuiContainer::drawScreen(int var1, int var2, float var3) {
		drawDefaultBackground();
		int var4 = (width - m_xSize) / 2;
		int var5 = (height - m_ySize) / 2;
		drawGuiContainerBackgroundLayer(var3);
  D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(1.0F,0.0F,0.0F), (180.0F)*DEG_TO_RAD);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
		RenderHelper::enableStandardItemLighting();
  Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();
  _savedMatrix = Shaders::getWorldMatrix();
  { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, (float)var4, (float)var5, 0.0F);
    D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
    Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);

		for(size_t var6 = 0; var6 < inventorySlots.size(); ++var6) {
			SlotInventory* var7 = (SlotInventory*)inventorySlots[var6];
			drawSlotInventory(var7);
			if(var7->isAtCursorPos(var1, var2)) {
    // Lighting is shader-based on Xbox 360 (disabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				int var8 = var7->m_xPos;
				int var9 = var7->m_yPos;
				drawGradientRect(var8, var9, var8 + 16, var9 + 16, -2130706433, -2130706433);
    // Lighting is shader-based on Xbox 360 (enabled)
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			}
		}

		InventoryPlayer* var10 = mc->thePlayer->m_inventory;
		if(var10->m_draggingItemStack != NULL) {
   { D3DXMATRIX _t; D3DXMatrixTranslation(&_t, 0.0F, 0.0F, 32.0F);
     D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);
     Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }
			s_itemRenderer->renderItemIntoGUI(fontRenderer, mc->m_renderEngine, var10->m_draggingItemStack, var1 - var4 - 8, var2 - var5 - 8);
			s_itemRenderer->renderItemOverlayIntoGUI(fontRenderer, mc->m_renderEngine, var10->m_draggingItemStack, var1 - var4 - 8, var2 - var5 - 8);
		}

  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
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

void GuiContainer::drawSlotInventory(SlotInventory* var1) {
		IInventory* var2 = var1->m_inventory;
		int var3 = var1->m_slotIndex;
		int var4 = var1->m_xPos;
		int var5 = var1->m_yPos;
		ItemStack* var6 = var2->getStackInSlot(var3);
		if(var6 == NULL) {
			int var7 = var1->func_775_c();
			if(var7 >= 0) {
    // Lighting is shader-based on Xbox 360 (disabled)
				g_pd3dDevice->SetTexture(0, Textures::getById(Textures::load("game:\\Content\\gui\\items.png")));
				drawTexturedModalRect(var4, var5, var7 % 16 * 16, var7 / 16 * 16, 16, 16);
    // Lighting is shader-based on Xbox 360 (enabled)
				return;
			}
		}

		s_itemRenderer->renderItemIntoGUI(fontRenderer, mc->m_renderEngine, var6, var4, var5);
		s_itemRenderer->renderItemOverlayIntoGUI(fontRenderer, mc->m_renderEngine, var6, var4, var5);
}

Slot* GuiContainer::getSlotAtPosition(int var1, int var2) {
		for(size_t var3 = 0; var3 < inventorySlots.size(); ++var3) {
			SlotInventory* var4 = (SlotInventory*)inventorySlots[var3];
			if(var4->isAtCursorPos(var1, var2)) {
				return var4;
			}
		}

		return NULL;
}

void GuiContainer::mouseClicked(int var1, int var2, int var3) {
		if(var3 == 0 || var3 == 1) {
			Slot* var4 = getSlotAtPosition(var1, var2);
			InventoryPlayer* var5 = mc->thePlayer->m_inventory;
			int var7;
			if(var4 != NULL) {
				ItemStack* var6 = var4->getStack();
				if(var6 != NULL || var5->m_draggingItemStack != NULL) {
					if(var6 != NULL && var5->m_draggingItemStack == NULL) {
						var7 = var3 == 0 ? var6->stackSize : (var6->stackSize + 1) / 2;
						var5->m_draggingItemStack = var4->m_inventory->decrStackSize(var4->m_slotIndex, var7);
						if(var6->stackSize == 0) {
							var4->putStack(NULL);
						}

						var4->onPickupFromSlot();
					} else if(var6 == NULL && var5->m_draggingItemStack != NULL && var4->isItemValid(var5->m_draggingItemStack)) {
						var7 = var3 == 0 ? var5->m_draggingItemStack->stackSize : 1;
						if(var7 > var4->getSlotStackLimit()) {
							var7 = var4->getSlotStackLimit();
						}

						var4->putStack(var5->m_draggingItemStack->splitStack(var7));
						if(var5->m_draggingItemStack->stackSize == 0) {
							var5->m_draggingItemStack = NULL;
						}
					} else if(var6 != NULL && var5->m_draggingItemStack != NULL) {
						if(var4->isItemValid(var5->m_draggingItemStack)) {
							if(var6->itemID != var5->m_draggingItemStack->itemID) {
								if(var5->m_draggingItemStack->stackSize <= var4->getSlotStackLimit()) {
									var4->putStack(var5->m_draggingItemStack);
									var5->m_draggingItemStack = var6;
								}
							} else if(var6->itemID == var5->m_draggingItemStack->itemID) {
								if(var3 == 0) {
									var7 = var5->m_draggingItemStack->stackSize;
									if(var7 > var4->getSlotStackLimit() - var6->stackSize) {
										var7 = var4->getSlotStackLimit() - var6->stackSize;
									}

									if(var7 > var5->m_draggingItemStack->getMaxStackSize() - var6->stackSize) {
										var7 = var5->m_draggingItemStack->getMaxStackSize() - var6->stackSize;
									}

									var5->m_draggingItemStack->splitStack(var7);
									if(var5->m_draggingItemStack->stackSize == 0) {
										var5->m_draggingItemStack = NULL;
									}

									var6->stackSize += var7;
								} else if(var3 == 1) {
									var7 = 1;
									if(var7 > var4->getSlotStackLimit() - var6->stackSize) {
										var7 = var4->getSlotStackLimit() - var6->stackSize;
									}

									if(var7 > var5->m_draggingItemStack->getMaxStackSize() - var6->stackSize) {
										var7 = var5->m_draggingItemStack->getMaxStackSize() - var6->stackSize;
									}

									var5->m_draggingItemStack->splitStack(var7);
									if(var5->m_draggingItemStack->stackSize == 0) {
										var5->m_draggingItemStack = NULL;
									}

									var6->stackSize += var7;
								}
							}
						} else if(var6->itemID == var5->m_draggingItemStack->itemID && var5->m_draggingItemStack->getMaxStackSize() > 1) {
							var7 = var6->stackSize;
							if(var7 > 0 && var7 + var5->m_draggingItemStack->stackSize <= var5->m_draggingItemStack->getMaxStackSize()) {
								var5->m_draggingItemStack->stackSize += var7;
								var6->splitStack(var7);
								if(var6->stackSize == 0) {
									var4->putStack(NULL);
								}

								var4->onPickupFromSlot();
							}
						}
					}
				}

				var4->onSlotChanged();
			} else if(var5->m_draggingItemStack != NULL) {
				int var9 = (width - m_xSize) / 2;
				var7 = (height - m_ySize) / 2;
				if(var1 < var9 || var2 < var7 || var1 >= var9 + m_xSize || var2 >= var7 + m_xSize) {
					EntityPlayerSP* var8 = mc->thePlayer;
					if(var3 == 0) {
						var8->dropPlayerItem(var5->m_draggingItemStack);
						var5->m_draggingItemStack = NULL;
					}

					if(var3 == 1) {
						var8->dropPlayerItem(var5->m_draggingItemStack->splitStack(1));
						if(var5->m_draggingItemStack->stackSize == 0) {
							var5->m_draggingItemStack = NULL;
						}
					}
				}
			}
		}

}

void GuiContainer::mouseMovedOrUp(int var1, int var2, int var3) {
		if(var3 == 0) {
		}

}

void GuiContainer::keyTyped(char var1, int var2) {
		if(var2 == 1 || var2 == mc->m_gameSettings->m_keyBindInventory.m_keyCode) {
			mc->displayGuiScreen(NULL);
		}

}

void GuiContainer::onGuiClosed() {
		if(mc->thePlayer != NULL) {
			InventoryPlayer* var1 = mc->thePlayer->m_inventory;
			if(var1->m_draggingItemStack != NULL) {
				mc->thePlayer->dropPlayerItem(var1->m_draggingItemStack);
				var1->m_draggingItemStack = NULL;
			}

		}
}

bool GuiContainer::doesGuiPauseGame() {
		return false;
}
