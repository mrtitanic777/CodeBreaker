// codebreaker/source/image_writer.hpp -- typed C++ of codebreaker.source.image_writer.
//
// image_writer.py -- what the writers of a program image share: a cartridge, a tape block, a ROM, a Windows file.
//
//     code, targets, starts = w.walk(starts, decode, inside, fits, follows, stops)
//     functions = w.functions(starts, names, "sub_%04X")
//     labels = w.labels(targets, names, line_start, "L_%04X")
//     out = w.lines(["; head"], syntax, module)      the source as one L2 Module, built as it is written
//     w.emit(out, start, end, byte, address, code_at, labels, functions, text, lines)
//     source = w.text(out) + "\n"                    ... written by the assembly back end (backends/asm.py)
//     r = w.result(folder, stem + ".asm", source, functions, code, size)
//     w.assembled(r, build, original, built_path)
//
//     r = w.flat(original, write, differing, outdir, name, say)     a whole x86 file, as NASM writes it
//     w.flat_report(path, r)
//
// Every one of these programs comes back the same way. Code is what the program's own flow reaches
// from its entry points; every byte it reaches is written as an instruction, every other byte as
// data. Function starts keep their entry point's name or are `sub_<address>`, jump targets
// `L_<address>`, and operands naming the start of a line get that line's label. The source is then
// assembled and the result compared with the original byte for byte.
//
// The source is built as one L2 tree as it is written (translate/l2.Assembly: emit() makes each
// function a MachineFragment, a writer's part() each bank or segment), and the text is the assembly
// back end's (translate/backends/asm.py) -- as for every lane whose output is assembly, the NASM and
// MASM writers too.
//
// What differs from one machine to the next -- its processor, the assembler's spelling, how the
// image is laid out in memory, which hardware has names -- stays in its writer (source/nes_writer.py,
// sms_writer.py, gb_writer.py, genesis_writer.py, z80_writer.py).
//
// The x86 files written whole -- a 32-bit Windows program, a 16-bit one, a driver (source/pe_writer.py,
// ne_writer.py, le_writer.py) -- are one NASM source each, assembled flat (-f bin), and found out in
// rounds: every round assembles the whole file, reads where each instruction went from the
// assembler's map of labels, and writes as its bytes, next round, each instruction the assembler
// rejected or gave back different; until nothing differs. flat() is those rounds; the writer says
// how the file is written and what "different" means for it.
//
// Typing notes (what the Python leaves open):
//   * An instruction is whatever a writer's decoder returns; this module reads only its `.size`
//     and `.target`. The functions that handle instructions are templates over the instruction type
//     `Insn`, which has `int64_t size` and `std::optional<int64_t> target` (the Python's
//     `insn.target is not None`). The `size` argument of the Python (default `_size`, `insn.size`)
//     is an optional function; absent, `insn.size` is used.
//   * The source is a translate::l2::Assembly (its parts keep origin, reason and length); text() hands
//     its document (an L2 Dict) to the assembly back end, which takes it as a typed Document.
//   * The result dict of result() is a record (Result); the fields a writer adds (`**more`:
//     "rom bytes", "config" ...) are `more`.
//   * The "raw" sets of flat() hold whatever a writer calls an instruction there (a template
//     parameter `K`, hashable).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/compare/image.hpp"
#include "codebreaker/translate/backends/asm.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::source::image_writer {

// An argument whose type must not be deduced from the call (so a lambda converts to it).
template <class T> using Fn = typename std::common_type<T>::type;

class Function {
public:
    py::str name;

    explicit Function(const py::str& name);
};

// The writers' own fields of a result ("rom bytes": a count, "config": a path ...).
using Extra = std::variant<int64_t, bool, py::str>;

// What every writer gives back (the Python's dict). Its own fields beyond these are `more`.
struct Result {
    py::str source;
    py::dict<int64_t, Function> functions;
    py::list<py::str> notes;
    bool linked = false;
    bool same = false;
    int64_t code_bytes = 0;                  // "code bytes"
    std::optional<py::str> built;            // "built" (set by assembled() when it keeps the image)
    py::dict<py::str, Extra> more;

    // r[key] for a count (KeyError when absent; TypeError when it is not a number)
    int64_t count(const py::str& key) const;
};

// The source being written, as one L2 Module as it goes (translate/l2.Assembly: a list of lines with part()
// and document()).
using Assembly = translate::l2::Assembly;

template <class Insn> int64_t _size(const Insn& insn) { return insn.size; }

// What flow reaches from `starts`. -> ({key: Insn}, {target}, [function starts])
//
// decode(pc) -> the instruction at pc, or None; inside(pc): may code stand at pc; fits(pc, insn):
// the whole instruction lies in the program; follows(insn): its target (not None) is a place
// flow goes; stops(insn): flow does not go on to the next instruction. key(pc): where the
// instruction is filed (default: pc); calls(insn): the target followed is a function's start.
// (The order the `todo` stack is worked in is the Python set's; the dict, the set and the list
// come out the same whatever it is, in all but the dict's insertion order.)
template <class Insn>
std::tuple<py::dict<int64_t, Insn>, py::set<int64_t>, py::list<int64_t>> walk(
    const py::list<int64_t>& starts, const Fn<std::function<std::optional<Insn>(int64_t)>>& decode,
    const Fn<std::function<bool(int64_t)>>& inside, const Fn<std::function<bool(int64_t, const Insn&)>>& fits,
    const Fn<std::function<bool(const Insn&)>>& follows, const Fn<std::function<bool(const Insn&)>>& stops,
    const Fn<std::function<int64_t(int64_t)>>& key = nullptr, const Fn<std::function<bool(const Insn&)>>& calls = nullptr,
    const Fn<std::function<int64_t(const Insn&)>>& size = nullptr) {
    py::set<int64_t> found = py::set<int64_t>::from(starts);
    py::dict<int64_t, Insn> code;
    py::set<int64_t> targets, seen;
    py::list<int64_t> todo = py::list<int64_t>::from(found);
    while (py::truthy(todo)) {
        int64_t pc = todo.pop();
        while (inside(pc) && !seen.contains(pc)) {
            seen.add(pc);
            std::optional<Insn> insn = decode(pc);
            if (!insn.has_value() || !fits(pc, *insn)) {
                break;
            }
            code.set(key ? key(pc) : pc, *insn);
            if (insn->target.has_value() && follows(*insn)) {
                targets.add(*insn->target);
                todo.append(*insn->target);
                if (calls && calls(*insn)) {
                    found.add(*insn->target);
                }
            }
            if (stops(*insn)) {
                break;
            }
            pc += size ? size(*insn) : _size(*insn);
        }
    }
    if (calls) {
        py::list<int64_t> starts_found = py::list<int64_t>::from(found);
        starts_found.sort();
        return std::make_tuple(code, targets, starts_found);
    }
    return std::make_tuple(code, targets, py::list<int64_t>::from(starts));
}

// {start: Function}, each named from `names` or `fmt` % address (the name is added to `names`).
py::dict<int64_t, Function> functions(const py::list<int64_t>& starts, const py::dict<int64_t, py::str>& names, const py::str& fmt);

// The addresses inside an instruction, past its first byte: no line starts there.
template <class Insn>
py::set<int64_t> covered(const py::dict<int64_t, Insn>& code, const Fn<std::function<int64_t(const Insn&)>>& size = nullptr) {
    py::set<int64_t> inside;
    for (const auto& [at, insn] : code.items()) {
        int64_t n = size ? size(insn) : _size(insn);
        for (int64_t a = at + 1; a < at + n; ++a) {
            inside.add(a);
        }
    }
    return inside;
}

// {address: label} for each flow target and each named address where a line starts.
py::dict<int64_t, py::str> labels(const py::set<int64_t>& targets, const py::dict<int64_t, py::str>& names,
                                  const std::function<bool(int64_t)>& line_start, const py::str& fmt);

// Bytes as source lines: rows of 16, and a run of 16 or more of one value as one fill line.
py::list<py::str> data_lines(const py::list<int64_t>& data, const py::str& db = py::str(".DB"),
                             const py::str& fill = py::str(".DSB %d,$%02X"));
py::list<py::str> data_lines(const py::bytes& data, const py::str& db = py::str(".DB"),
                             const py::str& fill = py::str(".DSB %d,$%02X"));

// (no quote or backslash: every assembler reads it alike)
bool _printable(int64_t b);

// A row's bytes as items: runs of 4 or more printable characters as one string, the rest as $XX.
py::list<py::str> _items(const py::list<int64_t>& row);

// The source being written, as one L2 Module as it goes (l2.Assembly): lines are appended to it
// as to a list, emit() begins a fragment at each function, part() begins one by hand.
Assembly lines(const py::list<py::str>& first = py::list<py::str>(), const std::optional<py::str>& syntax = std::nullopt,
               const std::optional<py::str>& module = std::nullopt);

// The source `out` holds: its L2 Module, written by the assembly back end (backends/asm.py).
py::str text(const Assembly& out);

// `lines` with `what` as a comment on the first.
py::list<py::str> commented(const py::list<py::str>& lines, const py::str& what);

// Append the lines for the program's bytes [start, end) to `out`.
//
// byte(offset) -> the byte there; address(offset) -> where the processor sees it (labels and
// functions are by address); code_at(offset, address) -> the instruction to write there, or None
// for data; text(insn) -> its line; lines(bytes) -> the data's lines. special(offset, address,
// flush), when given, writes what stands at a place of its own (a header field, the vectors) and
// returns how many bytes it took, or 0.
template <class Insn>
void emit(Assembly& out, int64_t start, int64_t end, const Fn<std::function<int64_t(int64_t)>>& byte,
          const Fn<std::function<int64_t(int64_t)>>& address, const Fn<std::function<std::optional<Insn>(int64_t, int64_t)>>& code_at,
          const py::dict<int64_t, py::str>& labels, const py::dict<int64_t, Function>& functions,
          const Fn<std::function<py::str(const Insn&)>>& text, const Fn<std::function<py::list<py::str>(const py::list<int64_t>&)>>& lines,
          const Fn<std::function<int64_t(int64_t, int64_t, const std::function<void()>&)>>& special = nullptr,
          const Fn<std::function<int64_t(const Insn&)>>& size = nullptr) {
    py::list<int64_t> data;
    std::function<void()> flush = [&]() {
        out.extend(lines(data));
        data.clear();
    };
    int64_t offset = start;
    while (offset < end) {
        int64_t addr = address(offset);
        int64_t took = special ? special(offset, addr, flush) : 0;
        if (took) {
            offset += took;
            continue;
        }
        if (labels.contains(addr)) {
            flush();
            if (functions.contains(addr)) {
                // (each function a fragment of the module's tree)
                translate::l2::Dict origin;
                origin.set(py::str("module"), out.module.has_value() ? translate::l2::Val(*out.module) : translate::l2::Val());
                origin.set(py::str("va"), translate::l2::Val(addr));
                out.part(functions.at(addr).name, py::str("function"), origin);
                out.append(py::str(""));
            }
            out.append(py::percent("%s:", labels.at(addr)));
        }
        std::optional<Insn> insn = code_at(offset, addr);
        if (insn.has_value()) {
            flush();
            out.append(py::str("        ") + text(*insn));
            offset += size ? size(*insn) : _size(*insn);
        } else {
            data.append(byte(offset));
            if (labels.contains(addr + 1)) {
                flush();
            }
            offset += 1;
        }
    }
    flush();
}

// Write the source; -> the result every writer gives (`more`: its own fields).
// (A `more` field named like one of the result's own -- "source", "linked", "same", "code bytes",
// "built" -- of the same type replaces it, as dict.update does; any other name stays in `more`.)
Result _result(const py::str& folder, const py::str& name, const py::str& source, const py::dict<int64_t, Function>& functions,
               int64_t code_bytes, const py::dict<py::str, Extra>& more);

template <class Insn>
Result result(const py::str& folder, const py::str& name, const py::str& source, const py::dict<int64_t, Function>& functions,
              const py::dict<int64_t, Insn>& code, const Fn<std::function<int64_t(const Insn&)>>& size = nullptr,
              const py::dict<py::str, Extra>& more = py::dict<py::str, Extra>()) {
    int64_t total = 0;
    for (const Insn& i : code.values()) {
        total += size ? size(i) : _size(i);
    }
    return _result(folder, name, source, functions, total, more);
}

// Assemble with build() -> bytes, keep what came out at `built_path`, and say in `r` whether it
// is the original. same(built), when given, is that test. -> the bytes built, or None.
std::optional<py::bytes> assembled(Result& r, const std::function<py::bytes()>& build, const py::bytes& original,
                                   const std::optional<py::str>& built_path = std::nullopt, const py::str& what = py::str("image"),
                                   const std::function<bool(const py::bytes&)>& same = nullptr);

// The x86 files written whole -------------------------------------------------------------------
extern const int64_t ROUNDS;

// The assembler the whole-file writers use (settings.json: nasm), or "" when there is none.
py::str nasm();

// Assemble `source` flat (-f bin) as <work>/<name>, with its map of labels beside it.
// -> (the file built, or None; what the assembler said; the map's path)
std::tuple<std::optional<py::str>, py::str, py::str> assemble_flat(const py::str& source, const py::str& work, const py::str& name);

// {label: (its address in its section, its place in the file)} from NASM's map.
py::dict<py::str, std::tuple<int64_t, int64_t>> nasm_map(const py::str& path);

// What flat() gives: {"source", "image", "same", "rounds", "raw" (the set), "log"}.
template <class K> struct FlatResult {
    py::str source;
    std::optional<py::str> image;
    bool same = false;
    int64_t rounds = 0;
    py::set<K> raw;
    py::str log;
};

// The tail of a string, as msg[-2000:].
py::str _tail(const py::str& msg, int64_t n);

// The rounds that find which instructions are written as their bytes, then the file built.
//
// write(raw, final) -> (source, {line number: instruction}): the file with the instructions in
// `raw` as bytes (`final`: the last writing, without the round's marks); differing(data, labels,
// raw) -> the instructions the assembler did not give back, given what it built and nasm_map's
// labels. The assembler rejecting nothing new it can be blamed for (anything, or with
// `new_only`, anything not already bytes) ends it.
template <class K>
FlatResult<K> flat(const py::bytes& original, const std::function<std::tuple<py::str, py::dict<int64_t, K>>(const py::set<K>&, bool)>& write,
                   const std::function<py::set<K>(const py::bytes&, const py::dict<py::str, std::tuple<int64_t, int64_t>>&, const py::set<K>&)>&
                       differing,
                   const py::str& work, const py::str& name, const std::function<void(const py::str&)>& say, bool new_only = false) {
    py::set<K> raw;
    int64_t rounds = 0;
    py::str msg("");
    while (rounds < ROUNDS) {
        rounds += 1;
        auto [src, lines_of] = write(raw, false);
        auto [built, msg_, mp] = assemble_flat(src, work, name);
        msg = msg_;
        if (!built.has_value()) {
            py::set<K> bad;
            for (const py::str& n : py::re::findall(py::str(":(\\d+): error"), msg)) {
                std::optional<K> hit = lines_of.get(py::int_parse(n) - 1);
                if (hit.has_value()) {
                    bad.add(*hit);
                }
            }
            say(py::percent("round %d: %d instructions the assembler rejects", rounds, py::len(bad)));
            if (!py::truthy(new_only ? bad - raw : bad)) {
                throw py::SystemExit(py::str("assembler: ") + _tail(msg, 2000));
            }
            raw.update(bad);
            continue;
        }
        py::set<K> bad = differing(py::open_rb(*built).read(), nasm_map(mp), raw);
        say(py::percent("round %d: %d instructions come back different", rounds, py::len(bad)));
        if (!py::truthy(bad)) {
            break;
        }
        raw.update(bad);
    }
    auto [src, _lines] = write(raw, true);
    auto [built, msg_, _mp] = assemble_flat(src, work, name);
    msg = msg_;
    std::optional<py::bytes> data;
    if (built.has_value()) {
        data = py::open_rb(*built).read();
    }
    bool same = built.has_value() && data.has_value() && *data == original;
    if (built.has_value() && !same) {
        int64_t k = *compare::image::first_difference(*data, original);
        say(py::percent("first difference at file offset 0x%x (sizes %d, %d)", k, py::len(*data), py::len(original)));
    }
    FlatResult<K> r;
    r.source = py::os::path::join(work, name + py::str(".asm"));
    r.image = built;
    r.same = same;
    r.rounds = rounds;
    r.raw = raw;
    r.log = _tail(msg, 2000);
    return r;
}

// What flat_report() reads of a whole-file writer's result: the writer replaces "raw" with a count
// and adds the number of "instructions".
struct FlatReport {
    int64_t instructions = 0;
    int64_t raw = 0;
    int64_t rounds = 0;
    bool same = false;
    std::optional<py::str> image;
    py::str source;
};

// What the command line of a whole-file writer prints. -> its exit code
int64_t flat_report(const py::str& path, const FlatReport& r);

// The command line every writer has: <program> [<out folder>].
int64_t main(const py::list<py::str>& argv, const std::function<Result(const py::str&, const py::str&)>& reconstruct, const py::str& doc,
             const py::str& unit = py::str("ROM bytes"), const py::str& total = py::str("rom bytes"));

}  // namespace codebreaker::source::image_writer
