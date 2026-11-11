#include "cb/decompile/shape16.hpp"

#include <algorithm>
#include <optional>

#include <nlohmann/json.hpp>

#include "cb/decompile/inline_asm.hpp"
#include "cb/decompile/l2tree.hpp"

namespace cb::decompile::shape16 {

namespace {

using l2tree::is_kind;
using l2tree::member;
using l2tree::text;

constexpr std::size_t kLongestInstruction = 8;   // the bytes read at a statement's instruction offset

// The mnemonic of each instruction at the offsets -- an inc or dec of anything but a place in the frame ([bp-k]: a
// register, a global) as "inc reg".
std::vector<std::string> mnemonics(const inline_asm::Decoder16& decoder, ByteView code, const std::vector<std::uint32_t>& at) {
    std::vector<std::string> out;
    for (std::uint32_t a : at) {
        if (a >= code.size()) continue;
        const auto window = code.subspan(a, std::min(kLongestInstruction, code.size() - a));
        const std::vector<inline_asm::Instruction> decoded = decoder.decode(window, a);
        if (decoded.empty()) continue;
        const inline_asm::Instruction& i = decoded.front();
        const bool step = i.mnemonic == "inc" || i.mnemonic == "dec";
        out.push_back(step && i.op_str.find("[bp") == std::string::npos ? i.mnemonic + " reg" : i.mnemonic);
    }
    return out;
}

bool is_one(const Json* v, int sign) {
    if (!v) return false;
    if (v->is_boolean()) return sign == 1 && v->get<bool>();
    if (v->is_number()) return v->get<double>() == sign;
    return false;
}

// "inc" / "dec" where the assignment is x = x + 1 / x - 1 (x + -1) of its own target, else empty.
std::string_view step_of(const Json& assign) {
    const Json* value = member(&assign, "value");
    const Json* target = member(&assign, "target");
    if (!is_kind(value, "Binary")) return {};
    const std::string_view op = text(value, "op");
    if (op != "+" && op != "-") return {};
    const Json* left = member(value, "left");
    const Json* right = member(value, "right");
    if (!is_kind(left, "Name") || text(member(left, "sym"), "name") != text(member(target, "sym"), "name") || !is_kind(right, "Lit")) return {};
    const Json* literal = member(right, "value");
    const bool plus_one = is_one(literal, 1);
    if (!plus_one && !is_one(literal, -1)) return {};
    return (op == "+") == plus_one ? "inc" : "dec";
}

// x++ / x-- where a statement assigned x from an inc / dec of its place
void shape_steps(Json& body, ByteView code, std::vector<std::string>& said) {
    std::optional<inline_asm::Decoder16> decoder;   // one Capstone handle for the pass (opened at the first statement that needs it)
    l2tree::walk(body, [&](Json& st) {
        if (!is_kind(st, "ExprStmt")) return;
        const Json* expr = member(&st, "expr");
        if (!is_kind(expr, "Assign") || l2tree::present(member(expr, "op")) || !is_kind(member(expr, "target"), "Name")) return;
        if (!decoder) decoder.emplace();
        std::vector<std::string> ms = mnemonics(*decoder, code, l2tree::origin_offsets(st, /*keep_repeats=*/true));
        const std::string_view step = step_of(*expr);
        if (ms.size() == 1 && (ms[0] == "inc reg" || ms[0] == "dec reg") && step == std::string_view(ms[0]).substr(0, 3)) ms[0].resize(3);
        if (ms.size() != 1 || (ms[0] != "inc" && ms[0] != "dec")) return;
        if (step != ms[0] && !is_kind(member(expr, "value"), "Lit")) return;
        const Json* target = member(expr, "target");
        const bool up = ms[0] == "inc";
        said.push_back(std::string(text(member(target, "sym"), "name")) + (up ? "++" : "--"));
        Json unary = Json::object();
        unary["node"] = "Unary";
        unary["op"] = up ? "++" : "--";
        unary["postfix"] = true;
        unary["x"] = *target;
        const Json* type = member(target, "type");
        unary["type"] = type ? *type : Json(nullptr);
        st["expr"] = std::move(unary);
    });
}

// An if whose then-part is one return, followed by a return: when the original had the other return first (it sits at the
// lower address) the condition is turned round and the two returns traded.
void turn_round_if_returns(Json& list, std::vector<std::string>& said) {
    auto& body = list.get_ref<Json::array_t&>();
    for (std::size_t k = 0; k + 1 < body.size(); ++k) {
        const Json& a = body[k];
        const Json& b = body[k + 1];
        if (!is_kind(a, "If") || l2tree::present(member(a, "orelse")) || !is_kind(b, "Return")) continue;
        const Json* then = member(a, "then");
        const bool then_is_list = then && then->is_array();
        const Json* then_return = then_is_list ? (then->size() == 1 ? &(*then)[0] : nullptr) : (then && then->is_object() ? then : nullptr);
        if (!is_kind(then_return, "Return")) continue;
        const auto ta = l2tree::origin_offsets(*then_return);
        const auto tb = l2tree::origin_offsets(b);
        const Json* cond = member(a, "cond");
        if (ta.empty() || tb.empty() || ta.front() <= tb.front() || !cond) continue;   // (the original has the other return first)
        Json turned;
        if (is_kind(cond, "Unary") && text(cond, "op") == "!" && member(cond, "x")) {
            turned = *member(cond, "x");
        } else {
            turned = Json::object();
            turned["node"] = "Unary";
            turned["op"] = "!";
            turned["x"] = *cond;
            turned["type"] = Json::object();
            turned["type"]["kind"] = "bool";
        }
        Json first_return = *then_return;
        Json second_return = b;
        Json replaced = a;
        replaced["cond"] = std::move(turned);
        if (then_is_list) {
            Json one = Json::array();
            one.push_back(std::move(second_return));
            replaced["then"] = std::move(one);
        } else {
            replaced["then"] = std::move(second_return);
        }
        body[k] = std::move(replaced);
        body[k + 1] = std::move(first_return);
        said.push_back("an if-return turned round");
    }
}

}  // namespace

std::vector<std::string> reshape(Json& doc, ByteView code) {
    std::vector<std::string> said;
    Json* body = member(&doc, "body");
    if (!body) return said;
    shape_steps(*body, code, said);
    if (l2tree::present(body) && body->is_array()) turn_round_if_returns(*body, said);
    l2tree::for_each_statement_list(*body, [&](Json& list) { turn_round_if_returns(list, said); });
    return said;
}

}  // namespace cb::decompile::shape16
