// cb/build/x86asm/format_test_support.hpp -- what the object-format tests (format_obj_test.cpp, format_coff_test.cpp) share: a
// small readable dumper of an OMF object and of a COFF object (so a difference shows as one differing record or table row, not
// as a byte offset), and `Sim`, a stand-in for the assembler core around a format: it is the FormatHost, plays the passes
// NASM plays (first, optimization, stabilization, final), defines labels the way the label manager reports them to a format,
// and converts the data of a script into the calls the encoder makes (output(), symdef(), section()).
#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/directives.hpp"
#include "cb/build/x86asm/format.hpp"
#include "cb/core/bytes.hpp"

namespace cb::build::x86asm::fmt_test {

// Hex text ("0a ff", spaces ignored) to bytes, and bytes to lower-case hex without spaces.
inline Bytes from_hex(std::string_view text) {
    Bytes out;
    int hi = -1;
    for (char c : text) {
        int v;
        if (c >= '0' && c <= '9') v = c - '0';
        else if (c >= 'a' && c <= 'f') v = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') v = c - 'A' + 10;
        else continue;
        if (hi < 0) {
            hi = v;
        } else {
            out.push_back(static_cast<std::uint8_t>(hi * 16 + v));
            hi = -1;
        }
    }
    return out;
}

inline std::string to_hex(const std::uint8_t* p, std::size_t n) {
    std::string s;
    for (std::size_t i = 0; i < n; ++i) s += std::format("{:02x}", p[i]);
    return s;
}

// One line per OMF record: its name, type byte, data length, data (hex) and whether the checksum adds up.
inline std::string dump_omf(const Bytes& d) {
    static const std::map<unsigned, const char*> names = {{0x80, "THEADR"}, {0x88, "COMENT"}, {0x94, "LINNUM"}, {0x96, "LNAMES"},
                                                           {0x98, "SEGDEF"}, {0x9a, "GRPDEF"}, {0x8c, "EXTDEF"}, {0x90, "PUBDEF"},
                                                           {0xb0, "COMDEF"}, {0xa0, "LEDATA"}, {0x9c, "FIXUPP"}, {0x8a, "MODEND"}};
    std::string out;
    std::size_t i = 0;
    while (i + 3 <= d.size()) {
        const unsigned type = d[i];
        const std::size_t n = d[i + 1] | (d[i + 2] << 8);
        if (n == 0 || i + 3 + n > d.size()) {
            out += std::format("TRUNCATED at {:#x}\n", i);
            break;
        }
        unsigned sum = 0;
        for (std::size_t k = 0; k < 3 + n; ++k) sum += d[i + k];
        const auto it = names.find(type & 0xfe);
        out += std::format("{} {:02x} len={} {} {}\n", it == names.end() ? "?" : it->second, type, n - 1,
                           (sum & 0xff) == 0 ? "ok" : "BADCHECKSUM", to_hex(d.data() + i + 3, n - 1));
        i += 3 + n;
    }
    if (i != d.size()) out += std::format("{} stray bytes\n", d.size() - i);
    return out;
}

inline std::uint32_t le(const Bytes& d, std::size_t at, int n) {
    std::uint32_t v = 0;
    for (int k = 0; k < n && at + static_cast<std::size_t>(k) < d.size(); ++k) v |= static_cast<std::uint32_t>(d[at + static_cast<std::size_t>(k)]) << (8 * k);
    return v;
}

// A COFF object as text: the header, each section header with its data and relocations, the symbol table and the strings.
inline std::string dump_coff(const Bytes& d) {
    if (d.size() < 20) return "TOO SHORT\n";
    std::string out;
    const unsigned nsects = le(d, 2, 2);
    const std::uint32_t symptr = le(d, 8, 4), nsyms = le(d, 12, 4);
    out += std::format("machine {:04x} sections {} timestamp {:08x} symptr {} symbols {} opt {} flags {:04x}\n", le(d, 0, 2), nsects,
                       le(d, 4, 4), symptr, nsyms, le(d, 16, 2), le(d, 18, 2));
    for (unsigned s = 0; s < nsects; ++s) {
        const std::size_t h = 20 + 40 * s;
        std::string name;
        for (int k = 0; k < 8 && d[h + static_cast<std::size_t>(k)]; ++k) name += static_cast<char>(d[h + static_cast<std::size_t>(k)]);
        const std::uint32_t rawsize = le(d, h + 16, 4), rawptr = le(d, h + 20, 4), relptr = le(d, h + 24, 4);
        const unsigned nrel = le(d, h + 32, 2);
        out += std::format("section {} \"{}\" vsize {} vaddr {} size {} at {} relocs {} at {} lines {} at {} flags {:08x}\n", s + 1, name,
                           le(d, h + 8, 4), le(d, h + 12, 4), rawsize, rawptr, nrel, relptr, le(d, h + 34, 2), le(d, h + 28, 4),
                           le(d, h + 36, 4));
        if (rawptr && rawptr + rawsize <= d.size()) out += std::format("  data {}\n", to_hex(d.data() + rawptr, rawsize));
        for (unsigned r = 0; r < nrel && relptr + 10 * (r + 1) <= d.size(); ++r)
            out += std::format("  reloc at {} symbol {} type {:#x}\n", le(d, relptr + 10 * r, 4), le(d, relptr + 10 * r + 4, 4),
                               le(d, relptr + 10 * r + 8, 2));
    }
    const std::size_t st = symptr + 18 * static_cast<std::size_t>(nsyms);
    for (std::uint32_t i = 0; i < nsyms && symptr + 18 * (i + 1) <= d.size(); ++i) {
        const std::size_t e = symptr + 18 * static_cast<std::size_t>(i);
        std::string name;
        if (le(d, e, 4) == 0) {
            name = std::format("<string@{}>", le(d, e + 4, 4));
        } else {
            for (int k = 0; k < 8 && d[e + static_cast<std::size_t>(k)]; ++k) name += static_cast<char>(d[e + static_cast<std::size_t>(k)]);
        }
        out += std::format("symbol {} \"{}\" value {} section {} type {:#x} class {} aux {}  raw {}\n", i, name, le(d, e + 8, 4),
                           static_cast<std::int16_t>(le(d, e + 12, 2)), le(d, e + 14, 2), d[e + 16], d[e + 17], to_hex(d.data() + e, 18));
    }
    if (st + 4 <= d.size()) {
        const std::uint32_t slen = le(d, st, 4);
        out += std::format("strings length {}: {}\n", slen, to_hex(d.data() + st + 4, std::min<std::size_t>(d.size() - st - 4, slen > 4 ? slen - 4 : 0)));
    } else {
        out += "NO STRING TABLE\n";
    }
    return out;
}

// ---- the stand-in core ----

class Sim final : public FormatHost {
public:
    Assembly a;
    Bytes file;                              // what the format wrote to the output file
    std::unique_ptr<OutputFormat> format;
    std::vector<std::string> predefined;    // pre_define calls
    bool uppercase_option = false;          // get_boolean_option's last value

    struct Label {
        std::int32_t segment = 0;
        std::int64_t offset = 0;
    };
    std::map<std::string, Label> labels;
    std::map<std::string, std::int32_t> externs;

    using Factory = std::function<std::unique_ptr<OutputFormat>(FormatHost&)>;

    explicit Sim(const Factory& make, std::string_view input_name = "t.asm", std::int64_t timestamp = 0) {
        a.inname = std::string(input_name);
        a.official_compile_time.posix = timestamp;
        a.official_compile_time.have_posix = true;
        a.src.set(1, input_name);
        a.error_init();
        format = make(*this);
    }

    // Play the passes NASM plays (first, optimization, stabilization, final); `body` is the source, run once per pass.
    // The output file's bytes are returned.
    Bytes run(const std::function<void(Sim&)>& body) {
        format->init();
        for (int pass = 0; pass < 4; ++pass) {
            a.begin_pass();
            format->reset();
            int sb = a.globl.bits;
            current_ = format->section(std::nullopt, sb);
            bits = sb;
            offsets_.clear();
            body(*this);
            a.error_pass_end();
        }
        a.src.set(0, "");   // the end of the input: later messages name no line
        format->cleanup();
        return file;
    }

    int bits = 16;
    bool final_pass() const { return a.pass_final(); }
    std::int64_t offset() const { return offsets_[current_]; }
    std::int32_t current() const { return current_; }
    const Label& label_of(const std::string& name) const { return labels.at(name); }
    // The segment a name has (0 while it is not defined yet: only the final pass needs the value), and a name's extern segment.
    std::int32_t seg_of(const std::string& name) const {
        const auto it = labels.find(name);
        return it == labels.end() ? 0 : it->second.segment;
    }
    std::int32_t xref(const std::string& name) const { return seg_of(name); }
    // The source line the following calls are on (for messages).
    void at(std::int32_t line) { a.src.set(line, a.inname); }

    // ---- what the source does ----

    // [SECTION text]: the format picks the segment (and the bits); a name it does not know is an error.
    std::int32_t section(std::string_view text) {
        int sb = bits;
        const std::int32_t seg = format->section(text, sb);
        if (seg == NO_SEG) return seg;
        bits = sb;
        current_ = seg;
        return seg;
    }
    DirectiveResult directive(Directive d, std::string_view value) { return format->directive(d, value); }

    // A label at the current offset (symdef is called in the stabilization pass only, as the label manager does).
    void label(const std::string& name, bool global = false, std::string_view special = {}) {
        labels[name] = {current_, offset()};
        if (a.pass_type() == PASS_STAB) format->symdef(name, current_, offset(), global ? 1 : 0, special);
    }
    // EXTERN name: a segment number of its own.
    std::int32_t extern_(const std::string& name, std::string_view special = {}) {
        auto it = externs.find(name);
        if (it == externs.end()) it = externs.emplace(name, seg_alloc()).first;
        labels[name] = {it->second, 0};
        if (a.pass_type() == PASS_STAB) format->symdef(name, it->second, 0, 1, special);
        return it->second;
    }
    // COMMON name size[:special]
    std::int32_t common(const std::string& name, std::int64_t size, std::string_view special = {}) {
        auto it = externs.find(name);
        if (it == externs.end()) it = externs.emplace(name, seg_alloc()).first;
        labels[name] = {it->second, 0};
        if (a.pass_type() == PASS_STAB) format->symdef(name, it->second, size, 2, special);
        return it->second;
    }
    // GLOBAL name / name EQU value for an absolute symbol.
    void equ_global(const std::string& name, std::int64_t value) {
        labels[name] = {NO_SEG, value};
        if (a.pass_type() == PASS_STAB) format->symdef(name, NO_SEG, value, 1, {});
    }

    // The data the encoder sends the format (only in the final pass; the offsets move in every pass).
    void raw(std::initializer_list<int> bytes) {
        Bytes b;
        for (int v : bytes) b.push_back(static_cast<std::uint8_t>(v));
        raw(b);
    }
    void raw(const Bytes& b) {
        if (final_pass()) {
            OutData d = base();
            d.type = OUT_RAWDATA;
            d.size = b.size();
            d.data = b.data();
            d.legacy.data = b.data();
            d.legacy.type = OUT_RAWDATA;
            d.legacy.size = b.size();
            d.legacy.tsegment = NO_SEG;
            d.legacy.twrt = NO_SEG;
            format->output(d);
        }
        offsets_[current_] += static_cast<std::int64_t>(b.size());
    }
    // An address of `size` bytes: the value is `toffset` in segment `tsegment` (wrt `wrt`).
    void addr(int size, std::int32_t tsegment, std::int64_t toffset, std::int32_t wrt = NO_SEG) {
        if (final_pass()) {
            OutData d = base();
            d.type = OUT_ADDRESS;
            d.size = static_cast<std::uint64_t>(size);
            d.toffset = static_cast<std::uint64_t>(toffset);
            d.tsegment = tsegment;
            d.twrt = wrt;
            d.legacy.data = &d.toffset;
            d.legacy.type = OUT_ADDRESS;
            d.legacy.size = static_cast<std::uint64_t>(size);
            d.legacy.tsegment = tsegment;
            d.legacy.twrt = wrt;
            format->output(d);
        }
        offsets_[current_] += size;
    }
    // A reference to the base of a segment: what the encoder makes of `seg x` (OUT_SEGMENT): the segment base value that
    // the format's segbase gives, as an odd segment number.
    void seg_base(int size, std::int32_t segment_index, std::int32_t wrt = NO_SEG) {
        std::int32_t t = format->segbase(segment_index | 1);
        if (t != NO_SEG && t < SEG_ABS) t |= 1;
        addr(size, t, 0, wrt);
    }
    // A relative address of `size` bytes to `toffset` in `tsegment`; `trailing` bytes of the instruction follow it.
    void rel(int size, std::int32_t tsegment, std::int64_t toffset, int trailing = 0) {
        if (final_pass()) {
            OutData d = base();
            d.type = OUT_RELADDR;
            d.size = static_cast<std::uint64_t>(size);
            d.toffset = static_cast<std::uint64_t>(toffset);
            d.tsegment = tsegment;
            d.twrt = NO_SEG;
            d.legacy.data = &d.toffset;
            d.legacy.type = size == 1 ? OUT_REL1ADR : size == 2 ? OUT_REL2ADR : size == 4 ? OUT_REL4ADR : OUT_REL8ADR;
            d.legacy.size = static_cast<std::uint64_t>(size + trailing);   // relbase - offset
            d.legacy.tsegment = tsegment;
            d.legacy.twrt = NO_SEG;
            format->output(d);
        }
        offsets_[current_] += size;
    }
    void reserve(std::uint64_t n) {
        if (final_pass()) {
            OutData d = base();
            d.type = OUT_RESERVE;
            d.size = n;
            d.legacy.type = OUT_RESERVE;
            d.legacy.size = n;
            d.legacy.tsegment = NO_SEG;
            d.legacy.twrt = NO_SEG;
            format->output(d);
        }
        offsets_[current_] += static_cast<std::int64_t>(n);
    }

    // ---- FormatHost ----
    Assembly& assembly() override { return a; }
    Bytes& ofile() override { return file; }
    bool ofile_is_text() const override { return false; }
    Bytes* open_write(std::string_view, bool) override { return nullptr; }
    void stdout_write(std::string_view) override {}
    void stderr_write(std::string_view) override {}
    std::optional<Expr> evaluate_text(std::string_view, int) override { return std::nullopt; }
    void define_label(std::string_view name, std::int32_t segment, std::int64_t offset, bool) override {
        labels[std::string(name)] = {segment, offset};
        if (a.pass_type() == PASS_STAB) format->symdef(name, segment, offset, 0, {});
    }
    LabelType lookup_label(std::string_view name, std::int32_t& segment, std::int64_t& offset) override {
        const auto it = labels.find(std::string(name));
        if (it == labels.end()) return LBL_none;
        segment = it->second.segment;
        offset = it->second.offset;
        return LBL_STATIC;
    }
    std::int32_t seg_alloc() override {
        const std::int32_t s = next_seg_;
        next_seg_ += 2;
        return s;
    }
    void backend_label(std::string_view name, std::int32_t segment, std::int64_t offset) override {
        labels[std::string(name)] = {segment, offset};
        format->symdef(name, segment, offset, 0, {});
    }
    ExprAtFront evaluate_critical(std::string_view text) override {
        // A number at the front (decimal), nothing more.
        ExprAtFront r;
        std::size_t n = 0;
        std::int64_t v = 0;
        while (n < text.size() && text[n] >= '0' && text[n] <= '9') v = v * 10 + (text[n++] - '0');
        if (n) {
            r.ok = r.simple = true;
            r.value = v;
        }
        r.consumed = n;
        return r;
    }
    void pre_define(std::string_view definition) override { predefined.emplace_back(definition); }
    void get_boolean_option(std::string_view text, bool& value) override {
        std::string_view t = text;
        while (!t.empty() && t.front() == ' ') t.remove_prefix(1);
        if (t.empty() || t == "on" || t == "yes" || t == "true" || t == "1") value = true;
        else if (t == "off" || t == "no" || t == "false" || t == "0") value = false;
        uppercase_option = value;
    }

private:
    OutData base() const {
        OutData d;
        d.loc.segment = current_;
        d.loc.offset = offsets_.count(current_) ? offsets_.at(current_) : 0;
        d.loc.known = true;
        d.bits = bits;
        return d;
    }

    std::int32_t current_ = NO_SEG;
    std::int32_t next_seg_ = 2;
    mutable std::unordered_map<std::int32_t, std::int64_t> offsets_;
};

}  // namespace cb::build::x86asm::fmt_test
