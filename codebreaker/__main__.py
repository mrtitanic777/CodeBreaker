"""python -m codebreaker <command> [arguments]

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
    serve [--port N] [--open]       the local web console (127.0.0.1 only)
    mcp [--http PORT]               CodeBreaker as an MCP server for an assistant (stdio; or HTTP with a token)
    progress [--once] [--window]    watch a running sweep
    app [file-or-module]            the desktop window
    workbench [--forms]             the Workbench: the earlier CodeBreaker's Windows 95-style window
    live <binary>                   the Live reconstruction window: the file's bytes and its C appearing
    take-apart <binary>             what the Workbench does with a dropped program, to a folder
    settings                        every setting, and whether what it names is there
    java <tool> ...                 the Java transpiler: a Java program as C++03 (convert, decompile,
                                    worklist, ...; `java --help` lists them)

`--corpus NAME` before or after any command selects another product's files (settings.json: corpora).

Every command is listed here, by name. Nothing is discovered by scanning folders.
"""
import importlib
import sys

COMMANDS = {
    "module": ("codebreaker.source.module_writer", []),
    "scoreboard": ("codebreaker.source.module_writer", ["--scoreboard"]),
    "place": ("codebreaker.source.placement", []),
    "sweep": ("codebreaker.verify.sweep", []),
    "measure": ("codebreaker.verify.measure", []),
    "match": ("codebreaker.verify.authored.match", []),
    "score16": ("codebreaker.verify.gate16.scoreboard16", []),
    "repair": ("codebreaker.verify.authored.repair", []),
    "teeth": ("codebreaker.verify.teeth", []),
    "invert": ("codebreaker.invert.run", []),
    "exact": ("codebreaker.exact.run", []),
    "vxd": ("codebreaker.source.vxd.run", []),
    "ne-asm": ("codebreaker.source.ne_asm.run", []),
    "build16": ("codebreaker.build.period16", []),
    "flat-thunk": ("codebreaker.build.flat_thunk", []),
    "decompile": ("codebreaker.decompile.decompiler", []),
    "names": ("codebreaker.symbols.names", []),
    "identify": ("codebreaker.symbols.runtime_library", []),
    "demangle": ("codebreaker.symbols.demangle", []),
    "media": ("codebreaker.formats.media", []),
    "dos-functions": ("codebreaker.analyze.dos_functions", []),
    "dos-interrupts": ("codebreaker.symbols.dos_interrupts", []),
    "analyze": ("codebreaker.analyze.report", []),
    "driver-services": ("codebreaker.symbols.driver_services", []),
    "driver-source": ("codebreaker.source.driver_writer", []),
    "dos-source": ("codebreaker.source.dos_writer", []),
    "dos-build": ("codebreaker.source.dos_build", []),
    "pe-source": ("codebreaker.source.pe_writer", []),
    "ne-source": ("codebreaker.source.ne_writer", []),
    "le-source": ("codebreaker.source.le_writer", []),
    "origin": ("codebreaker.analyze.origin", []),
    "boot-floppy": ("codebreaker.build.floppy", []),
    "driver-build": ("codebreaker.build.driver", []),
    "driver-judge": ("codebreaker.verify.lanes.driver", []),
    "build": ("codebreaker.build.module", []),
    "resources": ("codebreaker.build.resources", []),
    "cab": ("codebreaker.formats.cab", []),
    "disc": ("codebreaker.build.disc_image", []),
    "versions": ("codebreaker.compare.versions", []),
    "fonts": ("codebreaker.formats.fonts", []),
    "info": ("codebreaker.formats.inspect", []),
    "xex": ("codebreaker.formats.xex", []),
    "coverage": ("codebreaker.coverage.ledger", []),
    "reach": ("codebreaker.coverage.reach", []),
    "serve": ("codebreaker.ui.server", []),
    "mcp": ("codebreaker.ui.mcp_server", []),
    "progress": ("codebreaker.ui.progress", []),
    "app": ("codebreaker.ui.app", []),
    "workbench": ("codebreaker.ui.workbench", []),
    "live": ("codebreaker.ui.live", []),
    "take-apart": ("codebreaker.ui.take_apart", []),
    "settings": ("codebreaker.base.settings", []),
    "recompile": ("codebreaker.recompile.powerpc", []),
    "recompile-project": ("codebreaker.recompile.project", []),
    "xenon-recompile": ("codebreaker.recompile.xenon", []),
    "xdk-match": ("codebreaker.symbols.xdk_libraries", []),
    "xex-imports": ("codebreaker.formats.xex_imports", []),
    "xex-edit": ("codebreaker.formats.xex_edit", []),
    "ppc-calls": ("codebreaker.analyze.ppc_calls", []),
    "heavyiron": ("codebreaker.formats.xbox.heavyiron", []),
    "gfx": ("codebreaker.formats.gfx", []),
    "classes": ("codebreaker.symbols.cpp_classes", []),
    "system-ui": ("codebreaker.formats.xbox.system_ui", []),
    "crypto": ("codebreaker.formats.xbox.crypto", []),
    "java": ("codebreaker.java.run", []),
    "xna": ("codebreaker.xna.run", []),
}


def main(argv):
    from codebreaker.base import corpus
    argv = corpus.take_option(argv)                          # --corpus NAME, for any command
    if not argv or argv[0] in ("-h", "--help", "help") or argv[0] not in COMMANDS:
        print(__doc__)
        return 0 if argv and argv[0] in ("-h", "--help", "help") else 2
    module, fixed = COMMANDS[argv[0]]
    return importlib.import_module(module).main(fixed + argv[1:])


def _entry():
    """The `codebreaker` command an install provides."""
    sys.exit(main(sys.argv[1:]))


if __name__ == "__main__":
    _entry()
