package net.minecraft.client.model;

import util.MathHelper;

public class ModelBiped extends ModelBase {
	private ModelRenderer e;
	private ModelRenderer f;
	private ModelRenderer g;
	public ModelRenderer a;
	public ModelRenderer b;
	public ModelRenderer c;
	public ModelRenderer d;

	public ModelBiped() {
		this(0.0F);
	}

	public ModelBiped(float var1) {
		this.e = new ModelRenderer(0, 0);
		this.e.addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1);
		this.f = new ModelRenderer(32, 0);
		this.f.addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8, var1 + 0.5F);
		this.g = new ModelRenderer(16, 16);
		this.g.addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4, var1);
		this.a = new ModelRenderer(40, 16);
		this.a.addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		this.a.setRotationPoint(-5.0F, 2.0F, 0.0F);
		this.b = new ModelRenderer(40, 16);
		this.b.mirror = true;
		this.b.addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4, var1);
		this.b.setRotationPoint(5.0F, 2.0F, 0.0F);
		this.c = new ModelRenderer(0, 16);
		this.c.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		this.c.setRotationPoint(-2.0F, 12.0F, 0.0F);
		this.d = new ModelRenderer(0, 16);
		this.d.mirror = true;
		this.d.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4, var1);
		this.d.setRotationPoint(2.0F, 12.0F, 0.0F);
	}

	public final void render(float var1, float var2, float var3, float var4, float var5, float var6) {
		this.setRotationAngles(var1, var2, var3, var4, var5, 1.0F);
		this.e.render(1.0F);
		this.g.render(1.0F);
		this.a.render(1.0F);
		this.b.render(1.0F);
		this.c.render(1.0F);
		this.d.render(1.0F);
		this.f.render(1.0F);
	}

	public void setRotationAngles(float var1, float var2, float var3, float var4, float var5, float var6) {
		this.e.rotateAngleY = var4 / (180.0F / (float)Math.PI);
		this.e.rotateAngleX = var5 / (180.0F / (float)Math.PI);
		this.f.rotateAngleY = this.e.rotateAngleY;
		this.f.rotateAngleX = this.e.rotateAngleX;
		this.a.rotateAngleX = MathHelper.cos(var1 * 0.6662F + (float)Math.PI) * 2.0F * var2;
		this.a.rotateAngleZ = (MathHelper.cos(var1 * 0.2312F) + 1.0F) * var2;
		this.b.rotateAngleX = MathHelper.cos(var1 * 0.6662F) * 2.0F * var2;
		this.b.rotateAngleZ = (MathHelper.cos(var1 * 0.2812F) - 1.0F) * var2;
		this.c.rotateAngleX = MathHelper.cos(var1 * 0.6662F) * 1.4F * var2;
		this.d.rotateAngleX = MathHelper.cos(var1 * 0.6662F + (float)Math.PI) * 1.4F * var2;
		this.a.rotateAngleZ += MathHelper.cos(var3 * 0.09F) * 0.05F + 0.05F;
		this.b.rotateAngleZ -= MathHelper.cos(var3 * 0.09F) * 0.05F + 0.05F;
		this.a.rotateAngleX += MathHelper.sin(var3 * 0.067F) * 0.05F;
		this.b.rotateAngleX -= MathHelper.sin(var3 * 0.067F) * 0.05F;
	}
}
