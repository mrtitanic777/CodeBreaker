// Tests of cb/build/msc6.hpp. The text passes are checked on the decompiler-shaped samples the original msc6.py was checked
// on, with the outputs the original gave (read over by hand: the argument order, the far pointers, the folded temporaries).
// compile_many runs against a runner that stands in for DOSBox-X: it reads the C files the compile wrote and answers with
// object files (or the compiler's complaint), so what is tested is the batching, the bisecting of a refused file and the cache.
#include "cb/build/msc6.hpp"

#include <set>

#include "cb/base/cache.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/formats/omf.hpp"

namespace {

using namespace cb;
namespace msc6 = cb::build::msc6;
namespace wine16 = cb::symbols::wine16;

Bytes record(std::uint8_t kind, ByteView body) {
    Bytes r{kind, static_cast<std::uint8_t>((body.size() + 1) & 0xFF), static_cast<std::uint8_t>((body.size() + 1) >> 8)};
    append(r, body);
    r.push_back(0);
    return r;
}
Bytes record(std::uint8_t kind, std::string_view body) { return record(kind, as_bytes(body)); }

// A compiler's object file with one code segment, and an OMF with publics, as the harness of the original built them.
Bytes obj1() {
    Bytes o = record(0x80, std::string("\x03" "f.c", 4));
    append(o, record(0xA0, Bytes{1, 0, 0, 0x55, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB}));
    append(o, record(0x9C, Bytes{0xCC, 0x01, 0x26, 0x01, 0x01}));
    append(o, record(0xA0, std::string("\x02\x00\x00" "data", 7)));
    append(o, record(0x8A, Bytes{0}));
    return o;
}
Bytes obj2() {
    const Bytes helper{0x55, 0x8B, 0xEC, 0x5D, 0xC3}, judged{0x55, 0x9A, 0, 0, 0, 0, 0x5D, 0xCB};
    Bytes o = record(0x80, std::string("\x03" "f.c", 4));
    append(o, record(0x96, std::string("\x00\x05_TEXT\x04" "CODE", 12)));
    Bytes segdef{0x28, static_cast<std::uint8_t>(helper.size() + judged.size()), 0, 2, 3, 1};
    append(o, record(0x98, segdef));
    Bytes pub{0, 1};
    append(pub, to_bytes("\x07_helper"));
    append(pub, Bytes{0, 0, 0});
    append(pub, to_bytes("\x07_judged"));
    append(pub, Bytes{5, 0, 0});
    append(o, record(0x90, pub));
    append(o, record(0x8C, std::string("\x04_ext\x00", 6)));
    Bytes data{1, 0, 0};
    append(data, helper);
    append(data, judged);
    append(o, record(0xA0, data));
    append(o, record(0x9C, Bytes{0xCC, 0x06, 0x26, 0x01, 0x01}));
    append(o, record(0x8A, Bytes{0}));
    return o;
}

bool has(const std::string& s, std::string_view part) { return s.find(part) != std::string::npos; }

std::string hex(ByteView b) {
    std::string out;
    for (const std::uint8_t c : b) out += std::format("{:02x}", c);
    return out;
}

std::string lf(std::string s) { return text::replace_all(s, "\r\n", "\n"); }

}  // namespace

// ---- object files ---------------------------------------------------------------------------------------------------

CB_TEST("build.msc6.object_code_marks_what_the_linker_fills_in", [] {
    const msc6::Code one = msc6::object_code(obj1());
    CB_CHECK_EQ(hex(one.code), "559a000000005dcb");
    CB_CHECK_EQ(hex(one.mask), "0001010101000000");   // the far call's four bytes: a FIXUPP names them
    const msc6::Code two = msc6::object_code(obj2());
    CB_CHECK_EQ(hex(two.code), "558bec5dc3559a000000005dcb");
    CB_CHECK_EQ(hex(two.mask), "00000000000001010101000000");
    CB_CHECK(msc6::object_code({}).code.empty());
});

// DIVERGES: the original read a record that ran past the end of the file as far as the file went (a LEDATA's data cut short became
// the code, the clamped slice answering for the rest) and raised an IndexError where its fix-up record ran out. A record that holds
// data (LEDATA, FIXUPP) and does not fit the file is now a TruncatedError; any other record that does not fit ends the scan, and a
// record of no length is passed over. The compiler's own objects are never damaged; 13 of the 400 fuzzed byte strings of the old
// harness are answered differently for this reason.
CB_TEST("build.msc6.object_code_of_a_damaged_object_is_an_error", [] {
    CB_CHECK_THROWS_AS(FormatError, msc6::object_code(to_bytes(std::string("\xA0\x05\x00\x01", 4))));   // a record longer than the file
    Bytes bad = obj1();
    bad.resize(15);   // inside the first LEDATA
    CB_CHECK_THROWS_AS(FormatError, msc6::object_code(bad));
    // what is not data is not looked at: a record without a body, and a last record the file does not finish
    Bytes odd = obj1();
    odd.insert(odd.begin(), {0x88, 0x00, 0x00});   // a COMENT of no length first
    CB_CHECK_EQ(hex(msc6::object_code(odd).code), "559a000000005dcb");
    Bytes cut = obj1();
    cut.insert(cut.end() - 5, {0x88, 0x40, 0x00, 0x01});   // a COMENT that claims 64 bytes and the file ends first
    CB_CHECK_EQ(hex(msc6::object_code(cut).code), "559a000000005dcb");
});

CB_TEST("build.msc6.function_code_cuts_a_public_out_of_the_segment", [] {
    const Bytes obj = obj2();
    const auto judged = msc6::function_code(obj, "_judged");
    CB_CHECK(judged.has_value());
    CB_CHECK_EQ(hex(judged->code), "559a000000005dcb");
    CB_CHECK_EQ(hex(judged->mask), "0001010101000000");
    for (const char* name : {"judged", "@judged"}) CB_CHECK_EQ(hex(msc6::function_code(obj, name)->code), "559a000000005dcb");
    CB_CHECK_EQ(hex(msc6::function_code(obj, "_HELPER")->code), "558bec5dc3");   // (case is not compared)
    CB_CHECK(!msc6::function_code(obj, "none").has_value());
    CB_CHECK(!msc6::function_code(obj, "").has_value());
    CB_CHECK_THROWS_AS(FormatError, (void)msc6::function_code(obj1(), "x"));   // data for a segment the object never defined
});

// ---- the text of the reconstruction ------------------------------------------------------------------------------------

CB_TEST("build.msc6.dialect_says_the_header_the_way_msc6_takes_it", [] {
    const std::string h = msc6::dialect(
        "typedef unsigned long long uint8;\nuint2 codebreaker_ss(void);\n"
        "#pragma aux codebreaker_ss = \"mov ax,ss\" value [ax] modify exact [ax];\n"
        "#define __stdcall16far __far __stdcall\ntypedef long double float10;\nlong long x; unsigned long long y; long double z;\n");
    CB_CHECK(h.starts_with("#define __far _far\n#define __near _near\n#define __stdcall _pascal\n"));
    CB_CHECK(has(h, "#define __fastcall16far __far __fastcall\n#define MK_FP(s,o)"));
    // the 64-bit integers go, the pragma and the segment-register functions go, the rest stays in its order
    CB_CHECK(h.ends_with("typedef unsigned long uint8;\n#define __stdcall16far __far __stdcall\ntypedef double float10;\nlong x; unsigned long y; double z;\n\n"));
    CB_CHECK(!has(h, "pragma aux"));
    CB_CHECK(!has(h, "uint2 codebreaker_ss"));
    // an empty header is the definitions alone, and a blank line from the header's own end
    CB_CHECK(msc6::dialect("").ends_with(">> 16))\n\n"));
});

CB_TEST("build.msc6.pascal_names_and_order", [] {
    const std::string text =
        "int2 __stdcall16near f(param_1,param_2)\n{\n  USER_33((int2)&aiStack_c[0],unaff_SS,param_1);\n"
        "  return GDI_22(aiStack_c[3],aiStack_c[2],g(a, b),aiStack_c[0],param_2);\n  x = USER_25(\"a,(b\",0);\n}";
    const std::string header = "extern uint4 __far __stdcall GDI_22();\nextern uint4 __far __stdcall USER_33();";
    const std::vector<std::string> texts{header, text};
    const std::set<std::string> names = msc6::pascal_names(texts);
    CB_CHECK(names == (std::set<std::string>{"GDI_22", "USER_33", "f"}));
    CB_CHECK(msc6::pascal_names({}).empty());
    // the arguments of a pascal function and the parameters of its definition are first-pushed-first; an offset and its
    // segment register are one far pointer
    CB_CHECK_EQ(msc6::pascal_order(text, names),
                "int2 __stdcall16near f(param_2, param_1)\n{\n  USER_33(param_1, (void __far *)&aiStack_c[0]);\n"
                "  return GDI_22(param_2, aiStack_c[0], g(a, b), aiStack_c[2], aiStack_c[3]);\n  x = USER_25(\"a,(b\",0);\n}");
    const std::set<std::string> k{"K"};
    CB_CHECK_EQ(msc6::pascal_order("KERNEL_88((int2)param_1, (int2)((uint4)param_1 >> 0x10));", {"KERNEL_88"}), "KERNEL_88(param_1);");   // a long split into its words is the long
    CB_CHECK_EQ(msc6::pascal_order("K(K(a,b),K(c,(int2)d,codebreaker_ss()))", k),
                "K(K((void __far *)(char __near *)(d), c), K(b, a))");
    CB_CHECK_EQ(msc6::pascal_order("K((int2)&buf[0],unaff_SS,(int2)p,unaff_DS,x)", k),
                "K(x, (void __far *)(char __near *)(p), (void __far *)&buf[0])");
    CB_CHECK_EQ(msc6::pascal_order("K(a, (b, c", k), "K(a, (b, c");                     // unbalanced: left as it is
    CB_CHECK_EQ(msc6::pascal_order("K('(', \"a)\\\"\", z)", k), "K(z, \"a)\\\"\", '(')");   // literals hold their parentheses
    CB_CHECK_EQ(msc6::pascal_order("K()", k), "K()");
});

CB_TEST("build.msc6.cdecl_imports_swaps_the_convention_of_c_exports", [] {
    const std::string header =
        "extern uint4 __far __stdcall SETUPX_132();\nextern uint4 __far __stdcall USER_33();\nextern uint4 __far __stdcall KERNEL_88();\n";
    const msc6::OrdinalName names = [](std::string_view module, std::uint32_t ordinal) -> std::optional<std::string> {
        if (module == "SETUPX" && ordinal == 132) return "_SUFORMATMESSAGEBOX";   // a C export keeps the compiler's underscore
        if (module == "USER" && ordinal == 33) return "GetClientRect";
        if (module == "KERNEL" && ordinal == 88) return "lstrcpy";
        return std::nullopt;
    };
    const std::string swapped = msc6::cdecl_imports(header, "", names);
    CB_CHECK(has(swapped, "__cdecl SETUPX_132"));
    CB_CHECK(has(swapped, "__stdcall USER_33"));
    CB_CHECK(has(swapped, "__stdcall KERNEL_88"));
    const std::string sdk = msc6::cdecl_imports(header, "int CDECL lstrcpy(a);\nint CDECL USER_33 (x);\n", names);
    CB_CHECK(has(sdk, "__stdcall USER_33"));   // (the SDK says lstrcpy and USER_33 is not its name)
    CB_CHECK(has(sdk, "__cdecl KERNEL_88"));
});

CB_TEST("build.msc6.windows_h_and_the_sdk_header", [] {
    CB_CHECK_EQ(msc6::sdk_header("HDR", std::string("SDK")), "SDK\nHDR");
    CB_CHECK_EQ(msc6::sdk_header("HDR", std::string()), "HDR");
    CB_CHECK_EQ(msc6::sdk_header("HDR", std::nullopt), "HDR");
    fs::ScopedTempDir dir("msc6");
    msc6::Tools tools;
    tools.msvc152 = dir.path();
    CB_CHECK(!msc6::windows_h(tools).has_value());
    fs::write_file(dir.path() / "INCLUDE" / "WINDOWS.H", to_bytes("#define A 1\r\nint \xE9;\r\n"));
    const auto h = msc6::windows_h(tools);
    CB_CHECK(h.has_value());
    CB_CHECK_EQ(*h, "#define A 1\nint \xC3\xA9;\n");   // Latin-1 read, CRs gone
});

CB_TEST("build.msc6.sdk_prototypes_and_calls", [] {
    const std::string sdk =
        "LRESULT WINAPI DefWindowProc(HWND, UINT, WPARAM, LPARAM);\nvoid    WINAPI MessageBeep(UINT);\n"
        "LPSTR   WINAPI lstrcpy(LPSTR, LPCSTR);\nint WINAPI  wsprintf(LPSTR, LPCSTR, ...);\nvoid WINAPI Nothing(void);\n"
        "BOOL WINAPI GetClientRect(HWND, RECT FAR*);\nint WINAPI Plain();\n  VOID WINAPI Foo(int a, int b);\n#define WINAPI x WINAPI y(a);\n"
        "DWORD WINAPI Bar(void);\n";
    const msc6::Prototypes p = msc6::sdk_prototypes(sdk);
    CB_CHECK_EQ(p.params.size(), 9u);
    CB_CHECK(*p.params.at("DefWindowProc") == (std::vector<std::string>{"HWND", "UINT", "WPARAM", "LPARAM"}));
    CB_CHECK(!p.params.at("wsprintf").has_value());                    // a variable argument list
    CB_CHECK(p.params.at("Nothing")->empty());
    CB_CHECK(p.params.at("Plain")->empty());
    CB_CHECK(*p.params.at("Foo") == (std::vector<std::string>{"int a", "int b"}));
    CB_CHECK(p.no_result == (std::set<std::string>{"Foo", "MessageBeep", "Nothing"}));
    CB_CHECK(msc6::sdk_prototypes("").params.empty());
    const msc6::OrdinalName names = [](std::string_view module, std::uint32_t ordinal) -> std::optional<std::string> {
        if (module == "USER" && ordinal == 107) return "DefWindowProc";
        if (module == "USER" && ordinal == 104) return "MessageBeep";
        if (module == "KERNEL" && ordinal == 88) return "lstrcpy";
        if (module == "USER" && ordinal == 33) return "GetClientRect";
        return std::nullopt;
    };
    const std::string text = "  x = USER_107(a5, a4, a3, a2, a1);\n  y = USER_104(a1);\n  USER_104(a1);\n  KERNEL_88(a2, a1, b);\n";
    // DefWindowProc takes HWND, UINT, WPARAM, LPARAM: five words for a long LPARAM; a function of no value is called only as a statement
    CB_CHECK_EQ(msc6::sdk_calls(text, p, names),
                "  x = DefWindowProc(a5, a4, a3, MAKELONG(a1, a2));\n  y = USER_104(a1);\n  MessageBeep(a1);\n  KERNEL_88(a2, a1, b);\n");
    CB_CHECK_EQ(msc6::sdk_calls("{ USER_33(a, b, c); USER_33(a, (void __far *)&rc);\n  if (z) USER_104(1);\n  q = USER_104(1) + USER_33(1,2,3,4); }\n", p, names),
                "{ GetClientRect(a, MAKELP(b, c)); GetClientRect(a, (void __far *)&rc);\n  if (z) USER_104(1);\n  q = USER_104(1) + USER_33(1,2,3,4); }\n");
});

CB_TEST("build.msc6.fold_temporaries_puts_the_expression_back", [] {
    const std::string text =
        "int2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 a2;\n{\n  int2 iVar1;\n  int2 unaff_DS = codebreaker_ds();\n"
        "  int2 a2_00;\n  int2 n;\n  int2 w;\n  \n  a2_00 = 0x40;\n  iVar1 = KERNEL_90(a1,a2);\n"
        "  iVar1 = KERNEL_5(iVar1 + 1,a2_00);\n  n = g();\n  h(n, n);\n  w = g();\n  while (w != 0) {\n  }\n"
        "  return iVar1;\n}";
    // a2_00 (a constant) and the first iVar1 are folded into the call that uses them; n is used twice and w in a loop's
    // condition: they stay
    CB_CHECK_EQ(msc6::fold_temporaries(text),
                "int2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 a2;\n{\n  int2 iVar1;\n  int2 unaff_DS = codebreaker_ds();\n  int2 n;\n  int2 w;\n  \n"
                "  iVar1 = KERNEL_5(KERNEL_90(a1,a2) + 1,0x40);\n  n = g();\n  h(n, n);\n  w = g();\n  while (w != 0) {\n  }\n  return iVar1;\n}");
    CB_CHECK_EQ(msc6::fold_temporaries("int f()\n{\n  int2 iVar1;\n\n  iVar1 = f(x);\n  return iVar1;\n}"), "int f()\n{\n\n  return f(x);\n}");
    CB_CHECK_EQ(msc6::fold_temporaries(""), "");
    CB_CHECK_EQ(msc6::fold_temporaries("int f()\n  x = 1;\n  return x;"), "int f()\n  x = 1;\n  return x;");   // no body: nothing is a temporary
});

CB_TEST("build.msc6.join_far_parameters_makes_a_pair_one_pointer", [] {
    const std::string text = "int2 __stdcall16far f(a1,a2)\nint2 a1;\nint2 a2;\n{\n  x = KERNEL_90(a1,a2);\n  KERNEL_88(b, a1, a2);\n}";
    CB_CHECK_EQ(msc6::join_far_parameters(text),
                "int2 __stdcall16far f(a1)\nvoid __far * a1;\n{\n  x = KERNEL_90(a1);\n  KERNEL_88(b, a1);\n}");
    // a2 used alone: it is a word of its own
    std::string alone = text;
    alone.replace(alone.rfind('}'), 1, "  y = a2;\n}");
    CB_CHECK_EQ(msc6::join_far_parameters(alone), alone);
    CB_CHECK_EQ(msc6::join_far_parameters("nothing here"), "nothing here");
});

CB_TEST("build.msc6.split_frame_takes_the_frame_structure_apart", [] {
    const std::string text =
        "int2 f()\n{\n  _Packed struct {\n  xunknown1 axStack_200 [250];\n  xunknown1 axStack_106 [260];\n"
        "  } codebreaker_frame;\n  int2 iVar1;\n\n  g((int2)codebreaker_frame.axStack_106);\n"
        "  g((int2)(codebreaker_frame.axStack_200 + -10));\n"
        "  iVar1 = (*(int2 *)((uint1 *)codebreaker_frame.axStack_200 + 0xf8));\n  return iVar1;\n}";
    CB_CHECK_EQ(msc6::split_frame(text),
                "int2 f()\n{\n  xunknown1 local_106[260];\n  int2 local_108;\n  xunknown1 local_20a[258];\n  int2 iVar1;\n\n"
                "  g((int2)((xunknown1 *)&local_106));\n  g((int2)((xunknown1 *)&local_20a));\n  iVar1 = local_108;\n  return iVar1;\n}");
    // a place the reader does not know: the function is left as it was
    std::string odd = text;
    odd.replace(odd.find("axStack_106)"), 12, "axStack_999)");
    CB_CHECK_EQ(msc6::split_frame(odd), odd);
    CB_CHECK_EQ(msc6::split_frame("int f(){return 0;}"), "int f(){return 0;}");
});

CB_TEST("build.msc6.ds_globals_makes_a_constant_address_a_variable", [] {
    const msc6::DsGlobals g = msc6::ds_globals(
        "  *(xunknown1 *)0x34ee = 1;\n  x = *(int2 *)0x63b2 + *(int2 *)0x63b2;\n  y = *(uint2 __far *)p;\n  z = *(void *)0x1234 + *(unsigned int *)0x0f;\n");
    CB_CHECK_EQ(g.declarations, "extern char codebreaker_d34ee[];\nextern char codebreaker_d63b2[];\n");
    CB_CHECK_EQ(g.text,
                "  (*(xunknown1 *)codebreaker_d34ee) = 1;\n  x = (*(int2 *)codebreaker_d63b2) + (*(int2 *)codebreaker_d63b2);\n"
                "  y = *(uint2 __far *)p;\n  z = *(void *)0x1234 + *(unsigned int *)0x0f;\n");   // (void, and an address below 0x10, are not variables)
    CB_CHECK_EQ(msc6::ds_globals("nothing").declarations, "");
});

CB_TEST("build.msc6.wine_typing_and_definition", [] {
    const auto make = [](std::string name, wine16::Convention c, std::vector<wine16::ArgKind> args, bool ret16) {
        wine16::Export e;
        e.name = std::move(name);
        e.convention = c;
        e.args = std::move(args);
        e.ret16 = ret16;
        return e;
    };
    using K = wine16::ArgKind;
    const wine16::Export user33 = make("GetClientRect", wine16::Convention::Pascal, {K::Word, K::Ptr}, true);
    const wine16::Export lstrcpy = make("lstrcpy", wine16::Convention::Pascal, {K::SegPtr, K::Str}, false);
    const wine16::Export user1 = make("Y", wine16::Convention::Pascal, {K::Word}, true);
    const msc6::ExportLookup look = [&](std::string_view module, std::uint32_t ordinal) -> const wine16::Export* {
        if (module == "USER" && ordinal == 33) return &user33;
        if (module == "KERNEL" && ordinal == 88) return &lstrcpy;
        if (module == "USER" && ordinal == 1) return &user1;
        return nullptr;
    };
    const std::string header = "extern uint4 __far __stdcall USER_33();\nextern uint4 __far __stdcall KERNEL_88();\nextern uint4 __far __stdcall USER_1();\n";
    OrderedMap<std::string, std::string> texts;
    texts.insert_or_assign("f", "  USER_33(param_1, (void __far *)&rc);\n  KERNEL_88(a2, a1, b2, b1);\n  USER_1(1, 2);\n");
    const msc6::WineTyped typed = msc6::wine_typing(header, texts, look);
    // USER_1 is called with two arguments for a one-word parameter: it keeps its declaration without parameters
    CB_CHECK_EQ(typed.header,
                "extern uint2 __far __stdcall USER_33(uint2, void __far *);\nextern uint4 __far __stdcall KERNEL_88(void __far *, void __far *);\n"
                "extern uint4 __far __stdcall USER_1();\n");
    CB_CHECK_EQ(typed.texts.at("f"),
                "  USER_33(param_1, (void __far *)&rc);\n  KERNEL_88((void __far *)(((uint4)(uint2)(a2) << 16) | (uint2)(a1)), "
                "(void __far *)(((uint4)(uint2)(b2) << 16) | (uint2)(b1)));\n  USER_1(1, 2);\n");
    // a typed call inside the arguments of another is said too
    OrderedMap<std::string, std::string> nested;
    nested.insert_or_assign("g", "  USER_1(USER_1(a));\n");
    CB_CHECK_EQ(msc6::wine_typing(header, nested, look).texts.at("g"), "  USER_1(USER_1(a));\n");
    OrderedMap<std::string, std::string> nested_long;
    nested_long.insert_or_assign("g", "  KERNEL_88(USER_33(a, b), c, d, e);\n");
    CB_CHECK_EQ(msc6::wine_typing(header, nested_long, look).texts.at("g"),
                "  KERNEL_88((void __far *)(((uint4)(uint2)(USER_33(a, b)) << 16) | (uint2)(c)), (void __far *)(((uint4)(uint2)(d) << 16) | (uint2)(e)));\n");
    CB_CHECK(msc6::wine_typing(header, {}, look).texts.empty());

    const wine16::Export ip = make("IpFindFirstLine", wine16::Convention::Pascal, {K::Word, K::Str, K::Str, K::Ptr}, false);
    const std::string def =
        "int2 __stdcall16far IPFINDFIRSTLINE(a1,a2,a3,a4,a5,a6,a7)\nint2 a1;\nint2 a2;\nint2 a3;\nint2 a4;\n"
        "int2 a5;\nint2 a6;\nint2 a7;\n{\n  x = g(a1,a2);\n  y = a3 + 1;\n  h(a4, a5,a6);\n  return a7;\n}";
    CB_CHECK_EQ(msc6::wine_definition(def, &ip),
                "int2 __stdcall16far IPFINDFIRSTLINE(a1,a3,a5,a7)\nvoid __far * a1;\nvoid __far * a3;\nvoid __far * a5;\nint2 a7;\n"
                "{\n  x = g(a1);\n  y = FP_OFF(a3) + 1;\n  h(FP_SEG(a3), a5);\n  return a7;\n}");
    CB_CHECK_EQ(msc6::wine_definition(def, nullptr), def);
    wine16::Export no_args;
    CB_CHECK_EQ(msc6::wine_definition(def, &no_args), def);
    const wine16::Export one_word = make("X", wine16::Convention::Pascal, {K::Word}, false);
    CB_CHECK_EQ(msc6::wine_definition(def, &one_word), def);   // the words do not add up
    CB_CHECK_EQ(msc6::wine_definition("nothing", &ip), "nothing");
});

CB_TEST("build.msc6.batch_files_and_refusals", [] {
    const std::map<std::size_t, std::string> todo{{0, "int f()\n{\n}"}, {1, "int g()\n{\n}"}, {7, "t"}};
    const std::vector<std::vector<std::size_t>> groups{{0, 1}, {7}};
    const msc6::BatchFiles b = msc6::batch_files(groups, todo, "B");
    CB_CHECK_EQ(b.files.size(), 2u);
    CB_CHECK_EQ(b.files[0].stem, "B0");
    CB_CHECK_EQ(b.files[0].text, "int f()\n{\n}\n\nint g()\n{\n}");
    CB_CHECK_EQ(b.files[1].stem, "B1");
    // each function's first line: after the #include and its blank line (3), then its lines and a blank
    CB_CHECK((b.starts[0] == std::vector<std::pair<std::size_t, std::size_t>>{{3, 0}, {7, 1}}));
    CB_CHECK((b.starts[1] == std::vector<std::pair<std::size_t, std::size_t>>{{3, 7}}));

    const std::vector<std::pair<std::size_t, std::size_t>> starts{{3, 0}, {7, 1}};
    CB_CHECK((msc6::refused("B0.C\nB0.C(8) : error C2065: 'x' : undeclared identifier\n", starts) == std::vector<std::size_t>{1}));
    CB_CHECK(msc6::refused("B1.C\nm.h(211) : error C2059: syntax error : '('\n", starts).empty());   // the header's: none can be told
    CB_CHECK((msc6::refused("B2.C\r\nC:\\w\\R2.c(3) : fatal error C1001: x\r\nB2.C(9) : error C2: y\r\nB2.C(9) : error C3: y\r\n",
                            std::vector<std::pair<std::size_t, std::size_t>>{{3, 5}, {7, 6}}) == std::vector<std::size_t>{5, 6}));
    CB_CHECK(msc6::refused("", starts).empty());
});

// ---- compiling -------------------------------------------------------------------------------------------------------

namespace {

// A runner for what DOSBox-X would do with the compiler: it reads the C files, and "compiles" them. A function whose text
// holds "#error" is refused (the file is refused with the line of the error, as CL says it).
class FakeCompiler : public cb::build::DosRunner {
public:
    cb::build::DosRunResult run(const cb::build::DosRunSpec& spec) const override {
        ++sessions;
        files_per_session.push_back(spec.commands.size());
        const std::filesystem::path work = *spec.work;
        for (const std::string& command : spec.commands) {
            // cl /nologo /c /W0 <flags> /Fo<stem>.OBJ <stem>.C > <stem>.TXT
            const std::size_t fo = command.find("/Fo");
            const std::string stem = command.substr(fo + 3, command.find('.', fo) - fo - 3);
            flags_seen.push_back(command.substr(0, fo));
            const std::string c = lf(fs::read_text(work / (stem + ".C")));
            const auto lines = text::split(c, '\n');
            std::string said;
            std::vector<std::pair<std::string, std::string>> functions;   // (name, definition)
            for (std::size_t i = 0; i < lines.size(); ++i) {
                if (lines[i].starts_with("#error")) said += std::format("{}.C({}) : error C2000: refused\n", stem, i + 1);
                if (lines[i].starts_with("int2 ")) {
                    const std::string_view rest = lines[i].substr(5);
                    functions.emplace_back(std::string(rest.substr(0, rest.find('('))), std::string(lines[i]));
                }
            }
            if (!said.empty()) {
                fs::write_file(work / (stem + ".TXT"), to_bytes(said));
                continue;   // (no object)
            }
            // one code segment: a function is 0x55, 0x8B, 0xEC, its name's first letter, 0x5D, 0xCB (a far return);
            // the file's own first function is placed after a helper, as the stubs of a header come first
            Bytes code{0x90, 0x90};
            OrderedMap<std::string, std::int64_t> publics;
            for (const auto& [name, def] : functions) {
                publics.insert_or_assign("_" + name, static_cast<std::int64_t>(code.size()));
                append(code, Bytes{0x55, 0x8B, 0xEC, static_cast<std::uint8_t>(name[0]), static_cast<std::uint8_t>(def.size()), 0x5D, 0xCB});
            }
            fs::write_file(work / (stem + ".OBJ"), formats::omf::raw_object("f.c", code, publics));
            fs::write_file(work / (stem + ".TXT"), to_bytes("compiled\n"));
        }
        if (!hang) fs::write_file(work / "done.txt", to_bytes("_DONE_"));
        return {.ok = true, .work = work};
    }
    mutable int sessions = 0;
    mutable std::vector<std::size_t> files_per_session;
    mutable std::vector<std::string> flags_seen;
    bool hang = false;
};

msc6::Tools make_tools(const std::filesystem::path& dir) {
    fs::write_file(dir / "dosbox-x.exe", Bytes{1});
    fs::write_file(dir / "msc" / "BIN" / "CL.EXE", Bytes{1, 2, 3});
    msc6::Tools tools;
    tools.dosbox_x = dir / "dosbox-x.exe";
    tools.msc6 = dir / "msc";
    return tools;
}

std::unique_ptr<base::cache::Cache> make_cache(const std::filesystem::path& dir) {
    base::cache::Config config;
    config.state_dir = dir / "cache-state";
    config.shared_dir = dir / "cache-shared";
    return std::make_unique<base::cache::Cache>(config);
}

struct Rig {
    fs::ScopedTempDir dir{"msc6"};
    msc6::Tools tools = make_tools(dir.path());
    FakeCompiler runner;
    std::unique_ptr<base::cache::Cache> cache;
    msc6::Context ctx;
    msc6::CompileOptions options;
    explicit Rig(bool with_cache = false) : cache(with_cache ? make_cache(dir.path()) : nullptr), ctx{tools, runner, cache.get()} {
        options.work = dir.path() / "work";
        options.timeout = std::chrono::seconds(30);
    }
};

msc6::Source named(std::string name, int bulk = 0) {
    return {"int2 " + name + "(int2 a)\n{\n  return " + std::string(static_cast<std::size_t>(bulk), 'x') + "a;\n}", name};
}

std::string code_hex(const std::optional<msc6::Code>& c) { return c ? hex(c->code) : "none"; }

}  // namespace

CB_TEST("build.msc6.compile_many_one_source_a_file", [] {
    Rig rig;
    std::vector<msc6::Source> sources{{.text = "int2 f1(int2 a)\n{\n}"}, {.text = "int2 f2(int2 a)\n{\n}"}, {.text = "#error\nint2 bad(int2 a)\n{\n}"}};
    const auto built = msc6::compile_many(rig.ctx, sources, "typedef short int2;\n", "/AM /Gcsw /Os", rig.options);
    CB_CHECK_EQ(built.size(), 3u);
    CB_CHECK_EQ(rig.runner.sessions, 1);                 // all of them in one emulator session
    CB_CHECK_EQ(rig.runner.files_per_session[0], 3u);
    CB_CHECK_EQ(code_hex(built[0]), "909055" "8bec" "66" "0f" "5dcb");   // the first code segment: the helper filler and the function
    CB_CHECK(!built[2].has_value());                      // refused
    CB_CHECK_EQ(rig.runner.flags_seen[0], "cl /nologo /c /W0 /AM /Gcsw /Os ");
    // what the file had: the header is M.H, the source is wrapped with its #include
    CB_CHECK_EQ(lf(fs::read_text(rig.options.work / "M.H")), "typedef short int2;\n");
    CB_CHECK(lf(fs::read_text(rig.options.work / "F0.C")).starts_with("#include \"m.h\"\n\nint2 f1"));
});

CB_TEST("build.msc6.compile_many_named_functions_share_files_and_a_refused_one_is_found", [] {
    Rig rig;
    std::vector<msc6::Source> sources;
    for (const char* n : {"f1", "f2", "f3"}) sources.push_back(named(n, 1));
    sources.push_back({"#error\nint2 bad(int2 a)\n{\n}", "bad"});
    sources.push_back(named("g", 2));
    const auto built = msc6::compile_many(rig.ctx, sources, "", "/AM /Gcsw /Os", rig.options);
    CB_CHECK_EQ(built.size(), 5u);
    // round 1: all five in one file, refused with its errors in `bad`; round 2: bad on its own and the other four together
    CB_CHECK_EQ(rig.runner.sessions, 2);
    CB_CHECK_EQ(rig.runner.files_per_session[0], 1u);
    CB_CHECK_EQ(rig.runner.files_per_session[1], 2u);
    for (std::size_t i : {0u, 1u, 2u, 4u}) CB_CHECK(built[i].has_value());
    CB_CHECK(!built[3].has_value());
    CB_CHECK_EQ(built[0]->code.size(), 7u);   // each function is cut out between its public name and the next
    CB_CHECK_EQ(built[0]->code[3], 'f');
    CB_CHECK_EQ(built[4]->code[3], 'g');
});

CB_TEST("build.msc6.compile_many_together_batch_and_sessions_cut_off", [] {
    {
        Rig rig;
        std::vector<msc6::Source> sources{named("a"), named("b"), named("c"), named("d"), named("e")};
        rig.options.batch = 2;
        msc6::compile_many(rig.ctx, sources, "", "/AM", rig.options);
        CB_CHECK_EQ(rig.runner.sessions, 1);
        CB_CHECK_EQ(rig.runner.files_per_session[0], 3u);   // 2 + 2 + 1
    }
    {
        Rig rig;
        std::vector<msc6::Source> sources{named("a"), named("b"), named("c")};
        rig.options.together = {{0, 2, 99}, {1}};            // an index that is not a source is dropped
        const auto built = msc6::compile_many(rig.ctx, sources, "", "/AM", rig.options);
        CB_CHECK_EQ(rig.runner.files_per_session[0], 2u);
        CB_CHECK(built[0] && built[1] && built[2]);
        // functions of one file see each other: the code is the same whichever file it is in, here
        CB_CHECK_EQ(built[0]->code[3], 'a');
        CB_CHECK_EQ(built[2]->code[3], 'c');
    }
    {
        Rig rig;
        rig.runner.hang = true;   // the session never says it is done: no verdict for what it did not make
        std::vector<msc6::Source> sources{{.text = "int2 a(int2 a)\n{\n}"}, {.text = "#error\n"}};
        const auto built = msc6::compile_many(rig.ctx, sources, "", "/AM", rig.options);
        CB_CHECK(built[0].has_value());   // (an object file that exists is read)
        CB_CHECK(!built[1].has_value());
    }
    {
        Rig rig;
        CB_CHECK(msc6::compile_many(rig.ctx, {}, "", "/AM", rig.options).empty());
        CB_CHECK_EQ(rig.runner.sessions, 0);
    }
});

CB_TEST("build.msc6.compile_many_keeps_what_it_compiled", [] {
    Rig rig(true);
    std::vector<msc6::Source> sources{named("f1"), named("f2"), {"#error\nint2 bad(int2 a)\n{\n}", "bad"}};
    const auto first = msc6::compile_many(rig.ctx, sources, "H", "/AM", rig.options);
    const int sessions = rig.runner.sessions;
    CB_CHECK(sessions > 0);
    const auto second = msc6::compile_many(rig.ctx, sources, "H", "/AM", rig.options);
    CB_CHECK_EQ(rig.runner.sessions, sessions);            // all of it from the cache: the refusals too
    CB_CHECK_EQ(code_hex(second[0]), code_hex(first[0]));
    CB_CHECK_EQ(code_hex(second[1]), code_hex(first[1]));
    CB_CHECK(!second[2].has_value());
    // another header, another answer: the cache is by what the compile depended on
    msc6::compile_many(rig.ctx, sources, "H2", "/AM", rig.options);
    CB_CHECK(rig.runner.sessions > sessions);
    // one function of a file not known is compiled again, the whole file
    const int before = rig.runner.sessions;
    sources.push_back(named("f3"));
    sources.erase(sources.begin() + 2);
    const auto third = msc6::compile_many(rig.ctx, sources, "H", "/AM", rig.options);
    CB_CHECK(rig.runner.sessions > before);
    CB_CHECK(third[0] && third[1] && third[2]);
});

CB_TEST("build.msc6.compile_many_needs_the_tools", [] {
    Rig rig;
    rig.ctx.tools.msc6 = rig.dir.path() / "nowhere";
    CB_CHECK_THROWS_AS(NotFoundError, msc6::compile_many(rig.ctx, std::vector<msc6::Source>{named("a")}, "", "/AM", rig.options));
    CB_CHECK(!msc6::available(rig.ctx.tools));
    CB_CHECK(msc6::available(rig.tools));
    msc6::Tools msvc = rig.tools;
    CB_CHECK(!msc6::available(msvc, msc6::Compiler::Msvc152));
    CB_CHECK(!msc6::native_paths(rig.tools, msc6::Compiler::Msc6).has_value());   // a DOS program: always the emulator
    msvc.native16 = false;
    CB_CHECK(!msc6::native_paths(msvc, msc6::Compiler::Msvc152).has_value());
    CB_CHECK_EQ(msc6::flag_sets().size(), 9u);
    CB_CHECK_EQ(msc6::flag_sets()[0], "/AM /Gcsw /Os");
});

CB_TEST("build.msc6.native_paths_use_a_short_folder_when_there_is_one", [] {
    fs::ScopedTempDir dir("msc6");
    msc6::Tools tools;
    tools.msvc152 = dir.path() / "msvc";
    fs::write_file(tools.msvc152 / "BIN" / "CL.EXE", Bytes{1});
    tools.native16_tools = dir.path() / "short";   // (not made: no junction here)
    tools.native16_work = dir.path() / "w";
    CB_CHECK(!msc6::native_paths(tools, msc6::Compiler::Msvc152, false).has_value());
    fs::write_file(*tools.native16_tools / "BIN" / "CL.EXE", Bytes{1});   // a short folder that already holds the compiler
    const auto native = msc6::native_paths(tools, msc6::Compiler::Msvc152, false);
    CB_CHECK(native.has_value());
    CB_CHECK(fs::same_path(native->tools, *tools.native16_tools));
    CB_CHECK(std::filesystem::is_directory(native->work));
});
