// codebreaker/build/native_dos.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/native_dos.hpp for the ports that still use the
// old API (build/masm, source/vxd/masm). No logic: conversion and forwarding. Staged under native/cb/compat/shims/ until the
// exe's build is wired for C++20.
//
// Old API: NativeDosError, Assembly, available, enabled, _PROVEN, assemble, run_extended, ml_files, ml, assemble_many.
// (_run_tool, _stem_path, _masm_exe, _read_out, _cmdline were helpers of those: not carried over.)
//
// The DOS executive is exact/emu/dos with the DPMI host, which is not refactored yet: `OldDosExecutive` is it behind the new
// interface -- COMPAT(py): remove when exact/emu/dos is refactored (it then provides a cb::build::native_dos::DosExecutive itself).
// Settings the old functions read when not given (`masm`, `msvc152`, the checkout root) are read here, as the old code did.
//
// DIVERGES, for the old callers: an Assembly / Assembled has no work folder (`work` is "" -- the new runner deletes it, the
// products are already read; callers' rmtree of it is a no-op); a .LST / .LOG keeps the CRLF line ends the tool wrote (the old
// read translated them to LF); a stem that names a folder outside the work folder is refused with ValueError.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "dos_sessions.hpp"   // the shim beside this one: Product, Assembled

#include "cb/build/native_dos.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/emu/dpmi.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::native_dos {

namespace nd = cb::build::native_dos;

// The native runner could not complete a run (unimplemented service, CPU fault, limit): the caller falls back to DOSBox.
struct NativeDosError : py::RuntimeError {
    NativeDosError() : py::RuntimeError() {}
    explicit NativeDosError(const std::string& m) : py::RuntimeError(m) {}
    const char* type() const override { return "NativeDosError"; }
};

// what an assembly made: {obj, lst, log, work}; a part the tool did not make is nullopt.
struct Assembly {
    std::optional<py::bytes> obj;
    std::optional<py::str> lst;
    std::optional<py::str> log;
    py::str work;
    bool aborted = false;
};

using Product = dos_sessions::Product;
using Assembled = dos_sessions::Assembled;
using FileData = std::variant<py::str, py::bytes>;   // text (CRLF on "disk") or bytes

// tools proven byte-identical to DOSBox (MASM; the DDK TNT ML)
inline const py::set<py::str> _PROVEN = py::set<py::str>{py::str("masm"), py::str("ml")};

// The old exact/emu/dos behind the new executive interface. A refusal (any exception of the old layer) is the NativeDosError of the new
// layer, with the text the old one gave: "<type>: <message>".
class OldDosExecutive final : public nd::DosExecutive {
public:
    bool available() const override {
        try {
            codebreaker::exact::emu::dpmi::UnicornEngine probe;   // Python: `import unicorn`
        } catch (const std::exception&) {
            return false;
        }
        return true;
    }

    nd::Exit run(const std::filesystem::path& exe, const std::filesystem::path& work, std::string_view cmdline) const override {
        namespace dos = codebreaker::exact::emu::dos;
        try {
            dos::Result r = dos::run(py::str(cb::fs::to_text(exe)), py::str(cb::fs::to_text(work)), py::str(std::string(cmdline)),
                                     py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false, py::bytes(std::string("PATH=\0", 6)),
                                     std::nullopt, std::nullopt, true);
            nd::Exit out;
            if (r.exit) out.code = static_cast<std::int32_t>(*r.exit);
            out.console = cb::compat::from_py(r.console);
            return out;
        } catch (const py::Exception& e) {
            throw nd::NativeDosError(std::string(e.type()) + ": " + e.what());
        } catch (const std::exception& e) {
            throw nd::NativeDosError(std::string("Exception: ") + e.what());
        }
    }

    nd::MemoryResult run_dpmi(const nd::MemoryRun& run) const override {
        namespace dos = codebreaker::exact::emu::dos;
        // a name only: nothing is written there
        const py::str work = py::os::path::join(py::os::path::join(py::settings::checkout_root(), py::str("state")), py::str("native-dos-mem"));
        dos::MemFS fs;
        for (const auto& [name, data] : run.files) fs.put(py::os::path::join(work, py::str(name)), cb::compat::to_py(cb::ByteView(data)));
        dos::Result r;
        try {
            r = dos::run(py::os::path::join(work, py::str(run.exe)), work, py::str(run.cmdline), py::bytes(std::string()),
                         dos::DEFAULT_MAX_BLOCKS, false, cb::compat::to_py(cb::ByteView(run.env)), std::nullopt, std::nullopt, true, &fs, true);
        } catch (const py::Exception& e) {
            throw nd::NativeDosError(std::string(e.type()) + ": " + e.what());
        } catch (const std::exception& e) {
            throw nd::NativeDosError(std::string("Exception: ") + e.what());
        }
        nd::MemoryResult out;
        for (const std::string& name : run.artifacts) {
            const py::str p = py::os::path::join(work, py::str(name).upper());
            if (fs.isfile(p) && fs.files.contains(fs.key(p))) out.made[name] = cb::compat::from_py(fs.read(p));
        }
        out.console = cb::compat::from_py(r.console);
        return out;
    }
};

inline const OldDosExecutive& executive() {
    static const OldDosExecutive dos;
    return dos;
}

// A NativeDosError of the new layer is the old one; any other cb::Error the matching py:: exception.
template <class F>
decltype(auto) as_old(F&& f) {
    try {
        return std::forward<F>(f)();
    } catch (const nd::NativeDosError& e) {
        throw NativeDosError(e.what());
    } catch (const cb::Error& e) {
        cb::compat::rethrow_as_py(e);
    }
}

inline bool available() { return executive().available(); }

// Whether the native runner should be used for `tool`. See the CB_NATIVE_DOS switch of cb/build/native_dos.hpp.
inline bool enabled(const py::str& tool = py::str("masm")) { return nd::enabled(executive(), tool.utf8()); }

inline std::filesystem::path masm_root_of(const std::optional<py::str>& masmroot) {
    const py::str root = (masmroot && !masmroot->empty()) ? *masmroot : py::settings::get_str(py::str("masm"));
    return cb::fs::from_text(root.utf8());
}

namespace detail {
inline Product to_old(const cb::build::dos_sessions::Product& p) {
    Product out;
    if (p.obj) out.obj = cb::compat::to_py(cb::ByteView(*p.obj));
    if (p.lst) out.lst = py::str(*p.lst);
    if (p.log) out.log = py::str(*p.log);
    return out;
}
inline std::map<std::string, cb::build::DosFile> files_of(const py::dict<py::str, FileData>& files) {
    std::map<std::string, cb::build::DosFile> out;
    for (const auto& [name, src] : files.items()) {
        if (std::holds_alternative<py::str>(src)) {
            out[name.utf8()] = std::get<py::str>(src).utf8();
        } else {
            out[name.utf8()] = cb::compat::from_py(std::get<py::bytes>(src));
        }
    }
    return out;
}
inline std::vector<std::string> names_of(const py::list<py::str>& names) {
    std::vector<std::string> out;
    for (const py::str& n : names.vec()) out.push_back(n.utf8());
    return out;
}
inline py::dict<py::str, py::bytes> made_of(const std::map<std::string, cb::Bytes>& made) {
    py::dict<py::str, py::bytes> out;
    for (const auto& [name, data] : made) out.set(py::str(name), cb::compat::to_py(cb::ByteView(data)));
    return out;
}
}  // namespace detail

// One source, the native runner. Raises NativeDosError if the executive could not complete (the caller then uses DOSBox).
inline Assembly assemble(const py::str& asm_src, const py::str& stem = py::str("t"), const std::optional<py::str>& masmroot = std::nullopt) {
    return as_old([&] {
        const Product p = detail::to_old(nd::assemble(executive(), asm_src.utf8(), stem.utf8(), masm_root_of(masmroot)));
        Assembly a;
        a.obj = p.obj;
        a.lst = p.lst;
        a.log = p.log;
        a.work = py::str("");
        return a;
    });
}

// Run a DOS-extended (DPMI) tool -- the Phar Lap TNT ML.EXE -- entirely in memory: the tool's own files ({name: path}) and the
// inputs ({name: text or bytes}) are given. -> ({artifact: bytes} of the ones made, console bytes).
inline std::tuple<py::dict<py::str, py::bytes>, py::bytes> run_extended(
    const py::dict<py::str, py::str>& tool_files, const py::str& exe, const py::str& cmdline, const py::dict<py::str, FileData>& files,
    const py::list<py::str>& artifacts, const py::bytes& env = py::bytes(std::string("PATH=C:\\\0", 9))) {
    return as_old([&] {
        std::map<std::string, std::filesystem::path> tools;
        for (const auto& [name, path] : tool_files.items()) tools[name.utf8()] = cb::fs::from_text(path.utf8());
        const std::vector<std::string> wanted = detail::names_of(artifacts);
        const nd::MemoryResult r = nd::run_extended(executive(), tools, exe.utf8(), cmdline.utf8(), detail::files_of(files), wanted,
                                                    cb::compat::from_py(env));
        return std::make_tuple(detail::made_of(r.made), cb::compat::to_py(cb::ByteView(r.console)));
    });
}

namespace detail {
inline nd::MlToolchain toolchain_of(const std::optional<py::str>& root) {
    nd::MlToolchain tc;
    tc.ml_dir = cb::fs::from_text(base::corpus::reference(py::list<py::str>{py::str("Microsoft Windows 95 DDK"), py::str("MASM611C")}).utf8());
    py::str msvc = py::settings::get_str(py::str("msvc152"));
    if (msvc.empty()) {
        const py::str r = (root && !root->empty()) ? *root : py::settings::checkout_root();
        msvc = py::os::path::join(py::os::path::join(py::os::path::join(py::os::path::join(r, py::str("toolchains")), py::str("period")),
                                                     py::str("compilers")),
                                  py::str("msvc-1.52c"));
    }
    tc.msvc_root = cb::fs::from_text(msvc.utf8());
    return tc;
}
}  // namespace detail

// The Win95 DDK's Phar Lap TNT ML.EXE, its ML.ERR and the DOSXNT.EXE extender: {name: path}.
inline py::dict<py::str, py::str> ml_files(const std::optional<py::str>& root = std::nullopt) {
    return as_old([&] {
        py::dict<py::str, py::str> out;
        for (const auto& [name, path] : nd::ml_files(detail::toolchain_of(root))) out.set(py::str(name), py::str(cb::fs::to_text(path)));
        return out;
    });
}

// TNT ML.EXE natively: ml("/c /Fom.obj m.asm", {"m.asm": text}, ["m.obj"]) -> ({artifact: bytes}, console text).
inline std::tuple<py::dict<py::str, py::bytes>, py::str> ml(const py::str& args, const py::dict<py::str, FileData>& files,
                                                            const py::list<py::str>& artifacts, const std::optional<py::str>& root = std::nullopt) {
    return as_old([&] {
        const std::vector<std::string> wanted = detail::names_of(artifacts);
        const nd::MlResult r = nd::ml(executive(), detail::toolchain_of(root), args.utf8(), detail::files_of(files), wanted);
        return std::make_tuple(detail::made_of(r.made), py::str(r.console));
    });
}

// [(stem, asm_src)] -> {work: dir, results: {stem: Product}}. One MASM run per item, in a shared work folder. Raises NativeDosError the
// moment any item's run could not complete, so the caller falls the WHOLE call back to DOSBox.
inline Assembled assemble_many(const py::list<std::tuple<py::str, py::str>>& items, const std::optional<py::str>& masmroot = std::nullopt) {
    return as_old([&] {
        std::vector<cb::build::dos_sessions::Source> sources;
        for (const auto& [stem, text] : items.vec()) sources.push_back({stem.utf8(), text.utf8()});
        const cb::build::dos_sessions::Finished done = nd::assemble_many(executive(), sources, masm_root_of(masmroot));
        Assembled a;
        py::dict<py::str, Product> results;
        for (const auto& [stem, p] : done.results) results.set(py::str(stem), detail::to_old(p));
        a.results = results;
        a.work = py::str("");
        return a;
    });
}

}  // namespace codebreaker::build::native_dos
