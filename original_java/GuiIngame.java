package net.minecraft.client.gui;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import net.minecraft.client.ChatLine;
import net.minecraft.client.Minecraft;
import net.minecraft.client.RenderHelper;
import net.minecraft.client.render.entity.RenderItem;
import net.minecraft.game.entity.player.InventoryPlayer;
import net.minecraft.game.item.ItemStack;
import org.lwjgl.opengl.GL11;

public final class GuiIngame extends Gui {
	private static RenderItem itemRenderer = new RenderItem();
	private List chatMessageList = new ArrayList();
	private Random rand = new Random();
	private Minecraft mc;
	private int updateCounter = 0;

	public GuiIngame(Minecraft var1) {
		this.mc = var1;
	}

	public final void renderGameOverlay(float var1) {
		ScaledResolution var2 = new ScaledResolution(this.mc.displayWidth, this.mc.displayHeight);
		int var3 = var2.getScaledWidth();
		int var19 = var2.getScaledHeight();
		FontRenderer var4 = this.mc.fontRenderer;
		this.mc.entityRenderer.setupOverlayRendering();
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.mc.renderEngine.getTexture("/gui/gui.png"));
		GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
		GL11.glEnable(GL11.GL_BLEND);
		InventoryPlayer var5 = this.mc.thePlayer.inventory;
		this.zLevel = -90.0F;
		this.drawTexturedModalRect(var3 / 2 - 91, var19 - 22, 0, 0, 182, 22);
		this.drawTexturedModalRect(var3 / 2 - 91 - 1 + var5.currentItem * 20, var19 - 22 - 1, 0, 22, 24, 22);
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.mc.renderEngine.getTexture("/gui/icons.png"));
		GL11.glEnable(GL11.GL_BLEND);
		GL11.glBlendFunc(GL11.GL_ONE_MINUS_DST_COLOR, GL11.GL_ONE_MINUS_SRC_COLOR);
		this.drawTexturedModalRect(var3 / 2 - 7, var19 / 2 - 7, 0, 0, 16, 16);
		GL11.glDisable(GL11.GL_BLEND);
		boolean var20 = this.mc.thePlayer.heartsLife / 3 % 2 == 1;
		if(this.mc.thePlayer.heartsLife < 10) {
			var20 = false;
		}

		int var6 = this.mc.thePlayer.health;
		int var7 = this.mc.thePlayer.prevHealth;
		this.rand.setSeed((long)(this.updateCounter * 312871));
		int var10;
		int var12;
		if(this.mc.playerController.shouldDrawHUD()) {
			for(var10 = 0; var10 < 10; ++var10) {
				byte var11 = 0;
				if(var20) {
					var11 = 1;
				}

				var12 = var3 / 2 - 91 + (var10 << 3);
				int var13 = var19 - 32;
				if(var6 <= 4) {
					var13 += this.rand.nextInt(2);
				}

				this.drawTexturedModalRect(var12, var13, 16 + var11 * 9, 0, 9, 9);
				if(var20) {
					if((var10 << 1) + 1 < var7) {
						this.drawTexturedModalRect(var12, var13, 70, 0, 9, 9);
					}

					if((var10 << 1) + 1 == var7) {
						this.drawTexturedModalRect(var12, var13, 79, 0, 9, 9);
					}
				}

				if((var10 << 1) + 1 < var6) {
					this.drawTexturedModalRect(var12, var13, 52, 0, 9, 9);
				}

				if((var10 << 1) + 1 == var6) {
					this.drawTexturedModalRect(var12, var13, 61, 0, 9, 9);
				}
			}

			if(this.mc.thePlayer.isInsideOfMaterial()) {
				var10 = (int)Math.ceil((double)(this.mc.thePlayer.air - 2) * 10.0D / 300.0D);
				int var25 = (int)Math.ceil((double)this.mc.thePlayer.air * 10.0D / 300.0D) - var10;

				for(var12 = 0; var12 < var10 + var25; ++var12) {
					if(var12 < var10) {
						this.drawTexturedModalRect(var3 / 2 - 91 + (var12 << 3), var19 - 32 - 9, 16, 18, 9, 9);
					} else {
						this.drawTexturedModalRect(var3 / 2 - 91 + (var12 << 3), var19 - 32 - 9, 25, 18, 9, 9);
					}
				}
			}
		}

		GL11.glDisable(GL11.GL_BLEND);
		GL11.glEnable(GL11.GL_NORMALIZE);
		GL11.glPushMatrix();
		GL11.glRotatef(180.0F, 1.0F, 0.0F, 0.0F);
		RenderHelper.enableStandardItemLighting();
		GL11.glPopMatrix();

		for(var10 = 0; var10 < 9; ++var10) {
			int var26 = (var3 / 2 - 90 + var10 * 20 + 2);
			int var23 = var19 - 16 - 3;
			ItemStack var21 = this.mc.thePlayer.inventory.mainInventory[var10];
			if(var21 != null) {
				float var9 = (float)var21.animationsToGo - var1;
				if(var9 > 0.0F) {
					GL11.glPushMatrix();
					float var27 = 1.0F + var9 / 5.0F;
					GL11.glTranslatef((float)(var26 + 8), (float)(var23 + 12), 0.0F);
					GL11.glScalef(1.0F / var27, (var27 + 1.0F) / 2.0F, 1.0F);
					GL11.glTranslatef((float)(-(var26 + 8)), (float)(-(var23 + 12)), 0.0F);
				}

				itemRenderer.renderItemIntoGUI(this.mc.renderEngine, var21, var26, var23);
				if(var9 > 0.0F) {
					GL11.glPopMatrix();
				}

				itemRenderer.renderItemOverlayIntoGUI(this.mc.fontRenderer, var21, var26, var23);
			}
		}

		RenderHelper.disableStandardItemLighting();
		GL11.glDisable(GL11.GL_NORMALIZE);
		if(this.mc.options.showFPS) {
			var4.drawStringWithShadow("Minecraft Indev (" + this.mc.debug + ")", 2, 2, 16777215);
			Minecraft var22 = this.mc;
			var4.drawStringWithShadow(var22.renderGlobal.getDebugInfoRenders(), 2, 12, 16777215);
			var22 = this.mc;
			var4.drawStringWithShadow(var22.renderGlobal.getDebugInfoEntities(), 2, 22, 16777215);
			var22 = this.mc;
			var4.drawStringWithShadow("P: " + var22.effectRenderer.getStatistics() + ". T: " + var22.theWorld.debugSkylightUpdates(), 2, 32, 16777215);
			long var24 = Runtime.getRuntime().maxMemory();
			long var27 = Runtime.getRuntime().totalMemory();
			long var14 = Runtime.getRuntime().freeMemory();
			long var16 = var24 - var14;
			String var18 = "Free memory: " + var16 * 100L / var24 + "% of " + var24 / 1024L / 1024L + "MB";
			drawString(var4, var18, var3 - var4.getStringWidth(var18) - 2, 2, 14737632);
			var18 = "Allocated memory: " + var27 * 100L / var24 + "% (" + var27 / 1024L / 1024L + "MB)";
			drawString(var4, var18, var3 - var4.getStringWidth(var18) - 2, 12, 14737632);
		} else {
			var4.drawStringWithShadow("Minecraft Indev", 2, 2, 16777215);
		}

		for(var12 = 0; var12 < this.chatMessageList.size() && var12 < 10; ++var12) {
			if(((ChatLine)this.chatMessageList.get(var12)).updateCounter < 200) {
				this.chatMessageList.get(var12);
				var4.drawStringWithShadow((String)null, 2, var19 - 8 - var12 * 9 - 20, 16777215);
			}
		}

	}

	public final void addChatMessage() {
		++this.updateCounter;

		for(int var1 = 0; var1 < this.chatMessageList.size(); ++var1) {
			++((ChatLine)this.chatMessageList.get(var1)).updateCounter;
		}

	}
}
