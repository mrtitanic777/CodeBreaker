// codebreaker/translate/frontends/clang/package.cpp -- typed C++ port of codebreaker/translate/frontends/clang/__init__.py
// (see package.hpp).
#include "codebreaker/translate/frontends/clang/package.hpp"

#include <fstream>

#include "codebreaker/translate/dotnet/package.hpp"
#include "codebreaker/translate/frontends/clang/lower.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::translate::frontends::clang {

using py::str;

const str TRIPLE = "x86_64-pc-windows-msvc";
const int64_t REV = 1;
const py::list<str> _VS_LLVM{str("C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\VC\\Tools\\Llvm\\x64\\bin\\clang.exe"),
                             str("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\Llvm\\x64\\bin\\clang.exe")};
const int64_t TIMEOUT = 300;

namespace {
str& source_dir_() {
    static str d = py::os::path::join(py::settings::checkout_root(), str("codebreaker"), str("translate"), str("frontends"), str("clang"));
    return d;
}
}  // namespace

str _SOURCE_DIR() { return source_dir_(); }
void set_source_dir(const str& dir) { source_dir_() = dir; }

std::optional<str> clang() {
    try {
        auto got = py::settings::get_opt_str(str("clang"));
        if (got && !got->empty() && py::os::path::isfile(*got)) return got;
    } catch (const py::Exception&) {
        // (except Exception: pass)
    }
    for (const str& p : _VS_LLVM)
        if (py::os::path::isfile(p)) return p;
    return py::shutil::which(str("clang"));
}

str language_of(const str& path) { return path.lower().endswith(str(".c")) ? str("c") : str("c++"); }

py::list<str> flags(const str& language, const std::optional<str>& std_, const py::list<str>& includes, const py::list<str>& quote_includes,
                    const py::list<str>& defines) {
    py::list<str> out;
    out.append(str("--target=") + TRIPLE);
    if (language == str("c")) {
        out.append(str("-x"));
        out.append(str("c"));
        out.append(str("-std=") + (std_ && !std_->empty() ? *std_ : str("c11")));
    } else {
        out.append(str("-x"));
        out.append(str("c++"));
        out.append(str("-std=") + (std_ && !std_->empty() ? *std_ : str("c++17")));
    }
    for (const char* a : {"-Wno-everything", "-D_CRT_SECURE_NO_WARNINGS", "-D_CRT_NONSTDC_NO_WARNINGS"}) out.append(str(a));
    for (const str& d : includes) {
        out.append(str("-I"));
        out.append(d);
    }
    for (const str& d : quote_includes) {
        out.append(str("-iquote"));
        out.append(d);
    }
    for (const str& d : defines) out.append(str("-D") + d);
    return out;
}

ir::Dict dump(const str& path, const std::optional<str>& language, const std::optional<str>& std_, const py::list<str>& includes,
              const py::list<str>& quote_includes, const py::list<str>& defines) {
    auto exe = clang();
    if (!exe) throw py::RuntimeError("the C/C++ front end needs clang (settings.json: clang)");
    str lang = (language && !language->empty()) ? *language : language_of(path);
    py::list<str> cmd;
    cmd.append(*exe);
    for (const str& f : flags(lang, std_, includes, quote_includes, defines)) cmd.append(f);
    for (const char* a : {"-Xclang", "-ast-dump=json", "-fsyntax-only"}) cmd.append(str(a));
    cmd.append(path);
    dotnet::Process r = dotnet::run_process(cmd, std::nullopt, std::nullopt, static_cast<double>(TIMEOUT), false);
    if (r.returncode != 0) {
        str err = r.stderr_.decode("utf-8", "replace");
        throw py::RuntimeError("clang: " + (err.size() > 1500 ? err.slice(err.size() - 1500, err.size()) : err).utf8());   // [-1500:]
    }
    return lower::load_tree(r.stdout_.decode("utf-8", "replace"));
}

py::ref<ir::Unit> lower_file(const str& path, const std::optional<str>& language, const std::optional<str>& std_, const py::list<str>& includes,
                             const py::list<str>& quote_includes, const py::list<str>& defines, const std::optional<py::list<str>>& own) {
    str lang = (language && !language->empty()) ? *language : language_of(path);
    ir::Dict tree = dump(path, lang, std_, includes, quote_includes, defines);
    lower::Lowering lw(tree, path, own, lang, std_, TRIPLE);
    return lw.unit();
}

static str repr_list_(const py::list<str>& l) {   // repr(list(...))
    std::string o = "[";
    for (int64_t i = 0; i < l.size(); i++) o += (i ? ", " : "") + py::repr(l[i]).utf8();
    return str(o + "]");
}

str _key(const str& path, const str& language, const std::optional<str>& std_, const py::list<str>& includes, const py::list<str>& quote_includes,
         const py::list<str>& defines, const std::optional<py::list<str>>& own) {
    py::hashlib::Hash h = py::hashlib::sha1();
    // repr((REV, language, std, list(includes), list(quote_includes), list(defines), TRIPLE))
    str head = py::percent(str("(%d, %s, %s, %s, %s, %s, %s)"), REV, py::repr(language), std_ ? py::repr(*std_) : str("None"), repr_list_(includes),
                           repr_list_(quote_includes), repr_list_(defines), py::repr(TRIPLE));
    h.update(head.encode("utf-8"));
    str here = py::os::path::dirname(py::os::path::abspath(path));
    py::list<str> files;
    files.append(path);
    if (own)
        for (const str& f : *own) files.append(f);
    for (const str& f : py::sorted(py::os::listdir(here))) {
        str lo = f.lower();
        if (lo.endswith(str(".h")) || lo.endswith(str(".hpp")) || lo.endswith(str(".hh"))) files.append(py::os::path::join(here, f));
    }
    for (const str& f : files) {
        try {
            py::file fh = py::open_rb(f);
            h.update(fh.read());
        } catch (const py::OSError&) {
        }
    }
    for (const str& src : {py::os::path::join(_SOURCE_DIR(), str("lower.py")), py::os::path::join(_SOURCE_DIR(), str("ctypes.py"))}) {
        py::file fh = py::open_rb(src);
        h.update(fh.read());
    }
    auto exe = clang();
    h.update(str(exe ? py::repr(*exe) : str("None")).encode("utf-8"));   // repr(clang())
    return h.hexdigest().slice(0, 16);
}

// ---- json.dumps ---------------------------------------------------------------------------------------------------------
namespace {
void dump_val_(std::string& o, const ir::Val& v) {
    if (std::holds_alternative<std::monostate>(v.v)) o += "null";
    else if (const bool* b = std::get_if<bool>(&v.v)) o += *b ? "true" : "false";
    else if (const int64_t* i = std::get_if<int64_t>(&v.v)) o += std::to_string(*i);
    else if (const double* d = std::get_if<double>(&v.v)) py::json::put(o, *d);
    else if (const py::str* s = std::get_if<py::str>(&v.v)) py::json::put(o, *s);
    else if (const auto* l = std::get_if<ir::Val::List>(&v.v)) {
        o += "[";
        bool first = true;
        for (const ir::Val& x : *l) {
            if (!first) o += ", ";
            first = false;
            dump_val_(o, x);
        }
        o += "]";
    } else if (const auto* d = std::get_if<ir::Val::Dict>(&v.v)) {
        o += "{";
        bool first = true;
        for (const auto& kv : d->items()) {
            if (!first) o += ", ";
            first = false;
            py::json::put(o, std::get<0>(kv));
            o += ": ";
            dump_val_(o, std::get<1>(kv));
        }
        o += "}";
    } else {
        throw py::TypeError("Object of type Type is not JSON serializable");
    }
}
}  // namespace

str json_dumps(const ir::Val& value) {
    std::string o;
    dump_val_(o, value);
    return str(o);
}

ir::Dict l2_file(const str& path, const std::optional<str>& language, const std::optional<str>& std_, const py::list<str>& includes,
                 const py::list<str>& quote_includes, const py::list<str>& defines, const std::optional<py::list<str>>& own, bool cache) {
    str lang = (language && !language->empty()) ? *language : language_of(path);
    str key = _key(path, lang, std_, includes, quote_includes, defines, own);
    std::optional<str> where;
    if (cache) {
        try {
            // settings.state('translate', 'clang', '%s.%s.json' % (basename(path), key)): a path in the state folder, its folder made
            str name = py::percent(str("%s.%s.json"), py::os::path::basename(path), key);
            str p = py::os::path::join(py::settings::get_str(str("state")), str("translate"), str("clang"), name);
            py::os::makedirs(py::os::path::dirname(p), 0777, true);
            where = p;
        } catch (const py::Exception&) {
            where = std::nullopt;
        }
        if (where && py::os::path::isfile(*where)) {
            py::file f = py::open_rb(*where);
            return lower::load_tree(f.read().decode("utf-8"));
        }
    }
    ir::Dict doc = l2::to_json(lower_file(path, lang, std_, includes, quote_includes, defines, own));
    if (where) {
        py::os::makedirs(py::os::path::dirname(*where), 0777, true);
        std::ofstream f(std::filesystem::u8path(where->utf8()), std::ios::binary);
        f << json_dumps(ir::Val(doc)).utf8();
    }
    return doc;
}

}  // namespace codebreaker::translate::frontends::clang
