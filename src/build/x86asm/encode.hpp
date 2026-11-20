// build/x86asm/encode.hpp -- the encoder: how long an instruction is, and its bytes (NASM's asm/assemble.c).
//
// The parser hands the encoder an `Insn` (types.hpp) whose operands already carry evaluated values, segments and flags;
// the encoder finds the instruction template that matches it (`find_match`, over the tables in insns.hpp), works out the
// effective address and the prefixes, and either reports the length (`insn_size`, the passes before the last) or sends
// the bytes down to the output format (`assemble`, the last pass). `process_insn` is NASM's entry point for a line
// (it handles TIMES). The byte-code interpreter is `calcsize` (the size, and the decisions that depend on it) and
// `gencode` (the bytes); NASM's x86/bytecode.txt says what each code means.
//
// What the encoder needs from the rest of the assembler it asks for through the interfaces of the modules around it:
// `OutputFormat` and `DebugFormat` (format.hpp: NASM's `ofmt` and `dfmt`), `ListingSink` (below: the listing file,
// `lfmt`, implemented by Listing in listing.hpp), and a handful of callbacks in `EncoderHooks` (define a label for EQU,
// read a file for INCBIN). The state of one assembly run (the pass, the CPU, the options, the errors) is the `Assembly`.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "build/x86asm/assembly.hpp"
#include "build/x86asm/format.hpp"
#include "build/x86asm/insns.hpp"
#include "build/x86asm/types.hpp"

namespace cb::build::x86asm {

// The most data an OUT_ZERODATA record carries unless the format says it can take any size (ZERO_BUF_SIZE).
inline constexpr std::uint64_t kZeroBufSize = 65536;

// What the listing generator is told (NASM's struct lfmt, the parts assemble.c uses). Every method does nothing unless
// overridden, so an assembly without a listing passes `nullptr` or a plain `ListingSink`.
enum class ListType : int { READ, MACRO, INFO, INCLUDE, INCBIN, TIMES };

class ListingSink {
public:
    virtual ~ListingSink() = default;
    virtual void output(const OutData&) {}                                      // lfmt->output
    virtual void line(ListType, std::int32_t /*lineno*/, std::string_view) {}   // lfmt->line
    virtual void uplevel(ListType, std::int64_t /*size*/) {}                    // lfmt->uplevel
    virtual void downlevel(ListType) {}                                         // lfmt->downlevel
    virtual void set_offset(std::int64_t) {}                                    // lfmt->set_offset
    virtual bool option(char) const { return false; }                           // list_option('X'), list_option('p')
};

// The callbacks to the modules the encoder does not own.
struct EncoderHooks {
    // define_label(label, segment, offset, normal): EQU defines its label through this.
    std::function<void(const std::string& label, std::int32_t segment, std::int64_t offset, bool normal)> define_label;
    // The contents of a file for INCBIN, nullopt when it cannot be read (nasm_open_read / nasm_file_size_by_path).
    std::function<std::optional<std::vector<std::uint8_t>>(const std::string& name)> read_file;
};

// The values NASM's `match_result` has: errors in increasing priority, then successes in decreasing fuzziness.
enum MatchResult : int {
    MERR_INVALOP,
    MERR_OPSIZEINVAL,
    MERR_OPSIZEMISSING,
    MERR_OPSIZEMISMATCH,
    MERR_MASKNOTHERE,
    MERR_BRNOTHERE,
    MERR_BRNUMMISMATCH,
    MERR_DECONOTHERE,
    MERR_BADZU,
    MERR_MEMZU,
    MERR_BADNF,
    MERR_REQNF,
    MERR_BADCPU,
    MERR_BADMODE,
    MERR_BADHLE,
    MERR_ENCMISMATCH,
    MERR_BADBND,
    MERR_BADREPNE,
    MERR_REGSETSIZE,
    MERR_REGSET,
    MERR_WRONGIMM,

    MOK_FIRST,   // first successful anything
    MOK_FUZZY8,
    MOK_FUZZY7,  // matching unconditionally OK
    MOK_FUZZY6,
    MOK_FUZZY5,
    MOK_FUZZY4,
    MOK_FUZZY3,
    MOK_FUZZY2,
    MOK_FUZZY1,
    MOK_GOOD
};

// The encoder of one assembly run. It keeps no state beyond the last source position given to the debug sink.
class Encoder {
public:
    // `listing` and `debug` may be null (nothing is told).
    Encoder(Assembly& assembly, OutputFormat& output, ListingSink* listing = nullptr, DebugFormat* debug = nullptr, EncoderHooks hooks = {})
        : as_(assembly), ofmt_(output), lfmt_(listing), dfmt_(debug), hooks_(std::move(hooks)) {}

    // The entry point for a line (process_insn): handles TIMES, and runs insn_size (not the final pass) or assemble
    // (the final pass) at the current location, moving the location on.
    void process_insn(Insn& ins);

    // The length of the instruction in bytes (insn_size); -1 if it does not match or is invalid. For a non-final pass.
    // EQU defines its label here; DB-type data and INCBIN give their size.
    std::int64_t insn_size(Insn& ins);

    // Generate the instruction's bytes (assemble) for the final pass: returns the number of bytes produced.
    std::int64_t assemble(Insn& ins);

    // Find the template that matches (find_match); sets ins.itemp and ins.itempindex; returns MOK_GOOD or an error class.
    // (insn_size and assemble call insn_early_setup first; a caller using this directly must do the same.)
    MatchResult find_match(Insn& ins);

    // Does the template match the instruction, and how well (matches).
    MatchResult matches(const ItemTemplate& itemp, const Insn& ins);

    // Pre-match setup: the address-size prefix and the initial operand size (insn_early_setup).
    void insn_early_setup(Insn& ins);

    // Report the error for a failed match (no_match_error).
    void no_match_error(MatchResult m, const Insn& ins);

    // The size of the instruction given the template found (calcsize); it also settles the rex bits, prefixes and EA.
    std::int64_t calcsize(Insn& ins);

private:
    struct OutState {   // the "last debug info" static of out()
        SrcLocation where;
        std::int32_t segment = 0;
        bool any = false;
    };

    // ---- the output path ----
    void nasm_ofmt_output(OutData& data);
    void out(OutData& data);
    void out_rawdata(OutData& data, const void* rawdata, std::size_t size);
    void out_rawbyte(OutData& data, std::uint8_t byte);
    void out_rawdword(OutData& data, std::uint32_t value);
    void out_reserve(OutData& data, std::uint64_t size);
    void out_farseg(OutData& data, const Operand& opx);
    void out_imm(OutData& data, const Operand& opx, int size, int sign);
    void out_reladdr(OutData& data, const Operand& opx, int size);
    void out_eops(OutData& data, const std::vector<ExtOp>& eops);

    // ---- warnings and sizes ----
    void warn_overflow(int size, const char* prefix, const char* suffix);
    void warn_overflow_const(std::int64_t data, int size);
    void warn_overflow_out(std::int64_t data, int size, int flags, const char* what);

    // ---- instruction-level helpers ----
    MatchResult jmp_match(const Insn& ins, const ItemTemplate& temp);
    std::int64_t merge_resb(Insn& ins, std::int64_t isize);
    std::int64_t len_extops(const std::vector<ExtOp>& eops);
    void define_equ(Insn& ins);
    void debug_set_db_type(const Insn& ins);
    void debug_set_type(const Insn& ins);
    void bad_hle_warn(const Insn& ins, std::uint8_t hleok);
    int ea_evex_flags(Insn& ins, const Operand& opy);
    std::int64_t calcsize_speculative(const Insn& ins, const ItemTemplate& itemp);
    void emit_rex(OutData& data, Insn& ins);
    int emit_prefixes(OutData* data, const Insn& ins);
    void gencode(OutData& data, Insn& ins);
    unsigned memory_mod(int eaflags, Insn& ins, std::int64_t o, bool known, bool zerook);
    int process_ea(Operand& input, int rfield, OpFlags rflags, Insn& ins, EaType expected, std::string& errmsg);
    void add_asp(Insn& ins);
    void set_initial_opsize(Insn& ins);
    const ItemTemplate* masm_regdir_pref(const TemplateList& list, const ItemTemplate* best, const Insn& ins, int& besti);
    const ItemTemplate* masm_accum_imm_pref(const TemplateList& list, const ItemTemplate* best, const Insn& ins, int& besti);
    void process_one_insn(Insn& ins);
    void process_times_insn(Insn& ins);
    void list_nonfinal_pass(std::int64_t start);
    bool assert_no_prefix(Insn& ins, int pos);

    Assembly& as_;
    OutputFormat& ofmt_;
    ListingSink* lfmt_;
    DebugFormat* dfmt_;
    EncoderHooks hooks_;
    OutState last_debug_;
};

}  // namespace cb::build::x86asm
