// codebreaker/decompile/decompiler.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/decompile/decompiler*.{hpp,cpp};
// this header keeps the old py:: surface for the ports and the dynamic exe that still use it (`decompile`, `as_c`, `main`, `run` ... with
// py containers, the old Description and the L2 documents as the old layer's Dict). Conversion and forwarding only.
//
// What the new decompiler asks of the rest of CodeBreaker (the symbol tables, the structure recovery, the PowerPC reader, the L2 library)
// is answered by cb/decompile/environment (no old port): `SharedEnvironment` below gives it the old layer's corpus and L2 library. c_text's functions are in
// c_text_impl.hpp, which this header ends by including: they run on the new decompiler through the same `Handle`.
//
// Not carried over: `timeout=` of the old runs (the runtime's subprocess had none); the L2 documents the decompiler makes are nodes of
// the arena of the `Handle` that made them and come out of here as Dicts.
#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <unordered_map>

#include "cb/base/cache.hpp"
#include "cb/base/corpus.hpp"
#include "cb/compat/legacy_settings.hpp"
#include "cb/compat/old_val_json.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/core/json.hpp"
#include "cb/decompile/decompiler.hpp"
#include "cb/decompile/environment.hpp"
#include "cb/knowledge/store.hpp"
#include "cb/translate/l2.hpp"
#include "cb/translate/native_l2.hpp"
#include "codebreaker/analyze/image.hpp"
#include "codebreaker/analyze/structures.hpp"
#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/converted.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/watcom.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/decompile/c_text.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/formats/pe.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/symbols/names.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/translate/native_l2.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::decompile::describe {
class Description;   // codebreaker/decompile/describe.hpp
}

// verify/lanes/win32.hpp, declared here for its one function: that header brings in the symbolic engine (z3), which a decompiler has no use for
namespace codebreaker::verify::lanes::win32 {
py::list<int64_t> functions(const py::str& lane, const py::str& path);   // sorted(pe_functions.carve(path).funcs)
}

namespace codebreaker::decompile::decompiler {

namespace newd = ::cb::decompile::decompiler;

using core16::FunctionNotes;
using core16::L2Doc;

// bump when what is fed to / taken from the native program changes
inline constexpr int64_t REV = newd::kRevision;
// instructions a function may have, tried again when the first limit stops it
inline constexpr int64_t LARGE = newd::kLargeInstructionLimit;

namespace shim_detail {

namespace nir = ::cb::translate::ir;
namespace nl2 = ::cb::translate::l2;
namespace native_l2_old = codebreaker::translate::native_l2;

// What the decompiler asks of the rest of CodeBreaker is answered by cb/decompile/environment (the symbol tables, the structure
// recovery, the PowerPC reader, over the corpus of the settings and a knowledge base of the process's own); this gives it the old
// layer's corpus, its L2 library and settings, and passes on what the description of a module said (the registries the builders read).
class SharedEnvironment {
public:
    ::cb::decompile::environment::CorpusEnvironment& get() { return *environment_; }

    // The one environment of the process, made again when the settings it was made from change.
    static std::shared_ptr<SharedEnvironment> current() {
        static std::mutex lock;
        static std::shared_ptr<SharedEnvironment> kept;
        const ::cb::base::Corpus& corpus = base::corpus::detail::instance();
        const std::filesystem::path state = ::cb::compat::legacy::state_dir();
        const std::filesystem::path openwatcom = ::cb::compat::legacy::path_of(build::watcom::openwatcom_root());
        std::lock_guard<std::mutex> hold(lock);
        if (!kept || kept->corpus_ != &corpus || kept->state_ != state || kept->openwatcom_ != openwatcom) kept = std::make_shared<SharedEnvironment>(corpus, state, openwatcom);
        return kept;
    }

    SharedEnvironment(const ::cb::base::Corpus& corpus, std::filesystem::path state, std::filesystem::path openwatcom)
        : corpus_(&corpus), state_(std::move(state)), openwatcom_(std::move(openwatcom)) {
        const std::filesystem::path checkout = ::cb::compat::legacy::path_of(py::settings::checkout_root());
        std::filesystem::path reference;
        try {
            reference = corpus.reference();
        } catch (const ::cb::Error&) {   // (no reference folder: the headers of the compilers alone)
        }
        store_ = std::make_unique<::cb::knowledge::Store>(::cb::knowledge::Store::default_path(state_), ::cb::knowledge::DataFolders::under(checkout));
        knowledge_ = std::make_unique<::cb::knowledge::Knowledge>(*store_, ::cb::symbols::prototypes::HeaderFolders::standard(openwatcom_, reference));
        environment_ = ::cb::decompile::environment::CorpusEnvironment::over_knowledge(corpus, codebreaker::translate::l2::shim_detail::library(), *knowledge_);
        // what the builders ask of a module that was decompiled: the imports it has by number, the functions that take an argument in ECX
        environment_->on_described = [](const std::filesystem::path& module, const ::cb::decompile::describe::PeDescription& d) {
            const py::str key(::cb::fs::to_text(module));
            py::set<py::str> ecx;
            for (const std::string& n : d.in_ecx) ecx.add(py::str(n));
            describe::IN_ECX.set(key, ecx);
            py::dict<py::str, std::tuple<py::str, int64_t, std::optional<int64_t>>> numbered;
            for (const auto& [name, imp] : d.by_number) numbered.set(py::str(name), std::make_tuple(py::str(imp.module), imp.number, imp.pop));
            describe::BY_NUMBER.set(key, numbered);
        };
    }

private:
    const ::cb::base::Corpus* corpus_;
    std::filesystem::path state_, openwatcom_;
    std::unique_ptr<::cb::knowledge::Store> store_;
    std::unique_ptr<::cb::knowledge::Knowledge> knowledge_;
    std::unique_ptr<::cb::decompile::environment::CorpusEnvironment> environment_;
};

// The settings as the old layer reads them: the native program, its languages, struct_recovery, threads.
inline newd::Settings settings() {
    newd::Settings s;
    s.checkout = ::cb::compat::legacy::path_of(py::settings::checkout_root());
    const std::optional<py::str> bin = py::settings::get_opt_str(py::str("decompiler_bin"));
    if (!bin) throw py::TypeError("expected str, bytes or os.PathLike object, not NoneType");
    s.program = ::cb::compat::legacy::path_of(py::os::path::join(*bin, py::str("decompiler.exe")));
    // (CB_LANGUAGES_DIR: another folder of language files -- an older decompiler's, run beside this one to compare the two)
    const py::str env = py::os::getenv(py::str("CB_LANGUAGES_DIR"), py::str(""));
    s.languages = ::cb::compat::legacy::path_of(!env.empty() ? env : py::os::path::join(py::settings::checkout_root(), py::str("native"), py::str("decompiler"), py::str("languages")));
    const std::optional<py::str> recovery = py::settings::get_opt_str(py::str("struct_recovery"));
    s.struct_recovery = recovery && !recovery->empty() ? recovery->utf8() : "off";
    // (the setting is "" unless set: a str, or a number)
    try {
        const std::optional<int64_t> i = py::settings::get_opt_int(py::str("threads"));
        if (i && *i) s.threads = std::to_string(*i);
    } catch (const py::Exception&) {
        s.threads = py::settings::get_str(py::str("threads")).utf8();
    }
    return s;
}

// A decompiler as the old layer makes one: from the settings, over the one cache and the old ports. One per use (it is cheap, and a harness
// that changes the settings between runs finds them changed).
class Handle {
public:
    Handle() : decompiler(make_context()) {}
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    std::shared_ptr<SharedEnvironment> environment = SharedEnvironment::current();
    newd::Decompiler decompiler;

private:
    newd::Context make_context() {
        newd::Context c;
        c.settings = settings();
        c.policy = newd::Policy::from_environment();
        {
            auto& state = base::cache::shim::state();
            std::lock_guard<std::mutex> hold(state.mutex);
            c.cache = &base::cache::shim::any_cache();
        }
        c.environment = &environment->get();
        c.limits = ::cb::compat::legacy::budget_limits();
        return c;
    }
};

// ---- the L2 documents, old Dict <-> node ---------------------------------------------------------------------------------------------

// A document the new module hands back comes out as a Dict; one that went in as a Dict is a node of the bridge's arena. (The old documents
// and the new ones are the same documents: a node made from a Dict is remembered with it.)
class Bridge {
public:
    nir::Node* node_for(const L2Doc& doc) {
        const ::cb::Json json = ::cb::compat::to_json_value(doc);
        std::lock_guard<std::mutex> hold(lock_);
        nir::Node* node = nl2::from_json(arena_, json);
        docs_[node] = doc;
        return node;
    }
    L2Doc dict_for(const nir::Node* node) {
        {
            std::lock_guard<std::mutex> hold(lock_);
            const auto it = docs_.find(node);
            if (it != docs_.end()) return it->second;
        }
        const ::cb::Json json = nl2::to_json(*node);
        L2Doc doc = std::get<L2Doc>(::cb::compat::from_json_value(json).v);
        std::lock_guard<std::mutex> hold(lock_);
        docs_[node] = doc;
        return doc;
    }

    ::cb::core16::FunctionNotes new_notes(const FunctionNotes& f) {
        ::cb::core16::FunctionNotes n;
        for (const core16::GlobalNote& g : f.globals.vec()) n.globals.push_back({g.address, g.bytes, g.elem});
        for (const auto& [name, bytes] : f.pointers.items()) n.pointers.insert_or_assign(name.utf8(), bytes);
        for (const core16::StructNote& s : f.structs.vec()) {
            ::cb::core16::StructNote note{s.name.utf8(), s.bytes, {}};
            for (const core16::StructField& field : s.fields.vec()) note.fields.push_back({field.offset, field.bytes, field.type.utf8()});
            n.structs.push_back(std::move(note));
        }
        if (f.l2) n.l2 = node_for(*f.l2);
        if (f.l2_exact) n.l2_exact_json = native_l2_old::detail_shim::wire_to_json(*f.l2_exact).dump();
        n.placed = f.placed;
        n.from_l2 = f.from_l2;
        return n;
    }
    FunctionNotes old_notes(const ::cb::core16::FunctionNotes& n) {
        FunctionNotes f;
        for (const ::cb::core16::GlobalNote& g : n.globals) {
            core16::GlobalNote o;
            o.address = g.address;
            o.bytes = g.bytes;
            o.elem = g.elem;
            f.globals.append(o);
        }
        for (const auto& [name, bytes] : n.pointers) f.pointers.set(py::str(name), bytes);
        for (const ::cb::core16::StructNote& s : n.structs) {
            core16::StructNote note;
            note.name = py::str(s.name);
            note.bytes = s.bytes;
            for (const ::cb::core16::StructField& field : s.fields) note.fields.append(core16::StructField{field.offset, field.bytes, py::str(field.type)});
            f.structs.append(note);
        }
        if (n.l2 != nullptr) f.l2 = dict_for(n.l2);
        if (n.l2_exact_json) f.l2_exact = native_l2_old::detail_shim::json_to_wire(::cb::Json::parse(*n.l2_exact_json));
        f.placed = n.placed;
        f.from_l2 = n.from_l2;
        return f;
    }

private:
    ::cb::Arena arena_;
    std::mutex lock_;
    std::unordered_map<const nir::Node*, L2Doc> docs_;
};

inline std::filesystem::path path_of(const py::str& s) { return ::cb::compat::legacy::path_of(s); }

inline std::vector<std::int64_t> vector_of(const py::list<int64_t>& l) { return l.vec(); }

inline py::dict<py::str, FunctionNotes> old_notes_of(Bridge& bridge, const std::map<std::string, ::cb::core16::FunctionNotes>& notes) {
    py::dict<py::str, FunctionNotes> out;
    for (const auto& [name, n] : notes) out.set(py::str(name), bridge.old_notes(n));
    return out;
}

}  // namespace shim_detail

// `<settings decompiler_bin>/decompiler.exe`
inline py::str _exe() { return py::str(::cb::fs::to_text(shim_detail::settings().program)); }

// CB_LANGUAGES_DIR, else native/decompiler/languages under the project root
inline py::str _languages() { return py::str(::cb::fs::to_text(shim_detail::settings().languages)); }

// "1" when the native program is to recover structures for this description (its CB_STRUCTS), else "0"; the setting `struct_recovery`:
// "off", "32" (32-bit x86 only) or "all". `description` is the description file's path.
inline py::str _structs(const py::str& description) {
    shim_detail::Handle h;
    return py::str(h.decompiler.structs_for(shim_detail::path_of(description)));
}

// The native program and its languages are there
inline bool available() {
    shim_detail::Handle h;
    return h.decompiler.available();
}

// CB_FRAME16=1: a 16-bit function's locals laid out as the original's frame (decompile/frame16.py, shape16.py)
inline bool frame16_on() { return newd::Policy::from_environment().frame16; }

// Whether a 16-bit function's L2 tree is given passes that read its code (CB_ASM_BLOCKS, CB_FRAME16)
inline bool tree_images() { return newd::Policy::from_environment().tree_images(); }

// {segment: bytes} of a 16-bit module's code, from its description file's `bytes` lines
inline py::dict<int64_t, py::bytes> _segment_images(const py::str& description) {
    return ::cb::compat::guard([&] {
        py::dict<int64_t, py::bytes> out;
        for (const auto& [seg, bytes] : newd::images_in_description(shim_detail::path_of(description))) out.set(seg, ::cb::compat::to_py(::cb::ByteView(bytes)));
        return out;
    });
}

// A block of the native program's output: `/*@@ <name> <hex address> */` and the text after it, up to the next
// (tests/test_verification reaches in)
inline const py::re::Pattern& _BLOCK() {
    static const py::re::Pattern p = py::re::compile("(?ms)^/\\*@@ \\S+ ([0-9a-f]+) \\*/\\n.*?(?=^/\\*@@ |\\Z)");
    return p;
}

// (the C, its facts) of one function as the native program wrote it (the /*@G ... */ notes taken off its front)
inline std::tuple<py::str, FunctionNotes> split_notes(const py::str& text) {
    return ::cb::compat::guard([&] {
        ::cb::Arena arena;
        shim_detail::Bridge bridge;
        const newd::Split s = newd::split_notes(arena, text.utf8());
        return std::make_tuple(py::str(s.text), bridge.old_notes(s.notes));
    });
}

// run()'s (texts, order, by_va, stderr) -- the texts C alone -- and `notes`: {name: its facts}.
struct Decompiled {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    py::str stderr_;
    py::dict<py::str, FunctionNotes> notes;
};

namespace shim_detail {
inline Decompiled old_decompiled(Bridge& bridge, const newd::Run& r) {
    Decompiled out;
    for (const auto& [name, text] : r.texts) out.texts.set(py::str(name), py::str(text));
    out.order = ::cb::compat::to_py(r.order);
    for (const auto& [va, name] : r.by_va) out.by_va.set(va, py::str(name));
    out.stderr_ = py::str(r.stderr_text);
    out.notes = old_notes_of(bridge, r.notes);
    return out;
}
}  // namespace shim_detail

// A function's C written by the hub's C back end from its L2 document (facts.l2) in place of the C the decompiler printed (`text`).
// Without a document, or one the back end declines, the printed C stays. Sets facts.from_l2 (and facts.l2/placed when `images` gives the
// passes that read the code). `images` {segment: code}; `text` None is c_text's: the answer is then the C only when written (from_l2),
// else None. (`module`, what the module says of the function, is for the new c_text, which is the only caller that gives one.)
inline std::optional<py::str> c_from_l2(const std::optional<py::str>& text, FunctionNotes& facts,
                                        const std::optional<py::dict<int64_t, py::bytes>>& images = std::nullopt) {
    return ::cb::compat::guard([&]() -> std::optional<py::str> {
        shim_detail::Handle h;
        shim_detail::Bridge bridge;
        ::cb::core16::FunctionNotes notes = bridge.new_notes(facts);
        std::optional<::cb::decompile::c_text::SegmentImages> seen;
        if (images) {
            seen = ::cb::decompile::c_text::SegmentImages();
            for (const auto& [seg, bytes] : images->items()) (*seen)[seg] = ::cb::compat::from_py(bytes);
        }
        const std::optional<std::string> answer = h.decompiler.c_from_l2(text ? std::optional<std::string>(text->utf8()) : std::nullopt, notes, seen, nullptr);
        if (notes.from_l2) {   // (the back end's passes may have made another document of it)
            if (notes.l2 != nullptr) facts.l2 = bridge.dict_for(notes.l2);
            if (notes.l2_exact_json) facts.l2_exact = shim_detail::native_l2_old::detail_shim::json_to_wire(::cb::Json::parse(*notes.l2_exact_json));
        }
        facts.placed = notes.placed;
        facts.from_l2 = notes.from_l2;
        return answer ? std::optional<py::str>(py::str(*answer)) : std::nullopt;
    });
}

// As _run, the notes taken apart from the C (split_notes). `description` is the description file's path; `only` limits it to those
// addresses. With `l2` (None: the CB_C_FROM_L2 environment variable) each function's C is the hub's C back end's, from its L2 (c_from_l2).
// `write`=false: the L2 documents kept and the printed C handed on.
inline Decompiled run(const py::str& description, const py::list<int64_t>& only = py::list<int64_t>(), int64_t timeout = 3600,
                      const std::optional<int64_t>& max_instructions = std::nullopt, const std::optional<bool>& l2 = std::nullopt,
                      bool write = true) {
    (void)timeout;
    return ::cb::compat::guard([&] {
        shim_detail::Handle h;
        shim_detail::Bridge bridge;
        const newd::Run r = h.decompiler.run(shim_detail::path_of(description), only.vec(), newd::RunOptions{max_instructions, l2, write});
        return shim_detail::old_decompiled(bridge, r);
    });
}

// The language files' folder digest (cache.folder(_languages()))
inline py::str _language_stamp() {
    static std::optional<py::str> stamp;
    if (!stamp) stamp = base::cache::folder(_languages());
    return *stamp;
}

// The version of what makes decompiled C, as the cache keeps its results apart: (REV, the native program's digest, its languages' digest)
inline std::tuple<int64_t, py::str, py::str> generation() {
    // (not those of CB_LANGUAGES_DIR: a run comparing another folder of them shares the generation of now)
    py::str languages = py::os::getenv(py::str("CB_LANGUAGES_DIR"), py::str("")).empty()
                            ? _language_stamp()
                            : base::cache::folder(py::os::path::join(py::settings::checkout_root(), py::str("native"), py::str("decompiler"), py::str("languages")));
    return {REV, base::cache::tool(_exe()), languages};
}

// The native program's own output for a PE module, before it is made compilable (`l2`, `write`: run's).
inline Decompiled raw(const py::str& module_path, const py::list<int64_t>& funcs, const py::str& outdir, int64_t timeout = 3600,
                      const std::optional<bool>& l2 = std::nullopt, bool write = true) {
    (void)timeout;
    return ::cb::compat::guard([&] {
        shim_detail::Handle h;
        shim_detail::Bridge bridge;
        const newd::Decompiler::ModuleRun r = h.decompiler.run_module(shim_detail::path_of(module_path), funcs.vec(), shim_detail::path_of(outdir),
                                                                       newd::RunOptions{std::nullopt, l2, write});
        return shim_detail::old_decompiled(bridge, r.run);
    });
}

// Decompile a module (every function, or the addresses in `funcs`). -> the Module (an l2.Made with imports ...)
inline c_text::Module32 decompile(const py::str& module_path, const std::optional<py::list<int64_t>>& funcs0 = std::nullopt,
                                  const std::optional<py::str>& outdir0 = std::nullopt) {
    py::list<int64_t> funcs = funcs0 ? *funcs0 : verify::lanes::win32::functions(py::str("pe"), module_path);
    py::str stem = py::os::path::basename(module_path).lower();
    py::str outdir = outdir0 ? *outdir0 : build::watcom::_scratch(py::list<py::str>(std::vector<py::str>{py::str("decompiler"), stem}));
    c_text::Compilable32 c = c_text::compilable32(module_path, funcs, outdir);
    // (one L2 Module: each function a MethodDecl with its C; the imports it calls, and those of its functions that take their first
    // argument in ECX -- and their second in EDX -- beside it; each function's L2 document, and its declaration as the back end wrote
    // its heading from it)
    c_text::Module32 m(stem, c.preamble, c.texts, c.order, c.by_va);
    m.imports = c.imports;
    m.in_ecx = c.in_ecx;
    m.in_edx = c.in_edx;
    m.l2 = c.docs;
    m.declarations = c.declarations;
    return m;
}

// ---- 16-bit modules -----------------------------------------------------------------------------------------------------------------
// The addresses of places a function calls that are not known as functions yet (func_0x...): from its L2 document's calls, or without
// one (None) from its text
inline py::set<int64_t> _placeholders(const py::str& text, const std::optional<L2Doc>& doc) {
    return ::cb::compat::guard([&] {
        shim_detail::Bridge bridge;
        py::set<int64_t> out;
        for (const int64_t va : newd::placeholders(text.utf8(), doc ? bridge.node_for(*doc) : nullptr)) out.add(va);
        return out;
    });
}
// The number of arguments each call to `name` passes: in its L2 document's calls, or without one in `text`
inline py::list<int64_t> _call_arguments(const py::str& text, const py::str& name, const std::optional<L2Doc>& doc = std::nullopt) {
    return ::cb::compat::guard([&] {
        shim_detail::Bridge bridge;
        py::list<int64_t> out;
        for (const int64_t n : newd::call_arguments(text.utf8(), name.utf8(), doc ? bridge.node_for(*doc) : nullptr)) out.append(n);
        return out;
    });
}
// Does the function call `name`?
inline bool _calls(const py::str& text, const std::optional<L2Doc>& doc, const py::str& name) {
    return ::cb::compat::guard([&] {
        shim_detail::Bridge bridge;
        return newd::calls(text.utf8(), doc ? bridge.node_for(*doc) : nullptr, name.utf8());
    });
}
// (count, [bytes of each, "2" | "4" | "4p"]) of the arguments a function declares -- in its L2 document (facts.l2), or without one in its
// own text -- or None
inline std::optional<std::tuple<int64_t, py::list<py::str>>> _own_arguments(const py::str& text, const py::str& name,
                                                                            const std::optional<FunctionNotes>& facts = std::nullopt) {
    return ::cb::compat::guard([&]() -> std::optional<std::tuple<int64_t, py::list<py::str>>> {
        shim_detail::Bridge bridge;
        ::cb::core16::FunctionNotes notes;
        if (facts) notes = bridge.new_notes(*facts);
        const std::optional<newd::ArgumentSizes> own = newd::own_arguments(codebreaker::translate::l2::shim_detail::library(), text.utf8(), name.utf8(), facts ? notes.l2 : nullptr,
                                                                           facts ? &notes.pointers : nullptr);
        if (!own) return std::nullopt;
        return std::make_tuple(static_cast<int64_t>(own->sizes.size()), ::cb::compat::to_py(own->sizes));
    });
}

// decompile16()'s answer: ({name: text}, [names], {address: name}, Description)
struct Decompiled16 {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    py::dict<int64_t, py::str> by_va;
    std::shared_ptr<describe::Description> description;
};

// Decompile a 16-bit Windows module. Function starts come first from what the file states; decompiling then shows calls to places not yet
// known as functions (`func_0x...`); those are added and the module is done again. Sets description->notes ({name: its facts}).
inline Decompiled16 decompile16(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt, int64_t rounds = 4) {
    return ::cb::compat::guard([&] {
        shim_detail::Handle h;
        shim_detail::Bridge bridge;
        const ::cb::decompile::c_text::Decompiled16 got = h.decompiler.decompile16(shim_detail::path_of(module_path),
                                                                         outdir ? std::optional<std::filesystem::path>(shim_detail::path_of(*outdir)) : std::nullopt,
                                                                         static_cast<int>(rounds));
        auto d = std::make_shared<describe::Description>(describe::shim_detail::from_new(*got.description));
        d->impl = got.description;
        d->notes = shim_detail::old_notes_of(bridge, got.description->notes);
        Decompiled16 out;
        for (const auto& [name, text] : got.texts) out.texts.set(py::str(name), py::str(text));
        out.order = ::cb::compat::to_py(got.order);
        for (const auto& [va, name] : got.by_va) out.by_va.set(va, py::str(name));
        out.description = d;
        return out;
    });
}

// One virtual device driver member: (member name, {name: text}, [names], Description)
struct LeMember {
    py::str member;
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    std::shared_ptr<describe::Description> description;
};
// Decompile a virtual device driver file: one LE image, or every member of a container.
inline py::list<LeMember> decompile_le(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt) {
    return ::cb::compat::guard([&] {
        shim_detail::Handle h;
        const std::vector<newd::LeMember> got = h.decompiler.decompile_le(shim_detail::path_of(module_path),
                                                                          outdir ? std::optional<std::filesystem::path>(shim_detail::path_of(*outdir)) : std::nullopt);
        py::list<LeMember> results;
        for (const newd::LeMember& m : got) {
            auto d = std::make_shared<describe::Description>(py::str(m.description->path.string()), ::cb::compat::to_py(m.description->names),
                                                              py::dict<int64_t, core16::SegOff>(), py::list<py::str>());
            for (const auto& [obj, span] : m.description->spans) d->spans.set(obj, std::make_tuple(span.first, span.second));
            d->device = py::str(m.description->device);
            for (const std::string& l : m.description->lines) d->lines.append(py::str(l));
            LeMember out;
            out.member = py::str(m.member);
            for (const auto& [name, text] : m.functions.texts) out.texts.set(py::str(name), py::str(text));
            out.order = ::cb::compat::to_py(m.functions.order);
            out.description = d;
            results.append(out);
        }
        return results;
    });
}

// decompile_dos()'s answer: ({name: text}, [names], Description)
struct DosDecompiled {
    py::dict<py::str, py::str> texts;
    py::list<py::str> order;
    std::shared_ptr<describe::Description> description;
};
// Decompile a DOS program.
inline DosDecompiled decompile_dos(const py::str& module_path, const std::optional<py::str>& outdir = std::nullopt) {
    return ::cb::compat::guard([&] {
        shim_detail::Handle h;
        const newd::DosProgramC got = h.decompiler.decompile_dos(shim_detail::path_of(module_path),
                                                                  outdir ? std::optional<std::filesystem::path>(shim_detail::path_of(*outdir)) : std::nullopt);
        // the old description (the program, its analysis, its interrupt calls), with what growing the module added to it
        const py::str folder = py::str(got.description->path.parent_path().string());
        std::shared_ptr<describe::Description> d = describe::describe_dos(module_path, folder);
        d->names = ::cb::compat::to_py(got.description->names);
        d->lines = ::cb::compat::to_py(got.description->lines);
        DosDecompiled out;
        for (const auto& [name, text] : got.functions.texts) out.texts.set(py::str(name), py::str(text));
        out.order = ::cb::compat::to_py(got.functions.order);
        out.description = d;
        return out;
    });
}

// A PE module is x86 (machine 0x014C)
inline bool _is_x86(const py::str& module_path) {
    return ::cb::compat::guard([&] { return ::cb::formats::pe::Image::open(shim_detail::path_of(module_path)).header().machine == 0x014C; });
}

// Any executable as C: (text, functions decompiled, functions found). Raises py::ValueError for a file that is not an executable this can
// read.
inline std::tuple<py::str, int64_t, int64_t> as_c(const py::str& path) {
    std::optional<std::string> refused;
    const auto answer = ::cb::compat::guard([&] {
        shim_detail::Handle h;
        try {
            const newd::AsC c = h.decompiler.as_c(shim_detail::path_of(path));
            return std::make_tuple(py::str(c.text), static_cast<int64_t>(c.decompiled), static_cast<int64_t>(c.found));
        } catch (const ::cb::UsageError& e) {
            refused = e.what();
            return std::make_tuple(py::str(""), int64_t(0), int64_t(0));
        }
    });
    if (refused) throw py::ValueError(*refused);
    return answer;
}

// `python -m codebreaker decompile <module> [--out FILE]`: the exit code
inline int64_t main(const py::list<py::str>& argv) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    std::optional<std::string> refused;
    const int64_t code = ::cb::compat::guard([&] {
        shim_detail::Handle h;
        const ::cb::base::converted::Store store(::cb::compat::legacy::path_of(base::converted::root()));
        try {
            return static_cast<int64_t>(newd::command(h.decompiler, base::corpus::detail::instance(), store, args));
        } catch (const ::cb::UsageError& e) {
            refused = e.what();
            return int64_t(1);
        }
    });
    if (refused) throw py::SystemExit(py::str(*refused));
    return code;
}

}  // namespace codebreaker::decompile::decompiler

// What the functions of c_text that need the decompiler do (see c_text.hpp): defined once both headers are.
#include "codebreaker/decompile/c_text_impl.hpp"
