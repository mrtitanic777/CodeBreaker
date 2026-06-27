# Conversion Report

- **Profile:** rewrite (j2x)
- **Output Dir:** C:\Users\mrtit\Documents\Visual Studio 2010\Projects\MinecraftAlpha\ConversionTool\_canon

## Totals

| Severity | Count |
|---|---|
| 🔴 HIGH | 0 |
| 🟡 MED  | 441 |
| ⚪ LOW  | 806 |
| **Total events** | **1247** |

| Category | Count | | What it means |
|---|---:|---|---|
| `RISK` | 325 | █████············· | matches a known console-crash / leak pattern |
| `OVERRIDDEN` | 122 | ██················ | a hand-written file is in use for this class |
| `ANNOTATED` | 4 | ·················· | a note about a decision already taken |
| `PORTED` | 779 | ███████████······· | translated literally and cleanly |
| `RULE` | 17 | ·················· | an automatic fix rule fired |

## Files by risk (inspect top-down)

286 files have findings; 0 carry HIGH-severity risk.

| Risk | File | 🔴 | 🟡 | guess | stub | risk | err | override |
|---:|---|---:|---:|---:|---:|---:|---:|---:|
| 30 | [ModelMinecart.cpp](files/ModelMinecart.cpp.md) | 0 | 15 | 0 | 0 | 15 | 0 | 0 |
| 26 | [ModelBoat.cpp](files/ModelBoat.cpp.md) | 0 | 13 | 0 | 0 | 13 | 0 | 0 |
| 22 | [World.cpp](files/World.cpp.md) | 0 | 11 | 0 | 0 | 10 | 0 | 1 |
| 20 | [RenderBlocks.cpp](files/RenderBlocks.cpp.md) | 0 | 10 | 0 | 0 | 10 | 0 | 0 |
| 14 | [Block.cpp](files/Block.cpp.md) | 0 | 7 | 0 | 0 | 6 | 0 | 1 |
| 12 | [InventoryPlayer.cpp](files/InventoryPlayer.cpp.md) | 0 | 6 | 0 | 0 | 6 | 0 | 0 |
| 10 | [Entity.cpp](files/Entity.cpp.md) | 0 | 5 | 0 | 0 | 5 | 0 | 0 |
| 8 | [ChunkProviderLoadOrGenerate.cpp](files/ChunkProviderLoadOrGenerate.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 8 | [EntityArrow.cpp](files/EntityArrow.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 8 | [EntitySnowball.cpp](files/EntitySnowball.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 8 | [Explosion.cpp](files/Explosion.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 8 | [ItemStack.cpp](files/ItemStack.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 8 | [ModelGhast.cpp](files/ModelGhast.cpp.md) | 0 | 4 | 0 | 0 | 4 | 0 | 0 |
| 6 | [CraftingInventoryPlayerCB.cpp](files/CraftingInventoryPlayerCB.cpp.md) | 0 | 3 | 0 | 0 | 3 | 0 | 0 |
| 6 | [CraftingManager.cpp](files/CraftingManager.cpp.md) | 0 | 3 | 0 | 0 | 3 | 0 | 0 |
| 6 | [EntityPainting.cpp](files/EntityPainting.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 6 | [GuiMainMenu.cpp](files/GuiMainMenu.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 6 | [Item.cpp](files/Item.cpp.md) | 0 | 3 | 0 | 0 | 3 | 0 | 0 |
| 6 | [ModelRenderer.cpp](files/ModelRenderer.cpp.md) | 0 | 3 | 0 | 0 | 3 | 0 | 0 |
| 6 | [PlayerControllerSP.cpp](files/PlayerControllerSP.cpp.md) | 0 | 3 | 0 | 0 | 3 | 0 | 0 |
| 6 | [RenderManager.cpp](files/RenderManager.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 6 | [TileEntityRenderer.cpp](files/TileEntityRenderer.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 6 | [WorldGenBigTree.cpp](files/WorldGenBigTree.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 6 | [WorldRenderer.cpp](files/WorldRenderer.cpp.md) | 0 | 3 | 0 | 0 | 2 | 0 | 1 |
| 4 | [BlockChest.cpp](files/BlockChest.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [BlockFire.cpp](files/BlockFire.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [BlockFlowing.cpp](files/BlockFlowing.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [CanvasIsomPreview.cpp](files/CanvasIsomPreview.cpp.md) | 0 | 2 | 0 | 0 | 1 | 0 | 1 |
| 4 | [ChunkCache.cpp](files/ChunkCache.cpp.md) | 0 | 2 | 0 | 0 | 1 | 0 | 1 |
| 4 | [ChunkProviderGenerate.cpp](files/ChunkProviderGenerate.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [ChunkProviderHell.cpp](files/ChunkProviderHell.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [ClippingHelperImplementation.cpp](files/ClippingHelperImplementation.cpp.md) | 0 | 2 | 0 | 0 | 1 | 0 | 1 |
| 4 | [ColorizerFoliage.cpp](files/ColorizerFoliage.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [ColorizerGrass.cpp](files/ColorizerGrass.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [CraftingInventoryWorkbenchCB.cpp](files/CraftingInventoryWorkbenchCB.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [EffectRenderer.cpp](files/EffectRenderer.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [EntityClientPlayerMP.cpp](files/EntityClientPlayerMP.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [EntityFallingSand.cpp](files/EntityFallingSand.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [EntityFireball.cpp](files/EntityFireball.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |
| 4 | [EntityItem.cpp](files/EntityItem.cpp.md) | 0 | 2 | 0 | 0 | 2 | 0 | 0 |

## Override / back-port debt

- Hand-written overrides in use: **122**
- ❔ Undetermined — re-run with `--pristine <dir>` to classify: **122**

## Legend

Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), `lint` (known crash/leak pattern), `build` (compiler error), `override` (hand-written file).
