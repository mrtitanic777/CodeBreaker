// The port's part 4 on real programs: the source trees of Microsoft's own tools (lib 2.20, MS C 3.00's compiler
// pass 0) as the port has them after its constructor, run through every function of the module.
//
// testdata/<target>.state.json is the stored state of a Port after its constructor (parts 1-3 of pre_ansi, not yet
// refactored) made by the Python original, with the profile file next to it; testdata/<target>.golden.tsv has, for each
// call the harness makes (tag, key), the CRC-32 of the answer's canonical JSON. The goldens were made by this code and
// are equal to what the Python original answers for the same calls: the old harness (state/work/typed-port/
// port.pre_ansi.p4: expected.txt of the Python, compare.py) compared all 2,940 answers of the three trees by meaning,
// no difference. lib-2.20 is run in full (every per-file call and the program-wide ones: layouts, sizes, allocators,
// definitions, frames, finish, headers ...); MS C 3.00 only for the program-wide calls (its BSS list, aliases).
#include <fstream>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/port/real_program.hpp"

using namespace cb;
using namespace cb::port::pre_ansi::real;

namespace {

void check_target(const std::string& target, std::size_t at_least) {
    const std::filesystem::path dir = testing::data_path("port/testdata");
    const J state = json::load(dir / (target + ".state.json"));
    std::map<std::string, std::uint32_t> golden;   // "tag\tkey" -> crc
    {
        std::ifstream in(dir / (target + ".golden.tsv"), std::ios::binary);
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            const std::size_t a = line.find('\t'), b = line.find('\t', a + 1);
            golden[line.substr(0, a) + "\t" + line.substr(a + 1, b - a - 1)] = static_cast<std::uint32_t>(std::stoul(line.substr(b + 1)));
        }
    }
    CB_CHECK(golden.size() >= at_least);
    std::size_t checked = 0;
    for (const Case& c : run_target(state, dir.string())) {
        const auto want = golden.find(c.tag + "\t" + c.key.dump());
        if (want == golden.end()) continue;   // (a call the golden file does not cover)
        ++checked;
        const std::uint32_t got = golden_crc(c.tag, c.value);
        testing::count_check();
        if (got != want->second)
            testing::record_failure(__FILE__, __LINE__, "a different answer: " + c.tag + " " + c.key.dump() + " -> " + c.value.dump().substr(0, 300));
    }
    CB_CHECK_EQ(checked, golden.size());   // (every golden call was run)
}

}  // namespace

CB_TEST("port.real_program.lib_2_20_every_call_of_the_module", [] { check_target("lib-2.20", 400); });

CB_TEST("port.real_program.ms_c_3_00_p0_the_program_wide_calls", [] { check_target("msc-3.00-p0", 15); });
