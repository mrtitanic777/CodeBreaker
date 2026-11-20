// cb-unity: alone (helpers named for what they do here)
#include "source/dos_program.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"

namespace cb::source::dos_program {

namespace dos = cb::formats::dos;
namespace stdfs = std::filesystem;

namespace {

std::uint16_t u16_at(ByteView b, std::size_t at) { return static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)); }
void put_u16(Bytes& b, std::size_t at, std::uint32_t v) {
    b[at] = static_cast<std::uint8_t>(v & 0xFF);
    b[at + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFF);
}

std::string last_line(std::string_view messages, std::size_t at_most) {
    std::string_view trimmed = text::trim(messages);
    const std::size_t nl = trimmed.rfind('\n');
    std::string_view line = nl == std::string_view::npos ? trimmed : trimmed.substr(nl + 1);
    if (line.empty()) line = "failed";
    return std::string(text::trim(line).substr(0, at_most));
}

std::string with_crlf(std::string_view text_lf) { return text::replace_all(text_lf, "\n", "\r\n"); }

std::string registers_text(const std::array<std::uint32_t, 4>& r, const std::array<std::uint32_t, 4>& o) {
    return std::format("entry or stack differ: {:04x}:{:04x} {:04x}:{:04x}, original {:04x}:{:04x} {:04x}:{:04x}", r[0], r[1], r[2], r[3], o[0],
                       o[1], o[2], o[3]);
}

}  // namespace

LoadModule read_load_module(const stdfs::path& built, bool is_mz) {
    const Bytes data = fs::read_file(built);
    LoadModule m;
    if (!is_mz) {
        m.image = data;
        return m;
    }
    const std::optional<dos::MzHeader> h = dos::mz_header(data);
    if (!h || h->header_size > h->size || h->size > data.size())
        throw FormatError(fs::to_text(built), 0, "the built file has no MZ header that describes it");
    m.image.assign(data.begin() + h->header_size, data.begin() + static_cast<std::ptrdiff_t>(h->size));
    for (std::size_t i = 0; i < h->relocation_count; ++i) {
        const std::size_t entry = h->relocation_offset + 4 * i;
        if (entry + 4 > data.size()) throw TruncatedError(fs::to_text(built), entry, 4, data.size() > entry ? data.size() - entry : 0);
        const std::uint32_t off = u16_at(data, entry), seg = u16_at(data, entry + 2);
        const std::size_t at = seg * 16 + off;
        if (at + 2 <= m.image.size()) {
            m.relocations.insert(static_cast<std::uint32_t>(at));
            put_u16(m.image, at, (u16_at(m.image, at) + dos::kLoadSegment) & 0xFFFF);
        }
    }
    m.registers = std::array<std::uint32_t, 4>{(dos::kLoadSegment + h->cs) & 0xFFFF, h->ip, (dos::kLoadSegment + h->ss) & 0xFFFF, h->sp};
    return m;
}

DosProgramResult write_dos_program(const dos::Program& p, const std::string& stem, const std::string& name, const stdfs::path& outdir,
                                   const DosProgramOptions& options, const symbols::dos_interrupts::ServiceTable& services,
                                   const platforms::Decoder& decoder) {
    DosProgramResult result;
    result.kind = dos::kind_name(p.kind);
    const source::dos_image::DosImage w(p, name, services, decoder);
    result.flat = w.flat();
    for (const auto& [address, f] : w.analysis().functions) result.functions[address - p.base] = f;
    stdfs::create_directories(outdir);
    const stdfs::path src = outdir / (stem + ".asm");
    const stdfs::path out = outdir / (stem + (p.kind == dos::Kind::Com ? ".com" : !w.flat() ? ".exe" : ".bin"));
    result.source = src;

    // what assembles back to its own bytes: probed once, kept by the prober
    const auto candidates = w.candidates();
    std::vector<assembly::Candidate> asked;
    for (const auto& [at, c] : candidates) asked.push_back(c);
    std::set<std::string> labels;
    for (const std::uint32_t s : w.segments()) labels.insert(std::format("S{:04X}", s));
    std::set<assembly::Candidate> ok;
    if (options.prober != nullptr) ok = options.prober->probe(asked, labels, {}, outdir, assembly::Mode::Real16);
    std::set<std::uint32_t> good;
    for (const auto& [at, c] : candidates)
        if (ok.contains(c)) good.insert(at);
    for (const auto& [at, i] : w.instructions())
        if (i.target) good.insert(at);   // a jump to a label is the assembler's own business
    const ByteView whole = w.image();
    const ByteView original = w.flat() ? whole.subspan(std::min<std::size_t>(w.origin(), whole.size())) : whole;

    static const Regex error_line(R"(\((\d+)\)\s*:\s*Error)");
    static const Regex undefined_symbol(R"(Symbol (\w+) is not defined)");
    for (int rounds = 1; rounds <= options.max_rounds; ++rounds) {
        const source::dos_image::Source text = w.source(&good);
        fs::write_text(src, with_crlf(text.text));
        result.rounds = rounds;
        result.instructions = text.counts.instructions;
        result.raw = text.counts.raw;
        result.data = text.counts.data;

        const assembly::Made object = assembly::assemble(options.tools, src, outdir);
        if (!object.file) {
            // an instruction the assembler rejected goes back to bytes; so does a jump to a place that is not the start of anything
            // written (the middle of a word that holds a segment, say): it has no label to go to
            const std::vector<std::string_view> pieces = text::split(text.text, '\n');
            const std::vector<std::string> lines(pieces.begin(), pieces.end());
            std::set<std::string> failed;
            error_line.for_each_match(object.log, [&](const RegexMatch& m) {
                const std::size_t n = text::parse_int<std::size_t>(m[1], 10).value_or(0);
                if (n >= 1 && n <= lines.size()) {
                    std::string line = lines[n - 1];
                    if (const std::size_t at = line.find("    ;"); at != std::string::npos) line.resize(at);
                    failed.insert(std::string(text::trim(line)));
                }
                return true;
            });
            std::set<std::string> lost;
            undefined_symbol.for_each_match(object.log, [&](const RegexMatch& m) {
                lost.emplace(m[1]);
                return true;
            });
            std::vector<std::uint32_t> drop;
            for (const std::uint32_t at : good) {
                const auto mn = w.mnemonic(at);
                const auto& i = w.instructions().at(at);
                if ((mn && failed.contains(*mn)) || (i.target && lost.contains(w.label(*i.target)))) drop.push_back(at);
            }
            if (drop.empty()) {
                result.notes = {"assembler: " + last_line(object.log, 200)};
                break;
            }
            for (const std::uint32_t at : drop) good.erase(at);
            continue;
        }
        const assembly::Made linked = assembly::link(options.tools, w.link_lines(fs::to_text(object.file->filename())), out, outdir);
        if (!linked.file) {
            result.notes = {"linker: " + last_line(linked.log, 200)};
            break;
        }
        result.image = linked.file;
        result.linked = true;
        LoadModule built = read_load_module(*linked.file, !w.flat());
        const std::size_t n = std::min(built.image.size(), original.size());
        std::optional<std::size_t> first;
        for (std::size_t a = 0; a < n; ++a)
            if (built.image[a] != original[a]) {
                first = a;
                break;
            }
        if (!first && built.image.size() == original.size()) {
            std::vector<std::string> notes;
            const std::array<std::uint32_t, 4> want = {p.cs, p.ip, p.ss, p.sp};
            if (!w.flat()) {
                if (built.relocations != w.relocated())
                    notes.push_back(std::format("the relocation table differs ({} entries, original {})", built.relocations.size(), w.relocated().size()));
                if (built.registers && *built.registers != want && built.relocations == w.relocated()) {
                    // Where a program starts and where its stack is are four words of the file's header. A linker fills them in from
                    // the source's entry label and stack segment, and cannot be told a stack that lies inside the program or has no
                    // size, or an entry before the program's first byte; the code and data being the original's, the four words are
                    // written as the original has them.
                    Bytes file = fs::read_file(*linked.file);
                    if (file.size() >= 0x18) {
                        put_u16(file, 0x0E, (p.ss - dos::kLoadSegment) & 0xFFFF);
                        put_u16(file, 0x10, p.sp);
                        put_u16(file, 0x14, p.ip);
                        put_u16(file, 0x16, (p.cs - dos::kLoadSegment) & 0xFFFF);
                        fs::write_file(*linked.file, file);
                    }
                    built = read_load_module(*linked.file, true);
                    result.remarks.emplace_back("entry and stack set in the file's header after linking");
                    result.header_words = std::array<std::uint32_t, 4>{(p.ss - dos::kLoadSegment) & 0xFFFF, p.sp, p.ip, (p.cs - dos::kLoadSegment) & 0xFFFF};
                }
                if (built.registers && *built.registers != want) notes.push_back(registers_text(*built.registers, want));
            }
            result.same = notes.empty();
            result.notes = std::move(notes);
            if (result.same) {
                // how to make it again from the folder
                nlohmann::ordered_json desc;
                desc["stem"] = stem;
                desc["product"] = fs::to_text(out.filename());
                desc["flat"] = w.flat();
                if (result.header_words) desc["regs"] = *result.header_words;
                else desc["regs"] = nullptr;
                desc["facts"] = nullptr;
                fs::write_text(outdir / (stem + ".build.json"), desc.dump(1));
            }
            break;
        }
        // which instructions did not come back as they were -- found instruction by instruction, so one that came back a byte longer
        // is blamed itself and not everything behind it
        std::set<std::uint32_t> blamed = assembly::culprits<std::uint32_t>(original, built.image, w.pieces(good));
        std::erase_if(blamed, [&](std::uint32_t at) { return !good.contains(at); });
        if (!blamed.empty()) {
            for (const std::uint32_t at : blamed) good.erase(at);
            if (options.log)
                options.log(std::format("  round {}: {} instructions did not come back as they were; they go back to bytes", rounds, blamed.size()));
            continue;
        }
        const std::size_t at = (first ? *first : n) + w.origin();
        std::vector<std::uint32_t> hit;
        std::optional<std::uint32_t> last_before;
        for (const std::uint32_t a : good) {
            if (a > at) continue;
            last_before = last_before ? std::max(*last_before, a) : a;
            if (at < a + w.instructions().at(a).bytes.size()) hit.push_back(a);
        }
        if (hit.empty() && last_before) hit.push_back(*last_before);
        if (hit.empty()) {
            result.notes = {std::format("the built program differs at {:#x} and no instruction accounts for it", at)};
            break;
        }
        for (const std::uint32_t a : hit) good.erase(a);
        if (options.log) options.log(std::format("  round {}: differs at {:#x}; {} goes back to bytes", rounds, at, w.mnemonic(hit[0]).value_or("an instruction")));
    }
    return result;
}

}  // namespace cb::source::dos_program
