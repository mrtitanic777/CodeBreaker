// source/console_writer.cpp -- the one source writer for the consoles (console_writer.hpp).

#include "source/console_writer.hpp"

#include <format>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>

#include "base/config.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "knowledge/package.hpp"
#include "source/console_shapes.hpp"
#include "source/z80_writer.hpp"

namespace cb::source::console_writer {

namespace {

std::string text_of(const nlohmann::ordered_json& j, const char* key) {
    const auto it = j.find(key);
    return it != j.end() && it->is_string() ? it->get<std::string>() : std::string();
}

// A number in a description: a JSON number, or a text in hex ("0x0104", "$0104") or decimal.
std::uint32_t number_of(const nlohmann::ordered_json& j) {
    if (j.is_number_unsigned() || j.is_number_integer()) return j.get<std::uint32_t>();
    const std::string s = j.get<std::string>();
    if (s.starts_with("0x") || s.starts_with("0X")) return static_cast<std::uint32_t>(std::stoul(s.substr(2), nullptr, 16));
    if (s.starts_with("$")) return static_cast<std::uint32_t>(std::stoul(s.substr(1), nullptr, 16));
    return static_cast<std::uint32_t>(std::stoul(s));
}

std::uint32_t number_of(const nlohmann::ordered_json& j, const char* key, std::uint32_t fallback) {
    const auto it = j.find(key);
    return it == j.end() ? fallback : number_of(*it);
}

std::vector<std::string> texts_of(const nlohmann::ordered_json& j, const char* key) {
    std::vector<std::string> out;
    if (const auto it = j.find(key); it != j.end()) {
        for (const auto& s : *it) out.push_back(s.get<std::string>());
    }
    return out;
}

std::vector<HardwareTable> tables_of(const nlohmann::ordered_json& j) {
    std::vector<HardwareTable> out;
    if (const auto it = j.find("hardware"); it != j.end()) {
        for (const auto& t : *it) {
            out.push_back({text_of(t, "section"), text_of(t, "spelling"), text_of(t, "as") == "port", number_of(t, "digits", text_of(t, "as") == "port" ? 2 : 4)});
        }
    }
    return out;
}

CartShape cart_of(const nlohmann::ordered_json& j) {
    CartShape c;
    c.ext = text_of(j, "ext");
    c.prologue = text_of(j, "prologue");
    c.bank_block = text_of(j, "bank_block");
    if (j.contains("part")) c.part = text_of(j, "part");
    c.part_single = text_of(j, "part_single");
    c.header_part = text_of(j, "header_part");
    c.header_what = text_of(j, "header_what");
    c.tail_last_only = j.value("tail_last_only", false);
    c.tail = number_of(j, "tail", 0);
    c.tail_lines = texts_of(j, "tail_lines");
    c.equ_digits = number_of(j, "equ_digits", 0);
    c.data_floor = number_of(j, "data_floor", 0);
    c.whole = j.value("whole", false);
    if (const auto it = j.find("chips"); it != j.end()) {
        for (const auto& chip : *it) c.chips.push_back({text_of(chip, "read"), text_of(chip, "write")});
    }
    return c;
}

Segmented segmented_of(const nlohmann::ordered_json& j) {
    Segmented s;
    s.cpu = text_of(j, "cpu");
    s.assembler = text_of(j, "assembler");
    if (j.contains("source")) s.source = text_of(j, "source");
    s.built = text_of(j, "built");
    s.prologue = texts_of(j, "prologue");
    s.whole = j.value("whole", false);
    s.registers = text_of(j, "registers");
    s.data_floor = number_of(j, "data_floor", 0);
    s.address_digits = number_of(j, "address_digits", 4);
    if (const auto tail = j.find("tail"); tail != j.end()) {
        s.tail_at = number_of(*tail, "at", 0);
        s.tail = number_of(*tail, "size", 0);
        s.tail_lines = texts_of(*tail, "lines");
    }
    if (const auto roles = j.find("roles"); roles != j.end()) {
        for (const auto& [name, r] : roles->items()) {
            SegmentRole role{name, r.value("as", std::string("rows")), text_of(r, "file"), text_of(r, "note"), text_of(r, "what"), r.value("neutral", false)};
            s.roles.push_back(std::move(role));
        }
    }
    return s;
}

Vectored vectored_of(const nlohmann::ordered_json& j) {
    Vectored v;
    v.cpu = text_of(j, "cpu");
    v.assembler = text_of(j, "assembler");
    if (j.contains("source")) v.source = text_of(j, "source");
    v.built = text_of(j, "built");
    v.whole = j.value("whole", false);
    v.neutral_rows = j.value("neutral_rows", false);
    v.registers = text_of(j, "registers");
    v.immediate_floor = number_of(j, "immediate_floor", 0);
    v.calls_start_functions = j.value("calls_start_functions", false);
    v.org_digits = number_of(j, "org_digits", 6);
    v.blank_after_piece = j.value("blank_after_piece", false);
    v.equ_width = number_of(j, "equ_width", 0);
    if (const auto fields = j.find("fields"); fields != j.end()) {
        for (const auto& f : *fields) {
            v.fields.push_back({number_of(f.at("at")), number_of(f.at("size")), text_of(f, "what"), f.value("as", std::string("text")), text_of(f, "when"), number_of(f, "row", 12)});
        }
    }
    v.vector_names = text_of(j, "vector_names");
    v.vectors_plain = number_of(j, "vectors_plain", 0);
    v.prologue = texts_of(j, "prologue");
    v.big_endian = text_of(j, "endian") == "big";
    v.bare_region = number_of(j, "bare_region", 0);
    v.other_suffix = text_of(j, "other_suffix");
    v.ram_size = number_of(j, "ram_size", 0);
    if (const auto vars = j.find("variables"); vars != j.end()) {
        for (const auto& r : *vars) v.variables.push_back({number_of(r.at(0)), number_of(r.at(1))});
    }
    if (j.contains("built_what")) v.built_what = text_of(j, "built_what");
    v.block = text_of(j, "block");
    v.block_cpu = text_of(j, "block_cpu");
    return v;
}

Banked banked_of(const nlohmann::ordered_json& j) {
    Banked b;
    b.cpu = text_of(j, "cpu");
    b.assembler = text_of(j, "assembler");
    b.bank_size = number_of(j, "bank_size", 0x4000);
    if (const auto it = j.find("slots"); it != j.end()) {
        for (const auto& s : *it) b.slots.push_back(number_of(s));
    }
    if (b.slots.empty()) b.slots.push_back(0);
    b.power_on = number_of(j, "power_on", 0x8000);
    b.data_floor = number_of(j, "data_floor", 0);
    if (const auto it = j.find("data"); it != j.end()) {
        for (const auto& r : *it) b.data.push_back({number_of(r.at(0)), number_of(r.at(1))});
    }
    if (const auto it = j.find("header_fields"); it != j.end()) {
        for (const auto& f : *it) b.header_fields.push_back({number_of(f.at("at")), number_of(f.at("size")), text_of(f, "what")});
    }
    if (const auto it = j.find("vectors"); it != j.end()) {
        for (const auto& v : *it) b.vectors.push_back({number_of(v.at("at")), text_of(v, "name"), v.value("always", false)});
    }
    b.no_data_labels = texts_of(j, "no_data_labels");
    b.hardware = tables_of(j);
    b.prologue = text_of(j, "prologue");
    b.bank_blocks = texts_of(j, "bank_blocks");
    if (b.bank_blocks.empty()) b.bank_blocks.push_back("bank");
    if (const auto it = j.find("variants"); it != j.end()) {
        b.variant_by = text_of(*it, "by");
        if (const auto each = it->find("each"); each != it->end()) {
            for (const auto& [when, v] : each->items()) {
                Variant variant{when, tables_of(v), {}, {}, v.value("constants", true)};
                if (const auto drop = v.find("drop_ports"); drop != v.end()) {
                    for (const auto& p : *drop) variant.drop_ports.push_back(number_of(p));
                }
                if (const auto add = v.find("add_ports"); add != v.end()) {
                    for (const auto& [name, at] : add->items()) variant.add_ports[number_of(at)] = name;
                }
                b.variants.push_back(std::move(variant));
            }
        }
    }
    return b;
}

Description read_description(std::string_view name) {
    const std::filesystem::path file = config::current().config_root() / "platforms" / (std::string(name) + ".json");
    if (!std::filesystem::exists(file)) throw UsageError(std::format("no console platform {} (config/platforms/{}.json)", name, name));
    const nlohmann::ordered_json j = nlohmann::ordered_json::parse(fs::read_text(file));
    Description d;
    d.name = std::string(name);
    d.platform = text_of(j, "platform");
    if (d.platform.empty()) d.platform = d.name;
    d.title = text_of(j, "title");
    d.shape = text_of(j, "shape");
    d.dialect = text_of(j, "dialect");
    d.ports = text_of(j, "ports");
    d.fixed = text_of(j, "fixed");
    if (const auto head = j.find("head"); head != j.end()) {
        for (const auto& line : *head) {
            if (line.is_string()) {
                d.head.push_back({line.get<std::string>(), {}, {}});
            } else {
                d.head.push_back({text_of(line, "line"), text_of(line, "when"), text_of(line, "each")});
            }
        }
    }
    if (const auto banked = j.find("banked"); banked != j.end()) d.banked = banked_of(*banked);
    if (const auto cart = j.find("cart"); cart != j.end()) d.cart = cart_of(*cart);
    if (const auto segmented = j.find("segmented"); segmented != j.end()) d.segmented = segmented_of(*segmented);
    if (const auto vectored = j.find("vectored"); vectored != j.end()) d.vectored = vectored_of(*vectored);
    return d;
}

const Value& value_of(const Values& values, std::string_view name) {
    const auto it = values.find(name);
    if (it == values.end()) throw UsageError(std::format("a head template names {}, which the loader does not give", name));
    return it->second;
}

bool is_set(const Value& v) {
    if (const auto* n = std::get_if<std::int64_t>(&v)) return *n != 0;
    if (const auto* s = std::get_if<std::string>(&v)) return !s->empty();
    return !std::get<std::vector<std::string>>(v).empty();
}

// The position of the first `c` in `text` outside braces, npos when there is none.
std::size_t top_level(std::string_view text, char c) {
    int depth = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '{') ++depth;
        else if (text[i] == '}') --depth;
        else if (depth == 0 && text[i] == c) return i;
    }
    return std::string_view::npos;
}

std::string placeholder(std::string_view inner, const Values& values) {
    if (const std::size_t q = top_level(inner, '?'); q != std::string_view::npos) {
        const std::string_view branches = inner.substr(q + 1);
        const std::size_t bar = top_level(branches, '|');
        const std::string_view yes = branches.substr(0, bar);
        const std::string_view no = bar == std::string_view::npos ? std::string_view() : branches.substr(bar + 1);
        return fill(is_set(value_of(values, inner.substr(0, q))) ? yes : no, values);
    }
    const std::size_t colon = inner.find(':');
    const std::string_view name = inner.substr(0, colon);
    const std::string spec = colon == std::string_view::npos ? std::string("{}") : "{:" + std::string(inner.substr(colon + 1)) + "}";
    const Value& v = value_of(values, name);
    if (const auto* n = std::get_if<std::int64_t>(&v)) return std::vformat(spec, std::make_format_args(*n));
    if (const auto* s = std::get_if<std::string>(&v)) return std::vformat(spec, std::make_format_args(*s));
    throw UsageError(std::format("{} is a list: a head line says it with `each`", name));
}

// The head comment's lines the description's templates make.
std::vector<std::string> head_lines(const Description& d, const Values& values) {
    std::vector<std::string> out;
    for (const HeadLine& line : d.head) {
        if (!line.when.empty() && !is_set(value_of(values, line.when))) continue;
        if (line.each.empty()) {
            out.push_back(fill(line.text, values));
            continue;
        }
        const Value& list = value_of(values, line.each);
        if (!std::holds_alternative<std::vector<std::string>>(list)) throw UsageError(std::format("`each` names {}, which is not a list", line.each));
        Values with_it = values;
        for (const std::string& it : std::get<std::vector<std::string>>(list)) {
            with_it.insert_or_assign("it", it);
            out.push_back(fill(line.text, with_it));
        }
    }
    return out;
}

// The z80-block shape: one block at one address (source/z80_writer), with the platform's ports and fixed addresses.
image_writer::Written write_z80_block(const Description& d, const Loaded& program, const std::vector<std::string>& head, std::string_view stem,
                                      const std::filesystem::path& folder) {
    z80_writer::Machine machine{.ports = d.ports.empty() ? image_writer::Names{} : knowledge::standard().hardware_names(d.platform, d.ports),
                                .names = d.fixed.empty() ? image_writer::Names{} : knowledge::standard().table_names(d.platform, d.fixed),
                                .dialect = d.dialect.empty() ? std::string("sjasmplus") : d.dialect};
    const z80_writer::Block block{.image = program.image, .origin = program.origin, .entries = program.entries, .header = head};
    return z80_writer::write(block, machine, folder, stem);
}

}  // namespace

const Description& describe(std::string_view name) {
    static std::mutex lock;
    static std::map<std::string, std::unique_ptr<Description>, std::less<>> cache;
    std::lock_guard hold(lock);
    if (const auto it = cache.find(name); it != cache.end()) return *it->second;
    auto made = std::make_unique<Description>(read_description(name));
    const Description& ref = *made;
    cache.emplace(std::string(name), std::move(made));
    return ref;
}

std::string fill(std::string_view text, const Values& values) {
    std::string out;
    out.reserve(text.size());
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] != '{') {
            out += text[i];
            continue;
        }
        int depth = 0;
        std::size_t close = i;
        for (; close < text.size(); ++close) {
            if (text[close] == '{') ++depth;
            else if (text[close] == '}' && --depth == 0) break;
        }
        if (close == text.size()) throw UsageError(std::format("a head template has an unclosed brace: {}", text));
        out += placeholder(text.substr(i + 1, close - i - 1), values);
        i = close;
    }
    return out;
}

std::vector<std::string> head_comment(const Description& d, const Values& values) {
    std::vector<std::string> comment;   // (an empty template an empty line)
    for (const std::string& line : head_lines(d, values)) comment.push_back(line.empty() ? std::string() : "; " + line);
    return comment;
}

image_writer::Written write_shape(const Description& description, const Loaded& program, Values values, std::string_view file_name,
                                  const std::filesystem::path& folder);

image_writer::Written write(const Description& description, const Loaded& program, std::string_view file_name, const std::filesystem::path& folder) {
    Values values = program.values;
    values.try_emplace("file", std::string(file_name));
    image_writer::Written r = write_shape(description, program, values, file_name, folder);
    if (program.segments.after) program.segments.after(r, values, folder);
    return r;
}

image_writer::Written write_shape(const Description& description, const Loaded& program, Values values, std::string_view file_name,
                                  const std::filesystem::path& folder) {
    values.try_emplace("file", std::string(file_name));   // (a loader may name the program otherwise: a disc's boot file)
    values.insert_or_assign("size", static_cast<std::int64_t>(program.image.size()));
    values.insert_or_assign("kb", static_cast<std::int64_t>(program.image.size() / 1024));
    values.insert_or_assign("origin", static_cast<std::int64_t>(program.origin));
    values.try_emplace("stem", std::filesystem::path(file_name).stem().string());
    const std::string stem = std::get<std::string>(values.at("stem"));   // (a loader may name the source otherwise: the Neo Geo's `<name>.m1`)
    if (description.shape == "vectored" && description.vectored.cpu == "arm") return arm_code::write(description, program, values, folder);   // (their heads name what flow found)
    if (description.shape == "vectored" && description.vectored.cpu == "v810") return v810_code::write(description, program, values, folder);
    if (description.shape == "vectored" && description.vectored.cpu != "m68k") return mips_code::write(description, program, values, folder);
    const std::vector<std::string> head = head_lines(description, values);
    if (description.shape == "z80-block") return write_z80_block(description, program, head, stem, folder);
    std::vector<std::string> comment;   // (the shapes below write the head as comment lines, an empty template an empty line)
    for (const std::string& line : head) comment.push_back(line.empty() ? std::string() : "; " + line);
    if (description.shape == "banked") return write_banked(description, program, comment, values, file_name, folder);
    if (description.shape == "cart6502") return write_cart6502(description, program, comment, file_name, folder);
    if (description.shape == "segmented") return write_segmented(description, program, comment, values, file_name, folder);
    if (description.shape == "vectored") return write_vectored(description, program, comment, values, file_name, folder);
    throw UsageError(std::format("console platform {} names the shape {}, which the writer does not have", description.name, description.shape));
}

image_writer::Written reconstruct(std::string_view platform, const std::filesystem::path& file, const std::filesystem::path& folder) {
    const Description& d = describe(platform);
    return write(d, load(platform, file), file.filename().string(), folder);
}

}  // namespace cb::source::console_writer
