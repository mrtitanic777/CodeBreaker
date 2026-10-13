"""python -m codebreaker <command> [arguments]

    module <name> [--no-verify]     reconstruct one module: decompile, build, judge, write its source
    scoreboard                      rewrite the scoreboard from stored results
    sweep [options]                 reconstruct and judge every module (resumable)
    decompile <name> [--out FILE]   decompile any module (32-bit, 16-bit, PowerPC) to C; nothing is built
    measure <name>                  reconstruct and judge one module; writes nothing to the tree
    names <module>                  every function and import with its best name and where it came from
    identify <module>               runtime-library functions of a module, and what built it
    demangle <name>                 a decorated C++ name as its declaration
    media list|extract <image>      files of a floppy, hard-disk or CD image
    dos-functions <program>         functions found in a real-mode program, by evidence
    dos-interrupts [int [AH]]       name a DOS or BIOS service
    analyze <module> [--what a,b]   call graph, conventions, globals, strings, structures, classes, dispatch
    driver-services [FILE ...]      names of ring-0 services; how many of a driver's calls they name
    driver-build <folder or .asm>   assemble and link a driver, check it against the original
    driver-judge <rebuilt> <orig>   run every entry point of both drivers and compare
    info <file-or-module>           what an executable is and holds (headers, imports, exports, resources)
    coverage [--module NAME]        how much of every corpus module is accounted for, and in what state
    serve [--port N] [--open]       the local web console (127.0.0.1 only)
    progress [--once] [--window]    watch a running sweep
    app [file-or-module]            the desktop window

Every command is listed here, by name. Nothing is discovered by scanning folders.
"""
import importlib
import sys

COMMANDS = {
    "module": ("codebreaker.source.module_writer", []),
    "scoreboard": ("codebreaker.source.module_writer", ["--scoreboard"]),
    "sweep": ("codebreaker.verify.sweep", []),
    "measure": ("codebreaker.verify.measure", []),
    "decompile": ("codebreaker.decompile.decompiler", []),
    "names": ("codebreaker.symbols.names", []),
    "identify": ("codebreaker.symbols.runtime_library", []),
    "demangle": ("codebreaker.symbols.demangle", []),
    "media": ("codebreaker.formats.media", []),
    "dos-functions": ("codebreaker.analyze.dos_functions", []),
    "dos-interrupts": ("codebreaker.symbols.dos_interrupts", []),
    "analyze": ("codebreaker.analyze.report", []),
    "driver-services": ("codebreaker.symbols.driver_services", []),
    "driver-build": ("codebreaker.build.driver", []),
    "driver-judge": ("codebreaker.verify.driver", []),
    "info": ("codebreaker.formats.inspect", []),
    "coverage": ("codebreaker.coverage.ledger", []),
    "serve": ("codebreaker.ui.server", []),
    "progress": ("codebreaker.ui.progress", []),
    "app": ("codebreaker.ui.app", []),
}


def main(argv):
    if not argv or argv[0] in ("-h", "--help", "help") or argv[0] not in COMMANDS:
        print(__doc__)
        return 0 if argv and argv[0] in ("-h", "--help", "help") else 2
    module, fixed = COMMANDS[argv[0]]
    return importlib.import_module(module).main(fixed + argv[1:])


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
