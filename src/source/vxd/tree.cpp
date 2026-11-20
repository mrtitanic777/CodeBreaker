// source/vxd/tree.cpp -- a VxD as a source tree (see tree.hpp).
// cb-unity: alone
#include "source/vxd/tree.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/cli.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "core/process.hpp"
#include "core/text.hpp"

namespace cb::source::vxd::tree {

namespace stdfs = std::filesystem;
namespace le = formats::le;

SegmentClass segment_class(std::uint32_t flags) {
    if ((flags & 0x04) == 0) return (flags & 0x10) != 0 ? SegmentClass{"IDATA", "_IDATA"} : SegmentClass{"LDATA", "_LDATA"};   // not executable: data
    if ((flags & 0x8000) != 0) return {"MCODE", "_LMSGTABLE"};   // IOPL: the message-hook table
    if ((flags & 0x10) != 0) return {"ICODE", "_ITEXT"};         // discardable: init code
    if ((flags & 0x40) != 0) return {"LCODE", "_LTEXT"};         // preload and locked
    return {"PCODE", "_PTEXT"};                                  // pageable
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> entry_table_ddb(const le::Module& driver) {
    const ByteView raw = driver.data();
    const std::uint64_t table = driver.lfa() + driver.header().entry_table;
    if (driver.header().entry_table == 0 || table + 9 > raw.size()) return std::nullopt;
    const std::uint8_t count = raw[table], type = raw[table + 1];
    if (count < 1 || type != 3) return std::nullopt;   // a 32-bit entry bundle
    io::ByteReader reader(raw);
    reader.seek(table + 2);
    const std::uint32_t object = reader.u16();
    reader.seek(table + 5);
    const std::uint32_t rva = reader.u32();   // the ordinal-1 entry: one flags byte, then the offset
    return std::pair{object, rva};
}

namespace {

// The numbers `\d+` of the message that follow `open` and precede `close`, with the text after them satisfying `after`.
template <class After>
std::vector<std::size_t> numbers_between(std::string_view message, char open, std::string_view close, After after) {
    std::vector<std::size_t> found;
    std::size_t at = 0;
    while ((at = message.find(open, at)) != std::string_view::npos) {
        std::size_t i = at + 1;
        const std::size_t digits = i;
        while (i < message.size() && message[i] >= '0' && message[i] <= '9') ++i;
        if (i > digits && message.substr(i).starts_with(close)) {
            const std::string_view rest = text::ltrim(message.substr(i + close.size()));
            if (after(rest)) {
                found.push_back(*text::parse_int<std::size_t>(message.substr(digits, i - digits)));
                at = i;
                continue;
            }
        }
        ++at;
    }
    return found;
}

bool tree_starts_error(std::string_view rest) { return text::istarts_with(rest, "error") || text::istarts_with(rest, "fatal"); }
bool tree_starts_wasm_error(std::string_view rest) { return text::istarts_with(rest, "error"); }

struct ProbeResult {
    std::uint32_t code = 0;
    std::string message;
};

ProbeResult tree_probe(const Probe& probe, const stdfs::path& source, const stdfs::path& scratch) {
    process::ProcessSpec spec;
    if (probe.kind == Probe::Kind::Nasm) {
        spec.argv = {fs::to_text(probe.program), "--masm", "-f", "bin", fs::to_text(source), "-o", fs::to_text(scratch / "probe.out")};
        spec.err = process::Capture::Collect;   // the messages are on stderr
        const process::ProcessResult r = process::run(spec);
        return {r.exit_code, text::from_latin1(r.err)};
    }
    spec.argv = {fs::to_text(probe.program), "-zcm=masm", "-q", "-fo=" + fs::to_text(scratch / "probe.obj"), fs::to_text(source)};
    spec.err = process::Capture::Merge;
    const process::ProcessResult r = process::run(spec);
    return {r.exit_code, text::from_latin1(r.out)};
}

std::string tree_file_text(const std::vector<std::string>& head, const std::vector<std::string>& body, const std::vector<std::string>& tail) {
    std::vector<std::string> all = head;
    all.insert(all.end(), body.begin(), body.end());
    all.insert(all.end(), tail.begin(), tail.end());
    return text::join(all, "\r\n");
}

// One range of one object, written until the assembler takes it: every instruction it rejects is written as its bytes (db), and the
// file is assembled again. db is the exact bytes, so this trades an unassemblable reading (data walked as code, a mnemonic or operand
// the assembler will not spell) for the raw bytes -- byte-exact kept, and buildable. Returns the body lines.
std::vector<std::string> converge_buildable(const masm::Emission& code, std::uint32_t object, std::uint32_t lo, std::uint32_t hi,
                                            const std::map<std::uint32_t, std::string>& extra_labels, const std::vector<std::string>& head,
                                            const std::vector<std::string>& tail, const Probe& probe, const stdfs::path& scratch,
                                            std::size_t max_passes = 20) {
    const stdfs::path file = scratch / "probe.asm";
    std::set<std::uint32_t> force;
    masm::BodyOptions options;
    options.extra_labels = extra_labels;
    options.base_line = head.size();
    options.range = std::pair{lo, hi};
    masm::Body body;
    for (std::size_t pass = 0; pass < max_passes; ++pass) {
        options.force_raw = force;
        body = masm::object_body(code, object, options);
        fs::write_text(file, tree_file_text(head, body.lines, tail));
        const ProbeResult r = tree_probe(probe, file, scratch);
        if (r.code == 0) return body.lines;
        std::set<std::uint32_t> pinned;
        for (const std::size_t line : error_lines(r.message, probe.kind)) {
            const auto at = body.line_map.find(line);
            if (at != body.line_map.end() && !force.contains(at->second)) pinned.insert(at->second);
        }
        if (pinned.empty()) break;   // an error that cannot be pinned to one instruction: write the whole object raw
        force.insert(pinned.begin(), pinned.end());
    }
    // Last resort: an error the line map cannot pin (a re-encoded short jump, reported at its target label rather than at the jump).
    // Write the WHOLE object raw -- all db, still byte-exact -- so that it assembles. Reached only by the rare object that will not
    // converge instruction by instruction.
    std::set<std::uint32_t> everything = force;
    for (const auto& [place, instruction] : code.instructions())
        if (place.object == object && place.offset >= lo && place.offset < hi) everything.insert(static_cast<std::uint32_t>(place.offset));
    options.force_raw = everything;
    const masm::Body raw = masm::object_body(code, object, options);
    fs::write_text(file, tree_file_text(head, raw.lines, tail));
    return tree_probe(probe, file, scratch).code == 0 ? raw.lines : body.lines;
}

std::string hex_upper(std::uint32_t value, int digits) { return std::format("{:0{}X}", value, digits); }

}  // namespace

std::vector<std::size_t> error_lines(std::string_view message, Probe::Kind kind) {
    return kind == Probe::Kind::Nasm ? numbers_between(message, ':', ":", tree_starts_error) : numbers_between(message, '(', ")", [](std::string_view rest) {
        return rest.starts_with(':') && tree_starts_wasm_error(text::ltrim(rest.substr(1)));
    });
}

std::vector<std::string> emit_tree(const le::Module& driver, std::string_view name, const stdfs::path& out, const DdkNames& names,
                                   const Options& options) {
    const masm::Emission code(Disassembly::run(driver, names), names);
    const stdfs::path folder = out / text::lower(name);
    fs::make_dirs(folder);
    std::optional<fs::ScopedTempDir> scratch;
    if (options.probe && options.force_raw == nullptr) scratch.emplace();

    // The DDB's place for the exported VXD_DDB label: the scan of the code when it finds it, else the entry table's ordinal-1 export.
    std::optional<std::uint32_t> ddb_object;
    std::optional<std::int64_t> ddb_offset;
    if (code.ddb()) {
        ddb_object = code.ddb()->object;
        ddb_offset = code.ddb()->rva;
    } else if (const auto from_table = entry_table_ddb(driver)) {
        ddb_object = from_table->first;
        ddb_offset = from_table->second;
    }

    const std::string lower_name = text::lower(name);
    std::vector<std::string> written;
    std::vector<std::string> index = {std::format("; {} -- recovered VxD source tree (multi-file, by segment + function)", name), ";"};
    if (const auto& ddb = code.ddb()) {
        index.push_back(std::format("; Declare_Virtual_Device {}, {}, {}, {}_Control, 0{}h", name, ddb->major, ddb->minor, name, hex_upper(ddb->device_id, 4)));
        index.push_back(std::format("; DDK version 0{}h   init_order 0{}h", hex_upper(ddb->version, 3), hex_upper(ddb->init_order, 8)));
        index.push_back(";");
    }
    for (const char* line : {"; NOTE: a stripped LE binary carries no original file or (mostly) function names;",
                             "; files are named by segment class and role. Exported service names, control-dispatch",
                             "; handler names, and -- for the 12 core VxDs with a debug .SYM -- internal function",
                             "; names are the real DDK names. Other internals are sub_<offset>.", "", "; files:"})
        index.emplace_back(line);

    for (const le::Object& object : driver.objects()) {
        const std::uint32_t oi = object.index;
        const auto [class_name, segment] = segment_class(object.flags);
        std::size_t instruction_count = 0;
        for (const auto& [place, instruction] : code.instructions())
            if (place.object == oi) ++instruction_count;
        const auto size = static_cast<std::uint32_t>(code.code(oi).size());
        const std::vector<std::pair<std::uint32_t, std::uint32_t>> ranges = instruction_count <= options.max_instructions
                                                                                ? std::vector<std::pair<std::uint32_t, std::uint32_t>>{{0, size}}
                                                                                : masm::split_ranges(code, oi, options.max_instructions);
        for (std::size_t i = 0; i < ranges.size(); ++i) {
            const auto [lo, hi] = ranges[i];
            const std::string suffix = ranges.size() == 1 ? "" : std::format("_{:02}", i + 1);
            const std::string file = std::format("{}_{}_{:02}{}.asm", lower_name, text::lower(class_name), oi, suffix);
            std::map<std::uint32_t, std::string> extra;
            if (ddb_object == oi && ddb_offset && lo <= *ddb_offset && *ddb_offset < hi) extra[static_cast<std::uint32_t>(*ddb_offset)] = "VXD_DDB";
            const std::vector<std::string> head = {
                std::format("; {} -- {} object {} segment {}{}", file, name, oi, segment, ranges.size() > 1 ? std::format("  (part {}/{})", i + 1, ranges.size()) : ""),
                ".386p", std::format("{} segment dword public use32 '{}'", segment, class_name), ""};
            const std::vector<std::string> tail = {std::format("{} ends", segment), "        end", ""};
            std::vector<std::string> body;
            if (options.force_raw != nullptr) {   // a byte-exact build: the drift set is written raw
                masm::BodyOptions body_options;
                const auto forced = options.force_raw->find(oi);
                if (forced != options.force_raw->end())
                    for (const std::uint32_t at : forced->second)
                        if (lo <= at && at < hi) body_options.force_raw.insert(at);
                body_options.extra_labels = extra;
                body_options.range = std::pair{lo, hi};
                body = masm::object_body(code, oi, body_options).lines;
            } else if (options.probe) {
                body = converge_buildable(code, oi, lo, hi, extra, head, tail, *options.probe, scratch->path());
            } else {
                masm::BodyOptions body_options;
                body_options.extra_labels = extra;
                body_options.range = std::pair{lo, hi};
                body = masm::object_body(code, oi, body_options).lines;
            }
            fs::write_text(folder / file, tree_file_text(head, body, tail));
            written.push_back(file);
            index.push_back(std::format(";   {:<28} object {} {} [0x{:x}-0x{:x})", file, oi, segment, lo, hi));
        }
    }
    fs::write_text(folder / (lower_name + ".txt"), text::join(index, "\r\n") + "\r\n");
    return written;
}

int main(const Environment& env, std::span<const std::string> args) {
    const cli::Command command{
        "tree",
        "a driver as a source tree, one file per segment",
        {cli::Positional{"file", "a driver (.vxd, .386) or a container of them", true},
         cli::Positional{"out", "where the trees go", false}},
        [&](const cli::Args& a) -> int {
            const stdfs::path file = fs::from_text(a.required("file"));
            const std::string out_text = a.value("out").value_or(fs::to_text(env.output_folder));
            for (const auto& [member, module] : le::open_modules(file)) {
                const std::string stem = member.substr(0, member.find('.'));
                const std::vector<std::string> files = emit_tree(module, text::upper(stem), fs::from_text(out_text), env.names);
                out("{:<14} -> {} files in {}/{}\n", member, files.size(), out_text, text::lower(stem));
            }
            return kExitOk;
        },
        {}, {}, {}};
    const cli::Cli program("tree", {command});
    std::vector<std::string> line = {"tree"};
    line.insert(line.end(), args.begin(), args.end());
    return program.run(line);
}

}  // namespace cb::source::vxd::tree
