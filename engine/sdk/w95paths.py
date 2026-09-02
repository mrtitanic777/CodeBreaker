#!/usr/bin/env python3
r"""w95paths.py -- resolve the Win95 reconstruction tree + corpus locations.

Post-reorg the reconstructed **source tree** is the separate Windows95 repo (resolved from
`WT_WIN95_ROOT`, default a `Windows95` checkout beside this repo). The retail **input corpus**
and the period **toolchain** live under CodeBreaker's data home, configured in config.json as
`msc6_data` / `msc6_toolchain`. These replace the old hardcoded absolute paths
(`Windows95\tools\w95recon\WIN95\...`, `Windows95\data\files\...`, `Windows95\toolchain`).
"""
import os


def win95_root():
    """The reconstructed Win95 SOURCE tree (the Windows95 repo)."""
    r = os.environ.get("WT_WIN95_ROOT")
    if r and os.path.isdir(r):
        return r
    cb = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # CodeBreaker/
    return os.path.join(os.path.dirname(cb), "Windows95")                              # ../Windows95


def _cfg(key, default=""):
    try:
        from engine.foundation import config
        return config.load_config().get(key, default) or default
    except Exception:
        return default


def corpus_root():
    """The retail INPUT corpus (config: msc6_data); shipped DLLs live under <corpus>/files/."""
    return _cfg("msc6_data")


def toolchain_root():
    """The period MSC-6.0a toolchain (config: msc6_toolchain)."""
    return _cfg("msc6_toolchain")


def src(*parts):
    """A path inside the reconstructed source tree, e.g. src('base', 'lzexpand')."""
    return os.path.join(win95_root(), *parts)


def input_dll(*parts):
    """A retail input file under the corpus, e.g. input_dll('setup', 'inf', 'lzexpand.dll')."""
    return os.path.join(corpus_root(), "files", *parts)
