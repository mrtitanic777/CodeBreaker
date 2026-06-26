# World.h

Risk score: **62**  ·  23 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L48` (marker, `PORT`): determine element type */ m_loadedEntityList;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_loadedEntityList;
  ```
- **🔴 HIGH** `L49` (marker, `PORT`): determine element type */ m_loadedTileEntityList;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_loadedTileEntityList;
  ```
- **🔴 HIGH** `L50` (marker, `PORT`): determine element type */ m_playerEntities;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_playerEntities;
  ```
- **🔴 HIGH** `L152` (marker, `PORT`): determine element type */ getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2);
  ```cpp
  virtual std::vector<void*> /* PORT: determine element type */ getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2);
  ```
- **🔴 HIGH** `L153` (marker, `PORT`): determine element type */ getEntitiesWithinAABB(void* var1, AxisAlignedBB var2);
  ```cpp
  virtual std::vector<void*> /* PORT: determine element type */ getEntitiesWithinAABB(void* var1, AxisAlignedBB var2);
  ```
- **🔴 HIGH** `L154` (marker, `PORT`): determine element type */ func_658_i();
  ```cpp
  virtual std::vector<void*> /* PORT: determine element type */ func_658_i();
  ```
- **🔴 HIGH** `L157` (marker, `PORT`): determine element type */ var1);
  ```cpp
  virtual void func_636_a(std::vector<void*> /* PORT: determine element type */ var1);
  ```
- **🔴 HIGH** `L158` (marker, `PORT`): determine element type */ var1);
  ```cpp
  virtual void func_632_b(std::vector<void*> /* PORT: determine element type */ var1);
  ```
- **🔴 HIGH** `L191` (marker, `PORT`): determine element type */ m_field_1051_z;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1051_z;
  ```
- **🔴 HIGH** `L192` (marker, `PORT`): determine element type */ m_field_1024_A;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1024_A;
  ```
- **🔴 HIGH** `L204` (marker, `PORT`): determine element type */ m_field_1012_M;
  ```cpp
  std::vector<void*> /* PORT: determine element type */ m_field_1012_M;
  ```

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L13` (marker, `PORT`): m_field_9428_I is now std::vector<AxisAlignedBB> (value member needs complete type)
  ```cpp
  #include "AxisAlignedBB.h"  // PORT: m_field_9428_I is now std::vector<AxisAlignedBB> (value member needs complete type)
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L40` (marker, `PORT`): was java.io.File */* var1, const std::string& var2);
  ```cpp
  World(std::string /* PORT: was java.io.File */* var1, const std::string& var2);
  ```
- **⚪ LOW** `L43` (marker, `PORT`): was java.io.File */* var1, const std::string& var2, long long var3);
  ```cpp
  World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3);
  ```
- **⚪ LOW** `L44` (marker, `PORT`): was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5);
  ```cpp
  World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5);
  ```
- **⚪ LOW** `L61` (marker, `PORT`): was java.io.File */* m_field_9433_s;
  ```cpp
  std::string /* PORT: was java.io.File */* m_field_9433_s;
  ```
- **⚪ LOW** `L62` (marker, `PORT`): was java.io.File */* m_field_9432_t;
  ```cpp
  std::string /* PORT: was java.io.File */* m_field_9432_t;
  ```
- **⚪ LOW** `L69` (marker, `PORT`): was java.io.File */* var0, const std::string& var1);
  ```cpp
  static NBTTagCompound* func_629_a(std::string /* PORT: was java.io.File */* var0, const std::string& var1);
  ```
- **⚪ LOW** `L70` (marker, `PORT`): was java.io.File */* var0, const std::string& var1);
  ```cpp
  static void deleteWorld(std::string /* PORT: was java.io.File */* var0, const std::string& var1);
  ```
- **⚪ LOW** `L184` (marker, `PORT`): was java.io.File */* var1);
  ```cpp
  virtual IChunkProvider* func_4081_a(std::string /* PORT: was java.io.File */* var1);
  ```
- **⚪ LOW** `L199` (marker, `PORT`): scratch list for Block::getCollidingBoundingBoxes (value AABBs)
  ```cpp
  std::vector<AxisAlignedBB> m_field_9428_I;  // PORT: scratch list for Block::getCollidingBoundingBoxes (value AABBs)
  ```
- **⚪ LOW** `L206` (marker, `PORT`): was java.io.File */* var0);
  ```cpp
  void deleteFiles(std::string /* PORT: was java.io.File */* var0);
  ```
