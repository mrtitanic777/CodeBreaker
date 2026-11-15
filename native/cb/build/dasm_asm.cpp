#include "cb/build/dasm_asm.hpp"

#include <algorithm>
#include <cctype>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::build::dasm_asm {

namespace {

class LineFault : public Error {
public:
    using Error::Error;
    CB_ERROR_BOILERPLATE(LineFault)
};
[[noreturn]] void fail(const std::string& reason) { throw LineFault(reason); }

std::optional<std::int64_t> dasm_number(std::string_view s) {
    s = text::trim(s);
    if (s.starts_with('$')) return text::parse_int<std::int64_t>(s.substr(1), 16);
    if (s.starts_with('%')) return text::parse_int<std::int64_t>(s.substr(1), 2);
    return parse_number(s);
}

bool is_name_start(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_' || c == '.'; }
bool is_name_char(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '.'; }

// The directives that stand where an instruction does.
bool is_directive(std::string_view word) {
    const std::string w = text::lower(word);
    for (const char* d : {"processor", "org", "rorg", "seg", "seg.u", "echo", "equ", "ds", "ds.b", ".byte", "byte", ".word", "word", "dc.b", "dc.w", "hex",
                          "incbin", "rend"}) {
        if (w == d) return true;
    }
    return false;
}

class Pass {
public:
    Pass(const m6502_asm::Encoder6502& isa, Labels& labels, bool final, const std::filesystem::path& folder)
        : isa_(isa), labels_(labels), final_(final), folder_(folder) {}

    void line(std::string_view raw_line) {
        std::string_view text = raw_line;
        // a label stands at the start of the line (no indentation), a directive or instruction after white space
        if (!text.empty() && !text::is_space(text[0]) && is_name_start(text[0])) {
            std::size_t i = 1;
            while (i < text.size() && is_name_char(text[i])) ++i;
            const std::string_view name = text.substr(0, i);
            const std::string_view rest = text::trim(text.substr(i));
            if (rest.starts_with('=')) return constant(name, text::trim(rest.substr(1)));
            const std::size_t space = rest.find_first_of(" \t");
            if (text::lower(rest.substr(0, space)) == "equ") return constant(name, text::trim(rest.substr(space)));
            if (!is_directive(name)) {
                define(name);
                text = rest;
                if (text.empty()) return;
            }
        }
        text = text::trim(text);
        std::size_t i = 0;
        while (i < text.size() && !text::is_space(text[i])) ++i;
        const std::string word = text::lower(text.substr(0, i));
        const std::string_view args = text::trim(text.substr(i));
        if (word == "processor" || word == "seg" || word == "seg.u" || word == "echo" || word == "rend") return;
        if (word == "org") {
            const std::int64_t at = number(args);
            if (!origin_) origin_ = at;
            if (at < *origin_) fail("an ORG below the first one");
            const std::size_t offset = static_cast<std::size_t>(at - *origin_);
            if (offset < out_.size()) fail("an ORG that goes back over bytes already written");
            out_.resize(offset, 0);
            pc_ = at;
            return;
        }
        if (word == "rorg") {
            pc_ = number(args);
            return;
        }
        if (!origin_) fail("code before the first ORG");
        if (word == ".byte" || word == "byte" || word == "dc.b") return bytes(args);
        if (word == ".word" || word == "word" || word == "dc.w") return words(args);
        if (word == "ds" || word == "ds.b") return fill(args);
        if (word == "hex") return hex(args);
        if (word == "incbin") return incbin(args);
        if (word == "equ") fail("equ without a name");
        instruction(text);
    }

    const Bytes& image() const { return out_; }

private:
    void define(std::string_view name) {
        if (!origin_) fail("a label before the first ORG");
        labels_[std::string(name)] = pc_;
    }
    void constant(std::string_view name, std::string_view value) { labels_[std::string(name)] = number(value); }

    // A number or a label's value (an undefined label is 0 in the first pass).
    std::int64_t number(std::string_view s) const {
        s = text::trim(s);
        if (const auto v = dasm_number(s)) return *v;
        if (!s.empty() && (s[0] == '<' || s[0] == '>')) {
            const std::int64_t v = number(s.substr(1));
            return s[0] == '<' ? (v & 0xFF) : ((v >> 8) & 0xFF);
        }
        if (const auto it = labels_.find(std::string(s)); it != labels_.end()) return it->second;
        if (final_ || s.empty() || !is_name_start(s[0])) fail(std::format("'{}' is not a number or a known label", s));
        return 0;
    }

    void put(std::uint8_t b) {
        out_.push_back(b);
        ++pc_;
    }

    void bytes(std::string_view args) {
        for (const std::string_view a : split_items(args, false)) {
            if (a.starts_with('"')) {
                if (a.size() < 2 || !a.ends_with('"')) fail("a string without its closing quote");
                for (const char c : a.substr(1, a.size() - 2)) put(static_cast<std::uint8_t>(c));
            } else {
                put(static_cast<std::uint8_t>(number(a) & 0xFF));
            }
        }
    }
    void words(std::string_view args) {
        for (const std::string_view a : split_items(args, false)) {
            const std::int64_t v = number(a);
            put(static_cast<std::uint8_t>(v & 0xFF));
            put(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        }
    }
    void fill(std::string_view args) {
        const std::vector<std::string_view> items = split_items(args, false);
        if (items.empty() || items.size() > 2) fail("ds takes a count and an optional fill value");
        const std::int64_t count = number(items[0]);
        const std::uint8_t value = items.size() == 2 ? static_cast<std::uint8_t>(number(items[1]) & 0xFF) : 0;
        if (count < 0 || count > 0x100000) fail("a fill count out of range");
        for (std::int64_t i = 0; i < count; ++i) put(value);
    }
    void hex(std::string_view args) {
        std::string digits;
        for (const char c : args) {
            if (!text::is_space(c)) digits.push_back(c);
        }
        if (digits.size() % 2 != 0) fail("hex takes pairs of digits");
        for (std::size_t i = 0; i < digits.size(); i += 2) {
            const auto v = text::parse_int<int>(std::string_view(digits).substr(i, 2), 16);
            if (!v) fail("hex takes hexadecimal digits");
            put(static_cast<std::uint8_t>(*v));
        }
    }
    void incbin(std::string_view args) {
        if (args.size() < 2 || args.front() != '"' || args.back() != '"') fail("incbin without a file name in quotes");
        const Bytes data = fs::read_file(folder_ / std::string(args.substr(1, args.size() - 2)));
        for (const std::uint8_t b : data) put(b);
    }

    // `MNM.w arg` is `MNM a:arg` to the encoder: the absolute form is asked for.
    static std::string encoder_line(std::string_view text) {
        std::size_t i = 0;
        while (i < text.size() && !text::is_space(text[i])) ++i;
        std::string_view mnemonic = text.substr(0, i);
        const std::string_view arg = text::trim(text.substr(i));
        const std::string lower = text::lower(mnemonic);
        if (lower.size() > 2 && lower.ends_with(".w")) {
            mnemonic.remove_suffix(2);
            std::string prefixed(arg);
            prefixed.insert(prefixed.starts_with('(') ? 1 : 0, "a:");   // after a `(` if there is one
            return std::string(mnemonic) + " " + prefixed;
        }
        if (lower.size() > 2 && lower.ends_with(".b")) {
            mnemonic.remove_suffix(2);
            return std::string(mnemonic) + " " + std::string(arg);
        }
        return std::string(text);
    }

    void instruction(std::string_view text) {
        const std::string line = encoder_line(text);
        const std::uint32_t pc = static_cast<std::uint32_t>(pc_);
        if (final_) {
            Result<Bytes> r = isa_.assemble(line, pc, labels_);
            if (!r.ok()) fail(r.error().what());
            for (const std::uint8_t b : r.value()) put(b);
            return;
        }
        // The first pass needs sizes only: a name not known yet is far (absolute addressing), every branch near.
        std::size_t sp = 0;
        while (sp < line.size() && !text::is_space(line[sp])) ++sp;
        if (isa_.is_branch(text::upper(line.substr(0, sp)))) {
            put(0);
            put(0);
            return;
        }
        std::vector<std::string> placeholders;
        Result<Bytes> r = isa_.assemble(line, pc, labels_);
        while (!r.ok() && is_undefined(r.error())) {
            const std::string& name = static_cast<const UndefinedSymbolError&>(r.error()).name();
            if (labels_.contains(name)) break;
            labels_[name] = 0xFFFF;
            placeholders.push_back(name);
            r = isa_.assemble(line, pc, labels_);
        }
        for (const std::string& name : placeholders) labels_.erase(name);
        if (!r.ok()) fail(r.error().what());
        for (const std::uint8_t b : r.value()) put(b);
    }

    const m6502_asm::Encoder6502& isa_;
    Labels& labels_;
    bool final_;
    std::filesystem::path folder_;
    Bytes out_;
    std::optional<std::int64_t> origin_;
    std::int64_t pc_ = 0;
};

}  // namespace

Bytes Assembler::build(std::string_view source, const std::filesystem::path& folder) const {
    struct Numbered {
        std::uint32_t number;
        std::string_view text;
    };
    std::vector<Numbered> lines;
    std::uint32_t n = 0;
    for (const std::string_view raw : text::lines(source)) {
        ++n;
        std::string_view line = strip_comment(raw);
        while (!line.empty() && text::is_space(line.back())) line.remove_suffix(1);
        if (!text::trim(line).empty()) lines.push_back({n, line});
    }
    Labels labels;
    Bytes image;
    for (const bool final : {false, true}) {
        Pass pass(*isa_, labels, final, folder);
        for (const Numbered& l : lines) {
            try {
                pass.line(l.text);
            } catch (const LineFault& e) {
                throw AsmError(l.number, text::trim(l.text), e.what());
            }
        }
        if (final) image = pass.image();
    }
    return image;
}

}  // namespace cb::build::dasm_asm
