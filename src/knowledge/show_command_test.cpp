// Tests of the `knowledge` command over the knowledge store (cb/knowledge/show.hpp's `command`, store.hpp's `environment`): the output
// against the recorded output of `codebreaker knowledge ...` over the same authored files (testdata/commands/*.txt, made in a fresh
// state folder; the database's path, which is the one line that cannot agree, and the time a piece was made are left out).
#include <string>
#include <vector>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "knowledge/store.hpp"

namespace {

namespace stdfs = std::filesystem;

struct KnowledgeCommandWorld {
    cb::fs::ScopedTempDir state{"cb-knowledge-command"};
    cb::fs::ScopedEnv no_override{"CB_KNOWLEDGE_DB", std::nullopt};
    cb::cli::Cli cli{"codebreaker",
                     {cb::knowledge::show::command(cb::knowledge::environment(
                         state.path(), cb::knowledge::DataFolders::under(stdfs::path(CB_SOURCE_DIR).parent_path())))}};

    // What the command prints and returns, as the golden files have it: `exit=N` last, '/' in paths, no database path, no `made:` time.
    std::string run(std::vector<std::string> args) {
        cb::testing::CapturedStd cap;
        args.insert(args.begin(), "knowledge");
        const int code = cli.run(args);
        std::string text = cap.out();
        std::string out;
        for (std::string_view line : cb::text::lines(text)) {
            std::string l(line);
            for (char& c : l)
                if (c == '\\') c = '/';
            if (l.ends_with("knowledge.v2.sqlite")) continue;
            if (l.starts_with("made:") || l.starts_with("hash:")) continue;   // (the hash is of the base's own encoding of the value)
            out += l + "\n";
        }
        return out + "exit=" + std::to_string(code) + "\n";
    }
};

std::string golden_of(const char* name) {
    std::string text = cb::fs::read_text(cb::testing::data_path(std::string("knowledge/testdata/commands/") + name + ".txt"));
    std::string out;
    for (std::string_view line : cb::text::lines(text))
        if (!line.starts_with("made:") && !line.starts_with("hash:")) out += std::string(line) + "\n";
    return out;
}

}  // namespace

CB_TEST("knowledge.show_command.loads_and_lists", [] {
    KnowledgeCommandWorld w;
    const std::string loaded = w.run({"--load", "vxd"});
    // (the kinds' sizes are those of the way each base keeps a value, so the listing is compared by what it says of each kind)
    const std::string expected = golden_of("load_vxd");
    const auto words = [](const std::string& listing) {
        std::vector<std::string> out;
        for (std::string_view line : cb::text::lines(listing)) {
            std::vector<std::string> parts;
            for (std::string_view piece : cb::text::split(line, ' '))
                if (!piece.empty()) parts.emplace_back(piece);
            // platform, kind, keys ... source: drop the size (two words: "0.6 KB") and the origin counts' spacing
            if (parts.size() > 6 && parts[1] != "(bodies)") out.push_back(parts[0] + " " + parts[1] + " " + parts[2] + " " + parts.back());
            else out.push_back(std::string(line));
        }
        return out;
    };
    CB_CHECK(words(loaded) == words(expected));
});

CB_TEST("knowledge.show_command.keys_of_a_kind", [] {
    KnowledgeCommandWorld w;
    CB_CHECK_EQ(w.run({"vxd", "device"}), golden_of("keys_vxd_device"));
});

CB_TEST("knowledge.show_command.one_piece_with_its_origin", [] {
    KnowledgeCommandWorld w;
    CB_CHECK_EQ(w.run({"vxd", "device", "0001"}), golden_of("piece_vxd_device_0001"));
    CB_CHECK_EQ(w.run({"vxd", "service_table", "0001"}), golden_of("piece_vxd_service_table_0001"));
});

CB_TEST("knowledge.show_command.a_piece_nothing_knows_exits_one", [] {
    KnowledgeCommandWorld w;
    CB_CHECK_EQ(w.run({"vxd", "device", "zzzz"}), golden_of("unknown_piece"));
});
