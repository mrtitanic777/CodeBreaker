// DecompilePipeline.java -- Ghidra postScript: apply a period GDT + function-discovery
// cross-check + decompile-all to one .c. Java port of DecompilePipeline.py (Ghidra 12
// dropped Jython). Pair with AggressivePre.java as the -preScript.
//   ... -preScript AggressivePre.java -postScript DecompilePipeline.java <out.c> [<types.gdt>] [<names.json>]
//
// Positional args after the required out.c are recognized by extension:
//   <out.c> [<types.gdt>] [<names.json>]
// names.json is a flat {"0xVA": "name", ...} symbol map applied before decompile, so
// call sites in the emitted C read as real names instead of FUN_xxxx.
//@category Win95
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.cmd.function.ApplyFunctionDataTypesCmd;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.FileDataTypeManager;
import ghidra.program.model.data.DataTypeManager;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import java.io.File;
import java.io.OutputStreamWriter;
import java.io.FileOutputStream;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class DecompilePipeline extends GhidraScript {

    private Memory mem;

    private boolean inExec(Address addr) {
        MemoryBlock b = mem.getBlock(addr);
        return b != null && b.isExecute() && b.isInitialized();
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String outpath = args.length > 0 ? args[0] : currentProgram.getName() + ".c";
        String gdtPath = null, namesPath = null;
        for (int i = 1; i < args.length; i++) {
            String low = args[i].toLowerCase();
            if (low.endsWith(".json")) namesPath = args[i];
            else if (low.endsWith(".gdt")) gdtPath = args[i];
            else if (gdtPath == null) gdtPath = args[i];   // back-compat: bare 2nd arg = gdt
        }

        var prog = currentProgram;
        FunctionManager fm = prog.getFunctionManager();
        mem = prog.getMemory();

        // (0) Apply a period-correct type archive (GDT): makes its types available and
        // applies function signatures to matching named functions before decompilation.
        int applied = 0;
        if (gdtPath != null && new File(gdtPath).exists()) {
            try {
                FileDataTypeManager arch = FileDataTypeManager.openFileArchive(new File(gdtPath), false);
                List<DataTypeManager> archs = new ArrayList<>();
                archs.add(arch);
                new ApplyFunctionDataTypesCmd(archs, null, SourceType.IMPORTED, false, false)
                        .applyTo(prog, monitor);
                applied = 1;
            } catch (Exception e) {
                println("GDT_APPLY_ERR= " + e);
            }
        }

        int before = fm.getFunctionCount();
        int created = 0;

        // (A) create functions at undefined CALL targets flow analysis left behind
        ReferenceManager rm = prog.getReferenceManager();
        ReferenceIterator it = rm.getReferenceIterator(prog.getMinAddress());
        while (it.hasNext()) {
            Reference ref = it.next();
            if (ref.getReferenceType().isCall()) {
                Address to = ref.getToAddress();
                if (inExec(to) && fm.getFunctionAt(to) == null && fm.getFunctionContaining(to) == null) {
                    if (createFunction(to, null) != null) created++;
                }
            }
        }

        // (B) prologue scan for UNREFERENCED functions: standard frame + hot-patch stub
        for (String pat : new String[] {"\\x55\\x8b\\xec", "\\x8b\\xff\\x55\\x8b\\xec"}) {
            Address[] hits = findBytes(prog.getMinAddress(), pat, 200000);
            if (hits == null) continue;
            for (Address a : hits) {
                if (inExec(a) && fm.getFunctionAt(a) == null && fm.getFunctionContaining(a) == null) {
                    if (createFunction(a, null) != null) created++;
                }
            }
        }

        // (B2) apply a caller-supplied symbol map: name functions before decompile so the
        // emitted C uses real names at every call site.
        int renamed = 0;
        if (namesPath != null && new File(namesPath).exists()) {
            try {
                String raw = new String(java.nio.file.Files.readAllBytes(new File(namesPath).toPath()),
                        Charset.forName("UTF-8"));
                com.google.gson.JsonObject nm =
                        com.google.gson.JsonParser.parseString(raw).getAsJsonObject();
                for (var ent : nm.entrySet()) {
                    String va = ent.getKey();
                    Address addr = toAddr(Long.parseLong(va.replaceFirst("^0x", ""), 16));
                    Function f = fm.getFunctionAt(addr);
                    if (f == null && inExec(addr)) f = createFunction(addr, null);
                    if (f != null) {
                        f.setName(ent.getValue().getAsString(), SourceType.USER_DEFINED);
                        renamed++;
                    }
                }
            } catch (Exception e) {
                println("NAMES_APPLY_ERR= " + e);
            }
        }

        // (C) decompile everything
        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(prog);
        List<Function> funcs = new ArrayList<>();
        for (Function f : fm.getFunctions(true)) funcs.add(f);

        StringBuilder sb = new StringBuilder();
        int ok = 0, fail = 0;
        StringBuilder body = new StringBuilder();
        for (Function f : funcs) {
            try {
                DecompileResults res = decomp.decompileFunction(f, 60, monitor);
                if (res != null && res.decompileCompleted()) {
                    body.append(res.getDecompiledFunction().getC()).append("\n\n");
                    ok++;
                } else {
                    body.append("// ").append(f.getName()).append(" @ ")
                        .append(f.getEntryPoint()).append(": FAILED\n\n");
                    fail++;
                }
            } catch (Exception e) {
                body.append("// ").append(f.getName()).append(": EXC ").append(e).append("\n\n");
                fail++;
            }
        }
        sb.append(String.format("// %d ok, %d fail%n", ok, fail));
        sb.append(String.format("// Ghidra pipeline: %s | gdt_applied=%d named=%d before=%d created=%d total=%d%n%n",
                prog.getName(), applied, renamed, before, created, funcs.size()));
        sb.append(body);

        // Write UTF-8 (decompiled C can carry non-ASCII from wide-string literals).
        try (OutputStreamWriter fh = new OutputStreamWriter(
                new FileOutputStream(outpath), Charset.forName("UTF-8"))) {
            fh.write(sb.toString());
        }
        println(String.format("PIPELINE_DONE gdt=%d named=%d before=%d created=%d ok=%d fail=%d total=%d",
                applied, renamed, before, created, ok, fail, funcs.size()));
    }
}
