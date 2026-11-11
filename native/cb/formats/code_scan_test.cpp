#include "cb/formats/code_scan.hpp"

#include "cb/core/testing.hpp"

namespace cb::formats {
namespace {

// A toy CPU for the scan: one opcode byte, and for the flow ones a target byte after it.
//   00 nop   10 t call t   20 t jump t   30 t branch t   40 ret   50 halt   60 jump through a register
std::optional<DecodedInsn> toy(ByteView mem, std::uint32_t pc) {
    if (pc >= mem.size()) return std::nullopt;
    switch (mem[pc]) {
        case 0x00: return DecodedInsn{1, Flow::Next, std::nullopt};
        case 0x10: return DecodedInsn{2, Flow::Call, mem[pc + 1]};
        case 0x20: return DecodedInsn{2, Flow::Jump, mem[pc + 1]};
        case 0x30: return DecodedInsn{2, Flow::Branch, mem[pc + 1]};
        case 0x40: return DecodedInsn{1, Flow::Return, std::nullopt};
        case 0x50: return DecodedInsn{1, Flow::Halt, std::nullopt};
        case 0x60: return DecodedInsn{1, Flow::Indirect, std::nullopt};
        default: return std::nullopt;
    }
}

Bytes image(std::initializer_list<std::pair<std::uint32_t, Bytes>> parts, std::size_t size = 0x100) {
    Bytes mem(size, 0x99);   // 0x99 does not decode
    for (const auto& [at, bytes] : parts) std::copy(bytes.begin(), bytes.end(), mem.begin() + at);
    return mem;
}

using Starts = std::vector<std::uint32_t>;

CB_TEST("formats.code_scan.call_targets_become_starts", [] {
    // 00: call 10; ret.   10: call 20; ret.   20: ret.
    const Bytes mem = image({{0x00, {0x10, 0x10, 0x40}}, {0x10, {0x10, 0x20, 0x40}}, {0x20, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00, 0x10, 0x20})));
});

CB_TEST("formats.code_scan.branches_are_followed_but_are_not_starts", [] {
    // 00: branch 08; ret.   08: call 20; ret.   20: ret
    const Bytes mem = image({{0x00, {0x30, 0x08, 0x40}}, {0x08, {0x10, 0x20, 0x40}}, {0x20, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00, 0x20})));
});

CB_TEST("formats.code_scan.jump_ends_the_run_and_is_followed", [] {
    // 00: jump 10; call 40 (never reached)   10: call 20; halt   20: ret
    const Bytes mem = image({{0x00, {0x20, 0x10, 0x10, 0x40}}, {0x10, {0x10, 0x20, 0x50}}, {0x20, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00, 0x20})));
});

CB_TEST("formats.code_scan.falls_through_to_a_call_after_nops", [] {
    const Bytes mem = image({{0x00, {0x00, 0x00, 0x10, 0x30, 0x40}}, {0x30, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00, 0x30})));
});

CB_TEST("formats.code_scan.stops_at_what_does_not_decode", [] {
    const Bytes mem = image({{0x00, {0x00, 0x99, 0x10, 0x30}}, {0x30, {0x40}}});   // 0x99 stops the run before the call
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00})));
});

CB_TEST("formats.code_scan.targets_outside_the_code_space_are_ignored", [] {
    const Bytes mem = image({{0x00, {0x10, 0xF0, 0x10, 0x20, 0x40}}, {0x20, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x80})) == ((Starts{0x00, 0x20})));
});

CB_TEST("formats.code_scan.a_start_outside_the_space_is_kept_not_followed", [] {
    const Bytes mem = image({{0x90, {0x10, 0x20, 0x40}}, {0x20, {0x40}}});
    CB_CHECK((find_functions(mem, {0x90}, toy, {.begin = 0, .end = 0x80})) == ((Starts{0x90})));
});

CB_TEST("formats.code_scan.even_targets_only", [] {
    const Bytes mem = image({{0x00, {0x10, 0x21, 0x10, 0x30, 0x40}}, {0x21, {0x40}}, {0x30, {0x40}}});
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100, .even_targets_only = true})) == ((Starts{0x00, 0x30})));
});

CB_TEST("formats.code_scan.data_range_ends_a_run", [] {
    // The run reaches 0x10 where data begins: the call after it is never seen.
    const Bytes mem = image({{0x0E, {0x00, 0x00, 0x10, 0x40}}, {0x40, {0x40}}});
    CB_CHECK((find_functions(mem, {0x0E}, toy, {.begin = 0, .end = 0x100, .data_begin = 0x10, .data_end = 0x20})) == ((Starts{0x0E})));
});

CB_TEST("formats.code_scan.loops_terminate", [] {
    const Bytes mem = image({{0x00, {0x30, 0x00}}});   // branch to itself, forever
    CB_CHECK((find_functions(mem, {0x00}, toy, {.begin = 0, .end = 0x100})) == ((Starts{0x00})));
});

}  // namespace
}  // namespace cb::formats
