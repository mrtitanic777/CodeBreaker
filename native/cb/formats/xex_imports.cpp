#include "cb/formats/xex_imports.hpp"

#include <algorithm>
#include <map>

#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"

namespace cb::formats::xex_imports {

namespace {

// The two records an import can have, by (library, ordinal).
struct Records {
    std::optional<std::uint32_t> slot, thunk;
};

}  // namespace

std::vector<Import> imports(ByteView xex_file, const powerpc_image::Image& image, const OrdinalNames& names) {
    const std::optional<xex::OptionalHeader> header = xex::find_optional_header(xex_file, kImportHeader);
    if (!header) return {};
    io::ByteReader r(xex_file, "xex imports");
    r.seek(header->value);
    const std::uint32_t total = r.u32_be();
    const std::uint32_t strings_size = r.u32_be();
    const std::uint32_t library_count = r.u32_be();

    std::vector<std::string> libraries;
    while (libraries.size() < library_count) {
        const std::size_t at = r.pos();
        libraries.push_back(text::from_latin1(r.cstr()));
        r.seek(at + ((r.pos() - at + 3) & ~std::size_t{3}));   // each name is padded to four bytes
    }

    std::map<std::pair<std::string, std::uint32_t>, Records> found;
    const std::uint64_t end = std::uint64_t{header->value} + total;
    for (std::uint64_t block = std::uint64_t{header->value} + 12 + strings_size; block < end;) {
        r.seek(block);
        const std::uint32_t size = r.u32_be();
        if (size == 0) break;
        r.seek(block + 0x24);
        const std::uint16_t index = r.u16_be();
        const std::uint16_t count = r.u16_be();
        if ((index & 0xFF) >= libraries.size()) throw FormatError("xex imports", block + 0x24, "an import block names a library that is not listed");
        const std::string& library = libraries[index & 0xFF];
        for (std::uint16_t i = 0; i < count; ++i) {
            const std::uint32_t record = r.u32_be();
            const std::optional<std::uint32_t> word = image.word(record);
            if (!word) continue;
            Records& entry = found[{library, *word & 0xFFFF}];
            ((*word >> 24) != 0 ? entry.thunk : entry.slot) = record;
        }
        block += size;
    }

    std::vector<Import> out;
    for (const auto& [key, records] : found) {
        Import import;
        import.library = key.first;
        import.ordinal = key.second;
        std::optional<std::string> known = names ? names(import.library, import.ordinal) : std::nullopt;
        import.name = known && !known->empty() ? *known : std::string(text::split(import.library, '.')[0]) + "_" + std::to_string(import.ordinal);
        import.is_function = records.thunk.has_value();
        import.slot = records.slot;
        import.thunk = records.thunk;
        out.push_back(std::move(import));
    }
    return out;
}

cli::Command command(const Environment& environment) {
    cli::Command c;
    c.name = "xex-imports";
    c.summary = "an Xbox 360 module's imports: library, ordinal, name, function or variable, slot, thunk";
    c.area = "Xbox 360";
    c.params = {cli::Positional{"module", "the .xex"}};
    c.handler = [environment](const cli::Args& a) {
        const std::filesystem::path path = fs::from_text(a.required("module"));
        const Bytes file = fs::read_file(path);
        out_text("library\tordinal\tname\tkind\tslot\tthunk\n");
        if (!xex::find_optional_header(file, kImportHeader)) return kExitOk;   // (a module without imports needs no image)
        const powerpc_image::Image image = powerpc_image::Image::read_xex(environment.tools, path);
        for (const Import& i : imports(file, image, environment.ordinal_name)) {
            out("{}\t{}\t{}\t{}\t{}\t{}\n", i.library, i.ordinal, i.name, i.is_function ? "function" : "variable",
                i.slot && *i.slot != 0 ? std::format("0x{:08X}", *i.slot) : "", i.thunk && *i.thunk != 0 ? std::format("0x{:08X}", *i.thunk) : "");
        }
        return kExitOk;
    };
    return c;
}

}  // namespace cb::formats::xex_imports
