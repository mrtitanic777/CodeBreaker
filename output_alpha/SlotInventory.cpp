#include "stdafx.h"
// Auto-generated from SlotInventory.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "SlotInventory.h"
#include "Common.h"
#include "GuiContainer.h"
#include "IInventory.h"
#include "Slot.h"

SlotInventory::SlotInventory()
	: m_guiHandler(NULL)
	, m_xPos(0)
	, m_yPos(0)
{
}

SlotInventory::SlotInventory(GuiContainer* var1, IInventory* var2, int var3, int var4, int var5)
	: Slot(var2, var3)
	, m_guiHandler(NULL)
	, m_xPos(0)
	, m_yPos(0)
{
		m_guiHandler = var1;
		m_xPos = var4;
		m_yPos = var5;
}

SlotInventory::~SlotInventory() {
	m_guiHandler = NULL;
}

bool SlotInventory::isAtCursorPos(int var1, int var2) {
		int var3 = (m_guiHandler->width - m_guiHandler->m_xSize) / 2;
		int var4 = (m_guiHandler->height - m_guiHandler->m_ySize) / 2;
		var1 -= var3;
		var2 -= var4;
		return var1 >= m_xPos - 1 && var1 < m_xPos + 16 + 1 && var2 >= m_yPos - 1 && var2 < m_yPos + 16 + 1;
}
