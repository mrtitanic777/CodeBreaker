package net.minecraft.src;

import java.awt.image.BufferedImage;

public class IsoImageBuffer {
	public BufferedImage image;
	public World worldObj;
	public int anotherInt;
	public int anInt;
	public boolean isDirty = false;
	public boolean isUpdating = false;
	public int counter = 0;
	public boolean isLoaded = false;

	public IsoImageBuffer(World var1, int var2, int var3) {
		this.worldObj = var1;
		this.resetData(var2, var3);
	}

	public void resetData(int var1, int var2) {
		this.isDirty = false;
		this.anotherInt = var1;
		this.anInt = var2;
		this.counter = 0;
		this.isLoaded = false;
	}

	public void setup(World var1, int var2, int var3) {
		this.worldObj = var1;
		this.resetData(var2, var3);
	}
}
