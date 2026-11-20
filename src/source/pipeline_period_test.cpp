// Tests of the text passes `repair <32-bit module>` gives a period compiler (pipeline_period.hpp) and of the run-time library `match <16-bit module>`
// names (pipeline_match16.hpp). They cover period source, a run-time library routine named outside
// the linker's bytes, and the header a moved-field function needs.
#include "source/pipeline_period.hpp"

#include <string>
#include <vector>

#include "core/testing.hpp"
#include "source/pipeline_match16.hpp"

namespace sp = cb::source::pipeline;
using cb::Bytes;

namespace {

bool has(const std::string& text, const std::string& part) { return text.find(part) != std::string::npos; }

}  // namespace

CB_TEST("source.pipeline_period.a_period_source_has_the_declarations_an_author_had", [] {
    const std::string header =
        "typedef int int4;\nint __stdcall HeapFree();\n#define iRam00401000 (*(int4 *)0x401000)\n#define iRam00900000 (*(int4 *)0x900000)\n"
        "int4 f(int4 a1);\nint4 __cdecl g(void);";
    const std::unordered_map<std::string, std::string> texts = {
        {"f", "int4 f(int4 a1)\n{\n  g();\n  return HeapFree(iRam00401000,0,a1 -0x400eff90) + 0x402000 + 0x12345;\n}"},
        {"g", "int4 __cdecl g(void)\n{\n  int4 in_EAX;\n  \n  return in_EAX;\n}"}};
    const sp::RepairSource made = sp::repair_period_source(header, {"f", "g"}, texts, {"HeapFree"}, {0x401000, 0x402000, 0xbff10070});
    // an import is called through its slot
    CB_CHECK(has(made.header, "__declspec(dllimport) int __stdcall HeapFree();"));
    // a global is a variable, and only what the relocations name (the other macro stays a macro)
    CB_CHECK(has(made.header, "extern int4 iRam00401000;") && has(made.header, "#define iRam00900000"));
    // an address taken as a number is the address of a variable; a number no relocation names stays a number
    CB_CHECK(has(made.texts.at("f"), "a1 + (int4)&codebreaker_at_bff10070) + (int4)&codebreaker_at_00402000 + 0x12345"));
    CB_CHECK(has(made.header, "extern char codebreaker_at_00402000;"));
    // a function that only hands back what was in EAX returns nothing, and its declaration says so
    CB_CHECK(made.texts.at("g").starts_with("void __cdecl g(void)") && !has(made.texts.at("g"), "in_EAX") && has(made.header, "void __cdecl g(void);"));
    // the header a function needs: the import it never calls and the types stay, what it does not name goes
    const std::string near = sp::repair_needed_header(made.header, made.texts.at("g"));
    CB_CHECK(!has(near, "HeapFree") && has(near, "typedef int int4;") && has(near, "g(void);"));
});

CB_TEST("source.pipeline_period.a_function_whose_result_is_used_keeps_its_type", [] {
    // `g`'s result is taken by `f`, so what was in EAX is a result and the function is not made `void`
    const std::string header = "typedef int int4;\nint4 f(void);\nint4 __cdecl g(void);";
    const std::unordered_map<std::string, std::string> texts = {
        {"f", "int4 f(void)\n{\n  int4 x;\n  \n  x = g();\n  return x;\n}"}, {"g", "int4 __cdecl g(void)\n{\n  int4 in_EAX;\n  \n  return in_EAX;\n}"}};
    const sp::RepairSource made = sp::repair_period_source(header, {"f", "g"}, texts, {}, {});
    CB_CHECK(made.texts.at("g").starts_with("int4 __cdecl g(void)"));
    CB_CHECK(has(made.header, "int4 __cdecl g(void);") && !has(made.header, "void __cdecl g"));
});

CB_TEST("source.pipeline_period.the_needed_header_keeps_a_structure_only_when_the_function_names_it", [] {
    const std::string header =
        "typedef int int4;\ntypedef struct struct_1 {\n  int4 f_0;\n  uint1 _gap_4[4];\n  int4 f_8;\n  uint1 _gap_c[4];\n  code *f_10;\n} struct_1;";
    const std::string f = "int4 f(int4 a1,int4 a2)\n{\n  ((struct_1 *)a1)->f_8 = ((struct_1 *)a1)->f_0 + a2;\n}";
    const std::string g = "int4 g(int4 a1)\n{\n  return *(int4 *)(a1 + 8);\n}";
    const std::string near = sp::repair_needed_header(header, f);
    CB_CHECK(has(near, "int4 f_8;") && has(near, "struct_1"));
    const std::string none = sp::repair_needed_header(header, g);
    CB_CHECK(!has(none, "struct_1") && !has(none, "f_8") && has(none, "typedef int int4;"));
});

CB_TEST("source.pipeline_period.a_library_routine_is_named_outside_the_linkers_bytes", [] {
    // A function that is a library routine byte for byte, outside the places the linker filled in (the routine's fix-ups, the original's
    // relocations), is the library's -- __aFlmul in SUWIN.
    cb::symbols::libraries::Function routine;
    routine.name = "MDLLCEW.LIB:__aFlmul";
    routine.code = {0x55, 0x8B, 0xEC, 0x9A, 0, 0, 0, 0, 0x5D, 0xCA, 0x08, 0x00};
    routine.mask = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
    const Bytes linked = {0x55, 0x8B, 0xEC, 0x9A, 0x34, 0x12, 0xFF, 0xFF, 0x5D, 0xCA, 0x08, 0x00};
    const Bytes other = {0x55, 0x8B, 0xEC, 0x9A, 0x34, 0x12, 0xFF, 0xFF, 0x5D, 0xCA, 0x06, 0x00};
    std::vector<cb::verify::authored::match16::Function> functions(2);
    functions[0].name = "f";
    functions[0].original = {linked, Bytes(12, 0)};
    functions[1].name = "g";
    functions[1].original = {other, Bytes(12, 0)};
    const auto got = sp::identify_run_time_library({routine}, functions);
    CB_CHECK_EQ(got.size(), std::size_t{1});
    CB_CHECK_EQ(got.at("f"), std::string("MDLLCEW.LIB:__aFlmul"));
    // the longest routine that fits names it
    cb::symbols::libraries::Function longer = routine;
    longer.name = "MDLLCEW.LIB:__longer";
    longer.code.insert(longer.code.end(), {0x90, 0x90});
    longer.mask.insert(longer.mask.end(), {0, 0});
    const Bytes padded = [&] {
        Bytes b = linked;
        b.insert(b.end(), {0x90, 0x90});
        return b;
    }();
    functions[0].original = {padded, Bytes(14, 0)};
    CB_CHECK_EQ(sp::identify_run_time_library({routine, longer}, functions).at("f"), std::string("MDLLCEW.LIB:__longer"));
});
