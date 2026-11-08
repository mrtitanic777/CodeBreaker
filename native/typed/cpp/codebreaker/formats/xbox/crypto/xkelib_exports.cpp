// codebreaker/formats/xbox/crypto/xkelib_exports.cpp -- typed C++ of codebreaker.formats.xbox.crypto.xkelib_exports.
//
// (the Python: a script that creates .def files from Xbox 360 libraries using dumpbin in the SDK)

#include "codebreaker/formats/xbox/crypto/xkelib_exports.hpp"

#include <cstdio>
#include <cstdlib>
#include <tuple>

#ifdef _WIN32
#include <process.h>
#endif

namespace codebreaker::formats::xbox::crypto::xkelib_exports {

const py::str SDK_VER("11775.3");

py::str& SDK_DIR() {
    // join(settings.get("xbox360_sdk") or "C:\\Program Files (x86)\\Microsoft Xbox 360 SDK", "bin", "win32")
    static py::str dir = [] {
        py::str sdk = py::settings::get_str(py::str("xbox360_sdk"));
        if (sdk.empty()) {
            sdk = py::str("C:\\Program Files (x86)\\Microsoft Xbox 360 SDK");
        }
        return py::os::path::join(sdk, py::str("bin"), py::str("win32"));
    }();
    return dir;
}

const py::re::Pattern HEADER_RE = py::re::compile(py::str(R"(DLL\sname\s*:\s*([\w\W]{0,}?)\r\n)"));
const py::re::Pattern EXPORT_RE = py::re::compile(py::str(R"(\s+(\d+)\s+([\w\W]{0,}?)\r\n)"));

namespace {

// subprocess.list2cmdline: a list of arguments as one Windows command line.
py::str list2cmdline(const py::list<py::str>& seq) {
    std::u32string result;
    for (const py::str& arg : seq) {
        std::u32string bs_buf;
        if (!result.empty()) {
            result += U' ';
        }
        const std::u32string& a = arg.u32();
        bool needquote = a.find(U' ') != std::u32string::npos || a.find(U'\t') != std::u32string::npos || a.empty();
        if (needquote) {
            result += U'"';
        }
        for (char32_t c : a) {
            if (c == U'\\') {
                bs_buf += c;
            } else if (c == U'"') {
                result += std::u32string(bs_buf.size() * 2, U'\\');
                bs_buf.clear();
                result += U"\\\"";
            } else {
                if (!bs_buf.empty()) {
                    result += bs_buf;
                    bs_buf.clear();
                }
                result += c;
            }
        }
        if (!bs_buf.empty()) {
            result += bs_buf;
        }
        if (needquote) {
            result += bs_buf;
            result += U'"';
        }
    }
    return py::str(result);
}

// UTF-16 of a str, for the Windows API
std::wstring wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            char32_t v = c - 0x10000;
            w += static_cast<wchar_t>(0xD800 + (v >> 10));
            w += static_cast<wchar_t>(0xDC00 + (v & 0x3FF));
        } else {
            w += static_cast<wchar_t>(c);
        }
    }
    return w;
}

// subprocess.Popen(args, stdout=PIPE, shell=True) then communicate() and wait(): -> (stdout bytes, exit code).
// The shell is %COMSPEC% (cmd.exe) /c "<the command line>", as Python's; stderr and stdin are inherited.
std::tuple<py::bytes, int64_t> run_shell(const py::list<py::str>& args) {
    py::str cmd = py::str("\"") + list2cmdline(args) + py::str("\"");
    std::FILE* pipe = _wpopen(wide(cmd).c_str(), L"rb");
    if (!pipe) {
        throw py::OSError("cannot start the shell");
    }
    std::string out;
    char buf[65536];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, pipe)) > 0) {
        out.append(buf, n);
    }
    int rc = _pclose(pipe);
    return {py::bytes(out), static_cast<int64_t>(rc)};
}

}  // namespace

py::str dumpbin(const py::str& command, const py::str& filename) {
    py::list<py::str> argv;
    argv.append(py::os::path::join(SDK_DIR(), py::str("dumpbin.exe")));
    argv.append(py::str("/") + command.upper());
    argv.append(py::os::path::abspath(filename));
    auto [output, rc] = run_shell(argv);
    if (rc != 0) {
        py::print(py::str("Error!"));
    }
    return output.decode("utf8");
}

std::optional<py::str> fetch_version(const py::str& s) {
    std::optional<py::re::Match> matches = HEADER_RE.search(s);
    if (matches) {
        return matches->group(1);
    }
    return std::nullopt;
}

void create_def(const py::str& ifile, const py::str& ofile, bool verbose) {
    if (!py::os::path::isfile(ifile)) {
        throw py::AssertionError("Input file doesn't exist!");
    }

    py::str res = dumpbin(py::str("headers"), ifile);
    std::optional<py::str> lib_ver = fetch_version(res);
    if (!(lib_ver && !lib_ver->empty())) {
        throw py::AssertionError("Error locating library version!");
    }
    res = dumpbin(py::str("exports"), ifile);
    py::file f = py::open(ofile, py::str("wb"));
    if (verbose) {
        py::print(py::str("LIBRARY ") + *lib_ver);
    }
    f.write((py::str("LIBRARY ") + *lib_ver + py::str("\r\n")).encode("ascii"));
    if (verbose) {
        py::print(py::str("EXPORTS"));
    }
    f.write(py::str("EXPORTS\r\n").encode("ascii"));

    // if debug: print("Parsing dumpbin response...")
    py::list<std::tuple<int64_t, py::str>> clean;
    // The Python calls EXPORT_RE.findall(res, re.MULTILINE): the second argument of Pattern.findall is
    // `pos`, so re.MULTILINE (8) makes the search begin at the 9th character. (This pattern has no ^,
    // lookbehind or \b, so searching res[8:] finds exactly the same matches.)
    for (const auto& [ordinal, func] : EXPORT_RE.findall_groups<py::str, py::str>(res.slice_from(8))) {
        if (!func.startswith(py::str("."))) {
            clean.append(std::make_tuple(py::int_from_str(ordinal), func));
        }
    }

    // if debug: print("Sorting results by ordinal...")
    clean.sort([](const std::tuple<int64_t, py::str>& i) { return std::get<0>(i); });
    for (const auto& [ordinal, func] : clean) {
        if (verbose) {
            py::print(py::str("\t") + func + py::str(" @") + py::to_str(ordinal));
        }
        f.write((py::str("\t") + func + py::str(" @") + py::to_str(ordinal) + py::str("\r\n")).encode("ascii"));
    }
}

namespace {

// ---- argparse, for this one parser: positionals ifile and ofile, options -h/--help and -v/--verbose ----

const char* const HELP_TEXT =
    "\n"
    "A script to create .def files from Xbox 360 libraries using dumpbin in the SDK\n"
    "\n"
    "positional arguments:\n"
    "  ifile          The library you want to dump exports from\n"
    "  ofile          The file you want to write the definitions to\n"
    "\n"
    "options:\n"
    "  -h, --help     show this help message and exit\n"
    "  -v, --verbose  Print verbose information out\n";

void write_to(std::FILE* stream, const py::str& text) {
    std::string u = text.utf8();
    std::fwrite(u.data(), 1, u.size(), stream);
    std::fflush(stream);
}

py::str usage(const py::str& prog) { return py::str("usage: ") + prog + py::str(" [-h] [-v] ifile ofile\n"); }

// parser.error(message): the usage and the message on stderr, exit code 2
[[noreturn]] void fail(const py::str& prog, const py::str& message) {
    write_to(stderr, usage(prog));
    write_to(stderr, prog + py::str(": error: ") + message + py::str("\n"));
    throw SystemExit(2);
}

[[noreturn]] void print_help(const py::str& prog) {
    write_to(stdout, usage(prog) + py::str(HELP_TEXT));
    throw SystemExit(0);
}

bool looks_negative_number(const py::str& s) {   // argparse's '^-\d+$|^-\d*\.\d+$'
    return py::re::Pattern(py::str(R"(^-\d+$|^-\d*\.\d+$)"), 0).match(s).has_value();
}

enum class Flag { Help, Verbose };

struct Parsed {
    py::list<py::str> positionals;
    bool verbose = false;
};

}  // namespace

void main(const py::list<py::str>& args, const py::str& prog) {
    // parser.parse_args(): the options act in the order met, the positionals are matched at the end
    Parsed parsed;
    py::list<py::str> extras;
    bool only_positional = false;
    auto act = [&](Flag flag) {
        if (flag == Flag::Help) {
            print_help(prog);
        }
        parsed.verbose = true;
    };
    for (const py::str& arg : args) {
        if (only_positional) {
            parsed.positionals.append(arg);
            continue;
        }
        if (arg == py::str("--")) {
            only_positional = true;
            continue;
        }
        const std::u32string& a = arg.u32();
        if (a.size() < 2 || a[0] != U'-') {   // not an option (a lone "-" is a positional)
            parsed.positionals.append(arg);
            continue;
        }
        if (a[1] == U'-') {   // --long[=value], or an unambiguous prefix of one
            size_t eq = a.find(U'=');
            std::u32string name = eq == std::u32string::npos ? a : a.substr(0, eq);
            std::optional<std::u32string> explicit_arg;
            if (eq != std::u32string::npos) {
                explicit_arg = a.substr(eq + 1);
            }
            std::optional<Flag> flag;
            py::str shown;
            if (name == U"--help" || (name.size() > 2 && std::u32string(U"--help").compare(0, name.size(), name) == 0)) {
                flag = Flag::Help;
                shown = py::str("-h/--help");
            } else if (name == U"--verbose" || (name.size() > 2 && std::u32string(U"--verbose").compare(0, name.size(), name) == 0)) {
                flag = Flag::Verbose;
                shown = py::str("-v/--verbose");
            }
            if (!flag) {
                if (arg.u32().find(U' ') != std::u32string::npos) {
                    parsed.positionals.append(arg);
                } else {
                    extras.append(arg);
                }
                continue;
            }
            if (explicit_arg) {
                fail(prog, py::str("argument ") + shown + py::str(": ignored explicit argument ") + py::repr(py::str(*explicit_arg)));
            }
            act(*flag);
            continue;
        }
        // -x, or a cluster of short flags (-vh), possibly with =value
        if (looks_negative_number(arg)) {
            parsed.positionals.append(arg);
            continue;
        }
        std::u32string rest = a.substr(1);
        char32_t first = rest[0];
        if (first != U'h' && first != U'v') {
            if (arg.u32().find(U' ') != std::u32string::npos) {
                parsed.positionals.append(arg);
            } else {
                extras.append(arg);
            }
            continue;
        }
        // argparse's consume_optional: the flags of a cluster (-vh) are taken one by one; after "=", or
        // before a "-", a tail is an explicit argument the flag does not take (an error); a tail that
        // starts with another character that is no flag is left as an unrecognized "-tail".
        std::vector<Flag> actions;
        std::optional<std::u32string> explicit_arg;
        bool sep = false;
        if (a.size() > 2) {
            if (a[2] == U'=') {   // "-v=1"
                explicit_arg = a.substr(3);
                sep = true;
            } else {              // "-vx": the tail
                explicit_arg = a.substr(2);
            }
        }
        char32_t current = first;
        for (;;) {
            Flag flag = current == U'h' ? Flag::Help : Flag::Verbose;
            py::str shown = flag == Flag::Help ? py::str("-h/--help") : py::str("-v/--verbose");
            if (!explicit_arg) {
                actions.push_back(flag);
                break;
            }
            if (explicit_arg->empty()) {   // "-v=": an empty explicit argument
                fail(prog, py::str("argument ") + shown + py::str(": ignored explicit argument ") + py::repr(py::str(*explicit_arg)));
            }
            if (sep || (*explicit_arg)[0] == U'-') {
                fail(prog, py::str("argument ") + shown + py::str(": ignored explicit argument ") + py::repr(py::str(*explicit_arg)));
            }
            actions.push_back(flag);
            char32_t next = (*explicit_arg)[0];
            if (next != U'h' && next != U'v') {
                extras.append(py::str(std::u32string(U"-") + *explicit_arg));
                break;
            }
            current = next;
            explicit_arg = explicit_arg->substr(1);
            if (explicit_arg->empty()) {
                explicit_arg.reset();
            } else if ((*explicit_arg)[0] == U'=') {
                sep = true;
                explicit_arg = explicit_arg->substr(1);
            } else {
                sep = false;
            }
        }
        for (Flag flag : actions) {
            act(flag);
        }
    }

    // the positionals ifile, ofile; the rest are unrecognized
    py::list<py::str> names;
    names.append(py::str("ifile"));
    names.append(py::str("ofile"));
    if (py::len(parsed.positionals) < 2) {
        py::list<py::str> missing;
        for (int64_t i = py::len(parsed.positionals); i < 2; ++i) {
            missing.append(names[i]);
        }
        fail(prog, py::str("the following arguments are required: ") + py::join(py::str(", "), missing));
    }
    for (int64_t i = 2; i < py::len(parsed.positionals); ++i) {
        extras.append(parsed.positionals[i]);
    }
    if (!extras.empty()) {
        fail(prog, py::str("unrecognized arguments: ") + py::join(py::str(" "), extras));
    }
    const py::str& ifile = parsed.positionals[0];
    const py::str& ofile = parsed.positionals[1];

    if (!py::os::path::isdir(SDK_DIR())) {
        throw py::AssertionError("The Xbox 360 SDK doesn't appear to be installed");
    }
    if (!py::os::path::isfile(ifile)) {
        throw py::AssertionError("The input file doesn't exist");
    }

    create_def(ifile, ofile, parsed.verbose);
}

}  // namespace codebreaker::formats::xbox::crypto::xkelib_exports
