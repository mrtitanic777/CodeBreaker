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
JAVA_TO_CPP = {
    'boolean': 'bool', 'byte': 'unsigned char', 'short': 'short', 'int': 'int',
    'long': 'long long', 'float': 'float', 'double': 'double', 'char': 'char',
    'void': 'void',
    'String': 'JavaString',
    'ArrayList': 'std::vector', 'List': 'std::vector', 'LinkedList': 'std::vector',
    'HashMap': 'std::map', 'HashSet': 'std::set', 'TreeMap': 'std::map',
    'Map': 'std::map', 'Set': 'std::set',
    'Random': 'Random',
    # Java root / boxed types
    'Object': 'void*',
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
RAW_CONTAINER_DEFAULT = {'std::vector': '<void*>', 'std::set': '<void*>',
                         'std::map': '<std::string, void*>'}

# Java stdlib types that become C++ VALUE types (not pointers).
VALUE_STDLIB = {'String'}

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
                          'ARBOcclusionQuery', 'ARBVertexBufferObject', 'BufferUtils', 'ImageIO'}

# Runtime-support classes the converter EMITS a header for (see runtime.py) and #includes when
# referenced -- not part of the decompiled corpus. External Java base types our classes
# extend/implement, the static holders above, NIO buffers, and java.io/awt/net instance stubs.
RUNTIME_HEADERS = ({'Random', 'Runnable', 'Thread', 'Comparator', 'File',
                    'FilenameFilter', 'Canvas', 'Applet',
                    'DisplayMode', 'FloatBuffer', 'IntBuffer', 'ByteBuffer',
                    'DataInput', 'DataOutput', 'SoundSystem', 'BufferedImage', 'BufferedReader',
                    'Graphics', 'Component', 'Cursor', 'Robot', 'URL', 'FileDialog',
                    'DecimalFormat', 'ImageObserver', 'Class',
                    'InputStream', 'OutputStream', 'DataInputStream', 'DataOutputStream',
                    'BufferedInputStream', 'BufferedOutputStream', 'FileInputStream',
                    'FileOutputStream', 'ZipFile', 'Socket', 'DateFormat', 'Dimension',
                    'KeyListener', 'MouseListener', 'MouseMotionListener', 'MouseWheelListener',
                    'WindowListener', 'ComponentListener', 'FocusListener', 'WindowAdapter',
                    'KeyEvent', 'MouseEvent', 'WindowEvent', 'ComponentEvent', 'FocusEvent',
                    'InputEvent', 'Panel', 'Comparable', 'CodecJOrbis',
                    'Throwable', 'Exception', 'Error', 'RuntimeException', 'IOException',
                    'InterruptedException', 'IllegalStateException', 'IllegalArgumentException',
                    'MalformedURLException', 'AWTException', 'LWJGLException', 'NoSuchFieldError'}
                   | RUNTIME_STATIC_HOLDERS)

# Java stdlib static holders shimmed in the prelude (JavaRT.h, included everywhere -- so NOT
# in RUNTIME_HEADERS, which would emit a bogus per-type #include). `Name.member` -> `Name::member`.
JAVART_STATICS = {'System', 'Integer', 'Long', 'Double', 'Float', 'Character', 'Arrays'}

# Runtime types whose `Type.MEMBER` (no parens) is a CONSTANT, not an instance/static field
# -- accessed as Type::MEMBER with no s_ prefix (like an enum constant). e.g. GL11.GL_TEXTURE_2D,
# Integer.MAX_VALUE, System.out.
RUNTIME_ENUM_LIKE = RUNTIME_STATIC_HOLDERS | JAVART_STATICS

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
