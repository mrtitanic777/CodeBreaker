// symbols/cpp_classes_x64.cpp -- see cpp_classes_x64.hpp.
#include "symbols/cpp_classes_x64.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <tuple>

#include "symbols/demangle.hpp"

namespace cb::symbols::cpp_classes_x64 {

namespace {

// The image's memory as its sections lay it out: the raw bytes of each, read by RVA.
class RttiMemory {
public:
    explicit RttiMemory(const formats::pe::Image& image) : image_base_(image.header().image_base) {
        for (const formats::pe::Section& s : image.sections()) {
            RttiPiece p;
            p.rva = s.virtual_address;
            p.bytes = image.section_data(s);
            p.code = s.is_code();
            p.size = std::max(s.virtual_size, s.raw_size);
            pieces_.push_back(p);
        }
    }
    std::uint64_t image_base() const { return image_base_; }

    std::optional<std::uint32_t> u32(std::uint64_t rva) const {
        const ByteView b = at(rva, 4);
        if (b.size() < 4) return std::nullopt;
        return static_cast<std::uint32_t>(b[0] | (b[1] << 8) | (b[2] << 16)) | (static_cast<std::uint32_t>(b[3]) << 24);
    }
    std::optional<std::uint64_t> u64(std::uint64_t rva) const {
        const auto lo = u32(rva), hi = u32(rva + 4);
        if (!lo || !hi) return std::nullopt;
        return *lo | (static_cast<std::uint64_t>(*hi) << 32);
    }
    // The zero-terminated text at `rva` (at most `limit` bytes); nullopt when it does not end in the section.
    std::optional<std::string> text(std::uint64_t rva, std::size_t limit = 4096) const {
        const ByteView b = at(rva, limit);
        std::string out;
        for (const std::uint8_t c : b) {
            if (c == 0) return out;
            out.push_back(static_cast<char>(c));
        }
        return std::nullopt;
    }
    bool is_code_va(std::uint64_t va) const {
        if (va < image_base_) return false;
        const std::uint64_t rva = va - image_base_;
        for (const RttiPiece& p : pieces_)
            if (p.code && rva >= p.rva && rva < p.rva + p.size) return true;
        return false;
    }
    bool is_data_rva(std::uint64_t rva) const {
        for (const RttiPiece& p : pieces_)
            if (!p.code && rva >= p.rva && rva < p.rva + p.bytes.size()) return true;
        return false;
    }
    template <class F>
    void each_data_piece(F f) const {
        for (const RttiPiece& p : pieces_)
            if (!p.code) f(p.rva, p.bytes);
    }

private:
    struct RttiPiece {
        std::uint64_t rva = 0;
        ByteView bytes;
        std::uint64_t size = 0;
        bool code = false;
    };
    // Up to `n` bytes from `rva`, to the end of the section's raw bytes.
    ByteView at(std::uint64_t rva, std::size_t n) const {
        for (const RttiPiece& p : pieces_)
            if (rva >= p.rva && rva < p.rva + p.bytes.size()) {
                const std::size_t off = static_cast<std::size_t>(rva - p.rva);
                return p.bytes.subspan(off, std::min(n, p.bytes.size() - off));
            }
        return {};
    }
    std::uint64_t image_base_ = 0;
    std::vector<RttiPiece> pieces_;
};

bool rtti_decorated_class(std::string_view s) { return s.size() > 4 && s.substr(0, 3) == ".?A" && (s[3] == 'V' || s[3] == 'U'); }

// The decorated name a type descriptor holds (after type_info's vftable pointer and the spare pointer).
std::optional<std::string> rtti_descriptor_name(const RttiMemory& m, std::uint32_t td) {
    if (td == 0 || !m.is_data_rva(td)) return std::nullopt;
    std::optional<std::string> s = m.text(td + 16, 1024);
    if (!s || !rtti_decorated_class(*s)) return std::nullopt;
    return s;
}

std::string rtti_readable(const std::string& decorated) {
    if (const std::optional<std::string> n = demangle::type_name(decorated); n && !n->empty()) return *n;
    return decorated;
}

// A locator read and checked: its offset, its class's decorated name and its hierarchy descriptor.
struct RttiLocator {
    std::uint32_t offset = 0;
    std::string decorated;
    std::uint32_t chd = 0;
};
std::optional<RttiLocator> rtti_read_locator(const RttiMemory& m, std::uint32_t rva) {
    const auto sig = m.u32(rva), off = m.u32(rva + 4), td = m.u32(rva + 12), chd = m.u32(rva + 16), self = m.u32(rva + 20);
    if (!sig || *sig != 1 || !self || *self != rva || !off || !td || !chd) return std::nullopt;
    std::optional<std::string> name = rtti_descriptor_name(m, *td);
    if (!name || !m.is_data_rva(*chd)) return std::nullopt;
    return RttiLocator{*off, std::move(*name), *chd};
}

// The bases a hierarchy descriptor lists, the class itself first; nullopt when it does not read.
std::optional<std::pair<std::uint32_t, std::vector<BaseClass>>> rtti_read_hierarchy(const RttiMemory& m, std::uint32_t chd) {
    const auto sig = m.u32(chd), attributes = m.u32(chd + 4), count = m.u32(chd + 8), array = m.u32(chd + 12);
    if (!sig || *sig != 0 || !attributes || !count || !array || *count == 0 || *count > 4096 || !m.is_data_rva(*array)) return std::nullopt;
    std::vector<BaseClass> out;
    for (std::uint32_t k = 0; k < *count; ++k) {
        const auto bcd = m.u32(*array + 4 * k);
        if (!bcd || !m.is_data_rva(*bcd)) return std::nullopt;
        const auto td = m.u32(*bcd), contained = m.u32(*bcd + 4), mdisp = m.u32(*bcd + 8), pdisp = m.u32(*bcd + 12), vdisp = m.u32(*bcd + 16),
                   attrs = m.u32(*bcd + 20);
        if (!td || !contained || !mdisp || !pdisp || !vdisp || !attrs) return std::nullopt;
        std::optional<std::string> name = rtti_descriptor_name(m, *td);
        if (!name) return std::nullopt;
        BaseClass b;
        b.decorated = *name;
        b.name = rtti_readable(*name);
        b.contained = *contained;
        b.mdisp = static_cast<std::int32_t>(*mdisp);
        b.pdisp = static_cast<std::int32_t>(*pdisp);
        b.vdisp = static_cast<std::int32_t>(*vdisp);
        b.attributes = *attrs;
        out.push_back(std::move(b));
    }
    // the direct bases: the entries at the top of the pre-order tree under the class (each followed by its own contained bases)
    for (std::size_t k = 1; k < out.size(); k += out[k].contained + 1) out[k].direct = true;
    return std::make_pair(*attributes, std::move(out));
}

}  // namespace

const VTable* Class::primary() const {
    for (const VTable& t : vtables)
        if (t.offset == 0) return &t;
    return nullptr;
}

const BaseClass* Class::primary_base() const {
    for (const BaseClass& b : bases)
        if (b.direct && b.mdisp == 0 && !b.is_virtual()) return &b;
    return nullptr;
}

const Class* Classes::by_vtable(std::uint64_t vtable) const {
    for (const auto& [decorated, c] : classes)
        if (const VTable* t = c.primary(); t != nullptr && t->address == vtable) return &c;
    return nullptr;
}

Classes find_classes(const formats::pe::Image& image) {
    Classes out;
    if (image.header().machine != formats::pe::kMachineX64) return out;
    const RttiMemory m(image);
    // the locators, in image order
    std::vector<std::uint32_t> locators;
    std::map<std::uint32_t, RttiLocator> read;
    m.each_data_piece([&](std::uint64_t base, ByteView bytes) {
        for (std::size_t k = 0; k + 24 <= bytes.size(); k += 4) {
            if (bytes[k] != 1 || bytes[k + 1] != 0 || bytes[k + 2] != 0 || bytes[k + 3] != 0) continue;
            const std::uint32_t rva = static_cast<std::uint32_t>(base + k);
            if (std::optional<RttiLocator> l = rtti_read_locator(m, rva)) {
                locators.push_back(rva);
                read.emplace(rva, std::move(*l));
            }
        }
    });
    if (locators.empty()) return out;
    // the tables: a pointer to a locator, then the slots
    std::map<std::uint32_t, std::vector<std::uint64_t>> tables_of;   // locator -> the tables after a pointer to it
    std::set<std::uint64_t> table_starts;
    const std::uint64_t base_va = m.image_base();
    m.each_data_piece([&](std::uint64_t base, ByteView bytes) {
        for (std::size_t k = 0; k + 16 <= bytes.size(); k += 8) {
            std::uint64_t v = 0;
            for (int i = 7; i >= 0; --i) v = (v << 8) | bytes[k + static_cast<std::size_t>(i)];
            if (v < base_va) continue;
            const std::uint64_t rva = v - base_va;
            if (rva > 0xFFFFFFFFu || !read.contains(static_cast<std::uint32_t>(rva))) continue;
            tables_of[static_cast<std::uint32_t>(rva)].push_back(base_va + base + k + 8);
            table_starts.insert(base_va + base + k + 8);
        }
    });
    std::set<std::uint64_t> slot_functions;
    for (const std::uint32_t rva : locators) {
        const RttiLocator& l = read.at(rva);
        const auto hierarchy = rtti_read_hierarchy(m, l.chd);
        if (!hierarchy || hierarchy->second.empty() || hierarchy->second.front().decorated != l.decorated) continue;
        Class* c = out.classes.find(l.decorated);
        if (c == nullptr) {
            Class fresh;
            fresh.decorated = l.decorated;
            fresh.name = rtti_readable(l.decorated);
            fresh.attributes = hierarchy->first;
            fresh.bases.assign(hierarchy->second.begin() + 1, hierarchy->second.end());
            out.classes.insert_or_assign(l.decorated, std::move(fresh));
            c = out.classes.find(l.decorated);
        }
        for (const std::uint64_t at : tables_of[rva]) {
            VTable t;
            t.address = at;
            t.locator = base_va + rva;
            t.offset = l.offset;
            // (the slots end at the next table's locator pointer, which is not code, or at its first slot)
            for (std::uint64_t slot = at; slot == at || !table_starts.contains(slot); slot += 8) {
                const auto v = m.u64(slot - base_va);
                if (!v || !m.is_code_va(*v)) break;
                t.slots.push_back(*v);
                slot_functions.insert(*v);
            }
            if (t.slots.empty()) continue;
            c->vtables.push_back(std::move(t));
        }
    }
    for (auto& [decorated, c] : out.classes)
        std::sort(c.vtables.begin(), c.vtables.end(), [](const VTable& a, const VTable& b) { return std::tie(a.offset, a.address) < std::tie(b.offset, b.address); });
    out.slot_functions.assign(slot_functions.begin(), slot_functions.end());
    return out;
}

}  // namespace cb::symbols::cpp_classes_x64
