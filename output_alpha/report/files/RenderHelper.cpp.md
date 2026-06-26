# RenderHelper.cpp

Risk score: **2**  ·  4 findings

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L10` (marker, `PORT`): was FloatBuffer */ RenderHelper::s_field_1695_a = GLAllocation::createDirectFloatBuffer(16);
  ```cpp
  float* /* PORT: was FloatBuffer */ RenderHelper::s_field_1695_a = GLAllocation::createDirectFloatBuffer(16);
  ```
- **⚪ LOW** `L50` (marker, `PORT`): was FloatBuffer */ RenderHelper::func_1157_a(double var0, double var2, double var4, double var6) {
  ```cpp
  float* /* PORT: was FloatBuffer */ RenderHelper::func_1157_a(double var0, double var2, double var4, double var6) {
  ```
- **⚪ LOW** `L54` (marker, `PORT`): was FloatBuffer */ RenderHelper::func_1156_a(float var0, float var1, float var2, float var3) {
  ```cpp
  float* /* PORT: was FloatBuffer */ RenderHelper::func_1156_a(float var0, float var1, float var2, float var3) {
  ```
