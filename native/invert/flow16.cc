// flow16.cc -- invert/flow16.py, line for line: basic blocks from an instruction list, the edges,
// the switches (by a jump table, by compares), dominators and post-dominators, natural loops, and
// the dominator-guided structure tree (seq / if / ifelse / while / dowhile / loop / switch / goto).
//
// Python's sets become bit sets or sorted sets; where the Python's answer depended on a set's
// order (ipdom's first candidate) the order is CPython's for those sets: ascending (the
// candidates are dense small ints there, each in its own slot).  Python's IndexError and
// ValueError are raised (as PyError) where the Python raises them.
#include "flow16.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <set>
#include <unordered_map>

namespace cbinv {

thread_local const std::vector<Ins> *g_insns = nullptr;   // build_cfg's instructions, for its helpers

namespace {

const std::set<std::string> CJMP = {"jl", "jle", "jg", "jge", "je", "jne", "ja", "jae", "jb", "jbe", "jc",
                                    "jnc", "jz", "jnz", "jo", "jno", "js", "jns", "jp", "jnp", "jcxz", "loop"};
const std::set<std::string> RET = {"ret", "retf", "iret"};
const std::set<std::string> REG16AL = {"ax", "bx", "cx", "dx", "si", "di", "al"};

bool in(const std::set<std::string> &s, const std::string &x) { return s.count(x) != 0; }

[[noreturn]] void index_error(const char *what = "list index out of range") { throw PyError{1, what}; }

template <class T>
const T &at(const std::vector<T> &v, long long i) {             // Python's v[i], negative from the end
  long long n = (long long)v.size();
  if (i < 0) i += n;
  if (i < 0 || i >= n) index_error();
  return v[(size_t)i];
}
template <class T>
T &at(std::vector<T> &v, long long i) {
  long long n = (long long)v.size();
  if (i < 0) i += n;
  if (i < 0 || i >= n) index_error();
  return v[(size_t)i];
}

bool is_word(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool all_word(const std::string &s) {
  if (s.empty()) return false;
  for (char c : s)
    if (!is_word(c)) return false;
  return true;
}
bool all_digits(const std::string &s, size_t from = 0) {
  if (s.size() <= from) return false;
  for (size_t i = from; i < s.size(); i++)
    if (s[i] < '0' || s[i] > '9') return false;
  return true;
}
bool all_lhex(const std::string &s, size_t from) {
  if (s.size() <= from) return false;
  for (size_t i = from; i < s.size(); i++)
    if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f'))) return false;
  return true;
}
bool starts(const std::string &s, const std::string &p) { return s.compare(0, p.size(), p) == 0; }

// (0x[0-9a-f]+|\d+) as a whole string -> its value by int(x, 0); nullopt when it does not match
std::optional<long long> number(const std::string &s) {
  if (s.size() > 2 && s[0] == '0' && s[1] == 'x' && all_lhex(s, 2)) return std::stoll(s.substr(2), nullptr, 16);
  if (all_digits(s)) {
    bool zeros = s.find_first_not_of('0') == std::string::npos;
    if (s.size() > 1 && s[0] == '0' && !zeros)                // int("012", 0) raises
      throw PyError{2, "invalid literal for int() with base 0: '" + s + "'"};
    return std::stoll(s);
  }
  return std::nullopt;
}

// _jump_target: re.match(r"(0x[0-9a-f]+|\d+)$", op) (a "$" also before one trailing newline)
std::optional<long long> jump_target(const Ins &ins) {
  std::string s = ins.op;
  if (!s.empty() && s.back() == '\n') s.pop_back();
  return number(s);
}

// _cmp_of(ins, reg): (register, value) of a compare with a constant (or a register tested: 0)
std::optional<std::pair<std::string, long long>> cmp_of(const Ins &ins, const std::string &reg = "") {
  const std::string &op = ins.op;
  if (ins.mn == "or" || ins.mn == "test" || ins.mn == "and") {
    if (op.size() >= 4 && (op.size() - 2) % 2 == 0) {
      size_t h = (op.size() - 2) / 2;
      std::string x = op.substr(0, h);
      if (all_word(x) && op.compare(h, 2, ", ") == 0 && op.compare(h + 2, std::string::npos, x) == 0) {
        std::string r = op.substr(0, op.find(','));
        if (in(REG16AL, r) && (reg.empty() || r == reg)) return std::make_pair(r, 0LL);
        return std::nullopt;
      }
    }
  }
  if (ins.mn != "cmp") return std::nullopt;
  size_t c = op.find(", ");
  if (c == std::string::npos) return std::nullopt;
  std::string g1 = op.substr(0, c), g2 = op.substr(c + 2);
  if (!all_word(g1)) return std::nullopt;
  bool hex = g2.size() > 2 && g2[0] == '0' && g2[1] == 'x' && all_lhex(g2, 2);
  if (!hex && !all_digits(g2)) return std::nullopt;
  if (!in(REG16AL, g1) || (!reg.empty() && g1 != reg)) return std::nullopt;
  return std::make_pair(g1, *number(g2));
}

std::vector<int> sorted_unique(std::vector<int> v) {
  std::sort(v.begin(), v.end());
  v.erase(std::unique(v.begin(), v.end()), v.end());
  return v;
}

std::vector<std::vector<int>> preds_of(const std::vector<Block> &blocks) {
  std::vector<std::vector<int>> preds(blocks.size());
  for (auto &b : blocks)
    for (int t : b.succ) at(preds, t).push_back(b.id);
  return preds;
}

void table_switches(std::vector<Block> &blocks) {
  auto preds = preds_of(blocks);
  auto jmp_to = [&](int bid) {
    const Block &x = at(blocks, bid);
    return x.term == "jmp" && x.hi - x.lo == 1 ? at(x.succ, 0) : bid;
  };
  for (size_t di = 0; di < blocks.size(); di++) {
    Block &d = blocks[di];
    if (d.term != "switch" || !d.has_table || preds[d.id].size() != 1) continue;
    Block &h = at(blocks, preds[d.id][0]);
    int hn = h.hi - h.lo;
    if (h.term != "cond" || hn < 2) continue;
    // (h.insns are read from the caller's list through lo/hi)
    const std::vector<Ins> &I = *g_insns;
    auto hins = [&](int k) -> const Ins & { return I[(size_t)(h.hi + k)]; };   // k < 0
    if (!cmp_of(hins(-2)) || (hins(-1).mn != "ja" && hins(-1).mn != "jbe")) continue;
    auto rc = *cmp_of(hins(-2));
    std::string reg = rc.first;
    long long n = rc.second;
    int taken = at(h.succ, 0), fall = at(h.succ, 1);
    int other = taken == d.id ? fall : taken;
    if ((d.id != taken && d.id != fall) || (long long)d.table.size() != n + 1) continue;
    int deflt = jmp_to(other);
    long long base = 0;
    int cut = hn - 2;
    if (hn >= 3) {
      const Ins &s = hins(-3);
      std::optional<long long> v;
      if (starts(s.op, reg + ", ")) {
        std::string g = s.op.substr(reg.size() + 2);
        bool hex = g.size() > 2 && g[0] == '0' && g[1] == 'x' && all_lhex(g, 2);
        if (hex || all_digits(g)) v = 1;
        if (v && s.mn == "sub") {
          base = *number(g);
          cut = hn - 3;
        }
      }
    }
    h.term = "switch";
    h.cases.clear();
    for (size_t k = 0; k < d.table.size(); k++)
      if (d.table[k] != deflt) h.cases.push_back({base + (long long)k, d.table[k]});
    h.has_cases = true;
    h.def = deflt;
    h.selreg = reg;
    h.selbase = cut;
    h.has_table = false;
    h.table.clear();
    std::vector<int> s;
    for (auto &c : h.cases) s.push_back(c.second);
    s.push_back(deflt);
    h.succ = sorted_unique(s);
    d.succ.clear();
    d.term = "dead";
    if (other != deflt) {
      at(blocks, other).succ.clear();
      at(blocks, other).term = "dead";
    }
  }
}

void compare_switches(std::vector<Block> &blocks) {
  const std::vector<Ins> &I = *g_insns;
  auto preds = preds_of(blocks);
  auto bins = [&](const Block &b, long long k) -> const Ins & {   // b.insns[k]
    long long n = b.hi - b.lo;
    if (k < 0) k += n;
    if (k < 0 || k >= n) index_error();
    return I[(size_t)(b.lo + k)];
  };
  auto test_block = [&](int bid, const std::string &reg) {
    const Block &blk = at(blocks, bid);
    int n = blk.hi - blk.lo;
    if (n == 0 || blk.term == "dead") return false;
    if (n == 1) return in(CJMP, bins(blk, 0).mn) || blk.term == "jmp";
    return n == 2 && cmp_of(bins(blk, 0), reg).has_value() && (in(CJMP, bins(blk, 1).mn) || blk.term == "jmp");
  };
  for (size_t hi_ = 0; hi_ < blocks.size(); hi_++) {
    Block &h = blocks[hi_];
    int hn = h.hi - h.lo;
    if (h.term != "cond" || hn < 2 || !cmp_of(bins(h, -2))) continue;
    std::string reg = cmp_of(bins(h, -2))->first;
    {
      const Ins &i = bins(h, -2);
      static const std::set<std::string> M = {"mov", "add", "sub", "inc", "dec", "pop"};
      if (in(M, i.mn) && starts(i.op, reg + ",")) continue;
    }
    std::vector<std::pair<long long, int>> cases;
    std::set<int> leaves, region = {h.id};
    bool ok = true;
    auto all_in_region = [&](int t) {
      for (int p : preds[t])
        if (!region.count(p)) return false;
      return true;
    };
    std::function<void(int, std::optional<long long>, std::optional<long long>)> follow;
    std::function<void(int, std::optional<long long>, bool, std::optional<long long>)> walk =
        [&](int bid, std::optional<long long> v, bool first, std::optional<long long> lo) {
          const Block &blk = at(blocks, bid);
          int n = blk.hi - blk.lo;
          int s0 = first ? std::max(0, n - 2) : 0;          // seq = blk.insns[-2:] if first
          int sn = n - s0;
          auto seq = [&](int k) -> const Ins & {
            if (k < 0 || k >= sn) index_error();
            return I[(size_t)(blk.lo + s0 + k)];
          };
          int k = 0;
          auto c = cmp_of(seq(0), reg);
          if (c) {
            v = c->second;
            k = 1;
          }
          const Ins &last = seq(k);
          if (blk.term == "jmp") {
            follow(at(blk.succ, 0), v, std::nullopt);
            return;
          }
          if (!v || blk.succ.size() != 2) {
            ok = false;
            return;
          }
          int taken = blk.succ[0], fall = blk.succ[1];
          const std::string &m = last.mn;
          if (m == "je" || m == "jz") {
            cases.push_back({*v, taken});
            follow(fall, v, std::nullopt);
          } else if (m == "jne" || m == "jnz") {
            cases.push_back({*v, fall});
            follow(taken, v, std::nullopt);
          } else if ((m == "jl" || m == "jb") && k) {
            follow(taken, v, std::nullopt);
            follow(fall, v, v);
          } else if ((m == "jle" || m == "jbe") && lo && 0 <= *v - *lo && *v - *lo <= 16 && !test_block(taken, reg)) {
            for (long long x = *lo; x < *v + 1; x++) cases.push_back({x, taken});
            follow(fall, v, std::nullopt);
          } else if ((m == "jg" || m == "ja") && lo && 0 <= *v - *lo && *v - *lo <= 16 && !test_block(fall, reg)) {
            for (long long x = *lo; x < *v + 1; x++) cases.push_back({x, fall});
            follow(taken, v, std::nullopt);
          } else {
            follow(taken, v, std::nullopt);
            follow(fall, v, std::nullopt);
          }
        };
    follow = [&](int t, std::optional<long long> v, std::optional<long long> lo) {
      if (region.count(t)) return;
      if (test_block(t, reg) && all_in_region(t)) {
        region.insert(t);
        walk(t, v, false, lo);
      } else {
        leaves.insert(t);
        const Block &bt = at(blocks, t);
        int n = bt.hi - bt.lo;
        std::optional<std::pair<std::string, long long>> c_;
        if (n) c_ = cmp_of(bins(bt, 0), reg);
        if (c_ && n > 1 && !in(CJMP, bins(bt, 1).mn) && all_in_region(t)) cases.push_back({c_->second, t});
      }
    };
    walk(h.id, std::nullopt, true, std::nullopt);
    std::set<long long> vals;
    for (auto &c : cases) vals.insert(c.first);
    auto I3 = [&](int k) -> const Ins & { return bins(h, k); };
    bool loaded = (reg == "ax" || reg == "al") && hn >= 3 && I3(-3).mn == "mov" && starts(I3(-3).op, reg + ", ") &&
                  I3(-3).op.find('[') != std::string::npos;
    loaded = loaded || (reg == "ax" && hn >= 3 && I3(-3).mn == "mov" && (I3(-3).op == "si, ax" || I3(-3).op == "di, ax"));
    loaded = loaded || (reg == "ax" && hn >= 4 && (I3(-3).mn == "cbw" || I3(-3).mn == "cwde") && I3(-4).mn == "mov" &&
                        starts(I3(-4).op, "al, ") && I3(-4).op.find('[') != std::string::npos);
    bool computed = false;
    if ((reg == "ax" || reg == "al") && hn >= 3) {
      static const std::set<std::string> M = {"and", "or", "xor", "add", "sub", "shl", "shr", "sar"};
      std::string d = I3(-3).op.substr(0, I3(-3).op.find(','));
      computed = in(M, I3(-3).mn) && (d == "ax" || d == "al");
    }
    if (loaded && vals.size() == 1 && *vals.begin() == 0 && (I3(-2).mn == "or" || I3(-2).mn == "test")) loaded = false;
    size_t need = loaded ? 1 : computed ? 2 : 3;
    if (!ok || leaves.size() != 1 || vals.size() < need || vals.size() != cases.size()) continue;
    int deflt = *leaves.begin();
    auto through = [&](int t) {
      const Block &blk = at(blocks, t);
      if (blk.hi - blk.lo == 1 && blk.term == "jmp" && all_in_region(t)) {
        region.insert(t);
        return at(blk.succ, 0);
      }
      return t;
    };
    for (auto &c : cases) c.second = through(c.second);
    deflt = through(deflt);
    h.term = "switch";
    h.cases = cases;
    h.has_cases = true;
    h.def = deflt;
    h.selreg = reg;
    h.has_table = false;
    h.table.clear();
    std::vector<int> s;
    for (auto &c : cases) s.push_back(c.second);
    s.push_back(deflt);
    h.succ = sorted_unique(s);
    for (int r : region)
      if (r != h.id) {
        blocks[(size_t)r].succ.clear();
        blocks[(size_t)r].term = "dead";
      }
  }
}

}  // namespace

std::vector<Block> build_cfg(const std::vector<Ins> &insns, const std::map<long long, std::vector<long long>> &sw,
                             bool cmpswitch) {
  g_insns = &insns;
  std::unordered_map<long long, int> addr2idx;
  int n = (int)insns.size();
  for (int i = 0; i < n; i++) addr2idx[insns[i].addr] = i;
  auto switch_targets = [&](const Ins &ins) -> const std::vector<long long> * {
    if (ins.mn != "jmp" || ins.op.find('[') == std::string::npos) return nullptr;
    auto it = sw.find(ins.addr);
    return it == sw.end() ? nullptr : &it->second;
  };
  std::set<int> leaders = {0};
  for (int i = 0; i < n; i++) {
    const Ins &ins = insns[i];
    if (in(CJMP, ins.mn) || ins.mn == "jmp") {
      auto t = jump_target(ins);
      if (auto st = switch_targets(ins))
        for (long long a : *st) {
          auto it = addr2idx.find(a);
          if (it != addr2idx.end()) leaders.insert(it->second);
        }
      if (t) {
        auto it = addr2idx.find(*t);
        if (it != addr2idx.end()) leaders.insert(it->second);
      }
      if (i + 1 < n) leaders.insert(i + 1);
    } else if (in(RET, ins.mn)) {
      if (i + 1 < n) leaders.insert(i + 1);
    }
  }
  std::vector<int> leads(leaders.begin(), leaders.end());
  std::vector<Block> blocks;
  std::vector<int> idx2blk((size_t)n, NONE);
  for (size_t bi = 0; bi < leads.size(); bi++) {
    int start = leads[bi];
    int end = bi + 1 < leads.size() ? leads[bi + 1] : n;
    if (start >= n) index_error();                           // insns[start].address
    Block b;
    b.id = (int)bi;
    b.lo = start;
    b.hi = end;
    blocks.push_back(b);
    for (int k = start; k < end; k++) idx2blk[(size_t)k] = (int)bi;
  }
  for (auto &b : blocks) {
    const Ins &last = at(insns, b.hi - 1);
    const std::string &mn = last.mn;
    if (in(RET, mn)) {
      b.term = "ret";
      b.succ.clear();
    } else if (mn == "jmp") {
      auto t = jump_target(last);
      auto sw_ = switch_targets(last);
      bool all = sw_ && !sw_->empty();
      if (all)
        for (long long x : *sw_)
          if (!addr2idx.count(x)) all = false;
      if (t && addr2idx.count(*t)) {
        b.term = "jmp";
        b.succ = {idx2blk[(size_t)addr2idx[*t]]};
      } else if (all) {
        b.term = "switch";
        b.has_table = true;
        for (long long x : *sw_) b.table.push_back(idx2blk[(size_t)addr2idx[x]]);
        for (int x : b.table)
          if (std::find(b.succ.begin(), b.succ.end(), x) == b.succ.end()) b.succ.push_back(x);
      } else {
        b.term = "tailcall";
        b.succ.clear();
      }
    } else if (in(CJMP, mn)) {
      auto t = jump_target(last);
      int fall = b.hi < n ? idx2blk[(size_t)b.hi] : NONE;
      int taken = (t && addr2idx.count(*t)) ? idx2blk[(size_t)addr2idx[*t]] : NONE;
      b.term = "cond";
      b.cond_idx = b.hi - 1;
      b.succ.clear();
      if (taken != NONE) b.succ.push_back(taken);
      if (fall != NONE) b.succ.push_back(fall);
    } else {
      int fall = b.hi < n ? idx2blk[(size_t)b.hi] : NONE;
      b.term = "fall";
      b.succ.clear();
      if (fall != NONE) b.succ.push_back(fall);
    }
  }
  std::vector<char> seen(blocks.size(), 0);
  std::vector<int> work = {0};
  seen[0] = 1;
  while (!work.empty()) {
    int x = work.back();
    work.pop_back();
    for (int t : blocks[(size_t)x].succ)
      if (!seen[(size_t)t]) {
        seen[(size_t)t] = 1;
        work.push_back(t);
      }
  }
  for (auto &blk : blocks)
    if (!seen[(size_t)blk.id]) {
      blk.succ.clear();
      blk.term = "dead";
    }
  if (cmpswitch) {
    compare_switches(blocks);
    table_switches(blocks);
  }
  return blocks;
}

// ---------------------------------------------------------------- the structure tree

namespace {

struct Bits {                                                // a set of ids 0..n
  std::vector<uint64_t> w;
  explicit Bits(size_t n = 0, bool full = false) : w((n + 63) / 64, full ? ~0ULL : 0) {
    if (full && n % 64) w.back() = (1ULL << (n % 64)) - 1;
  }
  bool has(int i) const { return (w[(size_t)i >> 6] >> (i & 63)) & 1; }
  void add(int i) { w[(size_t)i >> 6] |= 1ULL << (i & 63); }
  void del(int i) { w[(size_t)i >> 6] &= ~(1ULL << (i & 63)); }
  void and_(const Bits &o) {
    for (size_t k = 0; k < w.size(); k++) w[k] &= o.w[k];
  }
  bool operator!=(const Bits &o) const { return w != o.w; }
  bool empty() const {
    for (auto x : w)
      if (x) return false;
    return true;
  }
  bool subset_of(const Bits &o) const {
    for (size_t k = 0; k < w.size(); k++)
      if (w[k] & ~o.w[k]) return false;
    return true;
  }
};

struct Structurer {
  const std::vector<Block> &blocks;
  bool nocap;
  int EXIT;
  std::vector<Bits> dom, pdom;
  std::vector<std::vector<int>> preds;
  std::map<int, std::vector<char>> headers;                  // header -> the loop's body (by id)
  std::map<int, int> latch;
  std::vector<char> emitted, active;
  std::map<int, bool> big_;
  struct LoopE {
    int h, fol;
    const std::vector<char> *body;
    std::shared_ptr<bool> flag;
  };
  std::vector<LoopE> loopstack;
  struct CaseE {
    std::set<int> arms;
    int depth, join;
  };
  std::vector<CaseE> casestack;

  Structurer(const std::vector<Block> &b, bool nc) : blocks(b), nocap(nc) {}

  const Block &B(int i) const { return at(blocks, i); }
  int size(int i) const { return B(i).hi - B(i).lo; }
  static bool inbody(const std::vector<char> *body, int x) { return x >= 0 && x < (int)body->size() && (*body)[(size_t)x]; }

  void analyse(int entry) {
    size_t n = blocks.size();
    preds.assign(n, {});
    for (auto &b : blocks)
      for (int s : b.succ) at(preds, s).push_back(b.id);
    // dominators
    dom.assign(n, Bits(n, true));
    if (entry < 0 || entry >= (int)n) throw PyError{3, "KeyError"};
    dom[(size_t)entry] = Bits(n);
    dom[(size_t)entry].add(entry);
    for (bool changed = true; changed;) {
      changed = false;
      for (auto &b : blocks) {
        if (b.id == entry) continue;
        Bits nw(n, true);
        for (int p : preds[(size_t)b.id]) nw.and_(dom[(size_t)p]);
        nw.add(b.id);
        if (nw != dom[(size_t)b.id]) {
          dom[(size_t)b.id] = nw;
          changed = true;
        }
      }
    }
    // post-dominators, from a virtual exit = n
    EXIT = (int)n;
    pdom.assign(n + 1, Bits(n + 1, true));
    pdom[n] = Bits(n + 1);
    pdom[n].add(EXIT);
    for (bool changed = true; changed;) {
      changed = false;
      for (size_t i = 0; i < n; i++) {
        Bits nw(n + 1, true);
        for (int s : blocks[i].succ) nw.and_(pdom[(size_t)s]);
        if (blocks[i].succ.empty()) nw.and_(pdom[n]);
        nw.add((int)i);
        if (nw != pdom[i]) {
          pdom[i] = nw;
          changed = true;
        }
      }
    }
    // back edges, natural loops (several back edges to one header: one loop), latches
    std::vector<std::pair<int, int>> be;
    for (auto &b : blocks)
      for (int s : b.succ)
        if (dom[(size_t)b.id].has(s)) be.push_back({b.id, s});
    for (auto &e : be) {
      int t = e.first, h = e.second;
      auto &body = headers[h];
      if (body.empty()) body.assign(n, 0);
      std::vector<char> loop(n, 0);
      loop[(size_t)h] = loop[(size_t)t] = 1;
      std::vector<int> stack = {t};
      while (!stack.empty()) {
        int x = stack.back();
        stack.pop_back();
        for (int p : preds[(size_t)x])
          if (!loop[(size_t)p]) {
            loop[(size_t)p] = 1;
            stack.push_back(p);
          }
      }
      for (size_t k = 0; k < n; k++)
        if (loop[k]) body[k] = 1;
    }
    for (auto &e : be) latch.emplace(e.second, e.first);
    emitted.assign(n + 1, 0);
    active.assign(n + 1, 0);
  }

  int ipdom(int bid) const {
    Bits cand = pdom[(size_t)bid];
    cand.del(bid);
    if (cand.empty()) return NONE;
    for (int ip = 0; ip <= EXIT; ip++)
      if (cand.has(ip) && cand.subset_of(pdom[(size_t)ip])) return ip == EXIT ? NONE : ip;
    return NONE;
  }

  bool short_to(int b, const std::set<int> &stops) {
    if (stops.empty() || casestack.empty()) return false;
    int n = 0, c = b;
    while (!stops.count(c)) {
      const Block &blk = B(c);
      if ((blk.term != "fall" && blk.term != "jmp") || blk.succ.size() != 1 || n >= 3) return false;
      n++;
      c = blk.succ[0];
    }
    return 0 < n && size(b) <= 8;
  }

  bool big(int b) {
    auto it = big_.find(b);
    if (it != big_.end()) return it->second;
    std::vector<int> seen = {b}, work = {b};
    auto has = [&](int x) { return std::find(seen.begin(), seen.end(), x) != seen.end(); };
    while (!work.empty() && seen.size() <= 5) {
      int x = work.back();
      work.pop_back();
      for (int t : B(x).succ)
        if (!has(t)) {
          seen.push_back(t);
          work.push_back(t);
        }
    }
    long long sum = 0;
    for (int x : seen) sum += size(x);
    bool r = seen.size() > 5 || sum > 24;
    big_[b] = r;
    return r;
  }

  bool ends_terminal(const Seq &nodes) const {
    if (nodes.empty()) return false;
    const Node &n = nodes.back();
    switch (n.kind) {
      case Node::BREAK: case Node::CONTINUE: case Node::GOTO: return true;
      case Node::BLOCK: return B(n.a).term == "ret" || B(n.a).term == "tailcall";
      case Node::IFELSE: return ends_terminal(n.s1) && ends_terminal(n.s2);
      case Node::SWITCH:
        if (n.join != NONE) return false;
        for (auto &c : n.cases)
          if (!ends_terminal(c.second)) return false;
        return true;
      default: return false;
    }
  }

  static Node leaf(Node::Kind k, int a = NONE) {
    Node x;
    x.kind = k;
    x.a = a;
    return x;
  }
  static Node go(int b) { return leaf(Node::GOTO, b); }

  Seq seq_from(int b, const std::set<int> &stops) {
    Seq out;
    std::set<int> local;
    while (b != NONE && b != EXIT) {
      if (!loopstack.empty()) {
        LoopE &L = loopstack.back();
        int depth = (int)loopstack.size();
        if (!casestack.empty() && casestack.back().depth == depth && casestack.back().join == b) return out;
        if (b == L.fol && !casestack.empty() && casestack.back().depth == depth) {
          out.push_back(B(b).term == "ret" ? leaf(Node::BLOCK, b) : go(b));
          return out;
        }
        if (b == L.fol) {
          out.push_back(leaf(Node::BREAK));
          return out;
        }
        if (b == L.h) {
          out.push_back(leaf(Node::CONTINUE));
          return out;
        }
        if (!inbody(L.body, b) && !*L.flag) {
          std::vector<int> chain;
          int c = b;
          while (c != NONE && chain.size() < 4) {
            chain.push_back(c);
            if (B(c).term == "ret") break;
            c = ((B(c).term == "jmp" || B(c).term == "fall") && !B(c).succ.empty()) ? B(c).succ[0] : NONE;
          }
          if (c != NONE && B(c).term == "ret" && chain.back() == c) {
            for (int x : chain) out.push_back(leaf(Node::BLOCK, x));
            return out;
          }
          std::shared_ptr<bool> flag = L.flag;
          *flag = true;
          Seq sub;
          try {
            sub = seq_from(b, stops);
          } catch (...) {
            *flag = false;
            throw;
          }
          *flag = false;
          if (ends_terminal(sub))
            out.insert(out.end(), sub.begin(), sub.end());
          else
            out.push_back(go(b));
          return out;
        }
      }
      if (stops.count(b)) {
        if (!casestack.empty() && casestack.back().arms.count(b)) out.push_back(go(b));
        break;
      }
      if (active[(size_t)b] || local.count(b)) {
        out.push_back(go(b));
        break;
      }
      if (emitted[(size_t)b] && big(b) && !nocap && !short_to(b, stops)) {
        out.push_back(go(b));
        break;
      }
      emitted[(size_t)b] = 1;
      local.insert(b);
      if (headers.count(b)) {
        active[(size_t)b] = 1;
        auto r = do_loop(b);
        active[(size_t)b] = 0;
        out.push_back(std::move(r.first));
        b = r.second;
        continue;
      }
      const Block &blk = B(b);
      if (blk.term == "cond") {
        active[(size_t)b] = 1;
        auto r = do_if(b, stops);
        active[(size_t)b] = 0;
        out.push_back(std::move(r.first));
        b = r.second;
        continue;
      }
      if (blk.term == "switch") {
        active[(size_t)b] = 1;
        auto r = do_switch(b, stops);
        active[(size_t)b] = 0;
        out.push_back(std::move(r.first));
        b = r.second;
        continue;
      }
      out.push_back(leaf(Node::BLOCK, b));
      b = blk.succ.empty() ? NONE : blk.succ[0];
    }
    return out;
  }

  static std::set<int> plus(const std::set<int> &s, int x) {
    std::set<int> r = s;
    if (x != NONE) r.insert(x);
    return r;
  }

  std::pair<Node, int> do_switch(int b, const std::set<int> &stops) {
    const Block &blk = B(b);
    int join = ipdom(b);
    if (!loopstack.empty() && join != NONE) {
      LoopE &L = loopstack.back();
      if (!inbody(L.body, join) && join != L.fol) join = NONE;
    }
    if (!loopstack.empty() && loopstack.back().h == b && (join == NONE || !inbody(loopstack.back().body, join))) {
      const std::vector<char> *lbody = loopstack.back().body;
      auto reach = [&](int t) {
        std::set<int> seen_ = {t};
        std::vector<int> work_ = {t};
        while (!work_.empty()) {
          int x0 = work_.back();
          work_.pop_back();
          for (int x : B(x0).succ)
            if (inbody(lbody, x) && x != b && !seen_.count(x)) {
              seen_.insert(x);
              work_.push_back(x);
            }
        }
        return seen_;
      };
      std::vector<std::set<int>> stay;
      for (int t : blk.succ)
        if (inbody(lbody, t) && t != b) stay.push_back(reach(t));
      std::set<int> common;
      if (!stay.empty()) {
        common = stay[0];
        for (size_t k = 1; k < stay.size(); k++) {
          std::set<int> r;
          for (int x : common)
            if (stay[k].count(x)) r.insert(x);
          common = r;
        }
      }
      std::vector<int> firsts;
      for (int j : common) {
        bool all = true;
        std::set<int> rj;
        bool have = false;
        for (int k : common) {
          if (j == k) continue;
          if (!have) {
            rj = reach(j);
            have = true;
          }
          if (!rj.count(k)) {
            all = false;
            break;
          }
        }
        if (all) firsts.push_back(j);
      }
      if (firsts.size() == 1) join = firsts[0];
    }
    std::set<int> sub = plus(stops, join);
    Node node = leaf(Node::SWITCH, b);
    for (int t : blk.succ) {
      if (blk.has_cases) {
        std::vector<long long> vs;
        for (auto &c : blk.cases)
          if (c.second == t) vs.push_back(c.first);
        std::sort(vs.begin(), vs.end());
        std::vector<CaseVal> vals;
        for (long long v : vs) vals.push_back({false, v});
        if (t == blk.def) vals.push_back({true, 0});
        if (t == join && vals.size() == 1 && vals[0].is_default) continue;
        std::set<int> arms(blk.succ.begin(), blk.succ.end());
        arms.erase(t);
        if (join != NONE) arms.erase(join);
        casestack.push_back({arms, (int)loopstack.size(), join});
        Seq arm;
        try {
          if (t != join) {
            std::set<int> s2 = sub;
            s2.insert(arms.begin(), arms.end());
            arm = seq_from(t, s2);
          }
        } catch (...) {
          casestack.pop_back();
          throw;
        }
        casestack.pop_back();
        node.cases.push_back({vals, arm});
        continue;
      }
      std::vector<CaseVal> vals;
      for (size_t i = 0; i < blk.table.size(); i++)
        if (blk.table[i] == t) vals.push_back({false, (long long)i});
      node.cases.push_back({vals, t != join ? seq_from(t, sub) : Seq()});
    }
    node.join = join;
    return {node, join};
  }

  std::pair<Node, int> do_if(int b, const std::set<int> &stops) {
    const Block &blk = B(b);
    int join = ipdom(b);
    if (!loopstack.empty() && join != NONE) {
      if (!inbody(loopstack.back().body, join)) join = NONE;
    }
    int taken = at(blk.succ, 0);
    int fall = blk.succ.size() > 1 ? blk.succ[1] : NONE;
    int then_s = fall, else_s = taken;
    std::set<int> sub = plus(stops, join);
    Node node;
    node.a = b;
    if (join != NONE && else_s == join) {
      node.kind = Node::IF;
      node.s1 = seq_from(then_s, sub);
      node.join = join;
      node.flag = false;
      return {node, join};
    }
    if (join != NONE && then_s == join) {
      node.kind = Node::IF;
      node.s1 = seq_from(else_s, sub);
      node.join = join;
      node.flag = true;
      return {node, join};
    }
    node.kind = Node::IFELSE;
    if (join != NONE) {
      node.s1 = seq_from(then_s, sub);
      node.s2 = seq_from(else_s, sub);
      node.join = join;
      return {node, join};
    }
    node.s1 = seq_from(then_s, stops);
    node.s2 = seq_from(else_s, stops);
    node.join = NONE;
    return {node, NONE};
  }

  std::pair<Node, int> do_loop(int h) {
    const std::vector<char> *body = &headers[h];
    const Block &hblk = B(h);
    std::set<int> exits;
    for (size_t nn = 0; nn < body->size(); nn++)
      if ((*body)[nn])
        for (int s : blocks[nn].succ)
          if (!inbody(body, s)) exits.insert(s);
    int follow = exits.size() == 1 ? *exits.begin() : NONE;
    auto li = latch.find(h);
    int lb = li == latch.end() ? NONE : li->second;
    bool selfloop = std::find(hblk.succ.begin(), hblk.succ.end(), h) != hblk.succ.end();
    bool hexit = false;
    for (int s : hblk.succ)
      if (!inbody(body, s)) hexit = true;
    Node node;
    node.a = h;
    if (hblk.term == "cond" && hexit && !selfloop) {
      std::vector<int> inside;
      int fol = NONE;
      for (int s : hblk.succ)
        if (inbody(body, s)) inside.push_back(s);
      for (int s : hblk.succ)
        if (!inbody(body, s)) {
          fol = s;
          break;
        }
      loopstack.push_back({h, fol, body, std::make_shared<bool>(false)});
      node.s1 = inside.empty() ? Seq() : seq_from(inside[0], {h});
      loopstack.pop_back();
      node.kind = Node::WHILE;
      node.join = fol;
      node.flag = !inside.empty() && inside[0] == hblk.succ[0];
      return {node, fol};
    }
    if (lb != NONE && B(lb).term == "cond") {
      bool lexit = false;
      int fol = NONE;
      for (int s : B(lb).succ)
        if (!inbody(body, s)) {
          if (!lexit) fol = s;
          lexit = true;
        }
      if (lexit) {
        loopstack.push_back({h, fol, body, std::make_shared<bool>(false)});
        Seq bt;
        if (h == lb) {
        } else if (hblk.term == "cond") {
          active[(size_t)h] = 1;
          auto r = do_if(h, {lb});
          active[(size_t)h] = 0;
          bt.push_back(std::move(r.first));
          int after = r.second;
          if (after != NONE && after != h && after != lb && after != EXIT) {
            Seq more = seq_from(after, {lb});
            bt.insert(bt.end(), more.begin(), more.end());
          }
        } else {
          bt.push_back(leaf(Node::BLOCK, h));
          if (!hblk.succ.empty() && hblk.succ[0] != lb) {
            Seq more = seq_from(hblk.succ[0], {lb});
            bt.insert(bt.end(), more.begin(), more.end());
          }
        }
        bt.push_back(leaf(Node::BLOCK, lb));
        loopstack.pop_back();
        node.kind = Node::DOWHILE;
        node.s1 = bt;
        node.join = lb;
        node.flag = inbody(body, at(B(lb).succ, 0));
        return {node, fol};
      }
    }
    loopstack.push_back({h, follow, body, std::make_shared<bool>(false)});
    Seq bt;
    if (hblk.term == "switch" || hblk.term == "cond") {
      auto r = hblk.term == "switch" ? do_switch(h, {h}) : do_if(h, {h});
      bt.push_back(std::move(r.first));
      int after = r.second;
      if (after != NONE && after != h && after != EXIT) {
        Seq more = seq_from(after, {h});
        bt.insert(bt.end(), more.begin(), more.end());
      }
    } else {
      bt.push_back(leaf(Node::BLOCK, h));
      if (!hblk.succ.empty()) {
        Seq more = seq_from(hblk.succ[0], {h});
        bt.insert(bt.end(), more.begin(), more.end());
      }
    }
    loopstack.pop_back();
    node.kind = Node::LOOP;
    node.s1 = bt;
    node.join = follow;
    return {node, follow};
  }
};

void j_int(std::string &o, int v) {
  if (v == NONE)
    o += "null";
  else
    o += std::to_string(v);
}

void j_node(std::string &o, const Node &n);
void j_seq(std::string &o, const Seq &s) {
  o += '[';
  for (size_t i = 0; i < s.size(); i++) {
    if (i) o += ',';
    j_node(o, s[i]);
  }
  o += ']';
}
void j_node(std::string &o, const Node &n) {
  auto b = [](bool f) { return f ? "true" : "false"; };
  switch (n.kind) {
    case Node::BLOCK: o += "[\"block\"," + std::to_string(n.a) + "]"; break;
    case Node::BREAK: o += "[\"break\"]"; break;
    case Node::CONTINUE: o += "[\"continue\"]"; break;
    case Node::GOTO: o += "[\"goto\",null," + std::to_string(n.a) + "]"; break;
    case Node::IF:
      o += "[\"if\"," + std::to_string(n.a) + ",";
      j_seq(o, n.s1);
      o += ",";
      j_int(o, n.join);
      o += std::string(",") + b(n.flag) + "]";
      break;
    case Node::IFELSE:
      o += "[\"ifelse\"," + std::to_string(n.a) + ",";
      j_seq(o, n.s1);
      o += ",";
      j_seq(o, n.s2);
      o += ",";
      j_int(o, n.join);
      o += "]";
      break;
    case Node::WHILE:
    case Node::DOWHILE:
      o += n.kind == Node::WHILE ? "[\"while\"," : "[\"dowhile\",";
      o += std::to_string(n.a) + ",";
      j_seq(o, n.s1);
      o += ",";
      j_int(o, n.join);
      o += std::string(",") + b(n.flag) + "]";
      break;
    case Node::LOOP:
      o += "[\"loop\"," + std::to_string(n.a) + ",";
      j_seq(o, n.s1);
      o += ",";
      j_int(o, n.join);
      o += "]";
      break;
    case Node::SWITCH:
      o += "[\"switch\"," + std::to_string(n.a) + ",[";
      for (size_t i = 0; i < n.cases.size(); i++) {
        if (i) o += ',';
        o += "[[";
        auto &vals = n.cases[i].first;
        for (size_t k = 0; k < vals.size(); k++) {
          if (k) o += ',';
          o += vals[k].is_default ? std::string("\"default\"") : std::to_string(vals[k].v);
        }
        o += "],";
        j_seq(o, n.cases[i].second);
        o += "]";
      }
      o += "],";
      j_int(o, n.join);
      o += "]";
      break;
  }
}

void j_ints(std::string &o, const std::vector<int> &v) {
  o += '[';
  for (size_t i = 0; i < v.size(); i++) {
    if (i) o += ',';
    o += std::to_string(v[i]);
  }
  o += ']';
}

}  // namespace

Seq structure(const std::vector<Block> &blocks, int entry, bool nocap) {
  Structurer s(blocks, nocap);
  s.analyse(entry);
  return s.seq_from(entry, {});
}

std::string json_seq(const Seq &s) {
  std::string o;
  j_seq(o, s);
  return o;
}

std::string json_blocks(const std::vector<Block> &blocks) {
  std::string o = "{\"b\":[";
  for (size_t i = 0; i < blocks.size(); i++) {
    const Block &b = blocks[i];
    if (i) o += ',';
    o += "[" + std::to_string(b.lo) + "," + std::to_string(b.hi) + ",\"" + b.term + "\",";
    j_ints(o, b.succ);
    o += ",";
    j_int(o, b.cond_idx);
    o += ",";
    if (b.has_table)
      j_ints(o, b.table);
    else
      o += "null";
    o += ",";
    if (b.has_cases) {
      o += '[';
      for (size_t k = 0; k < b.cases.size(); k++) {
        if (k) o += ',';
        o += "[" + std::to_string(b.cases[k].first) + "," + std::to_string(b.cases[k].second) + "]";
      }
      o += ']';
    } else {
      o += "null";
    }
    o += ",";
    j_int(o, b.def);
    o += ",";
    o += b.selreg.empty() ? std::string("null") : "\"" + b.selreg + "\"";
    o += ",";
    j_int(o, b.selbase);
    o += "]";
  }
  return o + "]}";
}

}  // namespace cbinv
