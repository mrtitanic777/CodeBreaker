# SoundPool.h

Risk score: **5**  ·  2 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L29` (marker, `PORT`): determine element type */ m_allSoundPoolEntries;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_allSoundPoolEntries;
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L22` (marker, `PORT`): was java.io.File */* var2);
  ```cpp
  virtual SoundPoolEntry addSound(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
  ```
