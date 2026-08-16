# NOTICE

`j2ctool` is a source-to-source transpiler. It is licensed under the MIT License (see `LICENSE`)
and is the original work of its authors.

## What this project does NOT contain

This repository ships **no** third-party game code and **no** platform SDK code. Specifically,
it does not include, and must never include:

- **Decompiled game source** (e.g. a `net.minecraft.*` tree). The transpiler operates on Java
  source that **you supply**; none is bundled here.
- **Any game's compiled assets** (textures, audio, models). The asset tooling extracts these from
  files **you already own**; nothing is redistributed.
- **Console or platform SDK headers, libraries, or documentation** (e.g. the Xbox 360 XDK, or a
  Windows SDK). The tooling *detects* and *references* an SDK you have installed locally by API
  name only; no SDK content is vendored.

## Names and API references

The transpiler and its Minecraft/Xbox-360 profile reference third-party class names, API names,
and file names as data (in tables, comments, and configuration) so that it can recognize and
translate them. Names and APIs are facts, not creative expression; referencing them does not copy
any protected work. All translation *logic* and the hand-written platform-layer *implementations*
in `docs/platform/` are original.

## Your responsibilities

To actually convert a game, you must independently and lawfully obtain:

1. The Java source you want to transpile (e.g. via your own decompilation of software you own), and
2. Any target-platform SDK required to build the emitted C++.

You are responsible for complying with the licenses and terms of anything you feed into, or build
with, this tool.
