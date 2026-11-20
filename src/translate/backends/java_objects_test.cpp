// Tests of the Java back end's idiomatic tier (java_objects), on C and C++ programs the clang front end lowered
// (testdata/java/objects/*.c, *.cpp; the L2 documents beside them are its output, without columns). Each golden .idiomatic.java is
// the checked-in output for that document; it was read through and
// found to say what the program says: ObjStructs (structs as classes with C's copies, an array field copied by clone, a boxed
// out-parameter, malloc as `new`, a function pointer as an interface, an enum's constants, a static local as a global),
// ObjClasses (a hierarchy with a base initialiser, a pure virtual method, a static member, an operator as `plus`, `new` and
// `delete`), ObjDtor (a virtual destructor run by `delete`, chained with super), ObjMore (a linked list, arrays of arrays and of
// structs, byte strings), ObjGaps (a method that returns a reference to an int and a library call the runtime lacks are gaps, a
// template instance named by its arguments) and ObjNames (a class named as a global, a Java keyword, an anonymous struct).
//
// Behaviour pinned here:
//   * a function pointer is called through its interface even when the source dereferenced it first (`(*f)(x)`);
//   * a refusal says `a reference to a i32` with the type as the back end spells it;
//   * a pointer to a struct that the program indexes (`many[1].y`) is still one reference, which the idiomatic tier cannot write as
//     compilable Java (a limit of the tier, noted in the bugs list); no test depends on it;
//   * an array with both a size expression and a fixed size (a `new` node that cannot be written as either) is refused.
#include "translate/backends/java_objects.hpp"

#include <chrono>
#include <cstdio>
#include <limits>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/testing.hpp"
#include "translate/backends/java_libc.hpp"
#include "translate/l2.hpp"

namespace {

namespace java = cb::translate::backends::java;
namespace jo = cb::translate::backends::java_objects;
namespace jlc = cb::translate::backends::java_libc;
namespace apimap = cb::java::apimap;
namespace ir = cb::translate::ir;
namespace l2 = cb::translate::l2;
using ir::Node;

const jlc::LibC& jot_library() {
    static const apimap::Registry registry = apimap::Registry::load(cb::testing::data_path("../config/java/api_maps"));
    static const jlc::LibC library(registry);
    return library;
}

struct JotProgram {
    cb::Arena arena;
    Node* unit = nullptr;
};

std::unique_ptr<JotProgram> jot_load(const std::string& name) {
    auto p = std::make_unique<JotProgram>();
    p->unit = l2::from_json(p->arena, cb::json::load(cb::testing::data_path("translate/testdata/java/objects/" + name + ".l2.json")));
    return p;
}

std::string jot_golden(const std::string& file) { return cb::fs::read_text(cb::testing::data_path("translate/testdata/java/objects/" + file)); }

java::Program jot_write(JotProgram& p, const std::string& name, const java::LibraryCalls* library = &jot_library()) {
    Node* units[] = {p.unit};
    return java::program(units, name, {.tier = java::Tier::Idiomatic, .idiomatic = jo::factory(library), .library = library});
}

std::string jot_notes_text(const java::Program& prog) {
    std::string out;
    for (const std::string& n : prog.notes) out += n + "\n";
    return out;
}

void jot_check_golden(const std::string& name) {
    auto p = jot_load(name);
    const java::Program prog = jot_write(*p, name);
    CB_CHECK(prog.tier == java::Tier::Idiomatic);
    CB_CHECK_EQ(prog.source, jot_golden(name + ".idiomatic.java"));
    CB_CHECK_EQ(jot_notes_text(prog), jot_golden(name + ".notes.txt"));
}

std::string jot_refusal(const std::string& name) {
    auto p = jot_load(name);
    try {
        (void)jot_write(*p, name);
    } catch (const java::Refused& r) {
        return r.what();
    }
    return "";
}

}  // namespace

CB_TEST("translate.java_objects.golden.structs", [] { jot_check_golden("ObjStructs"); });
CB_TEST("translate.java_objects.golden.classes", [] { jot_check_golden("ObjClasses"); });
CB_TEST("translate.java_objects.golden.destructors", [] { jot_check_golden("ObjDtor"); });
CB_TEST("translate.java_objects.golden.more", [] { jot_check_golden("ObjMore"); });
CB_TEST("translate.java_objects.golden.gaps", [] { jot_check_golden("ObjGaps"); });
CB_TEST("translate.java_objects.golden.names", [] { jot_check_golden("ObjNames"); });

CB_TEST("translate.java_objects.what_the_classes_offer", [] {
    // the copy and assignment constructors, the aggregate constructor and the no-argument one appear where the program uses them
    auto p = jot_load("ObjStructs");
    const std::string src = jot_write(*p, "ObjStructs").source;
    const auto has = [&](std::string_view s) { return src.find(s) != std::string::npos; };
    CB_CHECK(has("Vec(int x, int y) {"));                  // an aggregate initialiser is a constructor
    CB_CHECK(has("Vec(Vec o) {"));                         // passing by value copies
    CB_CHECK(has("Vec set(Vec o) {"));                     // assignment copies into the object
    CB_CHECK(has("this.tags = o.tags.clone();"));          // an array member is copied with its struct
    CB_CHECK(has("int[] n = {5};"));                       // a scalar whose address is taken is boxed
    CB_CHECK(has("n[0] += 1;"));                           // ... and the out-parameter writes through it
    CB_CHECK(has("copy.a.set(w);"));                       // a struct assigned to a field copies into it
    CB_CHECK(has("Vec p = new Vec();"));                   // malloc(sizeof(T)) is `new T()`
    CB_CHECK(has("int[] nums = new int[4];"));             // malloc(n * sizeof(int)) is `new int[n]`
    CB_CHECK(has("// free(p): the collector frees it"));
    CB_CHECK(has("apply(ObjStructs::cmp, 3, 1)"));         // a function's address is a method reference
    CB_CHECK(has("interface IntIntToInt {"));
    CB_CHECK(has("static final int GREEN = 5;"));          // enum constants
    CB_CHECK(has("static int main_calls = 0;"));           // a static local is a global
});

CB_TEST("translate.java_objects.gaps_are_listed_and_do_not_stop_the_program", [] {
    auto p = jot_load("ObjGaps");
    Node* units[] = {p->unit};
    // through the tier itself: its gaps and notes
    java::Analysis analysis(units);
    analysis.run();
    auto tier = jo::factory(&jot_library())(units, "ObjGaps", analysis);
    const std::string src = tier->write();
    CB_CHECK_EQ(tier->gaps().size(), std::size_t{2});
    CB_CHECK_EQ(tier->gaps()[0], "use_mystery (library function mystery: not in the C runtime for Java (api_maps/libc.json))");
    CB_CHECK_EQ(tier->gaps()[1], "Box::ref (ref returns a reference to a i32)");
    CB_CHECK_EQ(tier->notes().size(), std::size_t{2});
    CB_CHECK(src.find("static class Stack_int_4 {") != std::string::npos);
    // a program the analysis lets either tier have, with gaps, goes back to the faithful tier when it is not a class program
    auto q = jot_load("ObjStructs");
    Node* qu[] = {q->unit};
    CB_CHECK(java::program(qu, "ObjStructs", {.idiomatic = jo::factory(&jot_library()), .library = &jot_library()}).tier == java::Tier::Idiomatic);
});

CB_TEST("translate.java_objects.refusals", [] {
    // a local whose destructor must run at the end of its scope: the program is refused
    CB_CHECK_EQ(jot_refusal("ObjRaii"), "Guard g has a destructor that runs at the end of its scope (RAII: try/finally not written yet)");
    // a union is the faithful tier's (refused here by the tier itself, not only by the analysis that would have kept it away)
    JotProgram core;
    core.unit = l2::from_json(core.arena, cb::json::load(cb::testing::data_path("translate/testdata/java/FaithfulCore.l2.json")));
    Node* units[] = {core.unit};
    java::Analysis analysis(units);
    analysis.run();
    auto tier = jo::factory(&jot_library())(units, "FaithfulCore", analysis);
    try {
        (void)tier->write();
        CB_CHECK(false);
    } catch (const java::Refused& r) {
        CB_CHECK_EQ(std::string(r.what()), "union Pun");
    }
});

CB_TEST("translate.java_objects.a_library_call_without_a_mapping_refuses_main", [] {
    auto p = jot_load("ObjStructs");
    try {
        (void)jot_write(*p, "ObjStructs", nullptr);
        CB_CHECK(false);
    } catch (const java::Refused& r) {
        CB_CHECK(std::string(r.what()).find("no library mapping") != std::string::npos);
    }
});

CB_TEST("translate.java_objects.method_names", [] {
    CB_CHECK_EQ(jo::method_name("operator+"), "plus");
    CB_CHECK_EQ(jo::method_name("operator==") , "isEqual");
    CB_CHECK_EQ(jo::method_name("operator[]"), "at");
    CB_CHECK_EQ(jo::method_name("operator()"), "call");
    CB_CHECK_EQ(jo::method_name("operator bool"), "toBool");
    CB_CHECK_EQ(jo::method_name("operator++"), "increment");
    CB_CHECK_EQ(jo::method_name("area"), "area");
    CB_CHECK_EQ(jo::method_name("class"), "class_");               // a Java keyword
    CB_CHECK_EQ(jo::method_name("not-an-id"), "not_an_id");
    CB_CHECK_THROWS_AS(java::Refused, jo::method_name("operator->"));
    CB_CHECK_THROWS_AS(java::Refused, jo::method_name("operator new"));
});

// The speed of the writing: the best of 20 (printed; the idiomatic tier writes each in under a millisecond).
CB_TEST("translate.java_objects.timing", [] {
    for (const char* name : {"ObjStructs", "ObjMore", "ObjClasses"}) {
        auto p = jot_load(name);
        std::int64_t best_us = std::numeric_limits<std::int64_t>::max();
        for (int rep = 0; rep < 20; ++rep) {
            const auto t0 = std::chrono::steady_clock::now();
            (void)jot_write(*p, name);
            best_us = std::min(best_us, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count());
        }
        std::fprintf(stderr, "java_objects %s: %lld us\n", name, static_cast<long long>(best_us));
    }
});
