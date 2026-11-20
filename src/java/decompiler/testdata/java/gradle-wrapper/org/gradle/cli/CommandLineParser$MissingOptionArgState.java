package org.gradle.cli;

public final class CommandLineParser$MissingOptionArgState extends CommandLineParser$ParserState {
	public final CommandLineParser$OptionParserState option;

	public CommandLineParser$MissingOptionArgState(CommandLineParser$OptionParserState var1) {
		super(0);
		this.option = var1;
	}

	public final boolean maybeStartOption(String var1) {
		return var1.matches("(?s)-.+");
	}

	public final CommandLineParser$OptionParserState onStartOption(String var1, String var2) {
		return this.option.onComplete().onStartOption(var1, var2);
	}

	public final CommandLineParser$ParserState onNonOption(String var1) {
		return this.option.onArgument(var1);
	}

	public final void onCommandLineEnd() {
		this.option.onComplete();
	}
}
