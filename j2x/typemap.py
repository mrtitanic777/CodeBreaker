# -*- coding: utf-8 -*-
"""
typemap.py -- pure DATA: how Java types map to C++03 types.

No logic lives here (that's `types.py`); this is the lookup tables, lifted and
cleaned from the proven monolith so we keep its hard-won knowledge without its
inline `/* PORT: */` comment noise. Where the old converter embedded a PORT note
inside the type string (Object -> "void* /* PORT: was Object */"), we map to the
clean type and let `emit` record the PORT as a ledger event instead.
"""

# Java language primitives (value types, stack-allocated).
PRIMITIVES = {
    'boolean', 'byte', 'short', 'int', 'long', 'float', 'double', 'char', 'void',
}

# Base Java -> C++03 type names. Profile extras merge on top of this at runtime.
# CAVEAT: Java `byte` is SIGNED (-128..127). We map it to `unsigned char` because the vast majority
# of byte uses here are byte[] buffers (chunk/block data, packet payloads, inflate I/O) that want
# unsigned char* and are read via `& 0xFF`. But SCALAR byte fields used as signed values break:
# Packet30Entity x/y/z relative-move deltas read -1 as +255 -> entities rocketed into the sky
# (fixed via overrides/Packet30Entity.h = signed char). If a future class uses a scalar `byte` as a
# signed quantity, override that field to `signed char` (do NOT blanket-flip this map -> breaks byte[]).
JAVA_TO_CPP = {
    'boolean': 'bool', 'byte': 'unsigned char', 'short': 'short', 'int': 'int',
    'long': 'long long', 'float': 'float', 'double': 'double', 'char': 'char',
    'void': 'void',
    'String': 'JavaString',
    'ArrayList': 'JList', 'List': 'JList', 'LinkedList': 'JList',
    'HashMap': 'std::map', 'HashSet': 'std::set', 'TreeMap': 'std::map',
    'Map': 'std::map', 'Set': 'std::set',
    'Random': 'Random',
    # Java root / boxed types. Object -> JObject (a boxed-value type: holds null/int/char/String/
    # String[]/typed-pointer with RTTI), so Object[] autoboxing + instanceof + reflective recipe
    # decode work. JObject is a value (NOT a pointer); it converts to T*/JavaString via operators.
    'Object': 'JObject',
    'Integer': 'int', 'Long': 'long long', 'Float': 'float', 'Double': 'double',
    'Boolean': 'bool', 'Byte': 'unsigned char', 'Short': 'short', 'Character': 'char',
}

# Java types that emit warrants a PORT note for (mapped, but lossy / needs review).
PORT_NOTE_TYPES = {
    'Object': 'was Object (untyped); verify the concrete type',
}

# Java collection types -> STL. Held BY VALUE as fields by default (std::vector/map).
CONTAINERS = {'ArrayList', 'List', 'LinkedList', 'HashMap', 'HashSet', 'TreeMap', 'Map', 'Set'}

# default element types for a RAW (non-generic) container -> a valid C++ template
RAW_CONTAINER_DEFAULT = {'JList': '<void*>', 'std::vector': '<void*>', 'std::set': '<void*>',
                         # raw Map -> <JObject, JObject>: a Java raw map mixes key/value types
                         # (String/Integer/Character keys; any object/boxed value). JObject boxes
                         # all of them (and has operator< for the key), so put/get/containsKey/remove
                         # all bind regardless of the concrete Java types. Keys/values convert in
                         # implicitly; reads convert out via the cast the converter already emits.
                         'std::map': '<JObject, JObject>'}

# Java stdlib types that become C++ VALUE types (not pointers).
VALUE_STDLIB = {'String'}

# Boxed primitive wrappers -- map to C++ primitives, so they're VALUE types (not pointers).
BOXED_VALUE_TYPES = {'Integer', 'Long', 'Float', 'Double', 'Boolean', 'Byte', 'Short', 'Character'}

# Registry arrays whose slots are NULL until an id is registered. Indexing one with
# an unregistered id and calling through it = a virtual call on NULL = IP 0x0 crash
# on the console. A deref of these (or any scope array of reference type) is guarded.
KNOWN_NULLABLE = {'blocksList', 'itemsList'}

# Runtime-support classes the converter EMITS a header for (see runtime.py) and
# #includes when referenced -- not part of the decompiled corpus. Includes the external
# Java base types our classes extend/implement (Runnable/Thread/Comparator/...).
# Static-holder externals accessed as Name::member (constants + static functions) that get
# their own emitted header. Like GL11. Also need RUNTIME_ENUM_LIKE (no s_ prefix on members).
RUNTIME_STATIC_HOLDERS = {'GL11', 'GL12', 'GL15', 'GLU', 'Keyboard', 'Mouse', 'Display',
                          'ARBOcclusionQuery', 'ARBVertexBufferObject', 'BufferUtils', 'ImageIO',
                          'Sys', 'GLContext', 'Controllers', 'Collections'}

# Runtime-support classes the converter EMITS a header for (see runtime.py) and #includes when
# referenced -- not part of the decompiled corpus. External Java base types our classes
# extend/implement, the static holders above, NIO buffers, and java.io/awt/net instance stubs.
RUNTIME_HEADERS = ({'Random', 'Runnable', 'Thread', 'Comparator', 'File',
                    'FilenameFilter', 'Canvas', 'Applet',
                    'DisplayMode', 'FloatBuffer', 'IntBuffer', 'ByteBuffer',
                    'DataInput', 'DataOutput', 'SoundSystem', 'BufferedImage', 'BufferedReader',
                    'FileReader', 'FileWriter', 'PrintWriter',
                    'Graphics', 'Component', 'Cursor', 'Robot', 'URL', 'FileDialog',
                    'DecimalFormat', 'ImageObserver', 'Class',
                    'InputStream', 'OutputStream', 'DataInputStream', 'DataOutputStream',
                    'BufferedInputStream', 'BufferedOutputStream', 'FileInputStream',
                    'FileOutputStream', 'ZipFile', 'Socket', 'DateFormat', 'Dimension',
                    'KeyListener', 'MouseListener', 'MouseMotionListener', 'MouseWheelListener',
                    'WindowListener', 'ComponentListener', 'FocusListener', 'WindowAdapter',
                    'KeyEvent', 'MouseEvent', 'WindowEvent', 'ComponentEvent', 'FocusEvent',
                    'InputEvent', 'Panel', 'Comparable', 'CodecJOrbis', 'URLConnection',
                    'BorderLayout', 'GridLayout', 'FlowLayout', 'CardLayout', 'GridBagLayout', 'Color',
                    'Frame',
                    'Throwable', 'Exception', 'Error', 'RuntimeException', 'IOException',
                    'InterruptedException', 'IllegalStateException', 'IllegalArgumentException',
                    'MalformedURLException', 'AWTException', 'LWJGLException', 'NoSuchFieldError',
                    'FileNotFoundException', 'EOFException', 'SocketException', 'UnknownHostException',
                    'ConnectException', 'BindException', 'NumberFormatException', 'NullPointerException',
                    'ArrayIndexOutOfBoundsException', 'IndexOutOfBoundsException', 'ClassCastException',
                    'NoSuchElementException', 'UnsupportedOperationException', 'ConcurrentModificationException',
                    'OutOfMemoryError', 'NoSuchMethodException', 'CloneNotSupportedException',
                    'ClassNotFoundException'}
                   | RUNTIME_STATIC_HOLDERS)

# Java stdlib static holders shimmed in the prelude (JavaRT.h, included everywhere -- so NOT
# in RUNTIME_HEADERS, which would emit a bogus per-type #include). `Name.member` -> `Name::member`.
JAVART_STATICS = {'System', 'Integer', 'Long', 'Double', 'Float', 'Character', 'Arrays',
                  'Short', 'Byte', 'Boolean'}

# Runtime types whose `Type.MEMBER` (no parens) is a CONSTANT, not an instance/static field
# -- accessed as Type::MEMBER with no s_ prefix (like an enum constant). e.g. GL11.GL_TEXTURE_2D,
# Integer.MAX_VALUE, System.out.
# Color.BLACK / Color.white etc. are static Color CONSTANTS accessed Color::BLACK (no s_).
RUNTIME_ENUM_LIKE = RUNTIME_STATIC_HOLDERS | JAVART_STATICS | {'Color'}

# java.lang.Math.<fn> -> C math function. Float-suffixed to match the existing
# Xbox 360 project (float math); pulled in via <math.h>. min/max stay bare
# (the project provides them). Returns the C name for a given Java Math method.
MATH_API = {
    'abs': 'abs', 'sqrt': 'sqrtf', 'sin': 'sinf', 'cos': 'cosf', 'tan': 'tanf',
    'atan': 'atanf', 'atan2': 'atan2f', 'asin': 'asinf', 'acos': 'acosf',
    'floor': 'floorf', 'ceil': 'ceilf', 'pow': 'powf', 'exp': 'expf', 'log': 'logf',
    'min': 'min', 'max': 'max',
    # constants (no call): Math.PI is a member, not a method -> inline the float literal
    # (this is a float-math project, consistent with the sqrtf/sinf choices above)
    'PI': '3.1415927F',
    # no clean inline form -> free-function shims from the runtime prelude (JavaRT.h)
    'random': 'javaMathRandom', 'signum': 'javaMathSignum',
}

# User-defined C++ VALUE/struct types ('.' access, no heap). EMPTY by default: a Java class
# is a REFERENCE type and must be a pointer to be universal + faithful (Minecraft's Vec3D /
# AxisAlignedBB / MovingObjectPosition are reference types AND pooled -- modeling them as
# values broke nullable override returns at scale, 742 conflicting-return errors on alpha).
# The value-type mechanism still exists; a profile/caller can opt a genuinely-by-value struct
# in via TypeResolver(value_types=...).
DEFAULT_VALUE_TYPES = set()
