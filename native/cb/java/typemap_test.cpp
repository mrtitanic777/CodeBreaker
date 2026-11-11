#include <string>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/java/typemap.hpp"

namespace {

using namespace cb::java::typemap;

// testdata/typemap.golden.json: every table of the Python module, dumped by it and sorted (reviewed: the tables
// are data, and the same data is what the C++ must hold).
const cb::Json& golden() {
    static const cb::Json j = cb::json::load(cb::testing::data_path("java/testdata/typemap.golden.json"));
    return j;
}

void check_set(const char* key, const NameSet& actual) {
    const cb::Json& expected = golden().at(key);
    CB_CHECK_EQ(actual.size(), expected.size());
    for (const cb::Json& name : expected) CB_CHECK(actual.contains(name.get<std::string>()));
}

void check_map(const char* key, const NameMap& actual) {
    const cb::Json& expected = golden().at(key);
    CB_CHECK_EQ(actual.size(), expected.size());
    for (const auto& [k, v] : expected.items()) {
        auto it = actual.find(k);
        CB_CHECK(it != actual.end());
        if (it != actual.end()) CB_CHECK_EQ(it->second, v.get<std::string>());
    }
}

CB_TEST("java.typemap.sets_match_python", [] {
    check_set("primitives", primitives());
    check_set("containers", containers());
    check_set("value_stdlib", value_stdlib());
    check_set("boxed_value_types", boxed_value_types());
    check_set("runtime_static_holders", runtime_static_holders());
    check_set("runtime_headers", runtime_headers());
    check_set("javart_statics", javart_statics());
    check_set("runtime_enum_like", runtime_enum_like());
});

CB_TEST("java.typemap.maps_match_python", [] {
    check_map("java_to_cpp", java_to_cpp());
    check_map("port_note_types", port_note_types());
    check_map("raw_container_default", raw_container_default());
    check_map("math_api", math_api());
    check_map("math_api_emit", math_api_emit());
});

CB_TEST("java.typemap.policy_spot_checks", [] {
    // scalar byte is signed, char unsigned (byte[] buffers are made unsigned by types, not here)
    CB_CHECK_EQ(java_to_cpp().at("byte"), "signed char");
    CB_CHECK_EQ(java_to_cpp().at("char"), "unsigned char");
    CB_CHECK_EQ(java_to_cpp().at("Object"), "JObject");
    CB_CHECK_EQ(math_api().at("PI"), "3.141592653589793");   // a double literal
    CB_CHECK_EQ(math_api_emit().at("sin"), "::sin");          // same name: global-qualified
    CB_CHECK_EQ(math_api_emit().at("min"), "min");            // may be an MSVC macro: left bare
    CB_CHECK_EQ(math_api_emit().at("sqrt"), "javaMathSqrt");  // renamed: cannot collide
    CB_CHECK(runtime_enum_like().contains("Color"));
    CB_CHECK(runtime_enum_like().contains("GL11"));
    CB_CHECK(!runtime_headers().contains("System"));          // prelude statics have no header of their own
    CB_CHECK(runtime_headers().contains("GL11"));            // the static holders are included in the headers
});

CB_TEST("java.typemap.profile_defaults_empty", [] {
    TypeProfile p;
    CB_CHECK(p.nullable.empty() && p.nullable_fields.empty() && p.nullable_getters.empty());
    CB_CHECK(p.nullable_getter_return_type.empty() && p.unsigned_byte_scalar_fields.empty());
    CB_CHECK(p.default_value_types.empty());
});

}  // namespace

