package net.minecraft.src;

public class MovementInputFromOptions extends MovementInput {
	private boolean[] movementKeyState = new boolean[10];
	private GameSettings gameSettings;

	public MovementInputFromOptions(GameSettings var1) {
		this.gameSettings = var1;
	}

	public void setKeyState(int var1, boolean var2) {
		byte var3 = -1;
		if(var1 == this.gameSettings.keyBindForward.keyCode) {
			var3 = 0;
		}

		if(var1 == this.gameSettings.keyBindBack.keyCode) {
			var3 = 1;
		}

		if(var1 == this.gameSettings.keyBindLeft.keyCode) {
			var3 = 2;
		}

		if(var1 == this.gameSettings.keyBindRight.keyCode) {
			var3 = 3;
		}

		if(var1 == this.gameSettings.keyBindJump.keyCode) {
			var3 = 4;
		}

		if(var1 == this.gameSettings.keyBindSneak.keyCode) {
			var3 = 5;
		}

		if(var3 >= 0) {
			this.movementKeyState[var3] = var2;
		}

	}

	public void resetKeyState() {
		for(int var1 = 0; var1 < 10; ++var1) {
			this.movementKeyState[var1] = false;
		}

	}

	public void updatePlayerMoveState(EntityPlayer var1) {
		this.moveStrafe = 0.0F;
		this.moveForward = 0.0F;
		if(this.movementKeyState[0]) {
			++this.moveForward;
		}

		if(this.movementKeyState[1]) {
			--this.moveForward;
		}

		if(this.movementKeyState[2]) {
			++this.moveStrafe;
		}

		if(this.movementKeyState[3]) {
			--this.moveStrafe;
		}

		this.jump = this.movementKeyState[4];
		this.sneak = this.movementKeyState[5];
		if(this.sneak) {
			this.moveStrafe = (float)((double)this.moveStrafe * 0.3D);
			this.moveForward = (float)((double)this.moveForward * 0.3D);
		}

	}
}
