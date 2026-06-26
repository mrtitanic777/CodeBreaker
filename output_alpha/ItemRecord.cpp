#include "stdafx.h"
// Auto-generated from ItemRecord.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemRecord.h"
#include "Common.h"
#include "EntityPlayer.h"
#include "ItemStack.h"
#include "World.h"
#include <string>
#include "Item.h"
#include "Block.h"

ItemRecord::ItemRecord()
	: m_recordName("")
{
}

ItemRecord::ItemRecord(int var1, const std::string& var2)
	: Item(var1)
	, m_recordName("")
{
		m_recordName = var2;
		maxStackSize = 1;
}

ItemRecord::~ItemRecord() {
	// No dynamic resources
}

bool ItemRecord::onItemUse(ItemStack* var1, EntityPlayer* var2, World* var3, int var4, int var5, int var6, int var7) {
		if(var3->getBlockId(var4, var5, var6) == Block::s_jukebox->blockID && var3->getBlockMetadata(var4, var5, var6) == 0) {
			var3->setBlockMetadataWithNotify(var4, var5, var6, shiftedIndex - Item::s_record13->shiftedIndex + 1);
			var3->playRecord(m_recordName, var4, var5, var6);
			--var1->stackSize;
			return true;
		} else {
			return false;
		}
}
