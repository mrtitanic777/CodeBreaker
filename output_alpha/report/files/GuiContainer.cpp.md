# GuiContainer.cpp

Risk score: **14**  ·  4 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L50` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glEnable(GL12.GL_RESCALE_NORMAL);
  ```
- **🔴 HIGH** `L75` (marker, `PORT`): D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```cpp
  // PORT: D3D9 SetRenderState equivalent  // was: GL11.glDisable(GL12.GL_RESCALE_NORMAL);
  ```

## WARNING — suspicious; verify by hand

- **🟡 MED** `L86` (marker, `TODO`): implement
  ```cpp
  // TODO: implement
  ```

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)
