// store/versions_ledger.hpp -- what `codebreaker versions` asks of the rest of CodeBreaker, answered from the corpus, the function store and the
// body store.
//
//     store::VersionsLedger ledger({.corpus = corpus, .store = *store, .identity = identity_of, .bodies = &bodies});
//     compare::versions::run(args, corpus, ledger);
//
// compare/versions matches the functions of two builds of a module; to carry work across it asks (compare::versions::Ledger) which product a file is
// in and its content hash (the corpus), what is already reconstructed of the old build and where the new build's results are filed (the function
// store), and the names its bodies carry (the body store). This is the implementation over the pieces that exist in this tree:
//
//   * sha, owner:      base::Corpus.
//   * best, file_carried: engine::FunctionStore. A candidate is reported with the class its verdict makes it (verify/verdicts); a carried
//                      reconstruction is filed under the engine "carried" with the old build's hash as its revision, its signature the SHA-1 of its
//                      source, its verdict `inconclusive` and the reason it was carried: it is a candidate to judge in the new build, not a result.
//   * identity:        what results for a module are filed under -- the content hash and the digest of the verifier's settings for that kind of
//                      file. The settings belong to the verifier's lanes (verify/reconstruct, which is not in this tree yet), so the caller gives
//                      the function that knows them. Without one no module has an identity: nothing is reported as carried, and nothing filed.
//   * bodies, name_body: store::BodyStore. The knowledge base's body tables are not in this tree yet; without a body store `bodies` is empty (which
//                      `carry_names` reports as "the store has no bodies for the new build") and `name_body` does nothing.
#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <string>

#include "base/corpus.hpp"
#include "compare/versions.hpp"
#include "engine/function_store.hpp"
#include "store/bodies.hpp"

namespace cb::store {

class VersionsLedger final : public compare::versions::Ledger {
public:
    struct Services {
        const base::Corpus& corpus;
        engine::FunctionStore& store;
        // What results for a module are filed under: (content hash, verifier profile digest); nullopt when nothing reconstructs that kind of file.
        std::function<std::optional<Identity>(const std::filesystem::path&)> identity = {};
        BodyStore* bodies = nullptr;   // not owned; null: no bodies are known
    };
    explicit VersionsLedger(Services services) : services_(std::move(services)) {}

    std::optional<std::string> sha(const std::filesystem::path& module) override;
    std::optional<std::string> owner(const std::filesystem::path& module) override;
    std::optional<Identity> identity(const std::filesystem::path& module) override;
    std::map<std::string, Known> best(const Identity& identity) override;
    std::map<std::string, std::string> bodies(const std::string& sha) override;
    void name_body(const std::string& digest, const std::string& name, const std::string& source) override;
    void file_carried(const Identity& identity, const std::string& module_name, const std::filesystem::path& path, compare::versions::Kind kind,
                      std::span<const std::string> addresses, const std::string& rev, std::span<const Carried> rows) override;

    // The engine a carried candidate is filed under.
    static constexpr std::string_view kCarriedEngine = "carried";

private:
    Services services_;
};

}  // namespace cb::store
