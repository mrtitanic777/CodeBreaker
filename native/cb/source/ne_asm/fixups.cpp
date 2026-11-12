// cb-unity: alone (file-local helpers named for this module)
#include "cb/source/ne_asm/fixups.hpp"

#include <algorithm>
#include <format>

#include "cb/core/cli.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"

namespace cb::source::ne_asm::fixups {

namespace ne = formats::ne;

namespace {

constexpr std::uint16_t kChainEnd = 0xFFFF;
constexpr std::uint8_t kMoveableEntry = 0xFF;       // an internal reference whose "segment" says: entry-table ordinal
constexpr std::uint32_t kEntryListingBytes = 128;   // how much of an exported entry the entry listing shows
constexpr std::uint32_t kLastInstructionGuess = 8;  // the span given the last instruction, whose end is not known

// The name a module reference gets: the module's own name, "mod<N>" when the reference is outside the table.
std::string module_label(const std::vector<std::string>& modules, std::uint16_t module) {
    if (module >= 1 && module <= modules.size()) return modules[module - 1];
    return std::format("mod{}", module);
}

Target describe_target(const ne::Module& module, const std::vector<std::string>& modules, const ne::Relocation& rec) {
    struct Visitor {
        const ne::Module& module;
        const std::vector<std::string>& modules;
        Target operator()(const ne::ImportOrdinalRef& r) const { return {std::format("{}.{}", module_label(modules, r.module), r.ordinal), std::nullopt}; }
        Target operator()(const ne::ImportNameRef& r) const {
            return {std::format("{}.{}", module_label(modules, r.module), module.imported_name(r.name_offset)), std::nullopt};
        }
        Target operator()(const ne::InternalRef& r) const {
            // Segment 0xFF is not a segment: it says "this module's OWN entry-table ordinal N", the fix-up of a call
            // into a MOVEABLE segment, which has to go through the entry table so that the segment can be moved.
            // Naming it as a call of the module itself (GDI.273) reads better than "seg255:0x0111".
            if (r.segment == kMoveableEntry) return {std::format("{}.{}", module.module_name(), r.entry), std::nullopt};
            return {std::format("seg{}:0x{:04x}", r.segment, r.entry), SegmentRef{r.segment, r.entry}};
        }
        Target operator()(const ne::OsFixupRef& r) const { return {std::format("osfixup#{}", r.fixup_type), std::nullopt}; }
    };
    return std::visit(Visitor{module, modules}, rec.target);
}

// The bytes of a segment as the file holds them; none for a segment without data.
ByteView segment_bytes(const ne::Module& module, const ne::Segment& segment) {
    if (segment.file_offset == 0) return {};
    return io::checked_range(module.data(), segment.file_offset, segment.length);
}

const ne::Segment& segment_numbered(const std::vector<ne::Segment>& segments, std::size_t number) {
    if (number < 1 || number > segments.size()) throw UsageError(std::format("segment {} does not exist (the module has {})", number, segments.size()));
    return segments[number - 1];
}

std::string_view base_name(std::string_view path) {
    const std::size_t slash = path.find_last_of("/\\");
    return slash == std::string_view::npos ? path : path.substr(slash + 1);
}

}  // namespace

ne::Module open_module(Bytes data) {
    if (data.size() >= 2 && data[0] == 'M' && data[1] == 'Z') return ne::Module::open(std::move(data));
    if (data.size() >= 2 && data[0] == 'N' && data[1] == 'E') return ne::Module::parse(std::move(data), 0);
    throw FormatError("ne", 0, "not MZ or NE");
}

ne::Module open_module(const std::filesystem::path& file) { return open_module(fs::read_file(file)); }

FixupMap build_fixup_map(const ne::Module& module, const ne::Segment& segment, const std::vector<std::string>& modules) {
    FixupMap map;
    const ByteView code = segment_bytes(module, segment);
    if (code.empty() && segment.length != 0) return map;   // no data in the file: there are no relocations either
    for (const ne::Relocation& rec : module.relocations(segment)) {
        const Target target = describe_target(module, modules, rec);
        std::uint32_t cur = rec.offset;
        std::size_t seen = 0;
        while (cur != kChainEnd && cur + 1 < segment.length && seen < kMaxChain) {
            map[cur] = target;
            const std::uint32_t next = static_cast<std::uint32_t>(code[cur]) | (static_cast<std::uint32_t>(code[cur + 1]) << 8);
            if (rec.additive()) break;   // the field holds an addend, not the link to the next site
            if (next == cur) break;
            cur = next;
            ++seen;
        }
    }
    return map;
}

std::string run_objdump(ByteView code, std::uint32_t base) {
    const fs::ScopedTempDir dir("ne-seg");
    const std::filesystem::path blob = dir.path() / "segment.bin";   // a folder of its own: parallel runs never collide
    fs::write_file(blob, code);
    const process::ProcessResult done = process::run({.argv = {"objdump", "-D", "-b", "binary", "-mi386", "-Maddr16,data16,intel",
                                                               std::format("--adjust-vma={}", base), blob.string()}});
    return std::string(as_text(done.out));
}

std::vector<ListingLine> parse_objdump(std::string_view output) {
    std::vector<ListingLine> rows;
    for (const std::string_view line : text::lines(output)) {
        ListingLine row{std::nullopt, std::string(line), {}};
        const std::size_t colon = line.find(":\t");
        if (colon != std::string_view::npos) {
            if (const auto offset = text::parse_int<std::uint32_t>(text::trim(line.substr(0, colon)), 16)) {
                row.offset = *offset;
                const std::string_view rest = line.substr(colon + 2);
                const std::size_t last_tab = rest.rfind('\t');
                row.instruction = std::string(text::trim(last_tab == std::string_view::npos ? rest : rest.substr(last_tab + 1)));
            }
        }
        rows.push_back(std::move(row));
    }
    return rows;
}

std::string annotated_listing(const ne::Module& module, std::string_view file_name, const ListingRange& range, const Objdump& objdump) {
    const std::vector<ne::Segment> segments = module.segments();
    const ne::Segment& segment = segment_numbered(segments, range.segment);
    const std::vector<std::string> modules = module.module_refs();
    const ByteView code = segment_bytes(module, segment);
    const FixupMap map = build_fixup_map(module, segment, modules);

    std::string out;
    const char* kind = segment.is_data() ? "DATA" : "CODE";
    if (segment.is_data()) out += std::format("(segment {} is {})\n", range.segment, kind);

    const std::size_t from = std::min<std::size_t>(range.start, code.size());
    const std::size_t to = range.count ? std::min<std::size_t>(code.size(), static_cast<std::size_t>(range.start) + *range.count) : code.size();
    const std::string printed = objdump(code.subspan(from, std::max(from, to) - from), range.start);

    out += std::format("# {}  segment #{} ({}, 0x{:x} bytes)  imports: {}\n", base_name(file_name), range.segment, kind, segment.length,
                       text::join(modules, ", "));
    out += std::format("# {} fixup sites in this segment\n\n", map.size());

    // Each instruction's span runs to the next instruction's address, so a fix-up is credited to the instruction whose
    // own bytes it lies in.
    const std::vector<ListingLine> lines = parse_objdump(printed);
    std::vector<std::uint32_t> starts;
    for (const ListingLine& line : lines) {
        if (line.offset) starts.push_back(*line.offset);
    }
    std::size_t index = 0;
    for (const ListingLine& line : lines) {
        out += line.text;
        if (line.offset) {
            const std::uint32_t end = index + 1 < starts.size() ? starts[index + 1] : *line.offset + kLastInstructionGuess;
            ++index;
            for (std::uint32_t k = *line.offset; k < end; ++k) {
                if (const auto found = map.find(k); found != map.end()) {
                    out += std::format("   ; -> {}", found->second.label);
                    break;
                }
            }
        }
        out += '\n';
    }
    return out;
}

std::string entry_listing(const ne::Module& module, std::string_view file_name, std::uint32_t ordinal, const Objdump& objdump) {
    const std::vector<ne::Entry> entries = module.entries();
    const auto found = std::find_if(entries.rbegin(), entries.rend(), [&](const ne::Entry& e) { return e.ordinal == ordinal; });
    if (found == entries.rend() || found->kind == ne::Entry::Kind::Unused) return std::format("ordinal {} not exported\n", ordinal);
    std::string out = std::format("# entry ordinal {} -> segment {} offset 0x{:x}\n", ordinal, found->segment, found->offset);
    out += annotated_listing(module, file_name, {found->segment, found->offset, kEntryListingBytes}, objdump);
    return out;
}

int main(std::span<const std::string> args, const Objdump& objdump) {
    const cli::Command command{
        "fixups",
        "an NE segment disassembled with every fix-up site named",
        {cli::Positional{"file", "an NE module (MZ/NE, or a raw module starting with NE)"},
         cli::Positional{"segment", "the segment to list (from 1)", false},
         cli::Option{"--start", "", "the first byte of the segment to list", cli::Arg::Value, "OFFSET"},
         cli::Option{"--count", "", "how many bytes to list (default: to the end)", cli::Arg::Value, "N"},
         cli::Option{"--entry", "", "list the entry with this ordinal instead", cli::Arg::Value, "ORDINAL"}},
        [&](const cli::Args& a) -> int {
            const std::string path = a.required("file");
            const ne::Module module = open_module(fs::from_text(path));
            if (const auto ordinal = a.integer<std::uint32_t>("--entry")) {
                cb::out_text(entry_listing(module, path, *ordinal, objdump));
                return kExitOk;
            }
            ListingRange range;
            range.segment = a.integer<std::size_t>("segment").value_or(1);
            range.start = a.integer<std::uint32_t>("--start").value_or(0);
            range.count = a.integer<std::uint32_t>("--count");
            cb::out_text(annotated_listing(module, path, range, objdump));
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("fixups", {command});
    std::vector<std::string> line = {"fixups"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::ne_asm::fixups
