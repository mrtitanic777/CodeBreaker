// cb/source/vxd/from_source.cpp -- a loadable VxD from the recovered source (see from_source.hpp).
// cb-unity: alone
#include "cb/source/vxd/from_source.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/source/vxd/idiom.hpp"
#include "cb/source/vxd/le_writer.hpp"

namespace cb::source::vxd::from_source {

namespace stdfs = std::filesystem;
namespace le = formats::le;

namespace {

// MASM's reserved words a recovered tag can collide with.
const std::set<std::string, std::less<>>& masm_reserved() {
    static const std::set<std::string, std::less<>> words = {
        "LOCK", "REP", "REPE", "REPZ", "REPNE", "REPNZ", "REPT", "AND", "OR", "XOR", "NOT", "SHL", "SHR", "MOD", "EQ", "NE", "LT", "GT", "LE", "GE",
        "BYTE", "WORD", "DWORD", "NEAR", "FAR", "PTR", "OFFSET", "SEG", "PROC", "ENDP", "IF", "ELSE", "ENDIF", "END", "SIZE", "LENGTH", "TYPE",
        "THIS", "SHORT", "HIGH", "LOW", "GROUP", "ASSUME", "PUBLIC"};
    return words;
}

bool fs_is_alpha(char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
bool fs_is_alnum(char c) { return fs_is_alpha(c) || (c >= '0' && c <= '9'); }

std::uint32_t fs_u32(ByteView d, std::uint64_t at) {
    io::ByteReader reader(d);
    reader.seek(at);
    return reader.u32();
}

bool starts_with_segment(std::string_view line, std::string_view what, std::string& number) {
    // `VSEGnn <what>`: the two digits of the segment's number
    if (line.size() < 6 || !line.starts_with("VSEG")) return false;
    if (!(line[4] >= '0' && line[4] <= '9' && line[5] >= '0' && line[5] <= '9')) return false;
    if (!line.substr(6).starts_with(what)) return false;
    number = std::string(line.substr(4, 2));
    return true;
}

}  // namespace

std::vector<std::string> object_names(const le::Module& driver) {
    const ByteView d = driver.data();
    const std::uint64_t lfa = driver.lfa();
    const std::uint32_t table = fs_u32(d, lfa + 0x40), count = fs_u32(d, lfa + 0x44);
    std::vector<std::string> tags;
    for (std::uint32_t i = 0; i < count; ++i) {
        const std::uint64_t at = lfa + table + static_cast<std::uint64_t>(i) * 24 + 20;
        if (at + 4 > d.size()) throw TruncatedError("LE object table", at, 4, d.size() > at ? d.size() - at : 0);
        tags.push_back(text::from_latin1(d.subspan(at, 4)));
    }
    return tags;
}

std::string segment_name(std::string_view tag) {
    return masm_reserved().contains(text::upper(tag)) ? "_" + std::string(tag) : std::string(tag);
}

std::map<std::uint32_t, std::pair<std::string, std::string>> object_segments(std::span<const std::string> tags, std::uint32_t ddb_object) {
    std::map<std::uint32_t, std::pair<std::string, std::string>> segments;
    std::set<std::string> seen;
    std::uint32_t index = 0;
    for (const std::string& tag : tags) {
        ++index;
        if (index == ddb_object) continue;
        std::string_view t = tag;
        while (!t.empty() && (t.back() == '\0' || t.back() == ' ')) t.remove_suffix(1);
        t = text::trim(t);
        const bool valid = !t.empty() && (fs_is_alpha(t.front()) || t.front() == '_') &&
                           std::all_of(t.begin(), t.end(), [](char c) { return fs_is_alnum(c) || c == '_' || c == '$' || c == '?' || c == '@'; });
        const std::string base = valid ? segment_name(t) : std::format("SEG{}", index);
        std::string name = base;
        for (int k = 2; seen.contains(name); ++k) name = std::format("{}_{}", base, k);   // a repeated tag (two PCOD objects)
        seen.insert(name);
        segments[index] = {name, name + "CODE"};
    }
    return segments;
}

std::optional<std::uint32_t> reclassify_ddb_tail_as_data(masm::Emission& code, const le::Module& driver) {
    const auto& ddb = code.ddb();
    if (!ddb) return std::nullopt;
    const std::uint32_t object = ddb->object;
    const std::uint32_t vsize = driver.objects()[object - 1].vsize;
    const std::int64_t tail = ddb->rva + static_cast<std::int64_t>(idiom::ddb_size(code, *ddb));
    if (tail >= static_cast<std::int64_t>(vsize)) return std::nullopt;
    auto& instructions = code.instructions();
    for (auto at = instructions.lower_bound(Place{object, tail}); at != instructions.end() && at->first.object == object && at->first.offset < vsize;)
        at = instructions.erase(at);
    for (auto& [place, label] : code.labels())   // a proc label on data confuses the emitter into a BeginProc: a plain data label
        if (place.object == object && place.offset >= tail && place.offset < vsize && label.starts_with("sub_"))
            label = std::format("loc_{}_{:04X}", place.object, place.offset);
    return static_cast<std::uint32_t>(tail);
}

std::string realseg_source(masm::Emission& code, std::span<const std::string> tags) {
    const std::uint32_t ddb_object = code.ddb() ? code.ddb()->object : 1;
    const auto segments = object_segments(tags, ddb_object);
    // the open and close lines of an object's segment: the DDB object's is the locked-code macro (_LTEXT)
    const auto open_close = [&](std::uint32_t index) -> std::pair<std::string, std::string> {
        if (index == ddb_object) return {"VxD_LOCKED_CODE_SEG", "VxD_LOCKED_CODE_ENDS"};
        const std::string& name = segments.at(index).first;
        return {name + " SEGMENT", name + " ENDS"};
    };

    const std::string emitted = idiom::emit(code, true);
    std::vector<std::string> lines;
    for (std::size_t at = 0;;) {   // split on CRLF
        const std::size_t end = emitted.find("\r\n", at);
        if (end == std::string::npos) {
            lines.push_back(emitted.substr(at));
            break;
        }
        lines.push_back(emitted.substr(at, end - at));
        at = end + 2;
    }
    // 1) drop the synthetic VSEG predeclarations
    std::vector<std::string> kept;
    for (std::size_t i = 0; i < lines.size();) {
        std::string number;
        if (starts_with_segment(lines[i], " SEGMENT DWORD PUBLIC _FLAT", number) && i + 1 < lines.size() &&
            text::trim(lines[i + 1]) == "VSEG" + number + " ENDS") {
            i += 2;
            continue;
        }
        kept.push_back(lines[i]);
        ++i;
    }
    // 2) retarget each object walk's VSEGnn open and close to the real segment
    std::vector<std::string> result;
    for (const std::string& line : kept) {
        const std::string_view stripped = text::trim(line);
        std::string number;
        if (starts_with_segment(stripped, " SEGMENT", number) && stripped.size() == 6 + 8) {
            result.push_back(open_close(*text::parse_int<std::uint32_t>(number)).first);
            continue;
        }
        if (starts_with_segment(stripped, " ENDS", number) && stripped.size() == 6 + 5) {
            result.push_back(open_close(*text::parse_int<std::uint32_t>(number)).second);
            continue;
        }
        result.push_back(line);
    }
    // 3) The DDB object stays ONE contiguous `_LTEXT` segment: code, the DDB and the data after it. (LINK /VXD orders segments of one class
    // alphabetically, and `_LDATA` sorts before `_LTEXT`, so a separate data segment would land at object offset 0 and shift every label of the
    // tail.) The tail is data by db and dd whatever the segment's class, and the final LE object attributes come from the original.
    // 4) predeclare the custom segments in the flat model, right after the includes
    std::vector<std::string> declarations;
    for (const auto& [index, segment] : segments) {
        declarations.push_back(std::format("{} SEGMENT DWORD PUBLIC _FLAT '{}'", segment.first, segment.second));
        declarations.push_back(segment.first + " ENDS");
    }
    std::size_t j = 0;
    while (j < result.size() && !result[j].starts_with("include")) ++j;
    while (j < result.size() && (result[j].starts_with("include") || result[j].empty())) ++j;
    result.insert(result.begin() + static_cast<std::ptrdiff_t>(j), declarations.begin(), declarations.end());
    result.insert(result.begin() + static_cast<std::ptrdiff_t>(j + declarations.size()), "");
    return text::join(result, "\r\n") + "\r\n";
}

std::string realseg_def(std::string_view name, std::span<const std::string> tags, std::uint32_t ddb_object) {
    std::vector<std::string> segments = {"    _LTEXT   CLASS 'LCODE'  PRELOAD NONDISCARDABLE", "    _LDATA   CLASS 'LCODE'  PRELOAD NONDISCARDABLE"};
    for (const auto& [index, segment] : object_segments(tags, ddb_object))
        segments.push_back(std::format("    {:<8} CLASS '{}' PRELOAD NONDISCARDABLE", segment.first, segment.second));
    return std::format("VXD {0}\r\nDESCRIPTION '{0} reconstructed from source'\r\n\r\nSEGMENTS\r\n{1}\r\n\r\nEXPORTS\r\n        {0}_DDB @1\r\n", name,
                       text::join(segments, "\r\n"));
}

namespace {

// What the finished file says of itself: the checks that it is a container the loader takes.
Built describe_built(Bytes finished, const le::Module& original, const std::optional<le::Ddb>& original_ddb) {
    const le::Module made = le::Module::from_buffer(Bytes(finished));
    const ByteView d = made.data();
    const std::uint64_t lfa = made.lfa();
    Built built;
    const std::uint32_t page_map = fs_u32(d, lfa + 0x48), pages = fs_u32(d, lfa + 0x14);
    built.page_map_identity = true;
    for (std::uint32_t k = 0; k < pages; ++k) {
        const std::uint64_t at = lfa + page_map + static_cast<std::uint64_t>(k) * 4;
        if (at + 3 > d.size()) throw TruncatedError("LE page map", at, 3, d.size() > at ? d.size() - at : 0);
        const std::uint32_t number = (static_cast<std::uint32_t>(d[at]) << 16) | (static_cast<std::uint32_t>(d[at + 1]) << 8) | d[at + 2];
        if (number != k + 1) built.page_map_identity = false;
    }
    if (const auto ddb = made.ddb()) built.ddb_offset = ddb->rva;
    if (original_ddb) built.oracle_ddb_offset = original_ddb->rva;
    io::ByteReader reader(d);
    reader.seek(lfa + 0xC0);
    built.device_id = reader.u16();
    built.device_id_ok = original_ddb.has_value() && built.device_id == original_ddb->device_id;
    reader.seek(lfa + 0xBC);
    built.resource_length = reader.u32();
    const Bytes first_original = original.object_data(original.objects().front()), first_made = made.object_data(made.objects().front());
    const std::size_t common = std::min(first_original.size(), first_made.size());
    for (std::size_t k = 0; k < common; ++k) built.object1_diffs += first_original[k] != first_made[k];
    built.objects = made.objects().size();
    built.vxd_length = finished.size();
    built.oracle_length = original.data().size();
    built.vxd = std::move(finished);
    return built;
}

std::string tail_of(const std::string& s, std::size_t n) { return s.size() > n ? s.substr(s.size() - n) : s; }

}  // namespace

Built build(const Environment& env, const le::Module& driver, std::optional<std::string> name) {
    masm::Emission code(Disassembly::run(driver, env.names), env.names);
    const std::optional<le::Ddb> original_ddb = code.ddb();
    if (!name) name = original_ddb && !original_ddb->name.empty() ? original_ddb->name : "VXD";
    // A driver with pages other than 4096 bytes: our compile-and-link lane always writes 4096-byte pages, but the original linker paginated a few
    // drivers differently (BIOS.VXD has 512-byte pages, 33 of them), and VMM faults on BIOS when its pagination differs from retail's (its
    // early real-mode start-up is sensitive to page granularity). Such a driver is written by the LE writer at the original's page size, from the
    // same recovered code and fix-ups.
    if (driver.header().page_size != 0x1000) return describe_built(le_writer::build_le(code, driver, *name), driver, original_ddb);
    const std::vector<std::string> tags = object_names(driver);
    reclassify_ddb_tail_as_data(code, driver);
    const std::string source = realseg_source(code, tags);
    const std::string def = realseg_def(*name, tags, original_ddb ? original_ddb->object : 1);
    // Compile the readable source with the period toolchain (ML 6.11d, LINK 2.60 /VXD) to show it assembles and links. LINK's container is
    // not what ships: its page map, data-page layout and fix-up encoding differ from the original linker's, and VMM's initialisation faults
    // with them even when the code is byte-exact. The loadable file is the LE writer's, over the same recovered code.
    std::string log;
    bool link_ok = false;
    const std::vector<masm::Assembled> made = masm::assemble_macro_batch(env.assemblers, std::vector<std::string>{source});
    const bool compile_ok = !made.empty() && made.front().object.has_value();
    if (compile_ok) {
        fs::ScopedTempDir work("cbvxdfromsrc");
        const link::Linked linked = env.link(*made.front().object, def, work.path());
        link_ok = linked.vxd.has_value();
        log = linked.log;
    } else if (!made.empty()) {
        log = made.front().log;
    }
    Built built = describe_built(le_writer::build_le(code, driver, *name), driver, original_ddb);
    built.source = source;
    built.def = def;
    built.link_log = tail_of(log, 300);
    built.compile_ok = compile_ok;
    built.link_ok = link_ok;
    return built;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "from-source",
        "a loadable driver from its kit-macro source",
        {cli::Positional{"file", "a driver (.vxd, .386) or a container of them: its first driver is built", true},
         cli::Positional{"out", "where the built driver is written", false}},
        [&](const cli::Args& a) -> int {
            const auto modules = le::open_modules(fs::from_text(a.required("file")));
            if (modules.empty()) throw FormatError("LE", 0, "the file holds no driver");
            const Built built = build(env, modules.front().second);
            out("built {} bytes (oracle {}); objects={}; page map identity={}; DDB @0x{:x} (oracle 0x{:x}); obj1 diffs={}\n", built.vxd_length,
                built.oracle_length, built.objects, built.page_map_identity ? "True" : "False", built.ddb_offset.value_or(0),
                built.oracle_ddb_offset.value_or(0), built.object1_diffs);
            if (const auto target = a.value("out")) {
                fs::write_file(fs::from_text(*target), built.vxd);
                out("wrote {}\n", *target);
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("from-source", {command});
    std::vector<std::string> line = {"from-source"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::from_source
