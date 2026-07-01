package net.minecraft.src;

class LogoEffectRandomizer {
	public double posY;
	public double prevPosY;
	public double motionY;
	final GuiMainMenu mainMenu;

	public LogoEffectRandomizer(GuiMainMenu var1, int var2, int var3) {
		this.mainMenu = var1;
		this.posY = this.prevPosY = (double)(10 + var3) + GuiMainMenu.getRand().nextDouble() * 32.0D + (double)var2;
	}

	public void updatePosition() {
		this.prevPosY = this.posY;
		if(this.posY > 0.0D) {
			this.motionY -= 0.6D;
		}

		this.posY += this.motionY;
		this.motionY *= 0.9D;
		if(this.posY < 0.0D) {
			this.posY = 0.0D;
			this.motionY = 0.0D;
		}

	}
}
