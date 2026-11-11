"""The .NET toolchain's discovery of the SDK on disk (translate/dotnet), on a made-up installation: no SDK needed."""
import os
import sys
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT)

from codebreaker.translate import dotnet          # noqa: E402


def _touch(path):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    open(path, "w").close()


def _install(root, tfms=("net9.0",), pack="9.0.0"):
    _touch(os.path.join(root, "dotnet.exe"))
    _touch(os.path.join(root, "sdk", "9.0.317", "Roslyn", "bincore", "csc.dll"))
    for tfm in tfms:
        _touch(os.path.join(root, "packs", "Microsoft.NETCore.App.Ref", pack, "ref", tfm, "System.Runtime.dll"))
    os.makedirs(os.path.join(root, "shared", "Microsoft.NETCore.App", "9.0.5"), exist_ok=True)
    return os.path.join(root, "dotnet.exe")


def _with(exe, fn):
    saved = os.environ.get("CB_DOTNET")
    os.environ["CB_DOTNET"] = exe
    try:
        return fn()
    finally:
        if saved is None:
            del os.environ["CB_DOTNET"]
        else:
            os.environ["CB_DOTNET"] = saved


def test_discovery_on_an_installation():
    with tempfile.TemporaryDirectory() as d:
        exe = _install(d)
        assert _with(exe, dotnet.exe) == exe
        assert _with(exe, dotnet.runtime) == ("9", "9.0.5")
        assert _with(exe, dotnet.roslyn).endswith(os.path.join("9.0.317", "Roslyn", "bincore"))
        assert _with(exe, dotnet.refs).endswith("net9.0")
        assert _with(exe, dotnet.available)


def test_the_newest_target_framework_is_chosen():
    # `refs` looked for a name starting with a digit among names that start with "net", so it never found one and took
    # whichever the folder listed first
    with tempfile.TemporaryDirectory() as d:
        exe = _install(d, tfms=("net8.0", "net10.0", "netstandard2.1", "net9.0"), pack="10.0.0")
        assert _with(exe, dotnet.refs).endswith("net10.0")
    with tempfile.TemporaryDirectory() as d:
        exe = _install(d, tfms=("net10.0", "net8.0", "net9.0"), pack="10.0.0")
        assert _with(exe, dotnet.refs).endswith("net10.0")
    with tempfile.TemporaryDirectory() as d:
        exe = _install(d, tfms=("net6.0", "net8.0"), pack="8.0.0")     # (the folder lists net6.0 first)
        assert _with(exe, dotnet.refs).endswith("net8.0")
    with tempfile.TemporaryDirectory() as d:
        exe = _install(d, tfms=("netstandard2.1",), pack="10.0.0")     # no net<N>: the first net* name
        assert _with(exe, dotnet.refs).endswith("netstandard2.1")
