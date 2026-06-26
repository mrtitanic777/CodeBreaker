# GuiMainMenu.cpp

Risk score: **11**  ·  14 findings

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L61` (rule, `rule-of-three`): GuiMainMenu owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🔴 HIGH** `L193` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  m_logoEffects = new LogoEffectRandomizer*[cols];
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L111` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for GuiMainMenu.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for GuiMainMenu.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L14` (marker, `PORT`): .
  ```cpp
  //   * Calendar/Date holiday splash overrides dropped (unportable; // PORT:).
  ```
- **⚪ LOW** `L47` (marker, `PORT`): the Alpha splash list. Java read these from a packaged "splashes.txt" via a BufferedReader;
  ```cpp
  // PORT: the Alpha splash list. Java read these from a packaged "splashes.txt" via a BufferedReader;
  ```
- **⚪ LOW** `L69` (marker, `PORT(array-literal field init)`): C++03 has no init-list array fill; alloc + fill here.
  ```cpp
  // PORT(array-literal field init): C++03 has no init-list array fill; alloc + fill here.
  ```
- **⚪ LOW** `L78` (marker, `PORT`): pick a random splash. (Java built a vector from splashes.txt then random-indexed it.)
  ```cpp
  // PORT: pick a random splash. (Java built a vector from splashes.txt then random-indexed it.)
  ```
- **⚪ LOW** `L115` (marker, `PORT`): Java used Calendar/Date for holiday splash overrides (Happy birthday / Merry X-mas /
  ```cpp
  // PORT: Java used Calendar/Date for holiday splash overrides (Happy birthday / Merry X-mas /
  ```
- **⚪ LOW** `L166` (marker, `PORT`): bob the splash using a HIGH-RESOLUTION wall clock. Java used currentTimeMillis()%1000;
  ```cpp
  // PORT: bob the splash using a HIGH-RESOLUTION wall clock. Java used currentTimeMillis()%1000;
  ```
- **⚪ LOW** `L212` (marker, `PORT`): 9 _vp = { (DWORD)0, (DWORD)(this->mc->m_displayHeight - var3),
  ```cpp
  { D3DVIEWPORT9 _vp = { (DWORD)0, (DWORD)(this->mc->m_displayHeight - var3),
  ```
- **⚪ LOW** `L218` (marker, `PORT`): GL11.glCullFace(GL_BACK) -> handled by CULLMODE above.
  ```cpp
  // PORT: GL11.glCullFace(GL_BACK) -> handled by CULLMODE above.
  ```
- **⚪ LOW** `L312` (marker, `PORT`): 9 _vp = { (DWORD)0, (DWORD)0, (DWORD)(this->mc->m_displayWidth),
  ```cpp
  { D3DVIEWPORT9 _vp = { (DWORD)0, (DWORD)0, (DWORD)(this->mc->m_displayWidth),
  ```
