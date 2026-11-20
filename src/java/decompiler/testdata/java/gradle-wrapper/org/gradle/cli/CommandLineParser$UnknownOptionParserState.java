package org.gradle.cli;

public final class CommandLineParser$UnknownOptionParserState extends CommandLineParser$OptionParserState {
	public final CommandLineParser$ParserState state;
	public final String arg;
	public final ParsedCommandLine commandLine;

	public CommandLineParser$UnknownOptionParserState(String var1, ParsedCommandLine var2, CommandLineParser$ParserState var3) {
		super(0);
		this.arg = var1;
		this.commandLine = var2;
		this.state = var3;
	}

	public final boolean getHasArgument() {
		return true;
	}

	public final CommandLineParser$ParserState onStartNextArg() {
		return this.onComplete();
	}

	public final CommandLineParser$ParserState onArgument(String var1) {
		return this.onComplete();
	}

	public final CommandLineParser$ParserState onComplete() {
		CommandLineParser$UnknownOptionParserState var10000 = this;
		CommandLineParser$UnknownOptionParserState var10001 = this;
		ParsedCommandLine var1 = this.commandLine;
		var1.extraArguments.add(var10001.arg);
		return var10000.state;
	}
}
