// codebreaker/coverage/reach.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in
// native/cb/coverage/reach.{hpp,cpp}: what CodeBreaker made of every program it was given. This header keeps the old py::
// surface (the functions of reach.py the translated Python and the exe's command dispatcher reach by name) over it.
// Conversion and forwarding, plus the wiring of what the new module is handed (Services): the platform registry's answer
// (the hook below), the media volumes, the function store's tally and the measure command, all of which are still the old
// layer's here.
//   COMPAT(py): tally_of / open volumes -- store/functions, verify/reconstruct, formats/media (with the Xbox volumes);
//       remove when those are refactored and the app wires the new ones.
//
// Dropped with the shim (nothing else called them): Row's `extra` keys and to_value (a results file this program wrote holds
// no key it does not know), _PROGRAM_EXTS / _SKIP_FAMILIES / _CONTAINER_FORMATS (the new module's private tables).
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/coverage/reach.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/engine/native_engine.hpp"
#include "codebreaker/formats/media.hpp"
#include "codebreaker/store/functions.hpp"
#include "codebreaker/verify/reconstruct.hpp"
#include "py.hpp"
#include "py_settings.hpp"

namespace codebreaker::coverage::reach {

namespace nr = ::cb::coverage::reach;

// ---- hooks
// what platforms.identify(path) answers for a program of a platform (None: no platform recognises it)
struct PlatformInfo {
    py::str title, name, family, container;
    bool lane = false;                          // the platform has a lane (p.lane)
};
using PlatformIdentify = std::function<std::optional<PlatformInfo>(const py::str& path)>;
inline PlatformIdentify& platform_identify() {
    static PlatformIdentify h;
    return h;
}
// the command that measures one program (default: this executable, `measure PATH`)
inline std::function<py::list<py::str>(const py::str& path)>& measure_argv() {
    static std::function<py::list<py::str>(const py::str& path)> h;
    return h;
}
// time.time()
inline std::function<double()>& clock() {
    static std::function<double()> c;
    return c;
}

// ---- the records
struct Described {
    py::str found, platform;
    std::optional<py::str> lane;
    py::str family;
};

struct Judged {
    py::dict<py::str, int64_t> tally;
    double seconds = 0.0;
    py::str stopped;
};

struct Row {
    py::str found, platform;
    std::optional<py::str> lane;
    py::str family, path, origin;
    int64_t size = 0;
    py::str sha;
    bool picked = false;
    std::optional<Judged> judged;
    std::optional<py::str> tools;
};

using Log = std::function<void(const py::str&)>;

namespace detail {

namespace cm = ::cb::formats::media;
using ::cb::compat::from_py;
using ::cb::compat::to_py;

// An old volume (the media shim's, Xbox containers included) as the new module reads one.
class OldVolume : public cm::Volume {
  public:
    explicit OldVolume(std::shared_ptr<codebreaker::formats::media::Volume> v) : old_(std::move(v)) {
        label_ = from_py(old_->label);
        where_ = from_py(old_->where);
    }
    cm::VolumeKind kind() const override { return cm::VolumeKind::Iso9660; }
    const std::vector<cm::Entry>& files() override {
        if (!entries_) {
            entries_.emplace();
            try {
                for (const auto& e : old_->files()) {
                    olds_.push_back(e);
                    cm::Entry n;
                    n.path = from_py(e.path);
                    n.size = static_cast<std::uint64_t>(e.size);
                    n.is_dir = e.is_dir;
                    entries_->push_back(std::move(n));
                }
            } catch (const py::Exception& e) {
                throw ::cb::FormatError("media", 0, e.what());
            }
        }
        return *entries_;
    }
    ::cb::Bytes read(const cm::Entry& entry) override {
        try {
            return from_py(old_->read(olds_.at(static_cast<std::size_t>(&entry - entries_->data()))));
        } catch (const py::Exception& e) {
            throw ::cb::FormatError("media", 0, e.what());
        }
    }

  private:
    std::shared_ptr<codebreaker::formats::media::Volume> old_;
    std::optional<std::vector<cm::Entry>> entries_;
    std::vector<codebreaker::formats::media::Entry> olds_;
};

inline nr::Services services(const Log& log = nullptr) {
    nr::Services s;
    s.identify_platform = [](const std::filesystem::path& p) -> std::optional<nr::PlatformInfo> {
        if (!platform_identify()) throw py::ImportError("platforms");
        const auto found = platform_identify()(to_py(::cb::fs::to_text(p)));
        if (!found) return std::nullopt;
        return nr::PlatformInfo{from_py(found->title), from_py(found->name), from_py(found->family), from_py(found->container), found->lane};
    };
    s.open_volumes = [](const std::filesystem::path& p) {
        std::vector<std::unique_ptr<cm::Volume>> out;
        try {
            for (const auto& v : codebreaker::formats::media::volumes(to_py(::cb::fs::to_text(p)))) out.push_back(std::make_unique<OldVolume>(v));
        } catch (const py::Exception& e) {
            throw ::cb::FormatError("media", 0, e.what());
        }
        return out;
    };
    s.measure_command = [](const std::filesystem::path& p) {
        if (measure_argv()) return from_py(measure_argv()(to_py(::cb::fs::to_text(p))));
        return std::vector<std::string>{};   // (the new module's default: this executable)
    };
    s.tally = [](const std::filesystem::path& p) {
        namespace F = codebreaker::store::functions;
        const std::optional<py::str> retail = codebreaker::base::corpus::retail(to_py(::cb::fs::to_text(p)));
        if (!retail) throw ::cb::NotFoundError(p, 0, "no retail binary");
        const auto id = codebreaker::verify::reconstruct::identity(*retail);   // (filed as `measure` files it)
        auto fs = F::open();
        nr::Tally tally;
        try {
            for (const auto& kv : fs->tally(std::get<1>(id), std::get<0>(id)).items()) {
                tally[from_py(std::get<0>(kv))] = codebreaker::engine::native_engine::cell_int(std::get<1>(kv));
            }
        } catch (...) {
            fs->close();
            throw;
        }
        fs->close();
        return tally;
    };
    if (clock()) s.clock = clock();
    s.log = [log](std::string_view line) {
        if (log) {
            log(to_py(line));
        } else {
            py::print(to_py(line));
        }
    };
    s.state_dir = ::cb::fs::from_text(py::settings::get_str(py::str("state")).utf8());
    s.checkout_root = ::cb::fs::from_text(py::settings::checkout_root().utf8());
    return s;
}

inline Row to_old(const nr::Row& r) {
    Row o;
    o.found = to_py(r.found);
    o.platform = to_py(r.platform);
    if (r.lane) o.lane = to_py(*r.lane);
    o.family = to_py(r.family);
    o.path = to_py(r.path);
    o.origin = to_py(r.origin);
    o.size = static_cast<int64_t>(r.size);
    o.sha = to_py(r.sha);
    o.picked = r.picked;
    if (r.judged) {
        Judged j;
        for (const auto& [k, v] : r.judged->tally) j.tally.set(to_py(k), v);
        j.seconds = r.judged->seconds;
        j.stopped = to_py(r.judged->stopped);
        o.judged = std::move(j);
    }
    if (r.tools) o.tools = to_py(*r.tools);
    return o;
}

inline nr::Row to_new(const Row& o) {
    nr::Row r;
    r.found = from_py(o.found);
    r.platform = from_py(o.platform);
    if (o.lane) r.lane = from_py(*o.lane);
    r.family = from_py(o.family);
    r.path = from_py(o.path);
    r.origin = from_py(o.origin);
    r.size = static_cast<std::uint64_t>(o.size);
    r.sha = from_py(o.sha);
    r.picked = o.picked;
    if (o.judged) {
        nr::Judged j;
        for (const auto& kv : o.judged->tally.items()) j.tally[from_py(std::get<0>(kv))] = std::get<1>(kv);
        j.seconds = o.judged->seconds;
        j.stopped = from_py(o.judged->stopped);
        r.judged = std::move(j);
    }
    if (o.tools) r.tools = from_py(*o.tools);
    return r;
}

}  // namespace detail

// (title, cpu) for a program of a platform CodeBreaker has no lane for, else None
inline std::optional<std::tuple<py::str, py::str>> _gap(const py::str& path, const py::bytes& head) {
    const auto gap = ::cb::coverage::reach::gap_of(::cb::fs::from_text(path.utf8()), ::cb::compat::from_py(head));
    if (!gap) return std::nullopt;
    return std::make_tuple(::cb::compat::to_py(gap->title), ::cb::compat::to_py(gap->cpu));
}

// what a file is, where detect has nothing to say
inline Described describe(const py::str& path) {
    return ::cb::compat::guard([&] {
        const nr::Described d = nr::describe(::cb::fs::from_text(path.utf8()), detail::services());
        Described out;
        out.found = detail::to_py(d.found);
        out.platform = detail::to_py(d.platform);
        if (d.lane) out.lane = detail::to_py(*d.lane);
        out.family = detail::to_py(d.family);
        return out;
    });
}

// Programs inside a container (disc, package, GoD folder), written under state/reach/unpacked. Smallest first, at most `limit`.
inline py::list<py::str> _unpack(const py::str& path, int64_t limit, const Log& log) {
    return ::cb::compat::guard([&] {
        py::list<py::str> out;
        for (const auto& p : nr::unpack(::cb::fs::from_text(path.utf8()), static_cast<std::size_t>(limit), detail::services(log))) {
            out.append(detail::to_py(::cb::fs::to_text(p)));
        }
        return out;
    });
}

// [(path, where it came from)] for every file under `inputs`, containers opened.
inline py::list<std::tuple<py::str, py::str>> programs(const py::list<py::str>& inputs, int64_t containers = 20, const Log& log = nullptr) {
    return ::cb::compat::guard([&] {
        std::vector<std::filesystem::path> in;
        for (const py::str& i : inputs) in.push_back(::cb::fs::from_text(i.utf8()));
        py::list<std::tuple<py::str, py::str>> out;
        for (const nr::Found& f : nr::programs(in, static_cast<std::size_t>(containers), detail::services(log))) {
            out.append(std::make_tuple(detail::to_py(::cb::fs::to_text(f.path)), detail::to_py(f.origin)));
        }
        return out;
    });
}

// Run the platform's lane on one program in its own process.
inline Judged judge(const py::str& path, int64_t timeout) {
    return ::cb::compat::guard([&] {
        const nr::Judged j = nr::judge(::cb::fs::from_text(path.utf8()), static_cast<int>(timeout), detail::services());
        return detail::to_old(nr::Row{.judged = j}).judged.value();
    });
}

// (the function store's {class: count} for a program, "" or why there is none)
inline std::tuple<py::dict<py::str, int64_t>, py::str> tally_of(const py::str& path) {
    py::dict<py::str, int64_t> tally;
    try {
        for (const auto& [k, v] : detail::services().tally(::cb::fs::from_text(path.utf8()))) tally.set(detail::to_py(k), v);
    } catch (const std::exception& e) {
        return {py::dict<py::str, int64_t>(), py::percent("no tally: %s", detail::to_py(std::string(e.what()).substr(0, 120)))};
    }
    return {tally, py::str("")};
}

// `n` of `rows` evenly across their sizes (the smallest, the largest under `max_size`, and between)
inline py::list<Row> spread(const py::list<Row>& rows, int64_t n, int64_t max_size) {
    std::vector<nr::Row> rs;
    for (const Row& r : rows) rs.push_back(detail::to_new(r));
    std::vector<nr::Row*> ptrs;
    for (nr::Row& r : rs) ptrs.push_back(&r);
    py::list<Row> out;
    for (const nr::Row* r : nr::spread(ptrs, n < 0 ? 0 : static_cast<std::size_t>(n), max_size < 0 ? 0 : static_cast<std::uint64_t>(max_size))) {
        out.append(detail::to_old(*r));
    }
    return out;
}

// The rows of a results file (state/reach/<run>.json: its "programs").
inline py::list<Row> read_results(const py::str& path) {
    py::list<Row> out;
    for (const nr::Row& r : nr::read_results(::cb::fs::from_text(path.utf8()))) out.append(detail::to_old(r));
    return out;
}

// The run. -> (the rows of this run, the report). log: default prints.
inline std::tuple<py::list<Row>, py::str> run(const py::list<py::str>& inputs, int64_t per_platform = 5, bool do_judge = false,
                                              int64_t timeout = 300, int64_t containers = 20, const py::str& name = py::str("reach"),
                                              bool again = false, int64_t max_size = int64_t(4) << 20, const Log& log = nullptr) {
    return ::cb::compat::guard([&] {
        std::vector<std::filesystem::path> in;
        for (const py::str& i : inputs) in.push_back(::cb::fs::from_text(i.utf8()));
        nr::Options o;
        o.per_platform = static_cast<std::size_t>(per_platform < 0 ? 0 : per_platform);
        o.judge = do_judge;
        o.timeout_seconds = static_cast<int>(timeout);
        o.containers = static_cast<std::size_t>(containers < 0 ? 0 : containers);
        o.name = name.utf8();
        o.again = again;
        o.max_size = max_size < 0 ? 0 : static_cast<std::uint64_t>(max_size);
        const nr::Result r = nr::run(in, o, detail::services(log));
        py::list<Row> rows;
        for (const nr::Row& row : r.rows) rows.append(detail::to_old(row));
        return std::make_tuple(rows, detail::to_py(r.report));
    });
}

// Markdown: by platform, how many programs, how far they got.
inline py::str report(const py::list<Row>& rows) {
    std::vector<nr::Row> rs;
    for (const Row& r : rows) rs.push_back(detail::to_new(r));
    return detail::to_py(nr::report(rs));
}

// codebreaker reach ...   (usage errors: usage text and message on stderr, exit 2)
inline int64_t main(const py::list<py::str>& argv) {
    return ::cb::compat::guard([&] {
        ::cb::cli::Cli cli("codebreaker", {nr::command(detail::services())});
        std::vector<std::string> args = {"reach"};
        for (const py::str& a : argv) args.push_back(a.utf8());
        return static_cast<int64_t>(cli.run(args));
    });
}

}  // namespace codebreaker::coverage::reach
