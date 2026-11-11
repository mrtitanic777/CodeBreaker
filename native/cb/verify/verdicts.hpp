// cb/verify/verdicts.hpp -- one verdict vocabulary: what a judge says of a function or a source pair, binary or
// source alike. Every later wave (compare, the lanes, the function store, the module writer, the terminal, the
// MCP tools) builds on these types; the words people and stored files read are produced from them in one place.
//
// Three things are said, each in its own enum.
//
// 1. What a JUDGE says of a pair -- a reconstructed function beside the original in an emulator, a module's
//    image beside the original's, a port's program beside the original, the 16-bit gate's C beside the module's
//    own asm, a Java class beside its original:
//
//        Equivalent     behaves as the original: every compared result alike
//        Consistent     alike only in failing -- neither side returned normally (counts as weak)
//        Divergent      behaves otherwise (the reason says how: ret-differs, writes-differ, ...)
//        Inconclusive   not judged: it could not be run, or a verdict was carried, not reached here
//        NoCompile      it did not build
//        NoSource       nothing to judge: no reconstruction of it
//
// 2. What a verdict AMOUNTS TO, best first (the function store's classes, what the module writer, the terminal,
//    the MCP tools, the ledger and the reports count): Class. Its values are ordered best first, so `a < b`
//    means "a is the better class". `title` is how a source file and the reports write a class, `label` how the
//    terminal does in a narrow column; `word` is the stored spelling.
//
// 3. What a BYTE comparison says (a source compiled and held to the original's bytes: verify/authored, the
//    store's variant table): ByteMatch.
//
// A match is made weaker by WeakLimits (a run cut short, a stand-in callee, ...): a set of them, a bitmask.
//
// Older spellings are read as the words now (`parse_*`): a store or a cache written before keeps meaning what it
// meant. "equivalent" is the judge's word for behaving alike and nothing else: exact/'s byte ledger (identical,
// differs, "equivalent" up to another compiler state's habits) is a ledger of bytes, not a verdict, and keeps its
// own words. An unknown word is not a verdict: `parse_*` answers nullopt (classify counts that as NoSource).
//
// Everything here is allocation-free except Judgement and the store records, so a judge can classify per input.
#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cb::verify {

// ---- 1. what a judge says -------------------------------------------------------------------------------

enum class Verdict : std::uint8_t { Equivalent, Consistent, Divergent, Inconclusive, NoCompile, NoSource };

inline constexpr std::array<Verdict, 6> kVerdicts = {Verdict::Equivalent,   Verdict::Consistent, Verdict::Divergent,
                                                     Verdict::Inconclusive, Verdict::NoCompile,  Verdict::NoSource};

// The stored word: "equivalent", "consistent", "divergent", "inconclusive", "no-compile", "no-source".
std::string_view word(Verdict v);
// How a judge's word is said in a report of pairs (the port's cases, the gate's log): "same", "differs", ...
std::string_view said(Verdict v);
// A judge's word, as stored or as an older spelling ("same", "differs", "not run", "does not compile", ...).
std::optional<Verdict> parse_verdict(std::string_view text);

// A verdict that is a match (weak or not).
constexpr bool is_good(Verdict v) {
    return v == Verdict::Equivalent || v == Verdict::Consistent;
}
constexpr bool is_good(std::optional<Verdict> v) {
    return v && is_good(*v);
}

// The verdict of a yes/no comparison of a pair.
constexpr Verdict pair_verdict(bool same, bool ran = true, bool built = true) {
    if (!built)
        return Verdict::NoCompile;
    if (!ran)
        return Verdict::Inconclusive;
    return same ? Verdict::Equivalent : Verdict::Divergent;
}

// ---- limits that make a match weaker than it looks --------------------------------------------------------

enum class WeakLimit : std::uint8_t {
    FaultOnly,      // the two sides only ever fail alike; neither returned normally
    LowCoverage,    // under half of the original's instructions ran
    Truncated,      // a run was cut off at the step limit
    Unobserved,     // some inputs could not be run on one side
    WritesCapped,   // too many memory writes to compare them all
    StubCallee,     // a function it calls was a stand-in, not that function's reconstruction
    ArgcUnknown,    // an API call was compared by name only
    CallsCapped,    // it makes more calls than are compared
    MethodsPartial, // only some of the class's methods could be run beside the original (java)
};
inline constexpr unsigned kWeakLimitCount = 9;

// The stored code ("low-coverage") and the sentence that says what it means.
std::string_view word(WeakLimit l);
std::string_view explanation(WeakLimit l);
std::optional<WeakLimit> parse_weak_limit(std::string_view code);

// A set of weak limits. Iterates in the order of the enum.
class WeakLimits {
public:
    constexpr WeakLimits() = default;
    constexpr WeakLimits(std::initializer_list<WeakLimit> limits) {
        for (WeakLimit l : limits)
            insert(l);
    }
    constexpr void insert(WeakLimit l) { bits_ = static_cast<std::uint16_t>(bits_ | (1u << static_cast<unsigned>(l))); }
    constexpr bool contains(WeakLimit l) const { return ((bits_ >> static_cast<unsigned>(l)) & 1u) != 0; }
    constexpr bool empty() const { return bits_ == 0; }
    constexpr std::size_t size() const { return static_cast<std::size_t>(std::popcount(static_cast<unsigned>(bits_))); }
    friend constexpr bool operator==(WeakLimits a, WeakLimits b) = default;

    class iterator {
    public:
        constexpr WeakLimit operator*() const { return static_cast<WeakLimit>(index_); }
        constexpr iterator& operator++() {
            ++index_;
            skip();
            return *this;
        }
        constexpr bool operator==(const iterator& o) const { return index_ == o.index_; }

    private:
        friend class WeakLimits;
        constexpr iterator(std::uint16_t bits, unsigned index) : bits_(bits), index_(index) { skip(); }
        constexpr void skip() {
            while (index_ < kWeakLimitCount && ((bits_ >> index_) & 1u) == 0)
                ++index_;
        }
        std::uint16_t bits_;
        unsigned index_;
    };
    constexpr iterator begin() const { return iterator(bits_, 0); }
    constexpr iterator end() const { return iterator(bits_, kWeakLimitCount); }

private:
    std::uint16_t bits_ = 0;
};

// The weak limits named in a stored, comma-separated limits text; codes that are not weak limits (a lane's own
// notes, like "layout-differs") are ignored. The codes joined back, in enum order: `limits_text`.
WeakLimits parse_limits(std::string_view text);
std::string limits_text(WeakLimits limits);

// ---- 2. what it amounts to --------------------------------------------------------------------------------

// Best first: `a < b` means a is the better class.
enum class Class : std::uint8_t {
    Verified,   // behaves as the original, on inputs that covered it
    Weak,       // a match, but a limit makes it weaker than it looks
    Identified, // a runtime library's function: linked, not written; counted on its own
    Unverified, // the judge found it behaves otherwise
    NotJudged,  // it could not be run
    NoCompile,
    NoSource,
};

// The stored words, best first: Verified, Weak, Unverified, NotJudged, NoCompile, NoSource.
inline constexpr std::array<Class, 6> kClassOrder = {Class::Verified,  Class::Weak,      Class::Unverified,
                                                     Class::NotJudged, Class::NoCompile, Class::NoSource};
// The same with Identified third: the classes a scoreboard counts.
inline constexpr std::array<Class, 7> kClassScored = {Class::Verified,   Class::Weak,      Class::Identified,
                                                      Class::Unverified, Class::NotJudged, Class::NoCompile,
                                                      Class::NoSource};
// Compiled and run.
inline constexpr std::array<Class, 4> kClassBuilt = {Class::Verified, Class::Weak, Class::Unverified, Class::NotJudged};

std::string_view word(Class c);  // "verified", "weak", "not-judged", ... (the stored spelling)
std::string_view title(Class c); // "VERIFIED (weak)", "DOES NOT COMPILE", ... (source files and reports)
std::string_view label(Class c); // "weak", "does not compile", "runtime library", ... (terminal columns)
// A class as stored, as titled or labelled, or an older spelling ("run-time library"); case and surrounding
// whitespace ignored.
std::optional<Class> parse_class(std::string_view text);

// What a verdict amounts to before limits.
constexpr Class class_of(Verdict v) {
    switch (v) {
    case Verdict::Equivalent:
        return Class::Verified;
    case Verdict::Consistent:
        return Class::Weak;
    case Verdict::Divergent:
        return Class::Unverified;
    case Verdict::Inconclusive:
        return Class::NotJudged;
    case Verdict::NoCompile:
        return Class::NoCompile;
    case Verdict::NoSource:
        return Class::NoSource;
    }
    return Class::NoSource;
}

// A class and, for Weak, why.
struct Classification {
    Class cls = Class::NoSource;
    WeakLimits reasons; // empty unless cls is Weak (a Consistent verdict with no limit gets FaultOnly)
    friend bool operator==(const Classification&, const Classification&) = default;
};

// Equivalent with a weak limit is Weak; Consistent is always Weak; a missing verdict is NoSource. (Inline: the judge
// asks this for every function.)
constexpr Classification classify(std::optional<Verdict> v, WeakLimits limits = {}) {
    if (!v)
        return {Class::NoSource, {}};
    if (*v == Verdict::Equivalent)
        return limits.empty() ? Classification{Class::Verified, {}} : Classification{Class::Weak, limits};
    if (*v == Verdict::Consistent)
        return {Class::Weak, limits.empty() ? WeakLimits{WeakLimit::FaultOnly} : limits};
    return {class_of(*v), {}};
}
Classification classify(std::optional<Verdict> v, std::string_view limits_text);

// ---- 3. a byte comparison ---------------------------------------------------------------------------------

// Best first, except that Identified (linked, not written) stands apart at the end.
enum class ByteMatch : std::uint8_t { Identical, SameSize, Different, NoExtent, NoCompile, Identified };

inline constexpr std::array<ByteMatch, 5> kByteMatches = {
    ByteMatch::Identical, ByteMatch::SameSize, ByteMatch::Different, ByteMatch::NoExtent, ByteMatch::NoCompile};

std::string_view word(ByteMatch m);  // "identical", "same size", "different", "no original extent", ...
std::string_view label(ByteMatch m); // the same, with "runtime library" for Identified
std::optional<ByteMatch> parse_byte_match(std::string_view text);

// ---- by provenance ----------------------------------------------------------------------------------------

// What a judge returns for one function: its verdict, why, the limits it noted (raw codes, as stored: some are not
// weak limits), and a detail text.
struct Judgement {
    Verdict verdict = Verdict::Inconclusive;
    std::string reason;
    std::vector<std::string> limits;
    std::string detail;

    Classification classify() const;   // the verdict with the weak limits among `limits`
    std::string limits_joined() const; // the stored form: the codes joined by ","
};

// The judgement of a function the judge crashed on.
Judgement not_run();

// A function's row of the store, as stored: the verdict is the word as written (any spelling, or absent).
struct StoreRow {
    std::optional<std::string> addr;
    std::optional<std::string> verdict;
    std::string reason, limits, detail;
};

// A row's verdict, its word read as it is now.
struct FunctionVerdict {
    std::string module;
    std::optional<std::string> addr;
    std::optional<Verdict> verdict; // nullopt: absent, or a word that is not a verdict
    std::string reason, limits, detail;
};
FunctionVerdict of_row(std::string module, const StoreRow& row);

// An L2 provenance: the module and the virtual address a node stands for.
struct Origin {
    std::string module, va;
    bool empty() const { return module.empty() && va.empty(); }
};

// A module's rows by L2 provenance. The module writer reads a function's verdict so, from the tree's node, not
// by its name (a function renamed keeps its verdict).
class Index {
public:
    // `rows` by address. Lookup only, never iterated into output: a hash map.
    Index(std::string module, std::unordered_map<std::string, StoreRow> rows);
    const std::string& module() const { return module_; }
    // The row of the function the origin stands for, or null (the origin is another module's, or unknown).
    const StoreRow* at(const Origin& origin) const;
    Origin origin(std::string addr) const { return Origin{module_, std::move(addr)}; }

private:
    std::string module_;
    std::unordered_map<std::string, StoreRow> rows_;
};

} // namespace cb::verify
