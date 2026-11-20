// The reference for the C++ port's struct layer: what the original Fernflower reads from every class of a jar, as text (testdata/struct is its output).
// Build Fernflower from its source (github.com/Lassebq/Fernflower: javac --release 8, jar), then: javac -cp fernflower.jar StructDump.java; java -cp fernflower.jar;. StructDump <jar>
import de.fernflower.main.Fernflower;
import de.fernflower.main.extern.*;
import de.fernflower.struct.*;
import de.fernflower.struct.attr.*;
import de.fernflower.struct.consts.*;
import de.fernflower.struct.gen.*;
import de.fernflower.struct.gen.generics.*;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.jar.Manifest;

public class StructDump {
  static PrintStream out;

  public static void main(String[] args) throws Exception {
    out = new PrintStream(new FileOutputStream(FileDescriptor.out), false, "UTF-8") { public void println(String x) { print(x + "\n"); } };
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
    Fernflower ff = new Fernflower(provider, saver, new HashMap<String, Object>(), logger);
    ff.getStructContext().addSpace(new File(args[0]), true);
    TreeMap<String, StructClass> sorted = new TreeMap<>(ff.getStructContext().getClasses());
    for (StructClass c : sorted.values()) dumpClass(c);
    out.flush();
  }

  static String g(GenericType t) {
    if (t == null) return "null";
    StringBuilder b = new StringBuilder();
    b.append(t.type).append('/').append(t.arrayDim).append('/').append(t.value);
    for (GenericType e : t.getEnclosingClasses()) b.append(" enc(").append(g(e)).append(')');
    if (!t.getArguments().isEmpty()) {
      b.append(" <");
      for (int i = 0; i < t.getArguments().size(); i++) b.append(i > 0 ? ", " : "").append(t.getWildcards().get(i)).append(':').append(g(t.getArguments().get(i)));
      b.append('>');
    }
    return b.toString();
  }

  static String gl(List<GenericType> l) { StringBuilder b = new StringBuilder("["); for (int i = 0; i < l.size(); i++) b.append(i > 0 ? "; " : "").append(g(l.get(i))); return b.append(']').toString(); }

  static void dumpPool(ConstantPool pool) {
    for (int i = 1; i < pool.size(); i++) {
      PooledConstant k = pool.getConstant(i);
      if (k == null) continue;
      if (k instanceof PrimitiveConstant) {
        PrimitiveConstant p = (PrimitiveConstant)k;
        String v;
        if (p.value instanceof Float) v = Integer.toHexString(Float.floatToRawIntBits((Float)p.value));
        else if (p.value instanceof Double) v = Long.toHexString(Double.doubleToRawLongBits((Double)p.value));
        else v = String.valueOf(p.value);
        out.println("  pool " + i + " " + p.type + " " + v + (p.isArray ? " array" : ""));
      } else {
        LinkConstant l = (LinkConstant)k;
        out.println("  pool " + i + " " + l.type + " " + l.classname + " " + l.elementname + " " + l.descriptor + " " + l.paramCount + " " + l.isVoid + " " + l.returnCategory2);
      }
    }
  }

  static void dumpAttributes(String indent, StructMember m, ConstantPool pool) {
    VBStyleCollectionDump(indent, m, pool);
  }

  static void VBStyleCollectionDump(String indent, StructMember m, ConstantPool pool) {
    for (StructGeneralAttribute a : m.getAttributes()) {
      String d = "";
      if (a instanceof StructSourceFileAttribute) d = ((StructSourceFileAttribute)a).getFileName();
      else if (a instanceof StructGenericSignatureAttribute) {
        String s = ((StructGenericSignatureAttribute)a).getSignature();
        d = s;
        if (m instanceof StructClass) { GenericClassDescriptor c = GenericMain.parseClassSignature(s); d += c == null ? " => null" : " => " + c.fparameters + " " + c.fbounds.size() + " super " + g(c.superclass) + " if " + gl(c.superinterfaces); }
        else if (m instanceof StructField) { GenericFieldDescriptor f = GenericMain.parseFieldSignature(s); d += f == null ? " => null" : " => " + g(f.type); }
        else { GenericMethodDescriptor x = GenericMain.parseMethodSignature(s); d += x == null ? " => null" : " => " + x.fparameters + " " + x.fbounds.size() + " " + gl(x.params) + " ret " + g(x.ret) + " exc " + gl(x.exceptions); }
      }
      else if (a instanceof StructInnerClassesAttribute) {
        StructInnerClassesAttribute ic = (StructInnerClassesAttribute)a;
        for (int i = 0; i < ic.getClassEntries().size(); i++) { int[] e = ic.getClassEntries().get(i); String[] s = ic.getStringEntries().get(i); d += " [" + e[0] + " " + e[1] + " " + e[2] + " " + e[3] + " " + s[0] + " " + s[1] + " " + s[2] + "]"; }
      }
      else if (a instanceof StructEnclosingMethodAttribute) { StructEnclosingMethodAttribute e = (StructEnclosingMethodAttribute)a; d = e.getClassName() + " " + e.getMethodName() + " " + e.getMethodDescriptor(); }
      else if (a instanceof StructLineNumberTableAttribute) d = Arrays.toString(((StructLineNumberTableAttribute)a).getRawData());
      else if (a instanceof StructLocalVariableTableAttribute) d = new TreeMap<>(((StructLocalVariableTableAttribute)a).getMapVarNames()).toString();
      else if (a instanceof StructExceptionsAttribute) { StructExceptionsAttribute e = (StructExceptionsAttribute)a; for (int i = 0; i < e.getThrowsExceptions().size(); i++) d += " " + e.getExcClassname(i, pool); }
      else if (a instanceof StructConstantValueAttribute) d = String.valueOf(((StructConstantValueAttribute)a).getIndex());
      else if (a instanceof StructBootstrapMethodsAttribute) { StructBootstrapMethodsAttribute b = (StructBootstrapMethodsAttribute)a; for (int i = 0; i < b.getMethodsNumber(); i++) d += " [" + b.getMethodReference(i).elementname + " " + b.getMethodArguments(i).size() + "]"; }
      out.println(indent + "attr " + a.getName() + (d.isEmpty() ? "" : " " + d));
    }
  }

  static void dumpClass(StructClass c) {
    ConstantPool pool = c.getPool();
    out.println("class " + c.qualifiedName + " access=" + c.getAccessFlags() + " super=" + (c.superClass == null ? "null" : c.superClass.getString())
                + " bc=" + c.getBytecodeVersion() + " ge15=" + c.isVersionGE_1_5() + " ge17=" + c.isVersionGE_1_7() + " synthetic=" + c.isSynthetic());
    for (String i : c.getInterfaceNames()) out.println("  interface " + i);
    dumpPool(pool);
    dumpAttributes("  ", c, pool);
    for (StructField f : c.getFields()) {
      out.println("  field " + f.getName() + " " + f.getDescriptor() + " access=" + f.getAccessFlags() + " type=" + FieldDescriptor.parseDescriptor(f.getDescriptor()).type);
      dumpAttributes("    ", f, pool);
    }
    for (StructMethod m : c.getMethods()) {
      MethodDescriptor md = MethodDescriptor.parseDescriptor(m.getDescriptor());
      out.println("  method " + m.getName() + " " + m.getDescriptor() + " access=" + m.getAccessFlags() + " code=" + m.containsCode() + " locals=" + m.getLocalVariables()
                  + " params=" + Arrays.toString(md.params) + " ret=" + md.ret);
      dumpAttributes("    ", m, pool);
    }
  }
}
