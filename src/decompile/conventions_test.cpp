// cb-unity: alone (helpers named for what they do here)
// Tests of decompile/conventions.hpp: the service reads every compiler spec of the checkout, the facts it reads are the ones the
// decompiler's lanes assume, and the names round-trip.
#include "decompile/conventions.hpp"

#include <algorithm>
#include <set>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "core/text.hpp"

namespace cb::decompile::conventions {
namespace {

namespace stdfs = std::filesystem;
using std::string;

const Model& must(const Model* m, const char* what) {
    CB_CHECK(m != nullptr);
    if (!m) throw std::runtime_error(string("no model ") + what);
    return *m;
}

std::vector<string> reg_names(const Model& m) {
    std::vector<string> out;
    for (const RegisterArg& r : m.reg_args) out.push_back(r.reg);
    return out;
}

}  // namespace

CB_TEST("decompile.conventions.reads_every_cspec", [] {
    const Service& s = Service::get();
    std::size_t files = 0;
    for (const stdfs::directory_entry& e : stdfs::directory_iterator(s.folder()))
        if (e.path().extension() == ".cspec") {
            ++files;
            const string stem = fs::to_text(e.path().stem());
            CB_CHECK(!s.models_of(stem).empty());
        }
    CB_CHECK(files > 40);
    CB_CHECK_EQ(s.specs().size(), files);
    // every compiler of every language names a spec that is read (a spec the folder lacks is a fact about the checkout, not the
    // reader: x86.ldefs names x86-64-win.cspec and the like, which are not there yet)
    std::set<string> missing;
    for (const string& language : s.languages())
        for (const Compiler& c : s.compilers(language))
            if (s.models_of(c.spec).empty()) missing.insert(c.spec);
    CB_CHECK(missing.size() < 20);
    CB_CHECK(!s.compilers("x86:LE:16:Real Mode").empty());
});

CB_TEST("decompile.conventions.win16_models", [] {
    const Service& s = Service::get();
    const Model& far = must(s.model("x86:LE:16:Real Mode", "__stdcall16far"), "__stdcall16far");
    CB_CHECK_EQ(far.stack_args_offset.value_or(-1), 4);
    CB_CHECK_EQ(far.stack_shift, 4);
    CB_CHECK(!far.extra_pop.has_value());   // "unknown": the callee's own `ret N`
    CB_CHECK(far.far_call());
    CB_CHECK(far.stack_order == Order::LeftToRight);
    CB_CHECK_EQ(far.result_register(2), "AX");
    CB_CHECK_EQ(far.result_register(4), "DX:AX");
    CB_CHECK_EQ(far.result_register(8), "");
    CB_CHECK(!far.is_default);

    const Model& near = must(s.default_model("x86:LE:16:Real Mode"), "default");
    CB_CHECK_EQ(near.name, "__stdcall16near");   // the default prototype of the 16-bit specs
    CB_CHECK_EQ(near.stack_args_offset.value_or(-1), 2);
    CB_CHECK(!near.far_call());

    const Model& cdecl_far = must(s.model("x86:LE:16:Real Mode", "__cdecl16far"), "__cdecl16far");
    CB_CHECK_EQ(cdecl_far.extra_pop.value_or(-1), 4);
    CB_CHECK(cdecl_far.stack_order == Order::RightToLeft);

    const Model& fast = must(s.model("x86:LE:16:Real Mode", "__fastcall16near"), "__fastcall16near");
    CB_CHECK_EQ(reg_names(fast).size(), std::size_t{3});
    CB_CHECK_EQ(reg_names(fast)[0], "AX");
    CB_CHECK_EQ(reg_names(fast)[1], "DX");
    CB_CHECK_EQ(reg_names(fast)[2], "BX");
    CB_CHECK_EQ(fast.stack_args_offset.value_or(-1), 2);

    const Model& reg = must(s.model_of("x86-16", "__regcall"), "__regcall");
    CB_CHECK_EQ(reg_names(reg)[1], "BX");   // AX BX CX DX: not Watcom's order (AX DX BX CX)
    const Model& interrupt = must(s.model_of("x86-16", "__int16"), "__int16");
    CB_CHECK(std::find(interrupt.killed.begin(), interrupt.killed.end(), "CF") != interrupt.killed.end());
    CB_CHECK(std::find(far.unaffected.begin(), far.unaffected.end(), "BP") != far.unaffected.end());
});

CB_TEST("decompile.conventions.flat_x86_models", [] {
    const Service& s = Service::get();
    const Model& stdcall = must(s.default_model("x86:LE:32:default", "windows"), "default x86win");
    CB_CHECK_EQ(stdcall.name, "__stdcall");
    CB_CHECK(!stdcall.extra_pop.has_value());
    CB_CHECK_EQ(stdcall.stack_args_offset.value_or(-1), 4);
    CB_CHECK_EQ(stdcall.result_register(4), "EAX");
    CB_CHECK_EQ(stdcall.result_register(8), "EDX:EAX");
    CB_CHECK_EQ(stdcall.result_register(8, true), "ST0");
    const Model& thiscall = must(s.model("x86:LE:32:default", "__thiscall", "windows"), "__thiscall");
    CB_CHECK(thiscall.has_this);
    CB_CHECK(!thiscall.reg_args.empty() && thiscall.reg_args[0].reg == "ECX");
    const Model& c_model = must(s.model("x86:LE:32:default", "__cdecl", "windows"), "__cdecl");
    CB_CHECK_EQ(c_model.extra_pop.value_or(-1), 4);   // the caller pops the arguments; the core counts the return address
    CB_CHECK(c_model.far_call() == false);
    const Model& fastcall = must(s.model("x86:LE:32:default", "__fastcall", "windows"), "__fastcall");
    CB_CHECK(reg_names(fastcall).size() >= 2 && reg_names(fastcall)[0] == "ECX" && reg_names(fastcall)[1] == "EDX");
    for (const string& saved : {"EBX", "EBP", "ESI", "EDI"})
        CB_CHECK(std::find(stdcall.unaffected.begin(), stdcall.unaffected.end(), saved) != stdcall.unaffected.end());
});

CB_TEST("decompile.conventions.other_processors", [] {
    const Service& s = Service::get();
    const Model& ppc = must(s.model_of("ppc_32", "__stdcall"), "ppc_32 __stdcall");
    const std::vector<string> regs = reg_names(ppc);
    for (const char* wanted : {"r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "f1"})
        CB_CHECK(std::find(regs.begin(), regs.end(), wanted) != regs.end());
    CB_CHECK_EQ(ppc.result_register(4), "r3");
    CB_CHECK(!ppc.stack_args_offset.has_value() || ppc.stack_args_offset.value() >= 0);
    const Model& homed = must(s.model_of("ppc_64_32_homed", "__stdcall"), "ppc_64_32_homed");
    CB_CHECK(!reg_names(homed).empty());
    const Model& z80 = must(s.model_of("z80", "__asmA"), "z80 __asmA");
    CB_CHECK(!z80.reg_args.empty());
    CB_CHECK(!parse("__asmA").has_value());
});

CB_TEST("decompile.conventions.every_decompile_model_name_exists", [] {
    // every model name the frozen decompiler outputs carry resolves in the 16-bit or the 32-bit spec
    const Service& s = Service::get();
    const stdfs::path testdata = config::current().checkout_root() / "src" / "decompile" / "testdata";
    std::set<string> names;
    for (const stdfs::directory_entry& e : stdfs::recursive_directory_iterator(testdata)) {
        if (!e.is_regular_file()) continue;
        const string ext = fs::to_text(e.path().extension());
        if (ext != ".c" && ext != ".txt" && ext != ".jsonl" && ext != ".json" && ext != ".golden") continue;
        const string text = fs::read_text(e.path());
        for (std::size_t at = text.find("__"); at != string::npos; at = text.find("__", at + 2)) {
            std::size_t end = at + 2;
            while (end < text.size() && (std::isalnum(static_cast<unsigned char>(text[end])) || text[end] == '_')) ++end;
            const string word = text.substr(at, end - at);
            if (parse(word)) names.insert(word);
        }
    }
    CB_CHECK(names.size() >= 6);
    for (const string& name : names) {
        const bool known = s.model_of("x86-16", name) || s.model_of("x86win", name);
        if (!known) CB_CHECK_EQ(name, "(a model name the specs have)");
    }
});

CB_TEST("decompile.conventions.names_round_trip", [] {
    const Service& s = Service::get();
    std::size_t parsed = 0;
    for (const string& stem : s.specs())
        for (const Model& m : s.models_of(stem))
            if (const std::optional<Parsed> p = parse(m.name)) {
                ++parsed;
                CB_CHECK_EQ(name_of(p->convention, p->distance), m.name);
            }
    CB_CHECK(parsed > 20);
    CB_CHECK_EQ(name_of(Convention::Stdcall, Distance::Far), "__stdcall16far");
    CB_CHECK_EQ(name_of(Convention::Cdecl, Distance::Near), "__cdecl16near");
    CB_CHECK_EQ(name_of(Convention::Fastcall, Distance::Near), "__fastcall16near");
    CB_CHECK_EQ(name_of(Convention::Cdecl), "__cdecl");
    CB_CHECK_EQ(name_of(Convention::Thiscall, Distance::Far), "__thiscall");   // no near/far form
    CB_CHECK_EQ(name_of(Convention::Interrupt), "__int16");
    CB_CHECK_EQ(name_of(Convention::InterruptKeepDs), "__int16keepds");
    CB_CHECK(parse("__stdcall16far") == (Parsed{Convention::Stdcall, Distance::Far}));
    CB_CHECK(parse("__regcall") == (Parsed{Convention::Regcall, Distance::Flat}));
    CB_CHECK(!parse("__cdecl16").has_value());
    CB_CHECK(!parse("__thiscall16near").has_value());
    CB_CHECK(!parse("cdecl").has_value());
    CB_CHECK(!parse("").has_value());
});

CB_TEST("decompile.conventions.enum_mappings", [] {
    using L2 = translate::ir::CallingConvention;
    using E = symbols::wine16::Convention;
    CB_CHECK(to_l2("__stdcall16far") == L2::Pascal);
    CB_CHECK(to_l2("__stdcall16near") == L2::Pascal);
    CB_CHECK(to_l2("__cdecl16far") == L2::Cdecl);
    CB_CHECK(to_l2("__stdcall") == L2::Stdcall);
    CB_CHECK(to_l2("__cdecl") == L2::Cdecl);
    CB_CHECK(to_l2("__fastcall16near") == L2::Fastcall);
    CB_CHECK(to_l2("__fastcall") == L2::Fastcall);
    CB_CHECK(to_l2("__thiscall") == L2::Thiscall);
    CB_CHECK(to_l2("__regcall") == L2::Register);
    CB_CHECK(to_l2("__int16") == L2::Interrupt);
    CB_CHECK(to_l2("nonsense") == L2::Cdecl);
    CB_CHECK(to_export(Parsed{Convention::Stdcall, Distance::Far}) == E::Pascal);
    CB_CHECK(to_export(Parsed{Convention::Cdecl, Distance::Far}) == E::Cdecl);
    CB_CHECK(to_export(Parsed{Convention::Stdcall, Distance::Flat}) == E::Stdcall);
});

CB_TEST("decompile.conventions.extra_facts_live_beside_the_specs", [] {
    // The core refuses an element it does not know inside a <prototype> (so a fact the specs lack cannot be added there): this
    // holds that reason in place, and that no shipped spec carries a CodeBreaker element the core would choke on.
    const Service& s = Service::get();
    const stdfs::path core = config::current().checkout_root() / "src" / "decompile" / "engine" / "core" / "cpp" / "fspec.cc";
    if (stdfs::exists(core)) CB_CHECK(fs::read_text(core).find("Unknown element in prototype") != string::npos);
    for (const string& stem : s.specs()) {
        const string text = fs::read_text(s.folder() / (stem + ".cspec"));
        CB_CHECK(text.find("<cb_") == string::npos);
    }
    if (const Service::Extra* e = s.extra("__stdcall")) {
        CB_CHECK_EQ(e->decoration, "_%s@%N");
        const Service::Extra* pascal = s.extra("__stdcall16far");
        CB_CHECK(pascal != nullptr);
        if (pascal) CB_CHECK_EQ(pascal->compiler_keyword.at("msc6"), "_pascal");
    }
    CB_CHECK(s.extra("no such model") == nullptr);
});

}  // namespace cb::decompile::conventions
