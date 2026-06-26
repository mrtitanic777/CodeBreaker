# CompressedStreamTools.cpp

Risk score: **4**  ·  7 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L68` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  unsigned char* var1 = new unsigned char[var3];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): java.util.zip GZIPInputStream/GZIPOutputStream have no portable
  ```cpp
  // PORT: java.util.zip GZIPInputStream/GZIPOutputStream have no portable
  ```
- **⚪ LOW** `L29` (marker, `PORT`): InputStream is an empty stub on Xbox 360 (no in-memory data backing).
  ```cpp
  // PORT: InputStream is an empty stub on Xbox 360 (no in-memory data backing).
  ```
- **⚪ LOW** `L38` (marker, `PORT`): OutputStream is an empty stub on Xbox 360. The gzip+stream write path
  ```cpp
  // PORT: OutputStream is an empty stub on Xbox 360. The gzip+stream write path
  ```
- **⚪ LOW** `L45` (marker, `PORT`): read an NBT compound from a raw byte buffer. The Java version gunzipped
  ```cpp
  // PORT: read an NBT compound from a raw byte buffer. The Java version gunzipped
  ```
- **⚪ LOW** `L61` (marker, `PORT`): serialize an NBT compound to a freshly allocated raw byte buffer
  ```cpp
  // PORT: serialize an NBT compound to a freshly allocated raw byte buffer
  ```
