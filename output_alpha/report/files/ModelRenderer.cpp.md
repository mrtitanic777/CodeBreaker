# ModelRenderer.cpp

Risk score: **2**  ·  5 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L5` (marker, `PORT`): of ModelRenderer.java (Minecraft Alpha v1.2.6)
  ```cpp
  // Hand-written PORT of ModelRenderer.java (Minecraft Alpha v1.2.6)
  ```
- **⚪ LOW** `L31` (marker, `PORT`): (render): the original Java compiled geometry ONCE into a GL display list
  ```cpp
  // PORT (render): the original Java compiled geometry ONCE into a GL display list
  ```
- **⚪ LOW** `L135` (marker, `PORT`): Alpha's 5-float PositionTexureVertex ctor
  ```cpp
  // Build the 8 corner vertices. PORT: Alpha's 5-float PositionTexureVertex ctor
  ```
- **⚪ LOW** `L306` (marker, `PORT`): the Java compiled a GL display list here. We have no persistent per-part
  ```cpp
  // PORT: the Java compiled a GL display list here. We have no persistent per-part
  ```
