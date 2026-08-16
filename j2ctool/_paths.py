# -*- coding: utf-8 -*-
"""Resource path resolution that works both from source and from a PyInstaller build.

Two distinct roots:
  * data_path()   -- bundled READ-ONLY data shipped WITH the tool (docs/platform/**, profiles/**).
                     In a frozen build these are unpacked under sys._MEIPASS; from source they sit
                     at the repo root (the parent of this package).
  * output_root() -- a WRITABLE base for generated output / user files. Next to the executable when
                     frozen (never _MEIPASS, which is a read-only temp dir wiped on exit); repo root
                     from source.
"""

import os
import sys

# Repo root when running from source = parent of the j2ctool package directory.
_SOURCE_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def _frozen():
    return getattr(sys, 'frozen', False)


def data_path(*parts):
    """Absolute path to a bundled read-only data file/dir (e.g. data_path('docs','platform','canonical'))."""
    if _frozen():
        base = getattr(sys, '_MEIPASS', None) or os.path.dirname(sys.executable)
    else:
        base = _SOURCE_ROOT
    return os.path.join(base, *parts)


def output_root():
    """Writable base directory for generated output. Next to the exe when frozen; repo root from source."""
    if _frozen():
        return os.path.dirname(sys.executable)
    return _SOURCE_ROOT
