// Tests of what the Java transpiler EMITS, of the whole-program passes of the corpus, and of the platform layer's C++ sources:
//
//   call-argument casts, constructor delegation, int-array unboxing, equals() identity, pointer factories, int casts
//       Java source in, the transpiler's C++ out, as the application assembles it (java/assembly) with the real front end: javac's typed tree of
//       the sources (TreeDump). They need a JDK; without one they say so and pass.
//   host-compile fixes   the same, then the emitted classes and a runtime unit compiled with the host g++ (skipped without one)
//   collection out-parameters, build-3 and beta fixes   the whole-program passes on parsed classes
//   platform layer review   the platform layer's C++ (JavaString.h, JavaRT.h, Save360.reference.cpp, GL11.reference.cpp): read, and compiled by g++
//   arrays assigned null and the PC platform are in the last tests.
//   the C++17 back end (2): Modern.java as a whole C++17 program run beside the JVM, and Tally.java through the transpiler as C++17 beside C++03,
//       compiled with -fsyntax-only (the rest is translate.cpp17.* / translate.jmodel.* / translate.desugar.*). They need a JDK and g++;
//       the first also cbl2.dll (native/l2/build, or CB_L2_DLL).
//
// A conversion reads the profile it asks for (none by default); no profile is active between tests.
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <regex>
#include <set>

#include "cb/base/config.hpp"
#include "cb/core/arena.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/jar.hpp"
#include "cb/java/assembly.hpp"
#include "cb/java/corpus.hpp"
#include "cb/java/emit/ownership.hpp"
#include "cb/java/frontend.hpp"
#include "cb/java/init_driver.hpp"
#include "cb/java/parse.hpp"
#include "cb/java/probe.hpp"
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
#include "cb/translate/backends/cpp17.hpp"
#include "cb/translate/native_l2.hpp"
#include "cb/verify/lanes/java.hpp"

namespace stdfs = std::filesystem;
using namespace cb;
namespace jcor = cb::java::corpus;
namespace jir = cb::java::ir;

namespace {

// ---- the real front end and the assembly --------------------------------------------------------------------------------------------------------

struct JpoFront {
    bool ready = false;
    java::frontend::DumpTools tools;
    std::unique_ptr<fs::ScopedTempDir> state;
    std::unique_ptr<java::assembly::Assembly> assembly;
};

JpoFront& jpo_front() {
    static JpoFront front = [] {
        JpoFront f;
        const std::optional<stdfs::path> java = decompile::jar::find_java({});
        if (!java) return f;
        stdfs::path javac = java->parent_path() / "javac.exe";
        std::error_code ec;
        if (!stdfs::is_regular_file(javac, ec)) return f;
        f.state = std::make_unique<fs::ScopedTempDir>("cb-java-ports");
        f.tools.java = *java;
        f.tools.javac = javac;
        f.tools.treedump_source = testing::data_path("../../data/translate/frontends/java/TreeDump.java");
        f.tools.state = f.state->path();
        JpoFront* self = &f;
        f.assembly = std::make_unique<java::assembly::Assembly>(
            java::JavaData(testing::data_path("../../data/java")),
            [self](Arena& arena, const stdfs::path& dir) { return java::frontend::lower_program(arena, dir, self->tools); });
        f.ready = true;
        return f;
    }();
    return front;
}

bool jpo_ready(const char* test) {
    if (jpo_front().ready) return true;
    std::cerr << test << ": no JDK found; skipped\n";
    return false;
}

// The Java files converted for no platform (C++03, no report): {class name: its .cpp text}, and the output folder kept for the caller.
struct JpoConverted {
    std::map<std::string, std::string> cpp;
    std::unique_ptr<fs::ScopedTempDir> out;
    std::unique_ptr<fs::ScopedTempDir> src;   // the Java files, kept for a run beside the JVM
};

JpoConverted jpo_convert(const std::map<std::string, std::string>& files, const std::string& edition = "", const std::string& platform = "",
                        const std::string& cpp_std = "C++03") {
    JpoConverted done;
    done.src = std::make_unique<fs::ScopedTempDir>("cb-java-src");
    const fs::ScopedTempDir& src = *done.src;
    for (const auto& [name, text] : files) fs::write_text(src.path() / name, text);
    done.out = std::make_unique<fs::ScopedTempDir>("cb-java-out");
    java::transpile::Options options;
    options.write_report = false;
    options.emit_debug = false;
    options.edition = edition;
    options.platform = platform;
    options.cpp_std = cpp_std;
    jpo_front().assembly->convert(src.path(), done.out->path(), options);
    for (const auto& [name, text] : files) {
        if (!name.ends_with(".java")) continue;
        const stdfs::path cpp = done.out->path() / (name.substr(0, name.size() - 5) + ".cpp");
        std::error_code ec;
        if (stdfs::is_regular_file(cpp, ec)) done.cpp[name.substr(0, name.size() - 5)] = fs::read_text(cpp);
    }
    return done;
}

bool jpo_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

// A host tool that runs ("g++").
bool jpo_tool_runs(const char* tool, const char* flag) {
    try {
        return process::run({.argv = {tool, flag}}).ok();
    } catch (const ToolError&) {
        return false;
    }
}

// g++ over the files, `flags` first; the compiler's text on failure.
std::string jpo_gxx(const std::vector<std::string>& args, const stdfs::path& cwd) {
    process::ProcessSpec spec;
    spec.argv = {"g++"};
    spec.argv.insert(spec.argv.end(), args.begin(), args.end());
    spec.cwd = cwd;
    spec.timeout = std::chrono::seconds(600);
    const process::ProcessResult r = process::run(spec);
    return r.ok() ? std::string() : std::string(r.err.begin(), r.err.end()) + std::string(r.out.begin(), r.out.end());
}

std::string jpo_run(const stdfs::path& exe, const stdfs::path& cwd, int* code = nullptr) {
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(exe)};
    spec.cwd = cwd;
    spec.timeout = std::chrono::seconds(120);
    const process::ProcessResult r = process::run(spec);
    if (code) *code = static_cast<int>(r.exit_code);
    return std::string(r.out.begin(), r.out.end());
}

}  // namespace

// ================================================================================================================================================
// `entityMap.a((Entity)null, var1).size()` -- a nullable getter of another class must not type `entityMap.a()`, and a cast
// that is a call's argument is not a grouping cast: the call's result stays a value (`.size()`, not `->size()`).
CB_TEST("java.emitted.a_cast_argument_is_not_a_grouping_cast_and_a_list_result_keeps_its_dot", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({
        {"Entity.java", "public class Entity { public int id; }"},
        {"AABB.java", "public class AABB { public double x; }"},
        {"EntityMap.java", "import java.util.List;\nimport java.util.ArrayList;\n"
                           "public class EntityMap { public List es = new ArrayList();\n"
                           " public final List a(Entity e, AABB bb) { return this.es; } }"},
        {"World.java", "public class World { public EntityMap entityMap;\n"
                       " public boolean f(AABB var1) { return this.entityMap.a((Entity)null, var1).size() == 0; } }"},
        // obfuscated collision: Pathfinder.a returns a nullable pointer -> must NOT type EntityMap.a
        {"Pathfinder.java", "public class Pathfinder { public PathPoint a(int x){ if(x<0) return null; return new PathPoint(); } }"},
        {"PathPoint.java", "public class PathPoint { public int idx; }"},
    });
    const std::string& cpp = done.cpp.at("World");
    CB_CHECK(jpo_has(cpp, "a((Entity*)NULL, var1).size()"));   // '.' not '->', and no PathPoint* hoist
    CB_CHECK(!jpo_has(cpp, "->size()"));
    CB_CHECK(!jpo_has(cpp, "PathPoint"));                       // the mis-typed hoist is gone
});

// a transitive `this((Block)var1, 1)` delegation threads the argument through the X-typed overload.
CB_TEST("java.emitted.a_transitive_delegation_with_a_cast_argument_threads_blockid", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({
        {"ItemStack.java", "public class ItemStack {\n   public int itemID;\n   public int stackSize;\n"
                           "   public ItemStack(Block var1) { this((Block)var1, 1); }\n"
                           "   public ItemStack(Block var1, int var2) { this(var1.blockID, var2); }\n"
                           "   public ItemStack(int var1, int var2) { this.itemID = var1; this.stackSize = var2; }\n}"},
        {"Block.java", "public class Block { public int blockID; }"},
    });
    const std::string& cpp = done.cpp.at("ItemStack");
    CB_CHECK(jpo_has(cpp, "->blockID"));                                    // the Block constructor inlines the chain: itemID gets ((Block*)var1)->blockID
    CB_CHECK(!jpo_has(cpp, "this->itemID = ((Block*)var1);"));              // the Block*->int assignment is gone
});

// `(int[])list.remove(i)` of a raw list unboxes the JObject element.
CB_TEST("java.emitted.an_int_array_cast_of_a_raw_list_remove_unboxes", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({{"Holder.java", "import java.util.List;\nimport java.util.ArrayList;\n"
                                                           "public class Holder { public List q = new ArrayList();\n"
                                                           " public int[] pop() { return (int[])this.q.remove(this.q.size() - 1); } }"}});
    const std::string& cpp = done.cpp.at("Holder");
    CB_CHECK(jpo_has(cpp, "(JArray<int >)(this->q.remove(") || jpo_has(cpp, "(JArray<int >)this->q.remove("));
    CB_CHECK(!jpo_has(cpp, "(int[])"));
});

CB_TEST("java.emitted.an_int_array_cast_of_a_field_chain_unboxes", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({
        {"Holder.java", "import java.util.List;\nimport java.util.ArrayList;\npublic class Holder { public List q = new ArrayList(); }"},
        {"User.java", "public class User { public int[] f(Holder h) {\n  return (int[])h.q.remove(h.q.size() - 1); } }"},
    });
    const std::string& cpp = done.cpp.at("User");
    CB_CHECK(jpo_has(cpp, "(JArray<int >)(h->q.remove(") || jpo_has(cpp, "(JArray<int >)h->q.remove("));
    CB_CHECK(!jpo_has(cpp, "(int[])"));
});

// `.equals()` on a class that declares no equals() is reference identity; one that does keeps its call.
CB_TEST("java.emitted.equals_on_a_class_without_one_is_identity", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({
        {"Slot.java", "public class Slot { public int x; }"},
        {"M.java", "public class M { public boolean f(Slot a, Slot b) { return a.equals(b); } }"},
    });
    const std::string& cpp = done.cpp.at("M");
    CB_CHECK(jpo_has(cpp, "(a == b)"));
    CB_CHECK(!jpo_has(cpp, "->equals(") && !jpo_has(cpp, ".equals("));
});

CB_TEST("java.emitted.a_class_with_equals_keeps_the_call", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({
        {"Slot2.java", "public class Slot2 { public boolean equals(Slot2 o) { return true; } }"},
        {"N.java", "public class N { public boolean f(Slot2 a, Slot2 b) { return a.equals(b); } }"},
    });
    const std::string& cpp = done.cpp.at("N");
    CB_CHECK(jpo_has(cpp, "a->equals(b)"));   // a real equals() is preserved, not turned into identity
    CB_CHECK(!jpo_has(cpp, "(a == b)"));
});

// a stub class's static factory (`Runtime.getRuntime()`) gets `->` on its result.
CB_TEST("java.emitted.runtime_getruntime_chains_with_an_arrow", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({{"Gui.java", "package net.minecraft.client.gui;\npublic class Gui {\n public void draw() {\n"
                                                        "  long a = Runtime.getRuntime().maxMemory();\n  long b = Runtime.getRuntime().totalMemory();\n"
                                                        "  long c = Runtime.getRuntime().freeMemory();\n }\n}\n"}},
                                          "indev");
    const std::string& cpp = done.cpp.at("Gui");
    CB_CHECK(jpo_has(cpp, "Runtime::getRuntime()->maxMemory()"));
    CB_CHECK(jpo_has(cpp, "Runtime::getRuntime()->totalMemory()"));
    CB_CHECK(jpo_has(cpp, "Runtime::getRuntime()->freeMemory()"));
    CB_CHECK(!jpo_has(cpp, "getRuntime().maxMemory"));   // the `.` form is gone
});

// Java's (int)/(long) of a float or double saturates (NaN -> 0), so every such cast of an emitted class is j2xToInt / j2xToLong.
// The emitted class is compiled and run, and the saturated values are Java's.
CB_TEST("java.emitted.out_of_range_casts_saturate_like_java", [] {
    if (!jpo_ready("emitted-cpp") || !jpo_tool_runs("g++", "--version")) return;
    const JpoConverted done = jpo_convert({{"Casts.java", "public class Casts {\n\tpublic static int toInt(float var0) {\n\t\treturn (int)(var0 * 10430.378F);\n\t}\n\n"
                                                           "\tpublic static int toIntD(double var0) {\n\t\treturn (int)(var0 * 1.0E10D);\n\t}\n\n"
                                                           "\tpublic static long toLong(double var0) {\n\t\treturn (long)(var0 * 1.0E19D);\n\t}\n}\n"}});
    CB_CHECK(jpo_has(done.cpp.at("Casts"), "j2xToInt("));
    const stdfs::path dir = done.out->path();
    fs::write_text(dir / "main_casts.cpp",
                   "#include \"stdafx.h\"\n#include \"JavaRT.h\"\n#include \"Casts.h\"\n#include <cstdio>\n#include <limits>\n"
                   "int main() {\n"
                   "  std::printf(\"%d %d %lld %d\\n\", Casts::toInt(1000000.0f), Casts::toIntD(-1000.75), (long long)Casts::toLong(1.0),\n"
                   "              Casts::toInt(std::numeric_limits<float>::quiet_NaN()));\n  return 0;\n}\n");
    const std::string failed = jpo_gxx({"-std=c++03", "-w", "-iquote", fs::to_text(dir), "main_casts.cpp", "Casts.cpp", "-o", "casts.exe"}, dir);
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) return;
    // Java: (int)1.0430378E10F = Integer.MAX_VALUE; (int)-1.00075E13 = Integer.MIN_VALUE; (long)1.0E19 = Long.MAX_VALUE; (int)NaN = 0
    CB_CHECK_EQ(text::trim(jpo_run(dir / "casts.exe", dir)), "2147483647 -2147483648 9223372036854775807 0");
});

// ================================================================================================================================================
// the classes of the Alpha corpus that did not compile with a host g++, transpiled and compiled.
namespace {

const std::map<std::string, std::string>& jpo_host_sources() {
    static const std::map<std::string, std::string> sources = {
        {"Session.java", "public class Session {\n\tpublic String inventory;\n}\n"},
        {"InventoryPlayer.java", "public class InventoryPlayer {\n\tpublic int size;\n}\n"},
        {"EntityPlayer.java", "public class EntityPlayer {\n\tpublic InventoryPlayer inventory;\n}\n"},
        {"Probe.java", R"(public class Probe extends EntityPlayer {
	String[] logo = new String[]{"ab", "cd"};
	Probe[][] grid;
	Object value;
	Probe[] monsters = new Probe[]{null};
	byte[] bytes;
	public Probe(Session var3) {
		if(var3 != null && var3.inventory != null && var3.inventory.length() > 0) {
		}
		this.grid = new Probe[this.logo[0].length()][this.logo.length];
	}
	public String name(boolean var1, int var2) {
		return var1 ? "World" + var2 : null;
	}
	public Object get(Probe var2) {
		return var2 == null ? null : var2.value;
	}
	public Probe[] pick(int var1) {
		return var1 == 0 ? this.monsters : (var1 == 1 ? this.monsters : null);
	}
	public byte[] data(boolean var1) {
		return var1 ? new byte[0] : this.bytes;
	}
}
)"}};
    return sources;
}

// JObject/JavaString/JList/URL/Collections uses the Alpha classes made, compiled against the runtime
constexpr const char* jpo_runtime_unit = R"(#include "stdafx.h"
#include "JavaRT.h"
#include "Collections.h"
#include "URL.h"
#include "DecimalFormat.h"
int main() {
	JList<JObject> list;
	std::size_t n = 3;
	bool has = list.contains(n);                                  // JObject(size_t) -- was ambiguous
	JObject boxed(JavaString("x"));
	JavaString s = (JavaString)boxed;                             // (String)o -- was ambiguous
	JavaString t = boxed;
	JList<JObject> back = (JList<JObject>)JObject(list);          // (List)o -- was ambiguous
	JList<JObject>& sync = Collections::synchronizedList(list);
	JList<JObject> fresh = Collections::synchronizedList(JList<JObject>());   // a temporary
	URL* a = new URL("http://example/");                          // was ambiguous
	URL* b = new URL((JCat() + "http://" + 1));                   // was ambiguous
	std::string viaBase = (JCat() + "x");                         // JCat still reaches std::string
	JMap<JObject, JObject> map;
	JMap<JObject, JObject> mapBack = (JMap<JObject, JObject>)JObject(map);   // (Map)o -- was ambiguous (Beta)
	JArray<int> ints(3);
	JArray<int> intsBack = (JArray<int>)JObject(ints);            // (int[])o -- was ambiguous (Beta)
	DecimalFormat* df = new DecimalFormat("########0.00");        // was ambiguous (Beta StatBase)
	JArray<unsigned char> chars(2); chars[0] = 'h'; chars[1] = 'i';   // char[] (Java char is unsigned)
	if (String::valueOf(chars) != "hi" || String::valueOf(chars, 1, 1) != "i") return 2;   // char[] (Beta)
	if (max(3, 120) != 120 || max(2, 2.5f) != 2.5f || min(7LL, 3) != 3LL) return 3;       // Math.max/min
	if (intsBack.length != 3) return 4;
	(void)has; (void)t; (void)back; (void)sync; (void)fresh; (void)viaBase; (void)mapBack;
	delete df;
	delete a; delete b;
	return s == "x" ? 0 : 1;
}
)";

}  // namespace

CB_TEST("java.emitted.host_compile_fixes_text", [] {
    if (!jpo_ready("host-compile")) return;
    const JpoConverted done = jpo_convert(jpo_host_sources());
    const std::string& probe = done.cpp.at("Probe");
    // a member reached through a receiver is not the scope name: Session.inventory is a String; the inherited EntityPlayer.inventory (a pointer) must not decide it
    CB_CHECK(jpo_has(probe, "var3->inventory.length()"));
    // sized new with an indexed size
    CB_CHECK(jpo_has(probe, "j2x_newarr2<Probe* >(this->logo[0].length(), this->logo.length)"));
    CB_CHECK(jpo_has(probe, "JArray<unsigned char >(0) : this->bytes"));   // the text after ']' is kept
    // the null arm of a value return is the value type's own null, whatever grouping the writer chose
    CB_CHECK(jpo_has(probe, ": JavaString();") || jpo_has(probe, ": JavaString());"));
    CB_CHECK(jpo_has(probe, "? JObject() :"));
    CB_CHECK(jpo_has(probe, ": JArray<Probe* >())"));
    // the no-platform tree has what its runtime includes
    const stdfs::path out = done.out->path();
    CB_CHECK(stdfs::is_regular_file(out / "Inflate.h") && stdfs::is_regular_file(out / "Inflate.cpp"));
    const std::string stdafx = fs::read_text(out / "stdafx.h");
    CB_CHECK(jpo_has(stdafx, "#define NOGDI") && jpo_has(stdafx, "#include <windows.h>"));   // no GDI Rectangle()
    CB_CHECK(jpo_has(fs::read_text(out / "GL11.h"), "GL_LINEAR_MIPMAP_LINEAR = 0x2703"));
});

CB_TEST("java.emitted.every_class_and_the_runtime_compile", [] {
    if (!jpo_ready("host-compile") || !jpo_tool_runs("g++", "--version")) return;
    const JpoConverted done = jpo_convert(jpo_host_sources());
    const stdfs::path out = done.out->path();
    fs::write_text(out / "runtime_unit.cpp", jpo_runtime_unit);
    for (const char* unit : {"Probe.cpp", "Inflate.cpp", "runtime_unit.cpp"}) {
        const verify::lanes::JavaLane::Check check = verify::lanes::JavaLane::check("g++", out, out / unit);
        CB_CHECK(check.ok);
        if (!check.ok) std::cerr << unit << ": " << (check.errors.empty() ? "" : check.errors.front()) << "\n";
    }
    const std::string failed = jpo_gxx({"-std=c++03", "-w", "-iquote", fs::to_text(out), "runtime_unit.cpp", "-o", "runtime_unit.exe"}, out);
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) return;
    int code = -1;
    jpo_run(out / "runtime_unit.exe", out, &code);
    CB_CHECK_EQ(code, 0);   // (String)o gives the boxed string back
});

// ================================================================================================================================================
// a static method that shadows an inherited instance one is renamed; arrays assigned null become empty arrays.
// Through the real translator: `static int[][] x = new int[][]{{..},{..}}` is a
// nested JArray builder, never a braced `new int[][]` (the C2087 "missing subscript" crash class).
CB_TEST("java.emitted.a_nested_static_aggregate_array_is_not_a_braced_new", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({{"ModelBed.java", "public class ModelBed {\n public static final int[][] bedDirection = new int[][]{{1, 0}, {3, 2}};\n}\n"}});
    const std::string& cpp = done.cpp.at("ModelBed");
    CB_CHECK(jpo_has(cpp, "s_bedDirection"));
    CB_CHECK(!jpo_has(cpp, "new int[]"));
});

CB_TEST("java.corpus.a_static_shadowing_an_inherited_instance_method_is_renamed", [] {
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("public class Base { boolean flow(int x) { return false; } }"));
    classes.push_back(java::parse::parse_source("public class Sub extends Base { static boolean flow(int x) { return true; } }"));
    const auto renames = jcor::build_method_renames(classes);
    CB_CHECK(renames.contains("Sub") && renames.at("Sub").contains("flow"));          // the static one is renamed (flow -> flow_)
    CB_CHECK(!renames.contains("Base") || !renames.at("Base").contains("flow"));      // the base's virtual is untouched
});

CB_TEST("java.corpus.a_static_with_nothing_to_shadow_is_not_renamed", [] {
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("public class C { static int helper(int x) { return x; } }"));
    const auto renames = jcor::build_method_renames(classes);
    CB_CHECK(!renames.contains("C") || !renames.at("C").contains("helper"));
});

// NoiseGeneratorPerlin: a static gen(double) helper coexists with the instance gen(double,double) override of an abstract base. Different signatures --
// a legal C++ overload; renaming by name would kill the override and leave the class abstract.
CB_TEST("java.corpus.a_static_overload_beside_an_instance_override_is_not_renamed", [] {
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("public abstract class Base { public abstract double gen(double a, double b); }"));
    classes.push_back(java::parse::parse_source("public final class Sub extends Base {\n private static double gen(double a) { return a; }\n"
                                                " public double gen(double a, double b) { return a + b; } }"));
    const auto renames = jcor::build_method_renames(classes);
    CB_CHECK(!renames.contains("Sub") || !renames.at("Sub").contains("gen"));
});

CB_TEST("java.emitted.an_array_field_initialised_to_null_is_left_empty", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({{"G.java", "public class G { public String[] levels = null; }"}});
    CB_CHECK(!jpo_has(done.cpp.at("G"), "levels = NULL"));   // a JArray default-constructs to empty
});

CB_TEST("java.emitted.an_array_field_assigned_null_becomes_the_empty_array", [] {
    if (!jpo_ready("emitted-cpp")) return;
    const JpoConverted done = jpo_convert({{"G.java", "public class G {\n public String[] levels;\n public void clear() {\n  this.levels = null;\n } }"}});
    const std::string& cpp = done.cpp.at("G");
    CB_CHECK(jpo_has(cpp, "this->levels = JArray<JavaString >();"));   // reassign -> empty JArray, not NULL
    CB_CHECK(!jpo_has(cpp, "this->levels = NULL"));
});

// ================================================================================================================================================
// a signed byte scalar is `signed char` and its buffer unsigned; static initialisers run after what they read.
CB_TEST("java.types.a_byte_scalar_is_signed_and_its_buffer_unsigned", [] {
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("package sample;\npublic class P {}\n"));
    jcor::resolve_inheritance(classes);
    const java::types::TypeResolver resolver = jcor::build_resolver(classes);
    CB_CHECK_EQ(resolver.field_type(jir::JType{"byte", 0, {}}), "signed char");
    CB_CHECK_EQ(resolver.field_type(jir::JType{"byte", 1, {}}), "JArray<unsigned char >");
});

CB_TEST("java.init_driver.a_dependency_runs_before_its_dependent", [] {
    // B's static init reads A's statics -> A must come first, even though B < A alphabetically.
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("package sample;\npublic class Zebra { public static int x = 1; }\n"));
    classes.push_back(java::parse::parse_source("package sample;\npublic class Apple {\n  public static int y = Zebra.x + 1;\n}\n"));
    jcor::resolve_inheritance(classes);
    const auto order = java::init_driver::compute_static_init_order(classes, {"Zebra", "Apple"}, {});
    const auto zebra = std::find(order.begin(), order.end(), "Zebra"), apple = std::find(order.begin(), order.end(), "Apple");
    CB_CHECK(zebra != order.end() && apple != order.end());
    CB_CHECK(zebra < apple);   // dep before dependent
});

CB_TEST("java.init_driver.the_seed_order_of_a_profile_is_kept", [] {
    // Material before Block is a seeded (ctor-mediated) order even with no textual dependency (the seed is Minecraft's, its project's profile)
    std::vector<jir::JClass> classes;
    classes.push_back(java::parse::parse_source("package sample;\npublic class Block { public static int y = 2; }\n"));
    classes.push_back(java::parse::parse_source("package sample;\npublic class Material { public static int x = 1; }\n"));
    jcor::resolve_inheritance(classes);
    const auto order = java::init_driver::compute_static_init_order(
        classes, {"Material", "Block"}, java::projects::minecraft::profiles::minecraft_xbox360::static_init_order());
    const auto material = std::find(order.begin(), order.end(), "Material"), block = std::find(order.begin(), order.end(), "Block");
    CB_CHECK(material != order.end() && block != order.end());
    CB_CHECK(material < block);
});

// ================================================================================================================================================
// a collection parameter a method mutates is an out-parameter (a C++ reference), else the value copy drops the callee's
// writes (the getCollidingBoundingBoxes "player falls through the world" bug). The three signals, the read-only negative case, the emitter's rendering.
namespace {

struct JpoOutParams {
    std::vector<jir::JClass> classes;
    std::size_t marked = 0;
    const jir::JClass& by(const char* name) const {
        for (const jir::JClass& c : classes)
            if (c.name == name) return c;
        throw InternalError(std::string("no class ") + name);
    }
    JpoOutParams() {
        // Base.fill DIRECTLY mutates its list parameter (var1.add) -> out-parameter.
        classes.push_back(java::parse::parse_source("public class Base { void fill(World var0, ArrayList var1) { var1.add(var0); } }"));
        // Sub overrides fill and ONLY forwards to super -- never touches var1 directly. Still `&` (virtual override signatures must match).
        classes.push_back(java::parse::parse_source("public class Sub extends Base { void fill(World var0, ArrayList var1) { super.fill(var0, var1); } }"));
        // sink mutates a Map parameter; forward passes its Map into sink (non-override forwarding).
        classes.push_back(java::parse::parse_source("public class Holder { void sink(Map var1) { var1.put(1, 2); }\n void forward(Map var1) { this.sink(var1); } }"));
        // readOnly only ITERATES its list -> stays by value (a safe copy; not an out-parameter).
        classes.push_back(java::parse::parse_source("public class Reader { void readOnly(List var1) { Iterator var2 = var1.iterator(); } }"));
        jcor::resolve_inheritance(classes);
        marked = jcor::compute_collection_out_params(classes);
    }
};

}  // namespace

CB_TEST("java.corpus.a_collection_a_method_mutates_is_an_out_parameter", [] {
    const JpoOutParams p;
    CB_CHECK(p.by("Base").methods[0].params[1].by_ref);      // var1 mutated
    CB_CHECK(!p.by("Base").methods[0].params[0].by_ref);     // World var0 untouched
});

CB_TEST("java.corpus.an_override_that_only_forwards_matches_its_base", [] {
    const JpoOutParams p;
    CB_CHECK(p.by("Sub").methods[0].params[1].by_ref);       // only calls super: promoted so its signature matches Base's virtual `&`
});

CB_TEST("java.corpus.forwarding_a_collection_to_an_out_slot_makes_it_one", [] {
    const JpoOutParams p;
    const jir::JClass& holder = p.by("Holder");
    const jir::Method* forward = nullptr;
    for (const jir::Method& m : holder.methods)
        if (m.name == "forward") forward = &m;
    CB_CHECK(forward != nullptr);
    if (forward) CB_CHECK(forward->params[0].by_ref);        // forward() -> sink() out slot
});

CB_TEST("java.corpus.a_read_only_collection_stays_by_value", [] {
    const JpoOutParams p;
    CB_CHECK(!p.by("Reader").methods[0].params[0].by_ref);   // iterate-only: a safe copy
    CB_CHECK(p.marked >= 3);
});

CB_TEST("java.ownership.a_promoted_collection_is_a_reference_in_the_signature", [] {
    const JpoOutParams p;
    const java::types::TypeResolver resolver = jcor::build_resolver(p.classes);
    const std::string sig = java::emit::ownership::params_str(p.by("Base").methods[0], resolver);
    CB_CHECK(jpo_has(sig, "& var1"));    // promoted -> a reference in the C++ signature
    CB_CHECK(!jpo_has(sig, "& var0"));   // a non-collection parameter is unaffected
});

// ================================================================================================================================================
// the platform layer's C++ (the sources the transpiler copies out), read and compiled.
namespace {

stdfs::path jpo_canonical() { return testing::data_path("../../data/java/platform/canonical"); }
stdfs::path jpo_reference() { return testing::data_path("../../data/java/platform/reference"); }

// The `{...}` body of the function whose definition starts with `signature`.
std::string jpo_func_body(const std::string& text, const std::string& signature) {
    const std::size_t start = text.find(signature);
    if (start == std::string::npos) throw InternalError("no function " + signature);
    const std::size_t open = text.find('{', start);
    int depth = 0;
    for (std::size_t j = open; j < text.size(); ++j) {
        if (text[j] == '{') ++depth;
        else if (text[j] == '}' && --depth == 0) return text.substr(open, j + 1 - open);
    }
    throw InternalError("unbalanced body for " + signature);
}

// A test program compiled as C++98 against the canonical headers and run: its output.
std::string jpo_compile_and_run(const std::string& source) {
    fs::ScopedTempDir dir("cb-review");
    fs::write_text(dir.path() / "t.cpp", source);
    const std::string failed = jpo_gxx({"-std=c++98", "-w", "-I", fs::to_text(jpo_canonical()), "t.cpp", "-o", "t.exe"}, dir.path());
    if (!failed.empty()) throw InternalError("g++: " + failed);
    int code = -1;
    const std::string out = jpo_run(dir.path() / "t.exe", dir.path(), &code);
    if (code != 0) throw InternalError("the test program returned " + std::to_string(code));
    return out;
}

}  // namespace

// toCharArray / getBytes leak: the ring caps the memory they hold.
CB_TEST("java.platform.tochararray_and_getbytes_memory_is_bounded", [] {
    if (!jpo_tool_runs("g++", "--version")) return;
    const std::string out = jpo_compile_and_run(R"(
#include <cstdio>
#include <cstdlib>
#include <new>
static long g_live = 0;
void* operator new[](std::size_t n) throw(std::bad_alloc) {
    std::size_t* p = (std::size_t*)std::malloc(n + sizeof(std::size_t)); *p = n; g_live += (long)n; return p + 1; }
void operator delete[](void* q) throw() {
    if (!q) return; std::size_t* p = (std::size_t*)q - 1; g_live -= (long)*p; std::free(p); }
#include "JavaString.h"
int main() {
    JavaString s(std::string(1000, 'x'));
    for (int i = 0; i < 20000; ++i) { JArray<unsigned char> c = s.toCharArray(); JArray<unsigned char> b = s.getBytes("UTF-8"); }
    JArray<unsigned char> c = JavaString("abc").toCharArray();
    if (c.length != 3 || c[0] != 'a' || c[2] != 'c' || c.data()[3] != 0) return 2;   // copy + NUL tail
    std::printf("%ld\n", g_live);
    return 0;
}
)");
    // leaking would hold 20000 * 2 * 1001 bytes (~40 MB); the ring caps it at 2 * 256 slots
    CB_CHECK(std::stol(std::string(text::trim(out))) < 2 * 256 * 1001 + 4096);
});

CB_TEST("java.platform.save360_shutdown_releases_the_mount_gate", [] {
    const std::string body = jpo_func_body(fs::read_text(jpo_reference() / "Save360.reference.cpp"), "void Save360_Shutdown()");
    CB_CHECK(jpo_has(body, "g_mountState = 0"));
});

CB_TEST("java.platform.save360_unwritable_mount_is_closed_before_the_fallback", [] {
    const std::string text = fs::read_text(jpo_reference() / "Save360.reference.cpp");
    const std::size_t i = text.find("root is not writable");
    CB_CHECK(i != std::string::npos);
    if (i == std::string::npos) return;
    const std::string branch = text.substr(i, text.find("return false;", i) - i);
    CB_CHECK(jpo_has(branch, "XContentClose(SAVE_ROOT_NAME"));
});

// Math.sqrt is always double: the shim is compiled exactly as shipped in JavaRT.h (in isolation: the full runtime header set shadows system headers).
CB_TEST("java.platform.javamathsqrt_resolves_atan2_with_a_float_argument", [] {
    if (!jpo_tool_runs("g++", "--version")) return;
    std::vector<std::string> shim;
    const std::string rt = fs::read_text(jpo_canonical() / "JavaRT.h");
    std::size_t at = 0;
    while (at < rt.size()) {
        const std::size_t end = std::min(rt.find('\n', at), rt.size());
        const std::string line = rt.substr(at, end - at);
        if (jpo_has(line, "javaMathSqrt(double")) shim.push_back(line);
        at = end + 1;
    }
    CB_CHECK_EQ(shim.size(), std::size_t{1});
    if (shim.size() != 1) return;
    const std::string out = jpo_compile_and_run("#include <cstdio>\n#include <math.h>\n" + shim[0] +
                                                "\nint main() { float f = 4.0F; double d = 1.0; std::printf(\"%.3f\\n\", atan2(javaMathSqrt(f), d)); return 0; }\n");
    CB_CHECK_EQ(text::trim(out), "1.107");
});

CB_TEST("java.platform.toggling_alpha_test_does_not_reset_the_alpha_ref", [] {
    const std::string text = fs::read_text(jpo_reference() / "GL11.reference.cpp");
    const std::regex assigns(R"(g_alphaRef\s*=)");
    for (const char* signature : {"void glEnable(int cap)", "void glDisable(int cap)"}) CB_CHECK(!std::regex_search(jpo_func_body(text, signature), assigns));
    CB_CHECK(std::regex_search(text, std::regex(R"(static float g_alphaRef = 0\.5f;)")));
});

CB_TEST("java.platform.the_viewport_height_comes_from_the_backbuffer_and_y_is_clamped", [] {
    const std::string text = fs::read_text(jpo_reference() / "GL11.reference.cpp");
    const std::string vp = jpo_func_body(text, "void glViewport(int x, int y, int w, int h)");
    CB_CHECK(!jpo_has(vp, "g_fbH ="));   // no longer captured from an arbitrary (0,0,w,h)
    CB_CHECK(jpo_has(vp, "if (fy < 0) fy = 0;"));
    CB_CHECK(jpo_has(jpo_func_body(text, "void glSetDevice(IDirect3DDevice9* dev)"), "GetBackBuffer"));
});

// ================================================================================================================================================
// the PC platform (pc_opengl): SDL2 + real OpenGL behind LWJGL's API, a generated PC entry point and a CMake project.
// The conversion needs LWJGL on javac's classpath (without it the test is skipped); building and running needs g++ and SDL2.
namespace {

constexpr const char* jpo_spin_game = R"(import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;
import org.lwjgl.input.Keyboard;

public class Spin {
	public static void main(String[] var0) throws Exception {
		Display.setDisplayMode(new DisplayMode(320, 240));
		Display.create();
		for (int var1 = 0; var1 < 3 && !Display.isCloseRequested(); ++var1) {
			GL11.glClearColor(0.0F, 0.0F, 1.0F, 1.0F);
			GL11.glClear(GL11.GL_COLOR_BUFFER_BIT);
			if (Keyboard.isKeyDown(Keyboard.KEY_ESCAPE)) {
				break;
			}
			Display.update();
		}
		Display.destroy();
	}
}
)";

// The PC conversion of the Spin game, or nothing (the reason said) when this machine cannot make it.
std::optional<JpoConverted> jpo_spin() {
    try {
        return jpo_convert({{"Spin.java", jpo_spin_game}}, "", "pc_opengl");
    } catch (const Error& e) {   // no LWJGL on the classpath -> javac fails
        std::cerr << "pc platform: cannot convert without LWJGL: " << e.what() << "; skipped\n";
        return std::nullopt;
    }
}

}  // namespace

CB_TEST("java.pc_platform.the_pc_layer_replaces_the_stubs", [] {
    if (!jpo_ready("pc platform")) return;
    const auto spin = jpo_spin();
    if (!spin) return;
    const stdfs::path out = spin->out->path();
    for (const char* f : {"Display.cpp", "GL11.cpp", "Input.cpp", "Main.cpp", "CMakeLists.txt"}) CB_CHECK(stdfs::is_regular_file(out / f));   // (GLU, ImageIO: pruned, unused)
    const std::string main = fs::read_text(out / "Main.cpp");
    CB_CHECK(jpo_has(main, "int main(int argc, char** argv)") && jpo_has(main, "Spin::main(args)"));
    CB_CHECK(!jpo_has(fs::read_text(out / "GL11.h"), "#include <GL/"));   // no GL macros in the API
    CB_CHECK(jpo_has(fs::read_text(out / "stdafx.h"), "J2X_COMPONENT_WIDTH() j2xDisplayWidth()"));
    CB_CHECK(jpo_has(fs::read_text(out / "CMakeLists.txt"), "add_executable(Spin"));
});

CB_TEST("java.pc_platform.it_builds_and_runs", [] {
    std::error_code ec;
    if (!jpo_ready("pc platform") || !jpo_tool_runs("g++", "--version") || !stdfs::is_regular_file("C:/msys64/mingw64/include/SDL2/SDL.h", ec)) return;
    const auto spin = jpo_spin();
    if (!spin) return;
    const stdfs::path out = spin->out->path();
    // Without LWJGL's classes on javac's classpath the game's calls into it are not typed and the translator writes them as unsupported (a throw): the
    // program builds but cannot run.
    if (jpo_has(fs::read_text(out / "Spin.cpp"), "UnsupportedOperationException")) {
        std::cerr << "pc platform: LWJGL is not on the front end's classpath here, so the game's bodies are unsupported; skipped\n";
        return;
    }
    std::vector<std::string> args = {"-std=gnu++17", "-w", "-fpermissive", "-I", fs::to_text(out)};
    for (const auto& entry : stdfs::directory_iterator(out))
        if (entry.path().extension() == ".cpp") args.push_back(fs::to_text(entry.path().filename()));
    std::sort(args.begin() + 5, args.end());
    for (const char* a : {"-o", "spin.exe", "-lSDL2", "-lopengl32", "-lglu32", "-lws2_32"}) args.emplace_back(a);
    const std::string failed = jpo_gxx(args, out);
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) return;
    for (const char* dll : {"SDL2.dll", "libstdc++-6.dll", "libgcc_s_seh-1.dll", "libwinpthread-1.dll"}) {
        const stdfs::path p = stdfs::path("C:/msys64/mingw64/bin") / dll;
        if (stdfs::is_regular_file(p, ec)) stdfs::copy_file(p, out / dll, stdfs::copy_options::overwrite_existing, ec);
    }
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(out / "spin.exe")};
    spec.cwd = out;
    spec.env = {{"J2X_SCRIPT", "2:shot frame.bmp"}};
    spec.timeout = std::chrono::seconds(120);
    const process::ProcessResult r = process::run(spec);
    CB_CHECK(r.ok());
    if (!r.ok()) std::cerr << "exit " << r.exit_code << " stderr: " << std::string(r.err.begin(), r.err.end()).substr(0, 800) << '\n';
    CB_CHECK(jpo_has(std::string(r.err.begin(), r.err.end()), "frame saved"));
});

// The sound system, saving and resources on the PC (FIXED BUGs: the SoundSystem stub was silent, Writer.write(String) was lost so options.txt saved
// empty, a resource asked for under a folder was missing from the flattened Content folder). Building it needs SDL2 and the vorbis libraries; the
// the check writes a tone as a WAV by hand and reads the mixed output back.
CB_TEST("java.pc_platform.sound_saving_and_resources_work", [] {
    std::error_code ec;
    if (!jpo_ready("pc platform") || !jpo_tool_runs("g++", "--version") || !stdfs::is_regular_file("C:/msys64/mingw64/include/SDL2/SDL.h", ec) ||
        !stdfs::is_regular_file("C:/msys64/mingw64/include/vorbis/vorbisfile.h", ec))
        return;
    JpoConverted done;
    try {
        done = jpo_convert({{"Noise.java", R"(import paulscode.sound.SoundSystem;
import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.URL;

public class Noise {
	public static void main(String[] var0) throws Exception {
		SoundSystem var1 = new SoundSystem();
		URL var2 = new File("tone.wav").toURI().toURL();
		var1.newSource(false, "t", var2, "tone.wav", false, 0.0F, 0.0F, 0.0F, 0, 16.0F);
		var1.play("t");
		Thread.sleep(500L);
		var1.cleanup();
		InputStream var4 = Noise.class.getResourceAsStream("/misc/res.txt");
		PrintWriter var3 = new PrintWriter(new FileWriter("out.txt"));
		var3.println("key:7");
		var3.println(var4 != null ? "res ok" : "res missing");
		var3.close();
	}
}
)"}},
                           "", "pc_opengl");
    } catch (const Error& e) {
        std::cerr << "pc platform: cannot convert without the sound system: " << e.what() << "; skipped\n";
        return;
    }
    const stdfs::path out = done.out->path();
    if (!done.cpp.contains("Noise") || jpo_has(done.cpp.at("Noise"), "UnsupportedOperationException")) {   // (the sound system's classes are not on the classpath: see above)
        std::cerr << "pc platform: the sound system is not on the front end's classpath here, so the game's bodies are unsupported; skipped\n";
        return;
    }
    std::vector<std::string> args = {"-std=gnu++17", "-w", "-fpermissive", "-I", fs::to_text(out)};
    for (const auto& entry : stdfs::directory_iterator(out))
        if (entry.path().extension() == ".cpp") args.push_back(fs::to_text(entry.path().filename()));
    std::sort(args.begin() + 5, args.end());
    for (const char* a : {"-o", "noise.exe", "-lSDL2", "-lopengl32", "-lglu32", "-lvorbisfile", "-lvorbis", "-logg", "-lws2_32"}) args.emplace_back(a);
    const std::string failed = jpo_gxx(args, out);
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) return;
    const stdfs::path content = out / "Content";
    stdfs::create_directories(content, ec);
    // a half second of a 22050 Hz mono 16-bit sine tone (peak 12000)
    Bytes wav;
    const auto put32 = [&](std::uint32_t v) { for (int i = 0; i < 4; ++i) wav.push_back(static_cast<std::uint8_t>(v >> (8 * i))); };
    const auto put16 = [&](std::uint16_t v) { for (int i = 0; i < 2; ++i) wav.push_back(static_cast<std::uint8_t>(v >> (8 * i))); };
    const auto tag = [&](const char* t) { for (int i = 0; i < 4; ++i) wav.push_back(static_cast<std::uint8_t>(t[i])); };
    tag("RIFF"), put32(36 + 22050), tag("WAVE"), tag("fmt "), put32(16), put16(1), put16(1), put32(22050), put32(44100), put16(2), put16(16), tag("data"), put32(22050);
    for (int i = 0; i < 11025; ++i) put16(static_cast<std::uint16_t>(static_cast<std::int16_t>(12000 * std::sin(i * 0.1))));
    fs::write_file(content / "tone.wav", wav);
    fs::write_text(content / "res.txt", "x");
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(out / "noise.exe")};
    spec.cwd = out;
    spec.env = {{"J2X_AUDIO_DUMP", "mix.wav"}, {"SDL_AUDIODRIVER", "dummy"}, {"PATH", "C:\\msys64\\mingw64\\bin;" + fs::get_env("PATH").value_or("")}};
    spec.timeout = std::chrono::seconds(120);
    const process::ProcessResult r = process::run(spec);
    CB_CHECK(r.ok());
    if (!r.ok()) std::cerr << "exit " << r.exit_code << " stderr: " << std::string(r.err.begin(), r.err.end()).substr(0, 800) << '\n';
    const std::string written = fs::read_text(content / "out.txt");
    const std::vector<std::string_view> words = text::split_ws(written);
    CB_CHECK(words.size() == 3 && words[0] == "key:7" && words[1] == "res" && words[2] == "ok");
    // the mixed output has sound in it: a peak above 1000
    const Bytes mix = fs::read_file(content / "mix.wav");
    std::int32_t peak = 0;
    for (std::size_t i = 44; i + 1 < mix.size(); i += 2) peak = std::max<std::int32_t>(peak, std::abs(static_cast<std::int16_t>(mix[i] | (mix[i + 1] << 8))));
    CB_CHECK(peak > 1000);
});

// ================================================================================================================================================
// Method-rename collisions, the files written, the translation applied, and constructors becoming j2xinit, with the real front end:
// the Java in, the files out.

// Java allows a field `isRunning` and a method `isRunning(...)`; C++ does not: the method is renamed.
CB_TEST("java.pipeline.a_method_named_like_a_field_is_renamed_in_the_header", [] {
    if (!jpo_ready("pipeline")) return;
    const JpoConverted done = jpo_convert({{"NetworkManager.java",
                                            "public class NetworkManager {\n public boolean isRunning;\n"
                                            " public static boolean isRunning(NetworkManager var0) { return var0.isRunning; }\n}\n"}});
    const std::string h = fs::read_text(done.out->path() / "NetworkManager.h");
    CB_CHECK(jpo_has(h, "bool isRunning;"));                                          // the field is unchanged
    CB_CHECK(jpo_has(h, "static bool isRunning_(NetworkManager* var0);"));            // the method is renamed
});

CB_TEST("java.pipeline.a_class_gives_header_and_source_and_an_interface_only_a_header", [] {
    if (!jpo_ready("pipeline")) return;
    const JpoConverted done = jpo_convert({{"Foo.java", "package a;\npublic class Foo {\n    public int x;\n    public Foo(int var1) { this.x = var1; }\n    public int getX() { return this.x; }\n}\n"},
                                           {"IBar.java", "package a;\npublic interface IBar {\n    void doIt();\n}\n"}});
    const stdfs::path out = done.out->path();
    CB_CHECK(stdfs::is_regular_file(out / "Foo.h"));
    CB_CHECK(stdfs::is_regular_file(out / "Foo.cpp"));
    CB_CHECK(stdfs::is_regular_file(out / "IBar.h"));
    CB_CHECK(!stdfs::is_regular_file(out / "IBar.cpp"));
    // the translation is applied
    const std::string& cpp = done.cpp.at("Foo");
    CB_CHECK(jpo_has(cpp, "this->x = var1;"));
    CB_CHECK(jpo_has(cpp, "int Foo::getX()"));
});

// a conversion picks its edition's profile (an explicit one, the corpus's own marks, a version hint) for that
// conversion only, and says so; nothing conclusive keeps the default. (The profile is handed down,
// so there is no process-wide state to restore.)
namespace {

struct JpoEditionRun {
    std::vector<std::string> logs;
    java::transpile::Summary summary;
    bool logged(std::string_view part) const {
        return std::ranges::any_of(logs, [&](const std::string& m) { return m.find(part) != std::string::npos; });
    }
};

JpoEditionRun jpo_edition(const std::map<std::string, std::string>& files, const std::string& edition, const std::string& version) {
    fs::ScopedTempDir src("cb-edition-src"), out("cb-edition-out");
    for (const auto& [name, text] : files) fs::write_text(src.path() / name, text);
    JpoEditionRun run;
    java::transpile::Options options;
    options.write_report = false;
    options.emit_debug = false;
    options.edition = edition;
    options.mc_version = version;
    options.progress = [&run](std::string_view line) { run.logs.emplace_back(line); };
    run.summary = jpo_front().assembly->convert(src.path(), out.path(), options);
    return run;
}

}  // namespace

CB_TEST("java.edition_convert.an_explicit_edition_is_that_conversions_profile", [] {
    if (!jpo_ready("edition")) return;
    const JpoEditionRun run = jpo_edition({{"Level.java", "public class Level { public int x; }"}}, "rubydung", "");
    CB_CHECK_EQ(run.summary.parsed, std::size_t{1});
    CB_CHECK_EQ(run.summary.profile, "minecraft_rubydung");
});

CB_TEST("java.edition_convert.a_beta_corpus_is_detected", [] {
    if (!jpo_ready("edition")) return;
    const JpoEditionRun run = jpo_edition({{"World.java", "package net.minecraft.src;\npublic class World { public int x; }"},
                                           {"BiomeGenBase.java", "package net.minecraft.src;\npublic class BiomeGenBase { public int id; }"}}, "", "");
    CB_CHECK(run.logged("active target profile: minecraft_beta"));
    CB_CHECK_EQ(run.summary.profile, "minecraft_beta");
});

CB_TEST("java.edition_convert.nothing_conclusive_keeps_the_default", [] {
    if (!jpo_ready("edition")) return;
    const JpoEditionRun run = jpo_edition({{"Foo.java", "public class Foo { public int x; }"}}, "", "");
    CB_CHECK(!run.logged("active target profile"));
});

CB_TEST("java.edition_convert.a_version_hint_selects_the_edition", [] {
    if (!jpo_ready("edition")) return;
    const JpoEditionRun run = jpo_edition({{"Foo.java", "public class Foo { public int x; }"}}, "", "a1.2.6");
    CB_CHECK(run.logged("active target profile: minecraft_alpha"));
    CB_CHECK_EQ(run.summary.profile, "minecraft_alpha");
});

// A class whose constructor calls an overridable method is built in two phases: a field-zeroing C() and a j2xInit(...) per Java constructor.
CB_TEST("java.twophase.the_constructors_become_j2xinit", [] {
    if (!jpo_ready("two-phase")) return;
    const JpoConverted done = jpo_convert({
        {"Block.java",
         "public class Block {\n\tpublic static final boolean[] opaque = new boolean[16];\n\tpublic final int blockID;\n\tpublic int light = 3;\n\n"
         "\tprotected Block(int var1) {\n\t\tthis.blockID = var1;\n\t\topaque[var1] = this.isOpaqueCube();\n\t\tthis.light = this.lightValue() + this.light;\n\t}\n\n"
         "\tprotected Block(int var1, int var2) {\n\t\tthis(var1);\n\t\tthis.light += var2;\n\t}\n\n"
         "\tpublic boolean isOpaqueCube() {\n\t\treturn true;\n\t}\n\n\tpublic int lightValue() {\n\t\treturn 0;\n\t}\n\n"
         "\tpublic static boolean opaqueAt(int var0) {\n\t\treturn opaque[var0 & 15];\n\t}\n\n"
         "\tpublic static int glassLight(int var0) {\n\t\treturn new BlockGlass(var0 & 15).light;\n\t}\n\n"
         "\tpublic static boolean glassOpaque(int var0) {\n\t\tnew BlockGlass(var0 & 15);\n\t\treturn opaque[var0 & 15];\n\t}\n}\n"},
        {"BlockGlass.java",
         "public class BlockGlass extends Block {\n\tpublic int extra = 5;\n\n\tpublic BlockGlass(int var1) {\n\t\tsuper(var1, 2);\n\t\tthis.extra += var1;\n\t}\n\n"
         "\tpublic boolean isOpaqueCube() {\n\t\treturn false;\n\t}\n\n\tpublic int lightValue() {\n\t\treturn this.extra + 10;\n\t}\n}\n"},
        {"Plain.java", "public class Plain {\n\tpublic int v;\n\n\tpublic Plain(int var1) {\n\t\tthis.v = var1;\n\t}\n\n\tpublic int get() {\n\t\treturn this.v;\n\t}\n}\n"},
    });
    const std::string h = fs::read_text(done.out->path() / "BlockGlass.h");
    CB_CHECK(jpo_has(h, "BlockGlass();") && jpo_has(h, "BlockGlass* j2xInit(int var1);"));
    const std::string& cpp = done.cpp.at("BlockGlass");
    CB_CHECK(jpo_has(cpp, "Block::j2xInit(var1, 2);") && jpo_has(cpp, "return this;"));
    const std::string& block = done.cpp.at("Block");
    CB_CHECK(jpo_has(block, "Block::j2xInit(var1);"));              // this(var1)
    CB_CHECK(jpo_has(block, "(new BlockGlass())->j2xInit("));
    CB_CHECK(jpo_has(done.cpp.at("Plain"), "Plain::Plain(int var1)"));   // a class without the virtual call keeps its constructors
});

// ================================================================================================================================================
// The "beside the JVM" tests (byte fields, finally, Java semantics, the null model, strings and regex, probes, two-phase blocks, input I/O,
// probe libraries): the Java program is converted by the
// application's assembly (real front end), then every method of a class runs in the JVM and in the converted C++ on the same inputs and the
// results are compared -- the same programs, the same classes and methods wanted, the same "no difference" expectation. They need javac, a JVM and
// g++, and say so and pass without them.

namespace jprobe = cb::java::probe;

namespace {

std::optional<jprobe::Toolchain> jpb_tools(bool with_libraries) {
    if (!jpo_front().ready || !jpo_tool_runs("g++", "--version")) return std::nullopt;
    jprobe::Toolchain tools;
    tools.cxx = "g++";
    tools.java = jpo_front().tools.java;
    tools.javac = jpo_front().tools.javac;
    jpo_front().tools.classpath.clear();
    if (with_libraries) {
        try {
            const config::Config settings = config::Config::load(testing::data_path("../.."));
            const std::string libs = settings.text("java_libs");
            if (!libs.empty()) tools.libraries = jprobe::list_libraries(fs::from_text(libs));
            // The program is attributed against the libraries (and the stand-ins' folder) as production does: a call on a library class
            // (Keyboard.getKeyName) is a method of it, not a call javac could not resolve.
            if (!tools.libraries.empty()) {
                jprobe::Toolchain with_shims = tools;
                with_shims.shim_sources = java::JavaData(testing::data_path("../../data/java")).path({"probe_shims"});
                if (const auto shims = jprobe::compile_shims(jpo_front().state->path(), with_shims)) jpo_front().tools.classpath.push_back(*shims);
                for (const stdfs::path& jar : tools.libraries) jpo_front().tools.classpath.push_back(jar);
            }
        } catch (const std::exception&) {
        }
    }
    return tools;
}

struct JpbRun {
    JpoConverted converted;
    std::unique_ptr<fs::ScopedTempDir> work;
    jprobe::RunResult result;
    std::string what_differs() const {
        std::string text;
        for (std::size_t i = 0; i < result.differs.size() && i < 4; ++i)
            text += result.differs[i].method + "(" + text::join(result.differs[i].inputs, ",") + "): java " + result.differs[i].java + " / native " + result.differs[i].native + "; ";
        return text;
    }
};

// Convert `files`, then run the class `cls` beside the JVM.
JpbRun jpb_run(const jprobe::Toolchain& tools, const std::map<std::string, std::string>& files, const std::string& cls) {
    JpbRun run;
    run.converted = jpo_convert(files);
    run.work = std::make_unique<fs::ScopedTempDir>("cb-java-probe");
    const stdfs::path staged = jprobe::stage_sources(run.converted.src->path(), run.work->path() / "javasrc");
    const jprobe::SourceTree tree(staged);
    const jir::JClass* parsed = tree.lookup(cls);
    CB_CHECK(parsed != nullptr);
    if (parsed == nullptr) return run;
    run.result = jprobe::run(tree, run.converted.out->path(), *parsed, run.work->path() / "work", tools);
    return run;
}

bool jpb_has_method(const jprobe::RunResult& r, std::string_view name) { return std::ranges::find(r.methods, name) != r.methods.end(); }

constexpr const char* kJpbPacket =
    "public class Packet {\n\tpublic int entityId;\n\tpublic byte type;\n\tpublic byte yaw;\n\n\tpublic Packet() {\n\t}\n\n"
    "\tpublic String describe() {\n\t\treturn \"type \" + this.type + \", yaw \" + this.yaw;\n\t}\n\n"
    "\tpublic static String show(int var0) {\n\t\tbyte var1 = (byte)var0;\n\t\treturn \"\" + var1;\n\t}\n}\n";

}  // namespace

// every constructor zeroes byte fields as Java does (they were garbage on a fresh packet).
CB_TEST("java.beside_the_jvm.a_constructor_zeroes_the_byte_fields", [] {
    if (!jpo_ready("beside-the-jvm")) return;
    const JpoConverted done = jpo_convert({{"Packet.java", kJpbPacket}});
    const std::string& cpp = done.cpp.at("Packet");
    const std::size_t at = cpp.find("Packet::Packet()");
    CB_CHECK(at != std::string::npos);
    const std::string ctor = cpp.substr(at, cpp.find('{', at) - at);
    CB_CHECK(jpo_has(ctor, "type(0)") && jpo_has(ctor, "yaw(0)") && jpo_has(ctor, "entityId(0)"));
});

CB_TEST("java.beside_the_jvm.bytes", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Packet.java", kJpbPacket}}, "Packet");
    CB_CHECK(jpb_has_method(run.result, "describe") && jpb_has_method(run.result, "show"));
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// a `finally` body runs before every return that leaves its try.
CB_TEST("java.beside_the_jvm.the_finally_counter_comes_back_down", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Lighting.java",
        "public class Lighting {\n\tstatic int depth;\n\n\tstatic boolean step(int var0) {\n\t\t++depth;\n\t\ttry {\n\t\t\tif(var0 > 0) {\n\t\t\t\treturn true;\n\t\t\t}\n"
        "\t\t\treturn false;\n\t\t} finally {\n\t\t\t--depth;\n\t\t}\n\t}\n\n\tpublic static int run(int var0) {\n\t\tfor(int var1 = 0; var1 < var0; ++var1) {\n"
        "\t\t\tstep(var1);\n\t\t}\n\t\treturn depth;\n\t}\n}\n"}}, "Lighting");
    CB_CHECK(jpb_has_method(run.result, "run") && run.result.cases > 0);
    CB_CHECK(run.result.differs.empty());   // depth is 0 after any number of calls
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// an empty array is not null, a null dereference in a try reaches its handler, Java's char is unsigned.
namespace {

const std::map<std::string, std::string> kJpbSemantics = {
    {"Nibbles.java", "public class Nibbles {\n\tprivate byte[] data;\n\n\tpublic Nibbles(int var1) {\n\t\tthis.data = new byte[var1 >> 1];\n\t}\n\n"
                     "\tpublic boolean isValid() {\n\t\treturn this.data != null;\n\t}\n\n\tpublic int size() {\n\t\treturn this.data.length;\n\t}\n}\n"},
    {"Loader.java", "public class Loader {\n\tstatic int[] buffer = new int[4];\n\tstatic int failures;\n\tLoader next;\n\n\tstatic Loader find(int var0) {\n\t\treturn null;\n\t}\n\n"
                    "\tpublic static int load(int var0) {\n\t\ttry {\n\t\t\tLoader var1 = find(var0);\n\t\t\tvar1.touch();\n\t\t\treturn 1;\n\t\t} catch (Exception var2) {\n"
                    "\t\t\t++failures;\n\t\t\treturn -1;\n\t\t}\n\t}\n\n\tpublic static int guarded(int var0) {\n\t\ttry {\n\t\t\tLoader var1 = find(var0);\n"
                    "\t\t\tif(var1 != null && var1.next != null) {\n\t\t\t\treturn 2;\n\t\t\t}\n\t\t\treturn 3;\n\t\t} catch (Exception var2) {\n\t\t\treturn -1;\n\t\t}\n\t}\n\n\tvoid touch() {\n\t}\n}\n"},
    {"Chars.java", "public class Chars {\n\tpublic static int code(int var0) {\n\t\tchar var1 = (char)var0;\n\t\treturn var1 + 0;\n\t}\n\n"
                   "\tpublic static boolean high(int var0) {\n\t\tchar var1 = (char)var0;\n\t\treturn var1 > 127;\n\t}\n}\n"},
};

}  // namespace

CB_TEST("java.beside_the_jvm.char_is_unsigned_in_the_emitted_code", [] {
    if (!jpo_ready("beside-the-jvm")) return;
    const JpoConverted done = jpo_convert(kJpbSemantics);
    CB_CHECK(jpo_has(done.cpp.at("Chars"), "unsigned char var1 = (unsigned char)var0;"));
});

CB_TEST("java.beside_the_jvm.empty_arrays_null_handlers_and_unsigned_chars", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    struct Row {
        const char* name;
        std::vector<std::string> methods;
    };
    for (const Row& row : {Row{"Nibbles", {"isValid", "size"}}, Row{"Loader", {"guarded", "load"}}, Row{"Chars", {"code", "high"}}}) {
        const JpbRun run = jpb_run(*tools, kJpbSemantics, row.name);
        std::vector<std::string> methods = run.result.methods;
        std::ranges::sort(methods);
        CB_CHECK(methods == row.methods);
        CB_CHECK_EQ(run.result.judged, row.methods.size());
        if (std::string_view(row.name) == "Chars") {
            // unsigned 8-bit: every char of 0-255 agrees with Java; one past that cannot be held (strings are 8-bit)
            CB_CHECK(!run.result.differs.empty());
            for (const jprobe::Difference& d : run.result.differs) {
                const long value = d.inputs.empty() ? 0 : std::stol(d.inputs[0]);
                CB_CHECK(!(0 <= value && value <= 255));
            }
        } else {
            CB_CHECK(run.result.differs.empty());
            if (!run.result.differs.empty()) std::cerr << row.name << ": " << run.what_differs() << "\n";
        }
    }
});

// a String, List, Map or Set carries a null state of its own.
CB_TEST("java.beside_the_jvm.null_strings_lists_maps_and_sets", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Nulls.java",
        "import java.util.ArrayList;\nimport java.util.HashMap;\nimport java.util.HashSet;\nimport java.util.List;\nimport java.util.Map;\nimport java.util.Set;\n\n"
        "public class Nulls {\n\tpublic String value;\n\tprivate Map byName = new HashMap();\n\n\tpublic Nulls() {\n\t}\n\n"
        "\tpublic Nulls(String var1) {\n\t\tthis.value = var1;\n\t\tif(var1 == null) {\n\t\t\tthrow new IllegalArgumentException(\"Empty string not allowed\");\n\t\t}\n\t}\n\n"
        "\tpublic String toString() {\n\t\treturn \"\" + this.value;\n\t}\n\n"
        "\tpublic static boolean emptyIsNotNull(int var0) {\n\t\tString var1 = var0 > 0 ? \"\" : null;\n\t\treturn var1 == null;\n\t}\n\n"
        "\tpublic static int emptySaved(int var0) {\n\t\tString[] var1 = new String[]{\"\", \"\", \"\", \"\"};\n\t\treturn new Nulls(var1[var0 & 3]).toString().length();\n\t}\n\n"
        "\tpublic static String unset(int var0) {\n\t\tNulls var1 = new Nulls();\n\t\treturn var1.toString() + var0;\n\t}\n\n"
        "\tpublic static boolean slotIsNull(int var0) {\n\t\tString[] var1 = new String[4];\n\t\treturn var1[var0 & 3] == null;\n\t}\n\n"
        "\tpublic int lookup(int var1) {\n\t\tif(var1 > 0) {\n\t\t\tthis.byName.put(\"a\", new ArrayList());\n\t\t}\n\t\tList var2 = (List)this.byName.get(\"a\");\n\t\treturn var2 == null ? -1 : var2.size();\n\t}\n\n"
        "\tpublic static boolean nullMap(int var0) {\n\t\tMap var1 = var0 > 0 ? new HashMap() : null;\n\t\treturn var1 == null;\n\t}\n\n"
        "\tpublic static boolean nullSet(int var0) {\n\t\tSet var1 = var0 > 0 ? new HashSet() : null;\n\t\treturn var1 == null;\n\t}\n\n"
        "\tpublic int nested(int var1) {\n\t\tif(var1 > 0) {\n\t\t\tthis.byName.put(\"m\", new HashMap());\n\t\t}\n\t\tMap var2 = (Map)this.byName.get(\"m\");\n\t\treturn var2 == null ? -1 : var2.size();\n\t}\n\n"
        "\tpublic static boolean nullList(int var0) {\n\t\tList var1 = var0 > 0 ? new ArrayList() : null;\n\t\treturn var1 == null;\n\t}\n}\n"}}, "Nulls");
    for (const char* wanted : {"emptyIsNotNull", "emptySaved", "unset", "slotIsNull", "lookup", "nullList", "nullMap", "nullSet", "nested"}) CB_CHECK(jpb_has_method(run.result, wanted));
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// the string and regex runtime (Pattern/Matcher, replaceAll, split, compareTo, hashCode, MD5).
CB_TEST("java.beside_the_jvm.strings_and_regular_expressions", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Texts.java",
        "import java.math.BigInteger;\nimport java.security.MessageDigest;\nimport java.util.regex.Matcher;\nimport java.util.regex.Pattern;\n\n"
        "public class Texts {\n"
        "\tprivate static final Pattern NUMBER = Pattern.compile(\"(-?)(0|([1-9]([0-9]*)))(\\\\.[0-9]+)?((e|E)(\\\\+|-)?[0-9]+)?\");\n"
        "\tprivate static final Pattern CHUNK = Pattern.compile(\"c\\\\.(-?[0-9a-z]+)\\\\.(-?[0-9a-z]+)\\\\.dat\");\n\n"
        "\tpublic static boolean isNumber(String var0) {\n\t\treturn NUMBER.matcher(var0).matches();\n\t}\n\n"
        "\tpublic static String chunk(String var0) {\n\t\tMatcher var1 = CHUNK.matcher(\"c.\" + var0 + \".dat\");\n\t\treturn var1.matches() ? var1.group(1) + \"|\" + var1.group(2) : \"none\";\n\t}\n\n"
        "\tpublic static String dots(String var0) {\n\t\treturn var0.replaceAll(\"/\", \".\") + var0.replaceAll(\"[aeiou]+\", \"<$0>\");\n\t}\n\n"
        "\tpublic static int pieces(String var0) {\n\t\treturn var0.split(\":\").length * 100 + var0.split(\" \").length;\n\t}\n\n"
        "\tpublic static int compare(String var0, String var1) {\n\t\treturn var0.compareTo(var1);\n\t}\n\n"
        "\tpublic static int hash(String var0) {\n\t\treturn (\"\\u00a7c\" + var0).hashCode();\n\t}\n\n"
        "\tpublic static String md5(int var0) throws Exception {\n\t\tString var2 = \"MD5String\" + var0;\n\t\tMessageDigest var1 = MessageDigest.getInstance(\"MD5\");\n"
        "\t\tvar1.update(var2.getBytes(), 0, var2.length());\n\t\treturn (new BigInteger(1, var1.digest())).toString(16);\n\t}\n}\n"}}, "Texts");
    for (const char* wanted : {"isNumber", "chunk", "dots", "pieces", "compare", "hash", "md5"}) CB_CHECK(jpb_has_method(run.result, wanted));
    CB_CHECK_EQ(run.result.judged, run.result.methods.size());
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// primitives in and out (a static table built with Math.PI), and a result Java does not repeat is left out.
CB_TEST("java.beside_the_jvm.methods_with_primitives_and_a_char_that_cannot_be_held", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Calc.java",
        "package demo;\n\npublic class Calc {\n\tprivate static float[] TABLE = new float[256];\n\n"
        "\tpublic static int clamp(int var0, int var1) {\n\t\treturn var0 < 0 ? 0 : (var0 > var1 ? var1 : var0);\n\t}\n\n"
        "\tpublic static float wave(float var0) {\n\t\treturn TABLE[(int)(var0 * 40.743664F) & 255];\n\t}\n\n"
        "\tstatic double half(double var0) {\n\t\treturn var0 / 2.0D;\n\t}\n\n"
        "\tpublic static long mix(long var0, int var2) {\n\t\treturn var0 * 31L + (long)var2;\n\t}\n\n"
        "\tpublic static int widen(int var0) {\n\t\treturn (char)var0;\n\t}\n\n"
        "\tpublic int notStatic(int var1) {\n\t\treturn var1;\n\t}\n\n"
        "\tstatic {\n\t\tfor(int var0 = 0; var0 < 256; ++var0) {\n\t\t\tTABLE[var0] = (float)Math.sin((double)var0 * Math.PI * 2.0D / 256.0D);\n\t\t}\n\t}\n}\n"}}, "Calc");
    std::vector<std::string> methods = run.result.methods;
    std::ranges::sort(methods);
    CB_CHECK(methods == (std::vector<std::string>{"clamp", "half", "mix", "notStatic", "wave", "widen"}));
    CB_CHECK(run.result.judged == 6 && run.result.total == 6 && run.result.cases > 50);
    std::set<std::string> differing;
    std::vector<std::string> widen_inputs;
    for (const jprobe::Difference& d : run.result.differs) {
        differing.insert(d.method);
        if (d.method == "widen" && !d.inputs.empty()) widen_inputs.push_back(d.inputs[0]);
    }
    for (const char* agrees : {"wave", "clamp", "mix", "half"}) CB_CHECK(!differing.contains(agrees));   // wave is equal because PI is the double Java uses
    // Java's char is 16 bits and unsigned; here it is unsigned char: (char)255 agrees, (char)65535 cannot
    CB_CHECK(std::ranges::find(widen_inputs, "255") == widen_inputs.end());
    CB_CHECK(std::ranges::find(widen_inputs, "65535") != widen_inputs.end());
});

CB_TEST("java.beside_the_jvm.a_result_java_does_not_repeat_is_left_out", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    // an unseeded Random (seeded from the time and a counter): not the clock itself, which is left out before this check
    const JpbRun run = jpb_run(*tools, {{"Dice.java",
        "import java.util.Random;\n\npublic class Dice {\n\tpublic static int roll(int var0) {\n\t\treturn (new Random()).nextInt(1000000) + var0 * 0;\n\t}\n\n"
        "\tpublic static int twice(int var0) {\n\t\treturn var0 * 2;\n\t}\n}\n"}}, "Dice");
    CB_CHECK(run.result.unsteady > 0 && run.result.differs.empty());   // roll is left out, twice agrees
    CB_CHECK_EQ(run.result.judged, std::size_t{1});
});

// Strings in and out, objects made as arguments (by a static factory) and compared by their fields, a void method by its object
// afterwards, a double printed as Java prints it.
CB_TEST("java.beside_the_jvm.strings_objects_and_number_text", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const JpbRun run = jpb_run(*tools, {{"Point.java",
        "package geo;\n\npublic class Point {\n\tpublic double x;\n\tpublic double y;\n\tString label = \"p\";\n\n"
        "\tprivate Point(double var1, double var3) {\n\t\tthis.x = var1;\n\t\tthis.y = var3;\n\t}\n\n"
        "\tpublic static Point of(double var0, double var2) {\n\t\treturn new Point(var0, var2);\n\t}\n\n"
        "\tpublic Point add(Point var1) {\n\t\treturn new Point(this.x + var1.x, this.y + var1.y);\n\t}\n\n"
        "\tpublic void scale(double var1) {\n\t\tthis.x *= var1;\n\t\tthis.y *= var1;\n\t}\n\n"
        "\tpublic String toString() {\n\t\treturn \"(\" + this.x + \", \" + this.y + \")\";\n\t}\n\n"
        "\tpublic String named(String var1) {\n\t\treturn var1.toUpperCase() + \":\" + this.x;\n\t}\n\n"
        "\tpublic static int length(String var0) {\n\t\treturn var0.length();\n\t}\n}\n"}}, "Point");
    for (const char* wanted : {"add", "scale", "toString", "named", "length"}) CB_CHECK(jpb_has_method(run.result, wanted));
    CB_CHECK(run.result.differs.empty());   // toString prints doubles: "(0.0, 1000000.0)" in Java, once "(0, 1e+06)" here
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
    CB_CHECK(run.result.judged >= 6);
});

// A transparent block is not opaque; a case left out leaves no state.
CB_TEST("java.beside_the_jvm.a_glass_block_is_not_opaque", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    const std::map<std::string, std::string> files = {
        {"Block.java",
         "public class Block {\n\tpublic static final boolean[] opaque = new boolean[16];\n\tpublic final int blockID;\n\tpublic int light = 3;\n\n"
         "\tprotected Block(int var1) {\n\t\tthis.blockID = var1;\n\t\topaque[var1] = this.isOpaqueCube();\n\t\tthis.light = this.lightValue() + this.light;\n\t}\n\n"
         "\tprotected Block(int var1, int var2) {\n\t\tthis(var1);\n\t\tthis.light += var2;\n\t}\n\n"
         "\tpublic boolean isOpaqueCube() {\n\t\treturn true;\n\t}\n\n\tpublic int lightValue() {\n\t\treturn 0;\n\t}\n\n"
         "\tpublic static boolean opaqueAt(int var0) {\n\t\treturn opaque[var0 & 15];\n\t}\n\n"
         "\tpublic static int glassLight(int var0) {\n\t\treturn new BlockGlass(var0 & 15).light;\n\t}\n\n"
         "\tpublic static boolean glassOpaque(int var0) {\n\t\tnew BlockGlass(var0 & 15);\n\t\treturn opaque[var0 & 15];\n\t}\n}\n"},
        {"BlockGlass.java",
         "public class BlockGlass extends Block {\n\tpublic int extra = 5;\n\n\tpublic BlockGlass(int var1) {\n\t\tsuper(var1, 2);\n\t\tthis.extra += var1;\n\t}\n\n"
         "\tpublic boolean isOpaqueCube() {\n\t\treturn false;\n\t}\n\n\tpublic int lightValue() {\n\t\treturn this.extra + 10;\n\t}\n}\n"},
        {"Plain.java", "public class Plain {\n\tpublic int v;\n\n\tpublic Plain(int var1) {\n\t\tthis.v = var1;\n\t}\n\n\tpublic int get() {\n\t\treturn this.v;\n\t}\n}\n"},
    };
    const JpbRun run = jpb_run(*tools, files, "Block");
    CB_CHECK(jpb_has_method(run.result, "glassLight") && jpb_has_method(run.result, "glassOpaque"));
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

CB_TEST("java.beside_the_jvm.cases_left_out_leave_no_state_behind", [] {
    const auto tools = jpb_tools(false);
    if (!tools) return;
    // a case that threw in Java was left out of the C++ run but had already changed static state; and `new Reg(x);` once became `delete new Reg(x);`
    const JpbRun run = jpb_run(*tools, {{"Reg.java",
        "public class Reg {\n\tpublic static Reg[] list = new Reg[8];\n\tpublic int id;\n\n\tpublic Reg(int var1) {\n\t\tlist[var1 & 7] = this;\n\t\tthis.id = var1;\n"
        "\t\tif(var1 < 0) {\n\t\t\tthrow new IllegalArgumentException(\"negative\");\n\t\t}\n\t}\n\n"
        "\tpublic static int make(int var0) {\n\t\tnew Reg(var0);\n\t\treturn var0;\n\t}\n\n"
        "\tpublic static int peek(int var0) {\n\t\treturn list[var0 & 7].id;\n\t}\n}\n"}}, "Reg");
    CB_CHECK(jpb_has_method(run.result, "make") && jpb_has_method(run.result, "peek"));
    CB_CHECK_EQ(run.result.judged, std::size_t{2});
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// needs LWJGL in the settings' java_libs.
CB_TEST("java.beside_the_jvm.keyboard_names_and_a_missing_resource", [] {
    const auto tools = jpb_tools(true);
    if (!tools) return;
    const bool lwjgl = std::ranges::any_of(tools->libraries, [](const stdfs::path& jar) { return fs::to_text(jar.filename()).find("lwjgl-") != std::string::npos; });
    if (!lwjgl) {
        std::cerr << "beside-the-jvm: no LWJGL in java_libs; skipped\n";
        return;
    }
    const JpbRun run = jpb_run(*tools, {{"Inputs.java",
        "import java.io.BufferedReader;\nimport java.io.InputStreamReader;\nimport org.lwjgl.input.Keyboard;\n\npublic class Inputs {\n"
        "\tpublic static String binding(int var0) {\n\t\treturn \"Forward: \" + Keyboard.getKeyName(var0 & 255);\n\t}\n\n"
        "\tpublic static int index(int var0) {\n\t\tString var1 = Keyboard.getKeyName(var0 & 255);\n\t\treturn var1 == null ? -1 : Keyboard.getKeyIndex(var1);\n\t}\n\n"
        "\tpublic static String splash(int var0) {\n\t\tString var1 = \"missingno\";\n\t\ttry {\n"
        "\t\t\tBufferedReader var2 = new BufferedReader(new InputStreamReader(Inputs.class.getResourceAsStream(\"/title/none\" + var0 + \".txt\")));\n"
        "\t\t\tvar1 = var2.readLine();\n\t\t} catch (Exception var3) {\n\t\t}\n\t\treturn var1;\n\t}\n}\n"}}, "Inputs");
    for (const char* wanted : {"binding", "index", "splash"}) CB_CHECK(jpb_has_method(run.result, wanted));
    CB_CHECK_EQ(run.result.judged, std::size_t{3});
    CB_CHECK(run.result.differs.empty());
    if (!run.result.differs.empty()) std::cerr << run.what_differs() << "\n";
});

// the stand-in for Minecraft's own build of the sound codec lets CodecMus compile against the sound-system jar.
CB_TEST("java.beside_the_jvm.codec_mus_compiles_against_the_stand_in", [] {
    const auto tools = jpb_tools(true);
    if (!tools) return;
    const bool sound = std::ranges::any_of(tools->libraries, [](const stdfs::path& jar) { return fs::to_text(jar.filename()).find("soundsystem") != std::string::npos; });
    if (!sound) {
        std::cerr << "beside-the-jvm: no sound system in java_libs; skipped\n";
        return;
    }
    jprobe::Toolchain with_shims = *tools;
    with_shims.shim_sources = java::JavaData(testing::data_path("../../data/java")).path({"probe_shims"});
    fs::ScopedTempDir work("cb-java-shims");
    const auto shims = jprobe::compile_shims(work.path(), with_shims);
    CB_CHECK(shims.has_value());
    if (!shims) return;
    CB_CHECK(stdfs::is_regular_file(*shims / "paulscode" / "sound" / "codecs" / "CodecJOrbis.class"));
    const stdfs::path src = work.path() / "src" / "net" / "minecraft" / "src";
    fs::write_text(src / "CodecMus.java",
                   "package net.minecraft.src;\n\nimport java.io.IOException;\nimport java.io.InputStream;\nimport paulscode.sound.codecs.CodecJOrbis;\n\n"
                   "public class CodecMus extends CodecJOrbis {\n\tprotected InputStream openInputStream() throws IOException {\n\t\treturn this.urlConnection.getInputStream();\n\t}\n}\n");
    std::string classpath = fs::to_text(*shims);
    for (const stdfs::path& jar : tools->libraries) classpath += ";" + fs::to_text(jar);
    process::ProcessSpec spec;
    spec.argv = {fs::to_text(tools->javac), "-cp", classpath, "-d", fs::to_text(work.path() / "out"), fs::to_text(src / "CodecMus.java")};
    const process::ProcessResult r = process::run(spec);
    CB_CHECK(r.ok());
});

// ================================================================================================================================================
// A modern Java program (records, a sealed interface and patterns, a switch expression
// and a switch on strings, lambdas, streams, a map) lowered by the real front end, written as one C++17 program by the whole-program back end, built
// with g++ and run beside the JVM: the output and the exit code must agree.
namespace {

const char* const jpo_modern = R"JAVA(import java.util.*;
import java.util.function.*;
import java.util.stream.*;

public class Modern {
    record Point(int x, int y) {
        int dist2() { return x * x + y * y; }
    }

    sealed interface Shape permits Sq, Circ {}
    record Sq(int side) implements Shape {}
    record Circ(int r) implements Shape {}

    enum Color { RED, GREEN, BLUE }

    static int area(Shape s) {
        if (s instanceof Sq q) return q.side() * q.side();
        if (s instanceof Circ c) return 3 * c.r() * c.r();
        return 0;
    }

    static String name(Color c) {
        return switch (c) {
            case RED -> "r";
            case GREEN -> "g";
            default -> { String b = "b"; yield b + b; }
        };
    }

    static int sum(List<Integer> xs) {
        int s = 0;
        for (int v : xs) s += v;
        return s;
    }

    static int squares(int n) {
        return IntStream.rangeClosed(1, Math.abs(n) % 50).map(x -> x * x).sum();
    }

    static String words(String s) {
        return switch (s) { case "a", "b" -> "ab"; default -> "other"; };
    }

    public static void main(String[] args) {
        List<Integer> xs = new ArrayList<>();
        for (int i = 0; i < 6; i++) xs.add(i * i);
        Function<Integer, Integer> twice = v -> v * 2;
        System.out.println(sum(xs) + " " + twice.apply(21) + " " + squares(10));
        System.out.println(new Point(3, 4) + " " + new Point(3, 4).dist2() + " " + new Point(1, 2).equals(new Point(1, 2)));
        System.out.println(area(new Sq(4)) + " " + area(new Circ(2)) + " " + name(Color.BLUE) + " " + words("b"));
        System.out.println(xs.stream().filter(v -> v % 2 == 0).map(String::valueOf).collect(Collectors.joining(",")));
        Map<String, Integer> m = new HashMap<>();
        for (String w : "x y x z y x".split(" ")) m.merge(w, 1, Integer::sum);
        System.out.println(m);
    }
}
)JAVA";

const char* const jpo_tally = R"JAVA(import java.util.ArrayList;
import java.util.List;

public class Tally {
	public static int total(int n) {
		List<Integer> xs = new ArrayList<Integer>();
		for (int i = 0; i < n % 20; i++) xs.add(i);
		int s = 0;
		for (int v : xs) s += v;
		return s;
	}

	public static String kind(int k) {
		return switch (k % 3) { case 0 -> "zero"; case 1 -> "one"; default -> "many"; };
	}

	public static int fresh(int k) {
		Tally t = new Tally();
		return t.twice(k);
	}

	public int twice(int k) {
		return k * 2;
	}
}
)JAVA";

const translate::native_l2::Library* jpo_l2() {
    static const std::optional<translate::native_l2::Library> lib = []() -> std::optional<translate::native_l2::Library> {
        stdfs::path dll = translate::native_l2::Library::default_dll(testing::data_path("../.."));
        if (const auto env = fs::get_env("CB_L2_DLL")) dll = fs::from_text(*env);
        std::error_code ec;
        if (!stdfs::is_regular_file(dll, ec)) return std::nullopt;
        return translate::native_l2::Library::load(dll);
    }();
    return lib ? &*lib : nullptr;
}

std::string jpo_text_of(const process::ProcessResult& r) {
    std::string t(r.out.begin(), r.out.end());
    std::erase(t, '\r');
    return t;
}

}  // namespace

CB_TEST("java.cpp17.the_program_runs_as_the_jvm_runs_it", [] {
    if (!jpo_ready("cpp17-run") || !jpo_tool_runs("g++", "--version")) return;
    const translate::native_l2::Library* lib = jpo_l2();
    if (lib == nullptr) {
        std::cerr << "cpp17-run: cbl2.dll is not built; skipped\n";
        return;
    }
    fs::ScopedTempDir work("cb-java-modern");
    const stdfs::path src = work.path() / "src";
    fs::write_text(src / "Modern.java", jpo_modern);

    Arena arena;
    const std::vector<translate::ir::Node*> units = java::frontend::lower_program(arena, src, jpo_front().tools);
    CB_CHECK_EQ(units.size(), std::size_t{1});
    const std::string text = translate::backends::cpp17::program(*lib, arena, units);
    // the idioms the back end is for: lambdas, range-for, a switch expression as a lambda, real constructors for a record
    CB_CHECK(jpo_has(text, "jrt::fn<Function>(") && jpo_has(text, "jrt::each<int32_t>(xs)") && jpo_has(text, "[&]() -> ref<String> {") &&
             jpo_has(text, "std::make_shared<Modern_Point>("));
    fs::write_text(work.path() / "program.cpp", text);
    std::vector<std::string> args(std::begin(translate::backends::cpp17::kFlags), std::end(translate::backends::cpp17::kFlags));
    args.insert(args.end(), {"-I", fs::to_text(translate::backends::cpp17::runtime_dir()), "program.cpp", "-o", "program.exe"});
    const std::string failed = jpo_gxx(args, work.path());
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) {
        std::cerr << failed.substr(0, 2000) << "\n";
        return;
    }

    process::ProcessSpec javac;
    javac.argv = {fs::to_text(jpo_front().tools.javac), "-d", fs::to_text(work.path() / "classes"), fs::to_text(src / "Modern.java")};
    CB_CHECK(process::run(javac).ok());
    process::ProcessSpec jvm;
    jvm.argv = {fs::to_text(jpo_front().tools.java), "-cp", fs::to_text(work.path() / "classes"), "Modern"};
    jvm.timeout = std::chrono::seconds(120);
    const process::ProcessResult want = process::run(jvm);
    process::ProcessSpec native;
    native.argv = {fs::to_text(work.path() / "program.exe")};
    native.timeout = std::chrono::seconds(120);
    const process::ProcessResult got = process::run(native);
    CB_CHECK(jpo_has(jpo_text_of(want), "Point[x=3, y=4] 25 true"));   // (the JVM's own answer is what the native one is held to)
    CB_CHECK_EQ(jpo_text_of(got), jpo_text_of(want));
    CB_CHECK_EQ(got.exit_code, want.exit_code);
});

// Tally.java converted by the transpiler for C++17 and for C++03: the C++17 text has a
// range-for, the switch expression as a lambda and `auto*` for a new; neither has a gap; the C++17 text compiles (-fsyntax-only).
CB_TEST("java.cpp17.inside_the_transpiler_beside_cpp03", [] {
    if (!jpo_ready("cpp17-transpile")) return;
    const std::map<std::string, std::string> files = {{"Tally.java", jpo_tally}};
    const JpoConverted c17 = jpo_convert(files, "", "", "C++17");
    const JpoConverted c03 = jpo_convert(files);
    const std::string& t17 = c17.cpp.at("Tally");
    const std::string& t03 = c03.cpp.at("Tally");
    CB_CHECK(jpo_has(t17, ": xs)"));                  // a range-for
    CB_CHECK(jpo_has(t17, "[&]() ->"));               // the switch expression
    CB_CHECK(jpo_has(t17, "auto* t = new Tally"));
    CB_CHECK(!jpo_has(t17, "J2X-GAP") && !jpo_has(t03, "J2X-GAP"));   // cpp03 has it too, desugared
    if (!jpo_tool_runs("g++", "--version")) return;
    const stdfs::path out = c17.out->path();
    const std::string failed = jpo_gxx({"-std=c++17", "-fsyntax-only", "-w", "-iquote", fs::to_text(out), fs::to_text(out / "Tally.cpp")}, out);
    CB_CHECK_EQ(failed, std::string());
    if (!failed.empty()) std::cerr << failed.substr(0, 2000) << "\n";
});
