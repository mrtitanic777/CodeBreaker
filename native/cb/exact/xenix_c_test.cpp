// cb-unity: alone
// Tests of cb::exact::xenix_c: the XENIX C compiler (cpp, c0, c1, c2, as) as a toolchain, in the revision Microsoft built LINK and LIB with.
// The assembly rules are tested on small hand-made assembly (each case worked out by hand from the rule), the c1 and c2 patches on images
// that have, or do not have, the bytes they are written for, c2's block-move hooks on a machine of memory made here (a list of c2's
// instructions in which one block is moved), the variants and the cache without the kit. With the kit (CB_KIT_XENIX86 names the XENIX
// files' folder) both variants compile a small C file to an object, and one that does not parse is refused with the pass that stopped.
//
// Hand-checked: each rule's pattern and result, the patch's offsets (which bytes `was` must be), the hook's registers
// and the list's links after a swap, the line a stopped pass adds to the log.
//
// Design notes: a variant's `patch` that is not a boolean is false; the cache key is made
// from the profile's JSON text; the rules are applied by their letters in order, a letter that is no rule is
// a UsageError; a source with LF line ends is the same compile as one with CR LF.
#include "cb/exact/xenix_c.hpp"

#include <nlohmann/json.hpp>

#include "cb/base/cache.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/testing.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/omf.hpp"
#include "cb/exact/plugin.hpp"

namespace {

namespace xc = cb::exact::xenix_c;
namespace tc = cb::exact::toolchain;
namespace xenix = cb::exact::emu::xenix;
namespace stdfs = std::filesystem;
namespace cfs = cb::fs;
using cb::Bytes;
using cb::as_bytes;
using cb::to_bytes;

constexpr const char* kProfile = R"JSON({
  "name": "xenixc", "plugin": "codebreaker.exact.xenix_c:XenixC", "kit": "xenix86", "emulator": "xenix",
  "default_variant": "microsoft",
  "variants": {"altos": {}, "microsoft": {"rules": "UDMB", "patch": true, "c2move": "RS"}, "plain": {"rules": "B"}}
})JSON";

struct Fixture {
    cfs::ScopedTempDir dir{"cbxenixc"};
    cfs::ScopedEnv no_kit{"CB_KIT_XENIX86", std::nullopt};
    tc::Locations locations;
    cb::base::cache::Config cache_config;
    std::unique_ptr<cb::base::cache::Cache> cache;
    Fixture() {
        locations.profiles = dir.path() / "profiles";
        locations.state = dir.path() / "state";
        locations.checkout_root = dir.path();
        cfs::write_text(locations.profiles / "xenixc.json", kProfile);
        cache_config.state_dir = dir.path() / "cache";
        cache_config.shared_dir = dir.path() / "shared";
        cache = std::make_unique<cb::base::cache::Cache>(cache_config);
        cache->declare_kind(tc::kCacheKind, {cb::base::cache::Scope::State, "1"});
    }
    xc::XenixC make(const std::string& variant = "") {
        tc::Context c;
        c.locations = &locations;
        c.cache = cache.get();
        return xc::XenixC(tc::read_profile(locations, "xenixc"), variant.empty() ? std::nullopt : std::optional<std::string>(variant), c);
    }
};

// A machine of 1 MB of memory, and registers, for the hooks.
class FakeMachine final : public xenix::Machine {
public:
    FakeMachine() : memory(1 << 20, 0) {}
    std::uint32_t reg(xenix::Reg r) const override { return regs.count(r) ? regs.at(r) : 0; }
    void set_reg(xenix::Reg r, std::uint32_t v) override { regs[r] = v; }
    Bytes read(std::uint32_t address, std::size_t size) const override { return Bytes(memory.begin() + address, memory.begin() + address + size); }
    void write(std::uint32_t address, cb::ByteView data) override { std::copy(data.begin(), data.end(), memory.begin() + address); }
    void stop() override {}
    std::map<xenix::Reg, std::uint32_t> regs;
    Bytes memory;
};

// c2's list in the data segment 0x2000 (linear 0x20000): a node is op, sub, forward, back and reference words, and at +0xC its count.
struct List {
    FakeMachine& m;
    std::uint32_t base = 0x20000;
    void put16(std::uint32_t a, std::uint32_t v) { m.memory[base + a] = static_cast<std::uint8_t>(v & 0xFF), m.memory[base + a + 1] = static_cast<std::uint8_t>(v >> 8); }
    std::uint32_t get16(std::uint32_t a) const { return m.memory[base + a] | (m.memory[base + a + 1] << 8); }
    void node(std::uint32_t at, std::uint8_t op, std::uint8_t sub, std::uint32_t forw, std::uint32_t back, std::uint32_t ref = 0, std::uint32_t count = 0) {
        m.memory[base + at] = op;
        m.memory[base + at + 1] = sub;
        put16(at + 2, forw);
        put16(at + 4, back);
        put16(at + 6, ref);
        put16(at + 0xC, count);
    }
    std::uint32_t forw(std::uint32_t at) const { return get16(at + 2); }
    std::uint32_t back(std::uint32_t at) const { return get16(at + 4); }
};

constexpr int kJbr = 1, kCbr = 2, kJmp = 3, kLabel = 4;

}  // namespace

CB_TEST("exact.xenix_c.rule_u_an_unsigned_word_made_a_long", [] {
    // the low register is loaded, the high one cleared: the long is made in AX:DX instead
    CB_CHECK_EQ(xc::rule_u("mov\tsi,_a\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n"), "mov\tax,_a\nsub\tdx,dx\n");
    // the same from AX itself, with a label kept on the move
    CB_CHECK_EQ(xc::rule_u("L3:mov\tsi,ax\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\nret\n"), "L3:mov\tax,ax\nsub\tdx,dx\nret\n");
    CB_CHECK_EQ(xc::rule_u("mov\tsi,ax\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n"), "sub\tdx,dx\n");
    // pushed: the pair's halves go on as AX and DX
    CB_CHECK_EQ(xc::rule_u("mov\tsi,_b\nsub\tdi,di\npush\tsi\npush\tdi\n"), "mov\tax,_b\nsub\tdx,dx\npush\tax\npush\tdx\n");
    // work done on the low register in between is done on the high one
    CB_CHECK_EQ(xc::rule_u("mov\tsi,_c\ninc\tsi\nsub\tdi,di\npush\tsi\npush\tdi\n"), "mov\tdi,_c\ninc\tdi\nsub\tdx,dx\npush\tdi\npush\tdx\n");
    // the other pair, dx:si
    CB_CHECK_EQ(xc::rule_u("mov\tdx,_d\nsub\tsi,si\nmov\tax,dx\nmov\tdx,si\n"), "mov\tax,_d\nsub\tdx,dx\n");
    // not a pair, not the low register's move, or a move that reads the register itself: as it is
    CB_CHECK_EQ(xc::rule_u("sub\tax,ax\nmov\tax,1\n"), "sub\tax,ax\nmov\tax,1\n");
    CB_CHECK_EQ(xc::rule_u("mov\tbx,_a\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n"), "mov\tbx,_a\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n");
    CB_CHECK_EQ(xc::rule_u("mov\tsi,(si)\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n"), "mov\tsi,(si)\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\n");
    // the result stored straight away is left to c2
    CB_CHECK_EQ(xc::rule_u("mov\tsi,ax\nsub\tdi,di\nmov\t_lo,si\nmov\t_hi,di\njmp\tL9\n"), "mov\tsi,ax\nsub\tdi,di\nmov\t_lo,si\nmov\t_hi,di\njmp\tL9\n");
    CB_CHECK_EQ(xc::rule_u(""), "");
    CB_CHECK_EQ(xc::rule_u("sub\tdi,di"), "sub\tdi,di");   // (the first line alone)
});

CB_TEST("exact.xenix_c.rule_d_an_increment_put_off", [] {
    // the ++ waits until the byte is fetched
    CB_CHECK_EQ(xc::rule_d("movb\tdx,(bx)\ninc\t_n\nand\tdx,#255.\nret\n"), "movb\tdx,(bx)\nand\tdx,#255.\ninc\t_n\nret\n");
    CB_CHECK_EQ(xc::rule_d("movb\tdx,(bx)\ndec\t*-4.(bp)\nmov\tdi,dx\n"), "movb\tdx,(bx)\nmov\tdi,dx\ndec\t*-4.(bp)\n");
    // through DI, when three registers are in use at that place (the last `|NREG` before it says so)
    const std::string via = "mov\tax,*-4.(bp)\nmov\tbx,ax\nmovb\tax,(bx)\ncbw\ninc\t*-4.(bp)\nadd\tax,";
    const std::string through_di = "mov\tdi,*-4.(bp)\nmovb\tax,(di)\ncbw\ninc\t*-4.(bp)\nadd\tax,";
    CB_CHECK_EQ(xc::rule_d("|NREG 3\n" + via), "|NREG 3\n" + through_di);
    CB_CHECK_EQ(xc::rule_d("|NREG 2\n" + via), "|NREG 2\n" + via);
    CB_CHECK_EQ(xc::rule_d("|NREG 3\n|NREG 2\n" + via), "|NREG 3\n|NREG 2\n" + via);   // the last one counts
    CB_CHECK_EQ(xc::rule_d(via), via);                                                     // none: left alone
    CB_CHECK_EQ(xc::rule_d("|NREG 2\n|NREG 3\n" + via + "\n" + via), "|NREG 2\n|NREG 3\n" + through_di + "\n" + through_di);
    CB_CHECK_EQ(xc::rule_d(""), "");
});

CB_TEST("exact.xenix_c.rule_m_an_operation_on_memory", [] {
    CB_CHECK_EQ(xc::rule_m("add\t(bx),-2.(bp)\nret\n"), "mov\tax,-2.(bp)\nadd\t(bx),ax\nret\n");
    CB_CHECK_EQ(xc::rule_m("L3:and\t(di),_count\nret\n"), "L3:mov\tax,_count\nand\t(di),ax\nret\n");
    CB_CHECK_EQ(xc::rule_m("L1:L2:xor\t(si),*4.(bp)\n"), "L1:L2:mov\tax,*4.(bp)\nxor\t(si),ax\n");   // (every label of the line stays before the move)
    CB_CHECK_EQ(xc::rule_m("sub\t(si),#2.(bx)\nadd\t(bx),1.+_v\n"), "mov\tax,#2.(bx)\nsub\t(si),ax\nmov\tax,1.+_v\nadd\t(bx),ax\n");   // two in a row
    // a register destination, an immediate source or a last line without its line end are not changed
    CB_CHECK_EQ(xc::rule_m("add\tax,_x\nadd\t(bx),#5.\n"), "add\tax,_x\nadd\t(bx),#5.\n");
    CB_CHECK_EQ(xc::rule_m("add\t(bx),_x"), "add\t(bx),_x");
});

CB_TEST("exact.xenix_c.rule_b_a_char_or_ed_with_a_constant", [] {
    CB_CHECK_EQ(xc::rule_b("orb\tax,#5.\nret\n"), "or\tax,#5.\nret\n");
    CB_CHECK_EQ(xc::rule_b("mov\tbx,ax\nL4:orb\tdx,#255.\n"), "mov\tbx,ax\nL4:or\tdx,#255.\n");
    CB_CHECK_EQ(xc::rule_b("orb\t(bx),#5.\norb\tax,_x\norb\tax,#5."), "orb\t(bx),#5.\norb\tax,_x\norb\tax,#5.");   // other operands, no line end
});

CB_TEST("exact.xenix_c.the_rules_are_applied_in_the_order_given", [] {
    CB_CHECK_EQ(xc::apply_rules("", "orb\tax,#5.\n"), "orb\tax,#5.\n");
    CB_CHECK_EQ(xc::apply_rules("B", "orb\tax,#5.\n"), "or\tax,#5.\n");
    CB_CHECK_EQ(xc::apply_rules("UDMB", "mov\tsi,_a\nsub\tdi,di\nmov\tax,si\nmov\tdx,di\nadd\t(bx),_x\norb\tax,#5.\n"),
                "mov\tax,_a\nsub\tdx,dx\nmov\tax,_x\nadd\t(bx),ax\nor\tax,#5.\n");
    CB_CHECK_THROWS_AS(cb::UsageError, (void)xc::apply_rules("UX", "x"));
});

CB_TEST("exact.xenix_c.c1_is_patched_where_it_keeps_its_stack_limit", [] {
    // a c1 image: a header word at 2 (the text's size), the limit word 0x472 bytes into the data after the header and the text
    Bytes c1(16 + 0x100 + 0x472 + 8, 0);
    c1[2] = 0x00;
    c1[3] = 0x01;   // the text is 0x100 bytes
    const std::size_t q = 16 + 0x100 + 0x472;
    c1[q] = 100;
    const Bytes patched = xc::patched_c1(c1);
    CB_CHECK_EQ(patched[q], 0xFF);
    CB_CHECK_EQ(patched[q + 1], 0x7F);
    CB_CHECK_EQ(patched.size(), c1.size());
    Bytes other = c1;
    other[q] = 99;   // not the c1 this was written for
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xc::patched_c1(other));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xc::patched_c1(Bytes{1, 2}));
});

CB_TEST("exact.xenix_c.c2_that_is_not_the_one_the_patch_was_written_for_is_said", [] {
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xc::patched_c2(Bytes(16 + 0x4000, 0)));
    CB_CHECK_THROWS_AS(cb::FormatError, (void)xc::patched_c2(Bytes{}));
});

CB_TEST("exact.xenix_c.c2_block_move_hook_changes_places_with_a_block", [] {
    FakeMachine m;
    List l{m};
    // the list: Q (a conditional jump to L), X, P1 (a jump to T), L (a label, one reference), B1, BEND (a jump), T (a label), and P3
    // (an earlier jump to the same label T as P1)
    const std::uint32_t Q = 0x100, X = 0x120, P1 = 0x140, L = 0x160, B1 = 0x180, BEND = 0x1A0, T = 0x1C0, P3 = 0x1E0;
    l.node(P3, kJbr, 0, Q, 0, T);
    l.node(Q, kCbr, 2, X, P3, L);
    l.node(X, 9, 0, P1, Q);
    l.node(P1, kJbr, 0, L, X, T);
    l.node(L, kLabel, 0, B1, P1, 0, 1);   // one reference
    l.node(B1, 9, 0, BEND, L);
    l.node(BEND, kJmp, 0, T, B1);
    l.node(T, kLabel, 0, 0, BEND, 0, 2);  // two references: P1's and P3's
    m.memory[l.base + 0x106 + 2] = 0x07;  // the reversed sense of the conditional jump with sub 2
    // BP points at a frame whose word at -6 is P3; DI is P1
    const std::uint32_t bp = 0x4000;
    l.put16(bp - 6, P3);
    m.regs[xenix::Reg::Di] = P1;
    m.regs[xenix::Reg::Bp] = bp;
    m.regs[xenix::Reg::Si] = X;
    const auto hooks = xc::c2_hooks("RS");
    CB_CHECK_EQ(hooks.size(), std::size_t{2});
    CB_CHECK(hooks.contains(0xCE3) && hooks.contains(0xE5A));
    hooks.at(0xCE3)(m, 0x1000, 0x2000);
    // the swap: the block L..BEND is before X now, the jump P1 is gone, the conditional jump's sense reversed, T has one reference left,
    // execution goes on at 0xE5A with the jump's place in DX
    CB_CHECK_EQ(l.forw(Q), B1);
    CB_CHECK_EQ(l.back(B1), Q);
    CB_CHECK_EQ(l.forw(BEND), L);
    CB_CHECK_EQ(l.forw(L), X);
    CB_CHECK_EQ(l.forw(X), T);
    CB_CHECK_EQ(l.back(T), X);
    CB_CHECK_EQ(m.memory[l.base + Q + 1], 0x07);
    CB_CHECK_EQ(l.get16(T + 0xC), 1u);
    CB_CHECK_EQ(l.get16(0x800), 1u);
    CB_CHECK_EQ(m.reg(xenix::Reg::Dx), Q);
    CB_CHECK_EQ(m.reg(xenix::Reg::Ip), 0xE5Au);
});

CB_TEST("exact.xenix_c.c2_block_move_hook_leaves_the_block_where_it_is", [] {
    FakeMachine m;
    List l{m};
    const std::uint32_t Q = 0x100, X = 0x120, P1 = 0x140, L = 0x160, B1 = 0x180, BEND = 0x1A0, T = 0x1C0, P3 = 0x1E0;
    l.node(P3, kJbr, 0, Q, 0, T);
    l.node(Q, kCbr, 2, X, P3, L);
    l.node(X, 9, 0, P1, Q);
    l.node(P1, kJbr, 0, L, X, T);
    l.node(L, kLabel, 0, B1, P1, 0, 1);
    l.node(B1, 9, 0, BEND, L);
    l.node(BEND, kJmp, 0, T, B1);
    l.node(T, kLabel, 0, 0, BEND, 0, 2);
    const std::uint32_t bp = 0x4000;
    l.put16(bp - 6, P3);
    m.regs[xenix::Reg::Di] = P1;
    m.regs[xenix::Reg::Bp] = bp;
    m.regs[xenix::Reg::Si] = X;
    // mode R alone: no swap; the jump is put off to the other one (P3), DI and SI are moved back, and 0xE5A later gives it back in DX
    const auto hooks = xc::c2_hooks("R");
    hooks.at(0xCE3)(m, 0x1000, 0x2000);
    CB_CHECK_EQ(l.forw(Q), X);   // nothing moved
    CB_CHECK_EQ(m.reg(xenix::Reg::Di), P3);
    CB_CHECK_EQ(m.reg(xenix::Reg::Si), l.back(X));
    CB_CHECK_EQ(m.reg(xenix::Reg::Ip), 0xD4Fu);
    CB_CHECK_EQ(l.get16(bp + 4), P3);
    hooks.at(0xE5A)(m, 0x1000, 0x2000);
    CB_CHECK_EQ(m.reg(xenix::Reg::Dx), P1);
    // a second call of 0xE5A with nothing pending changes nothing
    m.regs[xenix::Reg::Dx] = 77;
    hooks.at(0xE5A)(m, 0x1000, 0x2000);
    CB_CHECK_EQ(m.reg(xenix::Reg::Dx), 77u);
    // not a block of the kind (P3 and P1 are the same jump): returns without a change
    FakeMachine m2;
    List l2{m2};
    l2.node(P1, kJbr, 0, 0, 0, T);
    l2.put16(bp - 6, P1);
    m2.regs[xenix::Reg::Di] = P1;
    m2.regs[xenix::Reg::Bp] = bp;
    xc::c2_hooks("RS").at(0xCE3)(m2, 0x1000, 0x2000);
    CB_CHECK_EQ(m2.reg(xenix::Reg::Ip), 0u);
});

CB_TEST("exact.xenix_c.a_variant_says_its_rules_patch_and_moves", [] {
    Fixture f;
    const xc::XenixC def = f.make();
    CB_CHECK_EQ(def.rules(), "UDMB");
    CB_CHECK(def.patch());
    CB_CHECK_EQ(def.c2move(), "RS");
    const xc::XenixC altos = f.make("altos");
    CB_CHECK_EQ(altos.rules(), "");
    CB_CHECK(!altos.patch());
    CB_CHECK_EQ(altos.c2move(), "");
    CB_CHECK_EQ(f.make("plain").rules(), "B");
    CB_CHECK(def.plugin_key() != altos.plugin_key());
    CB_CHECK_EQ(f.make("microsoft").plugin_key(), def.plugin_key());
    CB_CHECK_THROWS_AS(cb::UsageError, (void)f.make("nonesuch"));
});

CB_TEST("exact.xenix_c.a_compile_that_was_kept_is_not_run_again", [] {
    Fixture f;
    const xc::XenixC t = f.make("altos");
    const std::string source = "int f(a) int a; { return a; }\n";
    // the key's parts: the source with LF line ends, the headers by name, the include files, the toolchain's key
    std::vector<std::pair<std::string, Bytes>> parts = {{"tree.h", to_bytes("struct t { int a; };\n")}, {"\x01includes", Bytes{}}};
    f.cache->keep(tc::kCacheKind, cb::exact::plugin::compile_key(to_bytes(source), parts, "", t.plugin_key()), cb::exact::plugin::Built{to_bytes("A.OUT"), "log"});
    const auto got = t.compile(as_bytes(source), {.headers = {{"tree.h", to_bytes("struct t { int a; };\n")}}});   // (no kit: running it would throw)
    CB_CHECK(got.object.has_value() && *got.object == to_bytes("A.OUT"));
    CB_CHECK_EQ(got.console, "log");
    // CR LF in the source is the same source
    const auto crlf = t.compile(as_bytes(std::string("int f(a) int a; { return a; }\r\n")), {.headers = {{"tree.h", to_bytes("struct t { int a; };\n")}}});
    CB_CHECK(crlf.object.has_value());
    // another header is another compile, and the kit is not there
    CB_CHECK_THROWS_AS(cb::UsageError, (void)t.compile(as_bytes(source), {.headers = {{"other.h", to_bytes("x")}}}));
});

CB_TEST("exact.xenix_c.registered_as_a_plugin_by_the_name_the_profile_gives", [] {
    Fixture f;
    tc::Toolchains toolchains(f.locations, nullptr, f.cache.get());
    CB_CHECK_THROWS_AS(cb::UnsupportedError, (void)toolchains.load("xenixc"));
    xc::register_plugin(toolchains);
    const auto loaded = std::dynamic_pointer_cast<xc::XenixC>(toolchains.load("xenixc"));
    CB_CHECK(loaded != nullptr);
    CB_CHECK_EQ(loaded->rules(), "UDMB");   // the profile's default variant
});

// With the real kit: both variants compile a small C file, the microsoft one with its patches and rules.
CB_TEST("exact.xenix_c.the_real_compiler", [] {
    const auto kit = cfs::get_env("CB_KIT_XENIX86");
    if (!kit || !stdfs::is_directory(cfs::from_text(*kit))) return;   // (not set up: nothing to run)
    cfs::ScopedTempDir dir("cbxenixreal");
    tc::Locations locations;
    locations.profiles = cb::testing::data_path("../../data/exact/toolchains");
    locations.state = dir.path() / "state";
    locations.checkout_root = dir.path();
    const std::string source = "long widen(a, b)\nunsigned a;\nint b;\n{\n    b = b + 1;\n    return a;\n}\n";
    for (const char* variant : {"altos", "microsoft"}) {
        tc::Context c;
        c.locations = &locations;
        const xc::XenixC t(tc::read_profile(locations, "xenixc"), std::string(variant), c);
        const auto r = t.compile(as_bytes(source), {});
        CB_CHECK(r.object.has_value());
        if (!r.object) CB_CHECK_EQ(r.console, std::string(variant));
        if (r.object) CB_CHECK(r.object->size() > 32);   // (the assembler's object: a header and the code)
    }
    const xc::XenixC t(tc::read_profile(locations, "xenixc"), std::string("microsoft"), [&] {
        tc::Context c;
        c.locations = &locations;
        return c;
    }());
    const auto bad = t.compile(as_bytes(std::string("int f( { \n")), {});
    CB_CHECK(!bad.object.has_value());
    CB_CHECK(!bad.console.empty());
});
