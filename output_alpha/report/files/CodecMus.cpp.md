# CodecMus.cpp

Risk score: **2**  ·  3 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): dead audio code. CodecMus extends CodecJOrbis (a paulscode Ogg Vorbis
  ```cpp
  // PORT: dead audio code. CodecMus extends CodecJOrbis (a paulscode Ogg Vorbis
  ```
- **⚪ LOW** `L24` (marker, `PORT`): 360 uses XACT; no Java InputStream / MusInputStream backing.
  ```cpp
  // PORT: 360 uses XACT; no Java InputStream / MusInputStream backing.
  ```
