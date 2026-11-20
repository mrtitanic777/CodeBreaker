// app/role_commands.cpp -- one command per role: CodeBreaker's one disassembler, assembler, compiler, linker and decompiler.
//
//     codebreaker disassemble FILE [--at ADDR] [--count N]               any program: a DOS/Windows module through the analyses, a
//     codebreaker disassemble FILE --cpu NAME [--offset N] [--base A]    ROM or raw bytes as the processor it names (--list: them all)
//     codebreaker assemble SOURCE --cpu NAME [-o OUT]                    source for any processor to its bytes
//     codebreaker compile SOURCE.c [--target i86|i386] [compiler args]   C to an object (OpenWatcom's wcc / wcc386, in this program)
//     codebreaker link <linker args>                                      objects to a program (OpenWatcom's wlink, in this program)
//     codebreaker decompile FILE                                          any program to source (a module to C; anything else as `convert`)
//
// Each is the front door of its role: the backends behind it (source/processors' rows, x86cc, x86link, the decompiler's lanes) are
// chosen by what the input is, never by a different command.

#include "app/role_commands.hpp"

#include <algorithm>
#include <charconv>
#include <filesystem>
#include <format>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "base/config.hpp"
#include "base/corpus.hpp"
#include "build/watcom.hpp"
#include "build/x86cc/x86cc.hpp"
#include "build/x86link/x86link.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/detect.hpp"
#include "source/isa.hpp"
#include "source/processors.hpp"
#include "ui/mcp_server.hpp"
#include "ui/mcp_wiring.hpp"

namespace cb::app::roles {
namespace {

namespace stdfs = std::filesystem;
namespace px = source::processors;

std::uint64_t number(std::string_view flag, std::string_view s) {
    std::string_view t = text::trim(s);
    int radix = 10;
    if (t.starts_with("0x") || t.starts_with("0X")) {
        t.remove_prefix(2);
        radix = 16;
    } else if (t.starts_with("$")) {
        t.remove_prefix(1);
        radix = 16;
    } else if (t.ends_with("h") || t.ends_with("H")) {
        t.remove_suffix(1);
        radix = 16;
    }
    std::uint64_t v = 0;
    const auto [end, ec] = std::from_chars(t.data(), t.data() + t.size(), v, radix);
    if (ec != std::errc() || end != t.data() + t.size() || t.empty()) throw UsageError(std::format("{} wants a number (decimal, 0x.., $.. or ..h), not '{}'", flag, s));
    return v;
}

// The words of a command line: `--flag value` pairs taken out, the rest kept in order.
struct Words {
    std::vector<std::string> rest;
    std::map<std::string, std::string> values;
    std::set<std::string> flags;
};

Words take(std::span<const std::string> args, std::initializer_list<std::string_view> with_value, std::initializer_list<std::string_view> bare) {
    Words w;
    for (std::size_t k = 0; k < args.size(); ++k) {
        const std::string& a = args[k];
        if (std::find(with_value.begin(), with_value.end(), a) != with_value.end()) {
            if (k + 1 >= args.size()) throw UsageError(std::format("{} needs a value", a));
            w.values[a] = args[++k];
        } else if (std::find(bare.begin(), bare.end(), a) != bare.end()) {
            w.flags.insert(a);
        } else {
            w.rest.push_back(a);
        }
    }
    return w;
}

std::optional<std::string> value(const Words& w, std::string_view a, std::string_view b = {}) {
    if (auto it = w.values.find(std::string(a)); it != w.values.end()) return it->second;
    if (!b.empty())
        if (auto it = w.values.find(std::string(b)); it != w.values.end()) return it->second;
    return std::nullopt;
}

// The processor a console lane's files run on (the lanes' names, engine/lane_list.cpp).
const px::Processor* processor_of_lane(std::string_view lane) {
    static const std::map<std::string_view, std::string_view> kLane = {
        {"nes", "6502"},     {"atari2600", "6502"}, {"atari7800", "6502"}, {"atari5200", "6502"}, {"lynx", "65c02"},  {"pce", "huc6280"},
        {"snes", "65816"},   {"sms", "z80"},        {"msx", "z80"},        {"zx", "z80"},         {"coleco", "z80"},  {"cpc", "z80"},
        {"gb", "sm83"},      {"genesis", "m68k"},   {"neogeo", "m68k"},    {"segacd", "m68k"},    {"sega32x", "sh2"}, {"saturn", "sh2"},
        {"dreamcast", "sh4"}, {"ps1", "mips"},      {"n64", "r4300"},      {"ps2", "r5900"},      {"gba", "arm"},     {"nds", "arm9"},
        {"n3ds", "arm11"},   {"vita", "armv7"},     {"switch", "a64"},     {"gamecube", "ppc"},   {"wii", "ppc"},     {"wiiu", "ppc"},
        {"vb", "v810"},      {"intv", "cp1610"},    {"o2", "i8048"},       {"chf", "f8"},         {"ngp", "tlcs900"}, {"vectrex", "m6809"},
    };
    const auto it = kLane.find(lane);
    return it == kLane.end() ? nullptr : px::lookup(it->second);
}

void print_lines(const std::vector<px::Line>& lines, const std::optional<stdfs::path>& out) {
    std::string listing;
    for (const px::Line& l : lines) {
        std::string hex;
        for (std::size_t k = 0; k < l.bytes.size(); ++k) hex += std::format("{}{:02X}", k ? " " : "", l.bytes[k]);
        listing += std::format("{:08X}  {:<20} {}\n", l.address, hex, l.text);
    }
    if (out) fs::write_text(*out, listing);
    else out_text(listing);
}

int list_processors() {
    std::string s = "The processors (disassemble / assemble --cpu NAME; any of the other names works too):\n";
    for (const px::Processor& p : px::all()) {
        std::string aka;
        for (std::string_view a : p.aliases) aka += (aka.empty() ? "" : ", ") + std::string(a);
        s += std::format("  {:<9} {}{}\n            also: {}; syntax: {}\n", p.name, p.summary, p.assemble ? "" : " (read only)", aka, p.syntax);
    }
    out_text(s);
    return 0;
}

// The mnemonic of a line's text, lower case ("ld", "move.l"): what two syntaxes of one instruction share.
std::string mnemonic_of(std::string_view text) {
    const std::size_t end = text.find_first_of(" \t");
    return text::lower(text.substr(0, end));
}

// Read `code` with the processor's own tables and with its SLEIGH description, instruction by instruction from the same place: each
// step where both decode, whether they take the same bytes and name the same mnemonic. Where they part, both start again after the
// longer one.
int compare_readers(const px::Processor& cpu, ByteView code, std::uint64_t base, std::size_t count) {
    std::size_t steps = 0, same_length = 0, same_mnemonic = 0, only_own = 0, only_sleigh = 0, neither = 0;
    std::string shown;
    const std::size_t unit = cpu.name == "cp1610" ? 2 : 1;   // (a word-addressed processor: an address counts words)
    int shown_n = 0;
    for (std::size_t at = 0; at < code.size() && (count == 0 || steps < count);) {
        const std::vector<px::Line> own = cpu.disassemble(code.subspan(at), base + at / unit, 1);
        const std::vector<px::Line> desc = px::read_described(cpu, code.subspan(at), base + at / unit, 1);
        if (own.empty() || desc.empty()) break;
        const bool own_ok = !own[0].text.starts_with(".d"), desc_ok = !desc[0].text.starts_with(".d");
        ++steps;
        std::size_t step = std::max(own[0].bytes.size(), desc[0].bytes.size());
        if (own_ok && desc_ok) {
            const bool len = own[0].bytes.size() == desc[0].bytes.size();
            const bool mn = mnemonic_of(own[0].text) == mnemonic_of(desc[0].text);
            same_length += len;
            same_mnemonic += len && mn;
            if (!len && shown_n++ < 20) shown += std::format("  {:08X}  own: {:<28} sleigh: {}\n", base + at / unit, own[0].text, desc[0].text);
        } else if (own_ok) {
            ++only_own;
            if (shown_n++ < 20) shown += std::format("  {:08X}  own: {:<28} sleigh: (no instruction)\n", base + at / unit, own[0].text);
        } else if (desc_ok) {
            ++only_sleigh;
        } else {
            ++neither;
        }
        at += std::max<std::size_t>(step, 1);
    }
    const std::size_t both = steps - only_own - only_sleigh - neither;
    out_text(std::format("{}: {} places read; both decode {}: same length {} ({:.2f}%), same length and mnemonic {} ({:.2f}%); only the own tables {}, "
                         "only SLEIGH {}, neither {}\n{}",
                         cpu.name, steps, both, same_length, both ? 100.0 * same_length / both : 0.0, same_mnemonic, both ? 100.0 * same_mnemonic / both : 0.0,
                         only_own, only_sleigh, neither, shown));
    return 0;
}

// The lanes' own decoder of a processor (source/isa), where there is one.
const formats::Decoder* own_decoder(std::string_view cpu) {
    if (cpu == "6502") return &source::isa::m6502_decoder();
    if (cpu == "z80") return &source::isa::z80_decoder();
    if (cpu == "sm83") return &source::isa::sm83_decoder();
    if (cpu == "m68k") return &source::isa::m68k_decoder();
    if (cpu == "mips") return &source::isa::mips_decoder();
    if (cpu == "arm") return &source::isa::arm_decoder();
    if (cpu == "arm9") return &source::isa::arm9_decoder();
    if (cpu == "arm11") return &source::isa::arm11_decoder();
    return nullptr;
}

// The lanes' own decoder and the description's (processors::described_decoder) at each instruction the own decoder steps through:
// whether they agree on the size, how control leaves and where it goes.
int compare_flow(const px::Processor& cpu, const Bytes& data, std::size_t offset, std::size_t length, std::uint64_t base, std::size_t count) {
    const formats::Decoder* own = own_decoder(cpu.name);
    if (!own) throw UsageError(std::format("{} has no lane decoder of its own to compare with", cpu.name));
    const formats::Decoder described = px::described_decoder(cpu);
    // the image indexed by address, as the cartridge formats hand it to a decoder
    Bytes mem(static_cast<std::size_t>(base) + length, 0);
    std::copy(data.begin() + static_cast<std::ptrdiff_t>(offset), data.begin() + static_cast<std::ptrdiff_t>(offset + length), mem.begin() + static_cast<std::ptrdiff_t>(base));
    static const char* const kFlow[] = {"next", "branch", "jump", "call", "return", "indirect", "halt"};
    std::size_t n = 0, both = 0, size_same = 0, flow_same = 0, target_same = 0, shown = 0;
    std::string diffs;
    for (std::uint32_t pc = static_cast<std::uint32_t>(base); pc < base + length && (count == 0 || n < count);) {
        const std::optional<formats::DecodedInsn> a = (*own)(mem, pc), b = described(mem, pc);
        ++n;
        if (a && b) {
            ++both;
            const bool s = a->size == b->size, f = a->flow == b->flow, t = a->target == b->target;
            size_same += s;
            flow_same += s && f;
            target_same += s && f && t;
            if (!(s && f && t) && shown++ < 25)
                diffs += std::format("  {:08X}  own: {} {} {}   sleigh: {} {} {}\n", pc, a->size, kFlow[static_cast<int>(a->flow)], a->target ? std::format("{:X}", *a->target) : "-",
                                     b->size, kFlow[static_cast<int>(b->flow)], b->target ? std::format("{:X}", *b->target) : "-");
        }
        pc += a && a->size ? a->size : 1;
    }
    out_text(std::format("{}: {} places; both decode {}: same size {}, same size+flow {}, same size+flow+target {} ({:.2f}%)\n{}", cpu.name, n, both, size_same, flow_same,
                         target_same, both ? 100.0 * target_same / both : 0.0, diffs));
    return 0;
}

int disassemble(const base::Corpus& corpus, std::span<const std::string> args) {
    const Words w = take(args, {"--cpu", "--at", "--count", "--offset", "--base", "--length", "--out", "-o"}, {"--list", "--help", "--sleigh", "--compare", "--source", "--compare-flow"});
    if (w.flags.contains("--list")) return list_processors();
    if (w.rest.size() != 1 || w.flags.contains("--help")) throw UsageError("disassemble FILE [--at ADDR] [--count N] | disassemble FILE --cpu NAME [--offset N] [--length N] [--base ADDR] [--count N] [--out FILE] | disassemble --list");
    const stdfs::path file = w.rest[0];
    if (!stdfs::is_regular_file(file)) throw NotFoundError(file, 0);
    const std::size_t count = value(w, "--count") ? static_cast<std::size_t>(number("--count", *value(w, "--count"))) : 0;
    std::optional<stdfs::path> out;
    if (auto o = value(w, "--out", "-o")) out = *o;

    const px::Processor* cpu = nullptr;
    if (auto name = value(w, "--cpu")) cpu = &px::find(*name);
    // a DOS or Windows module, with no processor named: through the analyses (functions, names, imports in the listing)
    std::size_t dos_offset = 0;
    std::optional<std::uint64_t> dos_base;
    if (!cpu && formats::detect::kind(file) == formats::detect::Lane::Dos) {
        // a DOS program: its bytes as the 8086 reads them -- a .COM file loaded at 100h, an MZ program's load image (after its header) at 0
        cpu = &px::find("x86-16");
        const Bytes head = fs::read_file(file);
        if (const std::optional<formats::detect::DosHeader> h = formats::detect::dos_header(head)) dos_offset = h->header_size, dos_base = 0;
        else dos_base = 0x100;
    }
    if (!cpu && formats::detect::kind(file)) {
        const std::optional<std::string> at = value(w, "--at");
        if (!at) throw UsageError("disassemble a module at an address or function: --at ADDR (hex, SEG:OFF, or a name `inspect --names` lists); or --cpu NAME to read its bytes raw");
        const config::Config& config = config::current();
        auto context = std::make_shared<ui::mcp::wiring::Context>(config, corpus);
        ui::mcp::Services services;
        ui::mcp::wiring::fill_code(services, context);
        const Json answer = services.disasm(file, *at, static_cast<std::int64_t>(count));
        std::string head = std::format("; {} at {} ({})\n", answer.value("module", ""), answer.value("addr", ""), answer.value("how", ""));
        const std::string body = answer.value("listing", "") + "\n";
        if (out) fs::write_text(*out, head + body);
        else out_text(head + body);
        return 0;
    }
    // a console's ROM: the processor its lane runs on
    if (!cpu) {
        const std::string lane = engine::machine_lanes().name_of_file(file);
        cpu = lane.empty() ? nullptr : processor_of_lane(lane);
        if (!cpu) throw UsageError(std::format("which processor is {}? name it: --cpu NAME (disassemble --list)", fs::to_text(file.filename())));
        err_text(std::format("{}: {} ({})\n", fs::to_text(file.filename()), cpu->name, lane));
    }
    const Bytes data = fs::read_file(file);
    // --sleigh: through the processor's SLEIGH description (the decompiler core's reader) instead of its own tables
    const bool described = w.flags.contains("--sleigh");
    auto read = [&](const px::Processor& p, ByteView bytes, std::uint64_t at) { return described ? px::read_described(p, bytes, at, count) : p.disassemble(bytes, at, count); };
    if (w.flags.contains("--compare-flow")) {
        const std::size_t off = value(w, "--offset") ? static_cast<std::size_t>(number("--offset", *value(w, "--offset"))) : 0;
        if (off > data.size()) throw UsageError("--offset is past the end of the file");
        std::size_t len = data.size() - off;
        if (auto l = value(w, "--length")) len = std::min<std::size_t>(len, static_cast<std::size_t>(number("--length", *l)));
        const std::uint64_t b0 = value(w, "--base") ? number("--base", *value(w, "--base")) : off;
        return compare_flow(*cpu, data, off, len, b0, count);
    }
    if (w.flags.contains("--source")) {   // source that assembles back to exactly these bytes (assemble --sleigh)
        const std::size_t off = value(w, "--offset") ? static_cast<std::size_t>(number("--offset", *value(w, "--offset"))) : dos_offset;
        if (off > data.size()) throw UsageError("--offset is past the end of the file");
        std::size_t len = data.size() - off;
        if (auto l = value(w, "--length")) len = std::min<std::size_t>(len, static_cast<std::size_t>(number("--length", *l)));
        const std::uint64_t b0 = value(w, "--base") ? number("--base", *value(w, "--base")) : dos_base.value_or(off);
        const std::string text = px::source_described(*cpu, ByteView(data).subspan(off, len), b0);
        if (out) fs::write_text(*out, text);
        else out_text(text);
        return 0;
    }
    if (w.flags.contains("--compare")) {   // both readers in step: where their instructions differ in length or mnemonic
        const std::size_t off = value(w, "--offset") ? static_cast<std::size_t>(number("--offset", *value(w, "--offset"))) : dos_offset;
        if (off > data.size()) throw UsageError("--offset is past the end of the file");
        std::size_t len = data.size() - off;
        if (auto l = value(w, "--length")) len = std::min<std::size_t>(len, static_cast<std::size_t>(number("--length", *l)));
        const std::uint64_t b0 = value(w, "--base") ? number("--base", *value(w, "--base")) : dos_base.value_or(off);
        return compare_readers(*cpu, ByteView(data).subspan(off, len), b0, count);
    }
    const std::size_t offset = value(w, "--offset") ? static_cast<std::size_t>(number("--offset", *value(w, "--offset"))) : dos_offset;
    if (offset > data.size()) throw UsageError(std::format("--offset {} is past the end of the file ({} bytes)", offset, data.size()));
    std::size_t length = data.size() - offset;
    if (auto l = value(w, "--length")) length = std::min<std::size_t>(length, static_cast<std::size_t>(number("--length", *l)));
    std::uint64_t base = dos_base.value_or(offset);
    if (auto b = value(w, "--base")) base = number("--base", *b);
    if (auto a = value(w, "--at")) {   // --at with a processor: an address inside the bytes, given --base
        const std::uint64_t at = number("--at", *a);
        if (at < base || at - base > length) throw UsageError(std::format("--at {:X} is not in the bytes read ({:X}..{:X})", at, base, base + length));
        const std::size_t skip = static_cast<std::size_t>(at - base);
        print_lines(read(*cpu, ByteView(data).subspan(offset + skip, length - skip), at), out);
        return 0;
    }
    print_lines(read(*cpu, ByteView(data).subspan(offset, length), base), out);
    return 0;
}

int assemble(std::span<const std::string> args) {
    const Words w = take(args, {"--cpu", "--out", "-o"}, {"--list", "--help", "--sleigh"});
    if (w.flags.contains("--list")) return list_processors();
    if (w.rest.size() != 1 || w.flags.contains("--help")) throw UsageError("assemble SOURCE --cpu NAME [-o OUT]   (assemble --list: the processors; x86 NASM is the default for .asm/.nasm)");
    const stdfs::path src = w.rest[0];
    if (!stdfs::is_regular_file(src)) throw NotFoundError(src, 0);
    std::string cpu_name;
    if (auto c = value(w, "--cpu")) cpu_name = *c;
    else {
        const std::string ext = text::lower(fs::to_text(src.extension()));
        if (ext == ".asm" || ext == ".nasm") cpu_name = "x86-16";
        else throw UsageError(std::format("which processor is {} for? --cpu NAME (assemble --list)", fs::to_text(src.filename())));
    }
    const px::Processor& cpu = px::find(cpu_name);
    // --sleigh: through the processor's SLEIGH description (the one assembler), the source in the description's syntax
    const bool described = w.flags.contains("--sleigh");
    if (!described && !cpu.assemble) throw UsageError(std::format("{} is read but not yet assembled by CodeBreaker", cpu.name));
    const std::string source = fs::read_text(src);
    const stdfs::path folder = src.has_parent_path() ? src.parent_path() : stdfs::path(".");
    const px::Image image = described ? px::assemble_described(cpu, source, folder) : cpu.assemble(source, folder);
    stdfs::path out = src;
    out.replace_extension(".bin");
    if (auto o = value(w, "--out", "-o")) out = *o;
    fs::write_file(out, image.data);
    out_text(std::format("{}: {} bytes ({}{})\n", fs::to_text(out), image.data.size(), cpu.name, image.base ? std::format(", from {:X}", image.base) : std::string()));
    return 0;
}

// What OpenWatcom's compilers and linker read from the environment (WATCOM, INCLUDE, WLINK_LNK, LIB), for `target`, over the
// process's own: the settings' OpenWatcom tree.
std::map<std::string, std::string> watcom_environment(build::watcom::Target target) {
    const build::watcom::OpenWatcom ow = build::watcom::OpenWatcom::from_settings(config::current().text("openwatcom"));
    std::map<std::string, std::string> env = ow.environment(target);
    std::string lib;
    for (const stdfs::path& d : {ow.root() / "lib386", ow.root() / "lib386" / "nt", ow.root() / "lib386" / "dos", ow.root() / "lib286", ow.root() / "lib286" / "dos",
                                 ow.root() / "lib286" / "win"})
        lib += (lib.empty() ? "" : ";") + fs::to_text(d);
    if (const std::optional<std::string> mine = fs::get_env("LIB"); mine && !mine->empty()) lib = *mine + ";" + lib;
    env["LIB"] = lib;
    return env;
}

int compile(std::span<const std::string> args) {
    const Words w = take(args, {"--target"}, {"--help"});
    if (w.rest.empty() || w.flags.contains("--help"))
        throw UsageError("compile SOURCE.c [--target i86|i386] [compiler arguments, as wcc's]   (C, compiled in this program by OpenWatcom's wcc / wcc386)");
    const std::string target = text::lower(value(w, "--target").value_or("i386"));
    build::x86cc::Target t;
    if (target == "i86" || target == "x86-16" || target == "16") t = build::x86cc::Target::i86;
    else if (target == "i386" || target == "x86-32" || target == "32") t = build::x86cc::Target::i386;
    else throw UsageError(std::format("--target is i86 (16-bit) or i386 (32-bit), not '{}'", target));
    if (!build::x86cc::available()) throw UsageError("this build of CodeBreaker has no C compiler in it (x86cc)");
    const build::x86cc::Run run = build::x86cc::compile(t, w.rest, stdfs::current_path(),
                                                         watcom_environment(t == build::x86cc::Target::i86 ? build::watcom::Target::Win16 : build::watcom::Target::Win32));
    if (!run.out.empty()) out_text(run.out);
    if (!run.err.empty()) err_text(run.err);
    return run.exit_code;
}

int link(std::span<const std::string> args) {
    if (args.empty()) throw UsageError("link <linker arguments, as wlink's>   (OpenWatcom's wlink, in this program: `link system dos file a.obj name a.exe`)");
    if (!build::x86link::available()) throw UsageError("this build of CodeBreaker has no linker in it (x86link)");
    const build::x86link::Run run = build::x86link::link(std::vector<std::string>(args.begin(), args.end()), stdfs::current_path(),
                                                         watcom_environment(build::watcom::Target::Win32));
    if (!run.out.empty()) out_text(run.out);
    if (!run.err.empty()) err_text(run.err);
    return run.exit_code;
}

}  // namespace

void register_role_commands(CommandTable& table, std::shared_ptr<base::Corpus> corpus) {
    table.set("disassemble", [corpus](std::span<const std::string> args) { return disassemble(*corpus, args); });
    table.set("assemble", assemble);
    table.set("compile", compile);
    table.set("link", link);
    // decompile FILE: a DOS or Windows module through the decompiler's lanes (as it was); any other program (a console ROM, an Xbox
    // 360 game, a Java or .NET program) through `convert`, which knows every lane
    table.intercept("decompile", [&table](const RawRunner& original, std::span<const std::string> args) -> int {
        if (!args.empty() && !args[0].starts_with("-")) {
            const stdfs::path file = args[0];
            std::error_code ec;
            if (stdfs::exists(file, ec) && !formats::detect::kind(file)) return table.invoke("convert", args);
        }
        return original(args);
    });
}

}  // namespace cb::app::roles
