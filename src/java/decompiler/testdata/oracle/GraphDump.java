// The reference for the C++ port's code layer: every method of every class of a jar, its instructions and its control-flow graph after the passes the
// original runs before building statements (MethodProcessorRunnable.codeToJava up to mergeBasicBlocks), as text (testdata/graph is its output).
// Build Fernflower from its source (github.com/Lassebq/Fernflower), then: javac -cp fernflower.jar GraphDump.java; java -cp fernflower.jar;. GraphDump <jar>
import de.fernflower.code.*;
import de.fernflower.code.cfg.*;
import de.fernflower.main.DecompilerContext;
import de.fernflower.main.Fernflower;
import de.fernflower.main.extern.*;
import de.fernflower.modules.code.DeadCodeHelper;
import de.fernflower.modules.decompiler.deobfuscator.ExceptionDeobfuscator;
import de.fernflower.struct.*;
import java.io.*;
import java.util.*;
import java.util.jar.Manifest;

public class GraphDump {
  public static void main(String[] args) throws Exception {
    PrintStream out = new PrintStream(new FileOutputStream(FileDescriptor.out), false, "UTF-8") { public void println(String x) { print(x + "\n"); } };
    IBytecodeProvider provider = (externalPath, internalPath) -> {
      try (java.util.zip.ZipFile z = new java.util.zip.ZipFile(externalPath)) {
        java.util.zip.ZipEntry e = z.getEntry(internalPath);
        try (InputStream in = z.getInputStream(e)) { ByteArrayOutputStream b = new ByteArrayOutputStream(); byte[] buf = new byte[8192]; int n; while ((n = in.read(buf)) > 0) b.write(buf, 0, n); return b.toByteArray(); }
      }
    };
    IResultSaver saver = new IResultSaver() {
      public void saveFolder(String p) {} public void copyFile(String s, String p, String e) {} public void saveClassFile(String p, String q, String e, String c, int[] m) {}
      public void createArchive(String p, String a, Manifest m) {} public void saveDirEntry(String p, String a, String e) {} public void copyEntry(String s, String p, String a, String e) {}
      public void saveClassEntry(String p, String a, String q, String e, String c) {} public void closeArchive(String p, String a) {}
    };
    IFernflowerLogger logger = new IFernflowerLogger() { public void writeMessage(String m, Severity s) {} public void writeMessage(String m, Throwable t) {} };
    Map<String, Object> options = new HashMap<String, Object>();
    options.put("nls", "1");
    Fernflower ff = new Fernflower(provider, saver, options, logger);
    ff.getStructContext().addSpace(new File(args[0]), true);
    DecompilerContext.setStructContext(ff.getStructContext());
    TreeMap<String, StructClass> sorted = new TreeMap<>(ff.getStructContext().getClasses());
    for (StructClass c : sorted.values()) {
      for (StructMethod mt : c.getMethods()) {
        if (!mt.containsCode()) continue;
        out.println("== " + c.qualifiedName + " " + mt.getName() + " " + mt.getDescriptor());
        try {
          mt.expandData();
          InstructionSequence seq = mt.getInstructionSequence();
          out.print(seq.toString());
          for (ExceptionHandler h : seq.getExceptionTable().getHandlers())
            out.println("handler " + h.from + " " + h.to + " " + h.handler + " " + h.from_instr + " " + h.to_instr + " " + h.handler_instr + " " + h.exceptionClass);
          ControlFlowGraph graph = new ControlFlowGraph(seq);
          DeadCodeHelper.removeDeadBlocks(graph);
          graph.inlineJsr(mt);
          DeadCodeHelper.connectDummyExitBlock(graph);
          DeadCodeHelper.removeGotos(graph);
          ExceptionDeobfuscator.removeCircularRanges(graph);
          ExceptionDeobfuscator.restorePopRanges(graph);
          ExceptionDeobfuscator.removeEmptyRanges(graph);
          DeadCodeHelper.incorporateValueReturns(graph);
          ExceptionDeobfuscator.insertEmptyExceptionHandlerBlocks(graph);
          DeadCodeHelper.mergeBasicBlocks(graph);
          out.print(graph.toString());
          for (ExceptionRangeCFG r : graph.getExceptions()) {
            StringBuilder b = new StringBuilder("range handler=" + r.getHandler().id + " types=" + r.getExceptionTypes() + " unique=" + r.getUniqueExceptionsString() + " blocks=");
            for (BasicBlock x : r.getProtectedRange()) b.append(x.id).append(',');
            out.println(b.toString());
          }
          StringBuilder e = new StringBuilder("first=" + graph.getFirst().id + " last=" + graph.getLast().id + " last_id=" + graph.last_id + " obfuscated=" + ExceptionDeobfuscator.hasObfuscatedExceptions(graph) + " preds:");
          for (BasicBlock x : graph.getBlocks()) {
            e.append(' ').append(x.id).append('<');
            for (BasicBlock p : x.getPreds()) e.append(p.id).append(',');
            e.append('|');
            for (BasicBlock p : x.getPredExceptions()) e.append(p.id).append(',');
            e.append(" offs=").append(x.getInstrOldOffsets());
          }
          out.println(e.toString());
          mt.releaseResources();
        }
        catch (Throwable t) {
          out.println("ERROR");
        }
      }
    }
    out.flush();
  }
}
