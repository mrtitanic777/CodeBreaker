// Tests of the shared layer of the image writers, over a toy machine:
//
//     N          one byte, no flow             R          one byte, returns (flow stops)
//     J a        two bytes, jumps to a (stops)  B a        two bytes, branches to a (falls through too)
//     C a        two bytes, calls a (a is a function start)
//
// Any other byte is not an instruction.

#include "source/image_writer.hpp"

#include "build/asm_text.hpp"
#include "core/arena.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::source::image_writer {
namespace {

struct Toy {
    char op = 'N';
    std::uint32_t size = 1;
    std::optional<std::uint32_t> target;
};

class ToyFlow final : public CodeModel<Toy> {
public:
    ToyFlow(Bytes memory, bool find_functions) : memory_(std::move(memory)), find_(find_functions) {}

    std::optional<Toy> decode(std::uint32_t pc) const override {
        const char op = static_cast<char>(memory_[pc]);
        switch (op) {
            case 'N':
            case 'R': return Toy{op, 1, std::nullopt};
            case 'J':
            case 'B':
            case 'C': return Toy{op, 2, pc + 1 < memory_.size() ? std::optional<std::uint32_t>(memory_[pc + 1]) : std::nullopt};
            default: return std::nullopt;
        }
    }
    std::uint32_t size(const Toy& insn) const override { return insn.size; }
    std::optional<std::uint32_t> target(const Toy& insn) const override { return insn.target; }
    bool inside(std::uint32_t pc) const override { return pc < memory_.size(); }
    bool fits(std::uint32_t pc, const Toy& insn) const override { return pc + insn.size <= memory_.size(); }
    bool follows(const Toy& insn) const override { return insn.target && *insn.target < memory_.size(); }
    bool stops(const Toy& insn) const override { return insn.op == 'R' || insn.op == 'J'; }
    bool finds_functions() const override { return find_; }
    bool starts_function(const Toy& insn) const override { return insn.op == 'C'; }

private:
    Bytes memory_;
    bool find_;
};

Bytes program(std::string_view text) { return Bytes(text.begin(), text.end()); }

std::string joined(const std::vector<std::string>& lines) { return text::join(lines, "\n"); }

// 0: C 8 | 2: B 6 | 4: J 0 | 6: N R | 8: N R | 10: a byte that is not an instruction
const Bytes kFlowProgram = {'C', 8, 'B', 6, 'J', 0, 'N', 'R', 'N', 'R', 'X'};

CB_TEST("source.image_writer.walk_follows_every_path_and_finds_called_functions", [] {
    const ToyFlow flow(kFlowProgram, true);
    const std::uint32_t entries[] = {0};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    std::vector<std::uint32_t> keys;
    for (const auto& entry : w.code) keys.push_back(entry.first);
    CB_CHECK((keys == std::vector<std::uint32_t>{0, 2, 4, 6, 7, 8, 9}));
    CB_CHECK((w.targets == std::set<std::uint32_t>{0, 6, 8}));
    CB_CHECK((w.starts == std::vector<std::uint32_t>{0, 8}));   // the entry, and what is called
    CB_CHECK_EQ(w.code.at(2).op, 'B');
    CB_CHECK_EQ(code_bytes(w.code, flow), 2u + 2 + 2 + 1 + 1 + 1 + 1);
});

CB_TEST("source.image_writer.walk_without_function_finding_keeps_the_entries_as_they_are", [] {
    const ToyFlow flow(kFlowProgram, false);
    const std::uint32_t entries[] = {2, 0, 2};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    CB_CHECK((w.starts == std::vector<std::uint32_t>{2, 0, 2}));   // as given: no sorting, no merging
    CB_CHECK((w.targets == std::set<std::uint32_t>{0, 6, 8}));
});

CB_TEST("source.image_writer.walk_stops_at_bytes_that_are_not_code_and_at_a_cut_instruction", [] {
    const ToyFlow flow(program("NNXNN"), false);
    const std::uint32_t entries[] = {0};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    CB_CHECK_EQ(w.code.size(), 2u);   // the X ends the run; what is behind it is not reached
    const ToyFlow cut(program("NJ"), false);   // a jump whose operand is not in the program
    const Walked<Toy> c = walk<Toy>(cut, entries);
    CB_CHECK_EQ(c.code.size(), 1u);
    const std::uint32_t outside[] = {99};
    CB_CHECK(walk<Toy>(cut, outside).code.empty());   // an entry outside the program is no code
});

CB_TEST("source.image_writer.walk_does_not_follow_a_target_the_model_refuses", [] {
    // B 200: its target lies outside the program, so it is neither followed nor a label.
    const ToyFlow flow(Bytes{'B', 200, 'R'}, false);
    const std::uint32_t entries[] = {0};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    CB_CHECK(w.targets.empty());
    CB_CHECK_EQ(w.code.size(), 2u);
});

CB_TEST("source.image_writer.covered_is_what_lies_inside_an_instruction", [] {
    const ToyFlow flow(kFlowProgram, false);
    const std::uint32_t entries[] = {0};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    CB_CHECK((covered(w.code, flow) == std::set<std::uint32_t>{1, 3, 5}));
});

CB_TEST("source.image_writer.byte_marks_are_covered_without_a_node_per_byte", [] {
    const ToyFlow flow(kFlowProgram, false);
    const std::uint32_t entries[] = {0};
    const Walked<Toy> w = walk<Toy>(flow, entries);
    const ByteMarks inside = covered_marks(w.code, flow, kFlowProgram.size());
    std::set<std::uint32_t> marked;
    for (std::uint32_t a = 0; a < 40; ++a)
        if (inside.contains(a)) marked.insert(a);
    CB_CHECK((marked == covered(w.code, flow)));   // the same positions as covered(), and none past the image
    ByteMarks grown;
    CB_CHECK(!grown.contains(100));
    grown.insert(100);   // (a mark past the size given grows the table)
    CB_CHECK(grown.contains(100));
    CB_CHECK(!grown.contains(99));
    CB_CHECK(!grown.contains(101));
});

// What walk_beyond finds, merged into what was known, is what walk() finds from the entry alone, for every entry of the program and every amount asked for.
CB_TEST("source.image_writer.walk_beyond_adds_to_known_code_what_walk_would_find", [] {
    for (const bool find_functions : {false, true}) {
        const ToyFlow flow(kFlowProgram, find_functions);
        const std::uint32_t first[] = {8};
        for (std::uint32_t entry = 0; entry < kFlowProgram.size(); ++entry) {
            const Walked<Toy> known = walk<Toy>(flow, first);   // (8: N R)
            const std::uint32_t one[] = {entry};
            const Walked<Toy> whole = walk<Toy>(flow, one);
            for (const std::size_t enough : {1u, 2u, 3u, 6u, 100u}) {
                const Beyond<Toy> b = walk_beyond<Toy>(flow, known.code, entry, enough);
                std::map<std::uint32_t, Toy> merged = known.code;
                for (const auto& [at, insn] : b.code) merged.insert_or_assign(at, insn);
                std::map<std::uint32_t, Toy> expected = known.code;
                for (const auto& [at, insn] : whole.code) expected.insert_or_assign(at, insn);
                CB_CHECK_EQ(merged.size(), expected.size());
                CB_CHECK_EQ(b.reached >= enough, whole.code.size() >= enough);   // the decision is the same
                if (whole.code.size() < enough) CB_CHECK_EQ(b.reached, whole.code.size());
                for (const std::uint32_t t : b.targets) CB_CHECK(whole.targets.contains(t));
                for (const std::uint32_t s : b.starts) CB_CHECK(std::ranges::find(whole.starts, s) != whole.starts.end());
            }
        }
    }
});

CB_TEST("source.image_writer.spelling", [] {
    CB_CHECK_EQ((Spelling{"sub_", 4})(0x1234), "sub_1234");
    CB_CHECK_EQ((Spelling{"L_", 6})(0x2F), "L_00002F");
    CB_CHECK_EQ((Spelling{"D_", 4})(0x12345), "D_12345");   // wider than the field: not cut
});

CB_TEST("source.image_writer.functions_keep_the_names_given_and_make_up_the_rest", [] {
    Names names = {{0, "reset"}};
    const std::uint32_t starts[] = {0, 0x10};
    const Functions functions = name_functions(starts, names, Spelling{"sub_", 4});
    CB_CHECK((functions == Functions{{0, "reset"}, {0x10, "sub_0010"}}));
    CB_CHECK_EQ(names.at(0x10), "sub_0010");   // the made-up name is added to the names
});

CB_TEST("source.image_writer.labels_stand_where_a_line_starts_and_a_name_given_wins", [] {
    const Names names = {{0x20, "named"}, {0x30, "inside"}, {0x10, "given"}};
    const std::set<std::uint32_t> targets = {0x10, 0x11, 0x40};
    const Names labels = make_labels(
        targets, names, [](std::uint32_t a) { return a != 0x11 && a != 0x30; }, Spelling{"L_", 4});
    CB_CHECK((labels == Names{{0x10, "given"}, {0x20, "named"}, {0x40, "L_0040"}}));
});

CB_TEST("source.image_writer.data_lines_rows_strings_and_fills", [] {
    CB_CHECK_EQ(joined(data_lines(Bytes{})), "");
    CB_CHECK_EQ(joined(data_lines(Bytes{'A', 'B', 'C', 'D', 1})), "        .DB \"ABCD\",$01");   // four printable characters make a string
    CB_CHECK_EQ(joined(data_lines(Bytes{'A', 'B', 'C'})), "        .DB $41,$42,$43");               // three do not
    CB_CHECK_EQ(joined(data_lines(Bytes{0x22, 'A', 'B', 'C', 'D', 'E'})), "        .DB $22,\"ABCDE\"");   // the quote is never inside a string
    CB_CHECK_EQ(joined(data_lines(Bytes{0x5C, 'A', 'B', 'C', 'D'})), "        .DB $5C,\"ABCD\"");     // nor the backslash
    CB_CHECK_EQ(joined(data_lines(Bytes(16, 0))), "        .DSB 16,$00");
    CB_CHECK_EQ(joined(data_lines(Bytes(15, 0))), "        .DB " + text::join(std::vector<std::string>(15, "$00"), ","));   // 15 is still a row
    Bytes mixed = {1, 2};
    mixed.insert(mixed.end(), 20, 0xFF);
    mixed.push_back(3);
    CB_CHECK_EQ(joined(data_lines(mixed)), "        .DB $01,$02\n        .DSB 20,$FF\n        .DB $03");
    const DataSyntax gb{.bytes = "db", .fill = "ds", .fill_separator = ", "};
    CB_CHECK_EQ(joined(data_lines(mixed, gb)), "        db $01,$02\n        ds 20, $FF\n        db $03");
});

CB_TEST("source.image_writer.data_lines_split_long_runs_into_rows_of_sixteen", [] {
    Bytes data;
    for (int i = 0; i < 40; ++i) data.push_back(static_cast<std::uint8_t>(0x80 + i));   // none printable, no runs
    const std::vector<std::string> lines = data_lines(data);
    CB_CHECK_EQ(lines.size(), 3u);
    CB_CHECK_EQ(lines[0].size(), 8u + 4 + 16 * 3 + 15);   // 8 spaces, ".DB ", sixteen "$XX", fifteen commas
    CB_CHECK(lines[2].ends_with("$A7"));
});

CB_TEST("source.image_writer.commented_marks_the_first_line", [] {
    CB_CHECK((commented({"a", "b"}, "what") == std::vector<std::string>{"a  ; what", "b"}));
    CB_CHECK(commented({}, "what").empty());
});

// ---- emit -------------------------------------------------------------------------------------------------------

class ToyListing final : public Listing<Toy> {
public:
    ToyListing(translate::l2::Assembly& out, Bytes memory, const std::map<std::uint32_t, Toy>& code, std::uint32_t base)
        : out_(out), memory_(std::move(memory)), code_(code), base_(base) {}
    std::uint8_t byte(std::uint32_t offset) const override { return memory_[offset]; }
    std::uint32_t address(std::uint32_t offset) const override { return base_ + offset; }
    const Toy* code_at(std::uint32_t offset, std::uint32_t) const override {
        const auto it = code_.find(offset);
        return it == code_.end() ? nullptr : &it->second;
    }
    std::uint32_t size(const Toy& insn) const override { return insn.size; }
    std::string text(const Toy& insn) const override {
        return insn.target ? std::string(1, insn.op) + " " + std::to_string(*insn.target) : std::string(1, insn.op);
    }
    std::vector<std::string> data_lines(ByteView data) const override { return image_writer::data_lines(data); }
    std::uint32_t special(std::uint32_t offset, std::uint32_t, const Flush& flush) override {
        if (offset != special_at) return 0;
        flush();
        out_.append("; a header");
        return 2;
    }
    std::uint32_t special_at = 0xFFFF;

private:
    translate::l2::Assembly& out_;
    Bytes memory_;
    const std::map<std::uint32_t, Toy>& code_;
    std::uint32_t base_;
};

CB_TEST("source.image_writer.emit_writes_labels_functions_instructions_and_data", [] {
    // 0: N R | 2: data 41 42 | 4: N R   (at addresses 0x100...)
    const Bytes memory = {'N', 'R', 0x41, 0x42, 'N', 'R'};
    const std::map<std::uint32_t, Toy> code = {{0, {'N', 1, std::nullopt}}, {1, {'R', 1, std::nullopt}}, {4, {'N', 1, std::nullopt}}, {5, {'R', 1, std::nullopt}}};
    translate::l2::Assembly out({"; head"}, "toy", "prog.bin");
    ToyListing listing(out, memory, code, 0x100);
    emit<Toy>(out, 0, 6, Names{{0x100, "start"}, {0x104, "L_0104"}}, Functions{{0x100, "start"}}, listing);
    CB_CHECK_EQ(joined(out.lines()), "; head\n\nstart:\n        N\n        R\n        .DB $41,$42\nL_0104:\n        N\n        R");
    // the module is the head and the function as two fragments, the second with its place
    Arena arena;
    translate::ir::Node* doc = out.document(arena);
    const std::vector<translate::ir::Node*> units = translate::l2::units(doc);
    CB_CHECK_EQ(units.size(), 2u);
});

CB_TEST("source.image_writer.emit_flushes_data_before_a_special_place", [] {
    const Bytes memory = {0x41, 0x42, 'N', 'R'};
    const std::map<std::uint32_t, Toy> code = {{2, {'N', 1, std::nullopt}}, {3, {'R', 1, std::nullopt}}};
    translate::l2::Assembly out({}, "toy", "prog.bin");
    ToyListing listing(out, memory, code, 0);
    listing.special_at = 1;   // 2 bytes at offset 1 are a header: they are not read as data or as an instruction
    emit<Toy>(out, 0, 4, Names{}, Functions{}, listing);
    CB_CHECK_EQ(joined(out.lines()), "        .DB $41\n; a header\n        R");   // the header took the bytes at 1 and 2
});

CB_TEST("source.image_writer.emit_a_label_in_the_middle_of_data_splits_the_run", [] {
    const Bytes memory = {1, 2, 3, 4};
    translate::l2::Assembly out({}, "toy", "prog.bin");
    const std::map<std::uint32_t, Toy> none;
    ToyListing listing(out, memory, none, 0);
    emit<Toy>(out, 0, 4, Names{{2, "mid"}}, Functions{}, listing);
    CB_CHECK_EQ(joined(out.lines()), "        .DB $01,$02\nmid:\n        .DB $03,$04");
});

// ---- the result -------------------------------------------------------------------------------------------------

CB_TEST("source.image_writer.finish_writes_the_source_and_assembled_judges_the_rebuild", [] {
    fs::ScopedTempDir dir("cb_iw");
    Written r = finish(dir.path(), "x.asm", "line 1\nline 2\n", Functions{{1, "a"}}, 10, 100);
    CB_CHECK_EQ(fs::read_text(r.source), "line 1\nline 2\n");   // line ends as they are: no conversion
    CB_CHECK_EQ(r.code_bytes, 10u);
    CB_CHECK_EQ(r.image_bytes, 100u);
    CB_CHECK(!r.linked && !r.same);

    const Bytes original = {1, 2, 3, 4};
    const std::optional<Bytes> built = assembled(r, [&] { return original; }, original, dir.path() / "x.built");
    CB_CHECK(built && *built == original);
    CB_CHECK(r.linked && r.same && r.notes.empty());
    CB_CHECK(r.built && fs::read_file(*r.built) == original);

    Written r2 = finish(dir.path(), "y.asm", "z", {}, 0, 0);
    assembled(r2, [&] { return Bytes{1, 2, 9, 4}; }, original, std::nullopt, "ROM");
    CB_CHECK(r2.linked && !r2.same);
    CB_CHECK_EQ(r2.notes.size(), 1u);
    CB_CHECK_EQ(r2.notes[0], "the built ROM differs from the original at byte 0x2");
    CB_CHECK(!r2.built);

    Written r3 = finish(dir.path(), "z.asm", "z", {}, 0, 0);
    assembled(r3, [&] { return Bytes{1, 2}; }, original);   // shorter: the first difference is where it ends
    CB_CHECK_EQ(r3.notes[0], "the built image differs from the original at byte 0x2");
});

CB_TEST("source.image_writer.assembled_reports_source_that_does_not_assemble", [] {
    fs::ScopedTempDir dir("cb_iw");
    Written r = finish(dir.path(), "x.asm", "bad", {}, 0, 0);
    const std::optional<Bytes> built = assembled(r, []() -> Bytes { throw build::AsmError(3, "foo", "unknown mnemonic"); }, Bytes{1});
    CB_CHECK(!built);
    CB_CHECK(!r.linked && !r.same);
    CB_CHECK_EQ(r.notes.size(), 1u);
    CB_CHECK(r.notes[0].starts_with("does not assemble: "));
    CB_CHECK(r.notes[0].find("unknown mnemonic") != std::string::npos);   // the assembler's own words
});

CB_TEST("source.image_writer.assembled_does_not_hide_other_faults", [] {
    fs::ScopedTempDir dir("cb_iw");
    Written r = finish(dir.path(), "x.asm", "z", {}, 0, 0);
    CB_CHECK_THROWS_AS(IoError, assembled(r, []() -> Bytes { throw IoError("x", 2, "cannot read"); }, Bytes{1}));
});

CB_TEST("source.image_writer.assembled_with_its_own_test_of_sameness", [] {
    fs::ScopedTempDir dir("cb_iw");
    Written r = finish(dir.path(), "x.asm", "z", {}, 0, 0);
    // a block whose built image must also stand at its address: the caller's own test
    assembled(r, [] { return Bytes{7}; }, Bytes{7}, std::nullopt, "block", [](ByteView) { return false; });
    CB_CHECK(r.linked && !r.same);
});

CB_TEST("source.image_writer.report_says_what_was_rebuilt", [] {
    Written r;
    r.source = "out/game.s";
    r.functions = {{0, "a"}, {4, "b"}};
    r.code_bytes = 120;
    r.image_bytes = 32768;
    r.same = true;
    const Report same = report("dir/game.nes", r, "PRG bytes");
    CB_CHECK_EQ(same.text, "game.nes: 2 functions, 120 of 32768 PRG bytes reached as code -> out/game.s\n  rebuilt: identical to the original\n");
    CB_CHECK_EQ(same.exit_code, 0);
    r.same = false;
    r.notes = {"the built image differs from the original at byte 0x10", "second"};
    const Report differs = report("game.nes", r);
    CB_CHECK(differs.text.ends_with("  rebuilt: the built image differs from the original at byte 0x10; second\n"));
    CB_CHECK_EQ(differs.exit_code, 1);
    r.notes.clear();
    CB_CHECK(report("game.nes", r).text.ends_with("  rebuilt: differs\n"));
});

}  // namespace
}  // namespace cb::source::image_writer
