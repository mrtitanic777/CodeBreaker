// py_difflib.hpp -- Python's difflib: SequenceMatcher (isjunk, autojunk, ratio, quick_ratio, real_quick_ratio,
// find_longest_match, get_matching_blocks, get_opcodes, get_grouped_opcodes) and unified_diff, written as CPython's
// Lib/difflib.py writes them, so the blocks and opcodes come out the same on ties.
//
//   py::difflib::SequenceMatcher<py::str> sm(a, b);                 // a, b: py::list<T> or std::vector<T>
//   sm.ratio(); sm.get_opcodes();                                    // tuples (tag, i1, i2, j1, j2)
//   py::difflib::SequenceMatcher<int64_t> sm(a, b, isjunk, false);   // isjunk: std::function<bool(const T&)>, autojunk
//   py::difflib::unified_diff(a, b, fromfile, tofile, fromfiledate, tofiledate, n, lineterm)
//
// T: anything py::hasher<T> hashes and == compares (int64_t, py::str, py::bytes, tuples ...).
#pragma once
#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "py.hpp"

namespace py {
namespace difflib {

struct Match {   // difflib.Match(a, b, size)
    int64_t a, b, size;
    bool operator<(const Match& o) const { return std::tie(a, b, size) < std::tie(o.a, o.b, o.size); }
    bool operator==(const Match& o) const { return a == o.a && b == o.b && size == o.size; }
};
using Opcode = std::tuple<str, int64_t, int64_t, int64_t, int64_t>;   // (tag, i1, i2, j1, j2)

namespace detail_ {
template <class T> struct elt_hash {
    size_t operator()(const T& v) const { return py::hasher<T>()(v); }
};
template <class T, class C> std::vector<T> vec_of_(const C& c) {   // a py::list, std::vector, ... as a vector
    std::vector<T> v;
    for (const auto& x : c) v.push_back(x);
    return v;
}
inline double ratio_(int64_t matches, int64_t length) { return length ? 2.0 * static_cast<double>(matches) / static_cast<double>(length) : 1.0; }
}  // namespace detail_

template <class T> class SequenceMatcher {
  public:
    using Seq = std::vector<T>;
    using IsJunk = std::function<bool(const T&)>;

    SequenceMatcher() : SequenceMatcher(Seq(), Seq()) {}
    template <class A, class B>
    SequenceMatcher(const A& a, const B& b, IsJunk isjunk = nullptr, bool autojunk = true) : isjunk_(std::move(isjunk)), autojunk_(autojunk) {
        set_seqs(a, b);
    }

    template <class A, class B> void set_seqs(const A& a, const B& b) {
        set_seq1(a);
        set_seq2(b);
    }
    template <class A> void set_seq1(const A& a) {
        a_ = detail_::vec_of_<T>(a);
        matching_blocks_.reset();
        opcodes_.reset();
    }
    template <class B> void set_seq2(const B& b) {
        b_ = detail_::vec_of_<T>(b);
        matching_blocks_.reset();
        opcodes_.reset();
        chain_b_();
    }

    // find_longest_match(alo=0, ahi=None, blo=0, bhi=None); pass -1 for an omitted ahi / bhi
    Match find_longest_match(int64_t alo = 0, int64_t ahi = -1, int64_t blo = 0, int64_t bhi = -1) const {
        if (ahi < 0) ahi = static_cast<int64_t>(a_.size());
        if (bhi < 0) bhi = static_cast<int64_t>(b_.size());
        int64_t besti = alo, bestj = blo, bestsize = 0;
        // j2len[j - blo + 1] is the length of the match ending at b[j] for the previous i (index 0 stays 0: j - 1 < blo); flat
        // arrays with the touched slots listed, where CPython keeps a dict: the same values, none of the hashing
        const size_t width = static_cast<size_t>(bhi > blo ? bhi - blo : 0) + 2;
        std::vector<int64_t> j2len(width, 0);
        std::vector<std::pair<size_t, int64_t>> fresh, stale;
        for (int64_t i = alo; i < ahi; i++) {
            fresh.clear();
            const auto found = b2j_.find(a_[static_cast<size_t>(i)]);
            if (found != b2j_.end()) {
                for (const int64_t j : found->second) {
                    if (j < blo) continue;
                    if (j >= bhi) break;
                    const size_t at = static_cast<size_t>(j - blo + 1);
                    const int64_t k = j2len[at - 1] + 1;
                    fresh.emplace_back(at, k);
                    if (k > bestsize) {
                        besti = i - k + 1;
                        bestj = j - k + 1;
                        bestsize = k;
                    }
                }
            }
            for (const auto& s : stale) j2len[s.first] = 0;
            for (const auto& f : fresh) j2len[f.first] = f.second;
            std::swap(stale, fresh);
        }
        // extend over the non-junk elements (the popular ones too: they are not in b2j but not junk either) ...
        while (besti > alo && bestj > blo && !bjunk_(b_[bestj - 1]) && a_[besti - 1] == b_[bestj - 1]) {
            besti--;
            bestj--;
            bestsize++;
        }
        while (besti + bestsize < ahi && bestj + bestsize < bhi && !bjunk_(b_[bestj + bestsize]) && a_[besti + bestsize] == b_[bestj + bestsize]) bestsize++;
        // ... then the junk that matches on either side
        while (besti > alo && bestj > blo && bjunk_(b_[bestj - 1]) && a_[besti - 1] == b_[bestj - 1]) {
            besti--;
            bestj--;
            bestsize++;
        }
        while (besti + bestsize < ahi && bestj + bestsize < bhi && bjunk_(b_[bestj + bestsize]) && a_[besti + bestsize] == b_[bestj + bestsize]) bestsize++;
        return Match{besti, bestj, bestsize};
    }

    const std::vector<Match>& get_matching_blocks() const {
        if (matching_blocks_) return *matching_blocks_;
        const int64_t la = static_cast<int64_t>(a_.size()), lb = static_cast<int64_t>(b_.size());
        std::vector<std::tuple<int64_t, int64_t, int64_t, int64_t>> queue{{0, la, 0, lb}};
        std::vector<Match> matching;
        while (!queue.empty()) {
            const auto [alo, ahi, blo, bhi] = queue.back();
            queue.pop_back();
            const Match x = find_longest_match(alo, ahi, blo, bhi);
            if (x.size) {
                matching.push_back(x);
                if (alo < x.a && blo < x.b) queue.emplace_back(alo, x.a, blo, x.b);
                if (x.a + x.size < ahi && x.b + x.size < bhi) queue.emplace_back(x.a + x.size, ahi, x.b + x.size, bhi);
            }
        }
        std::sort(matching.begin(), matching.end());
        int64_t i1 = 0, j1 = 0, k1 = 0;
        std::vector<Match> out;
        for (const Match& m : matching) {   // (collapse adjacent blocks)
            if (i1 + k1 == m.a && j1 + k1 == m.b) {
                k1 += m.size;
            } else {
                if (k1) out.push_back({i1, j1, k1});
                i1 = m.a;
                j1 = m.b;
                k1 = m.size;
            }
        }
        if (k1) out.push_back({i1, j1, k1});
        out.push_back({la, lb, 0});
        matching_blocks_ = std::move(out);
        return *matching_blocks_;
    }

    const std::vector<Opcode>& get_opcodes() const {
        if (opcodes_) return *opcodes_;
        int64_t i = 0, j = 0;
        std::vector<Opcode> answer;
        for (const Match& blk : get_matching_blocks()) {
            const char* tag = "";
            if (i < blk.a && j < blk.b) tag = "replace";
            else if (i < blk.a) tag = "delete";
            else if (j < blk.b) tag = "insert";
            if (*tag) answer.emplace_back(str(tag), i, blk.a, j, blk.b);
            i = blk.a + blk.size;
            j = blk.b + blk.size;
            if (blk.size) answer.emplace_back(str("equal"), blk.a, i, blk.b, j);
        }
        opcodes_ = std::move(answer);
        return *opcodes_;
    }

    std::vector<std::vector<Opcode>> get_grouped_opcodes(int64_t n = 3) const {
        // (as CPython: `codes` is the cached get_opcodes() list itself, and the two fix-ups below change it in place, so
        // a later get_opcodes() on the same matcher sees the trimmed first and last "equal" -- difflib's own quirk)
        get_opcodes();
        std::vector<Opcode> fallback;
        if (opcodes_->empty()) fallback.emplace_back(str("equal"), 0, 1, 0, 1);
        std::vector<Opcode>& codes = opcodes_->empty() ? fallback : *opcodes_;
        if (std::get<0>(codes.front()) == str("equal")) {   // fix up leading and trailing groups if they show no changes
            const auto [tag, i1, i2, j1, j2] = codes.front();
            codes.front() = Opcode(tag, std::max(i1, i2 - n), i2, std::max(j1, j2 - n), j2);
        }
        if (std::get<0>(codes.back()) == str("equal")) {
            const auto [tag, i1, i2, j1, j2] = codes.back();
            codes.back() = Opcode(tag, i1, std::min(i2, i1 + n), j1, std::min(j2, j1 + n));
        }
        const int64_t nn = n + n;
        std::vector<std::vector<Opcode>> groups;
        std::vector<Opcode> group;
        for (const Opcode& c : codes) {
            auto [tag, i1, i2, j1, j2] = c;
            if (tag == str("equal") && i2 - i1 > nn) {   // end the current group and start a new one
                group.emplace_back(tag, i1, std::min(i2, i1 + n), j1, std::min(j2, j1 + n));
                groups.push_back(group);
                group.clear();
                i1 = std::max(i1, i2 - n);
                j1 = std::max(j1, j2 - n);
            }
            group.emplace_back(tag, i1, i2, j1, j2);
        }
        if (!group.empty() && !(group.size() == 1 && std::get<0>(group[0]) == str("equal"))) groups.push_back(group);
        return groups;
    }

    double ratio() const {
        int64_t matches = 0;
        for (const Match& m : get_matching_blocks()) matches += m.size;
        return detail_::ratio_(matches, static_cast<int64_t>(a_.size() + b_.size()));
    }

    double quick_ratio() const {   // an upper bound: the multiset intersection of the elements
        std::unordered_map<T, int64_t, detail_::elt_hash<T>> fullbcount;
        for (const T& elt : b_) fullbcount[elt]++;
        std::unordered_map<T, int64_t, detail_::elt_hash<T>> avail;
        int64_t matches = 0;
        for (const T& elt : a_) {
            auto it = avail.find(elt);
            int64_t numb;
            if (it != avail.end()) numb = it->second;
            else {
                auto f = fullbcount.find(elt);
                numb = f == fullbcount.end() ? 0 : f->second;
            }
            avail[elt] = numb - 1;
            if (numb > 0) matches++;
        }
        return detail_::ratio_(matches, static_cast<int64_t>(a_.size() + b_.size()));
    }

    double real_quick_ratio() const {
        const int64_t la = static_cast<int64_t>(a_.size()), lb = static_cast<int64_t>(b_.size());
        return detail_::ratio_(std::min(la, lb), la + lb);
    }

    const Seq& a() const { return a_; }
    const Seq& b() const { return b_; }

  private:
    bool bjunk_(const T& x) const { return bjunk_set_.find(x) != bjunk_set_.end(); }

    void chain_b_() {   // __chain_b: b2j, the junk and the popular elements left out
        b2j_.clear();
        bjunk_set_.clear();
        for (size_t i = 0; i < b_.size(); i++) b2j_[b_[i]].push_back(static_cast<int64_t>(i));
        if (isjunk_) {
            for (auto it = b2j_.begin(); it != b2j_.end();) {
                if (isjunk_(it->first)) {
                    bjunk_set_.insert(it->first);
                    it = b2j_.erase(it);
                } else ++it;
            }
        }
        const size_t n = b_.size();
        if (autojunk_ && n >= 200) {
            const size_t ntest = n / 100 + 1;
            for (auto it = b2j_.begin(); it != b2j_.end();) {
                if (it->second.size() > ntest) it = b2j_.erase(it);
                else ++it;
            }
        }
    }

    IsJunk isjunk_;
    bool autojunk_;
    Seq a_, b_;
    std::unordered_map<T, std::vector<int64_t>, detail_::elt_hash<T>> b2j_;
    std::unordered_set<T, detail_::elt_hash<T>> bjunk_set_;
    mutable std::optional<std::vector<Match>> matching_blocks_;
    mutable std::optional<std::vector<Opcode>> opcodes_;
};

namespace detail_ {
inline str format_range_unified_(int64_t start, int64_t stop) {   // difflib._format_range_unified
    int64_t beginning = start + 1;
    const int64_t length = stop - start;
    if (length == 1) return str(std::to_string(beginning));
    if (!length) beginning -= 1;
    return str(std::to_string(beginning) + "," + std::to_string(length));
}
}  // namespace detail_

// difflib.unified_diff(a, b, fromfile='', tofile='', fromfiledate='', tofiledate='', n=3, lineterm='\n'), as a list of lines
template <class A, class B>
list<str> unified_diff(const A& a, const B& b, const str& fromfile = str(""), const str& tofile = str(""), const str& fromfiledate = str(""),
                       const str& tofiledate = str(""), int64_t n = 3, const str& lineterm = str("\n")) {
    const std::vector<str> av = detail_::vec_of_<str>(a), bv = detail_::vec_of_<str>(b);
    list<str> out;
    bool started = false;
    const SequenceMatcher<str> sm(av, bv);
    for (const std::vector<Opcode>& group : sm.get_grouped_opcodes(n)) {
        if (!started) {
            started = true;
            const str fromdate = fromfiledate.empty() ? str("") : str("\t") + fromfiledate;
            const str todate = tofiledate.empty() ? str("") : str("\t") + tofiledate;
            out.append(str("--- ") + fromfile + fromdate + lineterm);
            out.append(str("+++ ") + tofile + todate + lineterm);
        }
        const Opcode& first = group.front();
        const Opcode& last = group.back();
        out.append(str("@@ -") + detail_::format_range_unified_(std::get<1>(first), std::get<2>(last)) + str(" +") +
                   detail_::format_range_unified_(std::get<3>(first), std::get<4>(last)) + str(" @@") + lineterm);
        for (const Opcode& op : group) {
            const auto& [tag, i1, i2, j1, j2] = op;
            if (tag == str("equal")) {
                for (int64_t k = i1; k < i2; k++) out.append(str(" ") + av[static_cast<size_t>(k)]);
                continue;
            }
            if (tag == str("replace") || tag == str("delete"))
                for (int64_t k = i1; k < i2; k++) out.append(str("-") + av[static_cast<size_t>(k)]);
            if (tag == str("replace") || tag == str("insert"))
                for (int64_t k = j1; k < j2; k++) out.append(str("+") + bv[static_cast<size_t>(k)]);
        }
    }
    return out;
}

}  // namespace difflib
}  // namespace py
