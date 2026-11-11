#include "cb/decompile/asm_blocks.hpp"

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"

namespace ab = cb::decompile::asm_blocks;
namespace ia = cb::decompile::inline_asm;
using cb::Bytes;
using cb::Json;
using std::string;

// DIVERGES from the Python module (its old harness, 1,848 place() cases built from a recorded decoder plus 7,000 calls of
// its helpers, agreed on everything else; the expectations in testdata/asm_blocks_cases.jsonl were produced by the
// original and read through by hand):
//  - a window that leaves no instruction in the block (the ends moved past each other) is reported ("the window leaves no
//    instruction in the block") and nothing is placed; the original wrote an empty MachineFragment and reported a block
//    "0020-0017" (or raised IndexError when an end went below the first instruction);
//  - statements of an else-part that the carry block swallows (the part is in the block's asm) are not looked at, so
//    place() no longer reports blocks it found in a list that has already left the tree;
//  - `[bp - 08]` (a decimal with a leading zero) names the place; the original raised ValueError (int("08", 0));
//  - the tree is edited in place (the original copied it first); the Python's `name_at` and `seg` arguments, which it never
//    read, are gone, and `fixups` is the set of offsets that carry a relocation;
//  - an operand's `size` and `access` come from Capstone as the Operand fields.

namespace {

Bytes bytes_of(const string& hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(static_cast<std::uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16)));
    return out;
}

// the document as Python's json.dumps(sort_keys=True) would write it: the comparison is by content, not by key order
string canonical(const Json& j) { return nlohmann::json::parse(j.dump()).dump(); }

std::vector<string> said_list(const nlohmann::json& j) {
    std::vector<string> out;
    for (const auto& s : j) out.push_back(s.get<string>());
    return out;
}

}  // namespace

// Hand-built functions (testdata/asm_blocks_cases.jsonl: the document, the code, the original's tree and report).
CB_TEST("decompile.asm_blocks.place_cases", [] {
    const string data = cb::fs::read_text(cb::testing::data_path("decompile/testdata/asm_blocks_cases.jsonl"));
    int cases = 0;
    for (std::string_view line : cb::text::lines(data)) {
        if (line.empty()) continue;
        const nlohmann::json c = nlohmann::json::parse(line);
        const string name = c["name"].get<string>();
        Json doc = cb::json::parse(c["doc"].dump());
        ab::FixupSites fixups;
        for (const auto& f : c["fixups"]) fixups.insert(f.get<std::uint32_t>());
        const ab::Window window{c["window"][0].get<int>(), c["window"][1].get<int>()};
        const auto said = ab::place(doc, bytes_of(c["code"].get<string>()), c["start"].get<std::uint32_t>(), c["end"].get<std::uint32_t>(),
                                    fixups, window);
        CB_CHECK_EQ(canonical(doc), nlohmann::json::parse(c["doc_after"].dump()).dump());
        if (said != said_list(c["said"])) {
            CB_CHECK_EQ(name + ": " + nlohmann::json(said).dump(), name + ": " + c["said"].dump());
        }
        ++cases;
    }
    CB_CHECK(cases >= 20);
});

CB_TEST("decompile.asm_blocks.carry_block", [] {
    // push bp / mov bp,sp / sub sp,4 | mov ax,4400h / mov bx,5 / int 21h | jb +3 | mov ax,[bp-2] | mov sp,bp / pop bp / ret
    const Bytes code = bytes_of("558bec83ec04b80044bb0500cd2172038b46fe8be55dc3");
    Json doc = cb::json::parse(R"({
      "node":"Function","name":"f","sym":{"name":"f","type":{"distance":"far","params":[]}},
      "body":[
        {"node":"Local","sym":{"name":"in_AX"}},
        {"node":"ExprStmt","expr":{"node":"Call","target":{"node":"Name","sym":{"name":"int21"}},"args":[]},"origin":{"at":[6,9,12]}},
        {"node":"If","cond":{"node":"Name","sym":{"name":"int_CF"}},"origin":{"at":[14]},
         "then":[{"node":"ExprStmt","expr":{"node":"Name","sym":{"name":"err"}},"origin":{"at":[16]}}]},
        {"node":"Return","origin":{"at":[19]}}]})");
    const auto said = ab::place(doc, code, 0, static_cast<std::uint32_t>(code.size()));
    CB_CHECK_EQ(nlohmann::json(said).dump(),
                R"(["INT 21h: an _asm block 0001-000c","its carry: an _asm block 000e-000e, the then-part at L_0010"])");
    // the block replaces the three statements of the INT; the carry test is a block of its own; the then-part is labelled
    // and a label follows it
    const nlohmann::json out = nlohmann::json::parse(doc.dump());
    const auto& body = out["body"];
    CB_CHECK_EQ(body.size(), static_cast<std::size_t>(5));
    CB_CHECK_EQ(body[0]["node"].get<string>(), "MachineFragment");
    CB_CHECK_EQ(body[0]["why"].get<string>(), "INT 21h");
    CB_CHECK_EQ(body[0]["length"].get<int>(), 13);   // mov bp,sp / sub sp,4 (they set registers too) / mov / mov / int
    CB_CHECK_EQ(body[0]["asm"].dump(), R"(["mov bp, sp","sub sp, 4","mov ax, 0x4400","mov bx, 5","int 0x21"])");
    CB_CHECK_EQ(body[1]["why"].get<string>(), "the carry INT 21h left");
    CB_CHECK_EQ(body[1]["asm"].dump(), R"(["jb L_0013"])");
    CB_CHECK_EQ(body[2]["label"].get<string>(), "L_0010");
    CB_CHECK_EQ(body[3]["label"].get<string>(), "L_0013");
    CB_CHECK(body[3]["body"].empty());
    CB_CHECK_EQ(body[4]["node"].get<string>(), "Return");
    CB_CHECK(canonical(doc).find("in_AX") == string::npos);   // the local only the block used is no longer declared
});

CB_TEST("decompile.asm_blocks.place_degenerate_window", [] {
    const Bytes code = bytes_of("558bec83ec04b80044bb0500cd2172038b46fe8be55dc3");
    Json doc = cb::json::parse(R"({"body":[{"node":"ExprStmt","expr":{"node":"Call","target":{"node":"Name","sym":{"name":"int21"}}},"origin":{"at":[12]}}]})");
    const Json before = doc;
    const auto said = ab::place(doc, code, 0, static_cast<std::uint32_t>(code.size()), {}, ab::Window{20, -20});
    CB_CHECK_EQ(nlohmann::json(said).dump(), R"(["the window leaves no instruction in the block"])");
    CB_CHECK_EQ(canonical(doc), canonical(before));
    // no instructions: the range is past the code
    CB_CHECK_EQ(nlohmann::json(ab::place(doc, code, 500, 600)).dump(), R"(["no instructions"])");
    // a decoder of the caller's
    int asked = 0;
    ab::place(doc, code, 0, 10, {}, {}, [&](cb::ByteView c, std::uint32_t at) {
        ++asked;
        CB_CHECK_EQ(c.size(), static_cast<std::size_t>(10));
        CB_CHECK_EQ(at, 0u);
        return std::vector<ia::Instruction>{};
    });
    CB_CHECK_EQ(asked, 1);
});

CB_TEST("decompile.asm_blocks.interrupt_called", [] {
    const auto call = [](const char* name) {
        return cb::json::parse(std::string(R"({"node":"ExprStmt","expr":{"node":"Call","target":{"node":"Name","sym":{"name":")") + name + R"("}}}})");
    };
    CB_CHECK_EQ(ab::interrupt_called(call("int21")).value(), 0x21u);
    CB_CHECK_EQ(ab::interrupt_called(call("int2f_xyz")).value(), 0x2fu);
    CB_CHECK_EQ(ab::interrupt_called(call("int21_a_b")).value(), 0x21u);
    CB_CHECK_EQ(ab::interrupt_called(call("int21__")).value(), 0x21u);   // (`_[a-z_]+`: underscores count)
    for (const char* bad : {"int2g", "int21_", "Int21", "int21x", "int2", "int", "int21_A", "xint21", ""}) {
        CB_CHECK(!ab::interrupt_called(call(bad)).has_value());
    }
    // the first call that is a helper, wherever it sits in the expression
    const Json nested = cb::json::parse(
        R"({"expr":{"node":"Binary","left":{"node":"Call","target":{"node":"Name","sym":{"name":"strlen"}}},
            "right":{"node":"Call","target":{"node":"Name","sym":{"name":"int10"}}}}})");
    CB_CHECK_EQ(ab::interrupt_called(nested).value(), 0x10u);
    CB_CHECK(!ab::interrupt_called(cb::json::parse(R"({"node":"ExprStmt"})")).has_value());
    CB_CHECK(!ab::interrupt_called(cb::json::parse("[]")).has_value());
});

CB_TEST("decompile.asm_blocks.condition_reads", [] {
    const Json stmt = cb::json::parse(
        R"({"node":"If","cond":{"node":"Unary","op":"!","operand":{"node":"Name","sym":{"name":"int_ZF"}}},"then":[]})");
    const std::vector<std::string_view> flags = {"int_CF", "int_ZF"};
    const std::vector<std::string_view> other = {"int_CF"};
    CB_CHECK(ab::condition_reads(stmt, flags));
    CB_CHECK(!ab::condition_reads(stmt, other));
    CB_CHECK(!ab::condition_reads(cb::json::parse(R"({"node":"If"})"), flags));
});

CB_TEST("decompile.asm_blocks.frame_names", [] {
    const Json doc = cb::json::parse(R"({
      "sym":{"type":{"params":[{"name":"hFile","at":{"stack":4}},{"name":"pcBuf","at":{"stack":6}},{"name":"gone"},{"name":"x","at":{"stack":null}}]}},
      "body":[{"node":"ExprStmt","expr":{"node":"Binary","left":{"node":"Name","sym":{"name":"wStack_2"}},
               "right":{"node":"Name","sym":{"name":"pcStack_10c"}}}},
              {"node":"Name","sym":{"name":"Stack_4"}},{"node":"Name","sym":{"name":"uStack_4"}},{"node":"Name","sym":{"name":"xStack_zz"}},
              {"node":"Name","sym":{"name":"wStack_"}},{"node":"Name","sym":{"name":"WStack_8"}}]})");
    const ab::Frame frame = ab::frame_names(doc);
    // BP is the entry SP less 2: a parameter at 4 is [bp+6]; Stack_N is [bp+2-N]; a parameter wins over a local of its place
    CB_CHECK_EQ(frame.size(), static_cast<std::size_t>(5));
    CB_CHECK_EQ(frame.at(6), "hFile");
    CB_CHECK_EQ(frame.at(8), "pcBuf");
    CB_CHECK_EQ(frame.at(0), "wStack_2");
    CB_CHECK_EQ(frame.at(2 - 0x10c), "pcStack_10c");
    CB_CHECK_EQ(frame.at(-2), "Stack_4");   // (the first name met for a place: uStack_4 came after)
    CB_CHECK(ab::frame_names(cb::json::parse("{}")).empty());
});

CB_TEST("decompile.asm_blocks.name_frame_places", [] {
    const ab::Frame frame = {{6, "hFile"}, {-4, "local"}, {0x10, "far"}};
    CB_CHECK_EQ(ab::name_frame_places("word ptr [bp + 6]", frame), "word ptr hFile");
    CB_CHECK_EQ(ab::name_frame_places("mov ax, word ptr [bp - 4]", frame), "mov ax, word ptr local");
    CB_CHECK_EQ(ab::name_frame_places("[bp + 0x10], [bp - 0x4]", frame), "far, local");
    CB_CHECK_EQ(ab::name_frame_places("[bp + 8]", frame), "[bp + 8]");           // no name for the place
    CB_CHECK_EQ(ab::name_frame_places("[bp+6]", frame), "[bp+6]");               // (Capstone's spacing only)
    CB_CHECK_EQ(ab::name_frame_places("[bp + 6", frame), "[bp + 6");
    CB_CHECK_EQ(ab::name_frame_places("[bp + 0x]", frame), "[bp + 0x]");
    CB_CHECK_EQ(ab::name_frame_places("[bp + 0x6]", frame), "hFile");
    CB_CHECK_EQ(ab::name_frame_places("[bp - 04]", frame), "local");             // DIVERGES: Python raised
    CB_CHECK_EQ(ab::name_frame_places("[bx + 6] [bp + 6] [bp + 6]", frame), "[bx + 6] hFile hFile");
    CB_CHECK_EQ(ab::name_frame_places("", frame), "");
});

CB_TEST("decompile.asm_blocks.feeds_and_lines", [] {
    const auto decode = [](const char* hex) { return ia::decode16(bytes_of(hex), 0x100).at(0); };
    CB_CHECK(ab::feeds_interrupt(decode("b83412")));       // mov ax, 0x1234
    CB_CHECK(ab::feeds_interrupt(decode("8b5e04")));       // mov bx, word ptr [bp + 4]
    CB_CHECK(ab::feeds_interrupt(decode("1e")));           // push ds
    CB_CHECK(ab::feeds_interrupt(decode("50")));           // push ax
    CB_CHECK(ab::feeds_interrupt(decode("33c0")));         // xor ax, ax
    CB_CHECK(!ab::feeds_interrupt(decode("8946fe")));      // mov [bp-2], ax: a store to memory
    CB_CHECK(!ab::feeds_interrupt(decode("e83412")));      // call
    CB_CHECK(!ab::feeds_interrupt(decode("eb02")));        // jmp
    CB_CHECK(!ab::feeds_interrupt(decode("cd21")));        // int
    CB_CHECK(!ab::feeds_interrupt(decode("c3")));          // ret
    CB_CHECK(!ab::feeds_interrupt(decode("66b800000000")));  // a 386 instruction
    CB_CHECK(!ab::feeds_interrupt(decode("53")) == false);   // push bx

    const ab::Frame frame = {{6, "hFile"}};
    const ab::FixupSites no_fixups;
    CB_CHECK_EQ(ab::inline_line(decode("8b5e06"), frame, {}, no_fixups).value(), "mov bx, word ptr hFile");
    CB_CHECK_EQ(ab::inline_line(decode("8b5e06"), {}, {}, no_fixups).value(), "mov bx, word ptr [bp + 6]");
    CB_CHECK_EQ(ab::inline_line(decode("f3a4"), frame, {}, no_fixups).value(), "rep movsb");
    CB_CHECK_EQ(ab::inline_line(decode("f2ae"), frame, {}, no_fixups).value(), "repne scasb");
    CB_CHECK(!ab::inline_line(decode("e80000"), frame, {}, no_fixups).has_value());                 // a call
    CB_CHECK(!ab::inline_line(decode("b80000"), frame, {}, ab::FixupSites{0x101}).has_value());     // a relocation inside it
    CB_CHECK_EQ(ab::inline_line(decode("b80000"), frame, {}, ab::FixupSites{0x103}).value(), "mov ax, 0");
    // jumps: to a labelled place (a jump at $100 to $104: rel 2), else nothing
    CB_CHECK_EQ(ab::inline_line(decode("7502"), frame, {{0x104, "L_0104"}}, no_fixups).value(), "jne L_0104");
    CB_CHECK(!ab::inline_line(decode("7502"), frame, {}, no_fixups).has_value());
    CB_CHECK(!ab::inline_line(decode("ffe3"), frame, {{0, "x"}}, no_fixups).has_value());   // jmp bx: no target to label
});

CB_TEST("decompile.asm_blocks.drop_unused_locals", [] {
    Json doc = cb::json::parse(R"({"body":[
        {"node":"Local","sym":{"name":"in_BX"}},{"node":"Local","sym":{"name":"kept"}},{"node":"Local","sym":{"name":"pcStack_10c"}},
        {"node":"Local","sym":{"name":"inasm"}},{"node":"Local"},
        {"node":"ExprStmt","expr":{"node":"Name","sym":{"name":"kept"}}},
        {"node":"ExprStmt","expr":{"node":"Name","sym":{"name":"_pcStack_10c"}}},
        {"node":"MachineFragment","asm":["mov ax, inasm","add 0x2f, bx"]},
        {"node":"If","then":[{"node":"Local","sym":{"name":"unaff_SS"}},{"node":"Local","sym":{"name":"kept"}}]}]})");
    ab::drop_unused_locals(doc);
    const auto body = nlohmann::json::parse(doc.dump())["body"];
    std::vector<string> declared;
    for (const auto& s : body) {
        if (s["node"] == "Local") declared.push_back(s.contains("sym") ? s["sym"]["name"].get<string>() : "(none)");
    }
    // in_BX goes; kept is used; pcStack_10c is used where `_pcStack_10c` is (a far pointer's halves); inasm is in a block;
    // a local with no name goes
    CB_CHECK_EQ(nlohmann::json(declared).dump(), R"(["kept","pcStack_10c","inasm"])");
    const auto& then = body[6]["then"];   // (the If: three locals, two statements and the fragment before it)
    CB_CHECK_EQ(then.size(), static_cast<std::size_t>(1));   // (in a nested list too)
    CB_CHECK_EQ(then[0]["sym"]["name"].get<string>(), "kept");
    // no body: nothing to do
    Json none = cb::json::parse(R"({"name":"f"})");
    ab::drop_unused_locals(none);
    CB_CHECK_EQ(none.dump(), R"({"name":"f"})");
});
