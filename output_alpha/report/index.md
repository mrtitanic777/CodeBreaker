# Conversion Report

- **Profile:** alpha
- **Output Dir:** C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\ConversionTool\output_alpha
- **Build Log:** C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\ConversionTool\build_alpha47.log

## Totals

| Severity | Count |
|---|---|
| 🔴 HIGH | 245 |
| 🟡 MED  | 460 |
| ⚪ LOW  | 476 |
| **Total events** | **1181** |

| Category | Count | | What it means |
|---|---:|---|---|
| `GUESSED` | 50 | █················· | converter made a heuristic call that may be wrong -- INSPECT |
| `STUBBED` | 128 | ██················ | placeholder; needs a hand implementation |
| `BLOCKED` | 23 | ·················· | explicitly could not be ported |
| `RISK` | 238 | ████·············· | matches a known console-crash / leak pattern |
| `WARNING` | 100 | ██················ | suspicious; verify by hand |
| `OVERRIDDEN` | 166 | ███··············· | a hand-written file is in use for this class |
| `ANNOTATED` | 476 | ███████··········· | a note about a decision already taken |

## Files by risk (inspect top-down)

290 files have findings; 129 carry HIGH-severity risk.

| Risk | File | 🔴 | 🟡 | guess | stub | risk | err | override |
|---:|---|---:|---:|---:|---:|---:|---:|---:|
| 93 | [Minecraft.cpp](files/Minecraft.cpp.md) | 17 | 4 | 0 | 16 | 3 | 0 | 1 |
| 90 | [EntityRenderer.cpp](files/EntityRenderer.cpp.md) | 16 | 5 | 1 | 3 | 5 | 0 | 1 |
| 73 | [Block.cpp](files/Block.cpp.md) | 7 | 19 | 1 | 1 | 8 | 0 | 1 |
| 68 | [World.cpp](files/World.cpp.md) | 8 | 14 | 6 | 2 | 10 | 0 | 1 |
| 62 | [World.h](files/World.h.md) | 12 | 1 | 11 | 1 | 0 | 0 | 1 |
| 39 | [ItemStack.cpp](files/ItemStack.cpp.md) | 5 | 7 | 0 | 5 | 6 | 0 | 0 |
| 32 | [RenderGlobal.cpp](files/RenderGlobal.cpp.md) | 4 | 6 | 0 | 4 | 5 | 0 | 1 |
| 27 | [ItemRenderer.cpp](files/ItemRenderer.cpp.md) | 5 | 1 | 0 | 5 | 1 | 0 | 0 |
| 26 | [Chunk.cpp](files/Chunk.cpp.md) | 2 | 8 | 0 | 2 | 7 | 0 | 1 |
| 24 | [Entity.cpp](files/Entity.cpp.md) | 0 | 12 | 0 | 0 | 5 | 0 | 1 |
| 24 | [TileEntity.cpp](files/TileEntity.cpp.md) | 4 | 2 | 0 | 4 | 0 | 0 | 1 |
| 23 | [BlockFlowing.cpp](files/BlockFlowing.cpp.md) | 3 | 4 | 0 | 1 | 6 | 0 | 0 |
| 23 | [GLAllocation.cpp](files/GLAllocation.cpp.md) | 3 | 4 | 0 | 3 | 3 | 0 | 1 |
| 22 | [ChunkProviderHell.cpp](files/ChunkProviderHell.cpp.md) | 0 | 11 | 0 | 0 | 10 | 0 | 1 |
| 21 | [Teleporter.cpp](files/Teleporter.cpp.md) | 3 | 3 | 0 | 1 | 2 | 0 | 1 |
| 20 | [ChunkProviderGenerate.cpp](files/ChunkProviderGenerate.cpp.md) | 0 | 10 | 0 | 0 | 9 | 0 | 1 |
| 18 | [ChunkProviderIso.cpp](files/ChunkProviderIso.cpp.md) | 2 | 4 | 0 | 0 | 4 | 0 | 1 |
| 18 | [WorldGenBigTree.cpp](files/WorldGenBigTree.cpp.md) | 2 | 4 | 0 | 0 | 5 | 0 | 1 |
| 17 | [AxisAlignedBB.cpp](files/AxisAlignedBB.cpp.md) | 3 | 1 | 1 | 0 | 2 | 0 | 1 |
| 17 | [MobSpawnerBase.cpp](files/MobSpawnerBase.cpp.md) | 3 | 1 | 0 | 0 | 1 | 0 | 1 |
| 17 | [SpawnerAnimals.cpp](files/SpawnerAnimals.cpp.md) | 3 | 1 | 0 | 0 | 0 | 0 | 1 |
| 17 | [TexturePackBase.cpp](files/TexturePackBase.cpp.md) | 1 | 6 | 0 | 0 | 0 | 0 | 1 |
| 17 | [TileEntityMobSpawner.cpp](files/TileEntityMobSpawner.cpp.md) | 3 | 1 | 0 | 3 | 0 | 0 | 1 |
| 17 | [WorldRenderer.cpp](files/WorldRenderer.cpp.md) | 3 | 1 | 1 | 0 | 2 | 0 | 1 |
| 16 | [PlayerController.cpp](files/PlayerController.cpp.md) | 0 | 8 | 0 | 0 | 0 | 0 | 1 |
| 15 | [Chunk.h](files/Chunk.h.md) | 3 | 0 | 3 | 0 | 0 | 0 | 0 |
| 15 | [Pathfinder.cpp](files/Pathfinder.cpp.md) | 3 | 0 | 0 | 0 | 3 | 0 | 0 |
| 15 | [RenderGlobal.h](files/RenderGlobal.h.md) | 3 | 0 | 3 | 0 | 0 | 0 | 0 |
| 15 | [WorldRenderer.h](files/WorldRenderer.h.md) | 3 | 0 | 3 | 0 | 0 | 0 | 0 |
| 14 | [GuiContainer.cpp](files/GuiContainer.cpp.md) | 2 | 2 | 0 | 2 | 0 | 0 | 1 |
| 14 | [MCHashTable.cpp](files/MCHashTable.cpp.md) | 2 | 2 | 0 | 0 | 3 | 0 | 1 |
| 14 | [RenderLiving.cpp](files/RenderLiving.cpp.md) | 2 | 2 | 0 | 2 | 0 | 0 | 0 |
| 14 | [TileEntitySign.cpp](files/TileEntitySign.cpp.md) | 2 | 2 | 0 | 2 | 1 | 0 | 1 |
| 13 | [PlayerControllerSP.cpp](files/PlayerControllerSP.cpp.md) | 1 | 4 | 0 | 1 | 3 | 0 | 1 |
| 13 | [TextureFlamesFX.cpp](files/TextureFlamesFX.cpp.md) | 1 | 4 | 0 | 1 | 4 | 0 | 0 |
| 13 | [WorldChunkManagerHell.cpp](files/WorldChunkManagerHell.cpp.md) | 1 | 4 | 0 | 0 | 4 | 0 | 1 |
| 12 | [BlockContainer.cpp](files/BlockContainer.cpp.md) | 2 | 1 | 1 | 1 | 0 | 0 | 1 |
| 12 | [BlockMobSpawner.cpp](files/BlockMobSpawner.cpp.md) | 2 | 1 | 0 | 2 | 0 | 0 | 1 |
| 12 | [BlockSign.cpp](files/BlockSign.cpp.md) | 2 | 1 | 0 | 2 | 0 | 0 | 1 |
| 12 | [EntityMinecart.cpp](files/EntityMinecart.cpp.md) | 2 | 1 | 0 | 0 | 2 | 0 | 1 |

## Override / back-port debt

- Hand-written overrides in use: **166**
- ❔ Undetermined — re-run with `--pristine <dir>` to classify: **166**

## Legend

Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), `lint` (known crash/leak pattern), `build` (compiler error), `override` (hand-written file).
