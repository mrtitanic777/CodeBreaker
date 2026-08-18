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
		return getDefaultFFOptionsMap().toString();
	}

	/**
	 * The authoritative baseline of decompiler options, defined in code. This is the single source of
	 * truth: it seeds the {@link #FERNFLOWER_OPTIONS} default (serialized) and is used directly as the
	 * baseline in {@link org.mcphackers.mcp.Options#getFernflowerOptions()}, where the user's stored
	 * ff_options string is overlaid on top. Building the map here (rather than only the serialized
	 * default) means newly added options always take effect even when a previously-saved options.cfg
	 * holds an older ff_options snapshot that predates them.
	 */
	public static Map<String, Object> getDefaultFFOptionsMap() {
		// Vineflower (unlike classic fernflower) exposes no IFernflowerPreferences.DEFAULTS map and no
		// NO_COMMENT_OUTPUT option -- it applies its own defaults for anything left unset and does not
		// emit the "$FF:" annotation comments, so we build the option map from scratch.
		Map<String, Object> ffOptions = new HashMap<>();
		ffOptions.put(IFernflowerPreferences.ASCII_STRING_CHARACTERS, "1");
		ffOptions.put(IFernflowerPreferences.OVERRIDE_ANNOTATION, "0");
		ffOptions.put(IFernflowerPreferences.DECOMPILE_GENERIC_SIGNATURES, "1");
		// Explicitly DISABLE REMOVE_SYNTHETIC (Vineflower defaults it to ON, so we must set "0", not
		// just omit it). It was tried for faithfulness (to strip enum $values(), access$ accessors,
		// bridge methods, enum switch-map arrays), but Vineflower's implementation removes the
		// synthetic member *declarations* while leaving the code that still *references* them --
		// synthetic outer-class fields (e.g. the captured "this$0"/mcApplet) and enum switch-map arrays
		// (e.g. $SwitchMap$.../osValues). javac does NOT regenerate those from the decompiled source
		// (it only would if the source were written with the corresponding high-level construct, but
		// the decompiler emits the desugared references), so leaving it on produced ~28 extra
		// "cannot find symbol" errors on Indev 20100213 and breaks recompilation.
		ffOptions.put(IFernflowerPreferences.REMOVE_SYNTHETIC, "0");
		// Ignore the MethodParameters attribute when naming parameters. RDInjector's remapper
		// (applyMappings) writes a MethodParameters attribute on every method, and when a mapping
		// supplies no parameter name it synthesises one from the type. For array parameters that
		// fallback uses Type.getInternalName(), which is the descriptor form, so illegal characters
		// leak into the identifier (e.g. "[B1" for byte[], "positionTextureVertex;1" for an object
		// array). Vineflower only sanitizes the references in the method body (rewriting them to varN
		// with a "$VF was:" comment) while leaving the illegal name in the parameter declaration,
		// producing source that neither recompiles nor patches. This broke every version carrying an
		// array-typed parameter (e.g. Indev 20100213). Discarding these names also keeps decompiled
		// output on the varN convention the patch set is written against, so we ignore the attribute
		// and let Vineflower fall back to consistent, always-legal varN naming.
		ffOptions.put(IFernflowerPreferences.USE_METHOD_PARAMETERS, "0");
		// Fold `cond ? X : false` / `cond ? true : X` back into the `&&`/`||` chains the code was
		// actually written with (verified on the Alpha corpus: matches the original, still recompiles).
		ffOptions.put(IFernflowerPreferences.TERNARY_CONSTANT_SIMPLIFICATION, "1");
		ffOptions.put(IFernflowerPreferences.INDENT_STRING, "\t");
		ffOptions.remove(IFernflowerPreferences.BANNER);
		return ffOptions;
	}

	public String getDesc() {
		String s = "task.param." + name;
		if (MCP.TRANSLATOR.hasKey(s)) {
			return MCP.TRANSLATOR.translateKey(s);
		}
		return MCP.TRANSLATOR.translateKey("task.noDesc");
	}
}
