#include "stdafx.h"
// Auto-generated from ItemSaddle.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "ItemSaddle.h"
#include "Common.h"
#include "EntityLiving.h"
#include "ItemStack.h"
#include "EntityPig.h"
#include "Item.h"

ItemSaddle::ItemSaddle()
{
}

ItemSaddle::ItemSaddle(int var1)
	: Item(var1)
{
		maxStackSize = 1;
		maxDamage = 64;
}

ItemSaddle::~ItemSaddle() {
	// No dynamic resources
}

void ItemSaddle::func_4019_b(ItemStack* var1, EntityLiving* var2) {
		if(dynamic_cast<EntityPig*>(var2) != NULL) {
			EntityPig* var3 = (EntityPig*)var2;
			if(!var3->m_rideable) {
				var3->m_rideable = true;
				--var1->stackSize;
			}
		}

}

void ItemSaddle::hitEntity(ItemStack* var1, EntityLiving* var2) {
		func_4019_b(var1, var2);
}
