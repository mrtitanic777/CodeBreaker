# -*- coding: utf-8 -*-
"""RetroMCP decompiler backend.

J2CTool bundles the RetroMCP CLI (MIT, MCPHackers; Vineflower engine) as a headless backend so a
Minecraft version can be decompiled straight into Java source and fed into the transpiler -- no manual
round-trip through the RetroMCP GUI. The decompile step needs a Java runtime and network access
(RetroMCP downloads the version + mappings on first setup). The C++ transpiler itself needs neither.

Pipeline, run inside a workspace directory:
    java -jar retromcp-cli.jar setup setup=<version>     # download + prepare the workspace
    java -jar retromcp-cli.jar decompile                 # decompile -> <workspace>/minecraft_*/src
"""

import os
import shutil
import subprocess

from ._paths import data_path


class DecompileError(Exception):
    pass


def backend_jar():
    """Path to the bundled RetroMCP CLI jar."""
    return data_path('backends', 'retromcp-cli.jar')


def find_java(explicit=None):
    """Locate a `java` executable. Order: explicit path, $JAVA_HOME/bin/java, PATH. Returns path or None."""
    exe = 'java.exe' if os.name == 'nt' else 'java'
    if explicit and os.path.isfile(explicit):
        return explicit
    jh = os.environ.get('JAVA_HOME')
    if jh:
        cand = os.path.join(jh, 'bin', exe)
        if os.path.isfile(cand):
            return cand
    return shutil.which('java')


def available():
    """True if both a Java runtime and the bundled backend jar are present."""
    return bool(find_java()) and os.path.isfile(backend_jar())


def _find_src(workspace):
    """Locate the decompiled source dir: the `src` directory holding the most `.java` files."""
    best, best_n = None, 0
    for root, _dirs, files in os.walk(workspace):
        if os.path.basename(root) == 'src':
            n = sum(1 for _r, _d, fs in os.walk(root) for f in fs if f.endswith('.java'))
            if n > best_n:
                best, best_n = root, n
    return best if best_n > 0 else None


def _run(java, jar, args, cwd, on_output=None):
    cmd = [java, '-jar', jar] + args
    if on_output:
        on_output('$ ' + ' '.join(['java', '-jar', os.path.basename(jar)] + args))
    proc = subprocess.Popen(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            universal_newlines=True, bufsize=1)
    for line in proc.stdout:
        if on_output:
            on_output(line.rstrip('\n'))
    proc.wait()
    return proc.returncode


def decompile(version, workspace, java=None, on_output=None, setup=True):
    """Decompile a Minecraft ``version`` (e.g. ``"a1.2.6"``, ``"c0.30_01c"``) into ``workspace``.

    Returns the absolute path to the decompiled Java source directory, suitable as the ``java_dir``
    input to :func:`j2ctool.pipeline.convert`. Raises :class:`DecompileError` on any failure.

    Set ``setup=False`` to skip the download/setup step (re-decompile an already-set-up workspace).
    """
    java = find_java(java)
    if not java:
        raise DecompileError(
            "Java runtime not found. Install a JRE/JDK (8+) and put `java` on PATH or set JAVA_HOME. "
            "Only the decompile backend needs Java; the C++ transpiler does not.")
    jar = backend_jar()
    if not os.path.isfile(jar):
        raise DecompileError("Bundled RetroMCP backend jar not found at: %s" % jar)

    workspace = os.path.abspath(workspace)
    os.makedirs(workspace, exist_ok=True)

    if setup:
        if on_output:
            on_output("[decompile] setting up Minecraft %s (downloads version + mappings)..." % version)
        rc = _run(java, jar, ['setup', 'setup=%s' % version], workspace, on_output)
        if rc != 0:
            raise DecompileError("RetroMCP setup failed for version '%s' (exit code %d)." % (version, rc))

    if on_output:
        on_output("[decompile] decompiling with Vineflower...")
    rc = _run(java, jar, ['decompile'], workspace, on_output)
    if rc != 0:
        raise DecompileError("RetroMCP decompile failed (exit code %d)." % rc)

    src = _find_src(workspace)
    if not src:
        raise DecompileError("Decompile produced no Java source under %s." % workspace)
    if on_output:
        n = sum(1 for _r, _d, fs in os.walk(src) for f in fs if f.endswith('.java'))
        on_output("[decompile] done: %d .java files at %s" % (n, src))
    return src
