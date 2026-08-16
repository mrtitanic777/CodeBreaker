package org.mcphackers.mcp.tasks.mode;

import java.util.HashMap;
import java.util.Map;

import org.jetbrains.java.decompiler.main.extern.IFernflowerPreferences;
import org.mcphackers.mcp.MCP;
import org.mcphackers.mcp.Options;
import org.mcphackers.mcp.tasks.Task.Side;

/**
 * Any optional parameters which can be accessed from tasks
 *
 * @see Options#getParameter(TaskParameter)
 */
public enum TaskParameter {

	SIDE("side", Integer.class, Side.ANY.index),
	PATCHES("patch", Boolean.class, true),
	IGNORED_PACKAGES("ignore", String[].class, new String[]{"paulscode", "com/jcraft", "de/jarnbjo", "isom", "ibxm", "de/matthiasmann/twl", "javax/xml", "javax/ws", "argo", "org/bouncycastle", "org/apache/commons", "com/google/gson", "com/google/common"}),
	FERNFLOWER_OPTIONS("ff_options", String.class, getDefaultFFOptions()),
	OBFUSCATION("obf", Boolean.class, false),
	SRG_OBFUSCATION("srgobf", Boolean.class, false),
	FULL_BUILD("fullbuild", Boolean.class, false),
	RUN_BUILD("runbuild", Boolean.class, false),
	RUN_ARGS("runargs", String[].class, new String[]{"-Xms1024M", "-Xmx1024M"}),
	GAME_ARGS("gameargs", String.class, ""),
	SETUP_VERSION("setup", String.class, null),
	SOURCE_VERSION("source", Integer.class, -1),
	TARGET_VERSION("target", Integer.class, -1),
	JAVA_HOME("javahome", String.class, ""),
	JAVAC_ARGS("javacargs", String.class, ""),
	EXCLUDED_CLASSES("excludedclasses", String.class, ""),
	DECOMPILE_RESOURCES("resources", Boolean.class, false),
	GUESS_GENERICS("generics", Boolean.class, false),
	STRIP_GENERICS("stripgenerics", Boolean.class, false),
	OUTPUT_SRC("outputsrc", Boolean.class, true),
	STRIP_SOURCE_FILE("stripsourcefile", Boolean.class, true),
	STRING_REMAP_PACKAGES("stringremap", String[].class, new String[0]);

	public static final TaskParameter[] VALUES = TaskParameter.values();

	public final String name;
	public final Class<?> type;
	public final Object defaultValue;

	<T> TaskParameter(String name, Class<T> c, T value) {
		TaskParameterMap.nameToParamMap.put(name, this);
		this.name = name;
		this.type = c;
		this.defaultValue = value;
	}

	private static String getDefaultFFOptions() {
		// Vineflower (unlike classic fernflower) exposes no IFernflowerPreferences.DEFAULTS map and no
		// NO_COMMENT_OUTPUT option -- it applies its own defaults for anything left unset and does not
		// emit the "$FF:" annotation comments, so we build the option map from scratch.
		Map<String, Object> ffOptions = new HashMap<>();
		ffOptions.put(IFernflowerPreferences.REMOVE_BRIDGE, "0");
		ffOptions.put(IFernflowerPreferences.ASCII_STRING_CHARACTERS, "1");
		ffOptions.put(IFernflowerPreferences.OVERRIDE_ANNOTATION, "0");
		ffOptions.put(IFernflowerPreferences.DECOMPILE_GENERIC_SIGNATURES, "1");
		// Faithful-to-original decompilation: strip compiler-synthesized members (enum $values(),
		// inner-class access$ accessors, generic/covariant bridge methods, enum switch-map arrays)
		// that were never written in the source. Verified recompile-safe -- javac regenerates all of
		// them from the clean source, so this also supersedes the conservative REMOVE_BRIDGE=0 above.
		ffOptions.put(IFernflowerPreferences.REMOVE_SYNTHETIC, "1");
		ffOptions.put(IFernflowerPreferences.INDENT_STRING, "\t");
		ffOptions.remove(IFernflowerPreferences.BANNER);
		return ffOptions.toString();
	}

	public String getDesc() {
		String s = "task.param." + name;
		if (MCP.TRANSLATOR.hasKey(s)) {
			return MCP.TRANSLATOR.translateKey(s);
		}
		return MCP.TRANSLATOR.translateKey("task.noDesc");
	}
}
