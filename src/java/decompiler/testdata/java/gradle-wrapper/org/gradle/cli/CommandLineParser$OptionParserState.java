package org.gradle.cli;

public abstract class CommandLineParser$OptionParserState {
	public CommandLineParser$OptionParserState() {
	}

	public CommandLineParser$OptionParserState(int var1) {
		this();
	}

	public abstract CommandLineParser$ParserState onStartNextArg();

	public abstract CommandLineParser$ParserState onArgument(String var1);

	public abstract boolean getHasArgument();

	public abstract CommandLineParser$ParserState onComplete();
}
