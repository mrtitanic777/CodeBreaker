#include "cb/decompile/shape16.hpp"

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"

namespace s16 = cb::decompile::shape16;
using cb::Bytes;
using cb::Json;
using std::string;

// DIVERGES from the Python module: none in behaviour (frame16_test.cpp runs the original's 450 documents through frame16
// and shape16, with the original's answers). The tree is edited in place; the decoder is Capstone 5.0.7 through
// capstone.dll.

namespace {

Bytes bytes_of(const string& hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    return out;
}

Json function(const string& body) { return cb::json::parse(R"({"node":"MethodDecl","name":"f","body":)" + body + "}"); }

string name(const string& n) { return R"({"node":"Name","type":{"kind":"i16"},"sym":{"kind":"local","name":")" + n + R"("}})"; }
string lit(int v) { return R"({"node":"Lit","type":{"kind":"i16"},"value":)" + std::to_string(v) + "}"; }
string binary(const string& op, const string& left, const string& right) {
    return R"({"node":"Binary","op":")" + op + R"(","left":)" + left + R"(,"right":)" + right + R"(,"type":{"kind":"i16"}})";
}
string assign(const string& target, const string& value, const string& at) {
    return R"({"node":"ExprStmt","origin":{"at":[)" + at + R"(]},"expr":{"node":"Assign","target":)" + target + R"(,"value":)" + value + "}}";
}
string ret(int value, int at) { return R"({"node":"Return","expr":)" + lit(value) + R"(,"origin":{"at":[)" + std::to_string(at) + "]}}"; }

// push bp / mov bp,sp / sub sp,4 / mov word ptr [bp-2],0 / inc word ptr [bp-2] / mov ax,[bp-2] / mov sp,bp / pop bp / ret
const Bytes kFlag = bytes_of("558bec83ec04c746fe0000ff46fe8b46fe8be55dc3");

std::vector<string> said_of(Json& doc, const Bytes& code) { return s16::reshape(doc, code); }

}  // namespace

CB_TEST("decompile.shape16.inc_is_written_as_plus_plus", [] {
    Json doc = function("[" + assign(name("bVar1"), lit(0), "6") + "," + assign(name("bVar1"), lit(1), "11") + "]");
    const auto said = said_of(doc, kFlag);
    CB_CHECK_EQ(nlohmann::json(said).dump(), R"(["bVar1++"])");
    const auto expr = nlohmann::json::parse(doc["body"][1]["expr"].dump());
    CB_CHECK_EQ(expr["node"].get<string>(), "Unary");
    CB_CHECK_EQ(expr["op"].get<string>(), "++");
    CB_CHECK(expr["postfix"].get<bool>());
    CB_CHECK_EQ(expr["x"]["sym"]["name"].get<string>(), "bVar1");
    CB_CHECK_EQ(expr["type"].dump(), R"({"kind":"i16"})");
    CB_CHECK_EQ(doc["body"][0]["expr"]["node"].get<string>(), "Assign");   // (the mov stays an assignment)
});

CB_TEST("decompile.shape16.an_inc_of_a_register_stays_an_assignment", [] {
    // inc ax / ret: the decompiler's `x = in_AX + 1` -- x++ would read x, not AX
    Json a = function("[" + assign(name("x"), binary("+", name("in_AX"), lit(1)), "0") + "]");
    const Json before = a;
    CB_CHECK(said_of(a, bytes_of("40c3")).empty());
    CB_CHECK_EQ(a.dump(), before.dump());
    // dec cx / ret: `b = p == 0` (the flag the dec set) is no b--; `x = x - 1` is x--
    Json b = function("[" + assign(name("b"), binary("==", name("p"), lit(0)), "0") + "," + assign(name("x"), binary("+", name("x"), lit(-1)), "0") + "]");
    CB_CHECK_EQ(nlohmann::json(said_of(b, bytes_of("49c3"))).dump(), R"(["x--"])");
    // a step of 1 written x - 1, x + -1 or x + 1 against inc / dec
    Json c = function("[" + assign(name("x"), binary("-", name("x"), lit(1)), "0") + "]");
    CB_CHECK_EQ(nlohmann::json(said_of(c, bytes_of("49c3"))).dump(), R"(["x--"])");
    Json d = function("[" + assign(name("x"), binary("-", name("x"), lit(-1)), "0") + "]");
    CB_CHECK_EQ(nlohmann::json(said_of(d, bytes_of("40c3"))).dump(), R"(["x++"])");
    Json e = function("[" + assign(name("x"), binary("+", name("x"), lit(1)), "0") + "]");
    CB_CHECK(said_of(e, bytes_of("49c3")).empty());   // an inc written, a dec in the code: left
});

CB_TEST("decompile.shape16.an_inc_of_a_place_is_not_a_larger_step", [] {
    // inc word ptr [bp-2] said to be `x = x + 3` (one of the statement's instructions): left
    Json doc = function("[" + assign(name("x"), binary("+", name("x"), lit(3)), "11") + "]");
    CB_CHECK(said_of(doc, kFlag).empty());
    // a compound assignment (x += 1) is not an assignment of x = ...
    Json compound = function("[" + assign(name("x"), lit(1), "11") + "]");
    compound["body"][0]["expr"]["op"] = "+=";
    CB_CHECK(said_of(compound, kFlag).empty());
    // a statement of several instructions, or one past the code, is left
    Json many = function("[" + assign(name("x"), lit(1), "6,11") + "," + assign(name("y"), lit(1), "900") + "]");
    CB_CHECK(said_of(many, kFlag).empty());
});

CB_TEST("decompile.shape16.if_return_turned_round", [] {
    const Bytes code(16, 0);
    const string cond = binary("==", name("x"), lit(0));
    const auto if_return = [&](const string& c, const string& then, int tail_at) {
        return function(string("[") + R"({"node":"If","cond":)" + c + R"(,"then":)" + then + "}," + ret(2, tail_at) + "]");
    };
    // the then-part returns at a higher address than the return that follows: the original had the other first, so the
    // condition is turned round and the two returns traded
    Json doc = if_return(cond, "[" + ret(1, 9) + "]", 5);
    CB_CHECK_EQ(nlohmann::json(said_of(doc, code)).dump(), R"(["an if-return turned round"])");
    const auto body = nlohmann::json::parse(doc["body"].dump());
    CB_CHECK_EQ(body[0]["cond"]["node"].get<string>(), "Unary");
    CB_CHECK_EQ(body[0]["cond"]["op"].get<string>(), "!");
    CB_CHECK_EQ(body[0]["cond"]["x"]["op"].get<string>(), "==");
    CB_CHECK_EQ(body[0]["cond"]["type"].dump(), R"({"kind":"bool"})");
    CB_CHECK_EQ(body[0]["then"].dump(), nlohmann::json::array({nlohmann::json::parse(ret(2, 5))}).dump());
    CB_CHECK_EQ(body[1].dump(), nlohmann::json::parse(ret(1, 9)).dump());
    // already `!c`: the negation is taken off; a bare statement as the then-part stays a bare statement
    Json negated = if_return(R"({"node":"Unary","op":"!","x":)" + cond + "}", ret(1, 9), 5);
    CB_CHECK_EQ(nlohmann::json(said_of(negated, code)).dump(), R"(["an if-return turned round"])");
    CB_CHECK_EQ(negated["body"][0]["cond"]["op"].get<string>(), "==");
    CB_CHECK_EQ(negated["body"][0]["then"]["node"].get<string>(), "Return");
    // the then-part is already first (lower address): left, as with an else-part, two statements, or no origin
    Json first = if_return(cond, "[" + ret(1, 3) + "]", 5);
    CB_CHECK(said_of(first, code).empty());
    Json with_else = if_return(cond, "[" + ret(1, 9) + "]", 5);
    with_else["body"][0]["orelse"] = cb::json::parse("[" + lit(1) + "]");
    CB_CHECK(said_of(with_else, code).empty());
    Json two = if_return(cond, "[" + ret(1, 9) + "," + ret(3, 10) + "]", 5);
    CB_CHECK(said_of(two, code).empty());
    Json unplaced = if_return(cond, R"([{"node":"Return","expr":)" + lit(1) + "}]", 5);
    CB_CHECK(said_of(unplaced, code).empty());
    // inside a nested list (a loop's body, an if's then-part)
    Json nested = function(R"([{"node":"While","cond":)" + lit(1) + R"(,"body":[{"node":"If","cond":)" + cond + R"(,"then":[)" + ret(1, 9) +
                           "]}," + ret(2, 5) + "]}]");
    CB_CHECK_EQ(nlohmann::json(said_of(nested, code)).dump(), R"(["an if-return turned round"])");
    CB_CHECK_EQ(nested["body"][0]["body"][1]["expr"]["value"].dump(), "1");
});

CB_TEST("decompile.shape16.documents_without_a_body", [] {
    Json none = cb::json::parse(R"({"node":"MethodDecl"})");
    CB_CHECK(said_of(none, Bytes(4, 0)).empty());
    Json odd = cb::json::parse(R"({"body":"not a list"})");
    CB_CHECK(said_of(odd, Bytes(4, 0)).empty());
});
