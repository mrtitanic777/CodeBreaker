The reference the Java decompiler port is checked against: the original Fernflower, built from its source, dumping what each layer holds.

1. Fernflower's source as RetroMCP uses it: github.com/Lassebq/Fernflower, branch master (package de.fernflower).
2. Its identity-hashed collections made insertion-ordered. Where the original keeps blocks, statements, edges or variable nodes in a HashSet or
   HashMap and iterates it, the order is the JVM's identity hash codes, which change with the JVM and with everything hashed before; no other
   program can reproduce it, and it is not behaviour of the decompiler. In a copy of the source, every `new HashSet<T>` / `new HashMap<T, ...>`
   (and `new HashSet<>()` declared as such) whose T is one of BasicBlock, Statement, VarVersionNode, StatEdge, DirectNode, ClassWrapper, IGraphNode,
   Node, ExceptionRangeCFG, ClassWrapperNode, MethodWrapper, VarType, Exprent, ClassNode, MatchProperties, FieldDescriptor, VarVersionEdge becomes
   LinkedHashSet / LinkedHashMap. Sets and maps of strings and numbers keep Java's own hash order (the port reproduces it: java_hash_set_order).
3. javac --release 8 every file, jar them; then javac -cp fernflower.jar StructDump.java GraphDump.java and run each on a jar:
     java -cp fernflower.jar;. StructDump <jar>   > ../struct/<name>.txt
     java -cp fernflower.jar;. GraphDump <jar>    > ../graph/<name>.txt
