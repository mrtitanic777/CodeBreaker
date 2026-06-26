package net.minecraft.client.model;

import util.MathHelper;

public class ModelZombie extends ModelBiped {
	public final void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		super.setRotationAngles(var1, var2, var3, var4, var5, var6);
		var1 = MathHelper.sin(0.0F);
		var2 = MathHelper.sin(0.0F);
		this.a.rotateAngleZ = 0.0F;
		this.b.rotateAngleZ = 0.0F;
		this.a.rotateAngleY = -(0.1F - var1 * 0.6F);
		this.b.rotateAngleY = 0.1F - var1 * 0.6F;
		this.a.rotateAngleX = (float)Math.PI * -0.5F;
		this.b.rotateAngleX = (float)Math.PI * -0.5F;
		this.a.rotateAngleX -= var1 * 1.2F - var2 * 0.4F;
		this.b.rotateAngleX -= var1 * 1.2F - var2 * 0.4F;
		this.a.rotateAngleZ += MathHelper.cos(var3 * 0.09F) * 0.05F + 0.05F;
		this.b.rotateAngleZ -= MathHelper.cos(var3 * 0.09F) * 0.05F + 0.05F;
		this.a.rotateAngleX += MathHelper.sin(var3 * 0.067F) * 0.05F;
		this.b.rotateAngleX -= MathHelper.sin(var3 * 0.067F) * 0.05F;
	}
}
