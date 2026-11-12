// codebreaker/symbols/runtime_library.cpp -- the shim's translation unit (see runtime_library.hpp). The new module
// (cb/symbols/runtime_library) does the matching, the naming and the guessing; this file gives it the module the old format
// readers and the corpus make of a path, the signature rows of the knowledge base, and converts types.

#include "codebreaker/symbols/runtime_library.hpp"

#include <algorithm>
#include <memory>
#include <mutex>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/symbols/runtime_library.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/flat_thunk.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/pe_functions.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/reads.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/ordinals.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::symbols::runtime_library {

namespace {

namespace rl = cb::symbols::runtime_library;
using cb::compat::from_py;
using cb::compat::to_py;

// The thread's decoders (a decoder belongs to one thread).
cb::platforms::DecoderSet& decoders() {
    thread_local cb::platforms::DecoderSet set;
    return set;
}

// The signature tables of the knowledge base, made once ("32" and "16") and again after learn() rewrites the file.
std::mutex g_tables_lock;
std::shared_ptr<const rl::SignatureTable> g_table32, g_table16;

std::shared_ptr<const rl::SignatureTable> table_for(int64_t bits) {
    std::lock_guard<std::mutex> hold(g_tables_lock);
    std::shared_ptr<const rl::SignatureTable>& slot = bits == 16 ? g_table16 : g_table32;
    if (!slot) {
        const py::str platform(bits == 16 ? "win16" : "win32");
        knowledge::reads::record_kind(platform, py::str("signature"));
        std::vector<rl::SignatureRow> rows;
        for (const auto& kv : knowledge::_table(platform, py::str("signature")).items()) {
            if (!std::holds_alternative<knowledge::sources::Json>(std::get<1>(kv))) continue;
            const knowledge::sources::Json& row = std::get<knowledge::sources::Json>(std::get<1>(kv));   // lead, size, mask, digest, name, library
            rows.push_back({row.list.at(0).s.utf8(), static_cast<std::size_t>(row.list.at(1).i), row.list.at(2).s.utf8(),
                            row.list.at(3).s.utf8(), row.list.at(4).s.utf8(), row.list.at(5).s.utf8()});
        }
        slot = std::make_shared<const rl::SignatureTable>(rl::SignatureTable::from_rows(rows));
    }
    return slot;
}

cb::ByteView view_of(const py::bytes& b) { return cb::as_bytes(b.raw()); }

std::optional<std::tuple<py::str, py::str>> to_old(const std::optional<rl::Identified>& hit) {
    if (!hit) return std::nullopt;
    return std::make_tuple(to_py(hit->name), to_py(hit->library));
}

// The function starts of a PE module, ascending.
std::vector<std::uint64_t> starts_of(const py::str& module_path) {
    std::vector<std::uint64_t> starts;
    for (int64_t k : formats::pe_functions::carve(module_path)->funcs.keys()) starts.push_back(static_cast<std::uint64_t>(k));
    std::sort(starts.begin(), starts.end());
    return starts;
}

std::vector<rl::CodeRegion> regions_of(const py::list<std::tuple<uint64_t, py::bytes, py::str>>& sections) {
    std::vector<rl::CodeRegion> out;
    for (const auto& s : sections) out.push_back({std::get<0>(s), view_of(std::get<1>(s))});
    return out;
}

bool is_flat_thunk(const py::str& module_path) {
    try {
        return build::flat_thunk::classify(module_path).subtype == py::str("flat-thunk");
    } catch (const std::exception&) {   // (not readable as one: not one)
        return false;
    }
}

std::map<std::uint32_t, std::string> ordinal_names_of(const py::str& stem) {
    std::map<std::uint32_t, std::string> out;
    for (const auto& kv : ordinals::table(stem).items()) out[static_cast<std::uint32_t>(std::get<0>(kv))] = std::get<1>(kv).utf8();
    return out;
}

}  // namespace

// modules that are a runtime library themselves: stem -> what their exports are
const py::dict<py::str, py::str> ITSELF = [] {
    py::dict<py::str, py::str> d;
    for (const auto& [stem, what] : rl::runtime_libraries()) d.set(to_py(stem), to_py(what));
    return d;
}();

const py::str THUNK_COMPILER = py::str(std::string(rl::kThunkCompiler));

// the instructions every function's frame is made of (symbols.names counts with it; rl::is_frame_instruction is the same test)
const py::re::Pattern _FRAME(
    "(push|pop) (e?bp|e?si|e?di|e?bx|ds|es)$|mov (e?bp, e?sp|e?sp, e?bp|ds, ax|ax, ds|es, ax)$|(inc|dec) bp$|nop|leave|"
    "ret|retf|mov (e?ax|e?dx), (0x)?[0-9a-f]+$|xor (e?ax, e?ax|e?dx, e?dx)$|(sub|add) (e?sp|bp), |lea sp, |int3",
    0);

std::optional<std::tuple<int64_t, py::str, py::str>> signature(const py::bytes& code, const py::bytes& mask) {
    const auto sig = rl::signature(view_of(code), view_of(mask));
    if (!sig) return std::nullopt;
    return std::make_tuple(static_cast<int64_t>(sig->size), to_py(sig->mask_text), to_py(sig->digest));
}

std::optional<std::tuple<py::str, py::str>> match(int64_t bits, const py::bytes& code) {
    knowledge::reads::record_kind(py::str(bits == 16 ? "win16" : "win32"), py::str("signature"));
    return to_old(table_for(bits)->match(view_of(code)));
}

py::str _c_name(const py::str& symbol) { return to_py(rl::c_name(from_py(symbol))); }

py::list<int64_t> _flat_thunk_stubs(const py::str& module_path, const formats::pe::PEFile& pe,
                                    const py::list<std::tuple<uint64_t, py::bytes, py::str>>& sections) {
    (void)pe;
    py::list<int64_t> out;
    if (!is_flat_thunk(module_path)) return out;
    const std::vector<rl::CodeRegion> code = regions_of(sections);
    const std::vector<std::uint64_t> starts = starts_of(module_path);
    for (const std::uint64_t va : rl::flat_thunk_stubs(code, starts, decoders())) out.append(static_cast<int64_t>(va));
    return out;
}

py::dict<int64_t, std::tuple<py::str, py::str>> identify(const py::str& module_path) {
    return cb::compat::guard([&] {
        std::optional<py::str> lane = base::corpus::lane(module_path);
        py::str stem = std::get<0>(py::os::path::splitext(py::os::path::basename(module_path))).upper();
        py::dict<int64_t, std::tuple<py::str, py::str>> out;
        if (lane && *lane == py::str("pe")) {
            formats::pe::PEFile pe = formats::pe::open_pe(module_path);
            if (pe.machine != 0x014C) return out;
            const auto sections = pe.code_sections();
            rl::Pe32Module m;
            m.stem = stem.utf8();
            m.image_base = pe.image_base;
            m.code = regions_of(sections);
            m.function_starts = starts_of(module_path);
            m.flat_thunk = is_flat_thunk(module_path);
            for (const auto& e : pe.exports()) {
                const std::optional<py::str>& name = std::get<1>(e);
                m.exports.push_back({static_cast<std::uint32_t>(std::get<0>(e)), name ? std::optional<std::string>(name->utf8()) : std::nullopt,
                                     static_cast<std::uint32_t>(std::get<2>(e))});
            }
            if (rl::runtime_library_of(m.stem) != nullptr) m.ordinal_names = ordinal_names_of(stem);
            for (const auto& [va, hit] : rl::identify_pe(*table_for(32), m, decoders())) out.set(static_cast<int64_t>(va), to_old(hit).value());
        } else if (lane && *lane == py::str("ne")) {
            formats::ne::NEFile nef(module_path);
            std::vector<py::bytes> bytes;   // each segment's file bytes (kept for the views)
            rl::Ne16Module m;
            m.module_name = nef.module_name().utf8();
            for (const auto& s : nef.segments()) bytes.push_back(nef.data.slice(s.file_off, s.file_off + s.length));
            std::size_t k = 0;
            for (const auto& s : nef.segments()) m.segments.push_back({static_cast<std::uint16_t>(s.idx), s.kind == py::str("CODE"), view_of(bytes[k++])});
            std::vector<decompile::describe::SegOff> funcs;
            for (const auto& so : decompile::describe::functions(nef).keys()) funcs.push_back(so);
            std::sort(funcs.begin(), funcs.end());
            for (const auto& so : funcs) m.function_starts.push_back({static_cast<std::uint16_t>(std::get<0>(so)), static_cast<std::uint16_t>(std::get<1>(so))});
            for (const auto& e : nef.entries()) {
                if ((e.kind == py::str("FIXED") || e.kind == py::str("MOVEABLE")) && e.seg)
                    m.entries.push_back({static_cast<std::uint16_t>(e.ordinal), static_cast<std::uint16_t>(*e.seg), static_cast<std::uint16_t>(e.offset.value_or(0))});
            }
            const py::str own = nef.module_name().upper();
            if (rl::runtime_library_of(own.utf8()) != nullptr) m.ordinal_names = ordinal_names_of(own);
            for (const auto& [where, hit] : rl::identify_ne(*table_for(16), m))
                out.set(decompile::describe::address(where.segment, where.offset), to_old(hit).value());
        }
        return out;
    });
}

namespace {

// "major.minor" as the PE header's linker version is given
std::pair<uint32_t, uint32_t> version_of(const py::str& text) {
    py::list<py::str> parts = py::split(text, py::str("."));
    return {static_cast<uint32_t>(py::int_parse(parts[0], 10)), static_cast<uint32_t>(py::int_parse(parts[1], 10))};
}

}  // namespace

Compiler compiler(const py::str& module_path, const std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>>& identified) {
    return cb::compat::guard([&] {
        std::optional<py::str> lane = base::corpus::lane(module_path);
        const py::bytes data = py::open_rb(module_path).read();
        rl::CompilerFacts facts;
        facts.file = view_of(data);
        if (lane && *lane == py::str("pe")) {
            formats::pe::PEFile pe = formats::pe::open_pe(module_path);
            facts.kind = rl::CompilerFacts::Kind::Pe;
            std::tie(facts.linker_major, facts.linker_minor) = version_of(pe.build_meta().linker_version);
            facts.pe_header_offset = static_cast<std::uint32_t>(pe.e_lfanew);
            for (const auto& kv : pe.imports().items()) facts.imported_modules.push_back(std::get<0>(std::get<1>(kv)).utf8());
        } else if (lane && *lane == py::str("ne")) {
            formats::ne::NEFile nef(module_path);
            facts.kind = rl::CompilerFacts::Kind::Ne;
            facts.linker_major = static_cast<std::uint32_t>(nef.h.linker_ver);
            facts.linker_minor = static_cast<std::uint32_t>(nef.h.linker_rev);
            for (const py::str& r : nef.module_refs()) facts.module_references.push_back(r.utf8());
        }
        const py::dict<int64_t, std::tuple<py::str, py::str>> found = identified ? *identified : identify(module_path);
        std::vector<std::string> libraries;
        for (const auto& kv : found.items()) libraries.push_back(std::get<1>(std::get<1>(kv)).utf8());
        const rl::CompilerInfo info = rl::describe_compiler(facts, libraries);
        Compiler c;
        c.linker = to_py(info.linker);
        c.compiler = to_py(info.compiler);
        for (const std::string& line : info.evidence) c.evidence.append(to_py(line));
        return c;
    });
}

int64_t substance(int64_t bits, const py::bytes& code, int64_t size) {
    return static_cast<int64_t>(rl::substance(decoders(), bits == 32 ? rl::Bits::X32 : rl::Bits::X16, view_of(code), static_cast<std::size_t>(std::max<int64_t>(size, 0))));
}

py::dict<py::str, int64_t> learn(const py::list<std::tuple<py::str, py::str>>& sources, const std::function<void(const py::str&)>& log0) {
    return cb::compat::guard([&] {
        std::function<void(const py::str&)> log = log0 ? log0 : [](const py::str& s) { py::print(s); };
        std::vector<rl::LearnSource> given;
        for (const auto& src : sources) given.push_back({from_py(std::get<0>(src)), cb::fs::from_text(from_py(std::get<1>(src)))});
        const rl::Learned learned = rl::learn(given, decoders(), [&](const std::string& line) { log(to_py(line)); });
        const py::str file = py::os::path::join(knowledge::sources::_SYMBOLS(), py::str("runtime_signatures.json"));
        cb::fs::write_text(cb::fs::from_text(from_py(file)), rl::format_signature_file(learned));
        knowledge::sources::changed(file);                         // the knowledge base loads it again
        {
            std::lock_guard<std::mutex> hold(g_tables_lock);
            g_table32.reset();
            g_table16.reset();
        }
        py::dict<py::str, int64_t> counts;
        counts.set(py::str("32"), static_cast<int64_t>(learned.count(rl::Bits::X32)));
        counts.set(py::str("16"), static_cast<int64_t>(learned.count(rl::Bits::X16)));
        return counts;
    });
}

int64_t main(const py::list<py::str>& argv) {
    // python -m codebreaker identify <module>                  its runtime-library functions and its compiler
    // python -m codebreaker identify --learn "WHAT=FILE" ...   make the signatures from library files
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker identify <module>                  its runtime-library functions and its compiler\n"
                          "    python -m codebreaker identify --learn \"WHAT=FILE\" ...   make the signatures from library files"));
        return 2;
    }
    if (argv[0] == py::str("--learn")) {
        py::list<std::tuple<py::str, py::str>> sources;
        for (int64_t i = 1; i < argv.size(); i++) {
            if (argv[i].contains(py::str("="))) {
                auto [a, sep, b] = argv[i].partition(py::str("="));
                sources.append({a, b});
            }
        }
        py::dict<py::str, int64_t> r = learn(sources);
        py::print(py::percent(py::str("%d 32-bit and %d 16-bit signatures"), r.at(py::str("32")), r.at(py::str("16"))));
        return 0;
    }
    std::optional<py::str> path = base::corpus::retail(argv[0]);
    if (!path || path->empty()) throw py::SystemExit(py::percent(py::str("no module %r in the corpus"), argv[0]));
    auto found = identify(*path);
    std::optional<py::str> lane = base::corpus::lane(*path);
    std::vector<int64_t> addrs;
    for (int64_t a : found.keys()) addrs.push_back(a);
    std::sort(addrs.begin(), addrs.end());
    for (int64_t address : addrs) {
        py::str name = std::get<0>(found.at(address)), library = std::get<1>(found.at(address));
        py::str where;
        if (lane && *lane == py::str("ne")) {
            auto so = decompile::describe::where(address);
            where = py::percent(py::str("%d:%04x"), std::get<0>(so), std::get<1>(so));
        } else {
            where = py::percent(py::str("%08x"), address);
        }
        py::print(py::percent(py::str("%s  %-40s %s"), where, name.slice(0, 40), library));
    }
    int64_t total = 0;
    if (lane && *lane == py::str("ne")) total = decompile::describe::functions(formats::ne::NEFile(*path)).size();
    else if (lane && *lane == py::str("pe")) total = decompile::describe::function_starts(*path).size();
    Compiler info = compiler(*path, found);
    py::print(py::percent(py::str("%s: %d of %d functions are a runtime library's"), py::os::path::basename(*path).lower(),
                          static_cast<int64_t>(found.size()), total));
    py::print(py::percent(py::str("built with: %s (linker %s)"), info.compiler, info.linker.empty() ? py::str("?") : info.linker));
    for (const py::str& line : info.evidence) py::print(py::str("    ") + line);
    return 0;
}

}  // namespace codebreaker::symbols::runtime_library
