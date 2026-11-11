// codebreaker/verify/lanes/image_lane.hpp -- COMPAT SHIM over cb/verify/lanes/image_lane.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/image_lane.{hpp,cpp} now (the lane of a program image written back as its
// assembly source: a cartridge, a tape block, a ROM). This header keeps the old py:: API the NES, Master System, Game Boy,
// Genesis, ZX Spectrum and MSX lanes are written against -- `Lane`, its `Prepared`/`Built`/`Context`/`Item` records, a
// `State` that is a py::dict, the effects of a run as a vector of variants -- and converts to and from the new lane.
// No logic of the lane's own.
//
// Differences the old callers can see, on purpose: the profile's digest is not the SHA-1 of Python's repr of the
// states; two runs that differ only in how many places they show are a divergent verdict "effects-differs" (the Python
// raised StopIteration); an address key longer than its format's digits is as before.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/core/text.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/verify/lanes/image_lane.hpp"
#include "py.hpp"

namespace codebreaker::verify::lanes::image_lane {

inline const py::str SAME = py::str(std::string(cb::verify::lanes::kSameImage));   // the cartridge built from this source is the original, byte for byte

using State = py::dict<py::str, int64_t>;
using Effect = std::variant<int64_t, py::str, py::bytes, py::list<py::str>>;
using Effects = std::vector<Effect>;

struct Function {
    py::str name;
};

// what the packager's writer returns (a dict)
struct Written {
    std::optional<py::str> source;                          // the assembly source file written
    std::optional<py::dict<int64_t, Function>> functions;   // address -> function
    std::optional<bool> linked;                             // the program was assembled and linked
    std::optional<py::list<py::str>> notes;                 // what the build said, the last line last
    std::optional<bool> same;                               // the built program is the original, byte for byte
    std::optional<py::str> built;                           // the built file
};

using Writer = std::function<Written(const py::str& module_path, const py::str& work)>;

struct Prepared {
    Written written;
    py::str header;                                    // the source text (read as latin-1)
    py::list<py::str> order;                           // function names, by address
    py::dict<py::str, py::str> key;                    // name -> its address as the store's keys write it
    py::list<py::str> all_keys;
    py::dict<py::str, py::str> texts;
    py::dict<py::str, int64_t> by_name;                // name -> address
    py::str sig;
    std::shared_ptr<const cb::verify::lanes::Prepared> native;   // (the new lane's own, which build() asks of)
};

// what judge() is given for the whole program: (same, original, rebuilt)
struct Context {
    bool same;
    py::str original;
    py::str rebuilt;
};
// a function's item: (its key, its address)
using Item = std::tuple<py::str, int64_t>;

struct Built {
    std::optional<Context> context;                    // None: it did not build
    py::dict<py::str, Item> items;
    py::dict<py::str, py::str> missing;                // {name: why it is not in the build}
};

// (verdict, what differs, [], text)
using Verdict = std::tuple<py::str, py::str, py::list<py::str>, py::str>;

namespace detail {
namespace nv = cb::verify::lanes;

inline State old_state(const nv::State& s) {
    State out;
    for (const auto& [k, v] : s) out.set(cb::compat::to_py(k), v);
    return out;
}
inline nv::State new_state(const State& s) {
    nv::State out;
    for (const auto& kv : s.items()) out[std::get<0>(kv).utf8()] = std::get<1>(kv);
    return out;
}
inline Written old_written(const nv::Written& w) {
    Written out;
    if (!w.source.empty()) out.source = cb::compat::to_py(w.source.generic_string());
    py::dict<int64_t, Function> fns;
    for (const auto& [address, name] : w.functions) fns.set(static_cast<int64_t>(address), Function{cb::compat::to_py(name)});
    out.functions = fns;
    out.linked = w.linked;
    out.notes = cb::compat::to_py(w.notes);
    out.same = w.same;
    if (!w.built.empty()) out.built = cb::compat::to_py(w.built.generic_string());
    return out;
}
inline nv::Written new_written(const Written& w) {
    nv::Written out;
    if (w.source) out.source = w.source->utf8();
    if (w.functions)
        for (const auto& kv : w.functions->items()) out.functions[static_cast<std::uint32_t>(std::get<0>(kv))] = std::get<1>(kv).name.utf8();
    out.linked = w.linked.value_or(false);
    if (w.notes) out.notes = cb::compat::from_py(*w.notes);
    out.same = w.same.value_or(false);
    if (w.built) out.built = w.built->utf8();
    return out;
}
inline nv::Effects new_effects(const Effects& in) {
    nv::Effects out;
    for (const Effect& e : in)
        out.push_back(std::visit(
            [](const auto& v) -> nv::Effect {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int64_t>) return v;
                else if constexpr (std::is_same_v<T, py::str>) return v.utf8();
                else if constexpr (std::is_same_v<T, py::bytes>) return cb::compat::from_py(v);
                else return cb::compat::from_py(v);
            },
            e));
    return out;
}
// "%04x" -> 4: the digits of the address in a key
inline unsigned key_digits(const py::str& format) {
    const std::string f = format.utf8();
    if (f.size() >= 4 && f[0] == '%' && f[1] == '0' && f.back() == 'x') {
        try { return static_cast<unsigned>(std::stoul(f.substr(2, f.size() - 3))); } catch (const std::exception&) {}
    }
    return 4;
}
}  // namespace detail

class Lane {
  public:
    using EffectsFn = std::function<Effects(const py::str& subject, int64_t entry, const State& state)>;
    using StartFn = std::function<py::str(const State& state)>;
    using SubjectsFn = std::function<std::tuple<py::str, py::str>(const py::str& module_path, const Written& written)>;

    // kind: the name results are filed under (its platform's: the writer is that platform's packager); rev:
    // bump when what a verdict depends on changes; states: where each run starts (part of every verdict's
    // profile); effects(subject, entry, state) -> a tuple; what: a name for each place in that tuple;
    // start(state) -> the state, as a verdict's detail says it; platform(): the packager's writer.
    Lane(const py::str& kind, int64_t rev, const py::list<State>& states, const EffectsFn& effects,
         const py::list<py::str>& what, const StartFn& start, const std::function<Writer()>& platform,
         const SubjectsFn& subjects = nullptr, const py::str& key = py::str("%04x"), const py::str& same = image_lane::SAME)
        : KIND(kind), REV(rev), SAME(same), platform_(platform), subjects_(subjects) {
        cb::verify::lanes::ImageLaneSpec spec;
        spec.kind = kind.utf8();
        spec.rev = static_cast<int>(rev);
        for (const State& s : states.vec()) spec.states.push_back(detail::new_state(s));
        spec.effects = [effects](const std::filesystem::path& subject, std::uint32_t entry, const detail::nv::State& state) {
            return detail::new_effects(effects(cb::compat::to_py(subject.generic_string()), static_cast<int64_t>(entry), detail::old_state(state)));
        };
        spec.what = cb::compat::from_py(what);
        spec.start = [start](const detail::nv::State& state) { return start(detail::old_state(state)).utf8(); };
        const std::function<Writer()> platform_fn = platform;
        spec.writer = [platform_fn](const std::filesystem::path& module, const std::filesystem::path& out_dir) {
            return detail::new_written(platform_fn()(cb::compat::to_py(module.generic_string()), cb::compat::to_py(out_dir.generic_string())));
        };
        if (subjects)
            spec.subjects = [subjects](const std::filesystem::path& module, const detail::nv::Written& w) {
                const auto [original, rebuilt] = subjects(cb::compat::to_py(module.generic_string()), detail::old_written(w));
                return std::make_pair(std::filesystem::path(original.utf8()), std::filesystem::path(rebuilt.utf8()));
            };
        spec.key_digits = detail::key_digits(key);
        spec.same = same.utf8();
        lane_ = std::make_shared<cb::verify::lanes::ImageLane>(std::move(spec));
    }

    py::str KIND;
    int64_t REV;
    py::str SAME;

    Writer writer() const { return platform_(); }

    // (a digest of the settings a verdict depends on, the text it is made from)
    std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path = std::nullopt) const {
        const auto p = lane_->profile(module_path ? std::filesystem::path(module_path->utf8()) : std::filesystem::path());
        return {cb::compat::to_py(p.digest), cb::compat::to_py(p.settings)};
    }

    Prepared prepare(const py::str& module_path, const py::str& work) const {
        return cb::compat::guard([&] {
            std::shared_ptr<const cb::verify::lanes::Prepared> native =
                lane_->prepare(module_path.utf8(), std::filesystem::path(work.utf8()));
            const auto& m = static_cast<const cb::verify::lanes::ImageLane::PreparedImage&>(*native);
            Prepared out;
            out.written = detail::old_written(m.written);
            out.header = cb::compat::to_py(cb::text::from_latin1(cb::as_bytes(m.header)));   // (the Python read it as latin-1)
            out.order = cb::compat::to_py(m.order);
            for (const std::string& n : m.order) {
                out.key.set(cb::compat::to_py(n), cb::compat::to_py(m.key.at(n)));
                out.texts.set(cb::compat::to_py(n), cb::compat::to_py(m.texts.at(n)));
                out.by_name.set(cb::compat::to_py(n), static_cast<int64_t>(m.by_name.at(n)));
            }
            out.all_keys = cb::compat::to_py(m.all_keys);
            out.sig = cb::compat::to_py(m.sig);
            out.native = std::move(native);
            return out;
        });
    }

    Built build(const Prepared& m, const py::str& module_path, const py::str& work) const {
        return cb::compat::guard([&] {
            const cb::verify::lanes::Built nb = lane_->build(*m.native, module_path.utf8(), std::filesystem::path(work.utf8()));
            Built out;
            for (const auto& [name, why] : nb.missing) out.missing.set(cb::compat::to_py(name), cb::compat::to_py(why));
            if (nb.judge) {
                const auto subjects = lane_->subjects_of(module_path.utf8(), detail::new_written(m.written));
                out.context = Context{m.written.same.value_or(false), cb::compat::to_py(subjects.first.generic_string()),
                                      cb::compat::to_py(subjects.second.generic_string())};
                for (const std::string& n : nb.items)
                    out.items.set(cb::compat::to_py(n), Item(m.key.at(cb::compat::to_py(n)), m.by_name.at(cb::compat::to_py(n))));
            }
            return out;
        });
    }

    Verdict judge(const std::optional<Context>& context, const Item& item) const {
        if (!context) throw py::TypeError("cannot unpack non-iterable NoneType object");
        return cb::compat::guard([&] {
            const cb::verify::Judgement j = lane_->judge_function(context->same, context->original.utf8(), context->rebuilt.utf8(),
                                                                  static_cast<std::uint32_t>(std::get<1>(item)));
            return Verdict{cb::compat::to_py(cb::verify::word(j.verdict)), cb::compat::to_py(j.reason),
                           cb::compat::to_py(j.limits), cb::compat::to_py(j.detail)};
        });
    }

  private:
    std::function<Writer()> platform_;
    SubjectsFn subjects_;
    std::shared_ptr<cb::verify::lanes::ImageLane> lane_;
};

}  // namespace codebreaker::verify::lanes::image_lane
