package net.minecraft.client.model;

public final class ModelSkeleton extends ModelZombie {
	public ModelSkeleton() {
		this.a = new ModelRenderer(40, 16);
		this.a.addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, 0.0F);
		this.a.setRotationPoint(-5.0F, 2.0F, 0.0F);
		this.b = new ModelRenderer(40, 16);
		this.b.mirror = true;
		this.b.addBox(-1.0F, -2.0F, -1.0F, 2, 12, 2, 0.0F);
		this.b.setRotationPoint(5.0F, 2.0F, 0.0F);
		this.c = new ModelRenderer(0, 16);
		this.c.addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, 0.0F);
		this.c.setRotationPoint(-2.0F, 12.0F, 0.0F);
		this.d = new ModelRenderer(0, 16);
		this.d.mirror = true;
		this.d.addBox(-1.0F, 0.0F, -1.0F, 2, 12, 2, 0.0F);
		this.d.setRotationPoint(2.0F, 12.0F, 0.0F);
	}
}
