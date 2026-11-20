// cb/build/x86asm/tokhash.cpp -- the keyword table of the scanner; see tokhash.hpp.
//
// NASM generates a perfect hash of the keywords; here the same table is an unordered map built once, on first use, from the
// instruction names (opcodes.hpp), the register names (registers.hpp) and the rest of the keywords (tokens.dat, below).
#include "cb/build/x86asm/tokhash.hpp"

#include <string>
#include <unordered_map>

#include "cb/build/x86asm/nctype.hpp"

namespace cb::build::x86asm {

namespace {

// One keyword's data (struct tokendata).
struct TokHashData {
    TokenType type;
    std::int64_t inttwo;
    int flag;
    std::int64_t integer;
};

using TokHashTable = std::unordered_map<std::string, TokHashData>;

void tokhash_add(TokHashTable& t, const char* name, TokenType type, std::int64_t inttwo, int flag, std::int64_t integer) {
    t.emplace(name, TokHashData{type, inttwo, flag, integer});
}

TokHashTable tokhash_build() {
    TokHashTable t;
    t.reserve(3600);

    // Instructions: the opcode is the token's integer.
    for (int i = 0; i < static_cast<int>(kInsnNames.size()); ++i) {
        if (kInsnNames[static_cast<std::size_t>(i)][0]) tokhash_add(t, kInsnNames[static_cast<std::size_t>(i)], TokenType::INSN, 0, 0, i);
    }

    // Registers: the register number is the token's integer; the opmask registers k1-k7 may or may not be in braces.
    for (int r = EXPR_REG_START; r < REG_ENUM_LIMIT; ++r) {
        const char* name = kRegNames[static_cast<std::size_t>(r - EXPR_REG_START)];
        const bool opmask = name[0] == 'k' && name[1] >= '1' && name[1] <= '7' && !name[2];
        tokhash_add(t, name, TokenType::REG, 0, opmask ? TFLAG_BRC_OPT : 0, r);
    }

    // The rest of tokens.dat. "The ? operator is a keyword, because ? is a legitimate symbol character."
    tokhash_add(t, "?", TokenType::QMARK, 0, 0, 0);

    tokhash_add(t, "a16", TokenType::PREFIX, PPS_ASIZE, 0, P_A16);
    tokhash_add(t, "a32", TokenType::PREFIX, PPS_ASIZE, 0, P_A32);
    tokhash_add(t, "a64", TokenType::PREFIX, PPS_ASIZE, 0, P_A64);
    tokhash_add(t, "asp", TokenType::PREFIX, PPS_ASIZE, 0, P_ASP);
    tokhash_add(t, "lock", TokenType::PREFIX, PPS_LOCK, 0, P_LOCK);
    tokhash_add(t, "o16", TokenType::PREFIX, PPS_OSIZE, 0, P_O16);
    tokhash_add(t, "o32", TokenType::PREFIX, PPS_OSIZE, 0, P_O32);
    tokhash_add(t, "o64", TokenType::PREFIX, PPS_OSIZE, 0, P_O64);
    tokhash_add(t, "osp", TokenType::PREFIX, PPS_OSIZE, 0, P_OSP);
    tokhash_add(t, "rep", TokenType::PREFIX, PPS_REP, 0, P_REP);
    tokhash_add(t, "repe", TokenType::PREFIX, PPS_REP, 0, P_REPE);
    tokhash_add(t, "repne", TokenType::PREFIX, PPS_REP, 0, P_REPNE);
    tokhash_add(t, "repnz", TokenType::PREFIX, PPS_REP, 0, P_REPNZ);
    tokhash_add(t, "repz", TokenType::PREFIX, PPS_REP, 0, P_REPZ);
    tokhash_add(t, "xacquire", TokenType::PREFIX, PPS_REP, 0, P_XACQUIRE);
    tokhash_add(t, "xrelease", TokenType::PREFIX, PPS_REP, 0, P_XRELEASE);
    tokhash_add(t, "bnd", TokenType::PREFIX, PPS_REP, 0, P_BND);
    tokhash_add(t, "nobnd", TokenType::PREFIX, PPS_REP, 0, P_NOBND);
    tokhash_add(t, "wait", TokenType::PREFIX, PPS_WAIT, 0, P_WAIT_);
    tokhash_add(t, "rex", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_REX);
    tokhash_add(t, "rex2", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_REX2);
    tokhash_add(t, "evex", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_EVEX);
    tokhash_add(t, "vex", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_VEX);
    tokhash_add(t, "vex3", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_VEX3);
    tokhash_add(t, "vex2", TokenType::PREFIX, PPS_REX, TFLAG_BRC, P_VEX2);
    tokhash_add(t, "nf", TokenType::PREFIX, PPS_NF, TFLAG_BRC, P_NF);
    tokhash_add(t, "zu", TokenType::PREFIX, PPS_ZU, TFLAG_BRC, P_ZU);
    tokhash_add(t, "pt", TokenType::PREFIX, PPS_SEG, TFLAG_BRC, P_PT);
    tokhash_add(t, "pn", TokenType::PREFIX, PPS_SEG, TFLAG_BRC, P_PN);

    tokhash_add(t, "byte", TokenType::SIZE, SIZE_BYTE, 0, S_BYTE);
    tokhash_add(t, "word", TokenType::SIZE, SIZE_WORD, 0, S_WORD);
    tokhash_add(t, "dword", TokenType::SIZE, SIZE_DWORD, 0, S_DWORD);
    tokhash_add(t, "long", TokenType::SIZE, SIZE_LONG, 0, S_LONG);
    tokhash_add(t, "qword", TokenType::SIZE, SIZE_QWORD, 0, S_QWORD);
    tokhash_add(t, "tword", TokenType::SIZE, SIZE_TWORD, 0, S_TWORD);
    tokhash_add(t, "oword", TokenType::SIZE, SIZE_OWORD, 0, S_OWORD);
    tokhash_add(t, "yword", TokenType::SIZE, SIZE_YWORD, 0, S_YWORD);
    tokhash_add(t, "zword", TokenType::SIZE, SIZE_ZWORD, 0, S_ZWORD);

    tokhash_add(t, "abs", TokenType::SPECIAL, 0, 0, S_ABS);
    tokhash_add(t, "far", TokenType::SPECIAL, 0, 0, S_FAR);
    tokhash_add(t, "near", TokenType::SPECIAL, 0, 0, S_NEAR);
    tokhash_add(t, "nosplit", TokenType::SPECIAL, 0, 0, S_NOSPLIT);
    tokhash_add(t, "rel", TokenType::SPECIAL, 0, 0, S_REL);
    tokhash_add(t, "short", TokenType::SPECIAL, 0, 0, S_SHORT);
    tokhash_add(t, "strict", TokenType::SPECIAL, 0, 0, S_STRICT);
    tokhash_add(t, "to", TokenType::SPECIAL, 0, 0, S_TO);

    // PTR is a legitimate symbol, but has an optional warning; DUP is a legitimate symbol, but also has a context-specific use.
    tokhash_add(t, "ptr", TokenType::ID, 0, TFLAG_WARN, 0);
    tokhash_add(t, "dup", TokenType::ID, 0, TFLAG_DUP, 0);

    tokhash_add(t, "__?infinity?__", TokenType::FLOAT, 0, 0, 0);
    tokhash_add(t, "__?nan?__", TokenType::FLOAT, 0, 0, 0);
    tokhash_add(t, "__?qnan?__", TokenType::FLOAT, 0, 0, 0);
    tokhash_add(t, "__?snan?__", TokenType::FLOAT, 0, 0, 0);

    tokhash_add(t, "__?float8?__", TokenType::FLOATIZE, 0, 0, FLOAT_8);
    tokhash_add(t, "__?float16?__", TokenType::FLOATIZE, 0, 0, FLOAT_16);
    tokhash_add(t, "__?float32?__", TokenType::FLOATIZE, 0, 0, FLOAT_32);
    tokhash_add(t, "__?float64?__", TokenType::FLOATIZE, 0, 0, FLOAT_64);
    tokhash_add(t, "__?float80m?__", TokenType::FLOATIZE, 0, 0, FLOAT_80M);
    tokhash_add(t, "__?float80e?__", TokenType::FLOATIZE, 0, 0, FLOAT_80E);
    tokhash_add(t, "__?float128l?__", TokenType::FLOATIZE, 0, 0, FLOAT_128L);
    tokhash_add(t, "__?float128h?__", TokenType::FLOATIZE, 0, 0, FLOAT_128H);
    tokhash_add(t, "__?bfloat16?__", TokenType::FLOATIZE, 0, 0, FLOAT_B16);

    tokhash_add(t, "__?utf16?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF16);
    tokhash_add(t, "__?utf16le?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF16LE);
    tokhash_add(t, "__?utf16be?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF16BE);
    tokhash_add(t, "__?utf32?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF32);
    tokhash_add(t, "__?utf32le?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF32LE);
    tokhash_add(t, "__?utf32be?__", TokenType::STRFUNC, 0, 0, STRFUNC_UTF32BE);

    tokhash_add(t, "__?ilog2e?__", TokenType::IFUNC, 0, 0, IFUNC_ILOG2E);
    tokhash_add(t, "__?ilog2w?__", TokenType::IFUNC, 0, 0, IFUNC_ILOG2W);
    tokhash_add(t, "__?ilog2f?__", TokenType::IFUNC, 0, 0, IFUNC_ILOG2F);
    tokhash_add(t, "__?ilog2c?__", TokenType::IFUNC, 0, 0, IFUNC_ILOG2C);

    tokhash_add(t, "seg", TokenType::SEG, 0, 0, 0);
    tokhash_add(t, "wrt", TokenType::WRT, 0, 0, 0);
    tokhash_add(t, "times", TokenType::TIMES, 0, 0, 0);
    tokhash_add(t, "__?masm_ptr?__", TokenType::MASM_PTR, 0, 0, 0);
    tokhash_add(t, "__?masm_flat?__", TokenType::MASM_FLAT, 0, 0, 0);

    tokhash_add(t, "1to2", TokenType::DECORATOR, 0, TFLAG_BRC | TFLAG_BRDCAST, BRC_1TO2);
    tokhash_add(t, "1to4", TokenType::DECORATOR, 0, TFLAG_BRC | TFLAG_BRDCAST, BRC_1TO4);
    tokhash_add(t, "1to8", TokenType::DECORATOR, 0, TFLAG_BRC | TFLAG_BRDCAST, BRC_1TO8);
    tokhash_add(t, "1to16", TokenType::DECORATOR, 0, TFLAG_BRC | TFLAG_BRDCAST, BRC_1TO16);
    tokhash_add(t, "1to32", TokenType::DECORATOR, 0, TFLAG_BRC | TFLAG_BRDCAST, BRC_1TO32);
    tokhash_add(t, "rn-sae", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_RN);
    tokhash_add(t, "rd-sae", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_RD);
    tokhash_add(t, "ru-sae", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_RU);
    tokhash_add(t, "rz-sae", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_RZ);
    tokhash_add(t, "sae", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_SAE);
    tokhash_add(t, "z", TokenType::DECORATOR, 0, TFLAG_BRC, BRC_Z);

    // The AVX10 dfv= flags: {dfv=} itself, and the flags it ORs together (pf is the same bit as cf).
    tokhash_add(t, "dfv=", TokenType::BRCCONST, IF_DFV, TFLAG_BRC, 0);
    tokhash_add(t, "dfv=cf", TokenType::BRCCONST, IF_DFV, TFLAG_BRC | TFLAG_ORBIT, DFV_CF);
    tokhash_add(t, "dfv=zf", TokenType::BRCCONST, IF_DFV, TFLAG_BRC | TFLAG_ORBIT, DFV_ZF);
    tokhash_add(t, "dfv=sf", TokenType::BRCCONST, IF_DFV, TFLAG_BRC | TFLAG_ORBIT, DFV_SF);
    tokhash_add(t, "dfv=of", TokenType::BRCCONST, IF_DFV, TFLAG_BRC | TFLAG_ORBIT, DFV_OF);
    tokhash_add(t, "dfv=pf", TokenType::BRCCONST, IF_DFV, TFLAG_BRC | TFLAG_ORBIT, DFV_CF);

    // Multi-character operators (used by the preprocessor's scanner).
    tokhash_add(t, ">>", TokenType::SHR, 0, 0, 0);
    tokhash_add(t, ">>>", TokenType::SAR, 0, 0, 0);
    tokhash_add(t, "<<", TokenType::SHL, 0, 0, 0);
    tokhash_add(t, "<<<", TokenType::SHL, 0, 0, 0);
    tokhash_add(t, "//", TokenType::SDIV, 0, 0, 0);
    tokhash_add(t, "%%", TokenType::SMOD, 0, 0, 0);
    tokhash_add(t, "==", TokenType::EQ, 0, 0, 0);
    tokhash_add(t, "!=", TokenType::NE, 0, 0, 0);
    tokhash_add(t, "<>", TokenType::NE, 0, 0, 0);
    tokhash_add(t, "<=", TokenType::LE, 0, 0, 0);
    tokhash_add(t, ">=", TokenType::GE, 0, 0, 0);
    tokhash_add(t, "<=>", TokenType::LEG, 0, 0, 0);
    tokhash_add(t, "&&", TokenType::DBL_AND, 0, 0, 0);
    tokhash_add(t, "||", TokenType::DBL_OR, 0, 0, 0);
    tokhash_add(t, "^^", TokenType::DBL_XOR, 0, 0, 0);
    return t;
}

const TokHashTable& tokhash_table() {
    static const TokHashTable t = tokhash_build();   // built once, never changed
    return t;
}

}  // namespace

int token_hash(std::string_view token, TokenVal& tv) {
    if (token.size() <= MAX_KEYWORD) {
        std::string lower(token);
        for (char& c : lower) c = nasm_tolower(c);
        const TokHashTable& t = tokhash_table();
        const auto it = t.find(lower);
        if (it != t.end()) {
            tv.integer = it->second.integer;
            tv.inttwo = it->second.inttwo;
            tv.flag = it->second.flag;
            tv.type = it->second.type;
            return token_value(tv.type);
        }
    }
    tv.integer = 0;
    tv.inttwo = 0;
    tv.flag = 0;
    tv.type = TokenType::ID;
    return token_value(TokenType::ID);
}

}  // namespace cb::build::x86asm
