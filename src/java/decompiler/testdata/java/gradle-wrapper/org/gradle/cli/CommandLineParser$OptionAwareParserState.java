package org.gradle.cli;

import java.util.regex.Pattern;

public abstract class CommandLineParser$OptionAwareParserState extends CommandLineParser$ParserState {
	public final ParsedCommandLine commandLine;
	public final CommandLineParser this$0;

	public CommandLineParser$OptionAwareParserState(CommandLineParser var1, ParsedCommandLine var2) {
		super(0);
		this.this$0 = var1;
		this.commandLine = var2;
	}

	public final boolean maybeStartOption(String var1) {
		return var1.matches("(?s)-.+");
	}

	public final CommandLineParser$ParserState onNonOption(String var1) {
		this.commandLine.extraArguments.add(var1);
		Pattern var10001 = CommandLineParser.OPTION_NAME_PATTERN;
		this.this$0.getClass();
		return new CommandLineParser$AfterOptions(this.commandLine);
	}
}
