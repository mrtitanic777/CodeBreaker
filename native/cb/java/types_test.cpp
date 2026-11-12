#include "cb/java/types.hpp"

#include "cb/core/testing.hpp"

namespace {

using cb::java::ir::JType;
using cb::java::types::Ownership;
using cb::java::types::ResolverData;
using cb::java::types::TypeResolver;
using cb::java::types::Use;

JType ty(std::string name, std::uint32_t dims = 0, std::vector<JType> args = {}) {
    return JType{std::move(name), dims, std::move(args)};
}

TypeResolver ownership_resolver() {
    ResolverData d;
    d.known = {"Block", "World", "Entity", "ItemStack"};
    d.values = {"Vec3D", "AxisAlignedBB"};
    d.interfaces = {"ICamera"};
    d.enums = {"EnumOS"};
    return TypeResolver(std::move(d));
}

TypeResolver spelling_resolver() {
    ResolverData d;
    d.known = {"Block", "Item", "ItemStack", "World"};
    d.values = {"Vec3D"};
    return TypeResolver(std::move(d));
}

CB_TEST("java.types.primitives_are_values", [] {
    const TypeResolver r = ownership_resolver();
    for (const char* n : {"int", "float", "boolean", "long", "char", "double"}) {
        CB_CHECK(r.ownership(ty(n)) == Ownership::Value);
        CB_CHECK(r.is_primitive(ty(n)));
    }
    CB_CHECK(!r.is_primitive(ty("Block")));
});

CB_TEST("java.types.value_and_reference_types", [] {
    const TypeResolver r = ownership_resolver();
    CB_CHECK(r.is_value(ty("Vec3D")));
    CB_CHECK(r.is_value(ty("AxisAlignedBB")));
    CB_CHECK(r.is_value(ty("String")));
    for (const char* n : {"Block", "World", "Entity", "ICamera"}) CB_CHECK(r.is_pointer(ty(n)));
    CB_CHECK(r.is_pointer(ty("SomeNewClass")));   // an unknown capitalized name is a reference
    CB_CHECK(r.is_value(ty("someGenericParam")));   // a stray lowercase name is not
    CB_CHECK(r.is_value(ty("")));
    CB_CHECK_EQ(cb::java::types::to_string(Ownership::Pointer), "pointer");
});

CB_TEST("java.types.enums_and_arrays_are_values", [] {
    const TypeResolver r = ownership_resolver();
    CB_CHECK(r.is_value(ty("EnumOS")));
    CB_CHECK_EQ(r.field_type(ty("EnumOS")), "int");
    CB_CHECK(r.is_value(ty("int", 1)));
    CB_CHECK(r.is_value(ty("Block", 1)));   // JArray<T> is a value wrapper
});

CB_TEST("java.types.typed_enum_is_a_pointer_class", [] {
    ResolverData d;
    d.enums = {"EnumArt", "EnumMobType"};
    d.typed_enums = {"EnumArt"};
    const TypeResolver r(std::move(d));
    CB_CHECK(r.is_pointer(ty("EnumArt")));
    CB_CHECK_EQ(r.field_type(ty("EnumArt")), "EnumArt*");
    CB_CHECK(r.is_value(ty("EnumMobType")));
    CB_CHECK_EQ(r.field_type(ty("EnumMobType")), "int");
});

CB_TEST("java.types.primitive_mapping", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("boolean")), "bool");
    CB_CHECK_EQ(r.field_type(ty("byte")), "signed char");   // a scalar byte is signed (Java-faithful)
    CB_CHECK_EQ(r.field_type(ty("byte", 1)), "JArray<unsigned char >");   // a byte[] buffer stays unsigned
    CB_CHECK_EQ(r.field_type(ty("long")), "long long");
    CB_CHECK_EQ(r.field_type(ty("int")), "int");
    CB_CHECK_EQ(r.field_type(ty("char")), "unsigned char");
});

CB_TEST("java.types.reference_field_gets_a_pointer_value_does_not", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("Block")), "Block*");
    CB_CHECK_EQ(r.field_type(ty("World")), "World*");
    CB_CHECK_EQ(r.field_type(ty("Vec3D")), "Vec3D");
});

CB_TEST("java.types.string_contexts", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("String")), "JavaString");
    CB_CHECK_EQ(r.param_type(ty("String")), "const JavaString&");
    CB_CHECK_EQ(r.return_type(ty("String")), "JavaString");   // by value: no dangling
    CB_CHECK_EQ(r.cpp_type(ty("String"), Use::Local), "JavaString");
    CB_CHECK_EQ(r.cpp_type(ty("String"), Use::Element), "JavaString");
});

CB_TEST("java.types.arrays", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("int", 1)), "JArray<int >");
    CB_CHECK_EQ(r.field_type(ty("boolean", 1)), "JArray<bool >");
    CB_CHECK_EQ(r.field_type(ty("Block", 1)), "JArray<Block* >");
    CB_CHECK_EQ(r.field_type(ty("ItemStack", 1)), "JArray<ItemStack* >");
    CB_CHECK_EQ(r.field_type(ty("Mystery", 1)), "JArray<Mystery* >");   // unknown capitalized = reference
    CB_CHECK_EQ(r.field_type(ty("Vec3D", 1)), "JArray<Vec3D >");
    CB_CHECK_EQ(r.field_type(ty("String", 1)), "JArray<JavaString >");
    CB_CHECK_EQ(r.field_type(ty("int", 2)), "JArray<JArray<int > >");
    CB_CHECK_EQ(r.field_type(ty("Block", 2)), "JArray<JArray<Block* > >");
    CB_CHECK_EQ(r.field_type(ty("Object", 1)), "JArray<JObject >");
    CB_CHECK_EQ(r.field_type(ty("byte", 2)), "JArray<JArray<unsigned char > >");   // a byte[] at any depth is the buffer
});

CB_TEST("java.types.generics_and_raw_containers", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("List", 0, {ty("ItemStack")})), "JList<ItemStack*>");
    CB_CHECK_EQ(r.field_type(ty("List", 0, {ty("int")})), "JList<int>");
    CB_CHECK_EQ(r.field_type(ty("HashMap", 0, {ty("String"), ty("Item")})), "JMap<JavaString, Item*>");
    CB_CHECK_EQ(r.field_type(ty("List", 0, {ty("List", 0, {ty("Block")})})), "JList<JList<Block*>>");
    CB_CHECK_EQ(r.field_type(ty("List")), "JList<JObject>");
    CB_CHECK_EQ(r.field_type(ty("HashMap")), "JMap<JObject, JObject>");
    CB_CHECK_EQ(r.field_type(ty("TreeMap")), "JMap<JObject, JObject>");
    CB_CHECK_EQ(r.field_type(ty("Map")), "JMap<JObject, JObject>");
    CB_CHECK_EQ(r.field_type(ty("HashSet")), "JSet<JObject>");
    CB_CHECK_EQ(r.field_type(ty("Collection")), "JList<JObject>");
    CB_CHECK_EQ(r.field_type(ty("List", 1, {ty("int")})), "JArray<JList<int> >");   // an array of lists
});

CB_TEST("java.types.object_is_a_boxed_value_with_a_port_note", [] {
    const TypeResolver r = spelling_resolver();
    CB_CHECK_EQ(r.field_type(ty("Object")), "JObject");
    CB_CHECK(r.is_value(ty("Object")));
    CB_CHECK(r.port_note(ty("Object")).has_value());
    CB_CHECK(!r.port_note(ty("Block")).has_value());
});

CB_TEST("java.types.method_renames", [] {
    ResolverData d;
    d.method_renames["Entity"] = {"health", "age"};
    const TypeResolver r(std::move(d));
    CB_CHECK_EQ(r.renamed_method("Entity", "health"), "health_");
    CB_CHECK_EQ(r.renamed_method("Entity", "tick"), "tick");
    CB_CHECK_EQ(r.renamed_method("Other", "health"), "health");
});

CB_TEST("java.types.member_environment", [] {
    ResolverData d;
    cb::java::types::MemberEnv world;
    world.fields.emplace("chunks", ty("Chunk", 1));
    world.methods.emplace("getBlock", ty("Block"));
    d.members["World"] = std::move(world);
    const TypeResolver r(std::move(d));
    const JType* f = r.member_field_type("World", "chunks");
    CB_CHECK(f != nullptr && *f == ty("Chunk", 1));
    CB_CHECK(r.member_field_type("World", "nope") == nullptr);
    CB_CHECK(r.member_field_type("Nowhere", "chunks") == nullptr);
    const JType* m = r.member_method_type("World", "getBlock");
    CB_CHECK(m != nullptr && m->name == "Block");
    CB_CHECK(r.member_method_type("World", "chunks") == nullptr);
});

CB_TEST("java.types.extends_value_container_walks_the_parents", [] {
    ResolverData d;
    d.members["J_JsonNodeList"].parent = "List";
    d.members["J_Deeper"].parent = "J_JsonNodeList";
    d.members["J_Map"].parent = "HashMap";
    d.members["Plain"].parent = "Entity";
    d.members["Entity"];   // an entry without a parent
    d.members["LoopA"].parent = "LoopB";
    d.members["LoopB"].parent = "LoopA";
    const TypeResolver r(std::move(d));
    CB_CHECK(r.extends_value_container("J_JsonNodeList"));
    CB_CHECK(r.extends_value_container("J_Deeper"));
    CB_CHECK(r.extends_value_container("J_Map"));
    CB_CHECK(!r.extends_value_container("Plain"));
    CB_CHECK(!r.extends_value_container("Entity"));
    CB_CHECK(!r.extends_value_container("Unknown"));
    CB_CHECK(!r.extends_value_container("LoopA"));   // a cycle in the data ends the walk
    CB_CHECK(!r.extends_value_container(""));
});

CB_TEST("java.types.a_profile_supplies_the_value_types", [] {
    cb::java::typemap::TypeProfile profile;
    profile.default_value_types = {"Vec3D"};
    const TypeResolver r(ResolverData::for_profile(profile));
    CB_CHECK(r.is_value(ty("Vec3D")));
    CB_CHECK_EQ(r.field_type(ty("Vec3D")), "Vec3D");
    CB_CHECK_EQ(r.field_type(ty("AxisAlignedBB")), "AxisAlignedBB*");   // the generic profile: a reference class
    const TypeResolver generic{ResolverData{}};
    CB_CHECK_EQ(generic.field_type(ty("Vec3D")), "Vec3D*");
});

}  // namespace
