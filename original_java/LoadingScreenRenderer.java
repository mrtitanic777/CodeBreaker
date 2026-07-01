package net.minecraft.src;

import net.minecraft.client.Minecraft;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.GL11;

public class LoadingScreenRenderer implements IProgressUpdate {
	private String message = "";
	private Minecraft mc;
	private String title = "";
	private long systemTime = System.currentTimeMillis();
	private boolean doneWorking = false;

	public LoadingScreenRenderer(Minecraft var1) {
		this.mc = var1;
	}

	public void func_596_a(String var1) {
		this.doneWorking = false;
		this.func_597_c(var1);
	}

	public void displayProgressMessage(String var1) {
		this.doneWorking = true;
		this.func_597_c(this.title);
	}

	public void func_597_c(String var1) {
		if(!this.mc.running) {
			if(!this.doneWorking) {
				throw new MinecraftError();
			}
		} else {
			this.title = var1;
			ScaledResolution var2 = new ScaledResolution(this.mc.displayWidth, this.mc.displayHeight);
			int var3 = var2.getScaledWidth();
			int var4 = var2.getScaledHeight();
			GL11.glClear(GL11.GL_DEPTH_BUFFER_BIT);
			GL11.glMatrixMode(GL11.GL_PROJECTION);
			GL11.glLoadIdentity();
			GL11.glOrtho(0.0D, (double)var3, (double)var4, 0.0D, 100.0D, 300.0D);
			GL11.glMatrixMode(GL11.GL_MODELVIEW);
			GL11.glLoadIdentity();
			GL11.glTranslatef(0.0F, 0.0F, -200.0F);
		}
	}

	public void displayLoadingString(String var1) {
		if(!this.mc.running) {
			if(!this.doneWorking) {
				throw new MinecraftError();
			}
		} else {
			this.systemTime = 0L;
			this.message = var1;
			this.setLoadingProgress(-1);
			this.systemTime = 0L;
		}
	}

	public void setLoadingProgress(int var1) {
		if(!this.mc.running) {
			if(!this.doneWorking) {
				throw new MinecraftError();
			}
		} else {
			long var2 = System.currentTimeMillis();
			if(var2 - this.systemTime >= 20L) {
				this.systemTime = var2;
				ScaledResolution var4 = new ScaledResolution(this.mc.displayWidth, this.mc.displayHeight);
				int var5 = var4.getScaledWidth();
				int var6 = var4.getScaledHeight();
				GL11.glClear(GL11.GL_DEPTH_BUFFER_BIT);
				GL11.glMatrixMode(GL11.GL_PROJECTION);
				GL11.glLoadIdentity();
				GL11.glOrtho(0.0D, (double)var5, (double)var6, 0.0D, 100.0D, 300.0D);
				GL11.glMatrixMode(GL11.GL_MODELVIEW);
				GL11.glLoadIdentity();
				GL11.glTranslatef(0.0F, 0.0F, -200.0F);
				GL11.glClear(GL11.GL_DEPTH_BUFFER_BIT | GL11.GL_COLOR_BUFFER_BIT);
				Tessellator var7 = Tessellator.instance;
				int var8 = this.mc.renderEngine.getTexture("/gui/background.png");
				GL11.glBindTexture(GL11.GL_TEXTURE_2D, var8);
				float var9 = 32.0F;
				var7.startDrawingQuads();
				var7.setColorOpaque_I(4210752);
				var7.addVertexWithUV(0.0D, (double)var6, 0.0D, 0.0D, (double)((float)var6 / var9));
				var7.addVertexWithUV((double)var5, (double)var6, 0.0D, (double)((float)var5 / var9), (double)((float)var6 / var9));
				var7.addVertexWithUV((double)var5, 0.0D, 0.0D, (double)((float)var5 / var9), 0.0D);
				var7.addVertexWithUV(0.0D, 0.0D, 0.0D, 0.0D, 0.0D);
				var7.draw();
				if(var1 >= 0) {
					byte var10 = 100;
					byte var11 = 2;
					int var12 = var5 / 2 - var10 / 2;
					int var13 = var6 / 2 + 16;
					GL11.glDisable(GL11.GL_TEXTURE_2D);
					var7.startDrawingQuads();
					var7.setColorOpaque_I(8421504);
					var7.addVertex((double)var12, (double)var13, 0.0D);
					var7.addVertex((double)var12, (double)(var13 + var11), 0.0D);
					var7.addVertex((double)(var12 + var10), (double)(var13 + var11), 0.0D);
					var7.addVertex((double)(var12 + var10), (double)var13, 0.0D);
					var7.setColorOpaque_I(8454016);
					var7.addVertex((double)var12, (double)var13, 0.0D);
					var7.addVertex((double)var12, (double)(var13 + var11), 0.0D);
					var7.addVertex((double)(var12 + var1), (double)(var13 + var11), 0.0D);
					var7.addVertex((double)(var12 + var1), (double)var13, 0.0D);
					var7.draw();
					GL11.glEnable(GL11.GL_TEXTURE_2D);
				}

				this.mc.fontRenderer.drawStringWithShadow(this.title, (var5 - this.mc.fontRenderer.getStringWidth(this.title)) / 2, var6 / 2 - 4 - 16, 16777215);
				this.mc.fontRenderer.drawStringWithShadow(this.message, (var5 - this.mc.fontRenderer.getStringWidth(this.message)) / 2, var6 / 2 - 4 + 8, 16777215);
				Display.update();

				try {
					Thread.yield();
				} catch (Exception var14) {
				}

			}
		}
	}
}
