// cb-unity: alone (test helpers named for this module)
// Tests of j2x, the Java runtime in C++ read back as Java's library.
//
// The real inputs: the Java lane's C++ for six classes (Vec3D, MathHelper, AxisAlignedBB, Base, Sub, and the RoundTrip driver, whose
// C++ holds every shape the pass maps: JavaString and JCat, JArray, System::out, j2xToInt, sequencing temporaries ...), read by clang
// into L2 (testdata/j2x_<class>.pre.json). testdata/j2x_<class>.post.json is the checked-in result for each unit; the pass must give the same document. testdata/j2x_roundtrip_src is the Java of those classes:
// the whole round trip (the Java lane's C++, read back through this pass, the Java back end, javac and the JVM) is run natively with
// the units read from the goldens, and must give the expected verdict (equivalent, idiomatic tier) and the same Java text
// (testdata/j2x_RoundTripped.java.txt).
//
// Behaviour pinned here:
//   * a symbol is lifted once and its uses keep sharing it;
//   * a class type keeps what the lowering knew of it beyond name and arguments;
//   * the types in a list (`implements`, `throws`, a catch's types, a call's type arguments) are lifted;
//   * `new JArray(n)` of a type that is not an array is left as it is, not made an array creation with no element type.
#include "cb/translate/frontends/clang/j2x.hpp"

#include <algorithm>
#include <map>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/translate/backends/java_libc.hpp"
#include "cb/translate/backends/java_objects.hpp"
#include "cb/translate/l2.hpp"

namespace {

namespace j2x = cb::translate::frontends::clang::j2x;
namespace ir = cb::translate::ir;
namespace jj = cb::translate::judge_native_jvm;
namespace l2 = cb::translate::l2;
namespace stdfs = std::filesystem;
using cb::Arena;
using cb::Json;

const char* const kJ2xClasses[] = {"Vec3D", "MathHelper", "AxisAlignedBB", "Base", "Sub", "RoundTrip"};

// A document without what carries no information (nulls, false flags, empty lists), keys in order.
Json j2xt_normal(const Json& j) {
    if (j.is_object()) {
        std::map<std::string, Json> sorted;
        for (const auto& [k, v] : j.items()) {
            if (v.is_null() || (v.is_boolean() && !v.get<bool>()) || (v.is_array() && v.empty())) continue;
            sorted.emplace(k, j2xt_normal(v));
        }
        Json out = Json::object();
        for (auto& [k, v] : sorted) out[k] = std::move(v);
        return out;
    }
    if (j.is_array()) {
        Json out = Json::array();
        for (const Json& v : j) out.push_back(j2xt_normal(v));
        return out;
    }
    return j;
}

std::string j2xt_difference(const Json& a, const Json& b, const std::string& path) {
    if (a == b) return {};
    if (a.is_object() && b.is_object()) {
        for (const auto& [k, v] : a.items()) {
            if (!b.contains(k)) return path + "/" + k + ": only in the first: " + v.dump().substr(0, 200);
            if (std::string d = j2xt_difference(v, b.at(k), path + "/" + k); !d.empty()) return d;
        }
        for (const auto& [k, v] : b.items()) {
            if (!a.contains(k)) return path + "/" + k + ": only in the second: " + v.dump().substr(0, 200);
        }
    }
    if (a.is_array() && b.is_array()) {
        if (a.size() != b.size()) return path + ": " + std::to_string(a.size()) + " items against " + std::to_string(b.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (std::string d = j2xt_difference(a[i], b[i], path + "/" + std::to_string(i)); !d.empty()) return d;
        }
    }
    return path + ": " + a.dump().substr(0, 200) + " against " + b.dump().substr(0, 200);
}

std::string j2xt_data(const std::string& name) { return "translate/frontends/clang/testdata/" + name; }

Json j2xt_load(const std::string& name) { return cb::json::load(cb::testing::data_path(j2xt_data(name))); }

}  // namespace

CB_TEST("translate.j2x.units_lift_to_the_recorded_document", [] {
    std::size_t changed = 0;
    for (const char* cls : kJ2xClasses) {
        Arena arena;
        const Json pre = j2xt_load(std::string("j2x_") + cls + ".pre.json");
        const Json post = j2xt_normal(j2xt_load(std::string("j2x_") + cls + ".post.json"));
        ir::Node* unit = l2::from_json(arena, pre);
        ir::Node* units[] = {unit};
        j2x::lift(arena, units);
        const Json lifted = j2xt_normal(l2::to_json(*unit));
        CB_CHECK_EQ(j2xt_difference(lifted, post, ""), std::string());
        changed += j2xt_normal(pre) != lifted;
        CB_CHECK(ir::unsupported(unit).empty());
    }
    CB_CHECK_EQ(changed, std::size_t(6));   // (every one had something to lift)
});

CB_TEST("translate.j2x.the_driver_has_no_runtime_left", [] {
    Arena arena;
    ir::Node* unit = l2::from_json(arena, j2xt_load("j2x_RoundTrip.pre.json"));
    ir::Node* units[] = {unit};
    const auto count_temps = [](ir::Node* root) {
        std::size_t n = 0;
        for (ir::Node* node : ir::walk(root)) {
            if (const auto* l = ir::get<ir::Local>(node); l != nullptr && l->sym && l->sym->name.starts_with("j2xo")) ++n;
        }
        return n;
    };
    const std::size_t temps_before = count_temps(unit);
    j2x::lift(arena, units);
    std::size_t concat = 0, println = 0;
    for (ir::Node* n : ir::walk(unit)) {
        if (ir::is<ir::Concat>(n)) ++concat;
        if (const auto* c = ir::get<ir::Call>(n); c != nullptr && c->sym) {
            println += c->sym->owner == "java.io.PrintStream";
            CB_CHECK(c->sym->name != "j2xToInt" && c->sym->name != "j2xUshr");
        }
        if (n->type) CB_CHECK(!(std::holds_alternative<ir::ClassType>(n->type->body) && std::get<ir::ClassType>(n->type->body).name == "JavaString"));
    }
    CB_CHECK(concat > 0);
    CB_CHECK(println > 0);
    CB_CHECK(temps_before > 0);
    CB_CHECK_EQ(count_temps(unit), std::size_t(10));   // (those whose use is not a comma of assignments stay)
});

CB_TEST("translate.j2x.types", [] {
    const ir::TypeP string = ir::class_type("java.lang.String");
    for (const char* name : {"JavaString", "JCat", "std.string", "std.basic_string", "StringBuilder"}) CB_CHECK(j2x::lift_type(ir::class_type(name)) == string);
    CB_CHECK(j2x::lift_type(ir::class_type("JArray", {ir::class_type("JavaString")})) == ir::array_of(string));
    CB_CHECK(j2x::lift_type(ir::class_type("JArray", {ir::class_type("JArray", {ir::i32_type()})})) == ir::array_of(ir::i32_type(), 2));
    CB_CHECK(j2x::lift_type(ir::class_type("System._PrintStream")) == ir::class_type("java.io.PrintStream"));
    // a string literal's char array is the string; any other array keeps its element, lifted
    CB_CHECK(j2x::lift_type(ir::array_of(ir::prim_type(ir::Prim::Char))) == string);
    CB_CHECK(j2x::lift_type(ir::array_of(ir::class_type("JavaString"))) == ir::array_of(string));
    // a pointer to an array or a string is the reference; a pointer to anything else stays a pointer
    CB_CHECK(j2x::lift_type(ir::pointer_to(ir::class_type("JavaString"))) == string);
    CB_CHECK(j2x::lift_type(ir::pointer_to(ir::class_type("JArray", {ir::i32_type()}))) == ir::array_of(ir::i32_type()));
    CB_CHECK(j2x::lift_type(ir::pointer_to(ir::class_type("JavaString"), ir::AddressSpace::Far)) == string);
    CB_CHECK(j2x::lift_type(ir::pointer_to(ir::class_type("Vec3D"))) == ir::pointer_to(ir::class_type("Vec3D")));
    // a class keeps its arguments, lifted, and what else it says of itself
    ir::ClassType list{.name = "std.vector", .args = {ir::class_type("JavaString")}, .nullable = true};
    const ir::TypeP lifted = j2x::lift_type(ir::make_type(list));
    const auto& got = std::get<ir::ClassType>(lifted->body);
    CB_CHECK(got.args.size() == 1 && got.args[0] == string && got.nullable);
    // what has nothing to lift is the same object, and nothing stays nothing
    const ir::TypeP plain = ir::class_type("Vec3D");
    CB_CHECK(j2x::lift_type(plain).get() == plain.get());
    CB_CHECK(!j2x::lift_type(ir::TypeP()));
    CB_CHECK(j2x::lift_type(ir::i32_type()) == ir::i32_type());
});

CB_TEST("translate.j2x.statics_lose_their_prefix_and_init_statics_is_the_static_initialiser", [] {
    Arena arena;
    const auto field = [&](const char* name, bool is_static) {
        ir::FieldDecl f;
        f.sym = ir::make_sym(ir::SymKind::Field, name, "Box", is_static, ir::i32_type());
        f.type = ir::i32_type();
        if (is_static) f.mods = {"static"};
        return ir::make(arena, std::move(f));
    };
    const auto method = [&](const char* name, const char* kind, bool is_static) {
        ir::MethodDecl m;
        m.name = name;
        m.kind = kind;
        m.sym = ir::make_sym(ir::SymKind::Method, name, "Box", is_static);
        if (is_static) m.mods = {"static"};
        m.body = ir::make(arena, ir::Block{});
        return ir::make(arena, std::move(m));
    };
    // s_count is count again; s_size stays (a field `size` exists beside it); an empty destructor is dropped
    ir::ClassDecl box;
    box.name = "Box";
    box.qname = "Box";
    box.members = {field("s_count", true), field("s_size", true), field("size", false), method("initStatics", "method", true), method("~Box", "dtor", false)};
    ir::Unit unit;
    unit.types = {ir::make(arena, std::move(box))};
    ir::Node* node = ir::make(arena, std::move(unit));
    ir::Node* units[] = {node};
    j2x::lift(arena, units);
    const auto* cls = ir::get<ir::ClassDecl>(ir::get<ir::Unit>(node)->types[0]);
    CB_CHECK_EQ(cls->members.size(), std::size_t(4));
    CB_CHECK_EQ(ir::get<ir::FieldDecl>(cls->members[0])->sym->name, std::string("count"));
    CB_CHECK_EQ(ir::get<ir::FieldDecl>(cls->members[1])->sym->name, std::string("s_size"));   // (a field `size` exists)
    CB_CHECK(ir::get<ir::MethodDecl>(cls->members[3])->kind == "static_init");
});

CB_TEST("translate.j2x.bind_sets_the_hook", [] {
    jj::Host host;
    CB_CHECK(!host.lift_java_runtime);
    j2x::bind(host);
    CB_CHECK(static_cast<bool>(host.lift_java_runtime));
    Arena arena;
    ir::Node* unit = l2::from_json(arena, j2xt_load("j2x_Vec3D.pre.json"));
    ir::Node* units[] = {unit};
    host.lift_java_runtime(arena, units);
    CB_CHECK_EQ(j2xt_difference(j2xt_normal(l2::to_json(*unit)), j2xt_normal(j2xt_load("j2x_Vec3D.post.json")), ""), std::string());
});

// The types in a list are lifted like the ones in a slot.
CB_TEST("translate.j2x.types_in_lists_are_lifted", [] {
    Arena arena;
    const ir::TypeP runtime_string = ir::class_type("JavaString"), string = ir::class_type("java.lang.String");
    ir::MethodDecl method;
    method.name = "m";
    method.kind = "method";
    method.throws = {runtime_string};
    ir::Catch handler;
    handler.types = {runtime_string};
    ir::Call call;
    call.typeargs = {runtime_string};
    ir::Node* call_node = ir::make(arena, std::move(call));
    handler.body = ir::make(arena, ir::Block{});
    method.body = ir::make(arena, ir::Block{.stmts = {ir::make(arena, std::move(handler)), ir::make(arena, ir::ExprStmt{.expr = call_node})}});
    ir::ClassDecl cls;
    cls.name = "C";
    cls.qname = "C";
    cls.implements = {runtime_string};
    cls.members = {ir::make(arena, std::move(method))};
    ir::Unit unit;
    unit.types = {ir::make(arena, std::move(cls))};
    ir::Node* node = ir::make(arena, std::move(unit));
    ir::Node* units[] = {node};
    j2x::lift(arena, units);
    const auto* c = ir::get<ir::ClassDecl>(ir::get<ir::Unit>(node)->types[0]);
    CB_CHECK(c->implements.size() == 1 && c->implements[0] == string);
    const auto* m = ir::get<ir::MethodDecl>(c->members[0]);
    CB_CHECK(m->throws.size() == 1 && m->throws[0] == string);
    const auto* body = ir::get<ir::Block>(std::get<ir::Node*>(m->body));
    CB_CHECK(ir::get<ir::Catch>(body->stmts[0])->types[0] == string);
    const auto* lifted_call = ir::get<ir::Call>(ir::get<ir::ExprStmt>(body->stmts[1])->expr);
    CB_CHECK(std::get<ir::TypeP>(lifted_call->typeargs[0]) == string);
});

// The round trip, whole, with the hook the native judge needs: Java classes -> (the Java lane's C++, read by clang: the units come
// from the goldens here) -> j2x -> the Java back end -> javac -> the JVM, judged against the original Java.
CB_TEST("translate.j2x.the_native_round_trip_judge_gives_the_expected_verdict", [] {
    const stdfs::path jdk = "C:/Program Files/Java/jdk-21/bin";
    const std::string javac_text = cb::fs::get_env("CB_JAVAC").value_or((jdk / "javac.exe").string());
    const std::string java_text = cb::fs::get_env("CB_JAVA").value_or((jdk / "java.exe").string());
    std::error_code ec;
    if (!stdfs::is_regular_file(javac_text, ec) || !stdfs::is_regular_file(java_text, ec)) return;   // no JDK here: nothing to judge on

    static const cb::java::apimap::Registry registry = cb::java::apimap::Registry::load(cb::testing::data_path("../../config/java/api_maps"));
    static const cb::translate::backends::java_libc::LibC libc(registry);
    jj::Host host;
    host.jdk = {javac_text, java_text};
    host.runtime_sources = cb::testing::data_path("../../config/translate/backends/java_runtime");
    const stdfs::path state = cb::testing::data_path("../../state/work/j2x-tests");
    cb::fs::make_dirs(state);
    host.state_folder = state / "state";
    host.java_options.library = &libc;
    host.java_options.idiomatic = cb::translate::backends::java_objects::factory(&libc);
    // the Java lane's C++ is read from the goldens: each class's unit by the stem of the file asked for
    host.lower = [](Arena& arena, const jj::LoweringRequest& request) -> ir::Node* {
        return l2::from_json(arena, j2xt_load("j2x_" + request.path.stem().string() + ".pre.json"));
    };
    host.java_to_cpp = [](const stdfs::path&, const stdfs::path&) { return std::string(); };
    j2x::bind(host);

    std::vector<stdfs::path> files;
    const stdfs::path src = cb::testing::data_path(j2xt_data("j2x_roundtrip_src/sample"));
    for (const char* cls : kJ2xClasses) files.push_back(src / (std::string(cls) + ".java"));
    const stdfs::path work = state / "roundtrip";
    stdfs::remove_all(work, ec);
    const jj::Verdict v = jj::roundtrip(host, files, "sample.RoundTrip", work);
    CB_CHECK_EQ(v.reason, std::string());
    CB_CHECK(v.verdict == jj::Kind::Equivalent);
    CB_CHECK_EQ(v.tier.value_or(""), std::string("idiomatic"));
    CB_CHECK(v.notes.empty());
    // and it is the checked-in Java
    std::string golden = cb::fs::read_text(cb::testing::data_path(j2xt_data("j2x_RoundTripped.java.txt")));
    std::string wrote = v.java.value_or("");
    golden.erase(std::remove(golden.begin(), golden.end(), '\r'), golden.end());
    wrote.erase(std::remove(wrote.begin(), wrote.end(), '\r'), wrote.end());
    CB_CHECK_EQ(wrote, golden);
    stdfs::remove_all(work, ec);
});
