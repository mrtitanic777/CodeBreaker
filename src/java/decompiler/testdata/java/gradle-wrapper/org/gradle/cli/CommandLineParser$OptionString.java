package org.gradle.cli;

public final class CommandLineParser$OptionString {
	public final String arg;
	public final String option;

	public CommandLineParser$OptionString(String var1, String var2) {
		this.arg = var1;
		this.option = var2;
	}

	public final String toString() {
		return this.arg.startsWith("--") ? "--" + this.option : "-" + this.option;
	}
}
