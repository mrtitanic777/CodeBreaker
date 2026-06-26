# -*- coding: utf-8 -*-
"""
j2x -- modular successor to the monolithic java2xbox360.py converter.

This package is being grown incrementally *around* the existing converter
(see RESTRUCTURE_PLAN.md). Phase 1 (current) is the reporting layer: it reads
the artifacts the converter already produces and turns them into a structured,
per-file LEDGER so you can see exactly what was ported, stubbed, guessed, or
left unresolved -- without changing a single byte of converter output.

Modules:
  ledger   -- Event / Category / Severity / Ledger (the data model)
  analyze  -- post-hoc analyzers that build a Ledger from existing artifacts
  report   -- render index.md / <File>.md / report.json from a Ledger
  cli      -- `python -m j2x report <output_dir>` entry point
"""

__version__ = '0.1.0'
