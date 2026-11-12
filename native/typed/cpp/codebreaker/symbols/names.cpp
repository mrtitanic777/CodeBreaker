// codebreaker/symbols/names.cpp -- the shim's translation unit (see names.hpp). The new module (cb/symbols/names) names the functions; this file gives
// its Namer the corpus, the table of numbered exports, the body store (the old store's, behind the BodyStore interface), and the readers that are not
// refactored yet (the PowerPC image, the console's imports, runtime-library identification, the compiler), and converts types.
#include "codebreaker/symbols/names.hpp"

#include <algorithm>
#include <mutex>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/names.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/source/module_writer.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "codebreaker/symbols/runtime_library.hpp"

namespace codebreaker::symbols::names {

namespace {

namespace nn = ::cb::symbols::names;
using engine::native_engine::Cell;
using cb::compat::from_py;
using cb::compat::to_py;

Cell cell_of(const std::optional<std::string>& s) { return s ? Cell(to_py(*s)) : Cell(); }

// The new body store over the old function store (which holds the knowledge base's bodies). The store is the one the call was given.
class OldBodies final : public cb::store::BodyStore {
public:
    FunctionStorePtr fs;

    void set_bodies(const cb::store::ModuleRef& module, const std::map<std::string, cb::store::Body>& by_key) override {
        fs->add_module(to_py(module.sha), to_py(module.name), to_py(module.path), to_py(module.kind));
        py::dict<py::str, std::tuple<py::str, Cell, Cell>> rows;
        for (const auto& [key, body] : by_key) rows.set(to_py(key), std::make_tuple(to_py(body.digest), cell_of(body.name), cell_of(body.source)));
        fs->set_bodies(to_py(module.sha), rows);
    }
    std::map<std::string, cb::store::Body> bodies(std::string_view module) override {
        std::map<std::string, cb::store::Body> out;
        for (const auto& kv : fs->bodies(to_py(module)).items()) {
            const auto& [digest, name, source] = std::get<1>(kv);
            cb::store::Body body{from_py(digest), std::nullopt, std::nullopt};
            if (!std::holds_alternative<std::monostate>(name)) body.name = from_py(engine::native_engine::cell_str(name));
            if (!std::holds_alternative<std::monostate>(source)) body.source = from_py(engine::native_engine::cell_str(source));
            out[from_py(std::get<0>(kv))] = std::move(body);
        }
        return out;
    }
    void name_body(std::string_view digest, std::string_view name, std::string_view source) override {
        fs->name_body(to_py(digest), to_py(name), to_py(source));
    }
    std::map<std::string, cb::store::SharedName> names_from_bodies(std::string_view module) override {
        std::map<std::string, cb::store::SharedName> out;
        for (const auto& kv : fs->names_from_bodies(to_py(module)).items())
            out[from_py(std::get<0>(kv))] = {from_py(engine::native_engine::cell_str(std::get<0>(std::get<1>(kv)))),
                                             from_py(engine::native_engine::cell_str(std::get<1>(std::get<1>(kv))))};
        return out;
    }
    cb::store::BodyCounts shared_bodies() override {
        const auto cells = fs->shared_bodies();
        return {engine::native_engine::cell_int(cells.at(0)), engine::native_engine::cell_int(cells.at(1)), engine::native_engine::cell_int(cells.at(2)),
                engine::native_engine::cell_int(cells.at(3))};
    }
};

// One Namer for the process, as the Python kept its caches in the module: what it asks of the rest is wired here.
struct Shim {
    OldBodies bodies;
    nn::Namer namer;

    static nn::Namer::Services services(OldBodies& bodies) {
        nn::Namer::Services s{::codebreaker::base::corpus::detail::instance(), ordinals::shared_ordinals()};
        s.bodies = &bodies;
        s.powerpc = [](const std::filesystem::path& module) -> std::optional<nn::PowerPcNames> {
            const auto img = formats::powerpc_image::read(cb::compat::legacy::native(module));
            nn::PowerPcNames out;
            if (img->entry) out.entry = static_cast<nn::Address>(img->entry);
            for (const int64_t f : img->functions) out.functions.push_back(static_cast<nn::Address>(f));
            for (const auto& kv : formats::powerpc_image::virtual_methods(*img).items())
                out.methods.push_back({static_cast<nn::Address>(std::get<0>(kv)), from_py(std::get<0>(std::get<1>(kv))),
                                       static_cast<std::uint32_t>(std::get<1>(std::get<1>(kv)))});
            std::sort(out.methods.begin(), out.methods.end(), [](const auto& a, const auto& b) { return a.address < b.address; });
            return out;
        };
        s.console_import_names = [](const std::filesystem::path& module) {
            std::map<std::string, std::string> out;
            for (const auto& kv : source::module_writer::console_import_names(cb::compat::legacy::native(module)).items())
                out[from_py(std::get<0>(kv))] = from_py(std::get<1>(kv));
            return out;
        };
        s.runtime_library = [](const std::filesystem::path& module) {
            std::map<nn::Address, cb::symbols::runtime_library::Identified> out;
            for (const auto& kv : runtime_library::identify(cb::compat::legacy::native(module)).items())
                out[static_cast<nn::Address>(std::get<0>(kv))] = {from_py(std::get<0>(std::get<1>(kv))), from_py(std::get<1>(std::get<1>(kv)))};
            return out;
        };
        s.compiler = [](const std::filesystem::path& module) {
            const runtime_library::Compiler c = runtime_library::compiler(cb::compat::legacy::native(module));
            return cb::symbols::runtime_library::CompilerInfo{from_py(c.linker), from_py(c.compiler), {}};
        };
        s.collect_ordinals = [](std::span<const std::filesystem::path> files) {
            py::list<py::str> paths;
            for (const auto& f : files) paths.append(cb::compat::legacy::native(f));
            ordinals::collect(paths);
        };
        return s;
    }
    Shim() : namer(services(bodies)) {}
};

std::mutex g_lock;   // the Namer keeps caches; the old module's were not guarded either, but the shim's callers are many

// The shim of the process, with the store this call is to use (the one it was given, else the default one).
struct Locked {
    std::lock_guard<std::mutex> guard{g_lock};
    Shim& shim;
    // `wants_store`: the call reads or writes bodies, so it needs the store (the one it was given, else the default one, opened).
    explicit Locked(const FunctionStorePtr& fs, bool wants_store = true) : shim(instance()) {
        shim.bodies.fs = wants_store ? (fs ? fs : store::functions::open()) : nullptr;
    }
    static Shim& instance() {
        static Shim shim;
        return shim;
    }
};

std::filesystem::path path_of(const py::str& p) { return cb::compat::legacy::path_of(p); }
nn::Address address_of(int64_t a) { return static_cast<nn::Address>(a); }

Named old_named(const nn::Named& named) {
    Named out;
    for (const auto& [address, found] : named) out.set(static_cast<int64_t>(address), std::make_tuple(to_py(found.name), to_py(found.source)));
    return out;
}

}  // namespace

py::str key(const py::str& module_path, int64_t address) {
    return cb::compat::guard([&] {
        const auto lane = ::codebreaker::base::corpus::detail::instance().lane(path_of(module_path));
        return to_py(nn::function_key(lane.value_or(cb::base::Lane::Pe), address_of(address)));
    });
}

py::dict<int64_t, py::str> bodies(const py::str& module_path) {
    return cb::compat::guard([&] {
        Locked l(nullptr, false);
        py::dict<int64_t, py::str> out;
        for (const auto& [address, digest] : l.shim.namer.bodies(path_of(module_path))) out.set(static_cast<int64_t>(address), to_py(digest));
        return out;
    });
}

py::dict<std::tuple<int64_t, int64_t>, py::str> codeview(const py::bytes& data) {
    return cb::compat::guard([&] {
        py::dict<std::tuple<int64_t, int64_t>, py::str> out;
        for (const auto& [at, name] : nn::codeview(from_py(data))) out.set(std::make_tuple<int64_t, int64_t>(at.seg, at.off), to_py(name));
        return out;
    });
}

py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> read_sym(const py::str& path) {
    return cb::compat::guard([&] {
        const auto segments = nn::read_sym(cb::fs::read_file(path_of(path)));
        if (!segments) throw cb::FormatError("sym", 0, "not a symbol file");
        py::list<std::tuple<int64_t, py::dict<int64_t, py::str>>> out;
        for (const nn::SymSegment& s : *segments) {
            py::dict<int64_t, py::str> symbols;
            for (const auto& [value, name] : s.symbols) symbols.set(static_cast<int64_t>(value), to_py(name));
            out.append({static_cast<int64_t>(s.number), symbols});
        }
        return out;
    });
}

py::dict<int64_t, py::str> sym_objects(const py::str& path) {
    return cb::compat::guard([&] {
        py::dict<int64_t, py::str> out;
        try {
            for (const auto& [number, name] : nn::sym_objects(cb::fs::read_file(path_of(path)))) out.set(static_cast<int64_t>(number), to_py(name));
        } catch (const cb::IoError&) {
        }
        return out;
    });
}

py::dict<int64_t, py::str> source_files(const py::str& module_path) {
    return cb::compat::guard([&] {
        Locked l(nullptr, false);
        py::dict<int64_t, py::str> out;
        for (const auto& [address, name] : l.shim.namer.source_files(path_of(module_path))) out.set(static_cast<int64_t>(address), to_py(name));
        return out;
    });
}

py::dict<int64_t, std::tuple<py::str, py::str>> debug_names(const py::str& module_path) {
    return cb::compat::guard([&] {
        Locked l(nullptr, false);
        return old_named(l.shim.namer.debug_names(path_of(module_path)));
    });
}

Named own_names(const py::str& module_path) {
    return cb::compat::guard([&] {
        Locked l(nullptr, false);
        return old_named(l.shim.namer.own_names(path_of(module_path)));
    });
}

Named powerpc_names(const py::str& module_path) {
    return cb::compat::guard([&] {
        Locked l(nullptr, false);
        return old_named(l.shim.namer.powerpc_names(path_of(module_path)));
    });
}

int64_t index(const py::str& module_path, const FunctionStorePtr& fs, const std::optional<Named>& own) {
    return cb::compat::guard([&] {
        Locked l(fs);
        std::optional<nn::Named> given;
        if (own) {
            given.emplace();
            for (const auto& kv : own->items())
                (*given)[address_of(std::get<0>(kv))] = {from_py(std::get<0>(std::get<1>(kv))), from_py(std::get<1>(std::get<1>(kv)))};
        }
        return static_cast<int64_t>(l.shim.namer.index(path_of(module_path), given));
    });
}

std::optional<py::str> set_name(const py::str& module_path, int64_t address, const py::str& name, const FunctionStorePtr& fs) {
    return cb::compat::guard([&] {
        Locked l(fs);
        const auto digest = l.shim.namer.set_name(path_of(module_path), address_of(address), from_py(name));
        return digest ? std::optional<py::str>(to_py(*digest)) : std::nullopt;
    });
}

ModuleNames of_module(const py::str& module_path, const FunctionStorePtr& fs) {
    return cb::compat::guard([&] {
        Locked l(fs);
        ModuleNames out;
        const nn::ModuleNames found = l.shim.namer.of_module(path_of(module_path));
        for (const nn::FunctionName& f : found.functions)
            out.functions.append(FunctionName{static_cast<int64_t>(f.address), to_py(f.key), to_py(f.name), to_py(f.source)});
        for (const nn::ImportName& i : found.imports)
            out.imports.append(ImportName{to_py(i.module), i.ordinal ? std::optional<int64_t>(*i.ordinal) : std::nullopt, to_py(i.name), to_py(i.source)});
        return out;
    });
}

int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        Locked l(nullptr);
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(from_py(a));
        return static_cast<int64_t>(l.shim.namer.run(args));
    });
}

}  // namespace codebreaker::symbols::names
