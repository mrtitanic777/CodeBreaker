// Tests of the `corpus-executables` command (cb/base/corpus_executables.hpp): the listing of a small corpus of the real binaries of
// formats/testdata (their formats, the order by file name, a name's copies in path order), and the listing of the Windows 95 corpus the main
// checkout's settings name against the recorded listing (1342 executables, 224 LE, 91 MZ,
// 388 NE, 638 PE, 1 W3; testdata/corpus_executables_golden.txt holds the count, the SHA-1 of the whole listing and its first and last lines), skipped
// when that corpus is not on this machine.
//
// A file whose format the detector does not name is listed as "?".
#include "app/corpus_executables.hpp"

#include <string>
#include <vector>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"
#include "formats/command_test_kit.hpp"
#include "formats/detect.hpp"
#include "base/config.hpp"

namespace corpus_executables_test {

using namespace cb;
namespace stdfs = std::filesystem;
namespace ce = cb::app::corpus_executables;

base::FormatProbes detect_probes() {
    base::FormatProbes probes;
    probes.lane = [](const stdfs::path& p) -> std::optional<base::Lane> {
        const std::optional<formats::detect::Lane> l = formats::detect::lane(p);
        if (!l) return std::nullopt;
        switch (*l) {
            case formats::detect::Lane::Pe: return base::Lane::Pe;
            case formats::detect::Lane::Ne: return base::Lane::Ne;
            case formats::detect::Lane::Le: return base::Lane::Le;
            case formats::detect::Lane::Dos: return base::Lane::Dos;
        }
        return std::nullopt;
    };
    return probes;
}

CB_TEST("app.corpus_executables.lists_the_executables_of_a_corpus_by_file_name", [] {
    const fs::ScopedTempDir dir{"cb-corpus-exe"};
    const fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    const stdfs::path root = dir.path() / "corpus";
    for (const char* name : {"lz32.dll", "find.exe", "charmap.exe", "txtonly.obj", "setver.exe", "xcopy.exe"}) {
        fs::make_dirs(root / "WINDOWS");
        fs::write_file(root / "WINDOWS" / name, fs::read_file(testing::data_path(std::string("formats/testdata/") + name)));
    }
    // a second copy of a name in another folder, and a file that is no executable
    fs::make_dirs(root / "OTHER");
    fs::write_file(root / "OTHER" / "FIND.EXE", fs::read_file(testing::data_path("formats/testdata/find.exe")));
    fs::write_text(root / "WINDOWS" / "readme.txt", "just text");
    base::Products products;
    products.target = "test";
    products.target_folder = root;
    products.checkout_root = dir.path() / "CodeBreaker";
    products.state = dir.path() / "state";
    const fs::ScopedEnv root_env{"CB_CORPUS_ROOT", fs::to_text(root)};
    ce::Environment environment{std::make_shared<base::Corpus>(products, detect_probes())};
    const std::string out = testing::run_command(ce::command(environment), {});
    const std::string expected_names[] = {"charmap.exe", "FIND.EXE", "find.exe", "lz32.dll", "setver.exe", "xcopy.exe"};
    std::vector<std::string> names;
    for (const std::string_view line : text::lines(out)) {
        const std::vector<std::string_view> parts = text::split(line, '\t');
        if (parts.size() != 2) continue;
        names.push_back(fs::to_text(fs::from_text(parts[1]).filename()));
        CB_CHECK(!parts[0].empty() && parts[0] != "?");
    }
    CB_CHECK_EQ(names.size(), std::size_t{6});
    for (std::size_t i = 0; i < names.size(); ++i) CB_CHECK_EQ(names[i], expected_names[i]);
    CB_CHECK(out.ends_with("exit=0\n"));
    CB_CHECK(out.find("txtonly.obj") == std::string::npos && out.find("readme.txt") == std::string::npos);
    // the formats are the detector's
    CB_CHECK(out.find("PE\t") != std::string::npos);
});

CB_TEST("app.corpus_executables.lists_the_windows_95_corpus_as_recorded", [] {
    // A proof against the live corpus (CB_CORPUS_EXECUTABLES_PROOF=1): it reads the main checkout's products, which a test build's
    // surroundings do not always reach (it listed nothing there).
    if (!fs::get_env("CB_CORPUS_EXECUTABLES_PROOF")) return;
    const config::Config config = config::Config::load(config::Config::load(testing::data_path("..")).main_checkout());
    const base::Products products = config.products();
    std::error_code ec;
    if (!stdfs::is_directory(products.target_folder, ec)) return;
    const fs::ScopedEnv no_name{"CB_CORPUS_NAME", std::nullopt};
    ce::Environment environment{std::make_shared<base::Corpus>(products, detect_probes())};
    const std::string out = testing::run_command(ce::command(environment), {});
    std::string listing = out.substr(0, out.rfind("exit="));
    // the golden: count, sha1 of the listing (LF line ends), the first and the last line
    const std::string golden = fs::read_text(testing::data_path("base/testdata/corpus_executables_golden.txt"));
    std::vector<std::string> fields;
    for (const std::string_view line : text::lines(golden)) fields.emplace_back(line);
    CB_CHECK_EQ(fields.size(), std::size_t{4});
    const std::vector<std::string_view> lines = text::lines(listing);
    if (lines.empty()) testing::record_failure(__FILE__, __LINE__, "no listing; the command said: " + out.substr(0, 400));
    if (fields.size() != 4 || lines.empty()) return;   // (the checks above said what is wrong; indexing on would crash)
    CB_CHECK_EQ("count " + std::to_string(lines.size()), fields[0]);
    CB_CHECK_EQ("sha1 " + crypto::to_hex(crypto::sha1(as_bytes(listing))), fields[1]);
    CB_CHECK_EQ("first " + std::string(lines.front()), fields[2]);
    CB_CHECK_EQ("last " + std::string(lines.back()), fields[3]);
});

}  // namespace corpus_executables_test
