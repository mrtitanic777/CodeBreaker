// codebreaker/source/pe_writer.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/source/pe_writer.{hpp,cpp}; this
// header keeps the old py:: surface (reconstruct, main, DOC) for the dynamic exe. Conversion and forwarding only, except
//   COMPAT(py): `Insn`, `decode`, `is_branch`, `branch_target` -- the capstone instruction codebreaker/source/dos_writer.cpp reads, kept in the
//       trimmed old pe_writer.cpp; and `_stub`, the DOS stub finder that reaches the old dos_writer. Remove all of it when source/dos_writer is
//       refactored (the new module then finds the stub itself).
//
// Not carried over: the internals the module's own port exposed (Writer, Sec, _STOP, _SEGS, _ARITH, _RR, _PRELUDE, _differing).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/source/pe_writer.hpp"
#include "codebreaker/source/image_writer.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::source::pe_writer {

inline const py::str DOC(cb::compat::to_py(cb::source::pe_writer::usage()));

// One instruction as capstone decodes it (with detail): the fields the old dos_writer port reads.
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str mnemonic;
    py::str op_str;
    py::bytes bytes;
    int64_t imm_size = 0, imm_offset = 0, disp_size = 0, disp_offset = 0;
    py::list<int64_t> prefix;                // the four prefix bytes
    py::list<int64_t> groups;                // capstone's instruction groups (X86_GRP_*)
    int64_t op_count = 0;
    int64_t op0_type = 0;                    // operands[0].type and .imm, when there is one
    int64_t op0_imm = 0;
};

// next(md.disasm(code, address, 1), None): the first instruction of `code` as the x86 decoder of `bits` reads it.
std::optional<Insn> decode(int64_t bits, const py::bytes& code, int64_t address);
bool is_branch(const py::str& mnemonic);
std::optional<int64_t> branch_target(const Insn& i);

// The DOS program in front of the headers, from its own source (dos-source) when that comes back as the original's bytes; None (its bytes
// kept) when it does not. `d` is the file, `e_lfanew` where its new header is.
std::optional<py::str> _stub(const py::bytes& d, int64_t e_lfanew, const py::str& outdir, const std::function<void(const py::str&)>& say);

struct Result {
    py::str source;
    std::optional<py::str> image;
    bool same = false;
    int64_t rounds = 0;
    int64_t raw = 0;
    py::str log;
    int64_t instructions = 0;

    image_writer::FlatReport report() const {
        image_writer::FlatReport r;
        r.instructions = instructions;
        r.raw = raw;
        r.rounds = rounds;
        r.same = same;
        r.image = image;
        r.source = source;
        return r;
    }
};

using StubArg = std::variant<std::monostate, bool, py::str>;   // None, False, or the stub's source

namespace shim_ {
// The DOS stub's source, found by the old DOS writer.
inline cb::source::FindStub find_stub() {
    return [](cb::ByteView file, std::uint64_t new_header, const std::filesystem::path& out,
              const std::function<void(std::string_view)>& say) -> std::optional<std::filesystem::path> {
        const std::optional<py::str> found = _stub(cb::compat::to_py(file), static_cast<int64_t>(new_header), cb::compat::to_py(cb::fs::to_text(out)),
                                                   [&](const py::str& line) { say(cb::compat::from_py(line)); });
        if (!found || found->empty()) return std::nullopt;
        return cb::fs::from_text(cb::compat::from_py(*found));
    };
}
}  // namespace shim_

inline Result reconstruct(const py::str& path, const py::str& outdir, const StubArg& stub = std::monostate(),
                          const std::function<void(const py::str&)>& log = nullptr) {
    return cb::compat::guard([&] {
        namespace n = cb::source::pe_writer;
        n::Options options;
        options.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm()));
        options.say = [&](std::string_view line) {
            if (log) log(cb::compat::to_py(line));
        };
        if (std::holds_alternative<std::monostate>(stub)) {
            options.stub = cb::source::SearchStub{};
            options.find_stub = shim_::find_stub();
        } else if (const auto* given = std::get_if<py::str>(&stub)) {
            options.stub = cb::fs::from_text(cb::compat::from_py(*given));
        } else {
            options.stub = cb::source::KeepStub{};
        }
        const n::Result r = n::reconstruct(cb::fs::from_text(cb::compat::from_py(path)), cb::fs::from_text(cb::compat::from_py(outdir)), options);
        Result out;
        out.source = cb::compat::to_py(cb::fs::to_text(r.source));
        if (r.image) out.image = cb::compat::to_py(cb::fs::to_text(*r.image));
        out.same = r.same;
        out.rounds = static_cast<int64_t>(r.rounds);
        out.raw = static_cast<int64_t>(r.raw);
        out.log = cb::compat::to_py(r.log);
        out.instructions = static_cast<int64_t>(r.instructions);
        return out;
    });
}

// python -m codebreaker pe-source <program> [--out DIR] [--stub FILE] [--keep-stub]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        cb::source::pe_writer::Environment env{.nasm = cb::fs::from_text(cb::compat::from_py(image_writer::nasm())),
                                               .state = cb::fs::from_text(cb::compat::from_py(py::settings::get_str(py::str("state")))),
                                               .find_stub = shim_::find_stub()};
        return static_cast<int64_t>(cb::source::pe_writer::command(args, env, [](std::string_view line) { py::print(cb::compat::to_py(line)); }));
    });
}

}  // namespace codebreaker::source::pe_writer
