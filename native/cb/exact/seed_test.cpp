// Tests of seeding a program's functions from the sources of another version, on the real projects of the research (C 3.00's P1/P0/P2, LIB 2.40 after
// 2.20, LINK 2.44 after 2.40) against the recorded answers (testdata/seed.golden.json): the
// split of real C files into declarations and functions, the files each pair gives (count, address list and the SHA-1 of every file, two in full) and
// the command with --write on a copy of a project; plus the shapes of C the splitter reads (K&R, remarks, static, a function met twice).
//
// Behaviour pinned here: the "seeded from" comment names the source's path with '/'; a file written
// keeps the source's bytes; a pair whose function is in no source file is
// left out (it cannot happen: a name found in a file is a function of it).
#include "cb/exact/seed.hpp"

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/exact/real_projects.hpp"

using namespace cb;
using namespace cb::exact;

namespace {

const Json& seed_golden() {
    static const Json doc = json::load(testing::data_path("exact/testdata/seed.golden.json"));
    return doc;
}

std::string sha1_text(std::string_view s) {
    const Bytes b(s.begin(), s.end());
    return crypto::to_hex(crypto::sha1(b));
}

std::string sha1_bytes(const Bytes& b) { return crypto::to_hex(crypto::sha1(b)); }

}  // namespace

CB_TEST("exact.seed.split_cuts_declarations_and_functions", [] {
    const std::string text =
        "#include <stdio.h>\n"
        "int count;\n"
        "/* the first */\n"
        "static char *first(a, b)\n"
        "int a;\n"
        "char *b;\n"
        "{\n"
        "    if (a) { return b; }\n"
        "    return 0;\n"
        "}\n"
        "\n"
        "/* a remark that is\n"
        "   two lines */\n"
        "long second(void)\n"
        "{\n"
        "    return first(1, 0) != 0;\n"
        "}\n"
        "int third(int x)\n"
        "{\n"
        "}\n";
    const seed::Split s = seed::split(text);
    CB_CHECK_EQ(s.decls.size(), std::size_t(2));
    CB_CHECK_EQ(s.decls[0], "#include <stdio.h>");
    CB_CHECK_EQ(s.decls[1], "int count;");
    CB_CHECK_EQ(s.funcs.size(), std::size_t(3));
    CB_CHECK_EQ(s.funcs.at("first"), "/* the first */\nstatic char *first(a, b)\nint a;\nchar *b;\n{\n    if (a) { return b; }\n    return 0;\n}\n");
    CB_CHECK_EQ(s.funcs.at("second"), "/* a remark that is\n   two lines */\nlong second(void)\n{\n    return first(1, 0) != 0;\n}\n");
    CB_CHECK_EQ(s.funcs.at("third"), "int third(int x)\n{\n}\n");
    CB_CHECK_EQ(s.types.size(), std::size_t(2));   // an int is the default and is not declared
    CB_CHECK_EQ(s.types.at("first"), "char *");
    CB_CHECK_EQ(s.types.at("second"), "long");
    CB_CHECK(!s.types.contains("third"));
    CB_CHECK_EQ(s.funcs.begin()->first, "first");   // in the order of the file
});

CB_TEST("exact.seed.split_keeps_the_later_text_of_a_name_met_twice_and_reads_no_keyword_as_a_function", [] {
    const seed::Split s = seed::split("int f(void)\n{\n  return 1;\n}\nint g(void)\n{\n}\nint f(void)\n{\n  return 2;\n}\nwhile (x)\n{\n}\n");
    CB_CHECK_EQ(s.funcs.size(), std::size_t(2));
    CB_CHECK_EQ(s.funcs.begin()->first, "f");
    CB_CHECK_EQ(s.funcs.at("f"), "int f(void)\n{\n  return 2;\n}\n");
    CB_CHECK(!s.funcs.contains("while"));
    // the head of a function with no body is a declaration of the file
    const seed::Split decl = seed::split("extern int h(int);\nint k(int a)\n");
    CB_CHECK(decl.funcs.empty());
    CB_CHECK_EQ(decl.decls.size(), std::size_t(2));
    CB_CHECK(seed::split("").funcs.empty());
});

CB_TEST("exact.seed.split_of_real_sources_matches_the_recorded_output", [] {
    test::Real real;
    if (test::skipped(real, "seed split")) return;
    for (auto it = seed_golden().at("split").begin(); it != seed_golden().at("split").end(); ++it) {
        const std::string key = it.key();
        const std::size_t space = key.rfind(' ');
        const auto p = real.open_named(key.substr(0, space));
        std::filesystem::path found;
        for (const auto& part : p->sets())
            for (const project::Source& s : part->sources())
                if (fs::to_text(s.path.filename()) == key.substr(space + 1)) found = s.path;
        test::check(!found.empty(), key + ": the source is there");
        std::string raw = fs::read_text(found);
        std::string text;
        for (std::size_t i = 0; i < raw.size(); ++i) {
            if (raw[i] == '\r') {
                if (i + 1 < raw.size() && raw[i + 1] == '\n') ++i;
                text.push_back('\n');
            } else {
                text.push_back(raw[i]);
            }
        }
        const seed::Split s = seed::split(text);
        const Json& want = it.value();
        test::check(static_cast<std::int64_t>(s.decls.size()) == want.at("decls").at(0).get<std::int64_t>(), key + ": the number of declarations");
        test::check(sha1_text(text::join(s.decls, "\n")) == want.at("decls").at(1).get<std::string>(), key + ": the declarations");
        test::check(s.funcs.size() == want.at("funcs").size(), key + ": the number of functions");
        std::size_t i = 0;
        for (const auto& [name, body] : s.funcs) {
            const Json& w = want.at("funcs").at(i++);
            test::check(name == w.at(0).get<std::string>() && sha1_text(body) == w.at(1).get<std::string>(), key + ": function " + name);
        }
        test::check(s.types.size() == want.at("types").size(), key + ": the number of typed functions");
        i = 0;
        for (const auto& [name, type] : s.types) {
            const Json& w = want.at("types").at(i++);
            test::check(name == w.at(0).get<std::string>() && type == w.at(1).get<std::string>(), key + ": the type of " + name);
        }
    }
});

CB_TEST("exact.seed.the_files_each_pair_gives_match_the_recorded_output", [] {
    test::Real real;
    if (test::skipped(real, "seeds")) return;
    for (auto it = seed_golden().at("seeds").begin(); it != seed_golden().at("seeds").end(); ++it) {
        const std::string key = it.key();
        const std::size_t arrow = key.find(" <- ");
        const auto p = real.open_named(key.substr(0, arrow)), o = real.open_named(key.substr(arrow + 4));
        const auto got = seed::seeds(*p, *o);
        const Json& want = it.value();
        test::check(static_cast<std::int64_t>(got.size()) == want.at("count").get<std::int64_t>(), key + ": the number of files");
        std::size_t i = 0;
        for (const auto& [a, text] : got) {
            if (i >= want.at("addresses").size()) break;
            test::check(a == want.at("addresses").at(i).get<std::int64_t>(), key + ": the address of file " + std::to_string(i));
            test::check(sha1_text(text) == want.at("sha").at(i).get<std::string>(), std::format("{}: the file of {:05x}", key, a));
            ++i;
        }
        for (const Json& full : want.at("full")) {
            const auto at = got.find(full.at(0).get<std::int64_t>());
            test::check(at != got.end() && at->second == full.at(1).get<std::string>(), key + ": a file in full");
        }
    }
});

CB_TEST("exact.seed.run_counts_and_writes_the_files_the_project_has_none_of", [] {
    test::Real real;
    if (test::skipped(real, "seed run")) return;
    const Json& want = seed_golden().at("main");
    const std::string other = fs::to_text(test::research_root() / "lib-2.20");
    fs::ScopedTempDir dir("w67-seed");
    const auto folder = test::copy_project("lib-2.40", dir.path() / "d");
    const auto p = real.open(folder);
    std::string quiet;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(seed::run(*p, {"--from", other}), 0);
        quiet = cap.out();
    }
    CB_CHECK_EQ(quiet, want.at("quiet").at(0).get<std::string>());
    const std::size_t before = std::distance(std::filesystem::directory_iterator(p->src()), std::filesystem::directory_iterator());
    std::string text;
    {
        testing::CapturedStd cap;
        CB_CHECK_EQ(seed::run(*p, {"--from", other, "--write"}), 0);
        text = cap.out();
    }
    const std::string first = want.at("text").get<std::string>();
    CB_CHECK(text.starts_with(first));
    CB_CHECK(text.find("written: " + want.at("written").get<std::string>() + " files in ") != std::string::npos);
    std::size_t written = 0;
    for (const Json& f : want.at("new")) {
        const std::filesystem::path path = p->path(f.at(0).get<std::string>());
        test::check(std::filesystem::exists(path), f.at(0).get<std::string>() + " is written");
        if (std::filesystem::exists(path)) test::check(sha1_bytes(fs::read_file(path)) == f.at(1).get<std::string>(), f.at(0).get<std::string>() + " is the recorded file");
        ++written;
    }
    const std::size_t after = std::distance(std::filesystem::directory_iterator(p->src()), std::filesystem::directory_iterator());
    CB_CHECK_EQ(after - before, written + want.at("headers").size());
    // a second run finds all it wrote: nothing new to write
    testing::CapturedStd cap;
    seed::run(*p, {"--from", other, "--write"});
    CB_CHECK(cap.out().find("written: 0 files") != std::string::npos);
});

CB_TEST("exact.seed.run_without_from_says_what_it_needs", [] {
    test::Real real;
    if (test::skipped(real, "seed usage")) return;
    const auto p = real.open_named("lib-2.40");
    testing::CapturedStd cap;
    CB_CHECK_EQ(seed::run(*p, {}), 1);
    CB_CHECK_EQ(cap.out(), seed_golden().at("main_no_from").at(0).get<std::string>());
    CB_CHECK_EQ(seed::run(*p, {"--from"}), 1);
});
