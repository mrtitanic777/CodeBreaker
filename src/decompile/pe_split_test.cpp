// Tests of a 32-bit program's functions as source files (pe_split.hpp).
#include "decompile/pe_split.hpp"

#include <string>
#include <vector>

#include "core/testing.hpp"

namespace cb::decompile::pe_split {
namespace {

Function pst_function(Va start, std::vector<std::size_t> calls, std::vector<Va> data) {
    return {start, "sub_" + std::to_string(start), "int sub_" + std::to_string(start) + "() { return 0; }", std::move(calls), std::move(data), false};
}

// Two files of three functions: each calls and shares data among its own.
std::vector<Function> pst_two_files() {
    return {pst_function(0x1000, {1, 2}, {0x9000}), pst_function(0x1010, {2}, {0x9000, 0x9004}), pst_function(0x1020, {}, {0x9004}),
            pst_function(0x1030, {4, 5}, {0x9100}), pst_function(0x1040, {5}, {0x9100, 0x9104}), pst_function(0x1050, {}, {0x9104})};
}

CB_TEST("decompile.pe_split.boundary_between_two_files", [] {
    const std::vector<Function> f = pst_two_files();
    CB_CHECK(boundaries(f) == std::vector<std::size_t>{3});
});

CB_TEST("decompile.pe_split.no_debug_information_is_a_best_guess", [] {
    const std::vector<Function> f = pst_two_files();
    const Split s = plan(f, {}, {}, "t");
    CB_CHECK_EQ(s.units.size(), std::size_t{2});
    CB_CHECK_EQ(s.units[0].file, std::string("t_part01.c"));
    CB_CHECK(s.units[0].tier == Tier::BestGuess);
});

CB_TEST("decompile.pe_split.debug_information_is_proven", [] {
    const std::vector<Function> f = pst_two_files();
    const std::vector<Module> modules = {{"src\\ALPHA.obj", {{0x1000, 0x30}}}, {"beta.obj", {{0x1030, 0x30}}}};
    const Split s = plan(f, modules, {}, "t");
    CB_CHECK_EQ(s.units.size(), std::size_t{2});
    CB_CHECK_EQ(s.units[0].file, std::string("alpha.c"));
    CB_CHECK_EQ(s.units[1].file, std::string("beta.c"));
    CB_CHECK(s.units[0].tier == Tier::Proven && s.units[1].tier == Tier::Proven);
});

CB_TEST("decompile.pe_split.library_code_is_the_runtime_file", [] {
    std::vector<Function> f = pst_two_files();
    f[5].stub = true;
    const std::vector<Module> modules = {{"prog.obj", {{0x1000, 0x20}}}, {"heap\\obj\\i386\\free.obj", {{0x1020, 0x10}}}};
    const Split s = plan(f, modules, {0x1040}, "t");
    bool found = false;
    for (const Unit& u : s.units)
        if (u.file == "t_runtime.c") found = u.tier == Tier::Runtime && u.functions == std::vector<std::size_t>{2, 4, 5};
    CB_CHECK(found);
    CB_CHECK(runtime_module("startup\\obj\\i386\\crt0.obj") && runtime_module("KERNEL32.dll") && !runtime_module("worker.obj"));
});

CB_TEST("decompile.pe_split.every_function_is_written_once", [] {
    const std::vector<Function> f = pst_two_files();
    const auto files = source_files("t", "/* t */\n\nint x;\n", plan(f, {}, {}, "t"), f);
    CB_CHECK_EQ(files.front().first, std::string("t.h"));
    CB_CHECK_EQ(files.back().first, std::string("SOURCE_FILES.txt"));
    CB_CHECK(files.back().second.find("best guess") != std::string::npos);
    for (const Function& g : f) {
        int seen = 0;
        for (std::size_t k = 1; k + 1 < files.size(); ++k)
            if (files[k].second.find(g.text) != std::string::npos) ++seen;
        CB_CHECK_EQ(seen, 1);
    }
});

}  // namespace
}  // namespace cb::decompile::pe_split
