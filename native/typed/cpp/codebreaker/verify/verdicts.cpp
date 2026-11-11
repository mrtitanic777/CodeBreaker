// codebreaker/verify/verdicts.cpp -- typed C++ port of codebreaker.verify.verdicts (see verdicts.hpp).

#include "codebreaker/verify/verdicts.hpp"

// The tables below are built from string literals on first use, never from the constants above: another file's namespace-scope
// initialiser may ask for SCORED() or TITLE() before this file's constants exist (construct-on-first-use).

namespace codebreaker::verify::verdicts {

// ---- 1. what a judge says ----
const py::str EQUIVALENT("equivalent");
const py::str CONSISTENT("consistent");
const py::str DIVERGENT("divergent");
const py::str INCONCLUSIVE("inconclusive");
const py::str NO_COMPILE("no-compile");
const py::str NO_SOURCE("no-source");

const py::list<py::str>& VERDICTS() {
    static const py::list<py::str> v{py::str("equivalent"), py::str("consistent"), py::str("divergent"), py::str("inconclusive"), py::str("no-compile"), py::str("no-source")};
    return v;
}
const py::list<py::str>& GOOD() {
    static const py::list<py::str> v{py::str("equivalent"), py::str("consistent")};
    return v;
}
const NotRun& NOT_RUN() {
    static const NotRun v{py::str("inconclusive"), py::str("judge: the emulator crashed on this function"),
                          py::list<py::str>(), py::str("")};
    return v;
}

const py::dict<py::str, py::str>& OLD() {
    static const py::dict<py::str, py::str> d{
        {py::str("no-synth"), py::str("no-source")},       // (reconstruct.run's row for a function the decompiler gave nothing)
        {py::str("same"), py::str("equivalent")},          // (the 16-bit gate, the port's cases)
        {py::str("alike"), py::str("equivalent")},
        {py::str("differs"), py::str("divergent")},
        {py::str("different"), py::str("divergent")},
        {py::str("not run"), py::str("inconclusive")},
        {py::str("not-run"), py::str("inconclusive")},
        {py::str("not judged"), py::str("inconclusive")},
        {py::str("untested"), py::str("inconclusive")},
        {py::str("does not compile"), py::str("no-compile")},
        {py::str("not compiled"), py::str("no-compile")},
    };
    return d;
}

const py::dict<py::str, py::str>& SAID() {
    static const py::dict<py::str, py::str> d{
        {py::str("equivalent"), py::str("same")},          {py::str("consistent"), py::str("same (both fail)")},
        {py::str("divergent"), py::str("differs")},        {py::str("inconclusive"), py::str("not run")},
        {py::str("no-compile"), py::str("does not compile")}, {py::str("no-source"), py::str("no source")},
    };
    return d;
}

std::optional<py::str> verdict(const std::optional<py::str>& word) {
    if (!word) return std::nullopt;
    return OLD().get(*word, *word);
}

py::str pair(bool same, bool ran, bool built) {
    if (!built) return py::str("no-compile");
    if (!ran) return py::str("inconclusive");
    return same ? py::str("equivalent") : py::str("divergent");
}

bool good(const std::optional<py::str>& word) {
    std::optional<py::str> v = verdict(word);
    return v && (*v == py::str("equivalent") || *v == py::str("consistent"));
}

// ---- 2. what it amounts to ----
const py::str VERIFIED("verified");
const py::str WEAK("weak");
const py::str UNVERIFIED("unverified");
const py::str NOT_JUDGED("not-judged");
const py::str IDENTIFIED("identified");

const py::list<py::str>& ORDER() {
    static const py::list<py::str> v{py::str("verified"), py::str("weak"), py::str("unverified"), py::str("not-judged"), py::str("no-compile"), py::str("no-source")};
    return v;
}
const py::list<py::str>& SCORED() {
    static const py::list<py::str> v{py::str("verified"), py::str("weak"), py::str("identified"), py::str("unverified"), py::str("not-judged"), py::str("no-compile"), py::str("no-source")};
    return v;
}
const py::list<py::str>& BUILT() {
    static const py::list<py::str> v{py::str("verified"), py::str("weak"), py::str("unverified"), py::str("not-judged")};
    return v;
}
const py::dict<py::str, py::str>& TITLE() {
    static const py::dict<py::str, py::str> d{
        {py::str("verified"), py::str("VERIFIED")},       {py::str("weak"), py::str("VERIFIED (weak)")},
        {py::str("unverified"), py::str("UNVERIFIED")},   {py::str("not-judged"), py::str("NOT JUDGED")},
        {py::str("no-compile"), py::str("DOES NOT COMPILE")}, {py::str("no-source"), py::str("NO SOURCE")},
        {py::str("identified"), py::str("IDENTIFIED (runtime library)")},
    };
    return d;
}
const py::dict<py::str, py::str>& LABEL() {
    static const py::dict<py::str, py::str> d{
        {py::str("verified"), py::str("verified")},       {py::str("weak"), py::str("weak")},
        {py::str("unverified"), py::str("unverified")},   {py::str("not-judged"), py::str("not judged")},
        {py::str("no-compile"), py::str("does not compile")}, {py::str("no-source"), py::str("no source")},
        {py::str("identified"), py::str("runtime library")},
    };
    return d;
}
const py::dict<py::str, py::str>& CLASS() {
    static const py::dict<py::str, py::str> d{
        {py::str("equivalent"), py::str("verified")}, {py::str("consistent"), py::str("weak")},       {py::str("divergent"), py::str("unverified")},
        {py::str("inconclusive"), py::str("not-judged")}, {py::str("no-compile"), py::str("no-compile")}, {py::str("no-source"), py::str("no-source")},
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
        {py::str("not judged"), py::str("not-judged")},
        {py::str("does not compile"), py::str("no-compile")},
        {py::str("no source"), py::str("no-source")},
        {py::str("runtime library"), py::str("identified")},
        {py::str("run-time library"), py::str("identified")},
        {py::str("verified (weak)"), py::str("weak")},
        {py::str("identified (runtime library)"), py::str("identified")},
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
    if (v && *v == py::str("equivalent")) return {weak.empty() ? py::str("verified") : py::str("weak"), weak};
    if (v && *v == py::str("consistent")) {
        if (weak.empty()) weak.append(py::str("fault-only"));
        return {py::str("weak"), weak};
    }
    // CLASS.get(v, py::str("no-source")); a None word is no-source
    if (!v) return {py::str("no-source"), py::list<py::str>()};
    return {CLASS().get(*v, py::str("no-source")), py::list<py::str>()};
}

// ---- 3. a byte comparison ----
const py::str IDENTICAL("identical");
const py::str SAME_SIZE("same size");
const py::str DIFFERENT("different");
const py::str NO_EXTENT("no original extent");

const py::list<py::str>& MATCHES() {
    static const py::list<py::str> v{py::str("identical"), py::str("same size"), py::str("different"), py::str("no original extent"), py::str("no-compile")};
    return v;
}
const py::dict<py::str, py::str>& MATCH_OLD() {
    static const py::dict<py::str, py::str> d{
        {py::str("does not compile"), py::str("no-compile")},
        {py::str("same length, different"), py::str("same size")},
        {py::str("different length"), py::str("different")},
        {py::str("run-time library"), py::str("identified")},
        {py::str("runtime library"), py::str("identified")},
    };
    return d;
}
const py::dict<py::str, py::str>& MATCH_LABEL() {
    static const py::dict<py::str, py::str> d{
        {py::str("identical"), py::str("identical")},     {py::str("same size"), py::str("same size")},
        {py::str("different"), py::str("different")},     {py::str("no original extent"), py::str("no original extent")},
        {py::str("no-compile"), py::str("does not compile")}, {py::str("identified"), py::str("runtime library")},
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
