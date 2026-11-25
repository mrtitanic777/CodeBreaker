// Tests of the Go back end for Java (go_java): the names and literals it writes, the path it adds to the capability tables, and -- where the machine has a JDK
// and Go -- whole Java files translated, built and run beside the originals (translate_file does the run and the comparison; the programs are in
// translate/testdata/go_java): Hello, Basics (numbers, strings, arrays, loops and jumps, switches, printf), Zoo (classes, interfaces, inheritance, virtual
// calls, super, statics, casts), Errs (exceptions), Lists (ArrayList, HashMap and HashSet in Java's order), Edge, Deep and Enums.
// What the back end does not write it declines with a reason, and the judge reports that program as refused.
#include "translate/backends/go_java_internal.hpp"

#include <iostream>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "translate/capability.hpp"
#include "translate/translate_file.hpp"

namespace {

namespace cap = cb::translate::capability;
namespace tf = cb::translate::translate_file;
namespace stdfs = std::filesystem;

stdfs::path gj_work(const std::string& name) {
    const stdfs::path p = cb::testing::data_path("../work/translate-file-tests") / ("go-" + name);
    std::error_code ec;
    stdfs::remove_all(p, ec);
    cb::fs::make_dirs(p);
    return p;
}

tf::Result gj_run(const stdfs::path& input, const stdfs::path& out, const stdfs::path& work) {
    cb::translate::judge_cs_cpp::Environment environment{cb::config::Config::load(cb::testing::data_path("..")), {}};
    return tf::run(*cap::find_path("java", "go"), input, out, work, environment);
}

// whether this machine can run the Java-to-Go path (a JDK and go)
bool gj_can_run() {
    const cap::Path* path = cap::find_path("java", "go");
    return path != nullptr && cap::missing_tool(*path).empty();
}

void gj_translates(const std::string& program) {
    if (!gj_can_run()) return;
    const stdfs::path work = gj_work(program);
    cb::fs::write_text(work / (program + ".java"), cb::fs::read_text(cb::testing::data_path("translate/testdata/go_java/" + program + ".java")));
    const tf::Result r = gj_run(work / (program + ".java"), work / "out", work / "scratch");
    CB_CHECK(r.verdict == tf::Verdict::Agrees);
    if (r.verdict != tf::Verdict::Agrees) std::cerr << program << ": " << r.detail.substr(0, 1000) << std::endl;
    for (const std::string file : {program + ".go", std::string("go.mod"), std::string("jrt/jrt.go"), std::string("BUILD.txt")}) CB_CHECK(stdfs::is_regular_file(work / "out" / file));
    std::error_code ec;
    stdfs::remove_all(work, ec);
}

}  // namespace

CB_TEST("translate.go_java.names", [] {
    using namespace cb::translate::backends::go_java::detail;
    CB_CHECK_EQ(ident("type"), "type_");
    CB_CHECK_EQ(ident("len"), "len_");   // (a local named like a Go builtin would hide it)
    CB_CHECK_EQ(ident("a$b"), "a_b");
    CB_CHECK_EQ(ident("plain"), "plain");
    CB_CHECK_EQ(capital("size"), "Size");
    CB_CHECK_EQ(field_ident("range"), "range_");
    CB_CHECK_EQ(go_string("a\"b\\c\nd"), "\"a\\\"b\\\\c\\nd\"");
    CB_CHECK_EQ(go_string("caf\xC3\xA9"), "\"caf\xC3\xA9\"");   // (UTF-8 stays as it is)
    bool refused = false;
    try {
        (void)go_string("\xED\xA0\x80");   // a lone surrogate has no Go text
    } catch (const cb::translate::backends::go_java::Decline&) {
        refused = true;
    }
    CB_CHECK(refused);
    CB_CHECK_EQ(float_text(1.5, false), "1.5");
    CB_CHECK_EQ(float_text(2, false), "2.0");
});

CB_TEST("translate.go_java.java_to_go_is_a_path_with_go_as_its_tool", [] {
    const cap::Path* path = cap::find_path("java", "go");
    CB_CHECK(path != nullptr);
    CB_CHECK(path != nullptr && tf::can_run(*path));
    CB_CHECK(path != nullptr && std::find(path->needs.begin(), path->needs.end(), "go") != path->needs.end());
    CB_CHECK(cap::find_language("go") != nullptr && cap::find_language("go")->back_end);
    CB_CHECK(cap::find_path("csharp", "go") == nullptr);
});

CB_TEST("translate.go_java.hello_and_static_methods", [] { gj_translates("Hello"); });
CB_TEST("translate.go_java.basics_ints_strings_arrays_loops_switches", [] { gj_translates("Basics"); });
CB_TEST("translate.go_java.zoo_classes_interfaces_inheritance_and_virtual_calls", [] { gj_translates("Zoo"); });
CB_TEST("translate.go_java.errs_exceptions_and_jumps_out_of_try", [] { gj_translates("Errs"); });
CB_TEST("translate.go_java.lists_maps_and_sets_iterate_as_java_does", [] { gj_translates("Lists"); });
CB_TEST("translate.go_java.deep_recursion_has_room", [] { gj_translates("Deep"); });
CB_TEST("translate.go_java.enums_with_fields_switches_values_and_valueof", [] { gj_translates("Enums"); });

// Edge has an assignment inside an expression whose order Go leaves open: the back end refuses it, with the reason.
CB_TEST("translate.go_java.declines_what_it_cannot_write", [] {
    if (!gj_can_run()) return;
    const stdfs::path work = gj_work("declines");
    cb::fs::write_text(work / "Edge.java", cb::fs::read_text(cb::testing::data_path("translate/testdata/go_java/Edge.java")));
    const tf::Result edge = gj_run(work / "Edge.java", work / "out1", work / "scratch1");
    CB_CHECK(edge.verdict == tf::Verdict::Refused);
    CB_CHECK(edge.detail.find("Decline") != std::string::npos && edge.detail.find("order") != std::string::npos);
    CB_CHECK(edge.files.empty());
    cb::fs::write_text(work / "Anon.java",
                       "public class Anon {\n    public static void main(String[] args) {\n        Runnable r = new Runnable() { public void run() { System.out.println(1); } };\n        r.run();\n    }\n}\n");
    const tf::Result anon = gj_run(work / "Anon.java", work / "out2", work / "scratch2");
    CB_CHECK(anon.verdict == tf::Verdict::Refused);
    std::error_code ec;
    stdfs::remove_all(work, ec);
});
