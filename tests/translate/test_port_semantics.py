# -*- coding: utf-8 -*-
"""The old text translator's unit tests (emit/statements: test_emit_statements, test_finally,
test_int_cast, test_labeled_block, test_call_arg_cast, test_java_semantics and the body-shape tests of
test_beta_fixes / test_emit_source), ported to the IR back end. Each behaviour those tests pinned on a
line of emitted text is a method here, written the way a decompiler writes it, converted by
transpile.convert and judged beside the JVM -- what the old tests asserted as a spelling, this asserts
as an outcome. A few spellings that matter on their own (the 360 passes match them, or the runtime has
one helper for them) are still checked on the text."""

import re

import pytest

from codebreaker.java import parse, probe, transpile
from codebreaker.verify import java

SOURCES = {
    'Node.java': '''public class Node {
	public int value;
	public Node next;

	public Node(int var1) {
		this.value = var1;
	}

	public Node add(int var1) {
		this.value += var1;
		return this;
	}

	public boolean same(Object var1) {
		return this == var1;
	}

	public String describe() {
		return "node" + this.value;
	}
}
''',
    'Child.java': '''public class Child extends Node {
	public int extra;

	public Child(int var1) {
		super(var1 * 2);
		this.extra = this.value + 1;
	}

	public String describe() {
		return "child:" + super.describe() + ":" + this.extra;
	}
}
''',
    'Holder.java': '''public class Holder {
	public static int count;
	public static final int[] PRIMES = new int[]{2, 3, 5, 7};
	public static int base = PRIMES[2] * 10;
	public static final int[][] GRID = new int[][]{{1, 2}, {3, 4, 5}};
	public static final Node[] REG = new Node[4];
	public static int[] PER_KIND = new int[Kind.values().length];
	public static double SKEW = 0.5D * (Math.sqrt(3.0D) - 1.0D);
	public static Node FIRST = (Node)(new Child(3));
	public static Node EITHER = (Node)(PRIMES[0] > 1 ? new Child(1) : new Node(2));
	public static String NONE = (String)null;
	public static Class NODE_CLASS = Node.class;
	public static java.util.logging.Logger LOG = java.util.logging.Logger.getLogger("Minecraft");

	static {
		REG[1] = new Node(5);
	}

	public static int twice(int var0) {
		return var0 * 2;
	}

	public static int statics2(int var0) {
		return PER_KIND.length * 1000 + (int)(SKEW * 1000.0D) + FIRST.value + EITHER.value * 10
				+ (NONE == null ? 7 : 0) + (NODE_CLASS == Node.class ? 50 : 0) + (var0 & 1);
	}

	public static int shadow(int count) {
		return count * 3 + Holder.count + base;
	}
}
''',
    'Worker.java': '''public class Worker extends Thread {
	public int runs;

	public void run() {
		++this.runs;
	}
}
''',
    'Vtx.java': '''public class Vtx {
	public Node pos;
	public float u;
	public float v;
	public int kind;

	public Vtx(float var1, float var2, float var3, float var4, float var5) {
		this(new Node((int)(var1 + var2 + var3)), var4, var5);
	}

	public Vtx(Vtx var1, float var2, float var3) {
		this(var1.pos, var2, var3);
		this.kind = 2;
	}

	public Vtx(Node pos, float u, float v) {
		this.pos = pos;
		this.u = u;
		this.v = v;
		this.kind += 1;
	}

	public Vtx(int var1) {
		this((float)var1, 1.0F, 2.0F, 0.5F, 0.25F);
	}

	public int sum() {
		return this.pos.value + (int)(this.u * 100.0F) + (int)(this.v * 10.0F) + this.kind * 1000;
	}
}
''',
    'Gen.java': '''public class Gen {
	public static int total;
	private int seed;

	public Gen(int var1) {
		this.seed = var1;
	}

	public void generate(int var1) {
		total += this.seed * var1 & 1023;
	}

	public static int total(int var0) {
		return total + var0;
	}
}
''',
    'Kind.java': '''public enum Kind {
	LOW(1, "lo"),
	HIGH(5, "hi");

	private final int weight;
	public final String tag;

	private Kind(int var3, String var4) {
		this.weight = var3;
		this.tag = var4;
	}

	public int weight() {
		return this.weight * 10;
	}
}
''',
    'Port.java': '''import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

public class Port {
	public static int access(int var0) {
		Node var1 = new Node(var0);
		var1.next = new Node(var0 + 1);
		var1.next.next = var1;
		return var1.next.value + var1.next.next.value * 3;
	}

	public static int statics(int var0) {
		Holder.count += var0 & 15;
		return Holder.twice(Holder.count) + Holder.PRIMES[var0 & 3] + Holder.GRID[1][2] + Holder.GRID[0].length;
	}

	public static int shadow(int var0) {
		return Holder.shadow(var0 & 255);
	}

	public static float math(float var0) {
		float var1 = (float)Math.sqrt((double)Math.abs(var0));
		float var2 = Math.max(var0, 1.0F) + (float)Math.min(var0, -2.0F);
		int var3 = Math.round(var0);
		return var1 + var2 + (float)var3 + (float)Math.PI + (float)Math.floor((double)var0);
	}

	public static int lists(int var0) {
		ArrayList var1 = new ArrayList();
		List var2 = new ArrayList();
		var1.add(Integer.valueOf(var0));
		var1.add(Integer.valueOf(var0 + 1));
		var2.add("x");
		boolean var3 = var2.isEmpty();
		var2.clear();
		int var4 = ((Integer)var1.get(1)).intValue();
		var1.remove(0);
		return var1.size() * 1000 + var4 + (var3 ? 1 : 0) + (var2.isEmpty() ? 7 : 0);
	}

	public static int maps(String var0) {
		HashMap var1 = new HashMap();
		var1.put(var0, Integer.valueOf(var0.length()));
		var1.put("k", Integer.valueOf(9));
		boolean var2 = var1.containsKey(var0);
		Integer var3 = (Integer)var1.get(var0);
		var1.remove(var0);
		return var3.intValue() * 10 + (var2 ? 1 : 0) + var1.size() * 100 + (var1.get("none") == null ? 5 : 0);
	}

	public static int sets(String var0) {
		HashSet var1 = new HashSet();
		var1.add(var0);
		var1.add("a");
		var1.add(var0);
		return var1.size() + (var1.contains("a") ? 10 : 0) + (var1.contains("zz") ? 100 : 0);
	}

	public static String strings(String var0) {
		if (var0.equals("a")) {
			return "was a";
		}
		char var1 = var0.length() > 0 ? var0.charAt(0) : 63;
		String var2 = var0.length() > 3 ? var0.substring(1, 3) : var0.trim();
		return var2 + var1 + var0.indexOf(46) + var0.toUpperCase() + (var1 > 128 ? "hi" : "lo");
	}

	public static String concat(int var0, char var1) {
		String var2 = "a" + var0 + "b";
		return "(" + var0 + ", " + var1 + ")" + var2 + (var0 > 0 ? "pos" + var0 : (var0 < 0 ? "neg" : "zero"));
	}

	public static String concatFloat(float var0) {
		return "f=" + var0 + ";" + (double)var0 * 2.0;
	}

	public static float floatMod(float var0) {
		return var0 % 4.0F + (var0 % 4.0F == 0.0F ? 1.0F : 0.0F);
	}

	public static int intMod(int var0) {
		int[] var1 = new int[]{1, 2, 3, 4};
		return var1[(var0 & 255) % 4] * 100 + var0 % 7;
	}

	public static int literalLength(int var0) {
		return "abc".length() + var0;
	}

	public static String casts(double var0) {
		int var2 = (int)var0;
		long var3 = (long)(var0 * 1.0E12);
		byte var5 = (byte)var2;
		short var6 = (short)var2;
		char var7 = (char)(var2 & 127);
		int var8 = (int)(float)(var0 * 1.0E30);
		return var2 + "," + var3 + "," + var5 + "," + var6 + "," + (int)var7 + "," + var8 + "," + (long)Math.floor(var0) * 2L;
	}

	public static int unsignedChar(char var0) {
		return (var0 > 128 ? 1 : 0) + var0 * 2;
	}

	public static int arrays(int var0) {
		int[] var1 = new int[4];
		Node[] var2 = new Node[2];
		int[][] var3 = new int[3][4];
		byte[] var4 = new byte[3];
		var1[var0 & 3] = var0;
		var2[1] = new Node(var0);
		var3[1][2] = var0 * 2;
		var4[0] = (byte)var0;
		return var1[var0 & 3] + var2[1].value + var3[1][2] + var3.length * 10 + var3[0].length + var4[0] + (var2[0] == null ? 3 : 0);
	}

	public static int castDeref(int var0) {
		Object var1 = var0 > 5 ? new Child(var0) : new Node(var0);
		int var2 = ((Node)var1).value;
		if (var1 instanceof Child) {
			var2 += ((Child)var1).extra * 100;
		}
		return var2 + (var1 instanceof Node ? 1 : 0) + (var1 instanceof String ? 1000 : 0);
	}

	public static int nullable(int var0) {
		Node var1 = var0 > 0 ? new Node(var0) : null;
		Node var2 = null;
		if (var0 > 100) {
			var2 = var1;
		}
		return (var1 == null ? -1 : var1.value) + (var2 != null ? var2.value : 0);
	}

	public static int unbox(int var0) {
		HashMap var1 = new HashMap();
		var1.put("b", Byte.valueOf((byte)var0));
		var1.put("i", Integer.valueOf(var0));
		var1.put("a", new int[]{var0, 2});
		byte var2 = ((Byte)var1.get("b")).byteValue();
		int var3 = ((Integer)var1.get("i")).intValue();
		int[] var4 = (int[])var1.get("a");
		return var2 + var3 * 2 + var4[1];
	}

	public static int chains(int var0) {
		return new Node(var0).add(3).add(4).value + (new Node(var0)).add(1).value;
	}

	public static String inherited(int var0) {
		Child var1 = new Child(var0);
		Node var2 = var1;
		return var2.describe() + var1.value + var1.same(var2);
	}

	public static int labeled(int var0) {
		int var1 = 0;
		label0:
		for (int var2 = 0; var2 < 5; ++var2) {
			for (int var3 = 0; var3 < 5; ++var3) {
				if (var3 == (var0 & 3)) {
					continue label0;
				}
				if (var2 * var3 > (var0 & 15)) {
					break label0;
				}
				var1 += var2 + var3;
			}
		}
		label1: {
			if (var0 > 3) {
				break label1;
			}
			var1 += 1000;
		}
		return var1;
	}

	public static int nullCatch(int var0) {
		Node var1 = var0 > 0 ? new Node(var0) : null;
		try {
			return var1.value + var1.add(1).value;
		} catch (NullPointerException var2) {
			return -5;
		}
	}

	public static int switches(int var0) {
		int var1 = 0;
		switch (var0 & 7) {
			case 0:
			case 1:
				var1 = 1;
				break;
			case 2: {
				int var2 = var0 * 2;
				var1 = var2;
				break;
			}
			case 3:
				var1 = 3;
			case 4:
				var1 += 4;
				break;
			default:
				var1 = -1;
		}
		return var1;
	}

	public static int iterate(int var0) {
		HashMap var1 = new HashMap();
		for (int var2 = 0; var2 < (var0 & 7); ++var2) {
			var1.put(Integer.valueOf(var2), Integer.valueOf(var2 * var2));
		}
		int var3 = 0;
		Iterator var4 = var1.values().iterator();
		while (var4.hasNext()) {
			var3 += ((Integer)var4.next()).intValue();
		}
		for (Iterator var5 = var1.keySet().iterator(); var5.hasNext(); var3 += ((Integer)var5.next()).intValue() * 100) {
		}
		Iterator var6 = var1.entrySet().iterator();
		while (var6.hasNext()) {
			Entry var7 = (Entry)var6.next();
			var3 += ((Integer)var7.getKey()).intValue() * ((Integer)var7.getValue()).intValue();
		}
		return var3;
	}

	public static int forInit(int var0) {
		int var1 = 0;
		for (Node var2 = new Node(var0 & 255); var2.value < (var0 & 255) + 5; var2 = var2.add(1)) {
			++var1;
		}
		return var1;
	}

	public static int identity(int var0) {
		Node var1 = new Node(var0);
		Object var2 = var1;
		Node var3 = new Node(var0);
		return (var2 == var1 ? 1 : 0) + (var1.same(var2) ? 10 : 0) + (var1.same(var3) ? 100 : 0) + (var3 != var2 ? 1000 : 0);
	}

	public static int shifts(int var0) {
		int var1 = var0 << 2 + 1;
		int var2 = var0 >> 3 ^ var0 >>> 28 & 255;
		long var3 = (long)var0 << 33 >>> 7;
		var1 >>>= 1;
		return var1 + var2 + (int)(var3 >> 20) + (var0 & 255 << 4);
	}

	public static int finallyCounter(int var0) {
		int var1 = 0;
		for (int var2 = 0; var2 < 6; ++var2) {
			try {
				if (var2 == (var0 & 3)) {
					continue;
				}
				if (var2 > (var0 & 7)) {
					break;
				}
				var1 += 10;
			} finally {
				++var1;
			}
		}
		return var1;
	}

	public static int finallyReturn(int var0) {
		int[] var1 = new int[1];
		return early(var0, var1) * 100 + var1[0];
	}

	private static int early(int var0, int[] var1) {
		try {
			if (var0 > 10) {
				return 1;
			}
			return 2;
		} finally {
			var1[0] = 7;
		}
	}

	public static int enums(int var0) {
		Kind var1 = Kind.values()[var0 & 1];
		return var1.weight() + Kind.values().length * 1000 + Kind.HIGH.ordinal() * 100 + var1.tag.length() + (var1 == Kind.LOW ? 7 : 0);
	}

	public static String builder(int var0) {
		StringBuilder var1 = new StringBuilder();
		var1.append(var0).append("-").append('c');
		return var1.toString() + var1.length();
	}

	public static int parse(String var0) {
		try {
			return Integer.parseInt(var0);
		} catch (NumberFormatException var1) {
			return -1;
		}
	}

	public static int bytesOf(String var0) {
		return var0.toCharArray().length * 10 + var0.getBytes().length;
	}

	public static long longs(long var0) {
		long var2 = var0 * 31L + (var0 >>> 3);
		return var2 ^ var2 >> 17;
	}

	public static int compound(int var0) {
		short var1 = (short)var0;
		var1 += 40000;
		byte var2 = (byte)var0;
		var2 *= 3;
		char var3 = 'a';
		var3 += var0 & 7;
		return var1 + var2 + var3;
	}

	public static String latin(String var0) {
		String var1 = "\\u00a7e" + var0 + "\\u00c7";
		return var1.length() + ":" + (var1.charAt(0) == 167 ? "color" : "plain") + var1.indexOf(199) + var1;
	}

	private static String glyphs() {
		return "~\\u2302\\u00c7" + '\\u2302';
	}

	public static int containers(int var0) {
		ArrayList var1 = new ArrayList();
		for (int var2 = 0; var2 < 3; ++var2) {
			var1.add(new Node(var0 + var2));
		}
		((Node)var1.get(1)).add(5);
		int var3 = 0;
		for (int var4 = 0; var4 < var1.size(); ++var4) {
			var3 += ((Node)var1.get(var4)).value;
		}
		return var3;
	}

	public static int library(String var0) {
		return String.valueOf(var0.length()).trim().length() * 100 + Integer.toString(var0.hashCode() & 255).length()
				+ (Character.isDigit(var0.length() > 0 ? var0.charAt(0) : 120) ? 1000 : 0) + var0.toLowerCase().indexOf("o");
	}

	public static float trig(float var0) {
		float var1 = (float)Math.sin((double)var0);
		float var2 = (float)Math.cos((double)var0);
		float sin = (float)Math.sin((double)(var0 * 2.0F));
		return var1 * 3.0F + var2 + sin + (float)Math.floor((double)var0);
	}

	public static int copies(int var0) {
		ArrayList var1 = new ArrayList();
		var1.add(Integer.valueOf(var0));
		var1.add(Integer.valueOf(var0 * 2));
		ArrayList var2 = new ArrayList(var1);
		var2.add(Integer.valueOf(7));
		return var1.size() * 100 + var2.size() * 10 + ((Integer)var2.get(1)).intValue();
	}

	private static int ticks;

	private static boolean touch() {
		++ticks;
		return ticks > 2;
	}

	public static int order(int var0) {
		ticks = 0;
		boolean var1 = (var0 & 3) == 0 && touch();
		touch();
		boolean var2 = (var0 & 1) == 1 || touch();
		if (var0 > 5) {
			touch();
		} else if (touch()) {
			var0 += 100;
		}
		return ticks * 1000 + var0 + (var1 ? 1 : 0) * 10 + (var2 ? 1 : 0);
	}

	private static void spawn() {
		(new Worker()).start();
	}

	public static int vtx(int var0) {
		return (new Vtx(var0 & 255)).sum() + (new Vtx(new Vtx(var0 & 7), 0.5F, 0.75F)).sum() * 3;
	}

	public static String maybe(int var0) {
		if (var0 < 0) {
			return null;
		}
		String var1 = var0 > 100 ? null : "p" + var0;
		return var1 == null ? "big" : var1;
	}

	public static int leak(int var0) {
		(new Gen(var0)).generate(3);
		(new Gen(1)).generate(var0);
		return Gen.total(0);
	}

	public static int registry(int var0) {
		Holder.REG[var0 & 1].add(1);
		return Holder.REG[1].value;
	}

	public static Object reflect(String var0) throws Exception {
		return Class.forName(var0).newInstance();
	}

	public static boolean bools(int var0) {
		boolean var1 = var0 > 3;
		var1 &= var0 < 100;
		var1 |= var0 == -1;
		return var1 ^ (var0 & 1) == 1;
	}
}
''',
}

JUDGED = {'access', 'statics', 'shadow', 'math', 'lists', 'maps', 'sets', 'strings', 'concat', 'concatFloat',
          'floatMod', 'intMod', 'literalLength', 'casts', 'unsignedChar', 'arrays', 'castDeref', 'nullable',
          'unbox', 'chains', 'inherited', 'labeled', 'nullCatch', 'switches', 'iterate', 'forInit', 'identity',
          'shifts', 'finallyCounter', 'finallyReturn', 'enums', 'builder', 'parse', 'bytesOf', 'longs',
          'compound', 'bools', 'leak', 'registry', 'containers', 'library', 'latin', 'trig', 'copies', 'order', 'vtx', 'maybe'}

needs_tools = pytest.mark.skipif(not (java.compiler() and java.jdk()[0]), reason='needs g++ and a JDK')


@pytest.fixture(scope='module')
def port(tmp_path_factory):
    src = tmp_path_factory.mktemp('src')
    for name, text in SOURCES.items():
        (src / name).write_text(text, encoding='utf-8')
    out = tmp_path_factory.mktemp('cpp')
    s = transpile.convert(str(src), str(out), platform=None, write_report=False)
    return src, out, s


def _body(text, name):
    """The C++ definition of Port::<name>."""
    m = re.search(r'\n\w[^\n]*\bPort::%s\(.*?\n}\n' % name, text, re.S)
    assert m, name
    return m.group(0)


def test_every_body_comes_from_the_tree(port):
    _src, _out, s = port
    written, declined = s['tree_bodies']
    assert written >= len(JUDGED) and not declined, (written, declined)


def test_spellings_the_runtime_and_passes_rely_on(port):
    _src, out, _s = port
    text = (out / 'Port.cpp').read_text(encoding='utf-8')
    assert '.is<Child>()' in _body(text, 'castDeref')                   # instanceof on an Object
    assert 'goto ' in _body(text, 'labeled')                            # labelled break/continue
    assert 'javaMathRound(' in _body(text, 'math')                      # Math.round's one shim
    assert 'j2xToInt(' in _body(text, 'casts') and 'j2xToLong(' in _body(text, 'casts')   # saturating casts
    assert 'fmod' in _body(text, 'floatMod')                            # float % is fmod
    assert 'unsigned char' in _body(text, 'strings')                    # Java's char is unsigned
    leak = _body(text, 'leak')                                          # a throwaway heap temporary: on the stack
    assert '{ Gen _j2xleak0(var0); _j2xleak0.generate(3); }' in leak and '_j2xleak1(1)' in leak, leak
    assert 'Gen::total_(0)' in leak                                      # a method named as a field: renamed
    assert 'if (Holder::s_REG[var0 & 1] != NULL) { Holder::s_REG[var0 & 1]->add(1); }' in _body(text, 'registry')
    assert 'J2X-REVIEW[reflection-unsupported]' in _body(text, 'reflect')
    assert 'catch (NumberFormatException*)' in _body(text, 'parse')     # an unused catch variable: unnamed (C4101)
    assert 'label1_brk:' in _body(text, 'labeled') and 'label1_cont' not in _body(text, 'labeled')   # C4102
    assert 'new Worker()' in _body(text, 'spawn') and '_j2xleak' not in _body(text, 'spawn')   # a Thread outlives it
    assert '::sin(' in _body(text, 'trig')                               # a local named sin shadows sin()
    holder = (out / 'Holder.cpp').read_text(encoding='utf-8')
    assert 'static int _Holder_s_PRIMES_data[] = { 2, 3, 5, 7 };' in holder        # new T[]{..}: C++03 data + JArray
    assert 'Holder::s_GRID = (JArrayInit<JArray<int > >()' in holder, holder    # new T[][]{..}: nested builders
    assert 'return JavaString();' in _body(text, 'maybe')                         # null String: empty, not NULL
    vtx = (out / 'Vtx.cpp').read_text(encoding='utf-8')
    assert 'this->pos = pos;' in vtx and 'this->(' not in vtx, vtx               # a param named as its field
    assert 'j2xClassOf(typeid(Node))' in holder and 'Kind::values().length' in holder, holder
    kind = (out / 'Kind.cpp').read_text(encoding='utf-8')               # a typed enum: its table, built
    assert 'LOW = new Kind(1, "lo");' in kind and '_j2x_values[1] = HIGH;' in kind, kind
    glyphs = _body(text, 'glyphs')                                    # a char per byte; past 8 bits,
    assert r'"~?\307"' in glyphs and '8962' in glyphs, glyphs          # ? (Latin-1), and a char as its int
    child = (out / 'Child.cpp').read_text(encoding='utf-8')
    assert 'Node::describe()' in child                                  # super.m() is the qualified call
    assert ': Node(' in child                                           # super(args) is the base initialiser


def test_finally_runs_on_every_way_out(port):
    _src, out, _s = port
    text = (out / 'Port.cpp').read_text(encoding='utf-8')
    body = _body(text, 'early')
    assert 'catch (...)' in body and 'throw;' in body                   # the exceptional path, rethrown


@needs_tools
def test_beside_the_jvm(port, tmp_path):
    src, out, _s = port
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / 'Port.java')), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert set(r['methods']) >= JUDGED, (sorted(JUDGED - set(r['methods'])), r['why'])
    assert r['differs'] == [], r['differs'][:8]


@needs_tools
@pytest.mark.parametrize('cls', ['Node', 'Child', 'Holder'])
def test_helpers_beside_the_jvm(port, tmp_path, cls):
    src, out, _s = port
    staged = probe.stage_sources(str(src), str(tmp_path / 'javasrc'))
    javac, jvm = java.jdk()
    r = probe.run(staged, str(out), parse.parse_file(str(src / ('%s.java' % cls))), str(tmp_path / 'work'),
                  java.compiler(), javac, jvm)
    assert r['methods'] and r['differs'] == [], (r['why'], r['differs'][:6])
