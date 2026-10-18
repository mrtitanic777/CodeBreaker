# -*- coding: utf-8 -*-
"""
stubs.py -- classify converter stubs by target subsystem.

Every stub header the converter emits needs a REAL implementation backed by
platform-specific APIs. This module inspects the stub headers, classifies them
by subsystem (rendering, audio, input, io, networking, ...), and annotates each
with what SDK/platform feature the stub needs.

When an SDK directory is provided, the classifier cross-references the stub's
requirements against the SDK headers to suggest which SDK APIs map to which
stub methods.
"""

import os
import re
from collections import OrderedDict


# ---------------------------------------------------------------------------
# Subsystem definitions: (subsystem_id, label, description, sdk_hint, members)
# ---------------------------------------------------------------------------
SUBSYSTEMS = OrderedDict([
    ('rendering', {
        'label': 'Rendering / Display',
        'description': 'OpenGL/LWJGL stubs that need a D3D9 backend (vertex submission, '
                       'texture loading, display management, framebuffer ops).',
        'sdk_hint': 'D3D9 (d3d9.h, d3dx9.h), HLSL shaders',
        'members': {
            'GL11', 'GL12', 'GL15', 'GLU', 'Display', 'DisplayMode',
            'BufferedImage', 'ImageIO', 'Canvas', 'Graphics', 'Frame',
            'Panel', 'Component', 'Applet', 'Color', 'Font', 'FontMetrics',
            'CardLayout', 'BorderLayout', 'FlowLayout', 'GridLayout',
            'GridBagLayout', 'Dimension', 'Cursor', 'Robot',
            'ComponentEvent', 'ComponentListener',
            'FocusEvent', 'FocusListener',
            'WindowAdapter', 'WindowEvent', 'WindowListener',
            'ARBOcclusionQuery', 'ARBVertexBufferObject',
            'ImageObserver', 'ScreenShotHelper',
        },
    }),
    ('audio', {
        'label': 'Audio',
        'description': 'OpenAL/paulscode stubs that need a platform audio backend '
                       '(sound effects, music, 3D positional audio).',
        'sdk_hint': 'XACT3 (xact3.h, xact3wb.h) or XAudio2',
        'members': {
            'SoundSystem', 'SoundPool', 'SoundManager',
            'CodecJOrbis', 'CodecWav', 'SoundSystemConfig', 'SoundBuffer',
        },
    }),
    ('input', {
        'label': 'Input / Controllers',
        'description': 'LWJGL Keyboard/Mouse stubs that need platform input '
                       '(controller, touchscreen, keyboard emulation).',
        'sdk_hint': 'XInput2 (xinput2.h) for Xbox 360 controller',
        'members': {
            'Keyboard', 'Mouse', 'Controllers',
            'MouseEvent', 'MouseListener', 'MouseMotionListener',
            'MouseWheelListener', 'KeyEvent', 'KeyListener',
            'InputEvent',
        },
    }),
    ('io', {
        'label': 'File I/O / Streams',
        'description': 'Java I/O stubs that need platform file system access '
                       '(file read/write, data streams, buffered I/O).',
        'sdk_hint': 'Platform file API (CreateFile/ReadFile on Xbox, or stdio)',
        'members': {
            'File', 'FileInputStream', 'FileOutputStream',
            'InputStream', 'OutputStream',
            'DataInput', 'DataOutput', 'DataInputStream', 'DataOutputStream',
            'BufferedInputStream', 'BufferedOutputStream', 'BufferedReader',
            'PrintWriter', 'ByteArrayOutputStream',
            'FileReader', 'FileWriter', 'FileDialog',
            'FileNotFoundException',
        },
    }),
    ('compression', {
        'label': 'Compression',
        'description': 'GZIP/ZIP stubs for save data and resource loading.',
        'sdk_hint': 'LZX (xcompress.h) on Xbox 360, or zlib/miniz',
        'members': {
            'GZIPInputStream', 'GZIPOutputStream', 'ZipFile', 'ZipEntry',
            'Inflate',
        },
    }),
    ('networking', {
        'label': 'Networking',
        'description': 'Java socket/URL stubs for multiplayer and web requests.',
        'sdk_hint': 'Winsock2 / XNet (xnetn.h) on Xbox 360',
        'members': {
            'Socket', 'URL', 'URLConnection', 'HttpURLConnection',
            'InetAddress', 'BindException', 'SocketException',
            'MalformedURLException', 'ConnectException', 'UnknownHostException',
        },
    }),
    ('nio', {
        'label': 'NIO Buffers',
        'description': 'Java NIO buffer stubs (ByteBuffer, IntBuffer, FloatBuffer) '
                       'used for vertex data and texture uploads.',
        'sdk_hint': 'Direct memory buffers (malloc/free or D3D vertex buffers)',
        'members': {
            'ByteBuffer', 'IntBuffer', 'FloatBuffer', 'BufferUtils',
        },
    }),
    ('threading', {
        'label': 'Threading',
        'description': 'Java Thread stubs for background tasks (downloads, network, sleep).',
        'sdk_hint': 'Platform threads (CreateThread / _beginthreadex)',
        'members': {
            'Thread', 'ThreadDownloadImage', 'ThreadDownloadResources',
            'ThreadSleepForever', 'ThreadConnectToServer', 'ThreadStatistics',
            'ThreadShutdown', 'ThreadCheckHasPaid',
        },
    }),
    ('text', {
        'label': 'Text / Locale / Date',
        'description': 'Java text formatting stubs (SimpleDateFormat, Locale, Calendar).',
        'sdk_hint': 'strftime / manual formatting (no ICU on 360)',
        'members': {
            'SimpleDateFormat', 'Locale', 'Calendar', 'DateFormat', 'DecimalFormat',
        },
    }),
    ('reflection', {
        'label': 'Reflection / Class',
        'description': 'Java reflection stubs (Class, typeid). Minecraft uses these for '
                       'entity/tile-entity registry lookups.',
        'sdk_hint': 'RTTI (typeid) + manual factory registry',
        'members': {
            'Class',
        },
    }),
    ('exceptions', {
        'label': 'Exceptions',
        'description': 'Java exception type stubs. Mostly trivial (extend std::exception).',
        'sdk_hint': 'C++ exceptions (std::exception subclasses)',
        'members': {
            'Exception', 'RuntimeException', 'IOException',
            'AWTException', 'ArrayIndexOutOfBoundsException',
            'ClassCastException', 'ClassNotFoundException',
            'CloneNotSupportedException', 'NumberFormatException',
            'IllegalArgumentException', 'IllegalStateException',
            'IndexOutOfBoundsException', 'InterruptedException',
            'LWJGLException', 'NoSuchElementException',
            'NoSuchMethodException', 'NullPointerException',
            'UnsupportedOperationException', 'ConcurrentModificationException',
            'EOFException', 'ConnectException',
        },
    }),
    ('system', {
        'label': 'System / Runtime',
        'description': 'JVM system stubs (Sys timer, error handling).',
        'sdk_hint': 'QueryPerformanceCounter / GetTickCount',
        'members': {
            'Sys', 'Throwable', 'Error', 'OutOfMemoryError', 'NoSuchFieldError',
            'Comparable',
        },
    }),
])


def classify_stub(class_name):
    """Return (subsystem_id, subsystem_info) for a stub class name, or ('unknown', None)."""
    for sid, info in SUBSYSTEMS.items():
        if class_name in info['members']:
            return sid, info
    return 'unknown', None


def scan_stubs(output_dir):
    """Scan converter output for stub headers. Returns list of dicts:
    [{file, class_name, subsystem, label, sdk_hint, description, method_count, methods}]"""
    results = []
    for fname in sorted(os.listdir(output_dir)):
        if not fname.endswith('.h'):
            continue
        path = os.path.join(output_dir, fname)
        with open(path, 'r', encoding='utf-8', errors='replace') as f:
            text = f.read()
        if 'EXTERNAL-LIBRARY STUB' not in text:
            continue

        class_name = fname[:-2]
        sid, info = classify_stub(class_name)

        methods = []
        for m in re.finditer(
                r'(?:virtual\s+)?(\w[\w:<>*& ]*?)\s+(\w+)\s*\(([^)]*)\)\s*\{', text):
            ret, name, params = m.group(1).strip(), m.group(2), m.group(3).strip()
            if name in (class_name, '~' + class_name):
                continue
            methods.append({'return': ret, 'name': name, 'params': params})

        results.append({
            'file': fname,
            'class_name': class_name,
            'subsystem': sid,
            'label': info['label'] if info else 'Unknown',
            'sdk_hint': info['sdk_hint'] if info else 'platform-specific',
            'description': info['description'] if info else '',
            'method_count': len(methods),
            'methods': methods,
        })
    return results


def scan_sdk(sdk_dir):
    """Scan an SDK directory for header files and extract available APIs.
    Returns {filename: [api_names]} for cross-referencing with stub needs."""
    apis = {}
    if not sdk_dir or not os.path.isdir(sdk_dir):
        return apis
    for root, _dirs, files in os.walk(sdk_dir):
        for f in files:
            if not f.endswith('.h'):
                continue
            path = os.path.join(root, f)
            try:
                with open(path, 'r', encoding='utf-8', errors='replace') as fh:
                    text = fh.read()
            except Exception:
                continue
            # extract function/method declarations
            funcs = set()
            for m in re.finditer(r'\b([A-Z]\w+)\s*\(', text):
                funcs.add(m.group(1))
            for m in re.finditer(r'#define\s+(\w+)', text):
                funcs.add(m.group(1))
            if funcs:
                rel = os.path.relpath(path, sdk_dir)
                apis[rel] = sorted(funcs)
    return apis


def match_sdk_to_stubs(stubs, sdk_apis):
    """Cross-reference stub method names against SDK APIs.
    Returns {stub_class: [(method_name, sdk_header, sdk_api)]}."""
    matches = {}
    if not sdk_apis:
        return matches
    # build reverse index: api_name -> sdk_header
    api_index = {}
    for header, apis in sdk_apis.items():
        for api in apis:
            api_index[api.lower()] = header

    for stub in stubs:
        hits = []
        for method in stub['methods']:
            name = method['name'].lower()
            # try exact match, then prefix match
            if name in api_index:
                hits.append((method['name'], api_index[name], name))
            else:
                for api_name, header in api_index.items():
                    if name in api_name or api_name in name:
                        hits.append((method['name'], header, api_name))
                        break
        if hits:
            matches[stub['class_name']] = hits
    return matches
