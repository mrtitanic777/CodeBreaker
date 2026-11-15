// cb/build/module_pack.cpp -- the functions rebuilt from C go back into the places of a segment that holds assembly.

#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "cb/build/module_internal.hpp"
#include "cb/build/msc6.hpp"
#include "cb/core/fs.hpp"

namespace cb::build::module::internal {

Packed pack_holes(const std::vector<HoleSpec>& holes, const OrderedMap<std::string, std::uint32_t>& size,
                  const std::set<std::string>& near, const std::set<std::string>& aside, std::optional<std::uint32_t> limit) {
    Packed out;
    std::vector<std::string> rest;
    for (const HoleSpec& h : holes) {
        Bin bin;
        bin.lo = h.lo;
        bin.hi = h.hi;
        bin.used = h.lo;
        if (h.name && size.contains(*h.name) && !aside.contains(*h.name)) {
            const std::uint32_t pad = bin.used & 1;   // (OpenWatcom aligns its code to a word)
            if (pad + size.at(*h.name) <= h.hi - h.lo) {
                bin.objects.emplace_back(pad, *h.name);
                bin.used += pad + size.at(*h.name);
            } else {
                rest.push_back(*h.name);
            }
        }
        out.bins.push_back(std::move(bin));
    }
    std::map<std::string, std::uint32_t> own;   // {name: lo of its hole} (the last hole of a name wins)
    for (const HoleSpec& h : holes) {
        if (h.name) own[*h.name] = h.lo;
    }
    std::stable_sort(rest.begin(), rest.end(), [&](const std::string& a, const std::string& b) {
        const bool far_a = !near.contains(a), far_b = !near.contains(b);   // (near ones first)
        if (far_a != far_b) return far_a < far_b;
        return size.at(a) > size.at(b);
    });
    for (const std::string& name : rest) {
        bool placed = false;
        for (Bin& bin : out.bins) {
            const auto it = own.find(name);
            if (limit && (it == own.end() ? 0u : it->second) < *limit && bin.hi > *limit) continue;
            const std::uint32_t pad = bin.used & 1;
            if (bin.used + pad + size.at(name) <= bin.hi) {
                bin.objects.emplace_back(pad, name);
                bin.used += pad + size.at(name);
                placed = true;
                break;
            }
        }
        if (!placed) (near.contains(name) ? out.no_room_near : out.no_room_far).push_back(name);
    }
    return out;
}

void note_assembled(Built& b, const std::string& name, std::map<std::string, std::string>& stems, const std::string& stem) {
    stems[name] = stem;
    const auto origin = b.origins.find(name);
    if (origin != b.origins.end() && origin->second.language == analyze::origin::Language::Assembly) {
        std::vector<std::string> evidence(origin->second.evidence.begin(),
                                          origin->second.evidence.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(2, origin->second.evidence.size())));
        b.assembled.insert_or_assign(name, "written in assembly: " + text::join(evidence, "; "));
    } else {
        const std::string* why = b.rejected.find(name);
        if (why != nullptr) {
            b.assembled.insert_or_assign(name, "its C does not compile: " + *why);
        } else {
            // (C the build did not trust -- the gate, a check of its calls, a run of the whole module -- and the evidence it recorded: kept as
            // its instructions, and said why)
            std::string evidence;
            if (origin != b.origins.end()) {
                const std::vector<std::string>& all = origin->second.evidence;
                evidence = text::join(std::vector<std::string>(all.begin(), all.begin() + static_cast<std::ptrdiff_t>(std::min<std::size_t>(2, all.size()))), "; ");
            }
            b.assembled.insert_or_assign(name, "its C is not used: " + evidence);
        }
    }
    b.rejected.erase(name);
}

// The frame of a segment with assembly in it, in the order the linker is to lay it out: its pieces at the original's places, and in the
// places of the functions rebuilt from C (the frame's holes) those functions -- each in its own place when it fits there, and the ones
// that do not, packed into what room the holes have left (biggest first; a near call reaches anywhere in the segment), filler for the
// rest. A far one with no room goes to the segment beside (CODE<n>X), a near one after the frame.
std::optional<std::vector<std::string>> frame_pieces(Built& b, Piece& piece, std::uint32_t seg, std::map<std::string, std::string>& stems,
                                                     const stdfs::path& folder, const Assembler& assembler,
                                                     const PieceAssembler& assemble_piece, const ElsewhereFn& elsewhere) {
    const asm16::Function& f = *piece.f;
    std::map<std::string, std::string> name_of;   // {symbol: name}
    for (const auto& [name, published] : b.publics) name_of[published.symbol] = name;
    const auto name_for = [&](const std::string& symbol) -> std::optional<std::string> {
        const auto it = name_of.find(symbol);
        return it == name_of.end() ? std::nullopt : std::optional<std::string>(it->second);
    };
    OrderedMap<std::string, std::uint32_t> size;
    std::map<std::string, std::string> cstem;
    for (const asm16::Hole& h : f.holes()) {
        const std::optional<std::string> name = name_for(h.symbol);
        if (!name) continue;
        const auto st = stems.find(*name);
        if (st == stems.end() || st->second.empty() || st->second.starts_with("asm/")) continue;
        try {
            const msc6::Code code = msc6::object_code(fs::read_file(folder / (st->second + ".obj")));
            size.insert_or_assign(*name, static_cast<std::uint32_t>(code.code.size()));
            cstem[*name] = st->second;
        } catch (const IoError&) {
            // (no object for it: it has no size, and no place in the frame)
        }
    }
    const std::set<std::string>& near = b.near_names;
    std::optional<std::uint32_t> limit;
    if (const auto it = b.resident_limit.find(seg); it != b.resident_limit.end()) limit = static_cast<std::uint32_t>(it->second);

    std::vector<HoleSpec> holes;
    for (const asm16::Hole& h : f.holes()) holes.push_back({h.start, h.end, name_for(h.symbol)});
    const auto place = [&](const std::set<std::string>& aside) { return pack_holes(holes, size, near, aside, limit); };

    // A near function with no room goes after the frame and makes the segment bigger than the original's -- past 64K, in the end. A far
    // one can go to the segment beside instead: the far ones give up their places, largest first, while near ones are still left over.
    // (no longer done by default: every function stays in its own segment -- a segment the original does not have renumbers the
    // selectors of all that loads after it. The link moves far ones out of a segment only past 64K. CB_ASIDE=1 brings it back.)
    std::set<std::string> aside;
    Packed packed = place(aside);
    while (!packed.no_room_near.empty() && elsewhere && env_is_set("CB_ASIDE")) {
        std::vector<std::string> placed_far;
        for (const auto& [n, bytes] : size) {
            if (!near.contains(n) && !aside.contains(n) &&
                std::find(packed.no_room_far.begin(), packed.no_room_far.end(), n) == packed.no_room_far.end())
                placed_far.push_back(n);
        }
        std::stable_sort(placed_far.begin(), placed_far.end(), [&](const std::string& x, const std::string& y) { return size.at(x) > size.at(y); });
        if (placed_far.empty()) break;
        aside.insert(placed_far[0]);
        packed = place(aside);
    }
    for (const asm16::Hole& h : f.holes()) {
        const std::optional<std::string> name = name_for(h.symbol);
        const std::uint32_t* bytes = name ? size.find(*name) : nullptr;
        b.frame_fit.push_back({name ? *name : "None", h.end - h.start, bytes != nullptr ? *bytes : 0});
    }
    std::vector<std::string> after;
    {
        std::vector<std::string> order(aside.begin(), aside.end());
        order.insert(order.end(), packed.no_room_far.begin(), packed.no_room_far.end());
        order.insert(order.end(), packed.no_room_near.begin(), packed.no_room_near.end());
        for (const std::string& name : order) {
            // (only those that gave up their places for near ones go to the segment beside; a far one that just did not fit its own place
            // goes after the frame, as a near one does: a segment the original does not have renumbers the selectors of all that loads
            // after it -- USER's sub_39_05AC, sent to a new segment 46, broke Setup with sub_7_03F8 rebuilt from C as well, though it
            // never ran. The link moves far ones out only past 64K.)
            std::optional<std::string> moved;
            if (elsewhere && aside.contains(name)) moved = elsewhere(name);
            if (moved && !moved->empty()) {   // far: into the segment beside this one
                stems[name] = *moved;
                b.moved_c.insert(name);
            } else {
                after.push_back(cstem.at(name));   // near: after the frame, in this segment
                std::optional<std::uint32_t> own;
                for (const asm16::Hole& h : f.holes()) {
                    if (name_for(h.symbol) == name) {
                        own = h.start;
                        break;
                    }
                }
                if (limit && own && *own < *limit) {
                    // (past where the segment is cut after start-up: the build is done again with it as its instructions)
                    b.resident_overflow.insert_or_assign(
                        name, std::format("its C finds no room in segment {} below {:04x} (where it is cut, or a fixed segment's end)", seg, *limit));
                }
            }
        }
    }
    for (const auto& [name, bytes] : size) {
        if (!b.moved_c.contains(name)) b.placed_c.insert(name);
    }
    std::vector<std::string> out;
    std::uint32_t at = f.start();
    std::vector<Bin> all = packed.bins;
    all.push_back(Bin{.lo = f.end(), .hi = f.end(), .objects = {}, .used = f.end()});
    for (std::size_t k = 0; k < all.size(); ++k) {
        const Bin& bin = all[k];
        if (at < bin.lo) {
            const std::string stem = std::format("asm/frame{:02d}_{:02d}", seg, k);
            const std::string why = assemble_piece(stem, piece, at, bin.lo);
            if (!why.empty()) {
                b.frame_why = std::format("{} ({:04x}-{:04x}): {}", stem, at, bin.lo, why);
                return std::nullopt;
            }
            out.push_back(stem);
        }
        if (bin.lo == bin.hi) break;
        std::uint32_t pos = bin.lo;
        for (const auto& [pad, name] : bin.objects) {
            if (pad != 0) out.push_back(write_filler(assembler, seg, std::format("pad{:02d}_{:04x}", seg, pos), pad));
            out.push_back(cstem.at(name));
            pos += pad + size.at(name);
        }
        if (bin.hi > bin.used) out.push_back(write_filler(assembler, seg, std::format("fill{:02d}_{:04x}", seg, bin.used), bin.hi - bin.used));
        at = bin.hi;
    }
    out.insert(out.end(), after.begin(), after.end());
    return out;
}

}  // namespace cb::build::module::internal
