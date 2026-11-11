#include "cb/exact/compare.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::compare {
namespace {

constexpr std::string_view kReader = "compare";
constexpr std::int64_t kWord = 0xFFFF;         // a 16-bit offset wraps at 64K
constexpr std::int64_t kFarMask = 0xFFFFF;     // a far address is 20 bits
constexpr std::size_t kStringPreview = 24;     // how much of a string a note shows

std::int64_t word_at(ByteView b, std::int64_t offset) {
    if (offset < 0) throw TruncatedError(kReader, 0, 2, 0);
    io::ByteReader r(b, kReader);
    r.seek(static_cast<std::uint64_t>(offset));
    return r.u16();
}

std::int64_t signed_word_at(ByteView b, std::int64_t offset) { return static_cast<std::int16_t>(word_at(b, offset)); }

// Python-style "%0Nx": a negative value is its magnitude with a minus sign, the width counting the sign.
std::string hex_pad(std::int64_t v, int width) {
    const bool negative = v < 0;
    const std::uint64_t magnitude = negative ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v);
    std::string digits = std::format("{:x}", magnitude);
    const int fill = width - static_cast<int>(digits.size()) - (negative ? 1 : 0);
    return (negative ? "-" : "") + std::string(fill > 0 ? static_cast<std::size_t>(fill) : 0, '0') + digits;
}

// Bytes for a human, the way a bytes literal reads: b'..' with printable ASCII as it is and the rest as escapes.
std::string preview(ByteView b, std::size_t limit = kStringPreview) {
    const ByteView shown = b.first(std::min(b.size(), limit));
    const bool has_single = std::ranges::find(shown, std::uint8_t{'\''}) != shown.end();
    const bool has_double = std::ranges::find(shown, std::uint8_t{'"'}) != shown.end();
    const char quote = has_single && !has_double ? '"' : '\'';
    std::string out = std::string("b") + quote;
    for (std::uint8_t c : shown) {
        if (c == static_cast<std::uint8_t>(quote) || c == '\\') out += std::string("\\") + static_cast<char>(c);
        else if (c == '\t') out += "\\t";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c >= 0x20 && c < 0x7F) out += static_cast<char>(c);
        else out += std::format("\\x{:02x}", c);
    }
    return out + quote;
}

std::string_view kind_name(FixKind k) {
    switch (k) {
        case FixKind::Off: return "off";
        case FixKind::Base: return "base";
        case FixKind::Ptr: return "ptr";
        case FixKind::Lo: return "lo";
        case FixKind::Rel: return "rel";
    }
    return "?";
}

// A target as the notes show it: ('ext', 'name').
std::string target_text(const Target& t) {
    const char* kind = t.kind == TargetKind::External ? "ext" : t.kind == TargetKind::Group ? "grp" : "seg";
    return std::format("('{}', '{}')", kind, t.name);
}

// The 16-bit instructions of the function's own code, decoded once, asked "does this instruction hold the word at
// fo, and does it address memory through a base or index register".
class FunctionCode {
public:
    FunctionCode(ByteView code, std::uint32_t off, std::uint32_t ln, const x86::Decoder16& dec)
        : code_(code.subspan(off, ln)), off_(off), dec_(dec) {}
    const std::vector<x86::Insn>& insns() const {
        if (!decoded_) {
            insns_ = dec_.all(code_, off_);
            decoded_ = true;
        }
        return insns_;
    }
    bool indexed(std::uint32_t fo) const {
        for (const x86::Insn& i : insns()) {
            if (fo < i.address || fo >= i.address + i.size()) continue;
            const std::size_t open = i.op_str.find('[');
            if (open == std::string::npos) return false;
            const std::size_t close = i.op_str.find(']', open);
            // (the text between the brackets; a missing ']' leaves out the last character, as the original read it)
            const std::string_view inside = std::string_view(i.op_str).substr(
                open, (close == std::string::npos ? i.op_str.size() - 1 : close) - open);
            return std::ranges::any_of(std::array<std::string_view, 4>{"bx", "bp", "si", "di"},
                                       [&](std::string_view r) { return inside.find(r) != std::string_view::npos; });
        }
        return false;
    }

private:
    ByteView code_;
    std::uint32_t off_;
    const x86::Decoder16& dec_;
    mutable std::vector<x86::Insn> insns_;
    mutable bool decoded_ = false;
};

// Bytes that differ alone (an immediate) and each by the same amount: a constant or an enumeration with members more
// or fewer in the program's source than in this one.
std::optional<Note> shift_note(const Bytes& code, ByteView img, std::uint32_t off, std::uint32_t at,
                               const std::vector<std::uint32_t>& bad) {
    const std::set<std::uint32_t> bad_set(bad.begin(), bad.end());
    std::vector<std::uint32_t> lone;
    for (std::uint32_t k : bad)
        if ((k == 0 || !bad_set.contains(k - 1)) && !bad_set.contains(k + 1) && at + k < img.size()) lone.push_back(k);
    std::set<std::uint8_t> deltas;
    for (std::uint32_t k : lone) deltas.insert(static_cast<std::uint8_t>(img[at + k] - code[off + k]));
    if (lone.empty() || deltas.size() != 1 || !(lone.size() >= 2 || lone == bad)) return std::nullopt;
    const unsigned d = *deltas.begin();
    std::string places;
    for (std::size_t i = 0; i < std::min<std::size_t>(lone.size(), 6); ++i) places += (i ? ", +" : "+") + std::format("{:x}", lone[i]);
    return Note{Severity::Warning,
                std::format("{} value{} {}{} in the program ({}) - a constant, or an enumeration with members more or "
                            "fewer before them",
                            lone.size(), lone.size() > 1 ? "s each" : "", d < 0x80 ? "+" : "-", d < 0x80 ? d : 0x100 - d, places)};
}

}  // namespace

bool Report::identical() const {
    return bad.empty() && std::ranges::none_of(notes, [](const Note& n) { return n.severity == Severity::Error; });
}

Report function(const Subject& s, ByteView img, const Program& prj, SymbolTable& syms, SymbolTable& learn,
                const Where& where_in, const x86::Decoder16& dec) {
    const ObjectFile& o = s.object;
    const Bytes* code_ptr = o.seg.find(s.segment);
    if (code_ptr == nullptr) throw FormatError(kReader, 0, "the object has no segment " + s.segment);
    const Bytes& code = *code_ptr;
    const std::uint32_t off = s.offset, ln = s.length;
    if (static_cast<std::uint64_t>(off) + ln > code.size())
        throw FormatError(kReader, off, "the function lies beyond the end of its segment");

    Report rep;
    std::set<std::uint32_t> skip;
    const std::int64_t dg = prj.dgroup, ls = prj.loadseg;
    const std::int64_t at = s.at;
    const bool placed = static_cast<bool>(where_in);   // (each function of the file at a place of its own)
    const Where where = placed ? where_in : Where([&](std::uint32_t x) { return at - off + x; });
    const std::int64_t base = prj.code_base(s.at);
    const bool has_relocs = prj.relocs != nullptr && !prj.relocs->empty();
    const FunctionCode fcode(code, off, ln, dec);

    const auto note = [&](std::string text) { rep.notes.push_back({Severity::Error, std::move(text)}); };
    const auto warn = [&](std::string text) { rep.notes.push_back({Severity::Warning, std::move(text)}); };
    const auto bind = [&](const std::string& name, SymKind kind, std::int64_t address) {
        const SymPlace place{kind, address};
        const SymPlace* known = syms.find(name);
        if (known == nullptr) {
            syms.insert_or_assign(name, place);
            learn.insert_or_assign(name, place);
        } else if (*known != place && !(known->kind == SymKind::Unknown && known->address == address)) {   // ("?": a place, kind unsaid)
            note(std::format("{} is {} {} in SYMBOLS, here {} {}", name, sym_kind_letter(known->kind), hex_pad(known->address, 4),
                             sym_kind_letter(kind), hex_pad(address, 4)));
        }
    };
    const auto far_at = [&](std::int64_t k) { return (word_at(img, at + k + 2) - ls) * 16 + word_at(img, at + k); };   // the image's segment:offset at +k

    for (const Fixup& f : o.fix) {
        if (f.segment != s.segment || !(off <= f.offset && f.offset < off + ln)) continue;
        const std::int64_t k = f.offset - off;
        skip.insert(static_cast<std::uint32_t>(k));
        skip.insert(static_cast<std::uint32_t>(k + 1));
        const std::int64_t mine = (word_at(code, f.offset) + f.displacement) & kWord;
        const std::int64_t theirs = word_at(img, at + k);
        const Target& tgt = f.target;
        const bool external = tgt.kind == TargetKind::External;
        const std::string where_k = std::format("+{:x}: ", k);

        switch (f.kind) {
            case FixKind::Rel: {
                const std::int64_t dest = base + ((at + k + 2 + theirs - base) & kWord);
                if (external) bind(tgt.name, SymKind::Code, dest - mine);
                else if (dest != where(static_cast<std::uint32_t>(mine)))
                    note(where_k + std::format("calls {}, this file's code would be at {}", hex_pad(dest, 5),
                                               hex_pad(where(static_cast<std::uint32_t>(mine)), 5)));
                break;
            }
            case FixKind::Ptr: {   // a far call or a far address: offset and segment
                skip.insert(static_cast<std::uint32_t>(k + 2));
                skip.insert(static_cast<std::uint32_t>(k + 3));
                const std::int64_t dest = far_at(k);
                if (external) {
                    bind(tgt.name, SymKind::Code, (dest - mine) & kFarMask);
                } else if (tgt.name == s.segment) {
                    if (dest != where(static_cast<std::uint32_t>(mine)))
                        note(where_k + std::format("a far place of this file's code {}, expected {}", hex_pad(dest, 5),
                                                   hex_pad(where(static_cast<std::uint32_t>(mine)), 5)));
                } else if (o.seg.contains(tgt.name) && tgt.name.ends_with("_TEXT")) {   // another code segment of the file
                    const Public* nearest = nullptr;   // the last public at or before the word's place
                    for (const Public& p : o.pubs) {
                        if (!p.segment || *p.segment != tgt.name || p.offset > mine) continue;
                        if (nearest == nullptr || std::pair(p.offset, p.name) > std::pair(nearest->offset, nearest->name)) nearest = &p;
                    }
                    const SymPlace* known = nearest != nullptr ? syms.find(nearest->name) : nullptr;
                    const std::optional<std::int64_t> expected =
                        known != nullptr ? std::optional<std::int64_t>(known->address + mine - nearest->offset) : std::nullopt;
                    if (!expected || dest != *expected)
                        note(where_k + std::format("far place {} in {}, expected {}", hex_pad(dest, 5), tgt.name,
                                                   expected ? hex_pad(*expected, 5) : "None"));
                } else {
                    note(where_k + "far fixup to " + target_text(tgt));
                }
                break;
            }
            case FixKind::Base: {   // a segment word alone
                if (tgt.kind == TargetKind::Segment && tgt.name == s.segment) {
                    const std::int64_t sb = (theirs - ls) * 16;
                    if (!(sb <= at && at < sb + 0x10000))
                        note(where_k + std::format("segment {:04x} is not this code's", theirs));
                } else if (!external) {   // (an external's: checked with the offset beside it)
                    if (has_relocs && !prj.relocs->contains(static_cast<std::uint32_t>(at + k)))   // (another segment of the file's: a word the loader sets)
                        note(where_k + "segment of " + target_text(tgt) + ", not a word the loader sets");
                    else bind(tgt.name + "@seg", SymKind::Segment, theirs);
                }
                break;
            }
            case FixKind::Off: {
                if (external) {
                    const Fixup* far_half = nullptr;   // a function's address in two halves
                    for (const Fixup& g : o.fix)
                        if (g.segment == s.segment && g.kind == FixKind::Base && g.target == tgt && off <= g.offset && g.offset < off + ln) {
                            far_half = &g;
                            break;
                        }
                    const SymPlace* known = syms.find(tgt.name);
                    if (far_half != nullptr) {
                        bind(tgt.name, SymKind::Code,
                             (word_at(img, at + far_half->offset - off) - ls) * 16 + ((theirs - mine) & kWord));
                    } else if (known != nullptr && known->kind == SymKind::Code) {
                        const std::int64_t a = known->address;
                        if (((a - prj.code_base(static_cast<std::uint32_t>(a))) & kWord) != ((theirs - mine) & kWord))
                            note(where_k + std::format("{} is at {}, here offset {:04x}", tgt.name, hex_pad(a, 5), (theirs - mine) & kWord));
                    } else {
                        bind(tgt.name, known != nullptr ? known->kind : SymKind::Data, (theirs - mine) & kWord);
                    }
                    break;
                }
                if (tgt.name == s.segment) {   // a place of this file's code (a switch's table)
                    if (((where(static_cast<std::uint32_t>(mine)) - base) & kWord) != theirs)
                        note(where_k + std::format("the place {:04x}, this file's code would be at {:04x}", theirs,
                                                   (where(static_cast<std::uint32_t>(mine)) - base) & kWord));
                    break;
                }
                // The file's own data: what the word points at must be there.
                const Fixup* far_item = nullptr;   // the segment of a far item, kept among the constants
                for (const Fixup& g : o.fix)
                    if (g.segment == tgt.name && g.offset == mine && g.kind == FixKind::Base && g.target.kind == TargetKind::External) {
                        far_item = &g;
                        break;
                    }
                if (far_item != nullptr) {
                    const std::string name = far_item->target.name + "@seg";
                    if (!has_relocs) bind(name, SymKind::Segment, word_at(img, dg + theirs));   // (an image without its relocation table: EXEPACK's)
                    else if (!prj.relocs->contains(static_cast<std::uint32_t>(dg + theirs)))
                        note(where_k + std::format("DS:{:04x} is not a segment the loader sets", theirs));
                    else bind(name, SymKind::Segment, word_at(img, dg + theirs));
                    break;
                }
                const Bytes* seg_bytes = o.seg.find(tgt.name);
                const ByteView d = seg_bytes != nullptr ? ByteView(*seg_bytes) : ByteView{};
                std::size_t e = static_cast<std::size_t>(mine);
                while (e < d.size() && d[e] != 0) ++e;
                // the string the word points at, its NUL included (only in the segments that hold strings)
                const ByteView str = (tgt.name == "CONST" || tgt.name == "_DATA") && static_cast<std::size_t>(mine) < d.size()
                                         ? d.subspan(static_cast<std::size_t>(mine), std::min(e + 1, d.size()) - static_cast<std::size_t>(mine))
                                         : ByteView{};
                std::set<std::int64_t> holes;
                for (const Fixup& g : o.fix)
                    if (g.segment == tgt.name)
                        for (int x = 0; x < (g.kind == FixKind::Ptr ? 4 : 2); ++x) holes.insert(g.offset + x);
                const auto differs = [&] {
                    for (std::size_t x = 0; x < str.size(); ++x)
                        if (!holes.contains(mine + static_cast<std::int64_t>(x)) && img[static_cast<std::size_t>(dg + theirs) + x] != str[x]) return true;
                    return false;
                };
                if (!str.empty() && (dg + theirs + static_cast<std::int64_t>(str.size()) > static_cast<std::int64_t>(img.size()) || differs())) {
                    const std::size_t have = static_cast<std::size_t>(dg + theirs) < img.size() ? std::min<std::size_t>(str.size(), img.size() - static_cast<std::size_t>(dg + theirs)) : 0;
                    note(where_k + std::format("{}+{:x} ({}) is not at DS:{:04x} ({})", tgt.name, mine, preview(str), theirs,
                                               preview(have != 0 ? img.subspan(static_cast<std::size_t>(dg + theirs), std::min(have, kStringPreview)) : ByteView{})));
                } else if (str.size() > 2 && (mine == 0 || d[static_cast<std::size_t>(mine) - 1] == 0) && theirs != 0 &&
                           static_cast<std::size_t>(dg + theirs) - 1 < img.size() && img[static_cast<std::size_t>(dg + theirs) - 1] >= 0x20 &&
                           img[static_cast<std::size_t>(dg + theirs) - 1] < 0x7F) {
                    warn(where_k + std::format("{}+{:x} ({}) is the end of another string at DS:{:04x} - a number, not a string?",
                                               tgt.name, mine, preview(str), theirs));
                }
                // (through a register the word may be an array's origin - its start less its first index - which can
                // be another item's place: such a word is held only to the others of its kind)
                bind(std::format("{}:{}+{:x}{}", s.name, tgt.name, mine, fcode.indexed(f.offset) ? "[]" : ""), SymKind::Data, theirs);
                break;
            }
            case FixKind::Lo:
                note(where_k + std::format("fixup {} to {}", kind_name(f.kind), target_text(tgt)));
                break;
        }
    }

    // Near calls and jumps from the function to another of its file have no fix-up (the compiler wrote them): with
    // the file's functions at places of their own, each must reach its callee's place.
    if (placed && where(off) == at) {
        for (const x86::Insn& i : fcode.insns()) {
            if (i.bytes.empty() || (i.bytes[0] != 0xE8 && i.bytes[0] != 0xE9) || i.size() != 3) continue;
            const std::int64_t k = static_cast<std::int64_t>(i.address) - off + 1;
            if (skip.contains(static_cast<std::uint32_t>(k))) continue;
            const std::int64_t mine = (i.address + 3 + static_cast<std::int16_t>(i.bytes[1] | (i.bytes[2] << 8))) & kWord;
            if (off <= mine && mine < off + ln) continue;
            if (at + k + 2 <= static_cast<std::int64_t>(img.size()) &&
                base + ((at + k + 2 + signed_word_at(img, at + k) - base) & kWord) == where(static_cast<std::uint32_t>(mine))) {
                skip.insert(static_cast<std::uint32_t>(k));
                skip.insert(static_cast<std::uint32_t>(k + 1));
            }
        }
        // (and a call the disassembly does not see - data after a call, as MS-Pascal's entry helper has it, puts it out
        // of step: an E8 on both sides whose targets are the same place)
        for (std::int64_t k = 1; k + 1 < static_cast<std::int64_t>(ln); ++k) {
            if (skip.contains(static_cast<std::uint32_t>(k)) || code[off + k - 1] != 0xE8 ||
                at + k + 1 >= static_cast<std::int64_t>(img.size()) || img[static_cast<std::size_t>(at + k - 1)] != 0xE8)
                continue;
            const std::int64_t mine = (off + k + 2 + signed_word_at(code, off + k)) & kWord;
            if (off <= mine && mine < off + ln) continue;
            const std::int64_t w = where(static_cast<std::uint32_t>(mine));
            if (w >= 0 && base + ((at + k + 2 + signed_word_at(img, at + k) - base) & kWord) == w) {
                skip.insert(static_cast<std::uint32_t>(k));
                skip.insert(static_cast<std::uint32_t>(k + 1));
            }
        }
    }

    for (std::uint32_t k = 0; k < ln; ++k)
        if (!skip.contains(k) && (static_cast<std::uint64_t>(at) + k >= img.size() || img[static_cast<std::size_t>(at) + k] != code[off + k]))
            rep.bad.push_back(k);
    if (std::optional<Note> shift = shift_note(code, img, off, s.at, rep.bad)) rep.notes.push_back(std::move(*shift));
    return rep;
}

}  // namespace cb::exact::compare
