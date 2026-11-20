package sample;

// Synthetic fixture (original). A wider interface with several abstract methods.
public interface IInventory {
	int getSizeInventory();

	ItemStack getStackInSlot(int var1);

	void setInventorySlotContents(int var1, ItemStack var2);

	String getInvName();
}
