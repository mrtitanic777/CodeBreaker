// image.h -- a module as the analyses see it (was codebreaker/analyze/image.py's Image).
//
// One address space per module: spans of bytes, function starts and their limits, the addresses
// standing for imports, and where the file says the loader writes an address. Functions are
// decoded with Capstone (the very capstone.dll Python's package loaded, so the text is the same)
// by following their code from the start; discover() adds the starts the module gives away.
#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <capstone/capstone.h>

namespace cba {

struct CsLib {
    decltype(&cs_open) open = nullptr;
    decltype(&cs_option) option = nullptr;
    decltype(&cs_disasm) disasm = nullptr;
    decltype(&cs_free) free = nullptr;
    decltype(&cs_reg_name) reg_name = nullptr;
    decltype(&cs_regs_access) regs_access = nullptr;
    decltype(&cs_close) close = nullptr;
};
extern CsLib CS;
bool load_capstone(const wchar_t *dll, char *err, size_t errlen);

// an operand: kind 'r' (name, size), 'i' (value, size), 'm' (base, index, scale, disp, seg, size)
struct Op {
    char kind = 0;
    std::string reg, base, index, seg;   // empty: None
    int64_t value = 0;                   // the immediate, or the displacement
    int scale = 0;
    int size = 0;
};

enum Flow { F_NONE, F_CALL, F_JMP, F_JCC, F_RET };

struct Ins {
    int64_t addr = 0;
    int size = 0;
    std::string mn, text;
    std::vector<Op> ops;
    std::set<std::string> rd, wr;
    Flow flow = F_NONE;
    bool has_target = false;
    int64_t target = 0;
};

struct Table {
    int64_t site = 0, table = 0, low = 0;
    std::vector<int64_t> targets;
    std::string selector;
    bool has_default = false;
    int64_t def = 0;
    bool has_bytes = false;
    std::vector<int> bytes;
    bool bounded = false;
};

struct Function {
    int64_t start = 0, limit = 0, end = 0;
    std::vector<Ins> insns;
    std::vector<Table> tables;
};

struct Span {
    int64_t va = 0;
    std::string data;
    bool exec = false, writable = false;
    int64_t init = 0;
    std::string name;       // the section's name (".rsrc", ...), as the file gives it
};

struct Pointer {
    int kind = 0;           // 0 abs32, 1 far, 2 selector, 3 offset, 4 byte
    bool has_target = false;
    int64_t target = 0;
};

class Image {
public:
    char kind = 'p';        // 'p' pe, 'n' ne, 'l' le
    int bits = 32;
    bool flat = false;
    std::vector<Span> spans;
    std::vector<int64_t> functions;            // sorted; the same set as the names
    std::unordered_set<int64_t> names;
    std::unordered_map<int64_t, int64_t> limits;
    std::unordered_set<int64_t> imports;
    bool has_pointers = false;
    std::vector<std::pair<int64_t, Pointer>> pointer_list;   // in the file's order
    std::unordered_map<int64_t, Pointer> pointers;
    std::set<int64_t> discovered;
    std::vector<int64_t> order;                // the discovered starts as they were added
    int64_t data_segment = 0;                  // 16-bit: the automatic data segment's address
    std::vector<int64_t> roots;                // where execution can enter from outside
    std::unordered_map<int64_t, std::string> name_text;     // function start -> name (UTF-8)
    std::unordered_map<int64_t, std::string> import_text;   // import address -> "MODULE!name" (UTF-8)
    struct Call { int64_t site; const char *kind; bool has_dest; int64_t dest; };
    bool graph_built = false;
    std::unordered_map<int64_t, std::vector<Call>> calls;   // the call graph's calls, by function
    struct Ref { int64_t fn, ins, va; const char *how; };
    std::vector<Ref> refs;                     // the call graph's references, in its order
    // strings.table, kept for strings.uses and strings.formats: in the table's order
    struct Str { int64_t va; std::string text; char kind; bool referenced; };
    bool strings_built = false;
    std::vector<Str> strings;
    std::unordered_map<int64_t, size_t> string_at;
    std::string label(int64_t va) const;       // the function, the import, or the address in hex

    ~Image();
    bool open_decoder(std::string &err);
    const Span *span(int64_t va) const;
    std::string read(int64_t va, int64_t n) const;
    bool u16(int64_t va, int64_t &v) const;
    bool u32(int64_t va, int64_t &v) const;
    bool is_code(int64_t va) const;
    bool is_data(int64_t va) const;
    const Function &function(int64_t start);
    // up to n instructions read straight on from va (not a function: no branch is followed)
    Function linear(int64_t va, int n);
    void discover();
    const std::map<int64_t, std::unique_ptr<Function>> &decoded() const { return fn_; }

private:
    csh md_ = 0;
    std::vector<int64_t> starts_;
    std::map<int64_t, std::unique_ptr<Function>> fn_;
    std::unordered_map<unsigned, std::string> regs_;

    const std::string &reg(unsigned rid);
    Ins convert(const cs_insn &ci);
    std::unique_ptr<Function> decode(int64_t start);
    bool jump_table(const Function &fn, const Ins &ins, const std::map<int64_t, Ins> &seen, Table &out);
    std::set<int64_t> held_code_addresses();
    bool looks_like_code(int64_t va, int most = 48);
public:
    const Ins *covering(int64_t va);
    bool owner(int64_t va, int64_t &start) const;
    void finish();
};

}  // namespace cba
