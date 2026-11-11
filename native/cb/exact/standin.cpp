#include "cb/exact/standin.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf_object.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::standin {

namespace omf = formats::omf;

namespace {

constexpr std::string_view kReader = "standin";

// OMF record types this module reads or writes.
constexpr std::uint8_t kTheadr = 0x80, kExtdef = 0x8C, kModend = 0x8A, kModend32 = 0x8B, kPubdef = 0x90, kLnames = 0x96,
                       kSegdef = 0x98, kGrpdef = 0x9A, kFixupp = 0x9C, kLedata = 0xA0, kLidata = 0xA2;

[[noreturn]] void bad(const std::string& what) { throw FormatError(kReader, 0, what); }

std::uint16_t checked_u16(std::int64_t v, const char* what) {
    if (v < 0 || v > 0xFFFF) bad(std::string(what) + " does not fit a word");
    return static_cast<std::uint16_t>(v);
}

std::uint16_t word_at(ByteView b, std::uint64_t at) {
    io::ByteReader r(b, kReader);
    r.seek(at);
    return r.u16();
}

void put_u16(Bytes& b, std::uint64_t at, std::uint16_t v) {
    if (at + 2 > b.size()) bad("a word written past the end of the code");
    b[at] = static_cast<std::uint8_t>(v & 0xFF);
    b[at + 1] = static_cast<std::uint8_t>(v >> 8);
}

void append_u16(Bytes& b, std::uint16_t v) {
    b.push_back(static_cast<std::uint8_t>(v & 0xFF));
    b.push_back(static_cast<std::uint8_t>(v >> 8));
}

// image[a, b) as the program has it. The last function of a file may end at the image's end, and a stand-in may be
// asked for bytes the image does not have: the slice is clamped to the image (an inverted one is empty).
ByteView image_slice(ByteView image, std::int64_t a, std::int64_t b) {
    const auto size = static_cast<std::int64_t>(image.size());
    a = std::clamp<std::int64_t>(a, 0, size);
    b = std::clamp<std::int64_t>(b, a, size);
    return image.subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(b - a));
}

std::string name_at(io::ByteReader& r) {
    const std::uint8_t n = r.u8();
    return text::from_latin1(r.bytes(n));
}

// A name as an OMF record holds it (length-prefixed latin1 bytes) from the text the module's names are kept as.
Bytes name_bytes(const std::string& utf8) { return omf::name(as_text(text::to_latin1(utf8))); }

// "T<digits><suffix>" -- the names the compiler gives the far data and code segments of a module.
bool is_module_segment(std::string_view name, std::string_view suffix) {
    if (name.size() < 2 + suffix.size() || name.front() != 'T' || !name.ends_with(suffix)) return false;
    const std::string_view digits = name.substr(1, name.size() - 1 - suffix.size());
    return !digits.empty() && std::all_of(digits.begin(), digits.end(), [](char c) { return c >= '0' && c <= '9'; });
}

// ---- the module as the records give it -----------------------------------------------------------------------------

struct Model {
    omf::Object object;
    ObjectFile compare_object;                                  // the same, as the function comparer takes it
    std::vector<std::string> segments;                          // SEGDEF names, in order
    std::vector<std::string> externals;                         // EXTDEF names
    std::vector<std::string> groups;                            // GRPDEF names
    std::unordered_map<std::uint32_t, std::vector<Fixup>> fixes;   // by SEGDEF index: lookup only
    std::uint32_t text_index = 0;                               // 1-based SEGDEF index of the code segment
    std::string text_name;
    Bytes text;
    std::vector<omf::FunctionSpan> functions;
};

Model model_of(ByteView raw) {
    Model m;
    std::vector<std::string> names;
    FixupThreads threads;
    std::uint32_t last_segment = 0, last_base = 0;
    for (const omf::Record& rec : omf::records(raw)) {
        io::ByteReader r(rec.payload, kReader);
        switch (rec.type) {
            case kLnames:
                while (!r.eof()) names.push_back(name_at(r));
                break;
            case kSegdef: {
                const std::uint8_t attributes = r.u8();
                r.seek(attributes >> 5 ? 1 : 4);   // (an absolute segment names its frame and offset first)
                r.skip(2);
                const auto [ni, q] = read_index(rec.payload, r.pos());
                (void)q;
                if (ni == 0 || static_cast<std::size_t>(ni) > names.size()) bad("a SEGDEF names a name that is not there");
                m.segments.push_back(names[ni - 1]);
                break;
            }
            case kGrpdef: {
                const auto [gi, q] = read_index(rec.payload, 0);
                (void)q;
                if (gi == 0 || static_cast<std::size_t>(gi) > names.size()) bad("a GRPDEF names a name that is not there");
                m.groups.push_back(names[gi - 1]);
                break;
            }
            case kExtdef:
                while (!r.eof()) {
                    m.externals.push_back(name_at(r));
                    r.seek(read_index(rec.payload, r.pos()).second);   // (the type index)
                }
                break;
            case kLedata:
            case kLidata: {
                const auto [si, q] = read_index(rec.payload, 0);
                last_segment = si;
                io::ByteReader at(rec.payload, kReader);
                at.seek(q);
                last_base = at.u16();
                break;
            }
            case kFixupp:
                for (Fixup f : read_fixups(rec.payload, threads)) {
                    f.segment = last_segment;
                    f.at = last_base + f.offset;
                    m.fixes[last_segment].push_back(f);
                }
                break;
            default: break;
        }
    }
    m.object = omf::read(raw);
    const std::vector<std::string> code = omf::code_segments(m.object);
    if (code.empty()) bad("the module has no code segment");
    m.text_name = code.front();
    const auto seg = std::find(m.segments.begin(), m.segments.end(), m.text_name);
    if (seg == m.segments.end()) bad("the code segment has no SEGDEF");
    m.text_index = static_cast<std::uint32_t>(seg - m.segments.begin()) + 1;
    const Bytes* text = m.object.segments.find(m.text_name);
    if (!text) bad("the code segment holds no bytes");
    m.text = *text;
    m.functions = omf::functions(m.object, m.text_name);
    m.compare_object = object_of(m.object, raw);
    return m;
}

// The thread table at the end of the whole module (what every FIXUPP of the rewritten module is read with).
FixupThreads threads_of(ByteView raw) {
    FixupThreads threads;
    for (const omf::Record& rec : omf::records(raw))
        if (rec.type == kFixupp) (void)read_fixups(rec.payload, threads);
    return threads;
}

// What a module holds besides its code, as the comparison of two toolchains' objects wants it: the far data's
// segment names are the toolchain's own numbering and do not count.
std::vector<std::pair<std::string, Bytes>> data_of(const Model& m) {
    std::vector<std::pair<std::string, Bytes>> out;
    for (const auto& [name, bytes] : m.object.segments)
        if (name != m.text_name) out.emplace_back(is_module_segment(name, "_DATA") ? "far" : name, bytes);
    return out;
}

}  // namespace

// ---- OMF fix-ups ---------------------------------------------------------------------------------------------------

std::pair<std::uint16_t, std::size_t> read_index(ByteView record, std::size_t at) {
    io::ByteReader r(record, kReader);
    r.seek(at);
    const std::uint8_t first = r.u8();
    if (first < 0x80) return {first, r.pos()};
    const std::uint8_t second = r.u8();
    return {static_cast<std::uint16_t>(((first & 0x7F) << 8) | second), r.pos()};
}

void append_index(Bytes& out, std::uint16_t index) {
    if (index < 0x80) {
        out.push_back(static_cast<std::uint8_t>(index));
    } else {
        if (index >= 0x8000) bad("an index past 0x7fff");
        out.push_back(static_cast<std::uint8_t>(0x80 | (index >> 8)));
        out.push_back(static_cast<std::uint8_t>(index & 0xFF));
    }
}

std::vector<Fixup> read_fixups(ByteView record, FixupThreads& threads) {
    std::vector<Fixup> out;
    io::ByteReader r(record, "FIXUPP");
    while (!r.eof()) {
        const std::uint8_t c = r.u8();
        if (!(c & 0x80)) {   // a THREAD subrecord: define a thread
            const unsigned is_frame = (c >> 6) & 1, method = (c >> 2) & 7, number = c & 3;
            FixupThreads::Thread t{static_cast<std::uint8_t>(method), std::nullopt};
            if (!(is_frame && method >= 4)) {
                const auto [datum, q] = read_index(record, r.pos());
                t.datum = datum;
                r.seek(q);
            }
            if (is_frame) threads.define_frame(number, t);
            else threads.define_target(number, t);
            continue;
        }
        Fixup f;
        f.segment_relative = (c >> 6) & 1;
        f.location = (c >> 2) & 0xF;   // (four bits; the module this was ported from kept three: a 32-bit location read as another)
        f.offset = static_cast<std::uint16_t>(((c & 3) << 8) | r.u8());
        const std::uint8_t fix_data = r.u8();
        if (fix_data & 0x80) {
            const auto& t = threads.frame((fix_data >> 4) & 3);
            if (!t) throw FormatError("FIXUPP", r.pos(), "a fix-up names a frame thread nothing defined");
            f.frame_method = t->method;
            f.frame_datum = t->datum;
        } else {
            f.frame_method = (fix_data >> 4) & 7;
            if (f.frame_method < 3) {
                const auto [datum, q] = read_index(record, r.pos());
                f.frame_datum = datum;
                r.seek(q);
            }
        }
        if (fix_data & 8) {
            const auto& t = threads.target(fix_data & 3);
            if (!t || !t->datum) throw FormatError("FIXUPP", r.pos(), "a fix-up names a target thread nothing defined");
            f.target_method = t->method;
            f.target_datum = *t->datum;
        } else {
            f.target_method = fix_data & 3;
            const auto [datum, q] = read_index(record, r.pos());
            f.target_datum = datum;
            r.seek(q);
        }
        f.no_displacement = (fix_data >> 2) & 1;
        if (!f.no_displacement) f.displacement = r.u16();
        out.push_back(f);
    }
    return out;
}

Bytes write_fixup_record(std::span<const Fixup> fixups, std::uint32_t base) {
    Bytes b;
    for (const Fixup& f : fixups) {
        if (f.at < base || f.at - base >= 1024) bad(std::format("a fix-up at {:#x} is not within 1024 bytes of its record at {:#x}", f.at, base));
        const std::uint32_t o = f.at - base;
        b.push_back(static_cast<std::uint8_t>(0x80 | (f.segment_relative ? 0x40 : 0) | (f.location << 2) | (o >> 8)));
        b.push_back(static_cast<std::uint8_t>(o & 0xFF));
        b.push_back(static_cast<std::uint8_t>((f.frame_method << 4) | (f.no_displacement ? 4 : 0) | f.target_method));
        if (f.frame_method < 3) {
            if (!f.frame_datum) bad("a fix-up with a frame method that names a datum has none");
            append_index(b, *f.frame_datum);
        }
        append_index(b, f.target_datum);
        if (!f.no_displacement) append_u16(b, f.displacement);
    }
    return omf::record(kFixupp, b);
}

// ---- the stand-in object ---------------------------------------------------------------------------------------------

namespace {

// Where each function of the rebuilt code stands: its place in the new code, and where the program has it.
struct Placed {
    std::string name;
    std::optional<std::pair<std::size_t, const Model*>> source;   // function number in `source->functions`; none: a stand-in
    std::size_t new_at = 0;    // offset in the rebuilt code
    std::int64_t at = 0;       // the program's place
    std::int64_t end = 0;      // where the program's function ends (the next one's start)
};

struct Output {
    Bytes head, code, data, tail;
    enum class Part { Head, Code, Data, Tail } current = Part::Head;
    Bytes& cur() {
        switch (current) {
            case Part::Head: return head;
            case Part::Code: return code;
            case Part::Data: return data;
            case Part::Tail: break;
        }
        return tail;
    }
    void add(ByteView b) { append(cur(), b); }
    Bytes all() const {
        Bytes out = head;
        append(out, code);
        append(out, data);
        append(out, tail);
        return out;
    }
};

}  // namespace

Rebuilt rebuild(const Program& program, ByteView raw, std::span<const std::uint32_t> addrs, std::uint32_t end,
                std::optional<std::string_view> title, std::span<const Bytes> alts, const x86::Decoder16& decoder) {
    const ByteView img = program.image;
    const FixupThreads all_threads = threads_of(raw);
    const std::vector<omf::Record> recs = omf::records(raw);
    const Model base = model_of(raw);
    if (base.functions.size() != addrs.size())
        bad(std::format("{}: {} functions compiled, {} places", title.value_or("module"), base.functions.size(), addrs.size()));

    // (the other toolchains' objects count only when their data is the same)
    std::vector<Model> others;
    for (const Bytes& x : alts) {
        Model a = model_of(x);
        if (a.functions.size() == addrs.size() && data_of(a) == data_of(base)) others.push_back(std::move(a));
    }
    std::vector<const Model*> candidates = {&base};
    for (const Model& a : others) candidates.push_back(&a);

    // The comparisons extend the table with the names they learn; a function is held to the project's table only, so
    // what a comparison learnt is taken out again.
    SymbolTable symbols = program.symbols;
    Bytes code;
    std::vector<Placed> placed;
    Rebuilt result;
    for (std::size_t k = 0; k < addrs.size(); ++k) {
        const std::int64_t at = addrs[k];
        std::int64_t next = k + 1 < addrs.size() ? addrs[k + 1] : end;
        Placed p;
        p.name = base.functions[k].name;
        for (const Model* m : candidates) {
            const omf::FunctionSpan& fn = m->functions[k];
            // (each of the file's functions at its own place: one before it may be of another length - a stand-in)
            const compare::Where where = [&](std::uint32_t x) -> std::int64_t {
                for (std::size_t j = 0; j < m->functions.size(); ++j) {
                    const auto& f = m->functions[j];
                    if (f.offset <= x && x < f.offset + std::max<std::int64_t>(f.length, 1))
                        return static_cast<std::int64_t>(addrs[j]) + x - f.offset;
                }
                return at - fn.offset + x;
            };
            const compare::Subject subject{m->compare_object, fn.name, static_cast<std::uint32_t>(fn.offset),
                                           static_cast<std::uint32_t>(fn.length), static_cast<std::uint32_t>(at), m->text_name};
            SymbolTable learnt;
            const compare::Report report =
                compare::function(subject, img, program.compare, symbols, learnt, where, decoder);
            for (const auto& entry : learnt) symbols.erase(entry.first);
            // (the last function may be followed by zeros that are not the module's - a segment of the next ones
            // begins with them - but its pad to an even place is)
            const std::int64_t length = fn.length;
            const ByteView after = image_slice(img, at + length, next);
            const bool tail = k + 1 == addrs.size() && length < next - at &&
                              std::all_of(after.begin(), after.end(), [](std::uint8_t b) { return b == 0; });
            if (report.identical() && (length == next - at || tail)) {
                if (tail) next = std::min(next, at + length + ((at + length) & 1));
                p.source = std::make_pair(k, m);
                break;
            }
        }
        p.new_at = code.size();
        p.at = at;
        p.end = next;
        if (!p.source) {
            append(code, image_slice(img, at, next));
            result.stand_ins.push_back(p.name);
        } else {
            const omf::FunctionSpan& fn = p.source->second->functions[p.source->first];
            const Bytes& text = p.source->second->text;
            const auto from = static_cast<std::size_t>(fn.offset), len = static_cast<std::size_t>(fn.length);
            if (from + len > text.size()) bad("a function lies past its code segment");
            append(code, ByteView(text).subspan(from, len));
            if (next > at + fn.length) append(code, image_slice(img, at + fn.length, next));   // (the module's pad to an even place)
        }
        placed.push_back(std::move(p));
    }

    // Where a place of the model's code is in the new code (and the function it is in); the very end counts for the last.
    const auto moved = [&](const Model& m, std::uint32_t v) -> std::optional<std::int64_t> {
        for (std::size_t k = 0; k < m.functions.size(); ++k) {
            const auto& fn = m.functions[k];
            if ((fn.offset <= v && v < fn.offset + fn.length) || (k + 1 == m.functions.size() && v == fn.offset + fn.length))
                return static_cast<std::int64_t>(placed[k].new_at) + v - fn.offset;
        }
        return std::nullopt;
    };

    std::vector<std::string> extnames = base.externals;
    std::vector<Fixup> fixes;
    // a stand-in's segment words (a far call's, DGROUP's): the program's relocations there are fixups again, so that
    // the linker lists them; the word itself is the linker's to fill
    std::unordered_map<std::int64_t, std::string> by_address;   // lookup only: the function (or whatever is `C`) at a place
    for (const auto& [name, place] : program.symbols)
        if (place.kind == SymKind::Code && name.find(':') == std::string::npos) by_address[place.address] = name;
    const auto external_name = [](const Model& m, std::size_t number) -> const std::string& {
        if (number == 0 || number > m.externals.size()) throw FormatError(kReader, 0, "a fix-up names an external that is not there");
        return m.externals[number - 1];
    };
    const auto extern_number = [&](const std::string& name) {
        auto it = std::find(extnames.begin(), extnames.end(), name);
        if (it == extnames.end()) it = extnames.insert(extnames.end(), name);
        return static_cast<std::uint16_t>(it - extnames.begin() + 1);
    };
    for (const Placed& p : placed) {
        if (p.source) continue;
        for (const std::uint32_t s : program.relocations) {
            if (!(p.at <= s && s < p.end) || s + 2 > p.end) continue;
            const std::size_t k = p.new_at + s - p.at;
            const std::uint16_t w = word_at(img, s);
            put_u16(code, k, 0);   // (the linker adds the segment to what is there)
            Fixup g;
            g.segment_relative = true;
            g.location = 2;       // a base: the segment word
            g.frame_method = 5;   // the target's own frame
            g.no_displacement = true;
            g.at = static_cast<std::uint32_t>(k);
            if (s >= 3 && img[s - 3] == 0x9A && s - 3 >= p.at) {   // a far call: the function it calls
                const std::int64_t dest = (static_cast<std::int64_t>(w) - program.compare.loadseg) * 16 + word_at(img, s - 2);
                const auto name = by_address.find(dest);
                if (name == by_address.end())
                    bad(std::format("{}: a far call to {:05x}, which SYMBOLS has no name for", p.name, dest));
                g.target_method = 2;   // an external
                g.target_datum = extern_number(name->second);
            } else if ((static_cast<std::int64_t>(w) - program.compare.loadseg) * 16 == program.compare.dgroup &&
                       std::find(base.groups.begin(), base.groups.end(), "DGROUP") != base.groups.end()) {
                g.target_method = 1;   // a group
                g.target_datum = static_cast<std::uint16_t>(std::find(base.groups.begin(), base.groups.end(), "DGROUP") - base.groups.begin() + 1);
            } else {
                bad(std::format("{}: a segment word {:04x} at {:05x} of no kind known", p.name, w, s));
            }
            fixes.push_back(g);
        }
    }
    for (const Placed& p : placed) {
        if (!p.source) continue;
        const Model& m = *p.source->second;
        const omf::FunctionSpan& fn = m.functions[p.source->first];
        const auto found = m.fixes.find(m.text_index);
        if (found == m.fixes.end()) continue;
        for (const Fixup& f : found->second) {
            if (!(fn.offset <= f.at && f.at < fn.offset + fn.length)) continue;
            Fixup g = f;
            g.at = static_cast<std::uint32_t>(p.new_at + f.at - fn.offset);
            if (f.target_method == 0 && f.target_datum == m.text_index) {   // a place of this file's own code: it has moved
                const std::uint16_t v = word_at(m.text, f.at);
                const auto v2 = moved(m, v);
                put_u16(code, g.at, static_cast<std::uint16_t>((v2 ? *v2 : v) & 0xFFFF));
            }
            if (&m != &base) {   // (the other module numbers its names its own way)
                if (f.target_method == 2) g.target_datum = extern_number(external_name(m, f.target_datum));
                if (f.frame_method == 2 && f.frame_datum) g.frame_datum = extern_number(external_name(m, *f.frame_datum));
            }
            fixes.push_back(g);
        }
    }
    std::vector<std::uint32_t> fixed_at;   // the words the fix-ups cover (membership only)
    for (const Fixup& f : fixes) fixed_at.push_back(f.at);
    std::sort(fixed_at.begin(), fixed_at.end());
    const auto is_fixed = [&](std::uint32_t at) { return std::binary_search(fixed_at.begin(), fixed_at.end(), at); };
    // calls and jumps between the file's functions that the compiler filled itself
    for (const Placed& p : placed) {
        if (!p.source) continue;
        const Model& m = *p.source->second;
        const omf::FunctionSpan& fn = m.functions[p.source->first];
        for (const x86::Insn& i : x86::disasm(m.text, static_cast<std::uint32_t>(fn.offset), static_cast<std::uint32_t>(fn.offset + fn.length),
                                              false, decoder)) {
            if ((i.mnemonic == "call" || i.mnemonic == "jmp") && i.size() == 3 && (m.text[i.address] == 0xE8 || m.text[i.address] == 0xE9)) {
                const std::int64_t here = static_cast<std::int64_t>(p.new_at) + i.address - fn.offset;
                if (is_fixed(static_cast<std::uint32_t>(here + 1))) continue;
                const auto displacement = static_cast<std::int16_t>(word_at(m.text, i.address + 1u));
                const std::uint32_t target = (i.address + 3 + displacement) & 0xFFFF;
                if (!(fn.offset <= target && target < fn.offset + fn.length)) {
                    if (const auto t2 = moved(m, target)) {
                        const std::int64_t rel = ((*t2 - (here + 3) + 0x8000) & 0xFFFF) - 0x8000;
                        put_u16(code, static_cast<std::uint64_t>(here + 1), static_cast<std::uint16_t>(rel & 0xFFFF));
                    }
                }
            }
        }
    }
    const std::vector<std::string> new_externals(extnames.begin() + static_cast<std::ptrdiff_t>(base.externals.size()), extnames.end());

    // the move of a PUBDEF's offset: where the model's place is in the new code, as a difference
    const auto where = [&](std::uint32_t x) -> std::int64_t {
        const auto v = moved(base, x);
        return v ? *v - x : 0;
    };

    // ---- the records again; a file's code (and the segment words its code names, in CONST) before its data ---------------
    Output out;
    const auto const_index = std::find(base.segments.begin(), base.segments.end(), "CONST");
    const std::uint32_t ci = const_index == base.segments.end() ? 0 : static_cast<std::uint32_t>(const_index - base.segments.begin()) + 1;
    bool done_text = false, skip_fixups = false, externals_done = false;
    std::uint32_t last_base = 0, segment_number = 0;
    for (const omf::Record& rec : recs) {
        const ByteView r = rec.payload;
        const std::uint8_t t = rec.type;
        if (t == kTheadr && title) {
            out.add(omf::record(t, omf::name(*title)));
            continue;
        }
        if (t == kLnames && title) {   // (the far data's segment is named after the file)
            Bytes renamed;
            io::ByteReader nr(r, "LNAMES");
            while (!nr.eof()) {
                const std::uint8_t n = nr.u8();
                std::string s(as_text(nr.bytes(n)));   // (the bytes as they are: renamed or kept)
                if (is_module_segment(s, "_DATA")) s = std::string(*title) + "_DATA";
                else if (is_module_segment(s, "_TEXT")) s = std::string(*title) + "_TEXT";
                append(renamed, omf::name(s));
            }
            out.add(omf::record(t, renamed));
            continue;
        }
        if (t == kSegdef) {
            ++segment_number;
            if (segment_number == base.text_index) {   // the code segment's new length
                const std::size_t q = r.empty() ? 0 : (r[0] >> 5 ? 1 : 4);
                if (q + 2 > r.size()) throw TruncatedError("SEGDEF", q, 2, r.size() > q ? r.size() - q : 0);
                Bytes changed(r.begin(), r.end());
                put_u16(changed, q, checked_u16(static_cast<std::int64_t>(code.size()), "the code's length"));
                out.add(omf::record(t, changed));
            } else {
                out.add(omf::record(t, r));
            }
            continue;
        }
        if ((t == kPubdef || t == kLedata || t == kLidata || t == kModend) && !new_externals.empty() && !externals_done) {
            externals_done = true;
            out.current = Output::Part::Head;
            Bytes names;
            for (const std::string& n : new_externals) {
                append(names, name_bytes(n));
                names.push_back(0);   // (no type)
            }
            out.add(omf::record(kExtdef, names));
        }
        if (t == kPubdef) {
            auto [gi, q] = read_index(r, 0);
            (void)gi;
            const auto [si, q2] = read_index(r, q);
            q = q2;
            std::size_t head_end = q;
            if (si == 0) head_end = q + 2;   // (an absolute symbol: its base frame)
            if (head_end > r.size()) throw TruncatedError("PUBDEF", q, 2, r.size() - q);
            Bytes record(r.begin(), r.begin() + static_cast<std::ptrdiff_t>(head_end));
            io::ByteReader pr(r, "PUBDEF");
            pr.seek(head_end);
            while (!pr.eof()) {
                const std::uint8_t n = pr.u8();
                pr.seek(pr.pos() - 1);
                append(record, pr.bytes(1u + n));   // the name, length-prefixed
                std::uint16_t offset = pr.u16();
                if (si == base.text_index) offset = checked_u16(static_cast<std::int64_t>(offset) + where(offset), "a moved public's offset");
                append_u16(record, offset);
                const std::size_t type_at = pr.pos();
                const auto [type_index, after] = read_index(r, type_at);
                (void)type_index;
                append(record, r.subspan(type_at, after - type_at));
                pr.seek(after);
            }
            out.add(omf::record(t, record));
            continue;
        }
        if (t == kLedata || t == kLidata) {
            const auto [si, q] = read_index(r, 0);
            last_base = word_at(r, q);
            out.current = (si == base.text_index || si == ci) ? Output::Part::Code : Output::Part::Data;
            if (si == base.text_index) {
                skip_fixups = true;
                if (!done_text) {
                    done_text = true;
                    std::vector<Fixup> sorted = fixes;
                    std::sort(sorted.begin(), sorted.end(), [](const Fixup& a, const Fixup& b) { return a.at < b.at; });
                    std::size_t p = 0;
                    while (p < code.size()) {
                        std::size_t e = std::min(p + 1000, code.size());
                        for (const Fixup& f : sorted) {   // (no fixup's word across two records)
                            const std::size_t width = f.location == 3 ? 4 : 2;
                            if (f.at < e && e < f.at + width) e = f.at;
                        }
                        if (e <= p) bad("a fix-up's word cannot be kept inside one record");
                        Bytes data_record;
                        append_index(data_record, static_cast<std::uint16_t>(base.text_index));
                        append_u16(data_record, checked_u16(static_cast<std::int64_t>(p), "a record's offset"));
                        append(data_record, ByteView(code).subspan(p, e - p));
                        out.add(omf::record(kLedata, data_record));
                        std::vector<Fixup> here;
                        for (const Fixup& f : sorted)
                            if (p <= f.at && f.at < e) here.push_back(f);
                        if (!here.empty()) out.add(write_fixup_record(here, static_cast<std::uint32_t>(p)));
                        p = e;
                    }
                }
                continue;
            }
            skip_fixups = false;
            out.add(omf::record(t, r));
            continue;
        }
        if (t == kFixupp) {
            if (skip_fixups) continue;
            FixupThreads copy = all_threads;
            std::vector<Fixup> mine = read_fixups(r, copy);
            if (!mine.empty()) {
                for (Fixup& f : mine) f.at = last_base + f.offset;
                out.add(write_fixup_record(mine, last_base));
            }
            continue;
        }
        if (t == kModend || t == kModend32) out.current = Output::Part::Tail;
        else out.current = Output::Part::Head;
        out.add(omf::record(t, r));
    }
    result.object = out.all();
    return result;
}

}  // namespace cb::exact::standin
