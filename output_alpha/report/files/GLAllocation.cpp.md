# GLAllocation.cpp

Risk score: **23**  ·  12 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L33` (marker, `PORT`): D3D9 has no display lists. Hand back a dummy base handle and reserve
  ```cpp
  // PORT: D3D9 has no display lists. Hand back a dummy base handle and reserve
  ```
- **🔴 HIGH** `L45` (marker, `PORT`): glGenTextures has no D3D9 equivalent here; texture creation/registry is
  ```cpp
  // PORT: glGenTextures has no D3D9 equivalent here; texture creation/registry is
  ```
- **🔴 HIGH** `L52` (marker, `PORT`): no glDeleteLists / glDeleteTextures on D3D9 (resources are released by
  ```cpp
  // PORT: no glDeleteLists / glDeleteTextures on D3D9 (resources are released by
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L63` (rule, `uninit-array`): new unsigned char[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new unsigned char[N]() (the new bool[10] phantom-input bug).
  ```cpp
  return new unsigned char[var0];
  ```
- **🟡 MED** `L71` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  return new int[var0];
  ```
- **🟡 MED** `L79` (rule, `uninit-array`): new float[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new float[N]() (the new bool[10] phantom-input bug).
  ```cpp
  return new float[var0];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GLAllocation.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GLAllocation.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L44` (marker, `PORT`): was IntBuffer */ var0) {
  ```cpp
  void GLAllocation::generateTextureNames(int* /* PORT: was IntBuffer */ var0) {
  ```
- **⚪ LOW** `L58` (marker, `PORT`): was ByteBuffer */ GLAllocation::createDirectByteBuffer(int var0) {
  ```cpp
  unsigned char* /* PORT: was ByteBuffer */ GLAllocation::createDirectByteBuffer(int var0) {
  ```
- **⚪ LOW** `L66` (marker, `PORT`): was IntBuffer */ GLAllocation::createDirectIntBuffer(int var0) {
  ```cpp
  int* /* PORT: was IntBuffer */ GLAllocation::createDirectIntBuffer(int var0) {
  ```
- **⚪ LOW** `L74` (marker, `PORT`): was FloatBuffer */ GLAllocation::createDirectFloatBuffer(int var0) {
  ```cpp
  float* /* PORT: was FloatBuffer */ GLAllocation::createDirectFloatBuffer(int var0) {
  ```
