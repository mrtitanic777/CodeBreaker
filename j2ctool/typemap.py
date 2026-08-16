# -*- coding: utf-8 -*-
"""
typemap.py -- pure DATA: how Java types map to C++03 types.

No logic lives here (that's `types.py`); this is the lookup tables, lifted and
cleaned from the proven monolith so we keep its hard-won knowledge without its
inline `/* PORT: */` comment noise. Where the old converter embedded a PORT note
inside the type string (Object -> "void* /* PORT: was Object */"), we map to the
clean type and let `emit` record the PORT as a ledger event instead.

The game-/target-specific sets below (nullable fields/getters, value-type opt-ins, unsigned-byte
overrides) are sourced from the active target profile (`j2ctool.profiles`) rather than hardcoded, so the
core stays game-agnostic. They remain mutable module-level names: the pipeline augments
KNOWN_NULLABLE_GETTERS with corpus-derived getters at convert time and restores them afterward.
"""

from . import profiles as _profiles

# Java language primitives (value types, stack-allocated).
PRIMITIVES = {
    'boolean', 'byte', 'short', 'int', 'long', 'float', 'double', 'char', 'void',
}

# Base Java -> C++03 type names. Profile extras merge on top of this at runtime.
# SIGNED-BYTE POLICY (correct by construction): Java `byte` is SIGNED (-128..127). A SCALAR byte is
# therefore mapped to `signed char` -- the Java-faithful default -- because scalar bytes are routinely
# used as signed values (negative sentinels like -1/-100, `< 0` tests, signed deltas/offsets). Mapping
# them unsigned silently turns -1 into 255 and detonates:
#   * Packet1Login.dimension: nether = -1 read as 255 -> getProviderForDimension(255) NULL -> crash.
#   * Packet30Entity x/y/z relative-move deltas: -1 read as +255 -> entities flung into the sky.
#   * live-tree [signedbyte] annotations: side indices, flow-decay sentinels, mouse-invert, door hinge,
#     track-shape ids, model offsets -- all LOCAL byte vars that broke as unsigned.
# EXCEPTION: `byte[]` BUFFERS stay `unsigned char` (chunk/block data, packet payloads, inflate I/O --
# read via `& 0xFF` and want `unsigned char*`). That split is applied in types.cpp_type: scalar byte ->
# `signed char`, byte[] element -> `unsigned char`. `& 0xFF` still works on signed char (int-promoted),
# so buffers that also see arithmetic are unaffected. This retires the old SIGNED_BYTE_FIELDS whitelist.
JAVA_TO_CPP = {
    'boolean': 'bool', 'byte': 'signed char', 'short': 'short', 'int': 'int',
    'long': 'long long', 'float': 'float', 'double': 'double', 'char': 'char',
    'void': 'void',
    'String': 'JavaString',
    'ArrayList': 'JList', 'List': 'JList', 'LinkedList': 'JList',
    'Collection': 'JList', 'Iterable': 'JList',   # java.util.Collection/Iterable -> a by-value list
    'HashMap': 'JMap', 'HashSet': 'JSet', 'TreeMap': 'JMap', 'LinkedHashMap': 'JMap',
    'TreeSet': 'JSet',                           # sorted set -> JSet (std::set subclass w/ Java API)
    'Map': 'JMap', 'Set': 'JSet',                # Map/HashMap/... -> JMap (std::map subclass w/ Java API)
    'Random': 'Random',
    # Java root / boxed types. Object -> JObject (a boxed-value type: holds null/int/char/String/
    # String[]/typed-pointer with RTTI), so Object[] autoboxing + instanceof + reflective recipe
    # decode work. JObject is a value (NOT a pointer); it converts to T*/JavaString via operators.
    'Object': 'JObject',
    'Integer': 'int', 'Long': 'long long', 'Float': 'float', 'Double': 'double',
    'Boolean': 'bool', 'Byte': 'signed char', 'Short': 'short', 'Character': 'char',
}

# RETIRED: SIGNED_BYTE_FIELDS whitelist. Scalar `byte` is now `signed char` by DEFAULT (Java-faithful),
# so per-field curation is no longer needed -- Packet1Login.dimension, Packet30Entity deltas, and every
# unlisted scalar byte are all correctly signed by construction. Only `byte[]` buffers stay unsigned
# (see JAVA_TO_CPP note + types.cpp_type). If a specific scalar byte ever needs to be UNSIGNED, add it
# to UNSIGNED_BYTE_SCALAR_FIELDS below (empty -- none known).
UNSIGNED_BYTE_SCALAR_FIELDS = set(_profiles.active().unsigned_byte_scalar_fields)   # (ClassName, fieldName) scalar bytes forced back to unsigned char

# Java types that emit warrants a PORT note for (mapped, but lossy / needs review).
PORT_NOTE_TYPES = {
    'Object': 'was Object (untyped); verify the concrete type',
}

# Java collection types -> STL. Held BY VALUE as fields by default (std::vector/map).
CONTAINERS = {'ArrayList', 'List', 'LinkedList', 'Collection', 'Iterable', 'HashMap', 'HashSet', 'TreeMap',
              'LinkedHashMap', 'TreeSet', 'Map', 'Set'}

# default element types for a RAW (non-generic) container -> a valid C++ template
RAW_CONTAINER_DEFAULT = {'JList': '<JObject>', 'JSet': '<JObject>', 'std::vector': '<void*>', 'std::set': '<void*>',
                         # raw Map -> JMap<JObject, JObject>: a Java raw map mixes key/value types
                         # (String/Integer/Character keys; any object/boxed value). JObject boxes
                         # all of them (and has operator< for the key), so put/get/containsKey/remove
                         # all bind regardless of the concrete Java types. Keys/values convert in
                         # implicitly; reads convert out via the cast the converter already emits.
                         'JMap': '<JObject, JObject>', 'std::map': '<JObject, JObject>'}

# Java stdlib types that become C++ VALUE types (not pointers).
VALUE_STDLIB = {'String'}

# Boxed primitive wrappers -- map to C++ primitives, so they're VALUE types (not pointers).
BOXED_VALUE_TYPES = {'Integer', 'Long', 'Float', 'Double', 'Boolean', 'Byte', 'Short', 'Character'}

# Registry arrays whose slots are NULL until an id is registered. Indexing one with
# an unregistered id and calling through it = a virtual call on NULL = IP 0x0 crash
# on the console. A deref of these (or any scope array of reference type) is guarded.
KNOWN_NULLABLE = set(_profiles.active().nullable)

# Fields that are legitimately NULL at certain points in the lifecycle (set later,
# cleared on shutdown, etc.). On Xbox 360, ANY dereference of these without a prior
# null check is a hard crash (no NULL page protection). The converter must guard them.
KNOWN_NULLABLE_FIELDS = set(_profiles.active().nullable_fields)

# Getters that can return NULL (return a lookup from a sparse registry, or an
# optional reference). Chaining `getter()->method()` without a null check is a
# hard crash on the 360.
KNOWN_NULLABLE_GETTERS = set(_profiles.active().nullable_getters)

# Return type (a reference/pointer type) of each POINTER-returning nullable getter, so the null-guard
# hoist in emit/statements.py can declare a typed temp in C++03 -- `Type* t = getter();` -- instead of
# C++11 `auto`. Keyed by getter name; the emitter appends `*`. Verified against the Alpha source.
# getSmeltingResultItem is intentionally ABSENT: it returns `int` (a value), so it can never form a
# `getter()->deref` and never triggers the guard.
NULLABLE_GETTER_RETURN_TYPE = dict(_profiles.active().nullable_getter_return_type)

# Runtime-support classes the converter EMITS a header for (see runtime.py) and
# #includes when referenced -- not part of the decompiled corpus. Includes the external
# Java base types our classes extend/implement (Runnable/Thread/Comparator/...).
# Static-holder externals accessed as Name::member (constants + static functions) that get
# their own emitted header. Like GL11. Also need RUNTIME_ENUM_LIKE (no s_ prefix on members).
RUNTIME_STATIC_HOLDERS = {'GL11', 'GL12', 'GL15', 'GLU', 'NVFogDistance', 'Keyboard', 'Mouse', 'Display',
                          'ARBOcclusionQuery', 'ARBVertexBufferObject', 'BufferUtils', 'ImageIO',
                          'Sys', 'GLContext', 'Controllers', 'Collections',
                          'Pattern',                # java.util.regex Pattern.compile(...) is static
                          'Runtime', 'ByteOrder',   # Runtime.getRuntime(), ByteOrder.BIG_ENDIAN
                          'Charset',                # java.nio.charset Charset.forName("UTF-8") is static
                          'SoundSystemConfig'}

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
                    'Frame', 'Pattern', 'Matcher',
                    'FileFilter', 'ByteArrayOutputStream', 'InputStreamReader', 'HttpURLConnection',
                    'ZipEntry', 'Node', 'NodeList', 'Element', 'Document', 'DocumentBuilder',
                    'DocumentBuilderFactory', 'Date', 'SimpleDateFormat', 'GZIPInputStream',
                    'GZIPOutputStream', 'Runtime', 'ByteOrder',
                    'RandomAccessFile', 'Writer', 'StringWriter', 'PushbackReader', 'Stack',
                    'Properties', 'Entry', 'NumberFormat', 'MessageDigest', 'RenderingHints',
                    'DataFlavor', 'Reference', 'Logger', 'Calendar', 'GregorianCalendar',
                    'Toolkit', 'TextArea', 'Transferable', 'Locale', 'Rectangle',
                    'Throwable', 'Exception', 'Error', 'RuntimeException', 'IOException',
                    'InterruptedException', 'IllegalStateException', 'IllegalArgumentException',
                    'MalformedURLException', 'AWTException', 'LWJGLException', 'NoSuchFieldError',
                    'FileNotFoundException', 'EOFException', 'SocketException', 'UnknownHostException',
                    'ConnectException', 'BindException', 'NumberFormatException', 'NullPointerException',
                    'ArrayIndexOutOfBoundsException', 'IndexOutOfBoundsException', 'ClassCastException',
                    'NoSuchElementException', 'UnsupportedOperationException', 'ConcurrentModificationException',
                    'OutOfMemoryError', 'NoSuchMethodException', 'CloneNotSupportedException',
                    'ClassNotFoundException',
                    # additional java.* stubs surfaced by the Beta corpus (runtime.py _GENERATED)
                    'Inflater', 'Deflater', 'InflaterInputStream', 'DeflaterOutputStream',
                    'ByteArrayInputStream', 'DataFormatException', 'NoSuchAlgorithmException',
                    'BigInteger', 'SoftReference', 'InetAddress', 'StringReader', 'Font', 'FontMetrics',
                    'AffineTransform', 'DataBufferInt', 'Graphics2D', 'BufferStrategy', 'LibraryLWJGLOpenAL',
                    'CodecWav', 'FontMetrics', 'Iterator', 'Reader', 'DataBuffer', 'Raster', 'WritableRaster',
                    'SocketAddress', 'URI', 'Clipboard'}
                   | RUNTIME_STATIC_HOLDERS)

# Java stdlib static holders shimmed in the prelude (JavaRT.h, included everywhere -- so NOT
# in RUNTIME_HEADERS, which would emit a bogus per-type #include). `Name.member` -> `Name::member`.
JAVART_STATICS = {'System', 'Integer', 'Long', 'Double', 'Float', 'Character', 'Arrays',
                  'Short', 'Byte', 'Boolean', 'String'}   # String.valueOf/format statics (type still -> JavaString)

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
    # Math.round(x) returns an integer (round-half-up); overloaded free-function shim in the prelude
    # (int for float, long long for double) -- there is no single C stdlib call with Java's semantics.
    'round': 'javaMathRound',
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
DEFAULT_VALUE_TYPES = set(_profiles.active().default_value_types)
