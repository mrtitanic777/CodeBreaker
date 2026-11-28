// format_check -- the format engine's proof program (formats/format_engine). For each file it reads the file through the
// descriptions of config/formats, prints the generic tree (with --tree) and diffs it, field by field, against the
// hand-written reader of the same format (mz: dos.cpp, pe: pe.cpp, ne: ne.cpp, ines: nes.cpp, the console headers: gb.cpp ...).
//
//   format_check [--formats DIR] [--tree] [--max N] [--quiet] [--readers] FILE-OR-FOLDER...
//
// --readers prints, per file, one digest of what the pe, ne and le readers answer (run it before and after a reader changes).
//
// A folder is searched recursively; a table of agreement per format ends the output. Exit code 0 when every file agrees.
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "formats/format_compare.hpp"
#include "formats/format_engine.hpp"

namespace fsys = std::filesystem;
using namespace cb;
namespace eng = cb::formats::engine;

namespace {

fsys::path find_formats(const fsys::path& start) {
    std::error_code ec;
    for (fsys::path dir = fsys::absolute(start, ec); !dir.empty(); dir = dir.parent_path()) {
        if (fsys::is_directory(dir / "config" / "formats", ec)) return dir / "config" / "formats";
        if (dir == dir.parent_path()) break;
    }
    return {};
}

struct Tally {
    std::size_t files = 0, reader = 0, engine = 0, agree = 0, disagree = 0, round_trip = 0, covered_round_trip = 0;
    std::size_t values = 0;
    std::uint64_t covered = 0, bytes = 0;
};

}  // namespace

int main(int argc, char** argv) {
    fsys::path formats_dir;
    bool tree = false, quiet = false, readers = false;
    std::size_t max_elements = 6;
    std::vector<fsys::path> inputs;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "--formats" && i + 1 < argc) formats_dir = argv[++i];
        else if (a == "--tree") tree = true;
        else if (a == "--quiet") quiet = true;
        else if (a == "--readers") readers = true;
        else if (a == "--max" && i + 1 < argc) max_elements = static_cast<std::size_t>(std::stoul(argv[++i]));
        else inputs.emplace_back(a);
    }
    if (inputs.empty()) {
        std::fprintf(stderr, "usage: format_check [--formats DIR] [--tree] [--max N] [--quiet] FILE-OR-FOLDER...\n");
        return 2;
    }
    try {
        if (formats_dir.empty()) formats_dir = find_formats(fsys::current_path());
        if (formats_dir.empty()) formats_dir = find_formats(fsys::path(argv[0]).parent_path());
        eng::Registry reg;
        const std::size_t loaded = formats_dir.empty() ? 0 : reg.add_directory(formats_dir);
        if (loaded == 0) {
            std::fprintf(stderr, "format_check: no descriptions found (config/formats/*.fmt; --formats DIR)\n");
            return 2;
        }

        std::vector<fsys::path> files;
        for (const fsys::path& in : inputs) {
            std::error_code ec;
            if (fsys::is_directory(in, ec)) {
                for (fsys::recursive_directory_iterator it(in, fsys::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec))
                    if (it->is_regular_file(ec)) files.push_back(it->path());
            } else {
                files.push_back(in);
            }
        }

        std::map<std::string, Tally> tally;
        std::size_t failures = 0;
        for (const fsys::path& file : files) {
            Bytes data;
            try {
                data = fs::read_file(file);
            } catch (const Error& e) {
                std::fprintf(stderr, "%s: %s\n", fs::to_text(file).c_str(), e.what());
                continue;
            }
            if (readers) {   // the digest of what the readers answer: the same before and after they moved onto the engine
                std::printf("%s %s\n", fs::to_text(file).c_str(), eng::reader_digest(data, file).c_str());
                continue;
            }
            if (!quiet || tree) std::printf("%s (%zu bytes): ", fs::to_text(file).c_str(), data.size());
            const std::vector<std::string> names = reg.identify(data);
            if (!quiet || tree) {
                if (names.empty()) std::printf("no description reads it");
                for (const std::string& n : names) std::printf("%s ", n.c_str());
                std::printf("\n");
            }
            for (const std::string& format : eng::compared_formats()) {
                if (!reg.has(format)) continue;
                const eng::Comparison c = eng::compare(reg, format, data);
                Tally& t = tally[format];
                ++t.files;
                t.reader += c.reader_accepts;
                t.engine += c.engine_accepts;
                (c.agree() ? t.agree : t.disagree) += 1;
                if (c.reader_accepts && c.engine_accepts) {
                    t.values += c.compared;
                    t.round_trip += c.round_trip;
                    t.covered_round_trip += c.covered_round_trip;
                    t.covered += c.covered;
                    t.bytes += c.file_size;
                    if (!c.round_trip || !c.covered_round_trip) ++failures;
                }
                if (!c.agree()) {
                    ++failures;
                    std::printf("  DISAGREE %s: reader %s, description %s%s%s\n", format.c_str(), c.reader_accepts ? "reads it" : "refuses",
                                c.engine_accepts ? "reads it" : "refuses", c.engine_error.empty() ? "" : " -- ", c.engine_error.c_str());
                    for (const eng::Mismatch& m : c.mismatches)
                        std::printf("    %-34s engine %-24s reader %s\n", m.field.c_str(), m.engine.c_str(), m.reader.c_str());
                } else if (c.reader_accepts && !quiet) {
                    std::printf("  agree %-5s %zu values, round trip %s, %llu of %zu bytes described%s%s\n", format.c_str(), c.compared,
                                c.round_trip ? "identical" : "DIFFERENT", static_cast<unsigned long long>(c.covered), data.size(),
                                c.note.empty() ? "" : "  [", c.note.empty() ? "" : (c.note + "]").c_str());
                }
                if (tree && c.engine_accepts) std::printf("%s", eng::dump(reg.read(format, data), max_elements).c_str());
            }
        }

        std::printf("\n%-6s %7s %7s %7s %7s %9s %10s %10s %14s\n", "format", "files", "reader", "engine", "agree", "disagree", "round-trip", "values", "bytes described");
        for (const auto& [format, t] : tally)
            std::printf("%-6s %7zu %7zu %7zu %7zu %9zu %5zu/%-4zu %10zu %7llu/%llu\n", format.c_str(), t.files, t.reader, t.engine, t.agree, t.disagree,
                        t.round_trip, t.reader && t.engine ? t.engine : std::size_t{0}, t.values, static_cast<unsigned long long>(t.covered),
                        static_cast<unsigned long long>(t.bytes));
        return failures == 0 ? 0 : 1;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "format_check: %s\n", e.what());
        return 1;
    }
}
