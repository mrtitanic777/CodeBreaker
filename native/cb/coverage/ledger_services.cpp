// cb/coverage/ledger_services.cpp -- the services the coverage ledger is built from, over the corpus and the function store of a state folder.
#include "cb/coverage/ledger.hpp"
#include "cb/store/functions.hpp"

namespace cb::coverage::ledger {

Services services_over(const base::Corpus& corpus, const std::filesystem::path& state_dir,
                       std::function<std::map<std::string, std::optional<std::string>>()> verifier_profiles) {
    Services s;
    s.corpus = &corpus;
    s.state_dir = state_dir;
    s.verifier_profiles = std::move(verifier_profiles);
    s.store_index = store::functions::default_path(state_dir);
    s.store_folder = store::functions::databases_folder(state_dir);
    return s;
}

}  // namespace cb::coverage::ledger
