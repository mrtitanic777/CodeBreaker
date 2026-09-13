// AggressivePre.java -- Ghidra preScript: enable the heavier analyzers that raise
// decompile accuracy. Java port of AggressivePre.py (Ghidra 12 dropped Jython).
//   analyzeHeadless <proj> <name> -import <bin> -scriptPath src/toolchain/ghidra \
//       -preScript AggressivePre.java -postScript DecompilePipeline.java <out.c> [gdt]
//@category Win95
import ghidra.app.script.GhidraScript;

public class AggressivePre extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[][] opts = {
            {"Decompiler Parameter ID", "true"},        // re-derive real param counts/types
            {"Aggressive Instruction Finder", "true"},  // find code in gaps
            {"Decompiler Switch Analysis", "true"},
            {"Call Convention ID", "true"},
        };
        for (String[] o : opts) {
            try {
                setAnalysisOption(currentProgram, o[0], o[1]);
                println("PRE set " + o[0] + "=" + o[1]);
            } catch (Exception e) {
                println("PRE opt " + o[0] + " ERR " + e);
            }
        }
    }
}
