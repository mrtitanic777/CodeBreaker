// codebreaker/verify/verdicts.cpp -- typed C++ port of codebreaker.verify.verdicts (see verdicts.hpp).

#include "codebreaker/verify/verdicts.hpp"

// The vocabulary below is read by other files' namespace-scope initialisers (a table built from SCORED(), TITLE() ...): this file's
// constants are initialised before theirs, whatever the link order (MSVC: lib initialisers run before user ones).
#pragma init_seg(lib)

namespace codebreaker::verify::verdicts {

// ---- 1. what a judge says ----
const py::str EQUIVALENT("equivalent");
const py::str CONSISTENT("consistent");
const py::str DIVERGENT("divergent");
const py::str INCONCLUSIVE("inconclusive");
const py::str NO_COMPILE("no-compile");
const py::str NO_SOURCE("no-source");

const py::list<py::str>& VERDICTS() {
    static const py::list<py::str> v{EQUIVALENT, CONSISTENT, DIVERGENT, INCONCLUSIVE, NO_COMPILE, NO_SOURCE};
    return v;
}
const py::list<py::str>& GOOD() {
    static const py::list<py::str> v{EQUIVALENT, CONSISTENT};
    return v;
}
const NotRun& NOT_RUN() {
    static const NotRun v{INCONCLUSIVE, py::str("judge: the emulator crashed on this function"),
                          py::list<py::str>(), py::str("")};
    return v;
}

const py::dict<py::str, py::str>& OLD() {
    static const py::dict<py::str, py::str> d{
        {py::str("no-synth"), NO_SOURCE},       // (reconstruct.run's row for a function the decompiler gave nothing)
        {py::str("same"), EQUIVALENT},          // (the 16-bit gate, the port's cases)
        {py::str("alike"), EQUIVALENT},
        {py::str("differs"), DIVERGENT},
        {py::str("different"), DIVERGENT},
        {py::str("not run"), INCONCLUSIVE},
        {py::str("not-run"), INCONCLUSIVE},
        {py::str("not judged"), INCONCLUSIVE},
        {py::str("untested"), INCONCLUSIVE},
        {py::str("does not compile"), NO_COMPILE},
        {py::str("not compiled"), NO_COMPILE},
    };
    return d;
}

const py::dict<py::str, py::str>& SAID() {
    static const py::dict<py::str, py::str> d{
        {EQUIVALENT, py::str("same")},          {CONSISTENT, py::str("same (both fail)")},
        {DIVERGENT, py::str("differs")},        {INCONCLUSIVE, py::str("not run")},
        {NO_COMPILE, py::str("does not compile")}, {NO_SOURCE, py::str("no source")},
    };
    return d;
}

std::optional<py::str> verdict(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    return OLD().get(*word, *word);
}

py::str pair(bool same, bool ran, bool built) {
    if (!built) return NO_COMPILE;
    if (!ran) return INCONCLUSIVE;
    return same ? EQUIVALENT : DIVERGENT;
}

bool good(const std::optional<py::str>& word) {
    std::optional<py::str> v = verdict(word);
    return v && (*v == EQUIVALENT || *v == CONSISTENT);
}

// ---- 2. what it amounts to ----
const py::str VERIFIED("verified");
const py::str WEAK("weak");
const py::str UNVERIFIED("unverified");
const py::str NOT_JUDGED("not-judged");
const py::str IDENTIFIED("identified");

const py::list<py::str>& ORDER() {
    static const py::list<py::str> v{VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED, NO_COMPILE, NO_SOURCE};
    return v;
}
const py::list<py::str>& SCORED() {
    static const py::list<py::str> v{VERIFIED, WEAK, IDENTIFIED, UNVERIFIED, NOT_JUDGED, NO_COMPILE, NO_SOURCE};
    return v;
}
const py::list<py::str>& BUILT() {
    static const py::list<py::str> v{VERIFIED, WEAK, UNVERIFIED, NOT_JUDGED};
    return v;
}
const py::dict<py::str, py::str>& TITLE() {
    static const py::dict<py::str, py::str> d{
        {VERIFIED, py::str("VERIFIED")},       {WEAK, py::str("VERIFIED (weak)")},
        {UNVERIFIED, py::str("UNVERIFIED")},   {NOT_JUDGED, py::str("NOT JUDGED")},
        {NO_COMPILE, py::str("DOES NOT COMPILE")}, {NO_SOURCE, py::str("NO SOURCE")},
        {IDENTIFIED, py::str("IDENTIFIED (runtime library)")},
    };
    return d;
}
const py::dict<py::str, py::str>& LABEL() {
    static const py::dict<py::str, py::str> d{
        {VERIFIED, py::str("verified")},       {WEAK, py::str("weak")},
        {UNVERIFIED, py::str("unverified")},   {NOT_JUDGED, py::str("not judged")},
        {NO_COMPILE, py::str("does not compile")}, {NO_SOURCE, py::str("no source")},
        {IDENTIFIED, py::str("runtime library")},
    };
    return d;
}
const py::dict<py::str, py::str>& CLASS() {
    static const py::dict<py::str, py::str> d{
        {EQUIVALENT, VERIFIED}, {CONSISTENT, WEAK},       {DIVERGENT, UNVERIFIED},
        {INCONCLUSIVE, NOT_JUDGED}, {NO_COMPILE, NO_COMPILE}, {NO_SOURCE, NO_SOURCE},
    };
    return d;
}

// limits that make a match weaker than it looks
const py::dict<py::str, py::str>& WEAK_LIMITS() {
    static const py::dict<py::str, py::str> d{
        {py::str("fault-only"), py::str("the two sides only ever fail alike; neither returned normally")},
        {py::str("low-coverage"), py::str("under half of the original's instructions ran")},
        {py::str("truncated"), py::str("a run was cut off at the step limit")},
        {py::str("unobserved"), py::str("some inputs could not be run on one side")},
        {py::str("writes-capped"), py::str("too many memory writes to compare them all")},
        {py::str("stub-callee"), py::str("a function it calls was a stand-in, not that function's reconstruction")},
        {py::str("argc-unknown"), py::str("an API call was compared by name only")},
        {py::str("calls-capped"), py::str("it makes more calls than are compared")},
        {py::str("methods-partial"),
         py::str("only some of the class's methods could be run beside the original (java)")},
    };
    return d;
}

// every older spelling of a class -> the class now
const py::dict<py::str, py::str>& CLASS_OLD() {
    static const py::dict<py::str, py::str> d{
        {py::str("not judged"), NOT_JUDGED},
        {py::str("does not compile"), NO_COMPILE},
        {py::str("no source"), NO_SOURCE},
        {py::str("runtime library"), IDENTIFIED},
        {py::str("run-time library"), IDENTIFIED},
        {py::str("verified (weak)"), WEAK},
        {py::str("identified (runtime library)"), IDENTIFIED},
    };
    return d;
}

std::optional<py::str> klass(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    py::str w = word->strip();
    if (TITLE().contains(w)) return w;
    py::str low = w.lower();
    if (TITLE().contains(low)) return low;
    return CLASS_OLD().get(low, w);
}

std::pair<py::str, py::list<py::str>> classify(const std::optional<py::str>& word,
                                               const std::optional<py::str>& limits) {
    std::optional<py::str> v = verdict(word);
    py::list<py::str> weak;
    if (limits) {
        for (const py::str& c : py::split(*limits, py::str(","))) {
            if (!c.empty() && WEAK_LIMITS().contains(c)) weak.append(c);
        }
    }
    if (v && *v == EQUIVALENT) return {weak.empty() ? VERIFIED : WEAK, weak};
    if (v && *v == CONSISTENT) {
        if (weak.empty()) weak.append(py::str("fault-only"));
        return {WEAK, weak};
    }
    // CLASS.get(v, NO_SOURCE); a None word is no-source
    if (!v) return {NO_SOURCE, py::list<py::str>()};
    return {CLASS().get(*v, NO_SOURCE), py::list<py::str>()};
}

// ---- 3. a byte comparison ----
const py::str IDENTICAL("identical");
const py::str SAME_SIZE("same size");
const py::str DIFFERENT("different");
const py::str NO_EXTENT("no original extent");

const py::list<py::str>& MATCHES() {
    static const py::list<py::str> v{IDENTICAL, SAME_SIZE, DIFFERENT, NO_EXTENT, NO_COMPILE};
    return v;
}
const py::dict<py::str, py::str>& MATCH_OLD() {
    static const py::dict<py::str, py::str> d{
        {py::str("does not compile"), NO_COMPILE},
        {py::str("same length, different"), SAME_SIZE},
        {py::str("different length"), DIFFERENT},
        {py::str("run-time library"), IDENTIFIED},
        {py::str("runtime library"), IDENTIFIED},
    };
    return d;
}
const py::dict<py::str, py::str>& MATCH_LABEL() {
    static const py::dict<py::str, py::str> d{
        {IDENTICAL, py::str("identical")},     {SAME_SIZE, py::str("same size")},
        {DIFFERENT, py::str("different")},     {NO_EXTENT, py::str("no original extent")},
        {NO_COMPILE, py::str("does not compile")}, {IDENTIFIED, py::str("runtime library")},
    };
    return d;
}

std::optional<py::str> match(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    return MATCH_OLD().get(*word, *word);
}

// ---- by provenance ----
Verdict of_row(const py::str& module, const Row& row) {
    std::optional<py::str> v = verdict(row.verdict);
    return Verdict{module, row.addr, v, row.reason ? *row.reason : py::str(""), row.limits ? *row.limits : py::str(""),
                   row.detail ? *row.detail : py::str("")};
}

Index::Index(const py::str& module_, const py::dict<py::str, Row>& rows) : module(module_) {
    for (const auto& kv : rows.items()) rows_.set(std::make_tuple(module_, std::get<0>(kv)), std::get<1>(kv));
}

std::optional<Row> Index::at(const std::optional<Origin>& origin) const {
    if (!origin || (!origin->module && !origin->va)) return std::nullopt;
    if (!origin->module || !origin->va) return std::nullopt;
    return rows_.get(std::make_tuple(*origin->module, *origin->va));
}

Origin Index::origin(const py::str& addr) const { return Origin{module, addr}; }

}  // namespace codebreaker::verify::verdicts
