# World.cpp

Risk score: **68**  ·  38 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L1925` (marker, `PORT`): determine element type */ World::getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2) {
  ```cpp
  std::vector<void*> /* PORT: determine element type */ World::getEntitiesWithinAABBExcludingEntity(Entity* var1, AxisAlignedBB var2) {
  ```
- **🔴 HIGH** `L1943` (marker, `PORT`): determine element type */ World::getEntitiesWithinAABB(void* var1, AxisAlignedBB var2) {
  ```cpp
  std::vector<void*> /* PORT: determine element type */ World::getEntitiesWithinAABB(void* var1, AxisAlignedBB var2) {
  ```
- **🔴 HIGH** `L1948` (marker, `PORT`): determine element type */;
  ```cpp
  std::vector<void*> var7 /* PORT: determine element type */;
  ```
- **🔴 HIGH** `L1961` (marker, `PORT`): determine element type */ World::func_658_i() {
  ```cpp
  std::vector<void*> /* PORT: determine element type */ World::func_658_i() {
  ```
- **🔴 HIGH** `L1989` (marker, `PORT`): determine element type */ var1) {
  ```cpp
  void World::func_636_a(std::vector<void*> /* PORT: determine element type */ var1) {
  ```
- **🔴 HIGH** `L1998` (marker, `PORT`): determine element type */ var1) {
  ```cpp
  void World::func_632_b(std::vector<void*> /* PORT: determine element type */ var1) {
  ```

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L100` (rule, `ctor-stub`): constructor World(std::string /* PORT: was java.io.File */* var1, const std::s) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  World(std::string /* PORT: was java.io.File */* var1, const std::string& var2) {...}
  ```
- **🔴 HIGH** `L277` (rule, `ctor-stub`): constructor World(std::string /* PORT: was java.io.File */* var1, const std::s) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3) {...}
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L135` (rule, `rule-of-three`): World owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L622` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1232` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1266` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1274` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1850` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1899` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1919` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L2052` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L2064` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## WARNING — suspicious; verify by hand

- **🟡 MED** `L471` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L2138` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```
- **🟡 MED** `L2174` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L100` (marker, `PORT`): was java.io.File */* var1, const std::string& var2)
  ```cpp
  World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2)
  ```
- **⚪ LOW** `L132` (marker, `PORT`): constructor delegation  // was: this(var1, var2, (new Random())->nextLong());
  ```cpp
  // PORT: constructor delegation  // was: this(var1, var2, (new Random())->nextLong());
  ```
- **⚪ LOW** `L277` (marker, `PORT`): was java.io.File */* var1, const std::string& var2, long long var3)
  ```cpp
  World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3)
  ```
- **⚪ LOW** `L309` (marker, `PORT`): constructor delegation  // was: this(var1, var2, var3, NULL);
  ```cpp
  // PORT: constructor delegation  // was: this(var1, var2, var3, NULL);
  ```
- **⚪ LOW** `L312` (marker, `PORT`): was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5)
  ```cpp
  World::World(std::string /* PORT: was java.io.File */* var1, const std::string& var2, long long var3, WorldProvider* var5)
  ```
- **⚪ LOW** `L373` (marker, `PORT`): java.io.File world-dir creation + session.lock write + level.dat NBT load are
  ```cpp
  // PORT: java.io.File world-dir creation + session.lock write + level.dat NBT load are
  ```
- **⚪ LOW** `L381` (marker, `PORT`): NBT level.dat load done by ContentStorage, not java.io.File here
  ```cpp
  if(false) {  // PORT: NBT level.dat load done by ContentStorage, not java.io.File here
  ```
- **⚪ LOW** `L424` (marker, `PORT`): was java.io.File */* var0, const std::string& var1) {
  ```cpp
  NBTTagCompound* World::func_629_a(std::string /* PORT: was java.io.File */* var0, const std::string& var1) {
  ```
- **⚪ LOW** `L425` (marker, `PORT`): reading saves/<name>/level.dat is handled by the STFS save path (ContentStorage).
  ```cpp
  // PORT: reading saves/<name>/level.dat is handled by the STFS save path (ContentStorage).
  ```
- **⚪ LOW** `L431` (marker, `PORT`): was java.io.File */* var0, const std::string& var1) {
  ```cpp
  void World::deleteWorld(std::string /* PORT: was java.io.File */* var0, const std::string& var1) {
  ```
- **⚪ LOW** `L432` (marker, `PORT`): world deletion goes through the STFS container API (ContentStorage / Save360),
  ```cpp
  // PORT: world deletion goes through the STFS container API (ContentStorage / Save360),
  ```
- **⚪ LOW** `L437` (marker, `PORT`): was java.io.File */* var0) {
  ```cpp
  void World::deleteFiles(std::string /* PORT: was java.io.File */* var0) {
  ```
- **⚪ LOW** `L438` (marker, `PORT`): recursive java.io.File delete -> handled by the STFS container API.
  ```cpp
  // PORT: recursive java.io.File delete -> handled by the STFS container API.
  ```
- **⚪ LOW** `L446` (marker, `PORT`): was java.io.File */* var1) {
  ```cpp
  IChunkProvider* World::func_4081_a(std::string /* PORT: was java.io.File */* var1) {
  ```
- **⚪ LOW** `L528` (marker, `PORT`): the level.dat write (+ .dat_new/.dat_old rotation) is performed by the STFS save
  ```cpp
  // PORT: the level.dat write (+ .dat_new/.dat_old rotation) is performed by the STFS save
  ```
- **⚪ LOW** `L2142` (marker, `PORT`): the session.lock multi-access guard relied on java.io.File/DataInputStream.
  ```cpp
  // PORT: the session.lock multi-access guard relied on java.io.File/DataInputStream.
  ```
