#include "cb/formats/xex.hpp"

#include <algorithm>
#include <format>

#include <nlohmann/json.hpp>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/io.hpp"
#include "cb/core/json.hpp"
#include "cb/core/process.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/probe.hpp"
#include "xex_file.h"   // native/xex/src: CodeBreaker's one XEX parser

namespace cb::formats::xex {

namespace {

constexpr std::string_view kReader = "xex";
constexpr std::size_t kTableCountAt = 0x14;
constexpr std::size_t kTableAt = 0x18;
constexpr std::size_t kEntrySize = 8;
constexpr std::size_t kDataOffsetAt = 8;
constexpr std::size_t kBasicBlockHeader = 16;

bool has_magic(ByteView data) { return data.size() >= kMagic.size() && std::equal(kMagic.begin(), kMagic.end(), data.begin()); }

// A name from the header as it reads on its own: the text without its blanks, or nullopt for a blank or a name that
// holds blanks inside (padding after the library's name is not part of it; two words are not a name).
std::optional<std::string> plain_name(std::string_view raw) {
    const std::string_view word = text::trim(raw, " \t\r\n\v\f");
    if (word.empty() || word.find_first_of(" \t\r\n\v\f") != std::string_view::npos) return std::nullopt;
    return text::from_latin1(as_bytes(word));
}

std::string version_text(const ::xex::Version& v) { return v.str().substr(1); }   // without the "v"

const std::filesystem::path& program(const Tools& tools) {
    if (!std::filesystem::is_regular_file(tools.xex_program)) {
        throw NotFoundError(tools.xex_program, 0,
                            "the xex program is not built: cmake -S native/xex -B native/xex/build "
                            "&& cmake --build native/xex/build --config Release --target xex");
    }
    return tools.xex_program;
}

std::uint32_t be32_at(ByteView data, std::uint64_t at) {
    io::ByteReader r(data, kReader);
    r.seek(at);
    return r.u32_be();
}

}  // namespace

bool probe(const std::filesystem::path& path) {
    const std::optional<Bytes> head = read_prefix(path, 0, kMagic.size());
    return head && has_magic(*head);
}

OptionalHeaders::OptionalHeaders(ByteView xex) : data_(xex), count_(be32_at(xex, kTableCountAt)) {}

std::optional<OptionalHeader> OptionalHeaders::next() {
    if (index_ >= count_) return std::nullopt;
    io::ByteReader r(data_, kReader);
    const std::uint64_t at = kTableAt + std::uint64_t{index_} * kEntrySize;
    r.seek(at);
    OptionalHeader h;
    h.offset = at;
    h.key = r.u32_be();
    h.value = r.u32_be();
    ++index_;
    return h;
}

std::optional<OptionalHeader> find_optional_header(ByteView xex, std::uint32_t key) {
    OptionalHeaders table(xex);
    while (std::optional<OptionalHeader> h = table.next()) {
        if (h->key == key) return h;
    }
    return std::nullopt;
}

TitleIds ids(ByteView xex) {
    if (!has_magic(xex)) return {};
    const std::optional<OptionalHeader> info = find_optional_header(xex, kExecutionInfo);
    if (!info) return {};
    TitleIds out;
    out.media_id = be32_at(xex, info->value);
    out.title_id = be32_at(xex, std::uint64_t{info->value} + 0x0C);
    return out;
}

Header parse_header(ByteView xex) {
    std::optional<::xex::XexFile> parsed;
    try {
        // The parser is native/xex's own and throws std::exception types: an isolation boundary, converted here.
        parsed = ::xex::XexFile::parse(std::vector<std::uint8_t>(xex.begin(), xex.end()));
    } catch (const std::exception& e) {
        throw FormatError(kReader, 0, e.what());
    }
    const ::xex::XexFile& x = *parsed;
    Header h;
    const std::optional<std::string> pe_name = x.original_pe_name();
    if (pe_name) {
        // the first word of the name: what the image calls itself
        const std::string_view all = text::trim(*pe_name, " \t\r\n\v\f");
        const std::string_view word = all.substr(0, all.find_first_of(" \t\r\n\v\f"));
        if (!word.empty()) h.name = text::from_latin1(as_bytes(word));
    }
    h.base = x.image_base();
    h.entry = x.entry_point();
    for (const ::xex::ImportLibrary& lib : x.import_libraries()) {
        if (std::optional<std::string> n = plain_name(lib.name)) h.imports.push_back(std::move(*n));
    }
    for (const ::xex::StaticLibrary& lib : x.static_libraries()) {
        if (std::optional<std::string> n = plain_name(lib.name)) h.libraries.push_back({std::move(*n), version_text(lib.version)});
    }
    return h;
}

Header read_header(const std::filesystem::path& path) {
    const fs::MappedFile file(path);
    return parse_header(file.view());
}

std::filesystem::path basefile(const Tools& tools, const std::filesystem::path& xex) {
    const fs::MappedFile file(xex);
    const std::string stem = text::lower(fs::to_text(xex.stem()));
    const std::string sha = crypto::to_hex(crypto::sha1(file.view()));
    std::filesystem::path out = tools.unpacked_dir / std::format("{}-{}.exe", stem, sha.substr(0, 12));
    if (std::filesystem::is_regular_file(out)) return out;

    fs::make_dirs(tools.unpacked_dir);
    std::filesystem::path part = out;
    part += ".part";
    const process::ProcessResult r =
        process::run({.argv = {fs::to_text(program(tools)), "extract", fs::to_text(xex), fs::to_text(part)}});
    if (!r.ok() || !std::filesystem::is_regular_file(part)) {
        const Bytes& said = r.err.empty() ? r.out : r.err;
        throw ToolError("xex", r.exit_code, text::trim(text::from_latin1(said)), "no basefile for " + path_text(xex));
    }
    const Header h = parse_header(file.view());
    nlohmann::ordered_json sidecar;
    sidecar["xex"] = fs::to_text(std::filesystem::absolute(xex));
    sidecar["name"] = h.name ? nlohmann::ordered_json(*h.name) : nlohmann::ordered_json(nullptr);
    sidecar["base"] = h.base ? nlohmann::ordered_json(*h.base) : nlohmann::ordered_json(nullptr);
    sidecar["entry"] = h.entry ? nlohmann::ordered_json(*h.entry) : nlohmann::ordered_json(nullptr);
    sidecar["imports"] = h.imports;
    nlohmann::ordered_json libraries = nlohmann::ordered_json::array();
    for (const Library& l : h.libraries) libraries.push_back(nlohmann::ordered_json::array({l.name, l.version}));
    sidecar["libraries"] = libraries;
    std::filesystem::path sidecar_path = out;
    sidecar_path += ".json";
    // indent 1 and escaped non-ASCII: the layout the Python wrote, which its readers (import_libraries) still read
    fs::write_text(sidecar_path, sidecar.dump(1, ' ', true));
    std::filesystem::rename(part, out);
    return out;
}

void write_basic(const Tools& tools, const std::filesystem::path& xex, const std::filesystem::path& out) {
    const fs::MappedFile file(xex);
    const ByteView data = file.view();
    if (!has_magic(data)) throw FormatError(kReader, 0, path_text(xex) + ": not an XEX2 module");
    const std::uint32_t pe_offset = be32_at(data, kDataOffsetAt);
    const std::optional<OptionalHeader> info = find_optional_header(data, kFileFormatInfo);
    if (!info) throw FormatError(kReader, 0, path_text(xex) + ": no file format info header");
    if (be32_at(data, info->value) < kBasicBlockHeader) {
        throw FormatError(kReader, info->value, path_text(xex) + ": file format info too small for one block");
    }
    const fs::MappedFile image_file(basefile(tools, xex));
    const ByteView image = image_file.view();

    const ByteView head = io::checked_range(data, 0, pe_offset);
    Bytes header(head.begin(), head.end());
    (void)io::checked_range(header, info->value, kBasicBlockHeader);   // the block must lie inside the header
    // one block, all of the image as data, no zero run (the block count comes from the size)
    io::ByteWriter block;
    block.u32_be(kBasicBlockHeader);
    block.u16_be(0);
    block.u16_be(1);
    block.u32_be(static_cast<std::uint32_t>(image.size()));
    block.u32_be(0);
    std::copy(block.data().begin(), block.data().end(), header.begin() + info->value);
    append(header, image);
    fs::write_file(out, header);
}

std::optional<std::vector<std::string>> import_libraries(const std::filesystem::path& image) {
    std::filesystem::path sidecar = image;
    sidecar += ".json";
    if (!std::filesystem::is_regular_file(sidecar)) return std::nullopt;
    Json doc;
    if (!json::try_parse(fs::read_text(sidecar), doc)) return std::nullopt;
    const auto imports = doc.find("imports");
    if (imports == doc.end() || !imports->is_array()) return std::nullopt;
    std::vector<std::string> out;
    for (const auto& name : *imports) {
        if (!name.is_string()) return std::nullopt;
        out.push_back(name.get<std::string>());
    }
    return out;
}

int run(const Tools& tools, const std::vector<std::string>& args) {
    process::ProcessSpec spec;
    spec.argv.push_back(fs::to_text(program(tools)));
    spec.argv.insert(spec.argv.end(), args.begin(), args.end());
    spec.out = process::Capture::Inherit;
    spec.err = process::Capture::Inherit;
    return static_cast<int>(process::run(spec).exit_code);
}

}  // namespace cb::formats::xex
