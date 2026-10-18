package sample;

// Synthetic fixture (original). A base class with a mix of primitive and reference fields and a
// couple of virtual-looking methods.
public class Entity {
	public double posX;
	public double posY;
	public double posZ;
	public boolean isDead;
	public World worldObj;

	public Entity(World var1) {
		this.worldObj = var1;
		this.isDead = false;
	}

	public void setPosition(double var1, double var3, double var5) {
		this.posX = var1;
		this.posY = var3;
		this.posZ = var5;
	}

	public void onUpdate() {
		this.posY = this.posY - 0.1D;
	}
}
