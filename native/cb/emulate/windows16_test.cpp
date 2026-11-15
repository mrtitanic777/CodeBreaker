// Tests of cb/emulate/windows16: the descriptors and the layout; real 16-bit Windows modules (LZEXPAND, VER, COMM.DRV) run through
// the Runner and compared, case by case, with the recorded results of the same calls (testdata/windows16.golden.json); the stand-ins, the heap, the errors.
//
// Design notes: (a) the code-base table belongs to the run, not the recorded call. (b) A relocation whose place lies outside
// its segment's 64K window is a FormatError at load. (c) An unnamed recording is not remembered (its `retf n` table has no key).
#include "cb/emulate/windows16.hpp"

#include <format>
#include <iostream>
#include <map>
#include <set>

#include <nlohmann/json.hpp>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/json.hpp"
#include "cb/core/testing.hpp"
#include "cb/emulate/pcode.hpp"
#include "cb/emulate/unicorn_uc.hpp"
#include "cb/formats/ne.hpp"
#include "cb/knowledge/package.hpp"

using namespace cb;
using namespace cb::emulate;
using namespace cb::emulate::windows16;
namespace sfs = std::filesystem;
using nlohmann::ordered_json;

namespace {

// FNV-1a over numbers, each as eight little-endian bytes: the checksum the golden holds of the lists it does not store whole.
std::uint64_t fold(const std::vector<std::uint64_t>& values) {
    std::uint64_t h = 0xCBF29CE484222325ULL;
    for (const std::uint64_t v : values) {
        for (int k = 0; k < 8; ++k) {
            h ^= (v >> (8 * k)) & 0xFF;
            h *= 0x100000001B3ULL;
        }
    }
    return h;
}

sfs::path module_path(const std::string& which) {
    if (which == "lz") return testing::data_path("formats/testdata/lzexpand.dll");
    if (which == "ver") return testing::data_path("emulate/testdata/ver.dll");
    return testing::data_path("emulate/testdata/comm.drv");
}

const ordered_json& golden() {
    static const ordered_json g = json::load(testing::data_path("emulate/testdata/windows16.golden.json"));
    return g;
}

// The bytes of arguments each import removes, as the recorded run found them: the Runner is told them the way the knowledge base would.
std::map<std::string, std::uint32_t> argument_sizes() {
    std::map<std::string, std::uint32_t> sizes;
    for (const ordered_json& c : golden()["cases"]) {
        if (!c.contains("result")) continue;
        for (const ordered_json& h : c["result"]["imports"])
            if (h[6].get<bool>() && !h[5].is_null()) sizes[h[0].get<std::string>()] = h[5].get<std::uint32_t>();
    }
    return sizes;
}

// A model lookup over the three stand-ins the recorded runs met (what the knowledge base's win16 models give them): the recorded runs
// used the knowledge base itself; this checks the Runner against the same answers.
ModelLookup real_models(knowledge::Knowledge& kb) { return models_of(kb); }

struct World {
    fs::ScopedTempDir dir{"cb-windows16"};
    knowledge::Store store{dir.path() / "k.sqlite", knowledge::DataFolders::under(sfs::path(CB_SOURCE_DIR).parent_path())};
    knowledge::Knowledge kb{store};
    native_trace::Governor governor;
    std::map<std::string, std::uint32_t> sizes = argument_sizes();
    Runner runner{[this] {
        Environment env;
        env.governor = &governor;
        env.models = real_models(kb);
        env.argument_bytes = [this](std::string_view api) -> std::optional<std::uint32_t> {
            const auto it = sizes.find(std::string(api));
            if (it == sizes.end()) return std::nullopt;
            return it->second;
        };
        return env;
    }()};
};

void differ(const std::string& name, const std::string& what, const std::string& got, const std::string& want) {
    testing::record_failure(__FILE__, __LINE__, std::format("{}: {} was {} but expected {}", name, what, got, want));
}
template <class A, class B>
void same(const std::string& name, const char* what, const A& got, const B& want) {
    testing::count_check();
    if (got != static_cast<A>(want)) differ(name, what, std::format("{}", got), std::format("{}", static_cast<A>(want)));
}

RecordedRegisters recorded_regs() {
    return {0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777};
}

std::array<std::uint8_t, 8> raw_descriptor(std::uint32_t base, std::uint32_t limit, std::uint8_t access) {
    std::array<std::uint8_t, 8> d{};
    d[0] = limit & 0xFF;
    d[1] = (limit >> 8) & 0xFF;
    d[2] = base & 0xFF;
    d[3] = (base >> 8) & 0xFF;
    d[4] = (base >> 16) & 0xFF;
    d[7] = (base >> 24) & 0xFF;
    d[5] = access;
    return d;
}

RecordedCall context_without_memory() {
    RecordedCall c;
    for (int i = 0; i < 256 * 256; ++i) c.stack.push_back(static_cast<std::uint8_t>(i & 0xFF));
    c.sp = 0xFFD0;
    c.ss = 0x4F;
    c.ds = std::uint16_t{0x57};
    c.es = std::uint16_t{0x5F};
    c.regs = recorded_regs();
    c.ldt[0x57] = {Bytes(100, 0x11), false};
    c.ldt[0x5F] = {Bytes(100, 0x22), true};
    c.data_selector = std::uint16_t{0x57};
    c.key = "ctx1";
    return c;
}

RecordedCall context_with_memory() {
    RecordedCall c;
    c.stack.assign(0x10000, 0);
    c.sp = 0xFFE0;
    c.ss = (1 << 3) | 7;
    c.ds = std::uint16_t{(3 << 3) | 7};
    c.es = std::uint16_t{(1 << 3) | 7};
    c.regs = recorded_regs();
    c.data_selector = std::uint16_t{(3 << 3) | 7};
    Bytes mem(0x6000);
    for (std::size_t i = 0; i < mem.size(); ++i) mem[i] = static_cast<std::uint8_t>((i * 7) & 0xFF);
    const std::array<std::array<std::uint32_t, 3>, 4> entries = {{{0, 0, 0}, {0x100, 0xFFF, 0x92}, {0x1000, 0xFFF, 0x9A}, {0x2000, 0xFFF, 0x92}}};
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto d = raw_descriptor(entries[i][0], entries[i][1], static_cast<std::uint8_t>(entries[i][2]));
        std::copy(d.begin(), d.end(), mem.begin() + 0x4000 + 8 * static_cast<std::ptrdiff_t>(i));
    }
    mem[0x1020] = 0xCA;
    mem[0x1021] = 0x04;
    mem[0x1022] = 0x00;
    c.snapshot = RecordedMemory{mem, 0x4000, 8 * 8 - 1};
    c.key = "ctx2";
    return c;
}

// The options of each call, by the call's name.
std::optional<ProbeOptions> options_for(const std::string& name, const RecordedCall& ctx1, const RecordedCall& ctx2) {
    ProbeOptions o;
    auto ordinal_of = [&](const std::string& tail) {
        o.function = Ordinal{static_cast<std::uint32_t>(std::stoul(tail.substr(tail.rfind(' ') + 1)))};
    };
    if (name.find(" zero ordinal ") != std::string::npos) {
        ordinal_of(name);
        o.args = {0, 0, 0, 0};
    } else if (name.find(" words ordinal ") != std::string::npos) {
        ordinal_of(name);
        o.args = {1, 2, 3, 4, 5, 6};
    } else if (name.find("comm ports ordinal ") == 0) {
        ordinal_of(name);
        o.args = {0, 1, 2, 3};
        o.ports = true;
    } else if (name == "lz bad ordinal") {
        o.function = Ordinal{999};
    } else if (name == "lz near") {
        o.function = CodePoint{2, 610};
        o.near_entry = true;
        o.args = {1, 2};
    } else if (name == "lz callee") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.callees[{2, 610}] = {"LZStart", 4, false};
        o.callees[{4, 348}] = {"Other", 8, true};
    } else if (name == "lz unknown callee size") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.callees[{2, 610}] = {"LZStart", std::nullopt, true};
    } else if (name == "lz cdecl") {
        o.function = Ordinal{7};
        o.args = {1, 2, 3};
        o.cdecl_args = true;
    } else if (name == "lz stub_ret") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.stub_ret = 0x12345678;
    } else if (name == "lz data") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        for (std::uint8_t i = 0; i < 64; ++i) o.data_image.push_back(i);
        o.data_end = DataEnd{64, 0x300};
    } else if (name == "lz mem_init") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.mem_init.push_back({DataOffset{8}, to_bytes("hello")});
        o.mem_init.push_back({std::uint32_t{0x120000}, to_bytes("zz")});
        o.mem_init.push_back({DataOffset{0xFFFE}, to_bytes("toolong")});
    } else if (name == "lz bp") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.bp = std::uint16_t{0x1234};
    } else if (name == "lz cap") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.insn_cap = 40;
    } else if (name == "lz branches") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.branches = OffsetRange{0, 0x800};
    } else if (name == "lz branches 2") {
        o.function = Ordinal{7};
        o.args = {1, 2, 3, 4};
        o.branches = OffsetRange{0, 0x400};
    } else if (name == "lz prelude") {
        o.function = Ordinal{3};
        o.args = {0, 0, 0, 0};
        o.prelude.push_back({Ordinal{7}, {1}});
        o.prelude.push_back({CodePoint{2, 0}, {0, 0, 0, 0}});
    } else if (name == "lz context") {
        o.function = Ordinal{3};
        o.context = &ctx1;
    } else if (name == "lz context near") {
        o.function = CodePoint{2, 610};
        o.near_entry = true;
        o.context = &ctx1;
    } else if (name == "lz context snapshot") {
        o.function = Ordinal{3};
        o.context = &ctx2;
    } else if (name == "lz context snapshot callees") {
        o.function = Ordinal{2};
        o.context = &ctx2;
        o.callees[{2, 0}] = {"A", 4, true};
    } else {
        return std::nullopt;
    }
    return o;
}

void compare(const std::string& name, const ordered_json& want, const Trace16& t) {
    const std::string exit = want["exit"].get<std::string>();
    same(name, "exit", t.exit_text(), exit);
    if (exit == "bad-ordinal") return;
    same(name, "ret", t.ax, want["ret"].get<std::uint16_t>());
    same(name, "ret32", t.dx_ax, want["ret32"].get<std::uint32_t>());
    same(name, "insn_count", t.observed.insn_count, want["insn_count"].get<std::int64_t>());
    same(name, "truncated", t.observed.truncated, want["truncated"].get<bool>());
    same(name, "sp_delta", t.sp_delta, want["sp_delta"].get<std::uint16_t>());
    same(name, "n_imports", t.n_imports, want["n_imports"].get<std::size_t>());
    const ordered_json& er = want["entry_regs"];
    same(name, "entry AX", t.entry_regs.ax, er["AX"].get<std::uint16_t>());
    same(name, "entry BX", t.entry_regs.bx, er["BX"].get<std::uint16_t>());
    same(name, "entry CX", t.entry_regs.cx, er["CX"].get<std::uint16_t>());
    same(name, "entry DX", t.entry_regs.dx, er["DX"].get<std::uint16_t>());
    same(name, "entry SI", t.entry_regs.si, er["SI"].get<std::uint16_t>());
    same(name, "entry DI", t.entry_regs.di, er["DI"].get<std::uint16_t>());
    same(name, "entry BP", t.entry_regs.bp, er["BP"].get<std::uint16_t>());
    same(name, "entry CS", t.entry_regs.cs, er["CS"].get<std::uint16_t>());
    same(name, "entry DS", t.entry_regs.ds, er["DS"].get<std::uint16_t>());
    same(name, "entry ES", t.entry_regs.es, er["ES"].get<std::uint16_t>());
    same(name, "entry SS", t.entry_regs.ss, er["SS"].get<std::uint16_t>());
    const ordered_json& xr = want["exit_regs"];
    same(name, "exit SI", t.exit_regs.si, xr["SI"].get<std::uint16_t>());
    same(name, "exit DI", t.exit_regs.di, xr["DI"].get<std::uint16_t>());
    same(name, "exit DS", t.exit_regs.ds, xr["DS"].get<std::uint16_t>());

    const std::vector<std::uint64_t>& path = t.observed.path;
    same(name, "path length", path.size(), want["path_len"].get<std::size_t>());
    same(name, "path checksum", fold(path), want["path_fold"].get<std::uint64_t>());
    for (std::size_t i = 0; i < want["path_head"].size() && i < path.size(); ++i) same(name, "path head", path[i], want["path_head"][i].get<std::uint64_t>());

    std::vector<std::uint64_t> w;
    for (const native_trace::MemWrite& m : t.observed.mem_writes) w.insert(w.end(), {m.addr, m.size, m.value});
    same(name, "writes", t.observed.mem_writes.size(), want["writes_len"].get<std::size_t>());
    same(name, "writes checksum", fold(w), want["writes_fold"].get<std::uint64_t>());
    const ordered_json& head = want["writes_head"];
    for (std::size_t i = 0; i < head.size() && i < t.observed.mem_writes.size(); ++i) {
        same(name, "write address", t.observed.mem_writes[i].addr, head[i][0].get<std::uint64_t>());
        same(name, "write value", t.observed.mem_writes[i].value, head[i][2].get<std::uint64_t>());
    }
    std::vector<std::uint64_t> r;
    for (const native_trace::MemRead& m : t.observed.mem_reads) r.push_back(m.addr);
    same(name, "reads", t.observed.mem_reads.size(), want["reads_len"].get<std::size_t>());
    same(name, "reads checksum", fold(r), want["reads_fold"].get<std::uint64_t>());

    const ordered_json& hits = want["imports"];
    same(name, "imports hit", t.imports_hit.size(), hits.size());
    for (std::size_t i = 0; i < hits.size() && i < t.imports_hit.size(); ++i) {
        const ImportHit& h = t.imports_hit[i];
        const ordered_json& g = hits[i];
        same(name, "import api", h.api, g[0].get<std::string>());
        same(name, "import n", h.n, g[1].get<std::int64_t>());
        same(name, "import args", h.args_stack.size(), g[2].size());
        for (std::size_t k = 0; k < g[2].size() && k < h.args_stack.size(); ++k) same(name, "import arg", h.args_stack[k], g[2][k].get<std::int64_t>());
        if (!g[3].is_null()) same(name, "import ret_ip", h.ret_ip.value_or(-1), g[3].get<std::int64_t>());
        if (!g[4].is_null()) same(name, "import ret_cs", h.ret_cs.value_or(-1), g[4].get<std::int64_t>());
        same(name, "import arg_bytes", h.arg_bytes.has_value(), !g[5].is_null());
        if (!g[5].is_null()) same(name, "import arg_bytes value", h.arg_bytes.value_or(-1), g[5].get<std::int64_t>());
        same(name, "import unmodelled", h.unmodelled, g[6].get<bool>());
        if (!g[7].is_null()) same(name, "import kind", h.kind, g[7].get<std::string>());
        same(name, "import pointed", h.pointed.size(), g[9].size());
        for (std::size_t k = 0; k < g[9].size() && k < h.pointed.size(); ++k) {
            same(name, "import pointed at", h.pointed[k].first, g[9][k][0].get<std::size_t>());
            same(name, "import pointed bytes", std::string(as_text(h.pointed[k].second)), [&] {
                const std::string hex = g[9][k][1].get<std::string>();
                std::string s;
                for (std::size_t p = 0; p + 1 < hex.size(); p += 2) s.push_back(static_cast<char>(std::stoi(hex.substr(p, 2), nullptr, 16)));
                return s;
            }());
        }
    }
    const ordered_json& out = want["calls_out"];
    same(name, "calls out", t.calls_out.size(), out.size());
    for (std::size_t i = 0; i < out.size() && i < t.calls_out.size(); ++i) {
        same(name, "callee name", t.calls_out[i].name, out[i][0].get<std::string>());
        same(name, "callee args", t.calls_out[i].args.size(), out[i][1].size());
        same(name, "callee n", t.calls_out[i].n, out[i][3].get<std::int64_t>());
        same(name, "callee ret_ip", t.calls_out[i].ret_ip, out[i][4].get<std::uint16_t>());
    }
    same(name, "unsure", t.unsure, want["unsure"].is_null() ? false : want["unsure"].get<bool>());
    same(name, "interrupt", t.interrupt.has_value(), !want["interrupt"].is_null());
    same(name, "fault", t.fault.has_value(), !want["fault"].is_null());
    if (t.fault && !want["fault"].is_null()) {
        same(name, "fault cs", t.fault->cs, want["fault"]["cs"].get<std::uint32_t>());
        same(name, "fault eip", t.fault->eip, want["fault"]["eip"].get<std::uint32_t>());
        same(name, "fault bytes", t.fault->bytes, want["fault"]["bytes"].get<std::string>());
        same(name, "fault insn", t.fault->insn.value_or("(none)"), want["fault"]["insn"].is_null() ? std::string("(none)") : want["fault"]["insn"].get<std::string>());
    }
    same(name, "heap selectors", t.heap_sels.size(), want["heap_sels"].size());
    for (const ordered_json& h : want["heap_sels"]) {
        const auto it = t.heap_sels.find(h[0].get<std::uint16_t>());
        same(name, "heap selector present", it != t.heap_sels.end(), true);
        if (it != t.heap_sels.end()) same(name, "heap base", it->second, h[1].get<std::uint32_t>());
    }
    same(name, "lazy maps", t.observed.lazy_maps.size(), want["lazy_maps"].get<std::size_t>());
    same(name, "memops", t.observed.memops.value_or(-1), want["memops"].is_null() ? std::int64_t{-1} : want["memops"].get<std::int64_t>());
    same(name, "prelude ran", t.prelude_ran, want["prelude_ran"].is_null() ? std::size_t{0} : want["prelude_ran"].get<std::size_t>());
    const ordered_json& bases = want["segment_bases"];
    same(name, "segment count", t.layout.segment_bases.size(), bases.size());
    for (std::size_t i = 0; i < bases.size() && i < t.layout.segment_bases.size(); ++i) same(name, "segment base", t.layout.segment_bases[i], bases[i].get<std::uint32_t>());
    same(name, "data base", t.layout.data_base.value_or(0), want["data_base"].is_null() ? 0u : want["data_base"].get<std::uint32_t>());

    if (want.contains("branches")) {
        const ordered_json& b = want["branches"];
        same(name, "branch log present", t.branches.has_value(), true);
        if (t.branches) {
            same(name, "branches", t.branches->size(), b.size());
            for (std::size_t i = 0; i < b.size() && i < t.branches->size(); ++i) {
                const native_trace::BranchEntry& e = (*t.branches)[i];
                same(name, "branch site", e.address, b[i][0].get<std::uint64_t>());
                same(name, "branch taken", e.taken, b[i][1].get<bool>());
                same(name, "branch compare", e.compare.has_value(), !b[i][2].is_null());
                if (e.compare && !b[i][2].is_null()) {
                    const ordered_json& c = b[i][2];
                    same(name, "branch op", std::string(native_trace::to_text(e.compare->op)), c[0].get<std::string>());
                    same(name, "branch size", e.compare->size, c[1].get<std::uint64_t>());
                    same(name, "branch a", e.compare->a, c[2].get<std::uint64_t>());
                    same(name, "branch b", e.compare->b, c[3].get<std::uint64_t>());
                    same(name, "branch at_a", e.compare->at_a.has_value(), !c[4].is_null());
                    if (e.compare->at_a && !c[4].is_null()) same(name, "branch at_a value", *e.compare->at_a, c[4].get<std::uint64_t>());
                    same(name, "branch at_b", e.compare->at_b.has_value(), !c[5].is_null());
                    if (e.compare->at_b && !c[5].is_null()) same(name, "branch at_b value", *e.compare->at_b, c[5].get<std::uint64_t>());
                }
            }
        }
    }
    if (want.contains("io")) {
        const ordered_json& io = want["io"];
        same(name, "io present", t.io.has_value(), true);
        if (t.io) {
            same(name, "io entries", t.io->size(), io.size());
            for (std::size_t i = 0; i < io.size() && i < t.io->size(); ++i) {
                const ports16::LogEntry& e = (*t.io)[i];
                same(name, "io step", e.step, io[i][0].get<std::uint64_t>());
                const std::string kind = io[i][1].get<std::string>();
                same(name, "io kind", e.kind == ports16::Access::In ? std::string("in") : e.kind == ports16::Access::Out ? std::string("out") : std::string("..."), kind);
                same(name, "io port", e.port, io[i][2].get<std::uint16_t>());
                same(name, "io value", e.value, io[i][3].get<std::uint8_t>());
            }
        }
    }
}

}  // namespace

CB_TEST("emulate.windows16.descriptors_and_selectors", [] {
    // a present, writable, byte-granular data segment of 64K at 0x00123456
    const auto d = descriptor(0x00123456, 0xFFFF, false);
    CB_CHECK(d == (std::array<std::uint8_t, 8>{0xFF, 0xFF, 0x56, 0x34, 0x12, 0x92, 0x00, 0x00}));
    // code is 0x9A; the privilege level goes to bits 5-6 of the access byte; `big` sets the default-size flag; the base's top byte is last
    CB_CHECK(descriptor(0xAB000000, 0x1FFFF, true, 3, true) == (std::array<std::uint8_t, 8>{0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFA, 0x41, 0xAB}));
    CB_CHECK_EQ(selector(101), 0x328);
    CB_CHECK_EQ(selector(300, 3), (300 << 3) | 3);
    CB_CHECK_EQ(kStackSelector, selector(kStackSlot));
    CB_CHECK_EQ(kSafeSelector, selector(200));
    CB_CHECK_EQ(segment_base(1), 0x110000u);
    CB_CHECK_EQ(segment_base(12), 0x1C0000u);
});

CB_TEST("emulate.windows16.the_real_modules_run_as_recorded", [] {
    World w;
    const ordered_json& g = golden();
    const RecordedCall ctx1 = context_without_memory(), ctx2 = context_with_memory();
    std::size_t ran = 0;
    for (const ordered_json& c : g["cases"]) {
        const std::string name = c["name"].get<std::string>();
        const std::optional<ProbeOptions> options = options_for(name, ctx1, ctx2);
        if (!options) {
            testing::record_failure(__FILE__, __LINE__, "no options for the recorded case " + name);
            continue;
        }
        w.governor = {};
        const Trace16 t = w.runner.probe(module_path(c["module"].get<std::string>()), *options);
        compare(name, c["result"], t);
        ++ran;
    }
    CB_CHECK_EQ(ran, g["cases"].size());
    CB_CHECK(ran >= 100);
});

CB_TEST("emulate.windows16.the_data_segment_is_the_loaders", [] {
    World w;
    for (const auto& [which, want] : golden()["relocated"].items()) {
        const Bytes data = w.runner.relocated_data(module_path(which));
        std::vector<std::uint64_t> values(data.begin(), data.end());
        CB_CHECK_EQ(data.size(), want["len"].get<std::size_t>());
        CB_CHECK_EQ(fold(values), want["fold"].get<std::uint64_t>());
        CB_CHECK(w.runner.relocated_data(module_path(which)) == data);   // (the second answer is the kept one)
    }
});

CB_TEST("emulate.windows16.a_run_is_deterministic_and_leaves_the_module_alone", [] {
    World w;
    ProbeOptions o;
    o.function = Ordinal{3};
    o.args = {0, 0, 0, 0};
    const Trace16 a = w.runner.probe(module_path("lz"), o);
    const Trace16 b = w.runner.probe(module_path("lz"), o);   // from the module as kept after the first
    CB_CHECK(a.observed.path == b.observed.path);
    CB_CHECK_EQ(a.dx_ax, b.dx_ax);
    CB_CHECK_EQ(a.imports_hit.size(), b.imports_hit.size());
    CB_CHECK_EQ(a.sp_delta, b.sp_delta);
});

CB_TEST("emulate.windows16.trace_answers_the_imports_it_is_given_and_probe_the_models", [] {
    World w;
    TraceOptions o;
    o.function = Ordinal{3};
    o.args = {0, 0, 0, 0};
    std::vector<std::string> seen;
    o.import_stub = [&](ImportHit& hit, core16::Uc&) -> std::optional<StubResult> {
        seen.push_back(hit.api);
        hit.unmodelled = true;
        return StubResult{7, std::nullopt, 8};
    };
    const Trace16 t = w.runner.trace(module_path("lz"), o);
    CB_CHECK(!seen.empty());
    CB_CHECK_EQ(seen.size(), t.imports_hit.size());
    CB_CHECK(t.imports_hit.front().unmodelled);   // the stub's note stays in the record
    CB_CHECK_EQ(t.imports_hit.front().kind, "IMPORTORD");
    CB_CHECK_EQ(t.imports_hit.front().module.value_or(""), "KERNEL");
    // without a stub every import answers AX = 0 and removes nothing
    TraceOptions bare;
    bare.function = Ordinal{3};
    bare.args = {0, 0, 0, 0};
    const Trace16 u = w.runner.trace(module_path("lz"), bare);
    CB_CHECK(!u.imports_hit.empty());
    CB_CHECK(u.imports_hit.front().n > 0);
});

CB_TEST("emulate.windows16.heap_blocks_are_distinct_selectors_over_real_windows", [] {
    fs::ScopedTempDir dir("cb-windows16-heap");
    auto machine = unicorn_uc::open_x86(32);
    machine->mem_map(kGdtAddress, 0x1000);
    Heaps heaps(*machine);
    const std::uint16_t a = heaps.alloc_heap(), b = heaps.alloc_heap(0x2000);
    CB_CHECK(a != b);
    CB_CHECK_EQ(a, selector(kGdtSlots - 8));
    CB_CHECK_EQ(b, selector(kGdtSlots - 9));
    CB_CHECK_EQ(heaps.blocks().at(a).base, kHeapBase);
    CB_CHECK_EQ(heaps.blocks().at(b).base, kHeapBase + kHeapStride);
    CB_CHECK_EQ(heaps.blocks().at(b).size, 0x2000u);
    CB_CHECK_EQ(selector_base(*machine, a), kHeapBase);                 // the descriptor was written into the GDT
    CB_CHECK_EQ(selector_base(*machine, b), kHeapBase + kHeapStride);
    CB_CHECK_EQ(selector_base(*machine, 0xFFF8), 0u);                   // past the table: unreadable, 0
});

CB_TEST("emulate.windows16.default_stub_follows_its_model", [] {
    native_trace::Governor governor;
    Environment env;
    env.governor = &governor;
    env.models = [](std::string_view api) -> std::optional<StubModel> {
        if (api == "KERNEL.GLOBALALLOC") return StubModel{ModelWord{FromHeap{}}, ModelWord{std::int64_t{0}}, ModelWord{std::int64_t{6}}};
        if (api == "KERNEL.LSTRLEN") return StubModel{ModelWord{FromArgument{1}}, std::nullopt, ModelWord{std::int64_t{4}}};
        if (api == "KERNEL.FAR") return StubModel{ModelWord{FromArgument{40}}, std::nullopt, std::nullopt};
        return std::nullopt;
    };
    Runner runner(env);
    struct Counting : HeapContext {
        std::uint16_t alloc_heap(std::uint32_t) override { return 0x1234; }
    } heap;
    ImportHit hit;
    hit.api = "KERNEL.GLOBALALLOC";
    StubResult r = runner.default_stub(hit, heap);
    CB_CHECK_EQ(r.ax.value_or(0), 0x1234u);
    CB_CHECK_EQ(r.dx.value_or(9), 0u);
    CB_CHECK_EQ(r.pop.value_or(0), 6u);
    hit.api = "KERNEL.LSTRLEN";
    hit.args_stack = {10, 20, 30};
    r = runner.default_stub(hit, heap);
    CB_CHECK_EQ(r.ax.value_or(0), 20u);   // the Nth word as pushed
    CB_CHECK(!r.dx.has_value());
    hit.api = "KERNEL.FAR";
    CB_CHECK_EQ(runner.default_stub(hit, heap).ax.value_or(9), 0u);   // an argument that was not pushed is 0
    hit.api = "KERNEL.NOSUCH";
    r = runner.default_stub(hit, heap);
    CB_CHECK_EQ(r.ax.value_or(9), 0u);   // unmodelled: a benign 0, nothing removed
    CB_CHECK(!r.pop.has_value());
});

CB_TEST("emulate.windows16.the_knowledge_models_are_read_with_ordinals_and_all", [] {
    World w;
    const ModelLookup models = models_of(w.kb);
    const std::optional<StubModel> a = models("KERNEL.GLOBALLOCK");
    CB_CHECK(a.has_value());
    CB_CHECK(models("kernel.globallock").has_value());                 // capitals do not matter
    CB_CHECK(!models("KERNEL.NOSUCHFUNCTIONATALL").has_value());
    CB_CHECK(!models("USER.@999").has_value());
});

CB_TEST("emulate.windows16.the_models_say_which_calls_fill_a_structure_through_a_pointer", [] {
    World w;
    const ModelLookup models = models_of(w.kb);
    const std::optional<StubModel> paint = models("USER.BEGINPAINT");
    CB_CHECK(paint.has_value());
    CB_CHECK_EQ(paint->fills.size(), 1u);
    CB_CHECK_EQ(paint->fills[0].seg_arg, 1u);          // (hwnd, then the far pointer: its segment is the word 1 as pushed)
    CB_CHECK_EQ(paint->fills[0].bytes, 32u);           // a PAINTSTRUCT
    CB_CHECK(!paint->ax && !paint->dx && !paint->pop);   // the result is not modelled: the call stays `unmodelled`
    const std::optional<StubModel> range = models("USER.GETSCROLLRANGE");
    CB_CHECK(range.has_value());
    CB_CHECK_EQ(range->fills.size(), 2u);              // (two pointers: lpMin at 2, lpMax at 4, a word each)
    CB_CHECK_EQ(range->fills[1].seg_arg, 4u);
    CB_CHECK_EQ(range->fills[1].bytes, 2u);
    // a call that is only a fill answers like an unmodelled one
    native_trace::Governor governor;
    Environment env;
    env.governor = &governor;
    env.models = models;
    Runner runner(env);
    struct Heap : HeapContext {
        std::uint16_t alloc_heap(std::uint32_t) override { return 1; }
    } heap;
    ImportHit hit;
    hit.api = "USER.BEGINPAINT";
    const StubResult r = runner.default_stub(hit, heap);
    CB_CHECK_EQ(r.ax.value_or(9), 0u);
    CB_CHECK(!r.pop.has_value());
});

CB_TEST("emulate.windows16.errors", [] {
    native_trace::Governor governor;
    CB_CHECK_THROWS_AS(UsageError, (void)Runner(Environment{}));
    Environment env;
    env.governor = &governor;
    Runner runner(env);
    ProbeOptions o;
    o.function = Ordinal{1};
    CB_CHECK_THROWS_AS(NotFoundError, (void)runner.probe(testing::data_path("emulate/testdata/no-such-module.dll"), o));
    fs::ScopedTempDir dir("cb-windows16-bad");
    const sfs::path junk = dir.path() / "junk.dll";
    fs::write_file(junk, to_bytes(std::string(300, 'x')));
    CB_CHECK_THROWS_AS(FormatError, (void)runner.probe(junk, o));
    // a function in a segment the module does not have
    o.function = CodePoint{99, 0};
    CB_CHECK_THROWS_AS(UsageError, (void)runner.probe(module_path("lz"), o));
    // an ordinal the entry table does not have is a record, not an error
    o.function = Ordinal{9999};
    const Trace16 t = runner.probe(module_path("lz"), o);
    CB_CHECK(t.ending == Ending::BadOrdinal);
    CB_CHECK_EQ(t.exit_text(), "bad-ordinal");
    CB_CHECK_EQ(t.ordinal.value_or(0), 9999u);
    CB_CHECK_EQ(t.module, "lzexpand.dll");
    // a prelude call by an ordinal the table does not have
    o.function = Ordinal{3};
    o.prelude.push_back({Ordinal{9999}, {}});
    CB_CHECK_THROWS_AS(UsageError, (void)runner.probe(module_path("lz"), o));
    // an exception a stub raises comes out of the run
    TraceOptions t2;
    t2.function = Ordinal{3};
    t2.args = {0, 0, 0, 0};
    t2.import_stub = [](ImportHit&, core16::Uc&) -> std::optional<StubResult> { throw ParseError("stub", 1, 1, "stop"); };
    CB_CHECK_THROWS_AS(ParseError, (void)runner.trace(module_path("lz"), t2));
});

CB_TEST("emulate.windows16.the_run_record_carries_what_the_judge_compares", [] {
    World w;
    ProbeOptions o;
    o.function = Ordinal{3};
    o.args = {0, 0, 0, 0};
    const Run r = w.runner.recorded(module_path("lz"), o);
    CB_CHECK_EQ(r.exit.value_or(""), "ret");
    CB_CHECK_EQ(r.ended(), "ret");
    CB_CHECK(std::holds_alternative<std::uint64_t>(r.results.at("AX")));
    CB_CHECK_EQ(r.results.size(), std::size_t{2});
    CB_CHECK_EQ(r.calls.size(), std::size_t{4});
    CB_CHECK_EQ(r.calls.at(0).name.value_or(""), "KERNEL.@84");
    CB_CHECK(std::holds_alternative<ImportHit>(r.calls.at(0).detail));
    CB_CHECK_EQ(r.steps.value_or(0), std::uint64_t{100});
    CB_CHECK_EQ(r.stack.value_or(-1), 6);
    CB_CHECK_EQ(r.preserved->size(), std::size_t{3});
    CB_CHECK(r.kept());   // SI DI DS came back as they went in
    CB_CHECK(std::any_cast<std::shared_ptr<const Trace16>>(r.raw)->ax == 0);
    // the record of a bad ordinal has no result
    ProbeOptions bad;
    bad.function = Ordinal{9999};
    const Run b = w.runner.recorded(module_path("lz"), bad);
    CB_CHECK_EQ(b.exit.value_or(""), "bad-ordinal");
    CB_CHECK(std::holds_alternative<std::monostate>(b.results.at("AX")));
    CB_CHECK(!b.steps.has_value());
});

// ---- the shadow only watches ------------------------------------------------

namespace {

// pcodecheck as the pcode tests find it: the environment's, else native/decompiler/build/bin of the checkout (a worktree's build is in the main one).
std::filesystem::path w16shadow_pcodecheck() {
    if (auto env = fs::get_env("CB_PCODECHECK"); env && !env->empty()) return *env;
    sfs::path dir = sfs::path(CB_SOURCE_DIR);
    for (int i = 0; i < 6 && !dir.empty(); ++i, dir = dir.parent_path()) {
        const sfs::path exe = dir / "native" / "decompiler" / "build" / "bin" / "pcodecheck.exe";
        if (sfs::exists(exe)) return exe;
    }
    return {};
}

// What the judge compares of a run: its end, steps, the calls out, the writes, the registers it hands back.
std::string w16shadow_seen(const Run& r) {
    std::string out = std::format("{} {} ", r.exit.value_or("-"), r.steps.value_or(0));
    for (const Call& c : r.calls) out += c.name.value_or("?") + ";";
    out += " |";
    for (const MemoryWrite& w : r.writes) out += std::format(" {:x}/{}/{:x}", w.address, w.size, w.value);
    out += " |";
    for (const auto& [name, value] : r.results) {
        out += " " + name + "=";
        if (const auto* v = std::get_if<std::uint64_t>(&value)) out += std::format("{:x}", *v);
    }
    return out;
}

}  // namespace

// The shadow only watches: LZEXPAND's functions run with it and without it end alike, make the same calls and the same writes, in as many steps --
// also where the shadow meets a selector no descriptor names (LZCOPY, an unmodelled call answering 2: it once stopped the run there).
CB_TEST("emulate.windows16.a_run_with_its_shadow_is_the_same_run", [] {
    const sfs::path exe = w16shadow_pcodecheck();
    if (exe.empty()) {
        std::cerr << "windows16 shadow test: no pcodecheck (native/decompiler); skipped\n";
        return;
    }
    pcode::LifterSet lifters(pcode::PcodeConfig::from_environment(exe.parent_path(), sfs::path(CB_SOURCE_DIR) / "decompiler" / "languages"));
    if (lifters.get(pcode::Mode::X86Real16) == nullptr) {
        std::cerr << "windows16 shadow test: no x86 real-mode lift; skipped\n";
        return;
    }
    World w;
    const sfs::path path = module_path("lz");
    const formats::ne::Module module = formats::ne::Module::open_file(path);
    std::vector<formats::ne::Entry> entries;
    for (const formats::ne::Entry& e : module.entries()) {
        if (e.kind != formats::ne::Entry::Kind::Unused && e.segment != 0 && entries.size() < 8) entries.push_back(e);
    }
    CB_CHECK_EQ(entries.size(), std::size_t{8});
    const std::uint32_t ds = segment_base(module.header().autodata_segment != 0 ? module.header().autodata_segment : 1);
    struct Input {
        std::vector<std::uint16_t> args;
        std::optional<std::uint32_t> stub_ret;
        std::vector<MemoryInit> mem;
    };
    const std::uint16_t safe = kSafeSelector;
    const std::vector<Input> inputs = {{{0, 0, 0, 0, 0, 0, 0, 0}, std::nullopt, {}},
                                       {{1, 0x40, safe, 2, 0x80, safe, 3, 0x100}, std::nullopt, {}},
                                       {{0, 0, 0, 0, 0, 0, 0, 0}, 2u, {MemoryInit{DataOffset{50}, Bytes(6, 0)}}}};
    std::size_t compared = 0;
    for (const formats::ne::Entry& e : entries) {
        for (const Input& in : inputs) {
            std::vector<std::string> runs;
            for (const bool watched : {false, true}) {
                ProbeOptions o;
                o.function = CodePoint{e.segment, e.offset};
                o.args = in.args;
                o.near_entry = false;
                o.ports = true;
                o.stub_ret = in.stub_ret;
                o.mem_init = in.mem;
                w.governor = {};
                std::unique_ptr<native_shadow::Shadow> shadow;
                if (watched) {
                    shadow = std::make_unique<native_shadow::Shadow>(native_shadow::Setup::for_lifter(
                        native_shadow::Kind::Real16, *lifters.get(pcode::Mode::X86Real16), {{ds, ds + 0x10000ull}}, std::nullopt, false));
                    o.shadow = shadow.get();
                }
                runs.push_back(w16shadow_seen(w.runner.recorded(path, o)));
            }
            CB_CHECK_EQ(runs[0], runs[1]);
            ++compared;
        }
    }
    CB_CHECK_EQ(compared, std::size_t{24});
});
