# GLAllocation.h

Risk score: **10**  ·  6 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L22` (marker, `PORT`): determine element type */ s_displayLists;
  ```cpp
  static std::vector<void*> /* PORT: determine element type */ s_displayLists;
  ```
- **🔴 HIGH** `L23` (marker, `PORT`): determine element type */ s_textureNames;
  ```cpp
  static std::vector<void*> /* PORT: determine element type */ s_textureNames;
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L15` (marker, `PORT`): was IntBuffer */ var0);
  ```cpp
  static void generateTextureNames(int* /* PORT: was IntBuffer */ var0);
  ```
- **⚪ LOW** `L17` (marker, `PORT`): was ByteBuffer */ createDirectByteBuffer(int var0);
  ```cpp
  static unsigned char* /* PORT: was ByteBuffer */ createDirectByteBuffer(int var0);
  ```
- **⚪ LOW** `L18` (marker, `PORT`): was IntBuffer */ createDirectIntBuffer(int var0);
  ```cpp
  static int* /* PORT: was IntBuffer */ createDirectIntBuffer(int var0);
  ```
- **⚪ LOW** `L19` (marker, `PORT`): was FloatBuffer */ createDirectFloatBuffer(int var0);
  ```cpp
  static float* /* PORT: was FloatBuffer */ createDirectFloatBuffer(int var0);
  ```
