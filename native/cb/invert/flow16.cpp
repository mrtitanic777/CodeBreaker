#include "cb/invert/flow16.hpp"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

#include "cb/core/error.hpp"
#include "cb/invert/model_text.hpp"

namespace cb::invert::flow16 {

namespace {

using platforms::Insn;
namespace model = cb::invert::model;

// ---- reading instructions -------------------------------------------------------------------------------------------------------------

bool is_conditional_jump(std::string_view m) {
    static constexpr std::string_view kJumps[] = {"jl", "jle", "jg", "jge", "je", "jne", "ja",  "jae", "jb",   "jbe", "jc",
                                                  "jnc", "jz", "jnz", "jo", "jno", "js", "jns", "jp", "jnp", "jcxz", "loop"};
    return std::ranges::find(kJumps, m) != std::end(kJumps);
}
bool is_return(std::string_view m) { return m == "ret" || m == "retf" || m == "iret"; }
bool is_selector_register(std::string_view r) { return r == "ax" || r == "bx" || r == "cx" || r == "dx" || r == "si" || r == "di" || r == "al"; }

bool all_word_chars(std::string_view s) { return !s.empty() && std::ranges::all_of(s, model::is_word_char); }
bool is_hex_literal(std::string_view s) { return s.size() > 2 && s.starts_with("0x") && std::all_of(s.begin() + 2, s.end(), model::is_hex_char); }
bool is_constant(const std::string& s) { return is_hex_literal(s) || model::is_digits(s); }

// The target of a jump whose operand is a plain number.
std::optional<long long> jump_target(const Insn& i) {
    if (!model::is_number(i.op_str)) return std::nullopt;
    return model::parse_number(i.op_str);
}

struct Compared {
    std::string reg;
    long long value;
};

// `cmp reg, constant`, or a register tested against itself (`or ax, ax`, `test ax, ax`: against 0). `only`: that register alone.
std::optional<Compared> compared(const Insn& i, std::string_view only = {}) {
    const std::string& op = i.op_str;
    const std::size_t comma = op.find(", ");
    if (comma == std::string::npos) return std::nullopt;
    const std::string left = op.substr(0, comma), right = op.substr(comma + 2);
    if (i.mnemonic == "or" || i.mnemonic == "test" || i.mnemonic == "and") {
        if (left == right && all_word_chars(left)) {
            if (is_selector_register(left) && (only.empty() || left == only)) return Compared{left, 0};
            return std::nullopt;
        }
    }
    if (i.mnemonic != "cmp") return std::nullopt;
    if (!all_word_chars(left) || !is_constant(right)) return std::nullopt;
    if (!is_selector_register(left) || (!only.empty() && left != only)) return std::nullopt;
    return Compared{left, model::parse_number(right)};
}

std::vector<int> sorted_unique(std::vector<int> v) {
    std::ranges::sort(v);
    v.erase(std::unique(v.begin(), v.end()), v.end());
    return v;
}

std::vector<std::vector<int>> predecessors(const std::vector<Block>& blocks) {
    std::vector<std::vector<int>> preds(blocks.size());
    for (const Block& b : blocks)
        for (const int t : b.successors) preds[static_cast<std::size_t>(t)].push_back(b.id);
    return preds;
}

// The k-th successor of a block; a block that has none there is a function whose flow leaves the code.
int successor_at(const Block& b, std::size_t k) {
    if (k >= b.successors.size()) model::fail("a branch that leaves the function");
    return b.successors[k];
}

void make_dead(Block& b) {
    b.successors.clear();
    b.end = End::Dead;
}

// ---- the switches ---------------------------------------------------------------------------------------------------------------------

// A function's blocks and instructions, for the passes that read instruction text.
struct Context {
    std::vector<Block>& blocks;
    std::span<const Insn> instructions;

    // The k-th instruction of a block; a negative k counts from its end.
    const Insn& insn(const Block& b, long long k) const {
        const long long n = static_cast<long long>(b.size());
        if (k < 0) k += n;
        if (k < 0 || k >= n) model::fail("a block shorter than the switch pattern");
        return instructions[b.first + static_cast<std::size_t>(k)];
    }
};

// A jump table behind its bounds test: `[sub reg, base;] cmp reg, n; ja default; ... jmp [table + reg*2]`, the test in the one block that reaches
// the table's block. The two become one switch block.
void join_table_switches(Context& cx) {
    auto& blocks = cx.blocks;
    const auto preds = predecessors(blocks);
    const auto through_jump = [&](int id) {
        const Block& x = blocks[static_cast<std::size_t>(id)];
        return x.end == End::Jump && x.size() == 1 ? successor_at(x, 0) : id;
    };
    for (std::size_t di = 0; di < blocks.size(); ++di) {
        Block& dispatch = blocks[di];
        if (dispatch.end != End::Switch || !dispatch.table || preds[static_cast<std::size_t>(dispatch.id)].size() != 1) continue;
        Block& head = blocks[static_cast<std::size_t>(preds[static_cast<std::size_t>(dispatch.id)][0])];
        const std::size_t head_size = head.size();
        if (head.end != End::Branch || head_size < 2) continue;
        const Insn& bound = cx.insn(head, -2);
        const Insn& jump = cx.insn(head, -1);
        const auto test = compared(bound);
        if (!test || (jump.mnemonic != "ja" && jump.mnemonic != "jbe")) continue;
        const std::string& reg = test->reg;
        const long long n = test->value;
        const int taken = successor_at(head, 0), fall = successor_at(head, 1);
        const int other = taken == dispatch.id ? fall : taken;
        if ((dispatch.id != taken && dispatch.id != fall) || static_cast<long long>(dispatch.table->size()) != n + 1) continue;
        const int default_block = through_jump(other);
        long long base = 0;
        long long cut = static_cast<long long>(head_size) - 2;
        if (head_size >= 3) {
            const Insn& s = cx.insn(head, -3);
            if (s.op_str.starts_with(reg + ", ")) {
                const std::string rest = s.op_str.substr(reg.size() + 2);
                if (is_constant(rest) && s.mnemonic == "sub") {
                    base = model::parse_number(rest);
                    cut = static_cast<long long>(head_size) - 3;
                }
            }
        }
        head.end = End::Switch;
        head.cases.emplace();
        for (std::size_t k = 0; k < dispatch.table->size(); ++k)
            if ((*dispatch.table)[k] != default_block) head.cases->push_back({base + static_cast<long long>(k), (*dispatch.table)[k]});
        head.default_target = default_block;
        head.selector = reg;
        head.selector_end = static_cast<int>(cut);
        head.table.reset();
        std::vector<int> targets;
        for (const Case& c : *head.cases) targets.push_back(c.target);
        targets.push_back(default_block);
        head.successors = sorted_unique(std::move(targets));
        make_dead(dispatch);
        if (other != default_block) make_dead(blocks[static_cast<std::size_t>(other)]);
    }
}

// A switch a compiler lays out as compares: the selector in a register, then blocks that only compare it with a constant and branch.
void join_compare_switches(Context& cx) {
    auto& blocks = cx.blocks;
    const auto preds = predecessors(blocks);
    const auto is_test_block = [&](int id, const std::string& reg) {
        const Block& blk = blocks[static_cast<std::size_t>(id)];
        const std::size_t n = blk.size();
        if (n == 0 || blk.end == End::Dead) return false;
        if (n == 1) return is_conditional_jump(cx.insn(blk, 0).mnemonic) || blk.end == End::Jump;
        return n == 2 && compared(cx.insn(blk, 0), reg) && (is_conditional_jump(cx.insn(blk, 1).mnemonic) || blk.end == End::Jump);
    };
    for (std::size_t hi = 0; hi < blocks.size(); ++hi) {
        Block& head = blocks[hi];
        if (head.end != End::Branch || head.size() < 2 || !compared(cx.insn(head, -2))) continue;
        const std::string reg = compared(cx.insn(head, -2))->reg;
        {
            static constexpr std::string_view kSetsIt[] = {"mov", "add", "sub", "inc", "dec", "pop"};
            const Insn& i = cx.insn(head, -2);
            if (std::ranges::find(kSetsIt, i.mnemonic) != std::end(kSetsIt) && i.op_str.starts_with(reg + ",")) continue;
        }
        std::vector<Case> cases;
        std::set<int> leaves, region{head.id};
        bool ok = true;
        const auto only_region_reaches = [&](int t) {
            return std::ranges::all_of(preds[static_cast<std::size_t>(t)], [&](int p) { return region.count(p) != 0; });
        };
        std::function<void(int, std::optional<long long>, std::optional<long long>)> follow;
        std::function<void(int, std::optional<long long>, bool, std::optional<long long>)> visit =
            [&](int id, std::optional<long long> value, bool first, std::optional<long long> low) {
                const Block& blk = blocks[static_cast<std::size_t>(id)];
                const std::size_t n = blk.size();
                const std::size_t skip = first ? (n > 2 ? n - 2 : 0) : 0;   // the head: only its last two instructions are the test
                const std::size_t shown = n - skip;
                const auto seq = [&](std::size_t k) -> const Insn& {
                    if (k >= shown) model::fail("a test block shorter than its compare");
                    return cx.instructions[blk.first + skip + k];
                };
                std::size_t k = 0;
                if (const auto c = compared(seq(0), reg)) {
                    value = c->value;
                    k = 1;
                }
                const Insn& last = seq(k);
                if (blk.end == End::Jump) {
                    follow(successor_at(blk, 0), value, std::nullopt);
                    return;
                }
                if (!value || blk.successors.size() != 2) {
                    ok = false;
                    return;
                }
                const int taken = blk.successors[0], fall = blk.successors[1];
                const std::string& m = last.mnemonic;
                const bool near_low = low && *value - *low >= 0 && *value - *low <= 16;
                if (m == "je" || m == "jz") {
                    cases.push_back({*value, taken});
                    follow(fall, value, std::nullopt);
                } else if (m == "jne" || m == "jnz") {
                    cases.push_back({*value, fall});
                    follow(taken, value, std::nullopt);
                } else if ((m == "jl" || m == "jb") && k) {
                    follow(taken, value, std::nullopt);
                    follow(fall, value, value);
                } else if ((m == "jle" || m == "jbe") && near_low && !is_test_block(taken, reg)) {
                    for (long long x = *low; x <= *value; ++x) cases.push_back({x, taken});
                    follow(fall, value, std::nullopt);
                } else if ((m == "jg" || m == "ja") && near_low && !is_test_block(fall, reg)) {
                    for (long long x = *low; x <= *value; ++x) cases.push_back({x, fall});
                    follow(taken, value, std::nullopt);
                } else {
                    follow(taken, value, std::nullopt);
                    follow(fall, value, std::nullopt);
                }
            };
        follow = [&](int t, std::optional<long long> value, std::optional<long long> low) {
            if (region.count(t)) return;
            if (is_test_block(t, reg) && only_region_reaches(t)) {
                region.insert(t);
                visit(t, value, false, low);
                return;
            }
            leaves.insert(t);
            const Block& bt = blocks[static_cast<std::size_t>(t)];
            if (bt.size() == 0) return;
            const auto c = compared(cx.insn(bt, 0), reg);
            if (c && bt.size() > 1 && !is_conditional_jump(cx.insn(bt, 1).mnemonic) && only_region_reaches(t)) cases.push_back({c->value, t});
        };
        visit(head.id, std::nullopt, true, std::nullopt);
        std::set<long long> values;
        for (const Case& c : cases) values.insert(c.value);
        const std::size_t head_size = head.size();
        const auto op_before = [&](long long k) -> const Insn& { return cx.insn(head, k); };
        const bool is_ax = reg == "ax" || reg == "al";
        bool loaded = is_ax && head_size >= 3 && op_before(-3).mnemonic == "mov" && op_before(-3).op_str.starts_with(reg + ", ") &&
                      op_before(-3).op_str.find('[') != std::string::npos;
        loaded = loaded || (reg == "ax" && head_size >= 3 && op_before(-3).mnemonic == "mov" &&
                            (op_before(-3).op_str == "si, ax" || op_before(-3).op_str == "di, ax"));
        loaded = loaded || (reg == "ax" && head_size >= 4 && (op_before(-3).mnemonic == "cbw" || op_before(-3).mnemonic == "cwde") &&
                            op_before(-4).mnemonic == "mov" && op_before(-4).op_str.starts_with("al, ") &&
                            op_before(-4).op_str.find('[') != std::string::npos);
        bool computed = false;
        if (is_ax && head_size >= 3) {
            static constexpr std::string_view kComputes[] = {"and", "or", "xor", "add", "sub", "shl", "shr", "sar"};
            const std::string dest = op_before(-3).op_str.substr(0, op_before(-3).op_str.find(','));
            computed = std::ranges::find(kComputes, op_before(-3).mnemonic) != std::end(kComputes) && (dest == "ax" || dest == "al");
        }
        if (loaded && values.size() == 1 && *values.begin() == 0 && (op_before(-2).mnemonic == "or" || op_before(-2).mnemonic == "test")) loaded = false;
        const std::size_t need = loaded ? 1 : computed ? 2 : 3;
        if (!ok || leaves.size() != 1 || values.size() < need || values.size() != cases.size()) continue;
        int default_block = *leaves.begin();
        const auto through_jump = [&](int t) {
            const Block& blk = blocks[static_cast<std::size_t>(t)];
            if (blk.size() == 1 && blk.end == End::Jump && only_region_reaches(t)) {
                region.insert(t);
                return successor_at(blk, 0);
            }
            return t;
        };
        for (Case& c : cases) c.target = through_jump(c.target);
        default_block = through_jump(default_block);
        head.end = End::Switch;
        head.cases = cases;
        head.default_target = default_block;
        head.selector = reg;
        head.table.reset();
        std::vector<int> targets;
        for (const Case& c : cases) targets.push_back(c.target);
        targets.push_back(default_block);
        head.successors = sorted_unique(std::move(targets));
        for (const int r : region)
            if (r != head.id) make_dead(blocks[static_cast<std::size_t>(r)]);
    }
}

}  // namespace

// ---- the graph ------------------------------------------------------------------------------------------------------------------------

Cfg build_cfg(std::span<const Insn> insns, const Options& options) {
    if (insns.empty()) throw UsageError("flow16::build_cfg: no instructions");
    const std::size_t n = insns.size();
    std::unordered_map<long long, std::size_t> index_of;
    for (std::size_t i = 0; i < n; ++i) index_of[static_cast<long long>(insns[i].address)] = i;
    // The table of an indirect `jmp [..]`, if the caller knows it.
    std::map<std::size_t, std::vector<long long>> tables;
    if (options.switch_targets) {
        for (std::size_t i = 0; i < n; ++i) {
            if (insns[i].mnemonic != "jmp" || insns[i].op_str.find('[') == std::string::npos) continue;
            if (auto t = options.switch_targets(static_cast<long long>(insns[i].address))) tables.emplace(i, std::move(*t));
        }
    }

    std::set<std::size_t> leaders{0};
    for (std::size_t i = 0; i < n; ++i) {
        const Insn& ins = insns[i];
        if (is_conditional_jump(ins.mnemonic) || ins.mnemonic == "jmp") {
            if (const auto t = tables.find(i); t != tables.end())
                for (const long long a : t->second)
                    if (const auto at = index_of.find(a); at != index_of.end()) leaders.insert(at->second);
            if (const auto t = jump_target(ins))
                if (const auto at = index_of.find(*t); at != index_of.end()) leaders.insert(at->second);
            if (i + 1 < n) leaders.insert(i + 1);
        } else if (is_return(ins.mnemonic) && i + 1 < n) {
            leaders.insert(i + 1);
        }
    }

    Cfg cfg;
    std::vector<Block>& blocks = cfg.blocks;
    const std::vector<std::size_t> starts(leaders.begin(), leaders.end());
    std::vector<int> block_of(n, kNone);
    for (std::size_t bi = 0; bi < starts.size(); ++bi) {
        Block b;
        b.id = static_cast<int>(bi);
        b.first = starts[bi];
        b.last = bi + 1 < starts.size() ? starts[bi + 1] : n;
        blocks.push_back(b);
        for (std::size_t k = b.first; k < b.last; ++k) block_of[k] = b.id;
    }
    for (Block& b : blocks) {
        const Insn& last = insns[b.last - 1];
        const std::string& m = last.mnemonic;
        const int fall = b.last < n ? block_of[b.last] : kNone;
        if (is_return(m)) {
            b.end = End::Return;
        } else if (m == "jmp") {
            const auto target = jump_target(last);
            const auto table = tables.find(b.last - 1);
            bool table_known = table != tables.end() && !table->second.empty();
            if (table_known)
                for (const long long x : table->second)
                    if (!index_of.count(x)) table_known = false;
            if (target && index_of.count(*target)) {
                b.end = End::Jump;
                b.successors = {block_of[index_of[*target]]};
            } else if (table_known) {
                b.end = End::Switch;
                b.table.emplace();
                for (const long long x : table->second) b.table->push_back(block_of[index_of[x]]);
                for (const int x : *b.table)
                    if (std::ranges::find(b.successors, x) == b.successors.end()) b.successors.push_back(x);
            } else {
                b.end = End::TailCall;
            }
        } else if (is_conditional_jump(m)) {
            const auto target = jump_target(last);
            b.end = End::Branch;
            b.branch = b.last - 1;
            if (target && index_of.count(*target)) b.successors.push_back(block_of[index_of[*target]]);
            if (fall != kNone) b.successors.push_back(fall);
        } else {
            b.end = End::Fall;
            if (fall != kNone) b.successors.push_back(fall);
        }
    }

    std::vector<bool> seen(blocks.size(), false);
    std::vector<int> work{0};
    seen[0] = true;
    while (!work.empty()) {
        const int x = work.back();
        work.pop_back();
        for (const int t : blocks[static_cast<std::size_t>(x)].successors)
            if (!seen[static_cast<std::size_t>(t)]) {
                seen[static_cast<std::size_t>(t)] = true;
                work.push_back(t);
            }
    }
    for (Block& b : blocks)
        if (!seen[static_cast<std::size_t>(b.id)]) make_dead(b);

    if (options.compare_switches) {
        Context cx{blocks, insns};
        join_compare_switches(cx);
        join_table_switches(cx);
    }
    return cfg;
}

// ---- the structure tree ---------------------------------------------------------------------------------------------------------------

namespace {

// A set of small ids.
class IdSet {
public:
    explicit IdSet(std::size_t n = 0, bool full = false) : words_((n + 63) / 64, full ? ~std::uint64_t{0} : 0) {
        if (full && n % 64) words_.back() = (std::uint64_t{1} << (n % 64)) - 1;
    }
    bool has(int i) const { return (words_[static_cast<std::size_t>(i) >> 6] >> (i & 63)) & 1; }
    void add(int i) { words_[static_cast<std::size_t>(i) >> 6] |= std::uint64_t{1} << (i & 63); }
    void remove(int i) { words_[static_cast<std::size_t>(i) >> 6] &= ~(std::uint64_t{1} << (i & 63)); }
    void intersect(const IdSet& o) {
        for (std::size_t k = 0; k < words_.size(); ++k) words_[k] &= o.words_[k];
    }
    bool operator==(const IdSet& o) const { return words_ == o.words_; }
    bool empty() const { return std::ranges::all_of(words_, [](std::uint64_t w) { return w == 0; }); }
    bool subset_of(const IdSet& o) const {
        for (std::size_t k = 0; k < words_.size(); ++k)
            if (words_[k] & ~o.words_[k]) return false;
        return true;
    }

private:
    std::vector<std::uint64_t> words_;
};

using BlockSet = std::vector<char>;   // by block id

class Structurer {
public:
    Structurer(const Cfg& cfg, bool uncapped) : blocks_(cfg.blocks), uncapped_(uncapped) {}

    Tree run(int entry) {
        analyse(entry);
        return seq_from(entry, {});
    }

private:
    struct LoopContext {
        int header = kNone, follow = kNone;
        const BlockSet* body = nullptr;
        bool escaping = false;   // a block outside the loop is being structured as part of it
    };
    struct CaseContext {
        std::set<int> arms;
        std::size_t depth = 0;
        int join = kNone;
    };

    const Block& block(int id) const { return blocks_.at(static_cast<std::size_t>(id)); }
    static bool in_body(const BlockSet* body, int x) { return x >= 0 && x < static_cast<int>(body->size()) && (*body)[static_cast<std::size_t>(x)]; }

    static Node leaf(Node::Kind kind, int block_id = kNone) {
        Node n;
        n.kind = kind;
        n.block = block_id;
        return n;
    }
    static Node goto_of(int target) { return leaf(Node::Kind::Goto, target); }
    static std::set<int> with(const std::set<int>& s, int x) {
        std::set<int> r = s;
        if (x != kNone) r.insert(x);
        return r;
    }

    // dominators, post-dominators (from a virtual exit), the natural loops (several back edges to one header make one loop), their latches
    void analyse(int entry) {
        const std::size_t n = blocks_.size();
        if (entry < 0 || static_cast<std::size_t>(entry) >= n) throw UsageError("flow16::structure: no such entry block");
        preds_ = predecessors_of();
        dom_.assign(n, IdSet(n, true));
        dom_[static_cast<std::size_t>(entry)] = IdSet(n);
        dom_[static_cast<std::size_t>(entry)].add(entry);
        for (bool changed = true; changed;) {
            changed = false;
            for (const Block& b : blocks_) {
                if (b.id == entry) continue;
                IdSet next(n, true);
                for (const int p : preds_[static_cast<std::size_t>(b.id)]) next.intersect(dom_[static_cast<std::size_t>(p)]);
                next.add(b.id);
                if (!(next == dom_[static_cast<std::size_t>(b.id)])) {
                    dom_[static_cast<std::size_t>(b.id)] = next;
                    changed = true;
                }
            }
        }
        exit_ = static_cast<int>(n);
        pdom_.assign(n + 1, IdSet(n + 1, true));
        pdom_[n] = IdSet(n + 1);
        pdom_[n].add(exit_);
        for (bool changed = true; changed;) {
            changed = false;
            for (std::size_t i = 0; i < n; ++i) {
                IdSet next(n + 1, true);
                for (const int s : blocks_[i].successors) next.intersect(pdom_[static_cast<std::size_t>(s)]);
                if (blocks_[i].successors.empty()) next.intersect(pdom_[n]);
                next.add(static_cast<int>(i));
                if (!(next == pdom_[i])) {
                    pdom_[i] = next;
                    changed = true;
                }
            }
        }
        std::vector<std::pair<int, int>> back_edges;   // (tail, header)
        for (const Block& b : blocks_)
            for (const int s : b.successors)
                if (dom_[static_cast<std::size_t>(b.id)].has(s)) back_edges.emplace_back(b.id, s);
        for (const auto& [tail, header] : back_edges) {
            BlockSet& body = loops_[header];
            if (body.empty()) body.assign(n, 0);
            std::vector<char> loop(n, 0);
            loop[static_cast<std::size_t>(header)] = loop[static_cast<std::size_t>(tail)] = 1;
            std::vector<int> stack{tail};
            while (!stack.empty()) {
                const int x = stack.back();
                stack.pop_back();
                for (const int p : preds_[static_cast<std::size_t>(x)])
                    if (!loop[static_cast<std::size_t>(p)]) {
                        loop[static_cast<std::size_t>(p)] = 1;
                        stack.push_back(p);
                    }
            }
            for (std::size_t k = 0; k < n; ++k)
                if (loop[k]) body[k] = 1;
        }
        for (const auto& [tail, header] : back_edges) latches_.emplace(header, tail);
        emitted_.assign(n + 1, 0);
        active_.assign(n + 1, 0);
    }

    std::vector<std::vector<int>> predecessors_of() const {
        std::vector<std::vector<int>> preds(blocks_.size());
        for (const Block& b : blocks_)
            for (const int s : b.successors) preds.at(static_cast<std::size_t>(s)).push_back(b.id);
        return preds;
    }

    // The immediate post-dominator: the first post-dominator that every other one post-dominates; none when it is the exit.
    int ipdom(int id) const {
        IdSet candidates = pdom_[static_cast<std::size_t>(id)];
        candidates.remove(id);
        if (candidates.empty()) return kNone;
        for (int ip = 0; ip <= exit_; ++ip)
            if (candidates.has(ip) && candidates.subset_of(pdom_[static_cast<std::size_t>(ip)])) return ip == exit_ ? kNone : ip;
        return kNone;
    }

    // A short run of plain blocks (at most three, the first no more than eight instructions) that ends in one of `stops`.
    bool short_run_to(int b, const std::set<int>& stops) const {
        if (stops.empty() || cases_.empty()) return false;
        int n = 0, c = b;
        while (!stops.count(c)) {
            const Block& blk = block(c);
            if ((blk.end != End::Fall && blk.end != End::Jump) || blk.successors.size() != 1 || n >= 3) return false;
            ++n;
            c = blk.successors[0];
        }
        return 0 < n && block(b).size() <= 8;
    }

    // Too much code to copy: the blocks reachable from b (up to six) hold more than 24 instructions.
    bool is_big(int b) {
        if (const auto it = big_.find(b); it != big_.end()) return it->second;
        std::vector<int> seen{b}, work{b};
        const auto seen_has = [&](int x) { return std::ranges::find(seen, x) != seen.end(); };
        while (!work.empty() && seen.size() <= 5) {
            const int x = work.back();
            work.pop_back();
            for (const int t : block(x).successors)
                if (!seen_has(t)) {
                    seen.push_back(t);
                    work.push_back(t);
                }
        }
        std::size_t total = 0;
        for (const int x : seen) total += block(x).size();
        return big_[b] = seen.size() > 5 || total > 24;
    }

    bool ends_terminal(const Seq& nodes) const {
        if (nodes.empty()) return false;
        const Node& n = nodes.back();
        switch (n.kind) {
            case Node::Kind::Break:
            case Node::Kind::Continue:
            case Node::Kind::Goto:
                return true;
            case Node::Kind::Block:
                return block(n.block).end == End::Return || block(n.block).end == End::TailCall;
            case Node::Kind::IfElse:
                return ends_terminal(n.body) && ends_terminal(n.else_body);
            case Node::Kind::Switch:
                if (n.join != kNone) return false;
                return std::ranges::all_of(n.arms, [&](const auto& arm) { return ends_terminal(arm.second); });
            default:
                return false;
        }
    }

    // The nodes from block b on, up to a block of `stops`.
    Seq seq_from(int b, const std::set<int>& stops) {
        Seq out;
        std::set<int> local;
        while (b != kNone && b != exit_) {
            if (!loop_stack_.empty()) {
                LoopContext& loop = loop_stack_.back();
                const std::size_t depth = loop_stack_.size();
                const bool in_case = !cases_.empty() && cases_.back().depth == depth;
                if (in_case && cases_.back().join == b) return out;
                if (b == loop.follow && in_case) {
                    out.push_back(block(b).end == End::Return ? leaf(Node::Kind::Block, b) : goto_of(b));
                    return out;
                }
                if (b == loop.follow) {
                    out.push_back(leaf(Node::Kind::Break));
                    return out;
                }
                if (b == loop.header) {
                    out.push_back(leaf(Node::Kind::Continue));
                    return out;
                }
                if (!in_body(loop.body, b) && !loop.escaping) {
                    // Outside the loop: a short chain into a return is copied; anything else is structured once, and kept only if it ends the path.
                    std::vector<int> chain;
                    int c = b;
                    while (c != kNone && chain.size() < 4) {
                        chain.push_back(c);
                        if (block(c).end == End::Return) break;
                        c = ((block(c).end == End::Jump || block(c).end == End::Fall) && !block(c).successors.empty()) ? block(c).successors[0] : kNone;
                    }
                    if (c != kNone && block(c).end == End::Return && chain.back() == c) {
                        for (const int x : chain) out.push_back(leaf(Node::Kind::Block, x));
                        return out;
                    }
                    loop.escaping = true;
                    Seq sub = seq_from(b, stops);
                    loop_stack_.back().escaping = false;
                    if (ends_terminal(sub)) out.insert(out.end(), std::make_move_iterator(sub.begin()), std::make_move_iterator(sub.end()));
                    else out.push_back(goto_of(b));
                    return out;
                }
            }
            if (stops.count(b)) {
                if (!cases_.empty() && cases_.back().arms.count(b)) out.push_back(goto_of(b));
                break;
            }
            if (active_[static_cast<std::size_t>(b)] || local.count(b)) {
                out.push_back(goto_of(b));
                break;
            }
            if (emitted_[static_cast<std::size_t>(b)] && is_big(b) && !uncapped_ && !short_run_to(b, stops)) {
                out.push_back(goto_of(b));
                break;
            }
            emitted_[static_cast<std::size_t>(b)] = 1;
            local.insert(b);
            if (loops_.count(b)) {
                active_[static_cast<std::size_t>(b)] = 1;
                auto [node, next] = structure_loop(b);
                active_[static_cast<std::size_t>(b)] = 0;
                out.push_back(std::move(node));
                b = next;
                continue;
            }
            const Block& blk = block(b);
            if (blk.end == End::Branch) {
                active_[static_cast<std::size_t>(b)] = 1;
                auto [node, next] = structure_if(b, stops);
                active_[static_cast<std::size_t>(b)] = 0;
                out.push_back(std::move(node));
                b = next;
                continue;
            }
            if (blk.end == End::Switch) {
                active_[static_cast<std::size_t>(b)] = 1;
                auto [node, next] = structure_switch(b, stops);
                active_[static_cast<std::size_t>(b)] = 0;
                out.push_back(std::move(node));
                b = next;
                continue;
            }
            out.push_back(leaf(Node::Kind::Block, b));
            b = blk.successors.empty() ? kNone : blk.successors[0];
        }
        return out;
    }

    std::pair<Node, int> structure_switch(int b, const std::set<int>& stops) {
        const Block& blk = block(b);
        int join = ipdom(b);
        if (!loop_stack_.empty() && join != kNone) {
            const LoopContext& loop = loop_stack_.back();
            if (!in_body(loop.body, join) && join != loop.follow) join = kNone;
        }
        // A switch that is a loop's header, with no join in the loop: the join is the one block every arm that stays in the loop reaches first.
        if (!loop_stack_.empty() && loop_stack_.back().header == b && (join == kNone || !in_body(loop_stack_.back().body, join))) {
            const BlockSet* loop_body = loop_stack_.back().body;
            const auto reach = [&](int t) {
                std::set<int> seen{t};
                std::vector<int> work{t};
                while (!work.empty()) {
                    const int x0 = work.back();
                    work.pop_back();
                    for (const int x : block(x0).successors)
                        if (in_body(loop_body, x) && x != b && !seen.count(x)) {
                            seen.insert(x);
                            work.push_back(x);
                        }
                }
                return seen;
            };
            std::vector<std::set<int>> stay;
            for (const int t : blk.successors)
                if (in_body(loop_body, t) && t != b) stay.push_back(reach(t));
            std::set<int> common;
            if (!stay.empty()) {
                common = stay[0];
                for (std::size_t k = 1; k < stay.size(); ++k) {
                    std::set<int> r;
                    for (const int x : common)
                        if (stay[k].count(x)) r.insert(x);
                    common = std::move(r);
                }
            }
            std::vector<int> firsts;
            for (const int j : common) {
                bool reaches_all = true;
                std::set<int> from_j;
                bool computed = false;
                for (const int k : common) {
                    if (j == k) continue;
                    if (!computed) {
                        from_j = reach(j);
                        computed = true;
                    }
                    if (!from_j.count(k)) {
                        reaches_all = false;
                        break;
                    }
                }
                if (reaches_all) firsts.push_back(j);
            }
            if (firsts.size() == 1) join = firsts[0];
        }
        const std::set<int> sub = with(stops, join);
        Node node = leaf(Node::Kind::Switch, b);
        for (const int t : blk.successors) {
            std::vector<CaseValue> values;
            if (blk.cases) {
                std::vector<long long> vs;
                for (const Case& c : *blk.cases)
                    if (c.target == t) vs.push_back(c.value);
                std::ranges::sort(vs);
                for (const long long v : vs) values.push_back({false, v});
                if (t == blk.default_target) values.push_back({true, 0});
                if (t == join && values.size() == 1 && values[0].is_default) continue;
                std::set<int> arms(blk.successors.begin(), blk.successors.end());
                arms.erase(t);
                if (join != kNone) arms.erase(join);
                cases_.push_back({arms, loop_stack_.size(), join});
                Seq arm;
                if (t != join) {
                    std::set<int> stops_in_arm = sub;
                    stops_in_arm.insert(arms.begin(), arms.end());
                    arm = seq_from(t, stops_in_arm);
                }
                cases_.pop_back();
                node.arms.emplace_back(std::move(values), std::move(arm));
                continue;
            }
            for (std::size_t i = 0; blk.table && i < blk.table->size(); ++i)
                if ((*blk.table)[i] == t) values.push_back({false, static_cast<long long>(i)});
            node.arms.emplace_back(std::move(values), t != join ? seq_from(t, sub) : Seq());
        }
        node.join = join;
        return {std::move(node), join};
    }

    std::pair<Node, int> structure_if(int b, const std::set<int>& stops) {
        const Block& blk = block(b);
        int join = ipdom(b);
        if (!loop_stack_.empty() && join != kNone && !in_body(loop_stack_.back().body, join)) join = kNone;
        const int taken = successor_at(blk, 0);
        const int fall = blk.successors.size() > 1 ? blk.successors[1] : kNone;
        const int then_side = fall, else_side = taken;
        const std::set<int> sub = with(stops, join);
        Node node;
        node.block = b;
        if (join != kNone && else_side == join) {
            node.kind = Node::Kind::If;
            node.body = seq_from(then_side, sub);
            node.join = join;
            node.negated = false;
            return {std::move(node), join};
        }
        if (join != kNone && then_side == join) {
            node.kind = Node::Kind::If;
            node.body = seq_from(else_side, sub);
            node.join = join;
            node.negated = true;
            return {std::move(node), join};
        }
        node.kind = Node::Kind::IfElse;
        const std::set<int>& arm_stops = join != kNone ? sub : stops;
        node.body = seq_from(then_side, arm_stops);
        node.else_body = seq_from(else_side, arm_stops);
        node.join = join;
        return {std::move(node), join};
    }

    std::pair<Node, int> structure_loop(int h) {
        const BlockSet* body = &loops_.at(h);
        const Block& header = block(h);
        std::set<int> exits;
        for (std::size_t x = 0; x < body->size(); ++x)
            if ((*body)[x])
                for (const int s : blocks_[x].successors)
                    if (!in_body(body, s)) exits.insert(s);
        const int follow = exits.size() == 1 ? *exits.begin() : kNone;
        const auto latch_it = latches_.find(h);
        const int latch = latch_it == latches_.end() ? kNone : latch_it->second;
        const bool self_loop = std::ranges::find(header.successors, h) != header.successors.end();
        const bool header_exits = std::ranges::any_of(header.successors, [&](int s) { return !in_body(body, s); });
        Node node;
        node.block = h;
        // A pre-tested loop: the header tests and leaves.
        if (header.end == End::Branch && header_exits && !self_loop) {
            std::vector<int> inside;
            int after = kNone;
            for (const int s : header.successors)
                if (in_body(body, s)) inside.push_back(s);
            for (const int s : header.successors)
                if (!in_body(body, s)) {
                    after = s;
                    break;
                }
            loop_stack_.push_back({h, after, body, false});
            node.body = inside.empty() ? Seq() : seq_from(inside[0], {h});
            loop_stack_.pop_back();
            node.kind = Node::Kind::While;
            node.join = after;
            node.negated = !inside.empty() && inside[0] == header.successors[0];
            return {std::move(node), after};
        }
        // A post-tested loop: the latch tests and leaves.
        if (latch != kNone && block(latch).end == End::Branch) {
            int after = kNone;
            bool latch_exits = false;
            for (const int s : block(latch).successors)
                if (!in_body(body, s)) {
                    if (!latch_exits) after = s;
                    latch_exits = true;
                }
            if (latch_exits) {
                loop_stack_.push_back({h, after, body, false});
                Seq loop_body;
                if (h == latch) {
                } else if (header.end == End::Branch) {
                    active_[static_cast<std::size_t>(h)] = 1;
                    auto [first, next] = structure_if(h, {latch});
                    active_[static_cast<std::size_t>(h)] = 0;
                    loop_body.push_back(std::move(first));
                    if (next != kNone && next != h && next != latch && next != exit_) {
                        Seq more = seq_from(next, {latch});
                        loop_body.insert(loop_body.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
                    }
                } else {
                    loop_body.push_back(leaf(Node::Kind::Block, h));
                    if (!header.successors.empty() && header.successors[0] != latch) {
                        Seq more = seq_from(header.successors[0], {latch});
                        loop_body.insert(loop_body.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
                    }
                }
                loop_body.push_back(leaf(Node::Kind::Block, latch));
                loop_stack_.pop_back();
                node.kind = Node::Kind::DoWhile;
                node.body = std::move(loop_body);
                node.join = latch;
                node.negated = in_body(body, successor_at(block(latch), 0));
                return {std::move(node), after};
            }
        }
        // Anything else: an endless loop with its exits inside.
        loop_stack_.push_back({h, follow, body, false});
        Seq loop_body;
        if (header.end == End::Switch || header.end == End::Branch) {
            auto [first, next] = header.end == End::Switch ? structure_switch(h, {h}) : structure_if(h, {h});
            loop_body.push_back(std::move(first));
            if (next != kNone && next != h && next != exit_) {
                Seq more = seq_from(next, {h});
                loop_body.insert(loop_body.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
            }
        } else {
            loop_body.push_back(leaf(Node::Kind::Block, h));
            if (!header.successors.empty()) {
                Seq more = seq_from(header.successors[0], {h});
                loop_body.insert(loop_body.end(), std::make_move_iterator(more.begin()), std::make_move_iterator(more.end()));
            }
        }
        loop_stack_.pop_back();
        node.kind = Node::Kind::Loop;
        node.body = std::move(loop_body);
        node.join = follow;
        return {std::move(node), follow};
    }

    const std::vector<Block>& blocks_;
    bool uncapped_;
    int exit_ = 0;
    std::vector<IdSet> dom_, pdom_;
    std::vector<std::vector<int>> preds_;
    std::map<int, BlockSet> loops_;   // header -> the loop's blocks
    std::map<int, int> latches_;
    std::vector<char> emitted_, active_;
    std::map<int, bool> big_;
    std::deque<LoopContext> loop_stack_;   // a deque: structuring inside a loop must not move the loops outside it
    std::vector<CaseContext> cases_;
};

}  // namespace

Tree structure(const Cfg& cfg, int entry, bool uncapped) { return Structurer(cfg, uncapped).run(entry); }

}  // namespace cb::invert::flow16
