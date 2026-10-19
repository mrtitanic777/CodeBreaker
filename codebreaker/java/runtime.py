# -*- coding: utf-8 -*-
"""
runtime.py -- support files the converter emits alongside the translated classes.

These stand in for Java runtime types that aren't part of the decompiled corpus
(java.util.Random, ...). They are emitted into the output by the pipeline and
#included by any class that references them (see typemap.RUNTIME_HEADERS).
"""

import re
import os

# Canonical hand-written java.io byte-stream headers (real, not stubs). Stored verbatim in
# docs/platform/canonical/ and loaded here so the netcode/save byte streams are REAL and there is exactly
# ONE definition of each (the C4743 vtable-ODR fix). [360] NO XDK EQUIVALENT -- hand-implemented: pure C++
# big-endian byte codecs (DataInput/Output) + passthrough/streaming wrappers (Buffered/Data*Stream).
from codebreaker.java._paths import data_path as _data_path
_CANONICAL_DIR = _data_path('platform', 'canonical')

def _load_canonical(header):
    with open(os.path.join(_CANONICAL_DIR, header), encoding='utf-8') as f:
        return f.read()

# Faithful java.util.Random: 48-bit LCG with Java's exact constants, so generated
# worlds/sequences match the Java original bit-for-bit.
RANDOM_H = _load_canonical('Random.h')

# --- external Java base types our corpus extends/implements but that aren't decompiled.
# Minimal but faithful stubs so inheritance + overrides compile (a missing base header is
# a fatal include error that blocks the whole translation unit). ---------------------------

RUNNABLE_H = _load_canonical('Runnable.h')

THREAD_H = _load_canonical('Thread.h')

COMPARATOR_H = _load_canonical('Comparator.h')

FILE_H = r'''#ifndef FILE_H
#define FILE_H
// java.io.File (stub)
#include <string>

class File {
public:
	File() {}
	File(const std::string&) {}
	File(File*, const std::string&) {}
	virtual ~File() {}
	std::string getName() { return std::string(); }
	std::string getPath() { return std::string(); }
	std::string getAbsolutePath() { return std::string(); }
	bool exists() { return false; }
	bool isDirectory() { return false; }
	bool isFile() { return false; }
	bool mkdir() { return false; }
	bool mkdirs() { return false; }
	bool renameTo(File*) { return false; }
	long long length() { return 0LL; }
};
#endif // FILE_H
'''

FILENAMEFILTER_H = _load_canonical('FilenameFilter.h')

CANVAS_H = _load_canonical('Canvas.h')

APPLET_H = _load_canonical('Applet.h')

# Universal runtime prelude: free-function shims for java.lang.* statics that have no
# clean inline mapping. #included by every emitted .cpp (named JavaRT.h, NOT Math.h, to
# avoid colliding with system <math.h> on a case-insensitive filesystem).
# java.lang.String -> a thin std::string subclass that adds Java's String methods, so calls
# like s.equals(t) / s.trim() / s.startsWith(p) compile even when the receiver's type wasn't
# resolved at translation time (the receiver-type-gated rewrites in statements.py handle the
# resolved cases; this catches the rest). Implicit ctors from const char*/std::string keep
# literals and concat/substr results assignable.
JAVASTRING_H = _load_canonical('JavaString.h')

JARRAY_H = _load_canonical('JArray.h')

JITERATOR_H = _load_canonical('JIterator.h')

JLIST_H = _load_canonical('JList.h')

# HashSet / TreeSet -> JSet<T>: a std::set subclass with the Java Set API (add/addAll/removeAll/
# contains/isEmpty/first + collection-copy ctor). Mirrors JList so set method calls compile without
# per-receiver type inference. Ordered (std::set) -> TreeSet's sorted iteration is preserved for free.
JSET_H = _load_canonical('JSet.h')

# HashMap / TreeMap / LinkedHashMap / Map -> JMap<K,V>: a std::map subclass with the Java Map API
# (put/get/containsKey/remove/isEmpty + keySet/values/entrySet views + null-compare). Mirrors JList/
# JSet so map method calls compile WITHOUT per-receiver type inference -- essential on chained/foreign
# receivers (map.entrySet().iterator().next(), a method call whose result is a map). std::map is the
# base, so every rewrite the emitter already does (put->[k]=v, get->[k], containsKey->count,
# remove->erase, isEmpty->empty, clear/size inherited) keeps working; the members catch the calls the
# emitter leaves intact. Views return by-value JList copies so they iterate, feed a JSet/JList copy-ctor
# (new HashSet(map.keySet())), and expose .iterator(). Order is key-sorted (TreeMap-faithful).
JMAP_H = _load_canonical('JMap.h')

JAVART_H = _load_canonical('JavaRT.h')

GL11_H = r'''#ifndef GL11_H
#define GL11_H
// LWJGL org.lwjgl.opengl.GL11 stub -- symbols only, so render code COMPILES; the real
// pipeline is the platform layer (Xbox 360 = Direct3D 9). Values are real GL 1.1 enums.

namespace GL11 {
	const int GL_POINTS = 0x0000;
	const int GL_TRIANGLES = 0x0004;
	const int GL_ZERO = 0x0000;
	const int GL_FRONT = 0x0404;
	const int GL_BACK = 0x0405;
	const int GL_FRONT_AND_BACK = 0x0408;
	const int GL_LIGHTING = 0x0B50;
	const int GL_LIGHT0 = 0x4000;
	const int GL_LIGHT1 = 0x4001;
	const int GL_FOG = 0x0B60;
	const int GL_FOG_MODE = 0x0B65;
	const int GL_FOG_DENSITY = 0x0B62;
	const int GL_FOG_START = 0x0B63;
	const int GL_FOG_END = 0x0B64;
	const int GL_FOG_COLOR = 0x0B66;
	const int GL_DEPTH_TEST = 0x0B71;
	const int GL_CULL_FACE = 0x0B44;
	const int GL_ALPHA_TEST = 0x0BC0;
	const int GL_BLEND = 0x0BE2;
	const int GL_TEXTURE_2D = 0x0DE1;
	const int GL_NORMALIZE = 0x0BA1;
	const int GL_COLOR_MATERIAL = 0x0B57;
	const int GL_MODELVIEW = 0x1700;
	const int GL_PROJECTION = 0x1701;
	const int GL_SMOOTH = 0x1D01;
	const int GL_FLAT = 0x1D00;
	const int GL_EXP = 0x0800;
	const int GL_LINEAR = 0x2601;
	const int GL_NEAREST = 0x2600;
	const int GL_GREATER = 0x0204;
	const int GL_GEQUAL = 0x0206;
	const int GL_EQUAL = 0x0202;
	const int GL_LEQUAL = 0x0203;
	const int GL_SRC_ALPHA = 0x0302;
	const int GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	const int GL_ONE = 0x0001;
	const int GL_DST_COLOR = 0x0306;
	const int GL_SRC_COLOR = 0x0300;
	const int GL_ONE_MINUS_SRC_COLOR = 0x0301;
	const int GL_ONE_MINUS_DST_COLOR = 0x0307;
	const int GL_DST_ALPHA = 0x0304;
	const int GL_RGB = 0x1907;
	const int GL_RGBA = 0x1908;
	const int GL_UNSIGNED_BYTE = 0x1401;
	const int GL_BYTE = 0x1400;
	const int GL_FLOAT = 0x1406;
	const int GL_TEXTURE = 0x1702;
	const int GL_NORMAL_ARRAY = 0x8075;
	const int GL_TEXTURE_MAG_FILTER = 0x2800;
	const int GL_TEXTURE_MIN_FILTER = 0x2801;
	const int GL_TEXTURE_WRAP_S = 0x2802;
	const int GL_TEXTURE_WRAP_T = 0x2803;
	const int GL_NEAREST_MIPMAP_LINEAR = 0x2702;
	const int GL_LINEAR_MIPMAP_LINEAR = 0x2703;
	const int GL_REPEAT = 0x2901;
	const int GL_CLAMP = 0x2900;
	const int GL_COLOR_BUFFER_BIT = 0x4000;
	const int GL_DEPTH_BUFFER_BIT = 0x0100;
	const int GL_COMPILE = 0x1300;
	const int GL_VERTEX_ARRAY = 0x8074;
	const int GL_COLOR_ARRAY = 0x8076;
	const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	const int GL_AMBIENT = 0x1200;
	const int GL_DIFFUSE = 0x1201;
	const int GL_SPECULAR = 0x1202;
	const int GL_POSITION = 0x1203;
	const int GL_AMBIENT_AND_DIFFUSE = 0x1602;
	const int GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	const int GL_PACK_ALIGNMENT = 0x0D05;
	const int GL_UNPACK_ALIGNMENT = 0x0CF5;
	const int GL_VENDOR = 0x1F00;
	const int GL_RENDERER = 0x1F01;
	const int GL_VERSION = 0x1F02;
	const int GL_POLYGON_OFFSET_FILL = 0x8037;
	const int GL_PROJECTION_MATRIX = 0x0BA7;
	const int GL_MODELVIEW_MATRIX = 0x0BA6;

	inline void glAlphaFunc(...) {}
	inline void glBindTexture(...) {}
	inline void glBlendFunc(...) {}
	inline void glCallList(...) {}
	inline void glCallLists(...) {}
	inline void glClear(...) {}
	inline void glClearColor(...) {}
	inline void glClearDepth(...) {}
	inline void glColor3f(...) {}
	inline void glColor4f(...) {}
	inline void glColorMask(...) {}
	inline void glColorMaterial(...) {}
	inline void glColorPointer(...) {}
	inline void glCullFace(...) {}
	inline void glDeleteTextures(...) {}
	inline void glDepthFunc(...) {}
	inline void glDepthMask(...) {}
	inline void glDisable(...) {}
	inline void glDisableClientState(...) {}
	inline void glDrawArrays(...) {}
	inline void glEnable(...) {}
	inline void glEnableClientState(...) {}
	inline void glEndList(...) {}
	inline void glFog(...) {}
	inline void glFogf(...) {}
	inline void glFogi(...) {}
	inline void glGenTextures(...) {}
	inline void glLight(...) {}
	inline void glLightModel(...) {}
	inline void glLineWidth(...) {}
	inline void glLoadIdentity(...) {}
	inline void glMatrixMode(...) {}
	inline void glMultMatrix(...) {}
	inline void glNewList(...) {}
	inline void glNormal3f(...) {}
	inline void glNormalPointer(...) {}
	inline void glOrtho(...) {}
	inline void glPixelStorei(...) {}
	inline void glPopMatrix(...) {}
	inline void glPushMatrix(...) {}
	inline void glReadPixels(...) {}
	inline void glRenderList(...) {}
	inline void glRotatef(...) {}
	inline void glScaled(...) {}
	inline void glScalef(...) {}
	inline void glShadeModel(...) {}
	inline void glSkyList(...) {}
	inline void glTexCoordPointer(...) {}
	inline void glTexImage2D(...) {}
	inline void glTexParameteri(...) {}
	inline void glTexSubImage2D(...) {}
	inline void glTranslatef(...) {}
	inline void glVertexPointer(...) {}
	inline void glViewport(...) {}
	inline int glGenLists(...) { return 1; }
	inline int glGenList(...) { return 1; }
	inline int glGetInteger(...) { return 0; }
	inline const char* glGetString(...) { return ""; }
	inline int glGetError() { return 0; }
	inline void glDeleteLists(...) {}
	inline void glPolygonOffset(...) {}
	inline void glGetFloat(...) {}
}
#endif // GL11_H
'''

# --- generated stubs for the remaining external platform/IO/AWT types ---------------------
# These are SYMBOL stubs so game logic COMPILES; the real implementations are the Xbox 360
# platform layer (Direct3D 9 / XInput / sound / file IO), out of scope for the converter.

def _guard(name):
    return name.upper().replace('.', '_') + '_H'


# Best-effort Java package for the external types we stub, so the banner names the real library.
_EXT_PKG = {
    'GL11': 'org.lwjgl.opengl', 'GL12': 'org.lwjgl.opengl', 'GL15': 'org.lwjgl.opengl',
    'NVFogDistance': 'org.lwjgl.opengl',
    'GLU': 'org.lwjgl.util.glu', 'GLContext': 'org.lwjgl.opengl', 'ARBOcclusionQuery': 'org.lwjgl.opengl',
    'ARBVertexBufferObject': 'org.lwjgl.opengl', 'Keyboard': 'org.lwjgl.input', 'Mouse': 'org.lwjgl.input',
    'Controllers': 'org.lwjgl.input', 'Display': 'org.lwjgl.opengl', 'DisplayMode': 'org.lwjgl.opengl',
    'BufferUtils': 'org.lwjgl', 'Sys': 'org.lwjgl',
    'File': 'java.io', 'InputStream': 'java.io', 'OutputStream': 'java.io', 'DataInput': 'java.io',
    'DataOutput': 'java.io', 'DataInputStream': 'java.io', 'DataOutputStream': 'java.io',
    'BufferedInputStream': 'java.io', 'BufferedOutputStream': 'java.io', 'FileInputStream': 'java.io',
    'FileOutputStream': 'java.io', 'BufferedReader': 'java.io',
    'FileReader': 'java.io', 'FileWriter': 'java.io', 'PrintWriter': 'java.io',
    'Socket': 'java.net', 'URL': 'java.net', 'URLConnection': 'java.net',
    'SoundSystem': 'paulscode.sound', 'CodecJOrbis': 'paulscode.sound.codecs',
    'ImageIO': 'javax.imageio', 'BufferedImage': 'java.awt.image', 'ImageObserver': 'java.awt.image',
    'ZipFile': 'java.util.zip', 'DecimalFormat': 'java.text', 'DateFormat': 'java.text',
    'Class': 'java.lang', 'Comparable': 'java.lang', 'Comparator': 'java.util',
    'Canvas': 'java.awt', 'Component': 'java.awt', 'Frame': 'java.awt', 'Panel': 'java.awt',
    'Graphics': 'java.awt', 'Color': 'java.awt', 'Cursor': 'java.awt', 'Robot': 'java.awt',
    'FileDialog': 'java.awt', 'Dimension': 'java.awt', 'Applet': 'java.applet',
    'BorderLayout': 'java.awt', 'GridLayout': 'java.awt', 'FlowLayout': 'java.awt',
    'CardLayout': 'java.awt', 'GridBagLayout': 'java.awt',
    'Point': 'java.awt', 'MouseInfo': 'java.awt', 'PointerInfo': 'java.awt', 'Dialog': 'java.awt',
    'FloatBuffer': 'java.nio', 'IntBuffer': 'java.nio', 'ByteBuffer': 'java.nio',
}


def _ext_banner(name):
    """Header banner marking a SYMBOL-ONLY stub for an external Java library type (the user asked
    every such stub to say so). Names the real package when known. These are NOT decompiled game
    source; their behaviour is the Xbox 360 platform layer's job."""
    from codebreaker.java.console import platform_equivalents as _pe   # lazy import (platform_equivalents has no deps -> no cycle)
    pkg = _EXT_PKG.get(name)
    who = ('%s.%s' % (pkg, name)) if pkg else ('the external Java type %s' % name)
    banner = ['',
            '// ============================ EXTERNAL-LIBRARY STUB ============================',
            '// Stands in for %s.' % who,
            '// This is NOT part of the decompiled Minecraft source -- it is an external Java',
            '// library. Symbol-only: methods are no-ops / return defaults so the converted game',
            '// code compiles and links; the real behaviour is the Xbox 360 platform layer',
            '// (Direct3D 9 / XInput / file IO / sound).']
    # [360] equivalent: name the exact XDK API to resolve this stub to (from platform_equivalents.py) so nobody
    # hand-guesses the port. Absent line = we have no mapping yet for this symbol (add it to the table).
    ann = _pe.annotation(name)
    if ann:
        banner.append('// ------------------------------------------------------------------------------')
        banner.append(ann)
    banner.append('// ==============================================================================')
    return banner


def _ns_stub(name, consts=(), void_fns=(), ret_fns=(), includes=(), fwds=()):
    """A namespace of static constants + functions (e.g. GL15, Keyboard) -- accessed Name::X."""
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + ['#include <string>']
    L += ['#include "%s.h"' % h for h in includes]
    L += ['class %s;' % f for f in fwds]
    L += ['', 'namespace %s {' % name]
    for k, v in consts:
        L.append('\tconst int %s = %s;' % (k, v))
    # every stub method carries a greppable J2X-STUB marker so a scan (`grep -rn J2X-STUB`) enumerates
    # the ENTIRE placeholder surface -- a no-op that turns out to be load-bearing (the glPolygonOffset
    # z-fight class) is never left silently undone.
    for f in void_fns:
        L.append('\tinline void %s(...) {}   // J2X-STUB[%s]: no-op placeholder -- verify not load-bearing' % (f, name))
    for f, (rt, rv) in ret_fns:
        L.append('\tinline %s %s(...) { return %s; }   // J2X-STUB[%s]: returns default -- verify' % (rt, f, rv, name))
    L += ['}', '#endif // %s' % g, '']
    return '\n'.join(L)


# Return-type registry for the emitted stub classes, so expression-type inference (the `.` vs `->`
# chain resolver in emit/statements.py) can type a chain THROUGH a stub method result -- e.g.
# `Runtime::getRuntime()->maxMemory()`, `getDocumentBase()->getHost()`, `var->getParentFile()->mkdirs()`.
# Populated as each stub is built: {class: {'parent': base_or_None, 'methods': {method: cpp_ret_type}}}.
# Last definition wins (matches _GENERATED overwrite for classes redefined, e.g. File). The pipeline
# converts the C++ return spellings to JTypes and merges this into the member environment.
STUB_MEMBERS = {}

# `Type* method(` / `static Type* method(` inside an `extra` line (static factories & pointer getters
# defined verbatim rather than via ret_methods, e.g. Runtime::getRuntime, Toolkit::getDefaultToolkit).
_EXTRA_PTR_METHOD = re.compile(r'(?:static\s+)?([A-Za-z_]\w*)\s*\*\s*(\w+)\s*\(')


def _class_stub(name, void_methods=(), ret_methods=(), ctors=True, bases=(),
                includes=(), fwds=(), extra=()):
    """A class with variadic stub methods (e.g. SoundSystem, DataInput) -- accessed var->X.
    `extra` = verbatim extra member lines (e.g. conversion operators)."""
    rec = {'parent': bases[0] if bases else None, 'methods': {}}
    for _m, (_rt, _rv) in ret_methods:
        rec['methods'][_m] = _rt
    for _line in extra:
        _pm = _EXTRA_PTR_METHOD.search(_line)
        if _pm and _pm.group(1) != 'void':
            rec['methods'][_pm.group(2)] = _pm.group(1) + '*'
    STUB_MEMBERS[name] = rec                          # last def wins (mirrors _GENERATED overwrite)
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + ['#include <string>', '#include "JavaString.h"']
    L += ['#include "%s.h"' % h for h in includes]
    L += ['class %s;' % f for f in fwds]
    decl = 'class %s' % name + ((' : ' + ', '.join('public %s' % b for b in bases)) if bases else '')
    L += ['', decl + ' {', 'public:']
    if ctors:
        L.append('\t%s(...) {}' % name)
    L.append('\tvirtual ~%s() {}' % name)
    # J2X-STUB marker on every stub method -> `grep -rn J2X-STUB` enumerates the full placeholder surface.
    for m in void_methods:
        L.append('\tvirtual void %s(...) {}   // J2X-STUB[%s]: no-op placeholder -- verify not load-bearing' % (m, name))
    for m, (rt, rv) in ret_methods:
        # A method that returns a Java String must return JavaString, not std::string -- else the
        # caller's `.startsWith()/.substring()/.trim()` (Java String methods) fail to compile.
        if rt == 'std::string':
            rt, rv = 'JavaString', 'JavaString()'
        L.append('\tvirtual %s %s(...) { return %s; }   // J2X-STUB[%s]: returns default -- verify' % (rt, m, rv, name))
    for line in extra:
        L.append('\t' + line)
    L += ['};', '#endif // %s' % g, '']
    return '\n'.join(L)


# NIO buffers: real classes (not raw pointers) so the .clear()/.put()/.flip() calls compile.
# Cross-return by pointer (forward-declared) to avoid include cycles.
def _buffer_stub(name):
    g = _guard(name)
    L = ['#ifndef %s' % g, '#define %s' % g] + _ext_banner(name) + [
         'class FloatBuffer; class IntBuffer; class ByteBuffer;', '',
         'class %s {' % name, 'public:',
         '\t%s() {}' % name, '\tvirtual ~%s() {}' % name]
    for m in ('clear', 'flip', 'rewind', 'mark'):                # no-arg, return self
        L.append('\t%s& %s() { return *this; }' % (name, m))
    for m in ('put', 'putInt', 'putShort', 'putLong', 'putFloat', 'putDouble', 'putChar'):
        L.append('\t%s& %s(...) { return *this; }' % (name, m))   # variadic, return self
    L += ['\tint get(...) { return 0; }', '\tint getInt(...) { return 0; }',
          '\tshort getShort(...) { return 0; }', '\tlong long getLong(...) { return 0; }',
          '\tfloat getFloat(...) { return 0.0F; }', '\tdouble getDouble(...) { return 0.0; }',
          '\tchar getChar(...) { return 0; }']
    for m in ('position', 'limit'):                              # getter + setter (no clash)
        L.append('\tint %s() { return 0; }' % m)
        L.append('\t%s& %s(int) { return *this; }' % (name, m))
    L += ['\tint remaining() { return 0; }', '\tint capacity() { return 0; }',
          '\tFloatBuffer* asFloatBuffer() { return 0; }', '\tIntBuffer* asIntBuffer() { return 0; }',
          '\t%s* order(...) { return this; }' % name,           # ByteBuffer.order(ByteOrder) -> self (ptr; matches STUB_MEMBERS + chains into a ByteBuffer* local)
          '\tstatic %s* allocateDirect(...) { return new %s(); }' % (name, name),   # ByteBuffer.allocateDirect(n)
          '\tstatic %s* allocate(...) { return new %s(); }' % (name, name),
          '\toperator void*() { return 0; }', '};', '#endif // %s' % g, '']
    return '\n'.join(L)


_GENERATED = {}
# common java.awt Component/Container surface, shared by Canvas/Panel/Component/Applet stubs
_AWT_VOID = ['setFocusable', 'setSize', 'repaint', 'requestFocus', 'setVisible', 'addNotify',
             'removeNotify', 'setPreferredSize', 'setMinimumSize', 'setMaximumSize', 'enableInputMethods',
             'setBackground', 'setForeground', 'validate', 'setLayout', 'add', 'remove', 'removeAll',
             'addKeyListener', 'addMouseListener', 'addMouseMotionListener', 'addMouseWheelListener',
             'addFocusListener', 'addComponentListener', 'addWindowListener', 'setFocusTraversalKeysEnabled',
             'setCursor', 'transferFocus', 'paint', 'update', 'doLayout',
             'invalidate', 'revalidate', 'pack']
# LWJGL input / display (static holders)
_GENERATED['Keyboard.h'] = _ns_stub('Keyboard',
    consts=[('KEY_ESCAPE', 1),
            ('KEY_1', 2), ('KEY_2', 3), ('KEY_3', 4), ('KEY_4', 5), ('KEY_5', 6),
            ('KEY_6', 7), ('KEY_7', 8), ('KEY_8', 9), ('KEY_9', 10), ('KEY_0', 11),
            ('KEY_MINUS', 12), ('KEY_EQUALS', 13), ('KEY_BACK', 14), ('KEY_TAB', 15),
            ('KEY_Q', 16), ('KEY_W', 17), ('KEY_E', 18), ('KEY_R', 19), ('KEY_T', 20),
            ('KEY_Y', 21), ('KEY_U', 22), ('KEY_I', 23), ('KEY_O', 24), ('KEY_P', 25),
            ('KEY_LBRACKET', 26), ('KEY_RBRACKET', 27), ('KEY_RETURN', 28), ('KEY_LCONTROL', 29),
            ('KEY_A', 30), ('KEY_S', 31), ('KEY_D', 32), ('KEY_F', 33), ('KEY_G', 34),
            ('KEY_H', 35), ('KEY_J', 36), ('KEY_K', 37), ('KEY_L', 38), ('KEY_SEMICOLON', 39),
            ('KEY_APOSTROPHE', 40), ('KEY_GRAVE', 41), ('KEY_LSHIFT', 42), ('KEY_BACKSLASH', 43),
            ('KEY_Z', 44), ('KEY_X', 45), ('KEY_C', 46), ('KEY_V', 47), ('KEY_B', 48),
            ('KEY_N', 49), ('KEY_M', 50), ('KEY_COMMA', 51), ('KEY_PERIOD', 52), ('KEY_SLASH', 53),
            ('KEY_RSHIFT', 54), ('KEY_MULTIPLY', 55), ('KEY_LMENU', 56), ('KEY_SPACE', 57),
            ('KEY_CAPITAL', 58),
            ('KEY_F1', 59), ('KEY_F2', 60), ('KEY_F3', 61), ('KEY_F4', 62), ('KEY_F5', 63),
            ('KEY_F6', 64), ('KEY_F7', 65), ('KEY_F8', 66), ('KEY_F9', 67), ('KEY_F10', 68),
            ('KEY_NUMLOCK', 69), ('KEY_SCROLL', 70), ('KEY_F11', 87), ('KEY_F12', 88),
            ('KEY_NUMPAD0', 82), ('KEY_NUMPAD1', 79), ('KEY_NUMPAD2', 80), ('KEY_NUMPAD3', 81),
            ('KEY_NUMPAD4', 75), ('KEY_NUMPAD5', 76), ('KEY_NUMPAD6', 77), ('KEY_NUMPAD7', 71),
            ('KEY_NUMPAD8', 72), ('KEY_NUMPAD9', 73), ('KEY_ADD', 78), ('KEY_SUBTRACT', 74),
            ('KEY_DECIMAL', 83), ('KEY_RCONTROL', 157), ('KEY_RMENU', 184), ('KEY_HOME', 199),
            ('KEY_UP', 200), ('KEY_PRIOR', 201), ('KEY_LEFT', 203), ('KEY_RIGHT', 205),
            ('KEY_END', 207), ('KEY_DOWN', 208), ('KEY_NEXT', 209), ('KEY_INSERT', 210),
            ('KEY_DELETE', 211), ('CHAR_NONE', 0), ('KEY_NONE', 0),
            ('KEY_LMETA', 219), ('KEY_RMETA', 221), ('KEY_LWIN', 219), ('KEY_RWIN', 221),
            ('KEY_APPS', 221)],
    void_fns=['create', 'destroy', 'enableRepeatEvents'],
    ret_fns=[('getEventKey', ('int', '0')), ('getEventKeyState', ('bool', 'false')),
             ('getEventCharacter', ('char', '0')), ('isKeyDown', ('bool', 'false')),
             ('next', ('bool', 'false')), ('getKeyName', ('std::string', 'std::string()'))])
# LWJGL 2's Keyboard.getKeyName: a key's constant without KEY_ (17 -> "W"), null for an unnamed code --
# what the options screen prints for each binding. Taken from lwjgl-2.9.3 itself (getKeyName(0..255)).
# FIXED BUG keyname-stub: it returned "", so every binding read "Forward: ". The 360 replaces Keyboard.h
# with the Input reference layer, which has its own.
_LWJGL_KEY_NAMES = dict(kv.split('=', 1) for kv in (
    '0=NONE 1=ESCAPE 2=1 3=2 4=3 5=4 6=5 7=6 8=7 9=8 10=9 11=0 12=MINUS 13=EQUALS 14=BACK 15=TAB 16=Q '
    '17=W 18=E 19=R 20=T 21=Y 22=U 23=I 24=O 25=P 26=LBRACKET 27=RBRACKET 28=RETURN 29=LCONTROL 30=A 31=S '
    '32=D 33=F 34=G 35=H 36=J 37=K 38=L 39=SEMICOLON 40=APOSTROPHE 41=GRAVE 42=LSHIFT 43=BACKSLASH 44=Z '
    '45=X 46=C 47=V 48=B 49=N 50=M 51=COMMA 52=PERIOD 53=SLASH 54=RSHIFT 55=MULTIPLY 56=LMENU 57=SPACE '
    '58=CAPITAL 59=F1 60=F2 61=F3 62=F4 63=F5 64=F6 65=F7 66=F8 67=F9 68=F10 69=NUMLOCK 70=SCROLL '
    '71=NUMPAD7 72=NUMPAD8 73=NUMPAD9 74=SUBTRACT 75=NUMPAD4 76=NUMPAD5 77=NUMPAD6 78=ADD 79=NUMPAD1 '
    '80=NUMPAD2 81=NUMPAD3 82=NUMPAD0 83=DECIMAL 87=F11 88=F12 100=F13 101=F14 102=F15 103=F16 104=F17 '
    '105=F18 112=KANA 113=F19 121=CONVERT 123=NOCONVERT 125=YEN 141=NUMPADEQUALS 144=CIRCUMFLEX 145=AT '
    '146=COLON 147=UNDERLINE 148=KANJI 149=STOP 150=AX 151=UNLABELED 156=NUMPADENTER 157=RCONTROL '
    '167=SECTION 179=NUMPADCOMMA 181=DIVIDE 183=SYSRQ 184=RMENU 196=FUNCTION 197=PAUSE 199=HOME 200=UP '
    '201=PRIOR 203=LEFT 205=RIGHT 207=END 208=DOWN 209=NEXT 210=INSERT 211=DELETE 218=CLEAR 219=LMETA '
    '220=RMETA 221=APPS 222=POWER 223=SLEEP '
).split())
_kn = ['\tinline JavaString getKeyName(int key) {', '\t\tswitch (key) {']
_kn += ['\t\tcase %s: return JavaString("%s");' % (k, v) for k, v in sorted(_LWJGL_KEY_NAMES.items(), key=lambda kv: int(kv[0]))]
_kn += ['\t\t}', '\t\treturn JavaString();   // null, as LWJGL', '\t}',
        '\tinline int getKeyIndex(const std::string& name) {']
_kn += ['\t\tif (name == "%s") return %s;' % (v, k) for k, v in sorted(_LWJGL_KEY_NAMES.items(), key=lambda kv: int(kv[0]))]
_kn += ['\t\treturn 0;   // KEY_NONE, as LWJGL', '\t}']
_GENERATED['Keyboard.h'] = _GENERATED['Keyboard.h'].replace(
    '\tinline std::string getKeyName(...) { return std::string(); }   // J2X-STUB[Keyboard]: returns default -- verify',
    '\n'.join(_kn)).replace('#include <string>', '#include <string>\n#include "JavaString.h"', 1)
_GENERATED['Mouse.h'] = _ns_stub('Mouse',
    void_fns=['create', 'destroy', 'setNativeCursor', 'setGrabbed', 'setCursorPosition'],
    ret_fns=[('getDX', ('int', '0')), ('getDY', ('int', '0')), ('getX', ('int', '0')),
             ('getY', ('int', '0')), ('getEventButton', ('int', '0')),
             ('getEventButtonState', ('bool', 'false')), ('getEventDWheel', ('int', '0')),
             ('getEventX', ('int', '0')), ('getEventY', ('int', '0')),
             ('isButtonDown', ('bool', 'false')), ('next', ('bool', 'false'))])
_GENERATED['DisplayMode.h'] = _class_stub('DisplayMode',
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getFrequency', ('int', '0')), ('getBitsPerPixel', ('int', '0'))])
_GENERATED['Display.h'] = _ns_stub('Display', includes=['DisplayMode'],
    void_fns=['create', 'destroy', 'setDisplayMode', 'setFullscreen', 'setParent',
              'setTitle', 'update', 'swapBuffers', 'setVSyncEnabled', 'setIcon', 'sync',
              'setResizable', 'setLocation', 'releaseContext', 'makeCurrent'],
    ret_fns=[('isActive', ('bool', 'false')), ('isCloseRequested', ('bool', 'false')),
             ('isCreated', ('bool', 'false')), ('isDirty', ('bool', 'false')),
             ('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
             ('getDisplayMode', ('DisplayMode', 'DisplayMode()')),
             ('getDesktopDisplayMode', ('DisplayMode', 'DisplayMode()')),
             ('getAvailableDisplayModes', ('void*', '0'))])
_GENERATED['GL15.h'] = _ns_stub('GL15',
    consts=[('GL_ARRAY_BUFFER', 0x8892), ('GL_STREAM_DRAW', 0x88E0), ('GL_SAMPLES_PASSED', 0x8914),
            ('GL_QUERY_RESULT', 0x8866), ('GL_QUERY_RESULT_AVAILABLE', 0x8867),
            ('GL_QUERY_COUNTER_BITS', 0x8864)])
_GENERATED['GL12.h'] = _ns_stub('GL12',
    consts=[('GL_RESCALE_NORMAL', 0x803A), ('GL_CLAMP_TO_EDGE', 0x812F),
            ('GL_SMOOTH_POINT_SIZE_RANGE', 0x0B12)])
# GL_NV_fog_distance extension (EntityRenderer picks radial fog when GL_NV_fog_distance is present).
_GENERATED['NVFogDistance.h'] = _ns_stub('NVFogDistance',
    consts=[('GL_FOG_DISTANCE_MODE_NV', 0x855A), ('GL_EYE_RADIAL_NV', 0x855B),
            ('GL_EYE_PLANE_ABSOLUTE_NV', 0x855C)])
_GENERATED['GLU.h'] = _ns_stub('GLU', void_fns=['gluPerspective', 'gluLookAt', 'gluProject', 'gluUnProject'],
    ret_fns=[('gluErrorString', ('std::string', 'std::string()')), ('gluBuild2DMipmaps', ('int', '0'))])
_GENERATED['ARBOcclusionQuery.h'] = _ns_stub('ARBOcclusionQuery',
    void_fns=['glBeginQueryARB', 'glEndQueryARB', 'glGetQueryObjectuARB', 'glGetQueryARB'],
    ret_fns=[('glGenQueriesARB', ('int', '0'))])
_GENERATED['ARBVertexBufferObject.h'] = _ns_stub('ARBVertexBufferObject',
    void_fns=['glBindBufferARB', 'glBufferDataARB', 'glDeleteBuffersARB'],
    ret_fns=[('glGenBuffersARB', ('int', '0'))])
# org.lwjgl.BufferUtils -- REAL direct-NIO allocation (was returning NULL -> F2 screenshot NULL-deref crash).
_GENERATED['BufferUtils.h'] = (
    '#ifndef BUFFERUTILS_H\n#define BUFFERUTILS_H\n'
    '#include "ByteBuffer.h"\n#include "IntBuffer.h"\n#include "FloatBuffer.h"\n'
    '// org.lwjgl.BufferUtils -- direct NIO buffer allocation.\n'
    '// [360] NO XDK EQUIVALENT -- hand-implemented: plain heap (NioBacking) via ByteBuffer::allocateDirect.\n'
    '// FIXED BUG bufferutils-null-screenshot: createByteBuffer returned NULL -> the F2 screenshot path\n'
    '// NULL-dereferenced. as{Int,Float}Buffer are views over the same backing.\n'
    'namespace BufferUtils {\n'
    '\tinline ByteBuffer*  createByteBuffer(int n)  { return ByteBuffer::allocateDirect(n); }\n'
    '\tinline IntBuffer*   createIntBuffer(int n)   { return ByteBuffer::allocateDirect(n * 4)->asIntBuffer(); }\n'
    '\tinline FloatBuffer* createFloatBuffer(int n) { return ByteBuffer::allocateDirect(n * 4)->asFloatBuffer(); }\n'
    '}\n#endif // BUFFERUTILS_H\n')
_GENERATED['ImageIO.h'] = _ns_stub('ImageIO', fwds=['BufferedImage'],
    void_fns=['write'], ret_fns=[('read', ('BufferedImage*', '0'))])
# javax.swing.JOptionPane: desktop dialogs, dead on console. Games call it on fatal-error paths
# (RubyDung: JOptionPane.showMessageDialog on an LWJGL init failure). A compile-only namespace stub
# is enough -- showMessageDialog is a no-op (route to the debug log if ever reached); the *Dialog
# queries return defaults. Emitting it also makes the converter resolve `JOptionPane.x` -> `::x`
# (else it stays an undeclared `JOptionPane.showMessageDialog`).
_GENERATED['JOptionPane.h'] = _ns_stub('JOptionPane',
    consts=[('YES_OPTION', '0'), ('NO_OPTION', '1'), ('CANCEL_OPTION', '2'),
            ('OK_OPTION', '0'), ('ERROR_MESSAGE', '0'), ('INFORMATION_MESSAGE', '1')],
    void_fns=['showMessageDialog'],
    ret_fns=[('showConfirmDialog', ('int', '2')), ('showInputDialog', ('void*', '0'))])
# NIO buffers (real classes)
for _b in ('FloatBuffer', 'IntBuffer', 'ByteBuffer'):
    _GENERATED[_b + '.h'] = _buffer_stub(_b)
# java.io data streams (interfaces -> default-impl structs)
# REAL canonical java.io byte codecs (big-endian, NBT/packet-faithful) -- were no-op stubs.
_GENERATED['DataInput.h'] = _load_canonical('DataInput.h')
_GENERATED['DataOutput.h'] = _load_canonical('DataOutput.h')
# java.util.regex: a backtracking matcher for the subset the games use (Regex.h), under real Pattern and
# Matcher classes (Beta's chunk-file names, JSON numbers) -- were stubs whose matches() was always false.
_GENERATED['Regex.h'] = _load_canonical('Regex.h')
_GENERATED['Matcher.h'] = _load_canonical('Matcher.h')
_GENERATED['Pattern.h'] = _load_canonical('Pattern.h')
STUB_MEMBERS['Matcher'] = {'parent': None, 'methods': {'matches': 'bool', 'find': 'bool', 'lookingAt': 'bool',
    'group': 'JavaString', 'groupCount': 'int', 'start': 'int', 'end': 'int', 'reset': 'Matcher*'}}
STUB_MEMBERS['Pattern'] = {'parent': None, 'methods': {'compile': 'Pattern*', 'matcher': 'Matcher*',
    'matches': 'bool', 'pattern': 'JavaString', 'split': 'JArray<JavaString>'}}
# java.io.FileFilter (Beta ChunkFolderPattern extends it) -- like FilenameFilter.
_GENERATED['FileFilter.h'] = _class_stub('FileFilter', fwds=['File'],
    ret_methods=[('accept', ('bool', 'false'))])
# java.io.ByteArrayOutputStream / InputStreamReader (Beta texture-pack + NBT IO).
# REAL ByteArrayOutputStream (growable byte buffer; buf/count exposed for RegionFileChunkBuffer) -- was a no-op
# stub, so the chunk-save buffer swallowed every byte.
_GENERATED['ByteArrayOutputStream.h'] = _load_canonical('ByteArrayOutputStream.h')
# java.io.Reader base (StringReader/InputStreamReader/BufferedReader all extend it; a parser param typed
# `Reader*` must accept a StringReader*).
_GENERATED['Reader.h'] = _class_stub('Reader', void_methods=['close'], ret_methods=[('read', ('int', '-1'))])
# Reader chain is load-bearing: BufferedReader over InputStreamReader over a FileInputStream is how the game
# reads splashes.txt / text config. No-op stubs -> every readLine() loop gets no data. Emit REAL adapters.
_GENERATED['InputStreamReader.h'] = _load_canonical('InputStreamReader.h')
STUB_MEMBERS['InputStreamReader'] = {'parent': 'Reader', 'methods': {}}
# java.net.HttpURLConnection (Beta ThreadDownloadImage / ThreadCheckHasPaid). Dead on the 360.
_GENERATED['HttpURLConnection.h'] = _class_stub('HttpURLConnection', bases=['URLConnection'],
    includes=['URLConnection'], fwds=['InputStream'],
    void_methods=['setDoInput', 'setDoOutput', 'connect', 'disconnect', 'setRequestMethod', 'setRequestProperty'],
    ret_methods=[('getResponseCode', ('int', '-1')), ('getInputStream', ('InputStream*', '0'))])
# java.util.zip.ZipEntry.
_GENERATED['ZipEntry.h'] = _class_stub('ZipEntry',
    ret_methods=[('getName', ('std::string', 'std::string()')), ('getSize', ('long long', '0')),
                 ('isDirectory', ('bool', 'false'))])
# org.w3c.dom / javax.xml.parsers (Beta ThreadDownloadResources texture-pack XML). Dead on the 360;
# newInstance/parse are flagged J2X-REVIEW[reflection-unsupported] at the call sites.
_GENERATED['NodeList.h'] = _class_stub('NodeList', fwds=['Node'],
    ret_methods=[('getLength', ('int', '0')), ('item', ('Node*', '0'))])
_GENERATED['Node.h'] = _class_stub('Node', fwds=['NodeList'],
    ret_methods=[('getNodeType', ('int', '0')), ('getNodeName', ('std::string', 'std::string()')),
                 ('getNodeValue', ('std::string', 'std::string()')), ('getChildNodes', ('NodeList*', '0'))])
_GENERATED['Element.h'] = _class_stub('Element', bases=['Node'], includes=['Node'], fwds=['NodeList'],
    ret_methods=[('getAttribute', ('std::string', 'std::string()')), ('getElementsByTagName', ('NodeList*', '0'))])
_GENERATED['Document.h'] = _class_stub('Document', fwds=['NodeList', 'Element'],
    ret_methods=[('getElementsByTagName', ('NodeList*', '0')), ('getDocumentElement', ('Element*', '0'))])
_GENERATED['DocumentBuilder.h'] = _class_stub('DocumentBuilder', fwds=['Document'],
    ret_methods=[('parse', ('Document*', '0'))])
_GENERATED['DocumentBuilderFactory.h'] = _class_stub('DocumentBuilderFactory', fwds=['DocumentBuilder'],
    ret_methods=[('newDocumentBuilder', ('DocumentBuilder*', '0'))],
    extra=['static DocumentBuilderFactory* newInstance(...) { return new DocumentBuilderFactory(); }'])
# java.util.Date / java.text.SimpleDateFormat (Beta ScreenShotHelper timestamps).
_GENERATED['Date.h'] = _class_stub('Date',
    ret_methods=[('getTime', ('long long', '0LL')), ('toString', ('std::string', 'std::string()'))])
_GENERATED['SimpleDateFormat.h'] = _class_stub('SimpleDateFormat', bases=['DateFormat'], includes=['DateFormat'],
    ret_methods=[('format', ('std::string', 'std::string()'))])
# java.util.zip gzip streams (Beta SaveConverterMcRegion / NBT). Real gzip lives in the hand-written
# Inflate/CompressedStreamTools platform code; these keep the type references compiling.
# REAL gzip streams via puff.c (Inflate.cpp) -- were no-op stubs so level.dat/player.dat never (de)compressed.
_GENERATED['GZIPInputStream.h'] = _load_canonical('GZIPInputStream.h')
_GENERATED['GZIPOutputStream.h'] = _load_canonical('GZIPOutputStream.h')
# java.lang.Runtime (Beta memory queries) + java.nio.ByteOrder.
# java.lang.Runtime -- memory reporters backed by the REAL 360 kernel query, NOT a stub returning 0 and NOT a
# diagnostic operator-new override. [360] equivalent: GlobalMemoryStatus (xboxkrnl.exe). The F3 debug overlay
# reads these, so it now shows the console's actual physical used/free.
_GENERATED['Runtime.h'] = (
    '#ifndef RUNTIME_H\n#define RUNTIME_H\n'
    '// java.lang.Runtime -- memory reporters backed by the REAL 360 kernel query (not a JVM heap, not a stub).\n'
    '// [360] equivalent: GlobalMemoryStatus (xboxkrnl.exe) fills MEMORYSTATUS.dwTotalPhys / dwAvailPhys over the\n'
    '// console\'s unified 512 MB physical pool. (Lower level: MmQueryStatistics(&MM_STATISTICS) for per-pool page\n'
    '// counts.) The F3 debug overlay reads these -> it shows the actual physical used/free.\n'
    '#include <string>\n#include "stdafx.h"\n#include "JavaString.h"\n\n'
    'class Runtime {\n'
    'public:\n'
    '\tRuntime(...) {}\n'
    '\tvirtual ~Runtime() {}\n'
    '\tvirtual void gc(...) {}                 // [360] no equivalent -- no GC on the console (explicit frees); no-op\n'
    '\tvirtual void runFinalization(...) {}    // [360] no equivalent -- no finalizers; no-op\n'
    '\tvirtual long long freeMemory(...)  { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwAvailPhys; }\n'
    '\tvirtual long long totalMemory(...) { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwTotalPhys; }\n'
    '\tvirtual long long maxMemory(...)   { MEMORYSTATUS m; GlobalMemoryStatus(&m); return (long long)m.dwTotalPhys; }\n'
    '\tstatic Runtime* getRuntime() { static Runtime r; return &r; }\n'
    '};\n#endif // RUNTIME_H\n')
_GENERATED['ByteOrder.h'] = _ns_stub('ByteOrder', consts=[('BIG_ENDIAN', '1'), ('LITTLE_ENDIAN', '0')],
                                     ret_fns=[('nativeOrder', ('int', '1'))])
# java.io.RandomAccessFile -- REAL stdio-backed implementation (Beta RegionFile chunk store).
# FIXED BUG raf-stub-worldgen-hang: the old symbol-only stub returned length()==0, so RegionFile's
# ctor built a ZERO-length sectorFree list and then JList::set(0,..) wrote out of bounds -> memory
# corruption -> world-gen froze the instant a chunk was loaded. Back it with real FILE* IO (Java
# big-endian on the wire) and self-create the parent "region" dir (File::mkdirs is a no-op on this
# port, so fopen would otherwise fail on the missing folder -> length()==0 -> the same crash).
_GENERATED['RandomAccessFile.h'] = r'''#ifndef RANDOMACCESSFILE_H
#define RANDOMACCESSFILE_H

// ============================ PLATFORM REFERENCE ============================
// Real java.io.RandomAccessFile for the Xbox 360 j2ctool port -- REPLACES the old symbol-only
// stub. The stub's length() returned 0, which made RegionFile's ctor build a ZERO-length
// sectorFree list and then call JList::set(0,..) on it -> out-of-bounds vector write ->
// world-gen hang the instant a chunk was loaded (RegionFileCache::getChunkInputStream).
//
// FIXED BUG raf-stub-worldgen-hang: back RandomAccessFile with real stdio (FILE*) so
// length()/seek()/readInt()/write() behave like Java's. Big-endian on the wire (Java
// DataOutput order; the 360 is big-endian too, but bytes are assembled explicitly so the
// format is host-independent). The ctor also creates the file's parent directories --
// File::mkdirs() is a no-op on this port, so the "region" folder would never exist and the
// underlying fopen("...\\region\\r.0.0.mcr","rw") would fail (-> length()==0 -> same crash).
// ==========================================================================
// NOTE: relies on <cstdio> and <xtl.h> (CreateDirectoryA), both pulled in by stdafx.h (PCH).

#include <cstdio>
#include <string>
#include "JavaString.h"
#include "JArray.h"
#include "File.h"

class RandomAccessFile {
	FILE* _fp;

	static void _ensureParentDirs(const std::string& p) {
		// Create every ancestor directory of the file path (backslash/forward-slash separated).
		// Skips a leading device/drive prefix like "game:\\" or "D:\\". CreateDirectoryA is a
		// harmless no-op when the directory already exists.
		std::string::size_type start = 0;
		std::string::size_type colon = p.find(':');
		if (colon != std::string::npos) start = colon + 1;
		for (std::string::size_type i = start; i < p.size(); ++i) {
			if ((p[i] == '\\' || p[i] == '/') && i > start) {
				std::string dir = p.substr(0, i);
				CreateDirectoryA(dir.c_str(), NULL);
			}
		}
	}

	void _open(const std::string& path) {
		_ensureParentDirs(path);
		// Java "rw": open read+write, DO NOT truncate, create if absent. "rb+" gives no-truncate
		// r/w but fails when the file is missing, so fall back to "wb+" (creates a fresh empty file).
		_fp = fopen(path.c_str(), "rb+");
		if (_fp == NULL) _fp = fopen(path.c_str(), "wb+");
	}

public:
	RandomAccessFile(File* f, const std::string& /*mode*/) : _fp(NULL) {
		if (f != NULL) _open(f->path);
	}
	RandomAccessFile(const std::string& path, const std::string& /*mode*/) : _fp(NULL) {
		_open(path);
	}
	virtual ~RandomAccessFile() { if (_fp) { fclose(_fp); _fp = NULL; } }

	// ---- positioning ----
	virtual void seek(long long pos) {
		if (_fp) { fflush(_fp); fseek(_fp, (long)pos, SEEK_SET); }
	}
	virtual long long getFilePointer() {
		return _fp ? (long long)ftell(_fp) : 0LL;
	}
	virtual long long length() {
		if (!_fp) return 0LL;
		fflush(_fp);
		long cur = ftell(_fp);
		fseek(_fp, 0, SEEK_END);
		long end = ftell(_fp);
		fseek(_fp, cur, SEEK_SET);
		return (long long)end;
	}
	virtual void setLength(long long /*n*/) { /* not needed by RegionFile; kept for API parity */ }
	virtual int skipBytes(int n) {
		if (!_fp || n <= 0) return 0;
		fseek(_fp, n, SEEK_CUR);
		return n;
	}

	// ---- writes (big-endian, matching java.io.DataOutput) ----
	virtual void write(int b) { if (_fp) fputc(b & 0xFF, _fp); }
	virtual void writeByte(int b) { if (_fp) fputc(b & 0xFF, _fp); }
	virtual void writeShort(int v) {
		if (!_fp) return;
		fputc((v >> 8) & 0xFF, _fp); fputc(v & 0xFF, _fp);
	}
	virtual void writeInt(int v) {
		if (!_fp) return;
		fputc((v >> 24) & 0xFF, _fp); fputc((v >> 16) & 0xFF, _fp);
		fputc((v >> 8) & 0xFF, _fp);  fputc(v & 0xFF, _fp);
	}
	virtual void writeLong(long long v) {
		if (!_fp) return;
		for (int s = 56; s >= 0; s -= 8) fputc((int)((v >> s) & 0xFF), _fp);
	}
	virtual void write(JArray<unsigned char> b) {
		if (_fp && b.length > 0) fwrite(b.data(), 1, (size_t)b.length, _fp);
	}
	virtual void write(JArray<unsigned char> b, int off, int len) {
		if (_fp && len > 0) fwrite(b.data() + off, 1, (size_t)len, _fp);
	}
	virtual void writeFully(JArray<unsigned char> b) { write(b); }

	// ---- reads (big-endian) ----
	virtual int read(JArray<unsigned char> b) {
		if (!_fp || b.length <= 0) return -1;
		size_t n = fread(b.data(), 1, (size_t)b.length, _fp);
		return n == 0 ? -1 : (int)n;
	}
	virtual int read(JArray<unsigned char> b, int off, int len) {
		if (!_fp || len <= 0) return -1;
		size_t n = fread(b.data() + off, 1, (size_t)len, _fp);
		return n == 0 ? -1 : (int)n;
	}
	virtual void readFully(JArray<unsigned char> b) {
		if (_fp && b.length > 0) fread(b.data(), 1, (size_t)b.length, _fp);
	}
	virtual int readByte() {                     // signed byte (-128..127); -1 at EOF
		if (!_fp) return -1;
		int c = fgetc(_fp);
		return c == EOF ? -1 : (int)(signed char)c;
	}
	virtual int readUnsignedByte() {
		if (!_fp) return 0;
		int c = fgetc(_fp);
		return c == EOF ? 0 : (c & 0xFF);
	}
	virtual int readShort() {
		int a = readUnsignedByte(), b = readUnsignedByte();
		return (short)((a << 8) | b);
	}
	virtual int readInt() {
		int a = readUnsignedByte(), b = readUnsignedByte(), c = readUnsignedByte(), d = readUnsignedByte();
		return (a << 24) | (b << 16) | (c << 8) | d;
	}
	virtual long long readLong() {
		long long v = 0;
		for (int i = 0; i < 8; ++i) v = (v << 8) | (long long)readUnsignedByte();
		return v;
	}

	virtual void close() { if (_fp) { fclose(_fp); _fp = NULL; } }
};
#endif // RANDOMACCESSFILE_H
'''
# java.io.Writer / StringWriter / PushbackReader. Writer.append returns the Writer (java.lang.Appendable)
# so `w.append(a).append(b)` chains (argo's compact formatter).
_GENERATED['Writer.h'] = _class_stub('Writer', void_methods=['write', 'flush', 'close'],
    extra=['Writer* append(...) { return this; }'])
_GENERATED['StringWriter.h'] = _class_stub('StringWriter', bases=['Writer'], includes=['Writer'],
    ret_methods=[('toString', ('std::string', 'std::string()')), ('getBuffer', ('void*', '0'))])
_GENERATED['PushbackReader.h'] = _class_stub('PushbackReader', void_methods=['unread', 'close'],
    ret_methods=[('read', ('int', '-1'))])
# java.util.Stack / Properties / Map.Entry / NumberFormat.
_GENERATED['Stack.h'] = (
    '#ifndef STACK_H\n#define STACK_H\n#include <vector>\n'
    'class Stack {\n'                                     # java.util.Stack<T> -> void*-element stub (a pointer class)
    '\tstd::vector<void*> _v;\n'
    'public:\n'
    '\tvoid* push(void* v) { _v.push_back(v); return v; }\n'
    '\tvoid* pop() { if (_v.empty()) return 0; void* v = _v.back(); _v.pop_back(); return v; }\n'
    '\tvoid* peek() const { return _v.empty() ? 0 : _v.back(); }\n'
    '\tbool isEmpty() const { return _v.empty(); }\n'
    '\tint size() const { return (int)_v.size(); }\n'
    '\tvoid* elementAt(int i) const { return (i >= 0 && i < (int)_v.size()) ? _v[i] : 0; }\n'
    '};\n#endif\n')
# Properties is NOT a no-op stub: StringTranslate loads the .lang file INTO it and reads every UI string
# back out. A stub load()/empty getProperty() -> the whole i18n table is empty -> blank menu buttons + all
# translated text blank. Emit the REAL key/value canonical (load() parses key=value lines).
_GENERATED['Properties.h'] = _load_canonical('Properties.h')
STUB_MEMBERS['Properties'] = {'parent': None, 'methods': {'getProperty': 'std::string'}}  # value return -> chained '.'
# java.util.Map.Entry -- a REAL pair box (JMap.entrySet() heap-allocates one per mapping). getKey()/
# getValue() return the stored void* (the boxed key/value pointer), cast at the call site exactly as
# Java casts Map.Entry.getKey()/getValue() from Object. The no-arg ctor keeps the old stub call sites
# compiling; setValue() mutates the local copy only (entrySet() is a snapshot here, like Iterator).
_GENERATED['Entry.h'] = (
    '#ifndef ENTRY_H\n#define ENTRY_H\n'
    'class Entry {\n'
    '\tvoid* _k; void* _v;\n'
    'public:\n'
    '\tEntry() : _k(0), _v(0) {}\n'
    '\tEntry(void* k, void* v) : _k(k), _v(v) {}\n'
    '\tvoid* getKey() const { return _k; }\n'
    '\tvoid* getValue() const { return _v; }\n'
    '\tvoid* setValue(void* v) { void* o = _v; _v = v; return o; }\n'
    '};\n#endif // ENTRY_H\n')
# Number formatting is load-bearing for the Statistics screen (stub format()->"" = blank values). Real.
_GENERATED['NumberFormat.h'] = _load_canonical('NumberFormat.h')
STUB_MEMBERS['NumberFormat'] = {'parent': None, 'methods': {'format': 'std::string',
    'getInstance': 'NumberFormat*', 'getIntegerInstance': 'NumberFormat*', 'getNumberInstance': 'NumberFormat*'}}
# java.security.MessageDigest (Beta MD5String, the login hash).
_GENERATED['MessageDigest.h'] = _load_canonical('MessageDigest.h')   # real MD5
STUB_MEMBERS['MessageDigest'] = {'parent': None, 'methods': {'getInstance': 'MessageDigest*',
    'digest': 'JArray<unsigned char >'}}
# AWT / misc leaf stubs (dead on the 360).
_GENERATED['RenderingHints.h'] = _class_stub('RenderingHints', void_methods=['put'],
    extra=['static const int s_KEY_INTERPOLATION = 0;',   # opaque hint keys/values (dead: no AWT on the 360)
           'static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;',
           'static const int s_KEY_ANTIALIASING = 0;', 'static const int s_VALUE_ANTIALIAS_OFF = 0;'])
_GENERATED['CodecWav.h'] = _class_stub('CodecWav')   # paulscode WAV codec (dead type stub on the console)
_GENERATED['DataFlavor.h'] = _class_stub('DataFlavor')
_GENERATED['Reference.h'] = _class_stub('Reference', ret_methods=[('get', ('void*', '0'))])
_GENERATED['Logger.h'] = _class_stub('Logger', void_methods=['log', 'info', 'warning', 'severe', 'fine'],
    extra=['static Logger* getLogger(...) { return new Logger(); }'])
# java.util.Calendar / GregorianCalendar (Beta seasonal-chest / timestamp checks).
_GENERATED['Calendar.h'] = _class_stub('Calendar', void_methods=['set', 'setTime', 'add'],
    ret_methods=[('get', ('int', '0')), ('getTime', ('void*', '0')), ('getTimeInMillis', ('long long', '0LL'))],
    extra=['static Calendar* getInstance() { return new Calendar(); }',
           'static const int JANUARY = 0, FEBRUARY = 1, DECEMBER = 11, DATE = 5, MONTH = 2, YEAR = 1;'])
_GENERATED['GregorianCalendar.h'] = _class_stub('GregorianCalendar', bases=['Calendar'], includes=['Calendar'])
# AWT / paulscode leaf stubs (all dead on the 360 -- desktop clipboard / config).
_GENERATED['Clipboard.h'] = _class_stub('Clipboard', fwds=['Transferable'],
    ret_methods=[('getContents', ('Transferable*', '0'))])
_GENERATED['Toolkit.h'] = _class_stub('Toolkit', includes=['Clipboard'],
    extra=['static Toolkit* getDefaultToolkit() { return new Toolkit(); }',
           'Clipboard getSystemClipboard(...) { return Clipboard(); }'])   # value receiver -> .getContents()
_GENERATED['TextArea.h'] = _class_stub('TextArea', ret_methods=[('getText', ('std::string', 'std::string()'))])
_GENERATED['Transferable.h'] = _class_stub('Transferable')
_GENERATED['SoundSystemConfig.h'] = _ns_stub('SoundSystemConfig',
    void_fns=['setCodec', 'setNumberStreamingChannels', 'addLibrary', 'removeLibrary', 'setSoundFilesPackage'])
# java.util.Collection (interface base) / Locale / java.awt.Rectangle.
_GENERATED['Collection.h'] = _class_stub('Collection',
    void_methods=['add', 'clear'], ret_methods=[('size', ('int', '0')), ('isEmpty', ('bool', 'true')),
                                                 ('iterator', ('void*', '0')), ('contains', ('bool', 'false'))])
_GENERATED['Locale.h'] = _class_stub('Locale',
    extra=['static const int US = 0, ENGLISH = 0, ROOT = 0;'])   # Locale.US et al -> dead int consts (locale is a no-op on the 360)
# java.nio.charset.Charset -- Charset.forName("UTF-8") is a static factory; returns a stub instance.
_GENERATED['Charset.h'] = _class_stub('Charset',
    extra=['static Charset* forName(...) { return new Charset(); }'])
_GENERATED['Rectangle.h'] = _class_stub('Rectangle', extra=['int x, y, width, height;'])
# Transferable / TextArea leaf methods (dead AWT; keep them compiling). RenderingHints/DataFlavor
# static-constant accesses (KEY_INTERPOLATION / stringFlavor) remain a few errors -- dead AWT, low pri.
_GENERATED['RenderingHints.h'] = _class_stub('RenderingHints', void_methods=['put'],
    extra=['static const int s_KEY_INTERPOLATION = 0;',   # opaque hint keys/values (dead: no AWT on the 360)
           'static const int s_VALUE_INTERPOLATION_NEAREST_NEIGHBOR = 0;',
           'static const int s_KEY_ANTIALIASING = 0;', 'static const int s_VALUE_ANTIALIAS_OFF = 0;'])
_GENERATED['CodecWav.h'] = _class_stub('CodecWav')   # paulscode WAV codec (dead type stub on the console)
_GENERATED['DataFlavor.h'] = _class_stub('DataFlavor',
    extra=['static const int s_stringFlavor = 0;'])   # DataFlavor.stringFlavor -> dead const (passed to varargs stubs)
_GENERATED['Transferable.h'] = _class_stub('Transferable',
    ret_methods=[('isDataFlavorSupported', ('bool', 'false')), ('getTransferData', ('JObject', 'JObject()'))])
_GENERATED['TextArea.h'] = _class_stub('TextArea', void_methods=['setFont', 'setText'],
    ret_methods=[('getText', ('std::string', 'std::string()'))])
# java.io concrete streams (DataInputStream/DataOutputStream are the heavy hitters on alpha)
# J2X-BUGFIX 2026-08-13 inputstream-odr: `class InputStream` was defined DIFFERENTLY here (variadic stub)
# and in FileIO.reference.h (typed) under the SAME guard INPUTSTREAM_H -> per-TU ODR: FileInputStream's
# read()/available() overrides bound in some TUs and not others, so virtual dispatch hit the base stub
# (return 0) and EVERY getResourceAsStream load returned empty (font/lang/colormaps). ONE canonical typed
# InputStream now; FileIO.reference.h includes THIS instead of redefining. Byte streams (FileInputStream)
# override the typed virtuals; the variadic read(...) absorbs JArray/other converted read forms.
_GENERATED['InputStream.h'] = (
    '#ifndef INPUTSTREAM_H\n#define INPUTSTREAM_H\n'
    '// java.io.InputStream -- ONE canonical definition (see j2ctool runtime note: inputstream-odr).\n'
    '#include <string>\n#include "JavaString.h"\n#include "JArray.h"\n'
    'class InputStream {\n'
    'public:\n'
    '\tInputStream(...) {}\n'
    '\tvirtual ~InputStream() {}\n'
    '\tvirtual int read() { return -1; }\n'
    '\tvirtual int read(unsigned char* b, int off, int len) {\n'
    '\t\tint n = 0;\n'
    '\t\tfor (; n < len; ++n) { int c = read(); if (c < 0) break; b[off + n] = (unsigned char)c; }\n'
    '\t\treturn (n > 0) ? n : (len == 0 ? 0 : -1);\n'
    '\t}\n'
    '\tvirtual int read(unsigned char* b, int len) { return read(b, 0, len); }\n'
    '\t// JArray forms (Java InputStream.read(byte[])) -> delegate to the typed virtual so byte-buffer\n'
    '\t// streams (FileInputStream) still fill them via their override:\n'
    '\tvirtual int read(JArray<unsigned char>& b) { return read(b.data(), 0, b.length); }\n'
    '\tvirtual int read(JArray<unsigned char>& b, int off, int len) { return read(b.data(), off, len); }\n'
    '\tvirtual int available() { return 0; }\n'
    '\tvirtual long long skip(long long) { return 0; }\n'
    '\tvirtual void close() {}\n'
    '\tvirtual void reset() {}\n'
    '\tvirtual void mark(int) {}\n'
    '};\n'
    '#endif // INPUTSTREAM_H\n')
# REAL canonical byte streams -- were no-op stubs (which swallowed every packet / vtable-ODR-folded).
_GENERATED['OutputStream.h'] = _load_canonical('OutputStream.h')
_GENERATED['DataInputStream.h'] = _load_canonical('DataInputStream.h')
_GENERATED['DataOutputStream.h'] = _load_canonical('DataOutputStream.h')
_GENERATED['BufferedOutputStream.h'] = _load_canonical('BufferedOutputStream.h')
_GENERATED['BufferedInputStream.h'] = _class_stub('BufferedInputStream', bases=['InputStream'], includes=['InputStream'])
# J2X-BUGFIX 2026-08-13 fileinputstream-odr: FileInputStream was ALSO defined (real) in FileIO.reference.h
# while this file held a symbol-only stub -> two `class FileInputStream` definitions -> the linker folded
# them to ONE vtable and picked the stub's (no overrides), so read()/available() dispatched to the
# InputStream base (0) and every getResourceAsStream load returned empty. ONE canonical raw-buffer
# definition now; FileIO.reference.h includes it (and defines _load + the File* ctor out-of-line).
_GENERATED['FileInputStream.h'] = (
    '#ifndef FILEINPUTSTREAM_H\n#define FILEINPUTSTREAM_H\n'
    '#include <string>\n#include "InputStream.h"\n'
    'class File;\n'
    'class FileInputStream : public InputStream {\n'
    '\tunsigned char* _data;\n'
    '\tint _size;\n'
    '\tint _pos;\n'
    '\tvoid _load(const char* p);\n'
    'public:\n'
    '\tFileInputStream(const std::string& p) : _data(0), _size(0), _pos(0) { _load(p.c_str()); }\n'
    '\tFileInputStream(File* f);\n'
    '\tvirtual ~FileInputStream() { if (_data) delete[] _data; }\n'
    '\tint read() { return (_pos < _size) ? (int)_data[_pos++] : -1; }\n'
    '\tint read(unsigned char* b, int off, int len) { int n = 0; while (n < len && _pos < _size) b[off + n++] = _data[_pos++]; return (n > 0) ? n : (len == 0 ? 0 : -1); }\n'
    '\tint read(unsigned char* b, int len) { return read(b, 0, len); }\n'
    '\tint available() { return _size - _pos; }\n'
    '\tvoid close() {}\n'
    '};\n'
    '#endif // FILEINPUTSTREAM_H\n')
# REAL FileOutputStream (CreateFile+WriteFile on close) -- was a no-op stub, so no disk write ever happened.
_GENERATED['FileOutputStream.h'] = _load_canonical('FileOutputStream.h')
_GENERATED['ZipFile.h'] = _class_stub('ZipFile', void_methods=['close'],
    ret_methods=[('getEntry', ('ZipEntry*', '0')), ('getInputStream', ('InputStream*', '0')),
                 ('size', ('int', '0'))], fwds=['InputStream', 'ZipEntry'])
_GENERATED['SocketAddress.h'] = _class_stub('SocketAddress')
_GENERATED['Socket.h'] = _class_stub('Socket',
    void_methods=['close', 'setSoTimeout', 'setTrafficClass', 'setTcpNoDelay', 'setKeepAlive', 'connect'],
    ret_methods=[('getInputStream', ('InputStream*', '0')), ('getOutputStream', ('OutputStream*', '0')),
                 ('getRemoteSocketAddress', ('SocketAddress*', '0')), ('getInetAddress', ('InetAddress*', '0')),
                 ('getPort', ('int', '0')), ('isConnected', ('bool', 'false')), ('isClosed', ('bool', 'false'))],
    fwds=['InputStream', 'OutputStream', 'SocketAddress', 'InetAddress'])
_GENERATED['DateFormat.h'] = _class_stub('DateFormat',
    ret_methods=[('format', ('std::string', 'std::string()'))])
_GENERATED['Dimension.h'] = _class_stub('Dimension',
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0'))])
# java.awt event listeners/adapters (implemented/extended by the desktop applet+window code)
for _li in ('KeyListener', 'MouseListener', 'MouseMotionListener', 'MouseWheelListener',
            'WindowListener', 'ComponentListener', 'FocusListener'):
    _GENERATED[_li + '.h'] = _class_stub(_li,
        void_methods=['keyPressed', 'keyReleased', 'keyTyped', 'mouseClicked', 'mousePressed',
                      'mouseReleased', 'mouseEntered', 'mouseExited', 'mouseMoved', 'mouseDragged',
                      'mouseWheelMoved', 'windowClosing', 'windowOpened', 'windowClosed',
                      'windowActivated', 'windowDeactivated', 'windowIconified', 'windowDeiconified',
                      'componentResized', 'componentMoved', 'componentShown', 'componentHidden',
                      'focusGained', 'focusLost'])
_GENERATED['WindowAdapter.h'] = _class_stub('WindowAdapter',
    void_methods=['windowClosing', 'windowOpened', 'windowClosed', 'windowActivated',
                  'windowDeactivated', 'windowIconified', 'windowDeiconified'])
# java.awt event objects
for _ev in ('KeyEvent', 'MouseEvent', 'WindowEvent', 'ComponentEvent', 'FocusEvent', 'InputEvent'):
    _GENERATED[_ev + '.h'] = _class_stub(_ev, void_methods=['consume'],
        ret_methods=[('getKeyCode', ('int', '0')), ('getKeyChar', ('char', '0')),
                     ('getX', ('int', '0')), ('getY', ('int', '0')), ('getButton', ('int', '0')),
                     ('getClickCount', ('int', '0')), ('getWheelRotation', ('int', '0')),
                     ('getWindow', ('void*', '0')), ('getComponent', ('void*', '0')),
                     ('isShiftDown', ('bool', 'false')), ('isControlDown', ('bool', 'false')),
                     ('isAltDown', ('bool', 'false'))])
_GENERATED['Panel.h'] = _class_stub('Panel', bases=['Component'], includes=['Component'], fwds=['Graphics'],
    void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')), ('getGraphics', ('Graphics*', '0'))])
_GENERATED['Comparable.h'] = _class_stub('Comparable', ret_methods=[('compareTo', ('int', '0'))])
# java.awt layout managers (new BorderLayout() etc. passed to setLayout -- no-op stubs)
for _lm in ('BorderLayout', 'GridLayout', 'FlowLayout', 'CardLayout', 'GridBagLayout'):
    _GENERATED[_lm + '.h'] = _class_stub(_lm)
# java.awt.Frame / Window (MinecraftApplet's desktop host). Extends Component so a Frame* converts
# to the Component* that AWT APIs (and MinecraftImpl) accept.
_GENERATED['Frame.h'] = _class_stub('Frame', bases=['Component'], includes=['Component'], fwds=['Graphics'],
    void_methods=list(_AWT_VOID) + [
    'setTitle', 'setResizable', 'dispose', 'show', 'hide', 'toFront', 'setIconImage', 'setMenuBar',
    'setLocationRelativeTo', 'setLocation', 'setUndecorated', 'setState', 'setExtendedState'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('getInsets', ('void*', '0')),
                 ('getTitle', ('std::string', 'std::string()'))])
# java.awt.Color. Constants are static members named exactly as Java (Color is in RUNTIME_ENUM_LIKE
# so `Color.BLACK` -> `Color::BLACK`, no s_ prefix). They are `static const int` (ARGB) -- a real
# `static Color X;` member would need an out-of-line definition (none, since stubs are header-only)
# and fail to LINK; an integral constant with an in-class initializer needs no definition, and the
# variadic Color ctor / setColor accept the int.
_COLOR_RGB = [('BLACK', 0xFF000000), ('WHITE', 0xFFFFFFFF), ('GRAY', 0xFF808080),
              ('LIGHT_GRAY', 0xFFC0C0C0), ('DARK_GRAY', 0xFF404040), ('RED', 0xFFFF0000),
              ('GREEN', 0xFF00FF00), ('BLUE', 0xFF0000FF), ('YELLOW', 0xFFFFFF00),
              ('CYAN', 0xFF00FFFF), ('MAGENTA', 0xFFFF00FF), ('ORANGE', 0xFFFFC800),
              ('PINK', 0xFFFFAFAF),
              ('black', 0xFF000000), ('white', 0xFFFFFFFF), ('gray', 0xFF808080),
              ('lightGray', 0xFFC0C0C0), ('darkGray', 0xFF404040), ('red', 0xFFFF0000),
              ('green', 0xFF00FF00), ('blue', 0xFF0000FF), ('yellow', 0xFFFFFF00),
              ('cyan', 0xFF00FFFF), ('magenta', 0xFFFF00FF), ('orange', 0xFFFFC800),
              ('pink', 0xFFFFAFAF)]
# java.awt.Color -- NOT a symbol-only stub: getHSBColor/HSBtoRGB/getRGB are REAL. The Alpha sky color
# is MobSpawnerBase::getSkyColorByTemp() -> Color::getHSBColor(h,s,b).getRGB(); a stub returning
# Color()/0 made the in-world SKY render BLACK. Standard java.awt HSB->RGB algorithm below.
_GENERATED['Color.h'] = (
    "#ifndef COLOR_H\n#define COLOR_H\n#include <string>\n#include <math.h>\n"
    "class Color {\n"
    "    int _rgb;\n"
    "public:\n"
    "    Color(int rgb) : _rgb(rgb) {}\n"
    "    Color(int rgba, bool) : _rgb(rgba) {}\n"
    "    Color(...) : _rgb(0) {}\n"
    "    virtual ~Color() {}\n"
    "    virtual int getRGB()   { return _rgb; }\n"
    "    virtual int getRed()   { return (_rgb >> 16) & 255; }\n"
    "    virtual int getGreen() { return (_rgb >> 8) & 255; }\n"
    "    virtual int getBlue()  { return _rgb & 255; }\n"
    "    virtual int getAlpha() { return (_rgb >> 24) & 255; }\n"
    "    static int HSBtoRGB(float hue, float saturation, float brightness) {\n"
    "        int r=0,g=0,b=0;\n"
    "        if (saturation == 0.0f) { r=g=b=(int)(brightness*255.0f+0.5f); }\n"
    "        else {\n"
    "            float h=(hue-(float)floorf(hue))*6.0f; float f=h-(float)floorf(h);\n"
    "            float p=brightness*(1.0f-saturation); float q=brightness*(1.0f-saturation*f);\n"
    "            float t=brightness*(1.0f-(saturation*(1.0f-f)));\n"
    "            switch((int)h){\n"
    "            case 0: r=(int)(brightness*255.0f+0.5f); g=(int)(t*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;\n"
    "            case 1: r=(int)(q*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(p*255.0f+0.5f); break;\n"
    "            case 2: r=(int)(p*255.0f+0.5f); g=(int)(brightness*255.0f+0.5f); b=(int)(t*255.0f+0.5f); break;\n"
    "            case 3: r=(int)(p*255.0f+0.5f); g=(int)(q*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;\n"
    "            case 4: r=(int)(t*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(brightness*255.0f+0.5f); break;\n"
    "            case 5: r=(int)(brightness*255.0f+0.5f); g=(int)(p*255.0f+0.5f); b=(int)(q*255.0f+0.5f); break;\n"
    "            }\n"
    "        }\n"
    "        return (int)(0xFF000000u | ((unsigned)r<<16) | ((unsigned)g<<8) | (unsigned)b);\n"
    "    }\n"
    "    static Color getHSBColor(float h, float s, float b) { return Color(HSBtoRGB(h,s,b)); }\n"
    + ''.join("    static const int %s = (int)0x%08X;\n" % (n, v) for n, v in _COLOR_RGB)
    + "};\n#endif // COLOR_H\n")
# org.lwjgl.input.Controllers (static holder)
_GENERATED['Controllers.h'] = _ns_stub('Controllers',
    void_fns=['create', 'destroy', 'poll', 'clearEvents'],
    ret_fns=[('isCreated', ('bool', 'false')), ('getControllerCount', ('int', '0')),
             ('getController', ('void*', '0')), ('next', ('bool', 'false')),
             ('getEventSource', ('void*', '0')), ('getEventControlIndex', ('int', '0')),
             ('getEventButtonState', ('bool', 'false'))])
# org.lwjgl.Sys + org.lwjgl.opengl.GLContext (static holders)
_GENERATED['Sys.h'] = _ns_stub('Sys', void_fns=['openURL', 'alert'],
    ret_fns=[('getVersion', ('std::string', 'std::string()')), ('getTime', ('long long', '0LL')),
             ('getTimerResolution', ('long long', '1000LL'))])
_GENERATED['GLContext.h'] = r'''#ifndef GLCONTEXT_H
#define GLCONTEXT_H
// org.lwjgl.opengl.GLContext + ContextCapabilities stub. Capability flags are public bools
// (the real values come from the Xbox 360 platform/D3D9 layer) so `getCapabilities().GL_ARB_x`
// compiles. Default false = feature absent.
struct ContextCapabilities {
	bool GL_ARB_occlusion_query, GL_ARB_vertex_buffer_object, GL_ARB_multitexture;
	bool GL_ARB_texture_env_combine, GL_EXT_blend_func_separate, GL_ARB_shader_objects;
	bool GL_NV_fog_distance, GL_EXT_texture_filter_anisotropic, GL_ARB_texture_non_power_of_two;
	bool OpenGL11, OpenGL12, OpenGL13, OpenGL14, OpenGL15, OpenGL20, OpenGL21;
	bool OpenGL30, OpenGL31, OpenGL32, OpenGL33, GL_ARB_compatibility;   // Indev logs these at boot
	ContextCapabilities() : GL_ARB_occlusion_query(false), GL_ARB_vertex_buffer_object(false),
		GL_ARB_multitexture(false), GL_ARB_texture_env_combine(false), GL_EXT_blend_func_separate(false),
		GL_ARB_shader_objects(false), GL_NV_fog_distance(false), GL_EXT_texture_filter_anisotropic(false),
		GL_ARB_texture_non_power_of_two(false), OpenGL11(false), OpenGL12(false), OpenGL13(false),
		OpenGL14(false), OpenGL15(false), OpenGL20(false), OpenGL21(false),
		OpenGL30(false), OpenGL31(false), OpenGL32(false), OpenGL33(false), GL_ARB_compatibility(false) {}
};
namespace GLContext {
	// J2X-BUGFIX glcontext-caps-ref: getCapabilities() returns a REFERENCE to the shared static, matching LWJGL
	// where the game does `GLContext.getCapabilities().GL_ARB_x` (member access via `.`). Returning a POINTER
	// made every faithful `.GL_ARB_x` site fail ("left of '.' must have class type ... use '->'").
	inline ContextCapabilities& getCapabilities() { static ContextCapabilities _c; return _c; }
}
#endif // GLCONTEXT_H
'''
_GENERATED['URLConnection.h'] = _class_stub('URLConnection', fwds=['InputStream', 'OutputStream'],
    void_methods=['connect', 'setDoInput', 'setDoOutput', 'setUseCaches', 'setRequestProperty',
                  'setConnectTimeout', 'setReadTimeout', 'disconnect'],
    ret_methods=[('getInputStream', ('InputStream*', '0')), ('getOutputStream', ('OutputStream*', '0')),
                 ('getContentLength', ('int', '0')), ('getResponseCode', ('int', '0')),
                 ('getHeaderField', ('std::string', 'std::string()'))])
_GENERATED['CodecJOrbis.h'] = _class_stub('CodecJOrbis', includes=['URL', 'URLConnection'],
    void_methods=['initialize', 'cleanup', 'setAudioFormat', 'reverseByteOrder'],
    ret_methods=[('read', ('void*', '0')), ('readAll', ('void*', '0')), ('getAudioFormat', ('void*', '0')),
                 ('initialized', ('bool', 'false')), ('endOfStream', ('bool', 'false'))],
    extra=['URL* url;', 'URLConnection urlConnection;'])   # paulscode ICodec protected fields
_GENERATED['SoundSystem.h'] = _class_stub('SoundSystem',
    void_methods=['cleanup', 'newSource', 'newStreamingSource', 'play', 'setListenerOrientation',
                  'setListenerPosition', 'setPitch', 'setVolume', 'stop', 'pause', 'removeSource',
                  'backgroundMusic', 'quickPlay', 'quickStream', 'setPosition', 'fadeOut', 'fadeOutIn'],
    ret_methods=[('playing', ('bool', 'false'))])
# small java.io / java.awt / java.net stubs (Applet/Canvas/Component/File/FileDialog/
# BufferedImage are enriched further below)
_GENERATED['BufferedReader.h'] = _load_canonical('BufferedReader.h')   # REAL line reader (see canonical)
STUB_MEMBERS['BufferedReader'] = {'parent': None, 'methods': {'readLine': 'std::string'}}
# character streams for options.txt / crash reports (GameSettings, PanelCrashReport): FileReader is
# wrapped by BufferedReader; PrintWriter wraps FileWriter. Real impls are the Xbox file layer
# (docs/platform/reference/FileIO.reference) -- BufferedReader.readLine etc. read nothing until then.
# options.txt load/save chain is load-bearing (stubs -> settings never load OR save). FileReader is a REAL
# Reader subclass (so it binds BufferedReader(Reader*)); FileWriter a REAL Writer (CreateFile+WriteFile on
# close); PrintWriter formats + forwards to the wrapped Writer.
_GENERATED['FileReader.h'] = _load_canonical('FileReader.h')
STUB_MEMBERS['FileReader'] = {'parent': 'Reader', 'methods': {}}
_GENERATED['FileWriter.h'] = _load_canonical('FileWriter.h')
STUB_MEMBERS['FileWriter'] = {'parent': 'Writer', 'methods': {}}
_GENERATED['PrintWriter.h'] = _load_canonical('PrintWriter.h')
_GENERATED['Graphics.h'] = _class_stub('Graphics',
    void_methods=['drawImage', 'dispose', 'setColor', 'fillRect', 'drawString'])
# J2X-BUGFIX 2026-08-13 null-graphics: BufferedImage.getGraphics() returned NULL, so RenderEngine's
# `missingTextureImage.getGraphics().setColor(...)` (in startGame) was a virtual call through a null
# vtable -> crash. Graphics is an all-no-op stub, so return a SHARED instance instead of NULL.
_GENERATED['Graphics.h'] = _GENERATED['Graphics.h'].replace(
    '};\n#endif',
    '};\ninline Graphics* j2xSharedGraphics() { static Graphics _g; return &_g; }\n#endif', 1)
_GENERATED['Cursor.h'] = _class_stub('Cursor')
_GENERATED['Class.h'] = r'''#ifndef CLASS_H
#define CLASS_H
// java.lang.Class -- backed by C++ RTTI (std::type_info). Identity, ==, getName and map-key
// ordering are REAL: `X.class` (Class(typeid(X))) and `obj.getClass()` (Class(typeid(*obj)))
// agree for the same runtime type. newInstance/getConstructor have no reflection equivalent and
// return 0 -- the class-keyed registries (EntityList/Packet/TileEntity) supply explicit factories.
#include <string>
#include <typeinfo>
#include <vector>
#if defined(_XBOX)
#include <xtl.h>        // classreg CRITICAL_SECTION (thread-safe RTTI under MP net threads)
#elif defined(_WIN32)
#include <windows.h>
#endif
#include "URL.h"                                          // getResource returns a path-carrying URL
class InputStream;
InputStream* J2X_getResourceAsStream(const char* name);   // FileIO.cpp: "/x.png" -> game:\Content\x.png

// Program-lifetime registry of every Class ever materialised (shared by j2xClassOf below and
// Class::getSuperclass). Lets getSuperclass turn a base NAME (from _j2xBases) back into a Class*.
class Class;
inline std::vector<Class*>& _j2xClassReg() { static std::vector<Class*> reg; return reg; }
// FIXED BUG classreg-multithread-race: j2xClassOf/getSuperclass touch this shared registry from the
// main thread (RTTI on every tick) AND the MP network reader thread (packet reflection). Serialize
// the lookup+append with one program-wide CS. Uncontended in single-thread SP. (j2xPrepopulateClassReg,
// emitted into j2x_rtti.cpp and called at boot before threads start, keeps it read-only in practice.)
inline CRITICAL_SECTION* _j2xClassRegCS() {
	static CRITICAL_SECTION cs;
	static volatile long init = 0;
	if (init != 2) {
		if (InterlockedCompareExchange(&init, 1, 0) == 0) { InitializeCriticalSection(&cs); init = 2; }
		else { while (init != 2) Sleep(0); }
	}
	return &cs;
}

class Class {
	const std::type_info* _ti;
public:
	Class() : _ti(0) {}
	Class(const std::type_info& ti) : _ti(&ti) {}
	std::string getName() const {
		if (!_ti) return std::string();
		std::string n(_ti->name());
		// MSVC type_info::name() returns "class Foo" / "struct Foo"; Java getName() is the BARE class
		// name, and the class-keyed render registries (RenderManager / TileEntityRenderer) key by bare-
		// name strings ("EntityPig"). Strip the C++ keyword prefix (+ any namespace) so lookups MATCH.
		if      (n.compare(0,6,"class ")  == 0) n.erase(0,6);
		else if (n.compare(0,7,"struct ") == 0) n.erase(0,7);
		else if (n.compare(0,5,"enum ")   == 0) n.erase(0,5);
		std::string::size_type cc = n.rfind("::");
		if (cc != std::string::npos) n.erase(0, cc + 2);
		return n;
	}
	std::string getSimpleName() const { return getName(); }
	std::string getCanonicalName() const { return getName(); }
	std::string toString() const { return std::string("class ") + getName(); }
	bool equals(const Class& o) const { return *this == o; }
	bool operator==(const Class& o) const { return _ti == o._ti || (_ti && o._ti && *_ti == *o._ti); }
	bool operator!=(const Class& o) const { return !(*this == o); }
	// A Java `.class` literal maps to a STABLE Class* (j2xClassOf, below); `getClass()` stays a Class
	// value. These let `getClass() == Foo.class` (value == Class*) compare by TYPE, not pointer.
	bool operator==(const Class* o) const { return o && *this == *o; }
	bool operator!=(const Class* o) const { return !(o && *this == *o); }
	const std::type_info* rawType() const { return _ti; }
	bool operator<(const Class& o) const {                 // strict-weak order for std::map keys
		if (!_ti) return o._ti != 0;
		if (!o._ti) return false;
		return _ti->before(*o._ti) != 0;
	}
	int hashCode() const {
		std::string n = getName(); int h = 0;
		for (std::string::size_type i = 0; i < n.size(); ++i) h = h * 31 + (unsigned char)n[i];
		return h;
	}
	bool isInstance(...) const { return false; }
	// FIXED BUG isassignablefrom-stub-mob-explosion: was `return false`, so World::countEntities() counted
	// ZERO of EVERY creature type -> the SpawnerAnimals mob cap `count <= maxNumberOfCreature*chunks/256`
	// was ALWAYS satisfied -> unbounded mob spawning (thousands of entities, 2 fps, heap pressure). Now a
	// real Java IS-A via the ancestry registry (which includes INTERFACES, e.g. EntityZombie -> IMob):
	// `A.isAssignableFrom(B)` is true iff B is-a A. j2xClassOf() passes a Class* here.
	bool isAssignableFrom(const Class* o) const {
		return _ti && o && o->_ti && _j2xIsA(o->_ti->name(), _ti->name());
	}
	bool isAssignableFrom(const Class& o) const {
		return _ti && o._ti && _j2xIsA(o._ti->name(), _ti->name());
	}
	bool isAssignableFrom(...) const { return false; }   // fallback for any other call shape
	// FIXED BUG getsuperclass-stub-no-firstperson-hand: was `return 0`, so RenderManager::
	// getEntityClassRenderObject could NOT walk up the class hierarchy -> getEntityRenderObject(thePlayer)
	// (an EntityPlayerSP; RenderPlayer is registered for EntityPlayer) returned NULL -> the first-person
	// hand/arm (ItemRenderer: `if(var15 != NULL) drawFirstPersonHand()`) was silently skipped, and any
	// entity whose exact class isn't directly registered fell back to no/served-wrong renderer. Real
	// superclass via the ancestry registry: _j2xBases()[me][0] is the EXTENDS parent (pipeline emits the
	// parent edge before interfaces), resolved back to a Class* through _j2xClassReg().
	Class* getSuperclass() {
		if (!_ti) return 0;
		std::map<std::string, std::vector<std::string> >::iterator it = _j2xBases().find(_ti->name());
		if (it == _j2xBases().end() || it->second.empty()) return 0;
		const std::string& sup = it->second[0];                 // [0] = extends parent (interfaces follow)
		CRITICAL_SECTION* _cs = _j2xClassRegCS(); EnterCriticalSection(_cs);
		std::vector<Class*>& reg = _j2xClassReg();
		for (std::size_t i = 0; i < reg.size(); ++i) {
			const std::type_info* rt = reg[i]->rawType();
			if (rt && sup == rt->name()) { LeaveCriticalSection(_cs); return reg[i]; }
		}
		LeaveCriticalSection(_cs);
		return 0;                                                // superclass not materialised as a Class yet
	}
	Class getConstructor(...) { return *this; }   // dead reflection: chain `.newInstance()` compiles; returns NULL
	void* newInstance(...) { return 0; }
	void* getDeclaredFields(...) { return 0; }
	// Real resource resolution (was `return 0`, which made ImageIO::read(getResource("/x.png")) return
	// a NULL image -> the loader derefed NULL -> crash). Returns a URL carrying the path; ImageIO::
	// read(URL*) resolves it via game:\Content\... The variadic fallback keeps other call shapes.
	URL* getResource(const std::string& name) { return new URL(name); }
	URL* getResource(...) { return 0; }
	// Real resource resolution (was `return 0`, which made every RenderEngine texture load NULL ->
	// readTextureImage crashed on NULL->close()). Routes "/terrain.png","/title/mojang.png",... to
	// game:\Content\... via FileIO. const std::string& accepts JavaString (extends std::string) and
	// const char* literals; the variadic fallback keeps any other call shape compiling.
	InputStream* getResourceAsStream(const std::string& name) { return J2X_getResourceAsStream(name.c_str()); }
	InputStream* getResourceAsStream(...) { return 0; }
	operator Class*() { return this; }                     // usable where a Class* is expected
};

// A Java `.class` literal must be a STABLE, non-dangling Class object (Java Class objects are
// process-lifetime singletons). j2xClassOf returns a Class* into a program-lifetime registry keyed
// by std::type_info, so the SAME type always yields the SAME pointer -- `EntityList::addMapping(
// EntityPig.class, ...)` stores a valid pointer (NOT the address of a destroyed temporary, the old
// `Class(typeid(X))` dangling bug) and `a.class == b.class` is correct pointer identity.
inline Class* j2xClassOf(const std::type_info& ti) {
	CRITICAL_SECTION* _cs = _j2xClassRegCS(); EnterCriticalSection(_cs);
	std::vector<Class*>& reg = _j2xClassReg();             // shared registry (see getSuperclass); leaked = program-lifetime
	for (unsigned i = 0; i < reg.size(); ++i) {
		const std::type_info* rt = reg[i]->rawType();
		if (rt && *rt == ti) { LeaveCriticalSection(_cs); return reg[i]; }
	}
	Class* c = new Class(ti); reg.push_back(c); LeaveCriticalSection(_cs); return c;
}
// free cross-comparisons so `Foo.class == obj->getClass()` (Class* == Class value) also works
inline bool operator==(const Class* a, const Class& b) { return a && *a == b; }
inline bool operator!=(const Class* a, const Class& b) { return !(a && *a == b); }
#endif // CLASS_H
'''
_GENERATED['Robot.h'] = _class_stub('Robot', void_methods=['mouseMove'])
# openStream() returns a real InputStream* (not void*) so `new DataInputStream(url.openStream())`
# binds to DataInputStream(InputStream*); parse(void*)/InputStreamReader(...) callers still accept it.
# URL carries a resource path when built by Class::getResource("/x.png") so ImageIO::read(URL*) can
# resolve it to game:\Content\... (the URL form of getResourceAsStream). openStream() returns
# InputStream* so `new DataInputStream(url.openStream())` binds.
_GENERATED['URL.h'] = _class_stub('URL', fwds=('InputStream', 'URLConnection'), ctors=False,
    ret_methods=[('openConnection', ('URLConnection*', '0')), ('openStream', ('InputStream*', '0')),
                 ('toString', ('std::string', 'std::string()')), ('getPath', ('std::string', 'std::string()')),
                 ('getFile', ('std::string', 'std::string()')), ('getProtocol', ('std::string', 'std::string()')),
                 ('getHost', ('std::string', 'std::string()')), ('getQuery', ('std::string', 'std::string()')),
                 ('getPort', ('int', '-1')), ('getDefaultPort', ('int', '-1')), ('toURI', ('void*', '0'))],
    extra=['std::string _j2xpath;',
           'explicit URL(...) {}   // explicit: a string reaches URL only through the path ctor below',
           'URL(const std::string& p) : _j2xpath(p) {}',
           'const std::string& j2xResourcePath() const { return _j2xpath; }'])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog', void_methods=['setVisible', 'setMode'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
_GENERATED['DecimalFormat.h'] = _load_canonical('DecimalFormat.h')   # REAL pattern formatter (stats screen)
STUB_MEMBERS['DecimalFormat'] = {'parent': 'NumberFormat', 'methods': {'format': 'std::string'}}
_GENERATED['ImageObserver.h'] = _class_stub('ImageObserver')

# --- additional java.* stubs surfaced by the Beta corpus (all dead or shimmed on the console) ---
# java.util.zip: chunk (de)compression. Real GZIP work happens through the GZIP streams elsewhere;
# these are type stubs so RegionFile / Packet51MapChunk / ChunkLoader compile.
# java.util.zip.Inflater -- REAL zlib/raw-DEFLATE decompress via the bundled puff.c (Inflate.reference.cpp:
# zlibInflate/rawInflate). [360] equivalent: NO XDK zlib -- XMemDecompress is LZX (wrong format); use puff.
# FIXED BUG mp-chunks-not-decompressed: the stub inflate() returned 0, so Packet51MapChunk decompressed SMP
# terrain into an EMPTY buffer -> a joined server rendered as a dark void. new Inflater() = zlib; (true) = raw.
_GENERATED['Inflater.h'] = (
    '#ifndef INFLATER_H\n#define INFLATER_H\n'
    '#include "JArray.h"\n#include "Inflate.h"\n\n'
    '// REAL java.util.zip.Inflater -- zlib (RFC-1950) / raw DEFLATE decompress via the bundled puff.c-based\n'
    '// Inflate.cpp (the 360 SDK ships no zlib; XMemDecompress is LZX, a DIFFERENT format). setInput()/inflate()\n'
    '// are what Packet51MapChunk feeds SMP terrain through. new Inflater() = zlib-wrapped; new Inflater(true) = raw.\n'
    'class Inflater {\n'
    '\tconst unsigned char* _in; int _inLen; bool _nowrap; int _lastOut; bool _done;\n'
    'public:\n'
    '\tInflater() : _in(0), _inLen(0), _nowrap(false), _lastOut(0), _done(false) {}\n'
    '\tInflater(bool nowrap) : _in(0), _inLen(0), _nowrap(nowrap), _lastOut(0), _done(false) {}\n'
    '\tvirtual ~Inflater() {}\n'
    '\t// BY-REFERENCE (JArray convention): a by-value JArray could deep-copy -> dangling _in / writes to a throwaway.\n'
    '\tvirtual void setInput(JArray<unsigned char>& b) { _in = b.data(); _inLen = b.length; _done = false; }\n'
    '\tvirtual void setInput(JArray<unsigned char>& b, int off, int len) { _in = b.data() + off; _inLen = len; _done = false; }\n'
    '\tvirtual int inflate(JArray<unsigned char>& out) {\n'
    '\t\tif (!_in || _inLen <= 0 || out.length <= 0) { _lastOut = 0; _done = true; return 0; }\n'
    '\t\tint n = _nowrap ? rawInflate(_in, _inLen, out.data(), out.length)\n'
    '\t\t                : zlibInflate(_in, _inLen, out.data(), out.length);\n'
    '\t\t_lastOut = (n < 0) ? 0 : n; _done = true; return _lastOut;\n'
    '\t}\n'
    '\tvirtual void end() {}\n'
    '\tvirtual void reset() { _in = 0; _inLen = 0; _lastOut = 0; _done = false; }\n'
    '\tvirtual bool finished() { return _done; }\n'
    '\tvirtual bool needsInput() { return _in == 0; }\n'
    '\tvirtual int getRemaining() { return 0; }\n'
    '\tvirtual int getTotalOut() { return _lastOut; }\n'
    '};\n#endif // INFLATER_H\n')
_GENERATED['Deflater.h'] = _class_stub('Deflater',
    void_methods=['setInput', 'finish', 'end', 'setLevel', 'reset'],
    ret_methods=[('deflate', ('int', '0')), ('finished', ('bool', 'false')), ('getTotalOut', ('int', '0'))])
# REAL InflaterInputStream (puff.c zlibInflate) -- was a no-op stub (region-chunk zlib read path).
_GENERATED['InflaterInputStream.h'] = _load_canonical('InflaterInputStream.h')
# REAL DeflaterOutputStream (puff.c zlibDeflateStored) -- was a no-op stub, so RegionFile chunk writes were empty.
_GENERATED['DeflaterOutputStream.h'] = _load_canonical('DeflaterOutputStream.h')
# REAL ByteArrayInputStream (reads from a byte array) -- was a no-op stub (chunk read path fed empty bytes).
_GENERATED['ByteArrayInputStream.h'] = _load_canonical('ByteArrayInputStream.h')
# java.util.zip exceptions
_GENERATED['DataFormatException.h'] = _class_stub('DataFormatException', bases=['Exception'], includes=['Exception'])
_GENERATED['NoSuchAlgorithmException.h'] = _class_stub('NoSuchAlgorithmException', bases=['Exception'], includes=['Exception'])
# java.math.BigInteger (MD5String prints the digest with it)
_GENERATED['BigInteger.h'] = _load_canonical('BigInteger.h')   # sign + magnitude, toString(radix)
STUB_MEMBERS['BigInteger'] = {'parent': None, 'methods': {'toString': 'std::string', 'abs': 'BigInteger*',
    'signum': 'int'}}
# java.lang.ref (RegionFileCache soft cache -> just holds the referent)
# java.lang.ref.SoftReference -- REAL strong ref (no GC on the console). FIXED BUG regionfile-cache-never-hits:
# the old stub's get()->0 permanently defeated RegionFileCache (every lookup missed -> a fresh RegionFile per
# chunk access -> region-path ResolvePath flood + slow world-gen). Store the referent; get() returns it.
_GENERATED['SoftReference.h'] = (
    '#ifndef SOFTREFERENCE_H\n#define SOFTREFERENCE_H\n'
    '#include <string>\n#include "JavaString.h"\n#include "Reference.h"\n'
    'class SoftReference : public Reference {\n'
    '\tvoid* _referent;\n'
    'public:\n'
    '\tSoftReference() : _referent(0) {}\n'
    '\ttemplate<class T> SoftReference(T* r) : _referent((void*)r) {}\n'
    '\ttemplate<class T, class Q> SoftReference(T* r, Q*) : _referent((void*)r) {}\n'
    '\tvirtual ~SoftReference() {}\n'
    '\tvirtual void* get(...) { return _referent; }   // strong ref: hand back the stored object\n'
    '};\n#endif\n')
# java.net.InetAddress -- now CARRIES the resolved host so the real XNet Socket(InetAddress,port) ctor
# can connect. FIXED BUG socket-inetaddress-host-dropped: getByName() used to return a HOSTLESS
# InetAddress, so `new Socket(InetAddress.getByName(host), port)` (the standard NetClientHandler idiom)
# stayed unconnected and multiplayer hung on "Connecting...". Socket.reference.cpp reads getHostName().
_GENERATED['InetAddress.h'] = (
    '#ifndef INETADDRESS_H\n#define INETADDRESS_H\n'
    '#include <string>\n#include "JavaString.h"\n\n'
    'class InetAddress {\n'
    '\tstd::string _host;\n'
    'public:\n'
    '\tInetAddress() {}\n'
    '\tInetAddress(const JavaString& h) : _host(h.c_str()) {}\n'
    '\tvirtual ~InetAddress() {}\n'
    '\tvirtual JavaString getHostName(...) { return JavaString(_host); }\n'
    '\tvirtual JavaString getHostAddress(...) { return JavaString(_host); }\n'
    '\tstatic InetAddress* getByName(const JavaString& h) { return new InetAddress(h); }\n'
    '\tstatic InetAddress* getLocalHost(...) { return new InetAddress(); }\n'
    '};\n#endif\n')
_GENERATED['StringReader.h'] = _class_stub('StringReader', bases=['InputStreamReader'], includes=['InputStreamReader'])
# java.awt (applet / isometric-preview / crash-report paths -- all dead on the console)
_GENERATED['Font.h'] = _class_stub('Font', ret_methods=[('getSize', ('int', '0')), ('deriveFont', ('Font*', '0'))])
_GENERATED['FontMetrics.h'] = _class_stub('FontMetrics',
    ret_methods=[('stringWidth', ('int', '0')), ('getHeight', ('int', '0')), ('charWidth', ('int', '0'))])
_GENERATED['AffineTransform.h'] = _class_stub('AffineTransform')
_GENERATED['DataBufferInt.h'] = _class_stub('DataBufferInt', ret_methods=[('getData', ('void*', '0'))])
_GENERATED['Graphics2D.h'] = _class_stub('Graphics2D', bases=['Graphics'],
    includes=['Graphics', 'FontMetrics'], fwds=['AffineTransform', 'Rectangle'],
    void_methods=['setClip', 'setRenderingHint', 'translate', 'scale', 'setColor', 'fillRect',
                  'drawImage', 'setTransform', 'drawString', 'dispose', 'rotate', 'shear'],
    ret_methods=[('getTransform', ('AffineTransform*', '0')), ('getClipBounds', ('Rectangle*', '0')),
                 ('getFontMetrics', ('FontMetrics*', '0'))])
_GENERATED['BufferStrategy.h'] = _class_stub('BufferStrategy', includes=['Graphics'], fwds=['Graphics'],
    void_methods=['show'], ret_methods=[('getDrawGraphics', ('Graphics*', '0'))])
# LWJGL/paulscode sound plug referenced by SoundManager.addLibrary(...)
_GENERATED['LibraryLWJGLOpenAL.h'] = _class_stub('LibraryLWJGLOpenAL')

# Java exception hierarchy (exceptions are OFF on the console; these are just type stubs so
# `extends Error`, `new IOException(...)`, and catch/throws references compile).
_EXC_METHODS = [('getMessage', ('std::string', 'std::string()')),
                ('getLocalizedMessage', ('std::string', 'std::string()')),
                ('toString', ('std::string', 'std::string()'))]
_GENERATED['Throwable.h'] = _class_stub('Throwable', void_methods=['printStackTrace'], ret_methods=_EXC_METHODS)
for _e, _base in [('Exception', 'Throwable'), ('Error', 'Throwable'), ('RuntimeException', 'Exception'),
                  ('IOException', 'Exception'), ('InterruptedException', 'Exception'),
                  ('IllegalStateException', 'RuntimeException'), ('IllegalArgumentException', 'RuntimeException'),
                  ('MalformedURLException', 'IOException'), ('AWTException', 'Exception'),
                  ('LWJGLException', 'Exception'), ('NoSuchFieldError', 'Error'),
                  # java.io / java.net / java.lang exceptions used in catch/throws clauses
                  ('FileNotFoundException', 'IOException'), ('EOFException', 'IOException'),
                  ('SocketException', 'IOException'), ('UnknownHostException', 'IOException'),
                  ('ConnectException', 'SocketException'), ('BindException', 'SocketException'),
                  ('NumberFormatException', 'IllegalArgumentException'),
                  ('NullPointerException', 'RuntimeException'),
                  ('ArrayIndexOutOfBoundsException', 'RuntimeException'),
                  ('IndexOutOfBoundsException', 'RuntimeException'),
                  ('ClassCastException', 'RuntimeException'),
                  ('NoSuchElementException', 'RuntimeException'),
                  ('UnsupportedOperationException', 'RuntimeException'),
                  ('ConcurrentModificationException', 'RuntimeException'),
                  ('OutOfMemoryError', 'Error'), ('NoSuchMethodException', 'Exception'),
                  ('CloneNotSupportedException', 'Exception'), ('ClassNotFoundException', 'Exception')]:
    _GENERATED[_e + '.h'] = _class_stub(_e, bases=[_base], includes=[_base])
# java.io.File (richer than the early minimal stub; overrides it via FILES.update)
_GENERATED['URI.h'] = _class_stub('URI', fwds=['URL'], ret_methods=[('toURL', ('URL*', '0'))])
# File is NOT a symbol-only stub: the entire IO layer reads File::path (CreateFile/fopen) and resource
# loading recurses listFiles()/isDirectory(). A no-op File silently breaks all file IO -- saves never
# write, the sound/resource pools never fill (silent no-audio). Emit the REAL path-backed canonical File.
_GENERATED['File.h'] = _load_canonical('File.h')
# Register File's member return types for the chain resolver (normally _class_stub's side effect). Without
# this it doesn't know getParentFile()/getAbsoluteFile() return File* -> emits `x->getParentFile().mkdirs()`
# (`.` on a pointer, C2228) instead of `->`. Mirrors the old _class_stub('File', ret_methods=[...]).
STUB_MEMBERS['File'] = {'parent': None, 'methods': {
    'getName': 'std::string', 'getPath': 'std::string', 'getAbsolutePath': 'std::string',
    'toString': 'std::string', 'getParent': 'std::string',
    'exists': 'bool', 'isDirectory': 'bool', 'isFile': 'bool', 'canRead': 'bool', 'canWrite': 'bool',
    'mkdir': 'bool', 'mkdirs': 'bool', 'renameTo': 'bool', 'delete_': 'bool', 'createNewFile': 'bool',
    'length': 'long long', 'lastModified': 'long long',
    'getParentFile': 'File*', 'getAbsoluteFile': 'File*',
    'toURI': 'URI', 'listFiles': 'JArray<File* >', 'list': 'void*'}}
# java.awt GUI bases (desktop-only entry points: MinecraftApplet extends Applet, etc.)
_GENERATED['Applet.h'] = _class_stub('Applet', bases=['Component'], includes=['Component'], fwds=['URL', 'Graphics'],
    void_methods=['validate', 'setLayout', 'add', 'repaint', 'requestFocus', 'setVisible',
                  'resize', 'init', 'start', 'stop', 'remove', 'removeAll', 'setSize',
                  'setBackground', 'setForeground', 'addKeyListener', 'addMouseListener',
                  'addMouseMotionListener', 'addFocusListener', 'addComponentListener',
                  'setFocusable', 'setFocusTraversalKeysEnabled', 'enableInputMethods'],
    ret_methods=[('getParameter', ('std::string', 'std::string()')),
                 ('getDocumentBase', ('URL*', '0')), ('getCodeBase', ('URL*', '0')),
                 ('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')), ('isActive', ('bool', 'false')),
                 ('isDisplayable', ('bool', 'false')), ('getParent', ('void*', '0'))])
_GENERATED['Canvas.h'] = _class_stub('Canvas', bases=['Component'], includes=['Component'], fwds=['Graphics', 'BufferStrategy'],
    void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0')),   # getLocationOnScreen inherited from Component (Point*)
                 ('getBufferStrategy', ('BufferStrategy*', '0'))], extra=['void createBufferStrategy(...) {}'])
# getLocationOnScreen returns java.awt.Point (Indev's MouseHelper does `Point p = c.getLocationOnScreen()`);
# a shared static Point keeps the dead cursor-centering path from NULL-derefing on the console.
_GENERATED['Component.h'] = _class_stub('Component', includes=['Point'], fwds=['Graphics'], void_methods=_AWT_VOID,
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getGraphics', ('Graphics*', '0'))],
    extra=['Point* getLocationOnScreen() { static Point _p; return &_p; }'])
_GENERATED['FileDialog.h'] = _class_stub('FileDialog',
    void_methods=['setVisible', 'setMode', 'setDirectory', 'setFile', 'setFilenameFilter',
                  'setLocationRelativeTo'],
    ret_methods=[('getFile', ('std::string', 'std::string()')),
                 ('getDirectory', ('std::string', 'std::string()'))])
# java.awt mouse/point stubs. Indev's MouseHelper centers the OS cursor via MouseInfo/Robot/Point --
# DEAD on the console (the controller drives look through the Input layer), but the game code must
# still compile. Point-returning getters hand back a shared static Point so the no-op path never
# NULL-derefs (`var1->x`). Dialog is only referenced as the `(Dialog*)NULL` parent of a FileDialog.
_GENERATED['Point.h'] = _class_stub('Point', extra=['int x;', 'int y;'])
_GENERATED['PointerInfo.h'] = _class_stub('PointerInfo', includes=['Point'],
    extra=['Point* getLocation() { static Point _p; return &_p; }'])
_GENERATED['MouseInfo.h'] = _ns_stub('MouseInfo', includes=['PointerInfo'],
    ret_fns=[('getPointerInfo', ('PointerInfo', 'PointerInfo()'))])
_GENERATED['Dialog.h'] = _class_stub('Dialog')
# java.awt.image raster chain: BufferedImage.getRaster().getDataBuffer() -> (DataBufferInt*)-> getData()
# (ImageBufferDownload reads the pixel int[] directly). Dead on the 360, but must type-check.
_GENERATED['DataBuffer.h'] = _class_stub('DataBuffer')
_GENERATED['DataBufferInt.h'] = _class_stub('DataBufferInt', bases=['DataBuffer'], includes=['DataBuffer', 'JArray'],
    ret_methods=[('getData', ('JArray<int >', 'JArray<int >()'))])
_GENERATED['Raster.h'] = _class_stub('Raster', fwds=['DataBuffer'],
    ret_methods=[('getDataBuffer', ('DataBuffer*', '0'))])
_GENERATED['WritableRaster.h'] = _class_stub('WritableRaster', bases=['Raster'], includes=['Raster'])
_GENERATED['BufferedImage.h'] = _class_stub('BufferedImage', includes=['Raster', 'Graphics'],
    void_methods=['setRGB', 'flush'],
    ret_methods=[('getWidth', ('int', '0')), ('getHeight', ('int', '0')),
                 ('getRGB', ('int', '0')), ('getData', ('void*', '0')),
                 ('getRaster', ('Raster', 'Raster()')),
                 ('getGraphics', ('Graphics*', 'j2xSharedGraphics()')),
                 ('createGraphics', ('Graphics*', 'j2xSharedGraphics()'))])


COLLECTIONS_H = _load_canonical('Collections.h')


# Runtime container/buffer method result-types, so the chain resolver can type a chain THROUGH them
# and pick `.` vs `->` correctly: `x.entrySet().iterator().next()` (iterator() -> Iterator*, so .next()
# -> ->next()) and `ByteBuffer::allocateDirect(n).order(bo).asIntBuffer()`. Merged into the member env.
STUB_MEMBERS['JList'] = {'parent': None, 'methods': {'iterator': 'Iterator*'}}
STUB_MEMBERS['JSet'] = {'parent': None, 'methods': {'iterator': 'Iterator*'}}
STUB_MEMBERS['JMap'] = {'parent': None, 'methods': {'entrySet': 'JList', 'values': 'JList', 'keySet': 'JSet'}}
STUB_MEMBERS['ByteBuffer'] = {'parent': None, 'methods': {
    'allocateDirect': 'ByteBuffer*', 'allocate': 'ByteBuffer*', 'order': 'ByteBuffer*',
    'asIntBuffer': 'IntBuffer*', 'asFloatBuffer': 'FloatBuffer*'}}

# filename -> content
FILES = {
    'Random.h': RANDOM_H,
    'JavaString.h': JAVASTRING_H,
    'JArray.h': JARRAY_H,
    'JIterator.h': JITERATOR_H,
    # java.util.Iterator maps to the `Iterator` struct, which lives in JIterator.h. Code that names the
    # type emits `#include "Iterator.h"` (RUNTIME_HEADERS), so provide it as a thin alias header.
    'Iterator.h': '#ifndef ITERATOR_H\n#define ITERATOR_H\n#include "JIterator.h"\n#endif\n',
    'JList.h': JLIST_H,
    'JSet.h': JSET_H,
    'JMap.h': JMAP_H,
    'JavaRT.h': JAVART_H,
    'GL11.h': GL11_H,
    'Runnable.h': RUNNABLE_H,
    'Thread.h': THREAD_H,
    'Comparator.h': COMPARATOR_H,
    'Collections.h': COLLECTIONS_H,
    'File.h': FILE_H,
    'FilenameFilter.h': FILENAMEFILTER_H,
    'Canvas.h': CANVAS_H,
    'Applet.h': APPLET_H,
}
FILES.update(_GENERATED)


def unwired_canonicals():
    """Every docs/platform/canonical/*.h must be _load_canonical'd into a real emitted file; one that isn't
    silently falls back to its no-op stub (same class as the GLU flat-world bug). Returns the canonical
    headers that are NOT present in the emitted FILES map -- empty = all wired."""
    if not os.path.isdir(_CANONICAL_DIR):
        return []
    bad = []
    for fn in sorted(os.listdir(_CANONICAL_DIR)):
        if fn.endswith('.h') and fn not in FILES:
            bad.append(fn)
    return bad


# Fail fast at import if a canonical header was added but never wired (so it can't ship as a silent stub).
_UNWIRED = unwired_canonicals()
if _UNWIRED:
    import sys as _sys
    _sys.stderr.write('[runtime] !! UNWIRED CANONICAL(S) -- exist but never _load_canonical\'d (silent stub '
                      'fallback): %s\n' % ', '.join(_UNWIRED))


def annotate_stub(text, class_name, platform_id):
    """Inject a platform-specific SDK hint comment after the EXTERNAL-LIBRARY STUB banner.
    If the file isn't a stub or no platform is set, returns the text unchanged."""
    if 'EXTERNAL-LIBRARY STUB' not in text or not platform_id:
        return text
    from codebreaker.java.stubs import classify_stub
    from codebreaker.java.platforms import get_stub_comment
    subsystem_id, _info = classify_stub(class_name)
    comment = get_stub_comment(platform_id, subsystem_id)
    marker = '// =============================================================================='
    return text.replace(marker, marker + '\n' + comment, 1)


_PLATFORM_BANNER_OLD = 'the real behaviour is the Xbox 360 platform layer'
_PLATFORM_BANNER_SDK_OLD = '// (Direct3D 9 / XInput / file IO / sound).'

# The file layer's out-of-line definitions for a conversion with no platform (see generate_files): what
# FileIO.reference.cpp gives the 360 through CreateFile, here through stdio.
HOST_IO_CPP = '''#include "stdafx.h"
#include "JavaRT.h"
#include <cstdio>
#include "File.h"
#include "FileInputStream.h"

JavaString File::s_separator = JavaString("\\\\");

void FileInputStream::_load(const char* p) {
	FILE* f = p && p[0] ? std::fopen(p, "rb") : 0;
	if (!f) return;                                     // _data stays NULL -> read() == -1 (EOF)
	if (std::fseek(f, 0, SEEK_END) == 0) {
		long sz = std::ftell(f);
		if (sz > 0 && std::fseek(f, 0, SEEK_SET) == 0) {
			_data = new unsigned char[sz]();
			_size = (int)std::fread(_data, 1, (size_t)sz, f);
		}
	}
	std::fclose(f);
}

FileInputStream::FileInputStream(File* f) : _data(0), _size(0), _pos(0) { _load(f ? f->path.c_str() : ""); }

// Class.getResourceAsStream: the file under the current folder, or null when there is none (as Java,
// when the resource is not on the classpath)
InputStream* J2X_getResourceAsStream(const char* name) {
	while (name && (*name == '/' || *name == '\\\\')) ++name;
	if (!name || !*name) return 0;
	FILE* f = std::fopen(name, "rb");
	if (!f) return 0;
	std::fclose(f);
	return new FileInputStream(std::string(name));
}
'''


def generate_files(cpp_std=None, platform_id=None):
    """Dynamically generate the full runtime file set, applying C++ standard
    transforms and platform-specific stub annotations on the fly.

    This is the dynamic replacement for the static FILES dict. Every call
    produces a fresh dict so the same converter session can re-run with
    different settings without stale cached output."""
    from codebreaker.java.emit.std_transforms import apply_std_transforms
    from codebreaker.java.platforms import PLATFORMS, generate_stdafx
    plat = PLATFORMS.get(platform_id, {}) if platform_id else {}
    plat_label = plat.get('label', 'Xbox 360')
    std_label = cpp_std.label if cpp_std else 'C++03'

    out = {}

    if platform_id:
        out['stdafx.h'] = generate_stdafx(platform_id, std_label)
        # The PCH *create* source: Xbox 360 projects build <Project>.pch from stdafx.cpp (/Yc),
        # every other .cpp uses it (/Yu). Without this the vcxproj's stdafx.cpp Create block has no
        # file and all sources fail with C1083 "cannot open precompiled header". (Emitted for all
        # platforms -- it is just `#include "stdafx.h"`, harmless where no PCH is used.)
        out['stdafx.cpp'] = '#include "stdafx.h"\n'
    else:
        # Every emitted file includes stdafx.h; with no platform it is the generic one (the C/C++
        # standard library and the fixed-width typedefs), so a platform-less conversion compiles.
        out['stdafx.h'] = generate_stdafx('generic', std_label)
        # ... and the out-of-line pieces of the file layer that a platform's reference layer defines
        # (FileIO.reference.cpp on the 360), in standard C, so a program that opens a file still links
        # (WorldProvider, CanvasIsomPreview: undefined FileInputStream::FileInputStream(File*)).
        out['j2x_host_io.cpp'] = HOST_IO_CPP

    is_non_windows = platform_id in ('ps3', 'wii', 'psp', 'nds')

    for fname, content in FILES.items():
        class_name = fname[:-2] if fname.endswith('.h') else fname
        if platform_id and plat_label != 'Xbox 360':
            content = content.replace(_PLATFORM_BANNER_OLD,
                                      'the real behaviour is the %s platform layer' % plat_label)
            subs = plat.get('subsystems', {})
            from codebreaker.java.stubs import classify_stub
            sid, _ = classify_stub(class_name)
            sub_info = subs.get(sid)
            if sub_info:
                content = content.replace(_PLATFORM_BANNER_SDK_OLD,
                                          '// (%s / %s).' % (sub_info['sdk'], sub_info['libs']))
        if (is_non_windows or not platform_id) and fname in ('JavaRT.h', 'Random.h'):
            content = _patch_timer_for_platform(content, platform_id or 'generic')
        if platform_id:
            content = annotate_stub(content, class_name, platform_id)
        if cpp_std is not None:
            content = apply_std_transforms(content, cpp_std)
        out[fname] = content
    return out


_TIMER_PATCHES = {
    # no platform: the C library's clocks (coarse, but portable C++03 -- no <windows.h> timers)
    'generic': {
        'GetTickCount()': '((unsigned long)(clock() * 1000.0 / CLOCKS_PER_SEC))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn (long long)time(0) * 1000LL;\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn (long long)(clock() * (1000000000.0 / CLOCKS_PER_SEC));\n'
            '\t}'
        ),
    },
    'ps3': {
        'GetTickCount()': '((unsigned long)(sys_time_get_system_time() / 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn (long long)(sys_time_get_system_time() / 1000);\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn (long long)sys_time_get_system_time() * 1000LL;\n'
            '\t}'
        ),
    },
    'wii': {
        'GetTickCount()': '((unsigned long)(ticks_to_millisecs(gettime())))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn (long long)ticks_to_millisecs(gettime());\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn (long long)ticks_to_nanosecs(gettime());\n'
            '\t}'
        ),
    },
    'psp': {
        'GetTickCount()': '((unsigned long)(sceKernelGetSystemTimeLow() / 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n'
            '\t\treturn (long long)(tick.tick / 1000);\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\tSceRtcTick tick; sceRtcGetCurrentTick(&tick);\n'
            '\t\treturn (long long)(tick.tick * 1000LL);\n'
            '\t}'
        ),
    },
    'nds': {
        'GetTickCount()': '((unsigned long)(cpuGetTiming() / BUS_CLOCK * 1000))',
        'currentTimeMillis': (
            'inline long long currentTimeMillis() {\n'
            '\t\treturn 0LL; // TODO: implement DS timer\n'
            '\t}'
        ),
        'nanoTime': (
            'inline long long nanoTime() {\n'
            '\t\treturn 0LL; // TODO: implement DS timer\n'
            '\t}'
        ),
    },
}

import re as _re

_CURRENT_TIME_RE = _re.compile(
    r'inline long long currentTimeMillis\(\)[^}]*\}')
_NANO_TIME_RE = _re.compile(
    r'inline long long nanoTime\(\)\s*\{[^}]*\}')
_GET_TICK_COUNT = _re.compile(
    r'\(long long\)GetTickCount\(\)')


def _patch_timer_for_platform(content, platform_id):
    """Replace Windows timer calls in JavaRT.h / Random.h with platform equivalents."""
    patch = _TIMER_PATCHES.get(platform_id)
    if not patch:
        return content
    if 'currentTimeMillis' in patch:
        content = _CURRENT_TIME_RE.sub(patch['currentTimeMillis'], content)
    if 'nanoTime' in patch:
        content = _NANO_TIME_RE.sub(patch['nanoTime'], content)
    if 'GetTickCount()' in patch:
        content = _GET_TICK_COUNT.sub(patch['GetTickCount()'], content)
    return content
