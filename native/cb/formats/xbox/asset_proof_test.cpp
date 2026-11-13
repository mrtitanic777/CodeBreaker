// The proof against real files: with CB_XBOX_PROOF_DIR=<folder> this reads every .xdbf / .xur / .xbe / .xpr / .bik / .chd file of the folder, dumps it as asset_dump_kit.hpp does, and compares
// the lines with <folder>/python.txt -- what `python make_asset_goldens.py dump <folder>` prints for the same files (the Python readers, through formats.dll). Without the
// variable the test does nothing: the files are on the owner's machine (the Minecraft title's SPA and XUR scenes, cut out of default.xex and its XUI package).
#include <algorithm>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/testing.hpp"
#include "cb/formats/xbox/asset_dump_kit.hpp"

namespace asset_proof_test {

using namespace cb;
using namespace cb::formats::xbox;

CB_TEST("formats.xbox.assets.real_files_agree_with_the_python", [] {
    const std::optional<std::string> folder = fs::get_env("CB_XBOX_PROOF_DIR");
    if (!folder) return;
    const std::filesystem::path dir = fs::from_text(*folder);
    const std::string python = fs::read_text(dir / "python.txt");
    std::vector<std::filesystem::path> files;
    for (const auto& e : std::filesystem::directory_iterator(dir)) files.push_back(e.path());
    std::sort(files.begin(), files.end());
    std::size_t compared = 0, differing = 0;
    for (const std::filesystem::path& p : files) {
        const std::string ext = text::lower(fs::to_text(p.extension())), name = fs::to_text(p.filename());
        asset_dump::Lines got;
        try {
            const Bytes data = fs::read_file(p);
            if (ext == ".xdbf") got = asset_dump::dump_xdbf(name, data);
            else if (ext == ".xur") got = asset_dump::dump_xur(name, data);
            else if (ext == ".xbe") got = asset_dump::dump_xbe(name, data);
            else if (ext == ".xpr") got = asset_dump::dump_xpr(name, data);
            else if (ext == ".bik") got = asset_dump::dump_bink(name, data);
            else if (ext == ".chd") got = asset_dump::dump_chd(name, data);
            else continue;
        } catch (const Error& e) {
            cb::testing::record_failure(__FILE__, __LINE__, name + ": " + e.what());
            continue;
        }
        asset_dump::Lines want;
        for (const std::string_view line : text::lines(python)) {
            if (line.starts_with(name + ".")) want.emplace_back(line);
        }
        // The one divergence from the Python that real files show: an SPA's namespace-1 entries are not all achievements, and the Python decodes each as one, splitting its
        // strings at any two zero bytes where we split on 16-bit boundaries. A line of such a record may differ, only when its payload has two zero bytes at an odd offset.
        std::vector<const xdbf::Entry*> records;
        xdbf::Database db;
        if (ext == ".xdbf") {
            db = xdbf::Database::parse(fs::read_file(p));
            for (const xdbf::Entry& e : db.entries) {
                if (e.ns == 1 && e.payload.size() >= 0x1C) records.push_back(&e);
            }
        }
        std::size_t explained = 0;
        if (want.size() == got.size()) {
            for (std::size_t i = 0; i < want.size(); ++i) {
                const std::string prefix = name + ".ach";
                if (want[i] == got[i] || !want[i].starts_with(prefix)) continue;
                const std::size_t k = std::stoul(want[i].substr(prefix.size()));
                const ByteView rest = ByteView(records.at(k)->payload).subspan(0x1C);
                bool odd_pair = false;
                for (std::size_t j = 1; j + 1 < rest.size(); j += 2) odd_pair = odd_pair || (rest[j] == 0 && rest[j + 1] == 0);
                if (!odd_pair) continue;
                want[i] = got[i];   // explained
                ++explained;
            }
        }
        if (explained) log::info(std::format("proof: {}: {} achievement record(s) split differently, each with two zero bytes at an odd offset", name, explained));
        const std::string diff = asset_dump::difference(want, got);
        ++compared;
        CB_CHECK(diff.empty());
        if (!diff.empty()) {
            ++differing;
            cb::testing::record_failure(__FILE__, __LINE__, name + " differs from the Python:\n" + diff.substr(0, 2000));
        }
    }
    log::info(std::format("proof: {} files compared, {} differ", compared, differing));
    CB_CHECK(compared > 0);
});

}  // namespace asset_proof_test
