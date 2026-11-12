// codebreaker/java/transpile.hpp -- SHIM over cb/java/transpile.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// The old module held the converter's state in module variables (the active target profile with its observers, the project of the
// conversion, the nullable-getter sets, ...) and ran one conversion at a time. The new converter holds none: a conversion is handed
// what it works with. Here the old shape is kept for the old callers, over the new code:
//
//   * the profile registry (`active_profile`, `use_profile`, `profile_names`) is the old profiles module's, and switching the active
//     profile reseeds the old typemap/rules/vcxproj shims, as the Python's `profiles.on_change` observers did;
//   * `convert` hands the new converter the old registry's profiles and the old Minecraft project (wrapped), and the translator of the
//     old world: javac's trees (`dump_trees`) lowered by the old C++03/C++17 back end (cpp03 `Bodies`), answering the new back-end
//     interface. The Java front end (`dump_trees`, `set_tree_dumper`) is here because the old back end is its only reader;
//   * `build_resolver` is the new corpus pass, handed back as the old resolver record.
#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <unordered_map>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/corpus.hpp"
#include "cb/java/transpile.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "codebreaker/java/console/vcxproj.hpp"
#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/emit/source.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/overlays.hpp"
#include "codebreaker/java/probe.hpp"
#include "codebreaker/java/profiles/package.hpp"
#include "codebreaker/java/projects/minecraft/package.hpp"
#include "codebreaker/java/projects/package.hpp"
#include "codebreaker/java/rules.hpp"
#include "codebreaker/java/runtime.hpp"
#include "codebreaker/java/typemap.hpp"
#include "codebreaker/java/types.hpp"
#include "codebreaker/translate/backends/cpp03.hpp"
#include "codebreaker/translate/backends/cpp17.hpp"
#include "codebreaker/translate/desugar.hpp"
#include "codebreaker/translate/frontends/java/lower.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

#ifdef ERROR
#undef ERROR   // (wingdi.h: it is the name of a ledger Category)
#endif


namespace codebreaker::verify::lanes::java {
std::tuple<py::str, py::str> jdk();   // (javac, java) -- defined with the Java lane
}

namespace codebreaker::java::transpile {

using ClassRef = py::ref<ir::JClass>;
using MethodRef = py::ref<ir::Method>;
using Progress = std::function<void(const py::str&)>;
using Profile = profiles::Profile;

namespace cbt = cb::java::transpile;

// ---------------------------------------------------------------------------------------------- profiles
namespace shim_ {

// The old typemap's record of a profile, from the registry's.
inline typemap::Profile typemap_profile(const Profile& p) {
    typemap::Profile t;
    t.nullable = py::set<py::str>::from(p.nullable);
    t.nullable_fields = py::set<py::str>::from(p.nullable_fields);
    t.nullable_getters = py::set<py::str>::from(p.nullable_getters);
    t.nullable_getter_return_type = p.nullable_getter_return_type.copy();
    for (const py::str& pair : p.unsigned_byte_scalar_fields) {   // "Class.field"
        const std::string text = cb::compat::from_py(pair);
        const std::size_t dot = text.find('.');
        if (dot != std::string::npos) t.unsigned_byte_scalar_fields.add(std::make_tuple(cb::compat::to_py(text.substr(0, dot)), cb::compat::to_py(text.substr(dot + 1))));
    }
    t.default_value_types = py::set<py::str>::from(p.default_value_types);
    return t;
}

// What the Python's profile observers did on every switch: the frozen profile globals of the old shims are re-seeded.
inline void reseed(const Profile& p) {
    typemap::_reseed(typemap_profile(p));
    rules::_reseed(p.heavy_types, p.heavy_types_high);
    console::vcxproj::_reseed(p.vcxproj_filters);
}

inline void observe() {
    static const bool registered = (profiles::on_change(reseed), true);
    (void)registered;
}

inline cb::java::typemap::NameSet names_of(const py::set<py::str>& s) {
    cb::java::typemap::NameSet out;
    for (const py::str& n : s) out.insert(cb::compat::from_py(n));
    return out;
}

// A profile of the registry as the new tree's record.
inline cb::java::profiles::Profile to_cb(const Profile& p) {
    namespace cc = cb::compat;
    cb::java::profiles::Profile o;
    o.name = cc::from_py(p.name);
    o.static_init_order = cc::from_py(p.static_init_order);
    for (const auto& [k, v] : p.package_roles.items()) o.package_roles.insert_or_assign(cc::from_py(k), cc::from_py(v));
    o.heavy_types = cc::from_py(p.heavy_types);
    o.heavy_types_high = cc::from_py(p.heavy_types_high);
    for (const auto& f : p.vcxproj_filters) o.vcxproj_filters.push_back({cc::from_py(std::get<0>(f)), cc::from_py(std::get<1>(f))});
    if (p.dead_types) o.dead_types = cc::from_py(*p.dead_types);
    if (p.optional_types) o.optional_types = cc::from_py(*p.optional_types);
    o.type.nullable = names_of(p.nullable);
    o.type.nullable_fields = names_of(p.nullable_fields);
    o.type.nullable_getters = names_of(p.nullable_getters);
    for (const auto& [k, v] : p.nullable_getter_return_type.items()) o.type.nullable_getter_return_type[cc::from_py(k)] = cc::from_py(v);
    for (const py::str& pair : p.unsigned_byte_scalar_fields) {
        const std::string text = cc::from_py(pair);
        const std::size_t dot = text.find('.');
        if (dot != std::string::npos) o.type.unsigned_byte_scalar_fields.insert({text.substr(0, dot), text.substr(dot + 1)});
    }
    o.type.default_value_types = names_of(p.default_value_types);
    return o;
}

}  // namespace shim_

inline const Profile& GENERIC_PROFILE() { return *profiles::GENERIC(); }
inline const Profile& active_profile() {
    shim_::observe();
    return *profiles::active();
}
// Switch the active target profile (reseeding the frozen profile globals). KeyError for an unknown name.
inline const Profile& use_profile(const py::str& name) {
    shim_::observe();
    return *profiles::use(name);
}
inline py::list<py::str> profile_names() { return profiles::names(); }

// ---------------------------------------------------------------------------------------------- the Java front end
// {source path relative to src without .json: javac's typed tree}
using Trees = py::dict<py::str, translate::ir::Dict>;
using TreeDumper = std::function<Trees(const py::str& java_dir, const py::list<py::str>& classpath)>;

namespace shim_ {
inline TreeDumper& dumper() {
    static TreeDumper d;
    return d;
}

inline py::str state(const py::list<py::str>& parts) {   // settings.state(*parts)
    py::str path = py::settings::get_str(py::str("state"));
    for (const py::str& p : parts) path = py::os::path::join(path, p);
    py::os::makedirs(py::truthy(parts) ? py::os::path::dirname(path) : path, 0777, true);
    return path;
}

// fn on every item, `jobs` at a time on threads of this process; the results in the items' order (an item's exception is raised
// after the others ended).
template <class Fn, class Item>
auto parallel_map(Fn fn, const py::list<Item>& items, int64_t jobs) {
    using R = decltype(fn(std::declval<const Item&>()));
    std::vector<R> out(static_cast<size_t>(items.size()));
    std::atomic<int64_t> next{0};
    std::mutex failed_lock;
    std::exception_ptr failed;
    auto work = [&]() {
        for (;;) {
            int64_t i = next.fetch_add(1);
            if (i >= items.size()) return;
            try {
                out[static_cast<size_t>(i)] = fn(items[i]);
            } catch (...) {
                std::lock_guard<std::mutex> hold(failed_lock);
                if (!failed) failed = std::current_exception();
            }
        }
    };
    int64_t n = std::max<int64_t>(1, std::min<int64_t>(jobs, items.size()));
    std::vector<std::thread> ts;
    for (int64_t k = 1; k < n; k++) ts.emplace_back(work);
    work();
    for (auto& t : ts) t.join();
    if (failed) std::rethrow_exception(failed);
    return out;
}

// The folder holding the compiled TreeDump, compiling it when its source or the JDK changed.
inline py::str treedump_tool(const py::str& javac) {
    py::str source = py::os::path::normpath(py::os::path::join(_paths::_HERE(), py::str(".."), py::str("translate"), py::str("frontends"), py::str("java"), py::str("TreeDump.java")));
    py::file f = py::open(source, py::str("rb"));
    py::bytes data = f.read();
    py::hashlib::Hash h = py::hashlib::sha1(data + py::bytes(py::repr(base::cache::tool(javac)).encode("utf-8")));
    py::str key = h.hexdigest().slice(0, 12);
    py::str folder = state(py::list<py::str>{py::str("translate"), py::str("treedump-") + key});
    if (!py::os::path::isfile(py::os::path::join(folder, py::str("TreeDump.class")))) {
        py::os::makedirs(folder, 0777, true);
        py::subprocess::RunOptions o;
        o.stdout_ = py::subprocess::Stream::Capture;
        o.stderr_ = py::subprocess::Stream::Stdout;
        o.encoding = "utf-8";
        o.errors = "replace";
        auto r = py::subprocess::run(py::list<py::str>{javac, py::str("-nowarn"), py::str("-encoding"), py::str("UTF-8"), py::str("-d"), folder, source}, o);
        if (r.returncode != 0) {
            py::shutil::rmtree(folder, true);
            throw py::RuntimeError(py::str("TreeDump did not compile: ") + r.stdout_.value_or(py::str()).slice(0, 400));
        }
        py::str parent = py::os::path::dirname(folder);
        for (const py::str& old : py::os::listdir(parent)) {   // (those of another source or JDK: never used again)
            if (old.startswith(py::str("treedump-")) && old != py::os::path::basename(folder)) py::shutil::rmtree(py::os::path::join(parent, old), true);
        }
    }
    return folder;
}

}  // namespace shim_

// A host gives the trees another way (an empty function: the built-in one, javac's TreeDump).
inline void set_tree_dumper(const TreeDumper& dumper) { shim_::dumper() = dumper; }

// translate.frontends.java.dump over a temporary folder (compiling TreeDump once, under the state folder).
inline Trees dump_trees(const py::str& java_dir, const py::list<py::str>& classpath = py::list<py::str>()) {
    if (shim_::dumper()) return shim_::dumper()(java_dir, classpath);
    auto jdk = verify::lanes::java::jdk();
    py::str javac = std::get<0>(jdk), java = std::get<1>(jdk);
    if (javac.empty()) throw py::RuntimeError(py::str("the Java front end needs a JDK (settings.json: java)"));
    py::str folder = shim_::treedump_tool(javac);
    py::list<py::str> files;   // every .java file under the source folder, sorted
    for (const auto& w : py::os::walk(java_dir))
        for (const py::str& f : std::get<2>(w))
            if (f.endswith(py::str(".java"))) files.append(py::os::path::join(std::get<0>(w), f));
    files = py::list<py::str>::from(py::sorted(files));
    py::str out = py::tempfile::mkdtemp(std::nullopt, py::str("cb-treedump-"));
    try {
        if (py::os::path::isdir(out)) py::shutil::rmtree(out);
        py::os::makedirs(out);
        py::str listing = py::os::path::join(out, py::str("_files.txt"));
        {
            py::file lf = py::open(listing, py::str("w"), py::str("utf-8"));
            lf.write(py::join(py::str("\n"), files));
        }
        py::list<py::str> cmd{java, py::str("-Xss16m"), py::str("-cp"), folder, py::str("TreeDump"), py::str("--out"), out, py::str("--sourcepath"), java_dir};
        if (py::truthy(classpath)) {
            cmd.append(py::str("--cp"));
            cmd.append(py::join(py::os::pathsep, classpath));
        }
        cmd.append(py::str("@") + listing);
        py::subprocess::RunOptions o;
        o.encoding = "utf-8";
        o.errors = "replace";
        auto r = py::subprocess::run(cmd, o);
        if (r.returncode != 0) {
            py::str err = r.stderr_.value_or(py::str());
            throw py::RuntimeError(py::str("TreeDump failed: ") + err.slice(std::max<int64_t>(err.size() - 600, 0), err.size()));
        }
        // every unit's tree, read and parsed on the workers' threads (the units are independent)
        py::list<std::tuple<py::str, py::str>> found;
        for (const auto& w : py::os::walk(out)) {
            for (const py::str& f : std::get<2>(w)) {
                if (f.endswith(py::str(".json")) && !f.startswith(py::str("_"))) {
                    py::str p = py::os::path::join(std::get<0>(w), f);
                    py::str rel = py::os::path::relpath(p, out);
                    found.append(std::make_tuple(p, rel.slice(0, rel.size() - 5).replace(py::str("\\"), py::str("/"))));
                }
            }
        }
        auto load_one = [](const std::tuple<py::str, py::str>& pr) {
            py::file fh = py::open(std::get<0>(pr), py::str("rb"));
            return translate::frontends::java::lower::load_tree(fh.read().decode("utf-8", "strict"));
        };
        auto loaded = shim_::parallel_map(load_one, found, base::parallel::workers());
        Trees units;
        for (size_t k = 0; k < loaded.size(); k++) units.set(std::get<1>(found[static_cast<int64_t>(k)]), loaded[k]);
        py::shutil::rmtree(out, true);
        return units;
    } catch (...) {
        py::shutil::rmtree(out, true);
        throw;
    }
}

// ---------------------------------------------------------------------------------------------- the passes
// All .java under java_dir, de-duplicated by basename (output is keyed by class name).
inline py::list<py::str> discover(const py::str& java_dir) {
    py::list<py::str> out;
    for (const auto& p : cb::java::corpus::discover(cb::fs::from_text(cb::compat::from_py(java_dir)))) out.append(cb::compat::to_py(cb::fs::to_text(p)));
    return out;
}

namespace shim_ {

// The new resolver's facts as the old resolver record (what the old code reads field by field).
inline py::ref<types::TypeResolver> to_old_resolver(const cb::java::types::TypeResolver& resolver) {
    namespace cc = cb::compat;
    const cb::java::types::ResolverData& d = resolver.data();
    py::dict<py::str, types::MemberEnv> members;
    for (const auto& [name, env] : d.members) {
        types::MemberEnv e;
        for (const auto& [f, t] : env.fields) e.fields.set(cc::to_py(f), ir::from_new(t));
        for (const auto& [m, t] : env.methods) e.methods.set(cc::to_py(m), ir::from_new(t));
        if (env.parent) e.parent = cc::to_py(*env.parent);
        members.set(cc::to_py(name), e);
    }
    py::dict<py::str, py::set<py::str>> renames;
    for (const auto& [cls, names] : d.method_renames) renames.set(cc::to_py(cls), cc::to_py(names));
    auto out = std::make_shared<types::TypeResolver>(cc::to_py(d.known), cc::to_py(d.values), cc::to_py(d.interfaces), cc::to_py(d.enums), members, renames,
                                                      cc::to_py(d.typed_enums), cc::to_py(d.thread_like));
    out->markers = cc::to_py(d.markers);
    out->own_catalog.return_methods = cc::to_py(d.own_catalog.return_methods);
    for (const auto& [k, v] : d.own_catalog.scalar_methods) out->own_catalog.scalar_methods.set(cc::to_py(k), cc::to_py(v));
    for (const auto& [k, positions] : d.own_catalog.retained_params) {
        py::set<int64_t> set;
        for (const std::size_t i : positions) set.add(static_cast<int64_t>(i));
        out->own_catalog.retained_params.set(cc::to_py(k), set);
    }
    out->own_catalog.corpus_methods = cc::to_py(d.own_catalog.corpus_methods);
    return out;
}

inline std::vector<cb::java::ir::JClass> to_new_classes(const py::list<ClassRef>& classes) {
    std::vector<cb::java::ir::JClass> out;
    out.reserve(static_cast<std::size_t>(classes.size()));
    for (const ClassRef& c : classes) out.push_back(ir::to_new(*c));
    return out;
}

}  // namespace shim_

inline py::ref<types::TypeResolver> build_resolver(const py::list<ClassRef>& classes, const std::optional<py::set<py::str>>& value_types = std::nullopt) {
    return cb::compat::guard([&] {
        const auto runtime_holder = (runtime::STUB_MEMBERS(), runtime::shim_::views().runtime.get());   // (the runtime, made if it was not)
        std::vector<cb::java::ir::JClass> news = shim_::to_new_classes(classes);
        cb::java::corpus::ResolverInputs inputs;
        cb::java::typemap::NameSet names;
        for (const py::str& n : (value_types ? *value_types : typemap::DEFAULT_VALUE_TYPES)) names.insert(cb::compat::from_py(n));
        inputs.value_types = names;
        inputs.runtime = runtime_holder;
        return shim_::to_old_resolver(cb::java::corpus::build_resolver(news, inputs));
    });
}

// What convert() answers (a dict in Python).
struct Summary {
    int64_t parsed = 0;
    int64_t headers = 0;
    int64_t sources = 0;
    py::list<py::str> markers;                               // empty decompiler marker classes, dropped on purpose
    std::tuple<int64_t, py::dict<py::str, int64_t>> tree_bodies;   // (written, declined by reason)
    int64_t rule_findings = 0;
    int64_t events = 0;
    int64_t high = 0;
    std::optional<py::str> report;
    py::ref<ledger::Ledger> ledger;
    py::str cpp_std;
    py::str platform;
    py::str profile;                                         // the target profile active DURING this conversion
};

namespace shim_ {

// The old Minecraft project as the new tree's `Project` (the hooks forward; the IR fixes are made by the translator below, which keeps
// their report for after_emit).
class OldProject : public cb::java::projects::Project {
  public:
    explicit OldProject(std::shared_ptr<projects::Project> old) : old_(std::move(old)) {
        name = cb::compat::from_py(old_->name);
        for (const auto& [k, v] : old_->char_map.items()) char_map[static_cast<std::uint32_t>(k)] = static_cast<std::uint32_t>(v);
        registrars = cb::compat::from_py(old_->registrars);
    }
    std::optional<std::string> profile_for(std::string_view edition, std::string_view mc_version, std::span<const cb::java::ir::JClass> classes) const override {
        py::list<ClassRef> olds;
        for (const cb::java::ir::JClass& c : classes) olds.append(ir::from_new(c));
        const auto r = old_->profile_for(edition.empty() ? std::nullopt : std::optional<py::str>(cb::compat::to_py(edition)),
                                         mc_version.empty() ? std::nullopt : std::optional<py::str>(cb::compat::to_py(mc_version)), olds);
        return r ? std::optional<std::string>(cb::compat::from_py(*r)) : std::nullopt;
    }
    std::string finalize_text(std::string text, std::optional<std::string_view> platform) const override {
        return cb::compat::from_py(old_->finalize_text(cb::compat::to_py(text), platform ? std::optional<py::str>(cb::compat::to_py(*platform)) : std::nullopt));
    }
    void after_emit(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const cb::java::projects::Patched&,
                    const cb::java::projects::Progress& progress) override {
        old_->after_emit(cb::compat::to_py(cb::fs::to_text(out_dir)), platform_(platform), patched, old_progress(progress));
    }
    void after_construction(const std::filesystem::path& out_dir, std::optional<std::string_view> platform, const cb::java::projects::Progress& progress) override {
        old_->after_construction(cb::compat::to_py(cb::fs::to_text(out_dir)), platform_(platform), old_progress(progress));
    }
    void reports(const std::filesystem::path& java_dir, const std::filesystem::path& out_dir, std::span<const std::string> runtime_files,
                 const cb::java::projects::Progress& progress) override {
        old_->reports(cb::compat::to_py(cb::fs::to_text(java_dir)), cb::compat::to_py(cb::fs::to_text(out_dir)),
                      cb::compat::to_py(std::vector<std::string>(runtime_files.begin(), runtime_files.end())), old_progress(progress));
    }

    std::shared_ptr<projects::Project> old_;
    projects::Patched patched;   // what the old IR fixes did, for after_emit

  private:
    static std::optional<py::str> platform_(std::optional<std::string_view> p) { return p ? std::optional<py::str>(cb::compat::to_py(*p)) : std::nullopt; }
    static projects::Progress old_progress(const cb::java::projects::Progress& p) {
        return [p](const py::str& line) { p(cb::compat::from_py(line)); };
    }
};

// The new back-end interface over the old cpp03 `Bodies`: a member of the converted class is found again by its position in the old
// class. What the old back end records on its ledger moves to the new one after each answer.
class OldBodies : public cb::java::emit::source::BodySource {
  public:
    OldBodies(std::shared_ptr<translate::backends::cpp03::Bodies> bodies, py::ref<ledger::Ledger> old_ledger, cb::java::ledger::Ledger& ledger,
              std::map<std::string, ClassRef> classes)
        : bodies_(std::move(bodies)), old_ledger_(std::move(old_ledger)), ledger_(ledger), classes_(std::move(classes)) {}

    std::optional<std::string> field_init(const cb::java::ir::JClass& cls, const cb::java::ir::Field& f) override {
        const auto r = bodies_->field_init(cb::compat::to_py(cls.name), cb::compat::to_py(f.name));
        drain();
        return r ? std::optional<std::string>(cb::compat::from_py(*r)) : std::nullopt;
    }
    std::optional<std::vector<std::string>> array_items(const cb::java::ir::JClass& cls, const cb::java::ir::Field& f) override {
        return lines(bodies_->array_items(cb::compat::to_py(cls.name), cb::compat::to_py(f.name)));
    }
    std::optional<std::vector<std::string>> instance_inits(const cb::java::ir::JClass& cls) override { return lines(bodies_->instance_inits(cb::compat::to_py(cls.name))); }
    std::optional<cb::java::emit::source::CtorBody> ctor(const cb::java::ir::JClass& cls, const cb::java::ir::Method* m, bool two_phase) override {
        const auto r = bodies_->ctor(cb::compat::to_py(cls.name), m != nullptr ? method_of(cls, *m) : MethodRef(), two_phase);
        drain();
        if (!r) return std::nullopt;
        cb::java::emit::source::CtorBody out;
        if (r->kind && *r->kind == py::str("super")) out.call = cb::java::emit::source::CtorCall::Super;
        else if (r->kind && *r->kind == py::str("this")) out.call = cb::java::emit::source::CtorCall::This;
        if (r->args) out.super_args = cb::compat::from_py(*r->args);
        out.lines = cb::compat::from_py(r->body);
        return out;
    }
    std::optional<std::vector<std::string>> enum_args(const cb::java::ir::JClass& cls, const cb::java::ir::EnumConst& c) override {
        return lines(bodies_->enum_args(cb::compat::to_py(cls.name), cb::compat::to_py(c.name)));
    }
    std::optional<std::vector<std::vector<std::string>>> static_blocks(const cb::java::ir::JClass& cls) override {
        const auto r = bodies_->static_blocks(cb::compat::to_py(cls.name));
        drain();
        if (!r) return std::nullopt;
        std::vector<std::vector<std::string>> blocks;
        for (const py::list<py::str>& b : r->vec()) blocks.push_back(cb::compat::from_py(b));
        return blocks;
    }
    std::optional<std::vector<std::string>> body(const cb::java::ir::JClass& cls, const cb::java::ir::Method& m) override {
        return lines(bodies_->body(cb::compat::to_py(cls.name), method_of(cls, m)));
    }

  private:
    // the old method at the converted method's position in its class (the conversion keeps the order)
    MethodRef method_of(const cb::java::ir::JClass& cls, const cb::java::ir::Method& m) const {
        return classes_.at(cls.name)->methods[static_cast<int64_t>(&m - cls.methods.data())];
    }
    std::optional<std::vector<std::string>> lines(const std::optional<py::list<py::str>>& r) {
        drain();
        return r ? std::optional<std::vector<std::string>>(cb::compat::from_py(*r)) : std::nullopt;
    }
    void drain() {   // the events the old back end recorded since the last answer
        for (; drained_ < old_ledger_->events.size(); ++drained_) ledger_.add(old_ledger_->events[drained_]->to_new());
    }

    std::shared_ptr<translate::backends::cpp03::Bodies> bodies_;
    py::ref<ledger::Ledger> old_ledger_;
    cb::java::ledger::Ledger& ledger_;
    std::map<std::string, ClassRef> classes_;
    int64_t drained_ = 0;
};

// The translator of the old world: javac's trees lowered to the translator's IR, the project's fixes on that tree, and the C++03 or C++17
// back end over it.
class OldTranslator : public cbt::BodyTranslator {
  public:
    cbt::TranslatedBodies translate(const cbt::TranslateRequest& request) override {
        namespace cpp03 = translate::backends::cpp03;
        const auto old_resolver = to_old_resolver(request.resolver);
        // the old classes, with the derived members the new passes computed (the conversion does not carry them)
        py::list<ClassRef> classes;
        std::map<std::string, ClassRef> by_name;
        std::unordered_map<const cb::java::ir::Field*, py::ref<ir::Field>> old_field;
        std::unordered_map<const cb::java::ir::Method*, MethodRef> old_method;
        for (const cb::java::ir::JClass& c : request.classes) {
            ClassRef o = ir::from_new(c);
            for (std::size_t i = 0; i < c.fields.size(); ++i) old_field[&c.fields[i]] = o->fields[static_cast<int64_t>(i)];
            for (std::size_t i = 0; i < c.methods.size(); ++i) old_method[&c.methods[i]] = o->methods[static_cast<int64_t>(i)];
            classes.append(o);
            by_name[c.name] = o;
        }
        for (std::size_t k = 0; k < request.classes.size(); ++k) {
            const cb::java::ir::JClass& c = request.classes[k];
            ClassRef& o = classes[static_cast<int64_t>(k)];
            for (const cb::java::ir::Field* f : c.inherited_fields) o->inherited_fields.append(old_field.at(f));
            for (const cb::java::ir::MiForward& m : c.mi_forwards) o->mi_forwards.append(std::make_tuple(old_method.at(m.interface_method), cb::compat::to_py(m.base_class)));
        }
        // the translator's reading of the project: its font, its fixes
        std::shared_ptr<projects::Project> old_project = projects::GENERIC();
        OldProject* wrapped = dynamic_cast<OldProject*>(&request.project);
        if (wrapped != nullptr) old_project = wrapped->old_;

        auto old_ledger = std::make_shared<ledger::Ledger>();
        auto ctx = std::make_shared<cpp03::Context>(old_resolver, classes);
        ctx->ledger = old_ledger;
        ctx->char_map = old_project->char_map.copy();
        const py::str java_dir = cb::compat::to_py(cb::fs::to_text(request.java_dir));
        py::list<py::str> cp = probe::libraries();
        py::str shim = py::truthy(cp) ? probe::shims(state(py::list<py::str>{py::str("translate")}), std::get<0>(verify::lanes::java::jdk())) : py::str();
        py::list<py::str> dump_cp;
        if (py::truthy(shim)) dump_cp.append(shim);
        dump_cp.extend(cp);
        py::list<translate::ir::NodeP> units = cpp03::java_units(dump_trees(java_dir, dump_cp));
        // the console's fixes to the game's own logic, made on the tree before it is written (by shape, so no printing of the C++ hides a
        // site): sound routed to XACT, the applet ping, the render budget and NaN
        const std::optional<py::str> platform = request.platform ? std::optional<py::str>(cb::compat::to_py(*request.platform)) : std::nullopt;
        const projects::Patched patched = old_project->ir_patches(units, platform);
        if (wrapped != nullptr) wrapped->patched = patched;
        std::shared_ptr<cpp03::Bodies> tree;
        if (request.cpp_year >= 2017) {   // the C++17 forms (backends/cpp17.Bodies: cpp03.Bodies17)
            tree = translate::backends::cpp17::Bodies(ctx, units);
        } else {   // modern Java lowered to what C++03 can say (desugar.py)
            translate::desugar::run(units);
            tree = std::make_shared<cpp03::Bodies>(ctx, units);
        }
        cbt::TranslatedBodies out;
        out.bodies = std::make_unique<OldBodies>(tree, old_ledger, request.ledger, std::move(by_name));
        out.classes_lowered = static_cast<std::size_t>(tree->decls.size());
        out.methods_written = static_cast<std::size_t>(tree->written);
        for (const auto& [reason, count] : tree->declined.items()) out.declined.emplace_back(cb::compat::from_py(reason), static_cast<std::size_t>(count));
        return out;
    }
};

}  // namespace shim_

// Convert a Java source tree to C++ (see cb::java::transpile::convert). `ledger`: null makes a new one.
inline Summary convert(const py::str& java_dir, const py::str& out_dir, const std::optional<py::set<py::str>>& value_types = std::nullopt,
                       const py::ref<ledger::Ledger>& ledger = nullptr, bool write_report = true, const py::str& cpp_std = py::str("C++03"),
                       const std::optional<py::str>& platform = std::nullopt, const Progress& progress_cb = nullptr, bool platform_refs = false,
                       const std::optional<py::str>& sdk_override = std::nullopt, bool emit_debug = true, const std::optional<py::str>& overlay_dir = std::nullopt,
                       const std::optional<py::str>& edition = std::nullopt, const std::optional<py::str>& mc_version = std::nullopt) {
    return cb::compat::guard([&] {
        namespace cc = cb::compat;
        shim_::observe();
        runtime::STUB_MEMBERS();   // (the runtime is made on first use)
        const cb::java::JavaData data(cb::fs::from_text(cc::from_py(_paths::_HERE())));
        cb::java::profiles::ProfileSet profile_set;   // the old registry's profiles, the active one first
        const auto active = profiles::active();
        profile_set.add(shim_::to_cb(*active));
        profile_set.use(cc::from_py(active->name));
        profile_set.set_loader([](cb::java::profiles::ProfileSet& set, std::string_view name) {   // a project's profile: its project registers it on load
            try {
                set.add(shim_::to_cb(*profiles::get(cc::to_py(name))));
            } catch (const py::KeyError&) {
            }
        });
        cb::java::projects::ProjectSet project_set(data.path({"projects"}));
        project_set.add("minecraft", std::make_unique<shim_::OldProject>(projects::minecraft::PROJECT()));
        shim_::OldTranslator translator;
        cbt::Services services{data, *runtime::shim_::views().runtime, profile_set, project_set, translator};

        cbt::Options options;
        options.cpp_std = cc::from_py(cpp_std);
        if (platform) options.platform = cc::from_py(*platform);
        if (value_types) {
            cb::java::typemap::NameSet names;
            for (const py::str& n : *value_types) names.insert(cc::from_py(n));
            options.value_types = names;
        }
        options.write_report = write_report;
        options.platform_refs = platform_refs;
        const auto hints = console::sdk::detail_::hints_for(platform, sdk_override);
        options.sdk_override = hints.override_root;
        options.sdk_configured = hints.configured_root;
        options.emit_debug = emit_debug;
        if (overlay_dir) options.overlay_dir = cb::fs::from_text(cc::from_py(*overlay_dir));
        options.overlays_root = cb::fs::from_text(cc::from_py(overlays::overlays_root()));
        options.bugfix_ledger = cb::fs::from_text(cc::from_py(shim_::state(py::list<py::str>{py::str("java"), py::str("BUGFIX_LEDGER.md")})));
        if (edition) options.edition = cc::from_py(*edition);
        if (mc_version) options.mc_version = cc::from_py(*mc_version);
        if (progress_cb) options.progress = [&progress_cb](std::string_view line) { progress_cb(cc::to_py(line)); };

        cbt::Summary s;
        try {
            s = cbt::convert(cb::fs::from_text(cc::from_py(java_dir)), cb::fs::from_text(cc::from_py(out_dir)), options, services);
        } catch (const cb::java::console::sdk::SdkNotFoundError& e) {   // the SDK gate: the callers catch the old exception
            throw console::sdk::SdkNotFound(std::string(e.what()));
        }
        Summary out;
        out.parsed = static_cast<int64_t>(s.parsed);
        out.headers = static_cast<int64_t>(s.headers);
        out.sources = static_cast<int64_t>(s.sources);
        out.markers = cc::to_py(s.markers);
        py::dict<py::str, int64_t> declined;
        for (const auto& [reason, count] : s.declined) declined.set(cc::to_py(reason), static_cast<int64_t>(count));
        out.tree_bodies = std::make_tuple(static_cast<int64_t>(s.methods_written), declined);
        out.rule_findings = static_cast<int64_t>(s.rule_findings);
        out.events = static_cast<int64_t>(s.events);
        out.high = static_cast<int64_t>(s.high);
        if (s.report) out.report = cc::to_py(cb::fs::to_text(*s.report));
        out.ledger = ledger ? ledger : std::make_shared<ledger::Ledger>();
        for (const cb::java::ledger::Event& e : s.ledger.events()) out.ledger->events.append(ledger::from_new(e));
        out.cpp_std = cc::to_py(s.cpp_std);
        out.platform = cc::to_py(s.platform);
        out.profile = cc::to_py(s.profile);
        return out;
    });
}

}  // namespace codebreaker::java::transpile
