// Tests of cb/port/asm16: reading assembler source (lines, operands, segments, the old assembler's listing), the C program's
// types, the kinds of the registers (join, merge), and whole modules ported -- the text emitted, compared with what the Python
// original made of the same module (testdata/asm16/golden, made by testdata/asm16/gen_golden.py and read before it was kept).
// cb-unity: alone
#include <algorithm>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/port/asm16.hpp"

namespace {

using namespace cb;
using namespace cb::port::asm16;
using J = nlohmann::json;

std::filesystem::path test_data(std::string_view name) { return testing::data_path("port/testdata/asm16/" + std::string(name)); }

std::string kinds_of(const Operand& o) {
    switch (o.kind) {
        case Operand::Kind::Reg: return "reg " + o.text;
        case Operand::Kind::Imm: return "imm " + o.text;
        case Operand::Kind::Off: return "off " + o.text + " " + std::to_string(o.disp);
        case Operand::Kind::Sym: return "sym " + o.text;
        case Operand::Kind::Mem: {
            std::string s = "mem";
            s += " size=" + (o.mem.size ? std::to_string(*o.mem.size) : std::string("-"));
            s += " seg=" + o.mem.seg.value_or("-");
            s += " sym=" + o.mem.sym.value_or("-");
            s += " regs=" + text::join(o.mem.regs, "+");
            s += " disp=" + std::to_string(o.mem.disp);
            return s;
        }
    }
    return {};
}

std::string op_text(std::string_view text_in, const Equates* equ = nullptr) { return kinds_of(operand(text_in, equ)); }

const std::string kHeader =
    "\nstruct a { int x; char *p; long l; struct { int u; char *q; } in; union { int i; char *pp; } un; char buf[6]; };\n"
    "struct b { struct a *first; far char *fp; int n; };\n"
    "struct c { char z[3]; int w; };\n"
    "union d { int i; long l; };\n"
    "struct e { struct { char *a; char *b; } pp; };\n"
    "extern struct a *g_a;\n"
    "char *g_s;\n"
    "int g_i[10];\n"
    "long g_l;\n"
    "struct b g_b;\n"
    "struct b *g_bp[4];\n"
    "static unsigned g_u = 5;\n"
    "extern struct c g_c;\n"
    "int (*g_fn)();\n"
    "double g_d;\n"
    "typedef int T;\n";

}  // namespace

// ---- the source ------------------------------------------------------------------------------------------------------

CB_TEST("port.asm16.strip_comment_and_split_ops", [] {
    CB_CHECK_EQ(strip_comment("mov ax, 'a;b' ; comment"), "mov ax, 'a;b' ");
    CB_CHECK_EQ(strip_comment("x db \"q;\" ; c"), "x db \"q;\" ");
    CB_CHECK_EQ(strip_comment(";only comment"), "");
    CB_CHECK_EQ(strip_comment("'"), "'");
    CB_CHECK(split_ops("").empty());
    CB_CHECK(split_ops("  ").empty());
    CB_CHECK((split_ops("a, [bx+2], c") == std::vector<std::string>{"a", "[bx+2]", "c"}));
    CB_CHECK((split_ops("db 'a,b', 1") == std::vector<std::string>{"db 'a,b'", "1"}));
    CB_CHECK((split_ops("f(1,2),g") == std::vector<std::string>{"f(1,2)", "g"}));
    CB_CHECK((split_ops("\"unterminated, x") == std::vector<std::string>{"\"unterminated, x"}));
    CB_CHECK((split_ops("a,,b") == std::vector<std::string>{"a", "", "b"}));
});

CB_TEST("port.asm16.number", [] {
    const std::pair<const char*, std::optional<std::int64_t>> cases[] = {
        {"", std::nullopt}, {"0", 0},   {"10", 10},    {"0ah", 10},        {"0Ah", 10},      {"0xff", 255},  {"0XFF", 255},
        {"12", 12},         {"-3", std::nullopt},      {"1_0", std::nullopt}, {"h", std::nullopt}, {"0h", 0},   {"ffh", std::nullopt},
        {"fh", std::nullopt}, {"09h", 9}, {" 7 ", 7},  {"0x", std::nullopt}, {"1x2", std::nullopt}, {"0ffffh", 65535}};
    for (const auto& [text_in, want] : cases) CB_CHECK(number(text_in) == want);
    CB_CHECK(!number("99999999999999999999"));   // (DIVERGES: the Python's integers are as big as the text; a count too big for 64 bits is no number here)
});

CB_TEST("port.asm16.operands", [] {
    CB_CHECK_EQ(op_text("AX"), "reg ax");
    CB_CHECK_EQ(op_text("AL"), "reg al");
    CB_CHECK_EQ(op_text("ES"), "reg es");
    CB_CHECK_EQ(op_text("offset foo"), "off foo 0");
    CB_CHECK_EQ(op_text("OFFSET DGROUP:foo+4"), "off foo 4");
    CB_CHECK_EQ(op_text("offset x-2"), "off x -2");
    CB_CHECK_EQ(op_text("byte ptr [bx]"), "mem size=1 seg=- sym=- regs=bx disp=0");
    CB_CHECK_EQ(op_text("WORD PTR [bp-4]"), "mem size=2 seg=- sym=- regs=bp disp=-4");
    CB_CHECK_EQ(op_text("dword ptr es:[di+2]"), "mem size=4 seg=es sym=- regs=di disp=2");
    CB_CHECK_EQ(op_text("es:[bx]"), "mem size=- seg=es sym=- regs=bx disp=0");
    CB_CHECK_EQ(op_text("[bx+si]"), "mem size=- seg=- sym=- regs=bx+si disp=0");
    CB_CHECK_EQ(op_text("[bx]+4"), "mem size=- seg=- sym=- regs=bx disp=4");
    CB_CHECK_EQ(op_text("foo[bx]"), "mem size=- seg=- sym=foo regs=bx disp=0");
    CB_CHECK_EQ(op_text("[4+bx]"), "mem size=- seg=- sym=- regs=bx disp=4");
    CB_CHECK_EQ(op_text("[bx - 2 + si]"), "mem size=- seg=- sym=- regs=bx+si disp=-2");
    CB_CHECK_EQ(op_text("word ptr foo"), "mem size=2 seg=- sym=foo regs= disp=0");
    CB_CHECK_EQ(op_text("tbyte ptr [x]"), "mem size=10 seg=- sym=x regs= disp=0");
    CB_CHECK_EQ(op_text("word ptr cs:[1234h]"), "mem size=2 seg=cs sym=- regs= disp=4660");
    CB_CHECK_EQ(op_text("0ffh"), "imm 0ffh");
    CB_CHECK_EQ(op_text("'a'"), "imm 'a'");
    CB_CHECK_EQ(op_text("-x"), "imm -x");
    CB_CHECK_EQ(op_text("-12h"), "imm -12h");
    CB_CHECK_EQ(op_text("foo"), "sym foo");
    CB_CHECK_EQ(op_text("foo+2"), "sym foo+2");
    CB_CHECK_EQ(op_text("seg foo"), "sym seg foo");
    CB_CHECK_EQ(op_text("call far ptr foo"), "sym call far ptr foo");
    CB_CHECK_THROWS_AS(Asm16Error, operand("[bx+foo+bar]"));
    // equates: a name that is a number counts in a sum, a bare name is an immediate; a variable named bare is its memory
    Equates eq;
    eq.values = {{"cnt", "5"}, {"msk", "0ffh"}, {"bad", "zz"}};
    eq.sizes = {{"v1", 2}, {"v2", 1}, {"q", 4}};
    CB_CHECK_EQ(op_text("[bx+cnt]", &eq), "mem size=- seg=- sym=- regs=bx disp=5");
    CB_CHECK_EQ(op_text("[bx-msk]", &eq), "mem size=- seg=- sym=- regs=bx disp=-255");
    CB_CHECK_EQ(op_text("[bx+bad]", &eq), "mem size=- seg=- sym=bad regs=bx disp=0");
    CB_CHECK_EQ(op_text("cnt", &eq), "imm cnt");
    CB_CHECK_EQ(op_text("v1", &eq), "mem size=2 seg=- sym=v1 regs= disp=0");
    CB_CHECK_EQ(op_text("q", &eq), "mem size=4 seg=- sym=q regs= disp=0");
    CB_CHECK_EQ(op_text("bad", &eq), "imm bad");   // (an equate, though its value is no number)
    CB_CHECK_EQ(op_text("offset foo+cnt", &eq), "off foo 5");
    // DIVERGES: `offset x+zz` (a displacement that is no number) left the displacement out of the Python's operand (None, and
    // the text written without it); it is an error here, so that an address is never written short.
    CB_CHECK_THROWS_AS(Asm16Error, operand("offset x+zz"));
});

CB_TEST("port.asm16.parse", [] {
    const std::string text_in =
        "        TITLE  test\n"
        "DGROUP  GROUP  DATA,BSS\n"
        "_TEXT   SEGMENT BYTE PUBLIC 'CODE'\n"
        "        PUBLIC _foo, bar\n"
        "cnt     EQU    5\n"
        "_foo    PROC   NEAR\n"
        " lab1:  rep movsb\n"
        "        repne  scasb\n"
        "lab2:   mov    ax, word ptr [bp+4] ; comment\n"
        "        ES:  mov  al, 1\n"
        "n::     nop\n"
        "_TEXT   ENDS\n";
    const std::vector<Line> lines = parse(text_in);
    CB_CHECK_EQ(lines.size(), 13u);
    CB_CHECK_EQ(lines[0].directive->kind, "TITLE");
    CB_CHECK(!lines[0].directive->name);
    CB_CHECK_EQ(lines[1].directive->kind, "GROUP");
    CB_CHECK_EQ(lines[1].directive->name.value(), "DGROUP");
    CB_CHECK_EQ(lines[1].directive->rest, "DATA,BSS");
    CB_CHECK_EQ(lines[2].directive->kind, "SEGMENT");
    CB_CHECK_EQ(lines[2].directive->rest, "BYTE PUBLIC 'CODE'");
    CB_CHECK_EQ(lines[4].directive->kind, "EQU");
    CB_CHECK_EQ(lines[5].directive->kind, "PROC");
    CB_CHECK_EQ(lines[6].label.value(), "lab1");
    CB_CHECK_EQ(lines[6].prefix.value(), "rep");
    CB_CHECK_EQ(lines[6].op.value(), "movsb");
    CB_CHECK_EQ(lines[7].prefix.value(), "repne");
    CB_CHECK_EQ(lines[7].op.value(), "scasb");
    CB_CHECK_EQ(lines[8].op.value(), "mov");
    CB_CHECK((lines[8].args == std::vector<std::string>{"ax", "word ptr [bp+4]"}));
    CB_CHECK(!lines[9].label);   // (`ES:` is a segment override, not a label)
    CB_CHECK_EQ(lines[9].op.value(), "es:");
    CB_CHECK(!lines[10].label);  // (`n::` is no label)
    CB_CHECK_EQ(lines[10].no, 11);
    CB_CHECK(parse("").size() == 1 && !parse("")[0].op);
    CB_CHECK_THROWS_AS(Asm16Error, parse("rep"));   // (DIVERGES: an IndexError in the Python)
    CB_CHECK_EQ(parse("foo EQU 3")[0].directive->name.value(), "foo");
    CB_CHECK_EQ(parse("@x: ret")[0].label.value(), "@x");
});

CB_TEST("port.asm16.code_segments", [] {
    auto segs = [](const std::string& t) {
        const auto lines = parse(t);
        const std::set<std::string> s = code_segments(lines);
        return std::vector<std::string>(s.begin(), s.end());
    };
    using V = std::vector<std::string>;
    CB_CHECK((segs("_TEXT SEGMENT BYTE PUBLIC 'CODE'\nASSUME CS:_TEXT\n") == V{"_TEXT"}));
    CB_CHECK((segs("_TEXT SEGMENT 'CODE'\nASSUME CS:DG\nDG GROUP _TEXT,X\n") == V{"X", "_TEXT"}));
    CB_CHECK((segs("FOO_TEXT SEGMENT\nFOO_TEXT ENDS\n") == V{"FOO_TEXT"}));
    CB_CHECK((segs("A SEGMENT 'PROG'\n") == V{"A"}));
    CB_CHECK((segs("ASSUME cs:NOTHING\n").empty()));
    CB_CHECK((segs("X GROUP A, B ; c\nASSUME CS:X\n") == V{"A", "B"}));
    CB_CHECK((segs("D SEGMENT 'DATA'\n").empty()));
});

CB_TEST("port.asm16.decode_direct", [] {
    auto show = [](std::vector<std::uint8_t> b) -> std::string {
        const auto d = decode_direct(b);
        return d ? std::to_string(d->length) + " " + d->op + " " + text::join(d->args, ",") : std::string("none");
    };
    CB_CHECK_EQ(show({0x8B, 0x1E, 0x34, 0x12}), "4 mov bx,word ptr [01234h]");
    CB_CHECK_EQ(show({0x89, 0x0E, 0xFF, 0x00}), "4 mov word ptr [000ffh],cx");
    CB_CHECK_EQ(show({0x8A, 0x06, 0x01, 0x02}), "4 mov al,byte ptr [00201h]");
    CB_CHECK_EQ(show({0x88, 0x26, 0x10, 0x20}), "4 mov byte ptr [02010h],ah");
    CB_CHECK_EQ(show({0xA0, 0x12, 0x34}), "3 mov al,byte ptr [03412h]");
    CB_CHECK_EQ(show({0xA3, 0xFF, 0xFF}), "3 mov word ptr [0ffffh],ax");
    CB_CHECK_EQ(show({0x8B, 0x1E, 0x34}), "none");   // (too short)
    CB_CHECK_EQ(show({0x90, 0x90, 0x90, 0x90}), "none");
    CB_CHECK_EQ(show({}), "none");
});

// ---- the old assembler's listing ------------------------------------------------------------------------------------

CB_TEST("port.asm16.listing_of_the_old_assembler", [] {
    const J want = J::parse(fs::read_text(test_data("golden/listings.json")));
    for (const char* name : {"strlen", "string", "movbytes"}) {
        const Listing l = parse_listing(fs::read_text(test_data(std::string("golden/lib220_") + name + ".lst")));
        J labels = J::array(), lines = J::array();
        for (const auto& [k, p] : l.labels) labels.push_back(J::array({k, J::array({p.seg, p.offset})}));
        for (const ListedLine& x : l.lines) {
            std::string hex;
            for (const std::uint8_t b : x.bytes) hex += std::format("{:02x}", b);
            lines.push_back(J::array({x.seg, x.offset, x.length, x.text, hex}));
        }
        CB_CHECK(labels == want[name]["labels"]);
        CB_CHECK(lines == want[name]["lines"]);
    }
    // a damaged listing gives what can be read
    CB_CHECK(parse_listing("").lines.empty());
    CB_CHECK(parse_listing("garbage\n\n 12345 nothing\n").labels.empty());
});

CB_TEST("port.asm16.listing_offsets_runs_the_assembler_it_is_given", [] {
    const std::string lst = fs::read_text(test_data("golden/lib220_strlen.lst"));
    std::string seen_args;
    bool had_source = false;
    const Listing l = listing_offsets(test_data("lib220_strlen.asm"), test_data("lib220_strlen.asm"),
                                      [&](const std::filesystem::path& folder, std::string_view args) {
                                          seen_args = std::string(args);
                                          const std::string src = fs::read_text(folder / "T.ASM");
                                          had_source = src.find("\r\n") != std::string::npos && src.find("_strlen") != std::string::npos;
                                          fs::write_text(folder / "T.LST", lst);
                                      });
    CB_CHECK_EQ(seen_args, " /ML T.ASM,T.OBJ,T.LST;");
    CB_CHECK(had_source);
    CB_CHECK(!l.lines.empty());
    CB_CHECK_EQ(l.lines.front().seg, "_TEXT");
});

// ---- the C program's types -----------------------------------------------------------------------------------------

CB_TEST("port.asm16.ctypes_variables", [] {
    const std::string orig = kHeader + "\nfar char *g_far;\nchar far *g_far2[3];\n";
    const std::vector<std::string> port = {kHeader}, origs = {orig};
    const CTypes ct(port, origs);
    auto var = [&](const char* n) {
        const VarType* v = ct.var(n);
        return v ? v->base + "|" + v->declarator : std::string("none");
    };
    CB_CHECK_EQ(var("g_a"), "struct a|*g_a");
    CB_CHECK_EQ(var("g_s"), "char|*g_s");
    CB_CHECK_EQ(var("g_i"), "int|g_i[10]");
    CB_CHECK_EQ(var("g_u"), "unsigned|g_u");   // (an initialiser and `static` go)
    CB_CHECK_EQ(var("g_c"), "struct c|g_c");   // (and `extern`)
    CB_CHECK_EQ(var("g_bp"), "struct b|*g_bp[4]");
    CB_CHECK_EQ(var("zz_none"), "none");
    CB_CHECK(!ct.var("T"));                    // (a typedef is no variable)
    CB_CHECK(ct.is_pointer("g_a") && ct.is_pointer("g_s") && ct.is_pointer("g_bp"));
    CB_CHECK(!ct.is_pointer("g_i") && !ct.is_pointer("g_l") && !ct.is_pointer("zz_none"));
    CB_CHECK_EQ(ct.pointee("g_a").value(), "struct a");
    CB_CHECK_EQ(ct.pointee("g_s").value(), "char");
    CB_CHECK(!ct.pointee("g_l"));
    auto es = [&](const char* n) {
        const auto e = ct.elem_sizes(n);
        return e ? std::to_string(e->old_size) + "," + std::to_string(e->new_size) : std::string("none");
    };
    CB_CHECK_EQ(es("g_a"), "2,8");
    CB_CHECK_EQ(es("g_l"), "4,4");
    CB_CHECK_EQ(es("g_b"), "8,18");   // (struct b: a pointer, a far pointer and an int, as 8086 and as x64)
    CB_CHECK_EQ(es("g_c"), "6,6");
    CB_CHECK_EQ(es("g_i"), "none");
    CB_CHECK_EQ(es("g_d"), "none");
    CB_CHECK_EQ(es("zz_none"), "none");
    CB_CHECK(ct.far_ptrs.contains("g_far2"));   // (a far pointer in the original: 4 bytes)
    CB_CHECK(!ct.elem_sizes("g_far2").has_value());   // (not a global the port defines)
});

CB_TEST("port.asm16.ctypes_fields", [] {
    // the original had `char far *p` (4 bytes) where the port has 8-byte pointers
    std::string orig = kHeader;
    for (const auto& [from, to] : {std::pair<const char*, const char*>{"char *p", "char far *p"}, {"far char *fp", "char far *fp"}})
        for (std::size_t at = orig.find(from); at != std::string::npos; at = orig.find(from, at + std::string(to).size()))   // (`p` and `pp`)
            orig.replace(at, std::string(from).size(), to);
    const CTypes ct(std::vector<std::string>{kHeader}, std::vector<std::string>{orig});
    auto field = [&](const char* st, int off) {
        const auto f = ct.field(st, off);
        if (!f) return std::string("none");
        return f->name + " " + std::to_string(f->old_offset) + "+" + std::to_string(f->old_size) + " -> " + std::to_string(f->new_offset) + "+" +
               std::to_string(f->new_size) + (f->is_pointer ? " ptr " : " val ") + f->type;
    };
    CB_CHECK_EQ(field("struct a", 0), "x 0+2 -> 0+2 val int");
    CB_CHECK_EQ(field("struct a", 1), "x 0+2 -> 0+2 val int");   // (a member around the offset)
    CB_CHECK_EQ(field("struct a", 2), "p 2+4 -> 2+8 ptr char");
    CB_CHECK_EQ(field("struct a", 5), "p 2+4 -> 2+8 ptr char");
    CB_CHECK_EQ(field("struct a", 6), "l 6+4 -> 10+4 val long");
    CB_CHECK_EQ(field("struct a", 10), "u 10+2 -> 14+2 val int");   // (inside the nested struct: its member)
    CB_CHECK_EQ(field("struct a", 12), "q 12+2 -> 16+8 ptr char");
    CB_CHECK_EQ(field("struct a", 14), "none");                       // (a union of scalars: no member to name)
    CB_CHECK_EQ(field("struct a", 18), "buf 18+6 -> 32+6 val char");
    CB_CHECK_EQ(field("struct a", 24), "none");
    CB_CHECK_EQ(field("struct b", 0), "first 0+2 -> 0+8 ptr struct a");
    CB_CHECK_EQ(field("struct b", 2), "fp 2+4 -> 8+8 ptr char");
    CB_CHECK_EQ(field("struct b", 6), "n 6+2 -> 16+2 val int");
    CB_CHECK_EQ(field("struct c", 3), "none");
    CB_CHECK_EQ(field("struct c", 4), "w 4+2 -> 4+2 val int");
    CB_CHECK_EQ(field("struct e", 2), "b 2+2 -> 8+8 ptr char");
    CB_CHECK_EQ(field("struct zz_none", 0), "none");
    CB_CHECK(!ct.field("struct c", 100));
    CB_CHECK_EQ(ct.member_type("struct a", 2).value(), "char *");
    CB_CHECK_EQ(ct.member_type("struct a", 0).value(), "int");
    CB_CHECK_EQ(ct.member_type("struct a", 10).value(), "struct a.in");   // (a nested structure's address: its own name)
    CB_CHECK_EQ(ct.member_type("struct a", 14).value(), "struct a.un");
    CB_CHECK_EQ(ct.member_type("struct b", 0).value(), "struct a *");
    CB_CHECK(!ct.member_type("struct a", 1));
    CB_CHECK(!ct.member_type("struct zz_none", 0));
    CB_CHECK(ct.old_layouts.at("struct a.un").is_union);
    CB_CHECK(!ct.old_layouts.at("struct a.in").is_union);
    CB_CHECK_EQ(ct.old_layouts.at("struct a").find("p")->type, "char");
});

CB_TEST("port.asm16.c_functions", [] {
    const std::string src =
        "#define NEAR near pascal\n"
        "#define FARF far\n"
        "#define NOTCALL int x\n"
        "int NEAR add(a, b)\nint a, b;\n{\n    return a + b;\n}\n"
        "char *getp(p, q, n)\nchar *p;\nchar far *q;\nlong n;\n{\n    return p;\n}\n"
        "long lmul(a, b)\nlong a;\ndouble b;\n{\n    return a;\n}\n"
        "void nothing()\n{\n}\n"
        "struct s *mk(n)\nstruct s x;\n{\n}\n"
        "static unsigned FARF fp(v)\nunsigned v[];\n{\n}\n"
        "int add(a)\nint a;\n{\n}\n"
        "float ff(f)\nfloat f;\n{\n}\n"
        "int add(a, b, c)\n{\n}\n";
    const OrderedMap<std::string, CSig> fns = c_functions(std::vector<std::string>{src});
    auto show = [&](const char* n) {
        const CSig* s = fns.find(n);
        if (!s) return std::string("none");
        std::string out = s->pascal ? "pascal " : "cdecl ";
        for (const CArg& a : s->args) out += std::to_string(a.size) + (a.kind == CArg::Kind::Val ? "v" : a.kind == CArg::Kind::Long ? "l" : a.kind == CArg::Kind::Ptr ? "p" : "d") + " ";
        out += s->ret == CSig::Ret::Val ? "-> val" : s->ret == CSig::Ret::Long ? "-> long" : s->ret == CSig::Ret::Ptr ? "-> ptr" : "-> void";
        if (s->to) out += " " + *s->to;
        return out;
    };
    CB_CHECK_EQ(show("getp"), "cdecl 2p 4p 4l -> ptr char");
    CB_CHECK_EQ(show("lmul"), "cdecl 4l 8d -> long");
    CB_CHECK_EQ(show("nothing"), "cdecl -> void");
    CB_CHECK_EQ(show("mk"), "cdecl 2v -> ptr struct s");   // (`struct s x` is declared but `n` is not: an int)
    CB_CHECK_EQ(show("fp"), "cdecl 2p -> val");
    CB_CHECK_EQ(show("ff"), "cdecl 8d -> val");
    // `add` is defined three times: the first with the most arguments wins, ties go to the later
    CB_CHECK_EQ(show("add"), "cdecl 2v 2v 2v -> val");
    CB_CHECK(c_functions(std::vector<std::string>{}).empty());
    // the calling words of a macro make the function pascal
    const OrderedMap<std::string, CSig> only_pascal = c_functions(std::vector<std::string>{"#define NEAR near pascal\nint NEAR f(a)\nint a;\n{\n}\n"});
    CB_CHECK(only_pascal.find("f") && only_pascal.find("f")->pascal);
    const RuntimeCall* m = runtime_call("_malloc");
    CB_CHECK(m && m->c_name == "malloc" && m->sig.ret == CSig::Ret::Ptr);
    CB_CHECK(runtime_call("__ffree") && runtime_call("__ffree")->c_name == "dos_ffree");
    CB_CHECK(!runtime_call("foo"));
});

// ---- what a register holds ----------------------------------------------------------------------------------------

CB_TEST("port.asm16.kinds_join", [] {
    const FirstMembers none;
    const FirstMembers chain = {{"struct b", "struct a"}};
    const std::optional<K> val = K::val(), ptr_char = K::ptr("char"), ptr_a = K::ptr("struct a"), ptr_b = K::ptr("struct b"), opq = K::opq();
    auto show = [](const std::optional<K>& k) {
        if (!k) return std::string("none");
        static const char* const names[] = {"val", "ptr", "opq", "frame", "seg", "ret"};
        return std::string(names[static_cast<int>(k->kind)]) + (k->to ? "(" + *k->to + ")" : std::string());
    };
    CB_CHECK_EQ(show(join(std::nullopt, ptr_char, none)), "ptr(char)");
    CB_CHECK_EQ(show(join(ptr_char, std::nullopt, none)), "ptr(char)");
    CB_CHECK_EQ(show(join(val, val, none)), "val");
    CB_CHECK_EQ(show(join(ptr_char, ptr_char, none)), "ptr(char)");
    CB_CHECK_EQ(show(join(ptr_char, ptr_a, none)), "ptr");                 // (two pointees: one pointer that points nowhere known)
    CB_CHECK_EQ(show(join(val, ptr_char, none)), "opq(char)");              // (a number and an address: possibly an address)
    CB_CHECK_EQ(show(join(ptr_char, val, none)), "opq(char)");
    CB_CHECK_EQ(show(join(opq, ptr_char, none)), "opq");
    CB_CHECK_EQ(show(join(ptr_a, ptr_b, chain)), "ptr(struct a)");         // (a chain's head: struct b begins with a pointer to a)
    CB_CHECK_EQ(show(join(ptr_b, ptr_a, chain)), "ptr(struct a)");
    CB_CHECK_EQ(show(join(ptr_a, ptr_b, none)), "ptr");
    // origins are united
    const K in_ax = K::val(OriginSet({Origin{Origin::Source::In, "r", "ax", 0}}));
    const auto joined = join(K::val(), in_ax, none);
    CB_CHECK(joined && joined->origin.get().size() == 1);
    CB_CHECK(!join(in_ax, K::val(), none)->origin.empty());
    // a code pointer joined with a typed one: the pointee is lost
    CB_CHECK_EQ(show(join(K::code_ptr(), ptr_char, none)), "ptr");
    CB_CHECK(join(K::code_ptr(), K::code_ptr(), none)->region == Region::Code);
});

CB_TEST("port.asm16.kinds_merge", [] {
    const FirstMembers none;
    State s1, s2;
    s1.slot("ax") = K::val();
    s1.slot("bx") = K::ptr("char");
    s1.stack = {{2, 8, K::val()}, {2, 8, std::nullopt}};
    s2.slot("ax") = K::ptr("char");
    s2.slot("cx") = K::opq();
    s2.stack = {{2, 8, K::ptr("char")}, {2, 8, K::val()}};
    s2.routines = {"r"};
    const State m = merge(&s1, s2, none);
    CB_CHECK(m.reg("ax")->kind == Kind::Opq && m.reg("ax")->to.value() == "char");
    CB_CHECK(m.reg("bx")->kind == Kind::Ptr);
    CB_CHECK(m.reg("cx")->kind == Kind::Opq);
    CB_CHECK(!m.reg("dx"));
    CB_CHECK_EQ(m.stack.size(), 2u);
    CB_CHECK(m.stack[0].k->kind == Kind::Opq);
    CB_CHECK(m.stack[1].k->kind == Kind::Val);
    CB_CHECK(m.routines == std::set<std::string>{"r"});
    CB_CHECK(same_state(merge(nullptr, s1, none), s1));
    CB_CHECK(!same_state(s1, s2));
    // stacks of different depths: the first stays
    State s3;
    s3.stack = {{2, 8, std::nullopt}};
    CB_CHECK_EQ(merge(&s1, s3, none).stack.size(), 2u);
    // the fixed point ignores what only the stack's origins say
    State a = s1, b = s1;
    b.stack[0].k = K::val(OriginSet({Origin{Origin::Source::Param, "bx", "", 4}}));
    CB_CHECK(same_state(a, b));
    b.slot("ax") = K::val(OriginSet({Origin{Origin::Source::Param, "bx", "", 4}}));
    CB_CHECK(!same_state(a, b));
});

CB_TEST("port.asm16.inner_name", [] {
    CB_CHECK_EQ(inner_name("_foo", {"foo"}), "_foo");
    CB_CHECK_EQ(inner_name("foo", {"foo"}), "@foo");
    CB_CHECK_EQ(inner_name("FOO", {"foo"}), "FOO");   // (the C name is `foo`, not the label)
    CB_CHECK_EQ(inner_name("_bar", {"foo"}), "_bar");
});

// ---- whole modules ------------------------------------------------------------------------------------------------

namespace {

std::string read_latin1(const std::filesystem::path& p) { return fs::read_text(p); }

std::vector<std::string> split_synth() {
    const std::string all = read_latin1(test_data("synth.txt"));
    std::vector<std::string> out;
    std::size_t pos = 0;
    while (true) {
        const std::size_t at = all.find("\n@@@@\n", pos);
        if (at == std::string::npos) {
            out.push_back(all.substr(pos));
            break;
        }
        out.push_back(all.substr(pos, at - pos));
        pos = at + 6;
    }
    return out;
}

// A synthetic old listing: a place for each label, and for each table line of the code.
void make_listing(Port& m) {
    for (const auto& [label, idx] : m.labels()) m.offsets.labels.insert_or_assign(label, Place{*m.code()[idx].seg, static_cast<std::uint32_t>(16 * idx)});
    for (std::size_t i = 0; i < m.code().size(); ++i) {
        const Line& l = m.code()[i];
        if (!l.directive || (l.directive->kind != "DB" && l.directive->kind != "DW" && l.directive->kind != "DD")) continue;
        std::string collapsed;
        bool sp = false;
        for (const char c : strip_comment(l.text)) {
            if (text::is_space(c)) sp = true;
            else {
                if (sp && !collapsed.empty()) collapsed.push_back(' ');
                sp = false;
                collapsed.push_back(c);
            }
        }
        static const std::uint8_t kBytes[] = {0x8B, 0x1E, 0x34, 0x12};
        ListedLine ll{*l.seg, static_cast<std::uint32_t>(16 * i + 1), 2, collapsed.substr(0, 24), {}};
        ll.bytes.assign(kBytes, kBytes + (i % 4) + 1);
        m.offsets.lines.push_back(std::move(ll));
    }
}

J facts_of(const Port& m) {
    J f;
    f["labels"] = J::array();
    for (const auto& [k, v] : m.labels()) f["labels"].push_back(J::array({k, v}));
    f["cname"] = J::array();
    for (const auto& [k, v] : m.cname) f["cname"].push_back(J::array({k, v}));
    f["pascal"] = std::vector<std::string>(m.pascal_routines().begin(), m.pascal_routines().end());
    f["called"] = std::vector<std::string>(m.called().begin(), m.called().end());
    std::vector<std::string> far;
    f["far"] = J::array();
    for (const Rt& r : m.far)
        if (r) far.push_back(*r);
    f["far"] = far;
    f["own_ptr"] = J::array();
    for (const auto& [s, d] : m.own_pointers()) f["own_ptr"].push_back(J::array({s, d}));
    f["params"] = J::array();
    for (const auto& [r, ps] : m.params()) {
        J lst = J::array();
        for (const Param& p : ps) lst.push_back(J::array({p.old_offset, p.size}));
        f["params"].push_back(J::array({r ? *r : std::string(), lst}));
    }
    f["unreached"] = m.unreached();
    f["asm_calls"] = std::vector<std::string>(m.asm_calls().begin(), m.asm_calls().end());
    std::vector<std::string> cc;
    for (const auto& [k, v] : m.c_calls()) cc.push_back(k);
    f["c_calls"] = cc;
    f["data_used"] = std::vector<std::string>(m.data_used().begin(), m.data_used().end());
    f["notes"] = m.notes();
    f["used_int21"] = m.used_dos();
    f["used_null"] = m.used_null_segment();
    return f;
}

// Ports the module as the golden's Python did, and compares the text and the facts.
void check_case(const std::string& id, Port& m, const std::set<std::string>& longret, const J& all_facts) {
    const J& want = all_facts.at(id);
    std::string text_out, error;
    try {
        m.run();
        text_out = m.emit(longret);
    } catch (const Asm16Error& e) {
        error = e.what();
    }
    if (want.contains("error")) {
        CB_CHECK_EQ(error, want["error"].get<std::string>());
        return;
    }
    CB_CHECK_EQ(error, "");
    const std::string golden = read_latin1(test_data("golden/" + id + ".asm"));
    if (text_out != golden) {
        std::size_t i = 0;
        while (i < text_out.size() && i < golden.size() && text_out[i] == golden[i]) ++i;
        testing::record_failure(__FILE__, __LINE__, id + ": text differs from the golden at byte " + std::to_string(i) + "\n--- got\n" + text_out.substr(i > 60 ? i - 60 : 0, 160) +
                                                          "\n--- want\n" + golden.substr(i > 60 ? i - 60 : 0, 160));
    }
    testing::count_check();
    const J got = facts_of(m);
    for (const auto& [key, value] : want.items())
        if (got[key] != value) testing::record_failure(__FILE__, __LINE__, id + ": " + key + " is " + got[key].dump() + ", wanted " + value.dump());
    testing::count_check();
}

}  // namespace

CB_TEST("port.asm16.synthetic_modules_match_the_python", [] {
    const std::vector<std::string> parts = split_synth();
    const J all_facts = J::parse(fs::read_text(test_data("golden/facts.json")));
    const std::string hdr = parts[5];
    const std::vector<std::string> hdrs = {hdr};
    const CTypes ct(hdrs, hdrs);
    std::string cfun = parts[6];
    for (std::size_t at; (at = cfun.find("PASC")) != std::string::npos;) cfun.replace(at, 4, "pascal");
    const OrderedMap<std::string, CSig> cf = c_functions(std::vector<std::string>{cfun});
    const std::vector<std::string> calls = {"walk", "sum", "lenp", "strx", "far1", "dosit", "p2", "p3", "bad", "q", "q5", "cfun", "other"};
    for (int mod = 1; mod <= 5; ++mod) {
        for (int variant = 0; variant < 3; ++variant) {
            const std::string name = "m" + std::to_string(mod);
            PortSettings s;
            s.c_names = calls;
            s.c_funcs = cf;
            if (variant == 1) s.asm_routines = {"_other"};
            if (variant == 2) s.pascal = {"sum"};
            s.underscore = variant != 1;
            Port m(module(name, parts[static_cast<std::size_t>(mod - 1)]), ct, s);
            if (variant) {
                for (const auto& [p, c] : m.cname) m.param_types[p] = {{2, "struct node"}, {4, std::nullopt}, {2, std::nullopt}};
                if (variant == 1) m.far = {Rt("strx")};
                m.data_map = {{0x100, "_val"}, {0x200, "_arr"}, {0x230, "_ptrs"}};
                m.aliases["val"] = {"arr", 4};
                if (variant == 2) m.used_outside = {"_walk"};
                make_listing(m);
            }
            check_case("synth_" + name + "_" + std::to_string(variant), m, variant == 2 ? std::set<std::string>{"far1"} : std::set<std::string>{}, all_facts);
        }
    }
});

CB_TEST("port.asm16.old_library_modules_match_the_python", [] {
    const J all_facts = J::parse(fs::read_text(test_data("golden/facts.json")));
    const std::vector<std::string> empty = {""};
    const CTypes ct(empty, empty);
    struct Case {
        const char* id;
        const char* file;
        std::vector<std::string> c_names;
        std::set<std::string> others;
    };
    for (const Case& c : {Case{"lib220_strlen", "lib220_strlen.asm", {"strlen"}, {}}, Case{"lib220_string", "lib220_string.asm", {"strcpy", "strcat"}, {"_strlen"}},
                          Case{"lib220_movbytes", "lib220_movbytes.asm", {"movbytes"}, {}}}) {
        PortSettings s;
        s.c_names = c.c_names;
        s.asm_routines = c.others;
        Port m(module(c.file, read_latin1(test_data(c.file))), ct, s);
        check_case(c.id, m, {}, all_facts);
    }
});

CB_TEST("port.asm16.the_kinds_of_a_routine", [] {
    // strcat: di is an address (the argument), ax (strlen's answer) a number, and the callee's writes are known
    const std::vector<std::string> empty = {""};
    const CTypes ct(empty, empty);
    PortSettings s;
    s.c_names = {"strcpy", "strcat"};
    s.asm_routines = {"_strlen"};
    Port m(module("string", read_latin1(test_data("lib220_string.asm"))), ct, s);
    m.run();
    CB_CHECK_EQ(m.labels().size(), 3u);   // _strcpy, CPY1, _strcat (cret and _strlen are outside)
    CB_CHECK(m.public_names().contains("_strcpy") && m.public_names().contains("_strcat"));
    CB_CHECK_EQ(m.cname.at("_strcat"), "strcat");
    CB_CHECK(m.called().contains("_strcpy"));   // (strcat calls strcpy)
    const std::set<std::string> w = m.writes("_strcpy");
    CB_CHECK(w.contains("ax") && w.contains("si") && w.contains("di") && w.contains("dx"));
    CB_CHECK(w.contains("bp"));   // (mov bp, sp)
    // the arguments of strcpy: two words after the return address (sp+2, sp+4 at the entry)
    const auto params = m.params().at("_strcpy");
    CB_CHECK_EQ(params.size(), 2u);
    CB_CHECK_EQ(params[0].old_offset, 2);
    CB_CHECK_EQ(params[1].old_offset, 4);
    CB_CHECK(!m.exit_of("_strcpy").has_value());   // (it leaves by a jump to cret, outside the module: no return of its own)
    // a line is in the routine whose label is the nearest one above
    CB_CHECK_EQ(m.routine(0).value(), "_strcpy");
    CB_CHECK_EQ(m.routine(m.labels().at("_strcat") + 2).value(), "_strcat");
});

CB_TEST("port.asm16.errors_name_the_line", [] {
    const std::vector<std::string> empty = {""};
    const CTypes ct(empty, empty);
    auto port_of = [&](const std::string& src) {
        PortSettings s;
        s.c_names = {"f"};
        auto m = std::make_unique<Port>(module("t", src), ct, s);
        m->run();
        return m;
    };
    const std::string head = "_TEXT SEGMENT BYTE PUBLIC 'CODE'\n ASSUME CS:_TEXT\n PUBLIC _f\n_f PROC NEAR\n";
    const std::string tail = "\n ret\n_f ENDP\n_TEXT ENDS\n END\n";
    // an instruction with no rule
    auto m1 = port_of(head + " frobnicate ax" + tail);
    try {
        (void)m1->emit();
        CB_CHECK(false);
    } catch (const Asm16Error& e) {
        CB_CHECK_EQ(std::string(e.what()), "line 5 (frobnicate ax): no rule for frobnicate");
    }
    // a call outside the module that nothing names
    auto m2 = port_of(head + " call nowhere" + tail);
    try {
        (void)m2->emit();
        CB_CHECK(false);
    } catch (const Asm16Error& e) {
        CB_CHECK_EQ(std::string(e.what()), "line 5 (call nowhere): a call of nowhere, outside the module (not yet ported)");
    }
    // a pop to memory
    auto m3 = port_of(head + " pop [bx]" + tail);
    CB_CHECK_THROWS_AS(Asm16Error, (void)m3->emit());
    // an int that is not DOS
    auto m4 = port_of(head + " int 10h" + tail);
    CB_CHECK_THROWS_AS(Asm16Error, (void)m4->emit());
    // an operand with no register, symbol or number to its sum, and a prefix with nothing after it
    CB_CHECK_THROWS_AS(Asm16Error, port_of(head + " mov ax, [foo+bar]" + tail));
    CB_CHECK_THROWS_AS(Asm16Error, port_of(head + " rep" + tail));
});

CB_TEST("port.asm16.dos_and_segments_become_the_platform_layer_and_one_flat_memory", [] {
    const std::vector<std::string> empty = {""};
    const CTypes ct(empty, empty);
    PortSettings s;
    s.c_names = {"f"};
    const std::string src =
        "_TEXT SEGMENT BYTE PUBLIC 'CODE'\n ASSUME CS:_TEXT\n PUBLIC _f\n_f PROC NEAR\n push bp\n mov bp, sp\n push ds\n pop es\n mov ah, 9\n int 21h\n pop bp\n ret\n_f ENDP\n_TEXT ENDS\n END\n";
    Port m(module("t", src), ct, s);
    m.run();
    const std::string out = m.emit();
    CB_CHECK(out.find("EXTERN dos_int21:PROC") != std::string::npos);
    CB_CHECK(out.find("call\tdos_int21") != std::string::npos);
    CB_CHECK(out.find("; push ds\t\t(a segment: one flat memory)") != std::string::npos);
    CB_CHECK(out.find("\n\tpush\trbp") != std::string::npos);   // (bp holds the caller's frame: pushed whole)
    CB_CHECK(m.used_dos());
    CB_CHECK(out.find("f PROC") != std::string::npos);
    CB_CHECK(out.find("PUBLIC f") != std::string::npos);
    // a far routine's arguments begin four bytes up
    Port far_port(module("t", src), ct, s);
    far_port.far.insert(Rt("_f"));
    far_port.run();
    CB_CHECK(far_port.emit().find("ret") != std::string::npos);
});

CB_TEST("port.asm16.damaged_source_is_an_asm16_error_or_a_port", [] {
    // the real modules cut at many places, and with a byte changed: either ported or refused by name, never anything else
    const std::vector<std::string> parts = split_synth();
    const std::vector<std::string> hdrs = {parts[5]};
    const CTypes ct(hdrs, hdrs);
    PortSettings s;
    s.c_names = {"walk", "sum", "lenp", "strx", "far1", "dosit", "p2", "p3", "strcpy", "strcat", "strlen"};
    s.asm_routines = {"_strlen", "_other"};
    std::size_t ported = 0, refused = 0;
    for (const std::string& original : {parts[0], read_latin1(test_data("lib220_string.asm")), parts[2]}) {
        for (std::size_t cut = 0; cut < original.size(); cut += 13) {
            for (const bool flip : {false, true}) {
                std::string src = original.substr(0, cut);
                if (flip && cut) src[cut / 2] = static_cast<char>(src[cut / 2] ^ 0x20);
                try {
                    Port m(module("t", src), ct, s);
                    m.run();
                    (void)m.emit();
                    ++ported;
                } catch (const Asm16Error&) {
                    ++refused;
                }
            }
        }
    }
    CB_CHECK(ported > 50 && refused > 50);
    // emit before run is a misuse, said so
    const std::vector<std::string> empty = {""};
    const CTypes none(empty, empty);
    Port unrun(module("t", "_TEXT SEGMENT BYTE PUBLIC 'CODE'\n nop\n_TEXT ENDS\n END\n"), none, {});
    CB_CHECK_THROWS_AS(Asm16Error, (void)unrun.emit());
});

CB_TEST("port.asm16.an_offset_with_a_displacement_keeps_it", [] {
    // DIVERGES (a bug of the Python, fixed in both): `push offset v+4`, `add ax, offset v+2` and `cmp bx, offset v+6` wrote the
    // address of v without its displacement.
    const std::vector<std::string> empty = {""};
    const CTypes ct(empty, empty);
    PortSettings s;
    s.c_names = {"f"};
    const std::string src =
        "_TEXT SEGMENT BYTE PUBLIC 'CODE'\n ASSUME CS:_TEXT\n EXTRN _v:WORD\n PUBLIC _f\n_f PROC NEAR\n push offset _v+4\n add ax, offset _v+2\n"
        " mov bx, offset _v-2\n ret\n_f ENDP\n_TEXT ENDS\n END\n";
    Port m(module("t", src), ct, s);
    m.run();
    const std::string out = m.emit();
    CB_CHECK(out.find("\tlea\tr11, v+4\n\tpush\tr11") != std::string::npos);
    CB_CHECK(out.find("\tlea\tr11, v+2\n\tadd\trax, r11") != std::string::npos);   // (ax is an argument register at the entry: whole)
    CB_CHECK(out.find("\tlea\trbx, v-2") != std::string::npos);
});
