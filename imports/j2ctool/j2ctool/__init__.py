# -*- coding: utf-8 -*-
"""
j2ctool -- a deterministic Java -> C++03 source-to-source transpiler.

A generic transpiler core (parse -> IR -> type/ownership resolution -> emit -> pipeline) with a
crash-class rule engine and a structured event ledger, plus a Minecraft/Xbox-360 *profile* that
layers game- and platform-specific tooling (asset extraction, audio banks, achievements, platform
reference layer, project scaffolding) on top.

Core modules:
  ir        -- the intermediate representation (JClass/Method/Field/JType)
  parse     -- Java source -> IR (tuned for decompiled, MCP-style Java)
  types     -- type + ownership resolution (value vs pointer -> `.` vs `->`)
  emit/     -- header/source/statement emission (the C++ back-end)
  pipeline  -- end-to-end orchestration: discover -> parse -> resolve -> emit -> verify -> report
  rules     -- structural crash-class detectors run over emitted C++
  ledger / report -- the event model and its Markdown/JSON renderers
  cli / gui -- command-line dispatch and the PySide6 desktop front-end (default entry point)
"""

__version__ = '0.1.0'
