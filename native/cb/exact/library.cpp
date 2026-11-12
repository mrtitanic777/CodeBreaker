#include "cb/exact/library.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/bytesearch.hpp"

namespace cb::exact::library {

namespace stdfs = std::filesystem;
namespace omf = cb::formats::omf;

namespace {

bool any_nonzero(ByteView b) { return std::any_of(b.begin(), b.end(), [](std::uint8_t c) { return c != 0; }); }

// Whether img[lo, hi) (clamped to the image) holds a byte that is not zero.
bool any_nonzero_in(ByteView img, std::int64_t lo, std::int64_t hi) {
    lo = std::max<std::int64_t>(lo, 0);
    hi = std::min<std::int64_t>(hi, static_cast<std::int64_t>(img.size()));
    return lo < hi && any_nonzero(img.subspan(static_cast<std::size_t>(lo), static_cast<std::size_t>(hi - lo)));
}

std::int64_t word_at(ByteView b, std::size_t at) { return b[at] | (b[at + 1] << 8); }

// The alignment code of a SEGDEF as a number of bytes (1 for a code that is not a word, paragraph or page).
std::uint32_t align_bytes(std::uint8_t code) {
    switch (code) {
        case 2: return 2;
        case 3: return 16;
        case 4: return 256;
        default: return 1;
    }
}

Piece make_piece(const omf::Object& o, const std::string& segment, const Bytes& bytes, std::uint64_t filled) {
    Piece p;
    p.segment = segment;
    p.bytes.assign(bytes.begin(), bytes.begin() + static_cast<std::ptrdiff_t>(std::min<std::uint64_t>(filled, bytes.size())));
    p.hole.assign(p.bytes.size(), false);
    for (std::int64_t h : omf::holes(o, segment)) {
        ++p.hole_total;
        if (h >= 0 && static_cast<std::size_t>(h) < p.hole.size()) p.hole[static_cast<std::size_t>(h)] = true;
    }
    return p;
}

// Whether the piece's solid bytes are those of the image at `at`.
bool stands_at(ByteView img, std::size_t at, const Piece& p) {
    if (at > img.size() || p.bytes.size() > img.size() - at) return false;
    for (std::size_t x = 0; x < p.bytes.size(); ++x) {
        if (!p.hole[x] && img[at + x] != p.bytes[x]) return false;
    }
    return true;
}

}  // namespace

// ---- the library's modules ----------------------------------------------------------------------------------------

Member::Member(std::string tag_, std::string name_, Bytes raw_, int rank_)
    : tag(std::move(tag_)), name(std::move(name_)), raw(std::move(raw_)), rank(rank_) {
    object = omf::read(raw);
    for (const std::string& s : omf::code_segments(object)) {
        if (auto p = piece_of(s)) code.push_back(std::move(*p));
    }
    data = piece_of("_DATA");
    for (const omf::SegDef& s : object.segdefs) segments.push_back(s.name);
    for (const omf::Public& p : object.publics) {
        if (p.segment && std::find(segments.begin(), segments.end(), *p.segment) == segments.end()) segments.push_back(*p.segment);
        pubs.insert_or_assign(p.name, PublicPlace{p.segment, static_cast<std::uint32_t>(p.offset)});
    }
}

std::optional<Piece> Member::piece_of(std::string_view segment) const {
    const std::uint64_t* filled = object.filled.find(std::string(segment));
    const Bytes* bytes = object.segments.find(std::string(segment));
    if (filled == nullptr || *filled == 0 || bytes == nullptr) return std::nullopt;
    return make_piece(object, std::string(segment), *bytes, *filled);
}

Member::SegmentSize Member::seglen(std::string_view segment) const {
    for (const omf::SegDef& s : object.segdefs) {
        if (s.name == segment) return {static_cast<std::uint32_t>(s.length), align_bytes(s.alignment)};
    }
    return {0, 1};
}

std::vector<std::unique_ptr<Member>> read_modules(const Project& project) {
    std::vector<std::unique_ptr<Member>> out;
    int rank = 0;
    for (const Project::LibraryFile& lib : project.libraries()) {
        const Bytes file = fs::read_file(lib.file);
        for (const auto& [name, raw] : omf::library(file)) {
            try {
                out.push_back(std::make_unique<Member>(lib.tag, name, Bytes(raw.begin(), raw.end()), rank));
            } catch (const FormatError&) {   // (a record this reader does not know: not a candidate)
            }
        }
        ++rank;
    }
    return out;
}

std::vector<std::size_t> find_piece(ByteView img, const Piece& piece, std::size_t lo, std::size_t hi) {
    // the longest run with no hole is what is looked for (of equal runs the later)
    std::size_t best_len = 0, best_at = 0;
    bool any = false;
    for (std::size_t k = 0; k < piece.bytes.size();) {
        if (piece.hole[k]) {
            ++k;
            continue;
        }
        std::size_t e = k;
        while (e < piece.bytes.size() && !piece.hole[e]) ++e;
        if (!any || e - k >= best_len) {
            best_len = e - k;
            best_at = k;
            any = true;
        }
        k = e;
    }
    std::vector<std::size_t> out;
    if (!any) return out;
    hi = std::min(hi, img.size());
    const ByteView key = ByteView(piece.bytes).subspan(best_at, best_len);
    for (std::size_t p = find_bytes(img, key, lo, hi); p != img.size(); p = find_bytes(img, key, p + 1, hi)) {
        if (p < best_at + lo) continue;
        const std::size_t s = p - best_at;
        if (s + piece.bytes.size() <= hi && stands_at(img, s, piece)) out.push_back(s);
    }
    return out;
}

std::optional<std::int64_t> Placed::end() const {
    if (!code) return std::nullopt;
    return *code + (module->code.empty() ? 0 : static_cast<std::int64_t>(module->code[0].bytes.size()));
}

// ---- locating -------------------------------------------------------------------------------------------------------

namespace {

constexpr std::int64_t kNoLimit = 1 << 16;   // the data segment's end when nothing says it

using MemberSet = std::set<const Member*>;

// Everything `locate` reads of the program and learns on the way; one run of it makes one Map.
class Locator {
public:
    Locator(const Project& project, const x86::Decoder16& decoder)
        : project_(project), decoder_(decoder), img_(project.image()), dg_(project.program().dgroup), syms_(project.symbols()) {}

    Map run();

private:
    // -- what the words of placed modules show
    void add_refs(Refs& refs, const Placed& p) const;
    Refs refs_of(const std::vector<Placed>& placed) const {
        Refs refs;
        for (const Placed& p : placed) add_refs(refs, p);
        return refs;
    }
    bool callers_name(const Member& m, std::int64_t at) const;
    int agrees(const Member& m, std::int64_t code, const std::map<std::string, std::int64_t>& lib_pubs) const;
    bool byteless(const Member& m) const;

    // -- the stages of the walk
    std::vector<DataRaw> data_standins(MemberSet& taken, std::map<std::string, Missing>& missing, const Refs& refs,
                                       const std::vector<Placed>& coded, const std::vector<Placed>& donly) const;
    std::map<std::int64_t, RawData> raw_data(const std::vector<Raw>& raws) const;
    std::vector<OrderEntry> order_of(const Map& map) const;

    std::int64_t code_end() const;
    std::vector<Map::KitObject> kit_objects() const;
    std::set<std::string> first_object_names() const;

    const Project& project_;
    const x86::Decoder16& decoder_;
    ByteView img_;
    std::int64_t dg_;
    SymbolTable syms_;
    std::vector<const Member*> mods_;
    std::map<std::int64_t, std::string> code_at_;   // lookup only: the last name SYMBOLS gives an address
};

void Locator::add_refs(Refs& refs, const Placed& p) const {
    if (p.code) {
        const Member& m = *p.module;
        const std::string& s0 = m.code.at(0).segment;
        const Bytes* b = m.object.segments.find(s0);
        for (const omf::Fixup& fx : m.object.fixups) {
            if (b == nullptr || fx.segment != s0 || fx.target.kind != omf::Target::Kind::External || fx.at < 0) continue;
            const std::size_t fo = static_cast<std::size_t>(fx.at);
            const std::int64_t at = *p.code + static_cast<std::int64_t>(fo);
            if (fo + 2 > b->size() || at < 0 || static_cast<std::size_t>(at) + 2 > img_.size()) continue;
            const std::int64_t mine = (word_at(*b, fo) + fx.displacement) & 0xFFFF;
            const std::int64_t theirs = word_at(img_, static_cast<std::size_t>(at));
            const std::string& name = fx.target.name.value_or("");
            if (fx.kind == omf::FixupKind::Relative) {
                const std::int64_t base = project_.program().code_base(static_cast<std::uint32_t>(*p.code));
                refs.try_emplace(name, Ref{RefKind::Code, base + ((at + 2 + theirs - base) & 0xFFFF) - mine});
            } else if (fx.kind == omf::FixupKind::Offset) {
                refs.try_emplace(name, Ref{RefKind::Data, (theirs - mine) & 0xFFFF});
            }
        }
    }
    if (p.data) {
        const Bytes* b = p.module->object.segments.find("_DATA");
        for (const omf::Fixup& fx : p.module->object.fixups) {
            if (b == nullptr || fx.segment != "_DATA" || fx.target.kind != omf::Target::Kind::External || fx.kind != omf::FixupKind::Offset || fx.at < 0) continue;
            const std::size_t fo = static_cast<std::size_t>(fx.at);
            const std::int64_t at = dg_ + *p.data + static_cast<std::int64_t>(fo);
            if (fo + 2 > b->size() || at < 0 || static_cast<std::size_t>(at) + 2 > img_.size()) continue;
            refs.try_emplace(fx.target.name.value_or(""),
                             Ref{RefKind::Data, (word_at(img_, static_cast<std::size_t>(at)) - ((word_at(*b, fo) + fx.displacement) & 0xFFFF)) & 0xFFFF});
        }
    }
}

// Whether the program's callers use a public name of the module at this place: SYMBOLS names an address the module's
// public has there.
bool Locator::callers_name(const Member& m, std::int64_t at) const {
    const std::string& s0 = m.code.at(0).segment;
    for (const auto& [name, place] : m.pubs) {
        if (place.segment != s0) continue;
        const auto it = code_at_.find(at + place.offset);
        if (it != code_at_.end() && it->second == name) return true;
    }
    return false;
}

// Of modules of the same bytes: how many of this one's calls go where the names they call are placed (less those that
// go elsewhere).
int Locator::agrees(const Member& m, std::int64_t code, const std::map<std::string, std::int64_t>& lib_pubs) const {
    Placed tmp;
    tmp.module = &m;
    tmp.code = code;
    Refs refs;
    add_refs(refs, tmp);
    int n = 0;
    for (const auto& [name, ref] : refs) {
        if (ref.kind != RefKind::Code) continue;
        const auto it = lib_pubs.find(name);
        if (it != lib_pubs.end()) n += it->second == ref.address ? 1 : -1;
    }
    return n;
}

// A module with no code and no byte but zeros: its bytes tell nothing.
bool Locator::byteless(const Member& m) const {
    if (!m.code.empty()) return false;
    for (const auto& [name, bytes] : m.object.segments) {
        const std::uint64_t* filled = m.object.filled.find(name);
        const std::size_t n = std::min<std::size_t>(bytes.size(), filled ? *filled : 0);
        if (any_nonzero(ByteView(bytes).first(n))) return false;
    }
    return true;
}

// Where the code ends: before the data segment, less the zeros that pad to its paragraph.
std::int64_t Locator::code_end() const {
    std::int64_t e = dg_;
    while (e > dg_ - 16 && e > 0 && static_cast<std::size_t>(e) <= img_.size() && img_[static_cast<std::size_t>(e) - 1] == 0) --e;
    return e;
}

// The kit's loose objects (the profile's "objects" for the model: SVARSTCK.OBJ, SSETARGV.OBJ ...) whose code stands in
// the program at one place.
std::vector<Map::KitObject> Locator::kit_objects() const {
    std::vector<Map::KitObject> out;
    for (const stdfs::path& file : project_.kit_objects()) {
        if (!stdfs::exists(file)) continue;
        const Member m("", file.filename().string(), fs::read_file(file), -1);
        if (m.code.empty() || m.code[0].solid() < 3) continue;
        const auto places = find_piece(img_, m.code[0], 0, static_cast<std::size_t>(dg_));
        if (places.size() == 1) out.push_back({static_cast<std::int64_t>(places[0]), static_cast<std::int64_t>(places[0] + m.code[0].bytes.size()), m.label()});
    }
    return out;
}

// The publics of the kit's objects the program links before the library (project "first_objects").
std::set<std::string> Locator::first_object_names() const {
    std::set<std::string> out;
    for (const stdfs::path& file : project_.first_objects()) {
        for (const omf::Public& p : omf::read(fs::read_file(file)).publics) out.insert(p.name);
    }
    return out;
}

Map Locator::run() {
    Map mp;
    mp.members = read_modules(project_);
    for (const auto& m : mp.members) mods_.push_back(m.get());
    for (const auto& [name, place] : syms_) {   // (the last name an address has)
        if (place.kind == SymKind::Code && name.find(':') == std::string::npos) code_at_[place.address] = name;
    }
    const auto in_syms = [&](const std::string& n, SymKind kind) {
        const SymPlace* p = syms_.find(n);
        return p != nullptr && p->kind == kind;
    };

    OrderedMap<std::int64_t, std::vector<const Member*>> hits;   // code place -> the modules whose code is there
    std::vector<std::pair<const Member*, std::vector<std::size_t>>> multi;
    std::map<const Member*, std::vector<std::int64_t>> data_hits;   // lookup only
    for (const Member* m : mods_) {
        if (!m->code.empty()) {
            const Piece& c = m->code[0];
            if (c.solid() < 3) continue;
            const auto ps = find_piece(img_, c, 0, static_cast<std::size_t>(dg_));
            if (ps.size() == 1) hits[static_cast<std::int64_t>(ps[0])].push_back(m);
            else if (!ps.empty()) multi.emplace_back(m, ps);
        }
        if (m->data && m->data->solid() >= 4) {
            for (std::size_t p : find_piece(img_, *m->data, static_cast<std::size_t>(dg_), img_.size())) data_hits[m].push_back(static_cast<std::int64_t>(p) - dg_);
        }
    }

    OrderedMap<std::int64_t, Placed> placed;
    MemberSet in_placed;
    const auto place = [&](std::int64_t at, Placed p) {
        in_placed.insert(p.module);
        for (const Member* a : p.also) in_placed.insert(a);
        placed.insert_or_assign(at, std::move(p));
    };
    for (auto& [at, ms] : hits) {
        std::stable_sort(ms.begin(), ms.end(), [&](const Member* a, const Member* b) {
            return std::pair(!callers_name(*a, at), a->rank) < std::pair(!callers_name(*b, at), b->rank);
        });
        Placed p;
        p.module = ms[0];
        p.code = at;
        if (const auto it = data_hits.find(ms[0]); it != data_hits.end() && it->second.size() == 1) p.data = it->second[0];
        p.also.assign(ms.begin() + 1, ms.end());
        place(at, std::move(p));
    }
    for (const auto& [m, ps] : multi) {   // (a module whose bytes stand in several places)
        std::vector<std::int64_t> free;
        for (std::size_t a : ps) {
            if (!placed.contains(static_cast<std::int64_t>(a))) free.push_back(static_cast<std::int64_t>(a));
        }
        std::vector<std::int64_t> named;
        for (std::int64_t a : free) {
            if (callers_name(*m, a)) named.push_back(a);
        }
        if (named.size() == 1 || free.size() == 1) {
            const std::int64_t a = (named.empty() ? free : named)[0];
            Placed p;
            p.module = m;
            p.code = a;
            p.why = std::format("(by {})", named.empty() ? "the one place left" : "its name in SYMBOLS");
            place(a, std::move(p));
        }
    }
    {
        std::vector<Placed> now;
        for (const auto& [a, p] : placed) now.push_back(p);
        mp.refs = refs_of(now);
    }
    std::set<const Member*> multi_members;
    for (const auto& [m, ps] : multi) multi_members.insert(m);
    // a module too short to be told by its bytes (a jump to another routine): placed by its names' places
    for (const Member* m : mods_) {
        if (m->code.empty() || in_placed.contains(m)) continue;
        const Piece& c = m->code[0];
        if (c.solid() >= 3 && !multi_members.contains(m)) continue;
        for (const auto& [n, pub] : m->pubs) {
            const bool by_symbols = in_syms(n, SymKind::Code);
            const auto called = mp.refs.find(n);
            const bool by_call = called != mp.refs.end() && called->second.kind == RefKind::Code;
            if (pub.segment != c.segment || !(by_symbols || by_call)) continue;
            const std::int64_t a = (by_symbols ? syms_.at(n).address : called->second.address) - pub.offset;
            if (a >= 0 && !placed.contains(a) && stands_at(img_, static_cast<std::size_t>(a), c)) {
                Placed p;
                p.module = m;
                p.code = a;
                p.why = std::format("(by {}, which {} names)", n, by_symbols ? "SYMBOLS" : "a call of the library's");
                place(a, std::move(p));
                break;
            }
        }
    }
    std::vector<Placed> coded;
    {
        std::vector<std::int64_t> addresses;
        for (const auto& [a, p] : placed) addresses.push_back(a);
        std::sort(addresses.begin(), addresses.end());
        for (std::int64_t a : addresses) coded.push_back(placed.at(a));
    }
    // the library part: the run of modules at the end of the code
    const std::int64_t gap = project_.library_gap();
    std::optional<std::int64_t> start;
    if (const auto given = project_.library_start()) start = *given;
    if (!start && !coded.empty()) {
        std::size_t k = coded.size() - 1;
        while (k > 0 && *coded[k].code - *coded[k - 1].end() <= gap) --k;
        start = coded[k].code;
    }
    std::vector<Placed> lone;
    for (const Placed& p : coded) {   // (a library module among the program's own)
        if (start && *p.code < *start) lone.push_back(p);
    }
    std::map<std::string, std::int64_t> lib_pubs;
    for (const Placed& p : coded) {
        if (!start || *p.code < *start) continue;
        for (const Member* m : [&] { std::vector<const Member*> g{p.module}; g.insert(g.end(), p.also.begin(), p.also.end()); return g; }()) {
            for (const auto& [n, pub] : m->pubs) lib_pubs.try_emplace(n, *p.code + pub.offset);
        }
    }
    // a lone module counts when what it calls is in the library part (a wrapper of the program's own that has a library
    // routine's bytes calls the program's code: P2's f05a85, fscanf's bytes)
    std::erase_if(lone, [&](const Placed& p) {
        Refs refs;
        add_refs(refs, p);
        for (const auto& [n, ref] : refs) {
            const auto it = lib_pubs.find(n);
            if (ref.kind == RefKind::Code && (ref.address < *start || (it != lib_pubs.end() && it->second != ref.address))) return true;
        }
        return false;
    });
    std::erase_if(coded, [&](const Placed& p) { return start && *p.code < *start; });
    // every module of the same bytes at a place is one of its group (atoi and atol: one jump to __catox) ...
    MemberSet inplace;
    for (const Placed& p : coded) {
        inplace.insert(p.module);
        for (const Member* m : p.also) inplace.insert(m);
    }
    for (Placed& p : coded) {
        const Piece& c = p.module->code.at(0);
        for (const Member* m : mods_) {
            if (inplace.contains(m) || m->code.empty() || m->code[0].bytes.size() != c.bytes.size()) continue;
            const Piece& mc = m->code[0];
            bool same = mc.hole == c.hole;
            for (std::size_t k = 0; k < c.bytes.size() && same; ++k) same = c.hole[k] || mc.bytes[k] == c.bytes[k];
            if (same) {
                p.also.push_back(m);
                inplace.insert(m);
            }
        }
    }
    // ... and of a group the one taken is the one whose calls go where the modules they name are
    for (Placed& p : coded) {
        if (p.also.empty()) continue;
        std::vector<const Member*> group{p.module};
        group.insert(group.end(), p.also.begin(), p.also.end());
        const Member* best = group[0];
        std::pair<int, bool> best_key{agrees(*best, *p.code, lib_pubs), best == p.module};
        for (const Member* m : group) {
            const std::pair<int, bool> key{agrees(*m, *p.code, lib_pubs), m == p.module};
            if (key > best_key) {
                best = m;
                best_key = key;
            }
        }
        if (best != p.module) {
            p.also.clear();
            for (const Member* m : group) {
                if (m != best) p.also.push_back(m);
            }
            p.module = best;
            p.why = (p.why.empty() ? "" : p.why + " ") + "(of the same bytes, the one whose calls go where their names are)";
        }
    }
    const Refs refs = refs_of(coded);
    MemberSet taken;
    for (const Placed& p : coded) {
        taken.insert(p.module);
        for (const Member* m : p.also) taken.insert(m);
    }
    // modules of data only (by their _DATA, or another data segment's bytes: the 8087 fixups' CDATA)
    std::vector<Placed> donly, first;
    for (const Member* m : mods_) {
        if (!m->code.empty() || taken.contains(m)) continue;
        std::optional<std::pair<std::int64_t, std::string>> got;
        const auto dp = data_hits.find(m);
        if (dp != data_hits.end() && dp->second.size() == 1) {
            got = {dp->second[0], ""};
        } else {
            for (const auto& [n, pub] : m->pubs) {
                if (pub.segment != "_DATA") continue;
                std::optional<std::int64_t> a;
                if (in_syms(n, SymKind::Data)) a = syms_.at(n).address;
                else if (const auto r = refs.find(n); r != refs.end() && r->second.kind == RefKind::Data) a = r->second.address;
                if (!a) continue;
                const std::int64_t base = *a - pub.offset;
                bool ok = syms_.contains(n);
                if (!ok && m->data) {
                    ok = true;
                    for (std::size_t x = 0; x < m->data->bytes.size() && ok; ++x) {
                        if (m->data->hole[x]) continue;
                        const std::int64_t at = dg_ + base + static_cast<std::int64_t>(x);
                        ok = at >= 0 && static_cast<std::size_t>(at) < img_.size() && img_[static_cast<std::size_t>(at)] == m->data->bytes[x];
                    }
                }
                if (ok) {
                    got = {base, std::format("(by {}, {:04x}{})", n, *a, syms_.contains(n) ? " in SYMBOLS" : ": a word of the code")};
                    break;
                }
            }
        }
        if (got) {
            const auto same = std::find_if(donly.begin(), donly.end(), [&](const Placed& d) {
                const auto a = d.module->seglen("_DATA"), b = m->seglen("_DATA");
                return d.data == got->first && a.length == b.length && a.align == b.align;
            });
            if (same != donly.end()) {   // (the same data in another library: one module)
                same->also.push_back(m);
            } else {
                Placed d;
                d.module = m;
                d.data = got->first;
                d.why = got->second;
                donly.push_back(std::move(d));
            }
            taken.insert(m);
            continue;
        }
        std::vector<Piece> other;
        for (const omf::SegDef& s : m->object.segdefs) {
            if (s.name == "_DATA") continue;
            if (auto p = m->piece_of(s.name)) other.push_back(std::move(*p));
        }
        const bool all_found = std::all_of(other.begin(), other.end(), [&](const Piece& d) { return d.solid() < 4 || find_piece(img_, d, static_cast<std::size_t>(dg_), img_.size()).size() == 1; });
        const bool some_tell = std::any_of(other.begin(), other.end(), [](const Piece& d) { return d.solid() >= 4; });
        if (!other.empty() && all_found && some_tell) {
            std::vector<std::string> names;
            for (const Piece& d : other) names.push_back(d.segment);
            Placed p;
            p.module = m;
            p.why = "(no code: its " + text::join(names, ", ") + " found in the data)";
            first.push_back(std::move(p));
            taken.insert(m);
        }
    }
    // a module its bytes cannot tell (none, or zeros the linker fills) is taken
    //  - when a placed module names one of its publics and nothing placed (or the kit's objects linked first) defines
    //    it: the 8087 fixups' FIxRQQ, which the emulator names
    //  - when it names a placed module's public that nothing else placed names (it is what took that module):
    //    __fltused's module, which names fakcvt's routines
    std::set<std::string> defined = first_object_names();
    std::map<std::string, std::vector<std::string>> named;   // external name -> the placed modules that name it
    const auto each_placed = [&](auto fn) {
        for (const std::vector<Placed>* group : {&coded, &donly, &first}) {
            for (const Placed& p : *group) fn(p);
        }
    };
    each_placed([&](const Placed& p) {
        for (const Member* m : [&] { std::vector<const Member*> g{p.module}; g.insert(g.end(), p.also.begin(), p.also.end()); return g; }()) {
            for (const auto& [n, pub] : m->pubs) defined.insert(n);
        }
        for (const std::string& e : p.module->object.externals) named[e].push_back(p.module->label());   // (of a group of the same bytes, the one taken)
    });
    for (const Member* m : mods_) {
        if (!byteless(*m) || taken.contains(m) || m->pubs.empty()) continue;
        std::vector<std::string> why;
        for (const auto& [n, pub] : m->pubs) {
            if (named.contains(n) && !defined.contains(n)) why.push_back(n);
        }
        std::sort(why.begin(), why.end());
        if (why.empty()) continue;
        Placed p;
        p.module = m;
        p.why = std::format("(its bytes tell nothing: {}, which {} names)", why[0], named.at(why[0])[0]);
        first.push_back(std::move(p));
        taken.insert(m);
        for (const auto& [n, pub] : m->pubs) defined.insert(n);
    }
    for (const std::vector<Placed>* group : {&coded, &donly}) {
        for (const Placed& p : *group) {
            std::set<std::string> own{p.module->label()};
            for (const Member* m : p.also) own.insert(m->label());
            std::set<std::string> alone;   // the publics of the group that no module outside it names
            for (const Member* m : [&] { std::vector<const Member*> g{p.module}; g.insert(g.end(), p.also.begin(), p.also.end()); return g; }()) {
                for (const auto& [n, pub] : m->pubs) {
                    bool outside = false;
                    if (const auto it = named.find(n); it != named.end()) {
                        for (const std::string& who : it->second) outside = outside || !own.contains(who);
                    }
                    if (!outside) alone.insert(n);
                }
            }
            for (const Member* m : mods_) {
                if (!byteless(*m) || taken.contains(m) || m->pubs.empty()) continue;
                std::string took;
                for (const std::string& e : m->object.externals) {
                    if (alone.contains(e) && !m->pubs.contains(e)) {
                        took = e;
                        break;
                    }
                }
                if (took.empty()) continue;
                Placed f;
                f.module = m;
                f.why = std::format("(its bytes tell nothing: it took {}, for {}, which no other module names)", p.module->label(), took);
                first.push_back(std::move(f));
                taken.insert(m);
            }
        }
    }
    {
        const auto index_of = [&](const Member* m) { return std::find(mods_.begin(), mods_.end(), m) - mods_.begin(); };
        std::stable_sort(first.begin(), first.end(), [&](const Placed& a, const Placed& b) {
            return std::pair(a.module->rank, index_of(a.module)) < std::pair(b.module->rank, index_of(b.module));
        });
    }
    // names the placed modules need that nothing placed defines, and the library modules that would: another version of
    // those is in the program (a stand-in), or the original link left them unresolved too
    for (const Placed& p : first) {
        for (const auto& [n, pub] : p.module->pubs) defined.insert(n);
    }
    std::map<std::string, Missing> missing;
    for (const auto& [n, who] : named) {
        if (defined.contains(n) || n.starts_with("FI") || n.starts_with("FJ")) continue;
        std::set<std::string> by;
        for (const Member* m : mods_) {
            if (m->pubs.contains(n)) by.insert(m->label());
        }
        missing[n] = {who[0], std::vector<std::string>(by.begin(), by.end())};
    }
    // stand-ins for the gaps: not zeros (a pad, a segment of zeros), nor a placed module's far data
    std::vector<std::pair<std::int64_t, std::int64_t>> far;
    for (const Placed& p : coded) {
        for (const omf::SegDef& s : p.module->object.segdefs) {
            const bool is_code = std::any_of(p.module->code.begin(), p.module->code.end(), [&](const Piece& c) { return c.segment == s.name; });
            const std::string cls = text::upper(s.class_name);
            if (is_code || cls == "DATA" || cls == "BSS" || cls == "STACK" || cls == "CONST") continue;
            const auto d = p.module->piece_of(s.name);
            if (!d || !any_nonzero(d->bytes)) continue;
            const auto ps = find_piece(img_, *d, static_cast<std::size_t>(*start), static_cast<std::size_t>(dg_));
            if (ps.size() == 1) far.emplace_back(static_cast<std::int64_t>(ps[0]), static_cast<std::int64_t>(ps[0]) + s.length);
        }
    }
    std::vector<Raw> raws;
    if (!coded.empty()) {
        std::int64_t end = project_.code_end() != 0 ? project_.code_end() : code_end();
        if (!far.empty()) end = std::min(end, std::min_element(far.begin(), far.end())->first);
        std::int64_t later = -1;
        for (std::uint32_t a : modules::starts(project_)) {
            if (a > *start && (later < 0 || a < later)) later = a;
        }
        if (later >= 0) end = std::min(end, later);   // (the program's own modules after the library: a code segment of their own, placed later by the linker)
        std::map<std::string, int> seg_count;
        for (const Placed& p : coded) ++seg_count[p.module->code[0].segment];
        std::string main_seg;   // the code segment most modules are in (a tie: the first by name)
        int most = 0;
        for (const auto& [name, count] : seg_count) {
            if (count > most) {
                most = count;
                main_seg = name;
            }
        }
        if (coded.back().module->code[0].segment != main_seg) end = std::min(end, *coded.back().end());   // (the last is a code segment of its own: what follows is of the classes after CODE)
        std::set<std::string> pubs_all;
        for (const Placed& p : coded) {
            for (const Member* m : [&] { std::vector<const Member*> g{p.module}; g.insert(g.end(), p.also.begin(), p.also.end()); return g; }()) {
                for (const auto& [n, pub] : m->pubs) pubs_all.insert(n);
            }
        }
        // names the program places: what the library's words show, then SYMBOLS's (which win)
        std::map<std::string, std::pair<char, std::int64_t>> known;
        for (const auto& [n, r] : refs) known[n] = {r.kind == RefKind::Code ? 'C' : 'D', r.address};
        for (const auto& [n, s] : syms_) {
            if (n.find(':') == std::string::npos) known[n] = {sym_kind_letter(s.kind), s.address};
        }
        std::int64_t pos = *start;
        std::vector<std::pair<std::int64_t, std::int64_t>> far_sorted = far;
        std::sort(far_sorted.begin(), far_sorted.end());
        for (std::size_t i = 0; i <= coded.size(); ++i) {
            const Placed* p = i < coded.size() ? &coded[i] : nullptr;
            const std::int64_t nxt = p ? *p->code : end;
            if (nxt > pos) {
                std::int64_t lo = pos;
                for (const auto& [a, b] : far_sorted) {
                    if ((a <= lo && lo < b) || (lo < a && a < nxt && !any_nonzero_in(img_, lo, a))) lo = std::max(lo, b);
                }
                std::int64_t hi = nxt;
                while (hi - lo > 1 && hi <= static_cast<std::int64_t>(img_.size()) && img_[static_cast<std::size_t>(hi) - 1] == 0) --hi;   // (zeros before the next module: pads, not the stand-in's)
                if ((hi & 1) && hi < nxt && (lo & 1) == 0) ++hi;                                                                           // (but a module's code of its own is of even length)
                if (any_nonzero_in(img_, lo, nxt)) {
                    pos = lo;
                    std::vector<std::pair<std::int64_t, std::string>> pubs;
                    for (const auto& [n, kv] : known) {
                        if (kv.first == 'C' && pos <= kv.second && kv.second < hi && !pubs_all.contains(n)) pubs.emplace_back(kv.second - pos, n);
                    }
                    std::sort(pubs.begin(), pubs.end());
                    raws.push_back({pos, hi, std::move(pubs)});
                }
            }
            if (p) pos = std::max(pos, *p->end());
        }
    }
    mp.start = start;
    mp.coded = std::move(coded);
    mp.donly = std::move(donly);
    mp.first = std::move(first);
    mp.raws = std::move(raws);
    mp.lone = std::move(lone);
    mp.refs = refs;
    {
        std::vector<Placed> both = mp.coded;
        both.insert(both.end(), mp.donly.begin(), mp.donly.end());
        mp.all_refs = refs_of(both);
    }
    mp.objects = kit_objects();
    mp.data_raws = data_standins(taken, missing, mp.all_refs, mp.coded, mp.donly);
    for (const DataRaw& d : mp.data_raws) {
        for (const auto& [n, off] : d.pubs) missing.erase(n);
    }
    mp.raw_data = raw_data(mp.raws);
    for (const Raw& r : mp.raws) {
        for (const auto& [o, n] : r.pubs) missing.erase(n);
    }
    mp.missing = std::move(missing);
    mp.order = order_of(mp);
    for (DataRaw& d : mp.data_raws) {   // (the modules after the stand-in, before the next known data, have their _DATA there too: it ends before them)
        if (!d.by_next) continue;
        const auto at = std::find_if(mp.order.begin(), mp.order.end(), [&](const OrderEntry& e) {
            const auto* x = std::get_if<const DataRaw*>(&e.what);
            return x != nullptr && *x == &d;
        });
        std::int64_t room = 0;
        for (auto it = at + 1; it != mp.order.end(); ++it) {
            if (std::holds_alternative<const DataRaw*>(it->what)) break;
            if (const auto* placed_item = std::get_if<const Placed*>(&it->what)) {
                if ((*placed_item)->data) break;
                if (it->item == Item::Module) {
                    const auto size = (*placed_item)->module->seglen("_DATA");
                    room += size.length + (size.length != 0 && size.align == 2 ? (room & 1) : 0);
                }
            }
        }
        d.end -= room;
        OrderedMap<std::string, std::int64_t> kept;
        for (const auto& [n, o] : d.pubs) {
            if (o < d.end - d.data) kept.insert_or_assign(n, o);
        }
        d.pubs = std::move(kept);
    }
    return mp;
}

// For each data-only module whose publics the program needs but whose bytes are not the kit's: where its data stands
// (the publics' places, the words of the placed modules show them, less their offsets in the kit's module: the first
// agree), to the next data whose place is known; its publics: those placed by words, and those before the first that
// moved at the kit's offsets.
std::vector<DataRaw> Locator::data_standins(MemberSet& taken, std::map<std::string, Missing>& missing, const Refs& refs,
                                            const std::vector<Placed>& coded, const std::vector<Placed>& donly) const {
    std::vector<DataRaw> out;
    std::set<std::int64_t> starts;
    for (const std::vector<Placed>* group : {&coded, &donly}) {
        for (const Placed& p : *group) {
            if (p.data) starts.insert(*p.data);
        }
    }
    for (const Member* m : mods_) {
        if (!m->code.empty() || taken.contains(m) || !m->data) continue;
        if (std::none_of(m->pubs.begin(), m->pubs.end(), [&](const auto& kv) { return missing.contains(kv.first); })) continue;
        std::vector<std::pair<std::int64_t, std::string>> kit;   // (offset, name) of the publics in _DATA
        for (const auto& [n, pub] : m->pubs) {
            if (pub.segment == "_DATA") kit.emplace_back(pub.offset, n);
        }
        std::sort(kit.begin(), kit.end());
        struct Seen {
            std::int64_t off;
            std::string name;
            std::int64_t address;
        };
        std::vector<Seen> placed;
        for (const auto& [off, n] : kit) {
            const auto r = refs.find(n);
            if (r != refs.end() && r->second.kind == RefKind::Data) placed.push_back({off, n, r->second.address});
        }
        if (placed.empty()) continue;
        const std::int64_t lo = placed[0].address - placed[0].off;
        const std::int64_t span = m->seglen("_DATA").length;   // (a public placed far off is not in this version's module)
        std::erase_if(placed, [&](const Seen& s) { return !(lo <= s.address && s.address < lo + span + 16); });
        std::int64_t moved = kNoLimit;
        for (const Seen& s : placed) {
            if (s.address - lo != s.off) moved = std::min(moved, s.off);
        }
        std::set<std::int64_t> shifts;
        for (const Seen& s : placed) {
            if (s.off >= moved) shifts.insert(s.address - lo - s.off);
        }
        std::int64_t hi = kNoLimit;
        for (std::int64_t s : starts) {
            if (s > lo) {
                hi = std::min(hi, s);
                break;
            }
        }
        bool by_next = true;
        if (shifts.size() <= 1) {   // (what moved, moved alike: the version's length is the kit's and that much)
            const std::int64_t own = lo + span + (shifts.empty() ? 0 : *shifts.begin());
            if (own <= hi) {
                hi = own;
                by_next = false;
            }
        }
        DataRaw d;
        d.name = m->name;
        d.member = m;
        d.source = m;
        d.data = lo;
        d.end = hi;
        for (const Seen& s : placed) {
            if (lo <= s.address && s.address < hi) d.pubs.insert_or_assign(s.name, s.address - lo);
        }
        for (const auto& [off, n] : kit) {   // (a public the words place elsewhere is not this one's)
            if (off < moved && !d.pubs.contains(n) && !refs.contains(n)) d.pubs.insert_or_assign(n, off);
        }
        d.why = std::format("(another version of {}: its data as the program has it{})", m->label(),
                            moved < kNoLimit ? std::format(", the publics from +{:x} on moved", moved) : std::string());
        d.by_next = by_next;
        out.push_back(std::move(d));
        taken.insert(m);
    }
    // a needed name the words place where no module's data is (another version keeps it in a module of its own): a
    // stand-in of its own, as long as the kit's module gives it (the link's drift map tells if it is more)
    std::vector<std::pair<std::int64_t, std::int64_t>> covered;
    std::set<std::string> have;
    for (const DataRaw& d : out) {
        covered.emplace_back(d.data, d.end);
        for (const auto& [n, o] : d.pubs) have.insert(n);
    }
    const auto sizes = project_.data_sizes();
    for (const auto& [n, miss] : missing) {
        const auto r = refs.find(n);
        if (have.contains(n) || r == refs.end() || r->second.kind != RefKind::Data) continue;
        const std::int64_t a = r->second.address;
        if (std::any_of(covered.begin(), covered.end(), [&](const auto& c) { return c.first <= a && a < c.second; })) continue;
        const Member* m = nullptr;
        for (const Member* candidate : mods_) {
            const PublicPlace* pub = candidate->pubs.find(n);
            if (pub != nullptr && pub->segment == "_DATA") {
                m = candidate;
                break;
            }
        }
        if (m == nullptr) continue;
        const std::int64_t off = m->pubs.at(n).offset;
        std::int64_t next = m->seglen("_DATA").length;
        for (const auto& [nm, pub] : m->pubs) {
            if (pub.segment == "_DATA" && pub.offset > off) next = std::min<std::int64_t>(next, pub.offset);
        }
        const auto given = sizes.find(n);
        const std::int64_t size = given != sizes.end() ? given->second : next - off;
        DataRaw d;
        d.name = std::string(n.begin() + static_cast<std::ptrdiff_t>(std::min(n.find_first_not_of('_'), n.size())), n.end());
        d.source = m;
        d.data = a;
        d.end = a + size;
        d.pubs.insert_or_assign(n, 0);
        d.why = std::format("({}, which {} defines in the kit: here in a module of its own; {} bytes{})", n, m->label(), size, given != sizes.end() ? "" : ", a guess");
        out.push_back(std::move(d));
    }
    // zeros of the data no module accounts for, which the link's fit found
    std::vector<Project::DataPad> pads = project_.data_pads();
    std::sort(pads.begin(), pads.end(), [](const Project::DataPad& a, const Project::DataPad& b) { return a.address < b.address; });
    for (const Project::DataPad& pad : pads) {
        DataRaw d;
        d.name = std::format("pad{:04x}", pad.address);
        d.data = pad.address;
        d.end = static_cast<std::int64_t>(pad.address) + pad.length;
        d.why = "(zeros no module of the kit accounts for: a pad the link's fit found)";
        out.push_back(std::move(d));
    }
    return out;
}

namespace {

// The memory operands [0xNNN] / [0xNNNN] an instruction's text names.
std::vector<std::uint32_t> memory_operands(std::string_view op) {
    std::vector<std::uint32_t> out;
    for (std::size_t at = op.find("[0x"); at != std::string_view::npos; at = op.find("[0x", at + 1)) {
        std::size_t e = at + 3;
        while (e < op.size() && ((op[e] >= '0' && op[e] <= '9') || (op[e] >= 'a' && op[e] <= 'f'))) ++e;
        const std::size_t digits = e - (at + 3);
        if ((digits == 3 || digits == 4) && e < op.size() && op[e] == ']') out.push_back(*text::parse_int<std::uint32_t>(op.substr(at + 3, digits), 16));
    }
    return out;
}

}  // namespace

// {a code stand-in's start: its data} - a code stand-in is another version of the kit's module that defines its
// publics; that module's data, as far as the program has it (placed by its first run of bytes that are not zeros, as
// long as the bytes agree, its zeros at the end left out), is the stand-in's.
std::map<std::int64_t, RawData> Locator::raw_data(const std::vector<Raw>& raws) const {
    std::map<std::int64_t, RawData> out;
    const ByteView ds = dg_ <= static_cast<std::int64_t>(img_.size()) ? img_.subspan(static_cast<std::size_t>(dg_)) : ByteView();
    for (const Raw& raw : raws) {
        for (const auto& [o, n] : raw.pubs) {
            for (const Member* m : mods_) {
                if (!m->pubs.contains(n) || !m->data) continue;
                const Piece& kit = *m->data;
                std::size_t r0 = 0;
                while (r0 < kit.bytes.size() && (kit.bytes[r0] == 0 || kit.hole[r0])) ++r0;
                if (r0 == kit.bytes.size()) continue;
                std::size_t r1 = r0;
                while (r1 < kit.bytes.size() && kit.bytes[r1] != 0 && !kit.hole[r1] && r1 - r0 < 12) ++r1;
                if (r1 - r0 < 4) continue;
                const ByteView key = ByteView(kit.bytes).subspan(r0, r1 - r0);
                std::optional<std::pair<std::size_t, std::size_t>> best;   // (where the module's data would begin, how far the bytes agree)
                for (std::size_t p = find_bytes(ds, key); p != ds.size(); p = find_bytes(ds, key, p + 1)) {
                    if (p < r0) continue;
                    const std::size_t base = p - r0;
                    std::size_t k = 0;
                    while (k < kit.bytes.size() && base + k < ds.size() && (kit.hole[k] || ds[base + k] == kit.bytes[k])) ++k;
                    if (!best || k > best->second) best = std::make_pair(base, k);
                }
                if (!best || best->second < r1) continue;
                auto [base, k] = *best;
                while (k > r1 && kit.bytes[k - 1] == 0) --k;
                k += k & 1;
                RawData rd;
                rd.from = static_cast<std::int64_t>(base);
                rd.to = static_cast<std::int64_t>(base + k);
                for (const auto& [nm, pub] : m->pubs) {
                    if (pub.segment == "_DATA" && pub.offset < k) rd.pubs.insert_or_assign(nm, pub.offset);
                }
                rd.note = "the data of " + m->label() + " as far as the program has it";
                out[raw.lo] = std::move(rd);
                break;
            }
            if (out.contains(raw.lo)) break;
        }
    }
    // a stand-in no kit module's data fits: the data its own instructions address, where no placed module's is
    std::set<std::int64_t> known_data;
    for (const auto& [n, s] : syms_) {
        if (s.kind == SymKind::Data && n.find(':') == std::string::npos) known_data.insert(s.address);
    }
    for (const Raw& raw : raws) {
        if (out.contains(raw.lo)) continue;
        std::set<std::int64_t> hits;
        for (const x86::Insn& insn : x86::disasm(img_, static_cast<std::uint32_t>(raw.lo), static_cast<std::uint32_t>(raw.hi), true, decoder_)) {
            for (std::uint32_t v : memory_operands(insn.op_str)) hits.insert(v);
        }
        std::vector<std::pair<std::int64_t, std::int64_t>> runs;
        for (std::int64_t h : hits) {
            if (known_data.contains(h)) continue;
            if (!runs.empty() && h - runs.back().second <= 2) runs.back().second = h + 2;
            else runs.emplace_back(h, h + 2);
        }
        std::erase_if(runs, [](const auto& r) { return r.second - r.first > 16; });
        if (runs.size() == 1) {
            RawData rd;
            rd.from = runs[0].first;
            rd.to = runs[0].second;
            rd.note = "its own data, which its instructions address";
            out[raw.lo] = std::move(rd);
        }
    }
    return out;
}

// The modules in the linker's order. By code; a module of data only where its data stands among the coded modules'
// data: the library's data is laid out module after module (the lengths and alignments of their SEGDEFs), a data-only
// module going in where the layout reaches its place; of the starts for the library's data, the one that puts the most
// modules whose data's place is known at that place.
std::vector<OrderEntry> Locator::order_of(const Map& map) const {
    struct Row {
        std::int64_t at;
        Item item;
        std::variant<const Placed*, const Raw*> what;
    };
    std::vector<Row> rows;
    for (const Placed& p : map.coded) rows.push_back({*p.code, Item::Module, &p});
    for (const Raw& r : map.raws) rows.push_back({r.lo, Item::Raw, &r});
    std::stable_sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return a.at < b.at; });
    struct Block {
        std::int64_t data;
        std::int64_t length;
        std::variant<const Placed*, const DataRaw*> what;
    };
    std::vector<Block> blocks;
    for (const Placed& p : map.donly) blocks.push_back({*p.data, p.module->seglen("_DATA").length, &p});
    for (const DataRaw& d : map.data_raws) blocks.push_back({d.data, d.end - d.data, &d});
    std::stable_sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) { return a.data < b.data; });
    std::vector<OrderEntry> out;
    for (const Placed& p : map.first) out.push_back({Item::First, &p});
    if (blocks.empty()) {
        for (const Row& r : rows) {
            if (const auto* p = std::get_if<const Placed*>(&r.what)) out.push_back({r.item, *p});
            else out.push_back({r.item, std::get<const Raw*>(r.what)});
        }
        return out;
    }
    // the layout from data start `s`: how many modules land where they were found, and (when asked) the order
    const auto lay = [&](std::int64_t s, std::vector<OrderEntry>* order) {
        int score = 0;
        std::int64_t pos = s;
        std::size_t bi = 0;
        for (const Row& r : rows) {
            std::int64_t ln = 0, al = 1;
            if (const auto* p = std::get_if<const Placed*>(&r.what)) {
                const auto size = (*p)->module->seglen("_DATA");
                ln = size.length;
                al = size.align;
            } else {
                const auto rd = map.raw_data.find(std::get<const Raw*>(r.what)->lo);
                if (rd != map.raw_data.end()) {
                    ln = rd->second.to - rd->second.from;
                    al = 2;
                }
            }
            while (bi < blocks.size() && blocks[bi].data <= pos + (al == 2 ? (pos & 1) : 0)) {
                if (order) order->push_back({Item::Data, std::visit([](auto* x) -> std::variant<const Placed*, const DataRaw*, const Raw*> { return x; }, blocks[bi].what)});
                pos = std::max(pos, blocks[bi].data + blocks[bi].length);
                ++bi;
            }
            if (ln != 0 && al == 2 && (pos & 1)) ++pos;
            if (const auto* p = std::get_if<const Placed*>(&r.what); p && (*p)->data && ln != 0) score += *(*p)->data == pos;
            if (order) {
                if (const auto* p = std::get_if<const Placed*>(&r.what)) order->push_back({r.item, *p});
                else order->push_back({r.item, std::get<const Raw*>(r.what)});
            }
            pos += ln;
        }
        for (; order && bi < blocks.size(); ++bi) {
            order->push_back({Item::Data, std::visit([](auto* x) -> std::variant<const Placed*, const DataRaw*, const Raw*> { return x; }, blocks[bi].what)});
        }
        return score;
    };
    std::int64_t lowest = blocks[0].data, highest = blocks[0].data;
    for (const Block& b : blocks) {
        lowest = std::min(lowest, b.data);
        highest = std::max(highest, b.data);
    }
    for (const Row& r : rows) {
        if (const auto* p = std::get_if<const Placed*>(&r.what); p && (*p)->data) {
            lowest = std::min(lowest, *(*p)->data);
            highest = std::max(highest, *(*p)->data);
        }
    }
    int best_score = -1;
    std::int64_t best_start = 0;
    for (std::int64_t s = std::max<std::int64_t>(0, lowest - 0x800); s <= highest; ++s) {
        const int score = lay(s, nullptr);
        if (score > best_score) {
            best_score = score;
            best_start = s;
        }
    }
    lay(best_start, &out);
    return out;
}

}  // namespace

Map locate(const Project& project, const x86::Decoder16& decoder) { return Locator(project, decoder).run(); }

// ---- src/CRTLIB -----------------------------------------------------------------------------------------------------

std::vector<std::string> lines(const Map& mp, std::string_view title) {
    std::vector<std::string> out;
    out.push_back(title.empty() ? "# the library's part of the program, in the order the linker took its modules" : "# " + std::string(title));
    out.push_back("# (codebreaker exact library): where each module's code (or data) stands in the program.");
    const auto by_offset = [](const OrderedMap<std::string, std::int64_t>& pubs, bool tie_by_name) {
        std::vector<std::pair<std::string, std::int64_t>> v(pubs.begin(), pubs.end());
        std::stable_sort(v.begin(), v.end(), [&](const auto& a, const auto& b) {
            return tie_by_name ? std::pair(a.second, a.first) < std::pair(b.second, b.first) : a.second < b.second;
        });
        return v;
    };
    for (const OrderEntry& e : mp.order) {
        if (const auto* raw = std::get_if<const Raw*>(&e.what)) {
            std::string nm = "lib" + std::format("{:05x}", (*raw)->lo);
            if (!(*raw)->pubs.empty()) {
                const std::string& first = (*raw)->pubs[0].second;
                nm = first.substr(std::min(first.find_first_not_of("_$"), first.size()));
            }
            std::string pubs, data_text;
            for (const auto& [o, n] : (*raw)->pubs) pubs += std::format(" pub={}@{:x}", n, o);
            std::string note;
            if (const auto rd = mp.raw_data.find((*raw)->lo); rd != mp.raw_data.end()) {
                data_text = std::format(" data={:04x}-{:04x}", rd->second.from, rd->second.to);
                for (const auto& [n, o] : by_offset(rd->second.pubs, false)) data_text += std::format(" dpub={}@{:x}", n, o);
                note = "; " + rd->second.note;
            }
            out.push_back(std::format("RAW {} code={:05x}-{:05x}{}{}  # (no library module has these bytes{})", nm, (*raw)->lo, (*raw)->hi, pubs, data_text, note));
            continue;
        }
        if (const auto* d = std::get_if<const DataRaw*>(&e.what)) {
            std::string dpubs;
            for (const auto& [n, o] : by_offset((*d)->pubs, true)) dpubs += std::format(" dpub={}@{:x}", n, o);
            out.push_back(std::format("RAW {} data={:04x}-{:04x}{}  # {}", (*d)->name, (*d)->data, (*d)->end, dpubs, (*d)->why));
            continue;
        }
        const Placed& p = *std::get<const Placed*>(e.what);
        std::vector<std::string> note;
        if (p.code) note.push_back(std::format("code {:05x} ({} bytes)", *p.code, *p.end() - *p.code));
        if (p.data) note.push_back(std::format("data {:04x}", *p.data));
        if (!p.why.empty()) note.push_back(p.why);
        std::vector<const Member*> group{p.module};   // (the same bytes: the build takes the one it needs)
        group.insert(group.end(), p.also.begin(), p.also.end());
        std::stable_sort(group.begin(), group.end(), [](const Member* a, const Member* b) { return a->label() < b->label(); });
        for (const Member* m : group) {
            if (m == p.module) out.push_back(std::format("{:<16} # {}", m->label(), text::join(note, "  ")));
            else out.push_back(std::format("{:<16} # {}  (the same bytes as {})", m->label(), p.code ? std::format("code {:05x}", *p.code) : std::format("data {:04x}", *p.data), p.module->label()));
        }
    }
    return out;
}

int run(const Project& project, const std::vector<std::string>& args, const x86::Decoder16& decoder) {
    const Map mp = locate(project, decoder);
    const std::string txt = text::join(lines(mp), "\n") + "\n";
    out("{}\n", txt);
    for (const Placed& p : mp.lone) out("   among the program's own code: {} at {:05x} ({} bytes)\n", p.module->label(), *p.code, *p.end() - *p.code);
    for (const Map::KitObject& o : mp.objects) out("   the kit's object {} at {:05x} ({} bytes)\n", o.label, o.start, o.end - o.start);
    std::int64_t stand_in_bytes = 0;
    for (const Raw& r : mp.raws) stand_in_bytes += r.hi - r.lo;
    out("library part from {:05x}: {} modules with code, {} of data only, {} stand-ins ({} bytes)\n", mp.start.value_or(0), mp.coded.size(), mp.donly.size(),
        mp.raws.size(), stand_in_bytes);
    std::vector<std::string> own;
    for (const auto& [n, miss] : mp.missing) {
        if (!miss.defined_by.empty()) {
            out("   needed, not in the program: {} (named by {}; defined by {}) - another version there, or an object\n", n, miss.named_by, text::join(miss.defined_by, ", "));
        } else {
            own.push_back(n);
        }
    }
    if (!own.empty()) out("   the program's or the linker's (no library module defines them): {}\n", text::join(own, ", "));
    if (std::find(args.begin(), args.end(), "--write") != args.end()) fs::write_text(project.path("CRTLIB"), txt);
    return 0;
}

}  // namespace cb::exact::library
