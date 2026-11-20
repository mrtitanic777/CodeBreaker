package org.gradle.cli;

import java.util.ArrayList;

public final class CommandLineParser$KnownOptionParserState extends CommandLineParser$OptionParserState {
	public final CommandLineParser$OptionString optionString;
	public final CommandLineOption option;
	public final ParsedCommandLine commandLine;
	public final CommandLineParser$ParserState state;
	public final ArrayList values;

	public CommandLineParser$KnownOptionParserState(CommandLineParser$OptionString var1, CommandLineOption var2, ParsedCommandLine var3, CommandLineParser$ParserState var4) {
		// $FF: Couldn't be decompiled
	}

	public final CommandLineParser$ParserState onArgument(String var1) {
		CommandLineParser$OptionString var2;
		Object[] var3;
		if(this.option.argumentType != Void.TYPE) {
			if(var1.length() != 0) {
				this.values.add(var1);
				return this.onComplete();
			} else {
				var2 = this.optionString;
				(var3 = new Object[1])[0] = var2;
				throw new CommandLineArgumentException(String.format("An empty argument was provided for command-line option \'%s\'.", var3));
			}
		} else {
			var2 = this.optionString;
			(var3 = new Object[1])[0] = var2;
			throw new CommandLineArgumentException(String.format("Command-line option \'%s\' does not take an argument.", var3));
		}
	}

	public final CommandLineParser$ParserState onStartNextArg() {
		return (CommandLineParser$ParserState)(this.option.argumentType != Void.TYPE && this.values.isEmpty() ? new CommandLineParser$MissingOptionArgState(this) : this.onComplete());
	}

	public final boolean getHasArgument() {
		return this.option.argumentType != Void.TYPE;
	}

	public final CommandLineParser$ParserState onComplete() {
		// $FF: Couldn't be decompiled
	}
}
