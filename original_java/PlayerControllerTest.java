package net.minecraft.src;

import net.minecraft.client.Minecraft;

public class PlayerControllerTest extends PlayerController {
	public PlayerControllerTest(Minecraft var1) {
		super(var1);
		this.isCreativeMode = true;
	}

	public void onPlayerCreated(EntityPlayer var1) {
		for(int var2 = 0; var2 < 9; ++var2) {
			if(var1.inventory.mainInventory[var2] == null) {
				this.mc.thePlayer.inventory.mainInventory[var2] = new ItemStack(((Block)Session.registeredBlocksList.get(var2)).blockID);
			} else {
				this.mc.thePlayer.inventory.mainInventory[var2].stackSize = 1;
			}
		}

	}

	public boolean shouldDrawHealthBar() {
		return false;
	}

	public void addPendingConnection(World var1) {
		super.addPendingConnection(var1);
	}

	public void resetBlockRemovingSound() {
	}
}
