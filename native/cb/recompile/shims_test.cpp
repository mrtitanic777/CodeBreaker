// The shims at the old header paths (native/typed/cpp/codebreaker/recompile/{gpu,jumptable,switches,xenon}.hpp) over the
// modules of this folder: they must compile with the old layer and forward to the new code. Built at /W3 (the old layer
// is not warning-clean).
#include <string>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/recompile/test_ppc.hpp"
#include "codebreaker/recompile/gpu.hpp"
#include "codebreaker/recompile/jumptable.hpp"
#include "codebreaker/recompile/switches.hpp"
#include "codebreaker/recompile/xenon.hpp"

namespace {

namespace old = codebreaker::recompile;
using cb::compat::from_py;
using cb::compat::to_py;
using namespace cb::recompile::test;

CB_TEST("recompile.shims.gpu_files_and_design_note", [] {
    const auto files = old::gpu::gpu_files(py::str("Anything"));
    CB_CHECK_EQ(files.size(), 4);
    CB_CHECK(files.contains(py::str("graphics/xe_gpu.h")));
    CB_CHECK(from_py(files.get(py::str("graphics/xe_gpu.h"), py::str())).find("class CommandProcessor") != std::string::npos);
    CB_CHECK(from_py(old::gpu::design_md(py::str("Up"))).starts_with("# Up GPU subsystem - native D3D12 design\n"));
});

CB_TEST("recompile.shims.jumptable_with_the_old_callables", [] {
    constexpr std::uint32_t kBase = 0x82000000;
    Mem m(kBase, 0x10000);
    const std::uint32_t end = m.code(kBase + 0x100, {lis(12, 0x8200), addi(12, 12, 0x1000), cmplwi(6, 11, 3), bgt(6, 0x40), slwi(0, 11, 2),
                                                      lwzx(11, 12, 0), mtctr(11), kBctrWord});
    for (std::uint32_t i = 0; i < 4; ++i) m.u32(kBase + 0x1000 + 4 * i, kBase + 0x200 + 0x10 * i);
    auto read = [&](int64_t a, int64_t n) -> std::optional<int64_t> {
        if (a < kBase || a + n > static_cast<int64_t>(kBase + m.bytes.size())) return std::nullopt;
        int64_t v = 0;
        for (int64_t i = 0; i < n; ++i) v = (v << 8) | m.bytes[static_cast<std::size_t>(a - kBase + i)];
        return v;
    };
    const old::jumptable::Reader u8 = [&](int64_t a) { return read(a, 1); };
    const old::jumptable::Reader u16 = [&](int64_t a) { return read(a, 2); };
    const old::jumptable::Reader u32 = [&](int64_t a) { return read(a, 4); };
    const old::jumptable::IsExec exec = [&](int64_t a) { return a >= kBase && a < kBase + 0x400; };

    // the raw word as an int, and as big-endian bytes
    const int64_t bctr = end - 4;
    py::list<old::jumptable::Insn> insns;
    insns.append(old::jumptable::Insn(kBase + 0x100, std::variant<int64_t, py::bytes>(static_cast<int64_t>(lis(12, 0x8200))), py::str("lis"), py::str("")));
    insns.append(old::jumptable::Insn(bctr, std::variant<int64_t, py::bytes>(py::bytes(std::string("\x4E\x80\x04\x20", 4))), py::str("bctr"), py::str("")));
    const auto found = old::jumptable::detect_jump_tables(insns, 0, 0, u8, u16, u32, exec);
    CB_CHECK_EQ(found.size(), 1);
    CB_CHECK(found.contains(bctr));
    const old::jumptable::JumpTable jt = found.get(bctr, old::jumptable::JumpTable{});
    CB_CHECK_EQ(std::get<0>(jt), 11);
    CB_CHECK_EQ(std::get<1>(jt).size(), 4);
    CB_CHECK_EQ(std::get<1>(jt)[0], kBase + 0x200);
    CB_CHECK_EQ(std::get<1>(jt)[3], kBase + 0x230);
    // one bctr, no table at the address the old code asks about
    CB_CHECK(!old::jumptable::_detect_one(u8, u16, u32, exec, kBase + 0x100).has_value());
    CB_CHECK(old::jumptable::_detect_one(u8, u16, u32, exec, bctr).has_value());
});

CB_TEST("recompile.shims.switch_file_from_the_old_records", [] {
    old::switches::Table guarded;
    guarded.bctr = 0x8200011C;
    guarded.r = 11;
    guarded.kind = py::str("absolute");
    guarded.labels = py::list<int64_t>{0x82000200, 0x82000210};
    guarded.default_ = 0x82000144;
    guarded.guard = 0x82000100;
    old::switches::Table analysed;                // what the project code adds from XenonAnalyse's file
    analysed.bctr = 0x82000700;
    analysed.r = 3;
    analysed.kind = py::str("XenonAnalyse");
    analysed.labels = py::list<int64_t>{0x82000800};
    analysed.default_ = 0;
    analysed.table = 0;
    analysed.guard = 0;
    analysed.inline_ = false;
    cb::fs::ScopedTempDir tmp("cbshim");
    const std::string file = cb::fs::to_text(tmp.path() / "t.toml");
    old::switches::write(to_py(file), py::list<old::switches::Table>{guarded, analysed});
    const std::string text = cb::fs::read_text(tmp.path() / "t.toml");
    CB_CHECK(text.find("# absolute table, guard at 0x82000100\n[[switch]]\nbase = 0x8200011C\nr = 11\ndefault = 0x82000144\n") != std::string::npos);
    CB_CHECK(text.find("# XenonAnalyse table, guard at 0x0\n[[switch]]\nbase = 0x82000700\n") != std::string::npos);
    // a kind nobody knows
    old::switches::Table odd = guarded;
    odd.kind = py::str("weird");
    CB_CHECK_THROWS_AS(py::ValueError, old::switches::write(to_py(file), py::list<old::switches::Table>{odd}));
});

CB_TEST("recompile.shims.xenon_locations_and_usage", [] {
    old::xenon::set_root(py::str("C:\\co"));
    CB_CHECK_EQ(from_py(old::xenon::SOURCE), "C:\\co\\native\\xenonrecomp");
    CB_CHECK_EQ(from_py(old::xenon::RECOMP), "C:\\co\\native\\xenonrecomp\\build\\XenonRecomp\\XenonRecomp.exe");
    CB_CHECK_EQ(from_py(old::xenon::ANALYSE), "C:\\co\\native\\xenonrecomp\\build\\XenonAnalyse\\XenonAnalyse.exe");
    CB_CHECK_EQ(from_py(old::xenon::TESTS), "C:\\co\\native\\xenonrecomp\\ppc_tests");
    CB_CHECK_EQ(old::xenon::main(py::list<py::str>{}), 2);                       // (it prints the usage)
    CB_CHECK_EQ(old::xenon::main(py::list<py::str>{py::str("a"), py::str("b")}), 2);
});

}  // namespace
