# CraftingManager.cpp

Risk score: **11**  ·  6 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L26` (marker, `PORT stub`): s whose addRecipes() bodies pass a NULL pattern (they never
  ```cpp
  // converted port those are dead PORT stubs whose addRecipes() bodies pass a NULL pattern (they never
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L82` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  int* map = new int[cellCount];
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L281` (marker, `XXX`): ", "###" };
  ```cpp
  const char* rows[] = { "###", "XXX", "###" };
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): for CraftingManager.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT for CraftingManager.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L555` (marker, `PORT`): remaining RecipesFood/RecipesIngots entries (e.g. ingot<-block reversals,
  ```cpp
  // PORT: remaining RecipesFood/RecipesIngots entries (e.g. ingot<-block reversals,
  ```
