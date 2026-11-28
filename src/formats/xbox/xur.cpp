#include "formats/xbox/xur.hpp"

#include <algorithm>
#include <format>
#include <limits>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/io.hpp"
#include "core/log.hpp"
#include "formats/format_builtin.hpp"
#include "formats/xbox/utf16.hpp"

namespace cb::formats::xbox::xur {

namespace eng = cb::formats::engine;

namespace {

constexpr std::string_view kXurReader = "xur";
constexpr std::size_t kXurFileSizeAt = 14;

std::uint32_t xur_shifted(std::uint32_t value, std::int64_t delta, std::string_view what) {
    const std::int64_t moved = std::int64_t{value} + delta;
    if (moved < 0 || moved > std::numeric_limits<std::uint32_t>::max()) throw FormatError(kXurReader, 0, std::string(what) + " does not fit after the edit");
    return static_cast<std::uint32_t>(moved);
}

void xur_put_be32(Bytes& v, std::size_t at, std::uint32_t x) {
    for (std::size_t i = 0; i < 4; ++i) v[at + i] = static_cast<std::uint8_t>(x >> (24 - 8 * i));
}

}  // namespace

std::string tag_text(std::uint32_t tag) {
    Bytes four(4);
    for (std::size_t i = 0; i < 4; ++i) four[i] = static_cast<std::uint8_t>(tag >> (24 - 8 * i));
    return text::from_latin1(four);
}

// The layout is xur.fmt's; whether a section lies in the file is this reader's.
Scene Scene::parse(ByteView file) {
    eng::ReadOptions options;
    options.keep_source = false;   // only read from
    const eng::Tree tree = eng::builtin().read("xur", file, options);
    const eng::Node& h = tree.root;
    Scene s;
    s.version = static_cast<std::uint32_t>(h.u("version"));
    s.flags = static_cast<std::uint32_t>(h.u("flags"));
    s.tool_version = static_cast<std::uint16_t>(h.u("tool_version"));
    s.file_size = static_cast<std::uint32_t>(h.u("file_size"));
    for (const eng::Node& n : h.get("sections").children)
        s.sections.push_back({static_cast<std::uint32_t>(n.u("tag")), static_cast<std::uint32_t>(n.u("offset")), static_cast<std::uint32_t>(n.u("length"))});
    s.file_.assign(file.begin(), file.end());
    return s;
}

std::optional<ByteView> Scene::section_data(std::uint32_t tag) const {
    for (const Section& s : sections) {
        if (s.tag == tag) return io::checked_range(file_, s.offset, s.length);
    }
    return std::nullopt;
}

std::vector<std::string> Scene::strings() const {
    std::vector<std::string> out;
    const std::optional<ByteView> blob = section_data(kStrn);
    if (!blob) return out;
    eng::ReadOptions options;
    options.root = "Strings";
    options.keep_source = false;
    const eng::Tree tree = eng::builtin().read("xur", *blob, options);
    for (const eng::Node& n : tree.root.get("strings").children) out.push_back(from_utf16be(blob->subspan(n.get("text").offset, n.get("text").size)));
    return out;
}

Bytes build_strn(std::span<const std::string> strings) {
    io::ByteWriter w;
    for (const std::string& s : strings) {
        const Bytes encoded = to_utf16be(s);
        if (encoded.size() / 2 > 0xFFFF) throw FormatError(kXurReader, 0, "a string is longer than 65535 UTF-16 units");
        w.u16_be(static_cast<std::uint16_t>(encoded.size() / 2));
        w.bytes(encoded);
    }
    return w.take();
}

Bytes set_strings(ByteView file, std::span<const std::string> strings) {
    const Scene scene = Scene::parse(file);
    const auto strn = std::find_if(scene.sections.begin(), scene.sections.end(), [](const Section& s) { return s.tag == kStrn; });
    if (strn == scene.sections.end()) throw UsageError("the scene has no STRN section to edit");
    const std::size_t index = static_cast<std::size_t>(strn - scene.sections.begin());
    (void)scene.section_data(kStrn);   // the section must lie in the file
    const Bytes body = build_strn(strings);
    const std::int64_t delta = static_cast<std::int64_t>(body.size()) - static_cast<std::int64_t>(strn->length);

    if (strn->offset < scene.table_end()) throw FormatError(kXurReader, strn->offset, "the STRN section overlaps the section table");
    Bytes out(file.begin(), file.begin() + strn->offset);   // everything before STRN: header, table, earlier sections
    append(out, body);
    append(out, file.subspan(std::size_t{strn->offset} + strn->length));   // and everything after it, moved by the size change
    xur_put_be32(out, kXurFileSizeAt, xur_shifted(scene.file_size, delta, "the file size"));
    for (std::size_t i = 0; i < scene.sections.size(); ++i) {
        const std::size_t entry = kHeaderSize + i * kSectionEntrySize;
        if (i == index) xur_put_be32(out, entry + 8, static_cast<std::uint32_t>(body.size()));
        else if (scene.sections[i].offset > strn->offset) xur_put_be32(out, entry + 4, xur_shifted(scene.sections[i].offset, delta, "a section offset"));
    }
    return out;
}

Bytes set_string(ByteView file, std::size_t index, std::string_view text) {
    std::vector<std::string> all = Scene::parse(file).strings();
    if (index >= all.size()) throw UsageError(std::format("string index {} out of range (the scene has {})", index, all.size()));
    all[index] = std::string(text);
    return set_strings(file, all);
}

// ---- the command line ----------------------------------------------------------------------------------------------------------

namespace {

Scene xur_open(const cli::Args& a) { return Scene::parse(fs::read_file(fs::from_text(a.required("file")))); }

}  // namespace

cli::Command command() {
    cli::Command c;
    c.name = "xur";
    c.summary = "read and edit Xbox 360 XUR (compiled XUI) scenes";
    c.area = "Xbox 360";
    const cli::Positional file{"file", "the .xur scene"};
    c.subcommands.push_back(cli::Command{"info", "the header and the section table", {file}, [](const cli::Args& a) {
        const std::filesystem::path path = fs::from_text(a.required("file"));
        const Bytes data = fs::read_file(path);
        const Scene s = Scene::parse(data);
        out("magic       : XUIB  version {}  toolVersion 0x{:X}\n", s.version, s.tool_version);
        out("file size   : {} (actual {})\n", s.file_size, data.size());
        out("sections    : {}\n", s.sections.size());
        for (const Section& sec : s.sections) out("   {:<6} offset=0x{:<6X} length={}\n", tag_text(sec.tag), sec.offset, sec.length);
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"strings", "the string table", {file}, [](const cli::Args& a) {
        const std::vector<std::string> strs = xur_open(a).strings();
        for (std::size_t i = 0; i < strs.size(); ++i) out("  [{:>3}] {}\n", i, strs[i]);
        out("total: {} strings\n", strs.size());
        return kExitOk;
    }});
    c.subcommands.push_back(cli::Command{"set", "replace one string and write the scene",
                                         {file, cli::Positional{"index", "the string's index"}, cli::Positional{"text", "the new text"}, cli::Positional{"out", "the .xur to write"}},
                                         [](const cli::Args& a) {
        const Bytes data = fs::read_file(fs::from_text(a.required("file")));
        const Bytes edited = set_string(data, *a.integer<std::size_t>("index"), a.required("text"));
        fs::write_file(fs::from_text(a.required("out")), edited);
        out("set string [{}] -> {} ({} bytes)\n", a.required("index"), a.required("out"), edited.size());
        return kExitOk;
    }});
    return c;
}

}  // namespace cb::formats::xbox::xur
