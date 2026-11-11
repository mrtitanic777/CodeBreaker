#include "cb/exact/aout.hpp"

#include <array>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::aout {
namespace {

constexpr std::string_view kReader = "a.out";
constexpr std::size_t kHeaderSize = 16;
constexpr std::size_t kSymbolSize = 12;       // 8 name characters, a type word, a value word
constexpr std::uint16_t kSymExternal = 0x20;  // the type bit that says the symbol is global

// Symbol type bits 0-2: what the symbol is in.
constexpr std::uint16_t kInText = 2, kInData = 3, kInBss = 4, kUndefined = 0;

std::uint16_t word_at(ByteView data, std::uint64_t offset) {
    io::ByteReader r(data, kReader);
    r.seek(offset);
    return r.u16();
}

std::string_view segment_name(std::uint16_t kind) {
    switch (kind) {
        case 1: return "_TEXT";
        case 2: return "_DATA";
        case 3: return "_BSS";
        default: return "?";   // a word naming nothing known: kept, the linker is the one to refuse it
    }
}

}  // namespace

bool is_aout(ByteView data) {
    if (data.size() < kHeaderSize) return false;
    const std::uint16_t magic = static_cast<std::uint16_t>(data[0] | (data[1] << 8));
    return magic == 0407 || magic == 0410 || magic == 0411;
}

Image read(ByteView data) {
    io::ByteReader header(data, kReader);
    header.u16();   // magic: the caller's business (is_aout)
    const std::uint32_t text_size = header.u16();
    const std::uint32_t data_size = header.u16();
    const std::uint32_t bss_size = header.u16();
    const std::uint32_t symbol_bytes = header.u16();
    header.u16();   // entry
    header.u16();
    const bool no_relocation = header.u16() != 0;

    Image image;
    ObjectFile& o = image.object;
    o.kind = ObjectKind::Aout;
    o.raw.assign(data.begin(), data.end());

    io::ByteReader body(data, kReader);
    body.seek(kHeaderSize);
    o.seg.insert_or_assign("_TEXT", to_bytes(as_text(body.bytes(text_size))));
    o.seg.insert_or_assign("_DATA", to_bytes(as_text(body.bytes(data_size))));
    o.seg.insert_or_assign("_BSS", Bytes(bss_size));
    o.filled.insert_or_assign("_TEXT", text_size);
    o.filled.insert_or_assign("_DATA", data_size);
    o.segs = {{"_TEXT", "CODE", text_size, 1}, {"_DATA", "DATA", data_size, 2}, {"_BSS", "BSS", bss_size, 2}};

    // One relocation word for every word of the text, then of the data.
    std::size_t relocs_at = body.pos();
    std::vector<std::uint16_t> text_relocs, data_relocs;
    if (!no_relocation) {
        for (std::uint32_t k = 0; k < text_size; k += 2) text_relocs.push_back(word_at(data, relocs_at + k));
        for (std::uint32_t k = 0; k < data_size; k += 2) data_relocs.push_back(word_at(data, relocs_at + text_size + k));
        body.skip(static_cast<std::uint64_t>(text_size) + data_size);
    }

    for (std::uint64_t q = body.pos(); q < body.pos() + symbol_bytes; q += kSymbolSize) {
        io::ByteReader s(data, kReader);
        s.seek(q);
        ByteView name_field = s.bytes(8);
        std::size_t len = 0;
        while (len < name_field.size() && name_field[len] != 0) ++len;
        Symbol sym;
        sym.name = text::from_latin1(name_field.first(len));
        sym.type = s.u16();
        sym.value = s.u16();
        image.symbols.push_back(std::move(sym));
    }

    // Publics and externals. A value counts from its own segment's start, as the relocated words do: XENIX C's
    // objects keep instructions and data apart.
    for (const Symbol& s : image.symbols) {
        if (!(s.type & kSymExternal)) continue;
        const std::uint16_t in = s.type & 7;
        const char* seg = in == kInText ? "_TEXT" : in == kInData ? "_DATA" : in == kInBss ? "_BSS" : nullptr;
        if (seg != nullptr) {
            o.pubs.push_back({s.name, std::string(seg), s.value});
        } else if (in == kUndefined) {
            o.ext.push_back(s.name);
            if (s.value != 0) o.comm.insert_or_assign(s.name, CommonVar{s.value, false});   // an external with a size
        }
    }

    for (const auto& [segment, words] : {std::pair<const char*, const std::vector<std::uint16_t>*>{"_TEXT", &text_relocs},
                                         {"_DATA", &data_relocs}}) {
        for (std::size_t j = 0; j < words->size(); ++j) {
            const std::uint16_t v = (*words)[j];
            if (v == 0) continue;
            const std::uint32_t at = static_cast<std::uint32_t>(2 * j + (v >> 15));   // bit 15: the operand is a byte on
            const std::uint16_t names = (v >> 1) & 7;
            Fixup f;
            f.segment = segment;
            f.offset = at;
            if (names == 4) {
                const std::size_t index = (v & 0x7FFFu) >> 4;
                if (index >= image.symbols.size())
                    throw FormatError(kReader, at, std::format("relocation names symbol {} of {}", index, image.symbols.size()));
                f.target = {TargetKind::External, image.symbols[index].name};
            } else {
                f.target = {TargetKind::Segment, std::string(segment_name(names))};
            }
            if (v & 1) {   // relative to the word's end
                f.kind = FixKind::Rel;
                f.displacement = static_cast<std::int32_t>(at + 2);
            } else {
                f.kind = FixKind::Off;
            }
            o.fix.push_back(std::move(f));
        }
    }
    return image;
}

}  // namespace cb::exact::aout
