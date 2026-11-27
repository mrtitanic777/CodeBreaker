#include "store/versions_ledger.hpp"

#include "core/crypto.hpp"
#include "core/fs.hpp"

namespace cb::store {

namespace versions = compare::versions;

std::optional<std::string> VersionsLedger::sha(const std::filesystem::path& module) { return services_.corpus.sha(module); }

std::optional<std::string> VersionsLedger::owner(const std::filesystem::path& module) { return services_.corpus.owner(module); }

std::optional<versions::Ledger::Identity> VersionsLedger::identity(const std::filesystem::path& module) {
    return services_.identity ? services_.identity(module) : std::nullopt;
}

std::map<std::string, versions::Ledger::Known> VersionsLedger::best(const Identity& identity) {
    std::map<std::string, Known> out;
    for (const auto& [address, best] : services_.store.best(identity.sha, identity.profile)) {
        const store::Candidate& c = best.candidate;
        out[address] = {std::string(store::word(best.cls)), c.engine.empty() ? std::nullopt : std::optional<std::string>(c.engine), c.name,
                        c.source.value_or("")};
    }
    return out;
}

std::map<std::string, std::string> VersionsLedger::bodies(const std::string& sha) {
    std::map<std::string, std::string> out;
    if (!services_.bodies || sha.empty()) return out;
    for (const auto& [key, body] : services_.bodies->bodies(sha)) out[key] = body.digest;
    return out;
}

void VersionsLedger::name_body(const std::string& digest, const std::string& name, const std::string& source) {
    if (services_.bodies) services_.bodies->name_body(digest, name, source);
}

void VersionsLedger::file_carried(const Identity& identity, const std::string& module_name, const std::filesystem::path& path,
                                  versions::Kind kind, std::span<const std::string> addresses, const std::string& rev, std::span<const Carried> rows) {
    store::FunctionStore& store = services_.store;
    store.add_module(identity.sha, module_name, fs::to_text(path), versions::kind_name(kind));
    store.add_functions(identity.sha, addresses);
    std::vector<store::Record> records;
    records.reserve(rows.size());
    for (const Carried& row : rows) {
        store::Record r;
        r.addr = row.address;
        r.name = row.name;
        r.source = row.source;
        r.sig = crypto::to_hex(crypto::sha1(as_bytes(row.source)));   // the sig is what a judge's verdict is kept against
        r.verdict = std::string(store::word(store::Verdict::Inconclusive));
        r.reason = row.reason;
        records.push_back(std::move(r));
    }
    store.record_many(identity.sha, kCarriedEngine, rev, identity.profile, records);
}

}  // namespace cb::store
