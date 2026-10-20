# -*- coding: utf-8 -*-
"""
emit/ -- Module 4: IR + resolved types -> correct C++03, by construction.

Deliberately split into separate files so this never becomes another 10k-line
monolith:
  naming.py      identifier conventions (s_ statics, bare instance/value fields,
                 include-guard macros)
  header.py      render the .h (guard, includes, forward decls, class body)
  source.py      render the .cpp (static defs, ctors w/ safety, method defs); the bodies come
                 from the translator's back end (codebreaker/translate/backends/cpp03.py)

The emitter takes a TypeResolver (Module 3) and an optional Ledger (so every
PORT / GUESS / STUB is reported as it is emitted, not discovered later).
"""

from codebreaker.java.emit.header import render_header
from codebreaker.java.emit.source import render_source
from codebreaker.java.emit.std_transforms import apply_std_transforms

__all__ = ['render_header', 'render_source', 'apply_std_transforms']
