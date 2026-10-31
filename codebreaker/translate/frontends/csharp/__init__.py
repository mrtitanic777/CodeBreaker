"""The C# front end, step one: Roslyn's bound tree as JSON.

CsTreeDump.cs (beside this file) runs the C# compiler of the installed .NET SDK over a set of files --
parse and bind, no assembly written -- and writes each file's declarations, and every body and
initialiser as Roslyn's IOperation tree, with what the compiler resolved: each expression's type, the
symbol each name, call and `new` refers to (the overload chosen, its arguments in parameter order), each
implicit conversion. `tool` compiles CsTreeDump once with the SDK's own csc.dll against the SDK's
Microsoft.CodeAnalysis assemblies, by path -- no project, no NuGet restore, nothing downloaded -- cached
under the state folder, keyed by its source and the SDK.

    from codebreaker.translate.frontends import csharp
    units = csharp.dump(['Prog.cs'], out_dir)      # {file name without .cs: tree}
    csharp.diagnostics(out_dir)                    # the compiler's errors and warnings
    csharp.units(['Prog.cs'])                      # dump + lower: [ir.Unit]

A declaration is a dict with "k" (type, field, property, indexer, method, ctor, operator, conversion,
enummember, ...); an operation a dict with "k" its OperationKind (Invocation, Binary, Loop, ...), "l"/"c"
its line and column, "$type" its type, and its children under the IOperation property names
("targetMethod", "instance", "arguments", "leftOperand", ...). A symbol is {"kind", "name", "owner",
"static", ...}; a type {"k": "prim"|"named"|"array"|"tv"|..., ...}.
"""
import hashlib
import json
import os
import shutil
import subprocess

_HERE = os.path.dirname(os.path.abspath(__file__))
_SOURCE = os.path.join(_HERE, 'CsTreeDump.cs')
_ROSLYN = ('Microsoft.CodeAnalysis.dll', 'Microsoft.CodeAnalysis.CSharp.dll')
TIMEOUT = 900


def _dotnet():
    from codebreaker.translate import dotnet
    if not dotnet.available():
        raise RuntimeError('the C# front end needs the .NET SDK (dotnet.exe with Roslyn and the targeting pack)')
    return dotnet


def _tmp():
    from codebreaker.base import settings
    return settings.state('translate', 'tmp')


def tool():
    """CsTreeDump.dll, compiled when its source or the SDK changed."""
    from codebreaker.base import cache, settings
    dn = _dotnet()
    ros = dn.roslyn()
    with open(_SOURCE, 'rb') as f:
        key = hashlib.sha1(f.read() + repr([cache.tool(os.path.join(ros, n)) for n in _ROSLYN + ('csc.dll',)]).encode()
                           + repr(dn.runtime()).encode()).hexdigest()[:12]
    folder = settings.state('translate', 'csdump-' + key)
    dll = os.path.join(folder, 'CsTreeDump.dll')
    if not os.path.isfile(dll):
        os.makedirs(folder, exist_ok=True)
        for n in _ROSLYN:                                 # beside the tool, as the runtime loads them
            shutil.copy2(os.path.join(ros, n), folder)
        ok, log = dn.compile([_SOURCE], dll, references=[os.path.join(folder, n) for n in _ROSLYN], nullable=False,
                             tmp=_tmp())
        if not ok:
            shutil.rmtree(folder, ignore_errors=True)
            raise RuntimeError('CsTreeDump did not compile: ' + log[-600:])
        for old in os.listdir(os.path.dirname(folder)):     # (those of another source or SDK: never used again)
            if old.startswith('csdump-') and old != os.path.basename(folder):
                shutil.rmtree(os.path.join(os.path.dirname(folder), old), ignore_errors=True)
    return dll


def run(files, out_dir, langversion=None):
    """Dump the C# files into out_dir. -> how many were dumped."""
    dn = _dotnet()
    dll = tool()
    if os.path.isdir(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)
    listing = os.path.join(out_dir, '_files.txt')
    with open(listing, 'w', encoding='utf-8') as f:
        f.write('\n'.join(os.path.abspath(p) for p in files))
    cmd = [dn.exe(), dll, '--out', out_dir, '--refs', dn.refs()]
    if langversion:
        cmd += ['--langversion', str(langversion)]
    r = subprocess.run(cmd + ['@' + listing], stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=TIMEOUT,
                       env=dn.env(_tmp()))
    if r.returncode != 0:
        raise RuntimeError('CsTreeDump failed: ' + r.stderr.decode('utf-8', 'replace')[-800:])
    out = r.stdout.decode('utf-8', 'replace')
    return int(out.split()[1]) if out.startswith('dumped ') else 0


def load(out_dir):
    """{file name without .cs: tree} for every dumped file."""
    units = {}
    for f in sorted(os.listdir(out_dir)):
        if f.endswith('.json') and not f.startswith('_'):
            with open(os.path.join(out_dir, f), encoding='utf-8') as fh:
                units[f[:-5]] = json.load(fh)
    return units


def diagnostics(out_dir):
    with open(os.path.join(out_dir, '_diagnostics.json'), encoding='utf-8') as f:
        return json.load(f)


def errors(out_dir):
    return [d for d in diagnostics(out_dir) if d['kind'] == 'ERROR']


def dump(files, out_dir, langversion=None):
    """run + load."""
    run(files, out_dir, langversion)
    return load(out_dir)


def sources(src):
    """Every .cs file under a folder (or the one file), sorted."""
    if os.path.isfile(src):
        return [src]
    out = []
    for dp, _dirs, fs in os.walk(src):
        out += [os.path.join(dp, f) for f in fs if f.endswith('.cs')]
    return sorted(out)


def units(files, out_dir=None):
    """Dump and lower C# files. -> [ir.Unit] (raises on a compile error: the front end needs bound code)."""
    import tempfile
    from codebreaker.translate.frontends.csharp import lower as L
    own = out_dir is None
    if own:
        out_dir = tempfile.mkdtemp(prefix='csdump-', dir=_tmp())
    try:
        trees = dump(files, out_dir)
        errs = errors(out_dir)
        if errs:
            raise RuntimeError('C# did not compile: %s:%d %s' % (errs[0]['file'], errs[0]['line'], errs[0]['message']))
        return [L.lower(t, n) for n, t in sorted(trees.items())]
    finally:
        if own:
            shutil.rmtree(out_dir, ignore_errors=True)
