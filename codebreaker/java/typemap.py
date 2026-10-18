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

from codebreaker.java import profiles as _profiles

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

# Java stdlib types that become C++ VALUE types (not pointers). ContextCapabilities (an LWJGL GL-caps
# struct stub) is here so `ContextCapabilities c = GLContext.getCapabilities()` is a VALUE local (a copy
# of the reference the stub returns), which both compiles for Indev's assign-then-read and keeps Beta's
# `getCapabilities().GL_ARB_x` working -- see the glcontext-caps-ref bugfix in runtime.py.
VALUE_STDLIB = {'String', 'ContextCapabilities'}

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
                          'MouseInfo',              # java.awt MouseInfo.getPointerInfo() is static (Indev MouseHelper)
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
                    'SocketAddress', 'URI', 'Clipboard', 'JOptionPane',
                    # java.awt mouse/point stubs surfaced by Indev's MouseHelper + level-save Dialog
                    'Point', 'PointerInfo', 'Dialog'}
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
    # TRIG is DOUBLE, not float. Java's Math.sin/cos/tan are double, and the game calls them on LARGE
    # arguments -- e.g. sin(System.currentTimeMillis()/100.0) for the pulsing block-highlight alpha. `sinf`
    # cannot range-reduce an argument that large (it returns NaN on the 360 PPC CRT), which packed the
    # highlight's alpha to 0 -> invisible. `sin` (double, native on PPC -> no perf cost) reduces correctly.
    # J2X-BUGFIX trig-double-precision.
    # J2X-BUGFIX sqrt-double-precision: Math.sqrt returns DOUBLE in Java. Mapping it to sqrtf (float) made
    # `atan2(Math.sqrt(...), doubleExpr)` an ambiguous (float,double) overload (RenderGlobal). A bare `sqrt`
    # doesn't fix it either: MSVC's <math.h> has a float sqrt(float) overload, so a float argument still
    # yields float. The JavaRT.h shim javaMathSqrt(double) forces the promotion (double sqrt is native on
    # PPC -- no perf cost). MathHelper.sqrt_float is a separate key and stays sqrtf.
    'abs': 'abs', 'sqrt': 'javaMathSqrt', 'sin': 'sin', 'cos': 'cos', 'tan': 'tan',
    'atan': 'atan', 'atan2': 'atan2', 'asin': 'asin', 'acos': 'acos',
    'floor': 'floorf', 'ceil': 'ceilf', 'pow': 'powf', 'exp': 'expf', 'log': 'logf',
    'min': 'min', 'max': 'max',
    # Math.round(x) returns an integer (round-half-up); overloaded free-function shim in the prelude
    # (int for float, long long for double) -- there is no single C stdlib call with Java's semantics.
    'round': 'javaMathRound',
    # constants (no call): Math.PI is a member, not a method -> inline the literal. A DOUBLE, as in Java.
    # J2X-BUGFIX math-pi-double: it was the float 3.1415927F, so `(float)Math.sin(i * Math.PI * 2.0D / 65536.0D)`
    # (MathHelper's SIN_TABLE) was computed with a single-precision PI and every MathHelper.sin/cos of a
    # non-zero angle came out a few ulps off Java's -- found by the java kind's behavioural judge. Where the
    # game casts it, `(float)Math.PI`, the result is the same float as before.
    'PI': '3.141592653589793',
    # no clean inline form -> free-function shims from the runtime prelude (JavaRT.h)
    'random': 'javaMathRandom', 'signum': 'javaMathSignum',
}

# EMIT form: global-qualify (`::sin`) ONLY the math functions whose C++ name is UNCHANGED from the Java
# name (sin, cos, tan, abs, ...). Those are the only ones a same-named local can shadow within its own
# initializer -- the `float sin = Math.sin(...)` C2064 from decompiled code (Entity.moveRelative). Names
# that already differ (sqrt->sqrtf, round->javaMathRound) can't collide, and `min`/`max` stay unqualified
# (they may be MSVC min/max MACROS, which `::` breaks). This is the map emit uses.
MATH_API_EMIT = {
    k: ('::' + v if (v == k and v not in ('min', 'max')) else v)
    for k, v in MATH_API.items()
}

# User-defined C++ VALUE/struct types ('.' access, no heap). EMPTY by default: a Java class
# is a REFERENCE type and must be a pointer to be universal + faithful (Minecraft's Vec3D /
# AxisAlignedBB / MovingObjectPosition are reference types AND pooled -- modeling them as
# values broke nullable override returns at scale, 742 conflicting-return errors on alpha).
# The value-type mechanism still exists; a profile/caller can opt a genuinely-by-value struct
# in via TypeResolver(value_types=...).
DEFAULT_VALUE_TYPES = set(_profiles.active().default_value_types)


def _reseed(p):
    """Re-derive every profile-sourced global from ``p`` when the active target profile changes.

    Registered as a `profiles.on_change` observer so `profiles.use()` actually retargets the
    transpiler (the module-level assignments above only seed the IMPORT-TIME profile). Each global is
    rebuilt as a fresh COPY -- never an alias of the profile's own collection -- because the pipeline
    mutates some of these in place at runtime (e.g. `KNOWN_NULLABLE_GETTERS |= corpus_getters`), and
    aliasing would leak that mutation back into the shared Profile object and across conversions."""
    global UNSIGNED_BYTE_SCALAR_FIELDS, KNOWN_NULLABLE, KNOWN_NULLABLE_FIELDS
    global KNOWN_NULLABLE_GETTERS, NULLABLE_GETTER_RETURN_TYPE, DEFAULT_VALUE_TYPES
    UNSIGNED_BYTE_SCALAR_FIELDS = set(p.unsigned_byte_scalar_fields)
    KNOWN_NULLABLE = set(p.nullable)
    KNOWN_NULLABLE_FIELDS = set(p.nullable_fields)
    KNOWN_NULLABLE_GETTERS = set(p.nullable_getters)
    NULLABLE_GETTER_RETURN_TYPE = dict(p.nullable_getter_return_type)
    DEFAULT_VALUE_TYPES = set(p.default_value_types)


_profiles.on_change(_reseed)
