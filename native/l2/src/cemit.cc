// cemit.cc -- the layout of backends/c.py: _Pretty (the core's EmitPrettyPrint over EmitNoMarkup:
// the same queue of tokens, lines of 100), _Flat (the same calls on one line), the core's operator
// tokens and PrintLanguage's reverse-Polish stack (_Stack).
#include "cback.hpp"

#include <mutex>

namespace cb {

thread_local Now* now = nullptr;

size_t cp_len(const std::string& s) {
    size_t n = 0;
    for (unsigned char c : s)
        if ((c & 0xc0) != 0x80) n++;
    return n;
}

std::string pyx(int64_t v) { return v < 0 ? "-" + py::hex((uint64_t)(-(v + 1)) + 1) : py::hex((uint64_t)v); }

// ---------------------------------------------------------------------------------------------- _Flat
void Flat::print(const std::string& text, const Opt& shown) {
    out.push_back(shown ? *shown : text);
    raw.push_back(text);
}
void Flat::open_paren(const std::string& p) {
    out.push_back(p);
    raw.push_back(p);
}
void Flat::close_paren(const std::string& p) { open_paren(p); }
void Flat::spaces(int n, int) {
    out.push_back(py::spaces(n));
    raw.push_back(py::spaces(n));
}
void Flat::tag_line(std::optional<int>) {
    out.push_back("\n");
    raw.push_back("\n");
}
std::string Flat::text() {
    std::string s;
    for (auto& x : out) s += x;
    return s;
}

// ---------------------------------------------------------------------------------------------- _Pretty
void Pretty::push(int cls, long size, const std::string& text, long sp, long bump, bool absolute) {
    toks.push_back({cls, size, text, sp, bump, absolute});
    scan();
}

void Pretty::scan() {
    size_t ref = toks.size() - 1;
    Tok& tok = toks[ref];
    int c = tok.cls;
    if (c == BEGIN || c == COMMENT) {
        if (scanq.empty()) left = right = 1;
        tok.size = -right;
        scanq.push_back(ref);
    } else if (c == END || c == UNCOMMENT) {
        tok.size = 0;
        if (!scanq.empty()) {
            Tok& r = toks[scanq.back()];
            scanq.pop_back();
            r.size += right;
            if (r.cls == BREAK && !scanq.empty()) {
                toks[scanq.back()].size += right;
                scanq.pop_back();
            }
            if (scanq.empty()) advance();
        }
    } else if (c == BREAK) {
        if (scanq.empty())
            left = right = 1;
        else {
            Tok& r = toks[scanq.back()];
            if (r.cls == BREAK) {
                scanq.pop_back();
                r.size += right;
            }
        }
        toks[ref].size = -right;
        scanq.push_back(ref);
        right += toks[ref].spaces;
    } else if (c == INDENT || c == UNINDENT) {
        tok.size = 0;
    } else if (!scanq.empty()) {           // a string
        right += tok.size;
        while (right - left > room) {
            toks[scanq.front()].size = 999999;
            scanq.pop_front();
            advance();
            if (scanq.empty()) break;
        }
    }
}

void Pretty::advance() {
    while (lo < toks.size() && toks[lo].size >= 0) {
        Tok tok = toks[lo];
        printtok(tok);
        if (tok.cls == BREAK) left += tok.spaces;
        else if (tok.cls == STRING) left += tok.size;
        lo++;
    }
}

void Pretty::line() {
    outv.push_back("\n" + py::spaces(width - room));
    if (commentmode && !fill.empty()) {
        outv.push_back(fill);
        room -= (long)cp_len(fill);
    }
}

void Pretty::overflow() {
    long half = width / 2;
    for (long i = (long)stack.size() - 1; i >= 0; i--) {
        if (stack[(size_t)i] < half) stack[(size_t)i] = half;
        else break;
    }
    long r = stack.empty() ? width : stack.back();
    if (r == room || (commentmode && r == room + (long)fill.size())) return;
    room = r;
    line();
}

void Pretty::printtok(const Tok& tok) {
    int c = tok.cls;
    if (c == INDENT) {
        stack.push_back(stack.back() - tok.bump);
    } else if (c == BEGIN || c == COMMENT) {
        commentmode = commentmode || c == COMMENT;
        stack.push_back(room);
    } else if (c == UNINDENT) {
        stack.pop_back();
    } else if (c == END || c == UNCOMMENT) {
        commentmode = commentmode && c != UNCOMMENT;
        stack.pop_back();
    } else if (c == STRING) {
        if (tok.size > room) overflow();
        outv.push_back(tok.text);
        room -= tok.size;
    } else if (tok.size > room) {      // a break where the line is full
        if (tok.absolute) {
            room = width - tok.bump;
        } else {
            long val = stack.back() - tok.bump;
            if (tok.spaces <= room && val - room < 10) {
                outv.push_back(py::spaces(tok.spaces));
                room -= tok.spaces;
                return;
            }
            stack.back() = room = val;
        }
        line();
    } else {
        outv.push_back(py::spaces(tok.spaces));
        room -= tok.spaces;
    }
}

void Pretty::start_() {
    if (needbreak) push(BREAK);
    needbreak = false;
}
void Pretty::string_() {
    if (needbreak) push(BREAK);
    needbreak = true;
}
void Pretty::end_() {
    if (!needbreak) push(STRING);
    needbreak = true;
}
void Pretty::break_() {
    if (!needbreak) push(STRING);
    needbreak = false;
}

void Pretty::print(const std::string& text, const Opt& shown) {
    string_();
    push(STRING, (long)cp_len(text), shown ? *shown : text);
}
void Pretty::open_group() {
    start_();
    push(BEGIN);
}
void Pretty::close_group() {
    end_();
    push(END);
}
void Pretty::open_paren(const std::string& p) {
    open_group();
    push(STRING, 1, p);
    needbreak = true;
}
void Pretty::close_paren(const std::string& p) {
    string_();
    push(STRING, 1, p);
    close_group();
}
void Pretty::spaces(int n, int bump) {
    break_();
    push(BREAK, 0, "", n, bump);
}
void Pretty::tag_line(std::optional<int> ind) {
    break_();
    push(BREAK, 0, "", 999999, ind ? *ind : 0, ind.has_value());
}
void Pretty::start_indent() { push(INDENT, 0, "", 0, indent); }
void Pretty::stop_indent() { push(UNINDENT); }
void Pretty::start_comment() {
    start_();
    push(COMMENT);
}
void Pretty::stop_comment() {
    end_();
    push(UNCOMMENT);
}
std::string Pretty::text() {
    for (size_t k = lo; k < toks.size(); k++) {
        Tok t = toks[k];
        printtok(t);
    }
    lo = toks.size();
    std::string s;
    for (auto& x : outv) s += x;
    return s;
}

// ---------------------------------------------------------------------------------------------- the core's operators
static const int ASSIGN_PREC = 14;
const Op HIDDEN{"", "", 1, 70, false, "hiddenfunction", 0, 0};
const Op SUBSCRIPT{"[", "]", 2, 66, false, "postsurround", 0, 0};
const Op CALL{"(", ")", 2, 66, false, "postsurround", 0, 10};
const Op CAST{"(", ")", 2, 62, false, "presurround", 0, 0};
const Op TYPE_SPACE{"", "", 2, 10, false, "space", 1, 0};
const Op TYPE_NOSPACE{"", "", 2, 10, false, "space", 0, 0};
const Op PTR{"*", "", 1, 62, false, "unary_prefix", 0, 0};
const Op ARRAY{"[", "]", 2, 66, false, "postsurround", 1, 0};
const Op WHOLE{"", "", 1, 100, false, "whole", 0, 0};

static const Op MEMBER_DOT{".", "", 2, 66, true, "binary", 0, 0};
static const Op MEMBER_ARROW{"->", "", 2, 66, true, "binary", 0, 0};
const Op* member_op(bool a) { return a ? &MEMBER_ARROW : &MEMBER_DOT; }

const Op* unary_op(const std::string& op) {
    static const std::map<std::string, Op> m = [] {
        std::map<std::string, Op> x;
        for (const char* o : {"~", "!", "-", "+", "&", "*"}) x[o] = Op{o, "", 1, 62, false, "unary_prefix", 0, 0};
        return x;
    }();
    auto it = m.find(op);
    return it == m.end() ? nullptr : &it->second;
}
const Op* postfix_op(const std::string& op) {
    static const Op inc{"++", "", 1, 66, false, "unary_postfix", 0, 0}, dec{"--", "", 1, 66, false, "unary_postfix", 0, 0};
    return op == "++" ? &inc : op == "--" ? &dec : nullptr;
}

const std::map<std::string, int>& binary_prec() {
    static const std::map<std::string, int> m{{",", 2}, {"||", 18}, {"&&", 22}, {"|", 26}, {"^", 30}, {"&", 34},
                                              {"==", 38}, {"!=", 38}, {"<", 42}, {"<=", 42}, {">", 42}, {">=", 42},
                                              {"<<", 46}, {">>", 46}, {"+", 50}, {"-", 50}, {"*", 54}, {"/", 54}, {"%", 54}};
    return m;
}
bool associative(const std::string& op) {
    return op == "," || op == "|" || op == "^" || op == "&" || op == "+" || op == "*" || op == "." || op == "->";
}

const Op* binop(const std::string& op) {
    static const std::map<std::string, Op> m = [] {
        std::map<std::string, Op> x;
        for (auto& kv : binary_prec())
            x[kv.first] = Op{kv.first, "", 2, kv.second, associative(kv.first), "binary", kv.first == "," ? 0 : 1, 0};
        return x;
    }();
    auto it = m.find(op);
    if (it == m.end()) py::raise("KeyError", py::str_repr(op));
    return &it->second;
}

const Op* assign_op(const V& op) {
    static std::mutex mu;
    static std::map<std::string, std::unique_ptr<Op>> m;
    std::string word = (py::truthy(op) ? op.str() : std::string()) + "=";
    std::lock_guard<std::mutex> lock(mu);
    auto& slot = m[word];
    if (!slot) slot.reset(new Op{word, "", 2, ASSIGN_PREC, false, "binary", 1, 5});
    return slot.get();
}

// ---------------------------------------------------------------------------------------------- _Stack
void Stack::op(const Op* tok, std::optional<bool> paren) {
    bool p;
    if (ops.empty())
        p = false;
    else {
        emit(ops.back());
        p = paren ? *paren : parens(tok);
    }
    if (p) em->open_paren("(");
    else em->open_group();
    ops.push_back({tok, 0, p});
}

void Stack::atom(const std::string& text, const Opt& shown, bool blank, const V* node) {
    if (!ops.empty()) emit(ops.back());
    if (node && now->seeing && !now->raw && em->flat()) now->at[id(*node)] = {static_cast<Flat*>(em), static_cast<Flat*>(em)->out.size()};
    if (!blank) em->print(text, shown);
    while (!ops.empty()) {
        Entry& top = ops.back();
        top.placed += 1;
        if (top.placed != top.tok->stage) break;
        emit(top);
        if (top.tok->kind == "unary_postfix") em->print(top.tok->print1);
        if (top.paren) em->close_paren(")");
        else em->close_group();
        ops.pop_back();
    }
}

void Stack::whole(const Op* tok, const std::string& raw, const Opt& shown, std::optional<bool> paren, const V* node) {
    if (!tok) {
        atom(raw, shown, false, node);
        return;
    }
    bool p;
    if (ops.empty())
        p = false;
    else {
        emit(ops.back());
        p = paren ? *paren : parens(tok);
    }
    if (p) em->open_paren("(");
    else em->open_group();
    ops.push_back({&WHOLE, 0, p});
    atom(raw, shown, false, node);
}

bool Stack::parens(const Op* op2) {
    const Op* tok = ops.back().tok;
    int stage = ops.back().placed;
    const std::string& k = tok->kind;
    if (k == "hiddenfunction") {
        if (stage == 0 && ops.size() > 1) {
            const Op* prev = ops[ops.size() - 2].tok;
            if ((prev->kind != "binary" && prev->kind != "unary_prefix") || prev->prec < op2->prec) return false;
        }
        return true;
    }
    if ((k == "postsurround" && stage == 1) || (k == "presurround" && stage == 0)) return false;
    if (tok->prec != op2->prec) return tok->prec > op2->prec;
    if (k == "space" || k == "binary") {
        if (tok->assoc && tok == op2) return false;
        return !(op2->kind == "postsurround" && stage == 0);
    }
    if (k == "unary_prefix" || k == "presurround") return op2->kind != "unary_prefix" && op2->kind != "presurround";
    return op2->kind != "postsurround" && op2->kind != "binary";
}

void Stack::emit(const Entry& entry) {
    const Op* tok = entry.tok;
    int v = entry.placed;
    const std::string& k = tok->kind;
    if (k == "binary" && v == 1) {
        em->spaces(tok->spacing, tok->bump);
        em->print(tok->print1);
        em->spaces(tok->spacing, tok->bump);
    } else if (k == "unary_prefix" && v == 0) {
        em->print(tok->print1);
        em->spaces(tok->spacing, tok->bump);
    } else if (k == "postsurround" && v == 1) {
        em->spaces(tok->spacing, tok->bump);
        em->open_paren(tok->print1);
        em->spaces(0, tok->bump);
    } else if (k == "postsurround" && v == 2) {
        em->close_paren(tok->print2);
    } else if (k == "presurround" && v == 0) {
        em->open_paren(tok->print1);
    } else if (k == "presurround" && v == 1) {
        em->close_paren(tok->print2);
        em->spaces(tok->spacing, tok->bump);
    } else if (k == "space" && v == 1) {
        em->spaces(tok->spacing, tok->bump);
    }
}

}  // namespace cb
