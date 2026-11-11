#include "cb/exact/recut.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>

#include "cb/core/error.hpp"
#include "cb/core/io.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/fixupp.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::recut {

namespace {

namespace omf = formats::omf;
using fixupp::Fixup;

constexpr const char* kReader = "exact.recut";

// OMF record types (16-bit forms).
constexpr std::uint8_t kLnames = 0x96;
constexpr std::uint8_t kSegdef = 0x98;
constexpr std::uint8_t kFixupp = 0x9C;
constexpr std::uint8_t kLedata = 0xA0;
constexpr std::uint8_t kLidata = 0xA2;

constexpr std::uint8_t kLocBase = 2;        // a segment word
constexpr std::uint8_t kLocPointer = 3;     // offset and segment, four bytes
constexpr std::size_t kMaxDataRecord = 1024;   // no data record is written longer than this
constexpr std::size_t kMaxFixupsPerRecord = 100;   // nor a FIXUPP record with more fix-ups than this

// A fix-up the loader must add the load segment to: a segment word or a pointer.
bool is_relocated(const Fixup& f) { return f.location == kLocBase || f.location == kLocPointer; }

// The offset of the word of a relocated fix-up that holds the segment: the pointer's second word, else the word.
std::int64_t relocated_word(const Fixup& f) { return static_cast<std::int64_t>(f.at) + (f.location == kLocPointer ? 2 : 0); }

std::int64_t fixup_width(const Fixup& f) { return f.location == kLocPointer ? 4 : 2; }

// A data record's segment index and the offset it starts at (LEDATA and LIDATA both begin so).
struct DataStart {
    std::size_t segment = 0;
    std::uint32_t offset = 0;
    std::size_t data_at = 0;   // where the data begins in the payload
};
DataStart data_start(ByteView r) {
    const fixupp::Index si = fixupp::read_index(r, 0);
    if (si.next + 2 > r.size()) throw TruncatedError(kReader, si.next, 2, r.size() - std::min(r.size(), si.next));
    return {si.value, static_cast<std::uint32_t>(r[si.next] | (r[si.next + 1] << 8)), si.next + 2};
}

// What the module's records say of its layout: the segments in definition order, and each segment's fix-ups placed at
// their offsets in it.
struct Layout {
    std::vector<std::string> segment_names;                       // SEGDEF order: segment index n is [n - 1]
    std::unordered_map<std::size_t, std::vector<Fixup>> fixups;   // by segment index
};

Layout read_layout(const std::vector<omf::Record>& records) {
    Layout out;
    std::vector<std::string> names;
    fixupp::Threads threads;
    std::optional<DataStart> last;
    for (const omf::Record& rec : records) {
        const ByteView r = rec.payload;
        if (rec.type == kLnames) {
            std::size_t q = 0;
            while (q < r.size()) {
                const ByteView name = io::checked_range(r, q + 1, r[q]);
                names.push_back(text::from_latin1(name));
                q += 1 + r[q];
            }
        } else if (rec.type == kSegdef) {
            // attributes (alignment in the top three bits; 0 is an absolute segment, whose frame and offset follow),
            // then the 16-bit length, then the segment name's index
            if (r.empty()) throw TruncatedError(kReader, 0, 1, 0);
            const std::size_t q = (r[0] >> 5) ? 1 : 4;
            const std::size_t name_index = fixupp::read_index(r, q + 2).value;
            if (name_index < 1 || name_index > names.size()) {
                throw FormatError(kReader, q + 2, std::format("a segment named by index {} of {} names", name_index, names.size()));
            }
            out.segment_names.push_back(names[name_index - 1]);
        } else if (rec.type == kLedata || rec.type == kLidata) {
            last = data_start(r);
        } else if (rec.type == kFixupp) {
            for (Fixup f : fixupp::parse(r, threads)) {
                if (!last) throw FormatError(kReader, 0, "a FIXUPP record before any data record");
                f.at = last->offset + f.offset;
                out.fixups[last->segment].push_back(f);
            }
        }
    }
    return out;
}

// What recut works out for a code segment it will write again.
struct Plan {
    Bytes code;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> records;   // [begin, end) of each data record to write
    std::vector<Fixup> fixups;
    std::unordered_map<std::int64_t, std::size_t> rank;             // relocated word -> its place in the original's order
};

// A cut at c splits no fix-up's word.
bool cuts_no_word(const std::vector<std::pair<std::int64_t, std::int64_t>>& spans, std::int64_t c) {
    return std::ranges::none_of(spans, [c](const auto& s) { return s.first < c && c < s.second; });
}

// What to do with one segment: leave it as it is (it is no code of the project's), give the whole module up (the
// program's relocations are not this segment's, or no cut fits them), or write it again by a plan.
struct Verdict {
    enum class Kind { Skip, GiveUp, Planned } kind = Kind::Skip;
    Plan plan;
};

Verdict plan_segment(const omf::Object& object, const std::string& name, std::size_t index, const Layout& layout,
                                 const SymbolTable& symbols, const std::vector<std::uint32_t>& original) {
    const Bytes* bytes = object.segments.find(name);
    if (!bytes || bytes->empty()) return {};
    const std::vector<std::string> code_segments = omf::code_segments(object);
    if (std::ranges::find(code_segments, name) == code_segments.end()) return {};

    // where the segment lies in the program: from its first public the project knows (a function or a place of no
    // said kind)
    std::vector<std::pair<std::int64_t, std::string>> publics;
    for (const omf::Public& p : object.publics) {
        if (!p.segment || *p.segment != name) continue;
        const SymPlace* place = symbols.find(p.name);
        if (place && (place->kind == SymKind::Code || place->kind == SymKind::Unknown)) publics.emplace_back(p.offset, p.name);
    }
    if (publics.empty()) return {};
    std::ranges::sort(publics);
    const std::int64_t base = symbols.at(publics.front().second).address - publics.front().first;
    const std::uint64_t filled = get_or(object.filled, name, static_cast<std::uint64_t>(bytes->size()));
    const std::size_t n = std::min<std::size_t>(filled, bytes->size());

    static const std::vector<Fixup> kNone;
    const auto found = layout.fixups.find(index);
    const std::vector<Fixup>& fl = found == layout.fixups.end() ? kNone : found->second;
    std::map<std::int64_t, const Fixup*> relocated;   // by the word the loader adds the load segment to; sorted for the check below
    for (const Fixup& f : fl) {
        if (is_relocated(f)) relocated[relocated_word(f)] = &f;
    }
    // the program's relocations that lie in this segment, in the program's order
    std::vector<std::int64_t> seq;
    for (const std::uint32_t x : original) {
        const std::int64_t place = static_cast<std::int64_t>(x) - base;
        if (place >= 0 && place < static_cast<std::int64_t>(n) && relocated.contains(place)) seq.push_back(place);
    }
    std::vector<std::int64_t> sorted_seq = seq;
    std::ranges::sort(sorted_seq);
    const bool same_set = sorted_seq.size() == relocated.size() &&
                          std::ranges::equal(sorted_seq, relocated | std::views::keys);
    if (!same_set) return {Verdict::Kind::GiveUp, {}};   // not the program's relocations: the module is left as it is

    // runs of relocations that go upward: each is one data record of the original
    std::vector<std::vector<std::int64_t>> runs;
    for (const std::int64_t x : seq) {
        if (!runs.empty() && x < runs.back().back()) runs.back().push_back(x);
        else runs.push_back({x});
    }
    std::vector<std::pair<std::int64_t, std::int64_t>> spans;
    for (const Fixup& f : fl) spans.emplace_back(f.at, f.at + fixup_width(f));

    // the cut between two runs goes as late as it can before the next one's first word, after the last of the one before
    std::vector<std::int64_t> cuts{0};
    for (std::size_t k = 1; k < runs.size(); ++k) {
        const std::int64_t lo = *std::ranges::max_element(runs[k - 1]) + 2;
        const std::int64_t first = *std::ranges::min_element(runs[k]);
        const std::int64_t hi = first - (relocated.at(first)->location == kLocPointer ? 2 : 0);
        std::optional<std::int64_t> cut;
        for (std::int64_t c = hi; c >= lo; --c) {
            if (cuts_no_word(spans, c)) {
                cut = c;
                break;
            }
        }
        if (!cut) return {Verdict::Kind::GiveUp, {}};
        cuts.push_back(*cut);
    }
    cuts.push_back(static_cast<std::int64_t>(n));

    Plan plan;
    for (std::size_t k = 0; k + 1 < cuts.size(); ++k) {
        std::int64_t a = cuts[k];
        const std::int64_t b = cuts[k + 1];
        while (b - a > static_cast<std::int64_t>(kMaxDataRecord)) {
            std::optional<std::int64_t> c;
            for (std::int64_t v = a + static_cast<std::int64_t>(kMaxDataRecord); v > a; --v) {
                if (cuts_no_word(spans, v)) {
                    c = v;
                    break;
                }
            }
            if (!c) throw FormatError(kReader, static_cast<std::uint64_t>(a), "no place to cut a data record");
            plan.records.emplace_back(static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(*c));
            a = *c;
        }
        plan.records.emplace_back(static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(b));
    }
    for (std::size_t k = 0; k < seq.size(); ++k) plan.rank[seq[k]] = k;
    plan.code.assign(bytes->begin(), bytes->begin() + static_cast<std::ptrdiff_t>(n));
    plan.fixups = fl;
    return {Verdict::Kind::Planned, std::move(plan)};
}

// The records of a planned segment: each data record, then the FIXUPP records of the fix-ups that lie in it,
// the relocated ones first in the original's order.
void write_segment(Bytes& out, std::size_t index, const Plan& plan) {
    for (const auto& [a, b] : plan.records) {
        if (a > 0xFFFF) throw FormatError(kReader, a, "a data record begins past what a 16-bit LEDATA can address");
        Bytes data = fixupp::index_bytes(static_cast<std::uint16_t>(index));
        data.push_back(static_cast<std::uint8_t>(a & 0xFF));
        data.push_back(static_cast<std::uint8_t>(a >> 8));
        append(data, ByteView(plan.code).subspan(a, b - a));
        append(out, omf::record(kLedata, data));

        std::vector<Fixup> relocated, other;
        for (const Fixup& f : plan.fixups) {
            if (f.at < a || f.at >= b) continue;
            (is_relocated(f) ? relocated : other).push_back(f);
        }
        std::ranges::stable_sort(relocated, [&](const Fixup& x, const Fixup& y) {
            return plan.rank.at(relocated_word(x)) < plan.rank.at(relocated_word(y));
        });
        relocated.insert(relocated.end(), other.begin(), other.end());
        for (std::size_t k = 0; k < relocated.size(); k += kMaxFixupsPerRecord) {
            const std::size_t count = std::min(kMaxFixupsPerRecord, relocated.size() - k);
            append(out, fixupp::encode(std::span<const Fixup>(relocated).subspan(k, count), a));
        }
    }
}

}  // namespace

std::vector<std::uint32_t> relocation_order(const Original& original) {
    if (original.packed_relocs) return *original.packed_relocs;
    if (!original.table) return {};
    // each entry is an offset word then a segment word
    const ByteView table = io::checked_range(original.file, original.table->offset, 4ull * original.table->count);
    io::ByteReader in(table, "exe relocation table");
    std::vector<std::uint32_t> out;
    out.reserve(original.table->count);
    while (!in.eof()) {
        const std::uint32_t offset = in.u16();
        const std::uint32_t segment = in.u16();
        out.push_back(segment * 16 + offset);
    }
    return out;
}

Bytes recut(const Original& original, ByteView raw) {
    const omf::Object object = omf::read(raw);
    const std::vector<omf::Record> records = omf::records(raw);
    const Layout layout = read_layout(records);
    const std::vector<std::uint32_t> relocations = relocation_order(original);

    std::map<std::size_t, Plan> plans;   // by segment index; visited in order
    for (std::size_t i = 0; i < layout.segment_names.size(); ++i) {
        Verdict v = plan_segment(object, layout.segment_names[i], i + 1, layout, original.symbols, relocations);
        if (v.kind == Verdict::Kind::GiveUp) return Bytes(raw.begin(), raw.end());
        if (v.kind == Verdict::Kind::Planned) plans.emplace(i + 1, std::move(v.plan));
    }
    if (plans.empty()) return Bytes(raw.begin(), raw.end());

    Bytes out;
    std::set<std::size_t> written;
    std::optional<std::size_t> last;   // the segment the last data record was of
    for (const omf::Record& rec : records) {
        if (rec.type == kLedata || rec.type == kLidata) {
            const std::size_t si = data_start(rec.payload).segment;
            last = si;
            const auto plan = plans.find(si);
            if (plan != plans.end()) {
                if (written.insert(si).second) write_segment(out, si, plan->second);   // all the segment's data, once
                continue;
            }
        }
        if (rec.type == kFixupp && last && plans.contains(*last)) continue;   // the planned segment's fixups were written with it
        append(out, omf::record(rec.type, rec.payload));
    }
    return out;
}

}  // namespace cb::exact::recut
