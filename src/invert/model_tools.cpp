// cb-unity: alone
#include "invert/model_tools.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <optional>
#include <nlohmann/json.hpp>
#include <set>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"
#include "core/log.hpp"
#include "core/regex.hpp"
#include "core/text.hpp"
#include "formats/omf.hpp"
#include "invert/early_c.hpp"
#include "invert/early_c_frame.hpp"
#include "invert/msc6.hpp"
#include "invert/msc6_small.hpp"
#include "invert/msvc2.hpp"
#include "invert/msvc2_battery.hpp"
#include "platforms/decoder.hpp"
#include "symbols/libraries/coff.hpp"

namespace cb::invert::tools {

namespace {

namespace stdfs = std::filesystem;
using command::Environment;

constexpr std::string_view kMsc6Flags = "/AM /G2 /Gsw /Ox";

Bytes bytes_of(std::string_view hex) {
    Bytes out;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) out.push_back(*text::parse_int<std::uint8_t>(hex.substr(i, 2), 16));
    return out;
}

std::string hex_of(ByteView b, char separator = 0) {
    std::string out;
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (i && separator) out += separator;
        out += std::format("{:02x}", b[i]);
    }
    return out;
}

const char* py_bool(bool b) { return b ? "True" : "False"; }

// The relocated operands zeroed, and trailing padding (nops and zeros) cut: what a standalone recompile is compared under.
Bytes masked(ByteView code, bool trim_padding) {
    Bytes b = msc6_small::mask(code);
    if (trim_padding)
        while (!b.empty() && (b.back() == 0x90 || b.back() == 0x00)) b.pop_back();
    return b;
}

// The text through Microsoft C 6. Said and nothing, where it is not set up; nothing for a text it refused.
std::optional<std::optional<Bytes>> through_msc6(const Environment& environment, std::string_view text, std::string_view flags) {
    if (!environment.compile_msc6) {
        err("Microsoft C 6 is not set up (settings.json: msc6): the text was not put to the compiler\n");
        return std::nullopt;
    }
    return environment.compile_msc6(text, flags);
}

}  // namespace

// ---- msc6 ----------------------------------------------------------------------------------------------------------------------------

namespace {

struct FarCase {
    const char* label;
    const char* hex;
    const char* return_type;
    std::vector<msc6::Param> params;
};

struct RealCase {
    const char* label;
    const char* hex;
    std::map<std::uint32_t, std::string> imports;
    msc6_small::Globals globals;
};

}  // namespace

int msc6(const Environment& environment) {
    using msc6::Kind;
    const Bytes calls = bytes_of("1e589045558bec1e8ed883ec0256ff76069a000000008bf00bf675042bc0eb08569a000000008bc65e4d4d8be51f5d4dca020090");
    msc6::Context context;
    context.imports = {{0, "G"}, {1, "H"}};
    const Result<msc6::Reading> first = msc6::invert(calls, context);
    if (!first.ok()) {
        out("DECLINED: {}\n", first.error().what());
        return 0;
    }
    out("{}\n", first.value().text);
    const auto built = through_msc6(environment, first.value().text, kMsc6Flags);
    if (!built) return 2;
    if (!*built) {
        out("\n// the compiler gave no object\n");
        return 1;
    }
    out("\n// INTEGRATION driver: masked-equal = {} ({} vs {} B)\n", py_bool(masked(**built, false) == masked(calls, false) && (*built)->size() == calls.size()), (*built)->size(),
        calls.size());

    // far-pointer battery: branch, deref load / pass / store (pointee width from usage)
    const std::vector<FarCase> far{
        {"fptr-branch", "1e589045558bec1e8ed883ec028b4e068b46080bc175052bc0eb0b908b460850519a000000004d4d8be51f5d4dca0400", "int", {{"name", Kind::Far}}},
        {"deref-return", "1e589045558bec1e8ed883ec02c45e06268b074d4d8be51f5d4dca040090", "int", {{"p", Kind::Far}}},
        {"deref-pass", "1e589045558bec1e8ed883ec02c45e0626ff379a000000004d4d8be51f5d4dca0400", "int", {{"p", Kind::Far}}},
        {"deref-store", "1e589045558bec1e8ed883ec028b4606c45e082689074d4d8be51f5d4dca0600", "void", {{"p", Kind::Far}, {"v", Kind::Int}}},
        {"dword-shl4", "1e589045558bec1e8ed883ec028b46068b5608d1e0d1d2d1e0d1d2d1e0d1d2d1e0d1d24d4d8be51f5d4dca040090", "long", {{"a", Kind::Long}}},
        {"dword-add", "1e589045558bec1e8ed883ec028b46068b560803460a13560c4d4d8be51f5d4dca080090", "long", {{"a", Kind::Long}, {"b", Kind::Long}}},
        {"dword-frword", "1e589045558bec1e8ed883ec028b56062bc04d4d8be51f5d4dca0200", "long", {{"a", Kind::Int}}},
        // short-circuit OR-of-ANDs converging on a JOIN: (f==1&&g==0)||(f==2&&g==0)||(f==3&&g==0)
        {"shortcircuit-or-and",
         "1e589045558bec1e8ed883ec028b4e088b560683f90175040bd2741283f90275040bd2740983f903750c0bd27508519a00000000eb03b80d004d4d8be51f5d4dca040090", "int",
         {{"f", Kind::Int}, {"g", Kind::Int}}},
        // materialized boolean: MSC builds (a||b) into a 0/1 register then tests it (olecli sub_28aa)
        {"bool-materialize", "1e589045558bec1e8ed883ec02c45e0626837f7f00750b26837f7d0075042bd2eb03ba01000bd274052bc0eb0490b805004d4d8be51f5d4dca040090", "int",
         {{"p", Kind::Far}}},
    };
    for (const FarCase& f : far) {
        const Bytes code = bytes_of(f.hex);
        msc6::Context c;
        c.imports = {{0, "G"}};
        c.return_type = f.return_type;
        c.params = f.params;
        const Result<msc6::Reading> r = msc6::invert(code, c);
        if (!r.ok()) {
            out("// {}: DECLINED\n", f.label);
            continue;
        }
        const auto again = through_msc6(environment, r.value().text, kMsc6Flags);
        if (!again) return 2;
        if (!*again) continue;
        out("// {:<12} masked-equal = {} ({} vs {} B)\n", f.label, py_bool(masked(**again, false) == masked(code, false) && (*again)->size() == code.size()), (*again)->size(), code.size());
    }

    // real retail LZEXPAND functions (cdecl + param-inference + cmp-consumes-call); padding-tolerant compare, since a standalone recompile appends an alignment nop
    const std::vector<RealCase> real{
        {"retail-cdecl(seg11)", "1e589045558bec1e8ed883ec02568b760cff7606ff760aff7608569a210500003bc67506b8ffffeb03902bc05e4d4d8be51f5d4dcb", {{0, "G"}}, {}},
        {"retail-wstore(seg10)", "1e589045558bec1e8ed883ec02b8ffffa382004d4d8be51f5d4dcb", {}, {}},
        {"retail-free(seg11)",
         "1e589045558bec1e8ed883ec02833e7e00007420ff367e009affff0000ff367e009affff0000c7067e0000002bc0a3ee00a3ec00c606800000"
         "2bc0a3ee00a3ec00a3f200a3f000a3f600a3f400a3fa00a3f800a3fe00a3fc00a30201a300014d4d8be51f5d4dcb",
         {{0, "G"}, {1, "H"}},
         {{0xec, {"gL_ec", msc6_small::Width::Long}}}},
        {"retail-cdecl2(seg12)",
         "1e589045558bec1e8ed883ec029affff0000833e8c01007412ff368c019a9b09000083c402c7068c010000833e8a01007412ff368a019affff000083c402c7068a010000833e8201007420ff3682"
         "019af4060000ff3682019afd060000c706820100002bc0a39801a396012bc0a38401a386014d4d8be51f5d4dcb",
         {},
         {{0x196, {"gL_196", msc6_small::Width::Long}}, {0x186, {"gL_186", msc6_small::Width::Long}}}},
        {"retail-addrglob(seg10)",
         "1e589045558bec1e8ed883ec02688203688003ff76069affff000083c4060bc07406b8ffffeb2090ff368203ff368003ff76089affff000083c4060bc07405b8feffeb03b801004d4d8be51f5d4dcb",
         {{0, "G0"}, {1, "G1"}},
         {}},
    };
    for (const RealCase& rc : real) {
        const Bytes code = bytes_of(rc.hex);
        msc6::Context c;
        c.imports = rc.imports;
        c.globals = rc.globals;
        const Result<msc6::Reading> r = msc6::invert(code, c);
        if (!r.ok()) {
            out("// {}: DECLINED\n", rc.label);
            continue;
        }
        const auto again = through_msc6(environment, r.value().text, kMsc6Flags);
        if (!again) return 2;
        if (!*again) continue;
        out("// {:<20} masked-equal = {}\n", rc.label, py_bool(masked(**again, true) == masked(code, true)));
    }
    return 0;
}

// ---- msc6-idioms ---------------------------------------------------------------------------------------------------------------------

namespace {

struct Idiom {
    const char* name;
    const char* source;
};

constexpr const char* kIdiomFlags[] = {"/AM /G2 /Gsw /Ox", "/AM /G2 /Gsw /Os", "/AM /G2 /Gsw /Ot", "/AM /G2 /Gsw /O1", "/AM /G2 /Gsw /O2", "/AM /G2 /Gsw"};

const Idiom kIdioms[] = {
    {"ret-const", "int far pascal F(void){ return 1; }"},
    {"ret-param", "int far pascal F(int a){ return a; }"},
    {"frame-local", "int far pascal F(void){ int i; i=0; return i; }"},
    {"bool-eq", "int far pascal F(int a){ return a == 1; }"},
    {"add-params", "int far pascal F(int a,int b){ return a+b; }"},
    {"far-deref", "int far pascal F(int far *p){ return *p; }"},
    {"far-field", "typedef unsigned int WORD; int far pascal F(char far *p){ return *(WORD far*)((char far*)p+4); }"},
    {"cdecl-clean", "typedef unsigned int WORD; extern WORD far G(); int far pascal F(int a){ return G(a); }"},
    {"loop-count", "int far pascal F(int n){ int i,s; s=0; for(i=0;i<n;i++) s+=i; return s; }"},
    {"long-shl", "long far pascal F(long a){ return a << 4; }"},
    {"global-store", "extern int gW; void far pascal F(void){ gW = -1; }"},
    {"if-else", "int far pascal F(int a){ if(a) return 1; return 0; }"},
};

// A short fingerprint of a snippet's frame, prologue and epilogue: the loadds prologue, a `sub sp,N` frame, the epilogue, the sbb/neg boolean.
std::string idiom_tag(ByteView b) {
    static const Bytes prologue = bytes_of("1e589045558bec1e8ed8"), epilogue = bytes_of("4d4d8be51f5d4d");
    const auto has = [&](const Bytes& p, std::size_t limit) {
        const std::size_t end = std::min(b.size(), limit);
        return std::search(b.begin(), b.begin() + static_cast<std::ptrdiff_t>(end), p.begin(), p.end()) != b.begin() + static_cast<std::ptrdiff_t>(end);
    };
    std::vector<std::string> tag;
    tag.push_back(b.size() >= prologue.size() && std::equal(prologue.begin(), prologue.end(), b.begin()) ? "pro" : "PRO?");
    if (has(bytes_of("83ec"), 16)) tag.push_back("frame");
    if (has(epilogue, b.size())) tag.push_back("epi");
    if (has(bytes_of("1bc0f7d8"), b.size())) tag.push_back("sbb/neg");
    return text::join(tag, ",");
}

}  // namespace

int msc6_idioms(const Environment& environment) {
    if (!environment.compile_msc6) {
        out("oracle not configured\n");
        return 1;
    }
    out("MSC 6.0a idiom flag-sensitivity (distinct byte forms across the flag matrix):\n\n");
    for (const Idiom& idiom : kIdioms) {
        // the forms, grouped by identical output, in the order the switch sets give them
        std::vector<std::pair<std::string, std::vector<std::string>>> forms;
        for (const char* flags : kIdiomFlags) {
            const std::optional<Bytes> built = environment.compile_msc6(idiom.source, flags);
            const std::string hx = built ? hex_of(*built) : "ERR";
            const std::string last = std::string(text::split(flags, ' ').back());
            const auto at = std::ranges::find(forms, hx, &std::pair<std::string, std::vector<std::string>>::first);
            if (at == forms.end()) forms.push_back({hx, {last}});
            else at->second.push_back(last);
        }
        out("{:<14} {} form(s){}\n", idiom.name, forms.size(), forms.size() > 1 ? "  FLAG-SENSITIVE" : "");
        for (const auto& [hx, flags] : forms) {
            if (hx == "ERR") out("      ERR under {}\n", text::join(flags, ","));
            else out("      [{}] {}\n", text::join(flags, ","), idiom_tag(bytes_of(hx)));
        }
    }
    return 0;
}

// ---- msvc2 ---------------------------------------------------------------------------------------------------------------------------

namespace {

constexpr std::string_view kMsvc2Flags = "/c /O1 /Oy-";

const char* const kNoMsvc2 = "the 1994 32-bit compiler is not set up (settings.json: period_compiler)\n";

// The compiler's code for one construct: the function `_f...` of its object, its bytes and the positions a linker fills in.
struct Built {
    Bytes code;
    std::set<std::uint64_t> fixups;
    bool found = false;
    std::string error;
};

Built build_with_msvc2(const Environment& environment, const std::string& source) {
    Built b;
    const Environment::Msvc2Object r = environment.compile_msvc2(source, kMsvc2Flags);
    if (!r.object) {
        b.error = r.error;
        return b;
    }
    b.found = true;   // an object; the function may still be missing (the compiler's objects are COFF)
    try {
        for (const symbols::libraries::Function& fn : symbols::libraries::coff::functions(*r.object)) {
            if (!fn.name.starts_with("_f")) continue;
            b.code = fn.code;
            for (std::size_t i = 0; i < fn.mask.size(); ++i)
                if (fn.mask[i]) b.fixups.insert(i);
            return b;
        }
    } catch (const Error&) {   // an object that cannot be read has no function
    }
    b.found = false;
    return b;
}

bool match_outside(ByteView predicted, ByteView real, const std::set<std::uint64_t>& mask) {
    if (predicted.size() != real.size()) return false;
    for (std::size_t i = 0; i < real.size(); ++i)
        if (!mask.contains(i) && predicted[i] != real[i]) return false;
    return true;
}

bool has_flag(std::span<const std::string> args, std::string_view flag) { return std::ranges::find(args, flag) != args.end(); }

}  // namespace

int msvc2_forward(const Environment& environment, std::span<const std::string> args) {
    if (!environment.compile_msvc2) {
        err("{}", kNoMsvc2);
        return 2;
    }
    const bool show = has_flag(args, "--show");
    std::size_t ok = 0, total = 0;
    if (has_flag(args, "--frame")) {
        for (const msvc2::FrameCase& c : msvc2::frame_battery()) {
            ++total;
            const msvc2::FrameLayout predicted = msvc2::frame_layout(c.locals);
            std::set<std::int64_t> predicted_offsets;
            for (const auto& [index, offset] : predicted.offsets) predicted_offsets.insert(offset);
            const Built b = build_with_msvc2(environment, c.source);
            if (!b.found) {
                out("  {:<8} ORACLE-FAIL\n", c.name);
                continue;
            }
            // from the compiled bytes: the frame size of `sub esp,N` and the distinct [ebp-X] offsets
            std::uint32_t real_frame = 0;
            std::set<std::int64_t> real_offsets;
            platforms::DecoderSet decoders;
            for (const platforms::Insn& i : decoders.get(platforms::Arch::X86, 32).disasm(b.code, 0)) {
                if (i.mnemonic == "sub" && i.op_str.starts_with("esp,")) real_frame = *text::parse_int<std::uint32_t>(text::trim(i.op_str.substr(4)), 0);
                static const Regex frame_ref(R"(\[ebp - (0x[0-9a-f]+|\d+)\])");
                frame_ref.for_each_match(i.op_str, [&](const RegexMatch& m) {
                    real_offsets.insert(-*text::parse_int<std::int64_t>(m.group(1), 0));
                    return true;
                });
            }
            const bool same = predicted.frame_size == real_frame && predicted_offsets == real_offsets;
            ok += same;
            if (show || !same) {
                const auto list = [](const std::set<std::int64_t>& s) {
                    std::vector<std::string> v;
                    for (const std::int64_t x : s) v.push_back(std::to_string(x));
                    return "[" + text::join(v, ", ") + "]";
                };
                out("  {:<8} {}  pred(fs={},off={}) real(fs={},off={})\n", c.name, same ? "OK " : "MISS", predicted.frame_size, list(predicted_offsets), real_frame, list(real_offsets));
            }
        }
        out("\nmsvc2 frame-layout self-test: {}/{} byte-exact  (MSVC 2.0 /O1 /Oy-)\n", ok, total);
        return ok == total ? 0 : 1;
    }
    for (const msvc2::Case& c : msvc2::battery()) {
        const auto predicted = msvc2::emit(c.node);
        ++total;
        const Built b = build_with_msvc2(environment, msvc2::to_c(c.node));
        if (!b.found) {
            out("  {:<8} ORACLE-FAIL {}\n", c.name, b.error.substr(0, 80));
            continue;
        }
        const bool same = predicted && match_outside(*predicted, b.code, b.fixups);
        ok += same;
        if (show || !same) out("  {:<8} {}  pred={}  real={}\n", c.name, same ? "OK " : "MISS", predicted ? hex_of(*predicted, ' ') : "(declined)", hex_of(b.code, ' '));
    }
    out("\nmsvc2model self-test: {}/{} byte-exact  (MSVC 2.0 /O1 /Oy-)\n", ok, total);
    std::size_t declined_ok = 0, declined_total = 0;
    for (const msvc2::Case& c : msvc2::declined_battery()) {
        ++declined_total;
        const bool declined = !msvc2::emit(c.node).has_value();
        declined_ok += declined;
        if (show || !declined) out("  declined {:<8} {}\n", c.name, declined ? "OK (None)" : "LEAK: emitted bytes!");
    }
    out("declined-correctly: {}/{}  (unmodeled shapes return None, never mis-modeled)\n", declined_ok, declined_total);
    return ok == total && declined_ok == declined_total ? 0 : 1;
}

int msvc2(const Environment& environment, std::span<const std::string> args) {
    if (!environment.compile_msvc2) {
        err("{}", kNoMsvc2);
        return 2;
    }
    const bool show = has_flag(args, "--show");
    std::size_t ok = 0, total = 0;
    for (const msvc2::Case& c : msvc2::battery()) {
        ++total;
        const Built b = build_with_msvc2(environment, msvc2::to_c(c.node));
        if (!b.found) {
            out("  {:<8} ORACLE-FAIL\n", c.name);
            continue;
        }
        const auto inverse = msvc2::invert(b.code, nullptr);
        std::set<long long> mask(b.fixups.begin(), b.fixups.end());
        bool good = false;
        if (inverse) {
            const auto again = msvc2::emit(inverse->node);   // re-emit the full node
            good = again && match_outside(*again, b.code, b.fixups);
        }
        ok += good;
        if (show || !good) out("  {:<8} {}  {}\n", c.name, good ? "OK " : "MISS", inverse ? inverse->c : "(declined)");
    }
    out("\nmsvc2inv round-trip: {}/{} bytes->C->byte-exact  (MSVC 2.0 /O1 /Oy-)\n", ok, total);
    return ok == total ? 0 : 1;
}

// ---- the early model against Microsoft C 2.03 ---------------------------------------------------------------------------------------------

namespace {

// 1 for each byte of a call's or jump's distance whose target is outside `code`.
std::vector<bool> external_branches(ByteView code) {
    std::vector<bool> mask(code.size(), false);
    platforms::DecoderSet decoders;
    for (const platforms::Insn& i : decoders.get(platforms::Arch::X86, 16).disasm(code, 0)) {
        const std::string& m = i.mnemonic;
        if (!(m == "call" || m == "jmp" || m.starts_with("j") || m.starts_with("loop")) || !i.op_str.starts_with("0x")) continue;
        const auto target = text::parse_int<long long>(i.op_str, 16);
        if (!target) continue;
        if (0 <= *target && *target < static_cast<long long>(code.size())) continue;
        const std::uint8_t first = code[i.address];
        const int width = first == 0xEB || (0x70 <= first && first <= 0x7F) ? 1 : (i.size >= 4 || first == 0xE8 || first == 0xE9) ? 2 : 1;
        for (int k = 0; k < width; ++k) {
            const std::size_t at = i.address + i.size - static_cast<std::size_t>(width) + static_cast<std::size_t>(k);
            if (at < mask.size()) mask[at] = true;
        }
    }
    return mask;
}

// The C text the model reads `code` as; throws where it cannot.
std::string early_source(ByteView code, const std::string& name) {
    const bool linear = fs::get_env("NO_CFG").value_or("") != "";
    const early_c::Options options = early_c::Options::from_environment();
    const early_c::Recovered r = linear ? early_c::recover(code, 0, {}, options) : early_c::recover_cfg(code, 0, {}, options);
    std::set<std::string> globals;
    static const Regex global_name(R"(g_[0-9a-f]+)");
    for (const std::string& s : r.statements)
        for (const std::string& g : global_name.find_all(s)) globals.insert(g);
    const std::string params = text::join(r.params, ", ");
    const std::string declared = r.params.empty() ? "" : std::format("{} {};", r.return_type, params);
    const std::vector<early_c_frame::Copy> copies = early_c::detect_copies(r.statements);
    std::string locals;
    for (const early_c_frame::Decl& d : early_c_frame::emit_local_decls(r.locals, r.frame_size, {}, copies)) locals += early_c_frame::format_decl(d) + " ";
    std::vector<std::string> body;
    if (!text::trim(locals).empty()) body.push_back(std::string(text::trim(locals)));
    body.insert(body.end(), r.statements.begin(), r.statements.end());
    const std::string head = globals.empty() ? "" : "int " + text::join(globals, ", ") + ";\n";
    std::string lines;
    for (const std::string& s : body) lines += (lines.empty() ? "" : "\n") + std::string("    ") + s;
    return std::format("{}{}({})\n{}\n{{\n{}\n}}\n", head, name, params, declared, lines);
}

struct Check {
    std::optional<std::string> text;
    std::optional<bool> same;
    std::string note;
};

// The function the object holds under `name` (the compiler's leading underscore not counted), or its first.
const formats::omf::FunctionBytes* function_named(const OrderedMap<std::string, formats::omf::FunctionBytes>& functions, const std::string& name) {
    for (const auto& [k, v] : functions) {
        std::string key = text::upper(k);
        while (key.starts_with("_")) key.erase(0, 1);
        if (key == text::upper(name)) return &v;
    }
    return functions.empty() ? nullptr : &functions.begin()->second;
}

Check reconstruct(const Environment& environment, ByteView image, std::size_t off, std::size_t size, bool verify) {
    Check c;
    const ByteView code = off < image.size() ? image.subspan(off, std::min(size, image.size() - off)) : ByteView();
    if (code.size() < 4) {
        c.same = false;
        c.note = "too small to be a function";
        return c;
    }
    const std::string name = std::format("f{:05x}", off);
    try {
        c.text = early_source(code, name);
    } catch (const Error& e) {
        c.same = false;
        c.note = std::string("the model failed: ") + e.what();
        return c;
    }
    if (!verify) {
        c.note = "not compiled";
        return c;
    }
    const std::optional<Bytes> obj = environment.compile_mc203(*c.text, "cand");
    if (!obj) {
        c.same = false;
        c.note = "does not compile";
        return c;
    }
    const auto functions = formats::omf::function_bytes(*obj);
    const formats::omf::FunctionBytes* built = function_named(functions, name);
    if (built == nullptr) {
        c.same = false;
        c.note = "no function in the object file";
        return c;
    }
    std::vector<bool> mask(built->code.size(), false);
    for (const std::uint64_t i : built->fixup_mask)
        if (i < mask.size()) mask[i] = true;
    const std::vector<bool> branches = external_branches(built->code);
    std::size_t masked_bytes = 0;
    for (std::size_t i = 0; i < mask.size(); ++i) {
        if (branches[i]) mask[i] = true;
        masked_bytes += mask[i];
    }
    bool same = built->code.size() == code.size();
    for (std::size_t i = 0; same && i < code.size(); ++i) same = mask[i] || built->code[i] == code[i];
    c.same = same;
    c.note = same ? std::format("identical ({} bytes the linker fills in)", masked_bytes) : std::format("different ({} bytes against {})", built->code.size(), code.size());
    return c;
}

}  // namespace

int early_c_check(const Environment& environment, std::span<const std::string> args) {
    if (args.empty()) throw UsageError("early-c-check IMAGE OFFSET LENGTH   (OFFSET in hex)\nearly-c-check IMAGE --batch LIST.json   [{name, off, size}]");
    const Bytes image = fs::read_file(fs::from_text(args[0]));
    const bool verify = static_cast<bool>(environment.compile_mc203);
    if (!verify) err("Microsoft C 2.03 is not set up (settings.json: kits.msc203): the text was not put to the compiler\n");
    const auto batch = std::ranges::find(args, "--batch");
    if (batch != args.end()) {
        if (batch + 1 == args.end()) throw UsageError("early-c-check: --batch wants a list file");
        const Json work = json::load(fs::from_text(*(batch + 1)));
        std::size_t same = 0;
        for (const Json& item : work) {
            const auto off = text::parse_int<std::size_t>(item.at("off").get<std::string>(), 16);
            if (!off) throw UsageError("early-c-check: an offset in the list is not hex");
            const Check c = reconstruct(environment, image, *off, item.at("size").get<std::size_t>(), verify);
            same += c.same.value_or(false);
            out("  {:<16} {:<34} {}\n", item.at("name").get<std::string>(), c.note, c.same.value_or(false) ? "OK" : "");
        }
        out("\n{} of {} functions give the original's bytes back\n", same, work.size());
        return verify ? 0 : 2;
    }
    if (args.size() < 3) throw UsageError("early-c-check IMAGE OFFSET LENGTH   (OFFSET in hex)");
    const auto off = text::parse_int<std::size_t>(args[1], 16);
    const auto size = text::parse_int<std::size_t>(args[2], 10);
    if (!off || !size) throw UsageError("early-c-check: the offset is hex and the length decimal");
    const Check c = reconstruct(environment, image, *off, *size, verify);
    out("same: {} | {}\n\n{}\n", c.same ? py_bool(*c.same) : "None", c.note, c.text.value_or("None"));
    return verify ? 0 : 2;
}

namespace {

constexpr std::string_view kRoundTripBattery = R"(
t01(){int a,b; a=1; b=2;}
t02(){int a,b,c; a=1; c=3;}
t04(){int a,b,c,d; a=1; d=4;}
t07(){int a,b,c; a=b+c+1;}
r09(){int a,b; a=1; b=a+a;}
r11(){int a,b; a=5; b=a;}
r12(){int a,b,c; a=1; b=2; c=a*b;}
r13(p)int p;{int a; a=p; return a+1;}
r14(){int a,b; a=1; b=2; return a+b;}
r10(){int a,b,c; a=1; b=2; c=a+b;}
r15(){int a,b,c; a=1; b=2; c=a-b;}
r16(){int a,b; a=3; b=a*a;}
c01(){int a,b; a=5; b=a;}
c02(){int a,b,c; a=11; b=a; c=b;}
c03(){int a,b,c,d; a=11; b=22; c=a; d=44;}
c04(){int a,b,c,d; a=11; b=22; c=33; d=a;}
c05(){int a,b,c; a=11; b=22; c=b;}
c06(){int a,b; a=11; b=a; a=99;}
c07(){int a,b,c,d; a=11; b=a; c=33; d=c;}
)";

constexpr std::string_view kFrameProbeBattery = R"(
g01(){int a,b; a=(b+1)*2;}
g02(){int a,b,c; a=(b+c)*(b-c);}
g03(){long a,b; a=b+1;}
g04(){long a; int b; a=b;}
g05(){int a; long b,c; a=1;}
g06(){char a,b; a=1; b=2;}
g07(){int a; char b; long c; a=1; b=2; c=3;}
g08(){int a[3]; a[0]=1;}
g09(){int a,b; a=1; {int c; c=a; b=c;}}
g10(p)int p;{int a; a=p+1;}
g11(){int a,b,c; a=b+c+1;}
g12(){int a; a=1; a=a*a+a;}
g13(){int a,b; a=1;b=2;return a*b+a-b;}
g14(){long a,b,c; a=b*c;}
g15(){int a,b,c,d,e; a=b=c=d=e=0;}
)";

constexpr const char* kOrderProbes[] = {
    "s1(){int a,b,c,d; a=11; b=22; c=a; d=44;}", "s2(){int a,b,c; a=11; b=a; c=33;}",     "s3(){int a,b,c; a=11; c=a; b=22;}",
    "s4(){int a,b,c,d; a=11; b=22; c=33; d=a;}", "s5(){int a,b,c; a=11; b=22; c=33;}",   "s6(){int a,b; a=11; b=a;}",
    "s7(){int a,b,c; a=11; b=22; c=a;}",         "s8(){int a,b,c,d; a=11; b=a; c=33; d=c;}",
};

int not_set_up_mc203() {
    err("Microsoft C 2.03 is not set up (settings.json: kits.msc203)\n");
    return 2;
}

std::string without_underscore(const std::string& k) { return k.starts_with("_") ? k.substr(1) : k; }

}  // namespace

int early_c_round_trip(const Environment& environment) {
    if (!environment.compile_mc203) return not_set_up_mc203();
    const std::optional<Bytes> object = environment.compile_mc203(kRoundTripBattery, "probe");
    if (!object) {
        out("COMPILE FAILED\n");
        return 1;
    }
    auto functions = formats::omf::function_bytes(*object);
    std::vector<std::string> keys;
    for (const auto& [k, v] : functions) keys.push_back(k);
    std::ranges::sort(keys, [](const std::string& a, const std::string& b) { return text::upper(a) < text::upper(b); });
    std::size_t ok = 0, total = 0;
    for (const std::string& k : keys) {
        const Bytes code = functions.at(k).code;
        ++total;
        const std::string name = without_underscore(k);
        early_c::Recovered r;
        std::string source;
        try {
            r = early_c::recover(code);
            source = early_c::to_c(name, r.frame_size, r.params, r.statements, r.return_type, r.locals);
        } catch (const Error& e) {
            out("  {:<5} INVERSE-FAIL {}\n", name, e.what());
            continue;
        }
        const std::optional<Bytes> again = environment.compile_mc203(source, "cand");
        const auto fns = again ? formats::omf::function_bytes(*again) : OrderedMap<std::string, formats::omf::FunctionBytes>();
        if (!again || fns.empty()) {
            out("  {:<5} RECOMPILE-FAIL\n", name);
            continue;
        }
        const formats::omf::FunctionBytes& recompiled = fns.begin()->second;
        std::vector<std::size_t> diffs;
        const std::vector<bool> branches = external_branches(recompiled.code);
        if (recompiled.code.size() == code.size())
            for (std::size_t i = 0; i < code.size(); ++i)
                if (!recompiled.fixup_mask.contains(i) && !branches[i] && recompiled.code[i] != code[i]) diffs.push_back(i);
        const bool good = recompiled.code.size() == code.size() && diffs.empty();
        ok += good;
        std::vector<std::string> locals;
        std::vector<early_c_frame::Offset> sorted(r.locals.begin(), r.locals.end());
        std::ranges::sort(sorted);
        for (const early_c_frame::Offset o : sorted) locals.push_back(std::to_string(o));
        std::string note = recompiled.code.size() != code.size() ? std::format("len {}/{}", recompiled.code.size(), code.size()) : good ? "BYTE-EXACT" : "diff@" + [&] {
            std::vector<std::string> d;
            for (std::size_t i = 0; i < std::min<std::size_t>(diffs.size(), 6); ++i) d.push_back(std::to_string(diffs[i]));
            return "[" + text::join(d, ", ") + "]";
        }();
        out("  {:<5} fs={:<3} loc={:<14} {}\n", name, r.frame_size, "[" + text::join(locals, ", ") + "]", note);
    }
    out("\nrtlocals: {}/{} round-trip BYTE-EXACT with framemodel local layout\n", ok, total);
    return 0;
}

int early_c_frame_probe(const Environment& environment) {
    if (!environment.compile_mc203) return not_set_up_mc203();
    const std::optional<Bytes> object = environment.compile_mc203(kFrameProbeBattery, "probe");
    if (!object) {
        out("COMPILE FAILED\n");
        return 1;
    }
    const auto functions = formats::omf::function_bytes(*object);
    std::vector<std::string> keys;
    for (const auto& [k, v] : functions) keys.push_back(k);
    std::ranges::sort(keys, [](const std::string& a, const std::string& b) { return text::upper(a) < text::upper(b); });
    platforms::DecoderSet decoders;
    static const Regex bp_ref(R"(\[bp\s*([+\-]\s*0x[0-9a-f]+|[+\-]\s*\d+)?\])");
    for (const std::string& k : keys) {
        std::optional<long long> frame;
        std::set<long long> displacements;
        for (const platforms::Insn& i : decoders.get(platforms::Arch::X86, 16).disasm(functions.at(k).code, 0)) {
            if (i.mnemonic == "sub" && i.op_str.starts_with("sp,")) frame = text::parse_int<long long>(text::trim(i.op_str.substr(3)), 0);
            bp_ref.for_each_match(i.op_str, [&](const RegexMatch& m) {
                std::string d = m.has(1) ? std::string(m.group(1)) : std::string();
                std::erase(d, ' ');
                displacements.insert(d.empty() ? 0 : *text::parse_int<long long>(d, 0));
                return true;
            });
        }
        std::vector<std::string> positive;
        for (const long long d : displacements)
            if (d >= 0) positive.push_back(std::to_string(d));
        out("{:<6} fs={:<4}  bp-disps=[{}]\n", without_underscore(k), frame ? std::to_string(*frame) : "None", text::join(positive, ", "));
    }
    return 0;
}

int early_c_order_probe(const Environment& environment) {
    if (!environment.compile_mc203) return not_set_up_mc203();
    platforms::DecoderSet decoders;
    static const Regex bp_offset(R"(bp \+ (0x[0-9a-f]+|\d+)|\[bp\])");
    const auto offset_of = [&](const std::string& operand) {
        const auto m = bp_offset.search(operand);
        return m && m->has(1) ? *text::parse_int<long long>(m->group(1), 0) : 0LL;
    };
    const auto is_general = [](const std::string& r) { return r == "ax" || r == "bx" || r == "cx" || r == "dx"; };
    for (const char* source : kOrderProbes) {
        const std::optional<Bytes> object = environment.compile_mc203(std::string(source) + "\n", "probe");
        std::vector<std::string> ops;
        if (object) {
            const auto functions = formats::omf::function_bytes(*object);
            if (!functions.empty()) {
                for (const platforms::Insn& i : decoders.get(platforms::Arch::X86, 16).disasm(functions.begin()->second.code, 0)) {
                    if (i.address < 0xf) continue;
                    const std::vector<std::string_view> parts = text::split(i.op_str, std::string_view(", "));
                    const std::string first = parts.empty() ? "" : std::string(parts[0]);
                    const std::string second = parts.size() > 1 ? std::string(parts[1]) : std::string();
                    const std::string& m = i.mnemonic;
                    if (m == "mov" && is_general(first) && (second.starts_with("0x") || text::parse_int<long long>(second).has_value())) ops.push_back(std::format("LD {}={}", first, second));
                    else if (m == "mov" && is_general(first) && second.find("[bp") != std::string::npos) ops.push_back(std::format("{}=[{}]", first, offset_of(second)));
                    else if ((m == "mov" || m == "add" || m == "sub") && first.find("[bp") != std::string::npos) ops.push_back(std::format("ST[{}]={}", offset_of(first), second));
                    else if (m == "inc" || m == "dec" || m == "imul" || m == "idiv") ops.push_back(m + " " + i.op_str);
                }
            }
        }
        std::vector<std::string> quoted;
        for (const std::string& o : ops) quoted.push_back("'" + o + "'");
        out("{:<44} [{}]\n", source, text::join(quoted, ", "));
    }
    return 0;
}

// ---- the module pass ---------------------------------------------------------------------------------------------------------------------

int module(const Environment& environment, std::span<const std::string> args) {
    std::string flags, show;
    std::size_t first = 0;
    std::vector<std::string> positional;
    for (std::size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--flags" || args[i] == "--first" || args[i] == "--show") {
            if (i + 1 >= args.size()) throw UsageError(std::format("invert: {} wants a value", args[i]));
            const std::string& value = args[++i];
            if (args[i - 1] == "--flags") flags = value;
            else if (args[i - 1] == "--show") show = value;
            else if (const auto n = text::parse_int<std::size_t>(value)) first = *n;
            else throw UsageError("invert: --first wants a number");
        } else if (!args[i].starts_with("--")) {
            positional.push_back(args[i]);
        }
    }
    if (positional.empty()) throw UsageError("invert <16-bit module> [--first N] [--flags \"/AM /G2 /Gsw /Ox\"] [--show NAME]");
    const stdfs::path given = fs::from_text(positional[0]);
    const std::optional<stdfs::path> file = stdfs::is_regular_file(given) ? std::optional<stdfs::path>(given) : (environment.retail ? environment.retail(positional[0]) : std::nullopt);
    if (!file) throw NotFoundError(given, 0, "no module of that name or path");
    std::vector<module_pass::Function> functions = environment.functions ? environment.functions(*file) : module_pass::framed_functions(*file);
    if (first && functions.size() > first) functions.resize(first);
    const auto say = [](const std::string& line) { out("{}\n", line); };
    const auto results = environment.compile_msc6_many || environment.compile_msvc152_many
                             ? module_pass::run_batched(functions, environment.compile_msc6_many, environment.compile_msvc152_many, flags, say)
                             : module_pass::run(functions, environment.compile_msc6, environment.compile_msvc152, flags, say);
    if (!show.empty()) {
        const auto it = results.find(show);
        if (it == results.end()) out("no function '{}'\n", show);
        else out("{}\n", it->second.text);
        return 0;
    }
    std::map<module_pass::Verdict, std::size_t> counts;
    std::vector<std::pair<std::string, std::size_t>> reasons;
    for (const auto& [name, r] : results) {
        ++counts[r.verdict];
        if (r.verdict != module_pass::Verdict::Declined) continue;
        std::string why = r.text.substr(0, r.text.find(" @"));
        why = std::string(text::utf8_prefix(why, 60));
        const auto at = std::ranges::find(reasons, why, &std::pair<std::string, std::size_t>::first);
        if (at == reasons.end()) reasons.push_back({why, 1});
        else ++at->second;
    }
    std::vector<std::string> summary;
    for (const module_pass::Verdict v : {module_pass::Verdict::Identical, module_pass::Verdict::Recognised, module_pass::Verdict::Declined})
        if (counts[v]) summary.push_back(std::format("{} {}", module_pass::to_text(v), counts[v]));
    out("{}: {} functions: {}\n", fs::to_text(file->filename()), results.size(), text::join(summary, ", "));
    std::ranges::stable_sort(reasons, [](const auto& a, const auto& b) { return a.second > b.second; });
    for (std::size_t i = 0; i < reasons.size() && i < 8; ++i) out("  declined {:>4}  {}\n", reasons[i].second, reasons[i].first);
    return 0;
}

}  // namespace cb::invert::tools
