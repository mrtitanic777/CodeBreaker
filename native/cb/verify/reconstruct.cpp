#include "cb/verify/reconstruct.hpp"

#include <algorithm>
#include <chrono>
#include <format>
#include <unordered_set>

#include "cb/core/bytes.hpp"
#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/jobs/pool.hpp"
#include "cb/platforms/package.hpp"

namespace cb::verify::reconstruct {

namespace {

using Clock = std::chrono::steady_clock;

double seconds_since(Clock::time_point t) { return std::chrono::duration<double>(Clock::now() - t).count(); }

constexpr bool is_word_byte(char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }
constexpr bool starts_word(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'; }

// Every identifier-shaped run in `text`, found the way a scan for [A-Za-z_]\w* finds them: a digit is not the start of
// one, but a letter inside a number is ("0x1f" holds "x1f").
template <class Each>
void for_each_identifier(std::string_view text, Each&& each) {
    std::size_t i = 0;
    while (i < text.size()) {
        if (!starts_word(text[i])) {
            ++i;
            continue;
        }
        std::size_t end = i + 1;
        while (end < text.size() && is_word_byte(text[end]))
            ++end;
        each(text.substr(i, end - i));
        i = end;
    }
}

// Every maximal run of word bytes of `text` that is an identifier and is followed (after blanks) by "(": the names a text calls.
template <class Each>
void for_each_called_name(std::string_view text, Each&& each) {
    std::size_t i = 0;
    while (i < text.size()) {
        if (!is_word_byte(text[i])) {
            ++i;
            continue;
        }
        std::size_t end = i + 1;
        while (end < text.size() && is_word_byte(text[end]))
            ++end;
        std::size_t after = end;
        while (after < text.size() && (text[after] == ' ' || text[after] == '\t' || text[after] == '\n' || text[after] == '\r' ||
                                       text[after] == '\f' || text[after] == '\v'))
            ++after;
        if (starts_word(text[i]) && after < text.size() && text[after] == '(')
            each(text.substr(i, end - i));
        i = end;
    }
}

// The text of a function after its first "{" (all of it when it has none): what the function does, not its declaration.
std::string_view body_of(std::string_view text) {
    const std::size_t brace = text.find('{');
    return brace == std::string_view::npos ? text : text.substr(brace + 1);
}

std::string hex_sha1(std::string_view text) { return crypto::to_hex(crypto::sha1(as_bytes(text))); }

std::string module_name(const std::filesystem::path& module_path) {
    std::filesystem::path p = module_path.lexically_normal();
    if (!p.has_filename())
        p = p.parent_path();
    return text::lower(p.filename().generic_string());
}

const std::string_view kNul("\0", 1);

}  // namespace

// ---- Kinds -------------------------------------------------------------------------------------------------------------

void Kinds::add(std::shared_ptr<const lanes::Kind> kind, KindTraits traits) {
    ++generation_;
    for (Entry& e : entries_) {
        if (e.kind->name() == kind->name()) {
            e = Entry{std::move(kind), std::move(traits)};
            return;
        }
    }
    entries_.push_back(Entry{std::move(kind), std::move(traits)});
}

const lanes::Kind* Kinds::find(std::string_view name) const {
    for (const Entry& e : entries_)
        if (e.kind->name() == name)
            return e.kind.get();
    return nullptr;
}

const KindTraits& Kinds::traits(std::string_view name) const {
    static const KindTraits kDefaults;
    for (const Entry& e : entries_)
        if (e.kind->name() == name)
            return e.traits;
    return kDefaults;
}

std::vector<std::string> Kinds::names() const {
    std::vector<std::string> out;
    out.reserve(entries_.size());
    for (const Entry& e : entries_)
        out.push_back(e.kind->name());
    return out;
}

// ---- identity ----------------------------------------------------------------------------------------------------------

std::optional<std::string> Reconstructor::kind_name(const std::filesystem::path& module_path) const {
    if (!services_.kind_of_file)
        return std::nullopt;
    return services_.kind_of_file(module_path);
}

const lanes::Kind& Reconstructor::kind_of(const std::filesystem::path& module_path) const {
    const std::optional<std::string> name = kind_name(module_path);
    if (!name)
        throw UsageError(std::format("{}: no way to reconstruct this kind of file yet", module_path.filename().generic_string()));
    const lanes::Kind* kind = services_.kinds.find(*name);
    if (!kind)
        throw UsageError(std::format("{}: no lane is registered for {} files", module_path.filename().generic_string(), *name));
    return *kind;
}

std::string Reconstructor::module_sha(std::string_view kind, const std::filesystem::path& module_path) const {
    const KindTraits& traits = services_.kinds.traits(kind);
    std::optional<std::string> sha;
    if (traits.sha)
        sha = traits.sha(module_path);
    else if (services_.content_sha)
        sha = services_.content_sha(module_path);
    return sha.value_or("");
}

std::map<std::string, std::optional<std::string>> Reconstructor::digests() const {
    if (digests_generation_ != services_.kinds.generation()) {
        digests_.clear();
        for (const std::string& name : services_.kinds.names()) {
            // A lane whose tools are not set up here has no digest: its modules are not measured, which is not an error.
            try {
                digests_[name] = services_.kinds.find(name)->profile({}).digest;
            } catch (const Error& e) {   // isolation boundary: one lane of the set
                log::debug(std::format("reconstruct: no verifier profile for {} files: {}", name, e.what()));
                digests_[name] = std::nullopt;
            }
        }
        digests_generation_ = services_.kinds.generation();
    }
    return digests_;
}

Identity Reconstructor::identity(const std::filesystem::path& module_path) const {
    const lanes::Kind& kind = kind_of(module_path);
    return Identity{.sha = module_sha(kind.name(), module_path), .profile = kind.profile(module_path).digest};
}

std::optional<Identity> Reconstructor::try_identity(const std::filesystem::path& module_path) const {
    const std::optional<std::string> name = kind_name(module_path);
    if (!name)
        return std::nullopt;
    const lanes::Kind* kind = services_.kinds.find(*name);
    if (!kind)
        return std::nullopt;
    try {
        return Identity{.sha = module_sha(*name, module_path), .profile = kind->profile(module_path).digest};
    } catch (const Error& e) {   // the lane's tools are not set up: no identity (see header)
        log::debug(std::format("reconstruct: no identity for {}: {}", module_path.generic_string(), e.what()));
        return std::nullopt;
    }
}

std::function<std::optional<std::string>(const std::filesystem::path&)> platform_kind(const platforms::Registry& registry) {
    return [&registry](const std::filesystem::path& path) -> std::optional<std::string> {
        const platforms::Platform* platform = registry.identify(path);
        if (platform && platform->lane)
            return platform->name;
        return std::nullopt;
    };
}

// ---- signatures --------------------------------------------------------------------------------------------------------

std::set<std::string> reaches(const std::unordered_map<std::string, std::string>& texts, std::span<const std::string> built,
                              const std::set<std::string>& dropped) {
    if (dropped.empty())
        return {};
    const std::set<std::string_view> in_built(built.begin(), built.end());
    // calls[n]: the functions of the build that n's text calls
    std::unordered_map<std::string_view, std::vector<std::string_view>> calls;   // lookup only
    std::set<std::string_view> tainted;                                          // those that call a dropped function directly
    for (const std::string& n : built) {
        const std::string& text = texts.at(n);
        std::vector<std::string_view>& mine = calls[n];
        for_each_called_name(text, [&](std::string_view callee) {
            if (in_built.contains(callee))
                mine.push_back(callee);
        });
        bool direct = false;
        for_each_called_name(body_of(text), [&](std::string_view callee) { direct = direct || dropped.contains(std::string(callee)); });
        if (direct)
            tainted.insert(n);
    }
    // whatever calls a tainted function is tainted
    for (bool grew = true; grew;) {
        grew = false;
        for (const std::string& n : built) {
            if (tainted.contains(n))
                continue;
            for (std::string_view callee : calls[n]) {
                if (tainted.contains(callee)) {
                    tainted.insert(n);
                    grew = true;
                    break;
                }
            }
        }
    }
    return std::set<std::string>(tainted.begin(), tainted.end());
}

std::map<std::string, std::string> signatures(const lanes::Prepared& prepared, std::string_view whole,
                                              const std::map<std::string, std::string>* own) {
    const auto& texts = prepared.texts;
    if (texts.empty() || prepared.header.empty()) {
        std::map<std::string, std::string> out;
        for (const std::string& n : prepared.order)
            out[n] = std::string(whole);
        return out;
    }

    // The functions in the order the preparation gives them (by address), then any text it holds that it did not order.
    std::vector<std::string> names;
    names.reserve(texts.size());
    std::set<std::string_view> listed;
    for (const std::string& n : prepared.order)
        if (texts.contains(n) && listed.insert(n).second)
            names.push_back(n);
    {
        std::vector<std::string> rest;
        for (const auto& [n, _] : texts)
            if (!listed.contains(n))
                rest.push_back(n);
        std::sort(rest.begin(), rest.end());
        names.insert(names.end(), rest.begin(), rest.end());
    }

    // calls[n]: the functions of the module that n's text mentions, but n, sorted
    std::unordered_map<std::string_view, std::vector<std::string_view>> calls;   // lookup only
    std::unordered_set<std::string_view> is_function;                           // the names with a text
    for (const std::string& n : names)
        is_function.insert(n);
    for (const std::string& n : names) {
        std::set<std::string_view> found;
        for_each_identifier(texts.at(n), [&](std::string_view word) {
            if (word != n && is_function.contains(word))
                found.insert(*is_function.find(word));
        });
        calls[n].assign(found.begin(), found.end());
    }

    // The strongly connected groups (Tarjan, without recursion: call chains are long), callees first.
    std::unordered_map<std::string_view, std::size_t> index, low, group;   // lookup only
    std::set<std::string_view> on_stack;
    std::vector<std::string_view> stack;
    std::vector<std::vector<std::string_view>> groups;   // members in the order the walk popped them
    std::size_t counter = 0;
    for (const std::string& root : names) {
        if (index.contains(root))
            continue;
        std::vector<std::pair<std::string_view, std::size_t>> work{{root, 0}};
        while (!work.empty()) {
            auto [n, i] = work.back();
            work.pop_back();
            if (i == 0) {
                index[n] = low[n] = counter++;
                stack.push_back(n);
                on_stack.insert(n);
            }
            const std::vector<std::string_view>& callees = calls[n];
            bool descended = false;
            for (std::size_t j = i; j < callees.size(); ++j) {
                const std::string_view c = callees[j];
                if (!index.contains(c)) {
                    work.emplace_back(n, j + 1);
                    work.emplace_back(c, 0);
                    descended = true;
                    break;
                }
                if (on_stack.contains(c))
                    low[n] = std::min(low[n], index[c]);
            }
            if (descended)
                continue;
            if (low[n] == index[n]) {
                std::vector<std::string_view> members;
                for (;;) {
                    const std::string_view x = stack.back();
                    stack.pop_back();
                    on_stack.erase(x);
                    members.push_back(x);
                    if (x == n)
                        break;
                }
                groups.push_back(members);
                for (std::string_view x : members)
                    group[x] = groups.size() - 1;
            }
            if (!work.empty()) {
                const std::string_view parent = work.back().first;
                low[parent] = std::min(low[parent], low[n]);
            }
        }
    }

    const std::string base = own ? "built" : hex_sha1(prepared.header);
    std::vector<std::string> signature_of;   // by group
    std::map<std::string, std::string> out;
    for (std::size_t g = 0; g < groups.size(); ++g) {   // callees come before their callers
        crypto::Hasher h(crypto::Hasher::Algorithm::Sha1);
        h.update(as_bytes(base));
        std::vector<std::string_view> members = groups[g];
        std::sort(members.begin(), members.end());
        for (std::string_view x : members) {
            std::string body;
            if (own) {
                const auto it = own->find(std::string(x));
                body = it == own->end() ? "absent" : it->second;
            } else {
                body = texts.at(std::string(x));
            }
            h.update(as_bytes(x));
            h.update(as_bytes(kNul));
            h.update(as_bytes(body));
            h.update(as_bytes(kNul));
        }
        std::set<std::size_t> others;   // the groups these call, in group order
        for (std::string_view x : members)
            for (std::string_view c : calls[x])
                others.insert(group[c]);
        others.erase(g);
        for (std::size_t other : others)
            h.update(as_bytes(signature_of[other]));
        signature_of.push_back(crypto::to_hex(h.finish()));
        for (std::string_view x : members)
            out[std::string(x)] = signature_of.back();
    }
    return out;
}

// ---- a run -------------------------------------------------------------------------------------------------------------

std::string Outcome::line() const {
    std::string parts;
    for (const Class c : kClassScored) {
        const auto it = scores.find(c);
        if (it == scores.end() || it->second == 0)
            continue;
        if (!parts.empty())
            parts += ", ";
        parts += std::format("{} {}", title(c), it->second);
    }
    return std::format("{}: {} functions, {} judged now{}: {}  ({:.0f}s: decompile {:.0f}, build {:.0f}, judge {:.0f})", name, functions, judged,
                       kept ? std::format(", {} kept (same code)", kept) : std::string(), parts, seconds, decompile_seconds, build_seconds,
                       judge_seconds);
}

namespace {

// One function's row as record_many takes it.
engine::Record row(std::string addr, std::string sig, std::string name, std::optional<std::string> source, Verdict verdict, std::string reason,
                   std::string limits = {}, std::string detail = {}, std::optional<std::string> problem = std::nullopt) {
    engine::Record r;
    r.addr = std::move(addr);
    r.sig = std::move(sig);
    r.name = std::move(name);
    r.source = std::move(source);
    r.verdict = std::string(word(verdict));
    r.reason = std::move(reason);
    r.limits = std::move(limits);
    r.detail = std::move(detail);
    r.problem = std::move(problem);
    return r;
}

// The weak limits a good verdict gets when a function it calls was a stand-in: its own, and "stub-callee", sorted.
std::string with_stub_callee(std::vector<std::string> limits) {
    limits.push_back(std::string(word(WeakLimit::StubCallee)));
    std::sort(limits.begin(), limits.end());
    limits.erase(std::unique(limits.begin(), limits.end()), limits.end());
    return text::join(limits, ",");
}

}  // namespace

Outcome Reconstructor::run(const std::filesystem::path& module_path) const {
    const auto started = Clock::now();
    const lanes::Kind& kind = kind_of(module_path);
    const std::string& kind_id = kind.name();
    const KindTraits& traits = services_.kinds.traits(kind_id);
    engine::FunctionStore& store = services_.store;

    const std::string sha = module_sha(kind_id, module_path);
    const lanes::Profile profile = kind.profile(module_path);
    const std::string name = module_name(module_path);
    const std::filesystem::path work = services_.scratch / "reconstruct" / kind_id / name;

    const std::unique_ptr<lanes::Prepared> prepared_ptr = kind.prepare(module_path, work);
    const lanes::Prepared& m = *prepared_ptr;
    Outcome out{.sha = sha, .profile = profile.digest, .name = name};
    out.decompile_seconds = seconds_since(started);

    std::map<std::string, std::string> sig;
    if (traits.per_function) {
        sig = signatures(m, m.sig);
    } else {
        for (const std::string& n : m.order)
            sig[n] = m.sig;
    }

    store.add_module(sha, name, module_path.generic_string(), kind_id);
    store.add_profile(profile.digest, profile.settings);
    store.add_functions(sha, m.all_keys);
    store.put_text(sha, kSource, "header", m.header);
    if (traits.identifies_library && services_.identify)
        store.set_identified(sha, services_.identify(module_path));

    const std::map<std::string, engine::Candidate> known = store.judged(sha, profile.digest, kSource);
    std::vector<std::string> fresh;   // the functions with no verdict for the text they have now
    for (const std::string& n : m.order) {
        const auto key = m.key.find(n);
        if (key == m.key.end())
            continue;
        const auto had = known.find(key->second);
        if (had == known.end() || had->second.sig != sig.at(n))
            fresh.push_back(n);
    }

    std::vector<engine::Record> rows;
    {
        std::set<std::string_view> have;
        for (const auto& [_, k] : m.key)
            have.insert(k);
        for (const std::string& k : m.all_keys)
            if (!have.contains(k))
                rows.push_back(row(k, "-", "", std::nullopt, Verdict::NoSource, "the decompiler produced nothing for this function", {}, {}, "none"));
    }

    auto build_started = Clock::now();
    double build_done = 0, judge_done = 0;
    std::size_t carried = 0;
    if (!fresh.empty()) {
        lanes::Built built = kind.build(m, module_path, work);
        build_done = seconds_since(build_started);
        const std::set<std::string_view> in_build(built.items.begin(), built.items.end());

        // What was built, per function: its own object code and that of everything it reaches. A function whose text changed but
        // whose compiled code did not -- nor that of anything it calls -- is the same function to a judge that runs code: its
        // verdict is kept.
        if (traits.per_function && traits.object_digests) {
            const std::map<std::string, std::string> objects = traits.object_digests(work);
            if (!objects.empty()) {
                const std::map<std::string, std::string> built_now = signatures(m, {}, &objects);
                if (!built_now.empty()) {
                    const auto before = store.built(sha, profile.digest);
                    std::map<std::string, engine::Candidate> had;
                    for (engine::Candidate& c : store.candidates(sha, profile.digest, kSource))
                        if (c.verdict)
                            had[c.addr] = std::move(c);
                    std::set<std::string> keep;
                    for (const std::string& n : fresh) {
                        const std::string& k = m.key.at(n);
                        const auto b = before.find(k);
                        const auto now = built_now.find(n);
                        if (!in_build.contains(n) || b == before.end() || !b->second || b->second->empty() || now == built_now.end() ||
                            *b->second != now->second)
                            continue;
                        const auto c = had.find(k);
                        if (c == had.end())
                            continue;
                        const std::optional<Verdict> v = parse_verdict(*c->second.verdict);
                        if (v == Verdict::NoCompile || v == Verdict::NoSource)
                            continue;
                        engine::Record r;
                        r.addr = k;
                        r.sig = sig.at(n);
                        r.name = n;
                        r.source = m.texts.at(n);
                        r.verdict = c->second.verdict;
                        r.reason = c->second.reason;
                        r.limits = c->second.limits;
                        r.detail = c->second.detail;
                        rows.push_back(std::move(r));
                        keep.insert(n);
                    }
                    carried = keep.size();
                    std::erase_if(fresh, [&](const std::string& n) { return keep.contains(n); });
                    std::map<std::string, std::string> by_key;
                    for (const auto& [n, s] : built_now)
                        if (const auto k = m.key.find(n); k != m.key.end())
                            by_key[k->second] = s;
                    store.set_built(sha, profile.digest, by_key);
                }
            }
        }

        std::set<std::string> dropped;
        for (const auto& [n, _] : built.missing)
            dropped.insert(n);
        const std::set<std::string> tainted = reaches(m.texts, built.items, dropped);

        for (const std::string& n : fresh) {
            if (in_build.contains(n))
                continue;
            const auto why = built.missing.find(n);
            rows.push_back(row(m.key.at(n), sig.at(n), n, m.texts.at(n), Verdict::NoCompile,
                               why == built.missing.end() || why->second.empty() ? "not in the build" : why->second, {}, {}, "compile"));
        }

        std::vector<std::string> to_judge;
        for (const std::string& n : fresh)
            if (in_build.contains(n))
                to_judge.push_back(n);
        std::map<std::string, Judgement> judged;
        std::optional<std::map<std::string, Judgement>> batch;
        if (services_.judge_batch)
            batch = services_.judge_batch(kind, built, to_judge);
        if (batch) {
            judged = std::move(*batch);
        } else {
            // One function's judge failing alone is that function's "not run"; the others go on.
            const auto judge_one = [&](const std::string& n) {
                try {
                    return built.judge(n);
                } catch (const std::exception& e) {   // isolation boundary: one function of a batch
                    log::warn(std::format("reconstruct: judging {} in {} failed: {}", n, name, e.what()));
                    return not_run();
                }
            };
            if (services_.judge_threads <= 1) {
                for (const std::string& n : to_judge)
                    judged.emplace(n, judge_one(n));
            } else {
                const std::vector<Judgement> answers = jobs::threads(to_judge, judge_one, services_.judge_threads);
                for (std::size_t i = 0; i < to_judge.size(); ++i)
                    judged.emplace(to_judge[i], answers[i]);
            }
        }
        for (const std::string& n : to_judge) {
            const auto got = judged.find(n);
            const Judgement j = got == judged.end() ? not_run() : got->second;
            const std::string limits = tainted.contains(n) && is_good(j.verdict) ? with_stub_callee(j.limits) : j.limits_joined();
            rows.push_back(row(m.key.at(n), sig.at(n), n, m.texts.at(n), j.verdict, j.reason, limits, j.detail));
        }
        judge_done = seconds_since(build_started) - build_done;
    }

    store.record_many(sha, kSource, traits.source_rev.value_or(services_.decompiler_rev), profile.digest, rows);

    out.functions = m.all_keys.size();
    out.judged = fresh.size();
    out.kept = carried;
    out.scores = store.scores(profile.digest, sha);
    out.build_seconds = build_done;
    out.judge_seconds = judge_done;
    out.seconds = seconds_since(started);
    if (services_.log)
        services_.log(out.line());
    return out;
}

}  // namespace cb::verify::reconstruct
