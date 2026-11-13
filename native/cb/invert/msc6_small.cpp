#include "cb/invert/msc6_small.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <format>
#include <optional>
#include <set>

#include "cb/core/intmath.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/platforms/decoder.hpp"

namespace cb::invert::msc6_small {

namespace {

namespace small_cs = cb::platforms;

// ---- the frame --------------------------------------------------------------------------------------------------------------

constexpr std::array<std::uint8_t, 10> kProlog{0x1e, 0x58, 0x90, 0x45, 0x55, 0x8b, 0xec, 0x1e, 0x8e, 0xd8};   // push ds; pop ax; nop; inc bp; push bp; mov bp,sp; push ds; mov ds,ax
constexpr std::array<std::uint8_t, 7> kEpilog{0x4d, 0x4d, 0x8b, 0xe5, 0x1f, 0x5d, 0x4d};                      // dec bp; dec bp; mov sp,bp; pop ds; pop bp; dec bp
constexpr std::uint8_t kFarCall = 0x9a, kFarReturn = 0xcb, kFarReturnPop = 0xca;

// The bytes of a function, read with their bounds: a reader that reaches past the end has met a function that is not the one it knows. This is the
// module's one boundary: every reader below catches what `at` throws and declines with it.
class Code {
public:
    explicit Code(ByteView bytes) : b_(bytes) {}
    std::size_t size() const { return b_.size(); }
    ByteView view() const { return b_; }

    std::uint8_t at(std::size_t i) const {
        if (i >= b_.size()) throw Declined("the code ends inside a statement");
        return b_[i];
    }
    std::uint32_t u16(std::size_t i) const { return at(i) | (static_cast<std::uint32_t>(at(i + 1)) << 8); }
    // Do these bytes stand at `i`?
    bool is(std::size_t i, std::initializer_list<std::uint8_t> bytes) const {
        if (i > b_.size() || b_.size() - i < bytes.size()) return false;
        return std::equal(bytes.begin(), bytes.end(), b_.begin() + static_cast<std::ptrdiff_t>(i));
    }
    bool is_epilog(std::size_t i) const { return i <= b_.size() && b_.size() - i >= kEpilog.size() && std::equal(kEpilog.begin(), kEpilog.end(), b_.begin() + static_cast<std::ptrdiff_t>(i)); }
    bool starts_with_prolog() const { return b_.size() >= kProlog.size() && std::equal(kProlog.begin(), kProlog.end(), b_.begin()); }
    // b[from:to], clamped to the code; empty when `to` is not past `from`.
    ByteView slice(std::size_t from, std::size_t to) const {
        const std::size_t lo = std::min(from, b_.size()), hi = std::min(to, b_.size());
        return hi > lo ? b_.subspan(lo, hi - lo) : ByteView();
    }
    // The first and the last place the epilogue stands at or after `from`.
    std::optional<std::size_t> find_epilog(std::size_t from) const {
        for (std::size_t i = from; i + kEpilog.size() <= b_.size(); ++i)
            if (is_epilog(i)) return i;
        return std::nullopt;
    }
    std::optional<std::size_t> rfind_epilog() const {
        for (std::size_t i = b_.size() < kEpilog.size() ? 0 : b_.size() - kEpilog.size() + 1; i-- > 0;)
            if (is_epilog(i)) return i;
        return std::nullopt;
    }

private:
    ByteView b_;
};

// The body starts after the prologue and the frame (`sub sp,N`, three bytes) if there is one.
std::size_t body_start(const Code& code) { return code.is(kProlog.size(), {0x83, 0xec}) ? kProlog.size() + 3 : kProlog.size(); }

std::string hex_bytes(ByteView b) {
    std::string out;
    for (const std::uint8_t x : b) out += std::format("{:02x}", x);
    return out;
}
std::string signed16(std::uint32_t v) { return std::to_string(v >= 0x8000 ? static_cast<int>(v) - 0x10000 : static_cast<int>(v)); }
std::string signed8(std::uint32_t v) { return std::to_string(v >= 0x80 ? static_cast<int>(v) - 0x100 : static_cast<int>(v)); }
// The parameter a frame slot is: [bp+6] is a0, [bp+8] a1 ... (the far frame keeps bp, ds, the return address)
std::string parameter(std::uint32_t slot) { return "a" + std::to_string(floor_div(static_cast<int>(slot) - 6, 2)); }
// The register readers name the parameters a, b, c ... (the readers of statements name them a0, a1 ...).
std::string lettered_parameter(std::uint32_t slot) { return std::string(1, static_cast<char>('a' + floor_div(static_cast<int>(slot) - 6, 2))); }
std::string join_with(const std::vector<std::string>& parts, std::string_view sep) { return cb::text::join(parts, sep); }

// What a global is declared as.
std::string c_type(Width w) { return w == Width::Long ? "long" : "int"; }

// The globals a walk names, in the order of their first mention; a name said again takes the latest width.
class GlobalList {
public:
    void note(const std::string& name, Width width) {
        const auto it = std::ranges::find(items_, name, &std::pair<std::string, Width>::first);
        if (it == items_.end()) items_.emplace_back(name, width);
        else it->second = width;
    }
    const std::vector<std::pair<std::string, Width>>& items() const { return items_; }

private:
    std::vector<std::pair<std::string, Width>> items_;
};

// ---- the walker -------------------------------------------------------------------------------------------------------------------

struct Step {
    std::vector<std::string> lines;
    std::size_t next = 0;
};

struct Pushes {
    std::vector<std::string> arguments;
    std::size_t next = 0;
};

struct WalkState {
    const Globals& globals;
    GlobalList named;
    bool calls = false;
};

Pushes read_pushes(const Code& code, std::size_t p, std::size_t end, WalkState& state) {
    Pushes out;
    while (p < end) {
        const std::uint8_t c = code.at(p);
        if (c == 0x6a) {                                        // push imm8
            out.arguments.push_back(signed8(code.at(p + 1)));
            p += 2;
        } else if (c == 0x68) {                                 // push imm16
            out.arguments.push_back(std::to_string(code.u16(p + 1)));
            p += 3;
        } else if (code.is(p, {0xff, 0x76})) {                  // push [bp+n]
            out.arguments.push_back(parameter(code.at(p + 2)));
            p += 3;
        } else if (code.is(p, {0xff, 0x36})) {                  // push [m]: a word of a global, or one half of a long
            const std::uint32_t at = code.u16(p + 2);
            const auto here = state.globals.find(at);
            const auto before = at >= 2 ? state.globals.find(at - 2) : state.globals.end();
            if (here != state.globals.end() && here->second.width == Width::Long) {
                state.named.note(here->second.name, Width::Long);
                out.arguments.push_back("(WORD)" + here->second.name);
            } else if (before != state.globals.end() && before->second.width == Width::Long) {
                state.named.note(before->second.name, Width::Long);
                out.arguments.push_back("(WORD)(" + before->second.name + " >> 16)");
            } else if (here != state.globals.end()) {
                state.named.note(here->second.name, Width::Word);
                out.arguments.push_back(here->second.name);
            } else {
                const std::string name = std::format("gW{:x}", at);
                state.named.note(name, Width::Word);
                out.arguments.push_back(name);
            }
            p += 4;
        } else {
            break;
        }
    }
    out.next = p;
    return out;
}

Result<std::vector<std::string>> walk_block(const Code& code, std::size_t p, std::size_t end, WalkState& state);

Result<Step> walk_statement(const Code& code, std::size_t p, std::size_t end, WalkState& state) {
    const std::size_t first = p;
    const Pushes pushed = read_pushes(code, p, end, state);
    const std::size_t q = pushed.next;
    if (q < end && code.at(q) == kFarCall) {                    // CALLF: the call, its arguments pushed before it
        state.calls = true;
        return Step{{"CALLEE(" + join_with(pushed.arguments, ", ") + ");"}, q + 5};
    }
    if (code.is(p, {0x2b, 0xc0}) && code.at(p + 2) == 0xa3 && code.at(p + 5) == 0xa3) {   // sub ax,ax; mov [m],ax; mov [m2],ax: a long cleared
        const auto named = state.globals.find(code.u16(p + 6));
        if (named != state.globals.end() && named->second.width == Width::Long) {
            state.named.note(named->second.name, Width::Long);
            return Step{{named->second.name + " = 0L;"}, p + 8};
        }
    }
    if (code.is(p, {0xc7, 0x06})) {                             // mov [m],imm16
        const std::uint32_t at = code.u16(p + 2);
        const auto named = state.globals.find(at);
        const std::string name = named != state.globals.end() ? named->second.name : std::format("gW{:x}", at);
        state.named.note(name, Width::Word);
        return Step{{name + " = " + std::to_string(code.u16(p + 4)) + ";"}, p + 6};
    }
    if (code.is(p, {0xb8}) && code.is_epilog(p + 3)) return Step{{"return " + signed16(code.u16(p + 1)) + ";"}, p + 3};
    if (code.is(p, {0x8b, 0x46}) && code.is_epilog(p + 3)) return Step{{"return " + parameter(code.at(p + 2)) + ";"}, p + 3};
    if (code.is(p, {0xa1}) && code.is(p + 3, {0x0b, 0x06}) && code.at(p + 7) == 0x74) {   // mov ax,[hi]; or ax,[lo]; je skip: if (p != 0L)
        const auto named = state.globals.find(code.u16(p + 5));
        if (named != state.globals.end() && named->second.width == Width::Long) {
            state.named.note(named->second.name, Width::Long);
            const std::size_t skip = p + 9 + code.at(p + 8);
            Result<std::vector<std::string>> then = walk_block(code, p + 9, skip, state);
            if (!then.ok()) return Declined("then-block unmodeled");
            std::vector<std::string> lines{"if (" + named->second.name + " != 0L) {"};
            for (const std::string& line : then.value()) lines.push_back("    " + line);
            lines.emplace_back("}");
            return Step{std::move(lines), skip};
        }
    }
    return Declined(std::format("unmodeled @+{}: {}", p - first, hex_bytes(code.slice(p, p + 6))));
}

Result<std::vector<std::string>> walk_block(const Code& code, std::size_t p, std::size_t end, WalkState& state) {
    std::vector<std::string> out;
    while (p < end) {
        Result<Step> step = walk_statement(code, p, end, state);
        if (!step.ok()) return Failure(step.shared_error());
        out.insert(out.end(), step.value().lines.begin(), step.value().lines.end());
        p = step.value().next;
    }
    return out;
}

// ---- the leaf templates -----------------------------------------------------------------------------------------------------

// The comparison a conditional jump makes true when the compiler jumped over the then-part: the opposite of the jump's own.
std::optional<std::string_view> opposite_test(std::uint8_t jump) {
    switch (jump) {
    case 0x74: return "!=";   // je
    case 0x75: return "==";   // jne
    case 0x7c: return ">=";   // jl
    case 0x7d: return "<";    // jge
    case 0x7e: return ">";    // jle
    case 0x7f: return "<=";   // jg
    case 0x72: return ">=";   // jb
    case 0x73: return "<";    // jae
    case 0x76: return ">";    // jbe
    case 0x77: return "<=";   // ja
    default: return std::nullopt;
    }
}

// A leaf's one statement: return a constant, a parameter or the global gG.
std::optional<std::string> leaf_statement(ByteView body) {
    if (body.size() == 3 && body[0] == 0xb8) return "return " + signed16(body[1] | (body[2] << 8)) + ";";
    if (body.size() == 3 && body[0] == 0x8b && body[1] == 0x46) return "return " + parameter(body[2]) + ";";
    if (body.size() == 3 && body[0] == 0xa1) return std::string("return gG;");
    return std::nullopt;
}

// `cmp [bp+n],k; jcc over; THEN [jmp short]; REST`: the if and what follows it, to the end of the body.
std::optional<std::vector<std::string>> leaf_branches(ByteView body) {
    if (body.size() >= 6 && body[0] == 0x83 && body[1] == 0x7e) {
        if (const std::optional<std::string_view> test = opposite_test(body[4])) {
            const int value = body[3] < 0x80 ? body[3] : body[3] - 0x100;
            const int rel = body[5] < 0x80 ? body[5] : body[5] - 0x100;
            const int skip = 6 + rel;
            if (skip < 6 || static_cast<std::size_t>(skip) > body.size()) return std::nullopt;   // (a jump out of the body: not a template)
            ByteView then = body.subspan(6, static_cast<std::size_t>(skip) - 6);
            if (then.size() >= 2 && then[then.size() - 2] == 0xeb) then = then.first(then.size() - 2);   // the jump over the else
            const ByteView rest = body.subspan(static_cast<std::size_t>(skip));
            const std::optional<std::string> then_statement = leaf_statement(then);
            if (!then_statement) return std::nullopt;
            std::vector<std::string> lines{std::format("if ({} {} {})", parameter(body[2]), *test, value), "    " + *then_statement};
            if (!rest.empty()) {
                const auto more = leaf_branches(rest);
                if (!more) return std::nullopt;
                lines.insert(lines.end(), more->begin(), more->end());
            }
            return lines;
        }
    }
    if (const auto only = leaf_statement(body)) return std::vector<std::string>{*only};
    return std::nullopt;
}

std::string leaf_parameters(std::uint32_t n) {
    std::vector<std::string> parts;
    for (std::uint32_t i = 0; i < n; ++i) parts.push_back("int a" + std::to_string(i));
    return parts.empty() ? "void" : join_with(parts, ", ");
}

// ---- the register variables -----------------------------------------------------------------------------------------------

std::string callee_name(const Callees& callees, std::size_t k) {
    const auto it = callees.find(static_cast<std::uint32_t>(k));
    return it != callees.end() ? it->second : "G" + std::to_string(k);
}

// The parameter of `word ptr [bp + n]`.
std::optional<std::string> frame_parameter(std::string_view operand) {
    static const Regex slot(R"(^word ptr \[bp \+ (\w+)\])");
    const auto m = slot.search(operand);
    if (!m) return std::nullopt;
    const auto n = cb::text::parse_int<std::uint32_t>(m->group(1), 0);
    if (!n) return std::nullopt;
    return lettered_parameter(*n);
}

bool starts_with_number(std::string_view operand) { return !operand.empty() && (std::isdigit(static_cast<unsigned char>(operand[0])) != 0); }

std::optional<std::string> number_text(std::string_view operand) {
    const auto n = cb::text::parse_int<std::int64_t>(operand, 0);
    if (!n) return std::nullopt;
    return std::to_string(*n);
}

// The 16-bit decoder of this thread: one engine is dear to make, and an engine is not shared between threads.
const small_cs::Decoder& decoder16() {
    thread_local small_cs::DecoderSet decoders;
    return decoders.get(small_cs::Arch::X86, 16);
}

std::size_t saved_registers(const Code& code, std::size_t p, std::vector<std::string>* saved) {
    while (p < code.size() && (code.at(p) == 0x57 || code.at(p) == 0x56)) {
        if (saved != nullptr) saved->push_back(code.at(p) == 0x57 ? "di" : "si");
        ++p;
    }
    return p;
}

}  // namespace

// ---- leaf -------------------------------------------------------------------------------------------------------------------

Result<Leaf> leaf(ByteView bytes) {
    try {
        const Code code(bytes);
        if (!code.starts_with_prolog()) return Declined("prologue not the /Gsw loadds template (T0 miss)");
        std::size_t p = kProlog.size();
        std::uint32_t frame = 0;
        if (code.is(p, {0x83, 0xec})) {
            frame = code.at(p + 2);
            p += 3;
        }
        const auto epilog = code.find_epilog(p);
        if (!epilog) return Declined("epilogue (T0 tail) not found");
        const ByteView body = code.slice(p, *epilog);
        std::size_t q = *epilog + kEpilog.size();
        std::uint32_t pop_bytes = 0;
        if (code.is(q, {kFarReturnPop})) {
            pop_bytes = code.u16(q + 1);
            q += 3;
        } else if (code.is(q, {kFarReturn})) {
            q += 1;
        } else {
            return Declined("no far return (ca/cb) after epilogue");
        }
        Leaf out;
        out.frame = frame;
        out.trailing_nop = q < code.size() && code.at(q) == 0x90;
        out.arguments = pop_bytes / 2;
        std::optional<std::string> statement;
        bool unused_local = false;
        const std::string hexed = hex_bytes(body);

        if (body.empty()) {
            unused_local = frame != 0 && out.trailing_nop;
        } else if (body.size() == 3 && body[0] == 0xb8) {
            statement = "return " + signed16(body[1] | (body[2] << 8)) + ";";
        } else if (body.size() == 3 && body[0] == 0x8b && body[1] == 0x46) {
            statement = "return " + parameter(body[2]) + ";";
        } else if (body.size() == 3 && body[0] == 0xa1) {
            statement = "return gG;";
            out.globals.push_back("gG");
        } else if (body.size() == 6 && body[0] == 0xc7 && body[1] == 0x06) {
            statement = "gG = " + std::to_string(body[4] | (body[5] << 8)) + ";";
            out.globals.push_back("gG");
            out.return_type = "void";
        } else if (const auto call = std::ranges::find(body, kFarCall); call != body.end()) {
            const std::size_t at = static_cast<std::size_t>(call - body.begin());
            if (body.size() - at != 5) return Declined("T5: code after the CALLF not modeled (" + hexed + ")");
            std::vector<std::string> pushes;
            for (std::size_t i = 0; i < at;) {
                if (body[i] == 0x6a && i + 1 < body.size()) {
                    pushes.push_back(signed8(body[i + 1]));
                    i += 2;
                } else if (body[i] == 0x68 && i + 2 < body.size()) {
                    pushes.push_back(std::to_string(body[i + 1] | (body[i + 2] << 8)));
                    i += 3;
                } else if (body[i] == 0xff && i + 2 < body.size() && body[i + 1] == 0x76) {
                    pushes.push_back(parameter(body[i + 2]));
                    i += 3;
                } else {
                    return Declined("T5: arg-push form not modeled (" + hex_bytes(body.first(at)) + ")");
                }
            }
            const std::string types = pushes.empty() ? "void" : join_with(std::vector<std::string>(pushes.size(), "int"), ", ");
            out.text = std::format("extern void far pascal G({});\nvoid far pascal F({})\n{{\n    G({});\n}}", types, leaf_parameters(out.arguments), join_with(pushes, ", "));
            out.shape = "T5 far-call";
            out.call_arguments = std::move(pushes);
            return out;
        } else if (body.size() >= 5 && body[0] == 0x83 && body[1] == 0x7e && opposite_test(body[4])) {
            const auto lines = leaf_branches(body);
            if (!lines) return Declined("T6: branch structure not fully modeled (" + hexed + ")");
            std::string inner;
            for (const std::string& line : *lines) inner += "    " + line + "\n";
            out.text = "int far pascal F(" + leaf_parameters(out.arguments) + ")\n{\n" + inner + "}";
            out.shape = "T6 if-branch";
            out.lines = *lines;
            return out;
        } else {
            return Declined(std::format("body not a modeled leaf template ({} bytes: {})", body.size(), hexed));
        }

        std::string declarations;
        for (const std::string& g : out.globals) declarations += "int " + g + ";\n";
        const std::string inner = unused_local ? "    int i;\n" : (statement ? "    " + *statement + "\n" : std::string());
        out.text = declarations + out.return_type + " far pascal F(" + leaf_parameters(out.arguments) + ")\n{\n" + inner + "}";
        out.shape = statement ? *statement : (unused_local ? "int i" : "empty");
        return out;
    } catch (const Declined& d) {
        return d;
    }
}

// ---- walk -------------------------------------------------------------------------------------------------------------------

Result<Walked> walk(ByteView bytes, const Globals& globals, std::string_view callee) {
    try {
        const Code code(bytes);
        if (!code.starts_with_prolog()) return Declined("prologue miss");
        const auto epilog = code.rfind_epilog();
        if (!epilog) return Declined("epilogue miss");
        WalkState state{globals, {}, false};
        Result<std::vector<std::string>> lines = walk_block(code, body_start(code), *epilog, state);
        if (!lines.ok()) return Failure(lines.shared_error());
        Walked out;
        std::string declarations = "typedef unsigned int WORD;\n";
        if (state.calls) declarations += "extern WORD far pascal " + std::string(callee) + "(WORD);\n";
        for (const auto& [name, width] : state.named.items()) declarations += "extern " + c_type(width) + " " + name + ";\n";
        std::string body;
        for (const std::string& line : lines.value()) body += "    " + cb::text::replace_all(line, "CALLEE", callee) + "\n";
        out.text = declarations + "\nvoid far F(void)\n{\n" + body + "}";
        out.statements = lines.value().size();
        out.globals = state.named.items();
        return out;
    } catch (const Declined& d) {
        return d;
    }
}

// ---- cleanup ----------------------------------------------------------------------------------------------------------------

Result<std::string> cleanup(ByteView bytes, const std::map<std::uint32_t, std::string>& names) {
    try {
        const Code code(bytes);
        if (!code.starts_with_prolog()) return Declined("not the loadds far frame");
        std::size_t p = body_start(code);
        std::vector<std::string> lines, used;
        // each block: mov ax,[hi]; or ax,[lo]; je skip; push [hi]; callf Free; sub ax,ax; mov [hi],ax; mov [lo],ax
        while (code.is(p, {0xa1}) && code.is(p + 3, {0x0b, 0x06}) && code.at(p + 7) == 0x74) {
            const std::uint32_t low = code.u16(p + 5);
            const auto named = names.find(low);
            const std::string name = named != names.end() ? named->second : std::format("g_{:x}", low);
            used.push_back(name);
            std::size_t q = p + 9;
            const auto expect = [&](std::initializer_list<std::uint8_t> bytes, std::size_t length) {
                if (!code.is(q, bytes)) throw Declined(std::format("the free idiom stops at +{}", q));
                q += length;
            };
            expect({0xff, 0x36}, 4);
            expect({kFarCall}, 5);
            expect({0x2b, 0xc0}, 2);
            expect({0xa3}, 3);
            expect({0xa3}, 3);
            lines.push_back("if (" + name + " != 0L) {");
            lines.push_back("    GlobalFree((WORD)(" + name + " >> 16));");
            lines.push_back("    " + name + " = 0L;");
            lines.emplace_back("}");
            p = q;
        }
        std::string declarations = "typedef unsigned int WORD;\nextern WORD far pascal GlobalFree(WORD);\n";
        for (const std::string& n : used) declarations += "extern long " + n + ";\n";
        std::string body;
        for (const std::string& line : lines) body += "    " + line + "\n";
        return declarations + "\nvoid far Cleanup(void)\n{\n" + body + "}";
    } catch (const Declined& d) {
        return d;
    }
}

// ---- registers --------------------------------------------------------------------------------------------------------------

Result<Registers> registers(ByteView bytes, const Callees& callees) {
    try {
        const Code code(bytes);
        if (!code.starts_with_prolog()) return Declined("prologue miss");
        Registers out;
        std::size_t p = saved_registers(code, body_start(code), &out.saved);
        const auto epilog = code.rfind_epilog();
        if (!epilog) return Declined("epilogue miss");
        std::size_t body_end = *epilog;
        while (body_end > p && (code.at(body_end - 1) == 0x5e || code.at(body_end - 1) == 0x5f)) --body_end;   // pop si; pop di

        std::map<std::string, std::string> value;   // register -> the C expression it holds ("_lastcall": the last call's)
        std::vector<std::string> pending;           // arguments pushed for the next call
        std::vector<std::pair<std::string, std::string>> locals;   // register -> the local that keeps its value
        std::size_t calls = 0;
        const auto is_local = [&](const std::string& r) { return std::ranges::any_of(locals, [&](const auto& l) { return l.first == r; }); };
        for (const small_cs::Insn& ins : decoder16().disasm(code.slice(p, body_end), 0)) {
            const std::string& op = ins.op_str;
            if (ins.bytes[0] == kFarCall) {
                const std::string call = callee_name(callees, calls++) + "(" + join_with(pending, ", ") + ")";
                pending.clear();
                value["ax"] = call;
                value["_lastcall"] = call;
            } else if (ins.mnemonic == "push") {
                if (const auto held = value.find(op); held != value.end()) {
                    pending.push_back(held->second);
                } else if (const auto slot = frame_parameter(op)) {
                    pending.push_back(*slot);
                } else if (starts_with_number(op) && number_text(op)) {
                    pending.push_back(*number_text(op));
                } else {
                    return Declined("push form unmodeled: " + op);
                }
            } else if (ins.mnemonic == "mov") {
                const std::size_t comma = op.find(", ");
                const std::string dst = op.substr(0, comma), src = comma == std::string::npos ? std::string() : op.substr(comma + 2);
                if (dst != "si" && dst != "di" && dst != "ax" && dst != "cx" && dst != "dx" && dst != "bx") return Declined("mov dst unmodeled: " + dst);
                if (const auto held = value.find(src); held != value.end()) {
                    const std::string held_value = held->second;
                    const auto last = value.find("_lastcall");
                    if ((dst == "si" || dst == "di") && !is_local(dst) && last != value.end() && held_value == last->second) {
                        // the call's result kept in a register variable as well as passed on: a local of its own
                        const std::string local(1, static_cast<char>('b' + locals.size()));
                        locals.emplace_back(dst, local);
                        out.statements.push_back(local + " = " + held_value + ";");
                        for (std::string& a : pending)
                            if (a == held_value) a = local;
                        value[dst] = local;
                    } else {
                        value[dst] = held_value;
                    }
                } else if (const auto slot = frame_parameter(src)) {
                    value[dst] = *slot;
                } else if (starts_with_number(src) && number_text(src)) {
                    value[dst] = *number_text(src);
                } else {
                    return Declined("mov src unmodeled: " + src);
                }
            } else if (ins.mnemonic == "sub" && op == "ax, ax") {
                value["ax"] = "0";
            } else {
                return Declined("insn unmodeled: " + ins.mnemonic + " " + op);
            }
        }

        // the function's end: the parameters its far return pops, the routines it names, the statements and the value it returns
        std::size_t q = *epilog + kEpilog.size();
        const std::uint32_t parameters = code.is(q, {kFarReturnPop}) ? code.u16(q + 1) / 2 : 0;
        std::string declarations = "typedef unsigned int WORD;\n";
        std::set<std::string> declared;
        for (std::size_t k = 0; k < calls; ++k)
            if (declared.insert(callee_name(callees, k)).second) declarations += "extern int far pascal " + callee_name(callees, k) + "();\n";
        const auto last = value.find("_lastcall");
        if (last != value.end() && !last->second.empty() && join_with(out.statements, "").find(last->second) == std::string::npos) out.statements.push_back(last->second + ";");
        if (const auto ax = value.find("ax"); ax != value.end() && !ax->second.empty()) out.statements.push_back("return " + ax->second + ";");
        std::string inner;
        for (const auto& [reg, local] : locals) {
            inner += "    register int " + local + ";\n";
            out.locals.push_back(local);
        }
        for (const std::string& line : out.statements) inner += "    " + line + "\n";
        std::vector<std::string> params;
        for (std::uint32_t i = 0; i < parameters; ++i) params.push_back(std::string("int ") + static_cast<char>('a' + i));
        out.text = declarations + "\nint far pascal F(" + (params.empty() ? std::string("void") : join_with(params, ", ")) + ")\n{\n" + inner + "}";
        return out;
    } catch (const Declined& d) {
        return d;
    }
}

Result<CountedLoop> counted_loop(ByteView bytes, const Callees& callees) {
    try {
        const Code code(bytes);
        if (!code.starts_with_prolog()) return Declined("prologue miss");
        std::size_t p = saved_registers(code, body_start(code), nullptr);
        if (!code.is(p, {0x8b, 0x7e})) return Declined("no di=param");                 // mov di,[bp+n]: the bound
        CountedLoop out;
        out.limit = lettered_parameter(code.at(p + 2));
        p += 3;
        if (!code.is(p, {0x2b, 0xf6})) return Declined("no si=0 (loop counter init)");   // sub si,si
        p += 2;
        if (!code.is(p, {0x0b, 0xff}) || code.at(p + 2) != 0x7e) return Declined("no loop guard (or di,di; jle)");
        p += 4;
        const std::size_t start = p;
        std::optional<std::size_t> back;                                                  // cmp si,di; jl
        for (std::size_t j = p; j + 1 < code.size(); ++j)
            if (code.is(j, {0x3b, 0xf7})) {
                back = j;
                break;
            }
        if (!back || code.at(*back + 2) != 0x7c) return Declined("no back-edge (cmp si,di; jl)");
        const std::size_t increment = *back - 1;
        if (code.at(increment) != 0x46) return Declined("no counter increment (inc si)");
        std::vector<std::string> pending;
        for (const small_cs::Insn& ins : decoder16().disasm(code.slice(start, increment), 0)) {
            const std::string& op = ins.op_str;
            if (ins.mnemonic == "push" && op == "si") {
                pending.emplace_back("i");
            } else if (ins.mnemonic == "push" && starts_with_number(op) && number_text(op)) {
                pending.push_back(*number_text(op));
            } else if (ins.bytes[0] == kFarCall) {
                out.body_calls.push_back(callee_name(callees, out.body_calls.size()) + "(" + join_with(pending, ", ") + ")");
                pending.clear();
            } else {
                return Declined("loop body insn unmodeled: " + ins.mnemonic + " " + op);
            }
        }
        std::string declarations = "typedef unsigned int WORD;\n";
        for (std::size_t k = 0; k < out.body_calls.size(); ++k) declarations += "extern int far pascal " + callee_name(callees, k) + "();\n";
        std::string body;
        if (out.body_calls.size() != 1) {
            for (const std::string& call : out.body_calls) body += "    " + call + ";\n";
        } else {
            body = "        " + out.body_calls.front() + ";";
        }
        out.text = declarations + "\nint far pascal F(int " + out.limit + ")\n{\n    register int i;\n    for (i = 0; i < " + out.limit + "; i++)\n" + body + "\n    return i;\n}";
        return out;
    } catch (const Declined& d) {
        return d;
    }
}

// ---- mask -------------------------------------------------------------------------------------------------------------------

Bytes mask(ByteView code) {
    Bytes out(code.begin(), code.end());
    const auto zero = [&](std::size_t at, std::size_t count) {
        for (std::size_t k = 0; k < count && at + k < out.size(); ++k) out[at + k] = 0;
    };
    for (const small_cs::Insn& ins : decoder16().disasm(code, 0)) {
        const std::uint8_t op = ins.bytes[0], modrm = ins.size > 1 ? ins.bytes[1] : 0;
        const std::size_t at = static_cast<std::size_t>(ins.address);
        if (op == kFarCall) {
            zero(at + 1, 4);
        } else if (op == 0xe8 || op == 0xa1 || op == 0xa3 || op == 0x68) {
            zero(at + 1, 2);
        } else if (ins.size >= 4 && (op == 0xff || op == 0x0b || op == 0x83 || op == 0xc6 || op == 0xc7 || op == 0x89 || op == 0x8b) &&
                   (modrm == 0x36 || modrm == 0x06 || modrm == 0x3e || modrm == 0x26 || modrm == 0x0e || modrm == 0x16 || modrm == 0x1e)) {
            zero(at + 2, 2);   // a direct [imm16] operand
        }
    }
    return out;
}

}  // namespace cb::invert::msc6_small
