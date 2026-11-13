// codebreaker/verify/reconstruct.hpp -- COMPAT SHIM over cb/verify/reconstruct.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/reconstruct.{hpp,cpp} now: `Reconstructor::run` (decompile, build what has no verdict for its text, judge,
// record) over a registry of lanes (`Kinds`). This header keeps the old py:: API for the lanes that are not refactored yet and for their callers:
// the old `Kind` (a lane's profile / prepare / build / judge over py:: records, with the judge's inputs opaque), `register_kind` and the
// functions that ask the registry, and `run`, which hands the old lanes to the new reconstructor through an adapter.
//
// What stays in the old world until its own module is refactored, installed through `hooks()` by the code that owns it (engine/lanes_wiring.cpp):
// which platform a file is (engine::lanes' registry) and the engine's way of making the same results (`engine_run`). And judging in worker
// processes (jobs/workers) for the lanes that say how their judge's inputs travel (`encode_context`): the new run takes it as its `judge_batch`.
// COMPAT(py): remove with the last old lane that is still an old Kind (the console lanes) and jobs/workers.
//
// The lanes of 32-bit and 16-bit Windows and of drivers ("pe", "ne", "le") are the lane kit's (engine/lane_kit): this header asks it which of them takes a file
// (`kind_name`), what their results are filed under (`identity`, `digests`) and has it reconstruct and judge a module (`run`), judging in worker processes
// when the program has them. The old `Kind` registry holds the lanes that are not refactored yet.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/old_lane_kit.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/reconstruct.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/symbols/runtime_library.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py.hpp"
#include "py_os.hpp"

// <windows.h> (through py_os.hpp) defines `near` and `far`, which headers of ours use as names (decompile/c_text.hpp's Module16): no header of ours
// wants them as macros.
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif
#ifdef VOID
#undef VOID
#endif

namespace codebreaker::verify::reconstruct {

// who a candidate in the store came from
inline const py::str SOURCE = py::str("decompiler");
inline const verdicts::NotRun& NOT_RUN() { return verdicts::NOT_RUN(); }
inline const py::list<py::str>& GOOD() { return verdicts::GOOD(); }

// (verdict, reason, limits, detail) -- what a lane's judge says of one function
using Verdict = verdicts::NotRun;

// The decompiled module a lane prepares.
struct Prepared {
    py::str header;
    py::dict<py::str, py::str> texts;             // {name: C text}; empty for a module that is not made of separate texts
    py::list<py::str> order;                      // function names, by address
    py::dict<py::str, py::str> key;               // {name: address key}
    py::list<py::str> all_keys;                   // every function's key
    py::str sig;                                  // what the whole preparation was made from
    std::shared_ptr<void> lane;                   // the lane's own extras (opaque here)
};

using Context = std::shared_ptr<void>;            // the lane's build context handed to every judge
using Item = std::shared_ptr<void>;               // one function's thing to judge

// build()'s answer: (context, {name: item to judge}, {name: why it was not built})
struct Built {
    Context context;
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> dropped;
};

// One kind of module: what a lane provides.
class Kind {
  public:
    virtual ~Kind() = default;
    virtual py::str KIND() const = 0;
    virtual std::tuple<py::str, py::str> profile(const py::str& module_path) const = 0;   // (digest, settings); "" : the settings in force
    virtual Prepared prepare(const py::str& module_path, const py::str& work) const = 0;
    virtual Built build(const Prepared& m, const py::str& module_path, const py::str& work) const = 0;
    virtual Verdict judge(const Context& context, const Item& item) const = 0;
    virtual std::optional<py::str> sha(const py::str& module_path) const { (void)module_path; return std::nullopt; }
    virtual bool PER_FUNCTION() const { return false; }
    virtual bool LIBRARY() const { return true; }
    virtual std::optional<py::str> SOURCE_REV() const { return std::nullopt; }
    // How the judge's inputs go to a worker process: text a worker can read back. A lane that does not say judges in the caller's process.
    virtual std::optional<std::string> encode_context(const Context& context) const { (void)context; return std::nullopt; }
    virtual Context decode_context(const std::string& text) const { (void)text; return nullptr; }
    virtual std::string encode_item(const Item& item) const { (void)item; return ""; }
    virtual Item decode_item(const std::string& text) const { (void)text; return nullptr; }
};

// What the old world installs (see the top): which kind of module a file is, and the engine's `run` (nullopt: the engine is not on for it).
struct Hooks {
    std::function<std::optional<py::str>(const py::str& module_path)> kind_name;
    std::function<std::optional<std::tuple<py::str, py::str>>(const py::str& module_path, const std::shared_ptr<Kind>& kind,
                                                              const std::shared_ptr<engine::native_engine::FunctionStore>& fs,
                                                              const std::function<void(const py::str&)>& log)> engine_run;
};
inline Hooks& hooks() {
    static Hooks h;
    return h;
}

namespace detail {

namespace nv = cb::verify;

inline std::string text_of(const py::str& s) { return s.utf8(); }

inline nv::Judgement judgement_of(const Verdict& v) {
    nv::Judgement j;
    j.verdict = nv::parse_verdict(std::get<0>(v).utf8()).value_or(nv::Verdict::Inconclusive);
    j.reason = std::get<1>(v).utf8();
    for (const py::str& l : std::get<2>(v)) j.limits.push_back(l.utf8());
    j.detail = std::get<3>(v).utf8();
    return j;
}

// The new lane over an old Kind: its preparation carries the old one, its build keeps the old context and items.
class Adapter final : public cb::verify::lanes::Kind {
  public:
    struct OldPrepared final : cb::verify::lanes::Prepared {
        codebreaker::verify::reconstruct::Prepared old;   // (qualified: `Prepared` alone is the base's name in here)
    };
    struct OldBuilt {
        Context context;
        py::dict<py::str, Item> items;
    };

    Adapter(std::string name, std::shared_ptr<codebreaker::verify::reconstruct::Kind> old) : name_(std::move(name)), old_(std::move(old)) {}

    const std::string& name() const override { return name_; }
    cb::verify::lanes::Profile profile(const std::filesystem::path& module_path) const override {
        return cb::compat::guard([&] {
            auto [digest, settings] = old_->profile(cb::compat::to_py(module_path.generic_string()));
            return cb::verify::lanes::Profile{digest.utf8(), settings.utf8()};
        });
    }
    std::unique_ptr<cb::verify::lanes::Prepared> prepare(const std::filesystem::path& module_path, const std::filesystem::path& work) const override {
        auto out = std::make_unique<OldPrepared>();
        out->old = old_->prepare(cb::compat::to_py(module_path.generic_string()), cb::compat::legacy::native(work));
        out->header = out->old.header.utf8();
        for (const py::str& n : out->old.order) out->order.push_back(n.utf8());
        for (const auto& kv : out->old.key.items()) out->key[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
        for (const py::str& k : out->old.all_keys) out->all_keys.push_back(k.utf8());
        for (const auto& kv : out->old.texts.items()) out->texts[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
        out->sig = out->old.sig.utf8();
        return out;
    }
    cb::verify::lanes::Built build(const cb::verify::lanes::Prepared& p, const std::filesystem::path& module_path,
                                   const std::filesystem::path& work) const override {
        const OldPrepared& op = static_cast<const OldPrepared&>(p);
        codebreaker::verify::reconstruct::Built ob = old_->build(op.old, cb::compat::to_py(module_path.generic_string()), cb::compat::legacy::native(work));
        auto kept = std::make_shared<OldBuilt>(OldBuilt{ob.context, ob.items});
        {
            std::lock_guard<std::mutex> g(mutex_);
            last_ = kept;
        }
        cb::verify::lanes::Built out;
        for (const auto& kv : ob.items.items()) out.items.push_back(std::get<0>(kv).utf8());
        for (const auto& kv : ob.dropped.items()) out.missing[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
        const std::shared_ptr<codebreaker::verify::reconstruct::Kind> old = old_;
        out.judge = [old, kept](const std::string& name) { return judgement_of(old->judge(kept->context, kept->items.at(py::str(name)))); };
        return out;
    }

    const std::shared_ptr<codebreaker::verify::reconstruct::Kind>& old_kind() const { return old_; }
    // The build the last build() made.
    std::shared_ptr<OldBuilt> last_built() const {
        std::lock_guard<std::mutex> g(mutex_);
        return last_;
    }

  private:
    std::string name_;
    std::shared_ptr<codebreaker::verify::reconstruct::Kind> old_;
    mutable std::mutex mutex_;
    mutable std::shared_ptr<OldBuilt> last_;
};

// The items judged by worker processes: shares of them, `count` running at once, each a process of this program that reads its share from a
// file and answers in another. An item whose worker gave no answer is judged "not run", as a dead worker's was.
inline std::map<std::string, nv::Judgement> judge_in_processes(const codebreaker::verify::reconstruct::Kind& kind, const std::string& lane,
                                                                 const std::string& context_text, const Adapter::OldBuilt& built,
                                                                 const std::vector<std::string>& names) {
    namespace W = codebreaker::jobs::workers;
    const int64_t workers = W::count();
    const int64_t count = std::min<int64_t>(static_cast<int64_t>(names.size()), workers * 4);
    py::list<py::list<std::string>> shares;
    for (int64_t k = 0; k < count; ++k) {
        py::list<std::string> share;
        for (int64_t i = k; i < static_cast<int64_t>(names.size()); i += count) share.append(names[static_cast<size_t>(i)]);
        shares.append(share);
    }
    auto one = [&](const py::list<std::string>& share) -> std::map<std::string, nv::Judgement> {
        const std::string dir = W::scratch_folder();
        W::write_file(dir + "\\context.txt", context_text);
        std::string items;
        for (const std::string& n : share) items += W::Enc().s(n).s(kind.encode_item(built.items.at(py::str(n)))).text() + "\n";
        W::write_file(dir + "\\items.txt", items);
        W::run({"judge", W::hex(lane), dir + "\\context.txt", dir + "\\items.txt", dir + "\\answer.txt"});
        std::map<std::string, nv::Judgement> out;
        const std::string answer = W::read_file(dir + "\\answer.txt");
        size_t at = 0;
        while (at < answer.size()) {
            size_t nl = answer.find('\n', at);
            if (nl == std::string::npos) nl = answer.size();
            if (nl > at) {
                W::Dec d(answer.substr(at, nl - at));
                const std::string key = d.s();
                const py::str word(d.s()), reason(d.s());
                const std::string limits = d.s();
                const py::str detail(d.s());
                py::list<py::str> lim;
                size_t p = 0;
                while (p < limits.size()) {
                    size_t c2 = limits.find(',', p);
                    if (c2 == std::string::npos) c2 = limits.size();
                    if (c2 > p) lim.append(py::str(limits.substr(p, c2 - p)));
                    p = c2 + 1;
                }
                out[key] = judgement_of(Verdict(word, reason, lim, detail));
            }
            at = nl + 1;
        }
        std::error_code ec;
        std::filesystem::remove_all(dir, ec);
        for (const std::string& n : share)
            if (!out.contains(n)) out[n] = nv::not_run();
        return out;
    };
    std::map<std::string, nv::Judgement> all;
    for (const auto& part : codebreaker::jobs::pool::threads(one, shares, workers)) all.insert(part.begin(), part.end());
    return all;
}

inline std::mutex& registry_mutex() {
    static std::mutex m;
    return m;
}
// KINDS: the name results are filed under -> the lane (in registration order)
inline std::vector<std::pair<py::str, std::shared_ptr<codebreaker::verify::reconstruct::Kind>>>& registry() {
    static std::vector<std::pair<py::str, std::shared_ptr<codebreaker::verify::reconstruct::Kind>>> r;
    return r;
}
// the same lanes as the new reconstructor's registry
inline cb::verify::reconstruct::Kinds& new_kinds() {
    static cb::verify::reconstruct::Kinds k;
    return k;
}

inline cb::verify::reconstruct::KindTraits traits_of(const std::shared_ptr<codebreaker::verify::reconstruct::Kind>& kind) {
    cb::verify::reconstruct::KindTraits t;
    t.per_function = kind->PER_FUNCTION();
    t.identifies_library = kind->LIBRARY();
    if (const auto rev = kind->SOURCE_REV()) t.source_rev = rev->utf8();
    t.sha = [kind](const std::filesystem::path& path) -> std::optional<std::string> {
        const auto own = kind->sha(cb::compat::to_py(path.generic_string()));
        return own ? std::optional<std::string>(own->utf8()) : std::nullopt;
    };
    if (kind->PER_FUNCTION()) {
        t.object_digests = [](const std::filesystem::path& work) {
            std::map<std::string, std::string> out;
            const py::str key = cb::compat::legacy::native(work);
            if (codebreaker::build::functions::OBJECTS.contains(key))
                for (const auto& kv : codebreaker::build::functions::OBJECTS.at(key).items()) out[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
            return out;
        };
    }
    return t;
}

// The lane kit's lanes as the reconstruction path's Kinds: "pe", "ne" and "le", for the files of those classes.
inline cb::verify::reconstruct::Kinds& kit_kinds() {
    static cb::verify::reconstruct::Kinds kinds = [] {
        cb::verify::reconstruct::Kinds made;
        cb::compat::old_lane_kit().add_lanes(made);
        return made;
    }();
    return kinds;
}
// Which of them takes the file (by its class), or nullopt.
inline std::optional<std::string> kit_kind(const py::str& module_path) {
    const std::optional<py::str> lane = base::corpus::lane(module_path);
    if (!lane) return std::nullopt;
    const std::string name = lane->utf8();
    return (name == "pe" || name == "ne" || name == "le") && kit_kinds().find(name) != nullptr ? std::optional<std::string>(name) : std::nullopt;
}

}  // namespace detail

// the lanes of every platform
inline void register_kind(const py::str& name, const std::shared_ptr<Kind>& kind) {
    std::lock_guard<std::mutex> g(detail::registry_mutex());
    bool found = false;
    for (auto& e : detail::registry())
        if (e.first == name) {
            e.second = kind;
            found = true;
        }
    if (!found) detail::registry().emplace_back(name, kind);
    detail::new_kinds().add(std::make_shared<detail::Adapter>(name.utf8(), kind), detail::traits_of(kind));
}

inline py::dict<py::str, py::str> kinds() {
    py::dict<py::str, py::str> out;
    for (const auto& e : detail::registry()) out.set(e.first, e.second->KIND());
    return out;
}

inline std::shared_ptr<Kind> kind_named(const py::str& name) {
    for (const auto& e : detail::registry())
        if (e.first == name) return e.second;
    throw py::KeyError(name.utf8());
}

// Which kind a file is: 'pe', 'ne', 'powerpc', 'le', 'dos', 'java' -- or None when nothing reconstructs it.
inline std::optional<py::str> kind_name(const py::str& module_path) {
    if (hooks().kind_name) return hooks().kind_name(module_path);
    const std::optional<std::string> kit = detail::kit_kind(module_path);
    return kit ? std::optional<py::str>(py::str(*kit)) : std::nullopt;
}

inline std::shared_ptr<Kind> kind_of(const py::str& module_path) {
    std::optional<py::str> name = kind_name(module_path);
    if (!name) {
        std::optional<py::str> lane = base::corpus::lane(module_path);
        throw py::ValueError(py::percent("%s: no way to reconstruct a %s file yet", py::os::path::basename(module_path),
                                         lane && !lane->empty() ? *lane : py::str("non-executable")).utf8());
    }
    return kind_named(*name);
}

// What a module's results are filed under: the kind's own `sha`, else the file's sha256.
inline py::str module_sha(const Kind& kind, const py::str& module_path) {
    std::optional<py::str> own = kind.sha(module_path);
    std::optional<py::str> got = own ? own : base::corpus::sha(module_path);
    return got ? *got : py::str("");
}

namespace detail {
// The new reconstructor over the old world's pieces, writing to the store `fs` is (the same files).
inline cb::verify::reconstruct::Services services_over(cb::engine::FunctionStore& store, const std::function<void(const py::str&)>& log,
                                                       const std::function<std::optional<std::string>(const std::filesystem::path&)>& sha_of) {
    cb::verify::reconstruct::Services s{.kinds = new_kinds(), .store = store};
    s.kind_of_file = [](const std::filesystem::path& p) -> std::optional<std::string> {
        const auto n = kind_name(cb::compat::to_py(p.generic_string()));
        return n ? std::optional<std::string>(n->utf8()) : std::nullopt;
    };
    s.content_sha = sha_of;
    s.scratch = cb::compat::legacy::path_of(codebreaker::build::watcom::_scratch(py::list<py::str>()));
    s.identify = [](const std::filesystem::path& p) {
        std::map<std::string, std::pair<std::string, std::string>> found;
        const py::str path = cb::compat::to_py(p.generic_string());
        for (const auto& kv : codebreaker::symbols::runtime_library::identify(path).items())
            found[codebreaker::symbols::names::key(path, std::get<0>(kv)).utf8()] = {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()};
        return found;
    };
    s.decompiler_rev = std::to_string(codebreaker::decompile::decompiler::REV);
    s.judge_threads = py::os::environ_::get(py::str("CB_JUDGE_THREADS")) ? static_cast<int>(codebreaker::jobs::pool::default_jobs()) : 1;
    s.judge_batch = [](const cb::verify::reconstruct::Services::JudgeBatch& batch) -> std::optional<std::map<std::string, nv::Judgement>> {
        namespace W = codebreaker::jobs::workers;
        const cb::verify::lanes::Kind& lane = batch.kind;
        const std::vector<std::string>& names = batch.names;
        const auto& adapter = static_cast<const Adapter&>(lane);
        const auto built = adapter.last_built();
        const std::optional<std::string> context_text = built && W::enabled() ? adapter.old_kind()->encode_context(built->context) : std::nullopt;
        if (!(context_text && names.size() >= 4 && W::count() > 1)) return std::nullopt;   // judged in this process, as the new run does
        return judge_in_processes(*adapter.old_kind(), lane.name(), *context_text, *built, names);
    };
    s.log = [log](std::string_view line) { log(cb::compat::to_py(line)); };
    return s;
}
}  // namespace detail

// (content hash, profile digest) -- what results for this module are filed under.
inline std::tuple<py::str, py::str> identity(const py::str& module_path) {
    if (const std::optional<std::string> kit = detail::kit_kind(module_path)) {
        const cb::verify::lanes::Kind& lane = *detail::kit_kinds().find(*kit);
        const std::optional<py::str> sha = base::corpus::sha(module_path);
        return {sha ? *sha : py::str(""), py::str(cb::compat::guard([&] { return lane.profile(cb::compat::legacy::path_of(module_path)); }).digest)};
    }
    std::shared_ptr<Kind> kind = kind_of(module_path);
    return {module_sha(*kind, module_path), std::get<0>(kind->profile(module_path))};
}

// {kind: digest of the verifier settings in force now} (None for a kind whose tools are not set up on this machine)
inline py::dict<py::str, std::optional<py::str>> digests() {
    static py::dict<py::str, std::optional<py::str>> cache;
    static size_t known = 0;
    if (cache.empty() || known != detail::registry().size()) {
        cache = py::dict<py::str, std::optional<py::str>>();
        for (const std::string& name : detail::kit_kinds().names()) {
            try {
                cache.set(py::str(name), std::optional<py::str>(py::str(detail::kit_kinds().find(name)->profile({}).digest)));
            } catch (const std::exception&) {   // (a lane whose tools are not set up has no digest)
                cache.set(py::str(name), std::optional<py::str>());
            }
        }
        for (const auto& e : detail::registry()) {
            try {
                cache.set(e.first, std::optional<py::str>(std::get<0>(e.second->profile(py::str("")))));
            } catch (const py::ImportError&) {
                throw;
            } catch (const std::exception&) {
                cache.set(e.first, std::optional<py::str>());
            }
        }
        known = detail::registry().size();
    }
    return cache;
}

// Names in `built` that call a dropped function (now a stand-in), directly or through others.
inline py::set<py::str> _reaches(const py::dict<py::str, py::str>& texts, const py::list<py::str>& built, const py::dict<py::str, py::str>& dropped) {
    std::unordered_map<std::string, std::string> t;
    for (const py::str& n : built) t[n.utf8()] = texts.at(n).utf8();
    std::vector<std::string> b;
    for (const py::str& n : built) b.push_back(n.utf8());
    std::set<std::string> d;
    for (const py::str& n : dropped) d.insert(n.utf8());
    py::set<py::str> out;
    for (const std::string& n : cb::verify::reconstruct::reaches(t, b, d)) out.add(py::str(n));
    return out;
}

// {name: what that function's verdict was reached for}; `own` {name: digest of the function's compiled code} stands in for the text.
inline py::dict<py::str, py::str> signatures(const Prepared& m, const std::optional<py::str>& whole,
                                             const std::optional<py::dict<py::str, py::str>>& own = std::nullopt) {
    cb::verify::lanes::Prepared p;
    p.header = m.header.utf8();
    for (const py::str& n : m.order) p.order.push_back(n.utf8());
    for (const auto& kv : m.texts.items()) p.texts[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    std::map<std::string, std::string> objects;
    if (own)
        for (const auto& kv : own->items()) objects[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    py::dict<py::str, py::str> out;
    for (const auto& kv : cb::verify::reconstruct::signatures(p, whole ? whole->utf8() : std::string(), own ? &objects : nullptr)) out.set(py::str(kv.first), py::str(kv.second));
    return out;
}

// Reconstruct and judge a module. -> (module hash, profile digest). fs: None -> the default store; log: where the one-line report goes.
inline std::tuple<py::str, py::str> run(const py::str& module_path, const std::shared_ptr<engine::native_engine::FunctionStore>& fs_in = nullptr,
                                        const std::function<void(const py::str&)>& log_in = nullptr) {
    const std::function<void(const py::str&)> log = log_in ? log_in : [](const py::str& s) { py::print(s); };
    if (detail::kit_kind(module_path)) {   // a lane of the kit's: the new reconstructor over its services, writing to the store `fs` is
        std::shared_ptr<engine::native_engine::FunctionStore> store_in = fs_in;
        if (!store_in) store_in = std::make_shared<engine::native_engine::FunctionStore>(store::functions::default_path());
        return cb::compat::guard([&] {
            cb::engine::FunctionStore store(cb::compat::legacy::path_of(store_in->path), cb::compat::legacy::path_of(store_in->folder));
            cb::verify::reconstruct::Services services = cb::compat::old_lane_kit().reconstruct_services(detail::kit_kinds(), store);
            services.log = [log](std::string_view line) { log(cb::compat::to_py(line)); };
            const cb::verify::reconstruct::Reconstructor rec(std::move(services));
            const cb::verify::reconstruct::Outcome o = rec.run(cb::compat::legacy::path_of(module_path));
            return std::tuple<py::str, py::str>{py::str(o.sha), py::str(o.profile)};
        });
    }
    std::shared_ptr<Kind> kind = kind_of(module_path);
    std::shared_ptr<engine::native_engine::FunctionStore> fs = fs_in;
    if (!fs) fs = std::make_shared<engine::native_engine::FunctionStore>(store::functions::default_path());
    const auto env = py::os::environ_::get(py::str("CB_ENGINE"));
    if ((!env || *env != py::str("0")) && hooks().engine_run) {
        if (auto done = hooks().engine_run(module_path, kind, fs, log)) return *done;
    }
    return cb::compat::guard([&] {
        cb::engine::FunctionStore store(cb::compat::legacy::path_of(fs->path), cb::compat::legacy::path_of(fs->folder));
        auto sha_of = [](const std::filesystem::path& p) -> std::optional<std::string> {
            const auto s = base::corpus::sha(cb::compat::to_py(p.generic_string()));
            return s ? std::optional<std::string>(s->utf8()) : std::nullopt;
        };
        cb::verify::reconstruct::Reconstructor rec(detail::services_over(store, log, sha_of));
        const cb::verify::reconstruct::Outcome o = rec.run(cb::compat::legacy::path_of(module_path));
        return std::tuple<py::str, py::str>{py::str(o.sha), py::str(o.profile)};
    });
}

// A judge worker process's share (jobs/workers): args = lane name, context file, items file, answer file.
inline int judge_worker(const std::vector<std::string>& args) {
    namespace W = codebreaker::jobs::workers;
    if (args.size() < 4) return 2;
    const std::shared_ptr<Kind> kind = kind_named(py::str(W::unhex(args[0])));
    const Context context = kind->decode_context(W::read_file(args[1]));
    const std::string text = W::read_file(args[2]);
    std::string out;
    size_t at = 0;
    while (at < text.size()) {
        size_t nl = text.find('\n', at);
        if (nl == std::string::npos) nl = text.size();
        if (nl > at) {
            W::Dec d(text.substr(at, nl - at));
            const std::string key = d.s();
            const Item item = kind->decode_item(d.s());
            Verdict v = NOT_RUN();
            try {
                v = kind->judge(context, item);
            } catch (const std::exception&) {
                v = NOT_RUN();
            }
            std::string limits;
            for (const py::str& l : std::get<2>(v)) limits += (limits.empty() ? "" : ",") + l.utf8();
            out += W::Enc().s(key).s(std::get<0>(v).utf8()).s(std::get<1>(v).utf8()).s(limits).s(std::get<3>(v).utf8()).text() + "\n";
        }
        at = nl + 1;
    }
    W::write_file(args[3], out);
    return 0;
}

// The old lanes' judge shares: registered with the program's worker kinds (jobs/workers) once this header is in the program.
inline const bool judge_worker_registered = [] {
    codebreaker::jobs::workers::kinds().add("judge", [](const cb::jobs::Args& args) { return judge_worker(args); });
    return true;
}();

}  // namespace codebreaker::verify::reconstruct
