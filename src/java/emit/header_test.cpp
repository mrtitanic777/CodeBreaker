#include "java/emit/header.hpp"

#include "core/testing.hpp"
#include "core/text.hpp"
#include "java/parse.hpp"

namespace {

namespace hd = cb::java::emit::header;
namespace ir = cb::java::ir;
namespace types = cb::java::types;
namespace ledger = cb::java::ledger;

types::TypeResolver sample_resolver() {
    types::ResolverData data;
    data.known = {"Thing", "Item", "Vec3D", "Base", "Tickable", "Mat", "Holder", "Widget", "Named"};
    data.values = {"Vec3D"};
    data.interfaces = {"Tickable", "Named"};
    data.enums = {"Mat"};
    data.typed_enums = {"Mat"};
    return types::TypeResolver(std::move(data));
}

std::string render(std::string_view java, const hd::Options& options = {}) {
    return hd::render_header(cb::java::parse::parse_source(java), sample_resolver(), options);
}

std::string includes_header() {
    return "#include \"JavaString.h\"\n#include \"JArray.h\"\n#include \"JList.h\"\n#include \"JSet.h\"\n#include \"JMap.h\"\n";
}

CB_TEST("java.header.ordinary_class", [] {
    hd::Options options;
    options.platform_label = "xbox360";
    const std::string h = render(R"(package a;
public class Widget extends Base implements Tickable {
    private int count;
    protected Thing owner;
    private Vec3D pos;
    private String name;
    public static int TOTAL;
    public Widget(int count) { this.count = count; }
    public void tick() { }
    public abstract Thing find(Item i, String s);
    public static Vec3D origin() { return null; }
}
)", options);
    // hand-checked against the rules: parent, interfaces and the by-value Vec3D are included, reference types only
    // forward-declared; statics get the s_ prefix; the abstract method ends in `= 0`; origin() returns null, so the
    // by-value Vec3D return is promoted to a pointer
    CB_CHECK_EQ(h,
                "#ifndef WIDGET_H\n"
                "#define WIDGET_H\n"
                "\n"
                "// Auto-generated from Widget.java\n"
                "// Target: xbox360 / C++03\n"
                "\n" + includes_header() +
                "#include \"Base.h\"\n"
                "#include \"Tickable.h\"\n"
                "#include \"Vec3D.h\"\n"
                "\n"
                "class Item;\n"
                "class Thing;\n"
                "\n"
                "class Widget : public Base, public Tickable {\n"
                "public:\n"
                "\tWidget(int count);\n"
                "\tvirtual ~Widget();\n"
                "\n"
                "\tint count;\n"
                "\tThing* owner;\n"
                "\tVec3D pos;\n"
                "\tJavaString name;\n"
                "\tstatic int s_TOTAL;\n"
                "\n"
                "\tvirtual void tick();\n"
                "\tvirtual Thing* find(Item* i, const JavaString& s) = 0;\n"
                "\tstatic Vec3D* origin();\n"
                "\n"
                "\tstatic void initStatics();\n"
                "};\n"
                "\n"
                "#endif // WIDGET_H");
});

CB_TEST("java.header.interface_gets_a_default_to_string", [] {
    const std::string h = render("public interface Named { String name(); void rename(String s); }");
    CB_CHECK_EQ(h,
                "#ifndef NAMED_H\n"
                "#define NAMED_H\n"
                "\n"
                "// Auto-generated from Named.java\n"
                "// Target: unknown / C++03\n"
                "\n" + includes_header() +
                "class Named {\n"
                "public:\n"
                "\n"
                "\tvirtual JavaString name() = 0;\n"
                "\tvirtual void rename(const JavaString& s) = 0;\n"
                "\tvirtual JavaString toString() { return JavaString(); }\n"
                "};\n"
                "\n"
                "#endif // NAMED_H");
});

CB_TEST("java.header.constant_enum", [] {
    ledger::Ledger led;
    hd::Options options;
    options.ledger = &led;
    const std::string h = render("public enum Color { RED, GREEN, NULL }", options);
    CB_CHECK_EQ(h,
                "#ifndef COLOR_H\n"
                "#define COLOR_H\n"
                "\n"
                "// Auto-generated from Color.java\n"
                "// Target: unknown / C++03\n"
                "\n"
                "struct Color {\n"
                "\tstatic const int RED = 0;\n"
                "\tstatic const int GREEN = 1;\n"
                "\tstatic const int NULL_ = 2;\n"   // NULL is a macro: the member gets a trailing underscore
                "\tstatic const int _J2X_COUNT = 3;\n"
                "};\n"
                "\n"
                "#endif // COLOR_H");
    CB_CHECK_EQ(led.size(), std::size_t{1});
    CB_CHECK_EQ(led.events()[0].rule, "enum");
    CB_CHECK_EQ(led.events()[0].file, "Color.h");
});

CB_TEST("java.header.typed_enum", [] {
    ledger::Ledger led;
    hd::Options options;
    options.ledger = &led;
    const std::string h = render(R"(public enum Mat {
    WOOD(1), STONE(2);
    private int hardness;
    Mat(int h) { hardness = h; }
    public int getHardness() { return hardness; }
}
)", options);
    CB_CHECK_EQ(h,
                "#ifndef MAT_H\n"
                "#define MAT_H\n"
                "\n"
                "// Auto-generated from Mat.java\n"
                "// Target: unknown / C++03\n"
                "\n" + includes_header() +
                "\n"
                "class Mat {\n"
                "public:\n"
                "\tMat(int h);\n"
                "\tvirtual ~Mat();\n"
                "\n"
                "\tint __ordinal;   // Java enum ordinal (0-based, declaration order)\n"
                "\tint hardness;\n"
                "\n"
                "\tstatic Mat* WOOD;\n"
                "\tstatic Mat* STONE;\n"
                "\n"
                "\tvirtual int getHardness();\n"
                "\tint ordinal();\n"
                "\tstatic JArray<Mat* > _j2x_values;\n"
                "\tstatic JArray<Mat* >& values();\n"
                "\tstatic void initStatics();\n"
                "};\n"
                "\n"
                "#endif // MAT_H");
    CB_CHECK_EQ(led.size(), std::size_t{1});
    CB_CHECK_EQ(led.events()[0].rule, "typed-enum");
    CB_CHECK_EQ(led.events()[0].message, "typed enum Mat -> class with 2 singleton constants + preserved fields/methods");
});

CB_TEST("java.header.two_phase_rule_of_three_and_ledger", [] {
    ledger::Ledger led;
    hd::Options options;
    options.ledger = &led;
    options.cpp_std = hd::CppStd::of_year(2011);
    options.two_phase = {"Holder"};
    options.unsigned_byte_scalar_fields = {{"Holder", "flag"}};
    const std::string h = render(R"(public class Holder {
    private Thing thing = new Thing();
    byte flag;
    byte other;
    public Holder(Thing t) { }
    public Holder() { }
}
)", options);
    CB_CHECK(h.find("// Target: unknown / C++11\n") != std::string::npos);
    CB_CHECK(h.find("class Thing;\n") != std::string::npos);
    CB_CHECK(h.find("class Holder {\npublic:\n"
                    "\tHolder();\n"
                    "\tHolder* j2xInit(Thing* t);\n"
                    "\tHolder* j2xInit();\n"
                    "\tvirtual ~Holder();\n"
                    "\n"
                    "\tThing* thing;\n"
                    "\tunsigned char flag;\n"
                    "\tsigned char other;\n"
                    "\n"
                    "\tstatic void initStatics();\n"
                    "\n"
                    "private:\n"
                    "\t// owns heap; copying disabled so a copy cannot double-free (Rule of Three)\n"
                    "\tHolder(const Holder&);\n"
                    "\tHolder& operator=(const Holder&);\n"
                    "};\n") != std::string::npos);
    CB_CHECK_EQ(led.size(), std::size_t{2});
    CB_CHECK(led.events()[0].category == ledger::Category::Risk);
    CB_CHECK(led.events()[0].severity == ledger::Severity::Med);
    CB_CHECK_EQ(led.events()[0].rule, "rule-of-three-nocopy");
    CB_CHECK(led.events()[1].category == ledger::Category::Ported);
    CB_CHECK_EQ(led.events()[1].message, "generated header for Holder");
});

CB_TEST("java.header.default_constructors", [] {
    // a by-value class with only parameterized constructors gets a default one; so does a class with fields and none
    const std::string v = render("public class Vec3D { double x; public Vec3D(double x) { this.x = x; } }");
    CB_CHECK(v.find("\tVec3D(double x);\n\tVec3D();\n\tvirtual ~Vec3D();\n") != std::string::npos);
    const std::string g = render("public class Gadget { int n; }");
    CB_CHECK(g.find("public:\n\tGadget();\n\tvirtual ~Gadget();\n") != std::string::npos);
    const std::string e = render("public class Empty { void f() { } }");
    CB_CHECK(e.find("public:\n\tvirtual ~Empty();\n") != std::string::npos);
});

CB_TEST("java.header.mi_forwards_are_declared_concretely", [] {
    ir::JClass iface = cb::java::parse::parse_source("public interface Sized { int size(Thing t); }");
    ir::JClass cls = cb::java::parse::parse_source("public class Both extends Base implements Sized { }");
    cls.mi_forwards.push_back({&iface.methods[0], "Base"});
    const std::string h = hd::render_header(cls, sample_resolver());
    CB_CHECK(h.find("\n\tstatic void initStatics();\n\n\tvirtual int size(Thing* t);\n};") != std::string::npos);
    CB_CHECK(h.find("class Thing;\n") != std::string::npos);   // the forwarded method's types are dependencies too
});

CB_TEST("java.header.std_header_for", [] {
    CB_CHECK_EQ(hd::std_header_for("std::map<JObject, JObject>").value_or("-"), "\"JavaRT.h\"");
    CB_CHECK_EQ(hd::std_header_for("std::string").value_or("-"), "<string>");
    CB_CHECK_EQ(hd::std_header_for("std::vector<int>").value_or("-"), "<vector>");
    CB_CHECK_EQ(hd::std_header_for("std::map<int, int>").value_or("-"), "<map>");
    CB_CHECK_EQ(hd::std_header_for("std::set<int>").value_or("-"), "<set>");
    CB_CHECK(!hd::std_header_for("JMap<int, int>"));   // JMap.h is always included
    CB_CHECK(!hd::std_header_for("JList<void* >"));
    CB_CHECK(!hd::std_header_for("Thing"));
});

CB_TEST("java.header.base_type_spelling", [] {
    CB_CHECK_EQ(hd::base_type_spelling("Thing"), "Thing");
    CB_CHECK_EQ(hd::base_type_spelling("ArrayList"), "JList<JObject>");
    CB_CHECK_EQ(hd::base_type_spelling("HashMap"), "JMap<JObject, JObject>");
    CB_CHECK_EQ(hd::base_type_spelling("HashSet"), "JSet<JObject>");
});

CB_TEST("java.header.dependencies", [] {
    const ir::JClass cls = cb::java::parse::parse_source(R"(public class Registry extends HashMap {
    private HashMap<String, Thing> things;
    private int[] counts;
    private Item[] items;
    private Vec3D[] boxes;
    public Registry self() { return null; }
}
)");
    const hd::Dependencies deps = hd::collect_dependencies(cls, sample_resolver());
    // the HashMap parent is an STL base needing JavaRT.h; the generic's elements are still declared; an array of a
    // by-value type is only forward-declared (the array is a handle); the class never lists itself
    CB_CHECK(deps.system_includes == std::set<std::string>{"\"JavaRT.h\""});
    CB_CHECK(deps.includes.empty());
    CB_CHECK(deps.forward_decls == (std::set<std::string>{"Item", "Thing", "Vec3D"}));
});

CB_TEST("java.header.banner", [] {
    CB_CHECK_EQ(hd::banner("A", std::nullopt, ""), "// Auto-generated from A.java\n// Target: unknown / C++03");
    CB_CHECK_EQ(hd::banner("A", hd::CppStd::of_year(2017), "pc"), "// Auto-generated from A.java\n// Target: pc / C++17");
    CB_CHECK_EQ(hd::CppStd::of_year(2003).label, "C++03");
});

}  // namespace
