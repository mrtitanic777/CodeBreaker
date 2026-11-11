#include "cb/invert/early_c_frame.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace cb::invert::early_c_frame {

std::uint32_t type_size(std::string_view type, std::uint32_t count) {
    struct Entry {
        std::string_view name;
        std::uint32_t size;
    };
    static constexpr Entry kSizes[] = {
        {"char", 1},   {"unsigned char", 1}, {"int", 2},     {"unsigned", 2}, {"unsigned int", 2},
        {"short", 2},  {"long", 4},          {"unsigned long", 4}, {"float", 4},    {"double", 8},
        {"pointer", 2}, {"ptr", 2},
    };
    std::uint32_t size = 2;
    for (const Entry& e : kSizes) {
        if (e.name == type) size = e.size;
    }
    return size * count;
}

Layout layout(std::span<const std::string> temps, std::span<const Local> locals, std::span<const Param> params) {
    Layout out;
    std::uint32_t off = 0;
    for (const std::string& t : temps) off += type_size(t);   // the temp region first, from bp+0
    out.temp_bytes = off;
    for (const Local& l : locals) {                           // the locals above it, in declaration order
        out.local_offsets.insert_or_assign(l.name, off);
        off += type_size(l.type, l.count);
    }
    out.frame_size = off + (off & 1);                         // rounded up to even
    std::uint32_t p = out.frame_size + 4;                     // params lie above the saved bp and the near return
    for (const Param& param : params) {
        out.param_offsets.insert_or_assign(param.name, p);
        p += type_size(param.type);
    }
    return out;
}

std::vector<std::pair<Offset, Offset>> chain_swaps(std::span<const Copy> copies) {
    std::unordered_map<Offset, Offset> next;   // src -> dest (lookup only; a src copied twice keeps its last dest)
    std::unordered_set<Offset> incoming;       // every dest
    for (const Copy& c : copies) {
        next[c.src] = c.dest;
        incoming.insert(c.dest);
    }
    std::vector<std::pair<Offset, Offset>> swaps;
    std::unordered_set<Offset> seen;
    for (const Copy& c : copies) {   // the origins, in copy order: sources that are no destination
        const Offset origin = c.src;
        if (incoming.contains(origin) || seen.contains(origin)) continue;
        Offset cur = origin;
        seen.insert(cur);
        // walk to the sink; a chain that runs into a cycle ends where it would re-enter it (the Python looped forever)
        std::unordered_set<Offset> walked = {cur};
        for (auto it = next.find(cur); it != next.end() && !walked.contains(it->second); it = next.find(cur)) {
            cur = it->second;
            walked.insert(cur);
            seen.insert(cur);
        }
        if (origin != cur) swaps.emplace_back(origin, cur);
    }
    return swaps;
}

OrderedMap<Offset, Offset> apply_copy_swaps(std::span<const Offset> offsets, std::span<const Copy> copies) {
    OrderedMap<Offset, Offset> current;
    for (const Offset o : offsets) current.insert_or_assign(o, o);
    for (const auto& [a, b] : chain_swaps(copies)) {
        if (current.contains(a) && current.contains(b)) std::swap(current.at(a), current.at(b));
    }
    return current;
}

std::vector<Offset> infer_decl_order(std::span<const Offset> offsets, std::span<const Copy> copies) {
    std::vector<Offset> sorted(offsets.begin(), offsets.end());
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    std::map<Offset, Offset> pos;   // offset -> its place in the declaration order (a permutation of the offsets)
    for (const Offset o : sorted) pos[o] = o;
    for (const auto& [a, b] : chain_swaps(copies)) {
        if (pos.contains(a) && pos.contains(b)) std::swap(pos[a], pos[b]);
    }
    std::vector<Offset> order = sorted;
    std::sort(order.begin(), order.end(), [&](Offset x, Offset y) { return pos[x] < pos[y]; });
    return order;
}

std::vector<Decl> emit_local_decls(std::span<const Offset> offsets, std::uint32_t frame_size, const std::set<Offset>& longs,
                                   std::span<const Copy> copies) {
    std::vector<Offset> offs(offsets.begin(), offsets.end());
    std::sort(offs.begin(), offs.end());
    offs.erase(std::unique(offs.begin(), offs.end()), offs.end());
    if (offs.empty()) return {};
    auto type_at = [&](Offset o) { return longs.contains(o) ? "long" : "int"; };

    bool contiguous = true;
    for (std::size_t k = 0; k + 1 < offs.size(); ++k) {
        if (offs[k + 1] - offs[k] != type_size(type_at(offs[k]))) contiguous = false;
    }
    std::vector<Decl> out;
    if (!copies.empty() && contiguous) {
        // copy reversal: declare in the inferred order so the compiler's src <-> dest swap lands each var at its offset
        for (const Offset o : infer_decl_order(offs, copies)) out.push_back({type_at(o), std::format("v{}", o), 1});
        return out;
    }
    for (std::size_t k = 0; k < offs.size(); ++k) {
        const Offset o = offs[k];
        out.push_back({type_at(o), std::format("v{}", o), 1});
        const std::int64_t next = k + 1 < offs.size() ? offs[k + 1] : frame_size;
        const std::int64_t gap = next - o - type_size(type_at(o));
        if (gap > 0) out.push_back({"char", std::format("_p{}", o), static_cast<std::uint32_t>(gap)});   // unused locals
    }
    return out;
}

std::string format_decl(const Decl& decl) {
    return decl.count > 1 ? std::format("{} {}[{}];", decl.type, decl.name, decl.count)
                          : std::format("{} {};", decl.type, decl.name);
}

}  // namespace cb::invert::early_c_frame
