// symbols/program_commands.cpp -- see program_commands.hpp.
#include "symbols/program_commands.hpp"

#include <algorithm>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/powerpc_image.hpp"
#include "formats/xex.hpp"

namespace cb::symbols::program_commands {

namespace {

namespace pi = formats::powerpc_image;

formats::xex::Tools xex_tools(const config::Config& config) {
    formats::xex::Tools tools;
    tools.unpacked_dir = config.state() / "unpacked" / "xex";
    return tools;
}

// The image of an .xex (through its basefile) or of a PowerPC PE file.
pi::Image read_program(const std::filesystem::path& program, const config::Config& config) {
    return formats::xex::probe(program) ? pi::Image::read_xex(xex_tools(config), program) : pi::Image::read(program);
}

const pi::Section* find_section(const pi::Image& image, std::string_view name) {
    for (const pi::Section& s : image.sections())
        if (s.name == name) return &s;
    return nullptr;
}

constexpr std::string_view kXdkUsage =
    "codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]\n\n"
    "Names the XDK library functions a program was statically linked with, by their code.";

}  // namespace

cpp_classes::Classes classes_of(const std::filesystem::path& program, const Environment& environment) {
    const pi::Image image = read_program(program, environment.config);
    cpp_classes::Image view;
    for (const pi::Section& s : image.sections()) view.sections.push_back({s.name, s.va, s.size});
    view.code_begin = image.code_begin();
    view.code_end = image.code_end();
    view.function_starts.insert(image.functions().begin(), image.functions().end());
    for (const auto& [start, extent] : image.extents()) view.function_starts.insert(start);
    for (const pi::Section& s : image.sections()) {
        if (s.name != ".data" && s.name != ".rdata") continue;
        const ByteView bytes = image.bytes_at(s.va, s.size);
        view.memory.push_back({s.va, Bytes(bytes.begin(), bytes.end())});
    }
    return cpp_classes::find_classes(view);
}

xdk_libraries::ProgramText program_text(const std::filesystem::path& program, const Environment& environment) {
    const pi::Image image = read_program(program, environment.config);
    const pi::Section* text = find_section(image, ".text");
    if (text == nullptr) throw FormatError("xdk-match", 0, fs::to_text(program) + ": no .text section");
    const ByteView bytes = image.bytes_at(text->va, text->size);
    xdk_libraries::ProgramText out;
    out.address = text->va;
    out.words.reserve(bytes.size() / 4);
    for (std::size_t i = 0; i + 4 <= bytes.size(); i += 4)
        out.words.push_back((std::uint32_t{bytes[i]} << 24) | (std::uint32_t{bytes[i + 1]} << 16) | (std::uint32_t{bytes[i + 2]} << 8) | bytes[i + 3]);
    for (const auto& [address, extent] : image.extents())
        if (address >= text->va) out.starts.insert((address - text->va) / 4);
    return out;
}

namespace {

// The signatures as a cache file: a magic, then for each its words, the bits left out of each, its names and libraries, and whether it is entered
// by falling into it.
constexpr std::uint32_t kSignatureCacheMagic = 0x53584243;   // "CBXS"

Bytes encode_signatures(std::span<const xdk_libraries::Signature> signatures) {
    io::ByteWriter w;
    w.u32(kSignatureCacheMagic);
    w.u32(static_cast<std::uint32_t>(signatures.size()));
    const auto strings = [&w](const std::set<std::string>& set) {
        w.u32(static_cast<std::uint32_t>(set.size()));
        for (const std::string& s : set) w.cstr(s);
    };
    for (const xdk_libraries::Signature& s : signatures) {
        w.u32(static_cast<std::uint32_t>(s.words.size()));
        for (const std::uint32_t word : s.words) w.u32(word);
        for (const std::uint32_t mask : s.ignore) w.u32(mask);
        strings(s.names);
        strings(s.libs);
        w.u8(s.secondary ? 1 : 0);
    }
    return w.take();
}

// nullopt for a file that is not what encode_signatures wrote.
std::optional<std::vector<xdk_libraries::Signature>> decode_signatures(ByteView data) {
    try {
        io::ByteReader r(data, "xdk signatures");
        if (r.u32() != kSignatureCacheMagic) return std::nullopt;
        std::vector<xdk_libraries::Signature> out(r.u32());
        const auto strings = [&r](std::set<std::string>& set) {
            const std::uint32_t n = r.u32();
            for (std::uint32_t i = 0; i < n; ++i) {
                const ByteView s = r.cstr();
                set.emplace(s.begin(), s.end());
            }
        };
        for (xdk_libraries::Signature& s : out) {
            const std::uint32_t n = r.u32();
            if (std::uint64_t{n} * 8 > r.remaining()) return std::nullopt;
            s.words.resize(n);
            s.ignore.resize(n);
            for (std::uint32_t& word : s.words) word = r.u32();
            for (std::uint32_t& mask : s.ignore) mask = r.u32();
            strings(s.names);
            strings(s.libs);
            s.secondary = r.u8() != 0;
        }
        return out;
    } catch (const FormatError&) {
        return std::nullopt;
    }
}

}  // namespace

std::vector<xdk_libraries::Signature> signatures_of(const std::filesystem::path& folder, const Environment& environment) {
    const std::string stamp = xdk_libraries::signatures_stamp(xdk_libraries::library_files(folder));
    const std::filesystem::path cache = environment.config.state() / "xdk_signatures" / (stamp + ".bin");
    std::error_code ec;
    if (std::filesystem::is_regular_file(cache, ec)) {
        if (auto cached = decode_signatures(fs::read_file(cache))) return std::move(*cached);
    }
    std::vector<xdk_libraries::Signature> made = xdk_libraries::make_signatures(xdk_libraries::read_libraries(folder));
    fs::make_dirs(cache.parent_path());
    fs::write_file_atomic(cache, encode_signatures(made));
    return made;
}

cli::Command classes_command(const Environment& environment) {
    cli::Command c;
    c.name = "classes";
    c.summary = "a PowerPC program's C++ classes from its run-time type information: bases, virtual functions";
    c.area = "Xbox 360";
    c.params = {cli::Positional{"program", "an .xex or a PowerPC image"},
                cli::Option{"--output", "-o", "write the classes as JSON to this file", cli::Arg::Value, "FILE"}};
    c.handler = [environment](const cli::Args& a) {
        const std::filesystem::path program = fs::from_text(a.required("program"));
        const cpp_classes::Classes classes = classes_of(program, environment);
        out("{}", cpp_classes::summary(classes, fs::to_text(program.filename())));
        if (const std::optional<std::string> target = a.value("output")) {
            fs::write_text(fs::from_text(*target), cpp_classes::to_json(classes));
            out("  -> {}\n", *target);
        }
        return kExitOk;
    };
    return c;
}

int xdk_match(std::span<const std::string> args, const Environment& environment) {
    xdk_libraries::Services services;
    services.load_program = [&environment](const std::filesystem::path& p) { return program_text(p, environment); };
    services.load_signatures = [&environment](const std::filesystem::path& f) { return signatures_of(f, environment); };
    const std::string configured = environment.config.text("xdk_libs");
    services.default_folder = fs::from_text(configured.empty() ? std::string(xdk_libraries::kDefaultLibraries) : configured);
    return xdk_libraries::run(args, services);
}

std::function<int(std::span<const std::string>)> xdk_match_runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return xdk_match(args, environment); };
}

}  // namespace cb::symbols::program_commands
