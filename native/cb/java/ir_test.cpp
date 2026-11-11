#include <string>
#include <unordered_set>

#include "cb/core/testing.hpp"
#include "cb/java/ir.hpp"

namespace {

using namespace cb::java::ir;

JType t(std::string name, std::uint32_t dims = 0, std::vector<JType> args = {}) {
    return JType{std::move(name), dims, std::move(args)};
}

CB_TEST("java.ir.jtype_text", [] {
    CB_CHECK_EQ(t("int").to_string(), "int");
    CB_CHECK_EQ(t("int", 2).to_string(), "int[][]");
    CB_CHECK_EQ(t("List", 0, {t("ItemStack")}).to_string(), "List<ItemStack>");
    CB_CHECK_EQ(t("Map", 1, {t("String"), t("List", 0, {t("Integer")})}).to_string(), "Map<String, List<Integer>>[]");
});

CB_TEST("java.ir.jtype_element_and_flags", [] {
    JType grid = t("int", 2);
    CB_CHECK(grid.is_array() && !grid.is_generic());
    CB_CHECK(grid.element() == t("int", 1));
    CB_CHECK(grid.element().element() == t("int"));
    CB_CHECK(!grid.element().element().is_array());
    CB_CHECK(t("int").element() == t("int"));   // a scalar is its own element
    CB_CHECK(t("List", 0, {t("A")}).is_generic());
    CB_CHECK(t("List", 1, {t("A")}).element() == t("List", 0, {t("A")}));   // arguments are kept
});

CB_TEST("java.ir.jtype_equality_and_hash", [] {
    CB_CHECK(t("List", 0, {t("A")}) == t("List", 0, {t("A")}));
    CB_CHECK(t("List", 0, {t("A")}) != t("List", 0, {t("B")}));
    CB_CHECK(t("List", 0, {t("A")}) != t("List", 1, {t("A")}));
    CB_CHECK(t("List") != t("List", 0, {t("A")}));
    CB_CHECK_EQ(t("List", 0, {t("A")}).hash(), t("List", 0, {t("A")}).hash());
    std::unordered_set<JType> seen = {t("int"), t("int", 1), t("List", 0, {t("A")})};
    CB_CHECK(seen.contains(t("int", 1)));
    CB_CHECK(!seen.contains(t("int", 2)));
    seen.insert(t("int"));
    CB_CHECK_EQ(seen.size(), std::size_t{3});
});

CB_TEST("java.ir.access", [] {
    for (Access a : {Access::Public, Access::Protected, Access::Package, Access::Private}) {
        CB_CHECK(parse_access(to_string(a)) == a);
    }
    CB_CHECK_EQ(std::string(to_string(Access::Package)), "package");
    CB_CHECK(!parse_access("Public"));
    CB_CHECK(!parse_access(""));
});

CB_TEST("java.ir.method_signature", [] {
    Method m;
    m.name = "put";
    m.params = {Param{"k", t("String")}, Param{"v", t("List", 0, {t("Integer")}), true, false}};
    CB_CHECK_EQ(m.arity(), std::size_t{2});
    Signature s = m.signature();
    CB_CHECK_EQ(s.name, "put");
    CB_CHECK_EQ(s.param_types.size(), std::size_t{2});
    CB_CHECK_EQ(s.param_types[1], "List<Integer>");
    CB_CHECK(m.param_types()[0] == t("String"));
    Method overload = m;
    overload.params[0].type = t("int");
    CB_CHECK(overload.signature() != s);                      // overloads differ by parameter types
    Method renamed_params = m;
    renamed_params.params[0].name = "other";
    CB_CHECK(renamed_params.signature() == s);                // parameter names are not part of the key
});

CB_TEST("java.ir.class_queries", [] {
    JClass c;
    c.name = "Entity";
    c.fields = {Field{"x", t("double")}, Field{"COUNT", t("int"), Access::Public, true, true, "0"},
                Field{"y", t("double")}};
    Method ctor;
    ctor.name = "Entity";
    ctor.is_ctor = true;
    Method tick;
    tick.name = "tick";
    tick.return_type = t("void");
    c.methods = {ctor, tick};

    CB_CHECK(c.field("y") == &c.fields[2]);
    CB_CHECK(c.field("nope") == nullptr);
    CB_CHECK_EQ(c.instance_fields().size(), std::size_t{2});
    CB_CHECK_EQ(c.static_fields().size(), std::size_t{1});
    CB_CHECK(c.static_fields()[0] == &c.fields[1]);
    CB_CHECK_EQ(c.constructors().size(), std::size_t{1});
    CB_CHECK(c.method("tick") == &c.methods[1]);
    CB_CHECK(c.method("missing") == nullptr);
    CB_CHECK(!c.owns_heap_field());
    c.fields[0].initializer = "new Vec3D(0, 0, 0)";
    CB_CHECK(c.owns_heap_field());
    c.fields[0].initializer = "newValue";   // "new " needs the space: an identifier is not an allocation
    CB_CHECK(!c.owns_heap_field());
    c.fields[0].initializer = "";
    CB_CHECK(!c.owns_heap_field());
});

CB_TEST("java.ir.typed_enum", [] {
    // a constant-only enum (EnumMobType) stays int constants
    JClass plain;
    plain.name = "EnumMobType";
    plain.is_enum = true;
    plain.enum_consts = {EnumConst{"everything", {}}, EnumConst{"mobs", {}}, EnumConst{"players", {}}};
    CB_CHECK(!plain.is_typed_enum());

    // constants carrying constructor args are real objects
    JClass args = plain;
    args.enum_consts[0].args = {"0", "32"};
    CB_CHECK(args.is_typed_enum());

    // so is an enum with instance state...
    JClass state = plain;
    state.fields = {Field{"damage", t("int")}};
    CB_CHECK(state.is_typed_enum());
    // ...but a static field alone is not state
    JClass stat = plain;
    stat.fields = {Field{"TABLE", t("int", 1), Access::Public, true}};
    CB_CHECK(!stat.is_typed_enum());

    // ...or behaviour (any non-constructor method); a constructor alone is not
    JClass method = plain;
    Method tick;
    tick.name = "tick";
    method.methods = {tick};
    CB_CHECK(method.is_typed_enum());
    JClass ctor = plain;
    Method c;
    c.name = "EnumMobType";
    c.is_ctor = true;
    ctor.methods = {c};
    CB_CHECK(!ctor.is_typed_enum());

    // a class is never a typed enum, whatever it holds
    JClass cls = args;
    cls.is_enum = false;
    CB_CHECK(!cls.is_typed_enum());
});

CB_TEST("java.ir.derived_members_point_into_other_classes", [] {
    JClass base;
    base.name = "Base";
    base.fields = {Field{"hp", t("int")}};
    Method area;
    area.name = "area";
    area.is_abstract = true;
    JClass iface;
    iface.name = "Shape";
    iface.is_interface = true;
    iface.methods = {area};

    JClass derived;
    derived.name = "Derived";
    derived.parent = "Base";
    derived.inherited_fields = base.instance_fields();
    derived.mi_forwards = {MiForward{iface.method("area"), "Base"}};
    CB_CHECK(derived.inherited_fields[0] == &base.fields[0]);
    CB_CHECK_EQ(derived.mi_forwards[0].interface_method->name, "area");
    CB_CHECK_EQ(derived.mi_forwards[0].base_class, "Base");
});

}  // namespace
