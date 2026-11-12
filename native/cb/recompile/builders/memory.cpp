#include "cb/recompile/builders/memory.hpp"

#include <array>
#include <cstddef>

#include "cb/recompile/builders/reading.hpp"

namespace cb::recompile::builders::memory {
namespace {

// Which register file operand 0 is in.
enum class File { General, Float, Vector };

// How the address is written.
enum class Shape {
    D,         // rD, disp(rA):    {i} = disp, {a} = rA, {j} = the address
    X,         // rD, rA, rB:      {a} = rA, {b} = rB, {j} = the address
    DUpdate,   // as D, rA is written back: the statements spell the address themselves
    XUpdate,   // as X, rA is written back
};

// One row serves every name in `names`; `params` gives each its own {p} {q} {r}, "p/q/r" per name.
struct Row {
    std::string_view names;
    std::string_view params;
    File file;
    Shape shape;
    std::string_view statements;   // '\n' between statements
};

// Loads and stores, then the vectors, in the order the builder groups them (li and lis are written in `emit`).
constexpr std::array kRows = std::to_array<Row>({
    // integer loads: rD.u64 = PPC_LOAD_Un(address)
    {"lbz lhz lwz ld", "U8 U16 U32 U64", File::General, Shape::D, "ctx.r{d}.u64 = PPC_LOAD_{p}({j});"},
    {"lbzx lhzx lwzx ldx", "U8 U16 U32 U64", File::General, Shape::X, "ctx.r{d}.u64 = PPC_LOAD_{p}({j});"},
    // with update: ea = address; rD = load(ea); rA = ea
    {"lbzu lhzu lwzu ldu", "U8 U16 U32 U64", File::General, Shape::DUpdate,
     "ea = {i} + ctx.r{a}.u32;\nctx.r{d}.u64 = PPC_LOAD_{p}(ea);\nctx.r{a}.u32 = ea;"},
    {"lbzux lhzux lwzux ldux", "U8 U16 U32 U64", File::General, Shape::XUpdate,
     "ea = ctx.r{a}.u32 + ctx.r{b}.u32;\nctx.r{d}.u64 = PPC_LOAD_{p}(ea);\nctx.r{a}.u32 = ea;"},
    // algebraic (sign-extending) loads
    {"lha lwa", "int16_t/U16 int32_t/U32", File::General, Shape::D, "ctx.r{d}.s64 = {p}(PPC_LOAD_{q}({j}));"},
    {"lhax lwax", "int16_t/U16 int32_t/U32", File::General, Shape::X, "ctx.r{d}.s64 = {p}(PPC_LOAD_{q}({j}));"},
    {"lhau", "int16_t/U16", File::General, Shape::DUpdate,
     "ea = {i} + ctx.r{a}.u32;\nctx.r{d}.s64 = {p}(PPC_LOAD_{q}(ea));\nctx.r{a}.u32 = ea;"},
    {"lhaux lwaux", "int16_t/U16 int32_t/U32", File::General, Shape::XUpdate,
     "ea = ctx.r{a}.u32 + ctx.r{b}.u32;\nctx.r{d}.s64 = {p}(PPC_LOAD_{q}(ea));\nctx.r{a}.u32 = ea;"},
    // byte-reversed loads
    {"lhbrx lwbrx", "16 32", File::General, Shape::X, "ctx.r{d}.u64 = __builtin_bswap{p}(PPC_LOAD_U{p}({j}));"},
    // load-and-reserve: ea = address; reserved = the guest's value; rD = bswap(reserved)
    {"lwarx ldarx", "uint32_t/u32/32 uint64_t/u64/64", File::General, Shape::X,
     "ea = {j};\nctx.reserved.{q} = *({p}*)PPC_RAW_ADDR(ea);\nctx.r{d}.u64 = __builtin_bswap{r}(ctx.reserved.{q});"},

    // floating-point loads
    {"lfd", "", File::Float, Shape::D, "ctx.fpscr.disableFlushMode();\nctx.f{d}.u64 = PPC_LOAD_U64({j});"},
    {"lfdx", "", File::Float, Shape::X, "ctx.fpscr.disableFlushMode();\nctx.f{d}.u64 = PPC_LOAD_U64({j});"},
    {"lfs", "", File::Float, Shape::D, "ctx.fpscr.disableFlushMode();\ntemp.u32 = PPC_LOAD_U32({j});\nctx.f{d}.f64 = double(temp.f32);"},
    {"lfsx", "", File::Float, Shape::X, "ctx.fpscr.disableFlushMode();\ntemp.u32 = PPC_LOAD_U32({j});\nctx.f{d}.f64 = double(temp.f32);"},
    {"lfdu", "", File::Float, Shape::DUpdate,
     "ctx.fpscr.disableFlushMode();\nea = {i} + ctx.r{a}.u32;\nctx.f{d}.u64 = PPC_LOAD_U64(ea);\nctx.r{a}.u32 = ea;"},
    {"lfdux", "", File::Float, Shape::XUpdate,
     "ctx.fpscr.disableFlushMode();\nea = ctx.r{a}.u32 + ctx.r{b}.u32;\nctx.f{d}.u64 = PPC_LOAD_U64(ea);\nctx.r{a}.u32 = ea;"},
    {"lfsu", "", File::Float, Shape::DUpdate,
     "ctx.fpscr.disableFlushMode();\nea = {i} + ctx.r{a}.u32;\ntemp.u32 = PPC_LOAD_U32(ea);\nctx.f{d}.f64 = double(temp.f32);\nctx.r{a}.u32 = ea;"},
    {"lfsux", "", File::Float, Shape::XUpdate,
     "ctx.fpscr.disableFlushMode();\nea = ctx.r{a}.u32 + ctx.r{b}.u32;\ntemp.u32 = PPC_LOAD_U32(ea);\nctx.f{d}.f64 = double(temp.f32);\nctx.r{a}.u32 = ea;"},

    // integer stores: PPC_STORE_Un(address, rS.field)
    {"stb sth stw std", "U8/u8 U16/u16 U32/u32 U64/u64", File::General, Shape::D, "PPC_STORE_{p}({j}, ctx.r{d}.{q});"},
    {"stbx sthx stwx stdx", "U8/u8 U16/u16 U32/u32 U64/u64", File::General, Shape::X, "PPC_STORE_{p}({j}, ctx.r{d}.{q});"},
    {"stbu sthu stwu stdu", "U8/u8 U16/u16 U32/u32 U64/u64", File::General, Shape::DUpdate,
     "ea = {i} + ctx.r{a}.u32;\nPPC_STORE_{p}(ea, ctx.r{d}.{q});\nctx.r{a}.u32 = ea;"},
    {"stbux sthux stwux stdux", "U8/u8 U16/u16 U32/u32 U64/u64", File::General, Shape::XUpdate,
     "ea = ctx.r{a}.u32 + ctx.r{b}.u32;\nPPC_STORE_{p}(ea, ctx.r{d}.{q});\nctx.r{a}.u32 = ea;"},
    // byte-reversed stores
    {"sthbrx stwbrx", "16/u16 32/u32", File::General, Shape::X, "PPC_STORE_U{p}({j}, __builtin_bswap{p}(ctx.r{d}.{q}));"},
    // store-conditional (both spellings): cr0 = {0, 0, whether the exchange held, so}
    {"stwcx. stwcx stdcx. stdcx", "uint32_t/s32/32 uint32_t/s32/32 uint64_t/s64/64 uint64_t/s64/64", File::General, Shape::X,
     "ea = {j};\nctx.cr0.lt = 0;\nctx.cr0.gt = 0;\n"
     "ctx.cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<{p}*>(PPC_RAW_ADDR(ea)), ctx.reserved.{q}, __builtin_bswap{r}(ctx.r{d}.{q}));\n"
     "ctx.cr0.so = ctx.xer.so;"},

    // floating-point stores
    {"stfd", "", File::Float, Shape::D, "ctx.fpscr.disableFlushMode();\nPPC_STORE_U64({j}, ctx.f{d}.u64);"},
    {"stfdx", "", File::Float, Shape::X, "ctx.fpscr.disableFlushMode();\nPPC_STORE_U64({j}, ctx.f{d}.u64);"},
    {"stfdux", "", File::Float, Shape::XUpdate,
     "ctx.fpscr.disableFlushMode();\nea = ctx.r{a}.u32 + ctx.r{b}.u32;\nPPC_STORE_U64(ea, ctx.f{d}.u64);\nctx.r{a}.u32 = ea;"},
    {"stfiwx", "", File::Float, Shape::X, "ctx.fpscr.disableFlushMode();\nPPC_STORE_U32({j}, ctx.f{d}.u32);"},
    {"stfs", "", File::Float, Shape::D, "ctx.fpscr.disableFlushMode();\ntemp.f32 = float(ctx.f{d}.f64);\nPPC_STORE_U32({j}, temp.u32);"},
    {"stfsx", "", File::Float, Shape::X, "ctx.fpscr.disableFlushMode();\ntemp.f32 = float(ctx.f{d}.f64);\nPPC_STORE_U32({j}, temp.u32);"},
    {"stfdu", "", File::Float, Shape::DUpdate,
     "ctx.fpscr.disableFlushMode();\nea = {i} + ctx.r{a}.u32;\nPPC_STORE_U64(ea, ctx.f{d}.u64);\nctx.r{a}.u32 = ea;"},
    {"stfsu", "", File::Float, Shape::DUpdate,
     "ctx.fpscr.disableFlushMode();\nea = {i} + ctx.r{a}.u32;\ntemp.f32 = float(ctx.f{d}.f64);\nPPC_STORE_U32(ea, temp.u32);\nctx.r{a}.u32 = ea;"},
    {"stfsux", "", File::Float, Shape::XUpdate,
     "ctx.fpscr.disableFlushMode();\ntemp.f32 = float(ctx.f{d}.f64);\nea = ctx.r{a}.u32 + ctx.r{b}.u32;\nPPC_STORE_U32(ea, temp.u32);\nctx.r{a}.u32 = ea;"},

    // vector loads (X form): the whole vector is reversed (VectorMaskL)
    {"lvx lvx128 lvxl128 lvebx lvehx lvewx lvewx128", "", File::Vector, Shape::X,
     "ea = ({j}) & ~0xF;\n"
     "simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(ea)), "
     "simde_mm_load_si128((simde__m128i*)VectorMaskL)));"},
    {"lvlx lvlx128", "", File::Vector, Shape::X,
     "temp.u32 = {j};\n"
     "simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), "
     "simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));"},
    {"lvrx lvrx128", "", File::Vector, Shape::X,
     "temp.u32 = {j};\n"
     "simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, temp.u32 & 0xF ? "
     "simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)PPC_RAW_ADDR(temp.u32 & ~0xF)), "
     "simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());"},
    {"lvsl", "", File::Vector, Shape::X,
     "temp.u32 = {j};\n"
     "simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, simde_mm_load_si128((simde__m128i*)&VectorShiftTableL[(temp.u32 & 0xF) * 16]));"},
    {"lvsr", "", File::Vector, Shape::X,
     "temp.u32 = {j};\n"
     "simde_mm_store_si128((simde__m128i*)ctx.v{d}.u8, simde_mm_load_si128((simde__m128i*)&VectorShiftTableR[(temp.u32 & 0xF) * 16]));"},

    // vector stores (X form): the element index is reversed in the statement
    {"stvehx", "", File::Vector, Shape::X, "ea = ({j}) & ~0x1;\nPPC_STORE_U16(ea, ctx.v{d}.u16[7 - ((ea & 0xF) >> 1)]);"},
    {"stvewx stvewx128", "", File::Vector, Shape::X, "ea = ({j}) & ~0x3;\nPPC_STORE_U32(ea, ctx.v{d}.u32[3 - ((ea & 0xF) >> 2)]);"},
    {"stvlx stvlx128 stvlxl128", "", File::Vector, Shape::X,
     "ea = {j};\nfor (size_t i = 0; i < (16 - (ea & 0xF)); i++)\n\tPPC_STORE_U8(ea + i, ctx.v{d}.u8[15 - i]);"},
    {"stvrx stvrx128", "", File::Vector, Shape::X,
     "ea = {j};\nfor (size_t i = 0; i < (ea & 0xF); i++)\n\tPPC_STORE_U8(ea - i - 1, ctx.v{d}.u8[i]);"},
    {"stvx stvx128", "", File::Vector, Shape::X,
     "ea = ({j}) & ~0xF;\n"
     "simde_mm_store_si128((simde__m128i*)PPC_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)ctx.v{d}.u8), "
     "simde_mm_load_si128((simde__m128i*)VectorMaskL)));"},
});

// The next space-separated word of `rest` (empty at the end).
std::string_view take_word(std::string_view& rest) {
    const std::size_t end = rest.find(' ');
    const std::string_view word = rest.substr(0, end);
    rest = end == std::string_view::npos ? std::string_view{} : rest.substr(end + 1);
    return word;
}

// "a/b/c" into p, q, r.
void split_params(std::string_view params, Slots& slots) {
    std::string* fields[] = {&slots.p, &slots.q, &slots.r};
    for (std::string* field : fields) {
        const std::size_t cut = params.find('/');
        *field = std::string(params.substr(0, cut));
        if (cut == std::string_view::npos) break;
        params.remove_prefix(cut + 1);
    }
}

Slots read_slots(const Row& row, std::string_view params, const Instruction& insn, const Operands& ops) {
    Slots s;
    switch (row.file) {
        case File::General: s.d = gpr_at(insn, ops, 0); break;
        case File::Float: s.d = fpr_at(insn, ops, 0); break;
        case File::Vector: s.d = vpr_at(insn, ops, 0); break;
    }
    if (row.shape == Shape::D || row.shape == Shape::DUpdate) {
        const MemoryOperand m = mem_at(insn, ops, 1);
        s.a = m.base;
        s.i = std::to_string(m.displacement);
        s.j = m.base != 0 ? "ctx.r" + std::to_string(m.base) + ".u32 + " + s.i : s.i;
    } else {
        s.a = gpr_at(insn, ops, 1);
        s.b = gpr_at(insn, ops, 2);
        s.j = (s.a != 0 ? "ctx.r" + std::to_string(s.a) + ".u32 + " : std::string()) + "ctx.r" + std::to_string(s.b) + ".u32";
    }
    split_params(params, s);
    return s;
}

// The mnemonics of every row, in order.
std::vector<std::string_view> all_mnemonics() {
    std::vector<std::string_view> out{"li", "lis"};
    for (const Row& row : kRows) {
        std::string_view rest = row.names;
        while (!rest.empty()) out.push_back(take_word(rest));
    }
    return out;
}

}  // namespace

std::optional<std::vector<std::string>> emit(const Instruction& insn, const Operands& operands) {
    // load immediate: not a memory operation
    if (insn.mnemonic == "li") {
        const int d = gpr_at(insn, operands, 0);
        return std::vector<std::string>{"ctx.r" + std::to_string(d) + ".s64 = " + std::to_string(as_i32(imm_at(insn, operands, 1))) + ";"};
    }
    if (insn.mnemonic == "lis") {
        const int d = gpr_at(insn, operands, 0);
        const std::int64_t shifted = (imm_at(insn, operands, 1) & 0xFFFFFFFF) << 16;
        return std::vector<std::string>{"ctx.r" + std::to_string(d) + ".s64 = " + std::to_string(as_i32(shifted)) + ";"};
    }
    for (const Row& row : kRows) {
        std::string_view names = row.names;
        std::string_view params = row.params;
        while (!names.empty()) {
            const std::string_view name = take_word(names);
            const std::string_view own = take_word(params);
            if (name == insn.mnemonic) return fill_lines(row.statements, read_slots(row, own, insn, operands));
        }
    }
    return std::nullopt;
}

std::string_view runtime_helpers() {
    return R"CBX(
// Provided by the runtime (native/rexglue/kernel; not redefined when it is included):
//   #include <simde/x86/sse4.1.h>   // simde_mm_shuffle_epi8, etc.
// extern const uint8_t VectorMaskL[];        // whole-vector byte-reverse mask
// extern const uint8_t VectorMaskR[];
// extern const uint8_t VectorShiftTableL[];  // lvsl table
// extern const uint8_t VectorShiftTableR[];  // lvsr table
// PPC_LOAD_U*/PPC_STORE_U*/PPC_RAW_ADDR come from the recompiler runtime header.
)CBX";
}

std::span<const std::string_view> mnemonics() {
    static const std::vector<std::string_view> names = all_mnemonics();
    return names;
}

}  // namespace cb::recompile::builders::memory
