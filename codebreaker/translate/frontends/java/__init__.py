"""The Java front end, step one: javac's typed tree as JSON.

TreeDump.java (beside this file) runs javac over a source tree -- parse and attribute, no class files --
and writes every node of every compilation unit with what javac resolved: each expression's type, the
declaration each name, call and `new` refers to (with the overload chosen). `dump` compiles TreeDump
once (cached under the state folder, keyed by its source and the JDK) and runs it.

    from codebreaker.translate.frontends import java
    units = java.dump(src_dir, out_dir, classpath=[...jars])    # {source path relative to src: tree}
    java.diagnostics(out_dir)                                   # javac's errors and warnings

A node is a dict: "k" its kind (javac's Tree.Kind name: METHOD_INVOCATION, IF, PLUS, ...), "l"/"c" its
line and column, "$type" the resolved type of an expression or declaration, "$sym" the element a name
refers to ({"kind", "name", "owner", "static", "ret", "params", ...}), and its children under javac's
property names ("methodSelect", "arguments", "condition", "thenStatement", ...).
"""
import hashlib
import json
import os
import shutil
import subprocess

_HERE = os.path.dirname(os.path.abspath(__file__))
_SOURCE = os.path.join(_HERE, 'TreeDump.java')
TIMEOUT = 1800


def _jdk():
    from codebreaker.verify.lanes import java as _vj
    javac, java = _vj.jdk()
    if not javac:
        raise RuntimeError('the Java front end needs a JDK (settings.json: java)')
    return javac, java


def tool(javac=None):
    """The folder holding the compiled TreeDump, compiling it when its source or the JDK changed."""
    from codebreaker.base import cache, settings
    if javac is None:
        javac = _jdk()[0]
    with open(_SOURCE, 'rb') as f:
        key = hashlib.sha1(f.read() + repr(cache.tool(javac)).encode()).hexdigest()[:12]
    folder = settings.state('translate', 'treedump-' + key)
    if not os.path.isfile(os.path.join(folder, 'TreeDump.class')):
        os.makedirs(folder, exist_ok=True)
        r = subprocess.run([javac, '-nowarn', '-encoding', 'UTF-8', '-d', folder, _SOURCE],
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=300)
        if r.returncode != 0:
            shutil.rmtree(folder, ignore_errors=True)
            raise RuntimeError('TreeDump did not compile: ' + r.stdout.decode('utf-8', 'replace')[:400])
    return folder


def sources(src_dir):
    """Every .java file under src_dir, sorted."""
    out = []
    for dp, _dirs, files in os.walk(src_dir):
        out += [os.path.join(dp, f) for f in files if f.endswith('.java')]
    return sorted(out)


def run(src_dir, out_dir, classpath=(), files=None, release=None):
    """Dump src_dir's compilation units (or just `files`) into out_dir. -> how many were dumped."""
    javac, java = _jdk()
    folder = tool(javac)
    files = list(files) if files is not None else sources(src_dir)
    if os.path.isdir(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)
    listing = os.path.join(out_dir, '_files.txt')
    with open(listing, 'w', encoding='utf-8') as f:
        f.write('\n'.join(files))
    cmd = [java, '-Xss16m', '-cp', folder, 'TreeDump', '--out', out_dir, '--sourcepath', src_dir]
    if classpath:
        cmd += ['--cp', os.pathsep.join(classpath)]
    if release:
        cmd += ['--release', str(release)]
    r = subprocess.run(cmd + ['@' + listing], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=TIMEOUT)
    if r.returncode != 0:
        raise RuntimeError('TreeDump failed: ' + r.stderr.decode('utf-8', 'replace')[-600:])
    out = r.stdout.decode('utf-8', 'replace')
    return int(out.split()[1]) if out.startswith('dumped ') else 0


def load(out_dir):
    """{path relative to out_dir without .json: tree} for every dumped unit."""
    units = {}
    for dp, _dirs, files in os.walk(out_dir):
        for f in files:
            if f.endswith('.json') and not f.startswith('_'):
                p = os.path.join(dp, f)
                with open(p, encoding='utf-8') as fh:
                    units[os.path.relpath(p, out_dir)[:-5].replace('\\', '/')] = json.load(fh)
    return units


def diagnostics(out_dir):
    with open(os.path.join(out_dir, '_diagnostics.json'), encoding='utf-8') as f:
        return json.load(f)


def dump(src_dir, out_dir, classpath=(), files=None, release=None):
    """run + load."""
    run(src_dir, out_dir, classpath, files, release)
    return load(out_dir)
