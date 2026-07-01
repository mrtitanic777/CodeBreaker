package net.minecraft.src;

class SlotArmor extends SlotInventory {
	final int armorType;
	final GuiInventory guiInventory;

	SlotArmor(GuiInventory var1, GuiContainer var2, IInventory var3, int var4, int var5, int var6, int var7) {
		super(var2, var3, var4, var5, var6);
		this.guiInventory = var1;
		this.armorType = var7;
	}

	public int getSlotStackLimit() {
		return 1;
	}

	public boolean isItemValid(ItemStack var1) {
		if(var1.getItem() instanceof ItemArmor) {
			return ((ItemArmor)var1.getItem()).armorType == this.armorType;
		} else {
			System.out.println(var1.getItem().shiftedIndex + ", " + this.armorType);
			return var1.getItem().shiftedIndex == Block.pumpkin.blockID ? this.armorType == 0 : false;
		}
	}

	public int getBackgroundIconIndex() {
		return 15 + this.armorType * 16;
	}
}
