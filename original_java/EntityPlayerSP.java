package net.minecraft.src;

import net.minecraft.client.Minecraft;

public class EntityPlayerSP extends EntityPlayer {
	public MovementInput movementInput;
	private Minecraft mc;
	public int timeUntilPortal = 20;
	private boolean inPortal = false;
	public float timeInPortal;
	public float prevTimeInPortal;

	public EntityPlayerSP(Minecraft var1, World var2, Session var3, int var4) {
		super(var2);
		this.mc = var1;
		this.dimension = var4;
		if(var3 != null && var3.inventory != null && var3.inventory.length() > 0) {
			this.skinUrl = "http://www.minecraft.net/skin/" + var3.inventory + ".png";
			System.out.println("Loading texture " + this.skinUrl);
		}

		this.field_771_i = var3.inventory;
	}

	public void updateEntityActionState() {
		super.updateEntityActionState();
		this.moveStrafing = this.movementInput.moveStrafe;
		this.moveForward = this.movementInput.moveForward;
		this.isJumping = this.movementInput.jump;
	}

	public void onLivingUpdate() {
		this.prevTimeInPortal = this.timeInPortal;
		if(this.inPortal) {
			if(this.timeInPortal == 0.0F) {
				this.mc.sndManager.playSoundFX("portal.trigger", 1.0F, this.rand.nextFloat() * 0.4F + 0.8F);
			}

			this.timeInPortal += 0.0125F;
			if(this.timeInPortal >= 1.0F) {
				this.timeInPortal = 1.0F;
				this.timeUntilPortal = 10;
				this.mc.sndManager.playSoundFX("portal.travel", 1.0F, this.rand.nextFloat() * 0.4F + 0.8F);
				this.mc.switchDimension();
			}

			this.inPortal = false;
		} else {
			if(this.timeInPortal > 0.0F) {
				this.timeInPortal -= 0.05F;
			}

			if(this.timeInPortal < 0.0F) {
				this.timeInPortal = 0.0F;
			}
		}

		if(this.timeUntilPortal > 0) {
			--this.timeUntilPortal;
		}

		this.movementInput.updatePlayerMoveState(this);
		if(this.movementInput.sneak && this.ySize < 0.2F) {
			this.ySize = 0.2F;
		}

		super.onLivingUpdate();
	}

	public void resetMovementInput() {
		this.movementInput.resetKeyState();
	}

	public void setKeyState(int var1, boolean var2) {
		this.movementInput.setKeyState(var1, var2);
	}

	public void writeEntityToNBT(NBTTagCompound var1) {
		super.writeEntityToNBT(var1);
		var1.setInteger("Score", this.score);
	}

	public void readEntityFromNBT(NBTTagCompound var1) {
		super.readEntityFromNBT(var1);
		this.score = var1.getInteger("Score");
	}

	public void displayGUIChest(IInventory var1) {
		this.mc.displayGuiScreen(new GuiChest(this.inventory, var1));
	}

	public void displayGUIEditSign(TileEntitySign var1) {
		this.mc.displayGuiScreen(new GuiEditSign(var1));
	}

	public void displayWorkbenchGUI() {
		this.mc.displayGuiScreen(new GuiCrafting(this.inventory));
	}

	public void displayGUIFurnace(TileEntityFurnace var1) {
		this.mc.displayGuiScreen(new GuiFurnace(this.inventory, var1));
	}

	public void onItemPickup(Entity var1, int var2) {
		this.mc.effectRenderer.addEffect(new EntityPickupFX(this.mc.theWorld, var1, this, -0.5F));
	}

	public int getPlayerArmorValue() {
		return this.inventory.getTotalArmorValue();
	}

	public void useCurrentItemOnEntity(Entity var1) {
		if(!var1.interact(this)) {
			ItemStack var2 = this.getCurrentEquippedItem();
			if(var2 != null && var1 instanceof EntityLiving) {
				var2.useItemOnEntity((EntityLiving)var1);
				if(var2.stackSize <= 0) {
					var2.onItemDestroyedByUse(this);
					this.destroyCurrentEquippedItem();
				}
			}

		}
	}

	public void sendChatMessage(String var1) {
	}

	public void onUpdateClientPlayer() {
	}

	public boolean isSneaking() {
		return this.movementInput.sneak;
	}

	public void setInPortal() {
		if(this.timeUntilPortal > 0) {
			this.timeUntilPortal = 10;
		} else {
			this.inPortal = true;
		}
	}

	public void setHealth(int var1) {
		int var2 = this.health - var1;
		if(var2 <= 0) {
			this.health = var1;
		} else {
			this.lastDamage = var2;
			this.prevHealth = this.health;
			this.hurtResistantTime = this.maxHurtResistantTime;
			this.damageEntity(var2);
			this.hurtTime = this.maxHurtTime = 10;
		}

	}

	public void respawnPlayer() {
		this.mc.respawn();
	}
}
