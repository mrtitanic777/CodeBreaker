// codebreaker/compare/versions.hpp -- typed C++ of codebreaker.compare.versions (see versions.cpp).
//
// versions.py -- two builds of a module, function by function: what is the same, what changed, what is new, and what is
// already known about the new one because of the old one.
//
//     python -m codebreaker versions <module> --from CORPUS --to CORPUS    one module, two products
//     python -m codebreaker versions <fileA> <fileB>                       any two files
//     python -m codebreaker versions --all --from CORPUS --to CORPUS       every module of a product:
//                                   [--sample N] [--jobs N]                how much of it is known
//     python -m codebreaker versions --shared A B C ... [--level shape]    the functions several different programs
//                                                                          share: a source module linked into each
//
//     Comparison c = versions::compare(path_a, path_b);       // pairs, added, removed, counts(), report()
//     auto side = versions::load(path);                       // functions {key: Function}, as the store keys them
//
// HOW. A function is everything from its start (the starts the decompiler would use) to the return after which nothing of it
// is left. It is fingerprinted at three strictnesses (bytes: its machine code with only what the loader or linker fills in
// blanked; text: its instructions with every address replaced by what it stands for; shape: instruction kinds and operand
// kinds only). Matching goes from certain to likely -- the same bytes, the same exported name, the same text, the same shape,
// then the call graph, then likeness of the shapes, then position between two matched neighbours -- and each step only
// considers what is still unmatched. A pair is `identical` when bytes or text agree, else `changed` with a score; functions
// of the old build with no partner are `removed`, of the new one `added`.  (See versions.py's text for what is carried.)
//
// Typing notes:
//   * Function / Side / Comparison are the Python's classes; a pair of a Comparison is the struct `Pair`, a row of
//     `product()` is `ModuleRow` (its keys depend on the status: the unset ones are absent from to_value()).  The --json
//     document is an ordered tree (`formats::inspect::Value`, as the other ports'): as_json() / product's totals.
//   * capstone: `platforms.decoder(bits, detail=True)` is a Capstone handle per thread over the DLL given to
//     set_capstone_dll() (Python's capstone package carries it; pe_functions has its own, set it too).  An instruction is the
//     struct `Ins` (what capstone's CsInsn gives: mnemonic, op_str, operands, bytes, the encoding offsets).
//   * difflib.SequenceMatcher (the runtime has none) is written out in versions.cpp for the sequences this module compares
//     (each element interned as a number); hashlib.blake2b (digest_size=8) is too.
//   * The two fixes of the module's behaviour (see state/work/typed-port/_bugs/LIST.md, compare.versions): candidates are
//     looked up in a fixed order (sorted marks, then sorted run ids) and a run of shapes is a blake2b id, not hash().
//   * `clock()` is the Python's time.time() (a seam for a test).
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/inspect.hpp"
#include "py.hpp"

namespace codebreaker::compare::versions {

extern const py::list<py::str> LEVELS;      // ("bytes", "text", "shape")
extern const py::str ENGINE;                // "carried": who a candidate carried across builds came from

// The Capstone DLL (capstone.dll of Python's `capstone` package), loaded by path on first use.
void set_capstone_dll(const py::str& path);

std::function<double()>& clock();

// one instruction, as capstone's CsInsn gives it
struct Operand {
    int64_t type = 0;                       // X86_OP_REG 1, X86_OP_IMM 2, X86_OP_MEM 3
    int64_t reg = 0;
    int64_t imm = 0;
    int64_t segment = 0, base = 0, index = 0, scale = 0, disp = 0;   // a memory operand
};
struct Ins {
    int64_t address = 0, size = 0;
    py::str mnemonic, op_str;
    py::bytes bytes;
    std::vector<Operand> operands;
    int64_t imm_offset = 0, imm_size = 0, disp_offset = 0, disp_size = 0;
};
class Decoder {
  public:
    explicit Decoder(int64_t bits);
    std::vector<Ins> disasm(const py::bytes& code, int64_t address) const;
    py::str reg_name(int64_t reg) const;

  private:
    int64_t bits_;
};
// platforms.decoder(bits, detail=True)
std::shared_ptr<const Decoder> decoder(int64_t bits);

// ("f", key) another function of the module, ("i", name) an imported one
struct Call {
    py::str kind, key;
    bool operator==(const Call& o) const { return kind == o.kind && key == o.key; }
};

struct Ids {
    py::str bytes, text, shape;
    const py::str& at(const py::str& level) const;      // ids[level]
};

// One function of one build.
struct Function {
    py::str key, name;
    bool named = false;
    int64_t at = 0, size = 0;
    std::vector<std::tuple<int64_t, py::str>> rows;     // (address, "mnemonic operands")
    std::vector<py::str> text, shape;
    Ids ids;
    std::vector<Call> calls;
    std::set<std::u32string> marks;                     // (sorted: str order)
    int64_t jumps = 0;
    int64_t count() const { return static_cast<int64_t>(shape.size()); }
};

// One build of a module.
class Side {
  public:
    Side(const py::str& path, const py::str& kind, int64_t code = 0);
    py::str path, kind, name;
    int64_t code = 0;
    py::dict<py::str, std::shared_ptr<Function>> functions;
    py::list<py::str> order;                            // keys by address
    std::map<std::u32string, std::set<std::u32string>> callers;   // {key: {keys of the functions that call it}}
    void add(const std::shared_ptr<Function>& f);
    std::shared_ptr<Side> done_();                      // (Python: done())
    int64_t size() const;
};

// Read a module and fingerprint its functions.  ValueError for a file whose kind is not compared.
std::shared_ptr<Side> load(const py::str& path);

// The functions several programs have in common: -> [(instructions, {program name: [keys]})]
using Where = std::vector<std::pair<py::str, py::list<py::str>>>;
std::vector<std::pair<int64_t, Where>> shared(const py::list<py::str>& paths, const py::str& level = py::str("text"), int64_t least = 8);

// 0..1: how alike two functions' shapes are (1 = the same sequence)
double likeness(const Function& fa, const Function& fb, std::map<std::pair<std::u32string, std::u32string>, double>* memo = nullptr);

// Pair the functions of two builds. -> {key in a: (key in b, how it was found)} (in the order they were found)
py::dict<py::str, std::tuple<py::str, py::str>> match(const Side& a, const Side& b, double least = 0.6);

// one matched pair
struct Pair {
    py::str from, to, from_name, to_name, status, level, how;
    double score = 0.0;
    int64_t size_from = 0, size_to = 0;
};

struct Counts {
    int64_t identical = 0, identical_bytes = 0, identical_text = 0, changed = 0, added = 0, removed = 0;
};
struct CodeBytes {
    int64_t to_bytes = 0, identical = 0, changed = 0, added = 0;
};
struct NameRow {
    py::str to, name, from;
    bool identical = false;
};
struct Reconstruction {
    py::str from, to, klass;
    std::optional<py::str> engine, name;
    py::str source;
};

using Store = std::shared_ptr<engine::native_engine::FunctionStore>;

// What compare() found.
class Comparison {
  public:
    Comparison(const std::shared_ptr<Side>& a, const std::shared_ptr<Side>& b, const py::dict<py::str, std::tuple<py::str, py::str>>& pair,
               double seconds);
    std::shared_ptr<Side> a, b;
    double seconds;
    std::vector<Pair> pairs;
    py::list<py::str> removed, added;

    Counts counts() const;
    // Bytes of the new build's code: all of it, and what is identical / changed / added.
    CodeBytes code() const;
    // Names the old build has for functions the new build has none for.
    std::vector<NameRow> names() const;
    // The old build's verified reconstructions of functions the new build has unchanged. (`fs` null: the default store is opened.)
    std::vector<Reconstruction> reconstructions(const Store& fs = nullptr) const;
    // Give the carried names to the new build's bodies in the store (identical pairs only). -> how many, or None when the store
    // has no bodies for the new build.
    std::optional<int64_t> carry_names(const Store& fs = nullptr) const;
    // File those reconstructions for the new build, to be judged there. -> how many
    int64_t carry(const Store& fs = nullptr) const;
    py::str report() const;
    // the --json document
    formats::inspect::Value as_json(bool listings = true, const Store& fs = nullptr) const;
};

// Match the functions of two builds of a module.
Comparison compare(const py::str& path_a, const py::str& path_b, double least = 0.6);

// ---- a whole product
struct ModuleRow {
    py::str name, status;                        // compared | same file | new module | failed
    std::optional<int64_t> functions, bytes, identical, identical_code, changed, changed_code, added, removed;
    std::optional<double> seconds;
    std::optional<py::str> error;
};
formats::inspect::Value to_value(const ModuleRow& r);

struct Product {
    py::str from, to;
    py::list<ModuleRow> modules;
    std::vector<std::pair<py::str, int64_t>> totals;     // the Counter, in the order its keys were first set
};
formats::inspect::Value to_value(const Product& p);

// How much of product `new_` is already known from product `old`: every 16- and 32-bit module of `new_` (or an evenly spread
// `sample` of them) against the same-named module of `old`.
Product product(const py::str& old, const py::str& new_, int64_t sample = 0, std::optional<int64_t> jobs = std::nullopt, double least = 0.6,
                const std::function<void(const py::str&)>& log = nullptr);

extern const char* DOC;

// python -m codebreaker versions ...
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::compare::versions
