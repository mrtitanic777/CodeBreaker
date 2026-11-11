// cb/verify/verdicts.cpp -- the verdict vocabulary (see verdicts.hpp). The words themselves, and the older
// spellings read as the words now.

#include "cb/verify/verdicts.hpp"

#include "cb/core/text.hpp"

namespace cb::verify {

namespace {

template <class E> struct Spelling {
    std::string_view text;
    E value;
};

// ---- a judge's words ---------------------------------------------------------------------------------

constexpr std::array<std::string_view, 6> kVerdictWords = {"equivalent",   "consistent", "divergent",
                                                           "inconclusive", "no-compile", "no-source"};
constexpr std::array<std::string_view, 6> kVerdictSaid = {"same",    "same (both fail)", "differs",
                                                          "not run", "does not compile", "no source"};

// every older spelling of a judge's word
constexpr Spelling<Verdict> kVerdictOld[] = {
    {"no-synth", Verdict::NoSource}, // (reconstruct.run's row for a function the decompiler gave nothing)
    {"same", Verdict::Equivalent},   // (the 16-bit gate, the port's cases)
    {"alike", Verdict::Equivalent},       {"differs", Verdict::Divergent},
    {"different", Verdict::Divergent},    {"not run", Verdict::Inconclusive},
    {"not-run", Verdict::Inconclusive},   {"not judged", Verdict::Inconclusive},
    {"untested", Verdict::Inconclusive},  {"does not compile", Verdict::NoCompile},
    {"not compiled", Verdict::NoCompile},
};

// ---- weak limits ---------------------------------------------------------------------------------------

constexpr std::array<std::string_view, kWeakLimitCount> kLimitWords = {
    "fault-only",  "low-coverage", "truncated",    "unobserved",     "writes-capped",
    "stub-callee", "argc-unknown", "calls-capped", "methods-partial"};
constexpr std::array<std::string_view, kWeakLimitCount> kLimitExplained = {
    "the two sides only ever fail alike; neither returned normally",
    "under half of the original's instructions ran",
    "a run was cut off at the step limit",
    "some inputs could not be run on one side",
    "too many memory writes to compare them all",
    "a function it calls was a stand-in, not that function's reconstruction",
    "an API call was compared by name only",
    "it makes more calls than are compared",
    "only some of the class's methods could be run beside the original (java)"};

// ---- classes -----------------------------------------------------------------------------------------

constexpr std::array<std::string_view, 7> kClassWords = {"verified",   "weak",       "identified", "unverified",
                                                         "not-judged", "no-compile", "no-source"};
constexpr std::array<std::string_view, 7> kClassTitles = {
    "VERIFIED",         "VERIFIED (weak)", "IDENTIFIED (runtime library)", "UNVERIFIED", "NOT JUDGED",
    "DOES NOT COMPILE", "NO SOURCE"};
constexpr std::array<std::string_view, 7> kClassLabels = {
    "verified", "weak", "runtime library", "unverified", "not judged", "does not compile", "no source"};

// every older spelling of a class (lowercase)
constexpr Spelling<Class> kClassOld[] = {
    {"not judged", Class::NotJudged},
    {"does not compile", Class::NoCompile},
    {"no source", Class::NoSource},
    {"runtime library", Class::Identified},
    {"run-time library", Class::Identified},
    {"verified (weak)", Class::Weak},
    {"identified (runtime library)", Class::Identified},
};

// ---- byte comparisons ----------------------------------------------------------------------------------

constexpr std::array<std::string_view, 6> kMatchWords = {"identical",          "same size",  "different",
                                                         "no original extent", "no-compile", "identified"};
constexpr std::array<std::string_view, 6> kMatchLabels = {"identical",          "same size",        "different",
                                                          "no original extent", "does not compile", "runtime library"};
constexpr Spelling<ByteMatch> kMatchOld[] = {
    {"does not compile", ByteMatch::NoCompile}, {"same length, different", ByteMatch::SameSize},
    {"different length", ByteMatch::Different}, {"run-time library", ByteMatch::Identified},
    {"runtime library", ByteMatch::Identified},
};

std::string lower_trimmed(std::string_view s) {
    return text::lower(text::trim(s));
}

} // namespace

// ---- Verdict -------------------------------------------------------------------------------------------

std::string_view word(Verdict v) {
    return kVerdictWords[static_cast<std::size_t>(v)];
}
std::string_view said(Verdict v) {
    return kVerdictSaid[static_cast<std::size_t>(v)];
}

std::optional<Verdict> parse_verdict(std::string_view t) {
    for (std::size_t i = 0; i < kVerdictWords.size(); ++i)
        if (kVerdictWords[i] == t)
            return static_cast<Verdict>(i);
    for (const auto& old : kVerdictOld)
        if (old.text == t)
            return old.value;
    return std::nullopt;
}

// ---- WeakLimit -----------------------------------------------------------------------------------------

std::string_view word(WeakLimit l) {
    return kLimitWords[static_cast<std::size_t>(l)];
}
std::string_view explanation(WeakLimit l) {
    return kLimitExplained[static_cast<std::size_t>(l)];
}

std::optional<WeakLimit> parse_weak_limit(std::string_view code) {
    for (std::size_t i = 0; i < kLimitWords.size(); ++i)
        if (kLimitWords[i] == code)
            return static_cast<WeakLimit>(i);
    return std::nullopt;
}

WeakLimits parse_limits(std::string_view t) {
    WeakLimits out;
    std::size_t at = 0;
    while (at <= t.size()) {
        std::size_t comma = t.find(',', at);
        if (comma == std::string_view::npos)
            comma = t.size();
        if (auto l = parse_weak_limit(t.substr(at, comma - at)))
            out.insert(*l);
        at = comma + 1;
    }
    return out;
}

std::string limits_text(WeakLimits limits) {
    std::string out;
    for (WeakLimit l : limits) {
        if (!out.empty())
            out += ',';
        out += word(l);
    }
    return out;
}

// ---- Class ---------------------------------------------------------------------------------------------

std::string_view word(Class c) {
    return kClassWords[static_cast<std::size_t>(c)];
}
std::string_view title(Class c) {
    return kClassTitles[static_cast<std::size_t>(c)];
}
std::string_view label(Class c) {
    return kClassLabels[static_cast<std::size_t>(c)];
}

std::optional<Class> parse_class(std::string_view t) {
    const std::string low = lower_trimmed(t);
    for (std::size_t i = 0; i < kClassWords.size(); ++i)
        if (kClassWords[i] == low)
            return static_cast<Class>(i);
    for (const auto& old : kClassOld)
        if (old.text == low)
            return old.value;
    return std::nullopt;
}

Classification classify(std::optional<Verdict> v, std::string_view limits_text) {
    return classify(v, parse_limits(limits_text));
}

// ---- ByteMatch -----------------------------------------------------------------------------------------

std::string_view word(ByteMatch m) {
    return kMatchWords[static_cast<std::size_t>(m)];
}
std::string_view label(ByteMatch m) {
    return kMatchLabels[static_cast<std::size_t>(m)];
}

std::optional<ByteMatch> parse_byte_match(std::string_view t) {
    for (std::size_t i = 0; i < kMatchWords.size(); ++i)
        if (kMatchWords[i] == t)
            return static_cast<ByteMatch>(i);
    for (const auto& old : kMatchOld)
        if (old.text == t)
            return old.value;
    return std::nullopt;
}

// ---- by provenance -------------------------------------------------------------------------------------

Classification Judgement::classify() const {
    WeakLimits weak;
    for (const std::string& code : limits)
        if (auto l = parse_weak_limit(code))
            weak.insert(*l);
    return verify::classify(verdict, weak);
}

std::string Judgement::limits_joined() const {
    return text::join(limits, ",");
}

Judgement not_run() {
    return Judgement{Verdict::Inconclusive, "judge: the emulator crashed on this function", {}, {}};
}

FunctionVerdict of_row(std::string module, const StoreRow& row) {
    FunctionVerdict out;
    out.module = std::move(module);
    out.addr = row.addr;
    if (row.verdict)
        out.verdict = parse_verdict(*row.verdict);
    out.reason = row.reason;
    out.limits = row.limits;
    out.detail = row.detail;
    return out;
}

Index::Index(std::string module, std::unordered_map<std::string, StoreRow> rows)
    : module_(std::move(module)), rows_(std::move(rows)) {}

const StoreRow* Index::at(const Origin& origin) const {
    if (origin.empty() || origin.module != module_)
        return nullptr;
    auto it = rows_.find(origin.va);
    return it == rows_.end() ? nullptr : &it->second;
}

} // namespace cb::verify
