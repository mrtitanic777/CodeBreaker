#include "cb/exact/habits/newcomp.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <functional>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/habits/boolx.hpp"
#include "cb/exact/habits/symx.hpp"

namespace cb::exact::habits::newcomp {

namespace {

using sv = std::string_view;
using Row = cmpp::Row;
using Renaming = std::map<std::string, std::string>;
using MaybeRenaming = std::map<std::string, std::optional<std::string>>;

constexpr std::uint8_t kDispose[5] = {0x9A, 0x50, 0x02, 0xA2, 0x0C};   // call far 0CA2:0250, the heap's routine that frees a block
constexpr std::uint8_t kTableJump[3] = {0x2E, 0xFF, 0xA7};             // jmp word ptr cs:[bx + table]
constexpr std::size_t kTableJumpLength = 5;                            // ... and its table's address: the instruction is five bytes
constexpr std::uint8_t kCall = 0xE8, kFarCall = 0x9A, kNearJump = 0xE9;
constexpr std::array<std::uint8_t, 8> kEnds = {0xE8, 0x9A, 0xE9, 0xEB, 0xC2, 0xC3, 0xCA, 0xCB};   // call, far call, jumps, returns
constexpr std::size_t kLookahead = 60;   // how far a condition worked out in full is looked for

// The register families: the word register and its low and high byte (SI and DI have no halves).
struct Family {
    const char* word;
    const char* low;
    const char* high;
};
constexpr std::array<Family, 6> kFamilies = {{{"ax", "al", "ah"}, {"cx", "cl", "ch"}, {"dx", "dl", "dh"}, {"bx", "bl", "bh"}, {"si", nullptr, nullptr}, {"di", nullptr, nullptr}}};

const char* nc_part(const Family& f, int k) { return k == 0 ? f.word : k == 1 ? f.low : f.high; }

// The family of a register name and which of its parts it is (0 word, 1 low byte, 2 high byte).
std::optional<std::pair<const Family*, int>> nc_part_of(sv name) {
    for (const Family& f : kFamilies)
        for (int k = 0; k < 3; ++k)
            if (nc_part(f, k) != nullptr && name == nc_part(f, k)) return std::make_pair(&f, k);
    return std::nullopt;
}

const Family* nc_family(sv word) {
    for (const Family& f : kFamilies)
        if (word == f.word) return &f;
    return nullptr;
}

std::int64_t nc_signed8(std::uint8_t b) { return b > 127 ? static_cast<std::int64_t>(b) - 256 : b; }
std::int64_t nc_word_at(ByteView b, std::size_t at) { return static_cast<std::int16_t>(b[at] | (b[at + 1] << 8)); }

bool nc_find_at(ByteView b, std::size_t from, std::size_t& at) {
    for (std::size_t i = from; i + sizeof kTableJump <= b.size(); ++i) {
        if (std::equal(std::begin(kTableJump), std::end(kTableJump), b.begin() + static_cast<std::ptrdiff_t>(i))) {
            at = i;
            return true;
        }
    }
    return false;
}

bool nc_is_ends(std::uint8_t c) { return std::ranges::find(kEnds, c) != kEnds.end(); }
bool nc_is_jcc(std::uint8_t c) { return c >= 0x70 && c <= 0x7F; }

std::string nc_hex4(std::int64_t n) { return std::format("{:04x}", n); }

// The rows of a procedure, the jump tables of its CASE statements kept out of the instructions: a table is a row of its
// own (mnemonic "table"), its length the one the compiled code's linker words give.
std::vector<Row> nc_rows(ByteView b, const std::vector<std::size_t>& table_lengths, const cmpp::Options& options) {
    std::vector<Row> out;
    std::size_t p = 0, t = 0, k = 0;
    const auto walk_from = [&](std::size_t from, std::size_t to) {
        for (Row r : cmpp::walk(b.subspan(std::min(from, b.size()), std::min(to, b.size()) - std::min(from, b.size())), 0, from == 0, options)) {
            r.offset += static_cast<std::uint32_t>(from);
            out.push_back(std::move(r));
        }
    };
    for (;;) {
        if (!nc_find_at(b, p, k) || t >= table_lengths.size()) {
            walk_from(p, b.size());
            return out;
        }
        walk_from(p, k + kTableJumpLength);
        out.push_back(Row{static_cast<std::uint32_t>(k + kTableJumpLength), static_cast<std::uint32_t>(table_lengths[t]),
                          std::format("table of {}", table_lengths[t] / 2), "table"});
        p = k + kTableJumpLength + table_lengths[t];
        ++t;
    }
}

}  // namespace

std::string mask(std::string_view text) {
    static const Regex number(R"re(0x[0-9a-f]+|\b\d+\b)re");
    return text::replace_all(number.replace_all(text, "N"), "- N", "+ N");
}

std::optional<Relative> relative(ByteView b) {
    if (b.size() == 2 && ((b[0] >= 0x70 && b[0] <= 0x7F) || b[0] == 0xEB || b[0] == 0xE2 || b[0] == 0xE3)) return Relative{1, nc_signed8(b[1])};
    if (b.size() == 3 && (b[0] == 0xE8 || b[0] == 0xE9)) return Relative{1, nc_word_at(b, 1)};
    return std::nullopt;
}

std::optional<std::string> rename(std::string_view instruction, const std::map<std::string, std::string>& renaming) {
    static const Regex any_register(R"re(\b(ax|al|ah|cx|cl|ch|dx|dl|dh|bx|bl|bh|si|di)\b)re");
    bool bad = false;
    const std::string out = any_register.replace_all(instruction, std::function<std::string(const RegexMatch&)>([&](const RegexMatch& m) {
        const auto [family, part] = *nc_part_of(m[1]);
        const auto to = renaming.find(family->word);
        const Family* target = to == renaming.end() ? family : nc_family(to->second);
        const char* p = target == nullptr ? nullptr : nc_part(*target, part);
        if (p == nullptr) {   // (SI and DI have no halves)
            bad = true;
            return std::string(m[1]);
        }
        return std::string(p);
    }));
    if (bad) return std::nullopt;
    return out;
}

namespace {

// ---- the comparison of the 1981 compiler's habits' --------------------------------------------------------------

class NewcompComparer {
public:
    NewcompComparer(ByteView want, ByteView got, const std::set<std::uint32_t>& fx, const cmpp::Options& options) : want_(want), got_(got), fx_(fx) {
        // the jump tables of CASE statements are words, not instructions: in the compiled code every one is an address
        // the linker fills in, so their length is known; the original's lie at the same jumps
        std::vector<std::size_t> tables;
        std::size_t k = 0;
        for (std::size_t from = 0; nc_find_at(got, from, k); from = k + kTableJumpLength) {
            std::size_t n = 0;
            while (fx.contains(static_cast<std::uint32_t>(k + 5 + 2 * n)) && fx.contains(static_cast<std::uint32_t>(k + 6 + 2 * n))) ++n;
            tables.push_back(2 * n);
        }
        W_ = nc_rows(want, tables, options);
        G_ = nc_rows(got, tables, options);
    }

    Finding run() {
        while (wi_ < W_.size() && gi_ < G_.size()) {
            if (const auto stop = step()) return failure(*stop);
        }
        if (count_) return failure(std::format("a shift count the original never loads, from +{}", nc_hex4(count_->second)));
        if (wi_ != W_.size() || gi_ != G_.size())
            return failure(std::format("the two do not end together: {} of {}, {} of {} instructions", wi_, W_.size(), gi_, G_.size()));
        set_renaming(Renaming{}, want_.size());
        std::map<std::int64_t, std::int64_t> at;
        for (const auto& [a, b] : amap_) at[a] = b;
        at[static_cast<std::int64_t>(want_.size())] = static_cast<std::int64_t>(got_.size());
        for (const std::int64_t t : fors_)
            if (!at.contains(t)) return failure(std::format("the FOR entry test's jump to +{} goes into an instruction", nc_hex4(t)));
        for (const auto& [tw, tg] : jumps_) {
            if (0 <= tw && tw <= static_cast<std::int64_t>(want_.size())) {
                const auto f = at.find(tw);
                if (f == at.end() || f->second != tg)
                    return failure(std::format("a jump to +{} goes to +{} in the compiled code, not +{}", nc_hex4(tw), nc_hex4(tg), f == at.end() ? std::string("?") : nc_hex4(f->second)));
            }
        }
        if (!temps_.empty()) {
            std::vector<std::string> parts;
            for (const auto& [a, b] : temps_) parts.push_back(std::format("-{:X} for -{:X}", a, b));
            notes_.push_back("temporaries elsewhere in the frame (" + text::join(parts, ", ") + ")");
        }
        Finding out;
        out.same = !notes_.empty();   // (nothing to explain: the two differ only where a jump's displacement does, which no habit is)
        out.notes = out.same ? notes_ : std::vector<std::string>{"differs only in jump displacements?"};
        std::set<std::pair<std::int64_t, std::int64_t>> unique(amap_.begin(), amap_.end());
        for (const auto& [a, b] : unique) out.alignment.emplace_back(static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(b));
        for (const Row& g : G_) out.compiled_starts.push_back(g.offset);
        return out;
    }

private:
    static Finding failure(std::string why) {
        Finding f;
        f.notes.push_back(std::move(why));
        return f;
    }

    bool fixed(std::size_t og, std::size_t sg) const {
        for (std::size_t k = 0; k < sg; ++k)
            if (fx_.contains(static_cast<std::uint32_t>(og + k))) return true;
        return false;
    }

    // The values the compiled code has in registers other than the original's: `new_map` filtered of what holds nothing
    // or holds its own (a note says when a renaming ends).
    void set_renaming(const MaybeRenaming& new_map, std::size_t original_offset) {
        Renaming next;
        for (const auto& [k, v] : new_map)
            if (v && k != *v) next[k] = *v;
        if (renaming_.empty() && !next.empty()) since_ = original_offset;
        if (!renaming_.empty() && next.empty()) {
            std::vector<std::string> parts;
            for (const auto& [k, v] : renaming_) parts.push_back(text::upper(k) + " for " + text::upper(v));
            notes_.push_back("registers used otherwise (" + text::join(parts, ", ") + ") from +" + nc_hex4(since_));
        }
        renaming_ = std::move(next);
    }
    void set_renaming(const Renaming& new_map, std::size_t original_offset) {
        MaybeRenaming m;
        for (const auto& [k, v] : new_map) m[k] = v;
        set_renaming(m, original_offset);
    }

    // What the compiled code has for the original's register x; none: nothing.
    std::optional<std::string> held(const std::string& x) const {
        if (const auto f = renaming_.find(x); f != renaming_.end()) return f->second;
        for (const auto& kv : renaming_)
            if (kv.second == x) return std::nullopt;
        return x;
    }

    bool same_text(const std::optional<std::string>& a, std::string_view b, bool fixed_here) const { return a && (*a == b || (fixed_here && mask(*a) == mask(b))); }

    // ---- the last resort: from the last call or jump to the next, do the two compute the same (symx)? ----------------
    bool stretch() {
        if (!sync_ || count_) return false;
        const auto end = [&](const std::vector<Row>& L, ByteView b, std::size_t i) -> std::optional<std::size_t> {
            static const Regex zeroed(R"re(mov \w\w, 0)re");
            while (i < L.size()) {
                const std::uint8_t c = b[L[i].offset];
                if (L[i].mnemonic == "table") return i;
                if (nc_is_jcc(c) && L[i].offset + 1 < b.size() && b[L[i].offset + 1] == 1 && i > 0 && zeroed.full_match(L[i - 1].text)) {
                    ++i;
                    continue;
                }
                if (nc_is_ends(c) || nc_is_jcc(c)) return i;
                ++i;
            }
            return std::nullopt;
        };
        const auto ew = end(W_, want_, sync_->first), eg = end(G_, got_, sync_->second);
        if (!ew || !eg || *ew < wi_ || *eg < gi_ || want_[W_[*ew].offset] != got_[G_[*eg].offset] || W_[*ew].mnemonic == "table") return false;
        const std::uint8_t c = want_[W_[*ew].offset];
        std::vector<symx::Instruction> iw, ig;
        for (std::size_t i = sync_->first; i < *ew; ++i) iw.push_back({W_[i].text, Bytes(want_.begin() + W_[i].offset, want_.begin() + W_[i].offset + W_[i].size)});
        for (std::size_t i = sync_->second; i < *eg; ++i) ig.push_back({G_[i].text, Bytes(got_.begin() + G_[i].offset, got_.begin() + G_[i].offset + G_[i].size)});
        symx::LinkerWords words;
        for (std::size_t k = 0; k < ig.size(); ++k) {
            const Row& x = G_[sync_->second + k];
            if (!fixed(x.offset, x.size)) continue;
            std::set<std::int64_t>& set = words[k];
            for (std::size_t j = 0; j + 1 < x.size; ++j)
                if (fx_.contains(x.offset + static_cast<std::uint32_t>(j)) && fx_.contains(x.offset + static_cast<std::uint32_t>(j) + 1))
                    set.insert(got_[x.offset + j] | (got_[x.offset + j + 1] << 8));
        }
        symx::Options options;
        options.registers = c != kCall && c != kFarCall;
        options.flags = nc_is_jcc(c);
        if (!symx::same(iw, ig, words, options).same) return false;
        const std::uint32_t keep = W_[sync_->first].offset;
        std::erase_if(amap_, [&](const auto& x) { return x.first > keep; });
        notes_.push_back(std::format("the same values, by other registers and temporaries, from +{} to +{}", nc_hex4(W_[sync_->first].offset), nc_hex4(W_[*ew].offset)));
        wi_ = *ew;
        gi_ = *eg;
        renaming_.clear();
        return true;
    }

    // ---- a condition the compiled code works out in full and the original leaves early (boolx) ---------------------
    bool condition() { return condition_in(W_, G_, want_, got_, wi_, gi_, amap_, jumps_, notes_); }

    static bool condition_in(const std::vector<Row>& W, const std::vector<Row>& G, ByteView want, ByteView got, std::size_t& wi, std::size_t& gi,
                             std::vector<std::pair<std::int64_t, std::int64_t>>& amap, std::vector<std::pair<std::int64_t, std::int64_t>>& jumps,
                             std::vector<std::string>& notes) {
        static const Regex compare(R"re((cmp|test) )re");
        static const Regex shift(R"re(shr (ax|cx|dx), 1)re");
        std::size_t ws = wi, gs = gi;
        while (ws > 0 && gs > 0 && W[ws - 1].text == G[gs - 1].text && compare.match(G[gs - 1].text)) {
            --ws;
            --gs;
        }
        std::optional<std::size_t> eg;
        for (std::size_t j = gi; j < std::min(G.size() - 1, gi + kLookahead); ++j) {
            const std::string& m = G[j].mnemonic;
            if (m == "call" || m == "lcall" || m == "ret" || m == "retf" || m == "table") return false;
            if (shift.full_match(G[j].text) && (G[j + 1].mnemonic == "jae" || G[j + 1].mnemonic == "jb")) {
                eg = j + 2;
                break;
            }
        }
        if (!eg || *eg + 1 >= G.size()) return false;
        const std::string next0 = mask(G[*eg].text), next1 = mask(G[*eg + 1].text);
        std::optional<std::size_t> ew;
        for (std::size_t j = wi + 1; j < std::min(W.size() - 1, wi + kLookahead); ++j)
            if (mask(W[j].text) == next0 && mask(W[j + 1].text) == next1) {
                ew = j;
                break;
            }
        if (!ew) return false;
        static const Regex address(R"re(\[(0x[0-9a-f]+|\d+)\])re");
        const auto lines = [&](const std::vector<Row>& L, ByteView b, std::size_t i0, std::size_t i1) {
            std::vector<boolx::Instruction> out;
            std::map<std::string, std::size_t> glob;   // a variable's address is the linker's: named by the order they come in
            for (std::size_t i = i0; i < i1; ++i) {
                const Row& r = L[i];
                const auto rel = relative(b.subspan(r.offset, r.size));
                std::string x = r.text;
                if (rel && b[r.offset] != kCall) {
                    x = std::format("{} 0x{:x}", r.mnemonic, static_cast<std::int64_t>(r.offset) + r.size + rel->displacement);
                } else {
                    x = address.replace_all(x, std::function<std::string(const RegexMatch&)>([&](const RegexMatch& m) {
                        return "[g" + std::to_string(glob.try_emplace(std::string(m[1]), glob.size()).first->second) + "]";
                    }));
                }
                out.push_back({r.offset, x});
            }
            return out;
        };
        static const Regex si_loaded(R"re(mov si, word ptr \[([^\]]+)\])re"), di_loaded(R"re(mov di, word ptr \[([^\]]+)\])re");
        static const Regex si_changed(R"re((mov|xchg) si)re"), di_changed(R"re((mov|xchg) di)re");
        const auto bases = [&](const std::vector<Row>& L, std::size_t i) {
            boolx::Bases d;
            const std::array<std::tuple<const Regex*, const Regex*, std::string*>, 2> registers = {{{&si_loaded, &si_changed, &d.si}, {&di_loaded, &di_changed, &d.di}}};
            for (const auto& [loaded, changed, base] : registers) {
                for (std::size_t j = i; j-- > 0 && j + 40 > i;) {
                    if (const auto m = loaded->full_match(L[j].text)) {
                        *base = std::string((*m)[1]);
                        break;
                    }
                    if (changed->match(L[j].text) || L[j].mnemonic == "call" || L[j].mnemonic == "lcall") break;
                }
            }
            return d;
        };
        const std::vector<boolx::Instruction> iw = lines(W, want, ws, *ew), ig = lines(G, got, gs, *eg);
        const boolx::Verdict v = boolx::same(boolx::Stretch{iw, W[ws].offset, W[*ew].offset, bases(W, ws)}, boolx::Stretch{ig, G[gs].offset, G[*eg].offset, bases(G, gs)});
        if (!v.same) return false;
        if (v.else_targets) jumps.emplace_back(v.else_targets->original, v.else_targets->compiled);
        notes.push_back(std::format("a condition worked out in full where the original jumps, from +{} to +{}", nc_hex4(W[ws].offset), nc_hex4(W[*ew].offset)));
        const std::uint32_t keep = W[ws].offset;
        std::erase_if(amap, [&](const auto& x) { return x.first > keep; });
        wi = *ew;
        gi = *eg;
        return true;
    }

private:
    // A temporary of the original is the same one of the compiled code's: [bp - a] against [bp - b], in the same order.
    bool temp_equal(const std::optional<std::string>& x, std::string_view y) {
        static const Regex frame(R"re(\[bp - (0x[0-9a-f]+|\d+)\])re");
        if (!x) return false;
        const auto mw = frame.search(*x);
        const auto mg = frame.search(y);
        if (!mw || !mg) return false;
        if (x->substr(0, mw->start()) + x->substr(mw->end()) != std::string(y.substr(0, mg->start())) + std::string(y.substr(mg->end()))) return false;
        const std::int64_t a = *number(mw->group(1)), b = *number(mg->group(1));
        const auto ta = temps_.find(a);
        const auto rb = reverse_temps_.find(b);
        if (a == b || (ta != temps_.end() && ta->second != b) || (rb != reverse_temps_.end() && rb->second != a)) return false;
        for (const auto& [ox, oy] : temps_)
            if (ox != a && (ox < a) != (oy < b)) return false;   // (in the same order: a parameter is not taken for a local)
        temps_[a] = b;
        reverse_temps_[b] = a;
        return true;
    }

    static std::optional<std::int64_t> number(sv s) {
        if (s.starts_with("0x")) return text::parse_int<std::int64_t>(s.substr(2), 16);
        return text::parse_int<std::int64_t>(s, 10);
    }

    // One instruction pair. Returns why the procedure fails when it does; nullopt to go on.
    std::optional<std::string> step() {
        const Row& rw_row = W_[wi_];
        const Row& rg_row = G_[gi_];
        const std::size_t ow = rw_row.offset, sw = rw_row.size, og = rg_row.offset, sg = rg_row.size;
        const std::string &tw = rw_row.text, &tg = rg_row.text;
        const ByteView bw = want_.subspan(ow, sw), bg = got_.subspan(og, sg);
        amap_.emplace_back(ow, og);
        if (wi_ > 0 && gi_ > 0) {   // behind a call the registers hold nothing; behind a jump the same, if they did before it
            const std::uint8_t pw = want_[W_[wi_ - 1].offset], pg = got_[G_[gi_ - 1].offset];
            if (pw == pg && (pw == kCall || pw == kFarCall || (renaming_.empty() && (nc_is_ends(pw) || nc_is_jcc(pw)))) && W_[wi_ - 1].mnemonic != "table") sync_ = std::make_pair(wi_, gi_);
        }
        const bool fxg = fixed(og, sg);
        bool same = sw == sg;
        for (std::size_t k = 0; same && k < sw; ++k) same = bw[k] == bg[k] || fx_.contains(static_cast<std::uint32_t>(og + k));
        const auto eq = [&](const std::optional<std::string>& a, std::string_view b) { return same_text(a, b, fxg); };
        const auto rw = relative(bw), rg = relative(bg);
        const auto advance = [&](std::size_t w, std::size_t g) {
            wi_ += w;
            gi_ += g;
        };
        if (rw && rg && bw[0] == bg[0] && !fxg) {   // a jump or a call: where it goes is checked at the end (a call's, by build.py)
            if (bw[0] != kCall) jumps_.emplace_back(static_cast<std::int64_t>(ow + sw) + rw->displacement, static_cast<std::int64_t>(og + sg) + rg->displacement);
            else set_renaming(Renaming{}, ow);
            advance(1, 1);
            return std::nullopt;
        }
        if (!same && tw != tg && renaming_.empty() && condition()) return std::nullopt;
        // FOR with constant bounds: the later compiler still tests them before the loop (xor dx,dx / shr dx,1 / jb past the loop)
        if (!same && tw == "xor dx, dx" && wi_ + 2 < W_.size() && W_[wi_ + 1].text == "shr dx, 1" && want_[W_[wi_ + 2].offset] == 0x72) {
            const Row& j3 = W_[wi_ + 2];
            const std::int64_t target = static_cast<std::int64_t>(j3.offset) + 2 + nc_signed8(want_[j3.offset + 1]);
            if (target > static_cast<std::int64_t>(j3.offset)) {
                fors_.push_back(target);
                Renaming kept;
                for (const auto& [k, v] : renaming_)
                    if (k != "dx" && v != "dx") kept[k] = v;
                set_renaming(kept, ow);
                notes_.push_back(std::format("FOR entry test on constants at +{}", nc_hex4(ow)));
                advance(3, 0);
                return std::nullopt;
            }
        }
        if (tw == tg && tw.starts_with("table of")) {   // (build.py compares where the entries point)
            advance(1, 1);
            return std::nullopt;
        }
        // CASE: the later compiler takes the least value by itself (sub bx,lo / ja / jmp to its case) before the jump through the table
        if (bw.size() == 2 && bw[0] == 0x77 && bw[1] == 0x03 && tg == "shl bx, 1" && wi_ > 0 && W_[wi_ - 1].text.starts_with("sub bx, ") && wi_ + 2 < W_.size() &&
            want_[W_[wi_ + 1].offset] == kNearJump && W_[wi_ + 2].text == "shl bx, 1") {
            const Row& j3 = W_[wi_ + 1];
            fors_.push_back(static_cast<std::int64_t>(j3.offset) + 3 + nc_word_at(want_, j3.offset + 1));
            notes_.push_back(std::format("CASE: least value taken first at +{}", nc_hex4(ow)));
            advance(2, 0);
            return std::nullopt;
        }
        if (bw.size() == 2 && bw[0] == 0x76 && tg == "shl bx, 1" && wi_ > 0 && W_[wi_ - 1].text.starts_with("sub bx, ") && wi_ + 1 < W_.size() && W_[wi_ + 1].text == "shl bx, 1") {   // ... or jbe case-lo, when it is near
            fors_.push_back(static_cast<std::int64_t>(ow) + 2 + rw->displacement);
            notes_.push_back(std::format("CASE: least value taken first at +{}", nc_hex4(ow)));
            advance(1, 0);
            return std::nullopt;
        }
        // after a DISPOSE through the library DI is lost: the original loads the WITH pointer again, the in-line code still has it
        if (reload_ && *reload_ == wi_ && !same && tw.starts_with("mov di, word ptr [bp") && wi_ + 1 < W_.size() && W_[wi_ + 1].text == tg) {
            notes_.push_back(std::format("WITH pointer loaded again after DISPOSE at +{}", nc_hex4(ow)));
            advance(1, 0);
            return std::nullopt;
        }
        // the frame: the two compilers do not need the same number of temporaries, so the frame's size may differ, and a
        // temporary may lie elsewhere in it - but one of the original's is always the same one of the compiled
        if (rw_row.mnemonic == "hdr" && rg_row.mnemonic == "hdr" && bw.size() == 2 && bg.size() >= 2 && !std::ranges::equal(bw, bg)) {   // (MS-Pascal 3.x: the size alone)
            notes_.push_back(std::format("frame of {} bytes, {} as compiled here", bw[0] | bw[1] << 8, bg[0] | bg[1] << 8));
            advance(1, 1);
            return std::nullopt;
        }
        if (rw_row.mnemonic == "hdr" && rg_row.mnemonic == "hdr" && bw.size() >= 3 && bg.size() >= 3 && bw[0] == bg[0] && !std::ranges::equal(bw, bg)) {
            notes_.push_back(std::format("frame of {} bytes, {} in the 1981 code", bw[1] | bw[2] << 8, bg[1] | bg[2] << 8));
            advance(1, 1);
            return std::nullopt;
        }
        if (!same && temp_equal(renaming_.empty() ? std::optional<std::string>(tw) : rename(tw, renaming_), tg)) {
            advance(1, 1);
            return std::nullopt;
        }
        // a shift count: the compiler of 1981 loads it into CX before the value to be shifted, the later one after it (the
        // value is in AX by then). Between the two places neither may touch CX.
        static const Regex count_load(R"re(mov cx, (0x[0-9a-f]+|\d+))re");
        static const Regex shift_by_cl(R"re(.*\b(shl|shr|sal|sar) \w+, cl.*)re");
        const auto mc = count_load.full_match(tg);
        if (!count_ && mc && tw != tg && !same && !tw.ends_with(", " + std::string((*mc)[1]))) {
            bool shifts = false;
            for (std::size_t j = gi_ + 1; j < std::min(G_.size(), gi_ + 8); ++j) shifts = shifts || shift_by_cl.full_match(G_[j].text);
            if (shifts) {
                count_ = std::make_pair(std::string((*mc)[1]), ow);
                advance(0, 1);
                return std::nullopt;
            }
        }
        if (count_) {
            if (tw == "mov cx, " + count_->first && tg != tw) {
                notes_.push_back(std::format("shift count loaded after the value at +{}", nc_hex4(count_->second)));
                count_.reset();
                advance(1, 0);
                return std::nullopt;
            }
            static const Regex uses_cx(R"re(.*\b(cx|cl|ch)\b.*)re");
            if (uses_cx.full_match(tw)) return std::format("CX used before the shift count is loaded, at +{}: original {}, compiled {}", nc_hex4(ow), tw, tg);
        }
        // before MUL the compiler of 1981 saves DX in a temporary it never reads again; the later one does not
        static const Regex save_dx(R"re(mov word ptr (\[bp - \w+\]), dx)re");
        if (const auto m = save_dx.full_match(tg); m && tw != tg && gi_ + 1 < G_.size() && G_[gi_ + 1].text.starts_with("mul ")) {
            std::size_t mentions_n = 0;
            for (const Row& g : G_) mentions_n += g.text.find(m->group(1)) != std::string::npos;
            if (mentions_n == 1) {
                notes_.push_back(std::format("DX not saved before MUL at +{}", nc_hex4(ow)));
                advance(0, 1);
                return std::nullopt;
            }
        }
        if (renaming_.empty() && same) {
            advance(1, 1);
            return std::nullopt;
        }
        static const Regex exchange(R"re(xchg (ax|cx|dx|bx|si|di), (ax|cx|dx|bx|si|di))re");
        static const Regex load(R"re(mov (ax|cx|dx|bx|si|di), (.+))re");
        const auto xw = exchange.full_match(tw), xg = exchange.full_match(tg);
        const auto lw = load.full_match(tw), lg = load.full_match(tg);
        if (!renaming_.empty()) return step_renamed(tw, tg, ow, same, fxg, xw, xg, lw, lg);
        // a value loaded into AX where the compiled code loads it into another register
        if (lw && lg && (*lw)[1] != (*lg)[1] && tw.find("ptr") != std::string::npos && eq(std::string((*lw)[2]), (*lg)[2])) {
            const std::string a(((*lw)[1])), b((*lg)[1]);
            const auto in_cx_dx_bx = [](const std::string& r) { return r == "cx" || r == "dx" || r == "bx"; };
            if (a == "ax" || b == "ax" || (in_cx_dx_bx(a) && in_cx_dx_bx(b))) {
                set_renaming(Renaming{{a, b}}, ow);
                advance(1, 1);
                return std::nullopt;
            }
        }
        // a value moved out of AX, which the next computation wants (the compiled code leaves it there and computes elsewhere)
        static const Regex out_of_ax(R"re(mov (cx|dx|bx|si|di), ax)re");
        if (const auto m = out_of_ax.full_match(tw); m && tw != tg && wi_ + 1 < W_.size() && W_[wi_ + 1].text.starts_with("mov ax, ")) {
            set_renaming(Renaming{{std::string((*m)[1]), "ax"}}, ow);
            advance(1, 0);
            return std::nullopt;
        }
        // ... or the compiled code moves the value out of AX to work on it (an index), which the original does in AX
        if (xg && ((*xg)[1] == "ax" || (*xg)[2] == "ax") && tw != tg && !same) {
            const std::string r = (*xg)[1] == "ax" ? std::string((*xg)[2]) : std::string((*xg)[1]);
            set_renaming(Renaming{{"ax", r}, {r, "ax"}}, ow);
            advance(0, 1);
            return std::nullopt;
        }
        // a register's value moved into AX to be worked on there
        if (xw && ((*xw)[1] == "ax" || (*xw)[2] == "ax")) {
            const std::string r = (*xw)[1] == "ax" ? std::string((*xw)[2]) : std::string((*xw)[1]);
            set_renaming(Renaming{{"ax", r}, {r, "ax"}}, ow);
            advance(1, 0);
            return std::nullopt;
        }
        // a pointer still in DX: the later compiler takes it from there (mov di,dx), the one of 1981 loads the variable again -
        // with the very instructions that loaded DX a moment ago, and no call between
        if (tw == "mov di, dx" && renaming_.empty()) {
            for (std::size_t k = 1; k <= 3; ++k) {
                if (gi_ + k > G_.size() || !G_[gi_ + k - 1].text.starts_with("mov di, ")) continue;
                const std::string x = G_[gi_ + k - 1].text.substr(8);
                const std::size_t floor = std::max<std::ptrdiff_t>(static_cast<std::ptrdiff_t>(gi_) - 10, static_cast<std::ptrdiff_t>(k) - 2) + 1;
                for (std::size_t j = gi_; j-- > 0 && j >= floor;) {
                    if (G_[j].text != "mov dx, " + x) continue;
                    bool same_loads = true;
                    for (std::size_t q = 0; q + 1 < k; ++q) same_loads = same_loads && G_[gi_ + q].text == G_[j - (k - 1) + q].text;
                    bool called = false;
                    for (std::size_t q = j; q < gi_; ++q) called = called || G_[q].text.find("call") != std::string::npos;
                    if (same_loads && !called) {
                        notes_.push_back(std::format("pointer taken from DX at +{}", nc_hex4(ow)));
                        advance(1, k);
                        return std::nullopt;
                    }
                }
            }
        }
        // ... and the other way: the compiler of 1981 still has an outer procedure's frame in SI and uses it; the later one
        // loads it again, into DI, with the instructions that loaded SI
        static const Regex si_word(R"re(.*\bsi\b.*)re");
        static const Regex di_word(R"re(\bdi\b)re");
        if (tw.starts_with("mov di, word ptr [bp") && renaming_.empty() && si_word.full_match(tg)) {
            for (std::size_t k = 1; k <= 2; ++k) {
                if (wi_ + k >= W_.size()) break;
                std::vector<std::string> loads;
                for (std::size_t q = 0; q < k; ++q) loads.push_back(di_word.replace_all(W_[wi_ + q].text, "si"));
                if (di_word.replace_all(W_[wi_ + k].text, "si") != tg || !std::ranges::all_of(loads, [](const std::string& l) { return l.starts_with("mov si, "); })) continue;
                for (std::ptrdiff_t j = static_cast<std::ptrdiff_t>(gi_) - static_cast<std::ptrdiff_t>(k); j > std::max<std::ptrdiff_t>(static_cast<std::ptrdiff_t>(gi_) - 14, -1); --j) {
                    bool equal = true;
                    for (std::size_t q = 0; q < k; ++q) equal = equal && G_[static_cast<std::size_t>(j) + q].text == loads[q];
                    if (!equal) continue;
                    bool clobbered = false;
                    static const Regex sets_si(R"re((mov|lea|pop|xchg) si,.*)re");
                    for (std::size_t q = static_cast<std::size_t>(j) + k; q < gi_; ++q) clobbered = clobbered || G_[q].text.find("call") != std::string::npos || sets_si.match(G_[q].text);
                    if (!clobbered) {
                        notes_.push_back(std::format("outer frame loaded again at +{}", nc_hex4(ow)));
                        advance(k + 1, 1);
                        return std::nullopt;
                    }
                }
            }
        }
        // an outer procedure's variable passed by reference right after it was assigned: the compiler of 1981 pushes the
        // variable itself there (push [di-n], its error: seen with a call put between, which makes it right); the later one
        // loads the frame again and pushes the address
        static const Regex push_var(R"re(push word ptr \[di ([-+] \w+)\])re");
        if (const auto m = push_var.full_match(tg); m && tw == "mov di, word ptr [bp]" && wi_ + 2 < W_.size() && W_[wi_ + 1].text == "lea dx, [di " + std::string((*m)[1]) + "]" &&
            W_[wi_ + 2].text == "push dx" && gi_ > 0) {
            const std::string& prev = G_[gi_ - 1].text;
            static const Regex rest(R"re(\w+)re");
            const std::string stored = "[di " + std::string((*m)[1]) + "], ";
            for (const char* width : {"byte", "word"}) {
                const std::string head = std::string("mov ") + width + " ptr " + stored;
                if (prev.starts_with(head) && rest.full_match(sv(prev).substr(head.size()))) {
                    notes_.push_back(std::format("address of the variable pushed, where the 1981 compiler pushes its value (an error of that compiler), at +{}", nc_hex4(ow)));
                    advance(3, 1);
                    return std::nullopt;
                }
            }
        }
        // DISPOSE
        if (gi_ + 2 < G_.size() && G_[gi_ + 1].text == "mov si, word ptr [si]" && G_[gi_ + 2].text == "or byte ptr [si - 2], 1") {
            std::optional<std::size_t> k;
            if (tg.starts_with("lea si, ") && eq(tw, "lea dx, " + tg.substr(8)) && wi_ + 2 < W_.size() && W_[wi_ + 1].text == "push dx") k = 2;
            else if (tg.starts_with("mov si, word ptr ") && eq(tw, "push word ptr " + tg.substr(17))) k = 1;
            else if (tg == "mov si, di" && tw == "mov dx, di" && wi_ + 2 < W_.size() && W_[wi_ + 1].text == "push di") k = 2;
            else if (tg.find("ptr") == std::string::npos && tg.starts_with("mov si, ") && tg.size() > 8 && tg[8] >= '0' && tg[8] <= '9' && eq(tw, "mov dx, " + tg.substr(8)) && wi_ + 2 < W_.size() &&
                     W_[wi_ + 1].text == "push dx")
                k = 2;   // a variable by its address
            if (k && wi_ + *k < W_.size() && is_dispose(W_[wi_ + *k].offset)) {
                notes_.push_back(std::format("DISPOSE through the library at +{}", nc_hex4(ow)));
                advance(*k + 1, 3);
                reload_ = wi_;
                return std::nullopt;
            }
        }
        // DISPOSE of a variable of an outer procedure: mov di,[bp] / lea dx,[di-n] / push dx against mov si,[bp] / lea si,[si-n]
        if (gi_ + 3 < G_.size() && wi_ + 3 < W_.size() && G_[gi_ + 2].text == "mov si, word ptr [si]" && G_[gi_ + 3].text == "or byte ptr [si - 2], 1") {
            static const Regex frame_load(R"re(mov si, (word ptr \[.+\]))re");
            static const Regex address_of(R"re(lea si, \[si ([-+] \w+)\])re");
            const auto m1 = frame_load.full_match(tg), m2 = address_of.full_match(G_[gi_ + 1].text);
            if (m1 && m2 && tw == "mov di, " + std::string((*m1)[1]) && W_[wi_ + 1].text == "lea dx, [di " + std::string((*m2)[1]) + "]" && W_[wi_ + 2].text == "push dx" &&
                is_dispose(W_[wi_ + 3].offset)) {
                notes_.push_back(std::format("DISPOSE through the library at +{}", nc_hex4(ow)));
                advance(4, 4);
                reload_ = wi_;
                return std::nullopt;
            }
        }
        // a conditional jump one of the two had to make long
        if (rw && nc_is_jcc(bw[0]) && bg[0] == (bw[0] ^ 1) && bg.size() == 2 && bg[1] == 3 && gi_ + 1 < G_.size() && got_[G_[gi_ + 1].offset] == kNearJump) {
            const Row& g2 = G_[gi_ + 1];
            jumps_.emplace_back(static_cast<std::int64_t>(ow + sw) + rw->displacement, static_cast<std::int64_t>(g2.offset) + 3 + nc_word_at(got_, g2.offset + 1));
            notes_.push_back(std::format("long jump at +{}", nc_hex4(ow)));
            advance(1, 2);
            return std::nullopt;
        }
        if (rg && nc_is_jcc(bg[0]) && bw[0] == (bg[0] ^ 1) && bw.size() == 2 && bw[1] == 3 && wi_ + 1 < W_.size() && want_[W_[wi_ + 1].offset] == kNearJump) {
            const Row& w2 = W_[wi_ + 1];
            jumps_.emplace_back(static_cast<std::int64_t>(w2.offset) + 3 + nc_word_at(want_, w2.offset + 1), static_cast<std::int64_t>(og + sg) + rg->displacement);
            notes_.push_back(std::format("long jump at +{}", nc_hex4(ow)));
            advance(2, 1);
            return std::nullopt;
        }
        if (stretch()) return std::nullopt;
        return std::format("no pattern at +{}: original {}, compiled {}", nc_hex4(ow), tw, tg);
    }

    bool is_dispose(std::size_t at) const { return at + sizeof kDispose <= want_.size() && std::equal(std::begin(kDispose), std::end(kDispose), want_.begin() + static_cast<std::ptrdiff_t>(at)); }

    // The step while a renaming is in force.
    std::optional<std::string> step_renamed(const std::string& tw, const std::string& tg, std::size_t ow, bool same, bool fxg, const std::optional<RegexMatch>& xw,
                                            const std::optional<RegexMatch>& xg, const std::optional<RegexMatch>& lw, const std::optional<RegexMatch>& lg) {
        const auto eq = [&](const std::optional<std::string>& a, std::string_view b) { return same_text(a, b, fxg); };
        const auto advance = [&](std::size_t w, std::size_t g) {
            wi_ += w;
            gi_ += g;
            return std::optional<std::string>();
        };
        if (same && tw.starts_with("lcall")) {
            set_renaming(Renaming{}, ow);
            return advance(1, 1);
        }
        if (eq(rename(tw, renaming_), tg)) return advance(1, 1);   // the same instruction, the registers renamed
        static const Regex clear_high(R"re(xor (ah|ch|dh|bh), \1)re");
        if (const auto m = clear_high.full_match(tw)) {   // the high byte cleared: SI and DI have none
            const auto f = renaming_.find(std::string(1, (*m)[1][0]) + "x");
            if (tg == "and " + (f == renaming_.end() ? std::string("?") : f->second) + ", 0xff") return advance(1, 1);
        }
        static const Regex put_aside(R"re(mov (ax|cx|dx|bx|si|di), ax)re");
        if (const auto m = put_aside.full_match(tw); m && tw != tg && wi_ + 1 < W_.size() && W_[wi_ + 1].text.starts_with("mov ax, ") && held("ax")) {   // the value put aside, AX wanted for the next term
            MaybeRenaming next(renaming_.begin(), renaming_.end());
            next[std::string((*m)[1])] = held("ax");
            set_renaming(next, ow);
            return advance(1, 0);
        }
        static const Regex addition(R"re(add (ax|cx|dx|bx|si|di), (ax|cx|dx|bx|si|di))re");
        const auto ma = addition.full_match(tw), mb = addition.full_match(tg);
        if (ma && mb) {   // a sum, the terms the other way round
            const auto a1 = held(std::string((*ma)[1])), a2 = held(std::string((*ma)[2]));
            if (a1 && *a1 == (*mb)[2] && a2 && *a2 == (*mb)[1]) {
                Renaming next;
                for (const auto& [k, v] : renaming_) {
                    const bool k_in_ma = k == (*ma)[1] || k == (*ma)[2];
                    const bool v_in_mb = v == (*mb)[1] || v == (*mb)[2];
                    if (!k_in_ma && !v_in_mb) next[k] = v;
                }
                next[std::string((*ma)[1])] = std::string((*mb)[1]);
                set_renaming(next, ow);
                return advance(1, 1);
            }
        }
        static const Regex constant_load(R"re(mov (ax|cx|dx|bx|si|di), (0x[0-9a-f]+|\d+))re");
        static const Regex frame_store(R"re(mov (byte|word) ptr \[bp.*)re");
        if (const auto m = constant_load.full_match(tw); m && wi_ + 1 < W_.size() && gi_ + 1 < G_.size() && G_[gi_ + 1].text == tw &&
                                                         (eq(rename(W_[wi_ + 1].text, renaming_), tg) || temp_equal(rename(W_[wi_ + 1].text, renaming_), tg)) && frame_store.match(tg)) {
            // a constant loaded before a store the compiled code must do first, because it has the value in the register the constant goes to
            Renaming next;
            for (const auto& [k, v] : renaming_)
                if (v != (*m)[1] && k != (*m)[1]) next[k] = v;
            set_renaming(next, ow);
            return advance(2, 2);
        }
        if (xw) {   // an exchange the compiled code has not: the values change places in the original only
            const std::string a((*xw)[1]), b((*xw)[2]);
            MaybeRenaming next(renaming_.begin(), renaming_.end());
            next[a] = held(b);
            next[b] = held(a);
            set_renaming(next, ow);
            return advance(1, 0);
        }
        if (xg) {   // ... or one only the compiled code has
            const std::string a((*xg)[1]), b((*xg)[2]);
            MaybeRenaming next;
            for (const Family& f : kFamilies) {
                const auto h = held(f.word);
                if (!h) next[f.word] = std::nullopt;
                else next[f.word] = *h == a ? b : *h == b ? a : *h;
            }
            set_renaming(next, ow);
            return advance(0, 1);
        }
        if (lw && lg && eq(rename(std::string((*lw)[2]), renaming_), (*lg)[2])) {   // the same value loaded, into different registers
            Renaming next;
            for (const auto& [k, v] : renaming_)
                if (v != (*lg)[1]) next[k] = v;
            next[std::string((*lw)[1])] = std::string((*lg)[1]);
            set_renaming(next, ow);
            return advance(1, 1);
        }
        static const Regex sets_register(R"re((?:pop|(?:xor|sub) (ax|cx|dx|bx|si|di),|lea|mov) (ax|cx|dx|bx|si|di)(?:, (.+))?)re");
        if (const auto m = sets_register.full_match(tw); m && same && (!m->has(1) || (*m)[1].empty() || (*m)[1] == (*m)[2])) {
            const bool has_rest = m->has(3) && !(*m)[3].empty();
            if ((m->has(1) && !(*m)[1].empty()) || !has_rest || eq(rename(std::string((*m)[3]), renaming_), (*m)[3])) {   // a register set anew by both
                const std::string r((*m)[2]);
                Renaming next;
                for (const auto& [k, v] : renaming_)
                    if (k != r && v != r) next[k] = v;
                set_renaming(next, ow);
                return advance(1, 1);
            }
        }
        if (stretch()) return std::nullopt;
        std::vector<std::string> parts;
        for (const auto& [k, v] : renaming_) parts.push_back(text::upper(k) + " for " + text::upper(v));
        return std::format("with {}, at +{}: original {}, compiled {}", text::join(parts, ", "), nc_hex4(ow), tw, tg);
    }

    ByteView want_, got_;
    const std::set<std::uint32_t>& fx_;
    std::vector<Row> W_, G_;
    std::size_t wi_ = 0, gi_ = 0;
    std::vector<std::pair<std::int64_t, std::int64_t>> amap_, jumps_;
    std::vector<std::string> notes_;
    Renaming renaming_;
    std::size_t since_ = 0;
    std::vector<std::int64_t> fors_;
    std::optional<std::size_t> reload_;
    std::map<std::int64_t, std::int64_t> temps_, reverse_temps_;
    std::optional<std::pair<std::string, std::size_t>> count_;
    std::optional<std::pair<std::size_t, std::size_t>> sync_;
};

}  // namespace

Finding equivalent(ByteView want, ByteView got, const std::set<std::uint32_t>& linker_words, const cmpp::Options& options) {
    return NewcompComparer(want, got, linker_words, options).run();
}

// ---- the entry helper of MS-Pascal 3.0x ---------------------------------------------------------------------------------

namespace {

std::string nc_local(const std::string& t) {   // [bp - n] as [bp - n + 4] in hex
    static const Regex frame(R"re(\[bp - (0x[0-9a-f]+|\d+)\])re");
    return frame.replace_all(t, std::function<std::string(const RegexMatch&)>([](const RegexMatch& m) {
        const sv n = m[1];
        const std::int64_t v = n.starts_with("0x") ? *text::parse_int<std::int64_t>(n.substr(2), 16) : *text::parse_int<std::int64_t>(n, 10);
        return std::format("[bp - 0x{:x}]", v + 4);
    }));
}

std::string nc_hexed(const std::string& t) {
    static const Regex frame(R"re(\[bp - (\d+)\])re");
    return frame.replace_all(t, std::function<std::string(const RegexMatch&)>([](const RegexMatch& m) { return std::format("[bp - 0x{:x}]", *text::parse_int<std::int64_t>(m[1], 10)); }));
}

}  // namespace

Finding equivalent3(ByteView want, ByteView got, const std::set<std::uint32_t>& fx, const cmpp::Options& options) {
    const auto failure = [](std::string why) {
        Finding f;
        f.notes.push_back(std::move(why));
        return f;
    };
    if (want.size() < 5 || got.size() < 6 || want[0] != kCall || got[0] != kCall ||
        ((got[4] | got[5] << 8) - (want[3] | want[4] << 8) != 0 && (got[4] | got[5] << 8) - (want[3] | want[4] << 8) != 4))
        return failure("the entry is not call helper / dw frame");
    std::vector<Row> W, G;
    for (Row r : cmpp::walk(want.subspan(5), 0, false, options)) {
        r.offset += 5;
        W.push_back(std::move(r));
    }
    for (Row r : cmpp::walk(got.subspan(6), 0, false, options)) {
        r.offset += 6;
        G.push_back(std::move(r));
    }
    std::vector<std::string> notes{"entry helper of MS-Pascal 3.0x"};
    std::vector<std::pair<std::int64_t, std::int64_t>> amap{{0, 0}}, jumps;
    std::size_t wi = 0, gi = 0;
    static const Regex add_di(R"re(add di, (\w+))re");
    static const Regex byte_to_dl(R"re(mov byte ptr \[.*\], dl)re");
    while (wi < W.size() && gi < G.size()) {
        const Row &rw_row = W[wi], &rg_row = G[gi];
        const std::size_t ow = rw_row.offset, sw = rw_row.size, og = rg_row.offset, sg = rg_row.size;
        const std::string &tw = rw_row.text, &tg = rg_row.text;
        const ByteView bw = want.subspan(ow, sw), bg = got.subspan(og, sg);
        amap.emplace_back(ow, og);
        bool fxg = false;
        for (std::size_t k = 0; k < sg; ++k) fxg = fxg || fx.contains(static_cast<std::uint32_t>(og + k));
        bool same = sw == sg;
        for (std::size_t k = 0; same && k < sw; ++k) same = bw[k] == bg[k] || fx.contains(static_cast<std::uint32_t>(og + k));
        const auto eq = [&](const std::string& a, const std::string& b) { return a == b || (fxg && mask(a) == mask(b)); };
        const auto rw = relative(bw), rg = relative(bg);
        if (rw && rg && bw[0] == bg[0] && !fxg) {
            if (bw[0] != kCall) jumps.emplace_back(static_cast<std::int64_t>(ow + sw) + rw->displacement, static_cast<std::int64_t>(og + sg) + rg->displacement);
            ++wi;
            ++gi;
            continue;
        }
        if (same) {
            ++wi;
            ++gi;
            continue;
        }
        if (nc_local(tw) == nc_hexed(tg) || (tw == "mov sp, bp" && tg == "lea sp, [bp - 4]")) {
            ++wi;
            ++gi;
            continue;
        }
        if (add_di.full_match(tw) && wi + 1 < W.size() && fxg && mask(text::replace_all(W[wi + 1].text, "[di]", "[di + N]")) == mask(tg)) {
            notes.push_back(std::format("indexed store at +{}", nc_hex4(ow)));
            wi += 2;
            ++gi;
            continue;
        }
        if (tw.starts_with("mov dx, word ptr [") && eq(text::replace_all(tw, "mov dx,", "mov ax,"), tg)) {
            notes.push_back(std::format("byte moved through DX at +{}", nc_hex4(ow)));
            ++wi;
            ++gi;
            continue;
        }
        if (byte_to_dl.full_match(tw) && eq(tw.substr(0, tw.size() - 2) + "al", tg)) {
            ++wi;
            ++gi;
            continue;
        }
        if (tw.starts_with("mov dx, ") && wi + 2 < W.size() && W[wi + 1].text == "push ds" && W[wi + 2].text == "push dx" && gi + 4 < G.size() && mask(tg) == "mov bx, N" &&
            G[gi + 1].text == "mov dx, ds" && G[gi + 2].text == "mov es, dx" && G[gi + 3].text == "push es" && G[gi + 4].text == "push bx") {
            notes.push_back(std::format("far address pushed at +{}", nc_hex4(ow)));
            wi += 3;
            gi += 5;
            continue;
        }
        if (tw.starts_with("or al, byte ptr [bp") && text::replace_all(nc_local(tw), "or al, byte ptr", "or ax, word ptr") == nc_hexed(tg)) {
            ++wi;
            ++gi;
            continue;
        }
        return failure(std::format("no pattern at +{}: original {}, compiled {}", nc_hex4(ow), tw, tg));
    }
    if (wi != W.size() || gi != G.size()) return failure("the two do not end together");
    std::map<std::int64_t, std::int64_t> at;
    for (const auto& [a, b] : amap) at[a] = b;
    at[static_cast<std::int64_t>(want.size())] = static_cast<std::int64_t>(got.size());
    for (const auto& [a, b] : jumps) {
        const auto f = at.find(a);
        if (f == at.end() || f->second != b) return failure(std::format("a jump to +{} goes to +{} in the compiled code", nc_hex4(a), nc_hex4(b)));
    }
    Finding out;
    out.same = true;
    out.notes = notes;
    std::set<std::pair<std::int64_t, std::int64_t>> unique(amap.begin(), amap.end());
    for (const auto& [a, b] : unique) out.alignment.emplace_back(static_cast<std::uint32_t>(a), static_cast<std::uint32_t>(b));
    for (const Row& g : G) out.compiled_starts.push_back(g.offset);
    return out;
}

}  // namespace cb::exact::habits::newcomp
