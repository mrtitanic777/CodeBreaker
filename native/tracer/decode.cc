// decode.cc -- Capstone, asked what emulator.py asks it (see decode.h).
#include "decode.h"

#include <cctype>
#include <cstring>
#include <mutex>
#include <unordered_map>

namespace cbt {

const char *const COMPARES[9] = {"cmp", "test", "sub", "and", "or", "dec", "inc", "add", "xor"};
// instructions that leave the flags alone: a compare before them still decides the jump after
static const char *const FLAGLESS[] = {"mov", "movzx", "movsx", "lea", "push", "pop", "nop", "xchg",
                                       "not", "cdq", "cwde", "cbw", "bswap"};

// One detailed decoder per thread (platforms.decoder(32, detail=True) is kept per thread too).
static csh decoder() {
    thread_local csh h = 0;
    thread_local bool opened = false;
    if (!opened) {
        CS.open(CS_ARCH_X86, CS_MODE_32, &h);
        CS.option(h, CS_OPT_DETAIL, CS_OPT_ON);
        opened = true;
    }
    return h;
}

static std::string strip(const std::string &s) {
    const char *ws = " \t\n\r\x0b\x0c";
    size_t a = s.find_first_not_of(ws);
    if (a == std::string::npos)
        return std::string();
    size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

static std::string text_of(const cs_insn &ins) {
    return strip(std::string(ins.mnemonic) + " " + ins.op_str);
}

bool decode_one(const uint8_t *p, size_t n, uint64_t address, InsnInfo &out) {
    cs_insn *insn = nullptr;
    size_t got = CS.disasm(decoder(), p, n, address, 1, &insn);
    if (got == 0)
        return false;
    out.text = text_of(insn[0]);
    out.is_call = strcmp(insn[0].mnemonic, "call") == 0;
    out.has_target = false;
    out.target = 0;
    if (out.is_call && insn[0].detail) {
        const cs_x86 &x = insn[0].detail->x86;
        for (int i = 0; i < x.op_count; i++)
            if (x.operands[i].type == X86_OP_IMM) {
                out.has_target = true;
                out.target = uint64_t(x.operands[i].imm) & 0xFFFFFFFFull;
            }
    }
    CS.free(insn, got);
    return true;
}

void straight_run(const uint8_t *p, size_t n, uint64_t address, Straight &out) {
    out = Straight();
    cs_insn *insn = nullptr;
    size_t got = CS.disasm(decoder(), p, n, address, 0, &insn);
    size_t total = 0;
    for (size_t i = 0; i < got; i++) {
        out.addrs.push_back(insn[i].address);
        out.texts.push_back(text_of(insn[i]));
        total += insn[i].size;
        if (insn[i].size == 2 && insn[i].bytes[0] == 0 && insn[i].bytes[1] == 0)
            out.zero = true;
    }
    if (got)
        CS.free(insn, got);
    out.ok = total == n && got > 0;
}

static bool starts(const char *s, const char *prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

int64_t callee_pops(uc_engine *uc, uint64_t ret) {
    uint8_t after[3], before[64];
    if (UC.mem_read(uc, ret, after, 3) != UC_ERR_OK)
        return 0;
    if ((after[0] == 0x83 || after[0] == 0x81) && after[1] == 0xC4)   // add esp, imm: the caller's
        return 0;
    if (ret < 64 || UC.mem_read(uc, ret - 64, before, 64) != UC_ERR_OK)
        return 0;
    csh h = decoder();
    for (int start = 0; start < 60; start++) {
        cs_insn *insn = nullptr;
        size_t got = CS.disasm(h, before + start, 64 - start, ret - 64 + start, 0, &insn);
        if (!got)
            continue;
        const cs_insn &last = insn[got - 1];
        if (last.address + last.size != ret || strcmp(last.mnemonic, "call") != 0) {
            CS.free(insn, got);
            continue;
        }
        int64_t pushed = 0;
        for (size_t k = got - 1; k-- > 0;) {
            const char *mn = insn[k].mnemonic;
            if (strcmp(mn, "push") == 0) {
                pushed += 4;
                continue;
            }
            if (!strcmp(mn, "call") || !strcmp(mn, "ret") || !strcmp(mn, "pop") || !strcmp(mn, "leave") ||
                !strcmp(mn, "enter") || !strcmp(mn, "pushal") || !strcmp(mn, "popal") || mn[0] == 'j' ||
                starts(insn[k].op_str, "esp"))
                break;
        }
        CS.free(insn, got);
        return pushed <= 64 ? pushed : 0;
    }
    return 0;
}

// ---- register names -------------------------------------------------------------------------

static std::mutex names_lock;
static std::unordered_map<std::string, int> uc_ids;      // "EAX" -> UC_X86_REG_EAX
static std::unordered_map<std::string, int> interned;

void register_reg(const char *upper_name, int id) {
    std::lock_guard<std::mutex> g(names_lock);
    uc_ids[upper_name] = id;
}

int intern(const std::string &s) {
    std::lock_guard<std::mutex> g(names_lock);
    auto it = interned.find(s);
    if (it != interned.end())
        return it->second;
    int id = int(interned.size());
    interned.emplace(s, id);
    return id;
}

// _uc_reg: getattr(x86_const, "UC_X86_REG_" + name.upper(), None)
static int uc_reg_of(const char *name) {
    if (!name)
        return -1;
    std::string up(name);
    for (char &c : up)
        c = char(toupper((unsigned char)c));
    std::lock_guard<std::mutex> g(names_lock);
    auto it = uc_ids.find(up);
    return it == uc_ids.end() ? -1 : it->second;
}

// _PARENT: capstone register name -> the 32-bit register it is part of
static const char *parent_of(const char *name) {
    static const char *const table[][2] = {
        {"al", "eax"}, {"ah", "eax"}, {"ax", "eax"}, {"eax", "eax"}, {"bl", "ebx"}, {"bh", "ebx"},
        {"bx", "ebx"}, {"ebx", "ebx"}, {"cl", "ecx"}, {"ch", "ecx"}, {"cx", "ecx"}, {"ecx", "ecx"},
        {"dl", "edx"}, {"dh", "edx"}, {"dx", "edx"}, {"edx", "edx"}, {"si", "esi"}, {"esi", "esi"},
        {"di", "edi"}, {"edi", "edi"}, {"bp", "ebp"}, {"ebp", "ebp"}, {"sp", "esp"}, {"esp", "esp"}};
    if (!name)
        return nullptr;
    for (auto &row : table)
        if (!strcmp(row[0], name))
            return row[1];
    return nullptr;
}

static int parent_id(const char *name) {         // _PARENT.get(name, name)
    const char *p = parent_of(name);
    return intern(p ? p : (name ? name : "\x01None"));
}

// operand(op) of _decode_branchy: false for a kind it does not follow (None)
static bool operand(csh h, const cs_x86_op &op, Operand &o) {
    o = Operand();
    o.size = op.size;
    if (op.type == X86_OP_REG) {
        const char *name = CS.reg_name(h, op.reg);
        o.kind = OP_REG;
        o.uc_reg = uc_reg_of(name);
        o.parent = parent_id(name);
        return true;
    }
    if (op.type == X86_OP_IMM) {
        o.kind = OP_IMM;
        o.imm = op.imm;
        return true;
    }
    if (op.type == X86_OP_MEM && op.mem.segment == X86_REG_INVALID) {
        o.kind = OP_MEM;
        o.base = op.mem.base ? uc_reg_of(CS.reg_name(h, op.mem.base)) : -1;
        o.index = op.mem.index ? uc_reg_of(CS.reg_name(h, op.mem.index)) : -1;
        o.scale = op.mem.scale;
        o.disp = op.mem.disp;
        return true;
    }
    return false;
}

void decode_branchy(uc_engine *uc, uint64_t address, Branchy &out) {
    out = Branchy();                          // ("other", None, (), None): also what a failure gives
    uint8_t raw[16];
    if (UC.mem_read(uc, address, raw, 16) != UC_ERR_OK)
        return;
    csh h = decoder();
    cs_insn *insn = nullptr;
    size_t got = CS.disasm(h, raw, 16, address, 1, &insn);
    if (!got)
        return;
    const cs_insn &ins = insn[0];
    const cs_x86 &x = ins.detail->x86;
    const char *mn = ins.mnemonic;
    std::vector<int> written;
    {
        cs_regs rr, rw;
        uint8_t nr = 0, nw = 0;
        if (CS.regs_access(h, &ins, rr, &nr, rw, &nw) == CS_ERR_OK)
            for (int i = 0; i < nw; i++)
                written.push_back(parent_id(CS.reg_name(h, rw[i])));
    }
    bool has_load = false;
    int load_parent = -1, load_size = 0;
    Operand load_src;
    if ((!strcmp(mn, "mov") || !strcmp(mn, "movzx") || !strcmp(mn, "movsx")) && x.op_count == 2 &&
        x.operands[0].type == X86_OP_REG && x.operands[1].type == X86_OP_MEM) {
        Operand src;
        if (operand(h, x.operands[1], src)) {
            const char *p = parent_of(CS.reg_name(h, x.operands[0].reg));
            has_load = true;                   // (None as the register: kept, and never used)
            load_parent = p ? intern(p) : -1;
            load_src = src;
            load_size = x.operands[1].size;
        }
    }
    bool flagless = starts(mn, "cmov") || starts(mn, "set");
    for (const char *f : FLAGLESS)
        flagless = flagless || !strcmp(mn, f);
    int cmp = -1;
    for (int i = 0; i < 9; i++)
        if (!strcmp(mn, COMPARES[i]))
            cmp = i;
    if (flagless) {
        out.kind = B_KEEP;
        out.written = written;
        out.has_load = has_load;
        out.load_parent = load_parent;
        out.load_src = load_src;
        out.load_size = load_size;
    } else if (mn[0] == 'j' && strcmp(mn, "jmp") != 0 && x.op_count > 0 && x.operands[0].type == X86_OP_IMM) {
        out.kind = B_JCC;
        out.target = uint64_t(x.operands[0].imm) & 0xFFFFFFFFull;
        out.next = address + ins.size;
    } else if (cmp >= 0 && (x.op_count == 1 || x.op_count == 2)) {
        Operand ops[2];
        bool all = true;
        for (int i = 0; i < x.op_count; i++) {
            if (!operand(h, x.operands[i], ops[i]) || (ops[i].kind == OP_REG && ops[i].uc_reg < 0))
                all = false;
        }
        if (all) {
            int n = x.op_count;
            if (n == 1) {                      // dec/inc: compared with what it becomes
                ops[1] = Operand();
                ops[1].kind = OP_IMM;
                ops[1].imm = !strcmp(mn, "dec") ? 1 : -1;
                ops[1].size = ops[0].size;
                n = 2;
            }
            out.kind = B_CMP;
            out.mn = cmp;
            out.nops = n;
            out.ops[0] = ops[0];
            out.ops[1] = ops[1];
            out.cmp_size = ops[0].size;
            out.written = written;
        } else {
            out.kind = B_OTHER;
            out.written = written;
        }
    } else {
        out.kind = B_OTHER;
        out.written = written;
    }
    CS.free(insn, got);
}

}  // namespace cbt
