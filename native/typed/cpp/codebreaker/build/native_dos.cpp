// codebreaker/build/native_dos.cpp -- typed C++17 port of codebreaker.build.native_dos (see native_dos.hpp).

#include "codebreaker/build/native_dos.hpp"

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/exact/emu/dos.hpp"
#include "codebreaker/exact/emu/dpmi.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::native_dos {

namespace dos = codebreaker::exact::emu::dos;
using py::str;

const py::set<str> _PROVEN = py::set<str>{str("masm"), str("ml")};   // tools proven byte-identical to DOSBox (MASM; the DDK TNT ML)

bool available() {
    try {
        exact::emu::dpmi::UnicornEngine probe;   // Python: `import unicorn`
    } catch (const std::exception&) {
        return false;
    }
    return true;
}

bool enabled(const str& tool) {
    str mode = py::os::environ_::get(str("CB_NATIVE_DOS"), str("auto")).lower();
    if (py::in(mode, std::make_tuple(str("0"), str("off"), str("no"), str("false")))) return false;
    if (!available()) return false;
    if (py::in(mode, std::make_tuple(str("1"), str("on"), str("yes"), str("true")))) return true;
    return _PROVEN.contains(tool);   // "auto": only the proven tools
}

namespace {
// "%s: %s" % (type(e).__name__, e)
NativeDosError refused(const std::exception& e) {
    const py::Exception* p = dynamic_cast<const py::Exception*>(&e);
    return NativeDosError(std::string(p ? p->type() : "Exception") + ": " + e.what());
}
}  // namespace

std::tuple<std::optional<int64_t>, py::bytes> _run_tool(const str& exe, const str& work, const str& cmdline, bool fast) {
    try {
        dos::Result r = dos::run(exe, work, cmdline, py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false,
                                 py::bytes(std::string("PATH=\0", 6)), std::nullopt, std::nullopt, fast);
        return {r.exit, r.console};
    } catch (const std::exception& e) {   // unimplemented INT, CPU fault, block/time limit
        throw refused(e);
    }
}

str _stem_path(const str& work, const str& stem, const str& ext) {
    py::list<str> parts = py::split(stem.replace(str("\\"), str("/")), str("/"));
    str d = work;
    if (parts.size() > 1)
        for (int64_t i = 0; i < parts.size() - 1; ++i) d = py::os::path::join(d, parts[i]);
    py::os::makedirs(d, 0777, true);
    return py::os::path::join(d, parts[parts.size() - 1] + ext);
}

str _masm_exe(const str& work, const str& masmroot) {
    str exe = py::os::path::join(work, str("MASM.EXE"));
    if (!py::os::path::exists(exe)) {
        str src = py::os::path::join(py::os::path::join(masmroot, str("BIN")), str("MASM.EXE"));
        if (!py::os::path::exists(src)) throw NativeDosError("MASM.EXE not found under " + masmroot.utf8() + "\\BIN");
        py::shutil::copy(src, exe);
    }
    return exe;
}

Product _read_out(const str& work, const str& stem) {
    Product out;
    for (const char* ext : {".OBJ", ".LST", ".LOG"}) {
        str p = _stem_path(work, stem, str(ext));
        if (py::os::path::isfile(p)) {
            if (str(ext) == str(".OBJ")) {
                py::file f = py::open_rb(p);
                out.obj = f.read();
                f.close();
            } else {
                py::file f = py::open(p, str("r"), str("latin1"));
                (str(ext) == str(".LST") ? out.lst : out.log) = f.read_text();
                f.close();
            }
        }
    }
    return out;
}

str _cmdline(const str& stem) {
    // the ASM must be given with a sub-path if the stem has one; the outputs the same
    str name = stem.replace(str("/"), str("\\"));
    return name + str(".ASM,") + name + str(".OBJ,") + name + str(".LST,NUL;");
}

namespace {
// open(path, "w", newline="\r\n").write(asm_src.replace("\r\n", "\n"))
void write_asm(const str& path, const str& asm_src) {
    py::file f = py::open(path, str("w"), std::nullopt, std::nullopt, str("\r\n"));
    f.write(asm_src.replace(str("\r\n"), str("\n")));   // one newline convention -> clean CRLF on disk
    f.close();
}
}  // namespace

Assembly assemble(const str& asm_src, const str& stem, const std::optional<str>& masmroot_in) {
    str masmroot = (masmroot_in && !masmroot_in->empty()) ? *masmroot_in : py::settings::get_str(str("masm"));
    str work = py::tempfile::mkdtemp(std::nullopt, str("masm_nat_"));
    write_asm(_stem_path(work, stem, str(".ASM")), asm_src);
    str exe = _masm_exe(work, masmroot);
    auto [_exit, console] = _run_tool(exe, work, _cmdline(stem));
    Product out = _read_out(work, stem);
    Assembly a;
    a.obj = out.obj;
    a.lst = out.lst;
    a.log = out.log ? *out.log : console.decode("latin1");   // setdefault("log", ...)
    a.work = work;
    return a;
}

std::tuple<py::dict<str, py::bytes>, py::bytes> run_extended(const py::dict<str, str>& tool_files, const str& exe, const str& cmdline,
                                                            const py::dict<str, FileData>& files, const py::list<str>& artifacts,
                                                            const py::bytes& env) {
    // a name only: nothing is written there
    str work = py::os::path::join(py::os::path::join(py::settings::checkout_root(), str("state")), str("native-dos-mem"));
    dos::MemFS fs;
    for (const auto& kv : tool_files.items())   // (read once, kept for every run after)
        fs.put(py::os::path::join(work, std::get<0>(kv).upper()), fs.disk(std::get<1>(kv)));
    for (const auto& kv : files.items()) {   // text: CRLF on "disk", as a text-mode write
        const FileData& src = std::get<1>(kv);
        py::bytes data = std::holds_alternative<str>(src)
                             ? std::get<str>(src).replace(str("\r\n"), str("\n")).replace(str("\n"), str("\r\n")).encode("latin1")
                             : std::get<py::bytes>(src);
        fs.put(py::os::path::join(work, std::get<0>(kv).upper()), data);
    }
    dos::Result r;
    try {
        r = dos::run(py::os::path::join(work, exe.upper()), work, cmdline, py::bytes(std::string()), dos::DEFAULT_MAX_BLOCKS, false, env,
                     std::nullopt, std::nullopt, true, &fs, true);
    } catch (const std::exception& e) {
        throw refused(e);
    }
    py::dict<str, py::bytes> made;
    for (const str& name : artifacts) {
        str p = py::os::path::join(work, name.upper());
        if (fs.isfile(p) && fs.files.contains(fs.key(p))) made.set(name, fs.read(p));
    }
    return {made, r.console};
}

py::dict<str, str> ml_files(const std::optional<str>& root) {
    str ml_dir = base::corpus::reference(py::list<str>{str("Microsoft Windows 95 DDK"), str("MASM611C")});
    str msvc = py::settings::get_str(str("msvc152"));
    if (msvc.empty()) {
        str r = (root && !root->empty()) ? *root : py::settings::checkout_root();
        msvc = py::os::path::join(py::os::path::join(py::os::path::join(py::os::path::join(r, str("toolchains")), str("period")),
                                                     str("compilers")),
                                  str("msvc-1.52c"));
    }
    str dx = py::os::path::join(py::os::path::join(msvc, str("BIN")), str("DOSXNT.EXE"));
    py::dict<str, str> out;
    out.set(str("ML.EXE"), py::os::path::join(ml_dir, str("ML.EXE")));
    out.set(str("ML.ERR"), py::os::path::join(ml_dir, str("ML.ERR")));
    out.set(str("DOSXNT.EXE"), dx);
    std::string missing;
    for (const str& p : out.values()) {
        if (!py::os::path::isfile(p)) missing += (missing.empty() ? "" : ", ") + p.utf8();
    }
    if (!missing.empty()) throw NativeDosError("TNT ML toolchain not found: " + missing);
    return out;
}

std::tuple<py::dict<str, py::bytes>, str> ml(const str& args, const py::dict<str, FileData>& files, const py::list<str>& artifacts,
                                            const std::optional<str>& root) {
    auto [made, console] = run_extended(ml_files(root), str("ML.EXE"), args, files, artifacts);
    return {made, console.decode("latin1")};
}

Assembled assemble_many(const py::list<std::tuple<str, str>>& items, const std::optional<str>& masmroot_in) {
    str masmroot = (masmroot_in && !masmroot_in->empty()) ? *masmroot_in : py::settings::get_str(str("masm"));
    str work = py::tempfile::mkdtemp(std::nullopt, str("masm_nat_"));
    str exe = _masm_exe(work, masmroot);
    py::dict<str, Product> results;
    for (const auto& item : items) {
        const str& stem = std::get<0>(item);
        write_asm(_stem_path(work, stem, str(".ASM")), std::get<1>(item));
        auto [_exit, console] = _run_tool(exe, work, _cmdline(stem));
        Product d = _read_out(work, stem);
        if (!d.log) d.log = console.decode("latin1");   // setdefault("log", ...)
        results.set(stem, d);
    }
    Assembled a;
    a.work = work;
    a.results = results;
    return a;
}

}  // namespace codebreaker::build::native_dos
