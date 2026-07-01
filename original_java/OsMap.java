package net.minecraft.src;

class OsMap {
	static final int[] osTypeOrdinals = new int[EnumOS1.values().length];

	static {
		try {
			osTypeOrdinals[EnumOS1.linux.ordinal()] = 1;
		} catch (NoSuchFieldError var4) {
		}

		try {
			osTypeOrdinals[EnumOS1.solaris.ordinal()] = 2;
		} catch (NoSuchFieldError var3) {
		}

		try {
			osTypeOrdinals[EnumOS1.windows.ordinal()] = 3;
		} catch (NoSuchFieldError var2) {
		}

		try {
			osTypeOrdinals[EnumOS1.macos.ordinal()] = 4;
		} catch (NoSuchFieldError var1) {
		}

	}
}
