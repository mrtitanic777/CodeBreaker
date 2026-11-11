#include "cb/decompile/frame16.hpp"

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/decompile/shape16.hpp"

namespace f16 = cb::decompile::frame16;
namespace s16 = cb::decompile::shape16;
using cb::Bytes;
using cb::Json;
using std::string;

// DIVERGES from the Python module (the old harness, 57 documents, and 400 random ones made for this port -- locals of
// every type, buffers, far pointers, pieces `_2_2_`, asm fragments, if-returns, bad ranges -- agreed on every tree and every
// report; testdata/frame16_cases.jsonl keeps 55 of them, with the original's answers, read through by hand):
//  - the tree is edited in place (the original copied it first);
//  - a Local with no symbol name is kept where it was and never given a place (the original keyed its places by None);
//  - the decoder is Capstone 5.0.7 through capstone.dll (the original used the Python binding of the same library).

namespace {

Bytes bytes_of(const string& hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    return out;
}

string canonical(const Json& j) { return nlohmann::json::parse(j.dump()).dump(); }
string canonical(const nlohmann::json& j) { return nlohmann::json::parse(j.dump()).dump(); }

// push bp / mov bp, sp / sub sp, 4 / mov word ptr [bp-2], 0 / inc word ptr [bp-2] / mov ax, [bp-2] / mov sp, bp / pop bp / ret
const char* const kFlagCode = "558bec83ec04c746fe0000ff46fe8b46fe8be55dc3";

Json function(const string& body) {
    return cb::json::parse(R"({"node":"MethodDecl","name":"f","sym":{"name":"f","type":{"kind":"fn","distance":"far","params":[]}},"body":)" + body + "}");
}

// a local the decompiler kept `at`; a statement assigning it
string local(const string& name, const string& kind, const string& at = "") {
    return R"({"node":"Local","sym":{"kind":"local","name":")" + name + R"("},"type":{"kind":")" + kind + R"("})" + (at.empty() ? "" : R"(,"at":)" + at) + "}";
}
string name(const string& n, const string& kind = "i16") {
    return R"({"node":"Name","type":{"kind":")" + kind + R"("},"sym":{"kind":"local","name":")" + n + R"("}})";
}
string lit(int v) { return R"({"node":"Lit","type":{"kind":"i16"},"value":)" + std::to_string(v) + "}"; }
string assign(const string& target, const string& value, int at) {
    return R"({"node":"ExprStmt","origin":{"at":[)" + std::to_string(at) + R"(]},"expr":{"node":"Assign","target":)" + target + R"(,"value":)" + value + "}}";
}

}  // namespace

CB_TEST("decompile.frame16.cases", [] {
    const string data = cb::fs::read_text(cb::testing::data_path("decompile/testdata/frame16_cases.jsonl"));
    int cases = 0;
    for (std::string_view line : cb::text::lines(data)) {
        if (line.empty()) continue;
        const nlohmann::json c = nlohmann::json::parse(line);
        const string name = c["name"].get<string>();
        const Bytes code = bytes_of(c["code"].get<string>());
        const Json doc = cb::json::parse(c["doc"].dump());
        const auto start = c["start"].get<std::uint32_t>();
        const auto end = c["end"].get<std::uint32_t>();

        Json laid = doc;
        const auto said = f16::lay_out(laid, code, start, end);
        CB_CHECK_EQ(name + " frame doc: " + canonical(laid), name + " frame doc: " + canonical(c["frame"][0]));
        CB_CHECK_EQ(name + " frame said: " + nlohmann::json(said).dump(), name + " frame said: " + c["frame"][1].dump());

        Json shaped = laid;
        const auto said2 = s16::reshape(shaped, code);
        CB_CHECK_EQ(name + " frame+shape doc: " + canonical(shaped), name + " frame+shape doc: " + canonical(c["frame_shape"][0]));
        CB_CHECK_EQ(name + " frame+shape said: " + nlohmann::json(said2).dump(), name + " frame+shape said: " + c["frame_shape"][1].dump());

        Json alone = doc;
        const auto said3 = s16::reshape(alone, code);
        CB_CHECK_EQ(name + " shape doc: " + canonical(alone), name + " shape doc: " + canonical(c["shape"][0]));
        CB_CHECK_EQ(name + " shape said: " + nlohmann::json(said3).dump(), name + " shape said: " + c["shape"][1].dump());
        ++cases;
    }
    CB_CHECK(cases >= 40);
});

CB_TEST("decompile.frame16.a_flag_kept_in_a_word_is_a_word", [] {
    Json doc = function("[" + local("bVar1", "bool", "\"none\"") + "," + assign(name("bVar1", "bool"), lit(0), 6) + "," +
                        assign(name("bVar1", "bool"), lit(1), 11) + R"(,{"node":"Return","expr":)" + name("bVar1", "bool") + R"(,"origin":{"at":[14]}}])");
    const Bytes code = bytes_of(kFlagCode);
    const auto said = f16::lay_out(doc, code, 0, static_cast<std::uint32_t>(code.size()));
    CB_CHECK_EQ(nlohmann::json(said).dump(), R"(["bVar1: 2 bytes at [bp-2]"])");
    CB_CHECK_EQ(doc["body"][0]["type"].dump(), R"({"kind":"i16"})");   // (the decompiler's bool: 2 bytes at [bp-2])
});

CB_TEST("decompile.frame16.the_pieces_of_a_buffer_are_one_array", [] {
    // push bp / mov bp,sp / sub sp,12h / lea ax,[bp-10h] / push ax / call $+3 / add sp,2 / mov ax,[bp-10h] /
    // add ax,[bp-0Ch] / mov word ptr [bp-12h],5 / mov sp,bp / pop bp / ret
    const Bytes code = bytes_of("558bec83ec128d46f050e8000083c4028b46f00346f4c746ee05008be55dc3");
    Json doc = function("[" + local("iVar1", "i16", "\"none\"") + "," + local("xStack_12", "i16", R"({"stack":-18})") + "," +
                        local("xStack_e", "i16", R"({"stack":-14})") + "," + assign(name("iVar1"), lit(5), 22) +
                        R"(,{"node":"Return","origin":{"at":[16,19]},"expr":{"node":"Binary","op":"+","left":)" + name("xStack_12") +
                        R"(,"right":)" + name("xStack_e") + R"(,"type":{"kind":"i16"}}}])");
    const auto said = f16::lay_out(doc, code, 0, static_cast<std::uint32_t>(code.size()));
    CB_CHECK_EQ(nlohmann::json(said).dump(), R"(["xStack_12: local_10's bytes 0..","xStack_e: local_10's bytes 4.."])");
    // the declarations: the array first (nearest the frame pointer), then iVar1; the pieces are gone
    const nlohmann::json body = nlohmann::json::parse(doc.dump())["body"];
    CB_CHECK_EQ(body[0]["sym"]["name"].get<string>(), "local_10");
    CB_CHECK(body[0]["type"] == nlohmann::json::parse(R"({"kind":"array","elem":{"kind":"u8"},"count":16})"));
    CB_CHECK(body[0]["at"] == nlohmann::json::parse(R"({"stack":-18})"));
    CB_CHECK_EQ(body[1]["sym"]["name"].get<string>(), "iVar1");
    CB_CHECK_EQ(body[2]["node"].get<string>(), "ExprStmt");
    // the use at offset 4 reads *(i16 __ss *)(local_10 + 4)
    const auto& sum = body[3]["expr"];
    CB_CHECK_EQ(sum["left"]["node"].get<string>(), "Unary");
    CB_CHECK_EQ(sum["right"]["x"]["x"]["right"]["value"].get<int>(), 4);
    CB_CHECK(sum["right"]["x"]["type"]["to"] == nlohmann::json::parse(R"({"kind":"i16"})"));
    CB_CHECK(doc.dump().find("xStack_12") == string::npos);
});

CB_TEST("decompile.frame16.a_buffer_with_a_far_pointer_is_left", [] {
    const Bytes code = bytes_of("558bec83ec128d46f050e8000083c4028b46f00346f4c746ee05008be55dc3");
    Json doc = function("[" + local("pxStack_12", "i16", R"({"stack":-18})") + "," + local("xStack_e", "i16", R"({"stack":-14})") + "]");
    doc["body"][0]["type"] = cb::json::parse(R"({"kind":"ptr","space":"far","to":{"kind":"u8"}})");
    const auto said = f16::lay_out(doc, code, 0, static_cast<std::uint32_t>(code.size()));
    CB_CHECK_EQ(nlohmann::json(said).dump(), R"x(["a buffer left as it was: a far pointer in it, an array in the frame, or a local over it ([bp-16])"])x");
    CB_CHECK_EQ(doc["body"][1]["sym"]["name"].get<string>(), "pxStack_12");   // still declared as it was (nearest the frame pointer first)
    CB_CHECK_EQ(doc["body"][1]["type"]["space"].get<string>(), "far");
});

CB_TEST("decompile.frame16.nothing_to_lay_out", [] {
    // mov ax,1 / ret: no frame
    const Bytes plain = bytes_of("b80100c3");
    Json doc = function(R"([{"node":"Return","expr":)" + lit(1) + R"(,"origin":{"at":[0]}}])");
    const Json before = doc;
    CB_CHECK_EQ(nlohmann::json(f16::lay_out(doc, plain, 0, 4)).dump(), R"(["no frame"])");
    CB_CHECK_EQ(doc.dump(), before.dump());
    // a frame, but no local the instructions place
    const Bytes code = bytes_of(kFlagCode);
    Json lonely = function("[" + local("x", "i16") + "]");
    CB_CHECK_EQ(nlohmann::json(f16::lay_out(lonely, code, 0, static_cast<std::uint32_t>(code.size()))).dump(), R"(["no local the frame places"])");
    // no body at all
    Json bodyless = cb::json::parse(R"({"node":"MethodDecl"})");
    CB_CHECK_EQ(nlohmann::json(f16::lay_out(bodyless, code, 0, static_cast<std::uint32_t>(code.size()))).dump(), R"(["no local the frame places"])");
    // a range past the code is the part that exists (none): no frame
    CB_CHECK_EQ(nlohmann::json(f16::lay_out(lonely, code, 900, 1000)).dump(), R"(["no frame"])");
    // ENTER makes the frame as well
    const Bytes entered = bytes_of("c8040000c746fe00008be55dc3");
    Json doc2 = function("[" + local("bVar1", "bool", R"({"stack":-4})") + "," + assign(name("bVar1", "bool"), lit(0), 4) + "]");
    CB_CHECK_EQ(nlohmann::json(f16::lay_out(doc2, entered, 0, static_cast<std::uint32_t>(entered.size()))).dump(), R"(["bVar1: 2 bytes at [bp-2]"])");
});

CB_TEST("decompile.frame16.a_decoder_of_the_callers", [] {
    // the decoder the caller gives is asked for the function's range, as it sits in the segment
    int asked = 0;
    Json doc = function("[]");
    const Bytes code = bytes_of(kFlagCode);
    f16::lay_out(doc, code, 3, 9, [&](cb::ByteView range, std::uint32_t at) {
        ++asked;
        CB_CHECK_EQ(range.size(), static_cast<std::size_t>(6));
        CB_CHECK_EQ(at, 3u);
        return std::vector<cb::decompile::inline_asm::Instruction>{};
    });
    CB_CHECK_EQ(asked, 1);
});
