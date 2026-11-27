// verify/lane_common.cpp -- see lane_common.hpp.
#include "verify/lane_common.hpp"

#include <algorithm>
#include <format>

#include "core/crypto.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::verify::lane_common {

namespace {

// An environment switch is on when it is set and not empty.
bool env_on(const char* name) {
    const std::optional<std::string> v = fs::get_env(name);
    return v && !v->empty();
}

// Byte `i` of an `n`-byte value as the emulator wrote it: little-endian bytes count from the low end, big-endian from
// the high end. A value holds at most eight bytes; the rest read as zero.
std::uint8_t byte_of(std::uint64_t value, std::size_t i, std::size_t n, bool little) {
    const std::size_t shift = 8 * (little ? i : n - 1 - i);
    return shift < 64 ? static_cast<std::uint8_t>((value >> shift) & 0xFF) : std::uint8_t{0};
}

// Text in single quotes, with the quote and the backslash escaped: how the digest names a tag or a token.
std::string single_quoted(std::string_view s) {
    std::string out = "'";
    for (char c : s) {
        if (c == '\'' || c == '\\') out += '\\';
        out += c;
    }
    out += '\'';
    return out;
}

}  // namespace

// ---- the switches ---------------------------------------------------------------------------------------------------

bool fp_cache_on() { return !env_on("CB_NO_FP_CACHE"); }
bool guided_on() { return !env_on("CB_NO_GUIDED"); }
bool concolic_on() { return !env_on("CB_NO_CONCOLIC"); }
bool plain() { return env_on("CB_PLAIN"); }

// ---- fingerprints kept across runs ----------------------------------------------------------------------------------

FingerprintStore store_of(base::cache::Cache& cache) {
    return FingerprintStore{
        [&cache](std::string_view key) { return compare::cache_get(cache, key); },
        [&cache](std::string_view key, const compare::Fingerprint& fp) { compare::cache_put(cache, key, fp); },
    };
}

compare::Fingerprint remembered(const FingerprintStore& store, const std::optional<std::string>& key,
                                const std::function<compare::Fingerprint()>& run,
                                const std::function<bool(const compare::Fingerprint&)>& keep) {
    if (!key || !fp_cache_on() || !store.get) return run();
    if (std::optional<compare::Fingerprint> known = store.get(*key)) return *known;
    compare::Fingerprint fp = run();
    if (!fp.err && (!keep || keep(fp)) && store.put) store.put(*key, fp);
    return fp;
}

// ---- calls, as two builds agree on them -----------------------------------------------------------------------------

std::string call(std::string_view name, const std::optional<std::vector<std::string>>& args) {
    if (!args) return std::string(name);
    return std::format("{}({})", name, text::join(*args, ","));
}

std::vector<Called> called(const emulate::Run& run) {
    std::vector<Called> out;
    out.reserve(run.calls.size());
    for (const emulate::Call& c : run.calls) out.push_back(Called{c.name, c.args});
    return out;
}

std::vector<std::string> calls(std::span<const Called> seq, const std::function<std::string(std::int64_t)>& argument,
                               const std::function<std::string(const std::optional<std::string>&)>& name,
                               const std::function<bool(const std::optional<std::string>&)>& skip, std::optional<std::size_t> cap) {
    std::vector<std::string> out;
    for (const Called& c : seq) {
        if (skip && skip(c.name)) continue;
        if (cap && out.size() >= *cap) {
            out.emplace_back("...");
            break;
        }
        const std::string shown = name ? name(c.name) : c.name.value_or("?");
        std::optional<std::vector<std::string>> args;
        if (c.args) {
            args.emplace();
            for (std::int64_t a : *c.args) args->push_back(argument(a));
        }
        out.push_back(call(shown, args));
    }
    return out;
}

std::string argument(std::int64_t v, const ArgumentContext& context) {
    // an input of the function, when it is above `above`
    const auto own = std::find(context.inputs.begin(), context.inputs.end(), v);
    if (own != context.inputs.end() && (!context.above || v > *context.above)) {
        return std::format("a{}", own - context.inputs.begin());
    }
    if (context.small) {
        const std::int64_t signed_v = (v & 0x80000000) != 0 ? v - (std::int64_t{1} << 32) : v;
        if (-0x10000 < signed_v && signed_v < 0x10000) return std::format("{}", signed_v);
    }
    for (const std::optional<AddressRange>& range : context.frame) {
        if (range && range->contains(v)) return "S";
    }
    if (const auto* range = std::get_if<AddressRange>(&context.image)) {
        if (range->contains(v)) return "P";
    } else if (const auto* test = std::get_if<std::function<bool(std::int64_t)>>(&context.image)) {
        if (*test && (*test)(v)) return "P";
    }
    return std::format("{:x}", v);
}

// ---- memory a run left changed --------------------------------------------------------------------------------------

std::string writes(const State& state) {
    if (state.empty()) return {};
    std::string text = "[";
    bool first = true;
    for (const auto& [place, held] : state) {
        if (!first) text += ", ";
        first = false;
        text += '(';
        if (const auto* address = std::get_if<std::uint64_t>(&place)) {
            text += std::to_string(*address);
        } else {
            const TaggedPlace& tagged = std::get<TaggedPlace>(place);
            text += std::format("({}, {})", single_quoted(tagged.tag), tagged.offset);
        }
        text += ", ";
        if (const auto* byte = std::get_if<std::uint8_t>(&held)) {
            text += std::to_string(static_cast<unsigned>(*byte));
        } else {
            text += single_quoted(std::get<std::string>(held));
        }
        text += ')';
    }
    text += ']';
    const crypto::Sha1Digest digest = crypto::sha1(as_bytes(text));
    return std::format("{}:{}", state.size(), crypto::to_hex(digest).substr(0, 16));
}

State changed(const emulate::Run& run, const ChangedOptions& options) {
    State state;
    std::map<std::uint64_t, std::optional<std::uint8_t>> before;   // what each byte held before the first write to it
    std::map<Place, std::uint64_t> at;                              // the address each place was first written through
    const bool little = run.endian == emulate::Endian::Little;
    for (const emulate::MemoryWrite& w : run.writes) {
        const std::uint64_t a = w.address;
        const std::size_t n = std::min<std::size_t>(w.size, options.most);
        auto old_byte = [&](std::size_t i) -> std::optional<std::uint8_t> {
            if (!w.old) return std::nullopt;
            return byte_of(*w.old, i, n, little);
        };
        if (options.everything) {
            for (std::size_t i = 0; i < n; ++i) before.try_emplace(a + i, old_byte(i));
        }
        if (w.harness) continue;
        const Where where = options.place ? options.place(a) : Where(Place(a));
        if (std::holds_alternative<Skip>(where)) continue;
        const std::optional<std::string> token = (options.word && n == 4) ? options.word(w.value) : std::nullopt;
        for (std::size_t i = 0; i < n; ++i) {
            Place key;
            if (const auto* namer = std::get_if<ByteNamer>(&where)) {
                std::optional<Place> named = (*namer)(i);
                if (!named) continue;
                key = *named;
            } else if (const auto* first = std::get_if<Place>(&where)) {
                if (const auto* address = std::get_if<std::uint64_t>(first)) {
                    key = Place(*address + i);
                } else {
                    const TaggedPlace& tagged = std::get<TaggedPlace>(*first);
                    key = Place(TaggedPlace{tagged.tag, tagged.offset + static_cast<std::int64_t>(i)});
                }
            }
            if (!options.everything) before.try_emplace(a + i, old_byte(i));
            state.insert_or_assign(key, token ? Held(*token) : Held(byte_of(w.value, i, n, little)));
            at.try_emplace(key, a + i);
        }
    }
    State out;
    for (const auto& [place, held] : state) {
        const auto* byte = std::get_if<std::uint8_t>(&held);
        const std::optional<std::uint8_t>& was = before.at(at.at(place));
        if (!byte || !was || *byte != *was) out.emplace(place, held);   // a token, or a byte that is not what it was
    }
    return out;
}

std::optional<std::string> left(const emulate::Run& run, const ChangedOptions& options) {
    if (run.capped) return std::nullopt;
    return writes(changed(run, options));
}

// ---- from a run to a fingerprint ------------------------------------------------------------------------------------

compare::Fingerprint fingerprint(const emulate::Run& run, std::vector<std::string> deps, FingerprintParts parts) {
    const std::string how = run.ended();
    const bool done = how == compare::kExitReturned;
    compare::Fingerprint fp;
    fp.ret = parts.ret;
    fp.exit = how;
    fp.truncated = run.truncated;
    fp.deps = std::move(deps);
    fp.writes = std::move(parts.writes);
    if (done) {
        fp.pop = run.stack;
        fp.saved = parts.saved;
    }
    fp.cov = std::move(parts.cov);
    fp.reach = std::move(parts.reach);
    fp.steps = parts.steps;
    return fp;
}

// ---- inputs ---------------------------------------------------------------------------------------------------------

std::vector<std::uint64_t> counting(std::size_t n, std::uint64_t first, std::uint64_t step) {
    std::vector<std::uint64_t> out;
    out.reserve(n);
    for (std::size_t i = 0; i < n; ++i) out.push_back(first + step * i);
    return out;
}

std::vector<std::uint64_t> descending(std::size_t n, std::uint64_t top) {
    std::vector<std::uint64_t> out;
    out.reserve(n);
    for (std::size_t i = 0; i < n; ++i) out.push_back(top - i);
    return out;
}

std::vector<std::uint64_t> alternating(std::size_t n, const Pattern& even, const Pattern& odd) {
    auto at = [](const Pattern& p, std::size_t i) {
        return std::holds_alternative<std::uint64_t>(p) ? std::get<std::uint64_t>(p) : std::get<1>(p)(i);
    };
    std::vector<std::uint64_t> out;
    out.reserve(n);
    for (std::size_t i = 0; i < n; ++i) out.push_back(i % 2 == 0 ? at(even, i) : at(odd, i));
    return out;
}

std::vector<guided::Input> guided(const guided::Run& run, std::span<const guided::Input> seeds, std::uint64_t seed,
                                  guided::Machine machine) {
    if (!guided_on()) return {};
    guided::Options options;
    options.budget = kGuidedBudget;
    options.keep = kMaxGuided;
    options.seed = seed;
    options.machine = machine;
    return guided::guided_inputs(run, seeds, options);
}

std::vector<std::string> path_limits(const concolic::Stats* got) {
    if (got == nullptr) return {};
    std::vector<std::string> out{std::format("paths={}", got->paths)};
    if (got->complete) out.emplace_back("paths-all");
    return out;
}

// ---- the verdict ----------------------------------------------------------------------------------------------------

store::Judgement judged(const std::function<store::Judgement()>& compare_one) {
    // One function must not sink the rest: this is an isolation boundary, the failure becomes the function's verdict.
    try {
        return compare_one();
    } catch (const NotFoundError&) {
        throw;
    } catch (const Error& e) {
        return store::Judgement{store::Verdict::Inconclusive, std::format("judge: {}", e.kind()), {}, {}};
    } catch (const std::exception&) {
        return store::Judgement{store::Verdict::Inconclusive, "judge: Exception", {}, {}};
    }
}

}  // namespace cb::verify::lane_common
