# samples/

Local sample inputs and extracted output. **Git-ignored** — these are Minecraft:
Xbox 360 Edition assets (game executable, audio, textures), which are copyrighted
and too large for the repo, and all extracted output is regenerable.

Regenerate extracted assets from a title's containers:

```bash
XboxTranspiler pck  <title>/x32Data.pck   -x
XboxTranspiler xzp  <title>/TexturePack.xzp -x
```

Produce a decoded PE for the `retool` diff commands:

```bash
XboxTranspiler xex  default.xex --dump default.exe
```
