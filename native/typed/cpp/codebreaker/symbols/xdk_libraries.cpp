// codebreaker/symbols/xdk_libraries.cpp -- the shim's translation unit (see xdk_libraries.hpp): the old format readers
// and the knowledge base behind the interfaces the new module takes, and conversions. No logic of its own.
#include "codebreaker/symbols/xdk_libraries.hpp"

#include <algorithm>
#include <mutex>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/xdk_libraries.hpp"
#include "codebreaker/engine/facts.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/store.hpp"
#include "py_json.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::symbols::xdk_libraries {

namespace {

namespace nx = cb::symbols::xdk_libraries;
using cb::compat::from_py;
using cb::compat::to_py;
using engine::facts::Json;
using engine::facts::Value;

Signature to_old(const nx::Signature& s) {
    Signature out;
    for (const std::uint32_t w : s.words) out.words.push_back(w);
    for (const std::uint32_t m : s.ignore) out.ignore.push_back(m);
    for (const std::string& n : s.names) out.names.add(to_py(n));
    for (const std::string& l : s.libs) out.libs.add(to_py(l));
    out.secondary = s.secondary;
    return out;
}

nx::Signature to_new(const Signature& s) {
    nx::Signature out;
    for (const int64_t w : s.words) out.words.push_back(static_cast<std::uint32_t>(w));
    for (const int64_t m : s.ignore) out.ignore.push_back(static_cast<std::uint32_t>(m));
    for (const py::str& n : s.names) out.names.insert(from_py(n));
    for (const py::str& l : s.libs) out.libs.insert(from_py(l));
    out.secondary = s.secondary;
    return out;
}

Json json_ints(const std::vector<int64_t>& xs) {
    Json j;
    j.kind = Json::List;
    for (int64_t x : xs) {
        Json i;
        i.kind = Json::Int;
        i.i = x;
        j.list.push_back(i);
    }
    return j;
}
Json json_strs(const py::set<py::str>& xs) {
    Json j;
    j.kind = Json::List;
    py::list<py::str> sorted_xs;
    for (const py::str& x : xs) sorted_xs.append(x);
    for (const py::str& x : py::sorted(sorted_xs)) {
        Json s;
        s.kind = Json::Str;
        s.s = x;
        j.list.push_back(s);
    }
    return j;
}

// The libraries of a folder as the new module takes them: only the PowerPC ones (the new module reads them, through
// symbols/libraries).
std::vector<nx::Library> read_libraries(const py::str& folder) { return nx::read_libraries(cb::fs::from_text(folder.utf8())); }

// The text section of a program: its words and where functions start.
nx::ProgramText read_program(const py::str& image_path) {
    py::str path = formats::xex::is_xex(image_path) ? formats::xex::basefile(image_path) : image_path;
    py::ref<formats::powerpc_image::Image> img = formats::powerpc_image::read(path);
    std::optional<std::tuple<py::str, int64_t, int64_t, int64_t>> text;
    for (const auto& s : img->sections) {
        if (std::get<0>(s).strip(py::str(std::u32string(1, U'\0'))) == ".text") {
            text = s;
            break;
        }
    }
    if (!text) throw py::ValueError(image_path.utf8() + ": no .text section");
    const int64_t t0 = std::get<1>(*text), size = std::get<2>(*text);
    const cb::Bytes bytes = from_py(img->bytes_at(t0, size));
    nx::ProgramText out;
    out.address = static_cast<std::uint32_t>(t0);
    for (std::size_t i = 0; i + 4 <= bytes.size(); i += 4) {
        out.words.push_back((static_cast<std::uint32_t>(bytes[i]) << 24) | (static_cast<std::uint32_t>(bytes[i + 1]) << 16) |
                            (static_cast<std::uint32_t>(bytes[i + 2]) << 8) | static_cast<std::uint32_t>(bytes[i + 3]));
    }
    for (const int64_t a : img->extents.keys()) {
        int64_t q = (a - t0) / 4;
        if ((a - t0) % 4 != 0 && (a - t0) < 0) --q;   // floor
        out.starts.insert(static_cast<std::uint32_t>(q));
    }
    return out;
}

}  // namespace

const py::str DEFAULT_LIBS = py::str(std::string(nx::kDefaultLibraries));

py::str lib_folder() {
    py::str s = py::settings::get_str(py::str("xdk_libs"));
    return s.empty() ? DEFAULT_LIBS : s;
}

bool _falls_through(int64_t w) { return nx::falls_through(static_cast<std::uint32_t>(w)); }

py::list<Signature> _read_signatures(const py::str& folder) {
    py::list<Signature> out;
    const auto libraries = read_libraries(folder);
    for (const nx::Signature& s : nx::make_signatures(libraries)) out.append(to_old(s));
    return out;
}

py::list<Signature> signatures(const py::str& folder) {
    const std::string stamp = "cpp-" + nx::signatures_stamp(nx::library_files(cb::fs::from_text(folder.utf8())));
    // COMPAT(py): remove when the staged knowledge/reads shim is installed (the old engine records its reads in the old layer)
    knowledge::reads::record(py::str("xbox360"), py::str("library_signatures"), to_py(stamp));

    auto kb = knowledge::store::open();
    auto table = kb->ensure_values(
        py::str("xbox360"), py::str("library_signatures"), to_py(stamp),
        [&]() {
            py::dict<py::str, Value> out;
            Json all;
            all.kind = Json::List;
            for (const Signature& s : _read_signatures(folder)) {
                Json entry;
                entry.kind = Json::List;
                entry.list.push_back(json_ints(s.words));
                entry.list.push_back(json_ints(s.ignore));
                entry.list.push_back(json_strs(s.names));
                entry.list.push_back(json_strs(s.libs));
                Json sec;
                sec.kind = Json::Bool;
                sec.b = s.secondary;
                entry.list.push_back(sec);
                all.list.push_back(entry);
            }
            out.set(to_py(stamp), Value(all));
            return out;
        },
        py::str("the XDK's libraries in ") + folder, knowledge::store::DERIVED);
    py::list<Signature> sigs;
    const Json& all = std::get<Json>(table.at(to_py(stamp)));
    for (const Json& e : all.list) {
        Signature s;
        for (const Json& w : e.list.at(0).list) s.words.push_back(w.i);
        for (const Json& w : e.list.at(1).list) s.ignore.push_back(w.i);
        for (const Json& n : e.list.at(2).list) s.names.add(n.s);
        for (const Json& n : e.list.at(3).list) s.libs.add(n.s);
        s.secondary = e.list.at(4).b;
        sigs.append(s);
    }
    return sigs;
}

py::dict<int64_t, Found> match(const py::str& image_path, const std::optional<py::str>& folder) {
    std::vector<nx::Signature> sigs;
    for (const Signature& s : signatures(folder ? *folder : lib_folder())) sigs.push_back(to_new(s));
    py::dict<int64_t, Found> out;
    for (const auto& [address, f] : nx::match(read_program(image_path), sigs)) {
        Found found;
        found.names = to_py(f.names);
        found.libs = to_py(f.libs);
        found.size = f.size;
        out.set(address, found);
    }
    return out;
}

const py::str USAGE(
    "xdk_libraries.py -- naming the XDK library functions an Xbox 360 program was statically linked with.\n\n"
    "    python -m codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]\n"
    "    from codebreaker.symbols import xdk_libraries\n"
    "    found = xdk_libraries.match(image_path, lib_folder)     # {address: {\"names\", \"libs\", \"size\"}}");

int64_t main(const py::list<py::str>& argv) {
    nx::Services services;
    services.default_folder = cb::fs::from_text(lib_folder().utf8());
    services.load_program = [](const std::filesystem::path& p) { return read_program(to_py(cb::fs::to_text(p))); };
    services.load_signatures = [](const std::filesystem::path& folder) {
        std::vector<nx::Signature> sigs;
        for (const Signature& s : signatures(to_py(cb::fs::to_text(folder)))) sigs.push_back(to_new(s));
        return sigs;
    };
    const std::vector<std::string> args = from_py(argv);
    return cb::compat::guard([&] { return nx::run(args, services); });
}

}  // namespace codebreaker::symbols::xdk_libraries
