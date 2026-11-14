// cb-unity: alone
// Tests of the `facts` command (cb/engine/show.hpp): the output over a binary's database against the recorded output
// over the same database file (testdata/commands/tracert.facts.sqlite: facts given by hand to tracert.exe's database). <STATE> and <BIN> stand for the folders. The one line that depends on where the binary is (the `binary` fact
// holds its path, so its size depends on the path's length) is left out of the comparisons.
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/engine/engine.hpp"
#include "cb/engine/show.hpp"

namespace {

namespace stdfs = std::filesystem;

constexpr const char* kFactsSha = "332f4933a1813622f345ab7d88cf02cac67f95f0d1f855047ce8ddfe4d50d3ca";

struct FactsCommandWorld {
    cb::fs::ScopedTempDir dir{"cb-facts-command"};
    stdfs::path state, binary;
    cb::engine::facts::Databases databases;
    cb::engine::Steps steps;
    cb::knowledge::ReadDigests digests{[](const cb::knowledge::Read&) { return std::string("absent"); }};
    cb::engine::Engine engine;
    cb::cli::Cli cli;

    FactsCommandWorld()
        : state(dir.path() / "state"),
          databases(cb::engine::facts::Config{.root = dir.path() / "state" / "db", .max_bytes = 4 * cb::engine::facts::kGiB}),
          engine(steps, databases, digests, 2),
          cli("codebreaker", {cb::engine::show::command(environment())}) {
        cb::fs::make_dirs(state / "db");
        cb::fs::make_dirs(dir.path() / "bin");
        binary = dir.path() / "bin" / "tracert.exe";
        cb::fs::write_file(binary, cb::fs::read_file(cb::testing::data_path("compare/testdata/versions/win95/tracert.exe")));
        cb::fs::write_file(state / "db" / (std::string(kFactsSha) + ".sqlite"),
                           cb::fs::read_file(cb::testing::data_path("engine/testdata/commands/tracert.facts.sqlite")));
    }

    cb::engine::show::Environment environment() {
        return {.engine = [this]() -> cb::engine::Engine& { return engine; },
                .retail = [this](std::string_view name) -> std::optional<stdfs::path> {
                    if (stdfs::exists(stdfs::path(std::string(name)))) return stdfs::path(std::string(name));
                    if (name == "tracert") return binary;
                    return std::nullopt;
                }};
    }

    // The output as the golden files have it: '/' in paths, `exit=N` last.
    std::string run(std::vector<std::string> args) {
        cb::testing::CapturedStd cap;
        args.insert(args.begin(), "facts");
        const int code = cli.run(args);
        std::string text = cap.out();
        for (char& c : text)
            if (c == '\\') c = '/';
        return text + "exit=" + std::to_string(code) + "\n";
    }

    std::string golden(const char* name) const {
        std::string text = cb::fs::read_text(cb::testing::data_path(std::string("engine/testdata/commands/") + name + ".txt"));
        text = cb::text::replace_all(text, "<STATE>", cb::fs::to_text(state));
        return cb::text::replace_all(text, "<BIN>", cb::fs::to_text(dir.path() / "bin"));
    }

    // Without the lines that depend on the binary's path.
    static std::string without_binary_row(const std::string& text) {
        std::string out;
        for (std::string_view line : cb::text::lines(text))
            if (!line.starts_with("  binary ")) out += std::string(line) + "\n";
        return out;
    }
};

}  // namespace

CB_TEST("engine.show.the_kinds_of_a_database", [] {
    FactsCommandWorld w;
    const std::string path = cb::fs::to_text(w.binary);
    CB_CHECK_EQ(FactsCommandWorld::without_binary_row(w.run({path})), FactsCommandWorld::without_binary_row(w.golden("kinds")));
});

CB_TEST("engine.show.the_facts_of_a_kind", [] {
    FactsCommandWorld w;
    const std::string path = cb::fs::to_text(w.binary);
    CB_CHECK_EQ(w.run({path, "note"}), w.golden("kind_note"));
    CB_CHECK_EQ(w.run({path, "table"}), w.golden("kind_table"));
    CB_CHECK_EQ(w.run({path, "nothing"}), w.golden("kind_empty"));
});

CB_TEST("engine.show.one_fact_and_its_value", [] {
    FactsCommandWorld w;
    const std::string path = cb::fs::to_text(w.binary);
    CB_CHECK_EQ(w.run({path, "note", "a"}), w.golden("fact_note_a"));
    CB_CHECK_EQ(w.run({path, "note", ""}), w.golden("fact_note_empty_key"));
    CB_CHECK_EQ(w.run({path, "table", "t1"}), w.golden("fact_table_t1"));   // (a document's entries in the order kept: sorted)
    CB_CHECK_EQ(w.run({path, "table", "t2"}), w.golden("fact_table_t2"));
    CB_CHECK_EQ(w.run({path, "blob", ""}), w.golden("fact_blob"));
    CB_CHECK_EQ(w.run({path, "num", "n"}), w.golden("fact_num"));
});

CB_TEST("engine.show.a_missing_fact_and_a_missing_binary", [] {
    FactsCommandWorld w;
    const std::string path = cb::fs::to_text(w.binary);
    CB_CHECK_EQ(w.run({path, "note", "zzz"}), w.golden("fact_missing"));
    CB_CHECK_EQ(w.run({"nosuchbinary"}), w.golden("no_binary"));
    CB_CHECK_EQ(w.run({"tracert", "num", "n"}), w.run({path, "num", "n"}));   // a corpus name finds the same binary
});

CB_TEST("engine.show.the_databases_and_the_cap", [] {
    FactsCommandWorld w;
    const std::string out = w.run({"--databases"});
    CB_CHECK(out.starts_with(std::string("  ") + kFactsSha + ".sqlite"));
    CB_CHECK(out.find(" KB  opened 20") != std::string::npos);
    CB_CHECK(out.find("  all ") != std::string::npos);
    CB_CHECK(out.find("(cap 4096.0 MB: setting db_max_gb)\nexit=0\n") != std::string::npos);
});

CB_TEST("engine.show.values_in_short", [] {
    using cb::engine::show::short_value;
    namespace k = cb::knowledge;
    CB_CHECK_EQ(short_value(cb::Bytes(5, 1)), "5 bytes");
    CB_CHECK_EQ(short_value(std::string("one")), "1 lines\n      one");
    CB_CHECK_EQ(short_value(std::string("")), "1 lines\n      ");
    std::string many;
    for (int i = 0; i < 14; ++i) many += "l" + std::to_string(i) + "\n";
    const std::string shown = short_value(many);
    CB_CHECK(shown.starts_with("14 lines\n      l0\n"));
    CB_CHECK(shown.ends_with("      l11\n      ..."));
    CB_CHECK_EQ(short_value(k::Value(7)), "7");
    CB_CHECK_EQ(short_value(k::Value(true)), "True");
    CB_CHECK_EQ(short_value(k::Value()), "None");
    CB_CHECK_EQ(short_value(k::Value(2.0)), "2.0");
    CB_CHECK_EQ(short_value(k::Value(std::string("it's"))), "1 lines\n      it's");   // (text in a document is shown as its lines)
    k::Value::List list;
    for (int i = 0; i < 8; ++i) list.emplace_back(i);
    CB_CHECK_EQ(short_value(k::Value(list)), "[8] 0, 1, 2, 3, 4, 5, ...");
    CB_CHECK_EQ(short_value(k::Value(1.5), 3), "1.5");
});

CB_TEST("engine.show.size_and_time_texts", [] {
    CB_CHECK_EQ(cb::engine::show::size_text(0), "0.0 KB");
    CB_CHECK_EQ(cb::engine::show::size_text(1536), "1.5 KB");
    CB_CHECK_EQ(cb::engine::show::size_text(3u << 20), "3.0 MB");
    CB_CHECK_EQ(cb::engine::show::when_text(0), "-");
    CB_CHECK_EQ(cb::engine::show::when_text(86400.0 * 400).size(), std::size_t{22});
});
