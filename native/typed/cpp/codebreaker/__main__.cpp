// codebreaker/__main__.cpp -- typed C++ port of codebreaker.__main__ (see __main__.hpp).
#include "codebreaker/__main__.hpp"

#include <io.h>

#include <cstdio>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/ui/mcp_server.hpp"

namespace codebreaker::main_module {

// ---- the docstring and COMMANDS (generated from __main__.py) ----------------------------------------------
// (generated from codebreaker/__main__.py: the module's docstring and COMMANDS, as they are there)

const std::string& DOC_TEXT() {
    static const std::string d = std::string()
        + std::string(R"DOC(python -m codebreaker <command> [arguments]
python -m codebreaker               with no command, in a terminal: the terminal interface (ui)

    ui [module]                     CodeBreaker in the command prompt: the corpus, a module's functions,
                                    their C, assembly, calls and verdicts, jobs followed live

    module <name> [--no-verify]     reconstruct one module: decompile, build, judge, write its source
    scoreboard                      rewrite the scoreboard from stored results
    place <module> ... | --all      where each module's source goes in the source tree
    sweep [options]                 reconstruct and judge every module (resumable)
    decompile <name> [--out FILE]   decompile any module (32-bit, 16-bit, PowerPC) to C; nothing is built
    measure <name>                  reconstruct and judge one module; writes nothing to the tree
    match <module> [--try]          optional: does a period compiler give the original's bytes back?
    repair <module>                 move each function toward the authored bytes (behaviour-preserving)
    teeth <module>                  break functions the judge passed, on purpose: does it notice?
    invert <16-bit module>          read functions back into the source their compiler was given (compiler models)
    vxd <tool> ...                  a driver as the driver kit's macro source; build and compare it
    ne-asm <tool> ...               16-bit hand-written assembly as cMacros source; build and compare it
    build16 <source folder>         a 16-bit DLL built with Microsoft C 6 and LINK 5.60, compared by segment
    flat-thunk <module> [--recover DIR] [--build DIR] [--ow-build DIR]
                                    a 32-to-16-bit thunk DLL (lz32 and its kind): its source recovered,
                                    built with the thunk compiler, ML, CL and LINK, and compared
    names <module>                  every function and import with its best name and where it came from
    identify <module>               runtime-library functions of a module, and what built it
    demangle <name>                 a decorated C++ name as its declaration
    media list|extract <image>      files of a floppy, hard-disk or CD image
    dos-functions <program>         functions found in a real-mode program, by evidence
    dos-interrupts [int [AH]]       name a DOS or BIOS service
    analyze <module> [--what a,b]   call graph, conventions, globals, strings, structures, classes, dispatch
    driver-services [FILE ...]      names of ring-0 services; how many of a driver's calls they name
    driver-source <driver>          a driver's assembly source from its image, built and checked
    dos-source <program>            a real-mode program's assembly source, built and compared
    dos-build <folder> [--against F]  that program made again from its source folder alone
    pe-source <program> [--stub F]  a 32-bit Windows program's assembly source (NASM), built and
                                    compared byte for byte
    ne-source <program>             a 16-bit Windows / OS/2 program's assembly source (NASM), built and
                                    compared byte for byte
    le-source <driver>              a VxD's (or VMM32.VXD's) file as assembly source (NASM), built and
                                    compared byte for byte
    origin <module> [--list]        which of a 16-bit module's functions were written in assembly
    boot-floppy                     the Win95 Setup boot floppy, made from the rebuilt files
    driver-build <folder or .asm>   assemble and link a driver, check it against the original
    driver-judge <rebuilt> <orig>   run every entry point of both drivers and compare
    build <module> [--out DIR]      build a whole module (functions, data, resources, exports) and check it
    resources <module> [--out DIR]  a module's resources as .rc and files, compiled and compared
    cab list|extract|create ...     Microsoft cabinet files
    disc <folder> <out.iso>         write a CD image (also --list, --extract)
    versions <module> --from A --to B   match the functions of two builds: identical, changed, added, removed
    fonts FILE [--show TEXT]        list and draw the raster fonts in a .FON/.FNT
    info <file-or-module>           what an executable is and holds (headers, imports, exports, resources)
    xex <command> <file.xex> ...    an Xbox 360 executable: info, list, extract the basefile, convert,
                                    patch, re-sign (any command takes a .xex: it is read as its basefile)
    recompile <image or .xex> --all [-o FILE]   an Xbox 360 program as C++ that runs on a PC (every
                                    function; --at ADDR --size N for one; --check compiles it)
    recompile-project <title> [--check [s]]     an Xbox 360 title (a .xex, package, disc image or folder)
                                    as a project that builds and runs on a PC (docs/RECOMPILE.md)
    xenon-recompile <config.toml>   XenonRecomp on a configuration, its output folder synced (changed files in,
                                    stale ones out): what a build runs, the Xbox 360 Dashboard's modules too
    heavyiron <cmd> <file.ho> ...   Heavy Iron GoodEngine archives (Up, WALL-E, SpongeBob): list, extract,
                                    replace an asset, textures as PNG
    gfx <cmd> <file> ...            Scaleform GFx menus: movies, images, fonts, placements; a font as TTF
    xex-imports <module.xex>        its imports: library, ordinal, name, function or variable, slot, thunk
    xbox-disc files|read|unpack <image> ...   an Xbox 360 disc image's files: listed, one read, all unpacked
    xex-basic <file.xex> <out.xex>  the unencrypted, basic-compressed form XenonRecomp reads
    xenon-config <basic.xex> <out.toml>   a module's XenonRecomp configuration (--out-dir, --switch-tables,
                                    --file-path)
    switch-tables <basic.xex> <out.t)DOC")
        + std::string(R"DOC(oml>  its switch tables (switches.py's, with XenonAnalyse's)
    corpus-executables              every corpus executable: its format, a tab, its path
    xex-edit <op> <file.xex> ...     change its privilege flags, or its code at an address (nop, bytes, asm, hook)
    ppc-calls <.xex or image> [-o F] what its calls show: argument counts, registers kept, variadic functions
    xna <package, folder or .exe> [--name N]   an XNA game: C# of its assemblies (ILSpy) and its content as PNG/JSON
    xdk-match <.xex or image> [libs] [-o F]   name the XDK library functions it was linked with
    system-ui <update package> <dir>    the Xbox 360 runtime's system UI assets from the owner's system update
    crypto <tool> [args]            Xbox 360 console crypto (XeCrypt, keyvaults, boot chain, NAND, shadowboots)
    classes <.xex or image> [-o F]  its C++ classes from run-time type information: bases, virtual
                                    function tables, which class introduces each virtual function
    coverage [--module NAME]        how much of every corpus module is accounted for, and in what state
    reach <file or folder> [--judge]    anything at all: what each program is, its platform (or the
                                    platform there is no lane for yet), and with --judge how far its
                                    lane gets; a few of each platform, one at a time
    mcp [--http PORT]               CodeBreaker as an MCP server for an assistant (stdio; or HTTP with a token)
    serve [--status|--stop]         the CodeBreaker service: one process the terminal interface, the MCP
                                    servers and `facts` all ask (started on demand; exits when idle)
    progress [--once]               watch a running sweep
    settings                        every setting, and whether what it names is there
    cache [--stats|--prune|--clear] what the cache keeps, kind by kind; old generations dropped, the cap
                                    held (setting cache_max_gb, default 2); or all of it gone
    facts <binary> [kind [key]]     what a binary's database holds (the engine's facts) and what each
                                    was made from; --make to make one current, --databases for all
    knowledge [platform [kind [key]]]  the knowledge base: every platform's names, prototypes, signatures,
                                    structures and stand-ins, and where each came from
    tidy-state [--dry] [--stray]    state/'s size by part, and what is stale in it removed (killed runs'
                                    disk copies, old caches, idle build objects)
    java <tool> ...                 the Java transpiler: a Java program as C++03 (convert, decompile,
                                    worklist, ...; `java --help` lists them)

`--corpus NAME` before or after any command selects another product's files (settings.json: corpora).

Every command is listed here, by name. Nothing is discovered by scanning folders.
)DOC")
        ;
    return d;
}

const std::vector<std::pair<py::str, Command>>& COMMAND_TABLE() {
    static const std::vector<std::pair<py::str, Command>> t = {
        {py::str("module"), {py::str("codebreaker.source.module_writer"), py::list<py::str>{}}},
        {py::str("scoreboard"), {py::str("codebreaker.source.module_writer"), py::list<py::str>{py::str("--scoreboard")}}},
        {py::str("place"), {py::str("codebreaker.source.placement"), py::list<py::str>{}}},
        {py::str("sweep"), {py::str("codebreaker.verify.sweep"), py::list<py::str>{}}},
        {py::str("measure"), {py::str("codebreaker.verify.measure"), py::list<py::str>{}}},
        {py::str("match"), {py::str("codebreaker.verify.authored.match"), py::list<py::str>{}}},
        {py::str("score16"), {py::str("codebreaker.verify.gate16.scoreboard16"), py::list<py::str>{}}},
        {py::str("repair"), {py::str("codebreaker.verify.authored.repair"), py::list<py::str>{}}},
        {py::str("teeth"), {py::str("codebreaker.verify.teeth"), py::list<py::str>{}}},
        {py::str("invert"), {py::str("codebreaker.invert.run"), py::list<py::str>{}}},
        {py::str("exact"), {py::str("codebreaker.exact.run"), py::list<py::str>{}}},
        {py::str("vxd"), {py::str("codebreaker.source.vxd.run"), py::list<py::str>{}}},
        {py::str("ne-asm"), {py::str("codebreaker.source.ne_asm.run"), py::list<py::str>{}}},
        {py::str("build16"), {py::str("codebreaker.build.period16"), py::list<py::str>{}}},
        {py::str("flat-thunk"), {py::str("codebreaker.build.flat_thunk"), py::list<py::str>{}}},
        {py::str("decompile"), {py::str("codebreaker.decompile.decompiler"), py::list<py::str>{}}},
        {py::str("names"), {py::str("codebreaker.symbols.names"), py::list<py::str>{}}},
        {py::str("identify"), {py::str("codebreaker.symbols.runtime_library"), py::list<py::str>{}}},
        {py::str("demangle"), {py::str("codebreaker.symbols.demangle"), py::list<py::str>{}}},
        {py::str("media"), {py::str("codebreaker.formats.media"), py::list<py::str>{}}},
        {py::str("dos-functions"), {py::str("codebreaker.analyze.dos_functions"), py::list<py::str>{}}},
        {py::str("dos-interrupts"), {py::str("codebreaker.symbols.dos_interrupts"), py::list<py::str>{}}},
        {py::str("analyze"), {py::str("codebreaker.analyze.report"), py::list<py::str>{}}},
        {py::str("driver-services"), {py::str("codebreaker.symbols.driver_services"), py::list<py::str>{}}},
        {py::str("driver-source"), {py::str("codebreaker.source.driver_writer"), py::list<py::str>{}}},
        {py::str("dos-source"), {py::str("codebreaker.source.dos_writer"), py::list<py::str>{}}},
        {py::str("dos-build"), {py::str("codebreaker.source.dos_build"), py::list<py::str>{}}},
        {py::str("pe-source"), {py::str("codebreaker.source.pe_writer"), py::list<py::str>{}}},
        {py::str("ne-source"), {py::str("codebreaker.source.ne_writer"), py::list<py::str>{}}},
        {py::str("le-source"), {py::str("codebreaker.source.le_writer"), py::list<py::str>{}}},
        {py::str("origin"), {py::str("codebreaker.analyze.origin"), py::list<py::str>{}}},
        {py::str("boot-floppy"), {py::str("codebreaker.build.floppy"), py::list<py::str>{}}},
        {py::str("driver-build"), {py::str("codebreaker.build.driver"), py::list<py::str>{}}},
        {py::str("driver-judge"), {py::str("codebreaker.verify.lanes.driver"), py::list<py::str>{}}},
        {py::str("build"), {py::str("codebreaker.build.module"), py::list<py::str>{}}},
        {py::str("resources"), {py::str("codebreaker.build.resources"), py::list<py::str>{}}},
        {py::str("cab"), {py::str("codebreaker.formats.cab"), py::list<py::str>{}}},
        {py::str("disc"), {py::str("codebreaker.build.disc_image"), py::list<py::str>{}}},
        {py::str("versions"), {py::str("codebreaker.compare.versions"), py::list<py::str>{}}},
        {py::str("fonts"), {py::str("codebreaker.formats.fonts"), py::list<py::str>{}}},
        {py::str("info"), {py::str("codebreaker.formats.inspect"), py::list<py::str>{}}},
        {py::str("xex"), {py::str("codebreaker.formats.xex"), py::list<py::str>{}}},
        {py::str("coverage"), {py::str("codebreaker.coverage.ledger"), py::list<py::str>{}}},
        {py::str("reach"), {py::str("codebreaker.coverage.reach"), py::list<py::str>{}}},
        {py::str("mcp"), {py::str("codebreaker.ui.mcp_server"), py::list<py::str>{}}},
        {py::str("serve"), {py::str("codebreaker.ui.serve"), py::list<py::str>{}}},
        {py::str("ui"), {py::str("codebreaker.ui.terminal"), py::list<py::str>{}}},
        {py::str("progress"), {py::str("codebreaker.ui.progress"), py::list<py::str>{}}},
        {py::str("settings"), {py::str("codebreaker.base.settings"), py::list<py::str>{}}},
        {py::str("cache"), {py::str("codebreaker.base.cache"), py::list<py::str>{}}},
        {py::str("facts"), {py::str("codebreaker.engine.show"), py::list<py::str>{}}},
        {py::str("knowledge"), {py::str("codebreaker.knowledge.show"), py::list<py::str>{}}},
        {py::str("tidy-state"), {py::str("codebreaker.base.tidy_state"), py::list<py::str>{}}},
        {py::str("recompile"), {py::str("codebreaker.recompile.powerpc"), py::list<py::str>{}}},
        {py::str("recompile-project"), {py::str("codebreaker.recompile.project"), py::list<py::str>{}}},
        {py::str("xenon-recompile"), {py::str("codebreaker.recompile.xenon"), py::list<py::str>{}}},
        {py::str("xdk-match"), {py::str("codebreaker.symbols.xdk_libraries"), py::list<py::str>{}}},
        {py::str("xex-imports"), {py::str("codebreaker.formats.xex_imports"), py::list<py::str>{}}},
        {py::str("xex-edit"), {py::str("codebreaker.formats.xex_edit"), py::list<py::str>{}}},
        {py::str("ppc-calls"), {py::str("codebreaker.analyze.ppc_calls"), py::list<py::str>{}}},
        {py::str("heavyiron"), {py::str("codebreaker.formats.xbox.heavyiron"), py::list<py::str>{}}},
        {py::str("gfx"), {py::str("codebreaker.formats.gfx"), py::list<py::str>{}}},
        {py::str("classes"), {py::str("codebreaker.symbols.cpp_classes"), py::list<py::str>{}}},
        {py::str("system-ui"), {py::str("codebreaker.formats.xbox.system_ui"), py::list<py::str>{}}},
        {py::str("crypto"), {py::str("codebreaker.formats.xbox.crypto"), py::list<py::str>{}}},
        {py::str("java"), {py::str("codebreaker.java.run"), py::list<py::str>{}}},
        {py::str("xna"), {py::str("codebreaker.xna.run"), py::list<py::str>{}}},
        {py::str("xbox-disc"), {py::str("codebreaker_native"), py::list<py::str>{py::str("xbox-disc")}}},
        {py::str("xex-basic"), {py::str("codebreaker_native"), py::list<py::str>{py::str("xex-basic")}}},
        {py::str("xenon-config"), {py::str("codebreaker_native"), py::list<py::str>{py::str("xenon-config")}}},
        {py::str("switch-tables"), {py::str("codebreaker_native"), py::list<py::str>{py::str("switch-tables")}}},
        {py::str("corpus-executables"), {py::str("codebreaker_native"), py::list<py::str>{py::str("corpus-executables")}}},
    };
    return t;
}


const Command* command(const py::str& name) {
    for (const auto& kv : COMMAND_TABLE())
        if (kv.first == name) return &kv.second;
    return nullptr;
}

py::list<py::str> command_names() {
    py::list<py::str> out;
    for (const auto& kv : COMMAND_TABLE()) out.append(kv.first);
    return out;
}

namespace {
std::map<std::string, Handler>& handlers_() {
    static std::map<std::string, Handler> h = [] {
        std::map<std::string, Handler> m;
        m["codebreaker.ui.mcp_server"] = [](const py::list<py::str>& argv) { return ui::mcp_server::main(argv); };
        return m;
    }();
    return h;
}
}  // namespace

void register_command(const py::str& module, Handler handler) { handlers_()[module.utf8()] = std::move(handler); }

const Handler* handler(const py::str& module) {
    auto it = handlers_().find(module.utf8());
    return it == handlers_().end() ? nullptr : &it->second;
}

std::function<bool()>& has_terminal_interface() {
    static std::function<bool()> f;
    return f;
}

bool terminal_available() {
    return _isatty(0) && _isatty(1) && has_terminal_interface() && has_terminal_interface()();
}

int64_t main(const py::list<py::str>& argv_in) {
    py::list<py::str> argv = base::corpus::take_option(argv_in);   // --corpus NAME, for any command
    if (argv.empty() && terminal_available()) {
        argv = py::list<py::str>{py::str("ui")};   // (no command, in a terminal: the interface)
    }
    const bool helped = !argv.empty() && (argv[0] == py::str("-h") || argv[0] == py::str("--help") || argv[0] == py::str("help"));
    if (argv.empty() || helped || command(argv[0]) == nullptr) {
        std::string text = DOC_TEXT() + "\n";
        std::fwrite(text.data(), 1, text.size(), stdout);
        std::fflush(stdout);
        return !argv.empty() && helped ? 0 : 2;
    }
    const Command& c = *command(argv[0]);
    py::list<py::str> args = c.fixed;
    for (size_t i = 1; i < argv.size(); i++) args.append(argv[i]);
    const Handler* h = handler(c.module);
    if (!h) throw py::ModuleNotFoundError("No module named " + py::repr(c.module).utf8());
    return (*h)(args);
}

int64_t _entry(const py::list<py::str>& argv) { return main(argv); }

}  // namespace codebreaker::main_module
