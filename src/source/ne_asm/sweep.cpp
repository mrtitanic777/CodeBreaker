// source/ne_asm/sweep.cpp -- the idiomatic NE re-emitter across the corpus (see sweep.hpp).
// cb-unity: alone
#include "source/ne_asm/sweep.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <map>
#include <nlohmann/json.hpp>
#include <set>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "formats/detect.hpp"
#include "symbols/debug_build.hpp"

namespace cb::source::ne_asm::sweep {

namespace stdfs = std::filesystem;

std::vector<std::pair<std::string, stdfs::path>> ne_modules(const base::Corpus& corpus) {
    static constexpr std::array<std::string_view, 7> kExtensions = {".exe", ".dll", ".drv", ".386", ".cpl", ".scr", ".fon"};
    static constexpr std::array<std::string_view, 4> kFolders = {"SYSTEM", "SYSTEM32", "", "COMMAND"};
    const stdfs::path root = corpus.root();
    std::set<std::string> seen;
    std::vector<std::pair<std::string, stdfs::path>> found;
    for (const stdfs::path& base : {root, root / "WINDOWS"}) {
        for (const std::string_view folder : kFolders) {
            const stdfs::path where = folder.empty() ? base : base / folder;
            std::error_code ec;
            for (const std::string_view extension : kExtensions) {
                std::vector<stdfs::path> files;
                for (const auto& entry : stdfs::directory_iterator(where, ec))
                    if (entry.is_regular_file(ec) && text::iequals(fs::to_text(entry.path().extension()), extension)) files.push_back(entry.path());
                std::sort(files.begin(), files.end(), [](const stdfs::path& a, const stdfs::path& b) { return fs::to_text(a) < fs::to_text(b); });
                for (const stdfs::path& file : files) {
                    const std::string name = text::lower(fs::to_text(file.filename()));
                    if (!seen.insert(name).second) continue;
                    if (formats::detect::lane(file) == formats::detect::Lane::Ne) found.emplace_back(name, file);
                }
            }
        }
    }
    return found;
}

ModuleRecord sweep_module(const std::string& module, const stdfs::path& path, const idiom::Assemblers& assemblers, idiom::Backend backend,
                          const stdfs::path& state, const segment::SharedOrdinals& ordinals, const std::optional<stdfs::path>& emit,
                          const fixups::Objdump& objdump) {
    const formats::ne::Module ne = fixups::open_module(path);
    const std::string tag = text::lower(fs::to_text(fs::from_text(module).stem()));
    std::map<std::size_t, segment::NameOverrides> overrides;
    const stdfs::path names_json = state / "idiomatic" / tag / "names.json";
    std::error_code ec;
    if (stdfs::is_regular_file(names_json, ec)) overrides = symbols::debug_build::parse_overrides(fs::read_text(names_json));
    ModuleRecord record;
    record.module = module;
    for (const formats::ne::Segment& s : ne.segments()) {
        if (s.is_data()) continue;
        const auto named = overrides.find(s.index);
        const segment::Seg seg(ne, s.index, ordinals, named != overrides.end() ? named->second : segment::NameOverrides{}, objdump);
        const std::size_t n = seg.code().size();
        if (n == 0) continue;
        const idiom::Verified r = idiom::verify_range(seg, 0, static_cast<std::uint32_t>(n), assemblers, backend);
        record.segments.push_back({s.index, n, r.ok, r.code_instructions, r.pinned, r.live});
        if (emit && r.ok) {
            const stdfs::path folder = *emit / tag;
            fs::make_dirs(folder);
            fs::write_text(folder / std::format("{}_seg{:02}.asm", tag, s.index), text::replace_all(r.source, "\n", "\r\n"));
        }
    }
    record.ok = !record.segments.empty() && std::all_of(record.segments.begin(), record.segments.end(), [](const SegmentRecord& s) { return s.ok; });
    for (const SegmentRecord& s : record.segments) {
        record.exact_segments += s.ok;
        record.code_instructions += s.code_instructions;
        record.live += s.live;
    }
    return record;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "sweep",
        "every code segment of the 16-bit corpus through idiom",
        {cli::Option{"--limit", "", "only this many modules", cli::Arg::Value, "N"},
         cli::Option{"--start", "", "skip this many modules first", cli::Arg::Value, "K", "0"},
         cli::Option{"--emit", "", "write each exact segment's source under this folder", cli::Arg::Value, "DIR"},
         cli::Option{"--backend", "", "the assembler: nasm (no DOS) or ml (the period ML under DOS)", cli::Arg::Value, "nasm|ml", "nasm"},
         cli::Option{"--list", "", "list the modules and stop", cli::Arg::Flag},
         cli::Option{"--ords", "", "the ORDINALS.md to name ordinals from", cli::Arg::Value, "PATH", "ORDINALS.md"}},
        [&](const cli::Args& a) -> int {
            const std::string backend_text = a.value_or("backend", "nasm");
            if (backend_text != "ml" && backend_text != "nasm") throw UsageError("--backend is ml or nasm");
            const idiom::Backend backend = backend_text == "ml" ? idiom::Backend::Ml : idiom::Backend::Nasm;
            std::vector<std::pair<std::string, stdfs::path>> modules = ne_modules(env.corpus);
            if (a.flag("--list")) {
                for (const auto& [name, path] : modules) out("{}\n", name);
                return kExitOk;
            }
            const std::size_t start = a.integer<std::size_t>("--start").value_or(0);
            modules.erase(modules.begin(), modules.begin() + static_cast<std::ptrdiff_t>(std::min(start, modules.size())));
            if (const auto limit = a.integer<std::size_t>("--limit"); limit && *limit != 0 && *limit < modules.size()) modules.resize(*limit);
            const std::optional<stdfs::path> emit = a.value("emit") ? std::optional<stdfs::path>(fs::from_text(*a.value("emit"))) : std::nullopt;
            if (emit) fs::make_dirs(*emit);
            const segment::SharedOrdinals ordinals = std::make_shared<const segment::Ordinals>(segment::load_ordinals(fs::from_text(a.required("--ords"))));
            out("NE idiomatic sweep: {} NE modules (backend={})\n\n", modules.size(), backend_text);
            std::size_t mods = 0, full = 0, segs = 0, segok = 0, code = 0, live = 0, errors = 0;
            nlohmann::ordered_json summary = nlohmann::ordered_json::array();
            for (const auto& [name, path] : modules) {
                try {
                    const ModuleRecord r = sweep_module(name, path, env.assemblers, backend, env.state, ordinals, emit, env.objdump);
                    ++mods;
                    full += r.ok;
                    segs += r.segments.size();
                    segok += r.exact_segments;
                    code += r.code_instructions;
                    live += r.live;
                    nlohmann::ordered_json segments = nlohmann::ordered_json::array();
                    for (const SegmentRecord& s : r.segments)
                        segments.push_back({{"seg", s.segment}, {"n", s.bytes}, {"ok", s.ok}, {"n_code", s.code_instructions}, {"db", s.pinned}, {"live", s.live}});
                    summary.push_back({{"module", name}, {"segs", std::move(segments)}, {"ok", r.ok}, {"nseg", r.segments.size()}, {"nok", r.exact_segments},
                                       {"code", r.code_instructions}, {"live", r.live}});
                    out("  {:<16} segs {}/{} exact  {} instrs  {:.0f}% live{}\n", name, r.exact_segments, r.segments.size(), r.code_instructions,
                        100.0 * static_cast<double>(r.live) / static_cast<double>(std::max<std::size_t>(1, r.code_instructions)), r.ok ? "" : "  (partial)");
                } catch (const std::exception& e) {   // one module of the sweep: its failure is its line, the sweep goes on
                    ++errors;
                    summary.push_back({{"module", name}, {"error", std::string(e.what()).substr(0, 200)}});
                    out("  {:<16} ERROR {}\n", name, std::string(e.what()).substr(0, 70));
                }
                flush_out();
            }
            out("\n{}\n", std::string(60, '='));
            out("NE IDIOMATIC SWEEP -- {} modules\n", mods);
            out("  fully byte-exact modules : {}/{}\n", full, mods);
            out("  CODE segments byte-exact : {}/{}\n", segok, segs);
            out("  instructions             : {}  ({:.1f}% live idiomatic, rest db-pinned)\n", code, 100.0 * static_cast<double>(live) / static_cast<double>(std::max<std::size_t>(1, code)));
            out("  module errors            : {}\n", errors);
            if (emit) {
                const nlohmann::ordered_json totals = {{"mods", mods}, {"full", full}, {"segs", segs}, {"segok", segok}, {"code", code}, {"live", live}, {"err", errors}};
                const nlohmann::ordered_json document = {{"totals", totals}, {"modules", summary}};
                fs::write_text(*emit / "_ne_idiom_summary.json", document.dump(1, ' ', true));
                out("  wrote _ne_idiom_summary.json\n");
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("sweep", {command});
    std::vector<std::string> line = {"sweep"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::sweep
