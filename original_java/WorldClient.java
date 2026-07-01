package net.minecraft.src;

import java.io.File;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Set;

public class WorldClient extends World {
	private LinkedList blockChangeList = new LinkedList();
	private NetClientHandler sendQueue;
	private ChunkProviderClient C;
	private boolean D = false;
	private MCHashTable entitiesById = new MCHashTable();
	private Set F = new HashSet();
	private Set entitySpawnQueue = new HashSet();

	public WorldClient(NetClientHandler var1, long var2, int var4) {
		super("MpServer", WorldProvider.getProviderForDimension(var4), var2);
		this.sendQueue = var1;
		this.spawnX = 8;
		this.spawnY = 64;
		this.spawnZ = 8;
	}

	public void tick() {
		++this.worldTime;
		int var1 = this.calculateSkylightSubtracted(1.0F);
		int var2;
		if(var1 != this.skylightSubtracted) {
			this.skylightSubtracted = var1;

			for(var2 = 0; var2 < this.worldAccesses.size(); ++var2) {
				((IWorldAccess)this.worldAccesses.get(var2)).updateAllRenderers();
			}
		}

		for(var2 = 0; var2 < 10 && !this.entitySpawnQueue.isEmpty(); ++var2) {
			Entity var3 = (Entity)this.entitySpawnQueue.iterator().next();
			if(!this.loadedEntityList.contains(var3)) {
				this.entityJoinedWorld(var3);
			}
		}

		this.sendQueue.processReadPackets();

		for(var2 = 0; var2 < this.blockChangeList.size(); ++var2) {
			WorldBlockPositionType var4 = (WorldBlockPositionType)this.blockChangeList.get(var2);
			if(--var4.countdown == 0) {
				super.setBlockAndMetadata(var4.xCoord, var4.yCoord, var4.zCoord, var4.blockId, var4.metadata);
				super.markBlockNeedsUpdate(var4.xCoord, var4.yCoord, var4.zCoord);
				this.blockChangeList.remove(var2--);
			}
		}

	}

	public void invalidateBlockReceiveRegion(int var1, int var2, int var3, int var4, int var5, int var6) {
		for(int var7 = 0; var7 < this.blockChangeList.size(); ++var7) {
			WorldBlockPositionType var8 = (WorldBlockPositionType)this.blockChangeList.get(var7);
			if(var8.xCoord >= var1 && var8.yCoord >= var2 && var8.zCoord >= var3 && var8.xCoord <= var4 && var8.yCoord <= var5 && var8.zCoord <= var6) {
				this.blockChangeList.remove(var7--);
			}
		}

	}

	protected IChunkProvider createChunkProvider(File var1) {
		this.C = new ChunkProviderClient(this);
		return this.C;
	}

	public void findSpawnGround() {
		this.spawnX = 8;
		this.spawnY = 64;
		this.spawnZ = 8;
	}

	protected void updateBlocksAroundPlayers() {
	}

	public void scheduleBlockUpdate(int var1, int var2, int var3, int var4) {
	}

	public boolean TickUpdates(boolean var1) {
		return false;
	}

	public void getServerSocket(int var1, int var2, boolean var3) {
		if(var3) {
			this.C.loadChunk(var1, var2);
		} else {
			this.C.unloadChunk(var1, var2);
		}

		if(!var3) {
			this.markBlockRangeNeedsUpdate(var1 * 16, 0, var2 * 16, var1 * 16 + 15, 128, var2 * 16 + 15);
		}

	}

	public boolean entityJoinedWorld(Entity var1) {
		boolean var2 = super.entityJoinedWorld(var1);
		this.F.add(var1);
		if(!var2) {
			this.entitySpawnQueue.add(var1);
		}

		return var2;
	}

	public void setEntityDead(Entity var1) {
		super.setEntityDead(var1);
		this.F.remove(var1);
	}

	protected void obtainEntitySkin(Entity var1) {
		super.obtainEntitySkin(var1);
		if(this.entitySpawnQueue.contains(var1)) {
			this.entitySpawnQueue.remove(var1);
		}

	}

	protected void releaseEntitySkin(Entity var1) {
		super.releaseEntitySkin(var1);
		if(this.F.contains(var1)) {
			this.entitySpawnQueue.add(var1);
		}

	}

	public void addEntityToWorld(int var1, Entity var2) {
		Entity var3 = this.getEntityByID(var1);
		if(var3 != null) {
			this.setEntityDead(var3);
		}

		this.F.add(var2);
		var2.entityId = var1;
		if(!this.entityJoinedWorld(var2)) {
			this.entitySpawnQueue.add(var2);
		}

		this.entitiesById.addKey(var1, var2);
	}

	public Entity getEntityByID(int var1) {
		return (Entity)this.entitiesById.lookup(var1);
	}

	public Entity removeEntityFromWorld(int var1) {
		Entity var2 = (Entity)this.entitiesById.removeObject(var1);
		if(var2 != null) {
			this.F.remove(var2);
			this.setEntityDead(var2);
		}

		return var2;
	}

	public boolean setBlockMetadata(int var1, int var2, int var3, int var4) {
		int var5 = this.getBlockId(var1, var2, var3);
		int var6 = this.getBlockMetadata(var1, var2, var3);
		if(super.setBlockMetadata(var1, var2, var3, var4)) {
			this.blockChangeList.add(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
			return true;
		} else {
			return false;
		}
	}

	public boolean setBlockAndMetadata(int var1, int var2, int var3, int var4, int var5) {
		int var6 = this.getBlockId(var1, var2, var3);
		int var7 = this.getBlockMetadata(var1, var2, var3);
		if(super.setBlockAndMetadata(var1, var2, var3, var4, var5)) {
			this.blockChangeList.add(new WorldBlockPositionType(this, var1, var2, var3, var6, var7));
			return true;
		} else {
			return false;
		}
	}

	public boolean setBlock(int var1, int var2, int var3, int var4) {
		int var5 = this.getBlockId(var1, var2, var3);
		int var6 = this.getBlockMetadata(var1, var2, var3);
		if(super.setBlock(var1, var2, var3, var4)) {
			this.blockChangeList.add(new WorldBlockPositionType(this, var1, var2, var3, var5, var6));
			return true;
		} else {
			return false;
		}
	}

	public boolean setBlockAndMetadataNoQueue(int var1, int var2, int var3, int var4, int var5) {
		this.invalidateBlockReceiveRegion(var1, var2, var3, var1, var2, var3);
		if(super.setBlockAndMetadata(var1, var2, var3, var4, var5)) {
			this.notifyBlockChange(var1, var2, var3, var4);
			return true;
		} else {
			return false;
		}
	}

	public void markTileEntityForUpdate(int var1, int var2, int var3, TileEntity var4) {
		if(!this.D) {
			this.sendQueue.addToSendQueue(new Packet59ComplexEntity(var1, var2, var3, var4));
		}
	}

	public void sendQuittingDisconnectingPacket() {
		this.sendQueue.addToSendQueue(new Packet255KickDisconnect("Quitting"));
	}
}
