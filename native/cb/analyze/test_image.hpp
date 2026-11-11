// A small hand-assembled 32-bit program for the analysis tests (included by the *_test.cpp files only).
//
// The program is ten tiny functions in a code span at 0x401000 and a data span at 0x402000. Between them they have
// what each analysis must read: a direct call, a recursive call and a tail jump, a recognised jump table, a call
// through an import and one through a register, a string handed to a function, a string built on the stack, a
// printf-like import called with a format string, and a function of the module's own handed formats from two
// places. The addresses are in `Sample`; the machine code is written with the little assembler below.
#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>

#include "cb/analyze/native.hpp"
#include "cb/core/bytes.hpp"

namespace cb::analyze::test {

// Just enough x86 to write the sample's functions.
class Asm {
public:
    explicit Asm(Va base) : base_(base) {}
    Va here() const { return base_ + static_cast<Va>(bytes_.size()); }
    Asm& raw(std::initializer_list<int> b) {
        for (int x : b) bytes_.push_back(static_cast<std::uint8_t>(x));
        return *this;
    }
    Asm& dword(std::uint32_t v) { return raw({static_cast<int>(v & 0xFF), static_cast<int>((v >> 8) & 0xFF), static_cast<int>((v >> 16) & 0xFF), static_cast<int>(v >> 24)}); }
    Asm& call(Va target) { raw({0xE8}); return dword(static_cast<std::uint32_t>(target - (here() + 4))); }
    Asm& jmp(Va target) { raw({0xE9}); return dword(static_cast<std::uint32_t>(target - (here() + 4))); }
    Asm& push_imm(std::uint32_t v) { raw({0x68}); return dword(v); }
    Asm& call_indirect(Va slot) { raw({0xFF, 0x15}); return dword(static_cast<std::uint32_t>(slot)); }
    Asm& ret() { return raw({0xC3}); }
    Asm& pad_to(Va addr, int fill = 0xCC) {
        while (here() < addr) raw({fill});
        return *this;
    }
    const Bytes& bytes() const { return bytes_; }

private:
    Va base_;
    Bytes bytes_;
};

struct Sample {
    // the code
    static constexpr Va kCode = 0x401000, kData = 0x402000;
    Va A = 0x401000, B = 0x401020, C = 0x401030, D = 0x401060, F = 0x401070, G = 0x401080, P = 0x4010B0, H = 0x4010D0,
       I = 0x4010E0, J = 0x4010F0, kCodeEnd = 0x401100;
    // the data
    Va hello = 0x402000, unreferenced = 0x402040, format = 0x402060, format_x = 0x402070, format_y = 0x402078,
       wide_string = 0x402080, jump_table = 0x402100, pointer_table = 0x402200, exit_slot = 0x402300, wsprintf_slot = 0x402304,
       buffer = 0x402400, kDataEnd = 0x402500, kDataInit = 0x400;
    Va case_address[3] = {};

    Span code, data;

    Sample() {
        Asm a(kCode);
        // A: calls B with two pushed arguments, one of them the address of a string
        a.raw({0x55, 0x8B, 0xEC, 0x6A, 0x05}).push_imm(static_cast<std::uint32_t>(hello)).call(B);
        a.raw({0x83, 0xC4, 0x08, 0x5D}).ret().pad_to(B);
        // B: returns zero
        a.raw({0x33, 0xC0}).ret().pad_to(C);
        // C: switch (arg) { 0, 1, 2 } through a table, anything else to the default
        a.raw({0x8B, 0x44, 0x24, 0x04, 0x83, 0xF8, 0x02, 0x77, 0x19});   // mov eax,[esp+4]; cmp eax,2; ja +0x19
        a.raw({0xFF, 0x24, 0x85}).dword(static_cast<std::uint32_t>(jump_table));   // jmp [eax*4 + table]
        for (int i = 0; i < 3; ++i) {
            case_address[i] = a.here();
            a.raw({0xB8}).dword(static_cast<std::uint32_t>(i + 1)).ret();           // mov eax, i+1; ret
        }
        a.raw({0x33, 0xC0}).ret().pad_to(D);                                         // default: xor eax,eax; ret
        // D: recursion, then a tail jump to B
        a.raw({0x55}).call(D).raw({0x5D}).jmp(B).pad_to(F);
        // F: a call through an import's slot, then a call through a register
        a.call_indirect(exit_slot).raw({0xFF, 0xD0}).ret().pad_to(G);
        // G: builds "Hello World" on the stack with three dword stores
        a.raw({0xC7, 0x45, 0xF0, 'H', 'e', 'l', 'l'});     // mov dword [ebp-0x10], 'Hell'
        a.raw({0xC7, 0x45, 0xF4, 'o', ' ', 'W', 'o'});     // mov dword [ebp-0x0c], 'o Wo'
        a.raw({0xC7, 0x45, 0xF8, 'r', 'l', 'd', 0});       // mov dword [ebp-0x08], 'rld\0'
        a.ret().pad_to(P);
        // P: wsprintfA(buffer, "%s=%d", 7)
        a.raw({0x6A, 0x07}).push_imm(static_cast<std::uint32_t>(format)).push_imm(static_cast<std::uint32_t>(buffer));
        a.call_indirect(wsprintf_slot).ret().pad_to(H);
        // H: the module's own printf-like function (does nothing); I and J hand it format strings
        a.ret().pad_to(I);
        a.push_imm(static_cast<std::uint32_t>(format_x)).call(H).ret().pad_to(J);
        a.push_imm(static_cast<std::uint32_t>(format_y)).call(H).ret().pad_to(kCodeEnd);

        code.va = kCode;
        code.data = a.bytes();
        code.executable = true;
        code.initialised = static_cast<Va>(code.data.size());
        code.name = ".text";

        data.va = kData;
        data.data.assign(static_cast<std::size_t>(kDataEnd - kData), 0);
        data.initialised = kDataInit;
        data.name = ".data";
        put(hello, "Hello");
        put(unreferenced, "A string nothing refers to");
        put(format, "%s=%d");
        put(format_x, "x=%d");
        put(format_y, "y=%s");
        put_wide(wide_string, "Wide!");
        for (int i = 0; i < 3; ++i) put32(jump_table + 4 * i, static_cast<std::uint32_t>(case_address[i]));
        put32(pointer_table, static_cast<std::uint32_t>(B));
        put32(pointer_table + 4, static_cast<std::uint32_t>(F));
    }

    // The program as an Image's recipe; every function known.
    ImageSpec spec() const {
        ImageSpec s;
        s.kind = ImageKind::Pe;
        s.bits = 32;
        s.flat = true;
        s.spans = {code, data};
        const Va starts[] = {A, B, C, D, F, G, P, H, I, J};
        for (std::size_t i = 0; i < std::size(starts); ++i)
            s.functions.emplace_back(starts[i], i + 1 < std::size(starts) ? starts[i + 1] : kCodeEnd);
        s.imports = {{exit_slot, "KERNEL32!ExitProcess"}, {wsprintf_slot, "USER32!wsprintfA"}};
        s.roots = {A};
        return s;
    }

private:
    void put(Va va, const char* text) {
        for (std::size_t i = 0; text[i] != 0; ++i) data.data[static_cast<std::size_t>(va - kData) + i] = static_cast<std::uint8_t>(text[i]);
    }
    void put_wide(Va va, const char* text) {   // 16-bit characters, then a zero word
        std::size_t at = static_cast<std::size_t>(va - kData);
        for (std::size_t i = 0; text[i] != 0; ++i, at += 2) data.data[at] = static_cast<std::uint8_t>(text[i]);
    }
    void put32(Va va, std::uint32_t v) {
        for (int i = 0; i < 4; ++i) data.data[static_cast<std::size_t>(va - kData) + i] = static_cast<std::uint8_t>(v >> (8 * i));
    }
};

}  // namespace cb::analyze::test
