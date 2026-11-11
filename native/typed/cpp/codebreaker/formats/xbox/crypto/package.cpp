// codebreaker/formats/xbox/crypto/package.cpp -- typed C++ port of codebreaker/formats/xbox/crypto/__init__.py (see package.hpp).
#include "codebreaker/formats/xbox/crypto/package.hpp"

#include <map>

#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::formats::xbox::crypto {

using py::str;

// the package docstring, as `codebreaker.formats.xbox.crypto.__doc__` has it
const str DOC = str(std::string(R"CBDOC(crypto -- Xbox 360 console crypto: XeCrypt, keyvaults, the boot chain, NAND images, shadowboots.

Most of this package is Xbox-360-Crypto by GoobyCorp (BSD 3-Clause; see LICENSE and ATTRIBUTION.md),
kept as its authors wrote it. Each tool runs as

    python -m codebreaker crypto <tool> [arguments]      (python -m codebreaker crypto: the list)

and takes its own --help. What the tools read that is not part of the package comes from settings:
`xbox360_keys` (the key store folder: 1BL_pub.bin, SB_prv.bin, ...; default Keys/ in the current
folder) and `xbox360_crypto_bin` (upstream's bin/ folder: the LZX libraries, the Free60 xenon-as and
xenon-objcopy, hypervisor images). Neither is in the repository.
)CBDOC"));

namespace {

str& here_() {
    static str here = py::os::path::join(py::settings::checkout_root(), str("codebreaker"), str("formats"), str("xbox"), str("crypto"));
    return here;
}

std::map<std::string, Tool>& registry_() {
    static std::map<std::string, Tool> r;
    return r;
}

}  // namespace

str _HERE() { return here_(); }
void set_here(const str& here) { here_() = here; }

py::list<str> tools() {
    py::list<str> out;
    for (const str& f : py::sorted(py::os::listdir(_HERE()))) {
        if (f.endswith(str(".py")) && !f.startswith(str("_"))) {
            py::bytes raw = py::open_rb(py::os::path::join(_HERE(), f)).read();
            str text = raw.decode("utf-8", "replace");
            if (text.contains(str("\ndef main("))) out.append(f.slice(0, f.size() - 3));
        }
    }
    return out;
}

void register_tool(const str& name, const Tool& tool) { registry_()[name.utf8()] = tool; }

int64_t main(const py::list<str>& argv) {
    py::list<str> names = tools();
    const bool asks_help = py::len(argv) == 0 || argv[0] == str("-h") || argv[0] == str("--help") || argv[0] == str("help");
    if (asks_help || !names.contains(argv[0])) {
        py::print(DOC);
        py::print(str("tools: ") + py::join(str(" "), names));
        return asks_help ? 0 : 2;
    }
    auto it = registry_().find(argv[0].utf8());
    if (it == registry_().end()) throw py::ModuleNotFoundError("No module named 'codebreaker.formats.xbox.crypto." + argv[0].utf8() + "'");
    py::list<str> sys_argv;                       // sys.argv = [argv[0]] + list(argv[1:])
    sys_argv.append(argv[0]);
    for (int64_t i = 1; i < py::len(argv); i++) sys_argv.append(argv[i]);
    std::optional<int64_t> result;
    try {
        result = it->second(sys_argv);
    } catch (const py::SystemExit& e) {
        if (const auto* c = std::get_if<int64_t>(&e.code)) result = *c;   // result = e.code (None / a message is not an int)
    }
    return result ? *result : 0;
}

}  // namespace codebreaker::formats::xbox::crypto
