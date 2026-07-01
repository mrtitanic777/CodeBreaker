package net.minecraft.src;

public class MaterialLiquid extends Material {
	public boolean getIsLiquid() {
		return true;
	}

	public boolean blocksMovement() {
		return false;
	}

	public boolean isSolid() {
		return false;
	}
}
