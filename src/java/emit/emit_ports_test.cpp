// Header and source emitter checks (the native header and source tests are goldens of other inputs): hand-built classes, one resolver,
// the substrings wanted. Each class is the Java source that parses to it -- the parser's own bodies are the raw lines the emitter reads. The source emitter's bodies come from
// the deterministic stand-in of the converter's tests (they are not what these checks are about: the skeleton around them is).
#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/emit/header.hpp"
#include "java/emit/source.hpp"
#include "java/emit/source_test_support.hpp"
#include "java/ledger.hpp"
#include "java/parse.hpp"
#include "java/types.hpp"

namespace {

namespace hd = cb::java::emit::header;
namespace src = cb::java::emit::source;
namespace ir = cb::java::ir;
namespace types = cb::java::types;
namespace ledger = cb::java::ledger;

types::TypeResolver ep_resolver(std::initializer_list<const char*> known, std::initializer_list<const char*> values = {}) {
    types::ResolverData data;
    for (const char* k : known) data.known.insert(k);
    for (const char* v : values) {
        data.values.insert(v);
        data.known.insert(v);
    }
    return types::TypeResolver(std::move(data));
}

bool ep_has(const std::string& text, std::string_view part) { return text.find(part) != std::string::npos; }

std::string ep_header(std::string_view java, const types::TypeResolver& r, hd::Options options = {}) {
    return hd::render_header(cb::java::parse::parse_source(java), r, options);
}

bool ep_event(const ledger::Ledger& led, std::string_view rule) {
    for (const ledger::Event& e : led.events())
        if (e.rule == rule) return true;
    return false;
}

std::filesystem::path ep_java(const char* name) { return cb::testing::data_path(std::string("java/testdata/parse/") + name); }

// ---- hand-built headers ---------------------------------------------------------------------------------------------------

constexpr const char* kFoo = R"(public class Foo extends Bar {
    public Block field;
    public int count;
    static Block[] list;
    static String name;
    public Foo(int id) { }
    public void doIt(Block b) { }
})";

std::string ep_foo_header() { return ep_header(kFoo, ep_resolver({"Block", "Bar"})); }

}  // namespace

CB_TEST("java.emit_ports.header_include_guard", [] {
    const std::string h = ep_foo_header();
    CB_CHECK(h.starts_with("#ifndef FOO_H\n#define FOO_H"));
    CB_CHECK(cb::text::rtrim(h).ends_with("#endif // FOO_H"));
});

CB_TEST("java.emit_ports.header_parent_included_not_forward_declared", [] {
    const std::string h = ep_foo_header();
    CB_CHECK(ep_has(h, "#include \"Bar.h\""));
    CB_CHECK(!ep_has(h, "class Bar;"));
});

CB_TEST("java.emit_ports.header_reference_member_forward_declared", [] { CB_CHECK(ep_has(ep_foo_header(), "class Block;")); });
CB_TEST("java.emit_ports.header_class_declaration_with_base", [] { CB_CHECK(ep_has(ep_foo_header(), "class Foo : public Bar {")); });

CB_TEST("java.emit_ports.header_constructor_and_destructor", [] {
    const std::string h = ep_foo_header();
    CB_CHECK(ep_has(h, "Foo(int id);"));
    CB_CHECK(ep_has(h, "virtual ~Foo();"));
});

CB_TEST("java.emit_ports.header_fields", [] {
    const std::string h = ep_foo_header();
    CB_CHECK(ep_has(h, "\tBlock* field;"));                       // reference instance field -> pointer
    CB_CHECK(ep_has(h, "\tint count;"));
    CB_CHECK(ep_has(h, "\tstatic JArray<Block* > s_list;"));      // ref array -> JArray<Block*>, s_ prefix
    CB_CHECK(ep_has(h, "\tstatic JavaString s_name;"));           // String -> JavaString
});

CB_TEST("java.emit_ports.header_string_pulls_its_header", [] { CB_CHECK(ep_has(ep_foo_header(), "#include \"JavaString.h\"")); });
CB_TEST("java.emit_ports.header_method_is_virtual", [] { CB_CHECK(ep_has(ep_foo_header(), "virtual void doIt(Block* b);")); });

// ---- TestHeaderRealFile ------------------------------------------------------------------------------------------------------------------

CB_TEST("java.emit_ports.vec3d_header_structure", [] {
    ledger::Ledger led;
    hd::Options options;
    options.ledger = &led;
    const std::string h = hd::render_header(cb::java::parse::parse_file(ep_java("Vec3D.java")), ep_resolver({}, {"Vec3D"}), options);
    CB_CHECK(ep_has(h, "#ifndef VEC3D_H"));
    CB_CHECK(ep_has(h, "class Vec3D"));
    CB_CHECK(ep_has(h, "double xCoord;"));   // a value-type field stays bare (Alpha: double coordinates)
    CB_CHECK(ep_has(h, "virtual ~Vec3D();"));
    bool ctor_line = false;
    for (const std::string_view line : cb::text::split(h, '\n'))
        if (cb::text::trim(line).starts_with("Vec3D(")) ctor_line = true;
    CB_CHECK(ctor_line);
    CB_CHECK(ep_has(h, "#endif // VEC3D_H"));
    CB_CHECK(ep_event(led, "header"));   // the emitter reported a PORTED event for the header
});

// ---- TestSignedByteFields: Java `byte` is SIGNED, so every scalar byte field is `signed char`; `byte[]` buffers stay unsigned ---------------------------

CB_TEST("java.emit_ports.a_scalar_byte_is_signed_char_and_a_buffer_stays_unsigned", [] {
    const std::string h = ep_header("public class Packet1Login { public byte dimension; public byte[] payload; }", ep_resolver({}));
    CB_CHECK(ep_has(h, "signed char dimension;"));            // nether = -1
    CB_CHECK(!ep_has(h, "unsigned char dimension;"));
    CB_CHECK(ep_has(h, "unsigned char"));                     // byte[] payload buffer stays unsigned
    CB_CHECK(!ep_has(h, "signed char payload"));
});

CB_TEST("java.emit_ports.any_scalar_byte_is_signed_by_default", [] {
    CB_CHECK(ep_has(ep_header("public class SomeClass { public byte flags; }", ep_resolver({})), "signed char flags;"));   // the flipped default
});

CB_TEST("java.emit_ports.the_unsigned_escape_hatch", [] {
    hd::Options options;
    options.unsigned_byte_scalar_fields.insert({"SomeClass", "raw"});
    CB_CHECK(ep_has(ep_header("public class SomeClass { public byte raw; }", ep_resolver({}), options), "unsigned char raw;"));
});

// ---- source safety -----------------------------------------------------------------------------------------------

namespace {

constexpr const char* kBlock = R"(public class Block {
    public static boolean[] tickOnLoad = new boolean[256];
    public static Block[] blocksList = new Block[256];
    public static Block stone;
    public int blockID;
    StepSound stepSound;
    float slipperiness;
    Block(int var1, Material var2) {
        this.stepSound = new StepSound();
        this.blockID = var1;
    }
    Block(int var1, int var2, Material var3) {
        this(var1, var3);
        this.blockID = var2;
    }
    int getBlockID() {
        return this.blockID;
    }
})";

types::TypeResolver ep_block_resolver() { return ep_resolver({"Block", "StepSound", "Material"}); }

std::string ep_block_source(ledger::Ledger* led = nullptr) {
    src::FakeBodies bodies;
    src::Options options;
    options.bodies = &bodies;
    options.ledger = led;
    return src::render_source(cb::java::parse::parse_source(kBlock), ep_block_resolver(), options).text.value_or("");
}

}  // namespace

CB_TEST("java.emit_ports.source_header_includes", [] {
    const std::string cpp = ep_block_source();
    CB_CHECK(cpp.starts_with("#include \"stdafx.h\""));
    CB_CHECK(ep_has(cpp, "#include \"Block.h\""));
});

CB_TEST("java.emit_ports.a_forward_declared_static_is_nulled", [] { CB_CHECK(ep_has(ep_block_source(), "Block* Block::s_stone = NULL;")); });

CB_TEST("java.emit_ports.a_constructor_zero_inits_members", [] {
    const std::string cpp = ep_block_source();
    CB_CHECK(ep_has(cpp, "blockID(0)"));
    CB_CHECK(ep_has(cpp, "stepSound(NULL)"));
    CB_CHECK(ep_has(cpp, "slipperiness(0)"));
});

CB_TEST("java.emit_ports.the_rule_of_three_destructor_frees_heap", [] {
    ledger::Ledger led;
    const std::string cpp = ep_block_source(&led);
    CB_CHECK(ep_has(cpp, "Block::~Block()"));
    CB_CHECK(ep_has(cpp, "delete stepSound;"));
    CB_CHECK(ep_has(cpp, "stepSound = NULL;"));
    CB_CHECK(ep_event(led, "rule-of-three-dtor"));
});

CB_TEST("java.emit_ports.a_method_signature", [] { CB_CHECK(ep_has(ep_block_source(), "int Block::getBlockID()")); });

// ---- TestRuleOfThreeNoCopy: owning classes forbid copying (no double free) ---------------------------------------------------------------------

CB_TEST("java.emit_ports.an_owning_class_disables_copy", [] {
    ledger::Ledger led;
    hd::Options options;
    options.ledger = &led;
    const std::string h = hd::render_header(cb::java::parse::parse_source(kBlock), ep_block_resolver(), options);
    CB_CHECK(ep_has(h, "private:"));
    CB_CHECK(ep_has(h, "Block(const Block&);"));
    CB_CHECK(ep_has(h, "Block& operator=(const Block&);"));
    CB_CHECK(ep_event(led, "rule-of-three-nocopy"));
});

CB_TEST("java.emit_ports.a_value_type_keeps_copy", [] {
    const std::string h = hd::render_header(cb::java::parse::parse_file(ep_java("Vec3D.java")), ep_resolver({}, {"Vec3D"}));   // owns no heap
    CB_CHECK(!ep_has(h, "Vec3D(const Vec3D&);"));
    CB_CHECK(!ep_has(h, "private:"));
});

// arrays are non-owning JArray<T> values now, so an array-element assignment does not force copy-disable (a shallow copy shares the buffer)
CB_TEST("java.emit_ports.an_array_field_does_not_disable_copy", [] {
    const std::string h = ep_header("public class FX { public List[] fxLayers; FX() { this.fxLayers[0] = new ArrayList(); } }", ep_resolver({}));
    CB_CHECK(!ep_has(h, "FX(const FX&);"));
});

// a real pointer field new'd in the constructor still owns heap -> copy disabled
CB_TEST("java.emit_ports.a_pointer_field_owner_disables_copy", [] {
    const std::string h = ep_header("public class W { public WorldProvider prov; W() { this.prov = new WorldProvider(); } }", ep_resolver({"WorldProvider"}));
    CB_CHECK(ep_has(h, "W(const W&);"));
});

// ---- TestSourceRealFiles -----------------------------------------------------------------------------------------------------------------

CB_TEST("java.emit_ports.the_vec3d_source_renders", [] {
    src::FakeBodies bodies;
    src::Options options;
    options.bodies = &bodies;
    const std::string cpp = src::render_source(cb::java::parse::parse_file(ep_java("Vec3D.java")), ep_resolver({}, {"Vec3D"}), options).text.value_or("");
    CB_CHECK(ep_has(cpp, "#include \"stdafx.h\""));
    CB_CHECK(ep_has(cpp, "Vec3D::Vec3D("));
    CB_CHECK(ep_has(cpp, "Vec3D::~Vec3D()"));
});

CB_TEST("java.emit_ports.an_interface_has_no_source", [] {
    CB_CHECK(!src::render_source(cb::java::parse::parse_file(ep_java("ICamera.java")), ep_resolver({})).text.has_value());
});

// ---- TestAggregateArrayStatic: `static T[] x = new T[]{a, b}` -> a JArray over a file-static data array --------------------------------------------

CB_TEST("java.emit_ports.a_static_aggregate_array_is_a_jarray_static", [] {
    const std::string h = ep_header("public class ItemAxe { public static final Block[] blocksEffectiveAgainst = new Block[]{Block.planks, Block.wood}; }",
                                    ep_resolver({"Block", "ItemAxe"}));
    CB_CHECK(ep_has(h, "static JArray<Block* > s_blocksEffectiveAgainst;"));
});

CB_TEST("java.emit_ports.a_nested_static_aggregate_array_is_a_nested_jarray", [] {
    const char* java = "public class ModelBed { public static final int[][] bedDirection = new int[][]{{1, 0}, {3, 2}}; }";
    const types::TypeResolver r = ep_resolver({"ModelBed"});
    CB_CHECK(ep_has(ep_header(java, r), "static JArray<JArray<int > > s_bedDirection;"));
    src::FakeBodies bodies;
    src::Options options;
    options.bodies = &bodies;
    const std::string cpp = src::render_source(cb::java::parse::parse_source(java), r, options).text.value_or("");
    CB_CHECK(ep_has(cpp, "s_bedDirection"));   // (that no braced new[][] survives -- C2087 -- is the translator's: java.emitted.a_nested_static_aggregate_array_is_not_a_braced_new)
});

// ---- TestValueTypeDefaultCtor: only parameterised constructors -> a synthesised zero-init default, so it can be held by value ------------------------

namespace {
constexpr const char* kAabb = "public class AxisAlignedBB { public double minX; public Vec3D hit; AxisAlignedBB(double var1) { this.minX = var1; } }";
}

CB_TEST("java.emit_ports.a_value_type_header_declares_a_default_constructor", [] {
    CB_CHECK(ep_has(ep_header(kAabb, ep_resolver({}, {"AxisAlignedBB", "Vec3D"})), "AxisAlignedBB();"));
});

CB_TEST("java.emit_ports.a_value_type_source_defines_the_zero_init_default_constructor", [] {
    src::FakeBodies bodies;
    src::Options options;
    options.bodies = &bodies;
    const std::string cpp = src::render_source(cb::java::parse::parse_source(kAabb), ep_resolver({}, {"AxisAlignedBB", "Vec3D"}), options).text.value_or("");
    CB_CHECK(ep_has(cpp, "AxisAlignedBB::AxisAlignedBB()"));
    CB_CHECK(ep_has(cpp, "minX(0)"));   // zero-inits primitive members
});

CB_TEST("java.emit_ports.no_default_constructor_is_made_when_one_exists", [] {
    const std::string h = ep_header("public class Vec3D { Vec3D() { } }", ep_resolver({}, {"Vec3D"}));
    std::size_t count = 0, at = 0;
    while ((at = h.find("\tVec3D();", at)) != std::string::npos) ++count, ++at;
    CB_CHECK_EQ(count, std::size_t{1});   // the parsed constructor only; not duplicated
});

// ---- TestNullableValueParams: a value-struct param used as a nullable reference becomes a pointer in the signature -------------------------------

CB_TEST("java.emit_ports.a_nullable_value_param_is_a_pointer", [] {
    const std::string h = ep_header("public class AxisAlignedBB { boolean isVecInYZ(Vec3D var1) { return var1 == null ? false : var1.yCoord > this.minY; } }",
                                    ep_resolver({}, {"Vec3D"}));
    CB_CHECK(ep_has(h, "virtual bool isVecInYZ(Vec3D* var1);"));
});

CB_TEST("java.emit_ports.a_non_nullable_value_param_stays_a_value", [] {
    const std::string h = ep_header("public class Vec3D { float distance(Vec3D var1) { return var1.xCoord - this.xCoord; } }", ep_resolver({}, {"Vec3D"}));
    CB_CHECK(ep_has(h, "virtual float distance(Vec3D var1);"));
});

// ---- TestJavaStringMapping -----------------------------------------------------------------------------------------------------------------

CB_TEST("java.emit_ports.the_header_includes_javastring_and_uses_it", [] {
    const std::string h = ep_header("public class C { public String name; String greet(String who) { return who; } }", ep_resolver({}));
    CB_CHECK(ep_has(h, "#include \"JavaString.h\""));
    CB_CHECK(ep_has(h, "JavaString name;"));                                    // field by value
    CB_CHECK(ep_has(h, "virtual JavaString greet(const JavaString& who);"));    // return value, param const&
});

// ---- TestTransitiveIncludes: a .cpp that holds a T and reaches T's field/method types through chains includes their definitions (one level) -------

CB_TEST("java.emit_ports.the_field_type_of_a_referenced_class_is_included", [] {
    types::ResolverData data;
    for (const char* k : {"World", "ItemHoe", "Random"}) data.known.insert(k);
    types::MemberEnv world;
    world.fields.emplace("random", ir::JType{"Random", 0, {}});   // World has a Random* field `random` (as the pipeline would build it)
    data.members.emplace("World", world);
    src::FakeBodies bodies;
    src::Options options;
    options.bodies = &bodies;
    const std::string cpp = src::render_source(cb::java::parse::parse_source("public class ItemHoe { void use(World var2) { int n = var2.random.nextInt(8); } }"),
                                               types::TypeResolver(std::move(data)), options).text.value_or("");
    CB_CHECK(ep_has(cpp, "#include \"World.h\""));
    CB_CHECK(ep_has(cpp, "#include \"Random.h\""));   // transitively, via World's field type
});

// ---- TestStringReturnUnification: String is always returned by value, never promoted to a pointer ----------------------------------------------

CB_TEST("java.emit_ports.a_literal_string_return_is_a_value", [] {
    const std::string h = ep_header("public class Mob extends Entity { String getTexture() { return \"/mob/char.png\"; } }", ep_resolver({"Entity"}));
    CB_CHECK(ep_has(h, "virtual JavaString getTexture();"));
});
