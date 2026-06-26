# EntityRenderer.cpp

Risk score: **90**  ·  54 findings

## GUESSED — converter made a heuristic call that may be wrong -- INSPECT

- **🔴 HIGH** `L675` (marker, `PORT`): was sizeof(m_field_1392_d) = pointer size (4), buffer is 16 floats
  ```cpp
  memset(m_field_1392_d, 0, sizeof(float) * 16);  // PORT: was sizeof(m_field_1392_d) = pointer size (4), buffer is 16 floats
  ```

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L386` (marker, `PORT`): XInput right stick X  // was: int var10 = Mouse.getX() * var8 / this.mc->displayWidth;
  ```cpp
  // PORT: XInput right stick X  // was: int var10 = Mouse.getX() * var8 / this.mc->displayWidth;
  ```
- **🔴 HIGH** `L388` (marker, `PORT`): XInput right stick Y  // was: int var11 = var9 - Mouse.getY() * var9 / this.mc->displayHeight - 1;
  ```cpp
  // PORT: XInput right stick Y  // was: int var11 = var9 - Mouse.getY() * var9 / this.mc->displayHeight - 1;
  ```
- **🔴 HIGH** `L661` (marker, `PORT`): D3D9 caps check  // was: if(GLContext::getCapabilities().GL_NV_fog_distance) {
  ```cpp
  // PORT: D3D9 caps check  // was: if(GLContext::getCapabilities().GL_NV_fog_distance) {
  ```

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L228` (marker, `PORT-BLOCKED`): var12 += 180.0F;
  ```cpp
  // PORT-BLOCKED: var12 += 180.0F;
  ```
- **🔴 HIGH** `L229` (marker, `PORT-BLOCKED`): var9 += 2.0;
  ```cpp
  // PORT-BLOCKED: var9 += 2.0;
  ```
- **🔴 HIGH** `L230` (marker, `PORT-BLOCKED`): }
  ```cpp
  // PORT-BLOCKED: }
  ```
- **🔴 HIGH** `L253` (marker, `PORT-BLOCKED`): GL11.glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
  ```cpp
  // PORT-BLOCKED: GL11.glRotatef(180.0F, 0.0F, 1.0F, 0.0F);
  ```
- **🔴 HIGH** `L254` (marker, `PORT-BLOCKED`): }
  ```cpp
  // PORT-BLOCKED: }
  ```
- **🔴 HIGH** `L344` (marker, `PORT-BLOCKED`): this.field_1395_a.renderItemInFirstPerson(var1);
  ```cpp
  // PORT-BLOCKED: this.field_1395_a.renderItemInFirstPerson(var1);
  ```
- **🔴 HIGH** `L345` (marker, `PORT-BLOCKED`): }
  ```cpp
  // PORT-BLOCKED: }
  ```
- **🔴 HIGH** `L393` (marker, `PORT-BLOCKED`): this.mc->ingameGUI.renderGameOverlay(var1, this.mc->currentScreen != null, var10, var11);
  ```cpp
  // PORT-BLOCKED: this.mc->ingameGUI.renderGameOverlay(var1, this.mc->currentScreen != null, var10, var11);
  ```
- **🔴 HIGH** `L394` (marker, `PORT-BLOCKED`): }
  ```cpp
  // PORT-BLOCKED: }
  ```
- **🔴 HIGH** `L662` (marker, `PORT-BLOCKED`): GL11.glFogi(NVFogDistance.GL_FOG_DISTANCE_MODE_NV, NVFogDistance.GL_EYE_RADIAL_NV);
  ```cpp
  // PORT-BLOCKED: GL11.glFogi(NVFogDistance.GL_FOG_DISTANCE_MODE_NV, NVFogDistance.GL_EYE_RADIAL_NV);
  ```
- **🔴 HIGH** `L663` (marker, `PORT-BLOCKED`): }
  ```cpp
  // PORT-BLOCKED: }
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L59` (rule, `rule-of-three`): EntityRenderer owns heap + has a destructor but no copy ctor / copy assignment -> Rule of Three: a copy double-frees / corrupts the heap (the intermittent gen-phase 0xC0000018 class). Add them or make copying private.
- **🔴 HIGH** `L148` (lint, `leak/new-leak`): `new` whose result is discarded/passed -> Java GC'd this; C++ leaks (TNT-stack OOM class). Use a stack value or delete it.
- **🟡 MED** `L399` (lint, `warn/clear-target`): Clear(TARGET|ZBUFFER): if this came from glClear(DEPTH) it should be ZBUFFER-only -- TARGET wipes the rendered world (the 'eraser' bug).
- **🟡 MED** `L437` (lint, `warn/clear-target`): Clear(TARGET|ZBUFFER): if this came from glClear(DEPTH) it should be ZBUFFER-only -- TARGET wipes the rendered world (the 'eraser' bug).
- **🟡 MED** `L542` (lint, `warn/list-deref`): blocksList/itemsList[id]-> with no NULL guard on this line -> a virtual call through an unregistered (NULL) entry is IP=0x0. Verify the id is registered.

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L176` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L185` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L188` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L191` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L204` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L207` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L210` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L213` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L227` (marker, `PORT`): XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```cpp
  // PORT: XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```
- **⚪ LOW** `L252` (marker, `PORT`): XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```cpp
  // PORT: XInputGetState  // was: if(Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```
- **⚪ LOW** `L257` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L260` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L263` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L266` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L269` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L273` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L278` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L281` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L292` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L302` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_t, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L316` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L319` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_s, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L322` (marker, `PORT(post-vs-pre-multiply)`): camera/view composes in view frame
  ```cpp
  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &_r, &Shaders::getWorldMatrix());   // PORT(post-vs-pre-multiply): camera/view composes in view frame
  ```
- **⚪ LOW** `L343` (marker, `PORT`): XInputGetState  // was: if(!this.mc->gameSettings.thirdPersonView && !Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```cpp
  // PORT: XInputGetState  // was: if(!this.mc->gameSettings.thirdPersonView && !Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```
- **⚪ LOW** `L360` (marker, `PORT`): LWJGL window state (Display) fixed on 360
  ```cpp
  if (false) { // PORT: LWJGL window state (Display) fixed on 360
  ```
- **⚪ LOW** `L387` (marker, `PORT`): stub for blocked declaration
  ```cpp
  int var10 = 0; // PORT: stub for blocked declaration
  ```
- **⚪ LOW** `L389` (marker, `PORT`): stub for blocked declaration
  ```cpp
  int var11 = 0; // PORT: stub for blocked declaration
  ```
- **⚪ LOW** `L392` (marker, `PORT`): XInputGetState  // was: if(!Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```cpp
  // PORT: XInputGetState  // was: if(!Keyboard.isKeyDown(Keyboard.KEY_F1)) {
  ```
- **⚪ LOW** `L396` (marker, `PORT`): 9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
  ```cpp
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
  ```
- **⚪ LOW** `L434` (marker, `PORT`): 9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
  ```cpp
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->mc->m_displayWidth),(DWORD)(this->mc->m_displayHeight),0.0f,1.0f};
  ```
- **⚪ LOW** `L556` (marker, `PORT`): 2D GUI render state — back-face cull OFF (the Y-flipped GUI ortho reverses quad
  ```cpp
  // PORT: 2D GUI render state — back-face cull OFF (the Y-flipped GUI ortho reverses quad
  ```
- **⚪ LOW** `L563` (marker, `PORT`): D3D LH GUI ortho — Minecraft's glOrtho(0,w,h,0,1000,3000) is Y-FLIPPED (origin
  ```cpp
  // PORT: D3D LH GUI ortho — Minecraft's glOrtho(0,w,h,0,1000,3000) is Y-FLIPPED (origin
  ```
- **⚪ LOW** `L674` (marker, `PORT`): was FloatBuffer */ EntityRenderer::func_908_a(float var1, float var2, float var3, float var4) {
  ```cpp
  float* /* PORT: was FloatBuffer */ EntityRenderer::func_908_a(float var1, float var2, float var3, float var4) {
  ```
