// cb/formats/inspect_text.cpp -- the byte-level views of inspect (strings, hex, resource bodies), the text reports
// (render, markdown) and the `info` command. describe() itself is inspect.cpp.
#include <algorithm>
#include <format>
#include <iterator>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/dos.hpp"
#include "cb/formats/inspect.hpp"
#include "cb/formats/resources.hpp"

namespace cb::formats::inspect {

using Json = cb::Json;

// ======================================================================================================================
// strings
// ======================================================================================================================
namespace {

bool printable(std::uint8_t b) { return b == '\t' || b == '\r' || b == '\n' || (b >= 0x20 && b <= 0x7e); }

// "%d", "%08x", "%-5s", "%lu", "%ws" ...: a printf conversion somewhere in the text
bool has_format_spec(std::string_view t) {
    constexpr std::string_view kFlagsAndSizes = "-+ 0123456789.*#hlLIw", kConversions = "sdiouxXeEfgGcpn";
    for (std::size_t i = 0; i < t.size(); ++i) {
        if (t[i] != '%') continue;
        std::size_t j = i + 1;
        while (j < t.size() && kFlagsAndSizes.find(t[j]) != std::string_view::npos) ++j;
        if (j < t.size() && kConversions.find(t[j]) != std::string_view::npos) return true;
    }
    return false;
}

// An identifier in capitals with at least one underscore: FILE_ATTRIBUTE_NORMAL
bool is_constant_name(std::string_view t) {
    const auto upper = [](char c) { return c >= 'A' && c <= 'Z'; };
    const auto upper_or_digit = [&](char c) { return upper(c) || (c >= '0' && c <= '9'); };
    if (t.empty() || !upper(t[0])) return false;
    std::size_t i = 1;
    while (i < t.size() && upper_or_digit(t[i])) ++i;
    int groups = 0;
    while (i < t.size() && t[i] == '_') {
        std::size_t j = i + 1;
        while (j < t.size() && upper_or_digit(t[j])) ++j;
        if (j == i + 1) return false;
        ++groups;
        i = j;
    }
    return i == t.size() && groups >= 1;
}

// file offset -> where that byte is loaded: an address in a 32-bit image, segment:offset in a 16-bit one
using Locator = std::function<std::optional<Address>(std::uint64_t)>;

Locator locator_for(ByteView data) {
    struct Span {
        std::uint64_t at, length, base;
    };
    const auto sig = detect::new_header(data);
    if (sig == detect::NewHeader::Pe) {
        const auto image = pe::Image::try_parse(data);
        if (!image) return {};
        std::vector<Span> spans;
        for (const pe::Section& s : image->sections()) spans.push_back({s.raw_ptr, s.raw_size, image->header().image_base + s.virtual_address});
        return [spans](std::uint64_t off) -> std::optional<Address> {
            for (const Span& s : spans)
                if (s.at <= off && off < s.at + s.length) return Address(s.base + (off - s.at));
            return std::nullopt;
        };
    }
    if (sig == detect::NewHeader::Ne) {
        const auto module = ne::Module::try_open(data);
        if (!module) return {};
        std::vector<Span> spans;
        try {
            for (const ne::Segment& s : module->segments())
                if (s.file_offset != 0) spans.push_back({s.file_offset, s.length, s.index});
        } catch (const Error&) {
            return {};
        }
        return [spans](std::uint64_t off) -> std::optional<Address> {
            for (const Span& s : spans)
                if (s.at <= off && off < s.at + s.length)
                    return Address(SegmentAddress{static_cast<std::uint32_t>(s.base), static_cast<std::uint32_t>(off - s.at),
                                                  SegmentAddress::Style::NeSegment});
            return std::nullopt;
        };
    }
    return {};
}

}  // namespace

const char* string_kind_name(StringKind k) { return k == StringKind::Ascii ? "ascii" : "utf16"; }

const char* string_category_name(StringCategory c) {
    switch (c) {
        case StringCategory::Format: return "format";
        case StringCategory::Module: return "module";
        case StringCategory::Path: return "path";
        case StringCategory::Constant: return "constant";
        case StringCategory::Error: return "error";
        case StringCategory::Other: return "other";
    }
    return "other";
}

StringCategory categorize(std::string_view raw) {
    const std::string_view t = text::trim(raw);
    if (has_format_spec(t)) return StringCategory::Format;
    for (const std::string_view ext : {".dll", ".exe", ".sys", ".drv", ".vxd", ".386", ".ocx", ".cpl", ".scr", ".inf", ".ini", ".hlp"})
        if (text::iends_with(t, ext)) return StringCategory::Module;
    if (t.find('\\') != std::string_view::npos || t.find('/') != std::string_view::npos) return StringCategory::Path;
    if (is_constant_name(t)) return StringCategory::Constant;
    const std::string lowered = text::lower(t);
    for (const std::string_view w : {"error", "fail", "invalid", "cannot", "unexpected", "exception"})
        if (lowered.find(w) != std::string::npos) return StringCategory::Error;
    return StringCategory::Other;
}

std::vector<StringRow> find_strings(ByteView d, const StringOptions& options) {
    const std::size_t min_length = std::max<std::size_t>(1, options.min_length);
    std::vector<StringRow> ascii, wide;
    const std::size_t n = d.size();
    for (std::size_t i = 0; i < n;) {  // [printable]{min,}
        std::size_t j = i;
        while (j < n && printable(d[j])) ++j;
        if (j > i && j - i >= min_length) ascii.push_back({i, std::nullopt, StringKind::Ascii, StringCategory::Other, std::string(as_text(d.subspan(i, j - i)))});
        i = j > i ? j : i + 1;
    }
    if (options.wide) {
        for (std::size_t i = 0; i < n;) {  // (?:[printable]\x00){min,}
            std::size_t j = i;
            while (j + 1 < n && printable(d[j]) && d[j + 1] == 0) j += 2;
            if (j > i && (j - i) / 2 >= min_length) {
                std::string s;
                for (std::size_t k = i; k < j; k += 2) s.push_back(static_cast<char>(d[k]));
                wide.push_back({i, std::nullopt, StringKind::Utf16, StringCategory::Other, std::move(s)});
                i = j;
            } else {
                ++i;
            }
        }
    }
    std::vector<StringRow> rows;
    rows.reserve(ascii.size() + wide.size());
    std::merge(std::make_move_iterator(ascii.begin()), std::make_move_iterator(ascii.end()),
               std::make_move_iterator(wide.begin()), std::make_move_iterator(wide.end()), std::back_inserter(rows),
               [](const StringRow& a, const StringRow& b) { return a.offset < b.offset; });  // stable: 8-bit first at a tie
    if (options.limit && rows.size() > *options.limit) rows.resize(*options.limit);
    for (StringRow& r : rows) r.category = categorize(r.text);
    return rows;
}

std::vector<StringRow> strings(const std::filesystem::path& path, const StringOptions& options) {
    const Bytes data = fs::read_file(path);
    std::vector<StringRow> rows = find_strings(data, options);
    if (const Locator where = locator_for(data))
        for (StringRow& r : rows) r.address = where(r.offset);
    return rows;
}

Json to_json(const std::vector<StringRow>& rows) {
    Json a = Json::array();
    for (const StringRow& r : rows) {
        Json j = Json::object();
        j["offset"] = r.offset;
        if (!r.address) j["address"] = nullptr;
        else if (const auto* linear = std::get_if<std::uint64_t>(&*r.address)) j["address"] = *linear;
        else j["address"] = address_text(*r.address);
        j["kind"] = string_kind_name(r.kind);
        j["category"] = string_category_name(r.category);
        j["text"] = r.text;
        a.push_back(std::move(j));
    }
    return a;
}

// ======================================================================================================================
// hex
// ======================================================================================================================
std::vector<std::string> hex_lines(ByteView data, std::uint64_t base) {
    static constexpr char kDigits[] = "0123456789ABCDEF";
    std::vector<std::string> lines;
    lines.reserve(data.size() / 16 + 1);
    for (std::size_t i = 0; i < data.size(); i += 16) {
        const std::size_t end = std::min<std::size_t>(i + 16, data.size());
        std::string hex, shown;
        for (std::size_t k = i; k < end; ++k) {
            if (k > i) hex.push_back(' ');
            hex.push_back(kDigits[data[k] >> 4]);
            hex.push_back(kDigits[data[k] & 15]);
            shown.push_back(data[k] >= 32 && data[k] < 127 ? static_cast<char>(data[k]) : '.');
        }
        hex.resize(std::max<std::size_t>(hex.size(), 47), ' ');
        lines.push_back(std::format("{:08X}  {}  {}", base + i, hex, shown));
    }
    return lines;
}

std::string hexdump(const std::filesystem::path& path, std::uint64_t offset, std::uint64_t length) {
    const fs::MappedFile file(path);
    const ByteView all = file.view();
    if (offset >= all.size()) return "";
    const std::size_t n = static_cast<std::size_t>(std::min<std::uint64_t>(length, all.size() - offset));
    return text::join(hex_lines(all.subspan(static_cast<std::size_t>(offset), n), offset), "\n");
}

// ======================================================================================================================
// resource bodies
// ======================================================================================================================
namespace {

std::string version_text(ByteView data, bool wide) {
    const resources::Version v = resources::version(data, wide);
    std::vector<std::string> out;
    if (v.fixed) {
        const resources::FixedFileInfo& f = *v.fixed;
        out.push_back(std::format("FileVersion      {}.{}.{}.{}", f.file_version_ms >> 16, f.file_version_ms & 0xFFFF,
                                  f.file_version_ls >> 16, f.file_version_ls & 0xFFFF));
        out.push_back(std::format("ProductVersion   {}.{}.{}.{}", f.product_version_ms >> 16, f.product_version_ms & 0xFFFF,
                                  f.product_version_ls >> 16, f.product_version_ls & 0xFFFF));
        out.emplace_back();
    }
    std::vector<const resources::VersionStringTable*> tables;
    for (const auto& t : v.strings) tables.push_back(&t);
    std::stable_sort(tables.begin(), tables.end(), [](const auto* a, const auto* b) { return a->language_block < b->language_block; });
    for (const auto* t : tables)
        for (const auto& [key, value] : t->entries) {
            const std::string_view shown = text::trim(value);
            if (!shown.empty()) out.push_back(std::format("{:<17}{}", key, shown));
        }
    const std::string joined = text::join(out, "\n");
    return joined.empty() ? "(no version fields could be read)" : joined;
}

// A string resource holds sixteen strings, each led by its length.
std::string string_table(ByteView data, bool wide) {
    std::vector<std::string> out;
    const std::vector<std::string> all = resources::strings(data, wide);
    for (std::size_t i = 0; i < all.size(); ++i) {
        if (all[i].empty()) continue;
        out.push_back(std::format("[{:2}] {}", i, text::replace_all(text::replace_all(all[i], "\r", " "), "\n", " ")));
    }
    const std::string joined = text::join(out, "\n");
    return joined.empty() ? "(no strings in this block)" : joined;
}

// An icon or cursor resource is one image of an icon file; wrap it as a file of one.
Bytes as_icon_file(ByteView image, bool cursor) {
    std::uint16_t hot_x = 0, hot_y = 0;
    if (cursor) {
        if (image.size() < 4) throw TruncatedError("icon", 0, 4, image.size());
        hot_x = static_cast<std::uint16_t>(image[0] | image[1] << 8);
        hot_y = static_cast<std::uint16_t>(image[2] | image[3] << 8);
        image = image.subspan(4);
    }
    std::uint32_t w = 0, h = 0, bits = 32;
    if (!(image.size() >= 4 && image[0] == 0x89 && image[1] == 'P' && image[2] == 'N' && image[3] == 'G')) {
        if (image.size() < 16) throw TruncatedError("icon", 4, 16, image.size());
        const auto u32 = [&](std::size_t at) { return std::uint32_t{image[at]} | image[at + 1] << 8 | image[at + 2] << 16 | std::uint32_t{image[at + 3]} << 24; };
        w = u32(4);
        h = u32(8) / 2;  // picture and mask are stacked
        bits = image[14] | image[15] << 8;
    }
    const std::uint16_t a = cursor ? hot_x : 1, b = cursor ? hot_y : static_cast<std::uint16_t>(bits);
    Bytes out;
    const auto put16 = [&](std::uint32_t v) { out.push_back(static_cast<std::uint8_t>(v)); out.push_back(static_cast<std::uint8_t>(v >> 8)); };
    const auto put32 = [&](std::uint32_t v) { put16(v & 0xFFFF); put16(v >> 16); };
    put16(0);
    put16(cursor ? 2 : 1);
    put16(1);
    out.push_back(static_cast<std::uint8_t>(w));
    out.push_back(static_cast<std::uint8_t>(h));
    out.push_back(0);
    out.push_back(0);
    put16(a);
    put16(b);
    put32(static_cast<std::uint32_t>(image.size()));
    put32(22);
    out.insert(out.end(), image.begin(), image.end());
    return out;
}

ResourceBody hex_body(std::string text_) {
    ResourceBody b;
    b.kind = ResourceBody::Kind::Hex;
    b.text = std::move(text_);
    return b;
}

ResourceBody text_body(std::string text_) {
    ResourceBody b;
    b.kind = ResourceBody::Kind::Text;
    b.text = std::move(text_);
    return b;
}

ResourceBody image_body(std::string content_type, Bytes bytes) {
    ResourceBody b;
    b.kind = ResourceBody::Kind::Image;
    b.content_type = std::move(content_type);
    b.bytes = std::move(bytes);
    return b;
}

// The body shown as its type; throws when the data is not of that type.
std::optional<ResourceBody> show_as_type(ByteView data, std::int32_t type_id, bool wide) {
    switch (type_id) {
        case 2:
            if (data.size() < 12) throw FormatError("resource", 0, "too short to be a picture");
            return image_body("image/bmp", resources::bitmap_file(data));
        case 1:
        case 3: return image_body("image/x-icon", as_icon_file(data, type_id == 1));
        case 16: return text_body(version_text(data, wide));
        case 6: return text_body(string_table(data, wide));
        case 23:
        case 24: return text_body(text::sanitize_utf8(as_text(data)));
        case 10: {  // raw data that is mostly readable text
            const ByteView head = data.first(std::min<std::size_t>(256, data.size()));
            std::size_t readable = 0;
            for (const std::uint8_t b : head)
                if ((b >= 9 && b <= 13) || (b >= 32 && b < 127)) ++readable;
            if (static_cast<double>(readable) > 0.85 * static_cast<double>(head.size())) return text_body(text::from_latin1(data));
            return std::nullopt;
        }
        default: return std::nullopt;
    }
}

std::string hex_listing(ByteView data) {
    std::vector<std::string> lines = hex_lines(data.first(std::min<std::size_t>(4096, data.size())));
    if (data.size() > 4096) lines.push_back(std::format("... {} more bytes", data.size() - 4096));
    return text::join(lines, "\n");
}

}  // namespace

ResourceBody resource_body(const std::filesystem::path& path, std::uint64_t offset, std::uint64_t size, std::int32_t type_id,
                           bool wide, std::uint64_t limit) {
    Bytes data;
    try {
        const fs::MappedFile file(path);
        const ByteView all = file.view();
        if (offset < all.size()) {
            const auto n = static_cast<std::size_t>(std::min({size, limit, std::uint64_t{all.size()} - offset}));
            data.assign(all.begin() + static_cast<std::ptrdiff_t>(offset), all.begin() + static_cast<std::ptrdiff_t>(offset + n));
        }
    } catch (const Error& e) {
        return hex_body(std::format("cannot be read: {}", e.what()));
    }
    if (data.empty()) return hex_body("(empty)");
    try {
        if (auto shown = show_as_type(data, type_id, wide)) return std::move(*shown);
    } catch (const Error& e) {  // damaged data of that type: the bytes are shown instead, with the reason
        return hex_body(std::format("could not be shown as its type ({})\n\n{}", e.what(),
                                    text::join(hex_lines(ByteView(data).first(std::min<std::size_t>(4096, data.size()))), "\n")));
    }
    return hex_body(hex_listing(data));
}

// ======================================================================================================================
// reports
// ======================================================================================================================
namespace {

// The keys whose integers are addresses and sizes of things in memory: shown in hex.
bool hex_key(std::string_view key) {
    for (const std::string_view part : {"address", "offset", "image_base", "code_base", "entry_point", "rva", "slot", "checksum",
                                         "flags_value", "module_flags", "magic", "machine_id", "device_id", "ddk_version"})
        if (key.find(part) != std::string_view::npos) return true;
    return key == "entry" || key == "key" || key.ends_with("_va");
}

// Characters (code points) in a text: columns of a listing are counted by characters (names from binaries hold
// control and high characters, which each take one column).
std::size_t char_count(std::string_view s) {
    std::size_t n = 0;
    for (const char c : s)
        if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) ++n;
    return n;
}

std::string ljust(std::string_view s, std::size_t width) {
    std::string out(s);
    const std::size_t have = char_count(s);
    if (have < width) out.append(width - have, ' ');
    return out;
}

bool is_record_list(const Json& v) { return v.is_array() && !v.empty() && v.front().is_object(); }

bool truthy(const Json& v) {
    if (v.is_null()) return false;
    if (v.is_boolean()) return v.get<bool>();
    if (v.is_number_integer()) return v.get<std::int64_t>() != 0 || (v.is_number_unsigned() && v.get<std::uint64_t>() != 0);
    if (v.is_string()) return !v.get_ref<const std::string&>().empty();
    return !v.empty();
}

}  // namespace

std::string cell(std::string_view key, const Json& value) {
    if (value.is_boolean()) return value.get<bool>() ? "yes" : "no";
    if (value.is_number_unsigned()) {
        const auto n = value.get<std::uint64_t>();
        return hex_key(key) ? std::format("0x{:x}", n) : std::to_string(n);
    }
    if (value.is_number_integer()) {
        const auto n = value.get<std::int64_t>();
        return hex_key(key) && n >= 0 ? std::format("0x{:x}", n) : std::to_string(n);
    }
    if (value.is_number_float()) return text::format_double(value.get<double>());
    if (value.is_array()) {
        std::vector<std::string> parts;
        for (const Json& v : value) parts.push_back(cell(key, v));
        return text::join(parts, " ");
    }
    if (value.is_null()) return "";
    if (value.is_string()) return text::replace_all(text::replace_all(value.get<std::string>(), "\r", "\\r"), "\n", "\\n");
    return value.dump();
}

std::vector<std::string> render(const Json& value, std::string_view key, int indent) {
    const std::string pad(static_cast<std::size_t>(indent) * 2, ' ');
    std::vector<std::string> lines;
    if (value.is_object()) {
        std::size_t width = 1;
        for (const auto& [k, v] : value.items()) width = std::max(width, char_count(k));
        for (const auto& [k, v] : value.items()) {
            if (v.is_object() || is_record_list(v)) {
                lines.push_back(pad + k + ":");
                for (std::string& l : render(v, k, indent + 1)) lines.push_back(std::move(l));
            } else {
                lines.push_back(pad + ljust(k, width) + "  " + cell(k, v));
            }
        }
        return lines;
    }
    if (is_record_list(value)) {
        // The columns are the first record's keys; a key is shown nested when ANY record holds records under it
        // (deciding by the first record alone printed later rows' records as one crowded cell).
        std::vector<std::string> plain, nested;
        for (const auto& [k, v] : value.front().items()) {
            bool holds_records = false;
            for (const Json& r : value)
                if (r.is_object() && r.contains(k) && (r[k].is_object() || is_record_list(r[k]))) holds_records = true;
            (holds_records ? nested : plain).push_back(k);
        }
        std::vector<std::vector<std::string>> rows;
        for (const Json& r : value) {
            std::vector<std::string> row;
            for (const std::string& k : plain) row.push_back(r.is_object() && r.contains(k) ? cell(k, r[k]) : "");
            rows.push_back(std::move(row));
        }
        std::vector<std::size_t> widths;
        for (std::size_t i = 0; i < plain.size(); ++i) {
            std::size_t w = char_count(plain[i]);
            for (const auto& row : rows) w = std::max(w, char_count(row[i]));
            widths.push_back(std::min<std::size_t>(48, w));
        }
        const auto join_row = [&](const std::vector<std::string>& cells) {
            std::string line = pad;
            for (std::size_t i = 0; i < cells.size(); ++i) {
                if (i) line += "  ";
                line += ljust(cells[i], widths[i]);
            }
            return std::string(text::rtrim(line));
        };
        lines.push_back(join_row(plain));
        std::size_t index = 0;
        for (const Json& r : value) {
            lines.push_back(join_row(rows[index++]));
            for (const std::string& k : nested)
                if (r.is_object() && r.contains(k))
                    for (std::string& l : render(r[k], k, indent + 2)) lines.push_back(std::move(l));
        }
        return lines;
    }
    if (value.is_array()) {
        for (const Json& v : value) lines.push_back(pad + cell(key, v));
        if (lines.empty()) lines.push_back(pad + "(none)");
        return lines;
    }
    lines.push_back(pad + cell(key, value));
    return lines;
}

std::string size_text(std::uint64_t bytes) {
    if (bytes < 1024) return std::format("{} B", bytes);
    double n = static_cast<double>(bytes);
    for (const char* unit : {"KB", "MB", "GB", "TB"}) {
        n /= 1024.0;
        if (n < 1024 || std::string_view(unit) == "TB") return std::format("{:.1f} {}", n, unit);
    }
    return "";
}

namespace {

std::string markdown_report(const Json& info, const Facts& xex) {
    std::vector<std::string> md;
    const std::string format = info["format"].is_null() ? "not recognised" : info["format"].get<std::string>();
    md.push_back("# " + info["name"].get<std::string>());
    md.emplace_back();
    md.push_back(std::format("{}, {}.", format, size_text(info["size"].get<std::uint64_t>())));
    md.emplace_back();
    if (!xex.empty()) {
        md.insert(md.end(), {"## Xbox 360 executable", "", "| Field | Value |", "|---|---|"});
        for (const auto& [k, v] : xex) md.push_back(std::format("| {} | {} |", k, v));
        md.emplace_back();
    }
    const Json& summary = info["summary"];
    if (!summary.empty()) {
        md.insert(md.end(), {"## Summary", "", "| Field | Value |", "|---|---|"});
        for (const auto& [k, v] : summary.items()) md.push_back(std::format("| {} | {} |", k, cell(k, v)));
        md.emplace_back();
    }
    const Json& sections = info["sections"];
    if (!sections.empty()) {
        md.insert(md.end(), {"## Sections", "", "| Name | Address | Size | Flags |", "|---|---|---:|---|"});
        for (const Json& s : sections)
            md.push_back(std::format("| {} | {} | {} | {} |", s.value("name", ""), cell("address", s["address"]), cell("size", s["size"]),
                                     cell("flags", s["flags"])));
        md.emplace_back();
    }
    const Json& imports = info["imports"];
    if (!imports.empty()) {
        md.insert(md.end(), {"## Imports", "", "| Module | Functions |", "|---|---:|"});
        for (const Json& i : imports) md.push_back(std::format("| {} | {} |", i.value("module", ""), i.value("count", std::uint64_t{0})));
        md.emplace_back();
    }
    if (info["exports"].value("count", std::uint64_t{0}) > 0) {
        md.push_back(std::format("- **Exports:** {}", info["exports"]["count"].get<std::uint64_t>()));
        md.emplace_back();
    }
    for (const Json& n : info["notes"]) md.push_back("> " + n.get<std::string>());
    return text::join(md, "\n") + "\n";
}

}  // namespace

namespace {

std::string markdown_folder(const Json& m) {
    const std::string folder = m["folder"].get<std::string>();
    std::vector<std::string> md = {"# " + folder, "",
                                   std::format("{} files, {}. Recognised:", m["files"].get<std::uint64_t>(), size_text(m["bytes"].get<std::uint64_t>())), "",
                                   "| Format | Platform | Files | Size | Read with |", "|---|---|---:|---:|---|"};
    for (const Json& r : m["formats"]) {
        const std::string command = r["command"].get<std::string>();
        md.push_back(std::format("| {} | {} | {} | {} | {} |", r["format"].get<std::string>(), r["family"].get<std::string>(),
                                 r["files"].get<std::uint64_t>(), size_text(r["bytes"].get<std::uint64_t>()),
                                 command.empty() ? "-" : "`codebreaker " + command + "`"));
    }
    const Json& others = m["unrecognised"];
    if (!others.empty()) {
        md.insert(md.end(), {"", "Not recognised, by extension:", "", "| Extension | Files | Size | Example |", "|---|---:|---:|---|"});
        std::size_t shown = 0;
        for (const Json& r : others) {
            if (shown++ == 40) break;
            const std::string example = r["example"].get<std::string>();
            std::error_code ec;
            const std::string rel = fs::to_text(std::filesystem::relative(fs::from_text(example), fs::from_text(folder), ec));
            md.push_back(std::format("| `{}` | {} | {} | `{}` |", r["extension"].get<std::string>(), r["files"].get<std::uint64_t>(),
                                     size_text(r["bytes"].get<std::uint64_t>()), ec ? example : rel));
        }
    }
    return text::join(md, "\n") + "\n";
}

}  // namespace

std::string markdown(const Json& document, const Facts& xex) {
    return document.contains("formats") ? markdown_folder(document) : markdown_report(document, xex);
}
std::string markdown(const Description& d) { return markdown_report(to_json(d), d.xex); }
std::string markdown(const FolderMap& m) { return markdown_folder(to_json(m)); }

// ======================================================================================================================
// the `info` command
// ======================================================================================================================
namespace {

std::string shortened(std::string_view s, std::size_t columns) { return std::string(text::utf8_prefix(s, columns)); }

// The first `count` characters (code points) of the text: a listing line is cut by characters, whatever their width
// (a tab or other control character in a string still counts as one).
std::string first_code_points(std::string_view s, std::size_t count) {
    std::size_t i = 0, seen = 0;
    while (i < s.size() && seen < count) {
        ++i;
        while (i < s.size() && (static_cast<unsigned char>(s[i]) & 0xC0) == 0x80) ++i;
        ++seen;
    }
    return std::string(s.substr(0, i));
}

struct InfoRun {
    const cli::Args& args;
    const InfoEnvironment& env;

    void emit(const std::string& text_) const {
        if (const auto out = args.value("out")) {
            fs::write_text(fs::from_text(*out), text_);
            cb::out("wrote {}\n", *out);
        } else {
            cb::out("{}\n", text_);
        }
    }

    int folder(const std::filesystem::path& dir) const {
        const FolderMap m = folder_map(dir, [](std::string_view line) { cb::out("{}\n", line); });
        if (args.flag("json")) {
            emit(json::dump(to_json(m), json::Style::Pretty));
        } else if (args.flag("markdown")) {
            emit(markdown(m));
        } else {
            std::vector<std::string> lines = {std::format("{}: {} files, {}", m.folder, m.files, size_text(m.bytes))};
            for (const FolderFormat& r : m.formats)
                lines.push_back(std::format("  {:<46} {:<9} {:>7} files {:>10}   {}", shortened(r.format, 46), r.family, r.files,
                                            size_text(r.bytes), r.command.empty() ? "" : "codebreaker " + r.command));
            if (!m.unrecognised.empty()) {
                std::vector<std::string> parts;
                for (std::size_t i = 0; i < m.unrecognised.size() && i < 25; ++i)
                    parts.push_back(std::format("{} {}", m.unrecognised[i].extension, m.unrecognised[i].files));
                lines.push_back("  not recognised: " + text::join(parts, ", "));
            }
            emit(text::join(lines, "\n"));
        }
        return 0;
    }

    int unpack(const std::filesystem::path& path, const std::string& out_file) const {
        const Bytes data = fs::read_file(path);
        const std::string name = fs::to_text(path.filename());
        const dos::Program p = dos::read_program(data, name);
        if (!p.unpacked) {
            cb::out("{} is not packed{}\n", name, p.packer ? " readably (" + text::join(p.notes, "; ") + ")" : "");
            return 1;
        }
        fs::write_file(fs::from_text(out_file), dos::unpacked_file(p, name));
        cb::out("{} ({}, unpacked {}) -> {}\n", name, dos::packer_name(*p.packer),
                *p.unpacked == dos::Unpacked::Static ? "static" : "emulated", out_file);
        return 0;
    }

    int strings_view(const std::filesystem::path& path) const {
        StringOptions o;
        o.min_length = args.integer<std::size_t>("min").value_or(4);
        const std::vector<StringRow> rows = strings(path, o);
        if (args.flag("json")) {
            cb::out("{}\n", json::dump(to_json(rows), json::Style::Compact));
            return 0;
        }
        for (const StringRow& r : rows) {
            std::string where;
            if (r.address) where = std::holds_alternative<std::uint64_t>(*r.address) ? cell("address", std::get<std::uint64_t>(*r.address)) : address_text(*r.address);
            cb::out("{:08x}  {:<10} {:<5} {:<8} {}\n", r.offset, where, string_kind_name(r.kind), string_category_name(r.category),
                    first_code_points(cell("", r.text), 120));
        }
        return 0;
    }

    int run() const {
        const std::string target = args.required("target");
        const std::filesystem::path as_path = fs::from_text(target);
        std::error_code ec;
        if (std::filesystem::is_directory(as_path, ec)) return folder(as_path);
        const Facts xex_head = std::filesystem::is_regular_file(as_path, ec) ? xex_facts(as_path) : Facts{};
        std::optional<std::filesystem::path> path = env.resolve ? env.resolve(target) : std::nullopt;
        if (!path && std::filesystem::exists(as_path, ec)) path = as_path;
        if (!path) {
            cb::out("no file or corpus module '{}'\n", target);
            return 2;
        }
        if (const auto out_file = args.value("unpack")) return unpack(*path, *out_file);

        const std::optional<std::string> section = args.value("section");
        const bool as_json = args.flag("json");
        if (section == "hex") {
            const std::string text_ = hexdump(*path, args.integer<std::uint64_t>("offset").value_or(0),
                                              args.integer<std::uint64_t>("length").value_or(256));
            if (as_json) {
                Json j = Json::object();
                j["hex"] = text_;
                cb::out("{}\n", json::dump(j, json::Style::Compact));
            } else {
                cb::out("{}\n", text_);
            }
            return 0;
        }
        if (section == "strings") return strings_view(*path);

        Description d = describe(*path, env.hooks);
        d.xex = xex_head;
        if (args.flag("markdown")) {
            emit(markdown(d));
            return d.ok ? 0 : 1;
        }
        const Json info = to_json(d);
        if (section && *section != "all" && !info.contains(*section)) {
            std::vector<std::string_view> have;
            for (const std::string_view k : kSections)
                if ((info.contains(std::string(k)) && truthy(info[std::string(k)])) || k == "strings" || k == "hex") have.push_back(k);
            cb::out("{} has no part '{}' (it has: {})\n", d.name, *section, text::join(have, " "));
            return 2;
        }
        const bool whole = !section || *section == "all";
        if (as_json) {
            cb::out("{}\n", json::dump(whole ? info : info[*section], json::Style::Pretty));
        } else if (!whole) {
            cb::out("{}\n", text::join(render(info[*section], *section), "\n"));
        } else {
            cb::out("{}  ({}, {} bytes)\n", d.path, d.format.value_or("not an executable"), d.size);
            std::vector<std::string> parts;
            if (!section) {
                parts = {"xex", "summary", "headers", "sections"};
            } else {
                for (const auto& [k, v] : info.items())
                    if ((v.is_object() || v.is_array()) && !v.empty() && k != "notes") parts.push_back(k);
            }
            for (const std::string& k : parts)
                if (info.contains(k) && truthy(info[k])) {
                    cb::out("\n{}:\n", k);
                    cb::out("{}\n", text::join(render(info[k], k, 1), "\n"));
                }
            if (!section && d.ok && info["summary"].contains("exports")) {
                const Json& s = info["summary"];
                cb::out("\n{} exports, {} imports from {} modules, {} resources  (--section exports | imports | resources | all)\n",
                        s["exports"].get<std::uint64_t>(), s["imports"].get<std::uint64_t>(), s["import_modules"].get<std::uint64_t>(),
                        s["resources"].get<std::uint64_t>());
            }
            for (const std::string& n : d.notes) cb::out("note: {}\n", n);
        }
        return d.ok ? 0 : 1;
    }
};

}  // namespace

cli::Command info_command(InfoEnvironment env) {
    cli::Command c;
    c.name = "info";
    c.summary = "show what a file is and holds";
    c.area = "Inspect";
    c.description =
        "Show what a file is and holds: executables in full (--section is one of: summary dos headers directories sections\n"
        "exports imports resources relocations tls debug certificates clr rich driver members strings hex, or `all`), and\n"
        "any other file by its format -- Xbox containers and assets, disc and disk images, objects, bytecode. A folder is\n"
        "mapped: every file in it identified, tallied by format. --unpack writes a packed DOS program (EXEPACK, PKLITE,\n"
        "...) back out unpacked, as a plain MZ file that runs the same way.";
    c.params = {cli::Positional{"target", "file, corpus module name, or folder"},
                cli::Option{"--json", "", "print the description (or the part) as JSON", cli::Arg::Flag, "", ""},
                cli::Option{"--section", "", "show one part", cli::Arg::Value, "NAME", ""},
                cli::Option{"--offset", "", "with --section hex: where to start", cli::Arg::Value, "N", ""},
                cli::Option{"--length", "", "with --section hex: how many bytes", cli::Arg::Value, "N", ""},
                cli::Option{"--min", "", "with --section strings: the shortest run", cli::Arg::Value, "N", ""},
                cli::Option{"--markdown", "", "a one-page markdown report", cli::Arg::Flag, "", ""},
                cli::Option{"--out", "", "write the report to this file", cli::Arg::Value, "FILE", ""},
                cli::Option{"--unpack", "", "write a packed DOS program back out unpacked", cli::Arg::Value, "OUT.EXE", ""}};
    c.handler = [env = std::move(env)](const cli::Args& args) { return InfoRun{args, env}.run(); };
    return c;
}

}  // namespace cb::formats::inspect
