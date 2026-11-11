// codebreaker/build/msvc152.cpp -- typed C++ port of codebreaker.build.msvc152 (see msvc152.hpp).

#include "codebreaker/build/msvc152.hpp"

#include <vector>

#include "codebreaker/build/dos_tools.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::build::msvc152 {

namespace {

py::str setting_or(const char* key, const char* dflt) {
    const py::str v = py::settings::get_str(py::str(key));
    return v.empty() ? py::str(dflt) : v;   // settings.get(key) or dflt
}

}  // namespace

py::str DBX = setting_or("dosbox_x", "dosbox-x");   // settings.json: dosbox_x
py::str MSVC = setting_or("msvc152", "");           // settings.json: msvc152 (Microsoft C/C++ 8.00c)

const py::str DOC(R"DOC(
cc16.py — compile 16-bit C with the PERIOD compiler (MSVC 1.52c) under DOSBox-X.

The foundation of byte-exact C matching (matching decompilation): to prove a
reconstructed C function is right, compile it with the SAME compiler+flags that
built retail Win98 and check the emitted bytes against the shipped bytes. That
requires the real compiler — Microsoft Visual C++ 1.52c, the Win16 (MS C8)
toolchain — which is a 16-bit DOS program (no NTVDM on x64), so it runs under the
vendored DOSBox-X, exactly like `masmlift`/`link16` drive MASM.

This module just RUNS the compiler: given C source (+ flags), it produces the
.OBJ and the `/Fc` assembly listing. Extracting the code bytes from the OBJ,
diffing against the target, and the permuter that searches flags/source for a
byte-match are the layers built on top (see docs / COMPILER_RE.md's codegen
oracle). First job: confirm MSVC 1.52c runs here and emits code.

    python cc16.py --selftest              # compile a trivial fn, show the asm
    python cc16.py foo.c --flags "/AM /Ox" # compile foo.c, dump obj + listing
)DOC");

Compiled compile_c(const py::str& csrc, const py::str& flags, const py::str& stem, double timeout) {
    // Compile csrc with MSVC 1.52c under DOSBox-X (build/dos_tools.py). Returns dict with obj
    // bytes, the /Fc listing text, and the raw compiler log; "work" is the folder it ran in (kept for
    // inspection; the caller cleans it up).
    if (!py::os::path::exists(DBX)) throw py::SystemExit(py::str("dosbox-x not found: ") + DBX);
    if (!py::os::path::exists(py::os::path::join(MSVC, py::str("BIN"), py::str("CL.EXE")))) {
        throw py::SystemExit(py::str("MSVC 1.52c CL.EXE not found under ") + MSVC);
    }
    // /Fc emits a mixed source+machine-code listing; /Fo the obj.
    const py::str cmd = py::percent(py::str("CL %s /Fc%s.COD /Fo%s.OBJ %s.C > %s.LOG 2>&1"), flags, stem, stem, stem, stem);
    dos_tools::Mounts mounts;
    mounts.append({py::str("c"), MSVC});
    py::list<py::str> bat;
    bat.append(cmd);
    py::list<py::str> artifacts;
    for (const char* ext : {".OBJ", ".COD", ".LOG"}) artifacts.append(stem + py::str(ext));
    py::dict<py::str, dos_tools::FileData> files;
    files.set(stem + py::str(".C"), dos_tools::FileData(csrc));
    py::dict<py::str, py::str> cpu;
    cpu.set(py::str("cputype"), py::str("386"));
    cpu.set(py::str("core"), py::str("normal"));
    cpu.set(py::str("cycles"), py::str("max"));
    py::dict<py::str, py::str> dos;
    dos.set(py::str("ver"), py::str("6.22"));
    py::dict<py::str, py::str> variables;
    variables.set(py::str("PATH"), py::str("C:\\BIN"));
    variables.set(py::str("INCLUDE"), py::str("C:\\INCLUDE"));
    variables.set(py::str("LIB"), py::str("C:\\LIB"));
    variables.set(py::str("TMP"), py::str("D:\\"));
    auto [ok, made, work] = dos_tools::run(mounts, bat, artifacts, timeout, files, cpu, dos, variables);
    (void)ok;
    Compiled out;
    out.work = work;
    for (const char* ext : {".OBJ", ".COD", ".LOG"}) {
        const py::str name = stem + py::str(ext);
        if (!made.contains(name)) continue;
        if (py::str(ext) == py::str(".OBJ")) out.obj = made.at(name);
        else if (py::str(ext) == py::str(".COD")) out.cod = made.at(name).decode("latin1");
        else out.log = made.at(name).decode("latin1");
    }
    return out;
}

namespace {

// ---- the argparse of this module's parser: csrc ('?'), --flags FLAGS, --selftest, -h/--help

struct ParserExit {};

std::string usage_line(const std::string& prog) { return "usage: " + prog + " [-h] [--flags FLAGS] [--selftest] [csrc]\n"; }

std::string help_text(const std::string& prog) {
    std::string doc = DOC.utf8();
    while (!doc.empty() && doc.front() == '\n') doc.erase(doc.begin());
    while (!doc.empty() && doc.back() == '\n') doc.pop_back();
    return usage_line(prog) + "\n" + doc + "\n\n" +
           "positional arguments:\n"
           "  csrc           C source file\n"
           "\n"
           "options:\n"
           "  -h, --help     show this help message and exit\n"
           "  --flags FLAGS\n"
           "  --selftest\n";
}

[[noreturn]] void fail(const std::string& prog, std::ostream& err, const std::string& message) {
    err << usage_line(prog) << prog << ": error: " << message << "\n";
    throw py::SystemExit(2);
}

bool is_negative_number(const std::string& s) {
    // '^-\d+$|^-\d*\.\d+$'
    if (s.size() < 2 || s[0] != '-') return false;
    size_t i = 1;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') i++;
    if (i == s.size()) return true;
    if (s[i] != '.') return false;
    i++;
    size_t digits = 0;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
        i++;
        digits++;
    }
    return i == s.size() && digits > 0;
}

enum class Kind { Positional, Help, Flags, Selftest, Unknown };

struct Classified {
    Kind kind = Kind::Positional;
    std::optional<std::string> explicit_arg;
};

// argparse's _parse_optional for this parser
Classified classify(const std::string& arg) {
    Classified c;
    if (arg.empty() || arg[0] != '-') return c;
    if (arg == "-") return c;
    if (arg == "-h" || arg == "--help") return Classified{Kind::Help, std::nullopt};
    if (arg == "--flags") return Classified{Kind::Flags, std::nullopt};
    if (arg == "--selftest") return Classified{Kind::Selftest, std::nullopt};
    std::string name = arg;
    std::optional<std::string> value;
    const size_t eq = arg.find('=');
    if (eq != std::string::npos) {
        name = arg.substr(0, eq);
        value = arg.substr(eq + 1);
        if (name == "-h" || name == "--help") return Classified{Kind::Help, value};
        if (name == "--flags") return Classified{Kind::Flags, value};
        if (name == "--selftest") return Classified{Kind::Selftest, value};
    }
    if (arg.size() > 1 && arg[1] == '-') {   // an unambiguous prefix of a long option
        const std::string prefix = name;
        const bool help = std::string("--help").compare(0, prefix.size(), prefix) == 0;
        const bool flags = std::string("--flags").compare(0, prefix.size(), prefix) == 0;
        const bool selftest = std::string("--selftest").compare(0, prefix.size(), prefix) == 0;
        if (prefix.size() > 2 && (help ? 1 : 0) + (flags ? 1 : 0) + (selftest ? 1 : 0) == 1) {
            return Classified{help ? Kind::Help : (flags ? Kind::Flags : Kind::Selftest), value};
        }
    } else if (arg[1] == 'h') {   // -h with something attached
        return Classified{Kind::Help, std::nullopt};
    }
    if (is_negative_number(arg)) return c;
    if (arg.find(' ') != std::string::npos) return c;
    return Classified{Kind::Unknown, std::nullopt};
}

}  // namespace

int64_t main(const py::list<py::str>& argv, std::ostream& out, std::ostream& err) {
    // the program's name: os.path.basename(sys.argv[0])
    const std::string prog = py::os::path::basename(argv.size() ? argv[0] : py::str("")).utf8();
    std::vector<std::string> args;
    for (int64_t i = 1; i < argv.size(); i++) args.push_back(argv[i].utf8());

    std::optional<std::string> csrc;
    std::string flags = "/AM /c";
    bool selftest = false;
    std::vector<std::string> extras;
    bool after_dd = false;
    for (size_t i = 0; i < args.size(); i++) {
        const std::string& tok = args[i];
        if (!after_dd && tok == "--") {
            after_dd = true;
            continue;
        }
        const Classified c = after_dd ? Classified() : classify(tok);
        switch (c.kind) {
            case Kind::Positional:
                if (!csrc) csrc = tok;
                else extras.push_back(tok);
                break;
            case Kind::Unknown:
                extras.push_back(tok);
                break;
            case Kind::Help:
                if (c.explicit_arg) fail(prog, err, "argument -h/--help: ignored explicit argument '" + *c.explicit_arg + "'");
                out << help_text(prog);
                throw py::SystemExit(0);
            case Kind::Selftest:
                if (c.explicit_arg) fail(prog, err, "argument --selftest: ignored explicit argument '" + *c.explicit_arg + "'");
                selftest = true;
                break;
            case Kind::Flags:
                if (c.explicit_arg) {
                    flags = *c.explicit_arg;
                } else if (i + 1 < args.size() && (args[i + 1] != "--") && classify(args[i + 1]).kind == Kind::Positional) {
                    flags = args[++i];
                } else {
                    fail(prog, err, "argument --flags: expected one argument");
                }
                break;
        }
    }
    if (!extras.empty()) {
        std::string joined;
        for (size_t i = 0; i < extras.size(); i++) joined += (i ? " " : "") + extras[i];
        fail(prog, err, "unrecognized arguments: " + joined);
    }

    Compiled r;
    if (selftest) {
        const py::str src("int add2(int a, int b) { return a + b; }\nlong mulshift(long x) { return (x * 3) >> 2; }\n");
        r = compile_c(src, py::str(flags));
    } else {
        if (!csrc) throw py::SystemExit(py::str("give a C file or --selftest"));
        py::file f = py::file::open(py::str(*csrc), py::str("r"));
        const py::str text = f.read_text();
        f.close();
        r = compile_c(text, py::str(flags));
    }

    if (r.cod) {
        out << "=== /Fc listing (codegen) ===\n";
        out << r.cod->slice(0, 3000).utf8() << "\n";
    }
    if (r.obj) {
        out << "=== OBJ: " << r.obj->size() << " bytes ===\n";
    } else {
        out << "=== NO OBJ produced — compiler log: ===\n";
        out << (r.log ? *r.log : py::str("(no log)")).slice(0, 2000).utf8() << "\n";
    }
    out << "# work dir: " << (r.work ? r.work->utf8() : std::string("None")) << "\n";
    return 0;
}

}  // namespace codebreaker::build::msvc152
