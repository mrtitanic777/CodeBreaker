package org.gradle.cli;

import java.util.HashMap;
import java.util.regex.Pattern;

public final class CommandLineParser {
	public static final Pattern OPTION_NAME_PATTERN = Pattern.compile("(\\?|\\p{Alnum}[\\p{Alnum}-_]*)");
	public static final boolean $assertionsDisabled = CommandLineParser.class.desiredAssertionStatus() ^ true;
	public final HashMap optionsByString;
	public boolean allowUnknownOptions;

	public CommandLineParser() {
		// $FF: Couldn't be decompiled
	}

	public final CommandLineOption option(String... var1) {
		// $FF: Couldn't be decompiled
	}
}
