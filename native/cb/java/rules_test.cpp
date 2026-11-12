#include "cb/java/rules.hpp"
#include "cb/java/strview.hpp"

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace r = cb::java::rules;
using cb::java::ledger::Category;
using cb::java::ledger::Severity;

// The Xbox 360 target (big-endian, null deref is a hard crash) with a profile that names a few heavy types and
// nullable names.
const r::Context& xbox() {
    static const r::Context ctx = [] {
        cb::java::profiles::Profile p;
        p.heavy_types = {"BigBuf", "Huge"};
        p.heavy_types_high = {"Huge"};
        p.type.nullable_getters = {"getBlock"};
        p.type.nullable_fields = {"worldObj"};
        return r::Context::make("xbox360", &p);
    }();
    return ctx;
}

r::Findings fire(std::string_view rule, std::string_view name, std::string_view text,
                 std::optional<std::string_view> header = std::nullopt, const r::Context& ctx = xbox()) {
    return r::check(rule, r::FileInput{name, text, header}, ctx);
}

bool mentions(const r::Finding& f, std::string_view word) { return cb::java::contains(f.message, word); }

// ---- virtual-call-in-ctor -------------------------------------------------------------------------------------------

constexpr std::string_view kFooHeader = "class Foo {\npublic:\n\tFoo();\n\tvirtual void setOpacity(int);\n\tvirtual int calc();\n};\n";

CB_TEST("java.rules.virtual_call_in_ctor.this_arrow_and_bare_calls_are_flagged", [] {
    auto f = fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tthis->setOpacity(3);\n}\n", kFooHeader);
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(f[0].rule, "virtual-call-in-ctor");
    CB_CHECK(mentions(f[0], "setOpacity"));
    CB_CHECK_EQ(f[0].line, 1u);
    CB_CHECK(f[0].category == Category::Risk && f[0].severity == Severity::Med);
    f = fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tint n = calc();\n}\n", kFooHeader);
    CB_CHECK(f.size() == 1 && mentions(f[0], "calc"));
    f = fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tthis -> setOpacity(1);\n\tcalc ();\n}\n", kFooHeader);
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(2));   // the names of one constructor come in sorted order
    CB_CHECK(mentions(f[0], "calc") && mentions(f[1], "setOpacity"));
});

CB_TEST("java.rules.virtual_call_in_ctor.calls_that_are_fine", [] {
    CB_CHECK(fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tother->setOpacity(3);\n\tbar.calc();\n\tBase::calc();\n}\n", kFooHeader).empty());
    CB_CHECK(fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\thelper(1);\n}\n", kFooHeader).empty());   // helper is not virtual
    CB_CHECK(fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo(){ calc(); }", std::nullopt).empty());          // no header
    CB_CHECK(fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tmycalc(); calcs(1);\n}\n", kFooHeader).empty());   // other names
    CB_CHECK(fire("virtual-call-in-ctor", "Foo.h", "Foo::Foo() { calc(); }", kFooHeader).empty());   // only a .cpp holds definitions
});

CB_TEST("java.rules.virtual_call_in_ctor.only_overridden_methods_are_flagged_when_the_hierarchy_is_known", [] {
    r::Context ctx = xbox();
    ctx.override_pairs = r::ClassMethodPairs{{"Foo", "calc"}};
    const auto f = fire("virtual-call-in-ctor", "Foo.cpp", "Foo::Foo()\n{\n\tthis->setOpacity(3);\n\tint n = calc();\n}\n", kFooHeader, ctx);
    CB_CHECK(f.size() == 1 && mentions(f[0], "calc"));
});

CB_TEST("java.rules.virtual_call_in_ctor.a_constructor_with_an_init_list_and_several_definitions", [] {
    const std::string cpp = "Foo::Foo(int a) : Base(a), n(0)\n{\n}\n\nFoo::Foo()\n{\n\tcalc();\n}\n";
    const auto f = fire("virtual-call-in-ctor", "Foo.cpp", cpp, kFooHeader);
    CB_CHECK(f.size() == 1 && f[0].line == 5);
});

// ---- static-init-order ------------------------------------------------------------------------------------------------

CB_TEST("java.rules.static_init_order", [] {
    auto f = fire("static-init-order", "Foo.cpp", "Bar* Foo::s_inst = new Bar();\n");
    CB_CHECK(f.size() == 1 && f[0].rule == "static-init-order" && mentions(f[0], "Foo::s_inst"));
    CB_CHECK_EQ(f[0].snippet, "Bar* Foo::s_inst = new Bar();");
    CB_CHECK_EQ(fire("static-init-order", "Foo.cpp", "Tessellator* Foo::s_t = Tessellator::s_instance;\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("static-init-order", "Foo.cpp", "int Foo::s_n = 5;\nconst float Foo::s_pi = 3.14f;\n").empty());   // compile-time constants
    CB_CHECK(fire("static-init-order", "Foo.cpp", "JavaString Foo::s_s = new JavaString(\"x\");\n").empty());      // a runtime value type
    CB_CHECK(fire("static-init-order", "Foo.cpp", "Bar* Foo::s_inst;\n").empty());                                // the deferred form
    f = fire("static-init-order", "Foo.cpp", "int a;\n\nBar* Foo::s_x = new Bar();\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3);
    CB_CHECK(fire("static-init-order", "Foo.h", "Bar* Foo::s_inst = new Bar();\n").empty());   // headers hold no definitions
});

// ---- endianness ---------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.endianness", [] {
    auto f = fire("endianness", "NBTTagInt.cpp", "void NBTTagInt::write() {\n\tint x = *(int*)ptr;\n}\n");
    CB_CHECK(f.size() == 1 && f[0].rule == "endianness" && f[0].line == 2 && f[0].category == Category::Warning);
    CB_CHECK(mentions(f[0], "Xbox 360"));
    CB_CHECK_EQ(fire("endianness", "RegionFile.cpp", "void RegionFile::read() {\n\tmemcpy(dst, src, 4);\n}\n").size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(fire("endianness", "ChunkLoader.cpp", "x = reinterpret_cast<int*>(p);\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("endianness", "Foo.cpp", "void Foo::go() {\n\tint x = *(int*)ptr;\n}\n").empty());              // not a binary-IO file
    CB_CHECK(fire("endianness", "NBTTagInt.cpp", "// memcpy(a, b, 4);\n").empty());                              // a comment
    // a little-endian target never flags it
    const r::Context pc = r::Context::make("pc_windows");
    CB_CHECK(!pc.big_endian);
    CB_CHECK(fire("endianness", "NBTTagInt.cpp", "memcpy(a, b, 4);\n", std::nullopt, pc).empty());
});

// ---- uninit-array ---------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.uninit_array", [] {
    auto f = fire("uninit-array", "A.cpp", "\tbool* a = new bool[16];\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && mentions(f[0], "new bool[]"));
    CB_CHECK_EQ(f[0].snippet, "bool* a = new bool[16];");
    f = fire("uninit-array", "A.h", "int* b = new int[4];\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::Med && mentions(f[0], "new int[N]()"));
    f = fire("uninit-array", "A.cpp", "x = new unsigned int[3]; y = new long long[2];\n");
    CB_CHECK(f.size() == 1 && mentions(f[0], "new unsigned int[]"));   // the first array of a line, with the longer name
    CB_CHECK(fire("uninit-array", "A.cpp", "int* b = new int[4]();\nBlock** c = new Block*[8]();\n").empty());   // value-initialised
    f = fire("uninit-array", "A.cpp", "Block** c = new Block*[8];\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && mentions(f[0], "pointer array"));
    CB_CHECK(fire("uninit-array", "A.cpp", "// new int[3]\nint* p = nullptr;\n").empty());
    CB_CHECK(fire("uninit-array", "A.txt", "int* b = new int[4];\n").empty());
    CB_CHECK(fire("uninit-array", "A.cpp", "int* b = new Foo(4);\n").empty());
});

// ---- ctor-stub -------------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.ctor_stub", [] {
    auto f = fire("ctor-stub", "Foo.cpp", "Foo::Foo(int a)\n{\n}\n");
    CB_CHECK(f.size() == 1 && f[0].category == Category::Stubbed && f[0].severity == Severity::High && f[0].line == 1);
    CB_CHECK(mentions(f[0], "constructor Foo(int a)"));
    CB_CHECK_EQ(f[0].snippet, "Foo(int a) {...}");
    CB_CHECK(fire("ctor-stub", "Foo.cpp", "Foo::Foo(int a) : Base(a)\n{\n}\n").empty());   // an init list is real work
    CB_CHECK(fire("ctor-stub", "Foo.cpp", "Foo::Foo(int a)\n{\n\tthis->a = a;\n}\n").empty());
    CB_CHECK(fire("ctor-stub", "Foo.cpp", "Foo::Foo()\n{\n}\n").empty());                    // no parameters: not a delegation stub
    CB_CHECK_EQ(fire("ctor-stub", "Foo.cpp", "Foo::Foo(int a)\n{\n\t// just a comment\n\t/* and a block */\n}\n").size(), static_cast<std::size_t>(1));
    f = fire("ctor-stub", "Foo.cpp", "Foo::Foo(int a)\n{\n\tinit(); // was: this(a)\n}\n");
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(1));   // the delegation marker flags it even beside a statement
    f = fire("ctor-stub", "Foo.cpp", "int x;\n\nFoo::Foo(int a)\n{\n}\nFoo::Foo(int a, int b)\n{\n\tb = a;\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3);
});

// ---- rule-of-three ---------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.rule_of_three", [] {
    const std::string cpp = "int pad;\nFoo::Foo()\n{\n\tbuf = new char[10];\n}\n";
    auto f = fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\tFoo();\n};\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && f[0].line == 2 && mentions(f[0], "NO destructor"));
    f = fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\tFoo();\n\t~Foo();\n};\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::Med && mentions(f[0], "Rule of Three"));
    CB_CHECK(fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\tFoo();\n\t~Foo();\n\tFoo(const Foo& o);\n\tFoo& operator=(const Foo& o);\n};\n").empty());
    CB_CHECK(fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\tFoo();\n\t~Foo ();\n\tDISALLOW_COPY(Foo);\n};\n").empty());
    CB_CHECK(fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\t~Foo();\nprivate:\n\tFoo(const Foo&);\n};\n").empty());   // copying made private
    CB_CHECK(fire("rule-of-three", "Foo.cpp", "Foo::Foo()\n{\n\tx = 1;\n}\n", "class Foo {\n\tFoo();\n};\n").empty());   // owns nothing
    CB_CHECK(fire("rule-of-three", "Foo.cpp", cpp, std::nullopt).empty());                                          // no header
    CB_CHECK(fire("rule-of-three", "Foo.cpp", cpp, "class Foo {\n\t/* ~Foo(); */\n};\n").size() == 1);                  // a commented-out dtor is none
    f = fire("rule-of-three", "Foo.cpp", "Foo::Foo()\n{\n\tbuf = new Thing[4];\n}\n", "class Foo {\n};\n");   // an array of objects
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(1));
});

// ---- value-copy-owner ------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.value_copy_owner_parameters", [] {
    auto f = fire("value-copy-owner", "A.h", "void f(BigBuf b, int x);\n");
    CB_CHECK(f.size() == 1 && f[0].category == Category::Guessed && f[0].severity == Severity::Med && f[0].line == 1);
    CB_CHECK(mentions(f[0], "BigBuf passed BY VALUE"));
    CB_CHECK(fire("value-copy-owner", "A.h", "void f(BigBuf& b, BigBuf* c, const BigBuf& d);\n").empty());
    CB_CHECK(fire("value-copy-owner", "A.h", "void f(BigBufX b, XBigBuf c);\n").empty());
    f = fire("value-copy-owner", "A.h", "void g(BigBuf a, BigBuf b);\n");
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(2));   // FIXED: the second parameter was lost (its introducer was consumed)
    f = fire("value-copy-owner", "A.cpp", "void g(int x,\n\tHuge h);\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && f[0].line == 1);   // reported at the introducer, as before
    CB_CHECK_EQ(f[0].snippet, "void g(int x,");
    CB_CHECK(fire("value-copy-owner", "A.cpp", "// void g(BigBuf a)\n/* (BigBuf b, int c) */\n").empty());   // comments are blanked
    CB_CHECK(fire("value-copy-owner", "A.txt", "void f(BigBuf b);\n").empty());
});

CB_TEST("java.rules.value_copy_owner_returns", [] {
    auto f = fire("value-copy-owner", "A.cpp", "int x;\n\nBigBuf make()\n{\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && mentions(f[0], "returned BY VALUE"));   // FIXED: it was reported a line (or blank lines) early
    CB_CHECK_EQ(f[0].snippet, "BigBuf make()");
    f = fire("value-copy-owner", "A.cpp", "\tHuge Foo::get()\n{\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && f[0].line == 1);
    CB_CHECK(fire("value-copy-owner", "A.cpp", "BigBuf& get();\nBigBuf* get2();\nstatic BigBuf get3();\nint BigBuf2 get4();\n").empty());
});

// ---- null-deref ------------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.null_deref_getter", [] {
    auto f = fire("null-deref-getter", "A.cpp", "x = getBlock(1)->id;\ny = other(1)->id;\n// z = getBlock(1)->id;\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && f[0].line == 1);
    CB_CHECK(mentions(f[0], "getBlock() can return NULL"));
    CB_CHECK(mentions(f[0], "NULL dereference") || mentions(f[0], "NULL"));
    f = fire("null-deref-getter", "A.h", "a = getBlock(1)->id + getBlock(2)->meta;\n");
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(2));   // one per dereference
    CB_CHECK(fire("null-deref-getter", "A.cpp", "x = getBlock(1);\n").empty());
});

CB_TEST("java.rules.null_deref_field", [] {
    auto f = fire("null-deref-field", "A.cpp", "void A::f() {\n\tworldObj->tick();\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 2 && f[0].severity == Severity::Med && mentions(f[0], "Add `if (worldObj != NULL)` guard."));
    CB_CHECK_EQ(fire("null-deref-field", "A.cpp", "void A::f() {\n\tthis->worldObj->tick();\n}\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("null-deref-field", "A.cpp", "void A::f() {\n\tif (worldObj != NULL) worldObj->tick();\n}\n").empty());   // guarded on its line
    CB_CHECK(fire("null-deref-field", "A.cpp", "void A::f() {\n\tif (worldObj != nullptr)\n\t{\n\t\tworldObj->tick();\n\t}\n}\n").empty());   // guarded above
    CB_CHECK(fire("null-deref-field", "A.cpp", "void A::f() {\n\tif (worldObj == 0) return;\n\tworldObj->tick();\n}\n").empty());
    // the guard comparison is spelled with its single space; the guard must be within five lines above
    CB_CHECK_EQ(fire("null-deref-field", "A.cpp", "if (worldObj !=NULL) {}\nworldObj->tick();\n").size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(fire("null-deref-field", "A.cpp", "if (worldObj != NULL) {}\n1;\n2;\n3;\n4;\n5;\nworldObj->tick();\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("null-deref-field", "A.cpp", "if (worldObj != NULL) {}\n1;\n2;\n3;\n4;\nworldObj->tick();\n").empty());
    CB_CHECK(fire("null-deref-field", "A.cpp", "other->tick();\n// worldObj->tick();\n").empty());
    CB_CHECK_EQ(fire("null-deref-field", "A.cpp", "worldObj->a(); worldObj->b();\n").size(), static_cast<std::size_t>(1));   // one per line
});

// ---- loops -----------------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.hotpath_alloc", [] {
    const std::string hot = "void World::tick()\n{\n\tfor (int i = 0; i < n; i++)\n\t{\n\t\tVec3D* v = new Vec3D(1, 2);\n\t}\n}\n";
    auto f = fire("hotpath-alloc", "World.cpp", hot);
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::High && f[0].line == 5 && mentions(f[0], "new Vec3D"));
    CB_CHECK_EQ(f[0].snippet, "new Vec3D(1, 2);");
    f = fire("hotpath-alloc", "World.cpp", "void World::helper()\n{\n\twhile (more())\n\t\tp = new Node(1);\n}\n");
    CB_CHECK(f.size() == 1 && f[0].severity == Severity::Med && f[0].line == 4);   // not a hot-path method: Medium
    // a loop in a constructor runs once per object
    CB_CHECK(fire("hotpath-alloc", "World.cpp", "World::World()\n{\n\tfor (int i = 0; i < n; i++)\n\t{\n\t\tc = new Chunk(i);\n\t}\n}\n").empty());
    // ... but not one in a destructor, or in a method after a constructor
    CB_CHECK_EQ(fire("hotpath-alloc", "World.cpp", "World::~World()\n{\n\tfor (;;)\n\t{\n\t\tc = new Chunk(i);\n\t\tbreak;\n\t}\n}\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("hotpath-alloc", "World.cpp", "void World::tick()\n{\n\tfor (int i = 0; i < n; i++) { a[i] = new int[3]; }\n}\n").empty());   // new T[] is not an object
    // an allocation in two nested loops is reported once
    CB_CHECK_EQ(fire("hotpath-alloc", "World.cpp", "void World::tick()\n{\n\tfor (;;)\n\t{\n\t\tfor (;;)\n\t\t{\n\t\t\tx = new Obj(1);\n\t\t}\n\t}\n}\n").size(), static_cast<std::size_t>(1));
    CB_CHECK(fire("hotpath-alloc", "World.h", hot).empty());
});

CB_TEST("java.rules.infinite_loop", [] {
    auto f = fire("infinite-loop", "A.cpp", "void A::f()\n{\n\twhile (true)\n\t{\n\t\tdoIt();\n\t}\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && f[0].severity == Severity::Med && f[0].snippet == "while (true)");
    CB_CHECK(mentions(f[0], "`while (true)`") && mentions(f[0], "Xbox 360"));
    CB_CHECK_EQ(fire("infinite-loop", "A.cpp", "void A::f()\n{\n\tfor ( ; ; ) { x++; }\n\twhile (1) x++;\n}\n").size(), static_cast<std::size_t>(2));
    CB_CHECK(fire("infinite-loop", "A.cpp", "void A::f()\n{\n\twhile (true)\n\t{\n\t\tif (x) break;\n\t}\n}\n").empty());
    CB_CHECK(fire("infinite-loop", "A.cpp", "void A::f()\n{\n\tfor (;;)\n\t{\n\t\tSleep(2500);\n\t}\n}\n").empty());   // a service loop
    CB_CHECK(fire("infinite-loop", "A.cpp", "void A::f()\n{\n\twhile (x < 3)\n\t{\n\t}\n}\n").empty());
});

CB_TEST("java.rules.leak_loop_reassign", [] {
    const std::string leak = "void A::f()\n{\n\tfor (int i = 0; i < 3; i++)\n\t{\n\t\tp = new Thing(i);\n\t\tuse(p);\n\t}\n}\n";
    auto f = fire("leak-loop-reassign", "A.cpp", leak);
    CB_CHECK(f.size() == 1 && f[0].line == 5 && mentions(f[0], "`p = new ...`"));
    CB_CHECK_EQ(f[0].snippet, "p = new Thing(");
    CB_CHECK(fire("leak-loop-reassign", "A.cpp", "void A::f()\n{\n\twhile (x)\n\t{\n\t\tdelete p;\n\t\tp = new Thing(i);\n\t}\n}\n").empty());   // freed first
    CB_CHECK(fire("leak-loop-reassign", "A.cpp", "void A::f()\n{\n\tdelete p;\n\twhile (x)\n\t{\n\t\tp = new Thing(i);\n\t}\n}\n").empty());      // freed before the loop
    CB_CHECK(fire("leak-loop-reassign", "A.cpp", "void A::f()\n{\n\twhile (x)\n\t{\n\t\tp = new Thing(i);\n\t\treturn;\n\t}\n}\n").empty());   // followed by return
    CB_CHECK(fire("leak-loop-reassign", "A.cpp", "void A::f()\n{\n\twhile (x)\n\t{\n\t\tp = new Thing(i); break;\n\t}\n}\n").empty());
    CB_CHECK_EQ(fire("leak-loop-reassign", "A.cpp", "while (x) { p = new A(1); q = new B(2); }\n").size(), static_cast<std::size_t>(1));   // one per loop
    CB_CHECK(fire("leak-loop-reassign", "A.cpp", "while (x) { Thing* p = new Thing(i); }\n").size() == 1);   // (a declaration is still an assignment here)
});

// ---- side-effect-in-branch -------------------------------------------------------------------------------------------------

CB_TEST("java.rules.side_effect_in_branch", [] {
    auto f = fire("side-effect-in-branch", "A.cpp", "void A::f()\n{\n\tif (isSolid(x))\n\t{\n\t\tloadTexture(t);\n\t\treturn;\n\t}\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && f[0].severity == Severity::Low && mentions(f[0], "J2X-REVIEW[side-effect-in-branch]"));
    CB_CHECK(f[0].snippet.starts_with("if (isSolid(x))"));
    // an error/edge guard is the expected path
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (FAILED(hr))\n{\n\tlog(hr);\n\treturn;\n}\n").empty());
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (n == 0)\n{\n\tlog(n);\n\treturn;\n}\n").empty());
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (!t)\n{\n\tlog(n);\n\treturn;\n}\n").empty());
    // cleanup-only
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (ok)\n{\n\tt->Release();\n\treturn;\n}\n").empty());
    // the returned value is the call
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (ok)\n{\n\treturn compute(x);\n}\n").empty());
    // no return: nothing to collapse
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (ok)\n{\n\tloadTexture(t);\n}\n").empty());
    // a braceless body is low-risk
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (ok) loadTexture(t);\nreturn;\n").empty());
    // already tagged
    CB_CHECK(fire("side-effect-in-branch", "A.cpp", "if (ok)\n{\n\t// J2X-REVIEW[side-effect-in-branch]\n\tloadTexture(t);\n\treturn;\n}\n").size() == 0);
    // a field assignment is a side effect too
    CB_CHECK_EQ(fire("side-effect-in-branch", "A.cpp", "if (ok)\n{\n\tthis->x = new Foo;\n\treturn;\n}\n").size(), static_cast<std::size_t>(1));
});

// ---- debug-bypass ----------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.debug_bypass", [] {
    const std::string cpp = "void A::f()\n{\n\tprintf(\"x\");\n\tDBG_LOG(\"y\");\n\tOutputDebugStringA(\"z\"); // J2X-REVIEW\n\tstd::cout << 1;\n\tsnprintf(b, 4, \"%d\", 1);\n}\n";
    const auto f = fire("debug-bypass", "A.cpp", cpp);
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(2));
    CB_CHECK(f[0].line == 3 && f[0].snippet == "printf(\"x\");" && f[0].severity == Severity::Low);
    CB_CHECK(f[1].line == 6 && f[1].snippet == "std::cout << 1;");
    CB_CHECK(fire("debug-bypass", "Debug.cpp", cpp).empty());   // the backend itself
    CB_CHECK(fire("debug-bypass", "A.h", cpp).empty());
    CB_CHECK_EQ(fire("debug-bypass", "A.cpp", "printf(\"first line\");\n").size(), static_cast<std::size_t>(1));   // on the first line of the file
});

// ---- leaks -----------------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.leak_temp_new", [] {
    const std::string base = "void A::f()\n{\n\tRandom* r = new Random();\n\tr->nextInt(5);\n";
    auto f = fire("leak-temp-new", "A.cpp", base + "}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && f[0].severity == Severity::Med && mentions(f[0], "`r = new ...`"));
    CB_CHECK_EQ(f[0].snippet, "Random* r = new");
    CB_CHECK(fire("leak-temp-new", "A.cpp", base + "\tdelete r;\n}\n").empty());
    CB_CHECK(fire("leak-temp-new", "A.cpp", base + "\treturn r;\n}\n").empty());
    CB_CHECK(fire("leak-temp-new", "A.cpp", base + "\tthis->rand = r;\n}\n").empty());
    CB_CHECK(fire("leak-temp-new", "A.cpp", base + "\tlist->add(r);\n}\n").empty());
    CB_CHECK(fire("leak-temp-new", "A.cpp", base + "\tm.put(k, r);\n}\n").empty());
    CB_CHECK_EQ(fire("leak-temp-new", "A.cpp", base + "\treturn r->next();\n}\n").size(), static_cast<std::size_t>(1));   // a receiver does not transfer ownership
    CB_CHECK_EQ(fire("leak-temp-new", "A.cpp", base + "\tif (a == r) x();\n}\n").size(), static_cast<std::size_t>(1));      // a comparison is not a store
    CB_CHECK_EQ(fire("leak-temp-new", "A.cpp", base + "\tfoo(r);\n}\n").size(), static_cast<std::size_t>(1));               // nor is passing it on
    // one finding per function, in each function
    CB_CHECK_EQ(fire("leak-temp-new", "A.cpp", base + "\tA* s = new A();\n}\n\nvoid A::g()\n{\n\tB* t = new B();\n}\n").size(), static_cast<std::size_t>(2));
    CB_CHECK(fire("leak-temp-new", "A.cpp", "void A::f()\n{\n\tx = (new Random())->nextInt(2);\n}\n").empty());
});

CB_TEST("java.rules.leak_dtor_member", [] {
    const std::string cpp =
        "Foo::Foo()\n{\n\tbuf = new char[4];\n\tother = new Thing();\n\tThing* tmp = NULL;\n\ttmp = new Thing();\n\tthis->mine = new Mine();\n}\n"
        "\nFoo::~Foo()\n{\n\tdelete other;\n}\n";
    const auto f = fire("leak-dtor-member", "Foo.cpp", cpp);
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(2));   // sorted by member: buf, mine -- `other` is deleted, `tmp` is a local
    CB_CHECK(f[0].line == 3 && mentions(f[0], "member 'buf'") && f[0].severity == Severity::High && f[0].snippet == "buf = new ...");
    CB_CHECK(f[1].line == 7 && mentions(f[1], "member 'mine'") && mentions(f[1], "~Foo()"));
    CB_CHECK(fire("leak-dtor-member", "Foo.cpp", "Foo::Foo()\n{\n\tbuf = new char[4];\n}\n").empty());   // no destructor: rule-of-three's finding
    CB_CHECK(fire("leak-dtor-member", "Foo.cpp", "Foo::Foo()\n{\n\tbuf = new char[4];\n}\n\nFoo::~Foo()\n{\n\tdelete[] buf;\n}\n").empty());
});

CB_TEST("java.rules.leak_drop_no_delete", [] {
    const std::string header = "class A {\n\tJList<Particle*> particles;\n\tJList<Listener*> listeners;\n\tstd::vector<Item*>* items;\n\tJList<int> ints;\n};\n";
    auto f = fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\tparticles.clear();\n}\n", header);
    CB_CHECK(f.size() == 1 && f[0].line == 3 && mentions(f[0], "particles.clear()") && f[0].severity == Severity::Med);
    CB_CHECK(fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\tdelete p;\n\tparticles.erase(it);\n}\n", header).empty());   // a delete nearby
    CB_CHECK(fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\tlisteners.clear();\n}\n", header).empty());              // observers hold borrowed pointers
    CB_CHECK(fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\tints.clear();\n}\n", header).empty());                   // not pointers
    CB_CHECK_EQ(fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\titems->pop_back();\n\tparticles.removeAll(x);\n\tmyparticles.clear();\n}\n", header).size(),
                static_cast<std::size_t>(1));   // `removeAll` and `myparticles` are other names
    CB_CHECK(fire("leak-drop-no-delete", "A.cpp", "void A::f()\n{\n\tparticles.clear();\n}\n", std::nullopt).empty());          // no declaration to read
});

// ---- containers ------------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.ptr_container_compare", [] {
    const auto f = fire("ptr-container-compare", "A.h",
                        "std::set<Entity*> a;\nstd::set<int*> b;\nstd::map<Chunk*, int> c;\nJMap< Chunk * , X> d;\nstd::set<Entity> e;\n"
                        "std::unordered_set<ns::Node*> g; // std::set<Hidden*>\nstd::map<int, Chunk*> h;\n");
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(4));
    CB_CHECK(f[0].line == 1 && mentions(f[0], "std::set<Entity*>"));
    CB_CHECK(f[1].line == 3 && mentions(f[1], "std::map<Chunk*, ...>"));
    CB_CHECK(f[2].line == 4 && f[3].line == 6 && mentions(f[3], "ns::Node"));
    CB_CHECK_EQ(f[0].snippet, "std::set<Entity*> a;");
});

CB_TEST("java.rules.map_read_insert", [] {
    const std::string header = "class A {\n\tstd::map<int, Chunk*> chunks;\n\tJMap<K, V>* cache;\n};\n";
    const auto f = fire("map-read-insert", "A.cpp", "void A::f()\n{\n\tif (chunks[key]) { }\n\tchunks[key] = c;\n\tx = chunks[k2] == 1;\n\ty = cache[3];\n\tz = other[3];\n}\n", header);
    CB_CHECK_EQ(f.size(), static_cast<std::size_t>(3));
    CB_CHECK(f[0].line == 3 && f[0].severity == Severity::High && mentions(f[0], "chunks[...] READ"));
    CB_CHECK(f[1].line == 5 && f[2].line == 6 && mentions(f[2], "cache[...]"));
    CB_CHECK(fire("map-read-insert", "A.cpp", "void A::f()\n{\n\tchunks[key] = c;\n}\n", header).empty());
    CB_CHECK(fire("map-read-insert", "A.cpp", "x = chunks[a];\n", std::nullopt).empty());   // no declaration: not known to be a map
    CB_CHECK_EQ(fire("map-read-insert", "A.cpp", "JMap<int, int> m;\nx = m[1];\n", std::nullopt).size(), static_cast<std::size_t>(1));   // declared in the .cpp itself
});

// ---- throw / dispatch ------------------------------------------------------------------------------------------------------

CB_TEST("java.rules.throw_fallthrough", [] {
    auto f = fire("throw-fallthrough", "A.cpp", "void A::f()\n{\n\tthrow new E();\n\tfoo();\n}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && mentions(f[0], "`throw` does NOT unwind") && f[0].snippet == "throw new E();");
    CB_CHECK(fire("throw-fallthrough", "A.cpp", "void A::f()\n{\n\tif (x)\n\t{\n\t\tthrow new E();\n\t}\n}\n").empty());   // last in its block
    CB_CHECK(fire("throw-fallthrough", "A.cpp", "switch (x)\n{\ncase 1:\n\tthrow e;\ncase 2:\n\tbreak;\n}\n").empty());
    CB_CHECK(fire("throw-fallthrough", "A.cpp", "\tthrow e;\n#endif\n").empty());
    CB_CHECK(fire("throw-fallthrough", "A.cpp", "\tthrows(1);\n\tthrow_it();\n\tfoo();\n").empty());   // other names
    CB_CHECK(fire("throw-fallthrough", "A.cpp", "\tthrow e; // the last statement\n").empty());
    // comment-only lines between are skipped
    CB_CHECK_EQ(fire("throw-fallthrough", "A.cpp", "\tthrow e;\n\t// why\n\n\tfoo();\n").size(), static_cast<std::size_t>(1));
});

CB_TEST("java.rules.silent_dispatch", [] {
    const std::string chain = "void A::f(int pname)\n{\n\tif (pname == GL_FOG_START) x = 1;\n\telse if (pname == GL_FOG_END) x = 2;\n";
    auto f = fire("silent-dispatch", "A.cpp", chain + "}\n");
    CB_CHECK(f.size() == 1 && f[0].line == 3 && f[0].severity == Severity::High && mentions(f[0], "dispatch on 'pname' (2 cases)"));
    CB_CHECK(mentions(f[0], "J2X_UNHANDLED(\"<fn>\", pname)"));
    CB_CHECK_EQ(fire("silent-dispatch", "A.cpp", chain + "\telse if (pname == GL_FOG_MODE) x = 3;\n}\n")[0].message.find("(3 cases)") != std::string::npos, true);
    CB_CHECK(fire("silent-dispatch", "A.cpp", chain + "\telse J2X_UNHANDLED(\"f\", pname);\n}\n").empty());
    CB_CHECK(fire("silent-dispatch", "A.cpp", chain + "\telse J2X_NOOP(\"on purpose\");\n}\n").empty());
    CB_CHECK(fire("silent-dispatch", "A.cpp", chain + "\telse x = 0;\n}\n").empty());   // a terminal else
    CB_CHECK(fire("silent-dispatch", "A.cpp", "if (rc == AAA) x = 1;\n").empty());     // a lone if
    CB_CHECK(fire("silent-dispatch", "A.cpp", "if (rc == AA) x = 1;\nelse if (rc == BB) x = 2;\n").empty());   // constants are three characters or more
    CB_CHECK(fire("silent-dispatch", "A.cpp", "if (a == AAA) x = 1;\nelse if (b == BBB) x = 2;\n").empty());   // different variables
    // an explanatory comment between the branches does not cut the chain
    CB_CHECK_EQ(fire("silent-dispatch", "A.cpp", "if (p == AAA) x = 1;\n// the next case\nelse if (p == BBB) x = 2;\n").size(), static_cast<std::size_t>(1));
    // only the first `if` of a chain is reported
    CB_CHECK_EQ(fire("silent-dispatch", "A.cpp", "if (p == AAA) x = 1;\nelse if (p == BBB) x = 2;\nelse if (p == CCC) x = 3;\n").size(), static_cast<std::size_t>(1));
});

// ---- whole-tree passes -----------------------------------------------------------------------------------------------------

CB_TEST("java.rules.odr_differing_definitions_across_headers", [] {
    cb::fs::ScopedTempDir tmp("cbrules");
    cb::fs::write_text(tmp.path() / "A.h", "class Stream {\n public:\n  int x;\n};\nclass Same {\n int a;\n};\n");
    cb::fs::write_text(tmp.path() / "B.h", "/* docs */ class Stream {\n  int y;\n};\nclass Same {   int  a;\n};\n");
    cb::fs::write_text(tmp.path() / "C.h", "class Same {\n int a;\n};\nstruct Fwd;\nclass Lone : public Base {\n int z;\n};\n");
    cb::java::ledger::Ledger ledger;
    CB_CHECK_EQ(r::scan_odr(ledger, tmp.path()), static_cast<std::size_t>(1));   // identical definitions are legal
    CB_CHECK_EQ(ledger.size(), static_cast<std::size_t>(1));
    const auto& e = ledger.events()[0];
    CB_CHECK(e.file == "A.h" && e.category == Category::Risk && e.severity == Severity::High && e.rule == "odr-duplicate-def" && e.source == "rule");
    CB_CHECK(cb::java::contains(e.message, "class 'Stream' has DIFFERING definitions across 2 headers: A.h, B.h"));
});

CB_TEST("java.rules.override_pairs_come_from_the_whole_hierarchy", [] {
    cb::fs::ScopedTempDir tmp("cbrules");
    cb::fs::write_text(tmp.path() / "Base.h", "class Base {\npublic:\n\tBase();\n\tvirtual void tick();\n};\n");
    cb::fs::write_text(tmp.path() / "Mid.h", "class Mid : public Base {\npublic:\n\tvoid tick();\n\tvoid extra(int a);\n};\n");
    cb::fs::write_text(tmp.path() / "Leaf.h", "class Leaf : public Mid, public Other<int> {\n\tvoid leafOnly() { if (x) { go(); } }\n};\n");
    const r::ClassMethodPairs p = r::build_override_pairs(tmp.path());
    CB_CHECK(p.contains({"Base", "tick"}) && p.contains({"Base", "extra"}) && p.contains({"Mid", "leafOnly"}) && p.contains({"Base", "leafOnly"}));
    CB_CHECK(p.contains({"Mid", "go"}));
    CB_CHECK(!p.contains({"Mid", "tick"}) && !p.contains({"Leaf", "leafOnly"}) && !p.contains({"Base", "Base"}) && !p.contains({"Mid", "if"}));
    CB_CHECK(!p.contains({"Leaf", "tick"}));   // an ancestor's methods flow up, not down
});

CB_TEST("java.rules.scan_rules_runs_every_rule_over_a_converted_tree", [] {
    cb::fs::ScopedTempDir tmp("cbrules");
    // Base declares calc(); Derived overrides it; Foo's constructor calls its own virtuals
    cb::fs::write_text(tmp.path() / "Foo.h", "class Foo : public Base {\npublic:\n\tFoo();\n\tvirtual int calc();\n\tvirtual void unrelated();\n};\n");
    cb::fs::write_text(tmp.path() / "Foo.cpp",
                       "Foo::Foo()\n{\n\tcalc();\n\tunrelated();\n\tbuf = new bool[4];\n}\n\nvoid Foo::f(BigBuf b)\n{\n\tprintf(\"x\");\n}\n");
    cb::fs::write_text(tmp.path() / "Sub.h", "class Sub : public Foo {\npublic:\n\tint calc();\n};\n");
    cb::fs::write_text(tmp.path() / "readme.txt", "new bool[4]\n");
    cb::java::profiles::Profile p;
    p.heavy_types = {"BigBuf"};
    cb::java::ledger::Ledger ledger;
    const std::size_t n = r::scan_rules(ledger, tmp.path(), {.platform = "xbox360", .profile = &p});
    CB_CHECK_EQ(n, ledger.size());
    std::vector<std::string> got;
    for (const auto& e : ledger.events()) got.push_back(e.file + ":" + std::to_string(e.line) + ":" + e.rule);
    // Foo.cpp: the rules in registry order; `unrelated` is not overridden anywhere, so only calc() is a finding
    CB_CHECK_EQ(cb::text::join(got, " "), "Foo.cpp:5:uninit-array Foo.cpp:1:rule-of-three Foo.cpp:8:value-copy-owner Foo.cpp:1:virtual-call-in-ctor Foo.cpp:10:debug-bypass");
    for (const auto& e : ledger.events()) CB_CHECK_EQ(e.source, "rule");
    // the same tree under a profile with no heavy types
    cb::java::ledger::Ledger generic;
    r::scan_rules(generic, tmp.path());
    CB_CHECK_EQ(generic.size(), static_cast<std::size_t>(4));
});

CB_TEST("java.rules.the_registry_and_the_contexts", [] {
    const auto& rules = r::all_rules();
    CB_CHECK_EQ(rules.size(), static_cast<std::size_t>(21));
    for (std::size_t i = 0; i < rules.size(); ++i) {
        for (std::size_t j = i + 1; j < rules.size(); ++j) CB_CHECK(rules[i].id != rules[j].id);
        CB_CHECK(!rules[i].description.empty() && rules[i].check != nullptr);
    }
    CB_CHECK_THROWS_AS(cb::UsageError, (void)r::check("no-such-rule", r::FileInput{"A.cpp", "", std::nullopt}, xbox()));
    const r::Context unknown = r::Context::make("somewhere");
    CB_CHECK(unknown.platform_label == "somewhere" && !unknown.big_endian);
    CB_CHECK(r::Context::make("").platform_label == "unknown");
    CB_CHECK(r::Context::make("xbox360").big_endian);
    CB_CHECK(!r::Context::make("xbox360").override_pairs.has_value());
    // an empty file gives nothing, whatever the rule
    for (const auto& rule : rules) CB_CHECK(rule.check(r::FileInput{"A.cpp", "", std::string_view("")}, xbox()).empty());
});

}  // namespace
