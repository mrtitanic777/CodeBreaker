# RenderBlocks.cpp

Risk score: **7**  ·  20 findings

## RISK — matches a known console-crash / leak pattern

- **🔴 HIGH** `L147` (rule, `uninit-array`): new <Type>*[] without () -- uninitialized pointer array = garbage vtables -> deref crash (IP 0x0). Use new T*[N]() and NULL-check before deref.
  ```cpp
  Vec3D** var21 = new Vec3D*[8];
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for RenderBlocks.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for RenderBlocks.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L19` (marker, `PORT`): the cube still renders at full bounds; the inset is a cosmetic offset.
  ```cpp
  //     // PORT: -- the cube still renders at full bounds; the inset is a cosmetic offset.
  ```
- **⚪ LOW** `L145` (marker, `PORT`): was `Vec3D* var21 = new Vec3D[8]` (array of VALUES) but createVector returns
  ```cpp
  // PORT: was `Vec3D* var21 = new Vec3D[8]` (array of VALUES) but createVector returns
  ```
- **⚪ LOW** `L1278` (marker, `PORT`): Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```cpp
  // PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```
- **⚪ LOW** `L1290` (marker, `PORT`): Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```cpp
  // PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```
- **⚪ LOW** `L1302` (marker, `PORT`): Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```cpp
  // PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```
- **⚪ LOW** `L1314` (marker, `PORT`): Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```cpp
  // PORT: Tesselator has no setTranslationF; the cactus 1px inset is dropped (cosmetic).
  ```
- **⚪ LOW** `L1754` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1773` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1786` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1813` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1823` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1836` (marker, `PORT`): Tesselator has no setTranslationF; cactus 1px inset dropped (cosmetic).
  ```cpp
  // PORT: Tesselator has no setTranslationF; cactus 1px inset dropped (cosmetic).
  ```
- **⚪ LOW** `L1853` (marker, `PORT`): PRE-multiply (local*world)
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply (local*world)
  ```
- **⚪ LOW** `L1878` (marker, `PORT`): PRE-multiply
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
  ```
- **⚪ LOW** `L1905` (marker, `PORT`): PRE-multiply
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
  ```
- **⚪ LOW** `L1929` (marker, `PORT`): PRE-multiply
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
  ```
- **⚪ LOW** `L1956` (marker, `PORT`): PRE-multiply
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT: PRE-multiply
  ```
