// TreeDump -- the Java front end's eyes: javac parses and attributes a source tree, and every node of
// every compilation unit is written out as JSON with what javac resolved about it (the type of each
// expression, the declaration each name and call refers to, the overload chosen). Python lowers the
// JSON into the translator's IR (codebreaker/translate/frontends/java/__init__.py).
//
// The walk is generic: for each node it calls the no-argument getters of the com.sun.source.tree
// interfaces the node implements, so every node kind javac knows -- including ones added in later
// Java versions -- is dumped without being named here.
//
//   java TreeDump --out DIR [--cp CLASSPATH] [--sourcepath DIR] [--release N] @files.txt | files...
//
// One DIR/<path of the source>.json per compilation unit, and DIR/_diagnostics.json with javac's
// errors and warnings. Errors do not stop the dump: an unresolved type is written as "<any>".
import com.sun.source.tree.*;
import com.sun.source.util.*;

import javax.lang.model.element.*;
import javax.lang.model.type.*;
import javax.tools.*;
import java.io.*;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.util.*;

public class TreeDump {
	private final Trees trees;
	private final SourcePositions positions;
	private CompilationUnitTree unit;
	private final Map<Class<?>, List<Method>> getters = new HashMap<>();

	TreeDump(JavacTask task) {
		this.trees = Trees.instance(task);
		this.positions = trees.getSourcePositions();
	}

	public static void main(String[] args) throws Exception {
		String out = null, cp = null, sourcepath = null, release = null;
		List<String> files = new ArrayList<>();
		for (int i = 0; i < args.length; i++) {
			String a = args[i];
			if (a.equals("--out")) out = args[++i];
			else if (a.equals("--cp")) cp = args[++i];
			else if (a.equals("--sourcepath")) sourcepath = args[++i];
			else if (a.equals("--release")) release = args[++i];
			else if (a.startsWith("@")) {
				for (String line : Files.readAllLines(Paths.get(a.substring(1)), StandardCharsets.UTF_8))
					if (!line.isBlank()) files.add(line.trim());
			} else files.add(a);
		}
		if (out == null || files.isEmpty()) {
			System.err.println("usage: TreeDump --out DIR [--cp CP] [--sourcepath DIR] [--release N] @files | files...");
			System.exit(2);
		}
		JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
		DiagnosticCollector<JavaFileObject> diags = new DiagnosticCollector<>();
		StandardJavaFileManager fm = compiler.getStandardFileManager(diags, null, StandardCharsets.UTF_8);
		List<String> opts = new ArrayList<>(List.of("-proc:none", "-encoding", "UTF-8", "-Xlint:none", "-nowarn"));
		if (cp != null) { opts.add("-cp"); opts.add(cp); }
		if (sourcepath != null) { opts.add("-sourcepath"); opts.add(sourcepath); }
		if (release != null) { opts.add("--release"); opts.add(release); }
		JavacTask task = (JavacTask) compiler.getTask(null, fm, diags, opts, null, fm.getJavaFileObjectsFromStrings(files));
		Iterable<? extends CompilationUnitTree> units = task.parse();
		task.analyze();
		TreeDump d = new TreeDump(task);
		Path root = Paths.get(out);
		Files.createDirectories(root);
		int n = 0;
		for (CompilationUnitTree cu : units) {
			d.unit = cu;
			StringBuilder sb = new StringBuilder(1 << 16);
			d.node(sb, cu, new TreePath(cu));
			Path src = Paths.get(cu.getSourceFile().toUri());
			Path dest = root.resolve(src.getFileName().toString().replaceAll("\\.java$", "") + ".json");
			String pkg = cu.getPackageName() == null ? "" : cu.getPackageName().toString();
			if (!pkg.isEmpty()) dest = root.resolve(pkg.replace('.', File.separatorChar)).resolve(dest.getFileName());
			Files.createDirectories(dest.getParent());
			Files.writeString(dest, sb, StandardCharsets.UTF_8);
			n++;
		}
		StringBuilder sb = new StringBuilder("[");
		boolean first = true;
		for (Diagnostic<? extends JavaFileObject> dg : diags.getDiagnostics()) {
			if (!first) sb.append(',');
			first = false;
			sb.append("{\"kind\":").append(str(dg.getKind().name()))
			  .append(",\"file\":").append(str(dg.getSource() == null ? "" : dg.getSource().getName()))
			  .append(",\"line\":").append(dg.getLineNumber())
			  .append(",\"message\":").append(str(dg.getMessage(Locale.ROOT))).append('}');
		}
		Files.writeString(root.resolve("_diagnostics.json"), sb.append(']'), StandardCharsets.UTF_8);
		System.out.println("dumped " + n + " compilation units");
	}

	// ---------------------------------------------------------------------------------- the walk
	private void node(StringBuilder sb, Tree t, TreePath path) {
		sb.append("{\"k\":").append(str(t.getKind().name()));
		long start = positions.getStartPosition(unit, t);
		if (start >= 0 && unit.getLineMap() != null) {
			sb.append(",\"l\":").append(unit.getLineMap().getLineNumber(start))
			  .append(",\"c\":").append(unit.getLineMap().getColumnNumber(start));
		}
		attributes(sb, t, path);
		for (Method m : gettersOf(t.getClass())) {
			Object v;
			try {
				v = m.invoke(t);
			} catch (ReflectiveOperationException | RuntimeException e) {
				continue;
			}
			if (v == null) continue;
			String name = property(m.getName());
			if (v instanceof Tree) {
				sb.append(",\"").append(name).append("\":");
				node(sb, (Tree) v, new TreePath(path, (Tree) v));
			} else if (v instanceof List) {
				List<?> list = (List<?>) v;
				if (!list.isEmpty() && !(list.get(0) instanceof Tree)) continue;
				sb.append(",\"").append(name).append("\":[");
				for (int i = 0; i < list.size(); i++) {
					if (i > 0) sb.append(',');
					Tree c = (Tree) list.get(i);
					node(sb, c, new TreePath(path, c));
				}
				sb.append(']');
			} else if (v instanceof Set) {                    // modifier flags
				sb.append(",\"").append(name).append("\":[");
				boolean first = true;
				for (Object o : (Set<?>) v) {
					if (!first) sb.append(',');
					first = false;
					sb.append(str(o.toString()));
				}
				sb.append(']');
			} else if (v instanceof CharSequence || v instanceof Enum) {
				sb.append(",\"").append(name).append("\":").append(str(v.toString()));
			} else if (v instanceof Boolean || v instanceof Integer || v instanceof Long) {
				sb.append(",\"").append(name).append("\":").append(v);
			} else if (v instanceof Character || v instanceof Float || v instanceof Double || v instanceof Short
					|| v instanceof Byte) {
				sb.append(",\"").append(name).append("\":").append(str(String.valueOf(v)));   // literal values
			}
		}
		sb.append('}');
	}

	// what javac resolved: the type of an expression or declaration, and the element a name refers to
	private void attributes(StringBuilder sb, Tree t, TreePath path) {
		if (t instanceof CompilationUnitTree) return;
		TypeMirror tm = null;
		try {
			tm = trees.getTypeMirror(path);
		} catch (RuntimeException ignored) {
		}
		if (tm != null && tm.getKind() != TypeKind.NONE && tm.getKind() != TypeKind.PACKAGE)
			sb.append(",\"$type\":").append(str(tm.toString()));
		if (t instanceof IdentifierTree || t instanceof MemberSelectTree || t instanceof NewClassTree
				|| t instanceof MethodTree || t instanceof VariableTree || t instanceof ClassTree
				|| t instanceof MemberReferenceTree || t instanceof MethodInvocationTree) {
			Element e = null;
			try {
				e = trees.getElement(path);
			} catch (RuntimeException ignored) {
			}
			if (e == null && t instanceof MethodInvocationTree) {
				ExpressionTree sel = ((MethodInvocationTree) t).getMethodSelect();
				try {
					e = trees.getElement(new TreePath(path, sel));
				} catch (RuntimeException ignored) {
				}
			}
			if (e != null) {
				sb.append(",\"$sym\":");
				element(sb, e);
			}
		}
	}

	private static void element(StringBuilder sb, Element e) {
		sb.append("{\"kind\":").append(str(e.getKind().name()))
		  .append(",\"name\":").append(str(e.getSimpleName().toString()));
		Element owner = e.getEnclosingElement();
		if (owner != null) sb.append(",\"owner\":").append(str(qualified(owner)));
		if (e instanceof TypeElement) sb.append(",\"qname\":").append(str(((TypeElement) e).getQualifiedName().toString()));
		Set<javax.lang.model.element.Modifier> mods = e.getModifiers();
		if (mods.contains(javax.lang.model.element.Modifier.STATIC)) sb.append(",\"static\":true");
		if (e instanceof ExecutableElement) {
			ExecutableElement x = (ExecutableElement) e;
			sb.append(",\"ret\":").append(str(x.getReturnType().toString()))
			  .append(",\"params\":[");
			for (int i = 0; i < x.getParameters().size(); i++) {
				if (i > 0) sb.append(',');
				sb.append(str(x.getParameters().get(i).asType().toString()));
			}
			sb.append(']');
			if (x.isVarArgs()) sb.append(",\"varargs\":true");
		} else {
			sb.append(",\"type\":").append(str(e.asType().toString()));
		}
		sb.append('}');
	}

	private static String qualified(Element e) {
		if (e instanceof TypeElement) return ((TypeElement) e).getQualifiedName().toString();
		if (e instanceof PackageElement) return ((PackageElement) e).getQualifiedName().toString();
		Element o = e.getEnclosingElement();
		return (o == null ? "" : qualified(o) + ".") + e.getSimpleName();
	}

	// the no-argument getters of the com.sun.source.tree interfaces a node class implements, in a fixed order
	private List<Method> gettersOf(Class<?> c) {
		List<Method> ms = getters.get(c);
		if (ms != null) return ms;
		Map<String, Method> found = new TreeMap<>();
		Deque<Class<?>> todo = new ArrayDeque<>(Arrays.asList(c.getInterfaces()));
		for (Class<?> k = c.getSuperclass(); k != null; k = k.getSuperclass()) todo.addAll(Arrays.asList(k.getInterfaces()));
		Set<Class<?>> seen = new HashSet<>();
		while (!todo.isEmpty()) {
			Class<?> i = todo.pop();
			if (!seen.add(i)) continue;
			todo.addAll(Arrays.asList(i.getInterfaces()));
			if (!i.getName().startsWith("com.sun.source.tree.")) continue;
			for (Method m : i.getMethods()) {
				if (m.getParameterCount() != 0 || m.getDeclaringClass() == Object.class) continue;
				String n = m.getName();
				if (n.equals("getKind") || n.equals("getLineMap") || n.equals("getSourceFile") || n.equals("hashCode")
						|| n.equals("toString") || n.equals("getClass") || n.equals("getPackage")) continue;
				if (!(n.startsWith("get") || n.startsWith("is"))) continue;
				found.putIfAbsent(n, m);
			}
		}
		ms = new ArrayList<>(found.values());
		getters.put(c, ms);
		return ms;
	}

	private static String property(String getter) {
		String p = getter.startsWith("get") ? getter.substring(3) : getter.substring(2);
		return Character.toLowerCase(p.charAt(0)) + p.substring(1);
	}

	private static String str(String s) {
		StringBuilder b = new StringBuilder(s.length() + 2).append('"');
		for (int i = 0; i < s.length(); i++) {
			char ch = s.charAt(i);
			switch (ch) {
				case '"': b.append("\\\""); break;
				case '\\': b.append("\\\\"); break;
				case '\n': b.append("\\n"); break;
				case '\r': b.append("\\r"); break;
				case '\t': b.append("\\t"); break;
				default:
					if (ch < 0x20) b.append(String.format("\\u%04x", (int) ch));
					else b.append(ch);
			}
		}
		return b.append('"').toString();
	}
}
