// cb-unity: alone (helpers named for what they do here)
#include "cb/source/dos_writer.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/build/exepack.hpp"
#include "cb/build/mzfile.hpp"
#include "cb/build/pklite.hpp"
#include "cb/build/stubjoin.hpp"
#include "cb/build/vxdfile.hpp"
#include "cb/compare/image.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/dos.hpp"
#include "cb/platforms/decoder.hpp"
#include "cb/symbols/dos_interrupts.hpp"

namespace cb::source::dos_writer {
namespace {

namespace dos = formats::dos;
namespace epk = build::exepack;
namespace mzf = build::mzfile;
namespace pkl = build::pklite;
using Json = nlohmann::ordered_json;

constexpr std::string_view kReader = "dos_writer";
constexpr std::uint32_t kExepackSegment = 0x5000;   // where the EXEPACK unpacker's code is read (it runs behind SYSINIT)

// ---- small things ----------------------------------------------------------------------------------------------------------------

std::uint16_t word_at(ByteView b, std::size_t at) {
    io::ByteReader reader(b, kReader);
    reader.seek(at);
    return reader.u16();
}

void put_word(Bytes& b, std::size_t at, std::uint32_t v) {
    if (at + 2 > b.size()) throw TruncatedError(kReader, at, 2, b.size() > at ? b.size() - at : 0);
    b[at] = static_cast<std::uint8_t>(v & 0xFF);
    b[at + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
}

void put_dword(Bytes& b, std::size_t at, std::uint32_t v) {
    put_word(b, at, v & 0xFFFF);
    put_word(b, at + 2, v >> 16);
}

// The MZ header of `data`: FormatError when it has none (a file the caller took for a program).
dos::MzHeader need_header(ByteView data, bool stub = false) {
    const std::optional<dos::MzHeader> h = dos::mz_header(data, stub);
    if (!h) throw FormatError(kReader, 0, "no MZ header");
    return *h;
}

ByteView part_of(ByteView whole, std::uint64_t lo, std::uint64_t hi) {
    if (lo > hi || hi > whole.size()) throw FormatError(kReader, lo, "a stretch of the file lies outside it");
    return whole.subspan(static_cast<std::size_t>(lo), static_cast<std::size_t>(hi - lo));
}

Bytes joined(std::initializer_list<ByteView> pieces) {
    Bytes out;
    for (const ByteView p : pieces) out.insert(out.end(), p.begin(), p.end());
    return out;
}

std::uint64_t first_difference(ByteView a, ByteView b) { return compare::image::first_difference(a, b).value_or(std::min(a.size(), b.size())); }

// A file kept beside the source: written from `make()` the first time, read after that (so an edited one is what is built).
template <class Make>
Bytes beside(const std::filesystem::path& path, Make make) {
    if (!std::filesystem::is_regular_file(path)) fs::write_file(path, make());
    return fs::read_file(path);
}

std::string stem_of(std::string_view lower_name) { return text::sanitize_identifier(fs::to_text(fs::from_text(lower_name).stem())); }

// The stretches of SYSINIT that LINK /EXEPACK stored as fills: its iterated data (`dup`s), as offset length byte, in hex.
std::string fills_text(const epk::Layout& layout, std::string_view program) {
    std::string out = "; the stretches of " + std::string(program) + " that LINK /EXEPACK stored as fills: its iterated data\n; (`dup`s), as offset length byte, in hex\n";
    for (const epk::Fill& f : layout.fills) out += std::format("{:05X} {:04X} {:02X}\n", f.offset, f.length, f.byte);
    return out;
}

std::vector<epk::Fill> read_fills(const std::filesystem::path& path) {
    std::vector<epk::Fill> out;
    const std::string content = fs::read_text(path);
    for (const std::string_view line : text::lines(content)) {
        const std::string_view t = text::trim(line);
        if (t.empty() || t.starts_with(';')) continue;
        const std::vector<std::string_view> f = text::split_ws(t);
        const auto offset = f.size() == 3 ? text::parse_int<std::uint32_t>(f[0], 16) : std::nullopt;
        const auto length = f.size() == 3 ? text::parse_int<std::uint32_t>(f[1], 16) : std::nullopt;
        const auto byte = f.size() == 3 ? text::parse_int<std::uint32_t>(f[2], 16) : std::nullopt;
        if (!offset || !length || !byte || *byte > 0xFF) throw ParseError(fs::to_text(path), 0, 0, "a fill is `offset length byte`, in hex");
        out.push_back({*offset, *length, static_cast<std::uint8_t>(*byte)});
    }
    return out;
}

std::string upper_name(const std::filesystem::path& p) { return text::upper(fs::to_text(p.filename())); }

// ---- the run ---------------------------------------------------------------------------------------------------------------------

struct Context {
    explicit Context(const Options& o) : options(o), prober(o.tools, o.probes) {}
    const Options& options;
    assembly::Prober prober;
    platforms::DecoderSet decoders;
    void say(const std::string& line) const {
        if (options.log) options.log(line);
    }
};

// One real-mode program, with what the file around it takes from the original.
struct Subject {
    dos::Program program;
    std::string stem;          // names the files
    std::string name;          // names the program in its source's head
    Bytes original;            // the file it was read from
    std::string file_name;     // the file made: the original's name in upper case
    bool in_kernel = false;    // SYSINIT of IO.SYS: built into the kernel's file, not a file of its own
};

Result run_program(Context& cx, const Subject& s, const std::filesystem::path& outdir);

Json json_of(const Facts& f) {
    Json j;
    j["file"] = f.file;
    j["packer"] = f.packer ? Json(*f.packer) : Json(nullptr);
    j["overlay"] = f.overlay;
    const auto order = [&] {
        if (!f.offsets.empty() || (f.packer && *f.packer == "EXEPACK")) return Json(f.offsets);
        Json pairs = Json::array();
        for (const auto& [a, b] : f.pairs) pairs.push_back(Json::array({a, b}));
        return pairs;
    };
    if (f.packer && *f.packer == "PKLITE") {   // (PKLITE's facts name the order before the memory)
        j["order"] = order();
        j["min_alloc"] = f.min_alloc ? Json(*f.min_alloc) : Json(nullptr);
        return j;
    }
    j["min_alloc"] = f.min_alloc ? Json(*f.min_alloc) : Json(nullptr);
    j["full_last"] = f.full_last.value_or(false);
    if (f.words) j["words"] = *f.words;
    j["order"] = order();
    return j;
}

Facts facts_from(const Json& j) {
    Facts f;
    f.file = j.value("file", std::string());
    if (j.contains("packer") && j["packer"].is_string()) f.packer = j["packer"].get<std::string>();
    f.overlay = j.value("overlay", false);
    if (j.contains("min_alloc") && j["min_alloc"].is_number_integer()) f.min_alloc = j["min_alloc"].get<std::uint32_t>();
    if (j.contains("full_last") && j["full_last"].is_boolean()) f.full_last = j["full_last"].get<bool>();
    if (j.contains("words") && j["words"].is_number_integer()) f.words = j["words"].get<unsigned>();
    if (j.contains("order") && j["order"].is_array()) {
        for (const Json& item : j["order"]) {
            if (item.is_number_integer()) f.offsets.push_back(item.get<std::uint32_t>());
            else if (item.is_array() && item.size() == 2) f.pairs.emplace_back(item[0].get<std::uint32_t>(), item[1].get<std::uint32_t>());
        }
    }
    return f;
}

// <stem>.build.json: how to make the program again from the folder.
void write_description(const std::filesystem::path& outdir, const Subject& s, const Result& r, const std::filesystem::path& product, bool flat,
                       const std::optional<std::array<std::uint32_t, 4>>& header_words) {
    Json desc;
    desc["stem"] = s.stem;
    desc["product"] = fs::to_text(product.filename());
    desc["flat"] = flat;
    desc["regs"] = header_words ? Json(*header_words) : Json(nullptr);
    desc["facts"] = r.facts ? json_of(*r.facts) : Json(nullptr);
    fs::write_text(outdir / (s.stem + ".build.json"), desc.dump(1));
}

// ---- the file laid out as the original's -----------------------------------------------------------------------------------------

struct Linked {
    Bytes file;       // the program as linked
    dos::MzHeader h;  // its header
    ByteView module;  // its load module, as stored
    std::vector<mzf::Relocation> relocations;   // its table, as the linker wrote it
};

Linked read_linked(Bytes exe) {
    Linked l;
    l.file = std::move(exe);
    l.h = need_header(l.file);
    l.module = part_of(l.file, l.h.header_size, l.h.size);
    for (std::size_t i = 0; i < l.h.relocation_count; ++i) {
        const std::size_t at = l.h.relocation_offset + 4 * i;
        l.relocations.push_back({word_at(l.file, at), word_at(l.file, at + 2)});
    }
    return l;
}

std::uint32_t linear_of(const mzf::Relocation& r) { return static_cast<std::uint32_t>(r.segment) * 16 + r.offset; }

bool same_places(std::vector<std::uint32_t> a, std::vector<std::uint32_t> b) {
    std::ranges::sort(a);
    std::ranges::sort(b);
    return a == b;
}

// The file, from what was linked (`exe`) and the folder: header data, packing, overlay.
Bytes compose_file(const std::filesystem::path& outdir, const std::string& stem, const Bytes& exe, const Facts& facts, Result& result);

// A program the original stores packed by PKLITE: the file composed behind PKLITE's entry code and decompressor.
Bytes compose_pklite(const std::filesystem::path& outdir, const std::string& stem, const Bytes& exe, const Facts& facts, Result& result) {
    const Linked l = read_linked(exe);
    const Bytes tpl = fs::read_file(outdir / (stem + ".hdr"));
    const Bytes head = fs::read_file(outdir / "pklite" / "pklite.bin");
    const std::string_view marker = "Reserved";
    const auto at = std::ranges::search(tpl, as_bytes(marker));
    if (at.empty()) throw FormatError(kReader, 0, "no copy of the program's header behind PKLITE's notice in " + stem + ".hdr");
    std::uint32_t memory = word_at(tpl, static_cast<std::size_t>(at.begin() - tpl.begin()) + 8 + 4 + 8);
    // (the table's runs in the original's order and frames, when it names the same places as the build's)
    std::vector<pkl::Relocated> relocs;
    for (const mzf::Relocation& r : l.relocations) relocs.push_back({r.segment, r.offset});
    std::vector<pkl::Relocated> theirs;
    for (const auto& [seg, off] : facts.pairs) theirs.push_back({static_cast<std::uint16_t>(seg), static_cast<std::uint16_t>(off)});
    const auto places = [](const std::vector<pkl::Relocated>& v) {
        std::vector<std::uint32_t> out;
        for (const pkl::Relocated& r : v) out.push_back(static_cast<std::uint32_t>(r.segment) * 16 + r.offset);
        return out;
    };
    const auto same_run = [](const std::vector<pkl::Relocated>& a, const std::vector<pkl::Relocated>& b) {
        return std::ranges::equal(a, b, [](const pkl::Relocated& x, const pkl::Relocated& y) { return x.segment == y.segment && x.offset == y.offset; });
    };
    if (same_places(places(relocs), places(theirs)) && !same_run(theirs, relocs)) {
        relocs = theirs;
        result.remarks.emplace_back("the relocation table is written in the original's frames and order");
    }
    const pkl::Packed packed = pkl::pack(l.module, relocs, {l.h.cs, l.h.ip, l.h.ss, l.h.sp}, head, memory);
    memory = packed.header.min_alloc;
    if (facts.min_alloc && *facts.min_alloc != memory) {
        // (PKLITE's own figure for the memory it asks for, by a rule not yet found: the original's)
        result.remarks.push_back(std::format("the memory it asks for is the original's ({} paragraphs; the rule gives {})", *facts.min_alloc, memory));
        memory = *facts.min_alloc;
    }
    std::vector<mzf::Relocation> table;
    for (const pkl::Relocated& r : packed.header.relocs) table.push_back({r.offset, r.segment});
    const Bytes body = mzf::compose(tpl, packed.module, table, {packed.header.cs, packed.header.ip, packed.header.ss, packed.header.sp},
                                    static_cast<std::uint16_t>(memory));
    Bytes overlay;
    if (facts.overlay) overlay = fs::read_file(outdir / (stem + ".ovl"));
    return joined({body, overlay});
}

Bytes compose_file(const std::filesystem::path& outdir, const std::string& stem, const Bytes& exe, const Facts& facts, Result& result) {
    if (facts.packer && *facts.packer == "PKLITE") return compose_pklite(outdir, stem, exe, facts, result);
    const Linked l = read_linked(exe);
    // the memory the program needs beyond its load module: as far as its stack reaches (the linker's own figure goes wrong -- FFFF -- for a
    // stack that lies inside the file)
    const std::int64_t reach = static_cast<std::int64_t>(l.h.ss) * 16 + l.h.sp - static_cast<std::int64_t>(l.module.size());
    std::int64_t beyond = std::max<std::int64_t>(0, (reach + 15) / 16);
    const Bytes tpl = fs::read_file(outdir / (stem + ".hdr"));
    Bytes overlay;
    if (facts.overlay) overlay = fs::read_file(outdir / (stem + ".ovl"));
    const bool full_last = facts.full_last.value_or(false);
    Bytes body;
    try {
        if (facts.packer && *facts.packer == "EXEPACK") {
            const Bytes stub_code = fs::read_file(outdir / "exepack" / "exepack.bin");
            std::vector<std::uint32_t> sites;
            for (const mzf::Relocation& r : l.relocations) sites.push_back(linear_of(r));
            if (same_places(facts.offsets, sites) && facts.offsets != sites) {
                sites = facts.offsets;
                result.remarks.emplace_back("the relocation table is written in the original linker's order");
            }
            const epk::Packed packed = epk::pack(l.module, sites, {l.h.cs, l.h.ip}, {l.h.ss, l.h.sp}, read_fills(outdir / (stem + ".pack")), stub_code,
                                                 facts.words.value_or(8));
            const epk::StartRegisters& r = packed.registers;
            // the memory it asks for beyond the stored module: the unpacked program and its stack (EXEPACK counted both down to whole
            // paragraphs), or the unpacker's copy and stack behind it (counted from the stored module rounded up) -- whichever is more
            const std::int64_t module_size = static_cast<std::int64_t>(l.module.size()), stored = static_cast<std::int64_t>(packed.module.size());
            const std::int64_t program =
                (module_size + 15) / 16 + std::max<std::int64_t>(0, static_cast<std::int64_t>(l.h.ss) * 16 + l.h.sp - module_size) / 16 - stored / 16;
            const std::int64_t unpacker = static_cast<std::int64_t>(r.ss) + r.sp / 16 - (stored + 15) / 16;
            std::int64_t memory = std::max(program, unpacker);
            if (facts.min_alloc && std::abs(static_cast<std::int64_t>(*facts.min_alloc) - memory) == 1) {
                // (LINK rounded this count down for some programs and up for others -- CL.EXE of MSC 6.0a one way, C1.EXE the other -- by a
                // rule not yet found: the original's)
                result.remarks.push_back(std::format("the memory it asks for is the original's ({} paragraphs; the rule gives {})", *facts.min_alloc, memory));
                memory = *facts.min_alloc;
            }
            if (memory < 0 || memory > 0xFFFF) throw FormatError(kReader, 0, "the memory the program asks for does not fit its header");
            body = mzf::compose(tpl, packed.module, {}, {r.cs, r.ip, r.ss, r.sp}, static_cast<std::uint16_t>(memory), full_last);
        } else {
            // the relocation table as the original's linker wrote it -- each entry in the frame of the segment it was recorded in, in the
            // order of its records -- when it names the same places as the build's; and the memory the original asks for beyond its stack
            // (a segment of no bytes behind it) when that is more than the stack needs
            std::vector<mzf::Relocation> relocs = l.relocations;
            std::vector<mzf::Relocation> theirs;
            for (const auto& [off, seg] : facts.pairs) theirs.push_back({static_cast<std::uint16_t>(off), static_cast<std::uint16_t>(seg)});
            const auto linear = [](const std::vector<mzf::Relocation>& v) {
                std::vector<std::uint32_t> out;
                for (const mzf::Relocation& r : v) out.push_back(linear_of(r));
                return out;
            };
            const auto same_table = [](const std::vector<mzf::Relocation>& a, const std::vector<mzf::Relocation>& b) {
                return std::ranges::equal(a, b, [](const mzf::Relocation& x, const mzf::Relocation& y) { return x.offset == y.offset && x.segment == y.segment; });
            };
            if (same_places(linear(relocs), linear(theirs)) && !same_table(theirs, relocs)) {
                relocs = theirs;
                result.remarks.emplace_back("the relocation table is written in the original linker's frames and order");
            }
            if (facts.min_alloc && *facts.min_alloc > beyond) {
                result.remarks.push_back(std::format("the original asks for {} bytes more memory than its stack needs", 16 * (*facts.min_alloc - beyond)));
                beyond = *facts.min_alloc;
            }
            if (beyond > 0xFFFF) throw FormatError(kReader, 0, "the memory the program asks for does not fit its header");
            body = mzf::compose(tpl, l.module, relocs, {l.h.cs, l.h.ip, l.h.ss, l.h.sp}, static_cast<std::uint16_t>(beyond), full_last);
        }
    } catch (const FormatError& e) {   // (the original's layout is a courtesy: where it does not fit, the program as linked is the file)
        result.notes.push_back(std::string("not laid out as the original file: ") + e.what());
        body = exe;
    } catch (const UsageError& e) {
        result.notes.push_back(std::string("not laid out as the original file: ") + e.what());
        body = exe;
    }
    return joined({body, overlay});
}

// What laying the file out takes from the original: written to the side files (<stem>.hdr, .ovl, .pack, the unpacker's own folder) and
// returned for <stem>.build.json.
Facts make_facts(Context& cx, const Subject& s, const std::filesystem::path& outdir) {
    const dos::Program& p = s.program;
    const dos::MzHeader oh = need_header(s.original, true);   // (a Windows module's program stub too)
    beside(outdir / (s.stem + ".hdr"), [&] { return mzf::make_template(part_of(s.original, 0, oh.header_size)); });
    if (!p.overlay.empty()) beside(outdir / (s.stem + ".ovl"), [&] { return p.overlay; });
    Facts facts;
    facts.file = s.file_name;
    if (p.packer) facts.packer = dos::packer_name(*p.packer);
    facts.overlay = !p.overlay.empty();
    facts.min_alloc = oh.min_alloc;
    facts.full_last = word_at(s.original, 2) == 512;
    if (p.packer == dos::Packer::ExePack) {
        const ByteView stored = part_of(s.original, oh.header_size, oh.size);
        const epk::Layout layout = epk::layout(stored, oh.cs, oh.ip);
        const std::string listed = fills_text(layout, s.name);
        beside(outdir / (s.stem + ".pack"), [&] { return Bytes(listed.begin(), listed.end()); });
        const std::filesystem::path stub_dir = outdir / "exepack";
        if (!std::filesystem::is_regular_file(stub_dir / "exepack.bin")) {
            Subject stub;
            stub.program.kind = dos::Kind::KernelPart;
            stub.name = "exepack (the unpacker LINK /EXEPACK put behind " + text::lower(s.file_name) + ")";
            stub.stem = "exepack";
            stub.program.base = kExepackSegment * 16 + 2 * layout.words;   // its code follows the header words
            stub.program.image = layout.stub_code;
            stub.program.spans = {{stub.program.base, static_cast<std::uint32_t>(layout.stub_code.size()), "exepack"}};
            stub.program.cs = kExepackSegment;
            stub.program.ds = kExepackSegment;
            stub.program.entries = {{stub.program.base, "unpack", dos::Entry::Call::Far}};
            stub.program.segments.try_emplace(stub.program.base, kExepackSegment);
            const Result r = run_program(cx, stub, stub_dir);
            if (!r.same) throw UnsupportedError(kReader, 0, "the EXEPACK unpacker does not build back");
        }
        facts.words = layout.words;
        facts.offsets = dos::unpack_exepack(stored, oh.cs, oh.ip).relocations;
    } else {
        for (std::size_t i = 0; i < oh.relocation_count; ++i) {
            const std::size_t at = oh.relocation_offset + 4 * i;
            facts.pairs.emplace_back(word_at(s.original, at), word_at(s.original, at + 2));
        }
    }
    return facts;
}

// A program the original stores packed by PKLITE: packed the same way (build/pklite), behind PKLITE's entry code and decompressor (written
// as source, in pklite/), in a header laid out as the original's -- which keeps, after PKLITE's notice, a copy of the program's own header:
// its min-alloc is the memory the program asks for beyond itself.
Facts make_pklite_facts(Context& cx, const Subject& s, const std::filesystem::path& outdir) {
    const dos::Program& p = s.program;
    const dos::MzHeader oh = need_header(s.original, true);
    beside(outdir / (s.stem + ".hdr"), [&] { return mzf::make_template(part_of(s.original, 0, oh.header_size)); });
    if (!p.overlay.empty()) beside(outdir / (s.stem + ".ovl"), [&] { return p.overlay; });
    const std::filesystem::path head_dir = outdir / "pklite";
    const ByteView stored_module = part_of(s.original, oh.header_size, oh.size);
    if (!std::filesystem::is_regular_file(head_dir / "pklite.bin")) {
        const std::size_t start = pkl::stream_start(stored_module);
        Subject head;
        head.program = dos::read_pklite_head(s.original, start);
        head.name = "pklite (the entry code and decompressor PKLITE put in front of " + text::lower(s.file_name) + ")";
        head.stem = "pklite";
        const Result r = run_program(cx, head, head_dir);
        if (!r.same) throw UnsupportedError(kReader, 0, "PKLITE's decompressor does not build back");
    }
    const pkl::Stored stored = pkl::read(stored_module);
    Facts facts;
    facts.file = s.file_name;
    facts.packer = "PKLITE";
    facts.overlay = !p.overlay.empty();
    for (const pkl::Relocation& run : stored.relocs) {
        for (const std::uint16_t off : run.offsets) facts.pairs.emplace_back(run.segment, off);
    }
    facts.min_alloc = oh.min_alloc;
    return facts;
}

// The built program as a file laid out as the original is: result.file, result.file_same.
void as_file(Context& cx, const Subject& s, const std::filesystem::path& outdir, const std::filesystem::path& linked, Result& result) {
    const Bytes exe = fs::read_file(linked);
    fs::make_dirs(outdir / "file");
    const std::filesystem::path out = outdir / "file" / s.file_name;
    const dos::Program& p = s.program;
    if (p.packer && *p.packer != dos::Packer::ExePack && *p.packer != dos::Packer::MovesItself && *p.packer != dos::Packer::PkLite) {
        // stored by a packer this does not write (LZEXE, DIET, UPX): the program as linked
        fs::write_file(out, joined({exe, p.overlay}));
        result.file = out;
        result.file_same = false;
        result.remarks.push_back(std::format("the original is stored packed by {}; the file is the program unpacked", dos::packer_name(*p.packer)));
        return;
    }
    result.facts = p.packer == dos::Packer::PkLite ? make_pklite_facts(cx, s, outdir) : make_facts(cx, s, outdir);
    const Bytes body = compose_file(outdir, s.stem, exe, *result.facts, result);
    fs::write_file(out, body);
    result.file = out;
    result.file_same = body == s.original;
    if (!*result.file_same) {
        result.remarks.push_back(std::format("the file differs from the original's at {:#x} ({} bytes, original {})", first_difference(body, s.original),
                                             body.size(), s.original.size()));
    }
}

// ---- one program -----------------------------------------------------------------------------------------------------------------

Result run_program(Context& cx, const Subject& s, const std::filesystem::path& outdir) {
    dos_program::DosProgramOptions po;
    po.tools = cx.options.tools;
    po.prober = &cx.prober;
    po.log = cx.options.log;
    const dos_program::DosProgramResult r = dos_program::write_dos_program(s.program, s.stem, s.name, outdir, po, symbols::dos_interrupts::ServiceTable::standard(),
                                                                          cx.decoders.get(platforms::Arch::X86, 16, true));
    Result out;
    out.sources = {r.source};
    out.image = r.image;
    out.linked = r.linked;
    out.same = r.same;
    out.rounds = static_cast<std::size_t>(r.rounds);
    out.notes = r.notes;
    out.remarks = r.remarks;
    out.kind = r.kind;
    out.instructions = r.instructions;
    out.raw = r.raw;
    out.data = r.data;
    out.functions = r.functions;
    if (r.same && !r.flat && !s.in_kernel) {
        as_file(cx, s, outdir, *r.image, out);
        write_description(outdir, s, out, *r.image, r.flat, r.header_words);   // (now with what the layout took from the original)
    }
    return out;
}

Part part_from(const std::string& name, const Result& r) {
    Part part;
    part.name = name;
    part.source = r.sources.empty() ? std::filesystem::path() : r.sources.front();
    part.image = r.image;
    part.linked = r.linked;
    part.same = r.same;
    part.rounds = r.rounds;
    part.instructions = r.instructions;
    part.raw = r.raw;
    part.data = r.data;
    part.notes = r.notes;
    return part;
}

std::string notes_of(const std::vector<std::string>& notes, std::string_view none) { return notes.empty() ? std::string(none) : text::join(notes, "; "); }

// ---- IO.SYS ----------------------------------------------------------------------------------------------------------------------

// IO.SYS as the programs it is made of, each where it runs: the loader and the BIOS + DOS kernel, stored in the file as they run; SYSINIT as
// the MZ program it is (unpacked: the file stores it as LINK /EXEPACK did); and the EXEPACK unpacker's code -- and how SYSINIT was packed.
// The file is: loader, kernel, SYSINIT packed. The loader's first bytes are the MZ header that describes the packed SYSINIT; to the loader
// they are data.
struct Kernel {
    std::vector<Subject> parts;
    epk::Layout layout;
};

Kernel kernel_parts(const Bytes& data, const std::string& file_name) {
    const std::string whole_name = text::lower(file_name);
    const dos::Program whole = dos::read_program(data, file_name);
    if (whole.kind != dos::Kind::Kernel) throw FormatError(kReader, 0, whole_name + ": not a kernel image");
    const dos::MzHeader h = need_header(data);
    const auto body = dos::kernel_body(data, h);
    if (!body) throw FormatError(kReader, 0, whole_name + ": no BIOS and DOS body");
    Kernel k;
    struct Piece {
        std::string_view stem;
        std::uint32_t segment;
        std::uint64_t lo, hi;
        std::string_view label;
    };
    for (const Piece& piece : {Piece{"msload", dos::kLoaderSegment, 0, body->first, "loader"},
                               Piece{"msdos", dos::kKernelSegment, body->first, h.header_size, "bios and dos"}}) {
        Subject s;
        s.stem = std::string(piece.stem);
        s.name = std::format("{} (the {} of {})", piece.stem, piece.label, whole_name);
        s.original = data;
        dos::Program& p = s.program;
        p.kind = dos::Kind::KernelPart;
        p.base = piece.segment * 16;
        const ByteView image = part_of(data, piece.lo, piece.hi);
        p.image.assign(image.begin(), image.end());
        p.spans = {{p.base, static_cast<std::uint32_t>(p.image.size()), std::string(piece.label)}};
        for (const dos::Entry& e : whole.entries) {
            if (p.base <= e.address && e.address < p.base + p.image.size()) p.entries.push_back(e);
        }
        for (const auto& [va, segment] : whole.segments) {
            if (p.base <= va && va < p.base + p.image.size()) p.segments.try_emplace(va, segment);
        }
        p.ds = piece.segment == dos::kKernelSegment ? whole.ds : std::optional<std::uint32_t>(piece.segment);
        if (piece.segment == dos::kKernelSegment) {
            p.devices = whole.devices;
            p.device_segment = whole.device_segment;
        }
        k.parts.push_back(std::move(s));
    }
    k.layout = epk::layout(part_of(data, h.header_size, h.size), h.cs, h.ip);
    Subject sysinit;
    sysinit.program = dos::read_load_module(data);
    sysinit.stem = "sysinit";
    sysinit.name = "sysinit (of " + whole_name + ")";
    sysinit.original = data;
    sysinit.in_kernel = true;   // its file is IO.SYS: built into it, not a file of its own
    if (sysinit.program.unpacked != dos::Unpacked::Static) throw FormatError(kReader, 0, whole_name + ": SYSINIT is not stored as EXEPACK");
    k.parts.push_back(std::move(sysinit));
    Subject unpacker;
    unpacker.stem = "exepack";
    unpacker.name = "exepack (the unpacker LINK /EXEPACK put behind " + whole_name + ")";
    dos::Program& u = unpacker.program;
    u.kind = dos::Kind::KernelPart;
    u.base = kExepackSegment * 16 + 2 * k.layout.words;   // its code follows the header words
    u.image = k.layout.stub_code;
    u.spans = {{u.base, static_cast<std::uint32_t>(u.image.size()), "exepack"}};
    u.cs = kExepackSegment;
    u.ds = kExepackSegment;
    u.entries = {{u.base, "unpack", dos::Entry::Call::Far}};
    u.segments.try_emplace(u.base, kExepackSegment);
    k.parts.push_back(std::move(unpacker));
    return k;
}

Result kernel(Context& cx, const std::filesystem::path& path, const Bytes& data, const std::filesystem::path& outdir) {
    Result result;
    result.kind = "kernel";
    result.linked = true;
    fs::make_dirs(outdir);
    const std::string file_name = fs::to_text(path.filename());
    const Kernel k = kernel_parts(data, file_name);
    const std::filesystem::path fills = outdir / "sysinit.pack";
    if (!std::filesystem::is_regular_file(fills)) fs::write_text(fills, fills_text(k.layout, "SYSINIT"));
    std::map<std::string, std::filesystem::path> built;
    for (const Subject& s : k.parts) {
        const Result r = run_program(cx, s, outdir);
        result.parts.push_back(part_from(s.stem, r));
        result.sources.insert(result.sources.end(), r.sources.begin(), r.sources.end());
        result.instructions += r.instructions;
        result.raw += r.raw;
        result.data += r.data;
        result.rounds = std::max(result.rounds, r.rounds);
        cx.say(std::format("  {}: {} instructions, {} as bytes; {}", s.stem, r.instructions, r.raw, r.same ? "builds back the same" : notes_of(r.notes, "differs")));
        if (!r.linked) {
            result.linked = false;
            result.notes.push_back(s.stem + ": " + notes_of(r.notes, "not built"));
            continue;
        }
        built[s.stem] = *r.image;
    }
    if (!result.linked) return result;
    const Bytes stub_code = fs::read_file(built.at("exepack"));
    const Linked sysinit = read_linked(fs::read_file(built.at("sysinit")));
    std::vector<std::uint32_t> sites;
    for (const mzf::Relocation& r : sysinit.relocations) sites.push_back(linear_of(r));
    const dos::MzHeader oh = need_header(data);
    try {   // (the table in the order the original's lists it)
        const std::vector<std::uint32_t> theirs = dos::unpack_exepack(part_of(data, oh.header_size, oh.size), oh.cs, oh.ip).relocations;
        if (same_places(theirs, sites)) sites = theirs;
    } catch (const FormatError&) {   // (not a table that reads: the build's order stands)
    }
    const epk::Packed packed = epk::pack(sysinit.module, sites, {sysinit.h.cs, sysinit.h.ip}, {sysinit.h.ss, sysinit.h.sp}, read_fills(fills), stub_code,
                                         k.layout.words);
    const Bytes loader = fs::read_file(built.at("msload"));
    const Bytes kernel_body = fs::read_file(built.at("msdos"));
    // the header the loader's first bytes hold: what it says must be what was built
    const Bytes whole = joined({loader, kernel_body, packed.module});
    const dos::MzHeader head = need_header(whole);
    struct Want {
        std::string_view name;
        std::uint64_t have, want;
    };
    std::vector<std::string> wrong;
    for (const Want& w : {Want{"cs", head.cs, packed.registers.cs}, Want{"header", head.header_size, loader.size() + kernel_body.size()},
                          Want{"ip", head.ip, packed.registers.ip}, Want{"nreloc", head.relocation_count, 0},
                          Want{"size", head.size, loader.size() + kernel_body.size() + packed.module.size()}}) {
        if (w.have != w.want) wrong.push_back(std::format("{} {:#x}, built {:#x}", w.name, w.have, w.want));
    }
    if (!wrong.empty()) result.notes.push_back("the MZ header in msload does not describe what was built: " + text::join(wrong, ", "));
    {   // (what rebuild() joins, from the folder)
        Json desc;
        desc["file"] = text::upper(file_name);
        desc["words"] = k.layout.words;
        desc["order"] = sites;
        fs::write_text(outdir / "kernel.build.json", desc.dump(1));
    }
    const std::filesystem::path out = outdir / text::lower(file_name);
    fs::write_file(out, whole);
    result.image = out;
    result.same = whole == data && wrong.empty();
    if (!result.same) {
        result.notes.push_back(std::format("the joined file differs from the original at {:#x} ({} bytes, original {})", first_difference(whole, data), whole.size(),
                                           data.size()));
        // what the difference is: the same programs, relocation table in another order?
        if (std::ranges::equal(ByteView(whole).first(oh.header_size), ByteView(data).first(oh.header_size)) && whole.size() == data.size()) {
            const dos::Unpack mine = dos::unpack_exepack(packed.module, packed.registers.cs, packed.registers.ip);
            const dos::Unpack theirs = dos::unpack_exepack(part_of(data, oh.header_size, oh.size), oh.cs, oh.ip);
            if (mine.module == theirs.module && mine.cs == theirs.cs && mine.ip == theirs.ip && mine.ss == theirs.ss && mine.sp == theirs.sp &&
                same_places(mine.relocations, theirs.relocations)) {
                result.notes.emplace_back("loader, kernel and SYSINIT are the original's, byte for byte, unpacked; only the order of SYSINIT's relocation table "
                                          "differs (LINK listed them in its fixups' order)");
                result.equivalent = true;
            }
        }
    }
    return result;
}

// ---- files of several programs ---------------------------------------------------------------------------------------------------

// A DOS program and a Windows module in one file (REGEDIT.EXE, XCOPY32.EXE, UNINSTAL.EXE): the DOS program written and built here (as a file
// laid out as the original's stub), the Windows module built whole by the module builder (its own reconstruction, folder windows/), and the
// module put behind the program as the original has it (build/stubjoin).
Result dos_and_windows(Context& cx, const std::filesystem::path& path, const Bytes& original, const std::filesystem::path& outdir) {
    if (!cx.options.build_windows) throw UsageError("a DOS program with a Windows module needs the module builder (Options::build_windows)");
    const std::string file_name = fs::to_text(path.filename());
    const auto stub = dos::dos_and_windows(original);
    if (!stub) throw FormatError(kReader, 0, file_name + ": no Windows module behind its DOS program");
    Subject s;
    s.program = dos::read_program(original, file_name, false);
    s.name = text::lower(file_name);
    s.stem = stem_of(s.name);
    s.original = original;
    s.file_name = upper_name(path);
    Result result = run_program(cx, s, outdir);
    result.kind = "dos+windows";
    if (!result.same) return result;
    CB_ASSERT(result.file.has_value(), "a program that came back has its file laid out");
    const Bytes program = fs::read_file(*result.file);
    const WindowsBuild b = cx.options.build_windows(path, outdir / "windows");
    result.windows = b;
    const std::filesystem::path built = outdir / "windows" / text::lower(file_name);
    if (!b.linked || !std::filesystem::is_regular_file(built)) {
        result.notes.push_back("the Windows module did not build: " + (b.error.empty() ? std::string("not linked") : b.error));
        return result;
    }
    const Bytes together = build::stubjoin::join(program, fs::read_file(built), static_cast<std::uint32_t>(stub->second));
    fs::write_file(*result.file, together);
    result.file_same = together == original;
    std::erase_if(result.remarks, [](const std::string& r) { return r.starts_with("the file differs"); });
    if (!*result.file_same) {
        result.remarks.push_back(std::format("the file is {} bytes, the original {}: the module is OpenWatcom's build of its reconstruction", together.size(), original.size()));
    }
    std::string extra;
    if (!b.stand_ins.empty()) {
        const std::vector<std::string> first(b.stand_ins.begin(), b.stand_ins.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(6, b.stand_ins.size())));
        extra = std::format(", {} of them stand-ins: {}", b.stand_ins.size(), text::join(first, ", "));
    }
    result.remarks.push_back(std::format("the Windows module ({} functions{}) put behind the DOS program", b.functions, extra));
    return result;
}

// A real-mode driver and a VxD in one file (DRVSPACE.BIN): the driver written and built here, the VxD by the driver lane, and the file put
// together as the original is -- its MZ header (<stem>.hdr), the driver, the VxD laid out as Microsoft's LINK lays one out (build/vxdfile),
// its version resource (<vxd>.ver) and what the original carries behind that (<stem>.tail).
Result driver_and_vxd(Context& cx, const std::filesystem::path& path, const Bytes& original, const std::filesystem::path& outdir) {
    if (!cx.options.write_vxd) throw UsageError("a driver with a VxD needs the driver writer (Options::write_vxd)");
    const std::string file_name = fs::to_text(path.filename());
    const auto header = dos::driver_and_vxd(original);
    if (!header) throw FormatError(kReader, 0, file_name + ": no VxD behind its driver");
    const auto [hdr, lfa] = *header;
    const std::string stem = stem_of(text::lower(file_name));
    Result result;
    result.linked = true;
    result.kind = "driver+vxd";
    fs::make_dirs(outdir);
    Subject s;
    s.program = dos::read_driver_part(original);
    s.name = "the real-mode driver of " + text::lower(file_name);
    s.stem = stem;
    s.original = original;
    s.file_name = upper_name(path);
    const Result r = run_program(cx, s, outdir);
    const VxdPart v = cx.options.write_vxd(path, outdir / "vxd", cx.options.log);
    Part vxd;
    vxd.name = "vxd";
    vxd.source = v.source;
    vxd.image = v.image;
    vxd.linked = v.linked;
    vxd.same = v.same;
    vxd.rounds = v.rounds;
    vxd.instructions = v.instructions;
    vxd.raw = v.raw;
    vxd.data = v.data;
    vxd.notes = v.notes;
    for (const Part& part : {part_from(stem, r), vxd}) {
        result.parts.push_back(part);
        result.sources.push_back(part.source);
        result.instructions += part.instructions;
        result.raw += part.raw;
        result.data += part.data;
        result.rounds = std::max(result.rounds, part.rounds);
        cx.say(std::format("  {}: {} instructions, {} as bytes; {}", part.name, part.instructions, part.raw,
                           part.same ? "builds back the same" : notes_of(part.notes, "differs")));
        if (!part.linked) {
            result.linked = false;
            result.notes.push_back(part.name + ": " + notes_of(part.notes, "not built"));
        }
    }
    if (!result.linked) return result;
    CB_ASSERT(v.image.has_value() && r.image.has_value(), "a part that linked has its image");
    const std::string vstem = fs::to_text(v.image->stem());
    const Bytes head = beside(outdir / (stem + ".hdr"), [&] {
        const ByteView h = part_of(original, 0, hdr);
        return Bytes(h.begin(), h.end());
    });
    io::ByteReader reader(original, kReader);
    reader.seek(lfa + 0xB8);
    const std::uint32_t resource_at = reader.u32(), resource_length = reader.u32();
    const Bytes resource = beside(outdir / "vxd" / (vstem + ".ver"), [&] {
        const ByteView x = part_of(original, resource_at, static_cast<std::uint64_t>(resource_at) + resource_length);
        return Bytes(x.begin(), x.end());
    });
    const Bytes tail = beside(outdir / (stem + ".tail"), [&] {
        const ByteView x = part_of(original, static_cast<std::uint64_t>(resource_at) + resource_length, original.size());
        return Bytes(x.begin(), x.end());
    });
    const Bytes driver = fs::read_file(*r.image);
    const Bytes built_vxd = fs::read_file(*v.image);
    const std::uint32_t at = static_cast<std::uint32_t>(head.size() + driver.size());
    Bytes body = joined({head, driver, build::vxdfile::relay(built_vxd, at, resource), tail});
    put_dword(body, 0x3C, at);   // where the VxD is
    put_word(body, 2, body.size() % 512);
    put_word(body, 4, static_cast<std::uint32_t>((body.size() + 511) / 512));
    fs::make_dirs(outdir / "file");
    const std::filesystem::path out = outdir / "file" / upper_name(path);
    fs::write_file(out, body);
    result.image = out;
    result.file = out;
    result.same = body == original;
    if (!result.same) {
        result.notes.push_back(std::format("the file differs from the original at {:#x} ({} bytes, original {})", first_difference(body, original), body.size(), original.size()));
        result.equivalent = r.same && v.same;
        if (result.equivalent) {
            result.notes.emplace_back("the driver is the original's byte for byte and the VxD the same module (objects, descriptor, services, exports); laid out by "
                                      "OpenWatcom's linker, its tables differ");
        }
    }
    return result;
}

// ---- making it again from the folder ---------------------------------------------------------------------------------------------

std::filesystem::path rebuild_program(const std::filesystem::path& folder, const Options& options, const std::string& description);

std::filesystem::path rebuild_kernel(const std::filesystem::path& folder, const Options& options) {
    const Json k = Json::parse(fs::read_text(folder / "kernel.build.json"));
    std::map<std::string, std::filesystem::path> made;
    for (const char* stem : {"exepack", "msload", "msdos", "sysinit"}) made[stem] = rebuild_program(folder, options, std::string(stem) + ".build.json");
    const Bytes stub_code = fs::read_file(made.at("exepack"));
    const Linked sysinit = read_linked(fs::read_file(made.at("sysinit")));
    const std::vector<std::uint32_t> order = k.at("order").get<std::vector<std::uint32_t>>();
    const epk::Packed packed = epk::pack(sysinit.module, order, {sysinit.h.cs, sysinit.h.ip}, {sysinit.h.ss, sysinit.h.sp}, read_fills(folder / "sysinit.pack"),
                                         stub_code, k.at("words").get<unsigned>());
    const Bytes loader = fs::read_file(made.at("msload"));
    const Bytes kernel_body = fs::read_file(made.at("msdos"));
    fs::make_dirs(folder / "file");
    const std::filesystem::path out = folder / "file" / k.at("file").get<std::string>();
    fs::write_file(out, joined({loader, kernel_body, packed.module}));
    return out;
}

std::filesystem::path rebuild_program(const std::filesystem::path& folder, const Options& options, const std::string& description) {
    const std::filesystem::path desc_path = folder / description;
    if (!std::filesystem::is_regular_file(desc_path)) throw NotFoundError(desc_path, 0, "no .build.json (write it with dos-source)");
    const Json desc = Json::parse(fs::read_text(desc_path));
    const std::string stem = desc.at("stem").get<std::string>();
    std::optional<Facts> facts;
    if (desc.contains("facts") && desc["facts"].is_object()) facts = facts_from(desc["facts"]);
    if (facts && facts->packer && (*facts->packer == "EXEPACK" || *facts->packer == "PKLITE")) {   // (its unpacker first: a program of its own)
        rebuild_program(folder / text::lower(*facts->packer), options, text::lower(*facts->packer) + ".build.json");
    }
    const assembly::Made object = assembly::assemble(options.tools, folder / (stem + ".asm"), folder);
    if (!object.file) throw ToolError("wasm", 1, object.log, "the assembler rejects the source");
    const std::filesystem::path product = desc.at("product").get<std::string>();
    const std::filesystem::path out = folder / product;
    std::vector<std::string> lines;
    const std::string link_script = fs::read_text(folder / (fs::to_text(product.stem()) + ".lnk"));   // (named: the lines are views into it)
    for (const std::string_view line : text::lines(link_script)) {
        const std::string_view t = text::trim(line);
        if (!t.empty() && !text::istarts_with(line, "name ")) lines.emplace_back(line);
    }
    const assembly::Made linked = assembly::link(options.tools, lines, out, folder);
    if (!linked.file) throw ToolError("wlink", 1, linked.log, "the linker rejects the objects");
    if (desc.contains("regs") && desc["regs"].is_array() && desc["regs"].size() == 4) {
        Bytes file = fs::read_file(*linked.file);
        put_word(file, 0x0E, desc["regs"][0].get<std::uint32_t>());
        put_word(file, 0x10, desc["regs"][1].get<std::uint32_t>());
        put_word(file, 0x14, desc["regs"][2].get<std::uint32_t>());
        put_word(file, 0x16, desc["regs"][3].get<std::uint32_t>());
        fs::write_file(*linked.file, file);
    }
    if (!facts) return *linked.file;
    Result scratch;
    const Bytes body = compose_file(folder, stem, fs::read_file(*linked.file), *facts, scratch);
    fs::make_dirs(folder / "file");
    const std::filesystem::path made = folder / "file" / facts->file;
    fs::write_file(made, body);
    return made;
}

}  // namespace

// ---- the module ------------------------------------------------------------------------------------------------------------------

std::string_view usage() {
    return R"(dos_writer -- a real-mode program as assembly source that builds back into it.

    dos-source <program> [--out DIR]

Write a real-mode program's assembly source from its image, build it and compare the build with the original.

The real-mode base of the system was written in assembly, so assembly is what its source is. The file has one segment for every segment the
program itself names (each word its relocation table marks holds one, and so do the entry point and the stack), and every such word is
written as the segment it names (`dw seg S0605`), so the program can be assembled and linked at any address. Code is what analyze/dos_functions
found by following the program; the rest is written as data, text as text. Interrupt calls carry the name of the service.

An instruction is written as its mnemonic only when the assembler turns that text back into the original's bytes; the file is then assembled
and linked and the result compared with the original: every byte of the load module, the relocation table, the entry point and the stack.
Kinds of file: MZ programs (unpacked first when the file stores them packed -- the rebuilt program is the unpacked one, and the file is laid out
as the original's, packed again), .COM files, device drivers, IO.SYS, and a DOS program with a Windows module or a VxD behind it.
)";
}

Options options_for(const build::watcom::OpenWatcom& open_watcom, const std::filesystem::path& probes) {
    Options o;
    o.tools.wasm = open_watcom.tool("wasm");
    o.tools.wlink = open_watcom.tool("wlink");
    o.tools.env = open_watcom.environment(build::watcom::Target::Win32);
    o.probes = probes;
    return o;
}

Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir_, const Options& options) {
    Context cx(options);
    const std::filesystem::path outdir = std::filesystem::absolute(outdir_);
    const Bytes data = fs::read_file(program);
    const std::string file_name = fs::to_text(program.filename());
    const std::optional<dos::Kind> kind = dos::kind_of(data, file_name);
    if (!kind) throw FormatError(fs::to_text(program), 0, "not a real-mode program");
    switch (*kind) {
    case dos::Kind::Kernel: return kernel(cx, program, data, outdir);
    case dos::Kind::DriverWithVxd: return driver_and_vxd(cx, program, data, outdir);
    case dos::Kind::DosWithWindows: return dos_and_windows(cx, program, data, outdir);
    default: break;
    }
    Subject s;
    s.program = dos::read_program(data, file_name, false);   // moving itself is the program's own code
    s.name = text::lower(file_name);
    s.stem = stem_of(s.name);
    s.original = data;
    s.file_name = upper_name(program);
    return run_program(cx, s, outdir);
}

std::filesystem::path rebuild(const std::filesystem::path& folder_, const Options& options, const std::optional<std::string>& description) {
    const std::filesystem::path folder = std::filesystem::absolute(folder_);
    if (!description && std::filesystem::is_regular_file(folder / "kernel.build.json")) return rebuild_kernel(folder, options);
    std::string name;
    if (description) {
        name = *description;
    } else {
        std::vector<std::string> found;
        for (const auto& entry : std::filesystem::directory_iterator(folder)) {
            const std::string file = fs::to_text(entry.path().filename());
            if (file.ends_with(".build.json")) found.push_back(file);
        }
        if (found.empty()) throw NotFoundError(folder, 0, "no .build.json (write it with dos-source)");
        std::ranges::sort(found);
        name = found.front();
    }
    return rebuild_program(folder, options, name);
}

FindStub stub_finder(const Options& options) {
    return [options](ByteView file, std::uint64_t new_header, const std::filesystem::path& out,
                     const std::function<void(std::string_view)>& say) -> std::optional<std::filesystem::path> {
        if (file.size() < 6) return std::nullopt;
        const std::uint32_t last = word_at(file, 2), pages = word_at(file, 4);
        const std::uint64_t size = pages != 0 ? static_cast<std::uint64_t>(pages - 1) * 512 + (last != 0 ? last : 512) : 0;
        if (!(0x80 < size && size <= new_header) || size > file.size()) return std::nullopt;
        const std::filesystem::path folder = out / "stub";
        const std::filesystem::path carved = folder / "original" / "STUB.EXE";
        fs::make_dirs(carved.parent_path());
        fs::write_file(carved, file.first(static_cast<std::size_t>(size)));
        Result r;
        try {
            r = reconstruct(carved, folder, options);
        } catch (const Error& e) {   // (a stub the DOS lane cannot take: its bytes are kept)
            say(std::string("the stub: ") + e.what());
            return std::nullopt;
        }
        if (r.file && r.file_same.value_or(false)) {
            say(std::format("the stub: {} bytes, from its own source ({})", size, r.sources.empty() ? std::string() : fs::to_text(r.sources.front())));
            return std::filesystem::absolute(*r.file);
        }
        say("the stub does not come back from its source; its bytes are kept");
        return std::nullopt;
    };
}

int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say) {
    std::vector<std::string> programs;
    std::optional<std::string> out;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--out") {
            if (i + 1 >= args.size()) throw UsageError("--out needs a folder");
            out = args[++i];
        } else if (!args[i].starts_with("--")) {
            programs.push_back(args[i]);
        }
    }
    if (programs.empty()) {
        say(usage());
        return 2;
    }
    std::filesystem::path path = fs::from_text(programs.front());
    if (!std::filesystem::is_regular_file(path)) {
        const auto found = env.retail ? env.retail(programs.front()) : std::nullopt;
        if (!found) throw NotFoundError(path, 0, "no program " + programs.front());
        path = *found;
    }
    const std::filesystem::path outdir = out ? fs::from_text(*out) : env.state / "dos_source" / text::lower(fs::to_text(path.stem()));
    Options options = env.options;
    options.log = [&](const std::string& line) { say(line); };
    const Result r = reconstruct(path, outdir, options);
    const std::size_t n = std::max<std::size_t>(1, r.instructions);
    say(std::format("{}: {} instructions, {} ({}%) written as bytes; {} bytes of data; {} round{}; {}", fs::to_text(path.filename()), r.instructions, r.raw,
                    100 * r.raw / n, r.data, r.rounds, r.rounds == 1 ? "" : "s",
                    r.same         ? "builds back THE SAME"
                    : r.equivalent ? "builds back the same program (see below)"
                    : r.linked     ? "built, differs"
                                   : "not built"));
    for (const std::string& note : r.notes) say("  " + note);
    for (const std::filesystem::path& s : r.sources) say("  source: " + fs::to_text(s));
    return r.same || r.equivalent ? 0 : 1;
}

}  // namespace cb::source::dos_writer
