package net.minecraft.src;

import org.lwjgl.opengl.GL11;

public class RenderPlayer extends RenderLiving {
	private ModelBiped modelBipedMain = (ModelBiped)this.e;
	private ModelBiped modelArmorChestplate = new ModelBiped(1.0F);
	private ModelBiped modelArmor = new ModelBiped(0.5F);
	private static final String[] armorFilenamePrefix = new String[]{"cloth", "chain", "iron", "diamond", "gold"};

	public RenderPlayer() {
		super(new ModelBiped(0.0F), 0.5F);
	}

	protected boolean a(EntityPlayer var1, int var2) {
		ItemStack var3 = var1.inventory.armorItemInSlot(3 - var2);
		if(var3 != null) {
			Item var4 = var3.getItem();
			if(var4 instanceof ItemArmor) {
				ItemArmor var5 = (ItemArmor)var4;
				this.loadTexture("/armor/" + armorFilenamePrefix[var5.renderIndex] + "_" + (var2 == 2 ? 2 : 1) + ".png");
				ModelBiped var6 = var2 == 2 ? this.modelArmor : this.modelArmorChestplate;
				var6.bipedHead.showModel = var2 == 0;
				var6.bipedHeadwear.showModel = var2 == 0;
				var6.bipedBody.showModel = var2 == 1 || var2 == 2;
				var6.bipedRightArm.showModel = var2 == 1;
				var6.bipedLeftArm.showModel = var2 == 1;
				var6.bipedRightLeg.showModel = var2 == 2 || var2 == 3;
				var6.bipedLeftLeg.showModel = var2 == 2 || var2 == 3;
				this.setRenderPassModel(var6);
				return true;
			}
		}

		return false;
	}

	public void a(EntityPlayer var1, double var2, double var4, double var6, float var8, float var9) {
		ItemStack var10 = var1.inventory.getCurrentItem();
		this.modelArmorChestplate.heldItemRight = this.modelArmor.heldItemRight = this.modelBipedMain.heldItemRight = var10 != null;
		this.modelArmorChestplate.isSneak = this.modelArmor.isSneak = this.modelBipedMain.isSneak = var1.isSneaking();
		double var11 = var4 - (double)var1.yOffset;
		if(var1.isSneakingRender) {
			var11 -= 0.125D;
		}

		super.a(var1, var2, var11, var6, var8, var9);
		this.modelArmorChestplate.isSneak = this.modelArmor.isSneak = this.modelBipedMain.isSneak = false;
		this.modelArmorChestplate.heldItemRight = this.modelArmor.heldItemRight = this.modelBipedMain.heldItemRight = false;
		float var13 = 1.6F;
		float var14 = (float)(1.0D / 60.0D) * var13;
		float var15 = var1.getDistanceToEntity(this.renderManager.livingPlayer);
		float var16 = var1.isSneaking() ? 32.0F : 64.0F;
		if(var15 < var16) {
			var14 = (float)((double)var14 * (Math.sqrt((double)var15) / 2.0D));
			FontRenderer var17 = this.getFontRendererFromRenderManager();
			GL11.glPushMatrix();
			GL11.glTranslatef((float)var2 + 0.0F, (float)var4 + 2.3F, (float)var6);
			GL11.glNormal3f(0.0F, 1.0F, 0.0F);
			GL11.glRotatef(-this.renderManager.playerViewYaw, 0.0F, 1.0F, 0.0F);
			GL11.glRotatef(this.renderManager.playerViewPitch, 1.0F, 0.0F, 0.0F);
			GL11.glScalef(-var14, -var14, var14);
			String var18 = var1.field_771_i;
			GL11.glDisable(GL11.GL_LIGHTING);
			Tessellator var19;
			int var20;
			if(!var1.isSneaking()) {
				GL11.glDepthMask(false);
				GL11.glDisable(GL11.GL_DEPTH_TEST);
				GL11.glEnable(GL11.GL_BLEND);
				GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE_MINUS_SRC_ALPHA);
				var19 = Tessellator.instance;
				GL11.glDisable(GL11.GL_TEXTURE_2D);
				var19.startDrawingQuads();
				var20 = var17.getStringWidth(var18) / 2;
				var19.setColorRGBA_F(0.0F, 0.0F, 0.0F, 0.25F);
				var19.addVertex((double)(-var20 - 1), -1.0D, 0.0D);
				var19.addVertex((double)(-var20 - 1), 8.0D, 0.0D);
				var19.addVertex((double)(var20 + 1), 8.0D, 0.0D);
				var19.addVertex((double)(var20 + 1), -1.0D, 0.0D);
				var19.draw();
				GL11.glEnable(GL11.GL_TEXTURE_2D);
				var17.drawString(var18, -var17.getStringWidth(var18) / 2, 0, 553648127);
				GL11.glEnable(GL11.GL_DEPTH_TEST);
				GL11.glDepthMask(true);
				var17.drawString(var18, -var17.getStringWidth(var18) / 2, 0, -1);
				GL11.glEnable(GL11.GL_LIGHTING);
				GL11.glDisable(GL11.GL_BLEND);
				GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
				GL11.glPopMatrix();
			} else {
				GL11.glTranslatef(0.0F, 0.25F / var14, 0.0F);
				GL11.glDepthMask(false);
				GL11.glEnable(GL11.GL_BLEND);
				GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE_MINUS_SRC_ALPHA);
				var19 = Tessellator.instance;
				GL11.glDisable(GL11.GL_TEXTURE_2D);
				var19.startDrawingQuads();
				var20 = var17.getStringWidth(var18) / 2;
				var19.setColorRGBA_F(0.0F, 0.0F, 0.0F, 0.25F);
				var19.addVertex((double)(-var20 - 1), -1.0D, 0.0D);
				var19.addVertex((double)(-var20 - 1), 8.0D, 0.0D);
				var19.addVertex((double)(var20 + 1), 8.0D, 0.0D);
				var19.addVertex((double)(var20 + 1), -1.0D, 0.0D);
				var19.draw();
				GL11.glEnable(GL11.GL_TEXTURE_2D);
				GL11.glDepthMask(true);
				var17.drawString(var18, -var17.getStringWidth(var18) / 2, 0, 553648127);
				GL11.glEnable(GL11.GL_LIGHTING);
				GL11.glDisable(GL11.GL_BLEND);
				GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
				GL11.glPopMatrix();
			}
		}

	}

	protected void a(EntityPlayer var1, float var2) {
		ItemStack var3 = var1.inventory.armorItemInSlot(3);
		if(var3 != null && var3.getItem().shiftedIndex < 256) {
			GL11.glPushMatrix();
			this.modelBipedMain.bipedHead.postRender(1.0F / 16.0F);
			if(RenderBlocks.canRenderBlockAsItem3D(Block.blocksList[var3.itemID].getRenderType())) {
				float var4 = 10.0F / 16.0F;
				GL11.glTranslatef(0.0F, -0.25F, 0.0F);
				GL11.glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
				GL11.glScalef(var4, -var4, var4);
			}

			this.renderManager.itemRenderer.renderItem(var3);
			GL11.glPopMatrix();
		}

		ItemStack var6 = var1.inventory.getCurrentItem();
		if(var6 != null) {
			GL11.glPushMatrix();
			this.modelBipedMain.bipedRightArm.postRender(1.0F / 16.0F);
			GL11.glTranslatef(-(1.0F / 16.0F), 7.0F / 16.0F, 1.0F / 16.0F);
			if(var1.fishEntity != null) {
				var6 = new ItemStack(Item.stick.shiftedIndex);
			}

			float var5;
			if(var6.itemID < 256 && RenderBlocks.canRenderBlockAsItem3D(Block.blocksList[var6.itemID].getRenderType())) {
				var5 = 0.5F;
				GL11.glTranslatef(0.0F, 3.0F / 16.0F, -(5.0F / 16.0F));
				var5 *= 12.0F / 16.0F;
				GL11.glRotatef(20.0F, 1.0F, 0.0F, 0.0F);
				GL11.glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
				GL11.glScalef(var5, -var5, var5);
			} else if(Item.itemsList[var6.itemID].isFull3D()) {
				var5 = 10.0F / 16.0F;
				if(Item.itemsList[var6.itemID].shouldRotateAroundWhenRendering()) {
					GL11.glRotatef(180.0F, 0.0F, 0.0F, 1.0F);
					GL11.glTranslatef(0.0F, -(2.0F / 16.0F), 0.0F);
				}

				GL11.glTranslatef(0.0F, 3.0F / 16.0F, 0.0F);
				GL11.glScalef(var5, -var5, var5);
				GL11.glRotatef(-100.0F, 1.0F, 0.0F, 0.0F);
				GL11.glRotatef(45.0F, 0.0F, 1.0F, 0.0F);
			} else {
				var5 = 6.0F / 16.0F;
				GL11.glTranslatef(0.25F, 3.0F / 16.0F, -(3.0F / 16.0F));
				GL11.glScalef(var5, var5, var5);
				GL11.glRotatef(60.0F, 0.0F, 0.0F, 1.0F);
				GL11.glRotatef(-90.0F, 1.0F, 0.0F, 0.0F);
				GL11.glRotatef(20.0F, 0.0F, 0.0F, 1.0F);
			}

			this.renderManager.itemRenderer.renderItem(var6);
			GL11.glPopMatrix();
		}

	}

	protected void b(EntityPlayer var1, float var2) {
		float var3 = 15.0F / 16.0F;
		GL11.glScalef(var3, var3, var3);
	}

	public void drawFirstPersonHand() {
		this.modelBipedMain.onGround = 0.0F;
		this.modelBipedMain.setRotationAngles(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F / 16.0F);
		this.modelBipedMain.bipedRightArm.render(1.0F / 16.0F);
	}

	protected void preRenderCallback(EntityLiving var1, float var2) {
		this.b((EntityPlayer)var1, var2);
	}

	protected boolean shouldRenderPass(EntityLiving var1, int var2) {
		return this.a((EntityPlayer)var1, var2);
	}

	protected void renderEquippedItems(EntityLiving var1, float var2) {
		this.a((EntityPlayer)var1, var2);
	}

	public void a(EntityLiving var1, double var2, double var4, double var6, float var8, float var9) {
		this.a((EntityPlayer)var1, var2, var4, var6, var8, var9);
	}

	public void doRender(Entity var1, double var2, double var4, double var6, float var8, float var9) {
		this.a((EntityPlayer)var1, var2, var4, var6, var8, var9);
	}
}
