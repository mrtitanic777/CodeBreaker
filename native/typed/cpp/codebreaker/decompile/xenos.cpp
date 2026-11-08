// codebreaker/decompile/xenos.cpp -- C++ port of codebreaker.decompile.xenos.

#include "codebreaker/decompile/xenos.hpp"

#include <array>
#include <cstdint>

namespace codebreaker::decompile::xenos {

const py::str DOC = R"DOC(xenos.py -- the Xbox 360 GPU's (Xenos) shader microcode, as instructions.

    python -m codebreaker.decompile.xenos <hex dword> ...     disassemble microcode words
    python -m codebreaker.decompile.xenos --sample            the shader the decode is checked on

    from codebreaker.decompile import xenos
    xenos.disassemble(dwords)       ["CF: alloc / exece", "muls r0.z, r0.yyxx", ...]

The console's second instruction set: what the SDK's shader compiler (fxc) makes of HLSL. An ALU
instruction is three words (96 bits) that issue a vector and a scalar operation together; control
flow (CF) instructions are 48 bits, two to three words; fetch instructions read vertices and
textures. The encodings are the ones docs/XBOX360_TOOLCHAIN.md's study recovered from the SDK's
microcode assembler.

What is checked (tests/test_xenos.py), against the shader compiler's own listing of the same
microcode (`fxc /T ps_3_0 /XuFormatRaw /Fh`): the ALU word's fields -- vector and scalar opcode,
destination, write masks, the export bit, saturate, each source's register, select, negate and
swizzle -- and the CF opcodes. What is not yet: the layout of a whole program (here the leading
CF words are one pair and every later group of three words is an ALU instruction -- true of the
simple shaders it is checked on; a real program interleaves them through CF exec ranges), the
fetch words (recognised, not decoded), and the swizzle's spelling (relative, so `r0.xy` reads
`r0.yyxx`: the same channels, written out in full).
)DOC";

const py::dict<int64_t, py::str> VECTOR = {
    {0x00, "add"}, {0x01, "mul"}, {0x02, "max"}, {0x03, "max"}, {0x04, "seq"}, {0x05, "sgt"}, {0x06, "sge"},
    {0x07, "sne"}, {0x08, "frc"}, {0x09, "trunc"}, {0x0a, "floor"}, {0x0b, "mad"}, {0x0c, "cndeq"},
    {0x0d, "cndge"}, {0x0e, "cndgt"}, {0x0f, "dp4"}, {0x10, "dp3"}, {0x11, "dp2add"}, {0x12, "cube"},
    {0x13, "max4"}, {0x14, "setp_eq_push"}, {0x15, "setp_ne_push"}, {0x16, "setp_gt_push"},
    {0x17, "setp_ge_push"}, {0x18, "kille"}, {0x19, "killgt"}, {0x1a, "killge"}, {0x1b, "killne"},
    {0x1c, "dst"}, {0x1d, "mova"}};
// how many sources each vector opcode reads
const py::list<int64_t> VECTOR_SOURCES = {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 3, 3, 3, 3, 2, 2, 3, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0};
const py::dict<int64_t, py::str> SCALAR = {
    {0x00, "adds"}, {0x01, "adds_prev"}, {0x02, "muls"}, {0x03, "muls_prev"}, {0x04, "muls_prev2"},
    {0x05, "maxs"}, {0x06, "mins"}, {0x07, "seqs"}, {0x08, "sgts"}, {0x09, "sges"}, {0x0a, "snes"},
    {0x0b, "frcs"}, {0x0c, "truncs"}, {0x0d, "floors"}, {0x0e, "exp"}, {0x0f, "logc"}, {0x10, "log"},
    {0x11, "rcpc"}, {0x12, "rcpf"}, {0x13, "rcp"}, {0x14, "rsqc"}, {0x15, "rsqf"}, {0x16, "rsq"},
    {0x17, "mova"}, {0x18, "movaf"}, {0x1a, "subs_prev"}, {0x1b, "setp_eq"}, {0x1c, "setp_ne"},
    {0x1d, "setp_gt"}, {0x1e, "setp_ge"}, {0x1f, "setp_inv"}, {0x20, "setp_pop"}, {0x21, "setp_clr"},
    {0x22, "setp_rst"}, {0x23, "kills_eq"}, {0x24, "kills_gt"}, {0x25, "kills_ge"}, {0x26, "kills_ne"},
    {0x27, "kills_one"}, {0x28, "sqrt"}, {0x2a, "mulsc0"}, {0x2b, "mulsc1"}, {0x2c, "addsc0"},
    {0x2d, "addsc1"}, {0x2e, "subsc0"}, {0x2f, "subsc1"}, {0x30, "sin"}, {0x31, "cos"}};
const py::dict<int64_t, py::str> CONTROL = {
    {0, "nop"}, {1, "exec"}, {2, "exece"}, {3, "cexec"}, {4, "cexece"}, {5, "cexecp"}, {6, "cexecpe"},
    {7, "loop"}, {8, "endloop"}, {9, "call"}, {0xa, "ret"}, {0xb, "jmp"}, {0xc, "alloc"}};
const py::str CHANNELS_ = "xyzw";

// a pixel shader as fxc built it (/XuFormatRaw /Fh), and fxc's own listing of it
const py::list<int64_t> SAMPLE = {0x00000000, 0x2001c400, 0x22000000, 0x08400000, 0x00000061, 0xe2000000,
                                  0xc80f8000, 0x00404000, 0xe2000000, 0, 0, 0};
const py::list<py::str> SAMPLE_LISTING = {"alloc colors", "exece", "muls r0.z, r0.xy", "mov oC0, r0.xyzx"};

// A write mask (x=1, y=2, z=4, w=8) as its channels.
py::str _mask(int64_t m) {
    py::list<py::str> items;
    for (int64_t i = 0; i < 4; ++i) {
        if ((m & (int64_t(1) << i)) != 0) {
            items.append(CHANNELS_[i]);
        }
    }
    return py::join(py::str(""), items);
}

// Two bits per channel, relative to the channel itself (0 = the same channel).
py::str _swizzle(int64_t sw) {
    py::list<py::str> items;
    for (int64_t i = 0; i < 4; ++i) {
        items.append(CHANNELS_[(i + ((sw >> (2 * i)) & 3)) & 3]);
    }
    return py::join(py::str(""), items);
}

// A source: a temporary register (rN) when selected, else a constant (cN); bit 7 takes its
// absolute value, bit 6 indexes it by the loop counter.
py::str _register(int64_t selected, int64_t number) {
    py::str name = selected != 0 ? py::percent("r%d", number & 0x3f) : py::percent("c%d", number);
    return name + ((number & 0x80) != 0 ? "_abs" : "") + ((number & 0x40) != 0 ? "[aL]" : "");
}

// The instructions one 96-bit ALU word issues: [vector, scalar], each when its slot is used.
py::list<py::str> alu(int64_t d0, int64_t d1, int64_t d2) {
    int64_t vop = (d2 >> 24) & 0x1f, sop = (d0 >> 26) & 0x3f;
    int64_t vdst = d0 & 0x3f, sdst = (d0 >> 8) & 0x3f;
    int64_t vmask = (d0 >> 16) & 0xf, smask = (d0 >> 20) & 0xf;
    int64_t export_ = (d0 >> 15) & 1, vsat = (d0 >> 24) & 1, ssat = (d0 >> 25) & 1;
    // each source: (register number, swizzle, select, negate)
    std::array<std::tuple<int64_t, int64_t, int64_t, int64_t>, 3> sources;
    const int64_t fields[3][4] = {{16, 16, 31, 26}, {8, 8, 30, 25}, {0, 0, 29, 24}};
    for (int k = 0; k < 3; ++k) {
        sources[k] = {(d2 >> fields[k][0]) & 0xff, (d1 >> fields[k][1]) & 0xff, (d2 >> fields[k][2]) & 1,
                      (d1 >> fields[k][3]) & 1};
    }

    auto source = [&](int64_t i) -> py::str {
        const auto& [number, sw, sel, neg] = sources[static_cast<size_t>(i)];
        return py::str(neg != 0 ? "-" : "") + _register(sel, number) + "." + _swizzle(sw);
    };
    py::list<py::str> out;
    if (export_ != 0 || vmask != 0) {
        int64_t count = vop < py::len(VECTOR_SOURCES) ? VECTOR_SOURCES[vop] : 3;
        py::str dst = export_ != 0 ? py::str("oC0") : py::percent("r%d.%s", vdst, _mask(vmask));
        // max of a source with itself is how the compiler writes a move, and how it lists it
        if ((vop == 2 || vop == 3) && count >= 2 && source(0) == source(1)) {
            out.append(py::percent("mov%s %s, %s", vsat != 0 ? "_sat" : "", dst, source(0)));
        } else {
            py::list<py::str> srcs;
            for (int64_t i = 0; i < count; ++i) {
                srcs.append(source(i));
            }
            out.append(py::percent("%s%s %s, %s", VECTOR.get(vop, py::percent("vop%02x", vop)),
                                   vsat != 0 ? "_sat" : "", dst, py::join(py::str(", "), srcs)));
        }
    }
    if (smask != 0) {   // the scalar operation reads source C
        out.append(py::percent("%s%s r%d.%s, %s", SCALAR.get(sop, py::percent("sop%02x", sop)),
                               ssat != 0 ? "_sat" : "", sdst, _mask(smask), source(2)));
    }
    return out;
}

// The two 48-bit CF instructions in three words: their opcodes.
py::list<py::str> control(int64_t d0, int64_t d1, int64_t d2) {
    // v = d0 | d1 << 32 | d2 << 64 is 96 bits: its two 48-bit halves, built from the 32-bit words
    uint64_t u0 = static_cast<uint64_t>(d0) & 0xffffffffull, u1 = static_cast<uint64_t>(d1) & 0xffffffffull,
             u2 = static_cast<uint64_t>(d2) & 0xffffffffull;
    int64_t halves[2] = {static_cast<int64_t>(u0 | ((u1 & 0xffff) << 32)),
                         static_cast<int64_t>((u1 >> 16) | (u2 << 16))};
    py::list<py::str> out;
    for (int64_t half : halves) {
        int64_t op = (half >> 44) & 0xf;
        out.append(CONTROL.get(op, py::percent("cf%x", op)));
    }
    return out;
}

// Lines for a simple shader: one CF pair, then ALU words up to three zero words.
py::list<py::str> disassemble(const py::list<int64_t>& dwords) {
    py::list<py::str> out;
    if (py::len(dwords) >= 3) {
        out.append(py::str("CF: ") + py::join(py::str(" / "), control(dwords[0], dwords[1], dwords[2])));
    }
    for (int64_t i = 3; i < py::len(dwords) - 2; i += 3) {
        if (dwords[i] == 0 && dwords[i + 1] == 0 && dwords[i + 2] == 0) {
            break;
        }
        out.extend(alu(dwords[i], dwords[i + 1], dwords[i + 2]));
    }
    return out;
}

int64_t main(const py::list<py::str>& argv) {
    if (argv.empty() || argv[0] == "-h" || argv[0] == "--help") {
        py::print(py::split(DOC, py::str("\n\n"))[0]);
        return 0;
    }
    py::list<int64_t> words;
    if (argv[0] == "--sample") {
        words = SAMPLE;
    } else {
        for (const py::str& x : argv) {
            words.append(py::int_from_str(x, 16));
        }
    }
    for (const py::str& line : disassemble(words)) {
        py::print(line);
    }
    if (argv[0] == "--sample") {
        py::print(py::str("; the compiler's listing: ") + py::join(py::str(" | "), SAMPLE_LISTING));
    }
    return 0;
}

}  // namespace codebreaker::decompile::xenos
