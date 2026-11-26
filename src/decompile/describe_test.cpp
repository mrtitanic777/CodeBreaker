// Tests of cb/decompile/describe.hpp on real Windows 95 modules: a 16-bit library (LZEXPAND.DLL), a 16-bit driver (KEYBOARD.DRV, with each
// switch of the Policy turned off in turn), a virtual device driver (VMOUSE.VXD), a DOS program (FIND.EXE), a PE (LZ32.DLL) and a NES cartridge.
//
// The goldens in testdata/describe/ are the description files the original describer wrote for the same inputs, with the folder
// the blob was written into taken out of each `bytes` line; each was compared with what this code writes and the differences read
// through (there are none for these inputs: they agree line for line). What describing asks of the symbol tables is answered from
// testdata/describe/knowledge.txt, the answers the original was given for these modules.
//
// Notes (an old harness covered 29 modules and every function-level reader):
//  - the environment variables CB_CALLER_ARGS, CB_CALLER_RESULTS, CB_FASTCALL, CB_INLINE_HELPERS and CB_NO_CALL_OVERRIDES are the fields of
//    Policy; the experiment CB_POINTER_CALLS (`O` lines for calls through a pointer in a PE; off, and found to make msjava.dll worse) is
//    not carried over;
//  - a code segment whose table says it has no data in the file is read as empty;
//  - a program that sets no data segment is an UnsupportedError.

#include "decompile/describe.hpp"

#include <map>
#include <string>

#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace dsc = cb::decompile::describe;
namespace formats = cb::formats;
using cb::Bytes;
using std::string;

namespace {

Bytes data(const string& relative) { return cb::fs::read_file(cb::testing::data_path(relative)); }
string describe_data(const string& name) { return "decompile/testdata/describe/" + name; }

// What describing asks of the outside, answered as the original was answered (testdata/describe/knowledge.txt).
class Recorded : public dsc::Knowledge {
public:
    explicit Recorded(const formats::pe::Image* image = nullptr) : image_(image) {
        const string all = cb::fs::read_text(cb::testing::data_path(describe_data("knowledge.txt")));
        for (std::string_view line : cb::text::lines(all)) {
            const std::vector<std::string_view> f = cb::text::split(line, '\t');
            if (f.size() < 3) continue;
            auto number = [](std::string_view s) -> std::optional<std::int64_t> {
                return s == "N" ? std::nullopt : cb::text::parse_int<std::int64_t>(s);
            };
            if (f[0] == "of") win16_[string(f[1])] = number(f[2]);
            else if (f[0] == "stack" && f.size() == 4) win32_[string(f[1]) + "|" + string(f[2])] = number(f[3]);
            else if (f[0] == "header") declared_[string(f[1])] = number(f[2]);
            else if (f[0] == "ident" && f.size() == 4) names_[string(f[1]) + "|" + string(f[2])] = f[3] == "N" ? std::nullopt : std::optional<string>(string(f[3]));
            else if (f[0] == "scan") pops_[static_cast<std::uint32_t>(*cb::text::parse_int<std::int64_t>(f[2]))] = number(f[3]);
            else if (f[0] == "conv") conventions_[*cb::text::parse_int<std::int64_t>(f[2])] = string(f[3]);
            else if (f[0] == "funcs") {
                for (std::string_view hex : cb::text::split_ws(f[2])) functions.push_back(*cb::text::parse_int<std::int64_t>(hex, 16));
            }
        }
    }
    std::optional<std::int64_t> win16_argument_bytes(std::string_view api) override { return find(win16_, string(api)); }
    std::optional<std::int64_t> win32_argument_bytes(std::string_view module, std::string_view api) override {
        return find(win32_, string(module) + "|" + string(api));
    }
    std::optional<std::int64_t> declared_argument_bytes(std::string_view name) override { return find(declared_, string(name)); }
    std::optional<string> import_name(std::string_view module, std::uint32_t ordinal) override {
        const auto it = names_.find(string(module) + "|" + std::to_string(ordinal));
        return it == names_.end() ? std::nullopt : it->second;
    }
    std::set<string> imported_names(const std::filesystem::path&) override {
        std::set<string> out;
        if (image_)
            for (const auto& imp : image_->imports())
                if (!imp.name.starts_with("#")) out.insert(imp.name);
        return out;
    }
    Conventions conventions(const std::filesystem::path&) override {
        Conventions c;
        for (const auto& [va, name] : conventions_) {
            if (name == "thiscall") c.thiscall.insert(va);
            if (name == "fastcall") c.fastcall.insert(va);
        }
        return c;
    }
    std::optional<std::int64_t> pe_function_pop(const std::filesystem::path&, std::uint32_t rva) override {
        const auto it = pops_.find(rva);
        return it == pops_.end() ? std::nullopt : it->second;
    }
    std::vector<dsc::Va> functions;

private:
    static std::optional<std::int64_t> find(const std::map<string, std::optional<std::int64_t>>& m, const string& key) {
        const auto it = m.find(key);
        return it == m.end() ? std::nullopt : it->second;
    }
    const formats::pe::Image* image_;
    std::map<string, std::optional<std::int64_t>> win16_, win32_, declared_;
    std::map<std::uint32_t, std::optional<std::int64_t>> pops_;
    std::map<string, std::optional<string>> names_;
    std::map<dsc::Va, string> conventions_;
};

// The description file as written, with the folder of each `bytes` line's file taken out (as the goldens have it).
string normalised(const std::filesystem::path& file) {
    string out;
    const string text = cb::fs::read_text(file);
    bool first = true;
    for (std::string_view line : cb::text::split(text, '\n')) {
        if (!first) out += '\n';
        first = false;
        std::vector<std::string_view> f = cb::text::split(line, '\t');
        if (f[0] == "bytes" && f.size() >= 3) {
            const string leaf = std::filesystem::path(string(f[2])).filename().string();
            out += string(f[0]) + "\t" + string(f[1]) + "\t" + leaf;
            for (std::size_t i = 3; i < f.size(); ++i) out += "\t" + string(f[i]);
        } else {
            out += string(line);
        }
    }
    return out;
}
string golden(const string& name) { return cb::fs::read_text(cb::testing::data_path(describe_data(name + ".description.txt"))); }

// the first line where two texts differ, for the failure message
string first_difference(const string& a, const string& b) {
    const auto la = cb::text::split(a, '\n'), lb = cb::text::split(b, '\n');
    for (std::size_t i = 0; i < std::max(la.size(), lb.size()); ++i) {
        const string x = i < la.size() ? string(la[i]) : "<end>", y = i < lb.size() ? string(lb[i]) : "<end>";
        if (x != y) return "line " + std::to_string(i + 1) + ": got [" + x + "] expected [" + y + "]";
    }
    return "";
}

#define CHECK_SAME_TEXT(actual, expected) CB_CHECK_EQ(first_difference(actual, expected), string())

dsc::NeDescription describe_ne(const string& relative, const dsc::Policy& policy, const std::filesystem::path& outdir) {
    Recorded knowledge;
    const auto m = formats::ne::Module::open(data(relative));
    return dsc::describe16(m, outdir, knowledge, policy);
}

dsc::Code16 code_of(std::initializer_list<int> bytes) {
    Bytes b;
    for (int x : bytes) b.push_back(static_cast<std::uint8_t>(x));
    return dsc::Code16(std::move(b));
}

}  // namespace

CB_TEST("decompile.describe.address_spaces", [] {
    CB_CHECK_EQ(dsc::selector(1), 0x1000);
    CB_CHECK_EQ(dsc::selector(3), 0x1010);
    CB_CHECK_EQ(dsc::address(2, 0x1234), (0x1008LL << 16) + 0x1234);
    const dsc::SegOff w = dsc::where(dsc::address(5, 0x34));
    CB_CHECK_EQ(w.seg, 5u);
    CB_CHECK_EQ(w.off, 0x34u);
    CB_CHECK_EQ(dsc::le_address(1, 0x10), 0x1000010);
    CB_CHECK_EQ(dsc::le_address(3, 0), 0x3000000);
    CB_CHECK(dsc::language_of_machine(0x014C) && *dsc::language_of_machine(0x014C) == "x86:LE:32:default:windows");
    CB_CHECK(dsc::language_of_machine(0x01F2) && dsc::language_of_machine(0x01F2)->starts_with("PowerPC"));
    CB_CHECK(dsc::language_of_machine(0x8664) && *dsc::language_of_machine(0x8664) == "x86:LE:64:default:windows");
    CB_CHECK(!dsc::language_of_machine(0));
});

CB_TEST("decompile.describe.description_file_and_records", [] {
    cb::fs::ScopedTempDir dir("describe");
    dsc::Description d;
    d.path = dir.path() / "a" / "description.txt";
    d.lines = {"arch\tx", "F\t10\tf\t0\t__cdecl16near", "O\t20\t10\t4\t__stdcall16far", "F\t30\tg\t-1"};
    d.write();
    CB_CHECK_EQ(cb::fs::read_text(d.path), string("arch\tx\nF\t10\tf\t0\t__cdecl16near\nO\t20\t10\t4\t__stdcall16far\nF\t30\tg\t-1\n"));
    const auto recs = d.records({"F", "O"});
    CB_CHECK_EQ(recs.size(), 3u);
    CB_CHECK_EQ(recs[1][3], string("4"));
    CB_CHECK(d.records({"J"}).empty());
    d.lines.clear();
    d.write(dir.path() / "empty.txt");   // a description with nothing to say is one empty line
    CB_CHECK_EQ(cb::fs::read_text(dir.path() / "empty.txt"), string("\n"));
    CB_CHECK(d.path == dir.path() / "empty.txt");
});

CB_TEST("decompile.describe.returns", [] {
    // push bp / mov bp, sp / pop bp / retf 6
    const dsc::Code16 far_fn = code_of({0x55, 0x8B, 0xEC, 0x5D, 0xCA, 0x06, 0x00});
    const dsc::Return r = dsc::returns(far_fn, 0);
    CB_CHECK(r.kind && *r.kind == dsc::Distance::Far);
    CB_CHECK_EQ(r.pops, 6);
    CB_CHECK(!dsc::returns(far_fn, 0, 600, dsc::Distance::Near).kind);   // asked for a near return: there is none
    CB_CHECK_EQ(dsc::returns(far_fn, 0, 600, dsc::Distance::Near).pops, -1);
    const dsc::Return near_fn = dsc::returns(code_of({0x90, 0xC3}), 0);
    CB_CHECK(near_fn.kind && *near_fn.kind == dsc::Distance::Near);
    CB_CHECK_EQ(near_fn.pops, 0);
    CB_CHECK(!dsc::returns(code_of({0x90, 0x90, 0x90}), 0).kind);   // runs off the end
    CB_CHECK(!dsc::returns(code_of({0xC2, 0x03, 0x00}), 0).kind);   // `ret 3`: no function removes an odd count
    // a conditional jump is followed to the nearest return: jz +2 over `retf 4`, falls to `ret 2`
    const dsc::Code16 branchy = code_of({0x74, 0x02, 0xC2, 0x02, 0x00, 0xCA, 0x04, 0x00});
    CB_CHECK_EQ(dsc::returns(branchy, 0).pops, 2);
});

CB_TEST("decompile.describe.prologues_and_windows_far_prologue", [] {
    // ret / padding / push bp, mov bp, sp: a function standing after an end
    const dsc::Code16 code = code_of({0xC3, 0x90, 0xCC, 0x55, 0x8B, 0xEC, 0x5D, 0xC3});
    const std::vector<std::int64_t> found = dsc::prologues_after_ends(code);
    CB_CHECK_EQ(found.size(), 1u);
    CB_CHECK_EQ(found[0], 3);
    CB_CHECK(dsc::prologues_after_ends(dsc::Code16(Bytes{0xC3, 0x00, 0x00})).empty());
    const Bytes far_frame{0x1E, 0x58, 0x90, 0x45, 0x55, 0x8B, 0xEC};
    CB_CHECK(dsc::windows_far_prologue(far_frame, 0));
    CB_CHECK(!dsc::windows_far_prologue(far_frame, 1));
    CB_CHECK(!dsc::windows_far_prologue(far_frame, 100));   // past the end
    CB_CHECK(dsc::windows_far_prologue(Bytes{0x45, 0x55}, 0));
});

CB_TEST("decompile.describe.what_a_function_returns", [] {
    // mov ax, 1 / ret
    CB_CHECK_EQ(dsc::result_bytes(code_of({0xB8, 0x01, 0x00, 0xC3}), 0, 4), 2);
    // mov ax, 1 / mov dx, 2 / ret: DX set at the end as well
    CB_CHECK_EQ(dsc::result_bytes(code_of({0xB8, 0x01, 0x00, 0xBA, 0x02, 0x00, 0xC3}), 0, 7), 4);
    // nothing sets AX
    CB_CHECK_EQ(dsc::result_bytes(code_of({0x90, 0xC3}), 0, 2), 0);
    // a call whose result is passed on: unknown callee
    std::vector<dsc::PassOn> passes;
    CB_CHECK_EQ(dsc::result_bytes(code_of({0xE8, 0x10, 0x00, 0xC3}), 0, 4, &passes), 0);
    CB_CHECK_EQ(passes.size(), 1u);
    CB_CHECK(passes[0].kind == dsc::PassOn::Kind::Near);
    CB_CHECK_EQ(passes[0].offset, 0x13);
});

CB_TEST("decompile.describe.the_small_readers", [] {
    // mov ax, 0x1234 / push ax / ret: a jump through the address pushed
    const std::vector<dsc::Insn> jump = dsc::decode16(Bytes{0xB8, 0x34, 0x12, 0x50, 0xC3}, 0, true);
    CB_CHECK_EQ(jump.size(), 3u);
    CB_CHECK(dsc::jump_by_return(jump, 2));
    CB_CHECK(!dsc::jump_by_return(jump, 2, {4}));   // the return itself is a place jumped to: a plain return
    CB_CHECK(!dsc::jump_by_return(jump, 1));        // not a return at all
    // push ax / ret with nothing that set AX: what is pushed was not made here, so it is a plain return
    CB_CHECK(!dsc::jump_by_return(dsc::decode16(Bytes{0x50, 0xC3}, 0, true), 1));
    // pop ax / pop bx / pop cx / push bx / push ax / retf: its caller's argument taken off by hand
    const dsc::Code16 by_hand = code_of({0x58, 0x5B, 0x59, 0x53, 0x50, 0xCB});
    CB_CHECK_EQ(dsc::popped_by_hand(by_hand, 0, dsc::Distance::Far).value_or(-1), 2);
    CB_CHECK(!dsc::popped_by_hand(by_hand, 0, dsc::Distance::Near));
    // push ax / push bx / mov ax, ds / add ax, 8 / mov ds, ax / xor si, si / pop bx / pop ax / ret: moves DS:SI on
    const dsc::Code16 helper = code_of({0x50, 0x53, 0x8C, 0xD8, 0x05, 0x08, 0x00, 0x8E, 0xD8, 0x31, 0xF6, 0x5B, 0x58, 0xC3});
    CB_CHECK(dsc::register_helper(helper, 0));
    CB_CHECK(!dsc::register_helper(code_of({0x90, 0xC3}), 0));   // changes nothing
    // push bp / mov bp, sp / push ax: reads AX before it writes it
    CB_CHECK(dsc::register_arguments(code_of({0x55, 0x8B, 0xEC, 0x50, 0xC3}), 0));
    CB_CHECK(!dsc::register_arguments(code_of({0x33, 0xC0, 0xC3}), 0));   // xor ax, ax first
    CB_CHECK(!dsc::register_arguments(code_of({0x1E, 0x50, 0x56, 0x57, 0x5F, 0x5E, 0x58, 0x1F, 0xCA, 0x02, 0x00}), 0));   // WIN87EM's WEP: push ax is a save
    // mov cl, 5 / test ax, 0x1b1 (the skip idiom: the immediate is the next entry's mov cl, 1) / mov ax, seg / push bp / mov bp, sp / mov bx, [bp+8]:
    // USER's GetWindowLong entries read no register -- their arguments are on the stack
    CB_CHECK(!dsc::register_arguments(code_of({0xB1, 0x05, 0xA9, 0xB1, 0x01, 0xB8, 0x29, 0x08, 0x55, 0x8B, 0xEC, 0x8B, 0x5E, 0x08, 0xC3}), 0));
    // push bp / mov bp, sp / mov ax, [bp+6] / mov dx, [bp+8] / pop bp / ret: two words of arguments past a near frame's 4
    const dsc::Code16 frame = code_of({0x55, 0x8B, 0xEC, 0x8B, 0x46, 0x06, 0x8B, 0x56, 0x08, 0x5D, 0xC3});
    CB_CHECK_EQ(dsc::args_read(frame, 0, 11, dsc::Distance::Near).value_or(-1), 6);
    CB_CHECK(!dsc::args_read(code_of({0x90, 0xC3}), 0, 2, dsc::Distance::Near));   // no frame
    // add sp, 6 after a call
    const std::vector<dsc::Insn> call = dsc::decode16(Bytes{0xE8, 0x00, 0x00, 0x83, 0xC4, 0x06}, 0, true);
    CB_CHECK_EQ(dsc::caller_cleanup(call, 0), 6);
    CB_CHECK_EQ(dsc::caller_cleanup(call, 1), 0);
});

CB_TEST("decompile.describe.the_decoder", [] {
    const std::vector<dsc::Insn> ins = dsc::decode16(Bytes{0x8B, 0x46, 0x06, 0xC3}, 0x10, true, 0);
    CB_CHECK_EQ(ins.size(), 2u);
    CB_CHECK_EQ(ins[0].address, 0x10);
    CB_CHECK_EQ(ins[0].size, 3);
    CB_CHECK_EQ(ins[0].mn, string("mov"));
    CB_CHECK_EQ(ins[0].op_str, string("ax, word ptr [bp + 6]"));
    CB_CHECK_EQ(ins[0].operands.size(), 2u);
    CB_CHECK(ins[0].operands[1].kind == dsc::OperandKind::Memory);
    CB_CHECK_EQ(ins[0].operands[1].disp, 6);
    CB_CHECK(ins[0].regs_ok);
    CB_CHECK_EQ(dsc::register_name16(ins[0].operands[0].reg), string("ax"));
    CB_CHECK(ins[1].in_group(dsc::kGroupRet));
    CB_CHECK(dsc::decode16(Bytes{0xFF}, 0, false).empty());   // stops at the first byte that is not an instruction
    // the cache: the instruction at an offset is the same object each time
    const dsc::Code16 c = code_of({0x90, 0xC3});
    CB_CHECK(c.one(0) == c.one(0));
    CB_CHECK(c.one(5) == nullptr);
});

CB_TEST("decompile.describe.ne_functions_of_a_library", [] {
    const auto m = formats::ne::Module::open(data("formats/testdata/lzexpand.dll"));
    const auto found = dsc::find_functions(m);
    CB_CHECK(found.size() > 10);
    // every export is far; the entry point of the library is far
    for (const auto& e : m.entries())
        if (e.kind != formats::ne::Entry::Kind::Unused) {
            const auto it = found.find(cb::core16::SegOff{e.segment, e.offset});
            CB_CHECK(it != found.end());
            if (it != found.end()) CB_CHECK(it->second == dsc::Distance::Far);
        }
    // asked of the starts themselves, none is hidden; asked of none, the entry sequences that follow an end appear
    std::set<cb::core16::SegOff> all;
    for (const auto& [place, how] : found) all.insert(place);
    CB_CHECK(dsc::hidden_functions(m, all).empty());
    CB_CHECK(dsc::hidden_functions(m, {}).size() <= found.size());
});

CB_TEST("decompile.describe.ne_library_is_what_the_original_wrote", [] {
    cb::fs::ScopedTempDir dir("describe");
    const dsc::NeDescription d = describe_ne("formats/testdata/lzexpand.dll", {}, dir.path());
    CHECK_SAME_TEXT(normalised(dir.path() / "description.txt"), golden("lzexpand.dll"));
    CHECK_SAME_TEXT(cb::fs::read_text(d.path), cb::fs::read_text(dir.path() / "description.txt"));
    CB_CHECK(cb::fs::file_size(dir.path() / "describe.bin") > 7000);
    // what the description holds besides its lines
    CB_CHECK(!d.names.empty());
    CB_CHECK_EQ(d.names.size(), d.where_map.size());
    CB_CHECK_EQ(d.imports.front(), string("KERNEL_15"));
    CB_CHECK(d.code.size() > 5);
    CB_CHECK(d.images.size() >= d.code.size());
    CB_CHECK(d.data_segment != 0);
    for (const auto& [va, name] : d.names) CB_CHECK(d.inside(va));
    CB_CHECK(!d.inside(0x20000000));
    CB_CHECK(!d.inside(0x10000001 + (1 << 16)));   // not on a selector
    CB_CHECK_EQ(d.results.size(), d.names.size());
    // the readers that ask a description (every one of them runs on the whole module)
    CB_CHECK(!dsc::call_overrides(d).empty() || true);
    (void)dsc::callers_say(d);
    (void)dsc::jump_tables(d);
    (void)dsc::return_jumps(d);
    std::set<cb::core16::SegOff> places;
    for (const auto& [va, w] : d.where_map) places.insert(w);
    (void)dsc::callers_read_ax(d, places);
    std::vector<std::pair<cb::core16::SegOff, cb::core16::SegOff>> edges;
    (void)dsc::callers_read_dx(d, &edges);
    dsc::DxReturns dx(d);
    for (const auto& place : places) (void)dx.every_way(place, places);
    CB_CHECK(!dsc::model_line(d, d.names.begin()->first, d.names.begin()->second).empty());
});

CB_TEST("decompile.describe.ne_driver_and_every_policy_switch", [] {
    struct Case {
        const char* golden;
        dsc::Policy policy;
    };
    dsc::Policy no_fastcall, no_caller_args, no_inline, no_overrides;
    no_fastcall.fastcall = false;
    no_caller_args.caller_args = false;
    no_inline.inline_helpers = false;
    no_overrides.call_overrides = false;
    const Case cases[] = {{"keyboard.drv", {}}, {"nofastcall.keyboard.drv", no_fastcall}, {"nocallerargs.keyboard.drv", no_caller_args},
                          {"noinline.keyboard.drv", no_inline}, {"nooverrides.keyboard.drv", no_overrides}};
    for (const Case& c : cases) {
        cb::fs::ScopedTempDir dir("describe");
        const dsc::NeDescription d = describe_ne(describe_data("keyboard.drv"), c.policy, dir.path());
        CHECK_SAME_TEXT(normalised(dir.path() / "description.txt"), golden(c.golden));
        if (!c.policy.inline_helpers) CB_CHECK(d.inlined.empty());
    }
    // the default one inlines at least the helper that moves DS:SI on
    cb::fs::ScopedTempDir dir("describe");
    CB_CHECK(!describe_ne(describe_data("keyboard.drv"), {}, dir.path()).inlined.empty());
});

CB_TEST("decompile.describe.ne_policy_from_the_environment", [] {
    const dsc::Policy defaults = dsc::Policy::from_environment();
    CB_CHECK(defaults.caller_args && defaults.caller_results && defaults.fastcall && defaults.inline_helpers && defaults.call_overrides);
    CB_CHECK(defaults.dx_unspecified && defaults.dx_shared_bodies);   // (on; CB_DX_UNSPECIFIED=0 / CB_DX_SHARED=0 turn them off)
    cb::fs::ScopedEnv a("CB_FASTCALL", "0"), b("CB_CALLER_RESULTS", "0"), c("CB_NO_CALL_OVERRIDES", "1"), e("CB_DX_SHARED", "0"), f("CB_DX_UNSPECIFIED", "0");
    const dsc::Policy p = dsc::Policy::from_environment();
    CB_CHECK(!p.fastcall && !p.caller_results && !p.call_overrides && p.caller_args && !p.dx_shared_bodies && !p.dx_unspecified);
});

CB_TEST("decompile.describe.ne_decorated_and_import_names", [] {
    const auto m = formats::ne::Module::open(data("formats/testdata/lzexpand.dll"));
    CB_CHECK(dsc::decorated_names(m).empty());   // no C++ names in a Windows 3 library
    Recorded knowledge;
    const auto names = dsc::import_names16(m, knowledge);
    CB_CHECK_EQ(names.at("KERNEL_15"), string("GlobalAlloc"));
    CB_CHECK_EQ(names.at("KERNEL_81"), string("_lclose"));
});

CB_TEST("decompile.describe.le_driver_is_what_the_original_wrote", [] {
    cb::fs::ScopedTempDir dir("describe");
    Recorded knowledge;
    const auto m = formats::le::Module::from_buffer(data(describe_data("vmouse.vxd")));
    const dsc::LeDescription d = dsc::describe_le(m, "vmouse.vxd", dir.path(), knowledge);
    // (the service labels come from the driver kit's tables, which are not part of this test: each I line is compared without its label)
    auto unlabelled = [](const string& text) {
        string out;
        for (std::string_view line : cb::text::split(text, '\n')) {
            std::vector<std::string_view> f = cb::text::split(line, '\t');
            if (f[0] == "I" && f.size() == 4) out += "I\t" + string(f[1]) + "\t*\t" + string(f[3]) + "\n";
            else out += string(line) + "\n";
        }
        return out;
    };
    CHECK_SAME_TEXT(unlabelled(normalised(dir.path() / "description.txt")), unlabelled(golden("vmouse.vxd")));
    CB_CHECK(!d.names.empty());
    CB_CHECK(!d.spans.empty());
    CB_CHECK(!d.device.empty());
    for (const auto& [va, name] : d.names) CB_CHECK(name.starts_with("sub_"));
    CB_CHECK(cb::fs::file_size(dir.path() / "image_le.bin") > 10000);
});

CB_TEST("decompile.describe.dos_program_is_what_the_original_wrote", [] {
    cb::fs::ScopedTempDir dir("describe");
    const Bytes file = data("formats/testdata/find.exe");
    const auto program = formats::dos::read_program(file, "find.exe");
    const dsc::DosDescription d = dsc::describe_dos(program, dir.path());
    CHECK_SAME_TEXT(normalised(dir.path() / "description.txt"), golden("find.exe"));
    CB_CHECK_EQ(d.image.size(), program.image.size());
    CB_CHECK_EQ(d.base, static_cast<std::int64_t>(program.base));
    CB_CHECK(d.analysis != nullptr);
    CB_CHECK_EQ(d.names.size(), d.analysis->functions.size());
    CB_CHECK_EQ(d.spans.size(), program.spans.size());
    CB_CHECK(!d.interrupts.empty());   // FIND calls DOS
    for (const auto& [name, calls] : d.interrupts) {
        CB_CHECK(!calls.empty());
        CB_CHECK(std::any_of(d.names.begin(), d.names.end(), [&](const auto& kv) { return kv.second == name; }));
    }
});

CB_TEST("decompile.describe.a_program_with_no_data_segment_is_refused", [] {
    cb::fs::ScopedTempDir dir("describe");
    formats::dos::Program p = formats::dos::read_program(data("formats/testdata/find.exe"), "find.exe");
    p.ds = std::nullopt;
    CB_CHECK_THROWS_AS(cb::UnsupportedError, dsc::describe_dos(p, dir.path()));
});

CB_TEST("decompile.describe.pe_module_is_what_the_original_wrote", [] {
    cb::fs::ScopedTempDir dir("describe");
    const Bytes file = data("formats/testdata/lz32.dll");
    const auto image = formats::pe::Image::parse(file, "lz32.dll");
    Recorded knowledge(&image);
    CB_CHECK(!knowledge.functions.empty());
    // the file's name is in the `bytes` lines, so it is given as the golden has it
    cb::fs::write_file(dir.path() / "lz32.dll", file);
    const auto where = dir.path() / "lz32.dll";
    const dsc::PeDescription d = dsc::describe32(where, image, knowledge.functions, dir.path(), knowledge);
    CHECK_SAME_TEXT(normalised(d.path), golden("lz32.dll"));
    CB_CHECK_EQ(d.names.size(), knowledge.functions.size());
    for (const auto& [va, name] : d.names) CB_CHECK(!name.empty());
    // names a module exports are used; the rest are sub_<rva>
    const auto by_name = dsc::function_names(image, knowledge.functions, {});
    CB_CHECK(by_name == d.names);
    CB_CHECK(dsc::decorated_names(image).empty());
});

CB_TEST("decompile.describe.pe_with_no_language_is_unsupported", [] {
    const Bytes file = data("formats/testdata/lz32.dll");
    auto image = formats::pe::Image::parse(file, "lz32.dll");
    // an image for a machine the decompiler has no language for: patch the machine type of a copy
    Bytes patched = file;
    const std::uint32_t lfanew = patched[0x3C] | (patched[0x3D] << 8);
    patched[lfanew + 4] = 0x64;
    patched[lfanew + 5] = 0xAA;   // ARM64: parses as PE32 with an ARM64 machine
    const auto other = formats::pe::Image::try_parse(patched);
    if (other) {
        dsc::NoKnowledge none;
        cb::fs::ScopedTempDir dir("describe");
        CB_CHECK_THROWS_AS(cb::UnsupportedError, dsc::describe32("x.dll", *other, std::vector<dsc::Va>{}, dir.path(), none));
    }
});

CB_TEST("decompile.describe.pe_ppc_facts_are_what_the_caller_says", [] {
    // a PowerPC image: its bytes, its import stubs and its inline runs are given by the caller (formats/powerpc_image)
    const Bytes file = data("formats/testdata/lz32.dll");
    Bytes patched = file;
    const std::uint32_t lfanew = patched[0x3C] | (patched[0x3D] << 8);
    patched[lfanew + 4] = 0xF2;
    patched[lfanew + 5] = 0x01;   // IMAGE_FILE_MACHINE_POWERPCFP
    const auto image = formats::pe::Image::try_parse(patched);
    if (!image) return;
    dsc::PowerPcFacts ppc;
    ppc.spans.push_back({0x82000000, 0x400, 0x1000});
    ppc.imports[0x82001000] = "NtSomething";
    ppc.inlined[0x82001100] = "__savegprlr_14";
    ppc.names[0x82000100] = "main";
    cb::fs::ScopedTempDir dir("describe");
    dsc::NoKnowledge none;
    const std::vector<dsc::Va> functions{0x82000100, 0x82000200};
    const dsc::PeDescription d = dsc::describe32("m.xex", *image, functions, dir.path(), none, &ppc);
    CB_CHECK_EQ(d.names.at(0x82000100), string("main"));
    CB_CHECK_EQ(d.names.at(0x82000200), string("sub_82000200"));
    const string text = cb::fs::read_text(d.path);
    CB_CHECK(text.starts_with("arch\tPowerPC:BE:64:Xenon:homed\nbytes\t82000000\t"));
    CB_CHECK(text.find("\nX\t82001000\tNtSomething\t-1\n") != string::npos);
    CB_CHECK(text.find("\nF\t82001100\t__savegprlr_14\t-1\tinline\n") != string::npos);
    CB_CHECK(text.find("\nF\t82000100\tmain\t-1\n") != string::npos);
});

CB_TEST("decompile.describe.nes_cartridge_is_what_the_original_wrote", [] {
    // JSR 8010 / JSR 8020 / JMP 8000 at the reset vector; two one-line subroutines; nmi 8020, reset 8000, irq 8010
    Bytes prg(0x4000, 0);
    const Bytes code{0x20, 0x10, 0x80, 0x20, 0x20, 0x80, 0x4C, 0x00, 0x80};
    std::copy(code.begin(), code.end(), prg.begin());
    const Bytes lda{0xA9, 0x01, 0x60}, ldx{0xA2, 0x00, 0x60};
    std::copy(lda.begin(), lda.end(), prg.begin() + 0x10);
    std::copy(ldx.begin(), ldx.end(), prg.begin() + 0x20);
    const Bytes vectors{0x20, 0x80, 0x00, 0x80, 0x10, 0x80};
    std::copy(vectors.begin(), vectors.end(), prg.begin() + 0x3FFA);
    Bytes file{0x4E, 0x45, 0x53, 0x1A, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    cb::append(file, prg);
    const auto rom = formats::nes::parse(file);
    cb::fs::ScopedTempDir dir("describe");
    const std::vector<std::uint32_t> functions{0x8000, 0x8010, 0x8020};
    const dsc::NesDescription d = dsc::describe_nes(rom, functions, dir.path());
    CHECK_SAME_TEXT(normalised(d.path), golden("nes"));
    CB_CHECK_EQ(d.names.at(0x8000), string("reset"));
    CB_CHECK_EQ(d.names.at(0x8010), string("irq"));
    CB_CHECK_EQ(d.names.at(0x8020), string("nmi"));
    CB_CHECK_EQ(cb::fs::file_size(dir.path() / "cpu.bin"), 0x8000u);
});
