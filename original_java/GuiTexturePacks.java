package net.minecraft.src;

import java.io.File;
import java.util.List;
import org.lwjgl.Sys;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.GL11;

public class GuiTexturePacks extends GuiScreen {
	protected GuiScreen parentScreen;
	private int scrollOffset = 0;
	private int listTop = 32;
	private int listBottom = this.height - 55 + 4;
	private int listLeft = 0;
	private int listRight = this.width;
	private int mousePrevY = -2;
	private int refreshTimer = -1;
	private String texturePackDir = "";

	public GuiTexturePacks(GuiScreen var1) {
		this.parentScreen = var1;
	}

	public void initGui() {
		this.controlList.add(new GuiSmallButton(5, this.width / 2 - 154, this.height - 48, "Open texture pack folder"));
		this.controlList.add(new GuiSmallButton(6, this.width / 2 + 4, this.height - 48, "Done"));
		this.mc.texturePackList.updateAvailableTexturePacks();
		this.texturePackDir = (new File(this.mc.mcDataDir, "texturepacks")).getAbsolutePath();
		this.listTop = 32;
		this.listBottom = this.height - 58 + 4;
		this.listLeft = 0;
		this.listRight = this.width;
	}

	protected void actionPerformed(GuiButton var1) {
		if(var1.enabled) {
			if(var1.id == 5) {
				Sys.openURL("file://" + this.texturePackDir);
			}

			if(var1.id == 6) {
				this.mc.renderEngine.refreshTextures();
				this.mc.displayGuiScreen(this.parentScreen);
			}

		}
	}

	protected void mouseClicked(int var1, int var2, int var3) {
		super.mouseClicked(var1, var2, var3);
	}

	protected void mouseMovedOrUp(int var1, int var2, int var3) {
		super.mouseMovedOrUp(var1, var2, var3);
	}

	public void drawScreen(int var1, int var2, float var3) {
		this.drawDefaultBackground();
		if(this.refreshTimer <= 0) {
			this.mc.texturePackList.updateAvailableTexturePacks();
			this.refreshTimer += 20;
		}

		List var4 = this.mc.texturePackList.availableTexturePacks();
		int var5;
		if(Mouse.isButtonDown(0)) {
			if(this.mousePrevY == -1) {
				if(var2 >= this.listTop && var2 <= this.listBottom) {
					var5 = this.width / 2 - 110;
					int var6 = this.width / 2 + 110;
					int var7 = (var2 - this.listTop + this.scrollOffset - 2) / 36;
					if(var1 >= var5 && var1 <= var6 && var7 >= 0 && var7 < var4.size() && this.mc.texturePackList.setTexturePack((TexturePackBase)var4.get(var7))) {
						this.mc.renderEngine.refreshTextures();
					}

					this.mousePrevY = var2;
				} else {
					this.mousePrevY = -2;
				}
			} else if(this.mousePrevY >= 0) {
				this.scrollOffset -= var2 - this.mousePrevY;
				this.mousePrevY = var2;
			}
		} else {
			if(this.mousePrevY >= 0 && this.mousePrevY == var2) {
			}

			this.mousePrevY = -1;
		}

		var5 = var4.size() * 36 - (this.listBottom - this.listTop - 4);
		if(var5 < 0) {
			var5 /= 2;
		}

		if(this.scrollOffset < 0) {
			this.scrollOffset = 0;
		}

		if(this.scrollOffset > var5) {
			this.scrollOffset = var5;
		}

		GL11.glDisable(GL11.GL_LIGHTING);
		GL11.glDisable(GL11.GL_FOG);
		Tessellator var16 = Tessellator.instance;
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.mc.renderEngine.getTexture("/gui/background.png"));
		GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
		float var17 = 32.0F;
		var16.startDrawingQuads();
		var16.setColorOpaque_I(2105376);
		var16.addVertexWithUV((double)this.listLeft, (double)this.listBottom, 0.0D, (double)((float)this.listLeft / var17), (double)((float)(this.listBottom + this.scrollOffset) / var17));
		var16.addVertexWithUV((double)this.listRight, (double)this.listBottom, 0.0D, (double)((float)this.listRight / var17), (double)((float)(this.listBottom + this.scrollOffset) / var17));
		var16.addVertexWithUV((double)this.listRight, (double)this.listTop, 0.0D, (double)((float)this.listRight / var17), (double)((float)(this.listTop + this.scrollOffset) / var17));
		var16.addVertexWithUV((double)this.listLeft, (double)this.listTop, 0.0D, (double)((float)this.listLeft / var17), (double)((float)(this.listTop + this.scrollOffset) / var17));
		var16.draw();

		for(int var8 = 0; var8 < var4.size(); ++var8) {
			TexturePackBase var9 = (TexturePackBase)var4.get(var8);
			int var10 = this.width / 2 - 92 - 16;
			int var11 = 36 + var8 * 36 - this.scrollOffset;
			byte var12 = 32;
			byte var13 = 32;
			if(var9 == this.mc.texturePackList.selectedTexturePack) {
				int var14 = this.width / 2 - 110;
				int var15 = this.width / 2 + 110;
				GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
				GL11.glDisable(GL11.GL_TEXTURE_2D);
				var16.startDrawingQuads();
				var16.setColorOpaque_I(8421504);
				var16.addVertexWithUV((double)var14, (double)(var11 + var12 + 2), 0.0D, 0.0D, 1.0D);
				var16.addVertexWithUV((double)var15, (double)(var11 + var12 + 2), 0.0D, 1.0D, 1.0D);
				var16.addVertexWithUV((double)var15, (double)(var11 - 2), 0.0D, 1.0D, 0.0D);
				var16.addVertexWithUV((double)var14, (double)(var11 - 2), 0.0D, 0.0D, 0.0D);
				var16.setColorOpaque_I(0);
				var16.addVertexWithUV((double)(var14 + 1), (double)(var11 + var12 + 1), 0.0D, 0.0D, 1.0D);
				var16.addVertexWithUV((double)(var15 - 1), (double)(var11 + var12 + 1), 0.0D, 1.0D, 1.0D);
				var16.addVertexWithUV((double)(var15 - 1), (double)(var11 - 1), 0.0D, 1.0D, 0.0D);
				var16.addVertexWithUV((double)(var14 + 1), (double)(var11 - 1), 0.0D, 0.0D, 0.0D);
				var16.draw();
				GL11.glEnable(GL11.GL_TEXTURE_2D);
			}

			var9.unbindTexturePack(this.mc);
			GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
			var16.startDrawingQuads();
			var16.setColorOpaque_I(16777215);
			var16.addVertexWithUV((double)var10, (double)(var11 + var12), 0.0D, 0.0D, 1.0D);
			var16.addVertexWithUV((double)(var10 + var13), (double)(var11 + var12), 0.0D, 1.0D, 1.0D);
			var16.addVertexWithUV((double)(var10 + var13), (double)var11, 0.0D, 1.0D, 0.0D);
			var16.addVertexWithUV((double)var10, (double)var11, 0.0D, 0.0D, 0.0D);
			var16.draw();
			this.drawString(this.fontRenderer, var9.texturePackFileName, var10 + var13 + 2, var11 + 1, 16777215);
			this.drawString(this.fontRenderer, var9.firstDescriptionLine, var10 + var13 + 2, var11 + 12, 8421504);
			this.drawString(this.fontRenderer, var9.secondDescriptionLine, var10 + var13 + 2, var11 + 12 + 10, 8421504);
		}

		byte var18 = 4;
		this.drawGradientOverlay(0, this.listTop, 255, 255);
		this.drawGradientOverlay(this.listBottom, this.height, 255, 255);
		GL11.glEnable(GL11.GL_BLEND);
		GL11.glBlendFunc(GL11.GL_SRC_ALPHA, GL11.GL_ONE_MINUS_SRC_ALPHA);
		GL11.glDisable(GL11.GL_ALPHA_TEST);
		GL11.glShadeModel(GL11.GL_SMOOTH);
		GL11.glDisable(GL11.GL_TEXTURE_2D);
		var16.startDrawingQuads();
		var16.setColorRGBA_I(0, 0);
		var16.addVertexWithUV((double)this.listLeft, (double)(this.listTop + var18), 0.0D, 0.0D, 1.0D);
		var16.addVertexWithUV((double)this.listRight, (double)(this.listTop + var18), 0.0D, 1.0D, 1.0D);
		var16.setColorRGBA_I(0, 255);
		var16.addVertexWithUV((double)this.listRight, (double)this.listTop, 0.0D, 1.0D, 0.0D);
		var16.addVertexWithUV((double)this.listLeft, (double)this.listTop, 0.0D, 0.0D, 0.0D);
		var16.draw();
		var16.startDrawingQuads();
		var16.setColorRGBA_I(0, 255);
		var16.addVertexWithUV((double)this.listLeft, (double)this.listBottom, 0.0D, 0.0D, 1.0D);
		var16.addVertexWithUV((double)this.listRight, (double)this.listBottom, 0.0D, 1.0D, 1.0D);
		var16.setColorRGBA_I(0, 0);
		var16.addVertexWithUV((double)this.listRight, (double)(this.listBottom - var18), 0.0D, 1.0D, 0.0D);
		var16.addVertexWithUV((double)this.listLeft, (double)(this.listBottom - var18), 0.0D, 0.0D, 0.0D);
		var16.draw();
		GL11.glEnable(GL11.GL_TEXTURE_2D);
		GL11.glShadeModel(GL11.GL_FLAT);
		GL11.glEnable(GL11.GL_ALPHA_TEST);
		GL11.glDisable(GL11.GL_BLEND);
		this.drawCenteredString(this.fontRenderer, "Select Texture Pack", this.width / 2, 16, 16777215);
		this.drawCenteredString(this.fontRenderer, "(Place texture pack files here)", this.width / 2 - 77, this.height - 26, 8421504);
		super.drawScreen(var1, var2, var3);
	}

	public void updateScreen() {
		super.updateScreen();
		--this.refreshTimer;
	}

	public void drawGradientOverlay(int var1, int var2, int var3, int var4) {
		Tessellator var5 = Tessellator.instance;
		GL11.glBindTexture(GL11.GL_TEXTURE_2D, this.mc.renderEngine.getTexture("/gui/background.png"));
		GL11.glColor4f(1.0F, 1.0F, 1.0F, 1.0F);
		float var6 = 32.0F;
		var5.startDrawingQuads();
		var5.setColorRGBA_I(4210752, var4);
		var5.addVertexWithUV(0.0D, (double)var2, 0.0D, 0.0D, (double)((float)var2 / var6));
		var5.addVertexWithUV((double)this.width, (double)var2, 0.0D, (double)((float)this.width / var6), (double)((float)var2 / var6));
		var5.setColorRGBA_I(4210752, var3);
		var5.addVertexWithUV((double)this.width, (double)var1, 0.0D, (double)((float)this.width / var6), (double)((float)var1 / var6));
		var5.addVertexWithUV(0.0D, (double)var1, 0.0D, 0.0D, (double)((float)var1 / var6));
		var5.draw();
	}
}
