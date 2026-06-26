#include "stdafx.h"
// Auto-generated from GuiChest.java  (handwritten override)
// Target: Xbox 360 / C++03 / DirectX 9

#include "GuiChest.h"
#include "Common.h"
#include "IInventory.h"
#include "Tesselator.h"
#include "Textures.h"
#include "FontRenderer.h"
#include "Minecraft.h"
#include "SlotInventory.h"

GuiChest::GuiChest()
	: m_field_982_j(NULL)
	, m_field_981_l(NULL)
	, m_field_980_m(0)
{
}

GuiChest::GuiChest(IInventory* var1, IInventory* var2)
	: m_field_982_j(NULL)
	, m_field_981_l(NULL)
	, m_field_980_m(0)
{
		m_field_982_j = var1;
		m_field_981_l = var2;
		m_field_948_f = false;
		short var3 = 222;
		int var4 = var3 - 108;
		m_field_980_m = var2->getSizeInventory() / 9;
		m_ySize = var4 + m_field_980_m * 18;
		int var5 = (m_field_980_m - 4) * 18;

		int var6;
		int var7;
		for(var6 = 0; var6 < m_field_980_m; ++var6) {
			for(var7 = 0; var7 < 9; ++var7) {
				inventorySlots.push_back(new SlotInventory(this, var2, var7 + var6 * 9, 8 + var7 * 18, 18 + var6 * 18));
			}
		}

		for(var6 = 0; var6 < 3; ++var6) {
			for(var7 = 0; var7 < 9; ++var7) {
				inventorySlots.push_back(new SlotInventory(this, var1, var7 + (var6 + 1) * 9, 8 + var7 * 18, 103 + var6 * 18 + var5));
			}
		}

		for(var6 = 0; var6 < 9; ++var6) {
			inventorySlots.push_back(new SlotInventory(this, var1, var6, 8 + var6 * 18, 161 + var5));
		}

}

GuiChest::~GuiChest() {
	m_field_982_j = NULL;
	m_field_981_l = NULL;
}

void GuiChest::drawGuiContainerForegroundLayer() {
		fontRenderer->drawString(m_field_981_l->getInvName(), 8, 6, 4210752);
		fontRenderer->drawString(m_field_982_j->getInvName(), 8, m_ySize - 96 + 2, 4210752);
}

void GuiChest::drawGuiContainerBackgroundLayer(float var1) {
		int var2 = Textures::load("game:\\Content\\gui\\container.png");
  Tesselator::getInstance().color4f(1.0F, 1.0F, 1.0F, 1.0F);
		g_pd3dDevice->SetTexture(0, Textures::getById(var2));
		int var3 = (width - m_xSize) / 2;
		int var4 = (height - m_ySize) / 2;
		drawTexturedModalRect(var3, var4, 0, 0, m_xSize, m_field_980_m * 18 + 17);
		drawTexturedModalRect(var3, var4 + m_field_980_m * 18 + 17, 0, 126, m_xSize, 96);
}
