// cb/source/ppc_recompiled_runs_test.cpp -- recompiled PowerPC runs and gives the answers the C it was compiled from gives.
//
// Each case is C compiled with the Xbox 360 compiler (settings.json: powerpc_tools), its PowerPC recompiled to C++ (recompile/powerpc), built with g++
// against the rexglue kernel core's runtime headers, run, and compared with the same computation done here. Also: two functions dispatched through the
// indirect-call table, a function reading big-endian guest memory, and VMX128 words decoded first. A machine without the compiler, g++ or the kernel
// core's headers prints SKIP. (It sits in area source because that area sees both the compiler's wrapper and the recompiler.)
//
// Names here are file-local to this unity unit: every one starts with `rr_`.
#include <cmath>
#include <cstdint>
#include <format>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/process.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/vmx128.hpp"
#include "cb/recompile/disassemble.hpp"
#include "cb/recompile/powerpc.hpp"
#include "cb/source/pipeline_ppc.hpp"

namespace rcp = cb::recompile::powerpc;
namespace stdfs = std::filesystem;
using cb::Bytes;

namespace {

constexpr std::uint32_t kRrVa = 0x82000000;

struct RrTools {
    stdfs::path cl;
    stdfs::path gpp;
    std::vector<stdfs::path> includes;
};

// The three tools, or why not.
std::optional<RrTools> rr_tools(std::string& why) {
    try {
        const cb::config::Config config = cb::config::Config::load(cb::config::current().main_checkout());
        const std::optional<stdfs::path> folder = config.path("powerpc_tools");
        std::error_code ec;
        if (!folder || !stdfs::is_regular_file(*folder / "cl.exe", ec)) {
            why = "no PowerPC compiler set up";
            return std::nullopt;
        }
        rcp::CompilerEnv env;
        env.gpp = config.path("gpp");
        env.checkout_root = stdfs::path(CB_SOURCE_DIR).parent_path();
        const std::optional<stdfs::path> gpp = rcp::find_compiler(env);
        if (!gpp) {
            why = "no g++";
            return std::nullopt;
        }
        RrTools tools{*folder / "cl.exe", *gpp, rcp::kernel_includes(env.checkout_root)};
        if (tools.includes.empty()) {
            why = "the rexglue kernel core is not in the tree (native/cb/recompile/rexglue/kernel)";
            return std::nullopt;
        }
        return tools;
    } catch (const cb::Error& e) {
        why = e.what();
        return std::nullopt;
    }
}

cb::recompile::disassemble::Disassembler& rr_dis() {
    static cb::recompile::disassemble::Disassembler dis(
        [](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
            if (const auto decoded = cb::decompile::vmx128::decode(word)) return std::make_pair(decoded->mnemonic, decoded->operands);
            return std::nullopt;
        });
    return dis;
}

// One C function compiled with the Xbox 360 compiler and recompiled at `va`: its C++ (every instruction translated, or the case fails).
std::string rr_leaf(const RrTools& tools, const cb::fs::ScopedTempDir& work, std::string_view csrc, std::string_view name, std::uint32_t va) {
    const auto built = cb::source::pipeline::ppc_compile_function(tools.cl, "", csrc, name, "/O2", work.path());
    CB_CHECK(built.has_value());
    if (!built) return {};
    const rcp::Recompiled r = rcp::recompile_function(rr_dis(), built->bytes, va, static_cast<std::uint32_t>(built->bytes.size()));
    CB_CHECK_EQ(r.stats.unimplemented, std::size_t{0});
    return r.cpp;
}

// Build a harness against the runtime a generated project uses (the kernel core's headers: its PPCContext, FPSCR, SIMD layer and traps are all
// inline) and run it: its output, split at white space.
std::vector<std::string> rr_run(const RrTools& tools, const std::string& harness) {
    cb::fs::ScopedTempDir dir("cb-ppc-run");
    cb::fs::write_text(dir.path() / "rex_ppc_min.h", rcp::runtime_header());
    cb::fs::write_text(dir.path() / "t.cpp", harness);
    cb::process::ProcessSpec build;
    build.argv = {cb::fs::to_text(tools.gpp), "-std=c++23", "-O1", "-w", "-DFMT_HEADER_ONLY", "-I", cb::fs::to_text(dir.path())};
    for (const stdfs::path& inc : tools.includes) {
        build.argv.emplace_back("-I");
        build.argv.push_back(cb::fs::to_text(inc));
    }
    build.argv.insert(build.argv.end(), {"-o", cb::fs::to_text(dir.path() / "t.exe"), cb::fs::to_text(dir.path() / "t.cpp")});
    build.timeout = std::chrono::minutes(10);
    const cb::process::ProcessResult compiled = cb::process::run(build);
    if (compiled.exit_code != 0) {
        std::cout << "g++: " << std::string(compiled.err.begin(), compiled.err.end()).substr(0, 600) << "\n";
        CB_CHECK(false);
        return {};
    }
    cb::process::ProcessSpec run;
    run.argv = {cb::fs::to_text(dir.path() / "t.exe")};
    run.timeout = std::chrono::seconds(60);
    const cb::process::ProcessResult ran = cb::process::run(run);
    CB_CHECK(ran.ok());
    std::istringstream in(std::string(ran.out.begin(), ran.out.end()));
    std::vector<std::string> words;
    for (std::string w; in >> w;) words.push_back(w);
    return words;
}

const char* const kRrPrologue = "#include \"rex_ppc_min.h\"\n#include <cstdio>\nvoid ppc_call_indirect(PPCContext&, uint8_t*, uint32_t) {}\n";

std::int32_t rr_wrap(std::int64_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v)); }

struct RrIntCase {
    const char* expr;
    std::int32_t a, b;
    std::int32_t want;   // C's value with the 32-bit wrap, the truncating division and the unsigned shift
};

}  // namespace

// test_integer_leaves
CB_TEST("source.ppc_recompiled.integer_leaves_give_what_the_c_gives", [] {
    std::string why;
    const auto tools = rr_tools(why);
    if (!tools) {
        std::cout << "SKIP " << why << "\n";
        return;
    }
    const RrIntCase cases[] = {
        {"a + b", 7, 5, rr_wrap(7 + 5)},
        {"a * 3 + b", 7, 5, rr_wrap(7 * 3 + 5)},
        {"a * b", 6, 7, rr_wrap(6 * 7)},
        {"(a << 4) - b", 10, 3, rr_wrap((10 << 4) - 3)},
        {"(a & b) | 7", 0x33, 0x14, rr_wrap((0x33 & 0x14) | 7)},
        {"a - b * 2", 100, 9, rr_wrap(100 - 9 * 2)},
        {"a * a + b * b", 8, 6, rr_wrap(8 * 8 + 6 * 6)},
        {"a / b + a % b", -37, 5, rr_wrap(-37 / 5 + -37 % 5)},
        {"(unsigned)a >> (b & 31)", -1, 7, rr_wrap(0xFFFFFFFFu >> 7)},
        {"a > b ? a - b : b - a", 3, 11, rr_wrap(11 - 3)},
    };
    cb::fs::ScopedTempDir work("cb-ppc-leaf");
    for (const RrIntCase& c : cases) {
        const std::string cpp = rr_leaf(*tools, work, std::format("int f(int a, int b) {{ return {}; }}\n", c.expr), "f", kRrVa);
        const std::vector<std::string> out = rr_run(
            *tools, std::string(kRrPrologue) + cpp +
                        std::format("\nint main() {{ static uint8_t mem[1 << 20]; PPCContext ctx{{}}; ctx.r1.u32 = 0x8000; ctx.r3.s64 = {}; ctx.r4.s64 = {}; "
                                    "sub_{:08X}(ctx, mem); std::printf(\"%d\\n\", ctx.r3.s32); }}\n",
                                    c.a, c.b, kRrVa));
        CB_CHECK(!out.empty());
        if (!out.empty()) CB_CHECK_EQ(std::stoi(out[0]), c.want);
    }
});

// test_floating_point
CB_TEST("source.ppc_recompiled.floating_point_gives_what_the_c_gives", [] {
    std::string why;
    const auto tools = rr_tools(why);
    if (!tools) {
        std::cout << "SKIP " << why << "\n";
        return;
    }
    cb::fs::ScopedTempDir work("cb-ppc-leaf");
    const std::string cpp = rr_leaf(*tools, work, "float f(float a, float b) { return a * b - a * a; }\n", "f", kRrVa);
    const std::vector<std::string> out =
        rr_run(*tools, std::string(kRrPrologue) + cpp +
                           std::format("\nint main() {{ static uint8_t mem[1 << 20]; PPCContext ctx{{}}; ctx.r1.u32 = 0x8000; ctx.f1.f64 = 2.5; ctx.f2.f64 = 4.0; "
                                       "sub_{:08X}(ctx, mem); std::printf(\"%.4f\\n\", ctx.f1.f64); }}\n",
                                       kRrVa));
    CB_CHECK(!out.empty());
    if (!out.empty()) CB_CHECK(std::abs(std::stod(out[0]) - 3.75) < 1e-6);   // (no constants: the harness maps no data)
});

// test_loop_over_memory: a loop summing big-endian words of guest memory; the loads byte-swap as the console's do.
CB_TEST("source.ppc_recompiled.a_loop_over_big_endian_memory", [] {
    std::string why;
    const auto tools = rr_tools(why);
    if (!tools) {
        std::cout << "SKIP " << why << "\n";
        return;
    }
    cb::fs::ScopedTempDir work("cb-ppc-leaf");
    const std::string cpp =
        rr_leaf(*tools, work, "int f(const int *p, int n) { int s = 0; while (n-- > 0) s += *p++; return s; }\n", "f", kRrVa);
    const std::int32_t words[] = {3, -7, 1000, 123456, 5};
    std::string init;
    int sum = 0;
    for (std::size_t i = 0; i < std::size(words); ++i) {
        sum += words[i];
        const std::uint32_t w = static_cast<std::uint32_t>(words[i]);
        for (int k = 0; k < 4; ++k) init += std::format("mem[0x1000 + {}] = {}; ", 4 * i + k, (w >> (24 - 8 * k)) & 0xFF);
    }
    const std::vector<std::string> out = rr_run(
        *tools, std::string(kRrPrologue) + cpp +
                    std::format("\nint main() {{ static uint8_t mem[1 << 20]; {} PPCContext ctx{{}}; ctx.r1.u32 = 0x8000; ctx.r3.u32 = 0x1000; "
                                "ctx.r4.s64 = {}; sub_{:08X}(ctx, mem); std::printf(\"%d\\n\", ctx.r3.s32); }}\n",
                                init, std::size(words), kRrVa));
    CB_CHECK(!out.empty());
    if (!out.empty()) CB_CHECK_EQ(std::stoi(out[0]), sum);
});

// test_indirect_call_table: two functions dispatched through the table of function pointers by guest address.
CB_TEST("source.ppc_recompiled.the_indirect_call_table_dispatches_by_guest_address", [] {
    std::string why;
    const auto tools = rr_tools(why);
    if (!tools) {
        std::cout << "SKIP " << why << "\n";
        return;
    }
    cb::fs::ScopedTempDir work("cb-ppc-leaf");
    const std::uint32_t a = 0x82001000, b = 0x82002000;
    const std::string dbl = rr_leaf(*tools, work, "int dbl(int x) { return x * 2; }\n", "dbl", a);
    const std::string inc = rr_leaf(*tools, work, "int inc(int x) { return x + 10; }\n", "inc", b);
    const std::vector<std::string> out = rr_run(
        *tools, std::string("#include \"rex_ppc_min.h\"\n#include <cstdio>\n") + dbl + inc +
                    std::format(R"(
struct Mapping {{ uint32_t addr; PPCFunc* func; }};
static Mapping table[] = {{{{0x{0:08X}, sub_{0:08X}}}, {{0x{1:08X}, sub_{1:08X}}}, {{0, nullptr}}}};
void ppc_call_indirect(PPCContext& ctx, uint8_t* base, uint32_t addr) {{
    for (auto* m = table; m->func; ++m) if (m->addr == addr) {{ m->func(ctx, base); return; }}
}}
int main() {{ static uint8_t mem[1 << 20]; PPCContext ctx{{}}; ctx.r1.u32 = 0x8000; ctx.r3.s64 = 5;
    ctx.ctr.u32 = 0x{0:08X}; ppc_call_indirect(ctx, mem, ctx.ctr.u32);
    ctx.ctr.u32 = 0x{1:08X}; ppc_call_indirect(ctx, mem, ctx.ctr.u32);
    std::printf("%d\n", ctx.r3.s32); }}
)",
                                a, b));
    CB_CHECK(!out.empty());
    if (!out.empty()) CB_CHECK_EQ(std::stoi(out[0]), 20);   // (5 * 2, then + 10)
});

// test_vmx128_first: a VMX128 word is read as VMX128, not as the later instruction a stock decoder makes of it. (Needs no tool.)
CB_TEST("source.ppc_recompiled.a_vmx128_word_is_read_as_vmx128", [] {
    const std::uint32_t word = 0x102320C3;   // lvx128 v1,r3,r4
    const Bytes code = {0x10, 0x23, 0x20, 0xC3};
    const auto rows = rr_dis().listing(code, kRrVa, 0, 1);
    CB_CHECK_EQ(rows.size(), std::size_t{1});
    if (rows.empty()) return;
    CB_CHECK_EQ(rows[0].mnemonic, "lvx128");
    CB_CHECK_EQ(rows[0].operands, "v1,r3,r4");
    const std::vector<std::string> stmts = rcp::emit_insn(rows[0], rcp::FunctionBounds{kRrVa, kRrVa + 4});
    CB_CHECK(!stmts.empty());
    for (const std::string& s : stmts) CB_CHECK(s.find("UNIMPLEMENTED") == std::string::npos);
    CB_CHECK_EQ(rows[0].word, word);
});
