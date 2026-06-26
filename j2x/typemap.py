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
    'String': 'std::string',
    'ArrayList': 'std::vector', 'List': 'std::vector', 'LinkedList': 'std::vector',
    'HashMap': 'std::map', 'HashSet': 'std::set', 'TreeMap': 'std::map',
    'Map': 'std::map', 'Set': 'std::set',
    'Random': 'Random',
    # Java root / boxed types
    'Object': 'void*',
    'Integer': 'int', 'Long': 'long long', 'Float': 'float', 'Double': 'double',
    'Boolean': 'bool', 'Byte': 'unsigned char', 'Short': 'short', 'Character': 'char',
    # Java NIO buffers
    'FloatBuffer': 'float*', 'IntBuffer': 'int*', 'ByteBuffer': 'unsigned char*',
}

# Java types that emit warrants a PORT note for (mapped, but lossy / needs review).
PORT_NOTE_TYPES = {
    'Object': 'was Object (untyped); verify the concrete type',
    'FloatBuffer': 'was java.nio.FloatBuffer',
    'IntBuffer': 'was java.nio.IntBuffer',
    'ByteBuffer': 'was java.nio.ByteBuffer',
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
# #includes when referenced -- not part of the decompiled corpus.
RUNTIME_HEADERS = {'Random'}

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
}

# Classic Minecraft value/struct types (stack values, '.' access). The pipeline can
# extend this per profile; kept here as a sensible default so the resolver is usable
# stand-alone and in tests.
DEFAULT_VALUE_TYPES = {
    'Vec3D', 'Vec3', 'AxisAlignedBB', 'ChunkCoordinates', 'MovingObjectPosition',
}
