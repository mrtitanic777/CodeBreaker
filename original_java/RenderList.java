package net.minecraft.src;

import java.nio.IntBuffer;
import org.lwjgl.opengl.GL11;

public class RenderList {
	private int posX;
	private int posY;
	private int posZ;
	private float cameraX;
	private float cameraY;
	private float cameraZ;
	private IntBuffer intBuffer = GLAllocation.createDirectIntBuffer(65536);
	private boolean isInitialized = false;
	private boolean isBufferFlipped = false;

	public void setupRenderList(int var1, int var2, int var3, double var4, double var6, double var8) {
		this.isInitialized = true;
		this.intBuffer.clear();
		this.posX = var1;
		this.posY = var2;
		this.posZ = var3;
		this.cameraX = (float)var4;
		this.cameraY = (float)var6;
		this.cameraZ = (float)var8;
	}

	public boolean isPositionMatching(int var1, int var2, int var3) {
		return !this.isInitialized ? false : var1 == this.posX && var2 == this.posY && var3 == this.posZ;
	}

	public void addGLRenderList(int var1) {
		this.intBuffer.put(var1);
		if(this.intBuffer.remaining() == 0) {
			this.callLists();
		}

	}

	public void callLists() {
		if(this.isInitialized) {
			if(!this.isBufferFlipped) {
				this.intBuffer.flip();
				this.isBufferFlipped = true;
			}

			if(this.intBuffer.remaining() > 0) {
				GL11.glPushMatrix();
				GL11.glTranslatef((float)this.posX - this.cameraX, (float)this.posY - this.cameraY, (float)this.posZ - this.cameraZ);
				GL11.glCallLists(this.intBuffer);
				GL11.glPopMatrix();
			}

		}
	}

	public void resetList() {
		this.isInitialized = false;
		this.isBufferFlipped = false;
	}
}
