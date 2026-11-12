// Tests of cb/emulate/unicorn_uc.hpp on the real prebuilt unicorn.dll: machine code run in each mode, memory and registers, the hooks, a saved
// context put back, and what is thrown when Unicorn refuses.
//
// DIVERGES from the Python module and its port:
//  - a call that fails throws UcFault (an EmulationError) carrying Unicorn's error number and text, not unicorn.UcError;
//  - the engine takes the library's table as an argument (the default is cb/core's prebuilt DLL), where the port loaded a DLL by path
//    once for the process (`load_unicorn`); register names are the upper-case names without the `UC_X86_REG_` prefix;
//  - an engine of a width other than 16, 32 or 64 bits is a UsageError (the port made a 32-bit one);
//  - reg_read_float is the first word of the register as a double, as the port's was (the Python binding's answer for ST0 was one
//    integer): kept so that judged runs are the same; see _bugs/LIST.md.
#include "cb/emulate/unicorn_uc.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/unicorn_api.hpp"

using namespace cb;
using namespace cb::emulate::unicorn_uc;

namespace {

constexpr std::uint64_t kCode = 0x1000;

int reg(const char* name) { return *x86_register(name); }

// Maps a page of code, writes `code` there and runs it to its end.
std::uint64_t run(core16::Uc& uc, const Bytes& code, const char* result) {
    uc.mem_map(kCode, 0x1000);
    uc.mem_write(kCode, code);
    uc.emu_start(kCode, kCode + code.size());
    return uc.reg_read(reg(result));
}

}  // namespace

CB_TEST("emulate.unicorn_uc.registers_by_name", [] {
    CB_CHECK(x86_register("EAX").has_value());
    CB_CHECK(x86_register("EFLAGS").has_value());
    CB_CHECK(!x86_register("eax").has_value());    // (upper case, as Unicorn names them)
    CB_CHECK(!x86_register("NOPE").has_value());
    CB_CHECK(x86_registers().size() > 100);
    CB_CHECK_EQ(*x86_register("AL"), 2);           // UC_X86_REG_AL
    CB_CHECK(*x86_register("EAX") != *x86_register("RAX"));
});

CB_TEST("emulate.unicorn_uc.runs_32_bit_code", [] {
    auto uc = open_x86(32);
    // mov eax, 5 ; inc eax ; add eax, 0x10
    CB_CHECK_EQ(run(*uc, {0xB8, 0x05, 0x00, 0x00, 0x00, 0x40, 0x83, 0xC0, 0x10}, "EAX"), 0x16u);
});

CB_TEST("emulate.unicorn_uc.runs_16_and_64_bit_code", [] {
    auto uc16 = open_x86(16);
    CB_CHECK_EQ(run(*uc16, {0xB8, 0x34, 0x12, 0x40}, "AX"), 0x1235u);   // mov ax, 0x1234 ; inc ax
    auto uc64 = open_x86(64);
    CB_CHECK_EQ(run(*uc64, {0x48, 0xC7, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x48, 0xFF, 0xC0}, "RAX"), 0u);   // mov rax, -1 ; inc rax
    CB_CHECK_THROWS_AS(UsageError, open_x86(8));
});

CB_TEST("emulate.unicorn_uc.memory_and_registers", [] {
    auto uc = open_x86(32);
    uc->mem_map(0x5000, 0x2000);
    const Bytes data = {1, 2, 3, 4, 5};
    uc->mem_write(0x5000, data);
    CB_CHECK(uc->mem_read(0x5000, 5) == data);
    CB_CHECK(uc->mem_read(0x5003, 4) == (Bytes{4, 5, 0, 0}));
    uc->mem_unmap(0x6000, 0x1000);
    CB_CHECK_THROWS_AS(UcFault, uc->mem_read(0x6000, 1));   // gone
    uc->reg_write(reg("EBX"), 0xDEADBEEF);
    CB_CHECK_EQ(uc->reg_read(reg("EBX")), 0xDEADBEEFu);
    CB_CHECK_EQ(uc->reg_read(reg("BL")), 0xEFu);
    CB_CHECK(uc->handle() != nullptr);
});

CB_TEST("emulate.unicorn_uc.errors_carry_unicorns_number_and_text", [] {
    auto uc = open_x86(32);
    try {
        (void)uc->mem_read(0x123000, 4);   // never mapped
        CB_CHECK(false);
    } catch (const UcFault& fault) {
        CB_CHECK_EQ(fault.code(), 6);   // UC_ERR_READ_UNMAPPED
        CB_CHECK(!describe_error(fault.code()).empty());
        CB_CHECK(std::string(fault.what()).find(describe_error(6)) != std::string::npos);
    }
    CB_CHECK_THROWS_AS(UcFault, uc->mem_map(0x1001, 0x1000));   // not page aligned
    uc->mem_map(kCode, 0x1000);
    uc->mem_write(kCode, Bytes{0x8B, 0x00});   // mov eax, [eax]: eax is 0, which is not mapped
    CB_CHECK_THROWS_AS(UcFault, uc->emu_start(kCode, kCode + 2));
});

CB_TEST("emulate.unicorn_uc.code_hook_sees_each_instruction", [] {
    auto uc = open_x86(32);
    std::vector<std::pair<std::uint64_t, std::uint32_t>> seen;
    uc->mem_map(kCode, 0x1000);
    uc->mem_write(kCode, Bytes{0xB8, 0x05, 0x00, 0x00, 0x00, 0x40, 0x90});   // mov eax,5 ; inc eax ; nop
    uc->hook_add_code([&](core16::Uc&, std::uint64_t address, std::uint32_t size) { seen.emplace_back(address, size); }, kCode, kCode + 0x100);
    uc->emu_start(kCode, kCode + 7);
    CB_CHECK_EQ(seen.size(), std::size_t(3));
    CB_CHECK(seen[0] == std::make_pair(kCode, std::uint32_t(5)));
    CB_CHECK(seen[1] == std::make_pair(kCode + 5, std::uint32_t(1)));
    CB_CHECK(seen[2] == std::make_pair(kCode + 6, std::uint32_t(1)));
});

CB_TEST("emulate.unicorn_uc.port_hooks", [] {
    auto uc = open_x86(32);
    std::vector<std::uint32_t> written;
    uc->hook_add_insn_in([](core16::Uc&, std::uint32_t port, std::uint32_t size) { return port == 0x60 && size == 1 ? 0x2Au : 0u; });
    uc->hook_add_insn_out([&](core16::Uc&, std::uint32_t port, std::uint32_t size, std::uint32_t value) {
        written.push_back(port);
        written.push_back(size);
        written.push_back(value);
    });
    // mov dx, 0x60 ; in al, dx ; mov dx, 0x61 ; out dx, al
    CB_CHECK_EQ(run(*uc, {0x66, 0xBA, 0x60, 0x00, 0xEC, 0x66, 0xBA, 0x61, 0x00, 0xEE}, "AL"), 0x2Au);
    CB_CHECK(written == (std::vector<std::uint32_t>{0x61, 1, 0x2A}));
});

CB_TEST("emulate.unicorn_uc.a_callback_that_throws_stops_the_run", [] {
    auto uc = open_x86(32);
    uc->mem_map(kCode, 0x1000);
    uc->mem_write(kCode, Bytes{0xB8, 0x05, 0x00, 0x00, 0x00, 0x40, 0x40, 0x40});
    int calls = 0;
    uc->hook_add_code(
        [&](core16::Uc&, std::uint64_t address, std::uint32_t) {
            if (++calls == 2) throw std::runtime_error("stop at " + std::to_string(address));
        },
        kCode, kCode + 0x100);
    CB_CHECK_THROWS_AS(std::runtime_error, uc->emu_start(kCode, kCode + 8));
    CB_CHECK_EQ(calls, 2);   // (not run to the end: the second instruction stopped it)
});

CB_TEST("emulate.unicorn_uc.saved_context_is_put_back", [] {
    auto uc = open_x86(32);
    uc->reg_write(reg("EAX"), 7);
    const auto saved = uc->context_save();
    uc->reg_write(reg("EAX"), 99);
    CB_CHECK_EQ(uc->reg_read(reg("EAX")), 99u);
    uc->context_restore(*saved);
    CB_CHECK_EQ(uc->reg_read(reg("EAX")), 7u);
    uc->ctl_flush_tb();   // (no code translated yet: nothing to do, nothing to fail)
});

CB_TEST("emulate.unicorn_uc.descriptor_table_register", [] {
    auto uc = open_x86(32);
    uc->reg_write_descriptor(reg("GDTR"), {0, 0x1000, 0xFF, 0});
    CB_CHECK(uc->handle() != nullptr);
    CB_CHECK_NOTHROW(uc->reg_write_descriptor(reg("LDTR"), {0, 0x2000, 0x7F, 0}));
});

CB_TEST("emulate.unicorn_uc.engines_from_a_library_by_path", [] {
    // the copy cb/core loaded, asked for by its path: the same module, so the same engines
    const auto path = emu::unicorn::library().path();
    load_library(path);
    auto uc = open_x86_from(path, 32);
    CB_CHECK_EQ(run(*uc, {0xB8, 0x07, 0x00, 0x00, 0x00}, "EAX"), 7u);
    CB_CHECK_EQ(describe_error_from(path, 6), describe_error(6));
    CB_CHECK_THROWS_AS(Error, load_library("a-folder-that-is-not-there/unicorn.dll"));
    CB_CHECK_THROWS_AS(UsageError, open_x86_from(path, 24));
});

CB_TEST("emulate.unicorn_uc.float_register_is_the_first_word", [] {
    auto uc = open_x86(32);
    uc->reg_write(reg("EAX"), 12);
    CB_CHECK_EQ(uc->reg_read_float(reg("EAX")), 12.0);
});
