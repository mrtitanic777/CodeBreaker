package net.minecraft.src;

class WorldBlockPositionType {
	int xCoord;
	int yCoord;
	int zCoord;
	int countdown;
	int blockId;
	int metadata;
	final WorldClient worldClient;

	public WorldBlockPositionType(WorldClient var1, int var2, int var3, int var4, int var5, int var6) {
		this.worldClient = var1;
		this.xCoord = var2;
		this.yCoord = var3;
		this.zCoord = var4;
		this.countdown = 80;
		this.blockId = var5;
		this.metadata = var6;
	}
}
