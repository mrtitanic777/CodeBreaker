#include "cb/core/testing.hpp"
#include "cb/java/emit/naming.hpp"

namespace {

using namespace cb::java::emit;

CB_TEST("java.emit.naming.fields", [] {
    CB_CHECK_EQ(naming::static_field_name("count"), "s_count");
    CB_CHECK_EQ(naming::static_field_name("_count"), "s_count");           // no reserved `s__name`
    CB_CHECK_EQ(naming::static_field_name("___"), "s_");
    CB_CHECK_EQ(naming::static_field_name(""), "s_");
    CB_CHECK_EQ(naming::static_field_name("a_b"), "s_a_b");                // only LEADING underscores go
    CB_CHECK_EQ(naming::instance_field_name("_x"), "_x");
    CB_CHECK_EQ(naming::field_name("blocksList", true), "s_blocksList");
    CB_CHECK_EQ(naming::field_name("blocksList", false), "blocksList");
});

CB_TEST("java.emit.naming.guard_macro", [] {
    CB_CHECK_EQ(naming::guard_macro("Vec3D"), "VEC3D_H");
    CB_CHECK_EQ(naming::guard_macro("net.minecraft.Block"), "NET_MINECRAFT_BLOCK_H");
    CB_CHECK_EQ(naming::guard_macro("Map$Entry"), "MAP_ENTRY_H");
    CB_CHECK_EQ(naming::guard_macro(""), "_H");
    // a two-byte character is one character, so one '_' (the old re.sub over code points)
    CB_CHECK_EQ(naming::guard_macro("Caf\xC3\xA9"), "CAF__H");
});

CB_TEST("java.emit.naming.enum_member", [] {
    for (const char* name : {"NULL", "TRUE", "FALSE", "NAN", "INFINITY", "ERROR", "DELETE", "IN", "OUT", "OPTIONAL",
                             "CONST", "VOID", "NEAR", "FAR", "THIS", "INTERFACE", "DOMAIN", "this", "new", "delete",
                             "class", "template", "operator", "default", "private", "public", "protected", "register",
                             "export", "try", "throw", "namespace", "true", "false", "nullptr", "min", "max"}) {
        CB_CHECK(naming::is_reserved_member(name));
        CB_CHECK_EQ(naming::enum_member(name), std::string(name) + "_");
    }
    CB_CHECK_EQ(naming::enum_member("STONE"), "STONE");
    CB_CHECK_EQ(naming::enum_member("Null"), "Null");          // case matters
    CB_CHECK_EQ(naming::enum_member("NULL_"), "NULL_");        // a result is not reserved: no double suffix
    CB_CHECK(!naming::is_reserved_member("MIN"));
    CB_CHECK(!naming::is_reserved_member(""));
});

}  // namespace
