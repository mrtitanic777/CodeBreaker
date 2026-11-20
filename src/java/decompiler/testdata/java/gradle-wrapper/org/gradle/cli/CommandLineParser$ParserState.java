package org.gradle.cli;

public abstract class CommandLineParser$ParserState {
	public CommandLineParser$ParserState() {
	}

	public CommandLineParser$ParserState(int var1) {
		this();
	}

	public abstract boolean maybeStartOption(String var1);

	public abstract CommandLineParser$OptionParserState onStartOption(String var1, String var2);

	public abstract CommandLineParser$ParserState onNonOption(String var1);

	public void onCommandLineEnd() {
	}
}
