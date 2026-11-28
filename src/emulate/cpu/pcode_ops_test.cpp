// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops.{hpp,cpp} and Emulator::bindUserOp: a table of named user operations bound once to an emulator, the handler called by operation number with its arguments as
// byte arrays (narrow or wide), a declined handler counted, a misspelt name reported, and the numbers to match what the same operation does through the by-name hook.
#include <cstdio>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/cpu/pcode_ops.hpp"
#include "emulate/cpu/pcode_vector.hpp"

namespace cb {
namespace {

namespace vec = emulate::vec;
namespace ops = emulate::pcode_ops;
using Emu = decompile::sleigh::Emulator;
using Bytes = std::vector<std::uint8_t>;

std::filesystem::path languages_dir() {
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

std::unique_ptr<Emu> x86_64() {
    return std::make_unique<Emu>(languages_dir() / "x86-64.sla", std::vector<std::pair<std::string, int>>{{"longMode", 1}, {"addrsize", 2}, {"bit64", 1}, {"opsize", 1}});
}
Bytes reg(const Emu& emu, const char* name) {
    const Emu::RegRef ref = emu.regRef(name);
    Bytes v(ref.size);
    emu.readRegWide(ref, v.data());
    return v;
}
void setReg(Emu& emu, const char* name, const Bytes& v) { emu.writeRegWide(emu.regRef(name), v.data()); }

// SSE and AArch64 handlers over the library
bool paddsb(Emu&, Emu::UserOpCall& c, void*) {
    if (c.nIn != 2 || c.inSize[0] != c.outSize || c.inSize[1] != c.outSize) return false;
    vec::addSat(c.out, c.in[0], c.in[1], c.outSize, 1, vec::Sign::Signed);
    return true;
}
bool psubusw(Emu&, Emu::UserOpCall& c, void*) {
    vec::subSat(c.out, c.in[0], c.in[1], c.outSize, 2, vec::Sign::Unsigned);
    return true;
}
bool neonSqadd(Emu&, Emu::UserOpCall& c, void*) {   // NEON_sqadd(Rn, Rm, esize in bytes)
    vec::addSat(c.out, c.in[0], c.in[1], c.outSize, c.in[2][0], vec::Sign::Signed);
    return true;
}
bool declines(Emu&, Emu::UserOpCall&, void*) { return false; }
unsigned g_context_calls = 0;
bool countsContext(Emu&, Emu::UserOpCall& c, void* context) {
    ++*static_cast<unsigned*>(context);
    vec::bitXor(c.out, c.in[0], c.in[1], c.outSize);
    return true;
}

}  // namespace

CB_TEST("emulate.pcode_ops.a_bound_sse_operation_runs_by_number_with_wide_arguments", [] {
    std::mt19937_64 rng(77);
    const std::unique_ptr<Emu> emu = x86_64();
    static const ops::Table table{{"paddsb", paddsb}, {"psubusw", psubusw}, {"notAnOperationOfTheDescription", declines}};
    // before binding: the operation is unserved and counted
    const std::uint8_t paddsbXmm[] = {0x66, 0x0f, 0xec, 0xc1};   // paddsb xmm0, xmm1
    const std::uint8_t psubuswXmm[] = {0x66, 0x0f, 0xd9, 0xc1};  // psubusw xmm0, xmm1
    Bytes a(16), b(16);
    for (auto& x : a) x = static_cast<std::uint8_t>(rng());
    for (auto& x : b) x = static_cast<std::uint8_t>(rng());
    setReg(*emu, "XMM0", a);
    setReg(*emu, "XMM1", b);
    emu->write(0x1000, paddsbXmm, sizeof paddsbXmm);
    emu->setPc(0x1000);
    CB_CHECK(emu->step() == Emu::Status::Ok);
    CB_CHECK(emu->unimplementedOps().count("paddsb") == 1);
    const ops::Binding binding = ops::bind(*emu, table);
    CB_CHECK_EQ(binding.bound, std::size_t{2});
    CB_CHECK_EQ(binding.unknown.size(), std::size_t{1});
    CB_CHECK(binding.unknown[0] == "notAnOperationOfTheDescription");
    CB_CHECK(binding.unbound.size() + binding.bound == emu->userOpCount());
    for (int iter = 0; iter < 200; ++iter) {
        for (auto& x : a) x = static_cast<std::uint8_t>(rng() % 5 == 0 ? (rng() & 1 ? 0x7f : 0x80) : rng());
        for (auto& x : b) x = static_cast<std::uint8_t>(rng() % 5 == 0 ? (rng() & 1 ? 0x7f : 0x80) : rng());
        setReg(*emu, "XMM0", a);
        setReg(*emu, "XMM1", b);
        emu->write(0x1000, paddsbXmm, sizeof paddsbXmm);
        emu->setPc(0x1000);
        CB_CHECK(emu->step() == Emu::Status::Ok);
        Bytes want(16);
        for (int i = 0; i < 16; ++i) {
            const int s = static_cast<std::int8_t>(a[i]) + static_cast<std::int8_t>(b[i]);
            want[i] = static_cast<std::uint8_t>(s > 127 ? 127 : s < -128 ? -128 : s);   // the instruction's definition, in plain integers
        }
        CB_CHECK(reg(*emu, "XMM0") == want);
        setReg(*emu, "XMM0", a);
        emu->write(0x1000, psubuswXmm, sizeof psubuswXmm);
        emu->setPc(0x1000);
        CB_CHECK(emu->step() == Emu::Status::Ok);
        for (int i = 0; i < 16; i += 2) {
            const int x = a[i] | (a[i + 1] << 8), y = b[i] | (b[i + 1] << 8), d = x > y ? x - y : 0;
            want[i] = static_cast<std::uint8_t>(d), want[i + 1] = static_cast<std::uint8_t>(d >> 8);
        }
        CB_CHECK(reg(*emu, "XMM0") == want);
    }
    CB_CHECK(emu->unimplementedOps().count("paddsb") == 1);   // only the first, unbound, step
});

CB_TEST("emulate.pcode_ops.a_declining_handler_is_counted_and_a_context_reaches_the_handler", [] {
    const std::unique_ptr<Emu> emu = x86_64();
    unsigned calls = 0;
    static const ops::Table declining{{"paddsb", declines}};
    static const ops::Table counting{{"paddsb", countsContext}};
    const std::uint8_t paddsbXmm[] = {0x66, 0x0f, 0xec, 0xc1};
    emu->write(0x1000, paddsbXmm, sizeof paddsbXmm);
    ops::bind(*emu, declining);
    emu->setPc(0x1000);
    CB_CHECK(emu->step() == Emu::Status::Ok);
    CB_CHECK(emu->unimplementedOps().count("paddsb") == 1);
    ops::bind(*emu, counting, &calls);   // a later binding replaces the earlier one
    emu->setPc(0x1000);
    CB_CHECK(emu->step() == Emu::Status::Ok);
    CB_CHECK_EQ(calls, 1u);
    CB_CHECK_EQ(emu->unimplementedOps().at("paddsb"), std::uint64_t{1});
    emu->clearUserOpBindings();
    emu->setPc(0x1000);
    CB_CHECK(emu->step() == Emu::Status::Ok);
    CB_CHECK_EQ(calls, 1u);
    CB_CHECK_EQ(emu->unimplementedOps().at("paddsb"), std::uint64_t{2});
    (void)g_context_calls;
});

CB_TEST("emulate.pcode_ops.an_aarch64_named_operation_gets_its_constant_argument_and_wide_registers", [] {
    Emu emu(languages_dir() / "AARCH64.sla", {});
    static const ops::Table table{{"NEON_sqadd", neonSqadd}};
    const ops::Binding binding = ops::bind(emu, table);
    CB_CHECK_EQ(binding.bound, std::size_t{1});
    std::mt19937_64 rng(5);
    for (int iter = 0; iter < 100; ++iter) {
        Bytes a(16), b(16);
        for (auto& x : a) x = static_cast<std::uint8_t>(rng());
        for (auto& x : b) x = static_cast<std::uint8_t>(rng());
        setReg(emu, "q1", a);
        setReg(emu, "q2", b);
        const std::uint32_t word = 0x4e220c20;   // sqadd v0.16b, v1.16b, v2.16b
        const std::uint8_t code[4] = {static_cast<std::uint8_t>(word), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 24)};
        emu.write(0x1000, code, 4);
        emu.setPc(0x1000);
        CB_CHECK(emu.step() == Emu::Status::Ok);
        Bytes want(16);
        for (int i = 0; i < 16; ++i) {
            const int s = static_cast<std::int8_t>(a[i]) + static_cast<std::int8_t>(b[i]);
            want[i] = static_cast<std::uint8_t>(s > 127 ? 127 : s < -128 ? -128 : s);
        }
        CB_CHECK(reg(emu, "q0") == want);
    }
    CB_CHECK(emu.unimplementedOps().empty());
    const ops::Table one{{"a", declines}};
    CB_CHECK_EQ(one.size(), std::size_t{1});
});

}  // namespace cb
