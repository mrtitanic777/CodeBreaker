# RenderEngine.h

Risk score: **5**  ·  7 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L44` (marker, `PORT`): determine element type */ m_field_1604_f;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1604_f;
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L28` (marker, `PORT`): was BufferedImage */ var1);
  ```cpp
  virtual int allocateAndSetupTexture(void* /* PORT: was BufferedImage */ var1);
  ```
- **⚪ LOW** `L29` (marker, `PORT`): was BufferedImage */ var1, int var2);
  ```cpp
  virtual void setupTexture(void* /* PORT: was BufferedImage */ var1, int var2);
  ```
- **⚪ LOW** `L42` (marker, `PORT`): was IntBuffer */ m_singleIntBuffer;
  ```cpp
  int* /* PORT: was IntBuffer */ m_singleIntBuffer;
  ```
- **⚪ LOW** `L43` (marker, `PORT`): was ByteBuffer */ imageData;
  ```cpp
  unsigned char* /* PORT: was ByteBuffer */ imageData;
  ```
- **⚪ LOW** `L51` (marker, `PORT`): was BufferedImage */ unwrapImageByColumns(void* /* PORT: was BufferedImage */ var1);
  ```cpp
  void* /* PORT: was BufferedImage */ unwrapImageByColumns(void* /* PORT: was BufferedImage */ var1);
  ```
- **⚪ LOW** `L54` (marker, `PORT`): was BufferedImage */ readTextureImage(InputStream* var1);
  ```cpp
  void* /* PORT: was BufferedImage */ readTextureImage(InputStream* var1);
  ```
