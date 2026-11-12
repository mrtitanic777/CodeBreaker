// cb/compare/versions_internal.hpp -- what versions.cpp and versions_report.cpp share: the digest the fingerprints are made with
// and the sequence matcher. Not an interface.
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <deque>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace cb::compare::versions::detail {

// ---- BLAKE2b with an 8-byte digest (RFC 7693) ------------------------------------------------------------------------------
// The fingerprints of a function and the runs of shapes are this digest, so that they are the same in every process and every
// run (a hash of the platform's would change where ties are broken).
inline std::array<std::uint8_t, 8> blake2b8(std::string_view data) {
    static constexpr std::uint64_t kIv[8] = {0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
                                             0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};
    static constexpr std::uint8_t kSigma[10][16] = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
        {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4}, {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
        {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13}, {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
        {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11}, {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
        {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5}, {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0}};
    auto rotr = [](std::uint64_t x, int n) { return (x >> n) | (x << (64 - n)); };
    std::uint64_t h[8];
    for (int i = 0; i < 8; ++i) h[i] = kIv[i];
    h[0] ^= 0x01010000ULL ^ 8ULL;   // digest length 8, no key, sequential
    auto compress = [&](const std::uint8_t* block, std::uint64_t counter, bool last) {
        std::uint64_t m[16], v[16];
        for (int i = 0; i < 16; ++i) {
            m[i] = 0;
            for (int k = 7; k >= 0; --k) m[i] = (m[i] << 8) | block[i * 8 + k];
        }
        for (int i = 0; i < 8; ++i) {
            v[i] = h[i];
            v[i + 8] = kIv[i];
        }
        v[12] ^= counter;
        if (last) v[14] = ~v[14];
        auto g = [&](int a, int b, int c, int d, std::uint64_t x, std::uint64_t y) {
            v[a] = v[a] + v[b] + x;
            v[d] = rotr(v[d] ^ v[a], 32);
            v[c] = v[c] + v[d];
            v[b] = rotr(v[b] ^ v[c], 24);
            v[a] = v[a] + v[b] + y;
            v[d] = rotr(v[d] ^ v[a], 16);
            v[c] = v[c] + v[d];
            v[b] = rotr(v[b] ^ v[c], 63);
        };
        for (int r = 0; r < 12; ++r) {
            const std::uint8_t* s = kSigma[r % 10];
            g(0, 4, 8, 12, m[s[0]], m[s[1]]);
            g(1, 5, 9, 13, m[s[2]], m[s[3]]);
            g(2, 6, 10, 14, m[s[4]], m[s[5]]);
            g(3, 7, 11, 15, m[s[6]], m[s[7]]);
            g(0, 5, 10, 15, m[s[8]], m[s[9]]);
            g(1, 6, 11, 12, m[s[10]], m[s[11]]);
            g(2, 7, 8, 13, m[s[12]], m[s[13]]);
            g(3, 4, 9, 14, m[s[14]], m[s[15]]);
        }
        for (int i = 0; i < 8; ++i) h[i] ^= v[i] ^ v[i + 8];
    };
    const std::size_t n = data.size();
    std::size_t off = 0;
    std::uint8_t block[128];
    while (n - off > 128) {
        std::memcpy(block, data.data() + off, 128);
        off += 128;
        compress(block, off, false);
    }
    std::memset(block, 0, sizeof block);
    if (n - off) std::memcpy(block, data.data() + off, n - off);
    compress(block, n, true);
    std::array<std::uint8_t, 8> out;
    for (int i = 0; i < 8; ++i) out[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>((h[0] >> (8 * i)) & 0xFF);
    return out;
}

inline std::string hex_of(const std::array<std::uint8_t, 8>& d) {
    static constexpr char kDigits[] = "0123456789abcdef";
    std::string out;
    for (std::uint8_t c : d) {
        out += kDigits[c >> 4];
        out += kDigits[c & 15];
    }
    return out;
}

// ---- difflib.SequenceMatcher(None, a, b, autojunk=False) over sequences of numbers -------------------------------------------
// The longest-match search and the block merging are difflib's, so the same inputs pair the same elements; the table of run
// lengths is two arrays instead of a dictionary per element.
class SeqMatcher {
public:
    struct Block {
        int i, j, k;
        friend bool operator<(const Block& x, const Block& y) { return std::tie(x.i, x.j, x.k) < std::tie(y.i, y.j, y.k); }
    };
    enum class Tag { Equal, Replace, Delete, Insert };
    struct Opcode {
        Tag tag;
        int i1, i2, j1, j2;
    };

    SeqMatcher(const std::vector<int>& a, const std::vector<int>& b) : a_(a), b_(b), prev_(b.size() + 1, 0), cur_(b.size() + 1, 0) {
        int top = -1;
        for (int x : b) top = std::max(top, x);
        for (int x : a) top = std::max(top, x);
        b2j_.assign(static_cast<std::size_t>(top + 1), {});
        for (std::size_t j = 0; j < b_.size(); ++j) b2j_[static_cast<std::size_t>(b_[j])].push_back(static_cast<int>(j));
    }

    Block find_longest_match(int alo, int ahi, int blo, int bhi) {
        int besti = alo, bestj = blo, bestsize = 0;
        std::vector<int>* prev = &prev_;
        std::vector<int>* cur = &cur_;
        std::vector<int> touched_prev, touched_cur;
        for (int i = alo; i < ahi; ++i) {
            for (int j : b2j_[static_cast<std::size_t>(a_[static_cast<std::size_t>(i)])]) {
                if (j < blo) continue;
                if (j >= bhi) break;
                const int k = (*prev)[static_cast<std::size_t>(j)] + 1;   // prev[j] is the run ending at b[j-1] (slot j)
                (*cur)[static_cast<std::size_t>(j) + 1] = k;
                touched_cur.push_back(j + 1);
                if (k > bestsize) {
                    besti = i - k + 1;
                    bestj = j - k + 1;
                    bestsize = k;
                }
            }
            for (int t : touched_prev) (*prev)[static_cast<std::size_t>(t)] = 0;
            touched_prev.clear();
            std::swap(prev, cur);
            std::swap(touched_prev, touched_cur);
        }
        for (int t : touched_prev) (*prev)[static_cast<std::size_t>(t)] = 0;
        return {besti, bestj, bestsize};
    }

    std::vector<Block> matching_blocks() {
        const int la = static_cast<int>(a_.size()), lb = static_cast<int>(b_.size());
        std::vector<std::array<int, 4>> queue = {{0, la, 0, lb}};
        std::vector<Block> matching;
        while (!queue.empty()) {
            const auto q = queue.back();
            queue.pop_back();
            const Block x = find_longest_match(q[0], q[1], q[2], q[3]);
            if (x.k) {
                matching.push_back(x);
                if (q[0] < x.i && q[2] < x.j) queue.push_back({q[0], x.i, q[2], x.j});
                if (x.i + x.k < q[1] && x.j + x.k < q[3]) queue.push_back({x.i + x.k, q[1], x.j + x.k, q[3]});
            }
        }
        std::sort(matching.begin(), matching.end());
        int i1 = 0, j1 = 0, k1 = 0;
        std::vector<Block> merged;
        for (const Block& b : matching) {
            if (i1 + k1 == b.i && j1 + k1 == b.j) {
                k1 += b.k;
            } else {
                if (k1) merged.push_back({i1, j1, k1});
                i1 = b.i;
                j1 = b.j;
                k1 = b.k;
            }
        }
        if (k1) merged.push_back({i1, j1, k1});
        merged.push_back({la, lb, 0});
        return merged;
    }

    std::vector<Opcode> opcodes() {
        int i = 0, j = 0;
        std::vector<Opcode> out;
        for (const Block& b : matching_blocks()) {
            if (i < b.i && j < b.j) out.push_back({Tag::Replace, i, b.i, j, b.j});
            else if (i < b.i) out.push_back({Tag::Delete, i, b.i, j, b.j});
            else if (j < b.j) out.push_back({Tag::Insert, i, b.i, j, b.j});
            i = b.i + b.k;
            j = b.j + b.k;
            if (b.k) out.push_back({Tag::Equal, b.i, i, b.j, j});
        }
        return out;
    }

    double ratio() {
        int matches = 0;
        for (const Block& b : matching_blocks()) matches += b.k;
        return score(matches, static_cast<int>(a_.size() + b_.size()));
    }
    double real_quick_ratio() const {
        const int la = static_cast<int>(a_.size()), lb = static_cast<int>(b_.size());
        return score(std::min(la, lb), la + lb);
    }
    // An upper bound of ratio(): the elements the two have in common, wherever they stand.
    double quick_ratio() const {
        std::vector<int> avail(b2j_.size(), 0);
        for (std::size_t x = 0; x < b2j_.size(); ++x) avail[x] = static_cast<int>(b2j_[x].size());
        int matches = 0;
        for (int elt : a_)
            if (avail[static_cast<std::size_t>(elt)]-- > 0) ++matches;
        return score(matches, static_cast<int>(a_.size() + b_.size()));
    }

private:
    static double score(int matches, int length) { return length ? 2.0 * matches / length : 1.0; }
    const std::vector<int>& a_;
    const std::vector<int>& b_;
    std::vector<std::vector<int>> b2j_;   // element -> its places in b, ascending (dense element numbers)
    std::vector<int> prev_, cur_;
};

// Numbers for the distinct strings of the sequences it is shown (dense, from 0), so the matcher can index by element.
class Interner {
public:
    int of(std::string_view s) {
        const auto it = ids_.find(s);
        if (it != ids_.end()) return it->second;
        keep_.emplace_back(s);
        const int n = static_cast<int>(ids_.size());
        ids_.emplace(keep_.back(), n);
        return n;
    }
    std::vector<int> all(const std::vector<std::string>& v) {
        std::vector<int> out;
        out.reserve(v.size());
        for (const std::string& s : v) out.push_back(of(s));
        return out;
    }

private:
    std::unordered_map<std::string_view, int> ids_;   // lookup only
    std::deque<std::string> keep_;                    // (a deque never moves what it holds: the views above stay valid)
};

}  // namespace cb::compare::versions::detail
