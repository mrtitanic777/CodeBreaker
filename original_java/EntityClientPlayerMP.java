package net.minecraft.src;

import net.minecraft.client.Minecraft;

public class EntityClientPlayerMP extends EntityPlayerSP {
	private NetClientHandler sendQueue;
	private int inventorySyncTimer = 0;
	private double oldPosX;
	private double oldMinY;
	private double oldPosY;
	private double oldPosZ;
	private float oldRotationYaw;
	private float oldRotationPitch;
	private InventoryPlayer lastSentInventory = new InventoryPlayer((EntityPlayer)null);
	private boolean wasOnGround = false;
	private boolean wasSwinging = false;
	private int positionUpdateTicks = 0;

	public EntityClientPlayerMP(Minecraft var1, World var2, Session var3, NetClientHandler var4) {
		super(var1, var2, var3, 0);
		this.sendQueue = var4;
	}

	public boolean canAttackEntity(Entity var1, int var2) {
		return false;
	}

	public void heal(int var1) {
	}

	public void onUpdate() {
		if(this.worldObj.blockExists(MathHelper.floor_double(this.posX), 64, MathHelper.floor_double(this.posZ))) {
			super.onUpdate();
			this.onUpdateWalkingPlayer();
		}
	}

	public void onUpdateClientPlayer() {
	}

	public void onUpdateWalkingPlayer() {
		if(this.inventorySyncTimer++ == 20) {
			this.sendInventoryChanged();
			this.inventorySyncTimer = 0;
		}

		boolean var1 = this.isSneaking();
		if(var1 != this.wasSwinging) {
			if(var1) {
				this.sendQueue.addToSendQueue(new Packet18ArmAnimation(this, 104));
			} else {
				this.sendQueue.addToSendQueue(new Packet18ArmAnimation(this, 105));
			}

			this.wasSwinging = var1;
		}

		double var2 = this.posX - this.oldPosX;
		double var4 = this.boundingBox.minY - this.oldMinY;
		double var6 = this.posY - this.oldPosY;
		double var8 = this.posZ - this.oldPosZ;
		double var10 = (double)(this.rotationYaw - this.oldRotationYaw);
		double var12 = (double)(this.rotationPitch - this.oldRotationPitch);
		boolean var14 = var4 != 0.0D || var6 != 0.0D || var2 != 0.0D || var8 != 0.0D;
		boolean var15 = var10 != 0.0D || var12 != 0.0D;
		if(this.ridingEntity != null) {
			if(var15) {
				this.sendQueue.addToSendQueue(new Packet11PlayerPosition(this.motionX, -999.0D, -999.0D, this.motionZ, this.onGround));
			} else {
				this.sendQueue.addToSendQueue(new Packet13PlayerLookMove(this.motionX, -999.0D, -999.0D, this.motionZ, this.rotationYaw, this.rotationPitch, this.onGround));
			}

			var14 = false;
		} else if(var14 && var15) {
			this.sendQueue.addToSendQueue(new Packet13PlayerLookMove(this.posX, this.boundingBox.minY, this.posY, this.posZ, this.rotationYaw, this.rotationPitch, this.onGround));
			this.positionUpdateTicks = 0;
		} else if(var14) {
			this.sendQueue.addToSendQueue(new Packet11PlayerPosition(this.posX, this.boundingBox.minY, this.posY, this.posZ, this.onGround));
			this.positionUpdateTicks = 0;
		} else if(var15) {
			this.sendQueue.addToSendQueue(new Packet12PlayerLook(this.rotationYaw, this.rotationPitch, this.onGround));
			this.positionUpdateTicks = 0;
		} else {
			this.sendQueue.addToSendQueue(new Packet10Flying(this.onGround));
			if(this.wasOnGround == this.onGround && this.positionUpdateTicks <= 20) {
				++this.positionUpdateTicks;
			} else {
				this.positionUpdateTicks = 0;
			}
		}

		this.wasOnGround = this.onGround;
		if(var14) {
			this.oldPosX = this.posX;
			this.oldMinY = this.boundingBox.minY;
			this.oldPosY = this.posY;
			this.oldPosZ = this.posZ;
		}

		if(var15) {
			this.oldRotationYaw = this.rotationYaw;
			this.oldRotationPitch = this.rotationPitch;
		}

	}

	private void sendInventoryChanged() {
		if(!this.inventory.compareInventory(this.lastSentInventory)) {
			this.sendQueue.addToSendQueue(new Packet5PlayerInventory(-1, this.inventory.mainInventory));
			this.sendQueue.addToSendQueue(new Packet5PlayerInventory(-2, this.inventory.craftingInventory));
			this.sendQueue.addToSendQueue(new Packet5PlayerInventory(-3, this.inventory.armorInventory));
			this.lastSentInventory = this.inventory.copyInventory();
		}

	}

	protected void joinEntityItemWithWorld(EntityItem var1) {
		Packet21PickupSpawn var2 = new Packet21PickupSpawn(var1);
		this.sendQueue.addToSendQueue(var2);
		var1.posX = (double)var2.xPosition / 32.0D;
		var1.posY = (double)var2.yPosition / 32.0D;
		var1.posZ = (double)var2.zPosition / 32.0D;
		var1.motionX = (double)var2.rotation / 128.0D;
		var1.motionY = (double)var2.pitch / 128.0D;
		var1.motionZ = (double)var2.roll / 128.0D;
	}

	public void sendChatMessage(String var1) {
		this.sendQueue.addToSendQueue(new Packet3Chat(var1));
	}

	public void swingItem() {
		super.swingItem();
		this.sendQueue.addToSendQueue(new Packet18ArmAnimation(this, 1));
	}

	public void respawnPlayer() {
		this.sendInventoryChanged();
		this.sendQueue.addToSendQueue(new Packet9());
	}

	protected void damageEntity(int var1) {
		this.health -= var1;
	}
}
