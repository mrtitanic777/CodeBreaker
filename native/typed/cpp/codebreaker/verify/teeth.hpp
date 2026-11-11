// codebreaker/verify/teeth.hpp -- COMPAT SHIM over cb/verify/teeth.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/teeth.{hpp,cpp} now: the five breaks, the round loop, the report and the declared
// `teeth` command, with the module and its judge behind a `Lane`. This header keeps the old py:: API for the ports that
// still use it (verify/lanes/driver's self-check uses plant/Round/Planted), and supplies the old lanes (the
// reconstruct kinds, the function store, the worker pool) to the new run as an adapter. No logic of teeth's own.
//
// `plant`'s `each` is called with the old Planted records, whose `outcome` is the word `outcome(said)` gave (the new
// loop has only caught/missed/other; the word is kept alongside).
//
// Not kept: the single-break helpers of the old file (_body, _constant, _comparison, _operator, _store, _arguments) --
// they were internal and nothing calls them; `breaks` runs them all.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/teeth.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "codebreaker/verify/verdicts.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::verify::teeth {

using Log = std::function<void(const py::str&)>;
inline void log_print(const py::str& line) { py::print(line); }

// [(kind of break, the function's text with it made)] -- every kind that applies.
inline py::list<std::tuple<py::str, py::str>> breaks(const py::str& text) {
    py::list<std::tuple<py::str, py::str>> out;
    for (const cb::verify::teeth::TextBreak& b : cb::verify::teeth::breaks(text.utf8()))
        out.append(std::make_tuple(cb::compat::to_py(cb::verify::teeth::word(b.kind)), cb::compat::to_py(b.text)));
    return out;
}

// 'old line  ->  new line' for the one line a break changed (or removed).
inline py::str _changed(const py::str& before, const py::str& after) {
    return cb::compat::to_py(cb::verify::teeth::changed(before.utf8(), after.utf8()));
}

// {name: every function it can reach, itself included}
inline py::dict<py::str, py::set<py::str>> _reach(const py::dict<py::str, py::str>& texts) {
    std::vector<cb::verify::teeth::NamedText> fs;
    for (const auto& kv : texts.items()) fs.push_back({std::get<0>(kv).utf8(), std::get<1>(kv).utf8()});
    const auto reaches = cb::verify::teeth::reach(fs);
    py::dict<py::str, py::set<py::str>> out;
    for (std::size_t i = 0; i < fs.size(); ++i) {
        py::set<py::str> mine;
        for (std::uint32_t j : reaches[i]) mine.add(cb::compat::to_py(fs[j].name));
        out.set(cb::compat::to_py(fs[i].name), mine);
    }
    return out;
}

// A function (or an entry point) and one break made in it: (name, what, made)
using Break = std::tuple<py::str, py::str, py::str>;
using Round = std::vector<Break>;

template <class Said>
struct Planted {
    py::str name, what, made;
    Said said;
    py::str outcome;
};

// The judge judged, whatever the lane. `rounds` gives the next round of breaks; judge(round) judges a round's together
// -> {name: what the judge said}; outcome(said) is "caught", "missed" or anything else (not counted). `each` is told,
// round by round, what came of it.
template <class Said>
void plant(const std::function<std::optional<Round>()>& rounds,
           const std::function<py::dict<py::str, Said>(const Round&)>& judge,
           const std::function<py::str(const Said&)>& outcome,
           const std::function<void(const std::vector<Planted<Said>>&)>& each) {
    namespace nv = cb::verify::teeth;
    Round current;                         // the round being judged, in the old shape
    std::vector<py::str> words;            // what the old `outcome` said of each of its attempts
    nv::plant<Said>(
        [&]() -> std::optional<nv::Round> {
            std::optional<Round> r = rounds();
            if (!r) return std::nullopt;
            current = *r;
            nv::Round out;
            for (const Break& b : *r) out.push_back({std::get<0>(b).utf8(), std::get<1>(b).utf8(), std::get<2>(b).utf8()});
            return out;
        },
        [&](const nv::Round& r) {
            py::dict<py::str, Said> said = judge(current);
            std::vector<Said> out;
            for (const nv::Attempt& a : r) out.push_back(said.at(cb::compat::to_py(a.name)));
            return out;
        },
        [&](const Said& s) {
            words.push_back(outcome(s));
            const py::str& w = words.back();
            return w == py::str("caught") ? nv::Outcome::Caught : w == py::str("missed") ? nv::Outcome::Missed : nv::Outcome::Other;
        },
        [&](std::span<const nv::Planted<Said>> planted) {
            std::vector<Planted<Said>> out;
            for (std::size_t i = 0; i < planted.size(); ++i)
                out.push_back({std::get<0>(current[i]), std::get<1>(current[i]), std::get<2>(current[i]), planted[i].said, words[i]});
            words.clear();
            each(out);
        });
}

// One function's result.
struct Result {
    py::str name;
    py::str was;                          // "verified" or "weak": what the judge had called it
    py::str made;                         // the kind of break
    py::str reason;
    py::str now;                          // "caught", "survived", "not judged" or "does not compile"
    bool compiled = true;                 // false: the Python's dict has no `change` and `limits`
    py::str change;                       // 'old line  ->  new line'
    py::str limits;                       // the verdict's limits, joined by ","
};

struct Caught {
    py::str group;                        // "verified", "weak" or "by break: <kind>"
    int64_t caught = 0;
    int64_t of = 0;
};

struct Report {
    py::str module;
    py::str kind;
    int64_t rounds = 0;
    py::list<Result> results;
    py::list<Caught> caught;              // sorted by group
};

namespace compat_detail {

namespace R = codebreaker::verify::reconstruct;
namespace V = codebreaker::verify::verdicts;

// The old lanes (a reconstruct Kind over the function store) as a teeth Lane: what the Python's `run` did before it broke
// anything -- make sure the module has been judged, prepare it, find the functions that passed -- and, for a round, build it
// with the broken texts and judge them.
class OldLane : public cb::verify::teeth::Lane {
  public:
    OldLane(const py::str& module_path, const Log& log) : module_path_(module_path) {
        kind_ = R::kind_of(module_path);
        if (!kind_->PER_FUNCTION())
            throw cb::UsageError(py::os::path::basename(module_path).utf8() +
                                 ": this kind of module is rebuilt whole and compared byte for byte; there is no judge of behaviour to test");
        auto fs = store::functions::open();
        auto [sha, digest] = R::run(module_path, fs, log);      // make sure it has been judged
        name_ = py::os::path::basename(module_path).lower();
        work_ = build::watcom::_scratch(py::list<py::str>{py::str("teeth"), kind_->KIND(), name_});
        m_ = kind_->prepare(module_path, work_);
        key_of_ = m_.key.copy();
        py::dict<py::str, cb::verify::Class> passed;
        for (const auto& kv : fs->best(sha, digest).items()) {
            const auto& klass = std::get<1>(kv).klass;
            if (std::holds_alternative<py::str>(klass)) {
                const auto c = cb::verify::parse_class(std::get<py::str>(klass).utf8());
                if (c == cb::verify::Class::Verified || c == cb::verify::Class::Weak) passed.set(std::get<0>(kv), *c);
            }
        }
        original_ = m_.texts.copy();
        auto add = [&](const py::str& n) {
            if (!original_.contains(n) || index_.count(n.utf8())) return;
            cb::verify::teeth::Function f;
            f.name = n.utf8();
            f.text = original_.at(n).utf8();
            if (key_of_.contains(n) && passed.contains(key_of_.at(n))) f.passed = passed.at(key_of_.at(n));
            index_[f.name] = functions_.size();
            functions_.push_back(std::move(f));
        };
        for (const py::str& n : m_.order) add(n);
        for (const py::str& n : original_.keys()) add(n);
    }
    ~OldLane() override { put_texts(original_); }

    std::string kind() const override { return kind_->KIND().utf8(); }
    std::string module_name() const override { return name_.utf8(); }
    const std::vector<cb::verify::teeth::Function>& functions() const override { return functions_; }

    std::vector<cb::verify::teeth::Said> judge(std::span<const cb::verify::teeth::Replacement> broken) override {
        py::dict<py::str, py::str> texts = original_.copy();
        for (const auto& r : broken) texts.set(cb::compat::to_py(functions_[r.function].name), cb::compat::to_py(r.text));
        put_texts(texts);
        R::Built built = kind_->build(m_, module_path_, py::os::path::join(work_, py::percent(py::str("round%d"), number_++)));
        py::list<std::tuple<py::str, R::Item>> todo;
        for (const auto& r : broken) {
            const py::str n = cb::compat::to_py(functions_[r.function].name);
            if (built.items.contains(n)) todo.append(std::make_tuple(key_of_.at(n), built.items.at(n)));
        }
        jobs::pool::Options<std::tuple<py::str, R::Item>, R::Verdict> opts;
        opts.crashed = [](const std::tuple<py::str, R::Item>&) { return R::NOT_RUN(); };
        opts.jobs = 1;                                            // (the emulators keep per-process state)
        const R::Kind& kref = *kind_;
        auto judged = jobs::pool::run_first(
            [&kref](const R::Context& context, const std::tuple<py::str, R::Item>& item) { return kref.judge(context, std::get<1>(item)); }, todo,
            built.context, opts);
        std::vector<cb::verify::teeth::Said> out;
        for (const auto& r : broken) {
            const py::str n = cb::compat::to_py(functions_[r.function].name);
            cb::verify::teeth::Said said;
            if (built.items.contains(n)) {
                const R::Verdict& v = judged.at(key_of_.at(n));
                said.verdict = cb::verify::parse_verdict(std::get<0>(v).utf8());
                said.reason = std::get<1>(v).utf8();
                said.limits = cb::compat::from_py(std::get<2>(v));
            } else {
                said.reason = built.dropped.get(n, py::str("")).utf8();
            }
            out.push_back(std::move(said));
        }
        return out;
    }

  private:
    // `m.texts = texts`. In the Python one object is the module the lane builds from and the one teeth reads; here the
    // lane's own module (a translate::l2::Made, the first base of the Module16 / Module32 / ModulePPC the lane keeps in
    // Prepared::lane) holds the texts it builds from, so they are set there as well as in the Prepared.
    void put_texts(const py::dict<py::str, py::str>& texts) {
        m_.texts = texts;
        const py::str k = kind_->KIND();
        if (m_.lane && (k == py::str("ne") || k == py::str("pe") || k == py::str("powerpc")))
            std::static_pointer_cast<translate::l2::Made>(m_.lane)->set_texts(texts);
    }

    py::str module_path_, name_, work_;
    std::shared_ptr<R::Kind> kind_;
    R::Prepared m_;
    py::dict<py::str, py::str> key_of_, original_;
    std::vector<cb::verify::teeth::Function> functions_;
    std::unordered_map<std::string, std::size_t> index_;
    int64_t number_ = 0;
};

inline py::str word_of(cb::verify::Class c) { return cb::compat::to_py(cb::verify::word(c)); }

inline Report to_old(const cb::verify::teeth::Report& r) {
    Report out;
    out.module = cb::compat::to_py(r.module);
    out.kind = cb::compat::to_py(r.kind);
    out.rounds = r.rounds;
    for (const auto& x : r.results) {
        Result o;
        o.name = cb::compat::to_py(x.name);
        o.was = word_of(x.was);
        o.made = cb::compat::to_py(x.made);
        o.reason = cb::compat::to_py(x.reason);
        o.now = cb::compat::to_py(cb::verify::teeth::word(x.now));
        o.compiled = x.now != cb::verify::teeth::Fate::DoesNotCompile;
        o.change = cb::compat::to_py(x.change);
        o.limits = cb::compat::to_py(x.limits);
        out.results.append(o);
    }
    for (const auto& g : r.caught) out.caught.append(Caught{cb::compat::to_py(g.name), static_cast<int64_t>(g.caught), static_cast<int64_t>(g.of)});
    return out;
}

}  // namespace compat_detail

// -> the report of a module: breaks the functions the judge passed, rebuilds, judges again.
inline Report run(const py::str& module_path, int64_t rounds = 3, const Log& log = log_print) {
    return cb::compat::guard([&] {
        compat_detail::OldLane lane(module_path, log);
        cb::verify::teeth::Log new_log;
        if (log) new_log = [&](std::string_view line) { log(cb::compat::to_py(line)); };
        return compat_detail::to_old(cb::verify::teeth::run(lane, static_cast<int>(rounds), new_log));
    });
}

// The `teeth` command over the old corpus and lanes: `main(argv)` takes what follows the command name.
inline int64_t main(const py::list<py::str>& argv) {
    namespace nv = cb::verify::teeth;
    nv::Services services;
    services.find_module = [](std::string_view name) -> std::optional<std::filesystem::path> {
        const py::str given = cb::compat::to_py(name);
        std::optional<py::str> path = py::os::path::isfile(given) ? std::optional<py::str>(given) : base::corpus::retail(given);
        if (!path || path->empty()) return std::nullopt;
        return std::filesystem::path(base::corpus::_readable(*path).utf8());   // (a .xex: its basefile, as the lanes read it)
    };
    services.open = [](const std::filesystem::path& p) -> std::unique_ptr<nv::Lane> {
        return std::make_unique<compat_detail::OldLane>(cb::compat::to_py(p.generic_string()), Log(log_print));
    };
    services.report_dir = std::filesystem::path(py::settings::get_str(py::str("state")).utf8()) / "teeth";
    std::vector<std::string> args = {"teeth"};
    for (const py::str& a : argv) args.push_back(a.utf8());
    return cb::compat::guard([&] { return cb::cli::Cli("codebreaker", {nv::command(services)}).run(args); });
}

}  // namespace codebreaker::verify::teeth
