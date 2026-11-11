// codebreaker/verify/verdicts.hpp -- COMPAT SHIM over cb/verify/verdicts.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/verdicts.{hpp,cpp} now (enums for the three vocabularies, no py::). This header keeps
// the old py::str API for the ports that have not been refactored yet: the words are read from the new enums, the
// answers converted back. No logic. Deleted with its last py:: consumer (grep "verify/verdicts.hpp").
//
// The old API answered for "a word that is any str": an unknown word stays itself through verdict(), klass() and
// match(); the new enums have no value for it, so the shim hands the word back unchanged in that case.
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "cb/compat/py_bridge.hpp"
#include "cb/verify/verdicts.hpp"

namespace codebreaker::verify::verdicts {

namespace detail {
namespace nv = cb::verify;

inline py::str str(std::string_view s) { return cb::compat::to_py(s); }

template <class E, std::size_t N>
py::list<py::str> words_of(const std::array<E, N>& values) {
    py::list<py::str> out;
    for (E e : values) out.append(str(nv::word(e)));
    return out;
}
}  // namespace detail

// ---- 1. what a judge says ----
inline const py::str EQUIVALENT = detail::str(detail::nv::word(detail::nv::Verdict::Equivalent));
inline const py::str CONSISTENT = detail::str(detail::nv::word(detail::nv::Verdict::Consistent));
inline const py::str DIVERGENT = detail::str(detail::nv::word(detail::nv::Verdict::Divergent));
inline const py::str INCONCLUSIVE = detail::str(detail::nv::word(detail::nv::Verdict::Inconclusive));
inline const py::str NO_COMPILE = detail::str(detail::nv::word(detail::nv::Verdict::NoCompile));
inline const py::str NO_SOURCE = detail::str(detail::nv::word(detail::nv::Verdict::NoSource));

inline const py::list<py::str>& VERDICTS() {
    static const py::list<py::str> v = detail::words_of(detail::nv::kVerdicts);
    return v;
}
inline const py::list<py::str>& GOOD() {
    static const py::list<py::str> v{detail::str("equivalent"), detail::str("consistent")};
    return v;
}
// NOT_RUN = (INCONCLUSIVE, "judge: the emulator crashed on this function", [], "")
using NotRun = std::tuple<py::str, py::str, py::list<py::str>, py::str>;
inline const NotRun& NOT_RUN() {
    static const NotRun v = [] {
        const detail::nv::Judgement j = detail::nv::not_run();
        return NotRun{detail::str(detail::nv::word(j.verdict)), detail::str(j.reason), py::list<py::str>(), detail::str(j.detail)};
    }();
    return v;
}
// every older spelling of a judge's word -> the word now
inline const py::dict<py::str, py::str>& OLD() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (const char* old : {"no-synth", "same", "alike", "differs", "different", "not run", "not-run", "not judged",
                                "untested", "does not compile", "not compiled"})
            out.set(detail::str(old), detail::str(detail::nv::word(*detail::nv::parse_verdict(old))));
        return out;
    }();
    return d;
}
// how a judge's word is said in a report of pairs
inline const py::dict<py::str, py::str>& SAID() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (detail::nv::Verdict v : detail::nv::kVerdicts) out.set(detail::str(detail::nv::word(v)), detail::str(detail::nv::said(v)));
        return out;
    }();
    return d;
}

// A judge's word as it is now (an older spelling mapped); None stays None, an unknown word stays itself.
inline std::optional<py::str> verdict(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    if (auto v = detail::nv::parse_verdict(cb::compat::from_py(*word))) return detail::str(detail::nv::word(*v));
    return word;
}

// The verdict of a yes/no comparison of a pair.
inline py::str pair(bool same, bool ran = true, bool built = true) {
    return detail::str(detail::nv::word(detail::nv::pair_verdict(same, ran, built)));
}

inline bool good(const std::optional<py::str>& word) {
    return word && detail::nv::is_good(detail::nv::parse_verdict(cb::compat::from_py(*word)));
}

// ---- 2. what it amounts to ----
inline const py::str VERIFIED = detail::str(detail::nv::word(detail::nv::Class::Verified));
inline const py::str WEAK = detail::str(detail::nv::word(detail::nv::Class::Weak));
inline const py::str UNVERIFIED = detail::str(detail::nv::word(detail::nv::Class::Unverified));
inline const py::str NOT_JUDGED = detail::str(detail::nv::word(detail::nv::Class::NotJudged));
inline const py::str IDENTIFIED = detail::str(detail::nv::word(detail::nv::Class::Identified));

inline const py::list<py::str>& ORDER() {   // best first
    static const py::list<py::str> v = detail::words_of(detail::nv::kClassOrder);
    return v;
}
inline const py::list<py::str>& SCORED() {
    static const py::list<py::str> v = detail::words_of(detail::nv::kClassScored);
    return v;
}
inline const py::list<py::str>& BUILT() {   // compiled and run
    static const py::list<py::str> v = detail::words_of(detail::nv::kClassBuilt);
    return v;
}
inline const py::dict<py::str, py::str>& TITLE() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (detail::nv::Class c : detail::nv::kClassScored) out.set(detail::str(detail::nv::word(c)), detail::str(detail::nv::title(c)));
        return out;
    }();
    return d;
}
inline const py::dict<py::str, py::str>& LABEL() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (detail::nv::Class c : detail::nv::kClassScored) out.set(detail::str(detail::nv::word(c)), detail::str(detail::nv::label(c)));
        return out;
    }();
    return d;
}
inline const py::dict<py::str, py::str>& CLASS() {   // judge's word -> class
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (detail::nv::Verdict v : detail::nv::kVerdicts)
            out.set(detail::str(detail::nv::word(v)), detail::str(detail::nv::word(detail::nv::class_of(v))));
        return out;
    }();
    return d;
}
// limits that make a match weaker than it looks
inline const py::dict<py::str, py::str>& WEAK_LIMITS() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (unsigned i = 0; i < detail::nv::kWeakLimitCount; ++i) {
            const auto l = static_cast<detail::nv::WeakLimit>(i);
            out.set(detail::str(detail::nv::word(l)), detail::str(detail::nv::explanation(l)));
        }
        return out;
    }();
    return d;
}
// every older spelling of a class -> the class now
inline const py::dict<py::str, py::str>& CLASS_OLD() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (const char* old : {"not judged", "does not compile", "no source", "runtime library", "run-time library",
                                "verified (weak)", "identified (runtime library)"})
            out.set(detail::str(old), detail::str(detail::nv::word(*detail::nv::parse_class(old))));
        return out;
    }();
    return d;
}

// A class as it is now: its TITLE or LABEL, or an older spelling, read back.
inline std::optional<py::str> klass(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    if (auto c = detail::nv::parse_class(cb::compat::from_py(*word))) return detail::str(detail::nv::word(*c));
    return word->strip();
}

// (class, weak reasons) for one verdict and its limits.
inline std::pair<py::str, py::list<py::str>> classify(const std::optional<py::str>& word,
                                                      const std::optional<py::str>& limits = std::nullopt) {
    std::optional<detail::nv::Verdict> v;
    if (word) v = detail::nv::parse_verdict(cb::compat::from_py(*word));
    const detail::nv::Classification c =
        detail::nv::classify(v, limits ? cb::compat::from_py(*limits) : std::string());
    py::list<py::str> reasons;
    for (detail::nv::WeakLimit l : c.reasons) reasons.append(detail::str(detail::nv::word(l)));
    return {detail::str(detail::nv::word(c.cls)), reasons};
}

// ---- 3. a byte comparison ----
inline const py::str IDENTICAL = detail::str(detail::nv::word(detail::nv::ByteMatch::Identical));
inline const py::str SAME_SIZE = detail::str(detail::nv::word(detail::nv::ByteMatch::SameSize));
inline const py::str DIFFERENT = detail::str(detail::nv::word(detail::nv::ByteMatch::Different));
inline const py::str NO_EXTENT = detail::str(detail::nv::word(detail::nv::ByteMatch::NoExtent));

inline const py::list<py::str>& MATCHES() {   // best first
    static const py::list<py::str> v = detail::words_of(detail::nv::kByteMatches);
    return v;
}
inline const py::dict<py::str, py::str>& MATCH_OLD() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (const char* old : {"does not compile", "same length, different", "different length", "run-time library", "runtime library"})
            out.set(detail::str(old), detail::str(detail::nv::word(*detail::nv::parse_byte_match(old))));
        return out;
    }();
    return d;
}
inline const py::dict<py::str, py::str>& MATCH_LABEL() {
    static const py::dict<py::str, py::str> d = [] {
        py::dict<py::str, py::str> out;
        for (detail::nv::ByteMatch m : {detail::nv::ByteMatch::Identical, detail::nv::ByteMatch::SameSize, detail::nv::ByteMatch::Different,
                                        detail::nv::ByteMatch::NoExtent, detail::nv::ByteMatch::NoCompile, detail::nv::ByteMatch::Identified})
            out.set(detail::str(detail::nv::word(m)), detail::str(detail::nv::label(m)));
        return out;
    }();
    return d;
}

// A byte comparison's word as it is now (an older spelling mapped).
inline std::optional<py::str> match(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    if (auto m = detail::nv::parse_byte_match(cb::compat::from_py(*word))) return detail::str(detail::nv::word(*m));
    return word;
}

// ---- by provenance ----
// Verdict = namedtuple("Verdict", "module addr verdict reason limits detail")
struct Verdict {
    py::str module;
    std::optional<py::str> addr;
    std::optional<py::str> verdict;
    py::str reason;
    py::str limits;
    py::str detail;
};

// A store row ({addr, verdict, reason, limits, detail}); a key the row lacks, or holds as None, is nullopt.
struct Row {
    std::optional<py::str> addr, verdict, reason, limits, detail;
};

namespace detail {
inline nv::StoreRow to_new(const Row& r) {
    nv::StoreRow out;
    out.addr = cb::compat::from_py(r.addr);
    out.verdict = cb::compat::from_py(r.verdict);
    out.reason = r.reason ? cb::compat::from_py(*r.reason) : std::string();
    out.limits = r.limits ? cb::compat::from_py(*r.limits) : std::string();
    out.detail = r.detail ? cb::compat::from_py(*r.detail) : std::string();
    return out;
}
inline Row to_old(const nv::StoreRow& r) {
    Row out;
    out.addr = cb::compat::to_py(r.addr);
    out.verdict = cb::compat::to_py(r.verdict);
    out.reason = str(r.reason);
    out.limits = str(r.limits);
    out.detail = str(r.detail);
    return out;
}
}  // namespace detail

// A store row as a Verdict, its word as it is now.
inline Verdict of_row(const py::str& module, const Row& row) {
    Verdict v;
    v.module = module;
    v.addr = row.addr;
    v.verdict = verdict(row.verdict);
    v.reason = row.reason ? *row.reason : py::str("");
    v.limits = row.limits ? *row.limits : py::str("");
    v.detail = row.detail ? *row.detail : py::str("");
    return v;
}

// An L2 provenance {module, va}.
struct Origin {
    std::optional<py::str> module, va;
};

// A module's rows by L2 provenance: at({module, va}) -> the row of the function the node stands for, or None.
class Index {
  public:
    Index(const py::str& module, const py::dict<py::str, Row>& rows) : module(module) {
        std::unordered_map<std::string, cb::verify::StoreRow> mine;
        for (const auto& kv : rows.items()) mine[cb::compat::from_py(std::get<0>(kv))] = detail::to_new(std::get<1>(kv));
        index_ = std::make_shared<cb::verify::Index>(cb::compat::from_py(module), std::move(mine));
    }
    py::str module;
    std::optional<Row> at(const std::optional<Origin>& origin) const {
        if (!origin) return std::nullopt;
        const cb::verify::StoreRow* row = index_->at(cb::verify::Origin{origin->module ? cb::compat::from_py(*origin->module) : std::string(),
                                                                       origin->va ? cb::compat::from_py(*origin->va) : std::string()});
        if (!row) return std::nullopt;
        return detail::to_old(*row);
    }
    Origin origin(const py::str& addr) const { return Origin{module, addr}; }

  private:
    std::shared_ptr<cb::verify::Index> index_;
};

}  // namespace codebreaker::verify::verdicts
