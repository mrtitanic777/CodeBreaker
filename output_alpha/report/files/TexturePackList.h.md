# TexturePackList.h

Risk score: **10**  ·  4 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L24` (marker, `PORT`): determine element type */ availableTexturePacks();
  ```cpp
  virtual std::vector<void*> /* PORT: determine element type */ availableTexturePacks();
  ```
- **🔴 HIGH** `L27` (marker, `PORT`): determine element type */ m_availableTexturePacks;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_availableTexturePacks;
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L17` (marker, `PORT`): was java.io.File */* var2);
  ```cpp
  TexturePackList(Minecraft* var1, std::string /* PORT: was java.io.File */* var2);
  ```
- **⚪ LOW** `L31` (marker, `PORT`): was java.io.File */* m_texturePackDir;
  ```cpp
  std::string /* PORT: was java.io.File */* m_texturePackDir;
  ```
