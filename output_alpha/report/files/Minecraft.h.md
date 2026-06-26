# Minecraft.h

Risk score: **2**  ·  10 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L42` (marker, `PORT`): was AWT Component */ var1, void* /* PORT: was AWT Canvas */ var2, MinecraftApplet* var3, int var4, int var5, bool var6);
  ```cpp
  Minecraft(void* /* PORT: was AWT Component */ var1, void* /* PORT: was AWT Canvas */ var2, MinecraftApplet* var3, int var4, int var5, bool var6);
  ```
- **⚪ LOW** `L54` (marker, `PORT`): was AWT Canvas */ m_mcCanvas;
  ```cpp
  void* /* PORT: was AWT Canvas */ m_mcCanvas;
  ```
- **⚪ LOW** `L56` (marker, `PORT`): game-loop flag (set in run(), read by LoadingScreenRenderer)
  ```cpp
  bool running;   // PORT: game-loop flag (set in run(), read by LoadingScreenRenderer)
  ```
- **⚪ LOW** `L72` (marker, `PORT`): was java.io.File */* m_field_6297_D;
  ```cpp
  std::string /* PORT: was java.io.File */* m_field_6297_D;
  ```
- **⚪ LOW** `L80` (marker, `PORT`): was pure-virtual (impl'd by the AWT/applet subclass); made concrete so the 360 boot can `new Minecraft()`
  ```cpp
  virtual void func_4007_a(UnexpectedThrowable* var1);  // PORT: was pure-virtual (impl'd by the AWT/applet subclass); made concrete so the 360 boot can `new Mine
  ```
- **⚪ LOW** `L84` (marker, `PORT`): was java.io.File */* getMinecraftDir();
  ```cpp
  static std::string /* PORT: was java.io.File */* getMinecraftDir();
  ```
- **⚪ LOW** `L85` (marker, `PORT`): was java.io.File */* getAppDir(const std::string& var0);
  ```cpp
  static std::string /* PORT: was java.io.File */* getAppDir(const std::string& var0);
  ```
- **⚪ LOW** `L101` (marker, `PORT`): was java.io.File */* var2);
  ```cpp
  virtual void installResource(const std::string& var1, std::string /* PORT: was java.io.File */* var2);
  ```
- **⚪ LOW** `L127` (marker, `PORT`): was java.io.File */* s_minecraftDir;
  ```cpp
  static std::string /* PORT: was java.io.File */* s_minecraftDir;
  ```
