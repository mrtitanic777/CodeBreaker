// decode.h -- what the recorder asks Capstone: an instruction's text, a straight run's
// instructions, the pushes before a call, and what a branch-log step needs to know of an
// instruction.  Each is the C++ form of a helper in codebreaker/emulate/emulator.py, decoding
// with the same capstone.dll and the same options (32-bit, detail on), so the text is the same.
#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "cb/emulate/tracer/libs.hpp"

namespace cb::emulate::tracer {

// One instruction as the record keeps it: (text, is a call, direct target) -- hook_code's `info`.
struct InsnInfo {
    std::string text;
    bool is_call = false;
    bool has_target = false;
    uint64_t target = 0;
};

// The first instruction in p[0..n) at `address`; false when nothing decodes (Python: "?").
bool decode_one(const uint8_t *p, size_t n, uint64_t address, InsnInfo &out);

// _straight_run: the instructions of a block of the file's code; ok is false when they do not
// decode into exactly its bytes.
struct Straight {
    bool ok = false;
    bool zero = false;                       // one of them is 00 00
    std::vector<uint64_t> addrs;
    std::vector<std::string> texts;
};
void straight_run(const uint8_t *p, size_t n, uint64_t address, Straight &out);

// _callee_pops: bytes of arguments the call returning to `ret` pushed, when its caller does not
// take them off itself; 0 when the code does not say.
int64_t callee_pops(uc_engine *uc, uint64_t ret);

// _decode_branchy's answer for one instruction.
enum { OP_NONE = 0, OP_REG, OP_IMM, OP_MEM };
struct Operand {
    int kind = OP_NONE;
    int uc_reg = -1;                         // a register's Unicorn id (-1: None)
    int parent = -1;                         // the 32-bit register it is part of (interned name)
    int64_t imm = 0;
    int base = -1, index = -1;               // a memory operand's registers (Unicorn ids, -1: None)
    int64_t scale = 0, disp = 0;
    int size = 0;
};
enum { B_OTHER = 0, B_KEEP, B_JCC, B_CMP };
struct Branchy {
    int kind = B_OTHER;
    uint64_t target = 0, next = 0;           // B_JCC
    int mn = -1;                             // B_CMP: the place in COMPARES
    int nops = 0;
    Operand ops[2];
    int cmp_size = 0;
    std::vector<int> written;                // interned names of the registers it writes
    bool has_load = false;                   // a load from memory into a 32-bit register's part
    int load_parent = -1;
    Operand load_src;
    int load_size = 0;
};
void decode_branchy(uc_engine *uc, uint64_t address, Branchy &out);

extern const char *const COMPARES[9];

// register names: Capstone's name -> Unicorn's id (cbt_register), and interned names
void register_reg(const char *upper_name, int id);
int intern(const std::string &s);

}  // namespace cb::emulate::tracer
