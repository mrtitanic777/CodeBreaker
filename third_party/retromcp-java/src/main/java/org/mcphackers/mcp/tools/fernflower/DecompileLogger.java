package org.mcphackers.mcp.tools.fernflower;

import org.jetbrains.java.decompiler.main.extern.IFernflowerLogger;
import org.mcphackers.mcp.MCP;
import org.mcphackers.mcp.tasks.ProgressListener;

public class DecompileLogger extends IFernflowerLogger {

	private final ProgressListener listener;

	public DecompileLogger(ProgressListener listener) {
		this.listener = listener;
	}

	@Override
	public void writeMessage(String message, Severity severity) {
		if (severity.ordinal() >= Severity.WARN.ordinal()) {
//			System.out.println(message);
		}
	}

	@Override
	public void writeMessage(String message, Throwable t) {
	}

	@Override
	public void writeMessage(String message, Severity severity, Throwable t) {
	}

	@Override
	public void startReadingClass(String className) {
		listener.setProgress(MCP.TRANSLATOR.translateKey("task.stage.decompile") + " " + className);
	}

	// NOTE: Vineflower's IFernflowerLogger has no startSave/updateSave (per-percentage save progress)
	// callbacks that lassebq's fernflower fork exposed; progress is reported per-class via
	// startReadingClass above.
}
