package org.mcphackers.mcp.tools.fernflower;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import de.fernflower.main.decompiler.BaseDecompiler;
import de.fernflower.main.decompiler.DirectoryResultSaver;
import de.fernflower.main.extern.IBytecodeProvider;
import de.fernflower.main.extern.IFernflowerPreferences;
import de.fernflower.util.InterpreterUtil;
import org.mcphackers.mcp.MCP;
import org.mcphackers.mcp.tasks.ProgressListener;
import org.mcphackers.mcp.tasks.mode.TaskParameter;

public class Decompiler implements IBytecodeProvider {
	public final DecompileLogger log;
	private final Path source;
	private final List<Path> libraries;
	private final Path destination;
	private final Map<String, Object> mapOptions;
	private final ZipFileCache openZips = new ZipFileCache();

	public Decompiler(ProgressListener listener, Path source, Path out, List<Path> libs, MCP mcp) {
		this.source = source;
		this.libraries = libs;
		this.destination = out;
		this.log = new DecompileLogger(listener);
		this.mapOptions = mcp.getOptions().getFernflowerOptions();
		this.mapOptions.put(IFernflowerPreferences.REMOVE_BRIDGE, mcp.getOptions().getBooleanParameter(TaskParameter.GUESS_GENERICS) ? "1" : "0");
	}

	public void decompile() throws IOException {
		// CodeBreaker starts RetroMCP with -Dcodebreaker.fernflower=<codebreaker.exe>: its own Fernflower (the same text, natively) does this stage.
		final String engine = System.getProperty("codebreaker.fernflower");
		if (engine != null && !engine.isEmpty()) {
			decompileNatively(engine);
			return;
		}
		BaseDecompiler decompiler = new BaseDecompiler(this, new DirectoryResultSaver(destination.toFile()), mapOptions, log/*, javadocs.exists() ? new TinyJavadocProvider(javadocs) : null*/);
		for (Path lib : libraries) {
			if (Files.exists(lib))
				decompiler.addSpace(lib.toAbsolutePath().toFile(), false);
		}
		decompiler.addSpace(source.toAbsolutePath().toFile(), true);
		decompiler.decompileContext();
		this.openZips.close();
	}

	private void decompileNatively(String engine) throws IOException {
		List<String> command = new ArrayList<>();
		command.add(engine);
		command.add("java");
		command.add("fernflower");
		command.add(source.toAbsolutePath().toString());
		command.add(destination.toAbsolutePath().toString());
		for (Path lib : libraries) {
			if (Files.exists(lib)) {
				command.add("--lib");
				command.add(lib.toAbsolutePath().toString());
			}
		}
		for (Map.Entry<String, Object> option : mapOptions.entrySet()) {
			command.add("--option");
			command.add(option.getKey() + "=" + option.getValue());
		}
		Files.createDirectories(destination);
		Process process = new ProcessBuilder(command).redirectErrorStream(true).start();
		process.getOutputStream().close();
		final ArrayDeque<String> said = new ArrayDeque<>();   // its last lines, for a failure
		try (BufferedReader out = new BufferedReader(new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8))) {
			String line;
			while ((line = out.readLine()) != null) {
				said.addLast(line);
				if (said.size() > 20) said.removeFirst();
				final String reading = "INFO:  Decompiling class ";
				if (line.startsWith(reading)) log.startReadingClass(line.substring(reading.length()));
			}
		}
		try {
			final int code = process.waitFor();
			if (code != 0) throw new IOException("CodeBreaker's Fernflower failed (exit " + code + "):\n" + String.join("\n", said));
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
			throw new IOException("interrupted", e);
		}
	}

	@Override
	public byte[] getBytecode(String externalPath, String internalPath) throws IOException {
		if (internalPath == null) {
			File file = new File(externalPath);
			return InterpreterUtil.getBytes(file);
		} else {
			final ZipFile archive = this.openZips.get(externalPath);
			final ZipEntry entry = archive.getEntry(internalPath);
			if (entry == null) {
				throw new IOException("Entry not found: " + internalPath);
			}
			return InterpreterUtil.getBytes(archive, entry);
		}
	}
}
