#include "cb/build/m6502_asm.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::build::m6502_asm {

namespace {

using Path = std::filesystem::path;

// A number as ca65 writes it: $1F hexadecimal, %0101 binary, else decimal or 0x/0b/0o.
std::optional<std::int64_t> ca65_number(std::string_view s) {
    s = text::trim(s);
    if (s.starts_with('$')) return text::parse_int<std::int64_t>(s.substr(1), 16);
    if (s.starts_with('%')) return text::parse_int<std::int64_t>(s.substr(1), 2);
    return parse_number(s);
}

class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};
[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

std::int64_t number_or_fail(std::string_view s) {
    const auto v = ca65_number(s);
    if (!v) fail(std::format("'{}' is not a number", s));
    return *v;
}

// `key=value, key=value` of a MEMORY or SEGMENTS entry, white space ignored.
std::unordered_map<std::string, std::string> attributes(std::string_view body) {
    std::string squeezed;
    for (const char c : body) {
        if (!text::is_space(c)) squeezed.push_back(c);
    }
    std::unordered_map<std::string, std::string> attrs;
    for (const std::string_view a : text::split(squeezed, ',')) {
        const std::size_t eq = a.find('=');
        if (eq != std::string_view::npos) attrs[std::string(a.substr(0, eq))] = std::string(a.substr(eq + 1));
    }
    return attrs;
}

// An unsigned 32-bit configuration value; a missing key is `fallback`.
std::uint32_t config_value(const std::unordered_map<std::string, std::string>& attrs, const char* key,
                           std::string_view area) {
    const auto it = attrs.find(key);
    if (it == attrs.end()) return 0;
    const auto v = ca65_number(it->second);
    if (!v || *v < 0 || *v > 0xFFFFFFFF) {
        throw AsmError(std::format("{}: {}={} is not a number the linker configuration can hold", area, key, it->second));
    }
    return static_cast<std::uint32_t>(*v);
}

// `name: attributes;` entries of a block body.
template <class Entry>
void for_each_entry(std::string_view body, Entry&& entry) {
    static const Regex re(R"((\w+)\s*:\s*([^;]*);)");
    re.for_each_match(body, [&](const RegexMatch& m) {
        entry(m.group(1), attributes(m.group(2)));
        return true;
    });
}

// ---- one assembly pass -----------------------------------------------------------------------------------------

// Where the bytes of each area are collected during a pass.
struct Area {
    const MemoryArea* spec = nullptr;
    Bytes body;
};

class Pass {
public:
    Pass(const Encoder6502& isa, const LinkConfig& config, Labels& labels, bool final, const Path& folder)
        : isa_(isa), config_(config), labels_(labels), final_(final), folder_(folder) {
        for (const MemoryArea& a : config.areas) areas_.push_back({&a, {}});
    }

    void line(std::string_view text) {
        if (const auto label = split_label(text)) {
            if (!current_) fail("a label outside any segment");
            labels_[std::string(label->first)] = address();
            text = label->second;
            if (text.empty()) return;
        }
        if (const auto constant = split_constant(text)) {
            labels_[std::string(constant->first)] = number_or_fail(constant->second);
            return;
        }
        if (text.starts_with(".segment")) return segment(text);
        if (!current_) fail("code outside any segment");
        Bytes& out = areas_[*current_].body;
        if (text.starts_with(".byt")) {   // .byte and .byt
            for (const std::string_view a : operands(text)) {
                if (a.starts_with('"')) {
                    if (a.size() < 2 || !a.ends_with('"')) fail("a string without its closing quote");
                    const auto bytes = as_bytes(a.substr(1, a.size() - 2));
                    out.insert(out.end(), bytes.begin(), bytes.end());
                } else {
                    out.push_back(static_cast<std::uint8_t>(number_or_fail(a) & 0xFF));
                }
            }
        } else if (text.starts_with(".word") || text.starts_with(".addr")) {
            for (const std::string_view a : operands(text)) {
                std::int64_t v = 0;
                if (text::is_identifier(a)) {
                    const auto it = labels_.find(std::string(a));
                    if (it != labels_.end()) v = it->second;
                    else if (final_) fail(std::format("unknown label {}", a));
                } else {
                    v = number_or_fail(a);
                }
                out.push_back(static_cast<std::uint8_t>(v & 0xFF));
                out.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
            }
        } else if (text.starts_with(".incbin")) {
            static const Regex quoted(R"re("([^"]+)")re");
            const auto m = quoted.search(text);
            if (!m) fail(".incbin without a file name in quotes");
            const Bytes data = fs::read_file(folder_ / std::string(m->group(1)));
            out.insert(out.end(), data.begin(), data.end());
        } else if (text.starts_with('.')) {
            fail("directive not understood");
        } else {
            instruction(text, out);
        }
    }

    // The linked image: the areas in order, each filled to its size when it says so.
    Bytes image() const {
        Bytes out;
        for (const Area& a : areas_) {
            if (a.body.size() > a.spec->size) {
                throw AsmError(std::format("{} holds {} bytes, more than its {}", a.spec->name, a.body.size(), a.spec->size));
            }
            out.insert(out.end(), a.body.begin(), a.body.end());
            if (a.spec->fill) out.resize(out.size() + (a.spec->size - a.body.size()), 0);
        }
        return out;
    }

private:
    std::uint32_t address() const {
        return static_cast<std::uint32_t>(areas_[*current_].spec->start + areas_[*current_].body.size());
    }

    // `name:` at the start of a line, and what follows it.
    static std::optional<std::pair<std::string_view, std::string_view>> split_label(std::string_view line) {
        if (line.empty() || !(std::isalpha(static_cast<unsigned char>(line[0])) || line[0] == '_')) return std::nullopt;
        std::size_t i = 1;
        while (i < line.size() && (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_')) ++i;
        if (i >= line.size() || line[i] != ':') return std::nullopt;
        return std::pair{line.substr(0, i), text::trim(line.substr(i + 1))};
    }

    // `NAME = value` with a single-word value.
    static std::optional<std::pair<std::string_view, std::string_view>> split_constant(std::string_view line) {
        if (line.empty() || !(std::isalpha(static_cast<unsigned char>(line[0])) || line[0] == '_')) return std::nullopt;
        std::size_t i = 1;
        while (i < line.size() && (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_')) ++i;
        const std::string_view rest = text::ltrim(line.substr(i));
        if (rest.empty() || rest[0] != '=') return std::nullopt;
        const std::string_view value = text::trim(rest.substr(1));
        if (value.empty() || value.find_first_of(" \t") != std::string_view::npos) return std::nullopt;
        return std::pair{line.substr(0, i), value};
    }

    // The operands of a data directive: everything after its first word.
    static std::vector<std::string_view> operands(std::string_view line) {
        std::size_t i = 0;
        while (i < line.size() && !text::is_space(line[i])) ++i;
        const std::string_view rest = text::ltrim(line.substr(i));
        if (rest.empty()) fail("a directive without operands");
        return split_items(rest, false);
    }

    void segment(std::string_view text) {
        static const Regex quoted(R"re("(\w+)")re");
        const auto m = quoted.search(text);
        if (!m) fail("a .segment without a name in quotes");
        const auto load = config_.segment_area.find(std::string(m->group(1)));
        if (load == config_.segment_area.end()) fail(std::format("segment {} is not in the configuration", m->group(1)));
        for (std::size_t i = 0; i < areas_.size(); ++i) {
            if (areas_[i].spec->name == load->second) {
                current_ = i;
                return;
            }
        }
        fail(std::format("segment {} loads into {}, which is not a MEMORY area", m->group(1), load->second));
    }

    void instruction(std::string_view text, Bytes& out) {
        const std::uint32_t pc = address();
        if (final_) {
            Result<Bytes> r = isa_.assemble(text, pc, labels_);
            if (!r.ok()) fail(r.error().what());
            append(out, r.value());
            return;
        }
        // The first pass needs sizes only: every label not known yet is far (absolute addressing), every branch
        // is near. Unknown names get a placeholder for this one line.
        if (isa_.is_branch(text::upper(split_mnemonic(text).first))) {
            out.insert(out.end(), {0, 0});
            return;
        }
        std::vector<std::string> placeholders;
        Result<Bytes> r = isa_.assemble(text, pc, labels_);
        while (!r.ok() && is_undefined(r.error())) {
            const std::string& name = static_cast<const UndefinedSymbolError&>(r.error()).name();
            if (labels_.contains(name)) break;
            labels_[name] = 0xFFFF;
            placeholders.push_back(name);
            r = isa_.assemble(text, pc, labels_);
        }
        for (const std::string& name : placeholders) labels_.erase(name);
        if (!r.ok()) fail(r.error().what());
        append(out, r.value());
    }

    static std::pair<std::string_view, std::string_view> split_mnemonic(std::string_view line) {
        std::size_t i = 0;
        while (i < line.size() && !text::is_space(line[i])) ++i;
        return {line.substr(0, i), text::ltrim(line.substr(i))};
    }

    const Encoder6502& isa_;
    const LinkConfig& config_;
    Labels& labels_;
    bool final_;
    Path folder_;
    std::vector<Area> areas_;
    std::optional<std::size_t> current_;
};

}  // namespace

LinkConfig parse_config(std::string_view text) {
    // `#` starts a comment that runs to the end of the line.
    std::string clean;
    for (const std::string_view line : text::lines(text)) {
        clean.append(line.substr(0, line.find('#')));
        clean.push_back('\n');
    }
    static const Regex block(R"((MEMORY|SEGMENTS)\s*\{([^}]*)\})");
    std::string_view memory, segments;
    block.for_each_match(clean, [&](const RegexMatch& m) {
        (m.group(1) == "MEMORY" ? memory : segments) = m.group(2);   // a later block of the same kind wins
        return true;
    });
    LinkConfig config;
    for_each_entry(memory, [&](std::string_view name, const std::unordered_map<std::string, std::string>& attrs) {
        MemoryArea area;
        area.name = std::string(name);
        area.start = config_value(attrs, "start", name);
        area.size = config_value(attrs, "size", name);
        const auto fill = attrs.find("fill");
        area.fill = fill != attrs.end() && fill->second == "yes";
        config.areas.push_back(std::move(area));
    });
    for_each_entry(segments, [&](std::string_view name, const std::unordered_map<std::string, std::string>& attrs) {
        const auto load = attrs.find("load");
        if (load == attrs.end()) throw AsmError(std::format("segment {} has no load area", name));
        config.segment_area[std::string(name)] = load->second;
    });
    return config;
}

Bytes Assembler::build(std::string_view source, std::string_view config_text, const Path& folder) const {
    const LinkConfig config = parse_config(config_text);
    struct Numbered {
        std::uint32_t number;
        std::string_view text;
    };
    std::vector<Numbered> lines;
    std::uint32_t n = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++n;
        const std::string_view line = text::trim(strip_comment(raw));
        if (!line.empty()) lines.push_back({n, line});
    }
    // Two passes: the first learns every label (sizes are all it needs), the second writes the final bytes.
    Labels labels;
    Bytes image;
    for (const bool final : {false, true}) {
        Pass pass(*isa_, config, labels, final, folder);
        for (const Numbered& l : lines) {
            try {
                pass.line(l.text);
            } catch (const LineFault& e) {
                throw AsmError(l.number, l.text, e.what());
            }
        }
        if (final) image = pass.image();
    }
    return image;
}

}  // namespace cb::build::m6502_asm
