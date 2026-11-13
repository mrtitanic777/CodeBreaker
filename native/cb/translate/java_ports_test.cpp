// The Python tests of tests/java that checked what the transpiler EMITS, or a pass of the corpus, or the platform layer's C++ sources -- the ones no other
// native test covered -- ported with the same inputs and the same expectations:
//
//   test_call_arg_cast, test_ctor_delegation_cast, test_int_array_unbox (2), test_noequals_identity (2), test_ptr_factory, test_int_cast
//       Java source in, the transpiler's C++ out, as the application assembles it (java/assembly) with the real front end: javac's typed tree of
//       the sources (TreeDump). They need a JDK; without one they say so and pass.
//   test_host_compile_fixes (5)   the same, then the emitted classes and a runtime unit compiled with the host g++ (skipped without one)
//   test_collection_out_params (5), test_build3_fixes (3 of 5), test_beta_fixes (3)   the whole-program passes on parsed classes
//   test_review_fixes (6)   the platform layer's C++ (JavaString.h, JavaRT.h, Save360.reference.cpp, GL11.reference.cpp): read, and compiled by g++
//   test_build3_fixes (2: arrays assigned null) and test_pc_platform (3) are in the last tests.
//
// The Python converted with `profiles.use('minecraft_xbox360')` active between tests; a conversion here reads the profile it asks for (none).
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <regex>

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
#include "cb/java/projects/minecraft/profiles/minecraft_xbox360.hpp"
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
        f.tools.treedump_source = testing::data_path("../../codebreaker/translate/frontends/java/TreeDump.java");
        f.tools.state = f.state->path();
        JpoFront* self = &f;
        f.assembly = std::make_unique<java::assembly::Assembly>(
            java::JavaData(testing::data_path("../../codebreaker/java")),
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
};

JpoConverted jpo_convert(const std::map<std::string, std::string>& files, const std::string& edition = "", const std::string& platform = "") {
    fs::ScopedTempDir src("cb-java-src");
    for (const auto& [name, text] : files) fs::write_text(src.path() / name, text);
    JpoConverted done;
    done.out = std::make_unique<fs::ScopedTempDir>("cb-java-out");
    java::transpile::Options options;
    options.write_report = false;
    options.emit_debug = false;
    options.edition = edition;
    options.platform = platform;
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
// test_call_arg_cast.py: `entityMap.a((Entity)null, var1).size()` -- a nullable getter of another class must not type `entityMap.a()`, and a cast
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

// test_ctor_delegation_cast.py: a transitive `this((Block)var1, 1)` delegation threads the argument through the X-typed overload.
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

// test_int_array_unbox.py: `(int[])list.remove(i)` of a raw list unboxes the JObject element.
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

// test_noequals_identity.py: `.equals()` on a class that declares no equals() is reference identity; one that does keeps its call.
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

// test_ptr_factory.py: a stub class's static factory (`Runtime.getRuntime()`) gets `->` on its result.
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

// test_int_cast.py: Java's (int)/(long) of a float or double saturates (NaN -> 0), so every such cast of an emitted class is j2xToInt / j2xToLong; the
// Python then judged the class against the JVM. Here the emitted class is compiled and run, and the saturated values are Java's.
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
// test_host_compile_fixes.py: the classes of the Alpha corpus that did not compile with a host g++, transpiled and compiled.
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
// test_build3_fixes.py: a static method that shadows an inherited instance one is renamed; arrays assigned null become empty arrays.
// test_emit_source.py TestNestedAggregateArrayStatic.test_no_braced_new_array, through the real translator: `static int[][] x = new int[][]{{..},{..}}` is a
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
// test_beta_fixes.py: a signed byte scalar is `signed char` and its buffer unsigned; static initialisers run after what they read.
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
// test_collection_out_params.py: a collection parameter a method mutates is an out-parameter (a C++ reference), else the value copy drops the callee's
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
// test_review_fixes.py: the platform layer's C++ (the sources the transpiler copies out), read and compiled.
namespace {

stdfs::path jpo_canonical() { return testing::data_path("../../codebreaker/java/platform/canonical"); }
stdfs::path jpo_reference() { return testing::data_path("../../codebreaker/java/platform/reference"); }

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
// test_pc_platform.py: the PC platform (pc_opengl): SDL2 + real OpenGL behind LWJGL's API, a generated PC entry point and a CMake project.
// The conversion needs LWJGL on javac's classpath (the Python skipped without it); building and running needs g++ and SDL2.
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
    // program builds but cannot run. The Python skipped here too ("cannot convert without LWJGL").
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
// Python's check wrote a tone with the `wave` module and read the mixed output back -- here the tone is written as a WAV by hand.
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
