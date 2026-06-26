# Minecraft.cpp

Risk score: **93**  ·  51 findings

## STUBBED — placeholder; needs a hand implementation

- **🔴 HIGH** `L183` (marker, `PORT`): AWT canvas/Graphics path and LWJGL Display setup replaced by the fixed
  ```cpp
  // PORT: AWT canvas/Graphics path and LWJGL Display setup replaced by the fixed
  ```
- **🔴 HIGH** `L196` (marker, `PORT`): D3D9 device already created at boot  // was: Display.create();
  ```cpp
  // PORT: D3D9 device already created at boot  // was: Display.create();
  ```
- **🔴 HIGH** `L213` (marker, `PORT`): D3D9 Clear()  // was: GL11.glClearDepth(1.0);
  ```cpp
  // PORT: D3D9 Clear()  // was: GL11.glClearDepth(1.0);
  ```
- **🔴 HIGH** `L219` (marker, `PORT`): GL constant -> D3D9  // was: GL11.glCullFace(GL11.GL_BACK);
  ```cpp
  // PORT: GL constant -> D3D9  // was: GL11.glCullFace(GL11.GL_BACK);
  ```
- **🔴 HIGH** `L230` (marker, `PORT`): resource-download thread (network) n/a on 360
  ```cpp
  // PORT: resource-download thread (network) n/a on 360
  ```
- **🔴 HIGH** `L279` (marker, `PORT`): D3D9 present  // was: Display.swapBuffers();
  ```cpp
  // PORT: D3D9 present  // was: Display.swapBuffers();
  ```
- **🔴 HIGH** `L346` (marker, `PORT`): D3D9 has no glGetError equivalent here; nothing to report on the 360.
  ```cpp
  // PORT: D3D9 has no glGetError equivalent here; nothing to report on the 360.
  ```
- **🔴 HIGH** `L355` (marker, `PORT`): resource-download thread n/a on 360 */;
  ```cpp
  /* PORT: resource-download thread n/a on 360 */;
  ```
- **🔴 HIGH** `L373` (marker, `PORT`): D3D9 Release  // was: Display.destroy();
  ```cpp
  // PORT: D3D9 Release  // was: Display.destroy();
  ```
- **🔴 HIGH** `L442` (marker, `PORT`): XInput poll; Display.update() handled by Present below.
  ```cpp
  // PORT: XInput poll; Display.update() handled by Present below.
  ```
- **🔴 HIGH** `L498` (marker, `PORT`): F2 screenshot (Keyboard) n/a on 360 — XInput has no equivalent binding here.
  ```cpp
  // PORT: F2 screenshot (Keyboard) n/a on 360 — XInput has no equivalent binding here.
  ```
- **🔴 HIGH** `L691` (marker, `PORT`): D3D9 present params  // was: Display.setDisplayMode(Display.getDesktopDisplayMode());
  ```cpp
  // PORT: D3D9 present params  // was: Display.setDisplayMode(Display.getDesktopDisplayMode());
  ```
- **🔴 HIGH** `L718` (marker, `PORT`): D3D9 present params  // was: Display.setDisplayMode(new DisplayMode(this.field_9236_T, this.field_9235_U));
  ```cpp
  // PORT: D3D9 present params  // was: Display.setDisplayMode(new DisplayMode(this.field_9236_T, this.field_9235_U));
  ```
- **🔴 HIGH** `L811` (marker, `PORT`): the Java Mouse/Keyboard event-poll loop (LWJGL) is replaced by XInput handled
  ```cpp
  // PORT: the Java Mouse/Keyboard event-poll loop (LWJGL) is replaced by XInput handled
  ```
- **🔴 HIGH** `L864` (marker, `PORT`): resource-download thread n/a on 360  // was: m_downloadResourcesThread->reloadResources();
  ```cpp
  // PORT: resource-download thread n/a on 360  // was: m_downloadResourcesThread->reloadResources();
  ```
- **🔴 HIGH** `L1091` (marker, `PORT`): the Java AWT applet launcher (Frame/Canvas/Dimension/MinecraftImpl/Thread/
  ```cpp
  // PORT: the Java AWT applet launcher (Frame/Canvas/Dimension/MinecraftImpl/Thread/
  ```

## BLOCKED — explicitly could not be ported

- **🔴 HIGH** `L943` (marker, `PORT-BLOCKED`): thePlayer = (EntityPlayerSP*)var1->func_4085_a(EntityPlayerSP.class);
  ```cpp
  // PORT-BLOCKED: thePlayer = (EntityPlayerSP*)var1->func_4085_a(EntityPlayerSP.class);
  ```

## RISK — matches a known console-crash / leak pattern

- **🟡 MED** `L86` (rule, `uninit-array`): new long long[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new long long[N]() (the new bool[10] phantom-input bug).
  ```cpp
  long long* Minecraft::s_field_9240_E = new long long[FRAME_TIMES_COUNT];
  ```
- **🟡 MED** `L87` (rule, `uninit-array`): new long long[] without () -- Java zero-fills arrays, C++ leaves this GARBAGE. Use new long long[N]() (the new bool[10] phantom-input bug).
  ```cpp
  long long* Minecraft::s_field_9239_F = new long long[FRAME_TIMES_COUNT];
  ```
- **🟡 MED** `L247` (lint, `warn/clear-target`): Clear(TARGET|ZBUFFER): if this came from glClear(DEPTH) it should be ZBUFFER-only -- TARGET wipes the rendered world (the 'eraser' bug).

## OVERRIDDEN — a hand-written file is in use for this class

- **🟡 MED** `—` (override, `override`): hand-written override in use (run with --pristine to measure backport debt)

## ANNOTATED — a note about a decision already taken

- **⚪ LOW** `L72` (marker, `PORT`): `running` is referenced by run()/shutdown() but the converted .h never declared it
  ```cpp
  // PORT: `running` is referenced by run()/shutdown() but the converted .h never declared it
  ```
- **⚪ LOW** `L77` (marker, `PORT`): `field_6316_m` (Java's "game is paused" flag) is referenced BARE throughout run()/
  ```cpp
  // PORT: `field_6316_m` (Java's "game is paused" flag) is referenced BARE throughout run()/
  ```
- **⚪ LOW** `L82` (marker, `PORT`): the converted code referenced a symbol `s_field_9240_E_count` (the Java array
  ```cpp
  // PORT: the converted code referenced a symbol `s_field_9240_E_count` (the Java array
  ```
- **⚪ LOW** `L89` (marker, `PORT`): was java.io.File */* Minecraft::s_minecraftDir = NULL;
  ```cpp
  std::string /* PORT: was java.io.File */* Minecraft::s_minecraftDir = NULL;
  ```
- **⚪ LOW** `L103` (marker, `PORT`): game-loop flag; set true in run(), cleared in shutdown()
  ```cpp
  , running(false)   // PORT: game-loop flag; set true in run(), cleared in shutdown()
  ```
- **⚪ LOW** `L144` (marker, `PORT`): Java Swing/AWT constructor (Component/Canvas/applet host) is not used on the 360.
  ```cpp
  // PORT: Java Swing/AWT constructor (Component/Canvas/applet host) is not used on the 360.
  ```
- **⚪ LOW** `L195` (marker, `PORT`): no window title on 360  // was: Display.setTitle("Minecraft Minecraft Alpha v1.2.6");
  ```cpp
  // PORT: no window title on 360  // was: Display.setTitle("Minecraft Minecraft Alpha v1.2.6");
  ```
- **⚪ LOW** `L211` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L226` (marker, `PORT`): 9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
  ```cpp
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
  ```
- **⚪ LOW** `L255` (marker, `PORT`): 9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
  ```cpp
  { D3DVIEWPORT9 _vp = {(DWORD)(0),(DWORD)(0),(DWORD)(this->m_displayWidth),(DWORD)(this->m_displayHeight),0.0f,1.0f};
  ```
- **⚪ LOW** `L259` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L295` (marker, `PORT`): was java.io.File */* Minecraft::getMinecraftDir() {
  ```cpp
  std::string /* PORT: was java.io.File */* Minecraft::getMinecraftDir() {
  ```
- **⚪ LOW** `L303` (marker, `PORT`): was java.io.File */* Minecraft::getAppDir(const std::string& var0) {
  ```cpp
  std::string /* PORT: was java.io.File */* Minecraft::getAppDir(const std::string& var0) {
  ```
- **⚪ LOW** `L304` (marker, `PORT`): the 360 has no user.home / APPDATA and no EnumOSMappingHelper switch. Like the
  ```cpp
  // PORT: the 360 has no user.home / APPDATA and no EnumOSMappingHelper switch. Like the
  ```
- **⚪ LOW** `L313` (marker, `PORT`): fixed platform on the 360 — always report Windows-like (the only branch that
  ```cpp
  // PORT: fixed platform on the 360 — always report Windows-like (the only branch that
  ```
- **⚪ LOW** `L351` (marker, `PORT`): applet host n/a on 360 (m_mcApplet is always NULL; MinecraftApplet is a stub).
  ```cpp
  // PORT: applet host n/a on 360 (m_mcApplet is always NULL; MinecraftApplet is a stub).
  ```
- **⚪ LOW** `L399` (marker, `PORT`): LWJGL window state (Display) fixed on 360
  ```cpp
  if (false) { // PORT: LWJGL window state (Display) fixed on 360
  ```
- **⚪ LOW** `L428` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L452` (marker, `PORT`): LWJGL window state (Display) fixed on 360
  ```cpp
  if (false) { // PORT: LWJGL window state (Display) fixed on 360
  ```
- **⚪ LOW** `L522` (marker, `PORT`): draw mode */ 7);
  ```cpp
  var7.init( /* PORT: draw mode */ 7);
  ```
- **⚪ LOW** `L543` (marker, `PORT`): draw mode */ 7);
  ```cpp
  var7.init( /* PORT: draw mode */ 7);
  ```
- **⚪ LOW** `L550` (marker, `PORT`): draw mode */ 1);
  ```cpp
  var7.init( /* PORT: draw mode */ 1);
  ```
- **⚪ LOW** `L574` (marker, `PORT`): bind active texture */);
  ```cpp
  g_pd3dDevice->SetTexture(0, g_terrainTexture /* PORT: bind active texture */);
  ```
- **⚪ LOW** `L582` (marker, `PORT`): LWJGL window state (Display) fixed on 360
  ```cpp
  if (true) { // PORT: LWJGL window state (Display) fixed on 360
  ```
- **⚪ LOW** `L722` (marker, `PORT`): always fullscreen on 360  // was: Display.setFullscreen(this.a);
  ```cpp
  // PORT: always fullscreen on 360  // was: Display.setFullscreen(this.a);
  ```
- **⚪ LOW** `L874` (marker, `PORT`): World's ctor takes std::string* (the converter mapped java.io.File -> std::string*),
  ```cpp
  // PORT: World's ctor takes std::string* (the converter mapped java.io.File -> std::string*),
  ```
- **⚪ LOW** `L1020` (marker, `PORT`): was java.io.File */* var2) {
  ```cpp
  void Minecraft::installResource(const std::string& var1in, std::string /* PORT: was java.io.File */* var2) {
  ```
- **⚪ LOW** `L1052` (marker, `PORT`): header mandates a by-reference return; back it with a file-scope buffer
  ```cpp
  // PORT: header mandates a by-reference return; back it with a file-scope buffer
  ```
- **⚪ LOW** `L1101` (marker, `PORT`): Java `public static void main(String[])` — the 360 entry point is Main.cpp.
  ```cpp
  // PORT: Java `public static void main(String[])` — the 360 entry point is Main.cpp.
  ```
- **⚪ LOW** `L1108` (marker, `PORT`): was pure-virtual, implemented by the Java applet/Frame subclass to pop a crash
  ```cpp
  // PORT: was pure-virtual, implemented by the Java applet/Frame subclass to pop a crash
  ```
