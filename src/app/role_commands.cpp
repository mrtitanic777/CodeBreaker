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
#include "toolchain/watcom.hpp"
#include "build/x86cc/x86cc.hpp"
#include "build/x86link/x86link.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/log.hpp"
#include "core/text.hpp"
#include "engine/machine_lanes.hpp"
#include "formats/detect.hpp"
#include "decompile/decompiler.hpp"
#include "engine/lane_kit.hpp"
#include "engine/lane_kit_internal.hpp"
#include "engine/lane_registry.hpp"
#include "formats/code_scan.hpp"
#include "knowledge/package.hpp"
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

// The console lane a file is for (as convert finds it), or empty.
std::string lane_of(const stdfs::path& file) {
    engine::register_all_lanes();
    std::string name;
    try {
        name = engine::kit_detail::machine_of_file(file);
        if (name.empty()) name = engine::machine_lanes().name_of_file(file);
    } catch (const std::exception&) {
    }
    return name;
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

int disassemble(const base::Corpus& corpus, std::span<const std::string> args) {
    const Words w = take(args, {"--cpu", "--at", "--count", "--offset", "--base", "--length", "--out", "-o"}, {"--list", "--help", "--sleigh", "--source"});
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
        const std::string lane = lane_of(file);
        cpu = lane.empty() ? nullptr : processor_of_lane(lane);
        if (!cpu) throw UsageError(std::format("which processor is {}? name it: --cpu NAME (disassemble --list)", fs::to_text(file.filename())));
        err_text(std::format("{}: {} ({})\n", fs::to_text(file.filename()), cpu->name, lane));
    }
    const Bytes data = fs::read_file(file);
    // --sleigh is accepted and does nothing: every processor with a description is read through it by default
    auto read = [&](const px::Processor& p, ByteView bytes, std::uint64_t at) { return p.disassemble(bytes, at, count); };
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
    // --sleigh is accepted and does nothing: every processor with a description is assembled through it by default
    if (!cpu.assemble) throw UsageError(std::format("{} is read but not yet assembled by CodeBreaker", cpu.name));
    const std::string source = fs::read_text(src);
    const stdfs::path folder = src.has_parent_path() ? src.parent_path() : stdfs::path(".");
    const px::Image image = cpu.assemble(source, folder);
    stdfs::path out = src;
    out.replace_extension(".bin");
    if (auto o = value(w, "--out", "-o")) out = *o;
    fs::write_file(out, image.data);
    out_text(std::format("{}: {} bytes ({}{})\n", fs::to_text(out), image.data.size(), cpu.name, image.base ? std::format(", from {:X}", image.base) : std::string()));
    return 0;
}

// What OpenWatcom's compilers and linker read from the environment (WATCOM, INCLUDE, WLINK_LNK, LIB), for `target`, over the
// process's own: the settings' OpenWatcom tree.
std::map<std::string, std::string> watcom_environment(toolchain::watcom::Target target) {
    const toolchain::watcom::OpenWatcom ow = toolchain::watcom::OpenWatcom::from_settings(config::current().text("openwatcom"));
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
                                                         watcom_environment(t == build::x86cc::Target::i86 ? toolchain::watcom::Target::Win16 : toolchain::watcom::Target::Win32));
    if (!run.out.empty()) out_text(run.out);
    if (!run.err.empty()) err_text(run.err);
    return run.exit_code;
}

int link(std::span<const std::string> args) {
    if (args.empty()) throw UsageError("link <linker arguments, as wlink's>   (OpenWatcom's wlink, in this program: `link system dos file a.obj name a.exe`)");
    if (!build::x86link::available()) throw UsageError("this build of CodeBreaker has no linker in it (x86link)");
    const build::x86link::Run run = build::x86link::link(std::vector<std::string>(args.begin(), args.end()), stdfs::current_path(),
                                                         watcom_environment(toolchain::watcom::Target::Win32));
    if (!run.out.empty()) out_text(run.out);
    if (!run.err.empty()) err_text(run.err);
    return run.exit_code;
}

// Where a console's cartridge sits in its CPU's memory and where its code starts: the file's bytes from `offset` at `base`, the entry
// points fixed ones and vectors read from the image (a 16- or 32-bit word, little- or big-endian). One row per machine: the rest
// is the one decoder and the one decompiler.
struct RomMap {
    std::string_view lane;
    std::size_t offset = 0;
    std::uint32_t base = 0;
    std::vector<std::uint32_t> entries;   // fixed entry points
    std::vector<std::uint32_t> vectors;   // addresses of words that hold an entry point
    unsigned vector_size = 2;
    bool vector_big = false;
    std::size_t max = 0;                  // the most of the file that is seen at `base` (0: all)
    std::uint32_t mirror = 0;             // an entry point's address within the cartridge's window when its bus mirrors it (the 2600: 4 KB)
};
const RomMap* rom_map(std::string_view lane) {
    static const RomMap kMaps[] = {
        {"sms", 0, 0, {0x0000, 0x0038, 0x0066}, {}, 2, false, 0xC000},
        {"gb", 0, 0, {0x0100, 0x0040, 0x0048, 0x0050, 0x0058, 0x0060}, {}, 2, false, 0x8000},
        {"genesis", 0, 0, {}, {0x04}, 4, true, 0x400000},
        {"atari2600", 0, 0xF000, {}, {0xFFFC}, 2, false, 0x1000, 0x0FFF},
        {"coleco", 0, 0x8000, {}, {0x800A}, 2, false, 0x8000},
        {"msx", 0, 0x4000, {}, {0x4002}, 2, false, 0x8000},
    };
    for (const RomMap& m : kMaps)
        if (m.lane == lane) return &m;
    return nullptr;
}

// The machine's hardware registers as the description's global lines (`G <address> <type> <name> [<space>]`), named as the
// knowledge base's registers (config/knowledge/<lane>/registers.json) name them, in the space they are in (a Z80 port: `io`). An
// address the machine's tables give two names (the 2600's TIA answers reads and writes differently) is left unnamed.
std::string register_globals(std::string_view lane, const stdfs::path& file) {
    knowledge::Knowledge& kb = knowledge::standard();
    std::string text;
    std::map<std::pair<std::string, std::uint32_t>, int> seen;
    const std::vector<knowledge::HardwareRegister> registers = kb.hardware_registers(lane);
    const bool game_gear = text::lower(fs::to_text(file.extension())) == ".gg";
    for (const knowledge::HardwareRegister& r : registers) ++seen[{r.space, r.address}];
    std::set<std::string> used;
    for (const knowledge::HardwareRegister& r : registers) {
        if (r.table == "game_gear_ports" && !game_gear) continue;
        if (seen[{r.space, r.address}] != 1 || !used.insert(r.name).second) continue;
        text += std::format("G\t{:x}\t{}x\t{}{}\n", r.address, r.width, r.name, r.space == "cpu" ? std::string() : "\t" + r.space);
    }
    return text;
}

// A console program to C through the one decompiler: its functions found by the one decoder (its processor's description), the
// description the decompiler reads written beside it, each function's C.
int decompile_console(const stdfs::path& file, std::string_view lane, const px::Processor& cpu, const std::optional<stdfs::path>& out) {
    const RomMap* map = rom_map(lane);
    if (!map || cpu.language.empty()) throw UsageError(std::format("{} ({}, {}): the one decompiler does not map this machine's memory yet", fs::to_text(file.filename()), lane, cpu.name));
    const Bytes data = fs::read_file(file);
    if (map->offset > data.size()) throw UsageError("the file is shorter than its header");
    std::size_t size = data.size() - map->offset;
    if (map->max != 0) size = std::min(size, map->max);
    Bytes mem(static_cast<std::size_t>(map->base) + size, 0);
    std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(map->offset), size, mem.begin() + map->base);
    std::set<std::uint32_t> starts(map->entries.begin(), map->entries.end());
    for (const std::uint32_t v : map->vectors) {
        if (v + map->vector_size > mem.size()) continue;
        std::uint32_t a = 0;
        for (unsigned k = 0; k < map->vector_size; ++k) a = map->vector_big ? (a << 8 | mem[v + k]) : (a | static_cast<std::uint32_t>(mem[v + k]) << (8 * k));
        starts.insert(map->mirror ? (a & map->mirror) | map->base : a);
    }
    formats::CodeSpace space;
    space.begin = map->base;
    space.end = static_cast<std::uint32_t>(map->base + size);
    const std::vector<std::uint32_t> functions = formats::find_functions(mem, starts, px::described_decoder(cpu), space);
    // the description: the processor's language, the cartridge's bytes where the CPU sees them, the functions
    const stdfs::path work = stdfs::path(file).concat(".decompile");
    stdfs::create_directories(work);
    std::string description = std::format("arch\t{}\nbytes\t{:x}\t{}\t{:x}\t{:x}\n", cpu.language, map->base, fs::to_text(stdfs::absolute(file)), map->offset, size);
    for (const std::uint32_t f : functions)
        if (f >= space.begin && f < space.end) description += std::format("F\t{:x}\tsub_{:04X}\t-1\n", f, f);
    description += register_globals(lane, file);
    const stdfs::path described = work / "description.txt";
    fs::write_text(described, description);
    engine::LaneKit kit(config::current());
    const decompile::decompiler::Run run = kit.decompiler().run(described);
    std::string text = std::format("/* {} -- {} ({}), {} functions, decompiled by CodeBreaker from the processor's description */\n\n", fs::to_text(file.filename()), lane, cpu.name, run.order.size());
    for (const std::string& name : run.order)
        if (const std::string* t = run.texts.find(name)) text += *t + "\n";
    if (out) fs::write_text(*out, text);
    else out_text(text);
    err_text(std::format("{}: {} functions found, {} decompiled\n", fs::to_text(file.filename()), functions.size(), run.order.size()));
    return 0;
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
            if (stdfs::exists(file, ec) && !formats::detect::kind(file)) {
                const std::string lane = lane_of(file);
                const px::Processor* cpu = lane.empty() ? nullptr : processor_of_lane(lane);
                if (cpu && rom_map(lane) && !cpu->language.empty()) {
                    std::optional<stdfs::path> out;
                    for (std::size_t k = 1; k + 1 < args.size(); ++k)
                        if (args[k] == "--out" || args[k] == "-o") out = args[k + 1];
                    return decompile_console(file, lane, *cpu, out);
                }
                return table.invoke("convert", args);
            }
        }
        return original(args);
    });
}

}  // namespace cb::app::roles
