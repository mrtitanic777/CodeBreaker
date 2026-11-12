#include "cb/symbols/cpp_classes.hpp"

#include <algorithm>
#include <format>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/text.hpp"
#include "cb/symbols/demangle.hpp"

namespace cb::symbols::cpp_classes {

// ---- the image ----------------------------------------------------------------------------------------------------

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

// ---- the search -------------------------------------------------------------------------------------------------------

namespace {

struct Range {
    std::int64_t begin = 0, end = 0;
    bool contains(std::int64_t a) const { return begin <= a && a < end; }
};

struct TypeDescriptor {
    std::uint32_t address = 0;
    std::string mangled;
    std::uint32_t vftable = 0;
    std::string name;
};

struct Locator {
    std::uint32_t offset = 0;
    std::uint32_t descriptor = 0;
    std::vector<BaseClass> bases;       // the hierarchy's, the class itself first
};

// The state of one search: the image's words, the places RTTI lives, and what has been found so far.
class Search {
public:
    explicit Search(const Image& image) : image_(image), starts_(image.function_starts) {}

    Classes run();

private:
    std::uint32_t u32(std::int64_t address) const {
        if (address < 0 || address > 0xFFFFFFFFLL) return 0;
        return image_.word(static_cast<std::uint32_t>(address)).value_or(0);
    }
    std::int32_t s32(std::int64_t address) const { return static_cast<std::int32_t>(u32(address)); }

    void find_type_descriptors(const std::vector<Range>& data);
    std::optional<std::vector<BaseClass>> hierarchy(std::uint32_t address) const;
    void find_locators();
    // Take function pointers into `slots` from `q` while they point at function starts; returns where it stopped.
    std::int64_t scan(std::int64_t q, std::vector<std::uint32_t>& slots) const;
    std::size_t required_slots(const Class& c, const VTable& v, const Classes& classes) const;
    void grow_tables_by_layout(Classes& out);
    void name_functions(Classes& out) const;

    const Image& image_;
    std::set<std::uint32_t> starts_;
    Range rdata_, text_;
    std::vector<TypeDescriptor> descriptors_;                         // in the order found
    std::unordered_map<std::uint32_t, std::size_t> descriptor_at_;    // lookup only
    std::vector<std::pair<std::uint32_t, Locator>> locators_;         // by address, ascending
    std::unordered_map<std::uint32_t, std::size_t> locator_at_;       // lookup only
    std::set<std::uint32_t> pointers_;                                // the words that point at a locator
};

// 1. Type descriptors: the name 8 bytes in, after type_info's vftable and a zero word; the vftable every real descriptor
// shares is type_info's.
void Search::find_type_descriptors(const std::vector<Range>& data) {
    std::vector<TypeDescriptor> found;
    for (const Range& r : data) {
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
            TypeDescriptor d;
            d.address = td;
            for (const char c : text.substr(i, end - i)) d.mangled += static_cast<unsigned char>(c) < 0x80 ? std::string(1, c) : std::string("\xEF\xBF\xBD");
            d.vftable = u32(td);
            found.push_back(std::move(d));
        }
    }
    if (found.empty()) return;
    // type_info's vftable is the one most descriptors share (the first of the most common)
    std::vector<std::pair<std::uint32_t, std::size_t>> counts;
    for (const TypeDescriptor& d : found) {
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
    for (TypeDescriptor& d : found) {
        if (d.vftable != type_info) continue;
        d.name = demangle::type_name(d.mangled).value_or(d.mangled);
        if (d.name.empty()) d.name = d.mangled;
        descriptor_at_[d.address] = descriptors_.size();
        descriptors_.push_back(std::move(d));
    }
}

// 2. Hierarchy descriptors and object locators (.rdata).
std::optional<std::vector<BaseClass>> Search::hierarchy(std::uint32_t a) const {
    if (!rdata_.contains(a) || a % 4 != 0 || u32(a) != 0) return std::nullopt;
    const std::uint32_t attributes = u32(std::int64_t{a} + 4), n = u32(std::int64_t{a} + 8), array = u32(std::int64_t{a} + 12);
    if (n < 1 || n > 256 || !rdata_.contains(array) || (attributes & ~3u) != 0) return std::nullopt;
    std::vector<BaseClass> bases;
    for (std::uint32_t k = 0; k < n; ++k) {
        const std::uint32_t bcd = u32(std::int64_t{array} + 4 * std::int64_t{k});
        if (!rdata_.contains(bcd)) return std::nullopt;
        const auto td = descriptor_at_.find(u32(bcd));
        if (td == descriptor_at_.end()) return std::nullopt;
        bases.push_back({descriptors_[td->second].name, u32(std::int64_t{bcd} + 4), s32(std::int64_t{bcd} + 8), s32(std::int64_t{bcd} + 12),
                         s32(std::int64_t{bcd} + 16), u32(std::int64_t{bcd} + 20)});
    }
    return bases;
}

void Search::find_locators() {
    for (std::int64_t a = rdata_.begin; a < rdata_.end - 20; a += 4) {
        if (u32(a) != 0) continue;
        const auto td = descriptor_at_.find(u32(a + 12));
        if (td == descriptor_at_.end()) continue;
        auto bases = hierarchy(u32(a + 16));
        if (!bases || bases->front().name != descriptors_[td->second].name) continue;
        locator_at_[static_cast<std::uint32_t>(a)] = locators_.size();
        locators_.push_back({static_cast<std::uint32_t>(a), Locator{u32(a + 4), u32(a + 12), std::move(*bases)}});
    }
}

std::int64_t Search::scan(std::int64_t q, std::vector<std::uint32_t>& slots) const {
    while (q < rdata_.end && !pointers_.contains(static_cast<std::uint32_t>(q)) && starts_.contains(u32(q))) {
        slots.push_back(u32(q));
        q += 4;
    }
    return q;
}

// 4. The layout rule: a derived class's table for a base sub-object is at least that base's table.
std::size_t Search::required_slots(const Class& c, const VTable& v, const Classes& classes) const {
    std::size_t need = 1;   // a class has a table only for a virtual function
    for (const BaseClass& b : c.bases) {
        const Class* base = classes.classes.find(b.name);
        if (b.pdisp != -1 || base == nullptr) continue;
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

void Search::grow_tables_by_layout(Classes& out) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& [name, c] : out.classes) {
            for (VTable& v : c.vtables) {
                const std::size_t need = required_slots(c, v, out);
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

// 5. Names: the class that introduces each virtual function.
void Search::name_functions(Classes& out) const {
    struct Occurrence {
        const std::string* class_name;
        std::uint32_t offset;
        std::size_t slot;
    };
    std::map<std::uint32_t, std::vector<Occurrence>> occurrences;
    for (const auto& [name, c] : out.classes) {
        for (const VTable& v : c.vtables) {
            for (std::size_t s = 0; s < v.slots.size(); ++s) occurrences[v.slots[s]].push_back({&name, v.offset, s});
        }
    }
    // each class and what it derives from, itself included
    std::unordered_map<std::string, std::unordered_set<std::string>> bases_of;
    for (const auto& [name, c] : out.classes) {
        auto& set = bases_of[name];
        set.insert(name);
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

Classes Search::run() {
    Classes out;
    // the sections by name: a name met twice keeps its first place and its last range
    std::vector<std::pair<std::string, Range>> named;
    for (const Image::Section& s : image_.sections) {
        const Range r{s.address, std::int64_t{s.address} + s.size};
        const auto it = std::find_if(named.begin(), named.end(), [&](const auto& n) { return n.first == s.name; });
        if (it == named.end()) {
            named.emplace_back(s.name, r);
        } else {
            it->second = r;
        }
    }
    auto section = [&](std::string_view name) -> std::optional<Range> {
        const auto it = std::find_if(named.begin(), named.end(), [&](const auto& n) { return n.first == name; });
        return it == named.end() ? std::nullopt : std::optional<Range>(it->second);
    };
    rdata_ = section(".rdata").value_or(Range{});
    text_ = section(".text").value_or(Range{image_.code_begin, image_.code_end});
    std::vector<Range> data;
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
    for (const auto& [address, loc] : locators_) {
        const TypeDescriptor& td = descriptors_[descriptor_at_.at(loc.descriptor)];
        if (out.classes.contains(td.name)) continue;
        Class c;
        c.name = td.name;
        c.mangled = td.mangled;
        c.bases.assign(loc.bases.begin() + 1, loc.bases.end());
        out.classes.insert_or_assign(td.name, std::move(c));
    }
    for (const std::uint32_t a : pointers_) {
        const Locator& loc = locators_[locator_at_.at(u32(a))].second;
        Class& c = out.classes.at(descriptors_[descriptor_at_.at(loc.descriptor)].name);
        VTable v;
        v.address = a + 4;
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
    for (auto& [name, c] : out.classes) {
        std::sort(c.vtables.begin(), c.vtables.end(), [](const VTable& a, const VTable& b) { return std::tie(a.offset, a.address) < std::tie(b.offset, b.address); });
    }
    grow_tables_by_layout(out);
    name_functions(out);
    return out;
}

}  // namespace

Classes find_classes(const Image& image) { return Search(image).run(); }

// ---- the command's output ---------------------------------------------------------------------------------------------

std::string summary(const Classes& classes, std::string_view file_name) {
    std::size_t tables = 0;
    for (const auto& [name, c] : classes.classes) tables += c.vtables.size();
    std::string out = std::format("{}: {} classes, {} virtual function tables, {} virtual functions named by class, {} functions found by layout\n",
                                  file_name, classes.classes.size(), tables, classes.functions.size(), classes.found.size());
    std::vector<const Class*> deepest;
    for (const auto& [name, c] : classes.classes) deepest.push_back(&c);
    std::stable_sort(deepest.begin(), deepest.end(), [](const Class* a, const Class* b) { return a->bases.size() > b->bases.size(); });
    if (deepest.size() > 8) deepest.resize(8);
    for (const Class* c : deepest) {
        std::string bases;
        for (std::size_t i = 0; i < c->bases.size(); ++i) {
            if (i != 0) bases += ", ";
            bases += c->bases[i].name;
        }
        out += std::format("  {:<40} bases: {}\n", text::utf8_prefix(c->name, 40), text::utf8_prefix(bases, 100));
    }
    return out;
}

std::string to_json(const Classes& classes) {
    Json doc = Json::object();
    std::vector<const Class*> by_name;
    for (const auto& [name, c] : classes.classes) by_name.push_back(&c);
    std::sort(by_name.begin(), by_name.end(), [](const Class* a, const Class* b) { return a->name < b->name; });
    Json list = Json::array();
    for (const Class* c : by_name) {
        Json bases = Json::array();
        for (const BaseClass& b : c->bases) {
            bases.push_back({{"name", b.name}, {"contained", b.contained}, {"mdisp", b.mdisp}, {"pdisp", b.pdisp}, {"vdisp", b.vdisp}, {"attributes", b.attributes}});
        }
        Json tables = Json::array();
        for (const VTable& v : c->vtables) {
            tables.push_back({{"address", v.address}, {"offset", v.offset}, {"for", v.for_base ? Json(*v.for_base) : Json(nullptr)}, {"slots", v.slots}});
        }
        list.push_back({{"name", c->name}, {"mangled", c->mangled}, {"bases", bases}, {"vtables", tables}});
    }
    doc["classes"] = list;
    for (const auto& [address, f] : classes.functions) doc["functions"][std::format("{:08X}", address)] = Json::array({f.class_name, f.label});
    if (!doc.contains("functions")) doc["functions"] = Json::object();
    Json found = Json::array();
    for (const FoundFunction& f : classes.found) found.push_back(Json::array({f.class_name, f.vtable, f.slot, f.address}));
    doc["found"] = found;
    return doc.dump(1, ' ', true) + "\n";
}

}  // namespace cb::symbols::cpp_classes
