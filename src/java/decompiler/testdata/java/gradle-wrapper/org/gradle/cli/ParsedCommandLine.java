package org.gradle.cli;

import java.util.AbstractCollection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

public final class ParsedCommandLine {
	public final HashMap optionsByString;
	public final HashSet presentOptions;
	public final HashSet removedOptions;
	public final ArrayList extraArguments;

	public ParsedCommandLine(HashSet var1) {
		// $FF: Couldn't be decompiled
	}

	public static String quoteAndJoin(AbstractCollection var0) {
		// $FF: Couldn't be decompiled
	}

	public final String toString() {
		ParsedCommandLine var10000 = this;
		ParsedCommandLine var10001 = this;
		String var4 = quoteAndJoin(this.presentOptions);
		String var1 = quoteAndJoin(var10001.extraArguments);
		String var2 = quoteAndJoin(var10000.removedOptions);
		Object[] var3;
		Object[] var5 = var3 = new Object[3];
		var5[0] = var4;
		var5[1] = var1;
		var5[2] = var2;
		return String.format("options: %s, extraArguments: %s, removedOptions: %s", var3);
	}

	public final ParsedCommandLineOption option(String var1) {
		ParsedCommandLineOption var2;
		if((var2 = (ParsedCommandLineOption)this.optionsByString.get(var1)) != null) {
			return var2;
		} else {
			Object[] var3;
			(var3 = new Object[1])[0] = var1;
			throw new IllegalArgumentException(String.format("Option \'%s\' not defined.", var3));
		}
	}
}
