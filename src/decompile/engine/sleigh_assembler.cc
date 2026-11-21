// decompile/engine/sleigh_assembler.cc -- see sleigh_assembler.hh. Built with the core (its own flags), so the core's headers stay here.

#include "decompile/engine/sleigh_assembler.hh"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "decompile/engine/core/cpp/loadimage.hh"
#include "decompile/engine/core/cpp/sleigh.hh"

namespace cb::decompile::sleigh {
namespace {

using namespace ghidra;

constexpr int kMaxInst = 16;         // bytes of an instruction (the translator's own limit)
constexpr int kMaxCtx = 64;           // bytes of context
constexpr std::size_t kMaxAlts = 32;  // alternative solutions of one operand
constexpr std::size_t kMaxParses = 3000;
constexpr std::size_t kMaxPhantoms = 12;   // constructors tried for an operand the text does not show
constexpr std::size_t kMaxCandidates = 400;

// ---------------------------------------------------------------- the translator used to verify

class Window : public LoadImage {
public:
    Window() : LoadImage("codebreaker") {}
    void set(const std::uint8_t* data, std::size_t size, std::uint64_t base) {
        data_ = data;
        size_ = size;
        base_ = base;
    }
    void loadFill(uint1* ptr, int4 size, const Address& addr) override {
        const std::uint64_t start = addr.getOffset();
        for (int4 i = 0; i < size; ++i) {
            const std::uint64_t at = start + static_cast<std::uint64_t>(i);
            ptr[i] = at >= base_ && at - base_ < size_ ? data_[at - base_] : 0;
        }
    }
    std::string getArchType() const override { return "codebreaker"; }
    void adjustVma(long) override {}

private:
    const std::uint8_t* data_ = nullptr;
    std::size_t size_ = 0;
    std::uint64_t base_ = 0;
};

class Text : public AssemblyEmit {
public:
    void dump(const Address&, const std::string& mnem, const std::string& body) override {
        mnemonic = mnem;
        operands = body;
    }
    std::string mnemonic, operands;
};

void initialize_once() {
    static std::once_flag once;
    std::call_once(once, [] {
        AttributeId::initialize();
        ElementId::initialize();
    });
}

// (CB_ASM_TRACE=1 in the environment: what the placing of each tree does, on stderr)
bool tracing() {
    static const bool on = std::getenv("CB_ASM_TRACE") != nullptr;
    return on;
}

std::string squash(std::string_view s) {
    std::string out;
    for (const char c : s)
        if (!std::isspace(static_cast<unsigned char>(c))) out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

// The text with whitespace kept only where it parts two words ("CP L" stays two words, "( 0x12 )" is "(0x12)"), lower case: what the
// verification compares, so that a line and an instruction whose texts differ only by a space between two words are told apart
// (the SM83's CPL and CP L squash to the same letters).
std::string spaced(std::string_view s) {
    auto word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_' || c == '.'; };
    std::string out;
    bool gap = false;
    for (const char c : s) {
        if (std::isspace(static_cast<unsigned char>(c))) {
            gap = true;
            continue;
        }
        if (gap && !out.empty() && word(out.back()) && word(c)) out += ' ';
        gap = false;
        out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return out;
}

// ---------------------------------------------------------------- the bits of an instruction being built

struct Inst;

// A context change whose value depends on fields not fixed yet: when a later constructor requires those context bits, the expression
// is solved for them.
struct Link {
    const Inst* n = nullptr;
    const ContextOp* op = nullptr;
};

struct State {
    std::uint8_t ival[kMaxInst] = {}, imask[kMaxInst] = {};   // instruction bytes: value and the bits fixed so far
    std::uint8_t cval[kMaxCtx] = {}, cmask[kMaxCtx] = {};     // context bytes (big-endian words): value and the bits known
    Link links[8];
    int nlinks = 0;
};

// A token or context field: where bit j of its value lives.
struct LeafInfo {
    const TokenField* tf = nullptr;
    const ContextField* cf = nullptr;
    int width = 0;
    bool sign = false;
    bool ctx() const { return cf != nullptr; }
    bool where(int absOff, int j, int& byte, int& bit) const {
        if (tf != nullptr) {
            const int size = tf->getByteEnd() - tf->getByteStart() + 1;
            const int p = tf->getShift() + j;
            const int k = tf->isBigEndian() ? size - 1 - p / 8 : p / 8;
            byte = absOff + tf->getByteStart() + k;
            bit = p % 8;
            return byte >= 0 && byte < kMaxInst && size <= 8;
        }
        const int size = cf->getEndByte() - cf->getStartByte() + 1;
        const int p = cf->getShift() + j;
        byte = cf->getStartByte() + (size - 1 - p / 8);
        bit = p % 8;
        return byte >= 0 && byte < kMaxCtx && size <= 8;
    }
};

bool leafInfo(const PatternExpression* e, LeafInfo& li) {
    if (const auto* tf = dynamic_cast<const TokenField*>(e)) {
        li.tf = tf;
        li.width = tf->getBitEnd() - tf->getBitStart() + 1;
        li.sign = tf->getSignBit();
        return true;
    }
    if (const auto* cf = dynamic_cast<const ContextField*>(e)) {
        li.cf = cf;
        li.width = cf->getEndBit() - cf->getStartBit() + 1;
        li.sign = cf->getSignBit();
        return true;
    }
    return false;
}

intb signExtend(std::uint64_t raw, int width, bool sign) {
    if (width >= 64) return static_cast<intb>(raw);
    raw &= (std::uint64_t{1} << width) - 1;
    if (sign && ((raw >> (width - 1)) & 1) != 0) raw |= ~((std::uint64_t{1} << width) - 1);
    return static_cast<intb>(raw);
}

std::optional<intb> readLeaf(const State& st, const LeafInfo& li, int absOff) {
    std::uint64_t v = 0;
    for (int j = 0; j < li.width && j < 64; ++j) {
        int byte, bit;
        if (!li.where(absOff, j, byte, bit)) return std::nullopt;
        const std::uint8_t* mask = li.ctx() ? st.cmask : st.imask;
        const std::uint8_t* val = li.ctx() ? st.cval : st.ival;
        if (((mask[byte] >> bit) & 1) == 0) return std::nullopt;
        v |= static_cast<std::uint64_t>((val[byte] >> bit) & 1) << j;
    }
    return signExtend(v, li.width, li.sign);
}

bool writeLeaf(State& st, const LeafInfo& li, int absOff, std::uint64_t raw) {
    for (int j = 0; j < li.width && j < 64; ++j) {
        int byte, bit;
        if (!li.where(absOff, j, byte, bit)) return false;
        std::uint8_t* mask = li.ctx() ? st.cmask : st.imask;
        std::uint8_t* val = li.ctx() ? st.cval : st.ival;
        const std::uint8_t b = static_cast<std::uint8_t>((raw >> j) & 1);
        if (((mask[byte] >> bit) & 1) != 0) {
            if (((val[byte] >> bit) & 1) != b) return false;
        } else {
            mask[byte] = static_cast<std::uint8_t>(mask[byte] | (1u << bit));
            val[byte] = static_cast<std::uint8_t>((val[byte] & ~(1u << bit)) | (b << bit));
        }
    }
    return true;
}

struct LeafSet {
    LeafInfo li;
    int absOff = 0;
    std::uint64_t raw = 0;
};
using Alt = std::vector<LeafSet>;

// ---------------------------------------------------------------- the parse of the text

struct PNode;
struct Slot {
    std::shared_ptr<const PNode> sub;
    intb value = 0;   // a number as printed, or the index into a table of names
    bool shown = false;   // the text had a value for it
};
struct PNode {
    const Constructor* ct = nullptr;
    std::vector<Slot> slots;
};
struct Parsed {
    std::size_t end;
    std::shared_ptr<const PNode> node;
};

// A constructor instantiated at an offset.
struct Inst {
    const Constructor* ct = nullptr;
    const PNode* p = nullptr;
    int offset = 0, length = 0;
    std::vector<int> opOff, opLen;
    std::vector<std::unique_ptr<Inst>> sub;
};

struct PatAlt {
    std::vector<std::uint8_t> im, iv, cm, cv;
};

struct Env {
    intb startVal = 0, nextVal = 0;
};

using Overrides = std::map<std::pair<const void*, int>, intb>;

}  // namespace

// ---------------------------------------------------------------- the assembler

struct Assembler::Impl {
    Window window;
    ContextInternal context;
    std::unique_ptr<Sleigh> trans;
    DocumentStorage store;
    std::vector<std::pair<std::string, int>> initial;
    AddrSpace* space = nullptr;
    int ctxBytes = 0;
    mutable std::mutex lock;
    mutable Diagnostics diag;
    mutable std::map<const SubtableSymbol*, std::vector<std::vector<PatAlt>>> patterns;   // per subtable: per constructor id

    // ---- what parsing a constructor's print pieces needs, made once per description: each literal piece squashed, each operand piece's
    // index, the operands the print shows; and the squashed names of each name table and register list
    struct CtorInfo {
        std::vector<std::string> lit;   // a literal piece, squashed (empty for an operand piece)
        std::vector<int> op;            // an operand piece's operand index; -1 for a literal one
        std::vector<bool> printed;      // per operand: the print shows it
    };
    mutable std::unordered_map<const Constructor*, CtorInfo> infos;
    struct NameList {
        std::vector<std::string> nm;   // squashed
        std::vector<char> ok;          // a name a table has (not its blank "\t" entries, not a missing register)
    };
    mutable std::unordered_map<const void*, NameList> squashedNames;
    mutable std::unordered_map<const void*, std::string> squashedName;

    const NameList& namesOf(const SleighSymbol* t) const {
        auto it = squashedNames.find(t);
        if (it != squashedNames.end()) return it->second;
        NameList l;
        for (const std::string& n : static_cast<const NameSymbol*>(t)->getNameTable()) {
            l.ok.push_back(n != "\t");
            l.nm.push_back(n == "\t" ? std::string() : squash(n));
        }
        return squashedNames.emplace(t, std::move(l)).first->second;
    }
    template <class Table>
    const NameList& registersOf(const SleighSymbol* t, const Table& vt) const {
        auto it = squashedNames.find(t);
        if (it != squashedNames.end()) return it->second;
        NameList l;
        for (const auto* v : vt) {
            l.ok.push_back(v != nullptr);
            l.nm.push_back(v == nullptr ? std::string() : squash(v->getName()));
        }
        return squashedNames.emplace(t, std::move(l)).first->second;
    }
    const std::string& nameOf(const SleighSymbol* t) const {
        auto it = squashedName.find(t);
        if (it != squashedName.end()) return it->second;
        return squashedName.emplace(t, squash(t->getName())).first->second;
    }

    const CtorInfo& infoOf(const Constructor* ct) const {
        auto it = infos.find(ct);
        if (it != infos.end()) return it->second;
        CtorInfo ci;
        ci.printed.assign(static_cast<std::size_t>(ct->getNumOperands()), false);
        for (int i = 0; i < ct->getNumPrintPieces(); ++i) {
            const std::string& pc = ct->getPrintPiece(i);
            if (!pc.empty() && pc[0] == '\n') {
                const std::size_t idx = pc.size() > 1 ? static_cast<std::size_t>(pc[1] - 'A') : ~std::size_t{0};
                ci.op.push_back(idx < ci.printed.size() ? static_cast<int>(idx) : -2);   // (-2: an operand the constructor does not have)
                ci.lit.emplace_back();
                if (idx < ci.printed.size()) ci.printed[idx] = true;
            } else {
                ci.op.push_back(-1);
                ci.lit.push_back(squash(pc));
            }
        }
        return infos.emplace(ct, std::move(ci)).first->second;
    }

    // The constructors of a subtable that can start with a given character (index 256: the end of the text), in the table's order: those
    // whose print starts with a word that begins with it, and those that start with anything else (an operand, no text). Made per
    // character when first asked.
    struct StartIndex {
        std::vector<std::vector<std::uint32_t>> by;   // [257]
        std::vector<char> made;
    };
    mutable std::unordered_map<const SubtableSymbol*, StartIndex> starts;

    const std::vector<std::uint32_t>& startingWith(const SubtableSymbol* st, int c) const {
        StartIndex& ix = starts[st];
        if (ix.by.empty()) {
            ix.by.resize(257);
            ix.made.assign(257, 0);
        }
        if (ix.made[static_cast<std::size_t>(c)] == 0) {
            std::vector<std::uint32_t>& list = ix.by[static_cast<std::size_t>(c)];
            for (int i = 0; i < st->getNumConstructors(); ++i) {
                const CtorInfo& ci = infoOf(st->getConstructor(static_cast<std::uint32_t>(i)));
                const bool word = !ci.op.empty() && ci.op[0] == -1 && !ci.lit[0].empty();
                if (!word || (c < 256 && static_cast<unsigned char>(ci.lit[0][0]) == c)) list.push_back(static_cast<std::uint32_t>(i));
            }
            ix.made[static_cast<std::size_t>(c)] = 1;
        }
        return ix.by[static_cast<std::size_t>(c)];
    }

    // ---- the patterns of a constructor
    const std::vector<PatAlt>& altsOf(const Constructor* ct) const {
        const SubtableSymbol* st = ct->getParent();
        auto it = patterns.find(st);
        if (it == patterns.end()) {
            std::vector<std::vector<PatAlt>> per(static_cast<std::size_t>(st->getNumConstructors()));
            std::vector<std::pair<const DisjointPattern*, const Constructor*>> all;
            st->collectPatterns(all);
            std::vector<std::set<std::vector<std::uint8_t>>> seen(per.size());
            for (const auto& [pat, c] : all) {
                PatAlt a;
                const int il = std::max(0, pat->getLength(false)), cl = std::max(0, pat->getLength(true));
                for (int i = 0; i < il; ++i) {
                    a.im.push_back(static_cast<std::uint8_t>(pat->getMask(8 * i, 8, false)));
                    a.iv.push_back(static_cast<std::uint8_t>(pat->getValue(8 * i, 8, false)));
                }
                for (int i = 0; i < cl; ++i) {
                    a.cm.push_back(static_cast<std::uint8_t>(pat->getMask(8 * i, 8, true)));
                    a.cv.push_back(static_cast<std::uint8_t>(pat->getValue(8 * i, 8, true)));
                }
                std::vector<std::uint8_t> key;
                key.push_back(static_cast<std::uint8_t>(il));
                key.insert(key.end(), a.im.begin(), a.im.end());
                key.insert(key.end(), a.iv.begin(), a.iv.end());
                key.insert(key.end(), a.cm.begin(), a.cm.end());
                key.insert(key.end(), a.cv.begin(), a.cv.end());
                const auto id = static_cast<std::size_t>(c->getId());
                if (id < per.size() && seen[id].insert(key).second) per[id].push_back(std::move(a));
            }
            it = patterns.emplace(st, std::move(per)).first;
        }
        return it->second[static_cast<std::size_t>(ct->getId())];
    }

    // ---- evaluating a pattern expression over the bits known so far
    // What kind of expression a node is, found once (the type tests of a dynamic_cast chain cost more than the arithmetic they lead to).
    enum class Kind : std::uint8_t { Leaf, Constant, Start, End, Next2, Operand, Plus, Sub, Mult, LeftShift, RightShift, And, Or, Xor, Div, OtherBinary, Minus, Not, OtherUnary, Other };
    struct Node {
        Kind kind = Kind::Other;
        LeafInfo li;
        intb constant = 0;
        int index = 0;                                // an operand's
        const PatternExpression *left = nullptr, *right = nullptr;   // a binary's (right: null for a unary)
    };
    mutable std::unordered_map<const PatternExpression*, Node> nodes;

    const Node& nodeOf(const PatternExpression* e) const {
        auto it = nodes.find(e);
        if (it != nodes.end()) return it->second;
        Node nd;
        if (leafInfo(e, nd.li)) nd.kind = Kind::Leaf;
        else if (const auto* c = dynamic_cast<const ConstantValue*>(e)) nd.kind = Kind::Constant, nd.constant = c->getConstant();
        else if (dynamic_cast<const StartInstructionValue*>(e) != nullptr) nd.kind = Kind::Start;
        else if (dynamic_cast<const EndInstructionValue*>(e) != nullptr) nd.kind = Kind::End;
        else if (dynamic_cast<const Next2InstructionValue*>(e) != nullptr) nd.kind = Kind::Next2;
        else if (const auto* ovl = dynamic_cast<const OperandValue*>(e)) nd.kind = Kind::Operand, nd.index = ovl->getIndex();
        else if (const auto* b = dynamic_cast<const BinaryExpression*>(e)) {
            nd.left = b->getLeft(), nd.right = b->getRight();
            nd.kind = dynamic_cast<const PlusExpression*>(e)         ? Kind::Plus
                      : dynamic_cast<const SubExpression*>(e)        ? Kind::Sub
                      : dynamic_cast<const MultExpression*>(e)       ? Kind::Mult
                      : dynamic_cast<const LeftShiftExpression*>(e)  ? Kind::LeftShift
                      : dynamic_cast<const RightShiftExpression*>(e) ? Kind::RightShift
                      : dynamic_cast<const AndExpression*>(e)        ? Kind::And
                      : dynamic_cast<const OrExpression*>(e)         ? Kind::Or
                      : dynamic_cast<const XorExpression*>(e)        ? Kind::Xor
                      : dynamic_cast<const DivExpression*>(e)        ? Kind::Div
                                                                     : Kind::OtherBinary;
        } else if (const auto* u = dynamic_cast<const UnaryExpression*>(e)) {
            nd.left = u->getUnary();
            nd.kind = dynamic_cast<const MinusExpression*>(e) ? Kind::Minus : dynamic_cast<const NotExpression*>(e) ? Kind::Not : Kind::OtherUnary;
        }
        return nodes.emplace(e, nd).first->second;
    }

    std::optional<intb> eval(const PatternExpression* e, const Inst& n, int curOff, const Env& env, const State& st, const Overrides* ov,
                             int depth = 0) const {
        if (depth > 40) return std::nullopt;
        const Node& nd = nodeOf(e);
        switch (nd.kind) {
        case Kind::Leaf: {
            const int abs = nd.li.ctx() ? 0 : curOff;
            if (ov != nullptr) {
                auto it = ov->find({e, abs});
                if (it != ov->end()) return it->second;
            }
            return readLeaf(st, nd.li, abs);
        }
        case Kind::Constant: return nd.constant;
        case Kind::Start: return env.startVal;
        case Kind::End: return env.nextVal;
        case Kind::Next2: return std::nullopt;
        case Kind::Operand: {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return std::nullopt;
            const PatternExpression* pe = operandExpr(n, idx);
            if (pe == nullptr) return nullptr == n.ct->getOperand(idx)->getDefiningSymbol() ? std::optional<intb>(std::nullopt) : std::optional<intb>(0);
            return eval(pe, n, n.opOff[static_cast<std::size_t>(idx)], env, st, ov, depth + 1);
        }
        case Kind::Plus: case Kind::Sub: case Kind::Mult: case Kind::LeftShift: case Kind::RightShift: case Kind::And: case Kind::Or: case Kind::Xor:
        case Kind::Div: case Kind::OtherBinary: {
            const auto l = eval(nd.left, n, curOff, env, st, ov, depth + 1);
            if (!l) return std::nullopt;
            const auto r = eval(nd.right, n, curOff, env, st, ov, depth + 1);
            if (!r) return std::nullopt;
            const auto ul = static_cast<std::uint64_t>(*l), ur = static_cast<std::uint64_t>(*r);
            switch (nd.kind) {
            case Kind::Plus: return static_cast<intb>(ul + ur);
            case Kind::Sub: return static_cast<intb>(ul - ur);
            case Kind::Mult: return static_cast<intb>(ul * ur);
            case Kind::LeftShift: return (*r < 0 || *r >= 64) ? std::nullopt : std::optional<intb>(static_cast<intb>(ul << *r));
            case Kind::RightShift: return (*r < 0 || *r >= 64) ? std::nullopt : std::optional<intb>(*l >> *r);
            case Kind::And: return static_cast<intb>(ul & ur);
            case Kind::Or: return static_cast<intb>(ul | ur);
            case Kind::Xor: return static_cast<intb>(ul ^ ur);
            case Kind::Div: return *r == 0 ? std::nullopt : std::optional<intb>(*l / *r);
            default: return std::nullopt;
            }
        }
        case Kind::Minus: case Kind::Not: case Kind::OtherUnary: {
            const auto v = eval(nd.left, n, curOff, env, st, ov, depth + 1);
            if (!v) return std::nullopt;
            if (nd.kind == Kind::Minus) return static_cast<intb>(0 - static_cast<std::uint64_t>(*v));
            if (nd.kind == Kind::Not) return ~*v;
            return std::nullopt;
        }
        case Kind::Other: break;
        }
        return std::nullopt;
    }

    // The expression that gives an operand its value (null: none; a subtable has none).
    static const PatternExpression* operandExpr(const Inst& n, int idx) {
        const OperandSymbol* sym = n.ct->getOperand(idx);
        if (const PatternExpression* pe = sym->getDefiningExpression()) return pe;
        const TripleSymbol* t = sym->getDefiningSymbol();
        if (t == nullptr || t->getType() == SleighSymbol::subtable_symbol) return nullptr;
        try {
            return t->getPatternExpression();
        } catch (const LowlevelError&) {
            return nullptr;
        }
    }

    // ---- free leaves of an expression (those whose bits are not all fixed yet)
    struct Free {
        LeafInfo li;
        const PatternExpression* e;
        int abs;
    };
    void freeLeaves(const PatternExpression* e, const Inst& n, int curOff, const Env& env, const State& st, std::vector<Free>& out,
                    int depth = 0) const {
        if (depth > 40) return;
        const Node& nd = nodeOf(e);
        if (nd.kind == Kind::Leaf) {
            const LeafInfo& li = nd.li;
            const int abs = li.ctx() ? 0 : curOff;
            if (readLeaf(st, li, abs)) return;
            for (const Free& f : out)
                if (f.e == e && f.abs == abs) return;
            out.push_back({li, e, abs});
            return;
        }
        if (nd.kind == Kind::Operand) {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return;
            if (const PatternExpression* pe = operandExpr(n, idx)) freeLeaves(pe, n, n.opOff[static_cast<std::size_t>(idx)], env, st, out, depth + 1);
            return;
        }
        if (nd.right != nullptr) {   // (a binary)
            freeLeaves(nd.left, n, curOff, env, st, out, depth + 1);
            freeLeaves(nd.right, n, curOff, env, st, out, depth + 1);
        } else if (nd.left != nullptr) {   // (a unary)
            freeLeaves(nd.left, n, curOff, env, st, out, depth + 1);
        }
    }

    // The bits an expression can set (an upper bound; ~0 when unknown).
    std::uint64_t maskOf(const PatternExpression* e, const Inst& n, int curOff, const Env& env, const State& st, int depth = 0) const {
        if (depth > 30) return ~std::uint64_t{0};
        if (const auto k = eval(e, n, curOff, env, st, nullptr)) return static_cast<std::uint64_t>(*k);
        const Node& nd = nodeOf(e);
        if (nd.kind == Kind::Leaf) return nd.li.sign || nd.li.width >= 64 ? ~std::uint64_t{0} : (std::uint64_t{1} << nd.li.width) - 1;
        if (nd.kind == Kind::Operand) {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return ~std::uint64_t{0};
            const PatternExpression* pe = operandExpr(n, idx);
            return pe == nullptr ? ~std::uint64_t{0} : maskOf(pe, n, n.opOff[static_cast<std::size_t>(idx)], env, st, depth + 1);
        }
        if (nd.right != nullptr) {   // (a binary)
            const auto r = eval(nd.right, n, curOff, env, st, nullptr);
            if (nd.kind == Kind::LeftShift && r && *r >= 0 && *r < 64) return maskOf(nd.left, n, curOff, env, st, depth + 1) << *r;
            if (nd.kind == Kind::RightShift && r && *r >= 0 && *r < 64) return maskOf(nd.left, n, curOff, env, st, depth + 1) >> *r;
            if (nd.kind == Kind::And && r) return maskOf(nd.left, n, curOff, env, st, depth + 1) & static_cast<std::uint64_t>(*r);
            if (nd.kind == Kind::Mult) {   // (a power of two: a shift)
                const auto kl = eval(nd.left, n, curOff, env, st, nullptr);
                const auto k = r ? r : kl;
                if (k && *k > 0 && (*k & (*k - 1)) == 0 && !(r && kl)) {
                    unsigned s = 0;
                    while ((intb{1} << s) != *k) ++s;
                    return maskOf(r ? nd.left : nd.right, n, curOff, env, st, depth + 1) << s;
                }
            }
            if (nd.kind == Kind::Or || nd.kind == Kind::Plus) return maskOf(nd.left, n, curOff, env, st, depth + 1) | maskOf(nd.right, n, curOff, env, st, depth + 1);
        }
        return ~std::uint64_t{0};
    }

    // ---- solving: the field values that make `e` equal `t`
    static void crossAdd(std::vector<Alt>& out, const std::vector<Alt>& a, const std::vector<Alt>& b) {
        for (const Alt& x : a)
            for (const Alt& y : b) {
                if (out.size() >= kMaxAlts) return;
                Alt z = x;
                z.insert(z.end(), y.begin(), y.end());
                out.push_back(std::move(z));
            }
    }

    // The field values that make `e & m` equal `r`, where m is a low mask (2^k - 1; a wrap such as `(next + rel) & 0xFFFF`), solved
    // without enumerating: a sum or difference with a known side moves it across, a field gives every value of its range with those low
    // bits. False: the expression is of a shape this does not solve (the caller enumerates).
    bool solveLow(const PatternExpression* e, std::uint64_t r, std::uint64_t m, const Inst& n, int curOff, const Env& env, const State& st,
                  std::vector<Alt>& out, int depth) const {
        if (depth > 30 || m == 0 || m > 0xFFFFFFFFull || ((m + 1) & m) != 0) return false;
        r &= m;
        if (const auto k = eval(e, n, curOff, env, st, nullptr)) {
            if (((static_cast<std::uint64_t>(*k) ^ r) & m) == 0) out.push_back({});
            return true;
        }
        const Node& nd = nodeOf(e);
        if (nd.kind == Kind::Leaf) {
            const LeafInfo& li = nd.li;
            const int w = li.width;
            if (w <= 0 || w >= 63) return false;
            const intb lo = li.sign ? -(intb{1} << (w - 1)) : 0, hi = li.sign ? (intb{1} << (w - 1)) - 1 : (intb{1} << w) - 1;
            const std::uint64_t wmask = (std::uint64_t{1} << w) - 1;
            intb v = lo + static_cast<intb>((r - static_cast<std::uint64_t>(lo)) & m);
            for (; v <= hi && out.size() < kMaxAlts; v += static_cast<intb>(m + 1))
                out.push_back({LeafSet{li, li.ctx() ? 0 : curOff, static_cast<std::uint64_t>(v) & wmask}});
            return true;
        }
        if (nd.kind == Kind::Operand) {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return true;
            const PatternExpression* pe = operandExpr(n, idx);
            return pe == nullptr ? true : solveLow(pe, r, m, n, n.opOff[static_cast<std::size_t>(idx)], env, st, out, depth + 1);
        }
        if (nd.kind == Kind::Minus) return solveLow(nd.left, 0 - r, m, n, curOff, env, st, out, depth + 1);
        if (nd.kind == Kind::Mult) {   // a field times a power of two (a displacement counted in halfwords or words)
            const auto kl = eval(nd.left, n, curOff, env, st, nullptr), kr = eval(nd.right, n, curOff, env, st, nullptr);
            const auto k = kr ? kr : kl;
            const PatternExpression* x = kr ? nd.left : (kl ? nd.right : nullptr);
            if (x == nullptr || kl == kr || *k <= 0 || (*k & (*k - 1)) != 0) return false;
            unsigned s = 0;
            while ((intb{1} << s) != *k) ++s;
            if ((r & ((std::uint64_t{1} << s) - 1)) != 0) return true;
            return solveLow(x, r >> s, m >> s, n, curOff, env, st, out, depth + 1);
        }
        if (nd.kind == Kind::Plus && !eval(nd.left, n, curOff, env, st, nullptr) == !eval(nd.right, n, curOff, env, st, nullptr)) {
            // a sum of fields that take bits of their own (a pc plus a displacement in two parts): each part is what its bits of the
            // target (less the sum's constant part) say
            std::vector<const PatternExpression*> terms;
            std::uint64_t constant = 0;
            std::vector<const PatternExpression*> stack{e};
            while (!stack.empty()) {
                const PatternExpression* x = stack.back();
                stack.pop_back();
                const Node& xn = nodeOf(x);
                if (xn.kind == Kind::Plus) {
                    stack.push_back(xn.left);
                    stack.push_back(xn.right);
                } else if (const auto k = eval(x, n, curOff, env, st, nullptr)) {
                    constant += static_cast<std::uint64_t>(*k);
                } else {
                    terms.push_back(x);
                }
            }
            if (terms.size() < 2) return false;
            std::vector<std::uint64_t> masks;
            std::uint64_t used = 0;
            for (const PatternExpression* t : terms) {
                const std::uint64_t mt = maskOf(t, n, curOff, env, st) & m;
                if (mt == 0 || (used & mt) != 0) return false;
                used |= mt;
                masks.push_back(mt);
            }
            const std::uint64_t want = (r - constant) & m;
            if ((want & ~used) != 0) return true;
            std::vector<Alt> acc{Alt{}};
            for (std::size_t i = 0; i < terms.size(); ++i) {
                std::vector<Alt> part;
                if (!solveLow(terms[i], want & masks[i], m, n, curOff, env, st, part, depth + 1)) return false;
                if (part.empty()) return true;
                std::vector<Alt> next;
                crossAdd(next, acc, part);
                acc = std::move(next);
            }
            for (Alt& a : acc) {
                if (out.size() >= kMaxAlts) break;
                out.push_back(std::move(a));
            }
            return true;
        }
        if (nd.kind == Kind::Plus || nd.kind == Kind::Sub) {
            const bool plus = nd.kind == Kind::Plus;
            const auto kl = eval(nd.left, n, curOff, env, st, nullptr), kr = eval(nd.right, n, curOff, env, st, nullptr);
            if (kr && !kl) return solveLow(nd.left, plus ? r - static_cast<std::uint64_t>(*kr) : r + static_cast<std::uint64_t>(*kr), m, n, curOff, env, st, out, depth + 1);
            if (kl && !kr)
                return solveLow(nd.right, plus ? r - static_cast<std::uint64_t>(*kl) : static_cast<std::uint64_t>(*kl) - r, m, n, curOff, env, st, out, depth + 1);
        }
        return false;
    }

    void solve(const PatternExpression* e, intb t, const Inst& n, int curOff, const Env& env, const State& st, std::vector<Alt>& out,
               int depth = 0) const {
        if (depth > 30 || out.size() >= kMaxAlts) return;
        if (const auto k = eval(e, n, curOff, env, st, nullptr)) {
            if (*k == t) out.push_back({});
            return;
        }
        const Node& nd = nodeOf(e);
        if (nd.kind == Kind::Leaf) {
            const LeafInfo& li = nd.li;
            const int w = li.width;
            if (w <= 0) return;
            if (w < 63) {
                const intb lo = li.sign ? -(intb{1} << (w - 1)) : 0, hi = li.sign ? (intb{1} << (w - 1)) - 1 : (intb{1} << w) - 1;
                if (t < lo || t > hi) return;
            } else if (!li.sign && t < 0) {
                return;
            }
            const std::uint64_t m = w >= 64 ? ~std::uint64_t{0} : (std::uint64_t{1} << w) - 1;
            out.push_back({LeafSet{li, li.ctx() ? 0 : curOff, static_cast<std::uint64_t>(t) & m}});
            return;
        }
        if (nd.kind == Kind::Operand) {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return;
            if (const PatternExpression* pe = operandExpr(n, idx)) solve(pe, t, n, n.opOff[static_cast<std::size_t>(idx)], env, st, out, depth + 1);
            return;
        }
        if (nd.kind == Kind::Minus || nd.kind == Kind::Not || nd.kind == Kind::OtherUnary) {
            if (nd.kind == Kind::Minus) solve(nd.left, static_cast<intb>(0 - static_cast<std::uint64_t>(t)), n, curOff, env, st, out, depth + 1);
            else if (nd.kind == Kind::Not) solve(nd.left, ~t, n, curOff, env, st, out, depth + 1);
            return;
        }
        if (nd.right != nullptr) {   // (a binary)
            const PatternExpression *l = nd.left, *r = nd.right;
            const auto kl = eval(l, n, curOff, env, st, nullptr), kr = eval(r, n, curOff, env, st, nullptr);
            const auto ut = static_cast<std::uint64_t>(t);
            auto rec = [&](const PatternExpression* x, intb v) { solve(x, v, n, curOff, env, st, out, depth + 1); };
            if (nd.kind == Kind::Plus) {
                if (kr) return rec(l, static_cast<intb>(ut - static_cast<std::uint64_t>(*kr)));
                if (kl) return rec(r, static_cast<intb>(ut - static_cast<std::uint64_t>(*kl)));
            } else if (nd.kind == Kind::Sub) {
                if (kr) return rec(l, static_cast<intb>(ut + static_cast<std::uint64_t>(*kr)));
                if (kl) return rec(r, static_cast<intb>(static_cast<std::uint64_t>(*kl) - ut));
            } else if (nd.kind == Kind::Mult) {
                const PatternExpression* x = kr ? l : (kl ? r : nullptr);
                const auto k = kr ? kr : kl;
                if (x != nullptr) {
                    if (*k == 0) return;
                    if (t % *k != 0) return;
                    return rec(x, t / *k);
                }
            } else if (nd.kind == Kind::LeftShift) {
                if (kr && *kr >= 0 && *kr < 64) {
                    if ((ut & ((std::uint64_t{1} << *kr) - 1)) != 0) return;
                    return rec(l, t >> *kr);
                }
            } else if (nd.kind == Kind::RightShift) {
                if (kr && *kr >= 0 && *kr < 62) return rec(l, static_cast<intb>(ut << *kr));
            } else if (nd.kind == Kind::And) {
                if (kr) {
                    if ((ut & ~static_cast<std::uint64_t>(*kr)) != 0) return;
                    if ((maskOf(l, n, curOff, env, st) & ~static_cast<std::uint64_t>(*kr)) == 0) return rec(l, t);   // (else the and cuts bits off: enumerate)
                    if (solveLow(l, ut, static_cast<std::uint64_t>(*kr), n, curOff, env, st, out, depth + 1)) return;
                } else if (kl) {
                    if ((ut & ~static_cast<std::uint64_t>(*kl)) != 0) return;
                    if ((maskOf(r, n, curOff, env, st) & ~static_cast<std::uint64_t>(*kl)) == 0) return rec(r, t);
                    if (solveLow(r, ut, static_cast<std::uint64_t>(*kl), n, curOff, env, st, out, depth + 1)) return;
                }
            } else if (nd.kind == Kind::Or) {
                if (kr) {
                    if ((ut & static_cast<std::uint64_t>(*kr)) != static_cast<std::uint64_t>(*kr)) return;
                    return rec(l, static_cast<intb>(ut & ~static_cast<std::uint64_t>(*kr)));
                }
                if (kl) {
                    if ((ut & static_cast<std::uint64_t>(*kl)) != static_cast<std::uint64_t>(*kl)) return;
                    return rec(r, static_cast<intb>(ut & ~static_cast<std::uint64_t>(*kl)));
                }
            } else if (nd.kind == Kind::Xor) {
                if (kr) return rec(l, static_cast<intb>(ut ^ static_cast<std::uint64_t>(*kr)));
                if (kl) return rec(r, static_cast<intb>(ut ^ static_cast<std::uint64_t>(*kl)));
            } else if (nd.kind == Kind::Div) {
                if (kr && *kr != 0) return rec(l, t * *kr);
            }
            // both sides have free fields: disjoint bits of a sum or an or ...
            if (nd.kind == Kind::Plus || nd.kind == Kind::Or) {
                const std::uint64_t ma = maskOf(l, n, curOff, env, st), mb = maskOf(r, n, curOff, env, st);
                if ((ma & mb) == 0) {
                    std::vector<Alt> ax, bx;
                    const std::uint64_t ta = ut & ma;
                    solve(l, static_cast<intb>(ta), n, curOff, env, st, ax, depth + 1);
                    solve(r, static_cast<intb>(ut - ta), n, curOff, env, st, bx, depth + 1);
                    crossAdd(out, ax, bx);
                    if (!out.empty()) return;
                }
            }
        }
        // otherwise: enumerate the small fields
        std::vector<Free> fr;
        freeLeaves(e, n, curOff, env, st, fr);
        int bits = 0;
        for (const Free& f : fr) bits += f.li.width;
        if (tracing()) std::fprintf(stderr, "enumerate %s: %zu free leaves, %d bits, target %lld\n", typeid(*e).name(), fr.size(), bits, static_cast<long long>(t));
        if (fr.empty() || bits > 16) return;
        // (what the enumeration finds depends on the expression, the target, the constructor the operand references resolve in, where each
        // leaf is, which leaves are already fixed and to what, and the address when the expression reads it: all of that is the key, so a
        // target solved once for such a state is not enumerated again. The same immediates come back line after line.)
        std::string key;
        const bool memo = !tracing();
        if (memo) {
            key.reserve(96);
            appendRaw(key, e);
            appendRaw(key, n.ct);
            appendRaw(key, t);
            appendRaw(key, curOff);
            bool usesEnv = false;
            signature(e, n, curOff, st, key, usesEnv, 0);
            if (usesEnv) {
                appendRaw(key, env.startVal);
                appendRaw(key, env.nextVal);
            }
            if (const auto hit = enumMemo.find(key); hit != enumMemo.end()) {
                for (const Alt& alt : hit->second) {
                    if (out.size() >= kMaxAlts) break;
                    out.push_back(alt);
                }
                return;
            }
        }
        // (one table of overrides, its entries set again for each value: a value costs an evaluation, no allocation)
        std::vector<Alt> found;
        Overrides ov;
        std::vector<intb*> slot;
        for (const Free& f : fr) slot.push_back(&(ov[{f.e, f.abs}] = 0));
        for (std::uint32_t v = 0; v < (1u << bits) && found.size() < kMaxAlts; ++v) {
            int at = 0;
            for (std::size_t k = 0; k < fr.size(); ++k) {
                const Free& f = fr[k];
                *slot[k] = signExtend((v >> at) & ((std::uint64_t{1} << f.li.width) - 1), f.li.width, f.li.sign);
                at += f.li.width;
            }
            const auto got = eval(e, n, curOff, env, st, &ov);
            if (!got || *got != t) continue;
            Alt alt;
            at = 0;
            for (const Free& f : fr) {
                alt.push_back(LeafSet{f.li, f.abs, (v >> at) & ((std::uint64_t{1} << f.li.width) - 1)});
                at += f.li.width;
            }
            found.push_back(std::move(alt));
        }
        for (const Alt& alt : found) {
            if (out.size() >= kMaxAlts) break;
            out.push_back(alt);
        }
        if (memo) {
            if (enumMemo.size() >= 200000) enumMemo.clear();
            enumMemo.emplace(std::move(key), std::move(found));
        }
    }

    // The answers of the enumeration in solve, by what they depend on (see there).
    mutable std::unordered_map<std::string, std::vector<Alt>> enumMemo;

    template <class T>
    static void appendRaw(std::string& key, const T& v) {
        key.append(reinterpret_cast<const char*>(&v), sizeof v);
    }

    // What the evaluation of `e` reads from the state, in the order freeLeaves meets it: each leaf's place and its value when fixed.
    void signature(const PatternExpression* e, const Inst& n, int curOff, const State& st, std::string& key, bool& usesEnv, int depth) const {
        if (depth > 40) return;
        const Node& nd = nodeOf(e);
        if (nd.kind == Kind::Leaf) {
            const int abs = nd.li.ctx() ? 0 : curOff;
            appendRaw(key, abs);
            const auto known = readLeaf(st, nd.li, abs);
            key.push_back(known ? 1 : 0);
            if (known) appendRaw(key, *known);
            return;
        }
        if (nd.kind == Kind::Start || nd.kind == Kind::End || nd.kind == Kind::Next2) {
            usesEnv = true;
            return;
        }
        if (nd.kind == Kind::Operand) {
            const int idx = nd.index;
            if (idx < 0 || idx >= n.ct->getNumOperands()) return;
            if (const PatternExpression* pe = operandExpr(n, idx)) signature(pe, n, n.opOff[static_cast<std::size_t>(idx)], st, key, usesEnv, depth + 1);
            return;
        }
        if (nd.right != nullptr) {
            signature(nd.left, n, curOff, st, key, usesEnv, depth + 1);
            signature(nd.right, n, curOff, st, key, usesEnv, depth + 1);
        } else if (nd.left != nullptr) {
            signature(nd.left, n, curOff, st, key, usesEnv, depth + 1);
        }
    }

    // ---- the text
    struct Parser {
        const Impl& impl;
        const std::string& text;
        std::map<std::pair<const SubtableSymbol*, std::size_t>, std::vector<Parsed>> memo;
        std::map<std::pair<const SubtableSymbol*, std::size_t>, int> active;   // (how often a subtable is being parsed at a position: a table may hold itself once)
        std::size_t cuts = 0;

        // The numbers the text can start with at `pos` as the translator prints them ("0x1f", "-0x1f"), longest first.
        std::vector<std::pair<std::size_t, intb>> numbers(std::size_t pos) const {
            std::vector<std::pair<std::size_t, intb>> out;
            std::size_t p = pos;
            bool neg = false;
            if (p < text.size() && text[p] == '-') {
                neg = true;
                ++p;
            }
            if (p + 2 > text.size() || text[p] != '0' || text[p + 1] != 'x') return out;
            p += 2;
            std::uint64_t v = 0;
            std::size_t q = p;
            std::vector<std::pair<std::size_t, std::uint64_t>> prefixes;
            while (q < text.size() && std::isxdigit(static_cast<unsigned char>(text[q])) && q - p < 16) {
                const char c = text[q];
                v = v * 16 + static_cast<std::uint64_t>(std::isdigit(static_cast<unsigned char>(c)) ? c - '0' : c - 'a' + 10);
                ++q;
                prefixes.emplace_back(q, v);
            }
            for (auto it = prefixes.rbegin(); it != prefixes.rend(); ++it)
                out.emplace_back(it->first, neg ? -static_cast<intb>(it->second) : static_cast<intb>(it->second));
            return out;
        }

        // `top`: the whole line's parse from its first character, by the constructors of the root table whose own pattern can stand in the
        // context the line starts in. (Placing a tree begins with its root constructor's pattern against that context; one with no
        // alternative that fits has no encoding whatever its operands are, so its operands are not parsed. A root constructor that is
        // met again as an operand at the same position is parsed as any other: this answer is not kept.)
        const std::vector<Parsed>& table(const SubtableSymbol* st, std::size_t pos, const State* top = nullptr) {
            const auto key = std::make_pair(st, pos);
            if (top == nullptr)
                if (auto it = memo.find(key); it != memo.end()) return it->second;
            static const std::vector<Parsed> none;
            if (active[key] >= 2) {
                ++cuts;
                return none;
            }
            ++active[key];
            const std::size_t cutsBefore = cuts;
            std::vector<Parsed> out;
            // (only the constructors that can start with the text's next character, in the table's own order)
            for (const std::uint32_t i : impl.startingWith(st, pos < text.size() ? static_cast<unsigned char>(text[pos]) : 256)) {
                if (out.size() >= kMaxParses) break;
                if (top != nullptr && !impl.startFits(st->getConstructor(i), *top)) continue;
                ctor(st->getConstructor(i), pos, out);
            }
            --active[key];
            if (cuts != cutsBefore || top != nullptr) {   // (a recursion was cut inside: the answer may be short, so it is not kept)
                static thread_local std::vector<Parsed> scratch;
                scratch = std::move(out);
                return scratch;
            }
            return memo.emplace(key, std::move(out)).first->second;
        }

        void ctor(const Constructor* ct, std::size_t pos, std::vector<Parsed>& out) {
            const CtorInfo& ci = impl.infoOf(ct);
            if (!ci.op.empty() && ci.op[0] == -1) {   // (a constructor that starts with a word: the text has to start with it)
                const std::string& lit = ci.lit[0];
                if (pos + lit.size() > text.size() || text.compare(pos, lit.size(), lit) != 0) return;
            }
            std::vector<Slot> slots(static_cast<std::size_t>(ct->getNumOperands()));
            walk(ct, ci, 0, pos, slots, ci.printed, out);
        }

        void walk(const Constructor* ct, const CtorInfo& ci, int piece, std::size_t pos, std::vector<Slot>& slots, const std::vector<bool>& printed,
                  std::vector<Parsed>& out) {
            if (out.size() >= kMaxParses) return;
            if (piece == ct->getNumPrintPieces()) return silent(ct, 0, pos, slots, printed, out);
            const int opIndex = ci.op[static_cast<std::size_t>(piece)];
            if (opIndex == -1) {
                const std::string& lit = ci.lit[static_cast<std::size_t>(piece)];
                if (pos + lit.size() > text.size() || text.compare(pos, lit.size(), lit) != 0) return;
                return walk(ct, ci, piece + 1, pos + lit.size(), slots, printed, out);
            }
            if (opIndex < 0) return;
            const std::size_t idx = static_cast<std::size_t>(opIndex);
            const OperandSymbol* sym = ct->getOperand(static_cast<int>(idx));
            const TripleSymbol* t = sym->getDefiningSymbol();
            auto next = [&](std::size_t np, intb value) {
                const Slot keep = slots[idx];
                slots[idx].sub.reset();
                slots[idx].value = value;
                slots[idx].shown = true;
                walk(ct, ci, piece + 1, np, slots, printed, out);
                slots[idx] = keep;
            };
            if (t == nullptr) {
                if (sym->getDefiningExpression() == nullptr) return walk(ct, ci, piece + 1, pos, slots, printed, out);
                for (const auto& [np, v] : numbers(pos)) next(np, v);
                return;
            }
            switch (t->getType()) {
            case SleighSymbol::subtable_symbol: {
                const SubtableSymbol* sub = static_cast<const SubtableSymbol*>(t);
                const std::vector<Parsed>& got = table(sub, pos);
                const auto kept = memo.find(std::make_pair(sub, pos));
                std::vector<Parsed> own;   // (a table whose recursion was cut is not kept: the answer is copied out of the scratch before the next table overwrites it)
                if (kept == memo.end() || &kept->second != &got) own = got;
                const std::vector<Parsed>& subs = kept != memo.end() && &kept->second == &got ? got : own;
                for (const Parsed& s : subs) {
                    const Slot keep = slots[idx];
                    slots[idx].sub = s.node;
                    walk(ct, ci, piece + 1, s.end, slots, printed, out);
                    slots[idx] = keep;
                }
                return;
            }
            case SleighSymbol::value_symbol:
            case SleighSymbol::context_symbol:
                for (const auto& [np, v] : numbers(pos)) next(np, v);
                return;
            case SleighSymbol::valuemap_symbol: {
                const auto& vt = static_cast<const ValueMapSymbol*>(t)->getValueTable();
                for (const auto& [np, v] : numbers(pos))
                    for (std::size_t k = 0; k < vt.size(); ++k)
                        if (vt[k] == v) next(np, static_cast<intb>(k));
                return;
            }
            case SleighSymbol::name_symbol: {
                const NameList& names = impl.namesOf(t);
                for (std::size_t k = 0; k < names.nm.size(); ++k) {
                    const std::string& nm = names.nm[k];
                    if (!names.ok[k] || pos + nm.size() > text.size() || text.compare(pos, nm.size(), nm) != 0) continue;
                    next(pos + nm.size(), static_cast<intb>(k));
                }
                return;
            }
            case SleighSymbol::varnodelist_symbol: {
                const auto& vt = static_cast<const VarnodeListSymbol*>(t)->getVarnodeTable();
                const NameList& names = impl.registersOf(t, vt);
                for (std::size_t k = 0; k < names.nm.size(); ++k) {
                    const std::string& nm = names.nm[k];
                    if (!names.ok[k] || pos + nm.size() > text.size() || text.compare(pos, nm.size(), nm) != 0) continue;
                    next(pos + nm.size(), static_cast<intb>(k));
                }
                return;
            }
            case SleighSymbol::varnode_symbol: {
                const std::string& nm = impl.nameOf(t);
                if (pos + nm.size() <= text.size() && text.compare(pos, nm.size(), nm) == 0) next(pos + nm.size(), 0);
                return;
            }
            case SleighSymbol::epsilon_symbol:
                if (pos < text.size() && text[pos] == '0') next(pos + 1, 0);
                return;
            case SleighSymbol::start_symbol:
            case SleighSymbol::end_symbol:
            case SleighSymbol::next2_symbol:
                for (const auto& [np, v] : numbers(pos)) next(np, v);
                return;
            default:
                return;
            }
        }

        // Every way a subtable's constructors can stand without any text (the print does not show the operand): each constructor, its
        // own subtable operands the same way, its values left to the bits its pattern fixes.
        std::map<const SubtableSymbol*, std::vector<std::shared_ptr<const PNode>>> phantoms;
        const std::vector<std::shared_ptr<const PNode>>& phantom(const SubtableSymbol* st, int depth) {
            static const std::vector<std::shared_ptr<const PNode>> none;
            if (depth > 5) return none;
            if (auto it = phantoms.find(st); it != phantoms.end()) return it->second;
            std::vector<std::shared_ptr<const PNode>> out;
            std::vector<const Constructor*> order;   // (the plainest constructors first: the cap keeps those)
            for (int i = 0; i < st->getNumConstructors(); ++i) order.push_back(st->getConstructor(static_cast<std::uint32_t>(i)));
            std::stable_sort(order.begin(), order.end(), [](const Constructor* a, const Constructor* b) {
                return a->getNumPrintPieces() + a->getNumOperands() < b->getNumPrintPieces() + b->getNumOperands();
            });
            for (std::size_t oi = 0; oi < order.size() && out.size() < kMaxPhantoms; ++oi) {
                const Constructor* ct = order[oi];
                std::vector<std::vector<std::shared_ptr<const PNode>>> choices(static_cast<std::size_t>(ct->getNumOperands()));
                bool ok = true;
                for (int k = 0; k < ct->getNumOperands() && ok; ++k) {
                    const TripleSymbol* t = ct->getOperand(k)->getDefiningSymbol();
                    if (t != nullptr && t->getType() == SleighSymbol::subtable_symbol) {
                        choices[static_cast<std::size_t>(k)] = phantom(static_cast<const SubtableSymbol*>(t), depth + 1);
                        ok = !choices[static_cast<std::size_t>(k)].empty();
                    }
                }
                if (!ok) continue;
                std::vector<Slot> slots(choices.size());
                std::function<void(std::size_t)> fill = [&](std::size_t k) {
                    if (out.size() >= kMaxPhantoms) return;
                    if (k == slots.size()) {
                        auto node = std::make_shared<PNode>();
                        node->ct = ct;
                        node->slots = slots;
                        out.push_back(std::move(node));
                        return;
                    }
                    if (choices[k].empty()) return fill(k + 1);
                    for (const auto& c : choices[k]) {
                        slots[k].sub = c;
                        fill(k + 1);
                    }
                };
                fill(0);
            }
            return phantoms.emplace(st, std::move(out)).first->second;
        }

        // Subtable operands the print does not show are chosen without text.
        void silent(const Constructor* ct, int from, std::size_t pos, std::vector<Slot>& slots, const std::vector<bool>& printed, std::vector<Parsed>& out) {
            for (int i = from; i < ct->getNumOperands(); ++i) {
                if (printed[static_cast<std::size_t>(i)]) continue;
                const TripleSymbol* t = ct->getOperand(i)->getDefiningSymbol();
                if (t == nullptr || t->getType() != SleighSymbol::subtable_symbol) continue;
                const std::vector<std::shared_ptr<const PNode>> subs = phantom(static_cast<const SubtableSymbol*>(t), 0);
                for (const auto& sn : subs) {
                    const Slot keep = slots[static_cast<std::size_t>(i)];
                    slots[static_cast<std::size_t>(i)].sub = sn;
                    silent(ct, i + 1, pos, slots, printed, out);
                    slots[static_cast<std::size_t>(i)] = keep;
                }
                return;
            }
            auto node = std::make_shared<PNode>();
            node->ct = ct;
            node->slots = slots;
            out.push_back({pos, std::move(node)});
        }
    };

    // ---- laying a tree out as the translator does
    static std::unique_ptr<Inst> build(const PNode* p, int offset) {
        auto n = std::make_unique<Inst>();
        n->ct = p->ct;
        n->p = p;
        n->offset = offset;
        const int nops = p->ct->getNumOperands();
        n->opOff.assign(static_cast<std::size_t>(nops), 0);
        n->opLen.assign(static_cast<std::size_t>(nops), 0);
        n->sub.resize(static_cast<std::size_t>(nops));
        int end = offset + p->ct->getMinimumLength();
        for (int i = 0; i < nops; ++i) {
            const OperandSymbol* sym = p->ct->getOperand(i);
            const int base = sym->getOffsetBase();
            if (base >= i) return nullptr;
            const int off = (base < 0 ? offset : n->opOff[static_cast<std::size_t>(base)] + n->opLen[static_cast<std::size_t>(base)]) +
                            static_cast<int>(sym->getRelativeOffset());
            n->opOff[static_cast<std::size_t>(i)] = off;
            const TripleSymbol* t = sym->getDefiningSymbol();
            if (t != nullptr && t->getType() == SleighSymbol::subtable_symbol) {
                const PNode* sp = p->slots[static_cast<std::size_t>(i)].sub.get();
                if (sp == nullptr) return nullptr;
                n->sub[static_cast<std::size_t>(i)] = build(sp, off);
                if (!n->sub[static_cast<std::size_t>(i)]) return nullptr;
                n->opLen[static_cast<std::size_t>(i)] = n->sub[static_cast<std::size_t>(i)]->length;
            } else {
                n->opLen[static_cast<std::size_t>(i)] = sym->getMinimumLength();
            }
            end = std::max(end, off + n->opLen[static_cast<std::size_t>(i)]);
        }
        n->length = end - offset;
        return n;
    }

    // ---- placing the fields, in the order the translator resolves them
    struct Task {
        enum Kind { Pattern, Context, Value } kind;
        const Inst* n;
        int op;
    };
    static void flatten(const Inst* n, std::vector<Task>& out) {
        out.push_back({Task::Pattern, n, 0});
        out.push_back({Task::Context, n, 0});
        for (int i = 0; i < n->ct->getNumOperands(); ++i) {
            if (n->sub[static_cast<std::size_t>(i)]) flatten(n->sub[static_cast<std::size_t>(i)].get(), out);
            else out.push_back({Task::Value, n, i});
        }
    }

    // Whether any pattern alternative of a constructor placed first (at the instruction's first byte) fits the state the line starts in.
    bool startFits(const Constructor* ct, const State& st) const {
        Inst first;
        first.offset = 0;
        for (const PatAlt& a : altsOf(ct))
            if (fits(st, first, a)) return true;
        return false;
    }

    // Whether applyPattern would succeed, without the copy of the state it changes (most alternatives of a pattern do not fit the bits placed so far).
    static bool fits(const State& st, const Inst& n, const PatAlt& a) {
        for (std::size_t i = 0; i < a.im.size(); ++i) {
            if (a.im[i] == 0) continue;
            const int b = n.offset + static_cast<int>(i);
            if (b >= kMaxInst) return false;
            if (((st.ival[b] ^ a.iv[i]) & st.imask[b] & a.im[i]) != 0) return false;
        }
        for (std::size_t i = 0; i < a.cm.size(); ++i) {
            if (a.cm[i] == 0) continue;
            if (static_cast<int>(i) >= kMaxCtx) return false;
            if (((st.cval[i] ^ a.cv[i]) & st.cmask[i] & a.cm[i]) != 0) return false;
        }
        return true;
    }

    bool applyPattern(State& st, const Inst& n, const PatAlt& a, std::uint8_t* newly) const {
        for (std::size_t i = 0; i < a.im.size(); ++i) {
            const int b = n.offset + static_cast<int>(i);
            if (a.im[i] == 0) continue;
            if (b >= kMaxInst) return false;
            if (((st.ival[b] ^ a.iv[i]) & st.imask[b] & a.im[i]) != 0) return false;
            st.ival[b] = static_cast<std::uint8_t>((st.ival[b] & ~a.im[i]) | (a.iv[i] & a.im[i]));
            st.imask[b] = static_cast<std::uint8_t>(st.imask[b] | a.im[i]);
        }
        for (std::size_t i = 0; i < a.cm.size(); ++i) {
            if (a.cm[i] == 0) continue;
            if (static_cast<int>(i) >= kMaxCtx) return false;
            if (((st.cval[i] ^ a.cv[i]) & st.cmask[i] & a.cm[i]) != 0) return false;
            newly[i] = static_cast<std::uint8_t>(a.cm[i] & ~st.cmask[i]);
            st.cval[i] = static_cast<std::uint8_t>((st.cval[i] & ~a.cm[i]) | (a.cv[i] & a.cm[i]));
            st.cmask[i] = static_cast<std::uint8_t>(st.cmask[i] | a.cm[i]);
        }
        return true;
    }

    void applyContext(State& st, const Inst& n, const Env& env) const {
        for (int i = 0; i < n.ct->getNumContextChanges(); ++i) {
            const auto* op = dynamic_cast<const ContextOp*>(n.ct->getContextChange(i));
            if (op == nullptr) continue;
            const auto v = eval(op->getPatternExpression(), n, n.offset, env, st, nullptr);
            if (tracing()) std::fprintf(stderr, "ctxop line %d word %d mask %08x shift %d: %s value=%lld\n", n.ct->getLineno(), op->getWordIndex(), op->getMask(), op->getShift(), v ? "known" : "unknown", v ? static_cast<long long>(*v) : 0LL);
            const std::uint32_t val = v ? static_cast<std::uint32_t>(*v) << op->getShift() : 0;
            const std::uint32_t mask = op->getMask();
            for (int j = 0; j < st.nlinks;) {   // (a link whose bits are all set again is no longer what the context holds)
                const ContextOp* old = st.links[j].op;
                if (old->getWordIndex() == op->getWordIndex() && (old->getMask() & ~mask) == 0) {
                    for (int q = j; q + 1 < st.nlinks; ++q) st.links[q] = st.links[q + 1];
                    --st.nlinks;
                } else {
                    ++j;
                }
            }
            if (!v && st.nlinks < 8) {
                st.links[st.nlinks].n = &n;
                st.links[st.nlinks].op = op;
                ++st.nlinks;
            }
            for (int k = 0; k < 4; ++k) {
                const std::size_t b = static_cast<std::size_t>(op->getWordIndex() * 4 + k);
                if (b >= kMaxCtx) break;
                const auto m = static_cast<std::uint8_t>(mask >> (24 - 8 * k)), bv = static_cast<std::uint8_t>(val >> (24 - 8 * k));
                st.cval[b] = static_cast<std::uint8_t>((st.cval[b] & ~m) | (bv & m));
                if (v) st.cmask[b] = static_cast<std::uint8_t>(st.cmask[b] | m);
                else st.cmask[b] = static_cast<std::uint8_t>(st.cmask[b] & ~m);
            }
        }
    }

    struct Run {
        const std::vector<Task>& tasks;
        const Env& env;
        int length;
        std::vector<std::vector<std::uint8_t>>& found;
        std::size_t budget;
    };

    // Context bits a constructor just required, that an earlier context change computed from fields: solve the fields for them.
    void resolveLinks(Run& run, std::size_t ti, const State& st, const std::uint8_t* newly, int from) const {
        for (int li = from; li < st.nlinks; ++li) {
            const Link& lk = st.links[li];
            const std::uint32_t mask = lk.op->getMask();
            bool hit = false;
            std::uint32_t word = 0;
            for (int k = 0; k < 4; ++k) {
                const std::size_t b = static_cast<std::size_t>(lk.op->getWordIndex() * 4 + k);
                if (b >= kMaxCtx) break;
                if ((static_cast<std::uint8_t>(mask >> (24 - 8 * k)) & newly[b]) != 0) hit = true;
                word |= static_cast<std::uint32_t>(st.cval[b]) << (24 - 8 * k);
            }
            if (!hit) continue;
            const intb target = static_cast<intb>((word & mask) >> lk.op->getShift());
            std::vector<Alt> alts;
            solve(lk.op->getPatternExpression(), target, *lk.n, lk.n->offset, run.env, st, alts);
            if (tracing()) {
                const auto kv = eval(lk.op->getPatternExpression(), *lk.n, lk.n->offset, run.env, st, nullptr);
                std::fprintf(stderr, "link expr %s known=%d value=%lld\n", typeid(*lk.op->getPatternExpression()).name(), kv ? 1 : 0, kv ? static_cast<long long>(*kv) : 0LL);
            }
            if (tracing())
                std::fprintf(stderr, "link %d (word %d mask %08x shift %d) target %lld -> %zu alts\n", li, lk.op->getWordIndex(), lk.op->getMask(), lk.op->getShift(),
                             static_cast<long long>(target), alts.size());
            for (const Alt& a : alts) {
                State s = st;
                bool ok = true;
                for (const LeafSet& ls : a)
                    if (!writeLeaf(s, ls.li, ls.absOff, ls.raw)) {
                        ok = false;
                        break;
                    }
                if (!ok) continue;
                for (int j = li; j + 1 < s.nlinks; ++j) s.links[j] = s.links[j + 1];
                --s.nlinks;
                std::uint8_t more[kMaxCtx];
                for (int i = 0; i < kMaxCtx; ++i) more[i] = static_cast<std::uint8_t>(newly[i] | (s.cmask[i] & ~st.cmask[i]));
                resolveLinks(run, ti, s, more, 0);
            }
            return;
        }
        place(run, ti, st);
    }

    void place(Run& run, std::size_t ti, const State& st) const {
        if (run.found.size() >= kMaxCandidates || run.budget == 0) return;
        if (ti == run.tasks.size()) {
            run.found.emplace_back(st.ival, st.ival + run.length);
            return;
        }
        const Task& t = run.tasks[ti];
        const Inst& n = *t.n;
        switch (t.kind) {
        case Task::Pattern:
            for (const PatAlt& a : altsOf(n.ct)) {
                if (!tracing() && !fits(st, n, a)) continue;
                State s = st;
                std::uint8_t newly[kMaxCtx] = {};
                const bool applied = applyPattern(s, n, a, newly);
                if (tracing())
                    for (std::size_t i = 0; i < a.cm.size(); ++i) std::fprintf(stderr, "   ctx byte %zu: mask %02x value %02x\n", i, a.cm[i], a.cv[i]);
                if (tracing())
                    std::fprintf(stderr, "pattern line %d off %d applied=%d links=%d\n", n.ct->getLineno(), n.offset, applied ? 1 : 0, s.nlinks);
                if (applied) {
                    if (s.nlinks == 0) place(run, ti + 1, s);
                    else resolveLinks(run, ti + 1, s, newly, 0);
                }
            }
            return;
        case Task::Context: {
            if (n.ct->getNumContextChanges() == 0) return place(run, ti + 1, st);   // (nothing to apply: no copy of the state)
            State s = st;
            applyContext(s, n, run.env);
            return place(run, ti + 1, s);
        }
        case Task::Value: {
            --run.budget;
            const OperandSymbol* sym = n.ct->getOperand(t.op);
            const TripleSymbol* tr = sym->getDefiningSymbol();
            const PatternExpression* pe = sym->getDefiningExpression();
            if (pe == nullptr && tr != nullptr) {
                switch (tr->getType()) {
                case SleighSymbol::value_symbol:
                case SleighSymbol::context_symbol:
                case SleighSymbol::valuemap_symbol:
                case SleighSymbol::name_symbol:
                case SleighSymbol::varnodelist_symbol:
                    pe = static_cast<const ValueSymbol*>(tr)->getPatternValue();
                    break;
                default:
                    break;
                }
            }
            if (pe == nullptr || !n.p->slots[static_cast<std::size_t>(t.op)].shown) return place(run, ti + 1, st);   // (nothing to solve: a fixed register, or a value the text did not show)
            std::vector<Alt> alts;
            solve(pe, n.p->slots[static_cast<std::size_t>(t.op)].value, n, n.opOff[static_cast<std::size_t>(t.op)], run.env, st, alts);
            if (tracing())
                std::fprintf(stderr, "value op %d of %s: value %lld off %d -> %zu alts\n", t.op, sym->getName().c_str(), static_cast<long long>(n.p->slots[static_cast<std::size_t>(t.op)].value), n.opOff[static_cast<std::size_t>(t.op)], alts.size());
            for (const Alt& a : alts) {
                State s = st;
                bool ok = true;
                for (const LeafSet& ls : a)
                    if (!writeLeaf(s, ls.li, ls.absOff, ls.raw)) {
                        ok = false;
                        break;
                    }
                if (!ok) continue;
                if (s.nlinks == 0) {   // (no context change waits on a field: nothing to resolve)
                    place(run, ti + 1, s);
                    continue;
                }
                std::uint8_t newly[kMaxCtx];
                for (int i = 0; i < kMaxCtx; ++i) newly[i] = static_cast<std::uint8_t>(s.cmask[i] & ~st.cmask[i]);
                resolveLinks(run, ti + 1, s, newly, 0);
            }
            return;
        }
        }
    }

    // ---- verifying: the same translator reads the bytes back
    bool roundTrips(const std::vector<std::uint8_t>& bytes, std::uint64_t address, const std::string& want) {
        window.set(bytes.data(), bytes.size(), address);
        trans->forgetParses();   // (the translator keeps the parse of an address it has read; these are other bytes)
        Text text;
        int4 length = 0;
        try {
            length = trans->printAssembly(text, Address(space, address));
        } catch (const LowlevelError&) {
            return false;
        }
        if (static_cast<std::size_t>(length) != bytes.size()) return false;
        return spaced(text.mnemonic + " " + text.operands) == want;
    }
};

Assembler::Assembler(const std::filesystem::path& sla, const std::vector<std::pair<std::string, int>>& context) : impl_(std::make_unique<Impl>()) {
    initialize_once();
    if (!std::filesystem::is_regular_file(sla)) throw std::runtime_error("no processor description " + sla.string());
    impl_->initial = context;
    impl_->trans = std::make_unique<Sleigh>(&impl_->window, &impl_->context);
    try {
        std::istringstream name("<sleigh>" + sla.generic_string() + "</sleigh>");
        Element* root = impl_->store.parseDocument(name)->getRoot();
        impl_->store.registerTag(root);
        impl_->trans->initialize(impl_->store);
        for (const auto& [variable, value] : context) impl_->context.setVariableDefault(variable, static_cast<uintm>(value));
    } catch (const LowlevelError& e) {
        throw std::runtime_error(sla.string() + ": " + e.explain);
    }
    impl_->space = impl_->trans->getDefaultCodeSpace();
    impl_->ctxBytes = impl_->context.getContextSize() * 4;
}

Assembler::~Assembler() = default;

Assembler::Diagnostics Assembler::last() const { return impl_->diag; }

std::vector<std::vector<std::uint8_t>> Assembler::assemble(std::string_view line, std::uint64_t address) const { return run(line, address, false); }

std::vector<std::vector<std::uint8_t>> Assembler::assemble_first(std::string_view line, std::uint64_t address) const { return run(line, address, true); }

std::vector<std::vector<std::uint8_t>> Assembler::run(std::string_view line, std::uint64_t address, bool first) const {
    Impl& I = *impl_;
    std::lock_guard hold(I.lock);
    I.diag = {};
    const std::string want = squash(line);
    std::vector<std::vector<std::uint8_t>> results;
    if (want.empty()) return results;

    State start;
    {
        // Every line is assembled in the context the assembler began with. Disassembling a candidate to verify it (roundTrips) commits what an
        // instruction changes for the ones after it (a 65816's REP and SEP: the widths m16 and x16), and that would stay in this database for
        // the next line at these addresses; a source says its context itself (`.context`), so what an earlier line left is put back.
        {
            const uintm* defaults = static_cast<const ContextDatabase&>(I.context).defaultValues();
            const Address from(I.space, address), to(I.space, address + static_cast<std::uint64_t>(kMaxInst));
            for (int w = 0; w < I.context.getContextSize(); ++w) I.context.setContextRegion(from, to, w, ~static_cast<uintm>(0), defaults[w]);
        }
        const uintm* words = I.context.getContext(Address(I.space, address));
        const int n = std::min(I.ctxBytes, kMaxCtx);
        for (int i = 0; i < n; ++i) {
            start.cval[i] = static_cast<std::uint8_t>(words[i / 4] >> (24 - 8 * (i % 4)));
            start.cmask[i] = 0xFF;
        }
    }
    Impl::Parser parser{I, want, {}, {}, 0};
    std::vector<Parsed> roots;
    {
        const std::vector<Parsed>& all = parser.table(I.trans->getRoot(), 0, &start);
        for (const Parsed& p : all)
            if (p.end == want.size()) roots.push_back(p);
    }
    I.diag.parses = roots.size();

    const int ws = static_cast<int>(I.space->getWordSize());
    std::set<std::vector<std::uint8_t>> tried;
    for (const Parsed& r : roots) {
        std::unique_ptr<Inst> inst = Impl::build(r.node.get(), 0);
        if (!inst || inst->length <= 0 || inst->length > kMaxInst) continue;
        // (a caller that takes only the first of the shortest: a tree no shorter than what is found cannot change it)
        if (first && !results.empty() && static_cast<std::size_t>(inst->length) >= results.front().size()) continue;
        Env env;
        env.startVal = static_cast<intb>(AddrSpace::byteToAddress(address, static_cast<uint4>(ws)));
        env.nextVal = static_cast<intb>(AddrSpace::byteToAddress(I.space->wrapOffset(address + static_cast<std::uint64_t>(inst->length)), static_cast<uint4>(ws)));
        std::vector<Impl::Task> tasks;
        Impl::flatten(inst.get(), tasks);
        std::vector<std::vector<std::uint8_t>> found;
        Impl::Run run{tasks, env, inst->length, found, 20000};
        I.place(run, 0, start);
        for (auto& bytes : found) {
            if (!tried.insert(bytes).second) continue;
            ++I.diag.candidates;
            if (I.roundTrips(bytes, address, spaced(line))) {
                ++I.diag.verified;
                if (first) {
                    results.assign(1, bytes);   // (this tree is shorter than any found before it: the others of its own length come later)
                    break;
                }
                results.push_back(bytes);
            }
        }
        if (tried.size() >= kMaxCandidates) break;
    }
    std::stable_sort(results.begin(), results.end(), [](const auto& a, const auto& b) { return a.size() < b.size(); });
    return results;
}

}  // namespace cb::decompile::sleigh
