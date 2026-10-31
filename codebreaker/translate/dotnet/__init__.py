"""dotnet -- the installed .NET SDK as a toolchain: compile C# with the SDK's own Roslyn (csc.dll), run the
result on the installed runtime. No project file, no NuGet restore, nothing downloaded: the compiler is
given the reference assemblies of the SDK's targeting pack by path, and the program gets a
runtimeconfig.json naming the installed Microsoft.NETCore.App.

    from codebreaker.translate import dotnet
    ok, log = dotnet.compile(['Prog.cs'], 'out/Prog.dll')       # an exe (Main) by default
    code, out, err = dotnet.run('out/Prog.dll')

The SDK is found from CB_DOTNET (dotnet.exe), then PATH, then the standard install folder.
"""
import os
import re
import shutil
import subprocess

TIMEOUT = 300
_STD = (r'C:\Program Files\dotnet\dotnet.exe', '/usr/share/dotnet/dotnet', '/usr/local/share/dotnet/dotnet')


def _ver(s):
    return tuple(int(x) if x.isdigit() else 0 for x in re.split(r'[.\-]', s)[:3])


def exe():
    """dotnet.exe, or None."""
    p = os.environ.get('CB_DOTNET')
    if p and os.path.isfile(p):
        return p
    p = shutil.which('dotnet')
    if p:
        return p
    return next((s for s in _STD if os.path.isfile(s)), None)


def home():
    e = exe()
    return os.path.dirname(os.path.realpath(e)) if e else None


def _newest(folder, prefix=''):
    try:
        vs = [v for v in os.listdir(folder) if v[:1].isdigit() and v.startswith(prefix)]
    except OSError:
        return None
    return max(vs, key=_ver) if vs else None


def sdk():
    """The newest SDK folder (it holds Roslyn), or None."""
    h = home()
    v = _newest(os.path.join(h, 'sdk')) if h else None
    return os.path.join(h, 'sdk', v) if v else None


def roslyn():
    """The folder of the SDK's compiler: csc.dll and Microsoft.CodeAnalysis*.dll."""
    s = sdk()
    p = os.path.join(s, 'Roslyn', 'bincore') if s else None
    return p if p and os.path.isfile(os.path.join(p, 'csc.dll')) else None


def runtime():
    """(major, full version) of the installed Microsoft.NETCore.App the SDK's targeting pack matches."""
    h = home()
    ref = _newest(os.path.join(h, 'packs', 'Microsoft.NETCore.App.Ref')) if h else None
    if not ref:
        return None, None
    major = ref.split('.')[0]
    run = _newest(os.path.join(h, 'shared', 'Microsoft.NETCore.App'), major + '.')
    return major, run or ref


def refs():
    """The targeting pack's reference assemblies folder (System.Runtime.dll, System.Console.dll, ...)."""
    h = home()
    ref = _newest(os.path.join(h, 'packs', 'Microsoft.NETCore.App.Ref')) if h else None
    if not ref:
        return None
    base = os.path.join(h, 'packs', 'Microsoft.NETCore.App.Ref', ref, 'ref')
    tfm = _newest(base, 'net') if os.path.isdir(base) else None
    if tfm is None and os.path.isdir(base):
        tfm = next((d for d in os.listdir(base) if d.startswith('net')), None)
    return os.path.join(base, tfm) if tfm else None


def available():
    return bool(exe() and roslyn() and refs())


def env(tmp=None):
    e = dict(os.environ, DOTNET_CLI_TELEMETRY_OPTOUT='1', DOTNET_NOLOGO='1', DOTNET_SKIP_FIRST_TIME_EXPERIENCE='1',
             DOTNET_CLI_UI_LANGUAGE='en', DOTNET_gcServer='0')
    if tmp:
        os.makedirs(tmp, exist_ok=True)
        e['TEMP'] = e['TMP'] = tmp
    return e


def runtimeconfig(dll):
    """Write <dll>.runtimeconfig.json beside a compiled program: run on the installed runtime."""
    major, full = runtime()
    path = dll[:-4] + '.runtimeconfig.json'
    with open(path, 'w', encoding='utf-8') as f:
        f.write('{"runtimeOptions": {"tfm": "net%s.0", "framework": {"name": "Microsoft.NETCore.App", '
                '"version": "%s"}, "rollForward": "LatestPatch"}}\n' % (major, full))
    return path


def compile(sources, out_dll, references=(), target='exe', nullable=True, unsafe=False, warn=False, tmp=None, main=None):
    """Compile C# sources to out_dll with the SDK's csc. -> (ok, compiler output)."""
    os.makedirs(os.path.dirname(os.path.abspath(out_dll)), exist_ok=True)
    rdir = refs()
    rsp = out_dll[:-4] + '.rsp'
    args = ['-nologo', '-nostdlib+', '-target:' + target, '-out:' + out_dll, '-langversion:latest',
            '-deterministic', '-debug-', '-optimize-', '-utf8output', '-checked-']
    if nullable:
        args.append('-nullable:enable')
    if unsafe:
        args.append('-unsafe+')
    if main:
        args.append('-main:' + main)
    if not warn:
        args.append('-nowarn:CS8600,CS8601,CS8602,CS8603,CS8604,CS8618,CS8625,CS0168,CS0219,CS0162,CS8321,CS0649,CS0169')
    args += ['-r:' + os.path.join(rdir, f) for f in sorted(os.listdir(rdir)) if f.endswith('.dll')]
    args += ['-r:' + r for r in references]
    args += [os.path.abspath(s) for s in sources]
    with open(rsp, 'w', encoding='utf-8') as f:
        f.write('\n'.join('"%s"' % a if ' ' in a else a for a in args))
    r = subprocess.run([exe(), os.path.join(roslyn(), 'csc.dll'), '-noconfig', '@' + rsp], stdout=subprocess.PIPE,
                       stderr=subprocess.STDOUT, timeout=TIMEOUT, env=env(tmp))
    os.remove(rsp)
    log = r.stdout.decode('utf-8', 'replace')
    ok = r.returncode == 0 and os.path.isfile(out_dll)
    if ok and target == 'exe':
        runtimeconfig(out_dll)
    return ok, log


def run(dll, args=(), timeout=60, stdin=None, tmp=None):
    """Run a compiled program on the installed runtime. -> (exit code, stdout, stderr); a timeout is
    exit code None."""
    try:
        r = subprocess.run([exe(), dll] + list(args), input=stdin, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                           timeout=timeout, env=env(tmp))
    except subprocess.TimeoutExpired:
        return None, '', 'timeout'
    return r.returncode, r.stdout.decode('utf-8', 'replace'), r.stderr.decode('utf-8', 'replace')
