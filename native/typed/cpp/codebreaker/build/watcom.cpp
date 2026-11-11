// codebreaker/build/watcom.cpp -- typed C++ of codebreaker.build.watcom (see watcom.hpp).

#include "codebreaker/build/watcom.hpp"

#include <mutex>

#include <cstdlib>
#include <filesystem>
#include <string>

#include "codebreaker/formats/omf.hpp"
#include "codebreaker/formats/pe.hpp"

namespace codebreaker::build::watcom {

namespace omf = codebreaker::formats::omf;
namespace pe = codebreaker::formats::pe;

const py::str _BASE = py::str("option offset=0x6E000000\n");

namespace {

bool lower_ends_with_exe(const py::str& name) {
    std::string s = name.lower().utf8();
    return s.size() >= 4 && s.compare(s.size() - 4, 4, ".exe") == 0;
}

}  // namespace

OW _ow() {
    OW ow;
    ow.root = openwatcom_root();
    ow.wcc = ow_cc(py::str("pe"));
    ow.wlink = ow_tool(py::str("wlink"));
    ow.inc = ow_include(py::str("pe"));
    return ow;
}

py::str _scratch(const py::list<py::str>& parts) {
    // import tempfile
    py::str path = py::os::path::join(py::tempfile::gettempdir(), py::str("codebreaker"), py::str("bodyverify"));
    for (const py::str& p : parts.vec()) path = py::os::path::join(path, p);
    return path;
}

py::dict<py::str, py::str> _ow_env(const OW&) { return ow_env(py::str("pe")); }

py::dict<py::str, py::dict<py::str, py::str>>& _IMPORT_MAPS() {
    static py::dict<py::str, py::dict<py::str, py::str>> maps;
    return maps;
}

py::dict<py::str, py::str>& _EXPORT_NAMES() {
    static py::dict<py::str, py::str> names;
    return names;
}

std::recursive_mutex& import_maps_lock() {
    static std::recursive_mutex m;
    return m;
}

py::dict<py::str, py::str> import_map(const py::str& module) {
    std::lock_guard<std::recursive_mutex> hold(import_maps_lock());   // (compiles run on threads: the two tables below are shared)
    if (!_IMPORT_MAPS().contains(module)) {
        py::dict<py::str, py::str> out;
        try {
            pe::PEFile file = pe::open_pe(module);
            auto imports = file.imports();
            for (const auto& kv : imports.items()) {
                const py::str& dll = std::get<0>(std::get<1>(kv));
                const py::str& name = std::get<1>(std::get<1>(kv));
                std::optional<py::str> c;
                if (!name.u32().empty() && py::re::fullmatch(py::str("[A-Za-z_]\\w*"), name))
                    c = name;
                else if (!name.u32().empty() && py::re::fullmatch(py::str("[A-Za-z_]\\w*@\\d+|\\?[\\w@$?]+"), name))
                    c = py::re::sub(py::str("\\W"), py::str("_"), name);
                if (c) {
                    out.setdefault(*c, std::get<0>(py::os::path::splitext(dll)).upper());
                    if (*c != name) _EXPORT_NAMES().set(*c, name);
                }
            }
        } catch (const py::Exception&) {
        }
        _IMPORT_MAPS().set(module, out);
    }
    return _IMPORT_MAPS().at(module);
}

py::str _import_lines(const py::bytes& obj, const py::set<py::str>& names, const py::dict<py::str, py::str>& imports,
                      const std::optional<Numbered>& numbered) {
    py::list<py::str> ext = omf::externs(obj);
    std::string out;
    py::list<py::str> ordered = py::sorted(names);
    for (const py::str& n : ordered.vec()) {
        for (const py::str& sym : ext.vec()) {
            // the decoration adds ONE underscore: `__lread@12` is `_lread`, not `lread`
            std::string s = sym.utf8();
            std::string undecorated = (!s.empty() && s[0] == '_') ? s.substr(1) : s;
            undecorated = undecorated.substr(0, undecorated.find('@'));
            if (py::str(undecorated) == n) {
                if (numbered && !numbered->empty() && numbered->contains(n)) {
                    const auto& num = numbered->at(n);
                    out += "import '" + s + "' '" + std::get<0>(num).utf8() + "'." + std::to_string(std::get<1>(num)) + "\n";
                } else if (imports.contains(n)) {
                    out += "import '" + s + "' '" + imports.at(n).utf8() + "'.'" + _EXPORT_NAMES().get(n, n).utf8() + "'\n";
                }
                break;
            }
        }
    }
    return py::str(out);
}

py::str _first(const py::list<py::str>& cands) {
    for (const py::str& c : cands.vec())
        if (!c.u32().empty() && py::os::path::exists(c)) return c;
    return py::str("");
}

py::str openwatcom_root() {
    std::optional<py::str> env = py::os::environ_::get(py::str("CB_OPENWATCOM"));
    if (env && !env->u32().empty()) return *env;                  // os.environ.get(...) or settings.get(...)
    return py::settings::get_str(py::str("openwatcom"));
}

py::str ow_tool(const py::str& name_) {
    py::str name = name_;
    if (!lower_ends_with_exe(name)) name = name + py::str(".exe");
    return _first(py::list<py::str>{py::os::path::join(openwatcom_root(), py::str("binnt64"), name)});
}

py::str ow_cc(const py::str& cls) { return ow_tool(py::str(cls == py::str("ne") ? "wcc" : "wcc386")); }

py::list<py::str> ow_include(const py::str& cls) {
    py::str h = py::os::path::join(openwatcom_root(), py::str("h"));
    return py::list<py::str>{h, py::os::path::join(h, py::str(cls == py::str("ne") ? "win" : "nt"))};
}

py::str ow_lib(const py::str& cls) {
    py::str root = openwatcom_root();
    return cls == py::str("ne") ? py::os::path::join(root, py::str("lib286"), py::str("win"))
                                : py::os::path::join(root, py::str("lib386"), py::str("nt"));
}

py::dict<py::str, py::str> ow_env(const py::str& cls) {
    py::str root = openwatcom_root();
    py::dict<py::str, py::str> env = py::os::environ_::copy();   // dict(os.environ)
    env.set(py::str("NoDefaultCurrentDirectoryInExePath"), py::str(""));       // OW runs its own tools from cwd
    env.set(py::str("WATCOM"), root);
    env.set(py::str("INCLUDE"), py::join(py::os::pathsep, ow_include(cls)));
    env.set(py::str("WLINK_LNK"), py::os::path::join(root, py::str("binnt64"), py::str("wlink.lnk")));
    env.set(py::str("PATH"), py::os::path::join(root, py::str("binnt64")) + py::os::pathsep + env.get(py::str("PATH"), py::str("")));
    return env;
}

}  // namespace codebreaker::build::watcom
