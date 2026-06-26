# PositionTexureVertex.cpp

Risk score: **5**  ·  2 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L17` (rule, `ctor-stub`): constructor PositionTexureVertex(float var1, float var2, float var3, float var4, float var5) has an empty/delegation-only body -- field init and static REGISTRATION may be missing (the Block blockID=0 / BlockContainer s_isBlockContainer bugs). Replicate the real ctor body.
  ```cpp
  PositionTexureVertex(float var1, float var2, float var3, float var4, float var5) {...}
  ```

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L22` (marker, `PORT`): constructor delegation  // was: this(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3), var4, var5);
  ```cpp
  // PORT: constructor delegation  // was: this(Vec3D::createVectorHelper((double)var1, (double)var2, (double)var3), var4, var5);
  ```
