#include "cb/java/parse.hpp"
#include "cb/java/strview.hpp"

#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace {

namespace jp = cb::java::parse;
namespace ir = cb::java::ir;

std::filesystem::path fixture(std::string_view name) {
    return cb::testing::data_path(std::string("java/testdata/parse/") + std::string(name));
}

// What the parser found in a class, one fact per line (the golden of the fixtures reads this form).
std::string describe_class(const ir::JClass& c) {
    std::string out = std::format("class {} package={} parent={} interfaces=[{}] interface={} abstract={} enum={}\n", c.name,
                                  c.package.value_or("-"), c.parent.value_or("-"), cb::text::join(c.interfaces, ","), c.is_interface,
                                  c.is_abstract, c.is_enum);
    for (const ir::EnumConst& e : c.enum_consts) out += std::format("  const {}({})\n", e.name, cb::text::join(e.args, "; "));
    for (const ir::Field& f : c.fields) {
        out += std::format("  field {} {} {} line {} static={} final={} init={}\n", ir::to_string(f.access), f.type.to_string(), f.name, f.line,
                           f.is_static, f.is_final, f.initializer ? "`" + *f.initializer + "`" : "-");
    }
    for (const ir::Method& m : c.methods) {
        std::string params;
        for (const ir::Param& p : m.params) params += (params.empty() ? "" : ", ") + p.type.to_string() + " " + p.name;
        out += std::format("  method {} {} {}({}) line {} static={} abstract={} ctor={} body={}\n", ir::to_string(m.access),
                           m.return_type ? m.return_type->to_string() : "-", m.name, params, m.line, m.is_static, m.is_abstract, m.is_ctor,
                           m.body.size());
    }
    return out;
}

CB_TEST("java.parse.fixtures_match_the_reviewed_golden", [] {
    std::string got;
    for (const char* name : {"AxisAlignedBB", "Block", "Entity", "EntityMob", "EnumOS1", "ICamera", "IInventory", "MathHelper", "Vec3D"}) {
        got += describe_class(jp::parse_file(fixture(std::string(name) + ".java")));
    }
    const std::string golden = cb::text::replace_all(cb::fs::read_text(fixture("fixtures.golden.txt")), "\r\n", "\n");
    CB_CHECK_EQ(got, golden);
});

CB_TEST("java.parse.value_type_header_fields_constructor_and_body", [] {
    const ir::JClass c = jp::parse_file(fixture("Vec3D.java"));
    CB_CHECK_EQ(c.name, "Vec3D");
    CB_CHECK(c.package == std::optional<std::string>("sample"));
    CB_CHECK(!c.is_interface && !c.parent);
    std::vector<std::string> names;
    for (const ir::Field* f : c.instance_fields()) {
        names.push_back(f->name);
        CB_CHECK_EQ(f->type.to_string(), "double");
    }
    CB_CHECK_EQ(cb::text::join(names, ","), "xCoord,yCoord,zCoord");
    CB_CHECK(c.field("vectorList") && c.field("vectorList")->is_static);
    CB_CHECK(c.field("nextVector") && c.field("nextVector")->is_static);
    CB_CHECK_EQ(c.constructors().size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(c.constructors()[0]->arity(), static_cast<std::size_t>(3));
    const ir::Method* sub = c.method("subtract");
    CB_CHECK(sub != nullptr);
    CB_CHECK_EQ(sub->return_type->to_string(), "Vec3D");
    CB_CHECK_EQ(sub->params.size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(sub->params[0].name, "var1");
    CB_CHECK(sub->access == ir::Access::Public);
    CB_CHECK(!sub->body.empty());
    // the body lines are the ORIGINAL ones, with their real numbers
    CB_CHECK_EQ(sub->body.front().number, sub->line);   // the body lines start at the signature line, as the original source has it
    CB_CHECK(cb::java::contains(sub->body.front().text, "subtract"));
    CB_CHECK(cb::java::contains(sub->body[1].text, "return new Vec3D("));
});

CB_TEST("java.parse.members_are_attributed_to_their_own_line", [] {
    // DIVERGES: the Python gave a member the line where the text since the previous member began (the previous
    // member's last line, or earlier past blank lines); the line the member itself starts on is what the ledger wants.
    const ir::JClass c = jp::parse_source("class A {\n\n\tint x;\n\n\n\tvoid f() {\n\t}\n}\n");
    CB_CHECK_EQ(c.field("x")->line, 3u);
    CB_CHECK_EQ(c.method("f")->line, 6u);
});

CB_TEST("java.parse.interface_methods_are_abstract_with_empty_bodies", [] {
    const ir::JClass c = jp::parse_file(fixture("ICamera.java"));
    CB_CHECK(c.is_interface);
    CB_CHECK_EQ(c.methods.size(), static_cast<std::size_t>(2));
    CB_CHECK_EQ(c.methods[0].name, "isBoundingBoxInFrustum");
    CB_CHECK(c.methods[0].is_abstract);
    CB_CHECK(c.methods[0].body.empty());
    CB_CHECK_EQ(c.methods[0].params[0].type.to_string(), "AxisAlignedBB");
});

CB_TEST("java.parse.enum_constants", [] {
    const ir::JClass c = jp::parse_file(fixture("EnumOS1.java"));
    CB_CHECK(c.is_enum);
    std::vector<std::string> names;
    for (const ir::EnumConst& e : c.enum_consts) names.push_back(e.name);
    CB_CHECK_EQ(cb::text::join(names, ","), "linux,solaris,windows,macos,unknown");
    CB_CHECK(!c.is_typed_enum());
});

CB_TEST("java.parse.typed_enum_with_arguments_and_members", [] {
    const ir::JClass c = jp::parse_source(
        "public enum EnumArt {\n"
        "\tKEBAB(\"Kebab\", 16, 16), AZTEC(\"Aztec\", 16, 16);\n"
        "\tpublic final String title;\n"
        "\tprivate EnumArt(String var3, int var4, int var5) {\n"
        "\t\tthis.title = var3;\n"
        "\t}\n"
        "}\n");
    CB_CHECK(c.is_enum && c.is_typed_enum());
    CB_CHECK_EQ(c.enum_consts.size(), static_cast<std::size_t>(2));
    CB_CHECK_EQ(c.enum_consts[0].name, "KEBAB");
    CB_CHECK_EQ(c.enum_consts[0].args.size(), static_cast<std::size_t>(3));
    CB_CHECK_EQ(c.enum_consts[0].args[0], "\"Kebab\"");
    CB_CHECK_EQ(c.enum_consts[1].args[1], "16");
    CB_CHECK(c.field("title") != nullptr);
    CB_CHECK_EQ(c.constructors().size(), static_cast<std::size_t>(1));
    CB_CHECK(c.constructors()[0]->body.size() == 3 && cb::java::contains(c.constructors()[0]->body[1].text, "this.title"));
});

CB_TEST("java.parse.two_constructors_and_a_static_array_with_initializer", [] {
    const ir::JClass c = jp::parse_file(fixture("Block.java"));
    std::vector<std::size_t> arities;
    for (const ir::Method* m : c.constructors()) arities.push_back(m->arity());
    std::sort(arities.begin(), arities.end());
    CB_CHECK(arities == (std::vector<std::size_t>{2, 3}));
    const ir::Field* f = c.field("blocksList");
    CB_CHECK(f != nullptr && f->is_static && f->is_final);
    CB_CHECK(f->type == (ir::JType{"Block", 1, {}}));
    CB_CHECK(f->initializer == std::optional<std::string>("new Block[256]"));
});

CB_TEST("java.parse.forward_declared_static_has_no_initializer", [] {
    const ir::JClass c = jp::parse_source("public class Holder {\n  public static final Block stone;\n  public int x;\n}\n");
    const ir::Field* f = c.field("stone");
    CB_CHECK(f != nullptr && f->is_static);
    CB_CHECK(!f->initializer.has_value());
    CB_CHECK(c.field("x") != nullptr);
});

CB_TEST("java.parse.array_initializer_field_is_not_a_method", [] {
    const ir::JClass c = jp::parse_source(
        "public class G {\n"
        "  public KeyBinding[] keyBindings = new KeyBinding[]{a, b, c};\n"
        "  public void foo() { x(); }\n"
        "}\n");
    const ir::Field* f = c.field("keyBindings");
    CB_CHECK(f != nullptr);
    CB_CHECK_EQ(f->type.to_string(), "KeyBinding[]");
    CB_CHECK(c.method("keyBindings") == nullptr);
    CB_CHECK(c.method("foo") != nullptr);
});

CB_TEST("java.parse.inheritance_and_interfaces", [] {
    const ir::JClass mob = jp::parse_file(fixture("EntityMob.java"));
    CB_CHECK(mob.parent == std::optional<std::string>("Entity"));
    const ir::JClass c = jp::parse_source("public abstract class A extends net.pkg.Base implements java.lang.Runnable, Comparable<A> {\n}\n");
    CB_CHECK(c.is_abstract);
    CB_CHECK(c.parent == std::optional<std::string>("Base"));   // the package qualifier is dropped
    CB_CHECK_EQ(cb::text::join(c.interfaces, "|"), "Runnable|Comparable");
});

CB_TEST("java.parse.static_and_instance_initializer_blocks", [] {
    const ir::JClass c = jp::parse_source(
        "class A {\n"
        "\tstatic int n;\n"
        "\tstatic {\n"
        "\t\tn = 1;\n"
        "\t}\n"
        "\t{\n"
        "\t\tn = 2;\n"
        "\t}\n"
        "\tint x;\n"
        "}\n");
    CB_CHECK_EQ(c.static_inits.size(), static_cast<std::size_t>(1));
    CB_CHECK_EQ(c.inst_inits.size(), static_cast<std::size_t>(1));
    CB_CHECK(c.static_inits[0].size() == 3 && c.static_inits[0][1].text == "\t\tn = 1;");
    CB_CHECK_EQ(c.static_inits[0][1].number, 4u);
    CB_CHECK(c.inst_inits[0][1].text == "\t\tn = 2;");
    CB_CHECK(c.field("x") != nullptr && c.field("n") != nullptr);
});

CB_TEST("java.parse.inner_types_and_comments_are_skipped", [] {
    const ir::JClass c = jp::parse_source(
        "// class Fake { int ghost; }\n"
        "public class A {\n"
        "\t/* int hidden;\n"
        "\t   void gone() {} */\n"
        "\tint shown = 1; // trailing { brace\n"
        "\tpublic static class Inner { int inner; void m() {} }\n"
        "\tString s = \"a;b{c}\";\n"
        "\tchar q = '}';\n"
        "\tvoid after() { }\n"
        "}\n");
    CB_CHECK_EQ(c.name, "A");
    std::vector<std::string> fields, methods;
    for (const ir::Field& f : c.fields) fields.push_back(f.name);
    for (const ir::Method& m : c.methods) methods.push_back(m.name);
    CB_CHECK_EQ(cb::text::join(fields, ","), "shown,s,q");   // no hidden, no ghost, no Inner's members
    CB_CHECK_EQ(cb::text::join(methods, ","), "after");
    CB_CHECK(c.field("s")->initializer == std::optional<std::string>("\"a;b{c}\""));
});

CB_TEST("java.parse.methods_params_modifiers_and_varargs", [] {
    const ir::JClass c = jp::parse_source(
        "public class A {\n"
        "\tprotected final synchronized int[] f(final int a, List<Map<String, Integer>> b, String... rest) {\n"
        "\t\treturn null;\n"
        "\t}\n"
        "\tstatic native void g();\n"
        "\tprivate A(int x) { }\n"
        "\tabstract void h();\n"
        "}\n");
    const ir::Method* f = c.method("f");
    CB_CHECK(f != nullptr);
    CB_CHECK(f->access == ir::Access::Protected && f->is_final && !f->is_static);
    CB_CHECK_EQ(f->return_type->to_string(), "int[]");
    CB_CHECK_EQ(f->params.size(), static_cast<std::size_t>(3));
    CB_CHECK(f->params[0].is_final && f->params[0].name == "a");
    CB_CHECK_EQ(f->params[1].type.to_string(), "List<Map<String, Integer>>");
    CB_CHECK_EQ(f->params[2].type.to_string(), "String[]");   // varargs become an array
    const ir::Method* g = c.method("g");
    CB_CHECK(g != nullptr && g->is_static && g->access == ir::Access::Package && g->return_type->name == "void");
    CB_CHECK(c.constructors().size() == 1 && c.constructors()[0]->access == ir::Access::Private);
    CB_CHECK(c.method("h")->is_abstract);
});

CB_TEST("java.parse.parse_type", [] {
    CB_CHECK(jp::parse_type("int[][]") == (ir::JType{"int", 2, {}}));
    CB_CHECK(jp::parse_type("net.minecraft.Block") == (ir::JType{"Block", 0, {}}));
    CB_CHECK(jp::parse_type(" List<ItemStack> ") == (ir::JType{"List", 0, {ir::JType{"ItemStack", 0, {}}}}));
    CB_CHECK(jp::parse_type("Map<String, List<int[]>>") ==
             (ir::JType{"Map", 0, {ir::JType{"String", 0, {}}, ir::JType{"List", 0, {ir::JType{"int", 1, {}}}}}}));
    CB_CHECK(jp::parse_type("java.util.List<a.B>[]") == (ir::JType{"List", 1, {ir::JType{"B", 0, {}}}}));
    CB_CHECK(jp::parse_type("") == (ir::JType{"", 0, {}}));
});

CB_TEST("java.parse.errors", [] {
    CB_CHECK_THROWS_AS(cb::ParseError, (void)jp::parse_source("// nothing here\nint x;\n"));
    CB_CHECK_THROWS_AS(cb::ParseError, (void)jp::parse_source(""));
    CB_CHECK_THROWS_AS(cb::NotFoundError, (void)jp::parse_file(fixture("Missing.java")));
    // a source that stops in the middle of a class still gives what came before
    const ir::JClass c = jp::parse_source("class A {\n  int x;\n  void f() {");
    CB_CHECK_EQ(c.name, "A");
    CB_CHECK(c.field("x") != nullptr);
});

CB_TEST("java.parse.source_path_is_recorded", [] {
    const ir::JClass c = jp::parse_file(fixture("Entity.java"));
    CB_CHECK(c.source_path.has_value());
    CB_CHECK(cb::fs::same_path(*c.source_path, fixture("Entity.java")));
});

CB_TEST("java.parse.crlf_sources_read_from_a_file_match_lf", [] {
    cb::fs::ScopedTempDir tmp("cbparse");
    const std::string lf = cb::fs::read_text(fixture("Vec3D.java"));
    const std::string no_cr = cb::text::replace_all(lf, "\r\n", "\n");
    cb::fs::write_text(tmp.path() / "Crlf.java", cb::text::replace_all(no_cr, "\n", "\r\n"));
    cb::fs::write_text(tmp.path() / "Lf.java", no_cr);
    CB_CHECK_EQ(describe_class(jp::parse_file(tmp.path() / "Crlf.java")), describe_class(jp::parse_file(tmp.path() / "Lf.java")));
});

}  // namespace
