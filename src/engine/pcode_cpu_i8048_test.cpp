// cb-unity: alone
// The Odyssey 2 lane's Intel 8048 on the one emulator (engine/pcode_cpu_i8048.cpp): the same run as the own core (emulate/i8048) on random instructions and programs. The lane has no
// homebrew folder in the samples (an Odyssey 2 cartridge is not among them), so the random runs and the lane's own tests (machine_o2_test: a cartridge comes back identical and an
// edit diverges, now on the emulator) are the proof. The devices answer with a count of the accesses made, so a read the emulator made more or fewer than the own core shows.
#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <map>
#include <memory>
#include <random>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/i8048.hpp"
#include "engine/pcode_cpu_i8048.hpp"

namespace cb {

namespace e48t = cb::emulate::i8048;

namespace {

std::string i8048_describe(const e48t::RunResult& a, const e48t::RunResult& b) {
    std::string out;
    if (a.exit() != b.exit()) out += std::format("exit('{}' / '{}') ", a.exit(), b.exit());
    if (a.steps != b.steps) out += std::format("steps[{}/{}] ", a.steps, b.steps);
    const auto reg = [&](const std::string& name, unsigned x, unsigned y) {
        if (x != y) out += std::format("{}[own {:X} emu {:X}] ", name, x, y);
    };
    const e48t::Registers &p = a.regs, &q = b.regs;
    reg("a", p.a, q.a);
    reg("pc", p.pc, q.pc);
    reg("c", p.c, q.c);
    reg("ac", p.ac, q.ac);
    reg("f0", p.f0, q.f0);
    reg("f1", p.f1, q.f1);
    reg("rb1", p.rb1, q.rb1);
    reg("mb1", p.mb1, q.mb1);
    reg("sp", p.sp, q.sp);
    reg("timer", p.timer, q.timer);
    reg("timer_flag", p.timer_flag, q.timer_flag);
    reg("timer_on", p.timer_on, q.timer_on);
    reg("counter_on", p.counter_on, q.counter_on);
    reg("ext_int", p.ext_int, q.ext_int);
    reg("timer_int", p.timer_int, q.timer_int);
    reg("p1", p.p1, q.p1);
    reg("p2", p.p2, q.p2);
    for (std::size_t i = 0; i < p.ram.size(); ++i) reg(std::format("ram{}", i), p.ram[i], q.ram[i]);
    if (a.fault.has_value() != b.fault.has_value()) out += "fault-presence ";
    else if (a.fault && (a.fault->pc != b.fault->pc || a.fault->reason != b.fault->reason)) out += std::format("fault['{}' / '{}'] ", a.fault->reason, b.fault->reason);
    if (a.io.size() != b.io.size()) out += std::format("io[{}/{}] ", a.io.size(), b.io.size());
    else
        for (std::size_t i = 0; i < a.io.size(); ++i)
            if (a.io[i].kind != b.io[i].kind || a.io[i].place != b.io[i].place || a.io[i].address != b.io[i].address || a.io[i].value != b.io[i].value) {
                out += std::format("io@{} ", i);
                break;
            }
    return out;
}

// A machine as the lane makes it: program memory, a data space that keeps what is written, and devices whose answers change with every access.
struct I8048Machine {
    e48t::Bus bus;
    std::shared_ptr<std::array<std::uint8_t, 256>> data = std::make_shared<std::array<std::uint8_t, 256>>();
    I8048Machine(const Bytes& image) {
        bus.load(0, image);
        for (std::size_t i = 0; i < data->size(); ++i) (*data)[i] = static_cast<std::uint8_t>(i * 7 + 1);
        auto reads = std::make_shared<unsigned>(0);
        auto d = data;
        bus.set_read([d, reads](e48t::Place place, std::uint8_t address) -> std::uint8_t {
            const unsigned n = (*reads)++;
            switch (place) {
                case e48t::Place::Data: return (*d)[address];
                case e48t::Place::T0: case e48t::Place::T1: case e48t::Place::Int: return static_cast<std::uint8_t>((n >> 1) & 1);
                default: return static_cast<std::uint8_t>(n * 37 + static_cast<unsigned>(place) * 11 + 0x5A);
            }
        });
        bus.set_write([d](e48t::Place place, std::uint8_t address, std::uint8_t value) {
            if (place == e48t::Place::Data) (*d)[address] = value;
        });
    }
};

void i8048_compare_random(bool single, const char* title) {
    std::mt19937 rng(single ? 20261126u : 20261127u);
    std::map<std::string, unsigned> by_kind;
    std::map<std::string, std::string> example;
    unsigned runs = 0, different = 0;
    const unsigned total = [] {
        const auto given = fs::get_env("CB_PCODE_EXHAUSTIVE");
        return given && !given->empty() && *given != "0" ? static_cast<unsigned>(std::strtoul(given->c_str(), nullptr, 10)) : 12000u;
    }();
    for (unsigned n = 0; n < total; ++n) {
        Bytes image(4096, 0);
        for (std::size_t i = 0x400; i < 0x1000; ++i) image[i] = static_cast<std::uint8_t>(rng());
        // where the run starts: mostly inside a page, now and then in the last bytes of one (a jump or a table read there leaves the page)
        std::uint16_t entry = static_cast<std::uint16_t>(0x400 + (rng() % 0xB00));
        if (const unsigned where = rng() % 8; where == 0) entry = static_cast<std::uint16_t>((entry & 0xF00) | 0xFE);
        else if (where == 1) entry = static_cast<std::uint16_t>((entry & 0xF00) | 0xFF);
        if (entry > 0xFF8) entry = 0xFF0;
        std::uint8_t first = 0;
        if (single) {   // every opcode, each many times with other registers and operands
            first = static_cast<std::uint8_t>(n % 256);
            image[entry] = first;
        } else {
            first = image[entry];
        }
        e48t::RunOptions options;
        options.max_steps = single ? 1 : 40;
        options.a = static_cast<std::uint8_t>((rng() & 3) == 0 ? rng() & 0x0F : rng());
        for (std::uint8_t& r : options.r) r = static_cast<std::uint8_t>((rng() & 1) ? rng() & 0x3F : rng());

        Bytes data[2];
        e48t::RunResult rr[2];
        for (int core = 0; core < 2; ++core) {
            I8048Machine machine(image);
            rr[core] = core == 1 ? engine::pcode_cpu::run(machine.bus, entry, options) : e48t::run(machine.bus, entry, options);
            data[core].assign(machine.data->begin(), machine.data->end());
        }
        ++runs;
        const std::string what = i8048_describe(rr[0], rr[1]);
        if (what.empty() && data[0] == data[1]) continue;
        ++different;
        std::string fields = what;
        if (!single && rr[0].exit() == rr[1].exit() && rr[0].steps != 0) {   // (a program: the instruction after which they differ is what is at fault)
            unsigned k0 = 0;
            for (unsigned k = 1; k <= options.max_steps && k0 == 0; ++k) {
                e48t::RunOptions o2 = options;
                o2.max_steps = k;
                e48t::RunResult ra, rb;
                Bytes da, db;
                for (int core = 0; core < 2; ++core) {
                    I8048Machine machine(image);
                    (core == 1 ? rb : ra) = core == 1 ? engine::pcode_cpu::run(machine.bus, entry, o2) : e48t::run(machine.bus, entry, o2);
                    (core == 1 ? db : da).assign(machine.data->begin(), machine.data->end());
                }
                if (!i8048_describe(ra, rb).empty() || da != db) k0 = k;
            }
            fields += std::format("[first differing step {} at {:03X}] ", k0, entry);
        }
        std::string key = single ? std::format("1: {:02X}", first) : std::string("N: ") + what.substr(0, 24);
        ++by_kind[key];
        if (!example.count(key))
            example[key] = std::format("at {:03X}: {:02X} {:02X} {:02X}: {}", entry, image[entry], image[entry + 1], image[entry + 2], fields);
    }
    std::printf("pcode_cpu i8048 %s: %u runs, %u differ\n", title, runs, different);
    unsigned shown = 0;
    for (const auto& [k, c] : by_kind)
        if (++shown <= 40) std::printf("    %u x %s   e.g. %s\n", c, k.c_str(), example[k].c_str());
    CB_CHECK_EQ(different, 0u);
}

}  // namespace

CB_TEST("engine.pcode_cpu.i8048_instructions_run_as_the_own_core_does", [] { i8048_compare_random(true, "instructions"); });

CB_TEST("engine.pcode_cpu.i8048_programs_run_as_the_own_core_does", [] { i8048_compare_random(false, "programs"); });

// Every accumulator and operand through ADD and then DA (the carries DA reads are those the add left), ADDC, and the register exchange XCHD at every pair of low digits.
CB_TEST("engine.pcode_cpu.i8048_decimal_adjust_and_exchange_run_as_the_own_core_does_on_every_pair_of_operands", [] {
    unsigned runs = 0, different = 0;
    for (unsigned program = 0; program < 3; ++program) {
        for (unsigned a = 0; a < 256; ++a) {
            for (unsigned x = 0; x < 256; ++x) {
                Bytes image(4096, 0);
                std::uint16_t at = 0x400;
                if (program == 0) {   // ADD A,#x then DA A
                    image[at] = 0x03, image[at + 1] = static_cast<std::uint8_t>(x), image[at + 2] = 0x57;
                } else if (program == 1) {   // ADD A,#x then ADDC A,#x then DA A
                    image[at] = 0x03, image[at + 1] = static_cast<std::uint8_t>(x), image[at + 2] = 0x13, image[at + 3] = static_cast<std::uint8_t>(x), image[at + 4] = 0x57;
                } else {   // XCHD A,@R0 with R0 pointing at a byte of internal RAM that holds x (R1 is stored there: R0 = 1)
                    image[at] = 0x30;
                }
                e48t::RunOptions options;
                options.max_steps = program == 0 ? 2 : program == 1 ? 3 : 1;
                options.a = static_cast<std::uint8_t>(a);
                options.r[0] = program == 2 ? 1 : 0;
                options.r[1] = static_cast<std::uint8_t>(x);
                e48t::RunResult rr[2];
                for (int core = 0; core < 2; ++core) {
                    I8048Machine machine(image);
                    rr[core] = core == 1 ? engine::pcode_cpu::run(machine.bus, at, options) : e48t::run(machine.bus, at, options);
                }
                ++runs;
                if (const std::string what = i8048_describe(rr[0], rr[1]); !what.empty() && ++different <= 6) std::printf("  program %u a %02X x %02X: %s\n", program, a, x, what.c_str());
            }
        }
    }
    std::printf("pcode_cpu i8048 arithmetic: %u runs, %u differ\n", runs, different);
    CB_CHECK_EQ(different, 0u);
});

}  // namespace cb
