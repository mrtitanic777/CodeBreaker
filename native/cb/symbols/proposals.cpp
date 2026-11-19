// cb/symbols/proposals.cpp -- see proposals.hpp.
#include "cb/symbols/proposals.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <nlohmann/json.hpp>
#include <set>

#include "cb/base/corpus.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::symbols::proposals {

namespace {

using Json = nlohmann::json;

bool identifier_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool identifier_part(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }

bool plain_identifier(std::string_view s) { return !s.empty() && identifier_start(s.front()) && std::all_of(s.begin(), s.end(), identifier_part); }

// A word that reads as the name of a function and not as a word of a sentence: "LZInit", "Read_Block" -- a capital after the first letter and
// a small letter somewhere, or an underscore between letters. ("Error", "FATAL" and "failed" are not.)
bool reads_as_a_function_name(std::string_view word) {
    if (word.size() < 4 || !plain_identifier(word)) return false;
    bool lower = false, hump = false, inner_underscore = false;
    for (std::size_t i = 0; i < word.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(word[i]);
        lower = lower || std::islower(c) != 0;
        hump = hump || (i > 0 && std::isupper(c) != 0);
        inner_underscore = inner_underscore || (i > 0 && i + 1 < word.size() && c == '_' && word[i - 1] != '_' && word[i + 1] != '_');
    }
    return lower && (hump || inner_underscore);
}

std::string last_part(std::string_view import_name) {
    const std::size_t bang = import_name.rfind('!');
    return std::string(bang == std::string_view::npos ? import_name : import_name.substr(bang + 1));
}

// An imported function's name as a person writes it: the stdcall decoration (`_Name@8`) left off.
std::string undecorated(std::string name) {
    const std::size_t at = name.rfind('@');
    const bool decorated = at != std::string::npos && at > 1 && at + 1 < name.size() &&
                           std::all_of(name.begin() + static_cast<std::ptrdiff_t>(at) + 1, name.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; });
    if (decorated) {
        name.resize(at);
        if (name.starts_with('_')) name.erase(0, 1);
    }
    return name;
}

std::string shortened(std::string_view s, std::size_t most) {
    std::string out(s.substr(0, most));
    if (s.size() > most) out += "...";
    return out;
}

}  // namespace

bool valid_name(std::string_view name) {
    std::size_t at = 0;
    while (true) {
        const std::size_t cut = name.find("::", at);
        const std::string_view piece = name.substr(at, cut == std::string_view::npos ? std::string_view::npos : cut - at);
        if (!plain_identifier(piece)) return false;
        if (cut == std::string_view::npos) return true;
        at = cut + 2;
    }
}

std::string name_in_message(std::string_view message) {
    std::string_view m = text::trim(message);
    const bool bracketed = m.starts_with('[');
    if (bracketed) m.remove_prefix(1);
    std::size_t n = 0;
    while (n < m.size() && identifier_part(m[n])) ++n;
    const std::string_view word = m.substr(0, n);
    if (!reads_as_a_function_name(word)) return {};
    const std::string_view rest = m.substr(n);
    const bool opens_a_message = (bracketed && rest.starts_with(']')) || rest.starts_with("()") || rest.starts_with(": ") || rest == ":";
    return opens_a_message ? std::string(word) : std::string();
}

std::optional<store::Proposal> import_wrapper(const FunctionEvidence& e) {
    if (!e.forwards_to_import) return std::nullopt;
    const auto call = std::find_if(e.calls.begin(), e.calls.end(), [](const CallEvidence& c) { return c.kind == CallEvidence::Kind::Import; });
    if (call == e.calls.end()) return std::nullopt;
    const std::string name = undecorated(last_part(call->import_name));
    if (!plain_identifier(name)) return std::nullopt;   // (an import by number no table names, or a C++ name)
    store::Proposal p;
    p.name = name + "_thunk";
    p.rule = "import-wrapper";
    p.confidence = e.forwards_purely ? 90 : 80;
    p.evidence = std::format("{} {}", e.forwards_purely ? "only jumps to" : "passes its arguments on to", call->import_name);
    p.description = std::format("Forwards to {}{}.", call->import_name, e.forwards_purely ? "" : " with its own arguments");
    return p;
}

std::optional<store::Proposal> string_reference(const FunctionEvidence& e) {
    std::string name, message;
    for (const std::string& s : e.strings) {
        const std::string found = name_in_message(s);
        if (found.empty()) continue;
        if (!name.empty() && name != found) return std::nullopt;   // two names: the strings are not this function's own
        if (name.empty()) message = s;
        name = found;
    }
    if (name.empty()) return std::nullopt;
    // a function that calls the import of that name is reporting on it, not being it
    for (const CallEvidence& c : e.calls)
        if (c.kind == CallEvidence::Kind::Import && text::lower(undecorated(last_part(c.import_name))) == text::lower(name)) return std::nullopt;
    store::Proposal p;
    p.name = name;
    p.rule = "string-reference";
    p.confidence = 50;
    p.evidence = std::format("uses the message \"{}\"", shortened(message, 60));
    p.description = std::format("Reports its errors under its own name, {}.", name);
    return p;
}

std::optional<store::Proposal> function_forward(const FunctionEvidence& e, std::string_view target_name) {
    if (!e.forwards_to_function || target_name.empty() || !valid_name(target_name) || target_name.starts_with("sub_") || target_name.ends_with("_thunk"))
        return std::nullopt;
    store::Proposal p;
    p.name = std::string(target_name) + "_thunk";
    p.rule = "function-forward";
    p.confidence = 70;
    p.evidence = std::format("only passes its arguments on to {}", target_name);
    p.description = std::format("Forwards to {}.", target_name);
    return p;
}

std::optional<store::Proposal> by_rules(const FunctionEvidence& evidence, std::string_view target_name, bool basic) {
    std::optional<store::Proposal> best;
    const auto consider = [&](std::optional<store::Proposal> p) {
        if (p && (!best || p->confidence > best->confidence)) best = std::move(p);
    };
    consider(import_wrapper(evidence));
    consider(string_reference(evidence));
    if (!basic) {
        consider(function_forward(evidence, target_name));
    }
    return best;
}

std::string comparable(std::string_view name) {
    std::string n = text::lower(undecorated(std::string(name)));
    while (n.starts_with('_')) n.erase(0, 1);
    for (const std::string_view suffix : {"_thunk", "_wrapper"})
        if (n.size() > suffix.size() && n.ends_with(suffix)) n.resize(n.size() - suffix.size());
    return n;
}

// ---- the work over a module ---------------------------------------------------------------------------------------------------------------

struct Proposals::Impl {
    Services s;
    explicit Impl(Services services) : s(std::move(services)) {}

    // What the module's functions are called now: by address, and the names in use.
    struct Known {
        std::string sha;
        names::ModuleNames names;
        std::map<Address, const names::FunctionName*> by_address;
        std::set<std::string> used;
    };
    Known known(const std::filesystem::path& module) {
        Known k;
        k.sha = s.namer.module_sha(module);
        k.names = s.namer.of_module(module);
        for (const names::FunctionName& f : k.names.functions) {
            k.by_address[f.address] = &f;
            if (f.source != "(none)") k.used.insert(f.name);
        }
        return k;
    }
    static bool unnamed(const names::FunctionName& f) { return f.source == "(none)"; }

    static const names::FunctionName& function(const Known& k, Address address) {
        const auto at = k.by_address.find(address);
        if (at == k.by_address.end()) throw UsageError("there is no function at that address");
        return *at->second;
    }

    // What the function a function forwards to is called, when it has a name (a placeholder is not one); "" otherwise.
    static std::string target_name(const Known& k, const FunctionEvidence& e) {
        if (!e.forwards_to_function) return {};
        const auto at = k.by_address.find(*e.forwards_to_function);
        return at == k.by_address.end() || unnamed(*at->second) ? std::string() : at->second->name;
    }

    static void check_name(std::string_view name, int confidence) {
        if (!valid_name(name)) throw UsageError("a name is letters, digits, _ and ::");
        if (confidence < 0 || confidence > 100) throw UsageError("a confidence is a percent, 0 to 100");
    }
};

Proposals::Proposals(Services services) : impl_(std::make_shared<Impl>(std::move(services))) {}

std::vector<store::Proposal> Proposals::propose(const std::filesystem::path& module) {
    Impl& self = *impl_;
    Impl::Known k = self.known(module);
    std::map<std::string, store::Proposal> existing;   // by function key
    for (store::Proposal& p : self.s.store.of_module(k.sha)) {
        if (p.state != store::ProposalState::Rejected) k.used.insert(p.name);
        existing[p.key] = std::move(p);
    }
    const std::map<Address, std::string> digests = self.s.namer.bodies(module);
    std::vector<store::Proposal> made;
    for (const FunctionEvidence& e : self.s.evidence(module)) {
        const auto at = k.by_address.find(e.address);
        if (at == k.by_address.end() || !Impl::unnamed(*at->second)) continue;
        const std::string& key = at->second->key;
        const auto old = existing.find(key);
        if (old != existing.end() && (old->second.state != store::ProposalState::Proposed || old->second.rule == "assistant")) continue;
        std::optional<store::Proposal> p = by_rules(e, self.target_name(k, e));
        if (!p) continue;
        if (old != existing.end()) k.used.erase(old->second.name);
        const std::string base = p->name;
        for (int n = 2; k.used.contains(p->name); ++n) p->name = std::format("{}_{}", base, n);
        k.used.insert(p->name);
        p->module = k.sha;
        p->key = key;
        if (const auto d = digests.find(e.address); d != digests.end()) p->digest = d->second;
        self.s.store.put(*p);
        made.push_back(std::move(*p));
    }
    return made;
}

Measure Proposals::measure(const std::filesystem::path& module, bool basic) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    Measure m;
    for (const FunctionEvidence& e : self.s.evidence(module)) {
        const auto at = k.by_address.find(e.address);
        if (at == k.by_address.end()) continue;
        const names::FunctionName& truth = *at->second;
        // ground truth: what the module itself says (its exports, its ordinal table, its debug symbols), not what a person or a rule gave
        const bool known_by_module = truth.source == "export" || truth.source.starts_with("debug symbols") || truth.source.starts_with("ordinal table");
        if (!known_by_module) continue;
        ++m.truth;
        const std::optional<store::Proposal> p = by_rules(e, Impl::target_name(k, e), basic);
        if (!p) continue;
        ++m.proposed;
        const std::string want = comparable(truth.name), got = comparable(p->name);
        const bool exact = want == got;
        const bool related = exact || (!want.empty() && !got.empty() && (want.find(got) != std::string::npos || got.find(want) != std::string::npos));
        auto& row = m.by_rule[p->rule];
        ++row[0];
        if (exact) { ++m.exact; ++row[1]; }
        if (related) { ++m.related; ++row[2]; }
        else m.misses.push_back(std::format("{} <- {} ({})", truth.name, p->name, p->rule));
    }
    return m;
}

store::Proposal Proposals::suggest(const std::filesystem::path& module, Address address, std::string_view name, std::string_view description, int confidence) {
    Impl& self = *impl_;
    Impl::check_name(name, confidence);
    Impl::Known k = self.known(module);
    const names::FunctionName& f = Impl::function(k, address);
    if (!Impl::unnamed(f)) throw UsageError(std::format("{} is already named {} (from {})", f.key, f.name, f.source));
    for (const store::Proposal& p : self.s.store.of_module(k.sha))
        if (p.key != f.key && p.state != store::ProposalState::Rejected) k.used.insert(p.name);
    if (k.used.contains(std::string(name))) throw UsageError(std::format("another function of the module is called {}", name));
    store::Proposal p;
    p.module = k.sha;
    p.key = f.key;
    p.name = std::string(name);
    p.description = std::string(description);
    p.rule = "assistant";
    p.confidence = confidence;
    p.evidence = "an assistant's reading of the function's evidence packet";
    const std::map<Address, std::string> digests = self.s.namer.bodies(module);
    if (const auto d = digests.find(address); d != digests.end()) p.digest = d->second;
    self.s.store.put(p);
    return p;
}

std::vector<store::Proposal> Proposals::list(const std::filesystem::path& module) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    std::map<std::string, store::Proposal> by_key;
    for (store::Proposal& p : self.s.store.of_module(k.sha)) by_key[p.key] = std::move(p);
    std::vector<store::Proposal> out;
    for (const names::FunctionName& f : k.names.functions)
        if (const auto at = by_key.find(f.key); at != by_key.end()) out.push_back(std::move(at->second));
    return out;
}

bool Proposals::accept(const std::filesystem::path& module, Address address) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    const names::FunctionName& f = Impl::function(k, address);
    std::optional<store::Proposal> p = self.s.store.get(k.sha, f.key);
    if (!p || p->state != store::ProposalState::Proposed) return false;
    if (k.used.contains(p->name)) throw UsageError(std::format("another function of the module is already called {}", p->name));
    if (p->digest) {
        // the name goes on the body, and every module holding it has it; a body too small to tell apart keeps the name in this module's record
        const std::optional<std::string> body = self.s.namer.set_name(module, address, p->name, "accepted proposal (" + p->rule + ")");
        if (!body) {
            p->digest.reset();
            self.s.store.put(*p);
        }
    }
    return self.s.store.set_state(k.sha, f.key, store::ProposalState::Accepted);
}

std::size_t Proposals::accept_all(const std::filesystem::path& module, int least) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    std::size_t taken = 0;
    for (const names::FunctionName& f : k.names.functions) {
        const std::optional<store::Proposal> p = self.s.store.get(k.sha, f.key);
        if (!p || p->state != store::ProposalState::Proposed || p->confidence < least) continue;
        try {
            taken += accept(module, f.address) ? 1 : 0;
        } catch (const UsageError& e) {   // (a name another function took since: this one stays proposed)
            cb::out("not accepted: {} {}: {}\n", f.key, p->name, e.what());
        }
    }
    return taken;
}

bool Proposals::reject(const std::filesystem::path& module, Address address) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    const names::FunctionName& f = Impl::function(k, address);
    const std::optional<store::Proposal> p = self.s.store.get(k.sha, f.key);
    if (!p || p->state != store::ProposalState::Proposed) return false;
    return self.s.store.set_state(k.sha, f.key, store::ProposalState::Rejected);
}

std::string Proposals::packets(const std::filesystem::path& module, bool with_c, std::size_t limit) {
    Impl& self = *impl_;
    const Impl::Known k = self.known(module);
    std::map<Address, FunctionEvidence> evidence;
    for (FunctionEvidence& e : self.s.evidence(module)) evidence[e.address] = std::move(e);
    const std::map<Address, std::string> digests = self.s.namer.bodies(module);
    std::map<std::string, store::Proposal> proposals;
    for (store::Proposal& p : self.s.store.of_module(k.sha)) proposals[p.key] = std::move(p);

    std::vector<Address> wanted;
    for (const names::FunctionName& f : k.names.functions)
        if (Impl::unnamed(f) && evidence.contains(f.address) && (limit == 0 || wanted.size() < limit)) wanted.push_back(f.address);
    std::map<Address, std::string> c;
    if (with_c && self.s.decompile) c = self.s.decompile(module, wanted);

    const auto name_of = [&](Address a) {
        const auto at = k.by_address.find(a);
        return at == k.by_address.end() ? std::format("{:x}", a) : at->second->name;
    };
    std::string out;
    for (const Address address : wanted) {
        const FunctionEvidence& e = evidence.at(address);
        const names::FunctionName& f = *k.by_address.at(address);
        Json packet = Json::object();
        packet["module"] = fs::to_text(module.filename());
        packet["address"] = f.key;
        packet["name"] = f.name;
        const auto d = digests.find(address);
        packet["body"] = d == digests.end() ? Json(nullptr) : Json(d->second);
        packet["bytes"] = e.bytes;
        packet["instructions"] = e.instructions;
        Json calls = Json::array();
        for (const CallEvidence& call : e.calls) {
            Json one = Json::object();
            if (call.kind == CallEvidence::Kind::Import) {
                one["kind"] = "import";
                one["target"] = call.import_name;
            } else if (call.kind == CallEvidence::Kind::Function) {
                one["kind"] = "function";
                one["target"] = name_of(call.target);
            } else {
                one["kind"] = "indirect";
            }
            one["sites"] = call.sites;
            calls.push_back(std::move(one));
        }
        packet["calls"] = std::move(calls);
        Json callers = Json::array();
        for (const std::uint64_t caller : e.callers) callers.push_back(name_of(caller));
        packet["callers"] = std::move(callers);
        packet["strings"] = e.strings;
        packet["listing"] = e.listing;
        if (with_c) {
            const auto text = c.find(address);
            packet["c"] = text == c.end() ? Json(nullptr) : Json(text->second);
        }
        if (const auto p = proposals.find(f.key); p != proposals.end()) {
            Json one = Json::object();
            one["name"] = p->second.name;
            one["description"] = p->second.description;
            one["rule"] = p->second.rule;
            one["confidence"] = p->second.confidence;
            one["state"] = std::string(store::state_name(p->second.state));
            packet["proposal"] = std::move(one);
        }
        out += packet.dump();
        out += '\n';
    }
    return out;
}

namespace {

void show(const std::vector<store::Proposal>& list) {
    for (const store::Proposal& p : list)
        cb::out("{:<10} {:<9} {:>3}%  {:<34} {:<17} {}\n", p.key, store::state_name(p.state), p.confidence, p.name, p.rule, p.description);
}

}  // namespace

int Proposals::execute(const cli::Args& a) {
    Impl& self = *impl_;
    const std::vector<std::string> arguments = a.values("arguments");
    if (arguments.empty()) throw UsageError("this needs a module");
    const auto found = self.s.corpus.retail(arguments[0]);
    if (!found) throw NotFoundError(std::filesystem::path(arguments[0]), 0, "no such module in the corpus");
    const std::filesystem::path module = *found;
    const auto address_of = [](const std::string& text_in) {
        const auto address = names::parse_address(text_in);
        if (!address) throw UsageError("'" + text_in + "' is not an address (hex, or segment:offset)");
        return *address;
    };
    if (a.flag("--propose")) {
        const names::ModuleNames all = self.s.namer.of_module(module);
        const std::size_t unnamed = static_cast<std::size_t>(std::count_if(all.functions.begin(), all.functions.end(), Impl::unnamed));
        const std::vector<store::Proposal> fresh = propose(module);
        show(fresh);
        std::map<std::string, std::size_t> by_rule;
        for (const store::Proposal& p : fresh) ++by_rule[p.rule];
        std::string rules;
        for (const auto& [rule, n] : by_rule) rules += std::format("{}{} {}", rules.empty() ? "" : ", ", rule, n);
        cb::out("{}: {} functions, {} with no name, {} proposed{}{}\n", text::lower(fs::to_text(module.filename())), all.functions.size(), unnamed, fresh.size(),
                rules.empty() ? "" : " -- ", rules);
        return 0;
    }
    if (a.flag("--measure")) {
        const Measure m = measure(module, a.value_or("--rules", "") == "basic");
        for (const std::string& line : m.misses) cb::out("miss  {}\n", line);
        for (const auto& [rule, n] : m.by_rule) cb::out("{:<17} proposed {:>4}  exact {:>4}  related {:>4}\n", rule, n[0], n[1], n[2]);
        const auto percent = [](std::size_t a_, std::size_t b_) { return b_ == 0 ? 0.0 : 100.0 * static_cast<double>(a_) / static_cast<double>(b_); };
        cb::out("{}: {} functions the module names, {} proposed (recall {:.1f}%), {} exact, {} related (precision {:.1f}%, exact {:.1f}%)\n",
                text::lower(fs::to_text(module.filename())), m.truth, m.proposed, percent(m.proposed, m.truth), m.exact, m.related, percent(m.related, m.proposed),
                percent(m.exact, m.proposed));
        return 0;
    }
    if (a.flag("--packets")) {
        const std::size_t limit = static_cast<std::size_t>(std::max<std::int64_t>(0, a.integer<std::int64_t>("--limit").value_or(0)));
        cb::out("{}", packets(module, a.flag("--c"), limit));
        return 0;
    }
    if (a.flag("--proposals")) {
        const std::vector<store::Proposal> all = list(module);
        show(all);
        cb::out("{} proposals\n", all.size());
        return 0;
    }
    if (const auto at = a.value("--suggest")) {
        if (arguments.size() < 2) throw UsageError("--suggest needs the address and then the name");
        const store::Proposal p = suggest(module, address_of(*at), arguments[1], a.value_or("--describe", ""), static_cast<int>(a.integer<std::int64_t>("--confidence").value_or(50)));
        cb::out("{} proposed for {} ({}%)\n", p.name, p.key, p.confidence);
        return 0;
    }
    if (const auto at = a.value("--accept")) {
        if (*at == "all") {
            const int least = static_cast<int>(a.integer<std::int64_t>("--min-confidence").value_or(kReviewBelow));
            cb::out("{} proposals accepted (those of {}% and over)\n", accept_all(module, least), least);
            return 0;
        }
        const bool done = accept(module, address_of(*at));
        cb::out("{}\n", done ? "accepted" : "that function has no proposal waiting");
        return done ? 0 : 1;
    }
    if (const auto at = a.value("--reject")) {
        const bool done = reject(module, address_of(*at));
        cb::out("{}\n", done ? "rejected" : "that function has no proposal waiting");
        return done ? 0 : 1;
    }
    return 0;
}

}  // namespace cb::symbols::proposals
