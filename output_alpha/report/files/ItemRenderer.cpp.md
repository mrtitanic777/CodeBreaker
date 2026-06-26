# ItemRenderer.cpp

Risk score: **27**  ·  6 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L75` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L167` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L211` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L251` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L290` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L311` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.
