// Tests of cb/symbols/demangle: names from the MFC and COMCTL32 ordinal tables and the system headers' own exports,
// each expected text checked by hand against what Microsoft's undname prints.
//
// DIVERGES from the Python module (and the engine it called, native/symbols/demangle.cc), by design:
//   * Nesting deeper than 100 levels is not read (it was 400; real names nest a dozen deep).
//   * Text is read as bytes, not code points: Latin-1 superscript digits (U+00B2, B3, B9), which Python's isdigit()
//     accepted and int() refused, no longer make a name unreadable; a byte above 0x7F is an ordinary name byte.
//   * plain(): `@N` with N beyond 32 bits is not a stack size (Python had a big integer): the name is returned as it
//     stands. No real decorated name has one.
// The old engine and this reader were also run over 4,600 decorated names (every name of the shipped ordinal
// tables, the harness names and the corpus's own) and agree on every parse, type name and plain name.
#include "cb/symbols/demangle.hpp"

#include "cb/core/error.hpp"
#include "cb/core/testing.hpp"

namespace dm = cb::symbols::demangle;

CB_TEST("symbols.demangle.constructors_and_destructors", [] {
    CB_CHECK_EQ(*dm::declaration("??0CString@@QAE@PBD@Z"), "public: __thiscall CString::CString(char const *)");
    CB_CHECK_EQ(*dm::declaration("??1CString@@UAE@XZ"), "public: virtual __thiscall CString::~CString(void)");
    CB_CHECK_EQ(*dm::declaration("??_ECObject@@UAEPAXI@Z"),
                "public: virtual void * __thiscall CObject::`vector deleting destructor'(unsigned int)");
    CB_CHECK_EQ(*dm::short_name("??1CString@@UAE@XZ"), "CString::~CString");
});

CB_TEST("symbols.demangle.members_and_results", [] {
    CB_CHECK_EQ(*dm::declaration("?AddRef@CFoo@@UAGKXZ"), "public: virtual unsigned long __stdcall CFoo::AddRef(void)");
    CB_CHECK_EQ(*dm::declaration("?Create@CButton@@QAEHPBDKABUtagRECT@@PAVCWnd@@I@Z"),
                "public: int __thiscall CButton::Create(char const *,unsigned long,struct tagRECT const &,class CWnd *,"
                "unsigned int)");
    CB_CHECK_EQ(*dm::declaration("?GetAt@CStringArray@@QBE?AVCString@@H@Z"),
                "public: class CString __thiscall CStringArray::GetAt(int)const");
    CB_CHECK_EQ(*dm::declaration("??HCString@@QBE?AV0@ABV0@@Z"),
                "public: class CString __thiscall CString::operator+(class CString const &)const");
    CB_CHECK_EQ(*dm::declaration("??BCString@@QBEPBDXZ"), "public: __thiscall CString::operator char const *(void)const");
    CB_CHECK_EQ(*dm::declaration("??4CRect@@QAEXABV0@@Z"),
                "public: void __thiscall CRect::operator=(class CRect const &)");
});

CB_TEST("symbols.demangle.parse_fields", [] {
    const auto p = dm::parse("?AddRef@CFoo@@UAGKXZ");
    CB_CHECK(p.has_value());
    CB_CHECK(p->kind == dm::Kind::Function);
    CB_CHECK_EQ(p->name, "CFoo::AddRef");
    CB_CHECK_EQ(p->base, "AddRef");
    CB_CHECK_EQ(p->owner, "CFoo");
    CB_CHECK_EQ(p->access, "public");
    CB_CHECK(p->is_virtual);
    CB_CHECK(!p->is_static);
    CB_CHECK(!p->far);
    CB_CHECK_EQ(p->convention, "__stdcall");
    CB_CHECK_EQ(p->returns, "unsigned long");
    CB_CHECK(p->parameters.empty());   // (void) is no parameters

    const auto q = dm::parse("??0CString@@QAE@PBD@Z");
    CB_CHECK(q.has_value());
    CB_CHECK_EQ(q->parameters.size(), std::size_t{1});
    CB_CHECK_EQ(q->parameters[0], "char const *");
    CB_CHECK_EQ(q->returns, "");   // a constructor has no result

    const auto m = dm::parse("??HCString@@QBE?AV0@ABV0@@Z");
    CB_CHECK(m.has_value());
    CB_CHECK_EQ(m->const_qualifier, "const");
});

CB_TEST("symbols.demangle.free_functions_and_templates", [] {
    CB_CHECK_EQ(*dm::declaration("?f@@YAXXZ"), "void __cdecl f(void)");
    CB_CHECK_EQ(*dm::declaration("?foo@@YAHHHZ"), "int __cdecl foo(int,int,...)");
    CB_CHECK_EQ(*dm::declaration("?Sum@@YGNNN@Z"), "double __stdcall Sum(double,double)");
    CB_CHECK_EQ(*dm::declaration("??$max@H@@YAHHH@Z"), "int __cdecl max<int>(int,int)");
    CB_CHECK_EQ(*dm::declaration("?Get@?$CArray@HH@@QAEHXZ"), "public: int __thiscall CArray<int,int>::Get(void)");
});

CB_TEST("symbols.demangle.function_pointers", [] {
    CB_CHECK_EQ(*dm::declaration("?x@@YAXP6AHH@Z@Z"), "void __cdecl x(int (__cdecl*)(int))");
    CB_CHECK_EQ(*dm::declaration("?pf@@3P6AXH@ZA"), "void (__cdecl* pf)(int)");
});

CB_TEST("symbols.demangle.data_and_special_names", [] {
    CB_CHECK_EQ(*dm::declaration("?m_nCount@CList@@2HA"), "public: static int CList::m_nCount");
    CB_CHECK_EQ(*dm::declaration("??_7CObject@@6B@"), "const CObject::`vftable'");
    CB_CHECK_EQ(*dm::declaration("??_C@_05ABC@hello?$AA@"), "`string'");
    const auto s = dm::parse("?m_nCount@CList@@2HA");
    CB_CHECK(s.has_value());
    CB_CHECK(s->kind == dm::Kind::Data);
    CB_CHECK(s->is_static);
    CB_CHECK_EQ(s->returns, "int");
});

CB_TEST("symbols.demangle.type_names", [] {
    CB_CHECK_EQ(*dm::type_name(".?AVCObject@@"), "CObject");
    CB_CHECK_EQ(*dm::type_name(".?AUtagRECT@@"), "tagRECT");
    CB_CHECK_EQ(*dm::type_name(".?AVInner@Outer@@"), "Outer::Inner");
    CB_CHECK(!dm::type_name(".?AX").has_value());
    CB_CHECK(!dm::type_name("CObject").has_value());
    CB_CHECK(!dm::type_name("").has_value());
});

CB_TEST("symbols.demangle.not_a_name", [] {
    CB_CHECK(!dm::parse("").has_value());
    CB_CHECK(!dm::parse("plain").has_value());
    CB_CHECK(!dm::parse("?").has_value());
    CB_CHECK(!dm::parse("??").has_value());
    CB_CHECK(!dm::parse("?bad").has_value());
    CB_CHECK(!dm::declaration("not a name").has_value());
    CB_CHECK(!dm::short_name("?bad").has_value());
    // 16-bit and other forms this reader does not understand give nothing, never a guess
    CB_CHECK(!dm::parse("?F@@YFHPEBD@Z").has_value());
});

CB_TEST("symbols.demangle.truncated_at_every_length", [] {
    // a name cut short anywhere must answer or decline, never read past its end
    const std::string name = "?Create@CButton@@QAEHPBDKABUtagRECT@@PAVCWnd@@I@Z";
    std::size_t answered = 0;
    for (std::size_t n = 0; n <= name.size(); ++n) {
        if (dm::parse(std::string_view(name).substr(0, n))) ++answered;
    }
    CB_CHECK(answered >= 1);
    CB_CHECK(dm::parse(name).has_value());
});

CB_TEST("symbols.demangle.runaway_nesting_is_declined", [] {
    // thousands of nested pointers: the depth limit declines instead of exhausting the stack
    std::string name = "?f@@YAX";
    for (int i = 0; i < 5000; ++i) name += "PA";
    name += "H@Z";
    CB_CHECK(!dm::parse(name).has_value());
});

CB_TEST("symbols.demangle.plain_names", [] {
    auto p = dm::plain("_LZRead@12");
    CB_CHECK_EQ(p.name, "LZRead");
    CB_CHECK_EQ(p.convention, "__stdcall");
    CB_CHECK(p.argument_bytes == std::optional<std::uint32_t>(12));
    p = dm::plain("LZRead@12");
    CB_CHECK_EQ(p.name, "LZRead");
    CB_CHECK_EQ(p.convention, "__stdcall");
    p = dm::plain("@fast@8");
    CB_CHECK_EQ(p.name, "fast");
    CB_CHECK_EQ(p.convention, "__fastcall");
    CB_CHECK(p.argument_bytes == std::optional<std::uint32_t>(8));
    p = dm::plain("_name");
    CB_CHECK_EQ(p.name, "name");
    CB_CHECK_EQ(p.convention, "__cdecl");
    CB_CHECK(!p.argument_bytes.has_value());
    p = dm::plain("plain");
    CB_CHECK_EQ(p.name, "plain");
    CB_CHECK_EQ(p.convention, "");
    p = dm::plain("__x@4");   // the first underscore is the convention's
    CB_CHECK_EQ(p.name, "_x");
    p = dm::plain("_@8");     // nothing left after it: the name is `_`
    CB_CHECK_EQ(p.name, "_");
    CB_CHECK_EQ(p.convention, "__stdcall");
    p = dm::plain("_1abc");   // not an identifier after the underscore
    CB_CHECK_EQ(p.name, "_1abc");
    CB_CHECK_EQ(p.convention, "");
    p = dm::plain("Foo@12abc");
    CB_CHECK_EQ(p.name, "Foo@12abc");
    p = dm::plain("_foo@+8");
    CB_CHECK_EQ(p.name, "_foo@+8");
    p = dm::plain("");
    CB_CHECK_EQ(p.name, "");
    p = dm::plain("_a@99999999999");   // DIVERGES: beyond 32 bits is not a stack size
    CB_CHECK_EQ(p.name, "_a@99999999999");
});

CB_TEST("symbols.demangle.readable", [] {
    CB_CHECK_EQ(dm::readable("?f@@YAXXZ"), "void __cdecl f(void)");
    CB_CHECK_EQ(dm::readable("_LZRead@12"), "LZRead");
    CB_CHECK_EQ(dm::readable("?bad"), "?bad");   // a ? name that cannot be read stays as it is
    CB_CHECK_EQ(dm::readable("name"), "name");
});

CB_TEST("symbols.demangle.stack_bytes", [] {
    using Bytes = std::optional<std::uint32_t>;
    CB_CHECK(dm::stack_bytes("_LZRead@12") == Bytes(12));
    CB_CHECK(dm::stack_bytes("?AddRef@CFoo@@UAGKXZ") == Bytes(4));                       // `this`
    CB_CHECK(dm::stack_bytes("?Create@CButton@@QAEHPBDKABUtagRECT@@PAVCWnd@@I@Z") == Bytes(20));   // thiscall: this is in ECX
    CB_CHECK(dm::stack_bytes("?Sum@@YGNNN@Z") == Bytes(16));                               // two doubles
    CB_CHECK(dm::stack_bytes("?GetAt@CStringArray@@QBE?AVCString@@H@Z") == Bytes(8));      // hidden result pointer + int
    CB_CHECK(!dm::stack_bytes("?f@@YAXXZ").has_value());                                    // cdecl: the caller cleans
    CB_CHECK(!dm::stack_bytes("?foo@@YAHHHZ").has_value());
    CB_CHECK(!dm::stack_bytes("plain").has_value());
    CB_CHECK(dm::stack_bytes("??1CString@@UAE@XZ") == Bytes(0));
});

CB_TEST("symbols.demangle.command", [] {
    cb::testing::CapturedStd cap;
    const std::vector<std::string> args = {"?f@@YAXXZ", "_LZRead@12", "?bad"};
    CB_CHECK_EQ(dm::run(args), 0);
    CB_CHECK_EQ(cap.out(), "void __cdecl f(void)\nLZRead\n?bad  (not readable)\n");
    CB_CHECK_EQ(dm::run({}), cb::kExitUsage);
});
