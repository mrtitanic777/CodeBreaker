// symbols/cpp_classes.cpp -- see cpp_classes.hpp.
#include "symbols/cpp_classes.hpp"

#include <algorithm>
#include <format>
#include <functional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "core/json.hpp"
#include "core/text.hpp"
#include "symbols/demangle.hpp"

namespace cb::symbols::cpp_classes {

// ---- what every target shares ------------------------------------------------------------------------------------------

namespace {

// A decorated name as C++ writes it; the decorated name itself when it cannot be read.
std::string rtti_readable(const std::string& decorated) {
    if (const std::optional<std::string> n = demangle::type_name(decorated); n && !n->empty()) return *n;
    return decorated;
}

// How one target's records are read: a word of a record (in the target's byte order) at an address of the space its refs are in,
// whether a ref lands where records may be, and the type descriptor a ref names.
struct RttiView {
    std::function<std::optional<std::uint32_t>(std::uint64_t at)> u32;
    std::function<bool(std::uint64_t ref)> holds_records;
    struct Named {
        std::string decorated, name;
    };
    std::function<std::optional<Named>(std::uint32_t ref)> descriptor;
    std::uint32_t most_bases = 4096;               // a hierarchy with more is not one
    std::uint32_t attribute_bits = ~std::uint32_t{0};   // a hierarchy with an attribute outside these is not one
};

struct RttiHierarchy {
    std::uint32_t attributes = 0;
    std::vector<BaseClass> bases;   // the class itself first
};

// The hierarchy descriptor at `chd`: its attributes and the bases its array lists, the class itself first; nullopt when it does not
// read as one.
std::optional<RttiHierarchy> rtti_hierarchy(const RttiView& view, std::uint64_t chd) {
    const auto sig = view.u32(chd), attributes = view.u32(chd + 4), count = view.u32(chd + 8), array = view.u32(chd + 12);
    if (!sig || *sig != 0 || !attributes || !count || !array || *count == 0 || *count > view.most_bases) return std::nullopt;
    if ((*attributes & ~view.attribute_bits) != 0 || !view.holds_records(*array)) return std::nullopt;
    RttiHierarchy out;
    out.attributes = *attributes;
    for (std::uint32_t k = 0; k < *count; ++k) {
        const auto bcd = view.u32(std::uint64_t{*array} + 4 * std::uint64_t{k});
        if (!bcd || !view.holds_records(*bcd)) return std::nullopt;
        const std::uint64_t at = *bcd;
        const auto td = view.u32(at), contained = view.u32(at + 4), mdisp = view.u32(at + 8), pdisp = view.u32(at + 12), vdisp = view.u32(at + 16),
                   attrs = view.u32(at + 20);
        if (!td || !contained || !mdisp || !pdisp || !vdisp || !attrs) return std::nullopt;
        std::optional<RttiView::Named> named = view.descriptor(*td);
        if (!named) return std::nullopt;
        BaseClass b;
        b.decorated = std::move(named->decorated);
        b.name = std::move(named->name);
        b.contained = *contained;
        b.mdisp = static_cast<std::int32_t>(*mdisp);
        b.pdisp = static_cast<std::int32_t>(*pdisp);
        b.vdisp = static_cast<std::int32_t>(*vdisp);
        b.attributes = *attrs;
        out.bases.push_back(std::move(b));
    }
    // the direct bases: the entries at the top of the pre-order tree under the class (each followed by its own contained bases)
    for (std::size_t k = 1; k < out.bases.size(); k += out.bases[k].contained + 1) out.bases[k].direct = true;
    return out;
}

// A class as its locator's hierarchy gives it (no tables yet).
template <class Address>
BasicClass<Address> rtti_class(const RttiView::Named& named, RttiHierarchy hierarchy) {
    BasicClass<Address> c;
    c.name = named.name;
    c.decorated = named.decorated;
    c.attributes = hierarchy.attributes;
    c.bases.assign(std::make_move_iterator(hierarchy.bases.begin() + 1), std::make_move_iterator(hierarchy.bases.end()));
    return c;
}

// The tables of every class by sub-object offset, then address; and every function a table holds.
template <class Address>
void rtti_order_tables(BasicClasses<Address>& out) {
    std::set<Address> functions;
    for (auto& [key, c] : out.classes) {
        std::sort(c.vtables.begin(), c.vtables.end(), [](const auto& a, const auto& b) { return std::tie(a.offset, a.address) < std::tie(b.offset, b.address); });
        for (const BasicVTable<Address>& t : c.vtables) functions.insert(t.slots.begin(), t.slots.end());
    }
    out.slot_functions.assign(functions.begin(), functions.end());
}

// Names: the class that introduces each virtual function.
template <class Address>
void rtti_name_functions(BasicClasses<Address>& out) {
    struct Occurrence {
        const std::string* class_name;
        std::uint32_t offset;
        std::size_t slot;
    };
    std::map<Address, std::vector<Occurrence>> occurrences;
    for (const auto& [key, c] : out.classes) {
        for (const BasicVTable<Address>& v : c.vtables) {
            for (std::size_t s = 0; s < v.slots.size(); ++s) occurrences[v.slots[s]].push_back({&c.name, v.offset, s});
        }
    }
    // each class and what it derives from, itself included
    std::unordered_map<std::string, std::unordered_set<std::string>> bases_of;
    for (const auto& [key, c] : out.classes) {
        auto& set = bases_of[c.name];
        set.insert(c.name);
        for (const BaseClass& b : c.bases) set.insert(b.name);
    }
    for (const auto& [function, occ] : occurrences) {
        // the classes every other occurrence derives from (or is)
        std::set<std::string> roots;
        for (const Occurrence& o : occ) {
            const bool root = std::all_of(occ.begin(), occ.end(), [&](const Occurrence& d) { return bases_of.at(*d.class_name).contains(*o.class_name); });
            if (root) roots.insert(*o.class_name);
        }
        std::set<std::pair<std::uint32_t, std::size_t>> root_at;
        for (const Occurrence& o : occ) {
            if (roots.contains(*o.class_name)) root_at.insert({o.offset, o.slot});
        }
        if (roots.size() != 1 || root_at.size() != 1) continue;
        const auto [offset, slot] = *root_at.begin();
        out.functions[function] = {*roots.begin(), offset == 0 ? std::format("vf{}", slot) : std::format("vf{}_off{:X}", slot, offset)};
    }
}

}  // namespace

// ---- the command's output ---------------------------------------------------------------------------------------------

template <class Address>
std::string summary(const BasicClasses<Address>& classes, std::string_view file_name) {
    std::size_t tables = 0;
    for (const auto& [name, c] : classes.classes) tables += c.vtables.size();
    std::string out = std::format("{}: {} classes, {} virtual function tables, {} virtual functions named by class, {} functions found by layout\n",
                                  file_name, classes.classes.size(), tables, classes.functions.size(), classes.found.size());
    std::vector<const BasicClass<Address>*> deepest;
    for (const auto& [name, c] : classes.classes) deepest.push_back(&c);
    std::stable_sort(deepest.begin(), deepest.end(), [](const auto* a, const auto* b) { return a->bases.size() > b->bases.size(); });
    if (deepest.size() > 8) deepest.resize(8);
    for (const BasicClass<Address>* c : deepest) {
        std::string bases;
        for (std::size_t i = 0; i < c->bases.size(); ++i) {
            if (i != 0) bases += ", ";
            bases += c->bases[i].name;
        }
        out += std::format("  {:<40} bases: {}\n", text::utf8_prefix(c->name, 40), text::utf8_prefix(bases, 100));
    }
    return out;
}

template <class Address>
std::string to_json(const BasicClasses<Address>& classes) {
    Json doc = Json::object();
    std::vector<const BasicClass<Address>*> by_name;
    for (const auto& [name, c] : classes.classes) by_name.push_back(&c);
    std::sort(by_name.begin(), by_name.end(), [](const auto* a, const auto* b) { return a->name < b->name; });
    Json list = Json::array();
    for (const BasicClass<Address>* c : by_name) {
        Json bases = Json::array();
        for (const BaseClass& b : c->bases) {
            bases.push_back({{"name", b.name}, {"contained", b.contained}, {"mdisp", b.mdisp}, {"pdisp", b.pdisp}, {"vdisp", b.vdisp}, {"attributes", b.attributes}});
        }
        Json tables = Json::array();
        for (const BasicVTable<Address>& v : c->vtables) {
            tables.push_back({{"address", v.address}, {"offset", v.offset}, {"for", v.for_base ? Json(*v.for_base) : Json(nullptr)}, {"slots", v.slots}});
        }
        list.push_back({{"name", c->name}, {"mangled", c->decorated}, {"bases", bases}, {"vtables", tables}});
    }
    doc["classes"] = list;
    for (const auto& [address, f] : classes.functions) doc["functions"][std::format("{:08X}", address)] = Json::array({f.class_name, f.label});
    if (!doc.contains("functions")) doc["functions"] = Json::object();
    Json found = Json::array();
    for (const BasicFoundFunction<Address>& f : classes.found) found.push_back(Json::array({f.class_name, f.vtable, f.slot, f.address}));
    doc["found"] = found;
    return doc.dump(1, ' ', true) + "\n";
}

template std::string summary(const BasicClasses<std::uint32_t>&, std::string_view);
template std::string summary(const BasicClasses<std::uint64_t>&, std::string_view);
template std::string to_json(const BasicClasses<std::uint32_t>&);
template std::string to_json(const BasicClasses<std::uint64_t>&);

// ---- a PowerPC program: the image -----------------------------------------------------------------------------------------

std::optional<std::uint32_t> Image::word(std::uint32_t address) const {
    for (const Memory& m : memory) {
        if (address >= m.address && std::uint64_t{address} + 4 <= std::uint64_t{m.address} + m.bytes.size()) {
            const std::size_t at = address - m.address;
            return (std::uint32_t{m.bytes[at]} << 24) | (std::uint32_t{m.bytes[at + 1]} << 16) | (std::uint32_t{m.bytes[at + 2]} << 8) |
                   std::uint32_t{m.bytes[at + 3]};
        }
    }
    return std::nullopt;
}

ByteView Image::bytes_at(std::uint32_t address, std::uint32_t n) const {
    for (const Memory& m : memory) {
        if (address >= m.address && address - m.address < m.bytes.size()) {
            const std::size_t at = address - m.address;
            return ByteView(m.bytes).subspan(at, std::min<std::size_t>(n, m.bytes.size() - at));
        }
    }
    return {};
}

// ---- a PowerPC program: the search --------------------------------------------------------------------------------------

namespace {

struct PpcRange {
    std::int64_t begin = 0, end = 0;
    bool contains(std::int64_t a) const { return begin <= a && a < end; }
};

struct PpcDescriptor {
    std::uint32_t address = 0;
    RttiView::Named named;
    std::uint32_t vftable = 0;
};

struct PpcLocator {
    std::uint32_t offset = 0;
    std::uint32_t descriptor = 0;
    RttiHierarchy hierarchy;
};

// The state of one search: the image's words, the places RTTI lives, and what has been found so far.
class PpcSearch {
public:
    explicit PpcSearch(const Image& image) : image_(image), starts_(image.function_starts), view_(make_view()) {}

    Classes run();

private:
    // (a word no memory holds reads as zero)
    std::uint32_t u32(std::int64_t address) const {
        if (address < 0 || address > 0xFFFFFFFFLL) return 0;
        return image_.word(static_cast<std::uint32_t>(address)).value_or(0);
    }

    RttiView make_view() const;
    void find_type_descriptors(const std::vector<PpcRange>& data);
    std::optional<RttiHierarchy> hierarchy(std::uint32_t address) const;
    void find_locators();
    // Take function pointers into `slots` from `q` while they point at function starts; returns where it stopped.
    std::int64_t scan(std::int64_t q, std::vector<std::uint32_t>& slots) const;
    std::size_t required_slots(const Class& c, const VTable& v, const std::unordered_map<std::string, const Class*>& by_name) const;
    void grow_tables_by_layout(Classes& out);

    const Image& image_;
    std::set<std::uint32_t> starts_;
    PpcRange rdata_, text_;
    std::vector<PpcDescriptor> descriptors_;                          // in the order found
    std::unordered_map<std::uint32_t, std::size_t> descriptor_at_;    // lookup only
    std::vector<std::pair<std::uint32_t, PpcLocator>> locators_;      // by address, ascending
    std::unordered_map<std::uint32_t, std::size_t> locator_at_;       // lookup only
    std::set<std::uint32_t> pointers_;                                // the words that point at a locator
    RttiView view_;                                                   // (over the members above)
};

// The records live in .rdata, their refs are pointers, and a ref names one of the type descriptors found.
RttiView PpcSearch::make_view() const {
    RttiView v;
    v.u32 = [this](std::uint64_t at) { return std::optional<std::uint32_t>(u32(static_cast<std::int64_t>(at))); };
    v.holds_records = [this](std::uint64_t ref) { return rdata_.contains(static_cast<std::int64_t>(ref)); };
    v.descriptor = [this](std::uint32_t ref) -> std::optional<RttiView::Named> {
        const auto td = descriptor_at_.find(ref);
        if (td == descriptor_at_.end()) return std::nullopt;
        return descriptors_[td->second].named;
    };
    v.most_bases = 256;
    v.attribute_bits = 3;
    return v;
}

// 1. Type descriptors: the name 8 bytes in, after type_info's vftable and a zero word; the vftable every real descriptor
// shares is type_info's.
void PpcSearch::find_type_descriptors(const std::vector<PpcRange>& data) {
    std::vector<PpcDescriptor> found;
    for (const PpcRange& r : data) {
        const ByteView block = image_.bytes_at(static_cast<std::uint32_t>(r.begin), static_cast<std::uint32_t>(r.end - r.begin));
        const std::string_view text = as_text(block);
        std::size_t p = 0;
        while (true) {
            const std::size_t i = text.find(".?A", p);
            if (i == std::string_view::npos) break;
            const std::size_t end = text.find('\0', i);
            if (end == std::string_view::npos) break;
            p = end;
            const std::uint32_t td = static_cast<std::uint32_t>(r.begin + static_cast<std::int64_t>(i) - 8);
            if (td % 4 != 0 || u32(std::int64_t{td} + 4) != 0) continue;
            PpcDescriptor d;
            d.address = td;
            for (const char c : text.substr(i, end - i))
                d.named.decorated += static_cast<unsigned char>(c) < 0x80 ? std::string(1, c) : std::string("\xEF\xBF\xBD");
            d.vftable = u32(td);
            found.push_back(std::move(d));
        }
    }
    if (found.empty()) return;
    // type_info's vftable is the one most descriptors share (the first of the most common)
    std::vector<std::pair<std::uint32_t, std::size_t>> counts;
    for (const PpcDescriptor& d : found) {
        auto it = std::find_if(counts.begin(), counts.end(), [&](const auto& c) { return c.first == d.vftable; });
        if (it == counts.end()) {
            counts.emplace_back(d.vftable, 1);
        } else {
            ++it->second;
        }
    }
    std::uint32_t type_info = counts.front().first;
    std::size_t best = 0;
    for (const auto& [vftable, n] : counts) {
        if (n > best) {
            best = n;
            type_info = vftable;
        }
    }
    for (PpcDescriptor& d : found) {
        if (d.vftable != type_info) continue;
        d.named.name = rtti_readable(d.named.decorated);
        descriptor_at_[d.address] = descriptors_.size();
        descriptors_.push_back(std::move(d));
    }
}

// 2. RttiHierarchy descriptors and object locators (.rdata).
std::optional<RttiHierarchy> PpcSearch::hierarchy(std::uint32_t a) const {
    if (!rdata_.contains(a) || a % 4 != 0) return std::nullopt;
    return rtti_hierarchy(view_, a);
}

void PpcSearch::find_locators() {
    for (std::int64_t a = rdata_.begin; a < rdata_.end - 20; a += 4) {
        if (u32(a) != 0) continue;
        const auto td = descriptor_at_.find(u32(a + 12));
        if (td == descriptor_at_.end()) continue;
        std::optional<RttiHierarchy> h = hierarchy(u32(a + 16));
        if (!h || h->bases.front().name != descriptors_[td->second].named.name) continue;
        locator_at_[static_cast<std::uint32_t>(a)] = locators_.size();
        locators_.push_back({static_cast<std::uint32_t>(a), PpcLocator{u32(a + 4), u32(a + 12), std::move(*h)}});
    }
}

std::int64_t PpcSearch::scan(std::int64_t q, std::vector<std::uint32_t>& slots) const {
    while (q < rdata_.end && !pointers_.contains(static_cast<std::uint32_t>(q)) && starts_.contains(u32(q))) {
        slots.push_back(u32(q));
        q += 4;
    }
    return q;
}

// 4. The layout rule: a derived class's table for a base sub-object is at least that base's table.
std::size_t PpcSearch::required_slots(const Class& c, const VTable& v, const std::unordered_map<std::string, const Class*>& by_name) const {
    std::size_t need = 1;   // a class has a table only for a virtual function
    for (const BaseClass& b : c.bases) {
        const auto it = by_name.find(b.name);
        if (b.pdisp != -1 || it == by_name.end()) continue;
        const Class* base = it->second;
        // the offsets of the base's own non-virtual sub-objects
        std::set<std::int64_t> nonvirtual = {0};
        for (const BaseClass& bb : base->bases) {
            if (bb.pdisp == -1) nonvirtual.insert(bb.mdisp);
        }
        for (const VTable& bv : base->vtables) {
            if (nonvirtual.contains(bv.offset) && std::int64_t{b.mdisp} + bv.offset == v.offset) need = std::max(need, bv.slots.size());
        }
    }
    return need;
}

void PpcSearch::grow_tables_by_layout(Classes& out) {
    std::unordered_map<std::string, const Class*> by_name;
    for (const auto& [key, c] : out.classes) by_name.emplace(c.name, &c);
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& [key, c] : out.classes) {
            for (VTable& v : c.vtables) {
                const std::size_t need = required_slots(c, v, by_name);
                std::int64_t q = std::int64_t{v.address} + 4 * static_cast<std::int64_t>(v.slots.size());
                while (v.slots.size() < need) {
                    const std::uint32_t t = u32(q);
                    // the layout and the image disagree: stop, guess nothing
                    if (pointers_.contains(static_cast<std::uint32_t>(q)) || !text_.contains(t)) break;
                    if (!starts_.contains(t)) {
                        out.found.push_back({c.name, v.address, v.slots.size(), t});
                        starts_.insert(t);
                    }
                    v.slots.push_back(t);
                    q += 4;
                    changed = true;
                }
                scan(q, v.slots);
            }
        }
    }
}

Classes PpcSearch::run() {
    Classes out;
    // the sections by name: a name met twice keeps its first place and its last range
    std::vector<std::pair<std::string, PpcRange>> named;
    for (const Image::Section& s : image_.sections) {
        const PpcRange r{s.address, std::int64_t{s.address} + s.size};
        const auto it = std::find_if(named.begin(), named.end(), [&](const auto& n) { return n.first == s.name; });
        if (it == named.end()) {
            named.emplace_back(s.name, r);
        } else {
            it->second = r;
        }
    }
    auto section = [&](std::string_view name) -> std::optional<PpcRange> {
        const auto it = std::find_if(named.begin(), named.end(), [&](const auto& n) { return n.first == name; });
        return it == named.end() ? std::nullopt : std::optional<PpcRange>(it->second);
    };
    rdata_ = section(".rdata").value_or(PpcRange{});
    text_ = section(".text").value_or(PpcRange{image_.code_begin, image_.code_end});
    std::vector<PpcRange> data;
    for (const auto& [name, range] : named) {
        if (name == ".data" || name == ".rdata") data.push_back(range);
    }

    find_type_descriptors(data);
    if (descriptors_.empty()) return out;
    find_locators();

    // 3. Tables: a word pointing at a locator, then function pointers.
    for (std::int64_t a = rdata_.begin; a < rdata_.end - 4; a += 4) {
        if (locator_at_.contains(u32(a))) pointers_.insert(static_cast<std::uint32_t>(a));
    }
    // (the classes keyed by C++ name: two locators of one class make one)
    for (const auto& [address, loc] : locators_) {
        const PpcDescriptor& td = descriptors_[descriptor_at_.at(loc.descriptor)];
        if (!out.classes.contains(td.named.name)) out.classes.insert_or_assign(td.named.name, rtti_class<std::uint32_t>(td.named, loc.hierarchy));
    }
    for (const std::uint32_t a : pointers_) {
        const PpcLocator& loc = locators_[locator_at_.at(u32(a))].second;
        Class& c = out.classes.at(descriptors_[descriptor_at_.at(loc.descriptor)].named.name);
        VTable v;
        v.address = a + 4;
        v.locator = u32(a);
        v.offset = loc.offset;
        scan(std::int64_t{a} + 4, v.slots);
        if (loc.offset != 0) {
            for (const BaseClass& b : c.bases) {
                if (b.pdisp == -1 && b.mdisp >= 0 && static_cast<std::uint32_t>(b.mdisp) == loc.offset) {
                    v.for_base = b.name;
                    break;
                }
            }
        }
        c.vtables.push_back(std::move(v));
    }
    rtti_order_tables(out);
    grow_tables_by_layout(out);
    rtti_order_tables(out);   // (the slots the layout added)
    rtti_name_functions(out);
    return out;
}

}  // namespace

Classes find_classes(const Image& image) { return PpcSearch(image).run(); }

// ---- an x64 program -----------------------------------------------------------------------------------------------------

namespace x64 {

namespace {

// The image's memory as its sections lay it out: the raw bytes of each, read by RVA.
class X64Memory {
public:
    explicit X64Memory(const formats::pe::Image& image) : image_base_(image.header().image_base) {
        for (const formats::pe::Section& s : image.sections()) {
            X64Piece p;
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
        for (const X64Piece& p : pieces_)
            if (p.code && rva >= p.rva && rva < p.rva + p.size) return true;
        return false;
    }
    bool is_data_rva(std::uint64_t rva) const {
        for (const X64Piece& p : pieces_)
            if (!p.code && rva >= p.rva && rva < p.rva + p.bytes.size()) return true;
        return false;
    }
    template <class F>
    void each_data_piece(F f) const {
        for (const X64Piece& p : pieces_)
            if (!p.code) f(p.rva, p.bytes);
    }

private:
    struct X64Piece {
        std::uint64_t rva = 0;
        ByteView bytes;
        std::uint64_t size = 0;
        bool code = false;
    };
    // Up to `n` bytes from `rva`, to the end of the section's raw bytes.
    ByteView at(std::uint64_t rva, std::size_t n) const {
        for (const X64Piece& p : pieces_)
            if (rva >= p.rva && rva < p.rva + p.bytes.size()) {
                const std::size_t off = static_cast<std::size_t>(rva - p.rva);
                return p.bytes.subspan(off, std::min(n, p.bytes.size() - off));
            }
        return {};
    }
    std::uint64_t image_base_ = 0;
    std::vector<X64Piece> pieces_;
};

bool x64_decorated_class(std::string_view s) { return s.size() > 4 && s.substr(0, 3) == ".?A" && (s[3] == 'V' || s[3] == 'U'); }

// The records live in the data sections, their refs are RVAs, and a ref names the type descriptor whose decorated name follows
// type_info's vftable pointer and the spare pointer.
RttiView x64_view(const X64Memory& m) {
    RttiView v;
    v.u32 = [&m](std::uint64_t rva) { return m.u32(rva); };
    v.holds_records = [&m](std::uint64_t rva) { return m.is_data_rva(rva); };
    v.descriptor = [&m](std::uint32_t td) -> std::optional<RttiView::Named> {
        if (td == 0 || !m.is_data_rva(td)) return std::nullopt;
        std::optional<std::string> s = m.text(std::uint64_t{td} + 16, 1024);
        if (!s || !x64_decorated_class(*s)) return std::nullopt;
        return RttiView::Named{*s, rtti_readable(*s)};
    };
    return v;
}

// A locator read and checked: its offset, its class's names and its hierarchy descriptor.
struct X64Locator {
    std::uint32_t offset = 0;
    RttiView::Named named;
    std::uint32_t chd = 0;
};
std::optional<X64Locator> x64_read_locator(const X64Memory& m, const RttiView& view, std::uint32_t rva) {
    const auto sig = m.u32(rva), off = m.u32(rva + 4), td = m.u32(rva + 12), chd = m.u32(rva + 16), self = m.u32(rva + 20);
    if (!sig || *sig != 1 || !self || *self != rva || !off || !td || !chd) return std::nullopt;
    std::optional<RttiView::Named> named = view.descriptor(*td);
    if (!named || !m.is_data_rva(*chd)) return std::nullopt;
    return X64Locator{*off, std::move(*named), *chd};
}

}  // namespace

Classes find_classes(const formats::pe::Image& image) {
    Classes out;
    if (image.header().machine != formats::pe::kMachineX64) return out;
    const X64Memory m(image);
    const RttiView view = x64_view(m);
    // the locators, in image order
    std::vector<std::uint32_t> locators;
    std::map<std::uint32_t, X64Locator> read;
    m.each_data_piece([&](std::uint64_t base, ByteView bytes) {
        for (std::size_t k = 0; k + 24 <= bytes.size(); k += 4) {
            if (bytes[k] != 1 || bytes[k + 1] != 0 || bytes[k + 2] != 0 || bytes[k + 3] != 0) continue;
            const std::uint32_t rva = static_cast<std::uint32_t>(base + k);
            if (std::optional<X64Locator> l = x64_read_locator(m, view, rva)) {
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
    // (the classes keyed by decorated name)
    for (const std::uint32_t rva : locators) {
        const X64Locator& l = read.at(rva);
        std::optional<RttiHierarchy> hierarchy = rtti_hierarchy(view, l.chd);
        if (!hierarchy || hierarchy->bases.empty() || hierarchy->bases.front().decorated != l.named.decorated) continue;
        Class* c = out.classes.find(l.named.decorated);
        if (c == nullptr) {
            out.classes.insert_or_assign(l.named.decorated, rtti_class<std::uint64_t>(l.named, std::move(*hierarchy)));
            c = out.classes.find(l.named.decorated);
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
            }
            if (t.slots.empty()) continue;
            if (l.offset != 0)
                for (const BaseClass& b : c->bases)
                    if (!b.is_virtual() && b.mdisp >= 0 && static_cast<std::uint32_t>(b.mdisp) == l.offset) {
                        t.for_base = b.name;
                        break;
                    }
            c->vtables.push_back(std::move(t));
        }
    }
    rtti_order_tables(out);
    rtti_name_functions(out);
    return out;
}

}  // namespace x64

}  // namespace cb::symbols::cpp_classes
