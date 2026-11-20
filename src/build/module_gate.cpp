// build/module_gate.cpp -- the gate: each function of a 16-bit module rebuilt from C is run beside the original's, on its own, every call
// out answered alike on both sides; one that does not do what the original does is rebuilt from its own instructions instead. A function's
// verdict is kept in the build cache by everything that makes it, so a function the same as in an earlier build is not run again.

#include <algorithm>
#include <set>

#include "base/cache.hpp"
#include "build/module_internal.hpp"
#include "core/fs.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::build::module {

namespace stdfs = std::filesystem;

namespace internal {

std::map<std::string, std::string> gate_keys(const Services& services, const stdfs::path& module, const std::vector<std::string>& names,
                                             const decompile::c_text::Module16& m) {
    static const Regex called_name(R"(\b([A-Za-z_]\w*)\s*\()");
    const std::string module_key = base::cache::key({fs::read_file(module)});
    // (everything else the verdicts depend on -- the compiler and its options, what each Windows call removes, the branch-guided inputs the checker
    // runs, the explorers, the emulators -- is the gate's stamp)
    const std::string tools = base::cache::key({services.gate_stamp ? services.gate_stamp() : std::string(), "build/module gate16 cb 1", fs::get_env("CB_FILL").has_value() ? "structures filled" : "",
                                                                                   fs::get_env("CB_SEQ").has_value() ? "message sequences " + *fs::get_env("CB_SEQ") : ""});
    const std::string header = base::cache::key({m.made.header()});
    // (and the description's lines for the function and for each it calls: the checker answers a call as the description says the callee does --
    // what it removes, what it returns -- so a callee described anew is a verdict to make again)
    std::map<std::string, std::string> name_line;
    if (m.description) {
        for (const std::string& line : m.description->lines) {
            const std::vector<std::string_view> p = text::split(line, '\t');
            if ((p[0] == "F" || p[0] == "X") && p.size() > 2) name_line[std::string(p[2])] = line;
        }
    }
    const OrderedMap<std::string, std::string> texts = m.made.texts();
    std::map<std::string, std::string> out;
    for (const std::string& n : names) {
        const std::string* text = texts.find(n);
        std::set<std::string> called = {n};
        if (text != nullptr) {
            called_name.for_each_match(*text, [&](const RegexMatch& match) {
                called.emplace(match.group(1));
                return true;
            });
        }
        std::vector<std::string> deps;
        for (const std::string& c : called) {
            if (const auto line = name_line.find(c); line != name_line.end()) deps.push_back(line->second);
        }
        std::sort(deps.begin(), deps.end());
        out[n] = base::cache::key({module_key, tools, header, n, text != nullptr ? *text : std::string(), deps});
    }
    return out;
}

}  // namespace internal

std::optional<OrderedMap<std::string, std::string>> ModuleBuilder::gate_known(const stdfs::path& module, const decompile::c_text::Module16& m) const {
    if (services_.cache == nullptr) return std::nullopt;
    const std::optional<Bytes> names = services_.cache->get("gate16_set", base::cache::key({fs::read_file(module), m.made.header()}));
    if (!names) return std::nullopt;
    std::vector<std::string> list;
    for (std::string_view n : text::split(as_text(*names), '\n')) {
        if (!n.empty()) list.emplace_back(n);
    }
    OrderedMap<std::string, std::string> differ;
    for (const auto& [name, key] : internal::gate_keys(services_, module, list, m)) {
        const std::optional<Bytes> had = services_.cache->get("gate16", key);
        if (!had) return std::nullopt;
        if (!had->empty() && (*had)[0] == 'D') differ.insert_or_assign(name, std::string(as_text(ByteView(*had).subspan(1))));
    }
    return differ;
}

OrderedMap<std::string, std::string> ModuleBuilder::gate(const stdfs::path& module, const stdfs::path& folder, const std::vector<std::string>& names,
                                                         const decompile::c_text::Module16& m,
                                                         std::vector<std::pair<std::string, std::string>>* not_run) const {
    if (not_run != nullptr) not_run->clear();
    const std::map<std::string, std::string> keys = internal::gate_keys(services_, module, names, m);
    OrderedMap<std::string, std::string> differ;
    std::vector<std::string> run;
    for (const std::string& n : names) {
        const std::optional<Bytes> had = services_.cache != nullptr ? services_.cache->get("gate16", keys.at(n)) : std::nullopt;
        if (!had) {
            run.push_back(n);
        } else if (!had->empty() && (*had)[0] == 'D') {
            differ.insert_or_assign(n, std::string(as_text(ByteView(*had).subspan(1))));
        }
    }
    if (run.empty() || !services_.gate_check) return differ;
    for (const GateResult& r : services_.gate_check(module, folder, run)) {
        if (r.verdict == GateResult::Verdict::Inconclusive) {   // (no verdict: not kept, and said)
            if (not_run != nullptr) not_run->emplace_back(r.name, r.why);
            continue;
        }
        if (services_.cache != nullptr) {
            const std::string kept = r.verdict == GateResult::Verdict::Divergent ? "D" + r.why : "S";
            services_.cache->put("gate16", keys.at(r.name), ByteView(reinterpret_cast<const std::uint8_t*>(kept.data()), kept.size()));
        }
        if (r.verdict == GateResult::Verdict::Divergent) differ.insert_or_assign(r.name, r.why);
    }
    return differ;
}

}  // namespace cb::build::module
