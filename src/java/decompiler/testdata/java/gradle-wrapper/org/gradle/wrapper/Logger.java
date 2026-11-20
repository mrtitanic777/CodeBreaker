package org.gradle.wrapper;

public final class Logger implements Appendable {
	public final boolean quiet;

	public Logger(boolean var1) {
		this.quiet = var1;
	}

	public final void log(String var1) {
		if(!this.quiet) {
			System.out.println(var1);
		}

	}

	public final Appendable append(CharSequence var1) {
		if(!this.quiet) {
			System.out.append(var1);
		}

		return this;
	}

	public final Appendable append(CharSequence var1, int var2, int var3) {
		if(!this.quiet) {
			System.out.append(var1, var2, var3);
		}

		return this;
	}

	public final Appendable append(char var1) {
		if(!this.quiet) {
			System.out.append(var1);
		}

		return this;
	}
}
