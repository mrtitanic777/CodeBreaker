# RenderGlobal.cpp

Risk score: **32**  ·  27 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L194` (marker, `PORT`): GL occlusion not available on Xbox 360. // was: var1->func_6251_l()->checkARBOcclusion();
  ```cpp
  // PORT: GL occlusion not available on Xbox 360. // was: var1->func_6251_l()->checkARBOcclusion();
  ```
- **🔴 HIGH** `L198` (marker, `PORT`): D3D9 no occlusion queries on 360.
  ```cpp
  // PORT: D3D9 no occlusion queries on 360.
  ```
- **🔴 HIGH** `L525` (marker, `PORT`): D3D9 no occlusion queries on 360.
  ```cpp
  // PORT: D3D9 no occlusion queries on 360.
  ```
- **🔴 HIGH** `L551` (marker, `PORT`): occlusion query result not available on Xbox 360 -- treat as visible (no cull).
  ```cpp
  // PORT: occlusion query result not available on Xbox 360 -- treat as visible (no cull).
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L85` (rule, `rule-of-three`): RenderGlobal owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🟡 MED** `L113` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1457_b(new int[50000])   // PORT: was new int[0xC350] -- 0xC350 == 50000; MSVC can't encode the char literal (C4566). Plain int[50000].
  ```
- **🟡 MED** `L167` (rule, `uninit-array`): new int[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new int[N]() (the new bool[10] phantom-input bug).
  ```cpp
  , m_field_1457_b(new int[50000])   // PORT: see ctor above (0xC350 == 50000).
  ```
- **🟡 MED** `L1150` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
- **🟡 MED** `L1154` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L2` (marker, `PORT`): override for RenderGlobal.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT override for RenderGlobal.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L17` (marker, `PORT`): ".
  ```cpp
  // cleanly on this platform are marked "// PORT:".
  ```
- **⚪ LOW** `L69` (marker, `PORT`): the per-chunk renderer arrays (m_field_1444_o / m_field_1445_n) are arrays of WorldRenderer
  ```cpp
  // PORT: the per-chunk renderer arrays (m_field_1444_o / m_field_1445_n) are arrays of WorldRenderer
  ```
- **⚪ LOW** `L113` (marker, `PORT`): was new int[0xC350] -- 0xC350 == 50000; MSVC can't encode the char literal (C4566). Plain int[50000].
  ```cpp
  , m_field_1457_b(new int[50000])   // PORT: was new int[0xC350] -- 0xC350 == 50000; MSVC can't encode the char literal (C4566). Plain int[50000].
  ```
- **⚪ LOW** `L134` (marker, `PORT(array-literal field init)`): C++03 has no init-list array fill; alloc here.
  ```cpp
  // PORT(array-literal field init): C++03 has no init-list array fill; alloc here.
  ```
- **⚪ LOW** `L167` (marker, `PORT`): see ctor above (0xC350 == 50000).
  ```cpp
  , m_field_1457_b(new int[50000])   // PORT: see ctor above (0xC350 == 50000).
  ```
- **⚪ LOW** `L202` (marker, `PORT(stars/sky lists)`): the converter emitted the star-field + horizon-plane "display list"
  ```cpp
  // PORT(stars/sky lists): the converter emitted the star-field + horizon-plane "display list"
  ```
- **⚪ LOW** `L227` (marker, `PORT(stars)`): built lazily + drawn inline in func_4142_a; this prebuild is a no-op here
  ```cpp
  // PORT(stars): built lazily + drawn inline in func_4142_a; this prebuild is a no-op here
  ```
- **⚪ LOW** `L357` (marker, `PORT`): header returns const std::string& -- back it with function-local static storage so the
  ```cpp
  // PORT: header returns const std::string& -- back it with function-local static storage so the
  ```
- **⚪ LOW** `L617` (marker, `PORT`): GL display lists / RenderList replay aren't available on Xbox 360. The per-chunk geometry
  ```cpp
  // PORT: GL display lists / RenderList replay aren't available on Xbox 360. The per-chunk geometry
  ```
- **⚪ LOW** `L739` (marker, `PORT(stars)`): the GL star-field list ('field_1434_y') has no display-list cache here.
  ```cpp
  // PORT(stars): the GL star-field list ('field_1434_y') has no display-list cache here.
  ```
- **⚪ LOW** `L1071` (marker, `PORT`): Alpha's Shaders has no setAlphaRef; rely on D3DRS_ALPHATESTENABLE off + alpha blend for the crack overlay.
  ```cpp
  // PORT: Alpha's Shaders has no setAlphaRef; rely on D3DRS_ALPHATESTENABLE off + alpha blend for the crack overlay.
  ```
- **⚪ LOW** `L1072` (marker, `PORT`): GL11.glPolygonOffset -- no polygon-offset on Xbox 360. Use ZWRITE off (overlay is
  ```cpp
  // PORT: GL11.glPolygonOffset -- no polygon-offset on Xbox 360. Use ZWRITE off (overlay is
  ```
- **⚪ LOW** `L1081` (marker, `PORT`): Tesselator::setTranslationD doesn't exist on this platform. Java offset the verts by
  ```cpp
  // PORT: Tesselator::setTranslationD doesn't exist on this platform. Java offset the verts by
  ```
- **⚪ LOW** `L1299` (marker, `PORT`): skin download (network) not used on Xbox 360
  ```cpp
  // PORT: skin download (network) not used on Xbox 360
  ```
- **⚪ LOW** `L1303` (marker, `PORT`): skin release (network) not used on Xbox 360
  ```cpp
  // PORT: skin release (network) not used on Xbox 360
  ```
- **⚪ LOW** `L1319` (marker, `PORT`): TileEntity special-renderer registration -- not used on Xbox 360 (no special TileEntities
  ```cpp
  // PORT: TileEntity special-renderer registration -- not used on Xbox 360 (no special TileEntities
  ```
