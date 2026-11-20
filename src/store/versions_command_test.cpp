// Tests of `codebreaker versions` as a complete command (store/versions_command): the comparison over the production ledger, made from the settings
// alone, on the real Windows 95 and 98 builds of compare/testdata/versions.
//
// The state folder is a temporary one (CB_STATE), so nothing of the checkout's state is read or written. The documents the command writes are the ones
// recorded for the same pairs (compare/testdata/versions/golden), compared as documents.
//
// CB_VERSIONS_PROOF="<old file>;<new file>[;<json out>]" runs one pair and prints what the command says and how long it took.
#include "store/versions_command.hpp"

#include <chrono>
#include <iostream>

#include <nlohmann/json.hpp>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace stdfs = std::filesystem;
using namespace cb;

namespace {

stdfs::path data(const std::string& name) { return testing::data_path("compare/testdata/versions/" + name); }

struct Surrounding {
    fs::ScopedTempDir state{"versions-command-state"};
    fs::ScopedEnv state_env{"CB_STATE", fs::to_text(state.path())};
    config::Config config = config::Config::load(config::current().main_checkout());
    cli::Cli cli{"codebreaker", {store::versions_command::command({.config = config, .corpus = nullptr})}};
};

}  // namespace

CB_TEST("store.versions_command.two_files_as_recorded", [] {
    Surrounding s;
    fs::ScopedTempDir out("versions-command-out");
    for (const char* module : {"thinkjet.drv", "tracert.exe"}) {
        const stdfs::path json = out.path() / (std::string(module) + ".json");
        testing::CapturedStd cap;
        CB_CHECK_EQ(s.cli.run({"versions", fs::to_text(data(std::string("win95/") + module)), fs::to_text(data(std::string("win98/") + module)), "--json", fs::to_text(json)}), 0);
        CB_CHECK(cap.out().find("identical") != std::string::npos);
        nlohmann::json got = nlohmann::json::parse(fs::read_text(json));
        nlohmann::json want = nlohmann::json::parse(fs::read_text(data(std::string("golden/") + module + ".compare.json")));
        for (nlohmann::json* d : {&got, &want}) {
            d->erase("seconds");
            d->erase("reconstructions");
            d->erase("listings");   // (the goldens are the document without them)
            for (const char* side : {"from", "to"})
                for (const char* k : {"path", "sha", "corpus"}) (*d)[side].erase(k);
        }
        // with the listings on, each changed pair also carries its instructions (the goldens are the document without them)
        CB_CHECK_EQ(got.at("pairs").size(), want.at("pairs").size());
        for (std::size_t i = 0; i < want.at("pairs").size(); ++i)
            for (auto it = got["pairs"][i].begin(); it != got["pairs"][i].end();)
                it = want["pairs"][i].contains(it.key()) ? std::next(it) : got["pairs"][i].erase(it);
        CB_CHECK(got == want);
    }
});

CB_TEST("store.versions_command.carry_needs_an_identity_and_a_store", [] {
    Surrounding s;
    testing::CapturedStd cap;
    // The files are not in the corpus, so nothing is known of the old one and nothing is filed: the command says so and succeeds.
    CB_CHECK_EQ(s.cli.run({"versions", fs::to_text(data("win95/thinkjet.drv")), fs::to_text(data("win98/thinkjet.drv")), "--carry"}), 0);
    CB_CHECK(cap.out().find("filed 0 reconstructions") != std::string::npos);   // (nothing is known of an old build outside the corpus)
});

CB_TEST("store.versions_command.a_usage_error_is_an_exit_code", [] {
    Surrounding s;
    testing::CapturedStd cap;
    CB_CHECK_EQ(s.cli.run({"versions"}), 2);
    CB_CHECK_EQ(s.cli.run({"versions", "--help"}), 0);
});

CB_TEST("store.versions_command.proof", [] {
    const std::optional<std::string> proof = fs::get_env("CB_VERSIONS_PROOF");
    if (!proof || proof->empty()) return;
    std::vector<std::string> parts;
    for (const std::string_view piece : text::split(*proof, ';')) parts.emplace_back(piece);
    if (parts.size() < 2) return;
    Surrounding s;
    std::vector<std::string> args{"versions", parts[0], parts[1]};
    if (parts.size() > 2) {
        args.push_back("--json");
        args.push_back(parts[2]);
    }
    const auto started = std::chrono::steady_clock::now();
    const int code = s.cli.run(args);
    std::cout << "exit " << code << ", " << std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count() << " s\n";
    CB_CHECK_EQ(code, 0);
});
