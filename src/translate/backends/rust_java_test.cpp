// Tests of the Rust back end for Java (rust_java): the names and literals it writes, the path it adds to the capability tables, and -- where the machine has a JDK
// and rustc -- whole Java files translated, built and run beside the originals (translate_file does the run and the comparison; the programs are in
// translate/testdata/rust_java): Hello (a static method), Basics (ints, longs, doubles, strings, arrays, loops and jumps, switches, printf), Zoo (classes,
// interfaces, inheritance, virtual calls, super, statics, casts), Errs (exceptions: try/catch/finally, throw, the run-time errors, a jump out of a try),
// Lists (ArrayList, HashMap and HashSet in Java's iteration order, boxed numbers), Edge (operand order, wrapping, casts, shifts, number and string
// printing, printf, split, labeled jumps, switch forms), Deep (recursion thousands of calls deep) and Enums (constants with fields, values, valueOf, switch).
// What the back end does not write it declines with a reason, and the judge reports that program as refused.
#include "translate/backends/rust_java_internal.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "translate/capability.hpp"
#include "translate/l2.hpp"
#include "translate/translate_file.hpp"

namespace {

namespace cap = cb::translate::capability;
namespace tf = cb::translate::translate_file;
namespace stdfs = std::filesystem;
namespace rj = cb::translate::backends::rust_java;
namespace ir = cb::translate::ir;

stdfs::path rj_work(const std::string& name) {
    const stdfs::path p = cb::testing::data_path("../work/translate-file-tests") / ("rust-" + name);
    std::error_code ec;
    stdfs::remove_all(p, ec);
    cb::fs::make_dirs(p);
    return p;
}

tf::Result rj_run(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work) {
    cb::translate::judge_cs_cpp::Environment environment{cb::config::Config::load(cb::testing::data_path("..")), {}};
    return tf::run(*cap::find_path("java", "rust"), input, out, work, environment);
}

// whether this machine can run the Java-to-Rust path (a JDK and rustc)
bool rj_can_run() {
    const cap::Path* path = cap::find_path("java", "rust");
    return path != nullptr && cap::missing_tool(*path).empty();
}

void rj_translates(const std::string& program) {
    if (!rj_can_run()) return;
    const stdfs::path work = rj_work(program);
    cb::fs::write_text(work / (program + ".java"), cb::fs::read_text(cb::testing::data_path("translate/testdata/rust_java/" + program + ".java")));
    const tf::Result r = rj_run(work / (program + ".java"), work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::Agrees);
    if (r.verdict != tf::Verdict::Agrees) std::cerr << program << ": " << r.detail.substr(0, 1000) << std::endl;
    for (const std::string file : {program + ".rs", std::string("runtime/jrt.rs"), std::string("BUILD.txt")}) CB_CHECK(stdfs::is_regular_file(work / "out" / file));
    std::error_code ec;
    stdfs::remove_all(work, ec);
}

}  // namespace

CB_TEST("translate.rust_java.names", [] {
    using namespace cb::translate::backends::rust_java::detail;
    CB_CHECK_EQ(ident("type"), "type_");
    CB_CHECK_EQ(ident("lit"), "lit_");        // (the runtime's own `lit` would be hidden by a local of that name)
    CB_CHECK_EQ(ident("a$b"), "a_b");
    CB_CHECK_EQ(ident("plain"), "plain");
    CB_CHECK_EQ(member_ident("lit"), "lit");   // (a method or field is never found by its bare name)
    CB_CHECK_EQ(member_ident("match"), "match_");
    CB_CHECK_EQ(rs_string("a\"b\\c\nd"), "\"a\\\"b\\\\c\\nd\"");
    CB_CHECK_EQ(rs_string("\t\x01"), "\"\\t\\u{1}\"");
    CB_CHECK_EQ(rs_string("caf\xC3\xA9"), "\"caf\xC3\xA9\"");   // (UTF-8 stays as it is)
    bool refused = false;
    try {
        (void)rs_string("\xED\xA0\x80");   // a lone surrogate has no Rust text
    } catch (const rj::Decline&) {
        refused = true;
    }
    CB_CHECK(refused);
    CB_CHECK_EQ(float_text(1.5, false), "1.5");
    CB_CHECK_EQ(float_text(2, false), "2.0");
    CB_CHECK_EQ(float_text(1e-5, false), "1e-05");
    CB_CHECK_EQ(float_text(0.5, true), "0.5f32");
    CB_CHECK_EQ(short_code(ir::i32_type()), "i32");
    CB_CHECK_EQ(short_code(ir::class_type("java.lang.String")), "str");
    CB_CHECK_EQ(short_code(ir::array_of(ir::class_type("a.Animal"))), "Animal_arr");
});

CB_TEST("translate.rust_java.java_to_rust_is_a_path_with_rustc_as_its_tool", [] {
    const cap::Path* path = cap::find_path("java", "rust");
    CB_CHECK(path != nullptr);
    CB_CHECK(path != nullptr && tf::can_run(*path));
    CB_CHECK(path != nullptr && std::find(path->needs.begin(), path->needs.end(), "rustc") != path->needs.end());
    CB_CHECK(cap::find_language("rust") != nullptr && cap::find_language("rust")->back_end);
    const std::vector<cap::Target> list = cap::targets_for_file("Calc.java");
    const cap::Target* rust = cap::find_target(list, "rs");   // (`--to rs` is `--to rust`)
    CB_CHECK(rust != nullptr && rust->id == "rust");
    CB_CHECK(cap::find_path("csharp", "rust") == nullptr);
});

CB_TEST("translate.rust_java.hello_and_static_methods", [] { rj_translates("Hello"); });
CB_TEST("translate.rust_java.basics_ints_strings_arrays_loops_switches", [] { rj_translates("Basics"); });
CB_TEST("translate.rust_java.zoo_classes_interfaces_inheritance_and_virtual_calls", [] { rj_translates("Zoo"); });
CB_TEST("translate.rust_java.errs_exceptions_and_jumps_out_of_try", [] { rj_translates("Errs"); });
CB_TEST("translate.rust_java.lists_maps_and_sets_iterate_as_java_does", [] { rj_translates("Lists"); });
CB_TEST("translate.rust_java.edge_cases_of_java_arithmetic_text_and_evaluation_order", [] { rj_translates("Edge"); });
CB_TEST("translate.rust_java.memory_is_reclaimed_while_a_program_makes_gigabytes_of_garbage", [] { rj_translates("Churn"); });
CB_TEST("translate.rust_java.varargs_switch_fallthrough_and_box_identity", [] { rj_translates("Misc2"); });
CB_TEST("translate.rust_java.records_with_compact_constructors_and_interfaces", [] { rj_translates("Recs"); });
CB_TEST("translate.rust_java.deep_recursion_has_room", [] { rj_translates("Deep"); });
CB_TEST("translate.rust_java.enums_with_fields_switches_values_and_valueof", [] { rj_translates("Enums"); });

// A program the back end does not write is refused, with the reason, and nothing is handed over.
CB_TEST("translate.rust_java.declines_what_it_cannot_write", [] {
    if (!rj_can_run()) return;
    const stdfs::path work = rj_work("declines");
    cb::fs::write_text(work / "Boxy.java",
                       "public class Boxy {\n    static class Box<T> { T v; }\n"
                       "    public static void main(String[] args) { Box<String> b = new Box<>(); System.out.println(b.v); }\n}\n");
    const tf::Result boxy = rj_run(work / "Boxy.java", work / "out1", work / "scratch1");
    CB_CHECK(boxy.verdict == tf::Verdict::Refused);
    CB_CHECK(boxy.detail.find("Decline") != std::string::npos && boxy.detail.find("generic") != std::string::npos);
    CB_CHECK(boxy.files.empty());
    cb::fs::write_text(work / "Anon.java",
                       "public class Anon {\n    interface Greeter { String hi(); }\n    public static void main(String[] args) {\n"
                       "        Greeter g = new Greeter() { public String hi() { return \"hi\"; } };\n        System.out.println(g.hi());\n    }\n}\n");
    const tf::Result anon = rj_run(work / "Anon.java", work / "out2", work / "scratch2");
    CB_CHECK(anon.verdict == tf::Verdict::Refused);
    CB_CHECK(anon.detail.find("anonymous") != std::string::npos);
    std::error_code ec;
    stdfs::remove_all(work, ec);
});

// Without a JDK: the units of a program the C back end writes (testdata/c_java/zoo, with records, lambdas and an anonymous class) are declined by the
// Rust back end with a reason, not written wrongly.
CB_TEST("translate.rust_java.declines_records_and_lambdas_with_a_reason", [] {
    cb::Arena arena;
    std::vector<ir::Node*> units;
    const cb::Json docs = cb::json::load(cb::testing::data_path("translate/testdata/c_java/zoo/units.l2.json"));
    for (const cb::Json& doc : docs) units.push_back(cb::translate::l2::from_json(arena, doc));
    std::string why;
    try {
        (void)rj::program(arena, units);
    } catch (const rj::Decline& d) {
        why = d.what();
    }
    CB_CHECK(!why.empty());
    CB_CHECK(why.find("record") != std::string::npos || why.find("anonymous") != std::string::npos || why.find("generic") != std::string::npos || why.find("Lambda") != std::string::npos || why.find("library type") != std::string::npos);
});
