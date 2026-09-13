// ExportC.java -- Ghidra headless post-script: export the analyzed program to one C
// file via the stock CppExporter. This is the method that produces the dogbolt.org
// "Decompiler Explorer -> Ghidra" output: a single .c with `#include "<out>.h"`, the
// DAT_ globals, and every function (with its segment/plate comments). The companion
// .h gets the type definitions (undefined2, etc.).
//
// Written in Java on purpose: Ghidra 12 dropped Jython, so a .py post-script needs
// PyGhidra; a .java script always compiles in-process with no Python runtime.
//
//   analyzeHeadless <proj> <name> -import <binary> \
//       -scriptPath src/toolchain/ghidra -postScript ExportC.java <out.c>
//@category Win95
import ghidra.app.script.GhidraScript;
import ghidra.app.util.exporter.CppExporter;
import java.io.File;

public class ExportC extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String out = args.length > 0 ? args[0] : currentProgram.getName() + ".c";
        // (options=null -> decompiler defaults) createHeader, createCFile, emitTypes,
        // emitGlobals, excludeTags, tags=null -- matches dogbolt's default export.
        CppExporter exp = new CppExporter(null, true, true, true, true, true, null);
        boolean ok = exp.export(new File(out), currentProgram, null, monitor);
        println("EXPORT_OK=" + ok + " -> " + out);
    }
}
