// codebreaker/analyze/dos_functions.hpp -- COMPAT SHIM over cb/analyze/dos_functions.hpp (docs/CPP_DESIGN.md 6.3), for the old
// ports that read a real-mode program's functions (decompile/describe, source/dos_writer, compare/versions). Deleted with its
// last py:: consumer. Conversion and forwarding only: the analysis is native/cb/analyze/dos_functions.hpp.
//
// dos_functions.py -- where the functions of a real-mode program are, and what is code at all.
//
//     namespace df = codebreaker::analyze::dos_functions;
//     df::Analysis a = df::discover(dos::read(path));
//     a.functions        {linear address: Function}  .name .distance ("far"|"near") .removes .evidence .sure
//     a.interrupts       [Interrupt]  every `int` reached: .address .number .ah .al .function
//     a.tables           {address: number of entries}  jump tables read
//     a.census()         bytes of the program by what is known about them
//     python -m codebreaker dos-functions <program> [--all]      (main)
//
// What differs from the port it replaces: the analysis's inner workings (`_decode`, `_looks_like_code`, `_table`, `_follow` ...)
// are not here (only the old harness called them); `_mark_bytes()` and `_guess_bytes()` stay, since source/dos_writer reads the
// instruction starts; `functions` is in address order, not in the order they were found; a program with no data segment, or a
// relocation on the last byte of the image, no longer raises TypeError.
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>

#include "cb/analyze/dos_functions.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/symbols/dos_interrupts.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/dos.hpp"
#include "py.hpp"

namespace codebreaker::analyze::dos_functions {

namespace ns = ::cb::analyze::dos_functions;

// (cb finds Capstone beside the executable; there is nothing to load from a path.)
inline void set_capstone_dll(const py::str&) {}

class Function {
public:
    int64_t address;
    py::str name, distance, evidence;
    int64_t cs;
    bool sure;             // found by following the program from its entry points
    int64_t removes = -1;  // bytes of arguments its return removes; -1 not established
    bool interrupt = false;  // returns with iret
    int64_t size = 0;      // bytes of instructions reached from its start

    Function(int64_t address_, const py::str& name_, const py::str& distance_, const py::str& evidence_, int64_t cs_, bool sure_ = true)
        : address(address_), name(name_), distance(distance_), evidence(evidence_), cs(cs_), sure(sure_) {}
    py::str repr() const { return py::percent(py::str("Function(%s at %#x, %s, %s)"), name, address, distance, evidence); }
};

class Interrupt {
public:
    int64_t address, number;
    std::optional<int64_t> ah, al;
    py::ref<Function> function;   // null: None
    Interrupt(int64_t address_, int64_t number_, const std::optional<int64_t>& ah_, const std::optional<int64_t>& al_, const py::ref<Function>& function_)
        : address(address_), number(number_), ah(ah_), al(al_), function(function_) {}
};

namespace shim_detail {

inline ::cb::formats::dos::Kind kind_of(const py::str& kind) {
    namespace cd = ::cb::formats::dos;
    const std::string k = kind.utf8();
    for (cd::Kind candidate : {cd::Kind::Com, cd::Kind::Mz, cd::Kind::Driver, cd::Kind::Kernel, cd::Kind::KernelPart, cd::Kind::DriverWithVxd,
                               cd::Kind::DosWithWindows}) {
        if (k == cd::kind_name(candidate)) return candidate;
    }
    return cd::Kind::Mz;
}

inline ::cb::formats::dos::Program program_to_new(const formats::dos::Program& p) {
    namespace cd = ::cb::formats::dos;
    cd::Program n;
    n.kind = kind_of(p.kind);
    if (p.packer) {
        for (cd::Packer packer : {cd::Packer::ExePack, cd::Packer::LzExe, cd::Packer::PkLite, cd::Packer::Diet, cd::Packer::Upx, cd::Packer::MovesItself}) {
            if (p.packer->utf8() == cd::packer_name(packer)) n.packer = packer;
        }
        if (p.unpacked) n.unpacked = p.unpacked->utf8() == "emulated" ? cd::Unpacked::Emulated : cd::Unpacked::Static;
    }
    n.base = static_cast<std::uint32_t>(p.base);
    n.image = ::cb::compat::from_py(p.image);
    for (const formats::dos::Span& s : p.spans) {
        n.spans.push_back({static_cast<std::uint32_t>(std::get<0>(s)), static_cast<std::uint32_t>(std::get<1>(s)), std::get<2>(s).utf8()});
    }
    for (const formats::dos::EntryPoint& e : p.entries) {
        n.entries.push_back({static_cast<std::uint32_t>(std::get<0>(e)), std::get<1>(e).utf8(),
                             std::get<2>(e) == py::str("far") ? cd::Entry::Call::Far : cd::Entry::Call::Near});
    }
    for (const auto& kv : p.segments.items()) {
        n.segments.insert_or_assign(static_cast<std::uint32_t>(std::get<0>(kv)), static_cast<std::uint32_t>(std::get<1>(kv)));
    }
    for (int64_t r : p.relocations) n.relocations.push_back(static_cast<std::uint32_t>(r));
    if (p.ds) n.ds = static_cast<std::uint32_t>(*p.ds); else n.ds.reset();
    for (const py::str& note : p.notes) n.notes.push_back(note.utf8());
    return n;
}

}  // namespace shim_detail

// What discover() found. See the top.
class Analysis {
public:
    formats::dos::Program program;
    py::dict<int64_t, py::ref<Function>> functions;
    py::list<Interrupt> interrupts;
    py::dict<int64_t, int64_t> tables;
    py::dict<int64_t, int64_t> vectors;

    explicit Analysis(const formats::dos::Program& program_) : program(program_) {}

    Analysis& run() {
        const ns::Analysis a = ns::discover(shim_detail::program_to_new(program));
        for (const auto& [va, f] : a.functions) {
            auto old = std::make_shared<Function>(static_cast<int64_t>(va), ::cb::compat::to_py(f.name), ::cb::compat::to_py(ns::name_of(f.distance)),
                                              ::cb::compat::to_py(ns::name_of(f.evidence)), static_cast<int64_t>(f.cs), f.sure);
            old->removes = f.removes;
            old->interrupt = f.interrupt;
            old->size = f.size;
            functions.set(static_cast<int64_t>(va), old);
        }
        for (const ns::Interrupt& i : a.interrupts) {
            interrupts.append(Interrupt(i.address, i.number, i.ah ? std::optional<int64_t>(*i.ah) : std::nullopt,
                                        i.al ? std::optional<int64_t>(*i.al) : std::nullopt,
                                        i.function ? functions.at(static_cast<int64_t>(*i.function)) : py::ref<Function>()));
        }
        for (const auto& [at, n] : a.tables) tables.set(static_cast<int64_t>(at), static_cast<int64_t>(n));
        for (const auto& [handler, number] : a.vectors) vectors.set(static_cast<int64_t>(handler), static_cast<int64_t>(number));
        census_ = a.census;
        for (ns::ByteClass b : a.bytes) marks_.push_back(static_cast<char>(b));
        for (bool g : a.guessed) guesses_.push_back(g ? '\1' : '\0');
        return *this;
    }

    // {"code": n, "guessed code": n, "jump tables": n, "not classified": n, "total": n} -- bytes of the program's own spans.
    py::dict<py::str, int64_t> census() const {
        py::dict<py::str, int64_t> out;
        out.set(py::str("code"), static_cast<int64_t>(census_.code));
        out.set(py::str("guessed code"), static_cast<int64_t>(census_.guessed_code));
        out.set(py::str("jump tables"), static_cast<int64_t>(census_.jump_tables));
        out.set(py::str("not classified"), static_cast<int64_t>(census_.not_classified));
        out.set(py::str("total"), static_cast<int64_t>(census_.total));
        return out;
    }

    // the Python's _mark (1 instruction start, 2 inside one, 3 jump table) and _guess bytearrays
    py::bytes _mark_bytes() const { return py::bytes(marks_); }
    py::bytes _guess_bytes() const { return py::bytes(guesses_); }

private:
    ns::Census census_;
    std::string marks_, guesses_;
};

// Find the functions of a Program (formats.dos.read). -> Analysis
inline Analysis discover(const formats::dos::Program& program) {
    Analysis a(program);
    a.run();
    return a;
}

// python -m codebreaker dos-functions <program> [--all]
//
// The functions found in a real-mode program (a DOS .EXE or .COM, a device driver, IO.SYS):
// how many by which evidence, how much of the program is code, and the interrupt services it
// calls. --all lists every function.
inline const py::str MAIN_DOC =
    "python -m codebreaker dos-functions <program> [--all]\n"
    "\n"
    "The functions found in a real-mode program (a DOS .EXE or .COM, a device driver, IO.SYS):\n"
    "how many by which evidence, how much of the program is code, and the interrupt services it\n"
    "calls. --all lists every function.";

inline int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(MAIN_DOC);
        return 2;
    }
    const formats::dos::Program p = formats::dos::read(codebreaker::base::corpus::resolve(argv[0]));
    const ::cb::formats::dos::Program program = shim_detail::program_to_new(p);
    const ns::Analysis a = ns::discover(program);
    bool all = false;
    for (const py::str& x : argv) all = all || x == py::str("--all");
    for (const std::string& line : ns::report(p.name.utf8(), program, a, all, ::cb::symbols::dos_interrupts::ServiceTable::standard())) {
        py::print(::cb::compat::to_py(line));
    }
    return 0;
}

}  // namespace codebreaker::analyze::dos_functions
