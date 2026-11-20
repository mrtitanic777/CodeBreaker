// cb/source/commands.cpp -- the source writers' commands, assembled from the settings and the corpus (see commands.hpp).
// cb-unity: alone
#include "cb/source/commands.hpp"

#include <algorithm>
#include <format>
#include <mutex>
#include <optional>
#include <set>

#include "cb/build/dos_tools.hpp"
#include "cb/build/floppy.hpp"
#include "cb/build/masm.hpp"
#include "cb/build/module.hpp"
#include "cb/build/msvc152.hpp"
#include "cb/build/native_dos.hpp"
#include "cb/build/watcom.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/emu/dos.hpp"
#include "cb/exact/emu/native_dosexec.hpp"
#include "cb/engine/lane_kit.hpp"
#include "cb/source/dos_build.hpp"
#include "cb/source/dos_writer.hpp"
#include "cb/source/driver_writer.hpp"
#include "cb/source/ne_asm/datacode.hpp"
#include "cb/source/ne_asm/fixups.hpp"
#include "cb/source/ne_asm/idiom.hpp"
#include "cb/source/ne_asm/reachability.hpp"
#include "cb/source/ne_asm/recon.hpp"
#include "cb/source/ne_asm/run.hpp"
#include "cb/source/ne_asm/segdiff.hpp"
#include "cb/source/ne_asm/segment.hpp"
#include "cb/source/ne_asm/sweep.hpp"
#include "cb/source/vxd/boot.hpp"
#include "cb/source/vxd/coalesce.hpp"
#include "cb/source/vxd/corpus.hpp"
#include "cb/source/vxd/disassembly.hpp"
#include "cb/source/vxd/from_source.hpp"
#include "cb/source/vxd/idiom.hpp"
#include "cb/source/vxd/link.hpp"
#include "cb/source/vxd/loadable_corpus.hpp"
#include "cb/source/vxd/masm.hpp"
#include "cb/source/vxd/owbuild.hpp"
#include "cb/source/vxd/run.hpp"
#include "cb/source/vxd/tree.hpp"
#include "cb/symbols/debug_build.hpp"
#include "cb/symbols/debug_build_estimate.hpp"
#include "cb/symbols/driver_services.hpp"
#include "cb/verify/lanes/driver.hpp"

namespace cb::source::commands {

namespace stdfs = std::filesystem;

namespace {

// CodeBreaker's own DOS executive (exact/emu/dosexec, linked in, on Unicorn) behind the interface the assemblers and compilers take. When Unicorn is not there (or it is
// switched off, CB_NATIVE_DOSEXEC=0) it says it is not available, and those programs run under DOSBox-X instead.
class CommandsNativeDos final : public build::native_dos::DosExecutive {
public:
    explicit CommandsNativeDos(const config::Config& config)
        : opened_(exact::emu::native_dosexec::open(exact::emu::native_dosexec::locate(config), exact::emu::native_dosexec::limits_from_environment())),
          work_(config.state() / "native-dos-mem") {}

    bool available() const override { return opened_.usable(); }

    build::native_dos::Exit run(const stdfs::path& exe, const stdfs::path& work, std::string_view cmdline) const override {
        exact::emu::dos::RunSpec spec;
        spec.exe = exe;
        spec.workdir = work;
        spec.command_tail = std::string(cmdline);
        spec.environment = {'P', 'A', 'T', 'H', '=', 0};
        spec.fast = true;
        const exact::emu::dos::Result r = executive().run(spec);
        build::native_dos::Exit out;
        if (r.exit) out.code = static_cast<std::int32_t>(*r.exit);
        out.console = r.console;
        return out;
    }

    build::native_dos::MemoryResult run_dpmi(const build::native_dos::MemoryRun& run) const override {
        exact::emu::dos::MemFS files;   // a name only: nothing is written there
        for (const auto& [name, data] : run.files) files.put(work_ / name, data);
        exact::emu::dos::RunSpec spec;
        spec.exe = work_ / run.exe;
        spec.workdir = work_;
        spec.command_tail = run.cmdline;
        spec.environment = run.env;
        spec.fast = true;
        spec.dpmi = true;
        spec.files = &files;
        const exact::emu::dos::Result r = executive().run(spec);
        build::native_dos::MemoryResult out;
        for (const std::string& name : run.artifacts) {
            const stdfs::path at = work_ / text::upper(name);
            if (files.is_file(at) && files.files().contains(exact::emu::dos::MemFS::key(at))) out.made[name] = files.read(at);
        }
        out.console = r.console;
        return out;
    }

private:
    const exact::emu::dos::Executive& executive() const {
        if (!opened_.usable()) throw EmulationError("the DOS executive is not available here");
        return *opened_.executive;
    }
    exact::emu::native_dosexec::Opened opened_;
    stdfs::path work_;
};
bool commands_is_dir(const stdfs::path& p) {
    std::error_code ec;
    return !p.empty() && stdfs::is_directory(p, ec);
}

// The period ML.EXE (a Phar Lap TNT program) under DOS, in as few sessions as it can: the texts are grouped so that each session stays under
// `budget` characters and `max_files` files (a text over the budget runs alone); the cold start of the emulator is what a batch saves.
vxd::masm::PeriodAssembler commands_period_assembler(const build::DosRunner& runner, stdfs::path ml_folder, stdfs::path extender, stdfs::path stage) {
    return [&runner, ml_folder = std::move(ml_folder), extender = std::move(extender), stage = std::move(stage)](std::span<const std::string> texts) {
        constexpr std::size_t kBudget = 300000, kMaxFiles = 32;
        fs::make_dirs(stage);
        std::error_code ec;
        for (const auto& [from, name] : {std::pair<stdfs::path, const char*>{ml_folder / "ML.EXE", "ML.EXE"}, {ml_folder / "ML.ERR", "ML.ERR"},
                                         {extender, "DOSXNT.EXE"}})
            if (!stdfs::exists(stage / name, ec) && stdfs::exists(from, ec)) stdfs::copy_file(from, stage / name, ec);
        std::vector<vxd::masm::Assembled> results(texts.size());
        std::vector<std::size_t> order(texts.size());
        for (std::size_t i = 0; i < order.size(); ++i) order[i] = i;
        std::stable_sort(order.begin(), order.end(), [&](std::size_t a, std::size_t b) { return texts[a].size() < texts[b].size(); });
        std::vector<std::vector<std::size_t>> batches;
        std::vector<std::size_t> current;
        std::size_t current_size = 0;
        for (const std::size_t i : order) {
            if (texts[i].size() > kBudget) {
                batches.push_back({i});
                continue;
            }
            if (!current.empty() && (current_size + texts[i].size() > kBudget || current.size() >= kMaxFiles)) {
                batches.push_back(std::move(current));
                current.clear();
                current_size = 0;
            }
            current.push_back(i);
            current_size += texts[i].size();
        }
        if (!current.empty()) batches.push_back(std::move(current));
        for (const std::vector<std::size_t>& batch : batches) {
            build::DosRunSpec spec;
            spec.mounts = {{"c", stage}};
            std::size_t total = 0;
            for (std::size_t j = 0; j < batch.size(); ++j) {
                const std::string stem = std::format("m{}", j);
                spec.files[stem + ".asm"] = Bytes(texts[batch[j]].begin(), texts[batch[j]].end());
                spec.commands.push_back(std::format("c:\\ml.exe -coff -c -Fo{0}.obj {0}.asm > {0}.txt", stem));
                spec.artifacts.push_back(stem + ".obj");
                spec.artifacts.push_back(stem + ".txt");
                total += texts[batch[j]].size();
            }
            spec.timeout = std::chrono::seconds(std::min<std::size_t>(600, 90 + total / 400));
            const build::DosRunResult ran = runner.run(spec);
            for (std::size_t j = 0; j < batch.size(); ++j) {
                const std::string stem = std::format("m{}", j);
                if (const auto object = ran.made.find(stem + ".obj"); object != ran.made.end()) results[batch[j]].object = object->second;
                if (const auto log = ran.made.find(stem + ".txt"); log != ran.made.end()) results[batch[j]].log = text::from_latin1(log->second);
            }
            if (ran.work) {
                std::error_code ignored;
                stdfs::remove_all(*ran.work, ignored);
            }
        }
        return results;
    };
}

}  // namespace

// ---- what the commands are made from ----------------------------------------------------------------------------------------------

struct Production::Impl {
    config::Config config;
    std::shared_ptr<base::Corpus> corpus;
    std::mutex lock;

    Impl(config::Config c, std::shared_ptr<base::Corpus> k) : config(std::move(c)), corpus(std::move(k)) {}

    // The toolchains live in the main checkout of a worktree.
    stdfs::path tools_root() const {
        const stdfs::path own = config.checkout_root();
        return commands_is_dir(own / "toolchains") ? own : config.main_checkout();
    }
    std::optional<stdfs::path> design_dir() const {
        try {
            return corpus->design_dir();
        } catch (const Error&) {
            return std::nullopt;
        }
    }
    // The Windows 95 DDK (INC32, MSVC20, MASM611C, INC16): the reference folder's, else the sibling repository's.
    stdfs::path ddk_root() const {
        std::vector<stdfs::path> candidates;
        try {
            candidates.push_back(corpus->reference({"Microsoft Windows 95 DDK"}));
        } catch (const Error&) {
        }
        candidates.push_back(config.main_checkout().parent_path() / "Windows95" / "design" / "reference" / "Microsoft Windows 95 DDK");
        for (const stdfs::path& c : candidates)
            if (commands_is_dir(c)) return c;
        return candidates.front();
    }

    // ---- DOS ----
    struct Dos {
        build::dos_tools::DosboxRunner runner;
        CommandsNativeDos executive;
        explicit Dos(const config::Config& config) : runner(build::dos_tools::Emulator::from_config(config)), executive(config) {}
    };
    std::unique_ptr<Dos> dos_;
    Dos& dos() {
        std::lock_guard hold(lock);
        if (!dos_) dos_ = std::make_unique<Dos>(config);
        return *dos_;
    }

    // ---- the VxD tools ----
    struct Vxd {
        vxd::DdkFolders ddk;
        vxd::DdkNames names;
        vxd::masm::Toolchain tools;
        vxd::link::Environment standard_link;
        vxd::link::Environment linker_260;
        vxd::idiom::Environment idiom;
        vxd::tree::Environment tree;
        vxd::masm::Environment masm;
        vxd::Environment disassembly;
        vxd::corpus::Environment corpus;
        vxd::owbuild::Environment owbuild;
        vxd::owbuild::CommandEnvironment owbuild_command;
        vxd::loadable_corpus::Environment loadable_corpus;
        vxd::boot::Environment boot;

        Vxd(Impl& impl, Dos& dos)
            : ddk(vxd::DdkFolders::standard(*impl.corpus, impl.config.main_checkout().parent_path())),
              names(vxd::DdkNames::standard(ddk, impl.design_dir())),
              tools(vxd::masm::Toolchain::standard(ddk, impl.tools_root(), impl.config.text("nasm").empty() ? std::nullopt : std::optional<std::string>(impl.config.text("nasm")),
                                                   commands_period_assembler(dos.runner, impl.ddk_root() / "MASM611C",
                                                                             impl.tools_root() / "toolchains" / "period" / "compilers" / "msvc-1.52c" / "BIN" / "DOSXNT.EXE",
                                                                             impl.config.state("vxdtc") / "tnt"))),
              standard_link{names, tools, vxd::link::standard_link(impl.config.path("period_compiler").value_or(stdfs::path()) / "LINK.EXE")},
              linker_260{names, tools, vxd::link::link_260(impl.ddk_root() / "MSVC20", impl.config.state("cb_link260") / "stage", 0x1000)},
              idiom{*impl.corpus, names, tools, impl.config.state(stdfs::path("vxd") / "idiom")},
              tree{names, impl.config.state(stdfs::path("vxd") / "tree")},
              masm{names, tools},
              disassembly{*impl.corpus, names, impl.config.state(stdfs::path("vxd") / "listing")},
              corpus{names, tools, *impl.corpus, impl.config.state(stdfs::path("vxd") / "byte_exact.json")},
              owbuild{standard_link, vxd::owbuild::tools_from_settings(impl.config.checkout_root(), impl.config.text("openwatcom"), impl.config.path("nasm").value_or(stdfs::path()))},
              owbuild_command{owbuild, *impl.corpus, impl.config.state("vxd_ow")},
              loadable_corpus{linker_260, *impl.corpus, impl.config.state(stdfs::path("vxd") / "loadable.json")},
              boot{names, *impl.corpus, impl.config.state("vxdboot"), impl.config.path("qemu").value_or(stdfs::path(R"(C:\Program Files\qemu\qemu-system-i386.exe)")),
                   impl.config.number("min_free_gb").value_or(6.0),
                   impl.config.path("win95_installed_hd").value_or(stdfs::path(R"(\\wsl$\Ubuntu\home\mrtit\win95_install.img)"))} {}
    };
    std::unique_ptr<Vxd> vxd_;
    Vxd& vxd_world() {
        Dos& d = dos();
        std::lock_guard hold(lock);
        if (!vxd_) vxd_ = std::make_unique<Vxd>(*this, d);
        return *vxd_;
    }

    // ---- the 16-bit assembly tools ----
    struct NeAsm {
        build::masm::Assembler masm4;
        build::msvc152::Tools msvc;
        ne_asm::idiom::Environment idiom;
        ne_asm::sweep::Environment sweep;
        ne_asm::recon::Environment recon;
        ne_asm::segdiff::Environment segdiff;
        symbols::debug_build::Environment debug_names;
        symbols::debug_build_estimate::Environment debug_estimate;

        static build::masm::Context masm_context(Impl& impl, Dos& dos) {
            return build::masm::Context{build::masm::Tools::from_config(impl.config), build::masm::Options::from_environment(), dos.executive, dos.runner, nullptr, {}};
        }
        static ne_asm::idiom::Assemblers assemblers(Impl& impl, Dos& dos) {
            ne_asm::idiom::Assemblers a;
            if (const auto nasm = impl.config.path("nasm"); nasm && !nasm->empty())
                a.nasm = ne_asm::idiom::nasm_assembler(*nasm, impl.config.state("ne_idiom_nasm_inc") / "inc");
            a.ml = ne_asm::idiom::ml_assembler(dos.runner, {impl.ddk_root() / "MASM611C", impl.tools_root() / "toolchains" / "period" / "compilers" / "msvc-1.52c" / "BIN" / "DOSXNT.EXE",
                                                            impl.ddk_root() / "INC16", impl.config.state("vxdtc") / "ml"});
            return a;
        }
        NeAsm(Impl& impl, Dos& dos)
            : masm4(masm_context(impl, dos)),
              msvc{impl.config.path("dosbox_x").value_or(stdfs::path()), impl.config.path("msvc152").value_or(stdfs::path())},
              idiom{*impl.corpus, assemblers(impl, dos), impl.config.state()},
              sweep{*impl.corpus, assemblers(impl, dos), impl.config.state()},
              recon{*impl.corpus,
                    [this](std::string_view source) {
                        const build::dos_sessions::Product product = masm4.assemble(source);
                        return ne_asm::recon::Assembled{product.obj, product.lst.value_or(""), product.log.value_or("")};
                    }},
              segdiff{*impl.corpus,
                      [this, &dos](std::string_view c_source, std::string_view flags) -> Bytes {
                          const build::msvc152::Compiled compiled = build::msvc152::compile_c(dos.runner, msvc, c_source, flags);
                          if (!compiled.obj) throw ToolError("msvc152", 1, compiled.log.value_or("no object made"));
                          return *compiled.obj;
                      }},
              debug_names{*impl.corpus, impl.config.state()},
              debug_estimate{*impl.corpus, impl.config.state()} {}
    };
    std::unique_ptr<NeAsm> ne_asm_;
    NeAsm& ne_asm_world() {
        Dos& d = dos();
        std::lock_guard hold(lock);
        if (!ne_asm_) ne_asm_ = std::make_unique<NeAsm>(*this, d);
        return *ne_asm_;
    }

    // ---- the lane kit: the driver judge, and the module builder behind the Windows half of a combined file ----
    std::shared_ptr<engine::LaneKit> kit_;
    std::shared_ptr<engine::LaneKit> kit() {
        std::lock_guard hold(lock);
        if (!kit_) kit_ = std::make_shared<engine::LaneKit>(config, corpus);
        return kit_;
    }
    build::watcom::OpenWatcom open_watcom() const { return build::watcom::OpenWatcom::from_settings(config.text("openwatcom")); }
    stdfs::path probes() const { return config.state() / "assembly_probes.json"; }

    // The names of the kit's services, as the driver lane has them: from the knowledge base.
    std::shared_ptr<const symbols::driver_services::Table> service_names() {
        std::shared_ptr<engine::LaneKit> k = kit();
        knowledge::Knowledge& kb = k->knowledge();
        return std::make_shared<const symbols::driver_services::Table>(symbols::driver_services::Table::from_rows(
            kb.table("vxd", "device"), kb.table("vxd", "service_table"), kb.table("vxd", "driver"), kb.table("vxd", "message")));
    }
    std::function<std::optional<stdfs::path>(std::string_view)> retail() const {
        const std::shared_ptr<base::Corpus> c = corpus;
        return [c](std::string_view name) { return c->retail(name); };
    }
    stdfs::path state() const { return config.state(); }

    // dos-source: the program written and built; the driver of a driver+vxd file by the driver writer, the Windows module of a DOS+Windows file by the
    // module builder.
    dos_writer::Options dos_options() {
        dos_writer::Options options = dos_writer::options_for(open_watcom(), probes());
        const std::shared_ptr<const symbols::driver_services::Table> table = service_names();
        driver_writer::Options driver = driver_writer::options_for(open_watcom(), probes(), [table](std::uint32_t device, std::uint32_t service) { return table->label(device, service); });
        options.write_vxd = driver_writer::vxd_writer(std::move(driver));
        std::shared_ptr<engine::LaneKit> k = kit();
        options.build_windows = [k](const stdfs::path& program, const stdfs::path& outdir) {
            dos_writer::WindowsBuild w;
            try {
                const build::module::ModuleBuilder builder(k->module_services());
                build::module::BuildOptions build_options;
                build_options.outdir = outdir / "windows";
                build_options.log = [](std::string_view) {};
                const build::module::Built b = builder.build(fs::to_text(program), build_options);
                w.linked = b.linked();
                w.functions = b.functions;
                std::set<std::string> stand_ins(b.absent.begin(), b.absent.end());
                for (const auto& [name, why] : b.rejected) stand_ins.insert(name);
                w.stand_ins.assign(stand_ins.begin(), stand_ins.end());
                w.undefined = b.undefined;
                for (const auto& i : b.imports_extra) w.extra_imports.push_back(i.module + "." + i.text());
                w.ran = b.ran.size();
                for (const build::module::RanExport& r : b.ran) w.ran_same += r.same;
                w.error = b.error;
            } catch (const Error& e) {
                w.error = e.what();
            }
            return w;
        };
        return options;
    }
};

Production::Production(config::Config config, std::shared_ptr<base::Corpus> corpus) : impl_(std::make_shared<Impl>(std::move(config), std::move(corpus))) {}
Production::~Production() = default;

// ---- vxd --------------------------------------------------------------------------------------------------------------------------

base::commands::ToolTable Production::vxd_tools() const {
    const std::shared_ptr<Impl> i = impl_;
    base::commands::ToolTable tools;
    tools["idiom"] = [i](std::span<const std::string> a) { return vxd::idiom::main(i->vxd_world().idiom, a); };
    tools["tree"] = [i](std::span<const std::string> a) { return vxd::tree::main(i->vxd_world().tree, a); };
    tools["masm"] = [i](std::span<const std::string> a) { return vxd::masm::main(i->vxd_world().masm, a); };
    tools["link"] = [i](std::span<const std::string> a) { return vxd::link::main(i->vxd_world().standard_link, a); };
    tools["from-source"] = [i](std::span<const std::string> a) { return vxd::from_source::main(i->vxd_world().linker_260, a); };
    tools["owbuild"] = [i](std::span<const std::string> a) { return vxd::owbuild::main(i->vxd_world().owbuild_command, a); };
    tools["finalize"] = [](std::span<const std::string> a) { return vxd::coalesce::main(a); };
    tools["disassembly"] = [i](std::span<const std::string> a) { return vxd::main(i->vxd_world().disassembly, a); };
    tools["corpus"] = [i](std::span<const std::string> a) { return vxd::corpus::main(i->vxd_world().corpus, a); };
    tools["loadable-corpus"] = [i](std::span<const std::string> a) { return vxd::loadable_corpus::main(i->vxd_world().loadable_corpus, a); };
    tools["boot"] = [i](std::span<const std::string> a) { return vxd::boot::main(i->vxd_world().boot, a); };
    return tools;
}

Runner Production::vxd() const {
    return [tools = vxd_tools()](std::span<const std::string> args) { return vxd::run(args, tools); };
}

// ---- ne-asm -----------------------------------------------------------------------------------------------------------------------

base::commands::ToolTable Production::ne_asm_tools() const {
    const std::shared_ptr<Impl> i = impl_;
    base::commands::ToolTable tools;
    tools["idiom"] = [i](std::span<const std::string> a) { return ne_asm::idiom::main(i->ne_asm_world().idiom, a); };
    tools["sweep"] = [i](std::span<const std::string> a) { return ne_asm::sweep::main(i->ne_asm_world().sweep, a); };
    tools["recon"] = [i](std::span<const std::string> a) { return ne_asm::recon::main(i->ne_asm_world().recon, a); };
    tools["segdiff"] = [i](std::span<const std::string> a) { return ne_asm::segdiff::main(i->ne_asm_world().segdiff, a); };
    tools["reachability"] = [i](std::span<const std::string> a) { return ne_asm::reachability::main(*i->corpus, a); };
    tools["datacode"] = [i](std::span<const std::string> a) { return ne_asm::datacode::main(*i->corpus, a); };
    tools["segment"] = [](std::span<const std::string> a) { return ne_asm::segment::main(a); };
    tools["fixups"] = [](std::span<const std::string> a) { return ne_asm::fixups::main(a); };
    tools["debug-names"] = [i](std::span<const std::string> a) { return symbols::debug_build::main(i->ne_asm_world().debug_names, a); };
    tools["debug-names-estimate"] = [i](std::span<const std::string> a) { return symbols::debug_build_estimate::main(i->ne_asm_world().debug_estimate, a); };
    tools["msvc152"] = [i](std::span<const std::string> a) -> int {
        const cli::Command command{
            "msvc152",
            "compile with Microsoft C/C++ 8 (Visual C++ 1.52c) and show its listing",
            {cli::Positional{"csrc", "a C source file", false},
             cli::Option{"--flags", "", "the compiler's switches", cli::Arg::Value, "FLAGS", "/AM /c"},
             cli::Option{"--selftest", "", "compile two small functions", cli::Arg::Flag}},
            [&](const cli::Args& args) -> int {
                std::string source;
                if (args.flag("--selftest")) source = std::string(build::msvc152::kSelfTestSource);
                else if (const auto file = args.value("csrc")) source = fs::read_text(fs::from_text(*file));
                else throw UsageError("give a C file or --selftest");
                Impl::NeAsm& world = i->ne_asm_world();
                const build::msvc152::Compiled compiled = build::msvc152::compile_c(i->dos().runner, world.msvc, source, args.value_or("flags", "/AM /c"));
                out("{}\n", build::msvc152::report(compiled));
                return kExitOk;
            },
            {}, {}, {}};
        const cli::Cli program("msvc152", {command});
        std::vector<std::string> line = {"msvc152"};
        line.insert(line.end(), a.begin(), a.end());
        return program.run(line);
    };
    return tools;
}

Runner Production::ne_asm() const {
    return [tools = ne_asm_tools()](std::span<const std::string> args) { return ne_asm::run::main(tools, args); };
}

// ---- the writers of a driver and a real-mode program --------------------------------------------------------------------------------

Runner Production::driver_source() const {
    const std::shared_ptr<Impl> i = impl_;
    return [i](std::span<const std::string> args) {
        const std::shared_ptr<const symbols::driver_services::Table> table = i->service_names();
        driver_writer::Environment env;
        env.options = driver_writer::options_for(i->open_watcom(), i->probes(),
                                                 [table](std::uint32_t device, std::uint32_t service) { return table->label(device, service); });
        env.state = i->state();
        env.retail = i->retail();
        return driver_writer::command(args, env, [](std::string_view line) { out("{}\n", line); });
    };
}

Runner Production::dos_source() const {
    const std::shared_ptr<Impl> i = impl_;
    return [i](std::span<const std::string> args) {
        dos_writer::Environment env;
        env.options = i->dos_options();
        env.state = i->state();
        env.retail = i->retail();
        return dos_writer::command(args, env, [](std::string_view line) { out("{}\n", line); });
    };
}

cli::Command Production::dos_build() const {
    return dos_build::command(dos_writer::options_for(impl_->open_watcom(), impl_->probes()));
}

cli::Command Production::boot_floppy() const {
    build::floppy::Environment env;
    env.corpus_root = impl_->corpus->root();
    env.design_dir = impl_->design_dir().value_or(stdfs::path());
    env.state = impl_->state();
    if (const auto nasm = impl_->config.path("nasm")) env.nasm = *nasm;
    env.writer = impl_->dos_options();
    return build::floppy::command(env);
}

cli::Command Production::driver_judge() const {
    const std::shared_ptr<engine::LaneKit> kit = impl_->kit();
    const auto lane = std::static_pointer_cast<const verify::lanes::DriverLane>(kit->driver_lane());
    if (!lane) throw UsageError("the driver lane is not set up on this machine (its assembler and linker are not found)");
    cli::Command command = lane->command();
    const cli::Handler inner = command.handler;
    command.handler = [kit, lane, inner](const cli::Args& args) { return inner(args); };   // (the kit and the lane live as long as the command does)
    return command;
}

}  // namespace cb::source::commands