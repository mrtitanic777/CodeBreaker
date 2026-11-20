// cb/recompile/command.cpp -- see command.hpp.
#include "cb/recompile/command.hpp"

#include <algorithm>
#include <memory>

#include <nlohmann/json.hpp>

#include "cb/base/converted.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/vmx128.hpp"
#include "cb/formats/xex.hpp"
#include "cb/recompile/disassemble.hpp"
#include "cb/recompile/title.hpp"
#include "cb/recompile/xenon.hpp"

namespace cb::recompile::command {

namespace {

namespace pi = formats::powerpc_image;
namespace stdfs = std::filesystem;
namespace rcfg = cb::recompile::config;

formats::xex::Tools xex_tools(const cb::config::Config& config) {
    formats::xex::Tools tools;
    tools.unpacked_dir = config.state() / "unpacked" / "xex";
    return tools;
}

stdfs::path converted_root(const cb::config::Config& config) { return config.path("converted").value_or(config.checkout_root() / "Converted"); }

xenon::Config xenon_config(const cb::config::Config& config) {
    xenon::Config c;
    c.checkout_root = config.checkout_root();
    c.state_dir = config.state();
    c.visual_studio = config.path("visual_studio");
    c.ppc_binutils = config.path("ppc_binutils");
    return c;
}

// The console's ordinal names ({library: {ordinal: name}}), read the first time a name is asked for.
project::ImportNames ordinal_names(const cb::config::Config& config) {
    const stdfs::path file = config.checkout_root() / "config" / "symbols" / "console_ordinals.json";
    auto table = std::make_shared<std::optional<Json>>();
    return [file, table](std::string_view library, std::uint32_t ordinal) -> std::string {
        if (!table->has_value()) {
            std::error_code ec;
            *table = stdfs::is_regular_file(file, ec) ? json::load(file) : Json::object();
        }
        const Json& root = **table;
        const auto lib = root.find(std::string(library));
        if (lib == root.end() || !lib->is_object()) return {};
        const auto name = lib->find(std::to_string(ordinal));
        return name != lib->end() && name->is_string() ? name->get<std::string>() : std::string();
    };
}

// The title image the scaffold reads: the image as the recompiler reads it, and what an .xex header says of its imports.
project::TitleImage read_title(const stdfs::path& file, const Environment& environment) {
    project::TitleImage title;
    const pi::Image image = read_image(file, environment);
    title.is_xex = formats::xex::probe(file);
    if (title.is_xex) {
        const formats::xex::Header header = formats::xex::read_header(file);
        title.image_name = header.name;
        title.libraries = header.imports;
    }
    // an import stub is named `import<library>_ord_<n>`
    for (const auto& [address, symbol] : image.imports()) {
        const std::string_view name = symbol;
        const std::size_t sep = name.find("_ord_");
        if (!name.starts_with("import") || sep == std::string_view::npos) continue;
        const auto library = text::parse_int<std::size_t>(name.substr(6, sep - 6));
        const auto ordinal = text::parse_int<std::uint32_t>(name.substr(sep + 5));
        if (library && ordinal) title.imports.push_back({address, *library, *ordinal});
    }
    title.loaded = loaded_image(image);
    return title;
}

stdfs::path rexglue_root(const cb::config::Config& config) { return config.checkout_root() / "native" / "cb" / "recompile" / "rexglue"; }

// The functions a module exports (its XEX security info's export table: {+0x20 high half of the addresses, +0x24 count, +0x28
// ordinal base, +0x2C offsets}, an address being (high << 16) + offset, 0 for none). Other modules call them through pointers the
// export lookup hands out, so nothing in the image need call them, and a leaf function has no .pdata entry either.
std::vector<std::uint32_t> exported_functions(const stdfs::path& basic, const pi::Image& image) {
    std::vector<std::uint32_t> out;
    if (!formats::xex::probe(basic)) return out;
    try {
        const std::optional<std::uint32_t> table = formats::xex::read_header(basic).export_table;
        if (!table) return out;
        const auto word = [&image](std::uint32_t at) -> std::optional<std::uint32_t> {
            const ByteView b = image.bytes_at(at, 4);
            if (b.size() < 4) return std::nullopt;
            return (static_cast<std::uint32_t>(b[0]) << 24) | (static_cast<std::uint32_t>(b[1]) << 16) | (static_cast<std::uint32_t>(b[2]) << 8) | b[3];
        };
        const std::optional<std::uint32_t> high = word(*table + 0x20), count = word(*table + 0x24);
        if (!high || !count || *count > 0x10000) return out;
        for (std::uint32_t i = 0; i < *count; ++i) {
            const std::optional<std::uint32_t> offset = word(*table + 0x2C + i * 4);
            if (!offset || *offset == 0 || (*offset & 3) != 0) continue;
            const std::uint32_t address = (*high << 16) + *offset;
            if (address >= image.code_begin() && address < image.code_end()) out.push_back(address);   // a data export is no function
        }
    } catch (const Error&) {   // a module without a readable export table has no exports to add
    }
    return out;
}

// XenonRecomp's tool chain for a title's project.
x360project::Recompiler make_recompiler(const Environment& environment, xenon::Xenon& tool) {
    const cb::config::Config& config = environment.config;
    x360project::Recompiler r;
    r.source = tool.source_dir();
    try {
        r.visual_studio = tool.visual_studio();
    } catch (const ToolError&) {   // the Build script names it; a title can still be recompiled without
    }
    r.runtime_cmake = config.checkout_root() / "native" / "cb" / "recompile" / "x360" / "x360.cmake";
    r.write_basic = [&environment](const stdfs::path& xex, const stdfs::path& out) {
        formats::xex::write_basic(xex_tools(environment.config), xex, out);
    };
    r.write_switch_tables = [&environment](const stdfs::path& file, const stdfs::path& basic, const std::optional<std::string>& analysed) {
        const pi::Image image = read_image(basic, environment);
        const rcfg::WrittenTables written = rcfg::write_switch_tables(file, code_image(image), analysed);
        return x360project::Recompiler::SwitchTables{written.found, written.added};
    };
    r.make_config = [&environment](const stdfs::path& basic, std::string_view out_dir, std::string_view switch_tables, std::string_view file_path) {
        const pi::Image image = read_image(basic, environment);
        disassemble::Disassembler dis(vector_decoder());
        rcfg::Title title = config_title(image);
        title.seeds = exported_functions(basic, image);
        rcfg::MakeOptions options;
        options.out_dir = std::string(out_dir);
        options.switch_tables = std::string(switch_tables);
        options.file_path = std::string(file_path);
        return rcfg::make(title, dis, basic, options);
    };
    r.savegprlr_14_address = [&environment](const stdfs::path& basic) -> std::uint32_t {
        const pi::Image image = read_image(basic, environment);
        disassemble::Disassembler dis(vector_decoder());
        const rcfg::Addresses helpers = rcfg::helpers(config_title(image), dis);
        const auto at = helpers.find("savegprlr_14_address");
        return at == helpers.end() ? 0 : at->second;
    };
    // A module the title ships is a program to translate when its image has code and an exception table (.pdata: the runtime and
    // the recompiler's function finder both need one), is not managed code (an IL assembly: its PE has a CLR header, data
    // directory 14, and its "code" is not PowerPC) and is more than a resource stub.
    r.has_native_code = [&environment](const stdfs::path& basic) {
        try {
            const pi::Image image = read_image(basic, environment);
            if (!image.has_code() || image.extents().empty()) return false;
            // The resource modules of a managed title (every .NET assembly and .nlp table of Terraria is one) carry the same small
            // native stub: nothing in it to translate or to export. A real program is larger or exports something.
            constexpr std::uint32_t kStubCode = 32 * 1024;
            const bool exports = std::ranges::any_of(image.sections(), [](const pi::Section& s) { return s.name == ".edata"; });
            if (image.code_end() - image.code_begin() < kStubCode && !exports) return false;
            const ByteView dos = image.bytes_at(image.base(), 0x40);
            if (dos.size() < 0x40) return true;
            const auto le32 = [](ByteView b, std::size_t at) { return static_cast<std::uint32_t>(b[at]) | (static_cast<std::uint32_t>(b[at + 1]) << 8) | (static_cast<std::uint32_t>(b[at + 2]) << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24); };
            const ByteView pe = image.bytes_at(image.base() + le32(dos, 0x3C), 0x18 + 0x60 + 15 * 8);
            // PE signature, 20-byte file header, optional header: 96 bytes of fields (PE32), then the data directories
            constexpr std::size_t kClrDirectory = 0x18 + 0x60 + 14 * 8;
            return !(pe.size() >= kClrDirectory + 8 && le32(pe, kClrDirectory) != 0);
        } catch (const Error&) {
            return false;
        }
    };
    return r;
}

}  // namespace

pi::Image read_image(const stdfs::path& file, const Environment& environment) {
    return formats::xex::probe(file) ? pi::Image::read_xex(xex_tools(environment.config), file) : pi::Image::read(file);
}

discover::Image discovery_image(const pi::Image& image) {
    discover::Image out;
    out.code_begin = image.code_begin();
    out.code_end = image.code_end();
    out.entry = image.entry();
    const ByteView code = image.bytes_at(out.code_begin, out.code_end - out.code_begin);
    out.code.assign(code.begin(), code.end());
    for (const pi::Section& section : image.sections()) {
        const ByteView bytes = image.bytes_at(section.va, section.size);
        out.sections.push_back({section.name, section.va, section.characteristics, Bytes(bytes.begin(), bytes.end())});
    }
    for (const auto& [start, extent] : image.extents()) out.extents[start] = extent.size;
    for (const auto& [address, name] : image.imports()) out.imports.insert(address);
    return out;
}

powerpc::LoadedImage loaded_image(const pi::Image& image) {
    powerpc::LoadedImage loaded;
    loaded.image = discovery_image(image);
    disassemble::ImageFacts& facts = loaded.facts;
    const ByteView file = image.file();
    facts.data.assign(file.begin(), file.end());
    facts.base = image.base();
    facts.entry = image.entry();
    facts.code_begin = image.code_begin();
    facts.code_end = image.code_end();
    for (const pi::Span& span : image.spans())
        facts.chunks.push_back({span.va, static_cast<std::uint32_t>(span.file_offset), static_cast<std::uint32_t>(span.size)});
    for (const pi::Section& s : image.sections()) facts.sections.push_back({s.name, s.va, s.size, s.characteristics});
    return loaded;
}

switches::CodeImage code_image(const pi::Image& image) {
    switches::CodeImage out;
    out.base = image.base();
    const std::uint32_t hi = image.code_end();
    const ByteView data = image.bytes_at(image.base(), hi - image.base());
    out.data.assign(data.begin(), data.end());
    out.code_begin = image.code_begin();
    out.code_end = hi;
    for (const auto& [start, extent] : image.extents()) out.functions.push_back({start, start + extent.size});
    std::sort(out.functions.begin(), out.functions.end(), [](const auto& a, const auto& b) { return a.begin < b.begin; });
    return out;
}

rcfg::Title config_title(const pi::Image& image) {
    rcfg::Title title;
    title.image = discovery_image(image);
    for (const auto& [address, name] : image.run_names()) title.run_names[address] = name;
    return title;
}

disassemble::VectorDecoder vector_decoder() {
    return [](std::uint32_t word) -> std::optional<std::pair<std::string, std::string>> {
        if (const auto decoded = decompile::vmx128::decode(word)) return std::make_pair(decoded->mnemonic, decoded->operands);
        return std::nullopt;
    };
}

int recompile(std::span<const std::string> args, const Environment& environment) {
    const cb::config::Config& config = environment.config;
    base::converted::Store store(converted_root(config));
    powerpc::Hooks hooks;
    hooks.load_image = [&environment](const stdfs::path& file) { return loaded_image(read_image(file, environment)); };
    hooks.vector = vector_decoder();
    hooks.compiler.checkout_root = config.checkout_root();
    if (const std::string gpp = config.text("gpp"); !gpp.empty()) hooks.compiler.gpp = fs::from_text(gpp);
    hooks.project_output = [&store](const stdfs::path& binary) {
        const base::converted::Project project = store.project(binary);
        return project.src() / (text::lower(project.name()) + ".cpp");
    };
    hooks.record = [&store](const stdfs::path& binary, const std::vector<stdfs::path>& written, std::string_view notes) {
        store.project(binary).record("recompile", written, notes);
    };
    return powerpc::main(hooks, args);
}

int recompile_project(std::span<const std::string> args, const Environment& environment) {
    const cb::config::Config& config = environment.config;
    xenon::Xenon tool(xenon_config(config));
    project::CommandHooks hooks{.env = {}, .recompiler = {}, .locate_title = {}, .store = base::converted::Store(converted_root(config))};
    hooks.env.read_title = [&environment](const stdfs::path& file) { return read_title(file, environment); };
    hooks.env.vector = vector_decoder();
    hooks.env.import_names = ordinal_names(config);
    hooks.env.rexglue = rexglue_root(config);
    hooks.recompiler = make_recompiler(environment, tool);
    hooks.locate_title = [](const stdfs::path& path) -> std::optional<project::TitleFiles> {
        std::optional<title::Title> found = title::locate(path);
        if (!found) return std::nullopt;
        auto shared = std::make_shared<title::Title>(std::move(*found));
        project::TitleFiles files;
        files.name = shared->name();
        files.source = fs::to_text(shared->source());
        files.xex = shared->read_xex();
        files.unpack = [shared](const stdfs::path& folder) { return shared->unpack(folder); };
        return files;
    };
    return project::run_command(hooks, args, [](std::string_view line) { out("{}\n", line); });
}

int xenon_recompile(std::span<const std::string> args, const Environment& environment) {
    xenon::Xenon tool(xenon_config(environment.config));
    if (args.size() != 1) {
        out("usage: codebreaker xenon-recompile <config.toml>\n");
        return kExitUsage;
    }
    try {
        xenon::xenon_recompile_command(tool, args);
    } catch (const UsageError& e) {
        out("{}\n", e.what());
        return kExitUsage;
    } catch (const Error& e) {
        out("{}\n", e.what());
        return kExitFailed;
    }
    return kExitOk;
}

Runner recompile_runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return recompile(args, environment); };
}
Runner recompile_project_runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return recompile_project(args, environment); };
}
Runner xenon_recompile_runner(const Environment& environment) {
    return [environment](std::span<const std::string> args) { return xenon_recompile(args, environment); };
}

}  // namespace cb::recompile::command
