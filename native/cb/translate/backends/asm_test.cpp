#include "cb/translate/backends/asm.hpp"

#include "cb/core/testing.hpp"

namespace {
namespace as = cb::translate::backends::assembly;

as::MachineFragment fragment(std::string name, std::string text) {
    return {.name = std::move(name), .text = std::move(text)};
}
}  // namespace

CB_TEST("translate.asm.comment", [] {
    CB_CHECK_EQ(as::comment(""), std::string(";"));
    CB_CHECK_EQ(as::comment("segment CODE"), std::string("; segment CODE"));
});

CB_TEST("translate.asm.module_joins_parts_by_line_break", [] {
    // a writer's module: a header comment, two fragments of machine code, a blank comment
    as::Document doc;
    doc.units = {as::Comment{"lz32.dll code"}, fragment("start", "start:\n    xor eax, eax\n    ret"),
                 as::Comment{""}, fragment("data", "db 1, 2, 3")};
    const auto text = as::module(doc);
    CB_CHECK(text.ok());
    CB_CHECK_EQ(text.value(), std::string("; lz32.dll code\nstart:\n    xor eax, eax\n    ret\n;\ndb 1, 2, 3"));
});

CB_TEST("translate.asm.first_line_of_each_unit", [] {
    as::Document doc;
    as::MachineFragment inner;
    inner.name = "inner";
    inner.within = true;   // its text is in the fragment that holds it
    doc.units = {fragment("a", "one\ntwo\nthree"), inner, as::Comment{"c"}, fragment("b", "x")};
    const auto asmd = as::assemble(doc).value();
    CB_CHECK_EQ(asmd.text, std::string("one\ntwo\nthree\n; c\nx"));
    CB_CHECK_EQ(asmd.first_line.size(), std::size_t{4});
    CB_CHECK_EQ(asmd.first_line[0].value(), std::size_t{1});
    CB_CHECK(!asmd.first_line[1]);
    CB_CHECK_EQ(asmd.first_line[2].value(), std::size_t{4});
    CB_CHECK_EQ(asmd.first_line[3].value(), std::size_t{5});
});

CB_TEST("translate.asm.empty_text_is_a_line", [] {
    // a fragment with an empty text is still a part: it takes its (empty) line, and the break after it stays
    as::Document doc;
    doc.units = {fragment("a", ""), fragment("b", "x")};
    const auto asmd = as::assemble(doc).value();
    CB_CHECK_EQ(asmd.text, std::string("\nx"));
    CB_CHECK_EQ(asmd.first_line[1].value(), std::size_t{2});
    // no units: an empty file
    CB_CHECK_EQ(as::module(as::Document{}).value(), std::string());
});

CB_TEST("translate.asm.declines", [] {
    // a unit of another kind is not assembly
    as::Document doc;
    doc.units = {fragment("a", "nop"), as::OtherUnit{"Function"}};
    const auto r = as::module(doc);
    CB_CHECK(!r.ok());
    CB_CHECK_THROWS_AS(as::Declined, (void)r.value());
    // a fragment with no text that is not `within` another
    as::Document bare;
    bare.units = {as::MachineFragment{.name = "oops"}};
    CB_CHECK(!as::module(bare).ok());
    // a document that is not a Module
    as::Document other;
    other.kind = "Unit";
    other.units = {fragment("a", "nop")};
    CB_CHECK(!as::assemble(other).ok());
});

CB_TEST("translate.asm.comment_leads_by_syntax", [] {
    // an assembler with another comment lead says so; the table is an argument, not a global
    const as::CommentLeads leads{{"gas", "#"}, {"nasm", ";"}, {"", "//"}};
    CB_CHECK_EQ(as::comment("c", "gas", leads), std::string("# c"));
    CB_CHECK_EQ(as::comment("", "gas", leads), std::string("#"));
    CB_CHECK_EQ(as::comment("c", "nasm", leads), std::string("; c"));
    CB_CHECK_EQ(as::comment("c", "", leads), std::string("// c"));       // (the default syntax may be given a lead too)
    CB_CHECK_EQ(as::comment("c", "masm", leads), std::string("; c"));    // a syntax with no entry: a semicolon
    CB_CHECK_EQ(as::comment("c", "gas"), std::string("; c"));            // no table: a semicolon
    as::Document doc;
    doc.units = {as::Comment{"a", "gas"}, as::Comment{"b", "nasm"}, as::Comment{"c", ""}};
    CB_CHECK_EQ(as::module(doc, leads).value(), std::string("# a\n; b\n// c"));
    CB_CHECK_EQ(as::module(doc).value(), std::string("; a\n; b\n; c"));
});
