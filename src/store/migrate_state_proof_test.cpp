// cb-unity: alone (test helpers named for this module)
// The proof of `migrate-state` on a COPY of a real state folder, run by hand:
//
//     set CB_MIGRATE_PROOF_STATE=<copy of state: functions.sqlite and db\*.sqlite of the modules named below, db\knowledge.sqlite>
//     set CB_MIGRATE_PROOF_MODULES=lz32.dll,lzexpand.dll
//     set CB_MIGRATE_PROOF_WCC=<wcc386.exe>            (the compiler the 32-bit lane's profile names)
//     set CB_MIGRATE_PROOF_DECOMPILER_BIN=<folder with pcodecheck.exe>   CB_MIGRATE_PROOF_LANGUAGES=<folder of the .sla files>
//     set CB_MIGRATE_PROOF_PYTHON_PE=<the 32-bit digest the Python has in force>
//     cb_tests_store --filter store.migrate_state_proof
//
// It reads each module the way the native commands do (store/versions_ledger over the function store, the name store over the knowledge base) before and after,
// and prints the numbers. Without the variables it does nothing: the folder is the person's, never a test's.
#include <format>
#include <iostream>

#include "base/corpus.hpp"
#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "emulate/pcode.hpp"
#include "knowledge/store.hpp"
#include "store/functions.hpp"
#include "store/knowledge_bodies.hpp"
#include "store/migrate_state.hpp"
#include "store/versions_ledger.hpp"
#include "verify/lanes/win32.hpp"

namespace mg = cb::store::migrate;
namespace stdfs = std::filesystem;

namespace {

std::optional<std::string> variable(const char* name) {
    const std::optional<std::string> v = cb::fs::get_env(name);
    return v && !v->empty() ? v : std::nullopt;
}

namespace c_text = cb::decompile::c_text;

// The decompiler is not asked by a profile.
class NoDecompilerServices final : public c_text::Services {
public:
    [[noreturn]] static void no() { throw cb::UsageError("the decompiler is not part of this proof"); }
    c_text::Decompiled raw(const stdfs::path&, std::span<const c_text::Va>, const stdfs::path&) override { no(); }
    c_text::Decompiled16 decompile16(const stdfs::path&, const std::optional<stdfs::path>&) override { no(); }
    std::optional<std::string> c_from_l2(const std::optional<std::string>&, cb::core16::FunctionNotes&, const std::optional<c_text::SegmentImages>&,
                                         c_text::ModuleFacts*) override { no(); }
    bool recovers_structures(const cb::decompile::describe::NeDescription&) override { no(); }
    bool tree_images() override { no(); }
    c_text::SegmentImages segment_images(const cb::decompile::describe::NeDescription&) override { no(); }
    const cb::translate::native_l2::Library& library() override { no(); }
    std::map<std::string, std::string> import_dlls(const stdfs::path&) override { no(); }
    std::optional<std::uint32_t> argument_bytes(std::string_view, std::string_view) override { no(); }
    std::optional<std::vector<cb::decompile::struct_text::Structure>> recover_structures(const stdfs::path&) override { no(); }
    cb::decompile::describe::PowerPcFacts powerpc_facts(const stdfs::path&) override { no(); }
};

std::string counts_text(const std::map<cb::verify::Class, std::int64_t>& tally) {
    std::string out;
    for (const auto& [cls, n] : tally)
        if (n != 0) out += std::format("{} {}  ", cb::verify::word(cls), n);
    return out.empty() ? "nothing judged" : out;
}

}  // namespace

CB_TEST("store.migrate_state_proof.a_copy_of_the_real_state", [] {
    const std::optional<std::string> state_text = variable("CB_MIGRATE_PROOF_STATE");
    if (!state_text) return;
    const stdfs::path state = cb::fs::from_text(*state_text);
    const std::string python_pe = variable("CB_MIGRATE_PROOF_PYTHON_PE").value_or("");

    // the 32-bit lane's profile as a program with the lifters and the compiler of this machine has it
    cb::emulate::pcode::LifterSet lifters(cb::emulate::pcode::PcodeConfig::from_environment(cb::fs::from_text(variable("CB_MIGRATE_PROOF_DECOMPILER_BIN").value_or("")),
                                                                                           cb::fs::from_text(variable("CB_MIGRATE_PROOF_LANGUAGES").value_or(""))));
    NoDecompilerServices decompiler;
    cb::build::functions::FunctionBuilder builder;
    cb::verify::lanes::Win32Lane::Services services{decompiler, builder, {}, {}, nullptr, nullptr, &lifters, {}, nullptr};
    services.tools.wcc.exe = cb::fs::from_text(variable("CB_MIGRATE_PROOF_WCC").value_or(""));
    const cb::verify::lanes::Win32Lane lane(services);
    const cb::verify::lanes::Profile native = lane.profile({});
    std::cout << "native pe profile: " << native.digest << "\n  " << native.settings.substr(0, 160) << " ...\n";
    mg::Services migrate_services{.state_dir = state, .verifier_profiles = [&native] {
                                      return std::map<std::string, mg::Verifier>{{"pe", {native.digest, native.settings}}};
                                  }};

    cb::base::Corpus corpus(cb::base::Products{});
    const auto read_all = [&](const char* when) {
        auto store = cb::store::functions::open(state, std::nullopt, true);
        cb::knowledge::Store kb(cb::knowledge::Store::default_path(state), cb::knowledge::DataFolders::from_environment());
        cb::store::KnowledgeBodies bodies(kb);
        cb::store::VersionsLedger ledger({.corpus = corpus, .store = *store, .bodies = &bodies});
        std::cout << "\n== " << when << "\n";
        const std::string wanted = variable("CB_MIGRATE_PROOF_MODULES").value_or("");
        for (const auto& row : store->module_rows()) {
            if (wanted.find(row.name) == std::string::npos) continue;
            std::map<cb::verify::Class, std::int64_t> by_class;
            for (const auto& [address, known] : ledger.best({row.sha, native.digest}))
                if (const auto cls = cb::verify::parse_class(known.klass)) ++by_class[*cls];
            std::cout << row.name << " (" << row.kind << ", " << row.sha.substr(0, 12) << ")\n";
            std::cout << "  versions_ledger best() under the native digest: " << counts_text(by_class) << "\n";
            std::cout << "  tally under the native digest:                  " << counts_text(store->tally(native.digest, row.sha)) << "\n";
            if (!python_pe.empty() && row.kind == "pe")
                std::cout << "  tally under the Python's digest " << python_pe << ": " << counts_text(store->tally(python_pe, row.sha)) << "\n";
            const auto digests = ledger.bodies(row.sha);
            std::size_t own_names = 0;
            for (const auto& body : kb.bodies(row.sha)) own_names += body.name.has_value() && !body.name->empty() ? 1 : 0;
            std::cout << "  bodies in the knowledge base: " << digests.size() << " (" << own_names << " named by the module itself), names its bodies have from other modules or by hand: "
                      << bodies.names_from_bodies(row.sha).size() << "\n";
        }
    };
    read_all("before");
    const mg::Report dry = mg::migrate(migrate_services, {.dry = true});
    std::cout << "\n";
    for (const std::string& line : mg::render(dry)) std::cout << line << "\n";
    const mg::Report done = mg::migrate(migrate_services, {});
    std::cout << "\n";
    for (const std::string& line : mg::render(done)) std::cout << line << "\n";
    read_all("after");
    const mg::Report again = mg::migrate(migrate_services, {.dry = true});
    std::cout << "\nrun again (dry): " << again.verdicts.modules_changed << " module(s) to change, " << again.knowledge.modules << " knowledge module(s), "
              << again.knowledge.learned << " learned row(s)\n";
});
