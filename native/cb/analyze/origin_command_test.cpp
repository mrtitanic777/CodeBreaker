// Tests of the `origin` command (cb/analyze/origin.hpp's `command`) against the recorded output for the same files
// (testdata/commands/*.txt).
#include <filesystem>
#include <string>
#include <vector>

#include "cb/analyze/origin.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"

namespace {

struct OriginCommandWorld {
    cb::fs::ScopedTempDir scratch{"cb-origin-command"};

    // What the command prints, then `exit=N` as the golden files have it.
    std::string run(std::vector<std::string> args) const {
        cb::analyze::origin::Environment env;
        env.scratch = scratch.path();
        env.retail = [](std::string_view name) -> std::optional<std::filesystem::path> {
            if (name == "ti850.drv") return cb::testing::data_path("compare/testdata/versions/win95/ti850.drv");
            return std::nullopt;
        };
        cb::cli::Cli cli("codebreaker", {cb::analyze::origin::command(env)});
        cb::testing::CapturedStd cap;
        args.insert(args.begin(), "origin");
        const int code = cli.run(args);
        return cap.out() + "exit=" + std::to_string(code) + "\n";
    }

    static std::string golden(const char* name) {
        return cb::fs::read_text(cb::testing::data_path(std::string("analyze/testdata/commands/") + name + ".txt"));
    }
};

std::string origin_data(const char* relative) { return cb::fs::to_text(cb::testing::data_path(relative)); }

}  // namespace

CB_TEST("analyze.origin_command.a_module_counts_as_recorded", [] {
    OriginCommandWorld w;
    CB_CHECK_EQ(w.run({origin_data("formats/testdata/lzexpand.dll")}), OriginCommandWorld::golden("lzexpand"));
    CB_CHECK_EQ(w.run({origin_data("compare/testdata/versions/win95/thinkjet.drv")}), OriginCommandWorld::golden("thinkjet"));
});

CB_TEST("analyze.origin_command.the_list_of_functions_matches_recorded", [] {
    OriginCommandWorld w;
    CB_CHECK_EQ(w.run({origin_data("formats/testdata/lzexpand.dll"), "--list"}), OriginCommandWorld::golden("lzexpand_list"));
    CB_CHECK_EQ(w.run({origin_data("compare/testdata/versions/win95/ti850.drv"), "--list"}), OriginCommandWorld::golden("ti850_list"));
});

CB_TEST("analyze.origin_command.a_corpus_name_finds_its_file_and_an_unknown_one_exits_two", [] {
    OriginCommandWorld w;
    CB_CHECK_EQ(w.run({"ti850.drv", "--list"}), OriginCommandWorld::golden("ti850_list"));
    CB_CHECK_EQ(w.run({"nosuchmodule"}), OriginCommandWorld::golden("no_such_module"));
});
