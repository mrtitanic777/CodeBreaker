// cb/emulate/pcode_test.cpp -- the p-code lifter, against the real pcodecheck when this machine has it built
// (CB_PCODECHECK, or native/decompiler/build/bin/pcodecheck.exe of the checkout); the tests that need it return
// quietly when it is not there, the rest run anywhere.
#include "cb/emulate/pcode.hpp"

#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <thread>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

using namespace cb;
using namespace cb::emulate::pcode;
namespace sfs = std::filesystem;

namespace {

// The built pcodecheck: the environment's, else the first native/decompiler/build/bin/pcodecheck.exe found from the
// source tree upwards (a worktree of the checkout keeps the build in the main checkout).
sfs::path find_exe() {
    if (auto env = cb::fs::get_env("CB_PCODECHECK"); env && !env->empty()) return *env;
    sfs::path dir = sfs::path(CB_SOURCE_DIR);
    for (int i = 0; i < 6 && !dir.empty(); ++i, dir = dir.parent_path()) {
        const sfs::path exe = dir / "native" / "decompiler" / "build" / "bin" / "pcodecheck.exe";
        if (sfs::exists(exe)) return exe;
        const sfs::path direct = dir / "decompiler" / "build" / "bin" / "pcodecheck.exe";
        if (sfs::exists(direct)) return direct;
    }
    return {};
}

sfs::path languages() { return sfs::path(CB_SOURCE_DIR) / "decompiler" / "languages"; }

PcodeConfig config() { return PcodeConfig{find_exe(), languages()}; }

bool available() { return !find_exe().empty() && sfs::exists(languages() / "x86.sla"); }

const Op* find_op(const Lifted& l, std::string_view opcode) {
    for (const Op& op : l.ops) {
        if (op.opcode == opcode) return &op;
    }
    return nullptr;
}

}  // namespace

CB_TEST("emulate.pcode.mode_names_and_specifications", [] {
    CB_CHECK_EQ(std::string(mode_name(Mode::X86Real16)), "x86-16-real");
    CB_CHECK_EQ(std::string(mode_name(Mode::X86Flat32)), "x86-32-flat");
    CB_CHECK_EQ(std::string(mode_name(Mode::PpcBe64)), "ppc-be-64");
    CB_CHECK_EQ(std::string(sla_file(Mode::X86Real16)), "x86.sla");
    CB_CHECK_EQ(std::string(sla_file(Mode::X86Flat32)), "x86.sla");
    CB_CHECK_EQ(std::string(sla_file(Mode::PpcBe64)), "ppc_64_xenon_be.sla");
});

CB_TEST("emulate.pcode.lifted_works_out_what_it_reads_writes_and_follows", [] {
    // r0 = r0 + 1 (a register op), then a load that reaches memory
    Op add{"INT_ADD", Varnode{Space::Register, 0, 4}, {Varnode{Space::Register, 0, 4}, Varnode{Space::Constant, 1, 4}}};
    Lifted plain(1, {add});
    CB_CHECK(!plain.full);
    CB_CHECK((plain.reads == std::set<std::uint64_t>{0, 1, 2, 3}));
    CB_CHECK((plain.writes == std::set<std::uint64_t>{0, 1, 2, 3}));
    Op load{"LOAD", Varnode{Space::Unique, 0x100, 4}, {Varnode{Space::Constant, 0x1000, 8}, Varnode{Space::Register, 0x14, 4}}};
    Lifted with_load(3, {load});
    CB_CHECK(with_load.full);                                        // a LOAD must be followed op by op
    CB_CHECK((with_load.reads == std::set<std::uint64_t>{0x14, 0x15, 0x16, 0x17}));
    CB_CHECK(with_load.writes.empty());                              // its output is a temporary
    Op ram{"COPY", Varnode{Space::Ram, 0x50103, 2}, {Varnode{Space::Constant, 1, 2}}};
    CB_CHECK(Lifted(1, {ram}).full);                                 // an output in ram
    Op ram_in{"COPY", Varnode{Space::Unique, 0, 2}, {Varnode{Space::Ram, 0x10, 2}}};
    CB_CHECK(Lifted(1, {ram_in}).full);                              // an input in ram
    Op branch{"CBRANCH", std::nullopt, {Varnode{Space::Constant, 5, 1}, Varnode{Space::Register, 0x206, 1}}};
    CB_CHECK(Lifted(2, {branch}).full);
    CB_CHECK(Lifted(1, {}).reads.empty());
});

CB_TEST("emulate.pcode.a_program_that_does_not_lift_is_not_started", [] {
    const sfs::path sla = languages() / "x86.sla";
    CB_CHECK(Lifter::try_start("this-program-does-not-exist.exe", sla, Mode::X86Flat32) == nullptr);
    CB_CHECK_THROWS_AS(ToolError, (void)Lifter("this-program-does-not-exist.exe", sla, Mode::X86Flat32));
    // a program that runs but does not answer with a register table
    if (auto sysroot = cb::fs::get_env("SystemRoot")) {
        const std::filesystem::path hostname = std::filesystem::path(*sysroot) / "System32" / "hostname.exe";
        if (sfs::exists(hostname)) CB_CHECK(Lifter::try_start(hostname, sla, Mode::X86Flat32) == nullptr);
    }
});

CB_TEST("emulate.pcode.config_prefers_the_environment", [] {
    const auto env = cb::fs::get_env("CB_PCODECHECK");
    const PcodeConfig c = PcodeConfig::from_environment("bin", "langs");
    if (!env || env->empty()) CB_CHECK_EQ(c.exe.generic_string(), "bin/pcodecheck.exe");
    else CB_CHECK_EQ(c.exe.string(), *env);
    CB_CHECK_EQ(c.sla(Mode::PpcBe64).generic_string(), "langs/ppc_64_xenon_be.sla");
});

CB_TEST("emulate.pcode.lifter_set_answers_null_when_there_is_no_program", [] {
    LifterSet set(PcodeConfig{"no-such-pcodecheck.exe", languages()});
    CB_CHECK(set.get(Mode::X86Flat32) == nullptr);
    CB_CHECK(set.get(Mode::X86Flat32) == nullptr);                   // not retried, same answer
});

CB_TEST("emulate.pcode.lifts_a_flat_32_bit_instruction", [] {
    if (!available()) return;
    LifterSet set(config());
    Lifter* lifter = set.get(Mode::X86Flat32);
    CB_CHECK(lifter != nullptr);
    if (lifter == nullptr) return;
    const auto* eax = lifter->registers().find("EAX");
    CB_CHECK(eax != nullptr);
    if (eax != nullptr) {
        CB_CHECK_EQ(eax->offset, 0u);
        CB_CHECK_EQ(eax->size, 4u);
    }
    CB_CHECK_EQ(lifter->registers().at("EIP").offset, 0x284u);
    CB_CHECK_EQ(lifter->registers().begin()->first, "EAX");          // the program's order

    const Bytes inc_eax = {0x40};
    const Lifted* inc = lifter->lift(inc_eax, 0);
    CB_CHECK(inc != nullptr);
    if (inc != nullptr) {
        CB_CHECK_EQ(inc->length, 1u);
        CB_CHECK(!inc->full);
        const Op* add = find_op(*inc, "INT_ADD");
        CB_CHECK(add != nullptr);
        if (add != nullptr) CB_CHECK(add->out == (Varnode{Space::Register, 0, 4}));
        CB_CHECK((inc->writes.contains(0) && inc->writes.contains(3)));   // EAX
        CB_CHECK(inc->writes.contains(0x206));                            // ZF
        CB_CHECK(!inc->writes.contains(4));                               // not ECX
    }
    // the same instruction, the same answer (it is lifted once)
    CB_CHECK(lifter->lift(inc_eax, 0) == inc);
    // mov eax, [ebp+8]: reaches memory
    const Bytes mov = {0x8B, 0x45, 0x08};
    const Lifted* load = lifter->lift(mov, 0x100);
    CB_CHECK(load != nullptr);
    if (load != nullptr) {
        CB_CHECK_EQ(load->length, 3u);
        CB_CHECK(load->full);
        CB_CHECK(load->reads.contains(0x14));                             // EBP
        CB_CHECK(find_op(*load, "LOAD") != nullptr);
    }
    // bytes that do not decode
    const Bytes junk = {0xFF, 0xFF};
    CB_CHECK(lifter->lift(junk, 0) == nullptr);
});

CB_TEST("emulate.pcode.lifts_a_16_bit_instruction_in_its_window", [] {
    if (!available()) return;
    LifterSet set(config());
    Lifter* lifter = set.get(Mode::X86Real16);
    CB_CHECK(lifter != nullptr);
    if (lifter == nullptr) return;
    // call near +0: the target is an offset in the segment, the window's high bits dropped
    const Bytes call = {0xE8, 0x00, 0x00};
    const Lifted* l = lifter->lift(call, 0x100);
    CB_CHECK(l != nullptr);
    if (l == nullptr) return;
    const Op* branch = find_op(*l, "BRANCH");
    CB_CHECK(branch != nullptr);
    if (branch != nullptr) {
        CB_CHECK(branch->inputs.size() == 1u);
        const Varnode* target = std::get_if<Varnode>(&branch->inputs[0]);
        CB_CHECK(target != nullptr);
        if (target != nullptr) {
            CB_CHECK(target->space == Space::Ram);
            CB_CHECK_EQ(target->offset, 0x103u);
        }
    }
    // int 21h: a user operation named by its first input
    const Bytes interrupt = {0xCD, 0x21};
    const Lifted* swi = lifter->lift(interrupt, 0x10);
    CB_CHECK(swi != nullptr);
    if (swi != nullptr) {
        const Op* other = find_op(*swi, "CALLOTHER");
        CB_CHECK(other != nullptr);
        if (other != nullptr) {
            CB_CHECK(!other->inputs.empty());
            if (!other->inputs.empty()) CB_CHECK(std::get<std::string>(other->inputs[0]) == "swi");
        }
        CB_CHECK(swi->full);
    }
});

CB_TEST("emulate.pcode.lifts_powerpc", [] {
    if (!available() || !sfs::exists(languages() / "ppc_64_xenon_be.sla")) return;
    LifterSet set(config());
    Lifter* lifter = set.get(Mode::PpcBe64);
    if (lifter == nullptr) return;
    const Bytes blr = {0x4E, 0x80, 0x00, 0x20};
    const Lifted* l = lifter->lift(blr, 0);
    CB_CHECK(l != nullptr);
    if (l != nullptr) {
        CB_CHECK_EQ(l->length, 4u);
        CB_CHECK(l->full);                                                // RETURN is followed
        CB_CHECK(find_op(*l, "RETURN") != nullptr);
    }
});

CB_TEST("emulate.pcode.line_is_pcodechecks_own_text", [] {
    if (!available()) return;
    LifterSet set(config());
    Lifter* lifter = set.get(Mode::X86Flat32);
    if (lifter == nullptr) return;
    const std::string line = lifter->line(Bytes{0x40}, 0);
    CB_CHECK(line.starts_with("1 INT_SCARRY"));
    CB_CHECK(lifter->line(Bytes{0xFF, 0xFF}, 0).starts_with("err"));
});

CB_TEST("emulate.pcode.threads_share_one_lifter", [] {
    if (!available()) return;
    LifterSet set(config());
    Lifter* lifter = set.get(Mode::X86Flat32);
    if (lifter == nullptr) return;
    std::vector<std::thread> threads;
    std::atomic<int> good{0};
    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&, t] {
            for (int i = 0; i < 40; ++i) {
                const Bytes code = {static_cast<std::uint8_t>(0x40 + (i + t) % 8)};   // inc/dec of a register
                const Lifted* l = lifter->lift(code, 0);
                if (l != nullptr && l->length == 1) ++good;
            }
        });
    }
    for (auto& th : threads) th.join();
    CB_CHECK_EQ(good.load(), 160);
});
