package sample;

// Synthetic fixture (original). Exercises single inheritance and a super() call.
public class EntityMob extends Entity {
	public int health;

	public EntityMob(World var1) {
		super(var1);
		this.health = 20;
	}

	public void onUpdate() {
		super.onUpdate();
		if (this.health <= 0) {
			this.isDead = true;
		}
	}
}
