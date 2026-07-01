package net.minecraft.src;

public class StepSound {
	public final String stepSoundName;
	public final float volume;
	public final float pitch;

	public StepSound(String var1, float var2, float var3) {
		this.stepSoundName = var1;
		this.volume = var2;
		this.pitch = var3;
	}

	public float getVolume() {
		return this.volume;
	}

	public float getPitch() {
		return this.pitch;
	}

	public String getBreakSound() {
		return "step." + this.stepSoundName;
	}

	public String getStepSound() {
		return "step." + this.stepSoundName;
	}
}
