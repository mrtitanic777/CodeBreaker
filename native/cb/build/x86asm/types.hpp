// cb/build/x86asm/types.hpp -- the types the parts of x86asm share: tokens, expressions, operands, the parsed
// instruction, locations, and the records an output format receives.
//
// This is the C++ port of NASM's include/nasm.h (and the type parts of insns.h, tables.h, labels.h, srcfile.h). The
// names are NASM's wherever a name could be kept (an enumerator keeps its NASM spelling, a structure is the same
// structure in UpperCamelCase); the values are NASM's exactly, because the encoder's matching, the output formats'
// records and the tables generated from insns.dat all depend on them. Pointers into NASM's buffers become owning
// strings and vectors; C's NULL-terminated lists become vectors.
//
// The register enum and its tables are in registers.hpp, the operand flag word in opflags.hpp, the instruction flag
// set in iflags.hpp, the instruction-name tokens in opcodes.hpp (all generated from NASM's data files). The state of
// one assembly run is `Assembly` (assembly.hpp); nothing here is global.
#pragma once

#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/x86asm/iflags.hpp"
#include "cb/build/x86asm/opcodes.hpp"
#include "cb/build/x86asm/opflags.hpp"
#include "cb/build/x86asm/registers.hpp"

namespace cb::build::x86asm {

// ---- segments and limits ----

inline constexpr std::int32_t NO_SEG = -1;        // null segment value
inline constexpr std::int32_t SEG_ABS = 0x40000000;   // mask for far-absolute segments

inline constexpr int IDLEN_MAX = 4096;
inline constexpr int DECOLEN_MAX = 32;

// ---- source locations (srcfile.h) ----

// A place in the source: a file name and a 1-based line (0 = no line). The name is kept by the Assembly that made it
// (so it stays valid for the whole run); two locations are the same when file and line are.
struct SrcLocation {
    std::string_view filename;   // empty: no file
    std::int32_t lineno = 0;

    friend bool operator==(const SrcLocation& a, const SrcLocation& b) = default;
};

// ---- assembly output location and records for the output formats ----

// Description of an assembly output location (NASM's struct location): the offset in a segment, and whether it is known.
struct Location {
    std::int64_t offset = 0;
    std::int32_t segment = 0;
    bool known = false;
};

// Values for the `type` of an output record.
enum OutType : int {
    OUT_RAWDATA,    // plain bytes
    OUT_RESERVE,    // reserved bytes (RESB et al)
    OUT_ZERODATA,   // initialized data, but all zero
    OUT_ADDRESS,    // an address (symbol value)
    OUT_RELADDR,    // a relative address
    OUT_SEGMENT,    // a segment number

    // Used by the legacy backend interface only (nasm_ofmt_output in assemble.c); never otherwise.
    OUT_REL1ADR,
    OUT_REL2ADR,
    OUT_REL4ADR,
    OUT_REL8ADR
};

enum OutFlags : int {
    OUT_WRAP = 0,       // undefined signedness (wraps)
    OUT_SIGNED = 1,     // value is signed
    OUT_UNSIGNED = 2,   // value is unsigned
    OUT_SIGNMASK = 3,   // mask for signedness bits
    OUT_NOWARN = 4      // don't warn on overflow (already done?)
};

struct ItemTemplate;   // below

// The data sent down to an output format (NASM's struct out_data).
struct OutData {
    Location loc;                           // seg:offset being written to
    OutType type = OUT_RAWDATA;
    int flags = OUT_WRAP;                   // OutFlags
    int inslen = 0;                         // length of instruction
    int insoffs = 0;                        // offset inside instruction
    int bits = 0;                           // bits mode of compilation
    std::uint64_t size = 0;                 // size of output
    const ItemTemplate* itemp = nullptr;    // instruction template
    const void* data = nullptr;             // data for OUT_RAWDATA
    std::uint64_t toffset = 0;              // target address offset for relocation
    std::int32_t tsegment = 0;              // target segment for relocation
    std::int32_t twrt = 0;                  // relocation with respect to
    std::int64_t relbase = 0;               // relative base for OUT_RELADDR
    SrcLocation where;                      // source file and line
    const char* what = nullptr;             // additional description, e.g. "immediate"

    // Legacy output data fields; some of these differ in their definition from the corresponding modern fields.
    struct Legacy {
        const void* data = nullptr;
        OutType type = OUT_RAWDATA;
        std::uint64_t size = 0;
        std::int32_t tsegment = 0;   // legacy name "segment"
        std::int32_t twrt = 0;       // legacy name "wrt"
    } legacy;
};

// ---- tokens (what the scanner returns) ----

// Token types returned by the scanner, in addition to ordinary ASCII character values (a single-character operator
// is its own character), and zero for end-of-string. The values are NASM's.
enum class TokenType : int {
    // Special codes
    INVALID = -1,   // a placeholder value
    BLOCK = -2,     // used for storage management
    FREE = -3,      // free token marker, use to catch leaks
    EOS = 0,        // end of string

    // Single-character operators.
    WHITESPACE = ' ',   // preprocessor use
    BOOL_NOT = '!',
    AND = '&',
    OR = '|',
    XOR = '^',
    NOT = '~',
    MULT = '*',
    DIV = '/',
    MOD = '%',
    LPAR = '(',
    RPAR = ')',
    PLUS = '+',
    MINUS = '-',
    COMMA = ',',
    LBRACE = '{',
    RBRACE = '}',
    LBRACKET = '[',
    RBRACKET = ']',
    QMARK = '?',
    EQ = '=',   // = or ==
    GT = '>',
    LT = '<',
    COLON = ':',

    // Multi-character operators
    SHL = 256,   // << or <<<
    SHR,         // >>
    SAR,         // >>>
    SDIV,        // //
    SMOD,        // %%
    GE,          // >=
    LE,          // <=
    NE,          // <> (!= is same as <>)
    LEG,         // <=>
    DBL_AND,     // &&
    DBL_OR,      // ||
    DBL_XOR,     // ^^

    MAX_OPERATOR,

    NUM,      // numeric constant
    ERRNUM,   // malformed numeric constant
    STR,      // string constant
    ERRSTR,   // unterminated string constant
    ID,       // identifier
    FLOAT,    // floating-point constant
    HERE,     // $, not '$' because it is not an operator
    BASE,     // $$

    // Token values only used by the assembler
    START_ASM,

    SEG,          // SEG
    WRT,          // WRT
    TIMES,        // TIMES
    FLOATIZE,     // __?floatX?__
    STRFUNC,      // __utf16*__, __utf32*__
    IFUNC,        // __ilog2*__
    DECORATOR,    // decorators such as {...}
    MASM_PTR,     // __?masm_ptr?__ for the masm package
    MASM_FLAT,    // __?masm_flat?__ for the masm package
    OPMASK,       // translated token for opmask registers
    SIZE,         // BYTE, WORD, DWORD, QWORD, etc
    SPECIAL,      // REL, FAR, NEAR, STRICT, NOSPLIT, etc
    PREFIX,       // A32, O16, LOCK, REPNZ, TIMES, etc
    BRCCONST,     // braced constant expression
    REG,          // register name
    INSN,         // instruction name

    END_ASM,

    // Token values only used by the preprocessor
    START_PP = END_ASM,

    OTHER,            // % sequence without (current) meaning
    PREPROC_ID,       // preprocessor ID, e.g. %symbol
    MMACRO_PARAM,     // MMacro parameter, e.g. %1
    LOCAL_SYMBOL,     // local symbol, e.g. %%symbol
    LOCAL_MACRO,      // context-local macro, e.g. %$symbol
    ENVIRON,          // %!
    INTERNAL_STR,     // unquoted string that should remain so
    NAKED_STR,        // unquoted string that can be re-quoted
    PREPROC_Q,        // %?
    PREPROC_QQ,       // %??
    PREPROC_SQ,       // %*?
    PREPROC_SQQ,      // %*??
    PASTE,            // %+
    COND_COMMA,       // %,
    INDIRECT,         // %[...]
    XDEF_PARAM,       // used during %xdefine processing
    // smacro parameters starting here; an arbitrary number.
    SMAC_START_PARAMS,   // MUST BE LAST IN THE LIST

    MAX = INT_MAX   // keep the range wide
};

// A token type as its integer value (for comparing with characters and with register or prefix numbers).
constexpr int token_value(TokenType t) { return static_cast<int>(t); }

// Token flags.
enum TokenFlags : int {
    TFLAG_BRC = 1 << 0,       // valid only with braces. {1to8}, {rd-sae}, ...
    TFLAG_BRC_OPT = 1 << 1,   // may or may not have braces. opmasks {k1}
    TFLAG_BRC_ANY = TFLAG_BRC | TFLAG_BRC_OPT,
    TFLAG_BRDCAST = 1 << 2,   // broadcasting decorator
    TFLAG_WARN = 1 << 3,      // warning only, treat as ID
    TFLAG_DUP = 1 << 4,       // valid ID but also has context-specific use
    TFLAG_ORBIT = 1 << 5      // OR in the bit given in inttwo
};

// Must match the order of the formats in floats.
enum Floatize : int {
    FLOAT_8,
    FLOAT_16,
    FLOAT_B16,
    FLOAT_32,
    FLOAT_64,
    FLOAT_80M,
    FLOAT_80E,
    FLOAT_128L,
    FLOAT_128H,
    FLOAT_ERR   // invalid format, MUST BE LAST
};

// Must match the list in string_transform (strfunc).
enum StrFunc : int {
    STRFUNC_UTF16,
    STRFUNC_UTF16LE,
    STRFUNC_UTF16BE,
    STRFUNC_UTF32,
    STRFUNC_UTF32LE,
    STRFUNC_UTF32BE
};

enum IFunc : int {
    IFUNC_ILOG2E,
    IFUNC_ILOG2W,
    IFUNC_ILOG2F,
    IFUNC_ILOG2C
};

// What a scanner hands the expression evaluator: the token's value(s), its text, where it is in the input, its type.
// (NASM's struct tokenval; the scanner function itself is the scanner module's.)
struct TokenVal {
    std::int64_t integer = 0;       // t_integer
    std::int64_t inttwo = 0;        // t_inttwo
    std::string text;               // t_charptr: the identifier, string or number text
    std::string_view start;         // t_start/t_len: the token in the input buffer
    TokenType type = TokenType::INVALID;   // t_type
    int flag = 0;                   // t_flag (TokenFlags)
};

// ---- expressions ----

// One component of an expression, in the evaluator's vector form: each type denotes a kind of component (a register, a
// segment base, EXPR_UNKNOWN, ...) and the value the multiple of it present. The exception is EXPR_WRT, whose value is
// the segment the expression is relative to. A value of zero is insignificant. NASM ends the array with a record of
// type 0; here an Expr is a std::vector and has no such record (its size is the length).
struct ExprTerm {
    std::int32_t type = 0;    // a register, or EXPR_xxx
    std::int64_t value = 0;   // must be >= 32 bits
};
using Expr = std::vector<ExprTerm>;

// What an expression turned out to be (expr_classes).
enum ExprClass : int {
    EC_ZERO = 0,        // all-zero expression
    EC_CONST = 1,       // additive constant
    EC_SEGABS = 2,      // absolute segment reference
    EC_SIMPLE = EC_CONST | EC_SEGABS,
    EC_SELFREL = 4,     // self-relative expression
    EC_SEG = 8,         // has a segment base
    EC_WRT = 16,        // has WRT
    EC_RELOC = EC_SIMPLE | EC_SELFREL | EC_SEG | EC_WRT,
    EC_UNKNOWN = 32,    // has an "unknown" part
    EC_REGISTER = 64,   // has a register
    EC_REGEXPR = 128,   // has more than one register and/or reg*n
    EC_COMPLEX = 256    // even more complex, problematic
};

// The evaluator can also return hints about which of two registers used in an expression should be the base register.
struct EvalHints {
    std::int64_t base = 0;
    int type = 0;
};

// `critical` for the evaluator: non-zero if the expression may not contain forward references (1: "symbol not defined
// before use", 2: "symbol undefined"); with CRITICAL (bit 8) set, relational and low-precedence logical operators are
// also accepted.
inline constexpr int CRITICAL = 0x100;

// Special values for ExprTerm::type, after EXPR_REG_END.
inline constexpr int EXPR_UNKNOWN = EXPR_REG_END + 1;   // forward references
inline constexpr int EXPR_SIMPLE = EXPR_REG_END + 2;
inline constexpr int EXPR_WRT = EXPR_REG_END + 3;
inline constexpr int EXPR_RDSAE = EXPR_REG_END + 4;
inline constexpr int EXPR_SEGBASE = EXPR_REG_END + 5;

// ---- preprocessor modes ----

enum PreprocMode : int {
    PP_NORMAL,    // assembly
    PP_DEPS,      // dependencies only
    PP_PREPROC    // preprocessing only
};

enum PreprocOpt : int {
    PP_TRIVIAL = 1,   // only %line or # directives
    PP_NOLINE = 2,    // ignore %line and # directives
    PP_TASM = 4       // TASM compatibility hacks
};

// ---- prefixes, registers as tokens, sizes ----

// REX flags. REX_[BXRW] and REX_P match the locations of these bits in an actual REX prefix (REX_P being the invariant
// 0x40 bit). REX_[BXR]1 match the REX2 prefix's bits shifted into the second byte (<< 12); REX_[BXR]V are BXR << 16.
// REX_W64 is a separate bit to say REX.W is set if the operand size is 64; "nw" (0327) can override it.
inline constexpr std::uint32_t REX_MASK = 0x4f;     // actual REX prefix bits
inline constexpr std::uint32_t REX_B = 0x01;        // ModRM r/m extension
inline constexpr std::uint32_t REX_X = 0x02;        // SIB index extension
inline constexpr std::uint32_t REX_R = 0x04;        // ModRM reg extension
inline constexpr std::uint32_t REX_W = 0x08;        // 64-bit operand size
inline constexpr std::uint32_t REX_L = 0x20;        // use LOCK prefix instead of REX.R
inline constexpr std::uint32_t REX_P = 0x40;        // REX prefix present/required
inline constexpr std::uint32_t REX_H = 0x80;        // high register present, REX forbidden
inline constexpr std::uint32_t REX_V = 0x0100;      // instruction uses VEX/XOP instead of REX
inline constexpr std::uint32_t REX_NH = 0x0200;     // instruction which doesn't use high regs
inline constexpr std::uint32_t REX_EV = 0x0400;     // instruction uses EVEX instead of REX
inline constexpr std::uint32_t REX_2 = 0x0800;      // instruction requires REX2
inline constexpr std::uint32_t REX_B1 = 0x1000;     // REX2/EVEX B second bit
inline constexpr std::uint32_t REX_X1 = 0x2000;     // REX2/EVEX X second bit
inline constexpr std::uint32_t REX_R1 = 0x4000;     // REX2/EVEX R second bit
inline constexpr std::uint32_t REX_NW = 0x8000;     // REX.W not required for 64-bit operand mode

inline constexpr std::uint32_t REX_BV = 0x10000;    // B is not a GPR or CREG
inline constexpr std::uint32_t REX_XV = 0x20000;    // X is not a GPR or CREG
inline constexpr std::uint32_t REX_RV = 0x40000;    // R is not a GPR or CREG

inline constexpr std::uint32_t REX_BXR0 = 0x0007;
inline constexpr std::uint32_t REX_BXR1 = 0x7000;
inline constexpr std::uint32_t REX_BXR = 0x7007;
inline constexpr std::uint32_t REX_BXRV = 0x70000;

// All flags pertaining to B, X, and R.
inline constexpr std::uint32_t REX_rB = REX_B | REX_B1 | REX_BV | REX_H | REX_P;
inline constexpr std::uint32_t REX_rX = REX_X | REX_X1 | REX_XV | REX_H | REX_P;
inline constexpr std::uint32_t REX_rR = REX_R | REX_R1 | REX_RV | REX_H | REX_P;

// A bit field of a word: where it starts, how wide it is, the unshifted mask and the mask in place (mk_field).
struct Field {
    unsigned pos;
    unsigned width;
    std::uint32_t basemask;
    std::uint32_t mask;
};
constexpr Field mk_field(unsigned pos, unsigned width) {
    const std::uint32_t base = (std::uint32_t{1} << width) - 1;
    return {pos, width, base, base << pos};
}

// EVEX bit fields. Note that the P[] numbers in the SDM do not include the leading 0x62 byte, so 8 is added to the
// position. B4 is in bit X3 = P[5] if B is a vector; X4 is in bit V4 = P[19] if X is a vector.
inline constexpr Field EVEX_MM = mk_field(8 + 0, 3);     // P[2:0]: opcode map
inline constexpr Field EVEX_B4 = mk_field(8 + 3, 1);     // P[3]: bit 4 B register (B4*)
inline constexpr Field EVEX_R4 = mk_field(8 + 4, 1);     // P[4]: bit 4 R register (R4)
inline constexpr Field EVEX_B3 = mk_field(8 + 5, 1);     // P[5]: bit 3 B register (B3)
inline constexpr Field EVEX_X3 = mk_field(8 + 6, 1);     // P[5]: bit 3 X register (X3*)
inline constexpr Field EVEX_R3 = mk_field(8 + 7, 1);     // P[7]: bit 3 R register (R3)
inline constexpr Field EVEX_PP = mk_field(8 + 8, 2);     // P[9:8]: legacy prefix
inline constexpr Field EVEX_X4 = mk_field(8 + 10, 1);    // P[10]: bit 4 X register (X4)
inline constexpr Field EVEX_VVVV = mk_field(8 + 11, 4);  // P[14:11]: V register (V3-V0)
inline constexpr Field EVEX_W = mk_field(8 + 15, 1);     // P[15]: osize extension (REX.W)
inline constexpr Field EVEX_AAA = mk_field(8 + 16, 3);   // P[18:16]: embedded opmask
inline constexpr Field EVEX_NF = mk_field(8 + 18, 1);    // P[18]: no flags bit
inline constexpr Field EVEX_V4 = mk_field(8 + 19, 1);    // P[19]: bit 4 V register (V4*)
inline constexpr Field EVEX_SCC = mk_field(8 + 16, 4);   // P[19:16]: modified condition
inline constexpr Field EVEX_B = mk_field(8 + 20, 1);     // P[20]: broadcast / RC / SAE
inline constexpr Field EVEX_ND = mk_field(8 + 20, 1);    // P[20]: new destination
inline constexpr Field EVEX_ZU = mk_field(8 + 20, 1);    // P[20]: zero upper
inline constexpr Field EVEX_LL = mk_field(8 + 21, 2);    // P[22:21]: vector length
inline constexpr Field EVEX_RC = mk_field(8 + 21, 2);    // P[22:21]: rounding control
inline constexpr Field EVEX_Z = mk_field(8 + 23, 1);     // P[23]: zeroing/merging

// REX_V "classes" (prefixes which behave like VEX).
enum VexClass : int {
    RV_VEX = 0,    // C4/C5
    RV_XOP = 1,    // 8F
    RV_EVEX = 2    // 62
};

// Instruction prefixes. Because segment registers may be used as instruction prefixes, the enumerations for prefixes
// and register names do not overlap: the prefixes start where the registers end. (The names, in prefix_name, MUST match.)
enum Prefix : int {
    P_none = 0,
    P_any = 1,
    PREFIX_ENUM_START = REG_ENUM_LIMIT,
    P_A16 = PREFIX_ENUM_START,
    P_A32,
    P_A64,
    P_ASP,
    P_LOCK,
    P_O16,
    P_O32,
    P_O64,
    P_OSP,
    P_REP,
    P_REPE,
    P_REPNE,
    P_REPNZ,
    P_REPZ,
    P_WAIT_,   // (NASM: P_WAIT; <process.h> defines that as a macro)
    P_XACQUIRE,
    P_XRELEASE,
    P_BND,
    P_NOBND,
    P_REX,
    P_REX2,
    P_EVEX,
    P_VEX,
    P_VEX3,
    P_VEX2,
    P_NF,
    P_ZU,
    P_PT,
    P_PN,
    PREFIX_ENUM_LIMIT
};

// Special EA flags.
enum EaFlags : int {
    EAF_BYTEOFFS = 1,     // force offset part to byte size
    EAF_WORDOFFS = 2,     // force offset part to [d]word size
    EAF_TIMESTWO = 4,     // really do EAX*2 not EAX+EAX
    EAF_REL = 8,          // IP-relative addressing
    EAF_ABS = 16,         // non-IP-relative addressing
    EAF_MIB = 32,         // mib operand
    EAF_SIB = 64,         // SIB encoding obligatory
    EAF_NOTFSGS = 128,    // no fs: or gs:
    EAF_FS = 256,         // fs segment override present
    EAF_GS = 512          // gs segment override present
};

// Values for an operand's `hinttype`.
enum EvalHint : int {
    EAH_NOHINT = 0,     // no hint at all - our discretion
    EAH_MAKEBASE = 1,   // try to make given reg the base
    EAH_NOTBASE = 2,    // try _not_ to make reg the base
    EAH_SUMMED = 3      // base and index are summed into index
};

// An operand to an instruction.
struct Operand {
    OpFlags type = 0;            // type of operand
    OpFlags xsize = 0;           // size flags used in find_match()
    int basereg = R_zero;        // Reg (or any register/prefix token number)
    int indexreg = R_zero;       // address registers
    int scale = 0;               // index scale
    int hintbase = 0;
    EvalHint hinttype = EAH_NOHINT;   // hint as to real base register
    std::int32_t segment = 0;    // immediate segment, if needed
    std::int64_t offset = 0;     // any immediate number
    std::int32_t wrt = 0;        // segment base it's relative to
    int eaflags = 0;             // special EA flags
    int opflags = 0;             // see OPFLAG_* below
    int iflag = 0;               // requires a specific IF_* flag
    DecoFlags decoflags = 0;     // decorator flags such as {...}
    bool bcast = false;          // broadcast operand
    std::uint8_t disp_size = 0;  // 0 means default; 16; 32; 64
    std::uint8_t opidx = 0;      // operand index
};

// An operand's `opflags`.
inline constexpr int OPFLAG_FORWARD = 1;    // operand is a forward reference
inline constexpr int OPFLAG_EXTERN = 2;     // operand is an external reference
inline constexpr int OPFLAG_UNKNOWN = 4;    // operand is an unknown reference (always a forward reference also)
inline constexpr int OPFLAG_RELATIVE = 8;   // operand is self-relative, e.g. [foo - $] where foo is not in the current segment
inline constexpr int OPFLAG_SIMPLE = 16;    // operand is a simple expression

// Extended operand types.
enum ExtOpType : int {
    EOT_NOTHING = 0,
    EOT_EXTOP,            // subexpression
    EOT_DB_STRING,        // byte string
    EOT_DB_FLOAT,         // floating-point number (special byte string)
    EOT_DB_STRING_FREE,   // byte string which should be freed (kept for the numbering; there is nothing to free)
    EOT_DB_NUMBER,        // integer
    EOT_DB_RESERVE        // ?
};

// An extended operand (the items of a DB/DW/... or INCBIN line): NASM's linked list is a vector, and its union is the
// members the type says are meaningful.
struct ExtOp {
    ExtOpType type = EOT_NOTHING;
    std::string string;               // text or byte string (EOT_DB_STRING, EOT_DB_FLOAT)
    std::int64_t offset = 0;          // numeric value or address offset (EOT_DB_NUMBER)
    std::int32_t segment = 0;         // address segment
    std::int32_t wrt = 0;             // address wrt
    bool relative = false;            // self-relative expression
    std::vector<ExtOp> subexpr;       // the actual expressions (EOT_EXTOP)
    std::size_t dup = 0;              // duplicated?
    int elem = 0;                     // element size override, if any (bytes)
};

enum EaType : int {
    EA_INVALID,   // not a valid EA at all
    EA_SCALAR,    // scalar EA
    EA_XMMVSIB,   // XMM vector EA
    EA_YMMVSIB,   // YMM vector EA
    EA_ZMMVSIB    // ZMM vector EA
};

// Prefix positions: each type of prefix goes in a specific slot, and the prefixes are emitted in order by slot. REX
// prefixes are handled differently for the time being. (LOCK and REP used to be one slot before HLE.)
enum PrefixPos : int {
    PPS_WAIT = 0,   // WAIT (technically not a prefix!)
    PPS_SEG,        // segment override prefix
    PPS_ASIZE,      // address size prefix
    PPS_LOCK,       // LOCK prefix
    PPS_OSIZE,      // operand size prefix
    PPS_REP,        // REP/HLE prefix
    PPS_REX,        // REX/VEX type
    PPS_NF,         // do not set flags
    PPS_ZU,         // zero upper register
    MAXPREFIX       // total number of prefix slots
};

// Tuple types used when determining Disp8*N eligibility (the order matches insns.pl's %tuple_codes).
enum Ttypes : int {
    FV = 001,
    HV = 002,
    FVM = 003,
    T1S8 = 004,
    T1S16 = 005,
    T1S = 006,
    T1F32 = 007,
    T1F64 = 010,
    T2 = 011,
    T4 = 012,
    T8 = 013,
    HVM = 014,
    QVM = 015,
    OVM = 016,
    M128 = 017,
    DUP = 020
};

// EVEX.L'L: vector length on vector insns.
enum VectLens : int {
    VL128 = 0,
    VL256 = 1,
    VL512 = 2,
    VLMAX = 3
};

// A postprocessed effective address (EA).
struct EaData {
    EaType type = EA_INVALID;        // what kind of EA is this?
    std::uint32_t rex = 0;           // EA-derived REX flags
    bool sib_present = false;        // is a SIB byte necessary?
    std::uint8_t bytes = 0;          // # of bytes of offset needed
    std::uint8_t size = 0;           // lazy - this is sib+bytes+1
    std::uint8_t modrm = 0, sib = 0; // the bytes themselves
    bool rip = false;                // RIP-relative?
    std::int8_t disp8 = 0;           // 8-bit displacement, possibly compressed
    std::uint8_t disp8_shift = 0;    // shift for 8-bit displacement
    bool disp8_ok = false;           // 8-bit displacement is valid
};

// Flags to disable optimizations selectively (the -O levels).
enum Optimization : int {
    // Individual flags
    OPTIM_NO_Jcc_RELAX = 1,    // disable Jcc relaxation
    OPTIM_NO_JMP_RELAX = 2,    // disable JMP relaxation
    OPTIM_STRICT_INSTR = 4,    // disable alternate instructions
    OPTIM_STRICT_OPER = 8,     // disable operand relaxation
    OPTIM_DISABLE_FWREF = 16,  // disable forward reference relaxation
    OPTIM_STRICT_OSIZE = 32,   // disable osize adjustments

    // Everything
    OPTIM_ALL_ENABLED = 0,

    // Disable all jump relaxations
    OPTIM_DISABLE_JMP_MATCH = OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX,

    // Level 0: 0.98 behavior
    OPTIM_LEVEL_0 = OPTIM_STRICT_INSTR | OPTIM_STRICT_OSIZE | OPTIM_STRICT_OPER | OPTIM_NO_JMP_RELAX,

    // Level 1: 0.98.09 behavior
    OPTIM_LEVEL_1 = OPTIM_STRICT_INSTR | OPTIM_STRICT_OSIZE | OPTIM_NO_Jcc_RELAX | OPTIM_NO_JMP_RELAX | OPTIM_DISABLE_FWREF,

    // Default
    OPTIM_DEFAULT = OPTIM_ALL_ENABLED
};

// Used with byte values for prefixes when no single byte value applies.
enum PrefixErr : int {
    PFE_NULL = -1,    // no output
    PFE_MULTI = -2,   // multibyte output (VEX, EVEX, REX2)
    PFE_ERR = -3,     // invalid prefix use
    PFE_WHAT = -4     // not a valid prefix (internal error)
};

// One instruction template of the encoder's tables (NASM's struct itemplate). `code` points into the bytecode table.
struct ItemTemplate {
    Opcode opcode = I_none;                          // the token, passed from the parser
    int operands = 0;                                // number of operands
    std::array<OpFlags, MAX_OPERANDS> opd{};         // bit flags for operand types
    std::array<DecoFlags, MAX_OPERANDS> deco{};      // bit flags for operand decorators
    const std::uint8_t* code = nullptr;              // what it assembles to
    std::uint32_t iflag_idx = 0;                     // some flags referenced by index (into the insns_flags table)
    unsigned xdaline = 0;                            // line # in insns.xda (for debug)
};

// The instruction the parser hands the encoder (NASM's struct insn).
struct Insn {
    Opcode opcode = I_none;                             // the opcode - not just the string
    Location loc;                                       // assembly location
    std::array<int, MAXPREFIX> prefixes{};              // instruction prefixes, if any
    std::array<int, MAXPREFIX> need_pfx{};              // byte prefixes used as opcodes
    std::string label;                                  // the label defined ("" for none)
    std::vector<ExtOp> eops;                            // extended operands
    int eops_float = 0;                                 // true if DD and floating
    std::int32_t times = 0;                             // repeat count (TIMES prefix)
    bool rex_done = false;                              // REX prefix emitted?
    bool dummy = false;                                 // not a real instruction, no errors
    std::uint8_t bits = 0;                              // execution mode (16, 32, 64)
    std::uint8_t op_size = 0;                           // operand size
    std::uint8_t addr_size = 0;                         // address size
    std::uint8_t operands = 0;                          // how many operands? 0-7 unless eops
    std::uint8_t vexreg = 0;                            // register encoded in VEX.V
    std::uint8_t veximm = 0;                            // bit-inverted immediate encoded in VEX.V
    std::uint8_t vex_cm = 0;                            // class and M field for VEX prefix
    std::uint8_t vex_wlp = 0;                           // W, P and L information for VEX prefix
    std::uint32_t rex = 0;                              // REX prefix flags
    std::uint32_t evex = 0;                             // EVEX prefix under construction
    Ttypes evex_tuple = static_cast<Ttypes>(0);          // tuple type for compressed Disp8*N (0: unset)
    int evex_rm = 0;                                    // static rounding mode for AVX512 (EVEX)
    int opt = OPTIM_DEFAULT;                            // optimization flags to use (Optimization)
    EaData ea;                                          // effective address data
    const ItemTemplate* itemp = nullptr;                // instruction template
    int evex_brerop = -1;                               // index (into oprs) of the BR/ER/SAE operand, -1 for none
    std::array<Operand, MAX_OPERANDS> oprs{};           // the operands
    int itempindex = 0;                                 // for debug: instruction template index in insnsa.c
};

// ---- directives and pragmas ----

// What to return from a directive- or pragma-handling function. DIRR_OK and DIRR_ERROR are treated the same way (the
// backend produces its own error message); DIRR_BADPARAM prints a generic error message.
enum DirectiveResult : int {
    DIRR_UNKNOWN,    // directive not handled by backend
    DIRR_OK,         // directive processed
    DIRR_ERROR,      // directive processed unsuccessfully
    DIRR_BADPARAM    // print bad argument error message
};

// Semi-arbitrary limits to keep the assembler from going into a black hole on certain kinds of bugs; overridable by
// command-line options or %pragma. (Their names, defaults and maxima are in Assembly.)
enum NasmLimit : int {
    LIMIT_PASSES,
    LIMIT_STALLED,
    LIMIT_MACRO_LEVELS,
    LIMIT_MACRO_TOKENS,
    LIMIT_MMACROS,
    LIMIT_REP,
    LIMIT_EVAL,
    LIMIT_LINES,
    LIMIT_PARAMS,
    LIMIT_MAX   // must be at end
};

inline constexpr std::int64_t LIMIT_MAX_VAL = INT64_MAX >> 1;   // absolute maximum for any limit

enum GetLimitWhich : int {
    GET_LIMIT_CURRENT,
    GET_LIMIT_INIT,
    GET_LIMIT_DEFAULT,
    GET_LIMIT_MAX
};

// ---- output formats ----

// An output format's flags (ofmt::flags).
inline constexpr unsigned OFMT_TEXT = 1;        // text file format
inline constexpr unsigned OFMT_KEEP_ADDR = 2;   // keep addr; no conversion to data
inline constexpr unsigned OFMT_ZERODATA = 4;    // "native" OUT_ZERODATA support

// The type definition values for debugging. Low 3 bits: reserved; next 5 bits: type; next 24 bits: number of elements
// for arrays (0 for labels).
inline constexpr unsigned TY_UNKNOWN = 0x00;
inline constexpr unsigned TY_LABEL = 0x08;
inline constexpr unsigned TY_BYTE = 0x10;
inline constexpr unsigned TY_WORD = 0x18;
inline constexpr unsigned TY_DWORD = 0x20;
inline constexpr unsigned TY_FLOAT = 0x28;
inline constexpr unsigned TY_QWORD = 0x30;
inline constexpr unsigned TY_TBYTE = 0x38;
inline constexpr unsigned TY_OWORD = 0x40;
inline constexpr unsigned TY_YWORD = 0x48;
inline constexpr unsigned TY_ZWORD = 0x50;
inline constexpr unsigned TY_COMMON = 0xE0;
inline constexpr unsigned TY_SEG = 0xE8;
inline constexpr unsigned TY_EXTERN = 0xF0;
inline constexpr unsigned TY_EQU = 0xF8;

constexpr unsigned tym_type(unsigned x) { return x & 0xF8; }
constexpr unsigned tym_elements(unsigned x) { return (x & 0xFFFFFF00u) >> 8; }
constexpr unsigned tys_elements(unsigned x) { return x << 8; }

// Sizes corresponding to various tokens.
enum ByteSizes : int {
    SIZE_BYTE = 1,
    SIZE_WORD = 2,
    SIZE_DWORD = 4,
    SIZE_LONG = 4,
    SIZE_QWORD = 8,
    SIZE_TWORD = 10,
    SIZE_OWORD = 16,
    SIZE_YWORD = 32,
    SIZE_ZWORD = 64
};

// The size and special-operand keyword tokens (after the prefixes), and the decorator tokens (after those).
enum SpecialTokens : int {
    SIZE_ENUM_START = PREFIX_ENUM_LIMIT,
    S_BYTE = SIZE_ENUM_START,
    S_WORD,
    S_DWORD,
    S_LONG,
    S_QWORD,
    S_TWORD,
    S_OWORD,
    S_YWORD,
    S_ZWORD,
    SIZE_ENUM_LIMIT,

    SPECIAL_ENUM_START = SIZE_ENUM_LIMIT,
    S_ABS = SPECIAL_ENUM_START,
    S_FAR,
    S_NEAR,
    S_NOSPLIT,
    S_REL,
    S_SHORT,
    S_STRICT,
    S_TO,
    SPECIAL_ENUM_LIMIT
};

enum DecoratorTokens : int {
    DECORATOR_ENUM_START = SPECIAL_ENUM_LIMIT,
    BRC_1TO2 = DECORATOR_ENUM_START,
    BRC_1TO4,
    BRC_1TO8,
    BRC_1TO16,
    BRC_1TO32,
    BRC_RN,
    BRC_RD,
    BRC_RU,
    BRC_RZ,
    BRC_SAE,
    BRC_Z,
    DECORATOR_ENUM_LIMIT
};

// ---- passes and per-pass options ----

// The kinds of pass an assembly executes (the names, in pass_type_name, are NASM's _pass_types).
enum PassType : int {
    PASS_INIT,      // initialization, not doing anything yet
    PASS_PREPROC,   // preprocess-only mode (similar to PASS_FIRST)
    PASS_FIRST,     // the very first pass over the code
    PASS_OPT,       // optimization pass
    PASS_STAB,      // stabilization pass (original pass 1)
    PASS_FINAL      // code generation pass (original pass 2)
};

// Pass-wide state; reset on top of each pass (NASM's struct globalopt).
struct GlobalOpt {
    int bits = 0;               // 16, 32 or 64-bit mode
    int rel = 0;                // default to relative addressing? (EaFlags)
    int reldef = 0;             // default rel/abs explicitly defined? (EaFlags)
    bool bnd = false;           // default to using bnd prefix?
    bool dollarhex = false;     // $-prefixed hexadecimal numbers?
};

// ---- labels ----

// What kind of symbol a label is (labels.h).
enum LabelType : int {
    LBL_none = -1,   // no label
    LBL_LOCAL = 0,   // must be zero
    LBL_STATIC,
    LBL_GLOBAL,
    LBL_EXTERN,
    LBL_REQUIRED,    // like extern but emit even if unused
    LBL_COMMON,
    LBL_SPECIAL,     // magic symbols like ..start
    LBL_BACKEND      // backend-defined symbols like ..got
};

constexpr bool is_extern(LabelType type) { return type == LBL_EXTERN || type == LBL_REQUIRED; }

// ---- helpers over the tables (common.c, opflags.h, insns.h) ----

// True if `reg` is a valid register token.
constexpr bool is_register(int reg) { return reg >= EXPR_REG_START && reg < REG_ENUM_LIMIT; }

// True if `reg` is a register matching class `cls`.
constexpr bool is_reg_class(OpFlags cls, int reg) { return is_register(reg) && is_class(cls, kRegFlags[static_cast<std::size_t>(reg)]); }

inline bool is_sreg(int reg) { return is_reg_class(opf::REG_SREG, reg); }
inline bool is_fsgs(int reg) { return is_reg_class(opf::REG_FSGS, reg); }

// The name of a register token, or nullptr when it is not one (register_name).
constexpr const char* register_name(int token) { return is_register(token) ? kRegNames[static_cast<std::size_t>(token - EXPR_REG_START)] : nullptr; }

// The name of a prefix token (or of a register, which can be a prefix too), or nullptr (prefix_name).
constexpr const char* prefix_name(int token) {
    constexpr std::array<const char*, PREFIX_ENUM_LIMIT - PREFIX_ENUM_START> kPrefixNames = {
        "a16", "a32", "a64", "asp", "lock", "o16", "o32", "o64", "osp", "rep", "repe", "repne", "repnz", "repz", "wait",
        "xacquire", "xrelease", "bnd", "nobnd", "{rex}", "{rex2}", "{evex}", "{vex}", "{vex3}", "{vex2}", "{nf}", "{zu}",
        "{pt}", "{pn}"};
    if (const char* name = register_name(token)) return name;
    const unsigned prefix = static_cast<unsigned>(token - PREFIX_ENUM_START);
    return prefix < kPrefixNames.size() ? kPrefixNames[prefix] : nullptr;
}

// True for a valid hinting-NOP opcode, after 0F (is_hint_nop).
constexpr bool is_hint_nop(std::uint64_t op) {
    if (op >> 16) return false;
    if ((op >> 8) == 0x0f) op = static_cast<std::uint8_t>(op);
    else if (op >> 8) return false;
    return ((op & ~std::uint64_t{7}) == 0x18) || op == 0x0d;
}

// An instruction name: DB-type instruction (DB, DW, ...).
constexpr bool opcode_is_db(Opcode opcode) { return opcode >= I_DB && opcode < I_RESB; }
// RESB-type instruction (RESB, RESW, ...).
constexpr bool opcode_is_resb(Opcode opcode) { return opcode >= I_RESB && opcode < I_INCBIN; }

// Initialized data bytes length from opcode (-1 for no opcode, 0 for another instruction).
constexpr int db_bytes(Opcode opcode) {
    switch (opcode) {
    case I_DB: return 1;
    case I_DW: return 2;
    case I_DD: return 4;
    case I_DQ: return 8;
    case I_DT: return 10;
    case I_DO: return 16;
    case I_DY: return 32;
    case I_DZ: return 64;
    case I_none: return -1;
    default: return 0;
    }
}

// Uninitialized data bytes length from opcode (-1 for no opcode, 0 for another instruction).
constexpr int resb_bytes(Opcode opcode) {
    switch (opcode) {
    case I_RESB: return 1;
    case I_RESW: return 2;
    case I_RESD: return 4;
    case I_RESQ: return 8;
    case I_REST: return 10;
    case I_RESO: return 16;
    case I_RESY: return 32;
    case I_RESZ: return 64;
    case I_none: return -1;
    default: return 0;
    }
}

// The values for the DFV bits in the CCMPcc and CTESTcc instructions (x86const.h).
enum DfvMask : int {
    DFV_CF = 1,
    DFV_ZF = 2,
    DFV_SF = 4,
    DFV_OF = 8
};

}  // namespace cb::build::x86asm
