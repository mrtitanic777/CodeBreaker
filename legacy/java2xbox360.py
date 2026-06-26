#!/usr/bin/env python3
"""
java2xbox360.py — Java to Xbox 360 C++ Converter (Single Script)

Converts decompiled Minecraft Java source to buildable Xbox 360 C++03 / D3D9.
Supports multiple game versions via --version flag.
No external dependencies. No second script. No old code directory needed.

Usage: python3 java2xbox360.py <java_dir> <output_dir> [--version <name>] [--log <logfile>]

Versions:
  rubydung  — Minecraft RubyDung (default)
  indev     — Minecraft Indev

What it does:
  1. Parses all .java files (type mapping, inheritance, fields, methods, enums)
  2. Pure-logic files — structural C++03 conversion
  3. Platform files — emits complete D3D9 code from embedded templates
  4. New files (Main.cpp, Shaders.cpp, Common.h) — generated from embedded templates
  5. Verifies output, generates build helpers, writes log

Performance (a 377-class version converts in ~1 min instead of ~70):
  - Per-file conversion is fanned out across worker processes (set USE_MULTIPROCESSING=False to disable;
    auto-disabled when frozen). Disjoint output files, so no contention.
  - The field-prefix postprocess pre-filters to field names present in each file and folds them into one
    combined regex per access kind (was O(lines x all-fields)).
  - Optional extra speedup with NO code change: run under PyPy — `pypy java2xbox360.py <in> <out> ...`
    (pure-stdlib: re / os / multiprocessing all supported). The two changes above already do the heavy
    lifting, so PyPy is additive, mainly helping the non-regex parse/codegen overhead.
"""

import os
import re
import sys
import shutil
import logging
import time
from datetime import datetime


def _split_top_args(s):
    """Split a call's argument string on TOP-LEVEL commas (ignoring commas inside
    () or []), so args containing casts/nested calls like '(float)x * y' aren't
    truncated. Used by the GL transform handlers."""
    args, depth, cur = [], 0, ''
    for ch in s:
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        if ch == ',' and depth == 0:
            args.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip():
        args.append(cur.strip())
    return args


# ============================================================================
# LOGGING
# ============================================================================

log = logging.getLogger('java2xbox360')


def setup_logging(log_path):
    log.setLevel(logging.DEBUG)
    log.handlers.clear()
    fh = logging.FileHandler(log_path, mode='w', encoding='utf-8')
    fh.setLevel(logging.DEBUG)
    fh.setFormatter(logging.Formatter('%(asctime)s [%(levelname)-7s] %(message)s', datefmt='%H:%M:%S'))
    log.addHandler(fh)
    ch = logging.StreamHandler(sys.stdout)
    ch.setLevel(logging.INFO)
    ch.setFormatter(logging.Formatter('[%(levelname)-7s] %(message)s'))
    log.addHandler(ch)
    log.info('Log file: %s', log_path)



# ============================================================================
# VERSION PROFILE SYSTEM
# ============================================================================

class VersionProfile:
    """Holds all version-specific conversion data for a Minecraft version."""

    def __init__(self, name, **kwargs):
        self.name = name
        self.skip_files = kwargs.get('skip_files', set())
        self.pure_logic_files = kwargs.get('pure_logic_files', set())
        self.platform_file_classes = kwargs.get('platform_file_classes', set())
        self.required_output_files = kwargs.get('required_output_files', [])
        self.value_types = kwargs.get('value_types', set())
        self.reference_types = kwargs.get('reference_types', set())
        self.interface_types = kwargs.get('interface_types', set())
        self.static_class_fields = kwargs.get('static_class_fields', {})
        self.parent_fields = kwargs.get('parent_fields', {})
        self.bare_fields = kwargs.get('bare_fields', set())
        self.bare_static_fields = kwargs.get('bare_static_fields', set())
        self.extra_type_map = kwargs.get('extra_type_map', {})
        self.extra_gl_map = kwargs.get('extra_gl_map', {})
        self.extra_math_map = kwargs.get('extra_math_map', {})
        self.post_fix_hooks = kwargs.get('post_fix_hooks', [])
        # Header-only structs that should NOT generate .cpp files
        self.header_only_structs = kwargs.get('header_only_structs', set())
        # Classes to convert to struct in output
        self.struct_classes = kwargs.get('struct_classes', set())


# ---- RubyDung Profile ----

_RUBYDUNG_PROFILE = VersionProfile(
    'rubydung',
    skip_files={
        'CanvasMinecraftApplet.java', 'MinecraftApplet.java',
        'MinecraftError.java', 'EnumOS.java', 'EnumOSMappingHelper.java',
        'FilenameFilterLevel.java', 'OpenGlCapsChecker.java',
        'ThreadDownloadImage.java', 'ThreadDownloadImageData.java',
        'ThreadDownloadResources.java', 'ThreadSleepForever.java',
        'Session.java',
    },
    pure_logic_files={'Entity', 'Level', 'LevelListener', 'Tile', 'Timer'},
    platform_file_classes={
        'Chunk', 'Cube', 'Frustum', 'LevelRenderer', 'Player',
        'Polygon', 'RubyDung', 'Tesselator', 'Textures', 'Zombie',
    },
    required_output_files=[
        'Main.cpp', 'RubyDung.cpp', 'RubyDung.h', 'Common.h',
        'stdafx.h', 'stdafx.cpp',
        'Shaders.cpp', 'Shaders.h', 'Tesselator.cpp', 'Tesselator.h',
        'Textures.cpp', 'Textures.h',
        'Level.cpp', 'Level.h', 'LevelRenderer.cpp', 'LevelRenderer.h',
        'Chunk.cpp', 'Chunk.h', 'Tile.cpp', 'Tile.h',
        'Entity.cpp', 'Entity.h', 'Player.cpp', 'Player.h',
        'Zombie.cpp', 'Zombie.h',
        'Cube.cpp', 'Cube.h', 'Polygon.cpp', 'Polygon.h',
        'AABB.h', 'Vec3.h', 'Vertex.h', 'HitResult.h', 'Timer.h',
        'Frustum.cpp', 'Frustum.h', 'LevelListener.h',
    ],
    value_types={
        'AABB', 'Vec3', 'HitResult', 'Vertex', 'Polygon',
    },
    reference_types={
        'Level', 'LevelRenderer', 'Chunk', 'Cube', 'Tile',
        'Player', 'Zombie', 'Frustum', 'Tesselator', 'Textures',
        'RubyDung', 'Random',
    },
    interface_types={
        'LevelListener',
    },
    static_class_fields={
        'Tile': {'tiles', 'rock', 'grass', 'dirt', 'stoneBrick', 'wood', 'bush'},
        'Chunk': {'rebuiltThisFrame', 't'},
        'Frustum': {'frustum'},
        'Tesselator': {'instance'},
    },
    parent_fields={
        'Entity': {
            'level', 'xo', 'yo', 'zo', 'x', 'y', 'z',
            'xd', 'yd', 'zd', 'yRot', 'xRot', 'bb',
            'onGround', 'removed', 'heightOffset',
            'bbWidth', 'bbHeight',
        },
        'Tile': {'tex'},
    },
    bare_fields={
        'x', 'y', 'z', 'xo', 'yo', 'zo', 'xd', 'yd', 'zd',
        'yRot', 'xRot', 'bb', 'onGround',
        'width', 'height', 'depth',
        'x0', 'y0', 'z0', 'x1', 'y1', 'z1',
        'o', 'f',
        'pos', 'u', 'v',
        'ticks', 'a', 'timeScale', 'fps', 'passedTime',
        'rock', 'grass',
        'rebuiltThisFrame', 'updates',
        'head', 'body', 'arm0', 'arm1', 'leg0', 'leg1',
        'rot', 'timeOffs', 'speed', 'rotA',
        'aabb', 'level',
        't', 'vertices', 'vertexCount',
    },
    bare_static_fields={'rock', 'grass', 'rebuiltThisFrame', 'updates', 't'},
    header_only_structs={'AABB', 'HitResult', 'Vec3', 'Vertex'},
    struct_classes={'AABB', 'HitResult', 'Vec3', 'Vertex', 'Polygon'},
)


# ---- Indev Profile ----

_INDEV_PROFILE = VersionProfile(
    'indev',
    skip_files={
        'CanvasMinecraftApplet.java', 'MinecraftApplet.java',
        'MinecraftError.java', 'EnumOS.java', 'EnumOSMappingHelper.java',
        'FilenameFilterLevel.java', 'OpenGlCapsChecker.java',
        'ThreadDownloadImage.java', 'ThreadDownloadImageData.java',
        'ThreadDownloadResources.java', 'ThreadSleepForever.java',
        'Session.java', 'ImageBufferDownload.java',
        'ThreadDownloadImageData.java', 'GuiLevelDialog.java',
        'GuiLoadLevel.java', 'GuiSaveLevel.java',
        'GuiNameLevel.java',  # Network level load/save — not needed on 360
        'IsomCamera.java',  # Isometric screenshot camera — desktop only
    },
    pure_logic_files={
        # Core game logic — no GL, pure data
        'AxisAlignedBB', 'Vec3D', 'MovingObjectPosition',
        'Block', 'BlockBookshelf', 'BlockBreakable', 'BlockChest',
        'BlockContainer', 'BlockCrops', 'BlockDirt', 'BlockFarmland',
        'BlockFire', 'BlockFlower', 'BlockFlowing', 'BlockFluid',
        'BlockGears', 'BlockGlass', 'BlockGrass', 'BlockLeaves',
        'BlockLeavesBase', 'BlockLog', 'BlockMushroom', 'BlockOre',
        'BlockOreBlock', 'BlockSand', 'BlockSapling', 'BlockSource',
        'BlockSponge', 'BlockStationary', 'BlockStep', 'BlockStone',
        'BlockTNT', 'BlockTorch', 'BlockWorkbench',
        'Entity', 'EntityAnimal', 'EntityArrow', 'EntityCreature',
        'EntityCreeper', 'EntityGiantZombie', 'EntityItem',
        'EntityLiving', 'EntityMob', 'EntityPig', 'EntityPlayer',
        'EntityPlayerSP', 'EntitySheep', 'EntitySkeleton',
        'EntitySpider', 'EntityTNT', 'EntityZombie',
        'EntitySorter', 'EntityMap', 'EntityMapSlot',
        'Item', 'ItemArmor', 'ItemAxe', 'ItemBlock', 'ItemBow',
        'ItemFlintAndSteel', 'ItemFood', 'ItemHoe', 'ItemPickaxe',
        'ItemSeeds', 'ItemSoup', 'ItemSpade', 'ItemSword', 'ItemTool',
        'ItemStack', 'ItemRenderer',
        'World', 'LevelGenerator', 'LevelLoader', 'Light',
        'MobSpawner', 'Pathfinder', 'Path', 'PathEntity', 'PathPoint',
        'PlayerController', 'PlayerControllerCreative', 'PlayerControllerSP',
        'PlayerLoader',
        'Material', 'MaterialLiquid', 'MaterialLogic', 'MaterialTransparent',
        'StepSound', 'StepSoundGlass', 'StepSoundGravel',
        'Timer', 'ChatLine', 'GameSettings', 'KeyBinding',
        'MathHelper', 'MouseHelper',
        'CraftingManager', 'CraftingRecipe',
        'RecipeSorter', 'RecipesArmor', 'RecipesCrafting', 'RecipesFood',
        'RecipesIngots', 'RecipesTools', 'RecipesWeapons',
        'InventoryPlayer', 'InventoryCrafting', 'InventoryCraftResult',
        'InventoryLargeChest',
        'MovementInput', 'MovementInputFromOptions',
        'NBTBase', 'NBTTagByte', 'NBTTagByteArray', 'NBTTagCompound',
        'NBTTagDouble', 'NBTTagEnd', 'NBTTagFloat', 'NBTTagInt',
        'NBTTagList', 'NBTTagLong', 'NBTTagShort', 'NBTTagString',
        'NextTickListEntry', 'ScaledResolution', 'Slot', 'SlotCrafting',
        'SoundManager', 'SoundPool', 'SoundPoolEntry',
        'TileEntity', 'TileEntityChest',
        'MetadataChunkBlock', 'RenderSorter',
        'NoiseGenerator', 'NoiseGeneratorDistort',
        'NoiseGeneratorOctaves', 'NoiseGeneratorPerlin',
        # Interfaces
        'ICamera', 'IInventory', 'IProgressUpdate', 'IWorldAccess',
    },
    platform_file_classes={
        # These need hand-written D3D9 templates (increment 3)
        'Minecraft', 'Tessellator', 'WorldRenderer', 'RenderGlobal',
        'RenderBlocks', 'EntityRenderer', 'FontRenderer',
        'RenderEngine', 'LoadingScreenRenderer',
        'EffectRenderer', 'RenderManager',
        'Gui', 'GuiScreen', 'GuiButton', 'GuiSmallButton',
        'GuiIngame', 'GuiContainer', 'GuiChest', 'GuiCrafting',
        'GuiInventory', 'GuiControls', 'GuiErrorScreen',
        'GuiGameOver', 'GuiIngameMenu', 'GuiMainMenu', 'GuiOptions',
        'Render', 'RenderLiving', 'RenderItem', 'RenderArrow',
        'RenderCreeper', 'RenderEntity', 'RenderGiantZombie',
        'RenderPlayer', 'RenderSheep', 'RenderSpider', 'RenderTNTPrimed',
        'RenderHelper',
        'ModelBase', 'ModelBiped', 'ModelCreeper', 'ModelPig',
        'ModelQuadruped', 'ModelRenderer', 'ModelSheep', 'ModelSheepFur',
        'ModelSkeleton', 'ModelSpider', 'ModelZombie',
        'Frustrum', 'ClippingHelperFaces',
        # Particle FX (all use GL)
        'EntityFX', 'EntityBubbleFX', 'EntityDiggingFX', 'EntityExplodeFX',
        'EntityFlameFX', 'EntityLavaFX', 'EntityPickupFX', 'EntityRainFX',
        'EntitySmokeFX', 'EntitySplashFX',
        # Animated textures (all use GL pixel buffers)
        'TextureFX', 'TextureFlamesFX', 'TextureGearsFX',
        'TextureLavaFX', 'TextureWaterFX', 'TextureWaterFlowFX',
        'TexturedQuad', 'PositionTextureVertex',
    },
    required_output_files=[
        'Main.cpp', 'Common.h',
        'stdafx.h', 'stdafx.cpp',
        'Shaders.cpp', 'Shaders.h',
        'Tesselator.cpp', 'Tesselator.h',
        'Textures.cpp', 'Textures.h',
    ],
    value_types={
        'AxisAlignedBB',
        'KeyBinding', 'ChatLine', 'NextTickListEntry',
        'MetadataChunkBlock', 'SoundPoolEntry',
        'PositionTextureVertex', 'TexturedQuad', 'ScaledResolution',
    },
    reference_types={
        'World', 'Minecraft', 'Entity', 'EntityLiving', 'EntityPlayer',
        'EntityPlayerSP', 'EntityCreature', 'EntityAnimal', 'EntityMob',
        'EntityArrow', 'EntityItem', 'EntityTNT', 'EntityFX',
        'EntityBubbleFX', 'EntityDiggingFX', 'EntityExplodeFX',
        'EntityFlameFX', 'EntityLavaFX', 'EntityPickupFX', 'EntityRainFX',
        'EntitySmokeFX', 'EntitySplashFX', 'EntityCreeper', 'EntityGiantZombie',
        'EntityPig', 'EntitySheep', 'EntitySkeleton', 'EntitySpider',
        'EntityZombie', 'EntitySorter',
        'Block', 'BlockBookshelf', 'BlockBreakable', 'BlockChest',
        'BlockContainer', 'BlockCrops', 'BlockDirt', 'BlockFarmland',
        'BlockFire', 'BlockFlower', 'BlockFlowing', 'BlockFluid',
        'BlockGears', 'BlockGlass', 'BlockGrass', 'BlockLeaves',
        'BlockLeavesBase', 'BlockLog', 'BlockMushroom', 'BlockOre',
        'BlockOreBlock', 'BlockSand', 'BlockSapling', 'BlockSource',
        'BlockSponge', 'BlockStationary', 'BlockStep', 'BlockStone',
        'BlockTNT', 'BlockTorch', 'BlockWorkbench',
        'Item', 'ItemArmor', 'ItemAxe', 'ItemBlock', 'ItemBow',
        'ItemFlintAndSteel', 'ItemFood', 'ItemHoe', 'ItemPickaxe',
        'ItemSeeds', 'ItemSoup', 'ItemSpade', 'ItemSword', 'ItemTool',
        'ItemStack',  # nullable object (empty slots are null) — modeled as pointer
        'Vec3D', 'MovingObjectPosition',  # nullable (getIntermediate/raytrace return null)
        'RenderEngine', 'RenderGlobal', 'RenderBlocks', 'RenderManager',
        'Render', 'RenderLiving', 'RenderItem', 'RenderArrow',
        'RenderCreeper', 'RenderEntity', 'RenderGiantZombie', 'RenderPlayer',
        'RenderSheep', 'RenderSpider', 'RenderTNTPrimed',
        'Tessellator', 'WorldRenderer', 'EntityRenderer', 'ItemRenderer',
        'Gui', 'GuiScreen', 'GuiButton', 'GuiSmallButton', 'GuiIngame',
        'GuiContainer', 'GuiChest', 'GuiCrafting', 'GuiInventory',
        'GuiControls', 'GuiErrorScreen', 'GuiGameOver', 'GuiIngameMenu',
        'GuiLoadLevel', 'GuiMainMenu', 'GuiNameLevel', 'GuiNewLevel',
        'GuiOptions', 'GuiSaveLevel', 'GuiLevelDialog',
        'ModelBase', 'ModelBiped', 'ModelCreeper', 'ModelPig',
        'ModelQuadruped', 'ModelRenderer', 'ModelSheep', 'ModelSheepFur',
        'ModelSkeleton', 'ModelSpider', 'ModelZombie',
        'FontRenderer', 'GameSettings', 'Timer', 'MouseHelper',
        'EffectRenderer', 'EntityMap', 'EntityMapSlot',
        'Light', 'LevelGenerator', 'MobSpawner', 'Pathfinder',
        'PathEntity', 'PlayerController', 'PlayerControllerCreative',
        'PlayerControllerSP', 'PlayerLoader',
        'SoundManager', 'SoundPool',
        'LoadingScreenRenderer', 'TileEntity', 'TileEntityChest',
        'Slot', 'SlotCrafting',  # container slots are objects (nullable, polymorphic) -> pointers
        'PathPoint',  # nullable refs: null sentinels, void* pointMap, new PathPoint, identity
        'Frustrum', 'IsomCamera', 'ClippingHelperFaces',
        'CraftingManager', 'CraftingRecipe',
        'InventoryPlayer', 'InventoryCrafting', 'InventoryCraftResult',
        'InventoryLargeChest',
        'MovementInput', 'MovementInputFromOptions',
        'LevelLoader',
        'TextureFX', 'TextureFlamesFX', 'TextureGearsFX',
        'TextureLavaFX', 'TextureWaterFX', 'TextureWaterFlowFX',
        'Material', 'MaterialLiquid', 'MaterialLogic', 'MaterialTransparent',
        'StepSound', 'StepSoundGlass', 'StepSoundGravel',
        'NoiseGenerator', 'NoiseGeneratorDistort', 'NoiseGeneratorOctaves',
        'NoiseGeneratorPerlin',
        'RecipeSorter', 'RecipesArmor', 'RecipesCrafting', 'RecipesFood',
        'RecipesIngots', 'RecipesTools', 'RecipesWeapons',
        'NBTTagCompound', 'NBTTagList', 'NBTTagFloat', 'NBTTagShort',
        'NBTBase',
        'Random', 'RenderHelper',
    },
    interface_types={
        'IWorldAccess', 'ICamera', 'IInventory', 'IProgressUpdate',
        'Comparator', 'Runnable', 'FilenameFilter',
    },
    static_class_fields={
        'Block': {
            'blocksList', 'opaqueCubeLookup', 'lightValue', 'lightOpacity',
            'fire', 'dirt', 'stone', 'planks', 'grass', 'waterMoving',
            'waterStill', 'lavaMoving', 'lavaStill', 'stairSingle',
            'cobblestone', 'tnt', 'sand', 'gravel', 'oreGold', 'oreIron',
            'oreDiamond', 'oreCoal', 'wood', 'leaves', 'sponge', 'glass',
            'cloth', 'blockGold', 'blockIron', 'blockDiamond',
            'workbench', 'crops', 'tilledField', 'chest', 'cactus',
            'bedrock', 'ice', 'bookShelf', 'mossStone', 'obsidian',
            'torchWood', 'mushroomBrown', 'mushroomRed', 'sapling',
            'flowerYellow', 'flowerRed', 'tickOnLoad',
            'crate', 'stairDouble',
        },
        'Material': {
            'air', 'ground', 'wood', 'rock', 'iron', 'water', 'lava',
            'leaves', 'plants', 'cloth', 'fire', 'sand', 'glass', 'tnt',
        },
        'Item': {
            'itemsList', 'shovelSteel', 'pickaxeSteel', 'axeSteel',
            'flintAndSteel', 'appleRed', 'bow', 'arrow', 'coal',
            'diamond', 'ingotIron', 'ingotGold', 'swordSteel',
            'swordWood', 'shovelWood', 'pickaxeWood', 'axeWood',
            'swordStone', 'shovelStone', 'pickaxeStone', 'axeStone',
            'swordDiamond', 'shovelDiamond', 'pickaxeDiamond', 'axeDiamond',
            'swordGold', 'shovelGold', 'pickaxeGold', 'axeGold',
            'stick', 'bowlEmpty', 'bowlSoup', 'bread', 'leather',
            'feather', 'gunpowder', 'hoeWood', 'hoeStone', 'hoeSteel',
            'hoeDiamond', 'hoeGold', 'wheat', 'seeds', 'porkRaw',
            'porkCooked',
        },
        'Tessellator': {'instance'},
    },
    parent_fields={
        'Block': {
            'blockID', 'blockIndexInTexture', 'blockHardness', 'blockResistance',
            'minX', 'minY', 'minZ', 'maxX', 'maxY', 'maxZ',
            'stepSound', 'blockParticleGravity', 'material', 'slipperiness',
        },
        'Entity': {
            'worldObj', 'posX', 'posY', 'posZ', 'motionX', 'motionY', 'motionZ',
            'rotationYaw', 'rotationPitch', 'prevRotationYaw', 'prevRotationPitch',
            'boundingBox', 'onGround', 'isDead', 'width', 'height',
            'yOffset', 'stepHeight', 'noClip', 'rand', 'ticksExisted',
            'fire', 'fireResistance', 'air', 'maxAir', 'heartsLife',
            'prevPosX', 'prevPosY', 'prevPosZ', 'lastTickPosX', 'lastTickPosY',
            'lastTickPosZ', 'isCollidedHorizontally', 'prevDistanceWalkedModified',
            'distanceWalkedModified', 'canTriggerWalking', 'skinUrl',
            'preventEntitySpawning',
        },
        'EntityCreature': {'pathToEntity', 'playerToAttack', 'hasAttacked'},
        'EntityLiving': {
            'health', 'hurtTime', 'deathTime', 'attackTime', 'heartsHalvesLife',
            'renderYawOffset', 'prevRenderYawOffset', 'isJumping', 'moveForward',
            'moveStrafing', 'randomYawVelocity', 'entityAge', 'attackedAtYaw',
            'texture', 'prevHealth', 'landMovementFactor', 'jumpMovementFactor',
            'defaultPitch', 'unusedFlag', 'moveSpeed', 'attackStrength',
            'scoreValue', 'swingProgress', 'prevSwingProgress', 'isSwinging',
        },
        'EntityFX': {
            'particleTextureIndex', 'particleGravity', 'particleRed',
            'particleGreen', 'particleBlue', 'particleAlpha', 'particleScale',
            'particleMaxAge', 'particleAge', 'particleScale',
            'particleTextureJitterX', 'particleTextureJitterY',
            'motionX1', 'motionY1', 'motionZ1', 'P',
        },
        'TextureFX': {'imageData', 'iconIndex', 'anaglyphEnabled', 'textureId'},
        'Gui': {'zLevel'},
        'GuiScreen': {'mc', 'width', 'height', 'controlList', 'fontRenderer'},
        'Render': {'renderManager', 'shadowSize', 'shadowOpaque'},
        'RenderLiving': {'model'},
        'ModelBase': {'textureWidth', 'textureHeight'},
        'ModelQuadruped': {'head', 'body', 'leg1', 'leg2', 'leg3', 'leg4'},
        'ModelBiped': {'a', 'b', 'c', 'd'},
        'Item': {'maxStackSize', 'maxDamage', 'shiftedIndex', 'iconIndex'},
        'GuiContainer': {'inventorySlots', 'inventoryWidth', 'inventoryHeight',
                         'xSize', 'ySize'},
        'MovementInput': {'moveStrafe', 'moveForward', 'jump'},
        'ItemTool': {'blocksEffectiveAgainst', 'efficiencyOnProperMaterial', 'damageVsEntity'},
        'BlockFluid': {'stillId', 'movingId'},
        'EntityPlayer': {'inventory', 'unusedByte', 'getScore', 'prevCameraYaw', 'cameraYaw'},
    },
    bare_fields={
        # Entity
        'posX', 'posY', 'posZ', 'prevPosX', 'prevPosY', 'prevPosZ',
        'motionX', 'motionY', 'motionZ', 'rotationYaw', 'rotationPitch',
        'prevRotationYaw', 'prevRotationPitch', 'boundingBox', 'onGround',
        'isDead', 'width', 'height', 'yOffset', 'stepHeight', 'noClip',
        'ticksExisted', 'fire', 'worldObj', 'rand',
        'lastTickPosX', 'lastTickPosY', 'lastTickPosZ',
        'isCollidedHorizontally', 'prevDistanceWalkedModified',
        'distanceWalkedModified', 'canTriggerWalking', 'skinUrl',
        'preventEntitySpawning', 'fireResistance', 'air', 'maxAir', 'heartsLife',
        # EntityLiving
        'health', 'hurtTime', 'deathTime', 'attackTime', 'texture',
        'moveForward', 'moveStrafing', 'isJumping', 'moveSpeed',
        'heartsHalvesLife', 'renderYawOffset', 'prevRenderYawOffset',
        'randomYawVelocity', 'entityAge', 'attackedAtYaw', 'prevHealth',
        'landMovementFactor', 'jumpMovementFactor', 'defaultPitch',
        'unusedFlag', 'attackStrength', 'scoreValue',
        'swingProgress', 'prevSwingProgress', 'isSwinging',
        # EntityFX
        'particleTextureIndex', 'particleGravity', 'particleRed',
        'particleGreen', 'particleBlue', 'particleAlpha', 'particleScale',
        'particleMaxAge', 'particleAge',
        'motionX1', 'motionY1', 'motionZ1', 'P',
        # TextureFX
        'imageData', 'anaglyphEnabled', 'textureId',
        # Block
        'blockID', 'blockIndexInTexture', 'material',
        'blockHardness', 'blockResistance', 'blockParticleGravity', 'slipperiness',
        'minX', 'minY', 'minZ', 'maxX', 'maxY', 'maxZ', 'stepSound',
        # Item
        'maxStackSize', 'maxDamage', 'shiftedIndex', 'iconIndex',
        # ItemStack
        'stackSize', 'itemID', 'itemDamage',
        # Gui/GuiScreen
        'zLevel', 'mc', 'controlList', 'fontRenderer',
        # Render
        'renderManager', 'shadowSize', 'shadowOpaque',
        # RenderLiving
        'model',
        # ModelBase
        'textureWidth', 'textureHeight',
        # GuiContainer
        'inventorySlots', 'inventoryWidth', 'inventoryHeight',
        # Tessellator
        'instance',
        # WorldRenderer
        'chunksUpdated', 'needsUpdate', 'isInFrustrum', 'isVisible',
        'isWaitingOnOcclusionQuery', 'glOcclusionQuery',
        # Timer
        'ticks', 'timeScale', 'fps', 'passedTime',
        # Minecraft
        'theWorld', 'thePlayer', 'currentScreen',
        # ModelQuadruped
        'head', 'body', 'leg1', 'leg2', 'leg3', 'leg4',
    },
    bare_static_fields={
        'instance', 'chunksUpdated', 'blocksList', 'itemsList',
    },
    # NOTE: intentionally empty. These value types are struct-ified (see
    # struct_classes) but still have out-of-line method definitions in their
    # .cpp (Vec3D::normalize, AxisAlignedBB::*, etc.). Listing them here would
    # delete those .cpp files and cause unresolved-external link errors.
    header_only_structs=set(),
    struct_classes={
        'AxisAlignedBB',
        'ScaledResolution', 'PositionTextureVertex', 'TexturedQuad',
    },
    extra_gl_map={
        # Indev-specific GL extensions
        'ARBOcclusionQuery.glGenQueriesARB': '// PORT: D3D9 no occlusion queries on 360',
        'ARBOcclusionQuery.glBeginQueryARB': '// PORT: D3D9 no occlusion queries on 360',
        'ARBOcclusionQuery.glEndQueryARB': '// PORT: D3D9 no occlusion queries on 360',
        'ARBOcclusionQuery.glGetQueryObjectivARB': '// PORT: D3D9 no occlusion queries on 360',
        'ARBVertexBufferObject.glBindBufferARB': '// PORT: D3D9 SetStreamSource',
        'ARBVertexBufferObject.glBufferDataARB': '// PORT: D3D9 vertex buffer create+lock',
        'ARBVertexBufferObject.glGenBuffersARB': '// PORT: D3D9 CreateVertexBuffer',
        'GL15.GL_ARRAY_BUFFER': '0 /* PORT: D3D9 vertex buffer */',
        'GL15.GL_STREAM_DRAW': '0 /* PORT: D3D9 D3DUSAGE_DYNAMIC */',
        'GL15.GL_QUERY_COUNTER_BITS': '0 /* PORT: removed */',
        'GLU.gluPerspective': '// PORT: D3D9 D3DXMatrixPerspectiveFovLH',
        'GLU.gluPickMatrix': '// PORT: D3D9 no GL selection',
        'GLContext.getCapabilities': '// PORT: D3D9 caps check',
        'Display.update': '// PORT: D3D9 EndScene + Present',
        'Display.isCloseRequested': 'false /* PORT: XInput quit */',
        'Display.isActive': 'true /* PORT: always active on 360 */',
        'Display.setTitle': '// PORT: no window title on 360',
        'Display.setDisplayMode': '// PORT: D3D9 present params',
        'Display.setFullscreen': '// PORT: always fullscreen on 360',
        'Display.create': '// PORT: D3D9 CreateDevice',
        'Display.destroy': '// PORT: D3D9 Release',
        'Display.getWidth': '1280 /* PORT: 360 resolution */',
        'Display.getHeight': '720 /* PORT: 360 resolution */',
        'Keyboard.isKeyDown': '// PORT: XInputGetState',
        'Keyboard.getEventKey': '// PORT: XInputGetKeystroke',
        'Keyboard.getEventKeyState': '// PORT: XInputGetKeystroke',
        'Keyboard.next': '// PORT: XInputGetKeystroke loop',
        'Mouse.getX': '// PORT: XInput right stick X',
        'Mouse.getY': '// PORT: XInput right stick Y',
        'Mouse.getDX': '// PORT: XInput right stick delta X',
        'Mouse.getDY': '// PORT: XInput right stick delta Y',
        'Mouse.getDWheel': '0 /* PORT: no mouse wheel on 360 */',
        'Mouse.getEventButton': '// PORT: XInput button',
        'Mouse.getEventButtonState': '// PORT: XInput button state',
        'Mouse.next': '// PORT: XInput poll loop',
        'Mouse.isButtonDown': '// PORT: XInput button check',
        'Mouse.setCursorPosition': '// PORT: no cursor on 360',
        'Mouse.setGrabbed': '// PORT: no cursor on 360',
        'Sys.getTime': 'GetTickCount() /* PORT: was LWJGL Sys.getTime */',
        'Sys.getTimerResolution': '1000 /* PORT: ms resolution */',
    },
    extra_type_map={
        'MathHelper': 'MathHelper',
        'Tessellator': 'Tesselator',  # Match Xbox 360 port spelling
        'DecimalFormat': 'char* /* PORT: was DecimalFormat */',
        'Canvas': 'void* /* PORT: was AWT Canvas */',
        'Component': 'void* /* PORT: was AWT Component */',
        'File': 'std::string /* PORT: was java.io.File */',
        'BufferedImage': 'void* /* PORT: was BufferedImage */',
        'ImageObserver': 'void* /* PORT: was ImageObserver */',
        'URL': 'std::string /* PORT: was java.net.URL */',
        'BufferedReader': 'void* /* PORT: was BufferedReader */',
        'InputStreamReader': 'void* /* PORT: was InputStreamReader */',
        'DataInput': 'DataInput*',
        'DataOutput': 'DataOutput*',
        'DataInputStream': 'void* /* PORT: was DataInputStream */',
        'DataOutputStream': 'void* /* PORT: was DataOutputStream */',
        'GZIPInputStream': 'void* /* PORT: was GZIPInputStream */',
        'GZIPOutputStream': 'void* /* PORT: was GZIPOutputStream */',
        'FileInputStream': 'void* /* PORT: was FileInputStream */',
        'FileOutputStream': 'void* /* PORT: was FileOutputStream */',
        'Cursor': 'void* /* PORT: was LWJGL Cursor */',
        'DisplayMode': 'void* /* PORT: was LWJGL DisplayMode */',
        'ContextCapabilities': 'void* /* PORT: was GL caps */',
        'IOException': 'int /* PORT: was IOException */',
        'Exception': 'int /* PORT: was Exception */',
        'LWJGLException': 'int /* PORT: was LWJGLException */',
    },
)

# ---- Profile registry ----

VERSION_PROFILES = {
    'rubydung': _RUBYDUNG_PROFILE,
    'indev': _INDEV_PROFILE,
}

# Active profile — set by main() from --version flag
PROFILE = _RUBYDUNG_PROFILE

# When True (--no-postfix), Phase 5 postfix is skipped to produce a pristine (pre-postfix)
# conversion that can be cached and restored for fast clean postfix-only iterations.
SKIP_POSTFIX = False


def set_active_profile(name):
    """Set the active version profile by name."""
    global PROFILE
    if name not in VERSION_PROFILES:
        print('ERROR: Unknown version "%s". Available: %s' % (name, ', '.join(sorted(VERSION_PROFILES.keys()))))
        sys.exit(1)
    PROFILE = VERSION_PROFILES[name]
    return PROFILE


# ---- Custom version profiles (profiles/*.json) -----------------------------
# Add a NEW Minecraft version without editing this file: drop a JSON in profiles/. Each custom profile
# INHERITS a base ("base": "indev"|"rubydung"|<another loaded profile>, default indev) and MERGES in
# deltas. The profile NAME is the JSON filename (profiles/infdev.json -> "infdev"). Rules:
#   set fields  (value_types, reference_types, skip_files, interface_types, pure_logic_files,
#                platform_file_classes, bare_fields, bare_static_fields, header_only_structs,
#                struct_classes): the JSON list ADDS to the base set; an optional "<field>_remove" list
#                removes entries.
#   list fields (required_output_files): the JSON list EXTENDS the base list.
#   dict-of-set (static_class_fields, parent_fields): merged per key (union of value lists).
#   dict-of-str (extra_type_map, extra_gl_map, extra_math_map): updated (override/add keys).
# See profiles/README.txt for a full example.
_PROFILE_SET_FIELDS = ['skip_files', 'pure_logic_files', 'platform_file_classes', 'value_types',
                       'reference_types', 'interface_types', 'bare_fields', 'bare_static_fields',
                       'header_only_structs', 'struct_classes']
_PROFILE_LIST_FIELDS = ['required_output_files']
_PROFILE_DICTSET_FIELDS = ['static_class_fields', 'parent_fields']
_PROFILE_DICTSTR_FIELDS = ['extra_type_map', 'extra_gl_map', 'extra_math_map']


def profiles_dir():
    """User-editable profiles folder: next to the .exe when frozen, else next to this script."""
    root = os.path.dirname(sys.executable) if getattr(sys, 'frozen', False) \
        else os.path.dirname(os.path.abspath(__file__))
    return os.path.join(root, 'profiles')


def _clone_profile(base, name):
    p = VersionProfile(name)
    for f in _PROFILE_SET_FIELDS:
        setattr(p, f, set(getattr(base, f)))
    for f in _PROFILE_LIST_FIELDS:
        setattr(p, f, list(getattr(base, f)))
    for f in _PROFILE_DICTSET_FIELDS:
        setattr(p, f, dict((k, set(v)) for k, v in getattr(base, f).items()))
    for f in _PROFILE_DICTSTR_FIELDS:
        setattr(p, f, dict(getattr(base, f)))
    p.post_fix_hooks = list(base.post_fix_hooks)
    return p


def _apply_profile_deltas(p, data):
    for f in _PROFILE_SET_FIELDS:
        cur = set(getattr(p, f))
        cur |= set(data.get(f, []) or [])
        cur -= set(data.get(f + '_remove', []) or [])
        setattr(p, f, cur)
    for f in _PROFILE_LIST_FIELDS:
        getattr(p, f).extend(data.get(f, []) or [])
    for f in _PROFILE_DICTSET_FIELDS:
        d = getattr(p, f)
        for k, v in (data.get(f, {}) or {}).items():
            d[k] = set(d.get(k, set())) | set(v)
    for f in _PROFILE_DICTSTR_FIELDS:
        getattr(p, f).update(data.get(f, {}) or {})


def load_external_profiles(directory=None):
    """Load profiles/*.json into VERSION_PROFILES (inherit a base + merge deltas).
    Returns the list of loaded names. Safe to re-call (re-reads from disk)."""
    import json
    directory = directory or profiles_dir()
    loaded = []
    if not os.path.isdir(directory):
        return loaded
    for fn in sorted(os.listdir(directory)):
        if not fn.lower().endswith('.json'):
            continue
        name = os.path.splitext(fn)[0].lower()
        try:
            with open(os.path.join(directory, fn), 'r', encoding='utf-8') as fh:
                data = json.load(fh)
        except Exception as e:                       # noqa: BLE001
            print('WARN: skipping bad profile %s (%s)' % (fn, e))
            continue
        base_name = str(data.get('base', 'indev')).lower()
        base = VERSION_PROFILES.get(base_name) or VERSION_PROFILES.get('indev') or _RUBYDUNG_PROFILE
        p = _clone_profile(base, name)
        _apply_profile_deltas(p, data)
        VERSION_PROFILES[name] = p
        loaded.append(name)
    return loaded


# Load custom profiles at import so both the CLI (--version <name>) and the GUI combo see them.
load_external_profiles()


# ============================================================================
# CONVERTER CORE (structural Java to C++03)
# ============================================================================

# ============================================================================
# TYPE CLASSIFICATION
# ============================================================================

PRIMITIVE_TYPES = {
    'boolean', 'byte', 'short', 'int', 'long', 'float', 'double', 'char',
    'void', 'bool', 'unsigned char',
}

# These delegate to the active PROFILE so existing code keeps working
def _get_value_types():
    return PROFILE.value_types
def _get_reference_types():
    return PROFILE.reference_types
def _get_interface_types():
    return PROFILE.interface_types

VALUE_TYPES = None       # replaced by PROFILE.value_types at call sites
REFERENCE_TYPES = None   # replaced by PROFILE.reference_types at call sites
INTERFACE_TYPES = None   # replaced by PROFILE.interface_types at call sites



def is_primitive(t):
    base = re.sub(r'<.*>', '', t).replace('[]', '').strip()
    return base in PRIMITIVE_TYPES


def is_value_type(t):
    return re.sub(r'<.*>', '', t).replace('[]', '').strip() in PROFILE.value_types


def is_reference_type(t):
    base = re.sub(r'<.*>', '', t).replace('[]', '').strip()
    JAVA_MAPPED_TYPES = {
        'FloatBuffer', 'IntBuffer', 'ByteBuffer',
        'String', 'ArrayList', 'List', 'LinkedList',
        'HashMap', 'HashSet', 'TreeMap',
    }
    if base in JAVA_MAPPED_TYPES:
        return False
    if base in PROFILE.reference_types or base in PROFILE.interface_types:
        return True
    if base and base[0].isupper() and base not in PROFILE.value_types and base not in PRIMITIVE_TYPES:
        return True
    return False


# ============================================================================
# CROSS-CLASS STATIC FIELDS & PARENT FIELDS (for post-processing)
# ============================================================================

# Delegate to active profile — all code referencing these names keeps working
STATIC_CLASS_FIELDS = None  # use PROFILE.static_class_fields
PARENT_FIELDS = None        # use PROFILE.parent_fields

# ============================================================================
# TYPE MAPPING
# ============================================================================

# Base type map — shared across all versions. Profile.extra_type_map is merged at runtime.
_BASE_JAVA_TO_CPP_TYPES = {
    'boolean': 'bool', 'byte': 'unsigned char', 'short': 'short',
    'int': 'int', 'long': 'long long', 'float': 'float',
    'double': 'double', 'char': 'char', 'void': 'void',
    'String': 'std::string',
    'ArrayList': 'std::vector', 'List': 'std::vector',
    'LinkedList': 'std::vector', 'HashMap': 'std::map',
    'HashSet': 'std::set', 'TreeMap': 'std::map',
    'Random': 'Random',
    # Java root/boxed types -> C++ equivalents
    'Object': 'void* /* PORT: was Object */',
    'Integer': 'int', 'Long': 'long long', 'Float': 'float',
    'Double': 'double', 'Boolean': 'bool', 'Byte': 'unsigned char',
    'Short': 'short', 'Character': 'char',
    # Java NIO buffer types -> C equivalents
    'FloatBuffer': 'float* /* PORT: was FloatBuffer */',
    'IntBuffer': 'int* /* PORT: was IntBuffer */',
    'ByteBuffer': 'unsigned char* /* PORT: was ByteBuffer */',
}

# Merged at runtime: base + profile extras
def _get_java_to_cpp_types():
    m = dict(_BASE_JAVA_TO_CPP_TYPES)
    m.update(PROFILE.extra_type_map)
    return m

JAVA_TO_CPP_TYPES = None  # use _get_java_to_cpp_types() — set in init_profile_globals()

MATH_REPLACEMENTS = {
    'Math.abs': 'abs', 'Math.sqrt': 'sqrtf', 'Math.sin': 'sinf',
    'Math.cos': 'cosf', 'Math.tan': 'tanf', 'Math.atan': 'atanf',
    'Math.atan2': 'atan2f', 'Math.floor': 'floorf', 'Math.ceil': 'ceilf',
    'Math.min': 'min', 'Math.max': 'max', 'Math.pow': 'powf',
    'Math.signum': 'signumf', 'Math.round': 'roundf',
    'Math.random()': '((float)::rand() / (float)RAND_MAX)',
    'Math.PI': '3.14159265358979323846f',
    '(float)Math.PI': '3.14159265358979323846f',
    # Indev: MathHelper (Mojang's own math class)
    'MathHelper.sin': 'sinf', 'MathHelper.cos': 'cosf',
    'MathHelper.sqrt_float': 'sqrtf', 'MathHelper.sqrt_double': 'sqrt',
    'MathHelper.floor_float': '(int)floorf', 'MathHelper.floor_double': '(int)floor',
    'MathHelper.abs': 'abs',
}

# Indev Tessellator API -> Xbox 360 Tesselator API
# These are applied as direct string replacements in method bodies
TESSELLATOR_REPLACEMENTS = {
    'Tessellator.instance': 'Tesselator::getInstance()',
    '.startDrawingQuads()': '.init()',
    '.startDrawing(': '.init( /* PORT: draw mode */ ',
    '.addVertexWithUV(': '.vertexUV(',
    '.addVertex(': '.vertex(',
    '.setColorOpaque_F(': '.color3f(',
    '.setColorOpaque_I(': '.colorI(',
    '.setColorOpaque(': '.color3b(',
    '.setColorRGBA_F(': '.color4f(',
    '.setColorRGBA(': '.color4b(',
    '.setNormal(': '.normal(',
    '.draw()': '.flush()',
    '.disableColor()': '.disableColor()',
    '.setTextureUV(': '.tex(',
}

# Indev renderEngine API -> Xbox 360 Textures API
RENDER_ENGINE_REPLACEMENTS = {
    'renderEngine.registerTextureFX(': '// Animated texture FX registered at init  // was: registerTextureFX(',
    'renderEngine.updateDynamicTextures(': '// PORT: animated texture update',
    'renderEngine.refreshTextures(': '// PORT: texture reload',
}

# Java texture path -> Xbox 360 Content folder path
TEXTURE_PATH_MAP = {
    '/terrain.png': 'game:\\\\Content\\\\terrain.png',
    '/char.png': 'game:\\\\Content\\\\mob\\\\char.png',
    '/default.png': 'game:\\\\Content\\\\default.png',
    '/dirt.png': 'game:\\\\Content\\\\dirt.png',
    '/grass.png': 'game:\\\\Content\\\\grass.png',
    '/water.png': 'game:\\\\Content\\\\water.png',
    '/rain.png': 'game:\\\\Content\\\\rain.png',
    '/rock.png': 'game:\\\\Content\\\\rock.png',
    '/shadow.png': 'game:\\\\Content\\\\shadow.png',
    '/clouds.png': 'game:\\\\Content\\\\clouds.png',
    '/particles.png': 'game:\\\\Content\\\\particles.png',
    '/gui/gui.png': 'game:\\\\Content\\\\gui\\\\gui.png',
    '/gui/icons.png': 'game:\\\\Content\\\\gui\\\\icons.png',
    '/gui/items.png': 'game:\\\\Content\\\\gui\\\\items.png',
    '/gui/logo.png': 'game:\\\\Content\\\\gui\\\\logo.png',
    '/gui/inventory.png': 'game:\\\\Content\\\\gui\\\\inventory.png',
    '/gui/container.png': 'game:\\\\Content\\\\gui\\\\container.png',
    '/gui/crafting.png': 'game:\\\\Content\\\\gui\\\\crafting.png',
    '/item/arrows.png': 'game:\\\\Content\\\\item\\\\arrows.png',
    '/item/sign.png': 'game:\\\\Content\\\\item\\\\sign.png',
    '/misc/gear.png': 'game:\\\\Content\\\\misc\\\\gear.png',
    '/misc/gearmiddle.png': 'game:\\\\Content\\\\misc\\\\gearmiddle.png',
    '/mob/creeper.png': 'game:\\\\Content\\\\mob\\\\creeper.png',
    '/mob/pig.png': 'game:\\\\Content\\\\mob\\\\pig.png',
    '/mob/sheep.png': 'game:\\\\Content\\\\mob\\\\sheep.png',
    '/mob/sheep_fur.png': 'game:\\\\Content\\\\mob\\\\sheep_fur.png',
    '/mob/skeleton.png': 'game:\\\\Content\\\\mob\\\\skeleton.png',
    '/mob/spider.png': 'game:\\\\Content\\\\mob\\\\spider.png',
    '/mob/spider_eyes.png': 'game:\\\\Content\\\\mob\\\\spider_eyes.png',
    '/mob/zombie.png': 'game:\\\\Content\\\\mob\\\\zombie.png',
    '/armor/chain_1.png': 'game:\\\\Content\\\\armor\\\\chain_1.png',
    '/armor/chain_2.png': 'game:\\\\Content\\\\armor\\\\chain_2.png',
    '/armor/diamond_1.png': 'game:\\\\Content\\\\armor\\\\diamond_1.png',
    '/armor/diamond_2.png': 'game:\\\\Content\\\\armor\\\\diamond_2.png',
    '/armor/gold_1.png': 'game:\\\\Content\\\\armor\\\\gold_1.png',
    '/armor/gold_2.png': 'game:\\\\Content\\\\armor\\\\gold_2.png',
    '/armor/iron_1.png': 'game:\\\\Content\\\\armor\\\\iron_1.png',
    '/armor/iron_2.png': 'game:\\\\Content\\\\armor\\\\iron_2.png',
    '/terrain/sun.png': 'game:\\\\Content\\\\terrain\\\\sun.png',
    '/terrain/moon.png': 'game:\\\\Content\\\\terrain\\\\moon.png',
}

_BASE_GL_TO_D3D = {
    'GL11.glEnable': '// PORT: D3D9 SetRenderState equivalent',
    'GL11.glDisable': '// PORT: D3D9 SetRenderState equivalent',
    'GL11.glPushMatrix': '// PORT: D3D9 matrix stack push',
    'GL11.glPopMatrix': '// PORT: D3D9 matrix stack pop',
    'GL11.glTranslatef': '// PORT: D3D9 matrix multiply translate',
    'GL11.glRotatef': '// PORT: D3D9 matrix multiply rotate',
    'GL11.glScalef': '// PORT: D3D9 matrix multiply scale',
    'GL11.glColor4f': '// PORT: D3D9 vertex color or material',
    'GL11.glBindTexture': '// PORT: D3D9 SetTexture',
    'GL11.glBlendFunc': '// PORT: D3D9 SetRenderState blend',
    'GL11.glDepthMask': '// PORT: D3D9 SetRenderState Z write',
    'GL11.glColorMask': '// PORT: D3D9 SetRenderState color write',
    'GL11.glMatrixMode': '// PORT: D3D9 matrix mode',
    'GL11.glLoadIdentity': '// PORT: D3D9 SetTransform identity',
    'GL11.glNewList': '// PORT: D3D9 no display lists',
    'GL11.glEndList': '// PORT: D3D9 no display lists',
    'GL11.glCallList': '// PORT: D3D9 no display lists',
    'GL11.glGenLists': '// PORT: D3D9 no display lists',
    'GL11.glClear': '// PORT: D3D9 Clear()',
    'GL11.glLight': '// PORT: D3D9 SetLight',
    'GL11.glBegin': '// PORT: D3D9 no immediate mode',
    'GL11.glEnd': '// PORT: D3D9 no immediate mode',
    'GL11.glVertex3f': '// PORT: D3D9 vertex buffer',
    'GL11.glTexCoord2f': '// PORT: D3D9 vertex buffer UV',
    'GL11.glNormal3f': '// PORT: D3D9 vertex buffer normal',
    'GL11.glPushName': '// PORT: D3D9 no GL selection mode',
    'GL11.glPopName': '// PORT: D3D9 no GL selection mode',
    'GL11.glInitNames': '// PORT: D3D9 no GL selection mode',
    'GL11.glLoadName': '// PORT: D3D9 no GL selection mode',
    'GL11.glSelectBuffer': '// PORT: D3D9 no GL selection mode',
    'GL11.glRenderMode': '// PORT: D3D9 no GL selection mode',
    'GL11.glColor3f': '// PORT: D3D9 vertex color or material',
    'GL11.glViewport': '// PORT: D3D9 SetViewport',
    'GL11.glClearColor': '// PORT: D3D9 Clear color',
    'GL11.glFog': '// PORT: D3D9 SetRenderState fog',
    'GL11.glOrtho': '// PORT: D3D9 orthographic projection',
    'GL11.glFlush': '// PORT: D3D9 Present()',
    'GL11.glLineWidth': '// PORT: D3D9 line width',
    'GL11.glTexImage2D': '// PORT: D3D9 CreateTexture',
    'GL11.glTexParameteri': '// PORT: D3D9 SetSamplerState',
    'GL11.glDeleteLists': '// PORT: D3D9 no display lists',
    'GL11.glGetFloat': '// PORT: D3D9 GetTransform',
    'GL11.glGetInteger': '// PORT: D3D9 GetViewport',
    'GL11.glGetError': '// PORT: D3D9 error checking',
    'GL11.glGenTextures': '// PORT: D3D9 CreateTexture',
    'GL11.glTexSubImage2D': '// PORT: D3D9 texture update',
    'GL11.glAlphaFunc': '// PORT: D3D9 SetRenderState alpha test',
    'GL11.glShadeModel': '// PORT: D3D9 SetRenderState shade mode',
    'GL11.glColorPointer': '// PORT: D3D9 vertex declaration',
    'GL11.glVertexPointer': '// PORT: D3D9 vertex declaration',
    'GL11.glTexCoordPointer': '// PORT: D3D9 vertex declaration',
    'GL11.glEnableClientState': '// PORT: D3D9 vertex declaration',
    'GL11.glDisableClientState': '// PORT: D3D9 vertex declaration',
    'GL11.glDrawArrays': '// PORT: D3D9 DrawPrimitive',
    'GL11.glMultMatrix': '// PORT: D3D9 matrix multiply',
}

# Merged at runtime: base + profile extras
def _get_gl_to_d3d():
    m = dict(_BASE_GL_TO_D3D)
    m.update(PROFILE.extra_gl_map)
    return m

GL_TO_D3D = None  # use _get_gl_to_d3d() — set in init_profile_globals()

# Delegate to active profile
SKIP_FILES = None        # use PROFILE.skip_files
BARE_FIELDS = None       # use PROFILE.bare_fields
BARE_STATIC_FIELDS = None  # use PROFILE.bare_static_fields


def init_profile_globals():
    """Call after set_active_profile() to populate merged global dicts.
    This must be called before any conversion work begins."""
    global JAVA_TO_CPP_TYPES, GL_TO_D3D
    global SKIP_FILES, BARE_FIELDS, BARE_STATIC_FIELDS
    global STATIC_CLASS_FIELDS, PARENT_FIELDS
    global VALUE_TYPES, REFERENCE_TYPES, INTERFACE_TYPES
    global PLATFORM_TEMPLATES
    JAVA_TO_CPP_TYPES = _get_java_to_cpp_types()
    GL_TO_D3D = _get_gl_to_d3d()
    SKIP_FILES = PROFILE.skip_files
    BARE_FIELDS = PROFILE.bare_fields
    BARE_STATIC_FIELDS = PROFILE.bare_static_fields
    STATIC_CLASS_FIELDS = PROFILE.static_class_fields
    PARENT_FIELDS = PROFILE.parent_fields
    VALUE_TYPES = PROFILE.value_types
    REFERENCE_TYPES = PROFILE.reference_types
    INTERFACE_TYPES = PROFILE.interface_types
    # PLATFORM_TEMPLATES rebuilt only if get_platform_templates is available
    # (not available during early module-level init — rebuilt in main())
    try:
        PLATFORM_TEMPLATES = get_platform_templates()
    except NameError:
        pass

# Initialize with default profile immediately so module-level code works
init_profile_globals()


# ============================================================================
# TYPE CONVERTERS
# ============================================================================

def convert_type(java_type, context='value'):
    if not java_type:
        return ''
    orig = java_type.strip()
    if orig.endswith('[]'):
        return convert_type(orig[:-2].strip(), 'value') + '*'
    generic_match = re.match(r'(\w+)<(.+)>', orig)
    if generic_match:
        outer = generic_match.group(1)
        inner = generic_match.group(2)
        cpp_outer = JAVA_TO_CPP_TYPES.get(outer, outer)
        parts = []
        for ip in inner.split(','):
            ip = ip.strip()
            cpp_ip = convert_type(ip, 'value')
            if is_reference_type(ip) and not cpp_ip.endswith('*'):
                cpp_ip += '*'
            parts.append(cpp_ip)
        return '%s<%s>' % (cpp_outer, ', '.join(parts))
    if orig in JAVA_TO_CPP_TYPES:
        return JAVA_TO_CPP_TYPES[orig]
    if orig in ('List', 'ArrayList', 'LinkedList'):
        return 'std::vector<void*> /* PORT: determine element type */'
    return orig


def convert_type_for_field(java_type, is_array):
    base = re.sub(r'<.*>', '', java_type).replace('[]', '').strip()
    cpp = convert_type(java_type, 'field')
    # Check if the core type (ignoring comments) already has a pointer
    cpp_core = re.sub(r'/\*.*?\*/', '', cpp).strip()
    if is_array:
        if not cpp_core.endswith('*'):
            cpp += '*'
        # Arrays of nullable reference types are arrays of pointers (empty slots NULL,
        # polymorphism preserved): ItemStack[] -> ItemStack**. convert_type may already
        # have added one '*', so ensure a SECOND for these types.
        if base in ('ItemStack', 'Item', 'Block') and not cpp.rstrip().endswith('**'):
            cpp += '*'
    if not is_array and is_reference_type(base) and not cpp_core.endswith('*'):
        cpp += '*'
    return cpp


def convert_type_for_param(java_type):
    base = re.sub(r'<.*>', '', java_type).replace('[]', '').strip()
    cpp = convert_type(java_type, 'param')
    if java_type.endswith('[]'):
        # Match convert_type_for_field: ItemStack/Item/Block arrays are arrays-of-pointers
        # (T**), so their params must be T** too (else field T** -> param T* mismatch).
        if base in ('ItemStack', 'Item', 'Block') and not cpp.rstrip().endswith('**'):
            cpp += '*'
        return cpp
    if base == 'String':
        return 'const std::string&'
    cpp_core = re.sub(r'/\*.*?\*/', '', cpp).strip()
    if is_reference_type(base) and not cpp_core.endswith('*'):
        cpp += '*'
    # Value-struct params pass BY VALUE (not 'const T&'): the decompiled code calls
    # non-const accessors on them (e.g. var.squareDistanceTo()), which a const ref
    # rejects. Small math structs (Vec3D/AABB/etc.) — copy cost is negligible.
    return cpp


def convert_type_for_return(java_type):
    if not java_type:
        return ''
    base = re.sub(r'<.*>', '', java_type).replace('[]', '').strip()
    cpp = convert_type(java_type, 'value')
    if java_type.endswith('[]'):
        return cpp
    if base == 'String':
        return 'const std::string&'
    cpp_core = re.sub(r'/\*.*?\*/', '', cpp).strip()
    if is_reference_type(base) and not cpp_core.endswith('*'):
        cpp += '*'
    return cpp



def field_cpp_name(name, is_static):
    # Strip leading underscores to avoid m__name (reserved in C++)
    clean = name.lstrip('_')
    if not clean:
        clean = name  # fallback if name is all underscores
    # Don't double-prefix if the field already starts with m_ or s_
    if clean.startswith('m_') or clean.startswith('s_'):
        return clean
    # Public API fields keep their Java name — no prefix
    if clean in BARE_FIELDS:
        if is_static and clean not in BARE_STATIC_FIELDS:
            return 's_' + clean
        return clean
    # Static fields in bare_static_fields keep their Java name even if not in bare_fields
    if is_static and clean in BARE_STATIC_FIELDS:
        return clean
    prefix = 's_' if is_static else 'm_'
    return prefix + clean


# ============================================================================
# PARSER
# ============================================================================

class JavaField:
    def __init__(self, access, is_static, is_final, type_name, name, default_value, is_array):
        self.access = access
        self.is_static = is_static
        self.is_final = is_final
        self.type_name = type_name
        self.name = name
        self.default_value = default_value
        self.is_array = is_array

class JavaMethod:
    def __init__(self, access, is_static, is_final, is_abstract, return_type,
                 name, params, body_lines, is_constructor):
        self.access = access
        self.is_static = is_static
        self.is_final = is_final
        self.is_abstract = is_abstract
        self.return_type = return_type
        self.name = name
        self.params = params
        self.body_lines = body_lines
        self.is_constructor = is_constructor

class JavaClass:
    def __init__(self):
        self.package = ''
        self.name = ''
        self.parent = ''
        self.interfaces = []
        self.is_abstract = False
        self.is_final = False
        self.is_interface = False
        self.is_enum = False
        self.enum_constants = []  # list of (CONST_NAME, raw_args_str) for Java enums
        self.inherited_fields = []  # [(name, type_name)] non-static fields from parent chain (resolved
        # in run_conversion); lets subclass bodies prefix bare parent-field refs -> m_field.
        self.fields = []
        self.methods = []
        self.imports = []
        self.static_init = []  # body lines of any 'static { }' initializer block(s)


def parse_java_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    content = content.replace('\r\n', '\n').replace('\r', '\n')
    cls = JavaClass()
    pkg = re.search(r'package\s+([\w.]+)\s*;', content)
    if pkg:
        cls.package = pkg.group(1)
    cls.imports = re.findall(r'import\s+([\w.*]+)\s*;', content)
    cm = re.search(
        r'(public\s+)?(abstract\s+)?(final\s+)?(class|interface)\s+(\w+)'
        r'(?:\s+extends\s+(\w+))?(?:\s+implements\s+([\w,\s]+))?', content)
    if cm:
        cls.is_abstract = cm.group(2) is not None
        cls.is_final = cm.group(3) is not None
        cls.is_interface = cm.group(4) == 'interface'
        cls.name = cm.group(5)
        cls.parent = cm.group(6) or ''
        if cm.group(7):
            cls.interfaces = [i.strip() for i in cm.group(7).split(',')]
    if not cls.name:
        # Not a class/interface — maybe a Java enum.
        enm = re.search(r'(public\s+)?enum\s+(\w+)', content)
        if enm:
            cls.is_enum = True
            cls.name = enm.group(2)
            _parse_enum_body(cls, content)
            return cls
    _parse_members(cls, content)
    return cls


def _parse_enum_body(cls, content):
    """Parse a Java enum: its constants (name + optional ctor args), plus any fields/constructor/
    methods after the first ';'. Stored on cls (is_enum, enum_constants, fields, methods)."""
    m = re.search(r'enum\s+' + re.escape(cls.name) + r'[^{]*\{', content)
    if not m:
        return
    start, depth, i = m.end(), 1, m.end()
    while i < len(content) and depth > 0:
        if content[i] == '{':
            depth += 1
        elif content[i] == '}':
            depth -= 1
        i += 1
    body = content[start:i - 1]
    # Split the constants section (up to the first top-level ';') from the members section.
    depth, semi, j, quote = 0, -1, 0, None
    while j < len(body):
        c = body[j]
        if quote:
            if c == quote and body[j - 1] != '\\':
                quote = None
        elif c in '"\'':
            quote = c
        elif c in '([{':
            depth += 1
        elif c in ')]}':
            depth -= 1
        elif c == ';' and depth == 0:
            semi = j
            break
        j += 1
    consts_str = body if semi < 0 else body[:semi]
    members_str = '' if semi < 0 else body[semi + 1:]
    for part in _split_top_level_commas(consts_str):
        part = part.strip()
        if not part:
            continue
        ecm = re.match(r'(\w+)\s*(?:\((.*)\))?\s*$', part, re.S)
        if ecm:
            cls.enum_constants.append((ecm.group(1), (ecm.group(2) or '').strip()))
    if members_str.strip():
        _parse_member_lines(cls, members_str)


def _find_class_body(content):
    m = re.search(r'(class|interface)\s+\w+[^{]*\{', content)
    if not m:
        return ''
    start, depth, i = m.end(), 1, m.end()
    while i < len(content) and depth > 0:
        if content[i] == '{': depth += 1
        elif content[i] == '}': depth -= 1
        i += 1
    return content[start:i-1]


def _parse_members(cls, content):
    body = _find_class_body(content)
    if not body:
        return
    _parse_member_lines(cls, body)


def _parse_member_lines(cls, body):
    lines = body.split('\n')
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if not line or line.startswith('//') or line.startswith('/*') or line.startswith('*'):
            i += 1
            continue
        # Skip control-flow statements that can match the method regex
        if re.match(r'\s*(for|while|if|else\s+if|switch)\s*\(', line):
            i += 1
            # Skip body if brace on this line
            if '{' in line:
                depth = line.count('{') - line.count('}')
                i_start = i
                while i < len(lines) and depth > 0:
                    depth += lines[i].count('{') - lines[i].count('}')
                    i += 1
            continue
        # Static initializer block: 'static {' — capture its body separately so its
        # local declarations aren't misread as class fields. Emitted as initStatics().
        if re.match(r'static\s*\{', line):
            depth = line.count('{') - line.count('}')
            i += 1
            while i < len(lines) and depth > 0:
                bl = lines[i]
                depth += bl.count('{') - bl.count('}')
                if depth > 0:
                    cls.static_init.append(bl)
                else:
                    last = bl.rstrip()
                    if last.endswith('}'):
                        last = last[:-1].rstrip()
                    if last:
                        cls.static_init.append(last)
                i += 1
            continue
        mm = re.match(
            r'(public|protected|private)?\s*(static\s+)?(final\s+)?(abstract\s+)?'
            r'([\w<>\[\],\s]+?\s+)?(\w+)\s*\(([^)]*)\)\s*(?:throws\s+[\w,\s]+)?\s*(\{|;)', line)
        if mm:
            access = mm.group(1) or 'package'
            is_static = mm.group(2) is not None
            is_final = mm.group(3) is not None
            is_abstract = mm.group(4) is not None
            ret = (mm.group(5) or '').strip()
            # Strip Java-only method modifiers that leak into the captured return type (no C++ keyword
            # equivalent): `static synchronized int f()` -> ret was 'synchronized int' -> 'int'. Left in,
            # they break the whole class header (C4430 cascade, e.g. GLAllocation).
            ret = re.sub(r'\b(synchronized|native|transient|volatile|strictfp)\s+', '', ret).strip()
            name = mm.group(6)
            params_str = mm.group(7)
            has_body = mm.group(8) == '{'
            is_ctor = (ret == '' or ret == name or name == cls.name)
            if is_ctor and ret and ret != name and ret != cls.name:
                is_ctor = False
            if is_ctor:
                ret = ''
            params = _parse_params(params_str)
            body_lines = []
            if has_body:
                depth = 1
                i += 1
                while i < len(lines) and depth > 0:
                    bl = lines[i]
                    depth += bl.count('{') - bl.count('}')
                    if depth > 0:
                        body_lines.append(bl)
                    else:
                        last = bl.rstrip()
                        if last.endswith('}'): last = last[:-1].rstrip()
                        if last: body_lines.append(last)
                    i += 1
            else:
                i += 1
            cls.methods.append(JavaMethod(access, is_static, is_final, is_abstract,
                                          ret, name, params, body_lines, is_ctor))
            continue
        fm = re.match(
            r'(public|protected|private)?\s*(static\s+)?(final\s+)?'
            r'([\w<>\[\]]+)\s+(\w+)(?:\s*\[\s*\])?\s*(?:=\s*(.+?))?\s*;', line)
        if fm:
            cls.fields.append(JavaField(
                fm.group(1) or 'package', fm.group(2) is not None,
                fm.group(3) is not None, fm.group(4), fm.group(5),
                fm.group(6), '[]' in line))
            i += 1
            continue
        i += 1


def _parse_params(s):
    s = s.strip()
    if not s:
        return []
    result = []
    for p in s.split(','):
        p = re.sub(r'\bfinal\s+', '', p.strip())
        # Java varargs: float... var0 -> float* var0 (+ int count param)
        vm = re.match(r'(\w+)\.\.\.\s+(\w+)', p)
        if vm:
            result.append(('%s*' % vm.group(1), vm.group(2)))
            result.append(('int', '%s_count' % vm.group(2)))
            continue
        parts = p.rsplit(None, 1)
        if len(parts) == 2:
            result.append((parts[0].strip(), parts[1].strip()))
        elif len(parts) == 1:
            result.append((parts[0].strip(), ''))
    return result


# ============================================================================
# EXPRESSION & BODY CONVERTER
# ============================================================================

def convert_expression_simple(expr):
    if not expr:
        return expr
    r = re.sub(r'\bthis\.', '', expr)
    for jm, cm in sorted(MATH_REPLACEMENTS.items(), key=lambda x: len(x[0]), reverse=True):
        r = r.replace(jm, cm)
    for java_call, cpp_call in TESSELLATOR_REPLACEMENTS.items():
        r = r.replace(java_call, cpp_call)
    r = re.sub(r'\bTessellator\b', 'Tesselator', r)
    r = re.sub(r'\bTesselator\.(\w+)\(', r'Tesselator::\1(', r)
    r = re.sub(r'\bnull\b', 'NULL', r)
    # Java double literals: 0.6D -> 0.6f, 1D -> 1.0f
    r = re.sub(r'(\d+\.\d+)[dD]\b', r'\1f', r)
    r = re.sub(r'(?<![\w.])(\d+)[dD]\b', r'\1.0f', r)
    # Java float literals: ensure F suffix
    r = re.sub(r'(\d+\.\d+)[fF]\b', r'\1f', r)
    return r


def convert_default_value(value, type_name):
    if value is None:
        return None
    v = value.strip()
    v = re.sub(r'\bnull\b', 'NULL', v)
    # Java platform calls in field initializers
    v = v.replace('System.currentTimeMillis()', 'GetTickCount()')
    v = v.replace('System.nanoTime()', 'GetNanoTime()')

    # BufferUtils.createFloatBuffer(N) -> new float[N]
    bm = re.match(r'BufferUtils\.createFloatBuffer\((\d+)\)', v)
    if bm:
        return 'new float[%s]' % bm.group(1)
    bm = re.match(r'BufferUtils\.createIntBuffer\((\d+)\)', v)
    if bm:
        return 'new int[%s]' % bm.group(1)
    bm = re.match(r'BufferUtils\.create\w+Buffer\((\d+)\)', v)
    if bm:
        return 'new unsigned char[%s]' % bm.group(1)

    if v.startswith('new '):
        nm = re.match(r'new\s+(\w+)(?:<[^>]*>)?\s*\(([^)]*)\)', v)
        if nm:
            cn, args = nm.group(1), nm.group(2)
            if cn in ('ArrayList', 'LinkedList', 'HashMap', 'HashSet'):
                return ''
            if is_reference_type(cn):
                return 'new %s(%s)' % (cn, convert_expression_simple(args))
            return '%s(%s)' % (cn, convert_expression_simple(args))
        # new float[6][4] -> (float**)NULL /* PORT: allocate 2D array */
        am2 = re.match(r'new\s+(\w+)\[(.+)\]\[(.+)\]', v)
        if am2:
            return 'NULL /* PORT: allocate %s[%s][%s] */' % (am2.group(1), am2.group(2), am2.group(3))
        am = re.match(r'new\s+(\w+)\[(.+)\]', v)
        if am:
            return 'new %s[%s]' % (convert_type(am.group(1)), am.group(2))
    if v.startswith('"'):
        return v
    v = re.sub(r'(\d+)[lL](?!\w)', r'\1LL', v)
    # Java double literals
    v = re.sub(r'(\d+\.\d+)[dD]\b', r'\1f', v)
    v = re.sub(r'(?<![\w.])(\d+)[dD]\b', r'\1.0f', v)
    cm = re.match(r"'\\u([0-9a-fA-F]{4})'", v)
    if cm:
        return '0x' + cm.group(1)
    return convert_expression_simple(v)


def build_field_lookup(cls):
    lookup = {}
    for f in cls.fields:
        cpp_name = field_cpp_name(f.name, f.is_static)
        base = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
        lookup[f.name] = (cpp_name, f.is_static, is_reference_type(base))
    # Inherited INSTANCE fields (resolved from the parsed parent chain in run_conversion). A subclass
    # body referencing a parent field bare (e.g. Entity.posX inside EntityMinecart, Block.blockMaterial
    # inside BlockFurnace) must prefix it to m_posX, exactly like an own field. Own fields win on a name
    # clash; statics are excluded (they need Class:: qualification, handled separately).
    for name, type_name in getattr(cls, 'inherited_fields', []):
        if name in lookup:
            continue
        base = re.sub(r'<.*>', '', type_name).replace('[]', '').strip()
        lookup[name] = (field_cpp_name(name, False), False, is_reference_type(base))
    return lookup


# ============================================================================
# CONCRETE GL -> D3D9 CODE CONVERTER
# ============================================================================
# Instead of PORT comments, emit real Xbox 360 D3D9 code for GL calls.

def _convert_gl_to_d3d9(line, stripped):
    """Convert an OpenGL call to concrete D3D9 code. Returns new line or None."""
    indent = len(line) - len(line.lstrip())
    pad = ' ' * indent

    # --- glEnable / glDisable ---
    m = re.match(r'.*GL11\.gl(Enable|Disable)\(GL11\.GL_(\w+)\)', stripped)
    if m:
        enable = m.group(1) == 'Enable'
        cap = m.group(2)
        mapping = {
            'BLEND':        ('g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, %s);', 'TRUE', 'FALSE'),
            'ALPHA_TEST':   ('g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, %s);', 'TRUE', 'FALSE'),
            'DEPTH_TEST':   ('g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, %s);', 'TRUE', 'FALSE'),
            'CULL_FACE':    ('g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, %s);', 'D3DCULL_CW', 'D3DCULL_NONE'),
            'TEXTURE_2D':   ('g_pd3dDevice->SetTexture(0, %s);', 'g_terrainTexture /* PORT: bind active texture */', 'NULL'),
            'FOG':          ('Shaders::setFogEnabled(%s);', 'true', 'false'),
            'LIGHTING':     ('// Lighting is shader-based on Xbox 360 (%s)', 'enabled', 'disabled'),
            'NORMALIZE':    ('// Normalize is shader-based on Xbox 360 (%s)', 'enabled', 'disabled'),
            'COLOR_MATERIAL': ('// Color material is shader-based on Xbox 360 (%s)', 'enabled', 'disabled'),
            'LIGHT0':       ('// Light 0 %s — handle in shader constants', 'enabled', 'disabled'),
            'LIGHT1':       ('// Light 1 %s — handle in shader constants', 'enabled', 'disabled'),
        }
        if cap in mapping:
            fmt, on_val, off_val = mapping[cap]
            return pad + fmt % (on_val if enable else off_val)
        return pad + '// PORT: glEnable/Disable %s' % cap

    # --- glBlendFunc ---
    m = re.match(r'.*GL11\.glBlendFunc\(GL11\.GL_(\w+),\s*GL11\.GL_(\w+)\)', stripped)
    if m:
        src_map = {'SRC_ALPHA': 'D3DBLEND_SRCALPHA', 'ONE': 'D3DBLEND_ONE', 'ZERO': 'D3DBLEND_ZERO',
                   'DST_COLOR': 'D3DBLEND_DESTCOLOR', 'ONE_MINUS_SRC_ALPHA': 'D3DBLEND_INVSRCALPHA'}
        dst_map = src_map.copy()
        dst_map['ONE_MINUS_SRC_ALPHA'] = 'D3DBLEND_INVSRCALPHA'
        s = src_map.get(m.group(1), 'D3DBLEND_SRCALPHA')
        d = dst_map.get(m.group(2), 'D3DBLEND_INVSRCALPHA')
        return (pad + 'g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, %s);\n' % s +
                pad + 'g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, %s);' % d)

    # --- glDepthMask ---
    m = re.match(r'.*GL11\.glDepthMask\((true|false)\)', stripped)
    if m:
        val = 'TRUE' if m.group(1) == 'true' else 'FALSE'
        return pad + 'g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, %s);' % val

    # --- glDepthFunc ---
    m = re.match(r'.*GL11\.glDepthFunc\(GL11\.GL_(\w+)\)', stripped)
    if m:
        func_map = {'LEQUAL': 'D3DCMP_LESSEQUAL', 'LESS': 'D3DCMP_LESS',
                    'EQUAL': 'D3DCMP_EQUAL', 'ALWAYS': 'D3DCMP_ALWAYS'}
        return pad + 'g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, %s);' % func_map.get(m.group(1), 'D3DCMP_LESSEQUAL')

    # --- glColorMask ---
    m = re.match(r'.*GL11\.glColorMask\((true|false),\s*(true|false),\s*(true|false),\s*(true|false)\)', stripped)
    if m:
        mask = 0
        if m.group(1) == 'true': mask |= 0x1  # D3DCOLORWRITEENABLE_RED
        if m.group(2) == 'true': mask |= 0x2  # GREEN
        if m.group(3) == 'true': mask |= 0x4  # BLUE
        if m.group(4) == 'true': mask |= 0x8  # ALPHA
        return pad + 'g_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x%X);' % mask

    # --- glColor4f / glColor3f ---
    # Use a greedy balanced capture (.+ to the last ')') so nested-paren args like
    # glColor4f(1,1,1, (MathHelper.sin((float)x)*0.2F+0.4F)*0.5F) are not truncated
    # at the first inner ')'. ([^)]+) would silently drop everything after it.
    m = re.match(r'.*GL11\.glColor4f\((.+)\)', stripped)
    if m:
        args = m.group(1)
        return pad + 'Tesselator::getInstance().color4f(%s);' % args
    m = re.match(r'.*GL11\.glColor3f\((.+)\)', stripped)
    if m:
        args = m.group(1)
        return pad + 'Tesselator::getInstance().color3f(%s);' % args

    # --- glBindTexture ---
    m = re.match(r'.*GL11\.glBindTexture\(GL11\.GL_TEXTURE_2D,\s*(.+)\)', stripped)
    if m:
        tex_id = m.group(1).strip()
        return pad + 'g_pd3dDevice->SetTexture(0, Textures::getById(%s));' % tex_id

    # --- glPushMatrix / glPopMatrix ---
    if 'GL11.glPushMatrix()' in stripped:
        return pad + 'D3DXMATRIX _savedMatrix = Shaders::getWorldMatrix();'
    if 'GL11.glPopMatrix()' in stripped:
        return pad + 'Shaders::setWorldMatrix(_savedMatrix); Shaders::updateShaderConstants();'

    # --- glTranslatef ---  (balanced arg split so '(float)x*y' args aren't truncated)
    m = re.match(r'.*GL11\.glTranslatef\((.+)\)', stripped)
    if m and len(_split_top_args(m.group(1))) == 3:
        x, y, z = _split_top_args(m.group(1))
        return (pad + '{ D3DXMATRIX _t; D3DXMatrixTranslation(&_t, %s, %s, %s);\n' % (x, y, z) +
                pad + '  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_t);\n' +
                pad + '  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }')

    # --- glRotatef ---
    m = re.match(r'.*GL11\.glRotatef\((.+)\)', stripped)
    if m and len(_split_top_args(m.group(1))) == 4:
        angle, ax, ay, az = _split_top_args(m.group(1))
        return (pad + '{ D3DXMATRIX _r; D3DXMatrixRotationAxis(&_r, &D3DXVECTOR3(%s,%s,%s), (%s)*DEG_TO_RAD);\n' % (ax, ay, az, angle) +
                pad + '  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_r);\n' +
                pad + '  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }')

    # --- glScalef ---
    m = re.match(r'.*GL11\.glScalef\((.+)\)', stripped)
    if m and len(_split_top_args(m.group(1))) == 3:
        x, y, z = _split_top_args(m.group(1))
        return (pad + '{ D3DXMATRIX _s; D3DXMatrixScaling(&_s, %s, %s, %s);\n' % (x, y, z) +
                pad + '  D3DXMATRIX _w; D3DXMatrixMultiply(&_w, &Shaders::getWorldMatrix(), &_s);\n' +
                pad + '  Shaders::setWorldMatrix(_w); Shaders::updateShaderConstants(); }')

    # --- glMatrixMode + glLoadIdentity ---
    if 'GL11.glMatrixMode' in stripped:
        return pad + '// Matrix mode handled by Shaders::setWorldMatrix/setViewMatrix/setProjectionMatrix'
    if 'GL11.glLoadIdentity()' in stripped:
        return pad + '{ D3DXMATRIX _id; D3DXMatrixIdentity(&_id); Shaders::setWorldMatrix(_id); }'

    # --- glOrtho ---
    m = re.match(r'.*GL11\.glOrtho\(([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', stripped)
    if m:
        return (pad + '{ D3DXMATRIX _ortho; D3DXMatrixOrthoOffCenterLH(&_ortho, (float)(%s), (float)(%s), (float)(%s), (float)(%s), (float)(%s), (float)(%s));\n'
                % (m.group(1), m.group(2), m.group(4), m.group(3), m.group(5), m.group(6)) +
                pad + '  Shaders::setProjectionMatrix(_ortho); Shaders::updateShaderConstants(); }')

    # --- glViewport ---
    m = re.match(r'.*GL11\.glViewport\(([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', stripped)
    if m:
        return (pad + '{ D3DVIEWPORT9 _vp = {(DWORD)(%s),(DWORD)(%s),(DWORD)(%s),(DWORD)(%s),0.0f,1.0f};\n'
                % (m.group(1), m.group(2), m.group(3), m.group(4)) +
                pad + '  g_pd3dDevice->SetViewport(&_vp); }')

    # --- glClear ---
    # PORT(★ "eraser" bug): respect the ACTUAL GL bits. The converter used to ALWAYS clear
    # TARGET|ZBUFFER, so a Java `glClear(GL_DEPTH_BUFFER_BIT)` (depth-only, e.g. before the
    # first-person hand) wiped the fully-rendered world to the clear color every frame -> terrain
    # "invisible". Map COLOR->TARGET, DEPTH->ZBUFFER, STENCIL->STENCIL; only OR the bits present.
    gcm = re.match(r'.*GL11\.glClear\(([^)]*)\)', stripped)
    if gcm:
        _arg = gcm.group(1)
        _flags = []
        if 'GL_COLOR_BUFFER_BIT' in _arg:   _flags.append('D3DCLEAR_TARGET')
        if 'GL_DEPTH_BUFFER_BIT' in _arg:   _flags.append('D3DCLEAR_ZBUFFER')
        if 'GL_STENCIL_BUFFER_BIT' in _arg: _flags.append('D3DCLEAR_STENCIL')
        if not _flags:
            _flags = ['D3DCLEAR_TARGET', 'D3DCLEAR_ZBUFFER']   # unknown arg -> safe full clear
        return pad + 'g_pd3dDevice->Clear(0, NULL, %s, D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);' % ' | '.join(_flags)

    # --- glClearColor ---
    m = re.match(r'.*GL11\.glClearColor\(([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', stripped)
    if m:
        return pad + '// Clear color set: (%s, %s, %s, %s) — applied in next Clear() call' % (m.group(1), m.group(2), m.group(3), m.group(4))

    # --- glNewList / glEndList / glCallList / glGenLists / glDeleteLists ---
    if 'GL11.glNewList(' in stripped:
        return pad + '// BEGIN display list -> vertex buffer recording'
    if 'GL11.glEndList()' in stripped:
        return pad + '// END display list -> vertex buffer recording'
    if 'GL11.glCallList(' in stripped:
        m_cl = re.match(r'.*GL11\.glCallList\(([^)]+)\)', stripped)
        if m_cl:
            return pad + '// Draw cached vertex buffer for list %s' % m_cl.group(1)
    if 'GL11.glCallLists(' in stripped:
        return pad + '// PORT: batch draw cached vertex buffers'
    if 'GL11.glGenLists(' in stripped:
        m_gl = re.match(r'.*GL11\.glGenLists\(([^)]+)\)', stripped)
        if m_gl:
            return pad + '0 /* PORT: allocate %s vertex buffer slots */' % m_gl.group(1)
    if 'GL11.glDeleteLists(' in stripped:
        return pad + '// PORT: release vertex buffers'

    # --- glTexParameteri ---
    if 'GL11.glTexParameteri(' in stripped:
        return pad + '// Texture params handled by SetSamplerState in init'

    # --- glNormal3f ---
    m = re.match(r'.*GL11\.glNormal3f\(([^)]+)\)', stripped)
    if m:
        return pad + 'Tesselator::getInstance().normal(%s);' % m.group(1)

    # --- glLight (lighting setup) ---
    if 'GL11.glLight(' in stripped:
        return pad + '// Lighting via shader constants on Xbox 360'
    if 'GL11.glLightModel(' in stripped:
        return pad + '// Light model ambient — handled by shader constants'

    # --- glFogf / glFogi ---
    m = re.match(r'.*GL11\.glFog[fi]\(GL11\.GL_(\w+),\s*([^)]+)\)', stripped)
    if m:
        param, val = m.group(1), m.group(2)
        if 'DENSITY' in param:
            return pad + 'Shaders::setFogDensity(%s);' % val
        if 'COLOR' in param:
            return pad + 'Shaders::setFogColor(%s, %s, %s);' % (val, val, val)
        if 'START' in param:
            return pad + '// Fog start = %s — handled by shader' % val
        if 'END' in param:
            return pad + '// Fog end = %s — handled by shader' % val
        if 'MODE' in param:
            return pad + '// Fog mode = %s — handled by shader' % val
        return pad + '// Fog param %s = %s — handled by shader' % (param, val)
    # glFog with buffer arg (fog color)
    if 'GL11.glFog(' in stripped:
        return pad + '// Fog color buffer — set via Shaders::setFogColor()'

    # --- glPixelStorei / glReadPixels (screenshot) ---
    if 'GL11.glPixelStorei(' in stripped:
        return pad + '// Pixel store alignment — not needed on D3D9'
    if 'GL11.glReadPixels(' in stripped:
        return pad + '// PORT: D3D9 screenshot via GetRenderTargetData + LockRect'

    # --- glAlphaFunc ---
    m = re.match(r'.*GL11\.glAlphaFunc\(GL11\.GL_(\w+),\s*([^)]+)\)', stripped)
    if m:
        func_map = {'GREATER': 'D3DCMP_GREATER', 'GEQUAL': 'D3DCMP_GREATEREQUAL'}
        return (pad + 'g_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, %s);\n' % func_map.get(m.group(1), 'D3DCMP_GREATER') +
                pad + 'g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)(255.0f * %s));' % m.group(2))

    # --- glShadeModel ---
    if 'GL11.glShadeModel(' in stripped:
        return pad + '// Shade model is shader-based on Xbox 360'

    # --- Display.update() ---
    if 'Display.update()' in stripped:
        return pad + 'g_pd3dDevice->EndScene(); g_pd3dDevice->Present(NULL, NULL, NULL, NULL);'

    # --- GLU.gluPerspective ---
    m = re.match(r'.*GLU\.gluPerspective\(([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', stripped)
    if m:
        return (pad + '{ D3DXMATRIX _persp; D3DXMatrixPerspectiveFovLH(&_persp, D3DXToRadian((float)(%s)), (float)(%s), (float)(%s), (float)(%s));\n'
                % (m.group(1), m.group(2), m.group(3), m.group(4)) +
                pad + '  Shaders::setProjectionMatrix(_persp); Shaders::updateShaderConstants(); }')

    # --- Vertex array / client state (Tessellator internal) ---
    if 'GL11.glEnableClientState(' in stripped or 'GL11.glDisableClientState(' in stripped:
        return pad + '// Client state handled by vertex declaration'
    if 'GL11.glVertexPointer(' in stripped or 'GL11.glTexCoordPointer(' in stripped or 'GL11.glColorPointer(' in stripped:
        return pad + '// Vertex pointers handled by vertex declaration + DrawPrimitiveUP'
    if 'GL11.glDrawArrays(' in stripped:
        return pad + '// DrawArrays replaced by Tesselator::flush() -> DrawPrimitiveUP'

    # --- Misc ---
    if 'GL11.glFlush()' in stripped:
        return pad + '// glFlush not needed on D3D9'
    if 'GL11.glGetString(' in stripped:
        return pad + '"Xbox 360" /* PORT: was glGetString */'
    if 'GL11.glGetError(' in stripped:
        return pad + '0 /* no GL errors on D3D9 */'
    if 'GL11.glGetFloat(' in stripped or 'GL11.glGetInteger(' in stripped:
        return pad + '// PORT: use D3D9 GetTransform/GetViewport instead'
    if 'GL11.glMultMatrix(' in stripped:
        return pad + '// PORT: multiply into Shaders world matrix'
    if 'GL11.glTexImage2D(' in stripped or 'GL11.glTexSubImage2D(' in stripped:
        return pad + '// PORT: D3D9 texture create/update via LockRect'
    if 'GL11.glGenTextures(' in stripped:
        return pad + '// PORT: D3D9 CreateTexture'
    if 'GL11.glLineWidth(' in stripped:
        return pad + '// Line width not supported on Xbox 360 GPU'
    if 'GL11.glColorMaterial(' in stripped:
        return pad + '// Color material is shader-based on Xbox 360'

    # No match — fall through to old GL_TO_D3D dict for any remaining cases
    for gl, d3d in GL_TO_D3D.items():
        if gl in stripped:
            ipad = len(line) - len(line.lstrip())
            return ' ' * ipad + d3d + '  // was: ' + stripped

    return None


def convert_body_line(line, cls, field_lookup, param_names=None):
    """Convert a single line of Java method body to C++03.
    param_names: set of parameter names for the current method — these must NOT
    be prefixed with m_ even if they share names with fields."""
    if param_names is None:
        param_names = set()
    stripped = line.strip()
    if not stripped or stripped.startswith('//') or stripped.startswith('/*') or stripped.startswith('*'):
        return line
    result = line

    # --- Concrete GL->D3D9 code replacement ---
    d3d_line = _convert_gl_to_d3d9(result, stripped)
    if d3d_line is not None:
        return d3d_line
    # Stray GL constants used as values — map common ones
    if 'GL11.GL_' in result or 'GL15.' in result or 'ARB' in result:
        indent = len(result) - len(result.lstrip())
        # Map specific GL constants to D3D9 values or remove
        gl_const_map = {
            'GL11.GL_FOG_COLOR': '0 /* fog color handled by Shaders */',
            'GL11.GL_COMPILE': '0 /* display list compile */',
            'GL11.GL_QUADS': '4 /* D3DPT_TRIANGLELIST via Tesselator */',
            'GL11.GL_TRIANGLES': '3 /* D3DPT_TRIANGLELIST */',
            'GL11.GL_LINES': '2 /* D3DPT_LINELIST */',
            'GL11.GL_TEXTURE_2D': '0 /* texture stage 0 */',
            'GL11.GL_SRC_ALPHA': 'D3DBLEND_SRCALPHA',
            'GL11.GL_ONE_MINUS_SRC_ALPHA': 'D3DBLEND_INVSRCALPHA',
            'GL11.GL_ONE': 'D3DBLEND_ONE',
            'GL11.GL_MODELVIEW': '0 /* world matrix */',
            'GL11.GL_PROJECTION': '1 /* projection matrix */',
            'GL11.GL_COLOR_BUFFER_BIT': 'D3DCLEAR_TARGET',
            'GL11.GL_DEPTH_BUFFER_BIT': 'D3DCLEAR_ZBUFFER',
            'GL15.GL_QUERY_COUNTER_BITS': '0',
            'GL15.GL_ARRAY_BUFFER': '0',
            'GL15.GL_STREAM_DRAW': '0',
        }
        new_result = result
        replaced = False
        for gl_c, d3d_c in gl_const_map.items():
            if gl_c in new_result:
                new_result = new_result.replace(gl_c, d3d_c)
                replaced = True
        if replaced:
            return new_result
        # ARB occlusion query calls
        if 'ARBOcclusionQuery' in result or 'ARBVertexBufferObject' in result:
            return ' ' * indent + '// Occlusion/VBO query — not used on Xbox 360'
        return ' ' * indent + '// PORT: GL constant -> D3D9  // was: ' + stripped

    tm = re.match(r'(\s*)throw\s+new\s+(\w+)\((.+)\)\s*;', result)
    if tm:
        return '%sDbgPrintF("ERROR [%s]: %%s\\n", %s);' % (tm.group(1), tm.group(2), tm.group(3))
    if stripped.startswith('try {') or stripped == 'try':
        # Keep the try body — just emit the opening brace
        return ' ' * (len(result) - len(result.lstrip())) + '{ // was try'
    if stripped == 'try':
        return ' ' * (len(result) - len(result.lstrip())) + '{ // was try'
    cm = re.match(r'(\s*)\}\s*catch\s*\(\s*(\w+)\s+(\w+)\s*\)\s*\{', result)
    if cm:
        # PORT(★ /EH OFF): the project builds with exceptions disabled, so a catch body is DEAD code --
        # nothing can throw into it. Emitting it as an unconditional `} { ... }` made the catch body
        # ALWAYS run, and a catch containing control flow (e.g. Minecraft::run's `return;`) then exited
        # the method right after init / on frame 1 (the "run() returns before the game loop" bug). Gate
        # it `if(false)` so the body still compiles (locals stay in scope) but never executes. Finally
        # blocks stay UNCONDITIONAL (real cleanup) -- handled separately.
        return '%s} if(false) { // was catch(%s %s)' % (cm.group(1), cm.group(2), cm.group(3))
    # Bare catch without typed variable
    cm2 = re.match(r'(\s*)\}\s*catch\s*\(.*\)\s*\{', result)
    if cm2:
        return cm2.group(1) + '} if(false) { // was catch (gated dead: /EH OFF, catch is unreachable)'
    sm = re.match(r'(\s*)synchronized\s*\(.*\)\s*\{', result)
    if sm:
        return sm.group(1) + '// PORT: synchronized -> EnterCriticalSection / __lwsync()  {'

    # Replace this.fieldName -> m_fieldName (or s_fieldName) BEFORE general this. removal
    # This correctly handles params that shadow fields: this.x = x -> m_x = x
    # BUG FIX: When field_cpp_name is bare (no m_/s_ prefix) and the parameter has
    # the same name, this.field -> field is a self-assignment. Use this->field instead.
    # NOTE: (?!\s*\() guards against rewriting a METHOD CALL as a field. When a
    # class has both a field `x` and a method `x` (common with Java obfuscated
    # single-letter names like `a`), `this.a(...)` is a call and must NOT become
    # `m_a(...)`; the trailing `(` lookahead leaves calls for the this.-strip below
    # to turn into `a(...)`.
    for java_name, (cpp_name, is_static, is_ref) in field_lookup.items():
        if cpp_name in param_names:
            # Bare field name collides with parameter — qualify with this->
            result = re.sub(r'\bthis\.%s\b(?!\s*\()' % re.escape(java_name), 'this->%s' % cpp_name, result)
        else:
            result = re.sub(r'\bthis\.%s\b(?!\s*\()' % re.escape(java_name), cpp_name, result)
    # Also handle parent class fields accessed via this.
    for parent_cls, parent_fields in PARENT_FIELDS.items():
        for pf in parent_fields:
            if pf not in field_lookup:  # only if not already handled as own field
                # Respect BARE_FIELDS — use bare name if field is in the set
                if pf in BARE_FIELDS:
                    cpp_pf = pf
                else:
                    cpp_pf = 'm_%s' % pf
                result = re.sub(r'\bthis\.%s\b(?!\s*\()' % re.escape(pf), cpp_pf, result)
    # Strip any remaining this. references (methods etc)
    result = re.sub(r'\bthis\.', '', result)
    # Java super.method() -> ParentClass::method() (C++ qualified base call)
    if 'super.' in result and cls.parent:
        result = re.sub(r'\bsuper\.(\w+)\s*\(', r'%s::\1(' % cls.parent, result)
        # super.field -> just field (inherited, accessible directly)
        result = re.sub(r'\bsuper\.(\w+)', r'\1', result)
    for jm, cm in sorted(MATH_REPLACEMENTS.items(), key=lambda x: len(x[0]), reverse=True):
        result = result.replace(jm, cm)
    # Indev: Tessellator API -> D3D9 Tesselator
    for java_call, cpp_call in TESSELLATOR_REPLACEMENTS.items():
        result = result.replace(java_call, cpp_call)
    # Indev: Tessellator class name -> Tesselator (Xbox 360 spelling)
    result = re.sub(r'\bTessellator\b', 'Tesselator', result)
    # Fix Tesselator.staticMethod -> Tesselator::staticMethod (C++ static call)
    result = re.sub(r'\bTesselator\.(\w+)\(', r'Tesselator::\1(', result)
    # Indev: renderEngine API -> D3D9 Textures
    for java_call, cpp_call in RENDER_ENGINE_REPLACEMENTS.items():
        if java_call in result:
            result = result.replace(java_call, cpp_call)
    # Indev: renderEngine.getTexture("/path") -> Textures::load("game:\Content\path")
    tex_match = re.search(r'(?:renderEngine\.getTexture|(?:this\.)?loadTexture|(?:this\.)?getTexture)\("([^"]*)"\)', result)
    if tex_match:
        java_path = tex_match.group(1)
        # Handle %%/ prefix (shadow texture)
        clean_path = java_path.replace('%%', '')
        xbox_path = TEXTURE_PATH_MAP.get(clean_path, 'game:\\\\Content' + clean_path.replace('/', '\\\\'))
        result = re.sub(
            r'(?:renderEngine\.getTexture|(?:this\.)?loadTexture|(?:this\.)?getTexture)\("[^"]*"\)',
            'Textures::load("%s")' % xbox_path, result)
    # Indev: renderEngine.bindTexture -> SetTexture
    if 'renderEngine.bindTexture(' in result:
        result = re.sub(r'renderEngine\.bindTexture\(([^)]+)\)',
                       r'g_pd3dDevice->SetTexture(0, Textures::getById(\1))', result)
    result = re.sub(r'\bnull\b', 'NULL', result)
    # Java double literals: 0.6D -> 0.6f, 1D -> 1.0f
    result = re.sub(r'(\d+\.\d+)[dD]\b', r'\1f', result)
    result = re.sub(r'(?<![\w.])(\d+)[dD]\b', r'\1.0f', result)
    result = re.sub(r'(\w+)\s+instanceof\s+(\w+)', r'dynamic_cast<\2*>(\1) != NULL', result)
    result = re.sub(r'\.get\((\w+)\)', r'[\1]', result)
    result = re.sub(r'\.add\(', '.push_back(', result)
    result = re.sub(r'\.remove\((\w+)\)', r'.erase(.begin() + \1) /* PORT: verify remove semantics */', result)
    result = re.sub(r'\.contains\(', '/* PORT: contains check */ .count(', result)
    result = re.sub(r'\.equals\(([^)]+)\)', r' == \1', result)
    result = re.sub(r'\.toString\(\)', ' /* PORT: toString */', result)
    # Bug 1: Map boolean->bool, byte->unsigned char in body code
    result = re.sub(r'\bboolean\b', 'bool', result)
    result = re.sub(r'\bbyte\b(?!\s*swap)', 'unsigned char', result)
    # Map Java String/StringBuilder to std::string in local var declarations
    result = re.sub(r'\bString\b(?!\s*\.)', 'std::string', result)
    result = re.sub(r'\bStringBuilder\b', 'std::string', result)
    # Map remaining Java types in body code
    result = re.sub(r'\bObject\b(?!\s*\.)', 'void*', result)
    result = re.sub(r'\bCharacter\b(?!\s*\.)', 'char', result)
    result = re.sub(r'\bList\b(?!\s*\.)', 'std::vector<void*>', result)
    result = re.sub(r'\bInteger\b(?!\s*\.)', 'int', result)
    result = re.sub(r'\bFloat\b(?!\s*\.)', 'float', result)
    result = re.sub(r'\bShort\b(?!\s*\.)', 'short', result)
    result = re.sub(r'\.length(?!\w|\()', ' /* PORT: array length - use tracked size */', result)
    result = result.replace('String.valueOf', '/* PORT: toString */')
    result = result.replace('System.arraycopy', '/* PORT: memcpy */')
    result = re.sub(r'new\s+ArrayList(?:<[^>]*>)?\s*\(\)', '/* default vector */', result)

    # Bug 2: Java array declarations: Type[] name = new Type[N] -> Type* name = new Type[N]
    # Also handles: int[] name = new int[N], float[] name = new float[N] etc.
    result = re.sub(r'\b(\w+)\[\]\s+(\w+)\s*=\s*new\s+\w+\[([^\]]+)\]',
                    lambda m: '%s* %s = new %s[%s]' % (
                        convert_type(m.group(1)), m.group(2),
                        convert_type(m.group(1)), m.group(3)),
                    result)

    # Fix Java cast-from-collection: ((Type)list[i]).method() -> list[i]->method()
    # This handles the common Java pattern of casting from untyped collection
    result = re.sub(r'\(\((\w+)\)(\w+)\[(\w+)\]\)\.(\w+)', r'\2[\3]->\4', result)

    # Fix: Type var = new Type(...) -> Type* var = new Type(...) for reference types
    new_decl = re.match(r'(\s+)(\w+)\s+(\w+)\s*=\s*new\s+(\w+)\s*\(', result)
    if new_decl:
        indent_str = new_decl.group(1)
        decl_type = new_decl.group(2)
        var_name = new_decl.group(3)
        new_type = new_decl.group(4)
        if is_reference_type(decl_type) and '*' not in result.split('=')[0]:
            result = re.sub(r'^(\s+)%s\s+(%s\s*=\s*new\s)' % (re.escape(decl_type), re.escape(var_name)),
                          r'\1%s* \2' % decl_type, result)

    # Fix constructor delegation: this(args) -> inline the delegated init
    # C++03 doesn't support delegating constructors, so we inline the body.
    # Pattern: this(expr1, expr2, ...) -> PORT comment, but also add the init code
    if re.match(r'\s*this\(', result.strip()):
        indent = len(result) - len(result.lstrip())
        # Extract the args from this(...)
        args_match = re.match(r'\s*this\((.+)\)\s*;?', result.strip())
        if args_match:
            args = args_match.group(1)
            result = ' ' * indent + '// PORT: constructor delegation  // was: ' + result.strip()
            if 'Vec3(' in args:
                vm = re.match(r'(?:new\s+)?Vec3\(([^)]+)\)\s*,\s*(.+)', args)
                if vm:
                    vec_args = vm.group(1)
                    rest = vm.group(2).strip()
                    uv = [x.strip() for x in rest.split(',')]
                    result += '\n' + ' ' * indent + '%s = Vec3(%s);' % (field_cpp_name('pos', False), vec_args)
                    if len(uv) >= 1:
                        result += '\n' + ' ' * indent + '%s = %s;' % (field_cpp_name('u', False), uv[0])
                    if len(uv) >= 2:
                        result += '\n' + ' ' * indent + '%s = %s;' % (field_cpp_name('v', False), uv[1])
            else:
                single_arg = args.strip()
                if re.match(r'^\w+$', single_arg):
                    result += '\n' + ' ' * indent + '%s = %s;' % (field_cpp_name('vertices', False), single_arg)
                    result += '\n' + ' ' * indent + '%s = 4; // PORT: inlined from delegation' % field_cpp_name('vertexCount', False)
        else:
            result = ' ' * indent + '// PORT: constructor delegation  // was: ' + result.strip()

    # PLATFORM REPLACEMENTS: Convert known Java platform calls to Xbox 360 equivalents
    # These fire BEFORE the PORT-comment fallback, emitting real working code.
    # System.nanoTime() -> GetNanoTime() (helper using QueryPerformanceCounter)
    # System.currentTimeMillis() -> GetTickCount()
    if 'System.nanoTime()' in result:
        result = result.replace('System.nanoTime()', 'GetNanoTime()')
    if 'System.currentTimeMillis()' in result:
        result = result.replace('System.currentTimeMillis()', 'GetTickCount()')
    # Runtime.getRuntime() -> stub (memory info not available same way on 360)
    if 'Runtime.getRuntime()' in result or 'getRuntime()' in result:
        result = re.sub(r'Runtime\.getRuntime\(\)\.totalMemory\(\)', '64 * 1024 * 1024 /* 64MB */', result)
        result = re.sub(r'Runtime\.getRuntime\(\)\.freeMemory\(\)', '32 * 1024 * 1024 /* 32MB */', result)
        result = re.sub(r'Runtime\.getRuntime\(\)\.maxMemory\(\)', '64 * 1024 * 1024 /* 64MB */', result)
        result = re.sub(r'(?:Runtime\.)?getRuntime\(\)\.\w+\(\)', '0 /* PORT: Runtime */', result)
    # Java boxing conversions: Character.valueOf('X') -> 'X', Integer.valueOf(x) -> x
    result = re.sub(r'Character\.valueOf\(([^)]*)\)', r'\1', result)
    result = re.sub(r'Integer\.valueOf\(([^)]*)\)', r'\1', result)
    result = re.sub(r'Float\.valueOf\(([^)]*)\)', r'\1', result)
    result = re.sub(r'Short\.valueOf\(([^)]*)\)', r'\1', result)
    result = re.sub(r'Boolean\.valueOf\(([^)]*)\)', r'\1', result)
    # Also handle the char. pattern that results from Character -> char body conversion
    result = re.sub(r'\bchar\.valueOf\(([^)]*)\)', r'\1', result)
    # Comment out a Java Character-method call lowered to `char.x` -- but NEVER touch a
    # path/filename like `/mob/char.png` or `char.png` (the player/default skin): `char`
    # can't be member-accessed in valid C++, so any `char.` inside a string is a path.
    result = re.sub(r'(?<![/\\])\bchar\.(?!h\b|png\b|class\b|gif\b|jpg\b)', '/* PORT: char. */ ', result)
    # String.valueOf(x) -> std::to_string or sprintf stub
    result = re.sub(r'std::string\.valueOf\(([^)]*)\)', r'/* PORT: toString(\1) */', result)
    result = re.sub(r'String\.valueOf\(([^)]*)\)', r'/* PORT: toString(\1) */', result)
    result = re.sub(r'\bvalueOf\(([^)]*)\)', r'/* PORT: valueOf(\1) */', result)

    # PORT: Java-only types in local variable declarations and expressions
    # --- Concrete Java platform type conversions ---
    # Keyboard/Mouse: stub out (XInput handled separately, not through these Java APIs)
    if re.search(r'\bKeyboard\b', result) and '// ' not in result.lstrip()[:4]:
        indent = len(result) - len(result.lstrip())
        result = ' ' * indent + '// STUBBED: ' + result.strip() + '  // XInput replaces Keyboard'
    if re.search(r'\bMouse\b', result) and '// ' not in result.lstrip()[:4]:
        if 'Mouse.setCursorPosition' in result or 'Mouse.setGrabbed' in result or 'Mouse.create' in result or 'Mouse.destroy' in result:
            indent = len(result) - len(result.lstrip())
            result = ' ' * indent + '// STUBBED: ' + result.strip() + '  // no cursor on 360'
        elif 'Mouse.getDX()' in result:
            result = re.sub(r'Mouse\.getDX\(\)', '0 /* XInput right stick */', result)
        elif 'Mouse.getDY()' in result:
            result = re.sub(r'Mouse\.getDY\(\)', '0 /* XInput right stick */', result)
        elif 'Mouse.getX()' in result:
            result = re.sub(r'Mouse\.getX\(\)', '(SCREEN_WIDTH / 2) /* XInput */', result)
        elif 'Mouse.getY()' in result:
            result = re.sub(r'Mouse\.getY\(\)', '(SCREEN_HEIGHT / 2) /* XInput */', result)
        elif 'Mouse.getDWheel()' in result:
            result = re.sub(r'Mouse\.getDWheel\(\)', '0', result)
        elif 'Mouse.isButtonDown(' in result:
            result = re.sub(r'Mouse\.isButtonDown\([^)]*\)', 'false /* XInput */', result)
        elif 'Mouse.getEventButton' in result or 'Mouse.next' in result or 'Mouse.getEventButtonState' in result:
            indent = len(result) - len(result.lstrip())
            result = ' ' * indent + '// STUBBED: ' + result.strip() + '  // XInput replaces Mouse'

    # System.out / System.err -> DbgPrintF
    sout = re.match(r'(\s*)System\.(out|err)\.println\((.+)\)\s*;', result)
    if sout:
        result = '%sDbgPrintF("%%s\\n", %s);' % (sout.group(1), sout.group(3))
    sout2 = re.match(r'(\s*)System\.(out|err)\.print\((.+)\)\s*;', result)
    if sout2:
        result = '%sDbgPrintF("%%s", %s);' % (sout2.group(1), sout2.group(3))

    # Display.* -> concrete stubs
    if 'Display.' in result and '// ' not in result.lstrip()[:4]:
        if 'Display.isCloseRequested()' in result:
            result = re.sub(r'Display\.isCloseRequested\(\)', 'false', result)
        elif 'Display.isActive()' in result:
            result = re.sub(r'Display\.isActive\(\)', 'true', result)
        elif 'Display.getWidth()' in result:
            result = re.sub(r'Display\.getWidth\(\)', 'SCREEN_WIDTH', result)
        elif 'Display.getHeight()' in result:
            result = re.sub(r'Display\.getHeight\(\)', 'SCREEN_HEIGHT', result)
        elif 'Display.update()' in result:
            pass  # already handled by _convert_gl_to_d3d9
        elif re.search(r'Display\.(create|destroy|setTitle|setFullscreen|setDisplayMode|setVSyncEnabled)', result):
            indent = len(result) - len(result.lstrip())
            result = ' ' * indent + '// STUBBED: ' + result.strip() + '  // handled by Main.cpp D3D9 init'

    # Thread.sleep -> Sleep()
    tsm = re.search(r'Thread\.sleep\((\d+)L?\)', result)
    if tsm:
        result = re.sub(r'Thread\.sleep\((\d+)L?\)', r'Sleep(\1)', result)
    elif 'Thread.sleep(' in result:
        result = re.sub(r'Thread\.sleep\(([^)]+)\)', r'Sleep((DWORD)(\1))', result)
    # Other Thread calls -> stub
    if re.search(r'\bnew\s+Thread\b', result):
        indent = len(result) - len(result.lstrip())
        result = ' ' * indent + '// STUBBED: ' + result.strip() + '  // Xbox 360 threading via CreateThread'
    if re.search(r'\bThread\.\w+', result) and 'Sleep' not in result and '// ' not in result.lstrip()[:4]:
        if 'Thread.currentThread' in result:
            result = re.sub(r'Thread\.currentThread\(\)', '0 /* current thread */', result)
        elif 'Thread.yield' in result:
            result = re.sub(r'Thread\.yield\(\)', 'Sleep(0)', result)

    # BufferUtils in method bodies -> concrete allocations
    result = re.sub(r'BufferUtils\.createFloatBuffer\(([^)]+)\)', r'new float[\1]', result)
    result = re.sub(r'BufferUtils\.createIntBuffer\(([^)]+)\)', r'new int[\1]', result)
    result = re.sub(r'BufferUtils\.createByteBuffer\(([^)]+)\)', r'new unsigned char[\1]', result)

    # Remaining Java platform types that genuinely need PORT markers
    JAVA_BODY_TYPES = {
        'DataInputStream': '// PORT: DataInputStream -> Xbox 360 file I/O',
        'DataOutputStream': '// PORT: DataOutputStream -> Xbox 360 file I/O',
        'FileInputStream': '// PORT: FileInputStream -> Xbox 360 file I/O',
        'FileOutputStream': '// PORT: FileOutputStream -> Xbox 360 file I/O',
        'GZIPInputStream': '// PORT: GZIPInputStream -> Xbox 360 decompression',
        'GZIPOutputStream': '// PORT: GZIPOutputStream -> Xbox 360 compression',
        'BufferedImage': '// PORT: BufferedImage -> D3D9 texture loading',
        'ImageIO': '// PORT: ImageIO -> D3D9 texture loading',
        'BufferUtils': '// PORT: BufferUtils -> direct allocation',
    }
    for java_type, port_comment in JAVA_BODY_TYPES.items():
        if java_type in result and '// PORT:' not in result and '// STUBBED:' not in result:
            if re.search(r'\b%s\b' % re.escape(java_type), result):
                indent = len(result) - len(result.lstrip())
                result = ' ' * indent + port_comment + '  // was: ' + result.strip()
                break

    # Handle 'ArrayList varName = new ArrayList()' -> 'std::vector<void*> varName;'
    result = re.sub(r'\bArrayList\s+(\w+)\s*=\s*/\* default vector \*/',
                    r'std::vector<void*> \1 /* PORT: determine element type */',
                    result)
    # Handle bare 'ArrayList varName = ...'
    result = re.sub(r'\bArrayList\b', 'std::vector<void*> /* PORT: type */', result)

    # Fix 'new X()' for value types in ALL contexts (return, local var, assignment, param)
    for vt in VALUE_TYPES:
        if ('new %s(' % vt) in result:
            # Local var: Type var = new Type(...) -> Type var = Type(...)
            # Return: return new Type(...) -> return Type(...)
            # Assignment: x = new Type(...) -> x = Type(...)
            result = re.sub(r'\bnew\s+%s\(' % re.escape(vt), '%s(' % vt, result)
    # Bug 3: Java inline array init: new Type[]{a,b,c,d} -> C++03 temp array
    # Emits a local array declaration and passes the pointer.
    # For use in Polygon() etc. we pass the elements as a stack array.
    def _convert_java_array_init(m):
        type_name = m.group(1)
        elements = m.group(2).strip()
        elem_list = [e.strip() for e in elements.split(',') if e.strip()]
        count = len(elem_list)
        # C++03: can't use compound literals. Emit a static helper comment with the array.
        # The caller (Polygon ctor) expects a Vertex* and count.
        # We'll emit the elements inline and let the postprocessor handle it.
        # Actually, best approach: emit elements as a brace-initialized local.
        # Since these always appear inside addBox() as args to Polygon(), emit:
        #   _tmpN (declared before), passed by pointer
        # For simplicity in auto-gen, use a unique name per call.
        _convert_java_array_init.counter = getattr(_convert_java_array_init, 'counter', 0) + 1
        arr_name = '_arr%d' % _convert_java_array_init.counter
        # We can't declare a variable mid-expression in C++03, so we use a trick:
        # Return a marker that the postprocessor will hoist.
        # Actually simpler: just return the elements as constructor args since
        # Polygon stores them. We need the Polygon ctor to take individual vertices.
        # Best pragmatic fix: emit the array elements with a count marker.
        return '(%s[]){%s} /*_count=%d*/' % (type_name, elements, count)
    _convert_java_array_init.counter = 0
    result = re.sub(r'new\s+(\w+)\[\]\s*\{([^}]+)\}', _convert_java_array_init, result)

    # Apply m_/s_ prefix to own field references (but NOT to method params)
    result = _apply_field_prefixes(result, field_lookup, param_names)

    # Fix pointer field access: m_field. -> m_field-> for pointer fields
    for java_name, (cpp_name, is_static, is_ref) in field_lookup.items():
        if is_ref:
            result = re.sub(r'\b%s\.(?!\d)' % re.escape(cpp_name), cpp_name + '->', result)

    return result


def _apply_field_prefixes(line, field_lookup, param_names=None):
    if param_names is None:
        param_names = set()
    # Detect local variable declarations on this line — don't prefix those
    local_vars = set()
    for decl_match in re.finditer(r'\b(?:int|float|double|long|short|byte|bool|char|unsigned|AABB|Vec3|HitResult)\s+(\w+)', line):
        local_vars.add(decl_match.group(1))
    for_match = re.search(r'for\s*\(\s*(?:int|float|double|long)\s+(\w+)', line)
    if for_match:
        local_vars.add(for_match.group(1))

    skip_names = param_names | local_vars
    result = line
    for java_name in sorted(field_lookup.keys(), key=len, reverse=True):
        if java_name in skip_names:
            continue
        cpp_name, is_static, is_ref = field_lookup[java_name]
        if len(java_name) == 1:
            pattern = r'(?<![.\w])(?<!m_)(?<!s_)\b%s\b(?!\s*\()(?!\w)' % re.escape(java_name)
        else:
            pattern = r'(?<![.\w])(?<!m_)(?<!s_)\b%s\b(?!\s*\()' % re.escape(java_name)
        result = re.sub(pattern, cpp_name, result)
    return result


# ============================================================================
# FILE GENERATORS
# ============================================================================

def get_includes(cls):
    includes, fwd = set(), set()

    # Java-only types that should never appear as includes or forward decls
    JAVA_ONLY_TYPES = {
        'Runnable', 'Comparator', 'FilenameFilter', 'Serializable',
        'Keyboard', 'Mouse', 'Display', 'DisplayMode', 'GLContext',
        'BufferUtils', 'FloatBuffer', 'IntBuffer', 'ByteBuffer',
        'BufferedImage', 'ImageIO', 'DataInputStream', 'DataOutputStream',
        'FileInputStream', 'FileOutputStream', 'GZIPInputStream', 'GZIPOutputStream',
        'File', 'IOException', 'Exception',
        'System', 'Thread', 'Applet', 'Canvas', 'Component', 'JFrame',
        'GLU', 'GL11', 'GL15', 'GL20',
        'Object', 'Integer', 'Long', 'Float', 'Double', 'Boolean',
        'Character', 'Byte', 'Short',
    }

    # Collect all types referenced by this class
    all_types = set()
    field_types = {}  # track which types are used as fields (need full include if value type)
    for f in cls.fields:
        all_types.add(f.type_name)
        base = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
        field_types[base] = True
    for m in cls.methods:
        if m.return_type: all_types.add(m.return_type)
        for pt, _ in m.params: all_types.add(pt)

    for t in all_types:
        bt = re.sub(r'<.*>', '', t).replace('[]', '').strip()
        # Also extract generic type parameter if present (e.g. ArrayList<Zombie> -> Zombie)
        gm = re.search(r'<\s*(\w+)\s*>', t)
        generic_type = gm.group(1) if gm else None

        if bt in JAVA_ONLY_TYPES:
            continue  # skip Java-only types entirely
        if bt in ('ArrayList', 'List', 'LinkedList'): includes.add('<vector>')
        elif bt in ('HashMap', 'HashSet', 'TreeMap'): includes.add('<map>')
        elif bt == 'String': includes.add('<string>')
        elif bt not in PRIMITIVE_TYPES and bt and bt[0].isupper():
            # Value types used as fields need full #include (not forward decl)
            if is_value_type(bt) and bt in field_types:
                includes.add('"%s.h"' % bt)
            else:
                fwd.add(bt)

        # Add forward decl for the generic type parameter too
        if generic_type and generic_type not in JAVA_ONLY_TYPES and generic_type not in PRIMITIVE_TYPES:
            if generic_type[0].isupper():
                fwd.add(generic_type)

    if cls.parent:
        includes.add('"%s.h"' % cls.parent)
        fwd.discard(cls.parent)
    for i in cls.interfaces:
        if i in JAVA_ONLY_TYPES:
            continue  # skip Java-only interfaces
        includes.add('"%s.h"' % i)
        fwd.discard(i)
    fwd.discard(cls.name)
    # Remove any Java-only types that leaked into forward decls
    fwd -= JAVA_ONLY_TYPES
    # Remove any forward decl that contains non-identifier chars (mapped types like void*)
    fwd = {f for f in fwd if re.match(r'^[A-Za-z_]\w*$', f)}
    return sorted(includes), sorted(fwd)


def _enum_field_type(java_type, is_array=False):
    # java.lang.Class has no C++03 equivalent here -> void* (singleton .class refs become NULL/PORT).
    if java_type == 'Class':
        return 'void*'
    return convert_type_for_field(java_type, is_array)


def _enum_param_type(java_type):
    if java_type == 'Class':
        return 'void*'
    return convert_type_for_param(java_type)


def _convert_enum_args(args):
    """Convert one enum constant's ctor argument list to C++ (Foo.class -> NULL/PORT, literals as-is)."""
    if not args.strip():
        return ''
    out = []
    for p in _split_top_level_commas(args):
        p = p.strip()
        cm = re.match(r'(\w+)\.class$', p)
        if cm:
            out.append('NULL /* PORT: %s.class */' % cm.group(1))
        else:
            out.append(convert_expression_simple(p))
    return ', '.join(out)


def generate_enum_header(cls):
    """Java enum -> C++ struct with static singleton instances (pointer identity == Java enum ==)."""
    L = []
    guard = cls.name.upper() + '_H'
    L += ['#ifndef %s' % guard, '#define %s' % guard, '',
          '// Auto-generated from %s.java (Java enum)' % cls.name,
          '// Target: Xbox 360 / C++03 / DirectX 9', '']
    incs, fwds = get_includes(cls)
    for inc in incs:
        L.append('#include %s' % inc)
    if incs:
        L.append('')
    for f in fwds:
        L.append('class %s;' % f)
    if fwds:
        L.append('')
    L.append('// Java enum -> struct of static singleton instances (compared by pointer identity).')
    L.append('struct %s {' % cls.name)
    for f in cls.fields:
        if f.is_static:
            continue
        L.append('\t%s %s;' % (_enum_field_type(f.type_name, f.is_array), f.name))
    L.append('\tint ordinal;        // Java ordinal()')
    L.append('\tstd::string nameStr; // Java name()')
    L.append('')
    ctor = next((m for m in cls.methods if m.is_constructor and m.params), None)
    L.append('\t%s();' % cls.name)
    if ctor:
        ps = ', '.join('%s %s' % (_enum_param_type(pt), pn) for pt, pn in ctor.params)
        L.append('\t%s(%s);' % (cls.name, ps))
    L.append('')
    # Enum constants are static fields -> referenced with the s_ prefix everywhere
    # (matches the static-field reference convention the postfix applies), so emit
    # the C++ symbol as s_<NAME>. nameStr keeps the original Java name() in source.
    for cn, _a in cls.enum_constants:
        L.append('\tstatic %s* s_%s;' % (cls.name, cn))
    L.append('')
    L.append('\tstatic %s* values_[%d];' % (cls.name, len(cls.enum_constants)))
    L.append('\tstatic const int values_count;')
    L.append('\tstatic void initStatics();  // call once at startup (creates the singletons)')
    L.append('};')
    L += ['', '#endif // %s' % guard]
    return '\n'.join(L)


def generate_enum_source(cls):
    L = []
    L.append('#include "stdafx.h"')
    L.append('#include "%s.h"' % cls.name)
    L.append('')
    for cn, _a in cls.enum_constants:
        L.append('%s* %s::s_%s = NULL;' % (cls.name, cls.name, cn))
    L.append('%s* %s::values_[%d] = {0};' % (cls.name, cls.name, len(cls.enum_constants)))
    L.append('const int %s::values_count = %d;' % (cls.name, len(cls.enum_constants)))
    L.append('')
    L.append('%s::%s() : ordinal(0) {}' % (cls.name, cls.name))
    ctor = next((m for m in cls.methods if m.is_constructor and m.params), None)
    if ctor:
        ps = ', '.join('%s %s' % (_enum_param_type(pt), pn) for pt, pn in ctor.params)
        L.append('')
        L.append('%s::%s(%s) : ordinal(0)' % (cls.name, cls.name, ps))
        L.append('{')
        for bl in ctor.body_lines:
            stmt = convert_expression_simple(bl.strip())
            if stmt:
                L.append('\t' + stmt)
        L.append('}')
    L.append('')
    L.append('void %s::initStatics()' % cls.name)
    L.append('{')
    for idx, (cn, args) in enumerate(cls.enum_constants):
        cargs = _convert_enum_args(args)
        L.append('\ts_%s = new %s(%s);' % (cn, cls.name, cargs))
        L.append('\ts_%s->ordinal = %d; s_%s->nameStr = "%s"; values_[%d] = s_%s;'
                 % (cn, idx, cn, cn, idx, cn))
    L.append('}')
    return '\n'.join(L) + '\n'


def generate_header(cls):
    L = []
    guard = cls.name.upper() + '_H'
    L.append('#ifndef %s' % guard)
    L.append('#define %s' % guard)
    L.append('')
    L.append('// Auto-generated from %s.java' % cls.name)
    L.append('// Target: Xbox 360 / C++03 / DirectX 9')
    L.append('')
    incs, fwds = get_includes(cls)
    for i in incs: L.append('#include %s' % i)
    if incs: L.append('')
    for f in fwds: L.append('class %s;' % f)
    if fwds: L.append('')

    if cls.is_interface:
        L.append('class %s {' % cls.name)
        L.append('public:')
        L.append('\tvirtual ~%s() {}' % cls.name)
        for m in cls.methods:
            ret = convert_type_for_return(m.return_type) or 'void'
            ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
            L.append('\tvirtual %s %s(%s) = 0;' % (ret, m.name, ps))
        L.append('};')
        L.append('')
        L.append('#endif // %s' % guard)
        return '\n'.join(L)

    # Java-only interfaces to skip in inheritance
    JAVA_ONLY_BASES = {'Runnable', 'Serializable', 'Comparator', 'FilenameFilter'}

    bases = []
    if cls.parent: bases.append('public %s' % cls.parent)
    for i in cls.interfaces:
        if i not in JAVA_ONLY_BASES:
            bases.append('public %s' % i)
    decl = 'class %s' % cls.name
    if bases: decl += ' : ' + ', '.join(bases)
    L.append(decl + ' {')

    def by_access(items, accs):
        return [x for x in items if x.access in accs]

    # public
    L.append('public:')
    has_ctor = any(m.is_constructor for m in cls.methods)
    has_default_ctor = any(m.is_constructor and len(m.params) == 0 for m in cls.methods)
    if has_ctor:
        # Always add a default constructor if none exists (needed for arrays, value members)
        if not has_default_ctor:
            L.append('\t%s();' % cls.name)
        for m in cls.methods:
            if m.is_constructor:
                ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
                L.append('\t%s(%s);' % (cls.name, ps))
    else:
        L.append('\t%s();' % cls.name)
    L.append('\tvirtual ~%s();' % cls.name)
    if cls.static_init:
        L.append('\tstatic void initStatics();  // run once at startup (was Java static {})')
    L.append('')
    for f in by_access(cls.fields, ['public']):
        ct = convert_type_for_field(f.type_name, f.is_array)
        # Self-referential value type: must be pointer to avoid incomplete type
        base_ft = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
        if base_ft == cls.name and not ct.endswith('*'):
            ct += '*'
        L.append('\t%s%s %s;' % ('static ' if f.is_static else '', ct, field_cpp_name(f.name, f.is_static)))
    if by_access(cls.fields, ['public']): L.append('')
    for m in by_access(cls.methods, ['public']):
        if m.is_constructor: continue
        ret = convert_type_for_return(m.return_type) or 'void'
        ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
        pfx = 'static ' if m.is_static else ''
        virt = '' if m.is_static else 'virtual '
        abst = ' = 0' if m.is_abstract else ''
        L.append('\t%s%s%s %s(%s)%s;' % (pfx, virt, ret, m.name, ps, abst))
    if by_access(cls.methods, ['public']): L.append('')

    # protected
    pf = by_access(cls.fields, ['protected'])
    pm = by_access(cls.methods, ['protected'])
    if pf or pm:
        L.append('protected:')
        for f in pf:
            ct = convert_type_for_field(f.type_name, f.is_array)
            base_ft = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
            if base_ft == cls.name and not ct.endswith('*'):
                ct += '*'
            L.append('\t%s%s %s;' % ('static ' if f.is_static else '', ct, field_cpp_name(f.name, f.is_static)))
        if pf: L.append('')
        for m in pm:
            if m.is_constructor: continue
            ret = convert_type_for_return(m.return_type) or 'void'
            ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
            virt = '' if m.is_static else 'virtual '
            abst = ' = 0' if m.is_abstract else ''
            L.append('\t%s%s %s(%s)%s;' % (virt, ret, m.name, ps, abst))
        if pm: L.append('')

    # private
    pvf = by_access(cls.fields, ['private', 'package'])
    pvm = by_access(cls.methods, ['private', 'package'])
    if pvf or pvm:
        L.append('private:')
        for f in pvf:
            ct = convert_type_for_field(f.type_name, f.is_array)
            base_ft = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
            if base_ft == cls.name and not ct.endswith('*'):
                ct += '*'
            L.append('\t%s%s %s;' % ('static ' if f.is_static else '', ct, field_cpp_name(f.name, f.is_static)))
        if pvf: L.append('')
        for m in pvm:
            if m.is_constructor: continue
            ret = convert_type_for_return(m.return_type) or 'void'
            ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
            L.append('\t%s %s(%s);' % (ret, m.name, ps))
        if pvm: L.append('')

    # Companion length fields for arrays (Java's array.length). The body converter emits
    # '<field>_count' for 'array.length'; declare them here so they resolve. Set at the
    # allocation site by the array-count postfix.
    array_fields = [f for f in cls.fields if f.is_array and not f.is_static]
    if array_fields:
        L.append('public:  // array-length companions')
        for f in array_fields:
            L.append('\tint %s_count;' % field_cpp_name(f.name, f.is_static))
        L.append('')

    L.append('};')
    L.append('')
    L.append('#endif // %s' % guard)
    return '\n'.join(L)


def _split_top_level_commas(s):
    """Split on commas that are NOT nested inside (), [], {} or a string/char literal.
    Used to split a Java array-literal initializer into its elements."""
    parts, depth, buf, i, quote = [], 0, [], 0, None
    while i < len(s):
        c = s[i]
        if quote:
            buf.append(c)
            if c == quote and (i == 0 or s[i-1] != '\\'):
                quote = None
        elif c == '"' or c == "'":
            quote = c; buf.append(c)
        elif c in '([{':
            depth += 1; buf.append(c)
        elif c in ')]}':
            depth -= 1; buf.append(c)
        elif c == ',' and depth == 0:
            parts.append(''.join(buf)); buf = []
        else:
            buf.append(c)
        i += 1
    if buf:
        parts.append(''.join(buf))
    return parts


def generate_source(cls):
    L = []
    L.append('#include "stdafx.h"')
    L.append('// Auto-generated from %s.java' % cls.name)
    L.append('// Target: Xbox 360 / C++03 / DirectX 9')
    L.append('')
    L.append('#include "%s.h"' % cls.name)
    L.append('#include "Common.h"')

    # BUG 20 FIX: .cpp needs full #include for all types that were forward-declared
    # in the .h — the .cpp calls methods through pointers which needs full definitions
    _, fwd_decls = get_includes(cls)
    # Filter out invalid/mapped types that shouldn't generate includes
    SKIP_INCLUDE_TYPES = {
        'Object', 'String', 'Integer', 'Long', 'Float', 'Double', 'Boolean',
        'Byte', 'Short', 'Character', 'System', 'Thread', 'Applet', 'Canvas',
        'Component', 'JFrame', 'Cursor', 'DisplayMode', 'ContextCapabilities',
    }
    for fd in fwd_decls:
        if fd in SKIP_INCLUDE_TYPES or '*' in fd or '/' in fd:
            continue
        L.append('#include "%s.h"' % fd)

    L.append('')

    fl = build_field_lookup(cls)

    # Statics
    # A bare reference to one of this class's OWN static fields inside another static's initializer
    # (e.g. `Block::s_grass = (new Block(..))->setStepSound(soundGrassFootstep)`) must be prefixed to the
    # s_ cpp name. The out-of-line static-def initializer is looked up in class scope, so the unqualified
    # `s_soundGrassFootstep` resolves. Method bodies + initStatics() already get this prefix; the static
    # DEFINITIONS did not -> "undeclared identifier" (hit Block's StepSound footstep constants hard).
    _own_statics = {}
    for _sf in cls.fields:
        if _sf.is_static:
            _cn = field_cpp_name(_sf.name, True)
            if _cn != _sf.name:
                _own_statics[_sf.name] = _cn
    for f in cls.fields:
        if not f.is_static: continue
        ct = convert_type_for_field(f.type_name, f.is_array)
        fn = field_cpp_name(f.name, f.is_static)
        dv = convert_default_value(f.default_value, f.type_name)
        if dv and _own_statics:
            for _jn, _cnn in sorted(_own_statics.items(), key=lambda x: -len(x[0])):
                dv = re.sub(r'(?<![\w.:>])%s\b(?!\s*\()' % re.escape(_jn), _cnn, dv)
        if dv and not dv.startswith('//') and dv != '':
            L.append('%s %s::%s = %s;' % (ct, cls.name, fn, dv))
        elif dv and dv.startswith('//'):
            L.append('%s %s::%s; %s' % (ct, cls.name, fn, dv))
        else:
            L.append('%s %s::%s;' % (ct, cls.name, fn))
    if any(f.is_static for f in cls.fields): L.append('')

    # Constructors
    has_ctor = any(m.is_constructor for m in cls.methods)
    has_default_ctor = any(m.is_constructor and len(m.params) == 0 for m in cls.methods)

    # PORT(★ array-literal field init): C++03 can't fill a `new T[]{...}` from a ctor init list, and the
    # converter otherwise emitted NULL for `int[] x = {0,1,2,3}` fields -> a NULL deref the first time the
    # array is used (e.g. BlockFlowing::m_liquidIntArray, the fluid flow-direction order array -> hang/
    # crash when water flows). Collect such fields and emit alloc + element fill (+ the _count companion)
    # at the TOP of every ctor body instead. See [[converter-runtime-backport]].
    array_lit_map = {}   # f.name -> (cpp_name, elem_cpp_type, [converted_elements])
    for _alf in cls.fields:
        if _alf.is_static or not _alf.default_value:
            continue
        if not (_alf.is_array or '[]' in _alf.type_name):
            continue
        _dvv = _alf.default_value.strip()
        _mlit = re.match(r'^(?:new\s+[\w<>\[\]\s]+)?\{(.*)\}$', _dvv, re.DOTALL)
        if not _mlit:
            continue
        _raw = _mlit.group(1).strip()
        _elems = ([convert_expression_simple(e.strip()) for e in _split_top_level_commas(_raw) if e.strip()]
                  if _raw else [])
        _base = re.sub(r'<.*>', '', _alf.type_name).replace('[]', '').strip()
        array_lit_map[_alf.name] = (field_cpp_name(_alf.name, _alf.is_static), convert_type(_base), _elems)

    def _array_lit_body_lines():
        out = []
        for _k in array_lit_map:
            fn_, et_, elems_ = array_lit_map[_k]
            out.append("\t// PORT(array-literal field init): C++03 has no init-list array fill; alloc + fill here.")
            out.append('\t%s = new %s[%d];' % (fn_, et_, len(elems_)))
            for _i, _e in enumerate(elems_):
                out.append('\t%s[%d] = %s;' % (fn_, _i, _e))
            out.append('\t%s_count = %d;' % (fn_, len(elems_)))
        return out

    def _get_field_init(f):
        """Get initializer for a field. Returns (cpp_name, init_value) or None."""
        if f.is_static:
            return None
        fn = field_cpp_name(f.name, f.is_static)
        if f.name in array_lit_map:
            return (fn, 'NULL')   # alloc + filled in the ctor BODY (C++03 array-literal init)
        dv = convert_default_value(f.default_value, f.type_name)
        cpp_type = convert_type_for_field(f.type_name, f.is_array)
        # PORT(★ console boot crash): a Java `String x = null` (or unset String) field maps to a C++
        # std::string VALUE. Initializing a std::string with NULL invokes std::string(const char* = NULL)
        # -> strlen(NULL) -> access violation on real Xenon hardware (Nexia tolerates the null low-page
        # read, which is why it only crashed on console). Empty string is the correct C++ equivalent of a
        # null Java String. See the 'console-boot-crash' note (was Minecraft::m_loadMapUser(NULL)).
        if cpp_type == 'std::string' and not f.is_array and (dv is None or dv == 'NULL' or dv == ''):
            return (fn, '""')
        if dv and not dv.startswith('//') and dv != '':
            return (fn, dv)
        # Zero-init primitives and pointers that Java auto-zeroes
        base = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
        if base in ('boolean', 'bool'):
            return (fn, 'false')
        elif base in ('byte', 'short', 'int', 'long', 'float', 'double', 'char'):
            return (fn, '0')
        elif f.is_array or is_reference_type(base) or cpp_type.endswith('*'):
            return (fn, 'NULL')
        return None

    # Always generate a default constructor if no explicit default exists
    if not has_default_ctor:
        il = []
        for f in cls.fields:
            init = _get_field_init(f)
            if init:
                il.append('%s(%s)' % init)
        L.append('%s::%s()' % (cls.name, cls.name))
        if il: L.append('\t: ' + '\n\t, '.join(il))
        L.append('{')
        L.extend(_array_lit_body_lines())
        L.append('}')
        L.append('')

    for m in cls.methods:
        if not m.is_constructor: continue
        ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
        il = []
        if cls.parent:
            for bl in m.body_lines:
                # Greedy to the LAST ')' so nested-paren args survive, e.g.
                # 'super(new ModelCreeper(), 0.5F)' -> args 'new ModelCreeper(), 0.5F'.
                sm = re.search(r'super\((.*)\)\s*;?\s*$', bl.strip())
                if sm:
                    il.append('%s(%s)' % (cls.parent, convert_expression_simple(sm.group(1))))
                    break
        for f in cls.fields:
            init = _get_field_init(f)
            if init:
                il.append('%s(%s)' % init)
        L.append('%s::%s(%s)' % (cls.name, cls.name, ps))
        if il: L.append('\t: ' + '\n\t, '.join(il))
        L.append('{')
        L.extend(_array_lit_body_lines())
        # Collect param names so field prefixer doesn't touch them
        ctor_param_names = set(pn for _, pn in m.params if pn)
        ctor_body = [bl for bl in m.body_lines if 'super(' not in bl]
        L.extend(_convert_body_block(ctor_body, cls, fl, ctor_param_names))
        L.append('}')
        L.append('')

    # Destructor
    L.append('%s::~%s() {' % (cls.name, cls.name))
    has_cleanup = False
    for f in cls.fields:
        if f.is_static: continue
        base = re.sub(r'<.*>', '', f.type_name).replace('[]', '').strip()
        fn = field_cpp_name(f.name, f.is_static)
        if f.is_array:
            L.append('\tdelete[] %s;' % fn)
            L.append('\t%s = NULL;' % fn)
            has_cleanup = True
        elif is_reference_type(base):
            # Non-owned pointers: just null them. If you later determine ownership, add delete.
            L.append('\t%s = NULL;' % fn)
            has_cleanup = True
    if not has_cleanup:
        L.append('\t// No dynamic resources')
    L.append('}')
    L.append('')

    # Methods
    for m in cls.methods:
        if m.is_constructor or m.is_abstract: continue
        ret = convert_type_for_return(m.return_type) or 'void'
        ps = ', '.join('%s %s' % (convert_type_for_param(pt), pn) for pt, pn in m.params)
        L.append('%s %s::%s(%s) {' % (ret, cls.name, m.name, ps))
        # Collect param names so field prefixer doesn't touch them
        method_param_names = set(pn for _, pn in m.params if pn)
        if m.body_lines:
            L.extend(_convert_body_block(m.body_lines, cls, fl, method_param_names))
        else:
            # Empty body — provide a sensible default return
            if ret == 'void':
                L.append('\t// TODO: implement')
            elif ret == 'bool':
                L.append('\treturn false; // TODO: implement')
            elif ret in ('int', 'float', 'double', 'short', 'long long'):
                L.append('\treturn 0; // TODO: implement')
            else:
                L.append('\treturn NULL; // TODO: implement')
        L.append('}')
        L.append('')

    # Static initializer block -> ClassName::initStatics() (call once at startup).
    # The block's bare static-field assignments (e.g. 'shovel = var0') are prefixed to
    # their cpp names ('s_shovel'); its local temps convert via the normal passes.
    if cls.static_init:
        static_names = {}
        for f in cls.fields:
            if f.is_static:
                cn = field_cpp_name(f.name, True)
                if cn != f.name:
                    static_names[f.name] = cn
        L.append('void %s::initStatics() {' % cls.name)
        for bl in _convert_body_block(cls.static_init, cls, fl, set()):
            for jname, cname in static_names.items():
                bl = re.sub(r'(?<![\w>.])%s\b' % re.escape(jname), cname, bl)
            L.append(bl)
        L.append('}')
        L.append('')

    return '\n'.join(L)


def _convert_body_block(body_lines, cls, fl, param_names):
    """Convert body lines, commenting out entire if-blocks when the if-condition
    gets PORT-marked (Keyboard, Mouse, Display, etc).
    Also converts Java Iterator patterns to C++ for-loops."""
    result = []
    i = 0
    while i < len(body_lines):
        bl = body_lines[i]

        # ---- Iterator pattern detection ----
        # Pattern: Iterator varN = collection.iterator();
        #          while(varN.hasNext()) {
        #              Type varM = (Type)varN.next();
        iter_match = re.match(r'(\s*)Iterator\s+(\w+)\s*=\s*(.+)\.iterator\(\)\s*;', bl.strip())
        if iter_match:
            indent = bl[:len(bl) - len(bl.lstrip())]
            iter_var = iter_match.group(2)
            collection = iter_match.group(3)
            # Convert collection expression through the normal converter
            collection_conv = convert_body_line(indent + 'x = ' + collection + ';', cls, fl, param_names)
            collection_conv = collection_conv.strip().lstrip('x = ').rstrip(';').strip()
            # Look ahead for while(iterVar.hasNext()) {  — skipping blank lines, which
            # the decompiler often leaves between the iterator decl and the loop.
            j = i + 1
            while j < len(body_lines) and not body_lines[j].strip():
                j += 1
            if j < len(body_lines):
                next_line = body_lines[j].strip()
                while_match = re.match(r'while\s*\(\s*%s\.hasNext\(\)\s*\)\s*\{' % re.escape(iter_var), next_line)
                if while_match:
                    # Look for Type varM = (Type)iterVar.next(); after the while (skip blanks)
                    cast_var = None
                    cast_type = None
                    k = j + 1
                    while k < len(body_lines) and not body_lines[k].strip():
                        k += 1
                    if k < len(body_lines):
                        next2 = body_lines[k].strip()
                        cast_match = re.match(r'(\w+)\s+(\w+)\s*=\s*\((\w+)\)\s*%s\.next\(\)\s*;' % re.escape(iter_var), next2)
                        if cast_match:
                            cast_type = cast_match.group(1)
                            cast_var = cast_match.group(2)
                            # Emit: for (size_t _iN = 0; _iN < collection.size(); ++_iN) {
                            loop_idx = '_i%s' % iter_var.replace('var', '')
                            result.append('%sfor (size_t %s = 0; %s < %s.size(); ++%s) {' % (
                                indent, loop_idx, loop_idx, collection_conv, loop_idx))
                            # Emit: Type* varM = (Type*)collection[_iN];
                            cpp_type = convert_type(cast_type, 'value') if cast_type != 'Object' else 'void*'
                            if cpp_type != 'void*' and is_reference_type(cast_type):
                                result.append('%s\t%s* %s = (%s*)%s[%s];' % (
                                    indent, cpp_type, cast_var, cpp_type, collection_conv, loop_idx))
                            else:
                                result.append('%s\t%s %s = (%s)%s[%s];' % (
                                    indent, cpp_type, cast_var, cpp_type, collection_conv, loop_idx))
                            i = k + 1  # consumed through the cast line (blanks skipped)
                            continue
                    # No cast line — simpler pattern, just emit indexed loop
                    loop_idx = '_i%s' % iter_var.replace('var', '')
                    result.append('%sfor (size_t %s = 0; %s < %s.size(); ++%s) {' % (
                        indent, loop_idx, loop_idx, collection_conv, loop_idx))
                    i = j + 1  # consumed through the while line (blanks skipped)
                    continue
            # Fallback: couldn't match while pattern, PORT-comment it
            converted = convert_body_line(bl, cls, fl, param_names)
            result.append(converted.replace('Iterator', '// PORT-TODO: Iterator'))
            i += 1
            continue

        # ---- Existing: PORT-BLOCKED if-block handling ----
        converted = convert_body_line(bl, cls, fl, param_names)
        # BUG 25: If the original line was an if() with { and it got PORT-marked,
        # comment out the entire block body until the matching }
        if ('// PORT:' in converted and '// was:' in converted and
            '{' in bl.strip() and bl.strip().endswith('{')):
            result.append(converted)
            # Skip body lines until matching close brace
            depth = 1
            i += 1
            while i < len(body_lines) and depth > 0:
                inner = body_lines[i]
                depth += inner.count('{') - inner.count('}')
                indent = len(inner) - len(inner.lstrip())
                result.append(' ' * indent + '// PORT-BLOCKED: ' + inner.strip())
                i += 1
            continue
        result.append(converted)
        i += 1
    return result


# ============================================================================
# POST-PROCESSOR (integrated — runs on generated content strings)
# ============================================================================

def postprocess(content, is_cpp, global_fields=None):
    """Apply all post-processing fixes to generated content."""
    if global_fields is None:
        global_fields = set()

    # Fix 0 (★ MathHelper trig / any char-as-int mask): a Java char literal written as a LITERAL
    # non-ASCII character -- e.g. the sine-table index mask `& '￿'` stored in the source as the
    # raw char '￿' (U+FFFF). The \uXXXX *escape* form is handled in convert_value, but the raw-char
    # form reaches here intact and MSVC silently substitutes the unrepresentable narrow char ('￿' ->
    # '?' = 0x3F = 63), so `& '￿'` became `& 63` and MathHelper::sin/cos only read table[0..63] ~= 0
    # (invisible view-bob / mob limbs / particles). Emit the code point as a hex int so the mask is
    # exact. ASCII char literals (e.g. 'a') are untouched; any single non-ASCII char in '' is always a
    # mask/index here, never a real character. See the 'mathhelper-uffff-mask-bug' note.
    content = re.sub(r"'([^\x00-\x7f])'",
                     lambda m: '0x%04X' % ord(m.group(1)), content)
    # Fix 0b: the \uXXXX ESCAPE form of the same char-mask (e.g. `& '￿'`) — alpha's
    # MathHelper kept it as an escape literal that slipped past convert_value. MSVC C4566:
    # '￿' can't be represented in cp1252 and collapses to '?' (63) -> trig table read
    # only [0..63] ~= 0. Emit as an exact hex int. See 'mathhelper-uffff-mask-bug'.
    content = re.sub(r"'\\u([0-9a-fA-F]{4})'", lambda m: '0x' + m.group(1).upper(), content)

    # Fix 1: std::string* -> const std::string& for PARAMETERS only (cheaper to pass a string).
    # A std::string* in a FIELD / local / return type is an ARRAY: Java 'String[]' -> 'std::string*',
    # allocated with 'new std::string[N]' plus an _count companion. The old code blanket-replaced ALL
    # 'std::string*' and then forced field decls to a value 'std::string', which DROPPED the array and --
    # via the generated 'm_x(NULL)' init -- reintroduced the std::string(const char*=NULL) -> strlen(NULL)
    # console crash (e.g. TileEntitySign::signText[], GuiMainMenu::minecraftLogo[]). So convert only param
    # occurrences ('std::string* name,' or 'std::string* name)'); leave every array pointer intact.
    content = re.sub(r'std::string\*(\s+\w+\s*[,)])', r'const std::string&\1', content)

    # Fix 2: ClassName.field -> ClassName::s_field (or ClassName::field for bare statics)
    for cn, fields in STATIC_CLASS_FIELDS.items():
        for fld in fields:
            if fld in BARE_STATIC_FIELDS:
                cpp_name = fld
            else:
                cpp_name = 's_%s' % fld
            content = re.sub(r'\b%s\.%s\b' % (re.escape(cn), re.escape(fld)),
                           '%s::%s' % (cn, cpp_name), content)

    # Fix 2b: ClassName.staticMethod() -> ClassName::staticMethod()
    # Exclude Java-only classes that should stay as PORT markers
    JAVA_ONLY_CLASSES = {'GL11', 'GL15', 'GLU', 'Display', 'Keyboard', 'Mouse',
                         'BufferUtils', 'System', 'Thread', 'Integer', 'Long',
                         'Float', 'Double', 'Boolean', 'ImageIO', 'JOptionPane',
                         'Math'}  # Math already handled by MATH_REPLACEMENTS
    def _static_method_replace(m):
        cls_name = m.group(1)
        method = m.group(2)
        if cls_name in JAVA_ONLY_CLASSES:
            return m.group(0)  # leave as-is, will be PORT-marked elsewhere
        return '%s::%s(' % (cls_name, method)
    content = re.sub(r'\b([A-Z]\w+)\.(\w+)\s*\(', _static_method_replace, content)

    # Fix 3: Chained static: ClassName::field.sub -> ClassName::field->sub
    for cn, fields in STATIC_CLASS_FIELDS.items():
        for fld in fields:
            if fld in BARE_STATIC_FIELDS:
                cpp_name = fld
            else:
                cpp_name = 's_%s' % fld
            pattern = r'%s::%s\.(\w+)' % (re.escape(cn), re.escape(cpp_name))
            content = re.sub(pattern, lambda m, c=cn, n=cpp_name: '%s::%s->%s' % (c, n, m.group(1)), content)

    # Fix 4: Bare std::vector -> std::vector<void*>
    content = re.sub(r'std::vector\*(?!\s*/\*)', 'std::vector<void*>* /* PORT: determine element type */', content)
    content = re.sub(r'std::vector\s+(m_|s_)', r'std::vector<void*> /* PORT: determine element type */ \1', content)
    # Also catch bare std::vector followed by any identifier (header fields without prefix)
    content = re.sub(r'std::vector\s+([a-zA-Z_]\w*)', r'std::vector<void*> /* PORT: determine element type */ \1', content)
    # Catch bare std::vector as return type or param type (before * or &)
    content = re.sub(r'\bstd::vector\b(?!\s*<)(?!\s*\w)', 'std::vector<void*>', content)

    if is_cpp:
        # Fix 8 (BUG 28): FloatBuffer/IntBuffer method calls on float*/int*
        # MUST run before stub generation so PORT-marked var decls get stubs
        content = _fix_nio_buffer_method_calls(content)
        # Fix 12: Emit stub declarations for PORT-blocked variable declarations
        # MUST run before parent field refs so stubs are detected as local vars
        content = _fix_port_blocked_var_stubs(content)
        # Fix 5: Pointer param . -> ->
        content = _fix_pointer_param_access(content)
        # Fix 6: Parent field refs -> m_ prefix
        content = _fix_parent_field_refs(content)
        # Fix 7 (BUG 24): Cross-object field access: obj.field -> obj.m_field
        content = _fix_cross_object_field_access(content, global_fields)
        # Fix 7b: `new X(...)` is a pointer -> member access on it (and the builder chain it heads) is ->
        content = _fix_new_pointer_chains(content)
        # Fix 7c: Java downcast (RefType)expr -> (RefType*)expr
        content = _fix_reference_casts(content)
        # Fix 7d: java.lang boxed constants (Integer.MAX_VALUE, ...) -> C++ literals
        content = _fix_java_boxed_constants(content)
        # Bug 3 postprocess: Hoist C++03 array inits out of expressions
        content = _hoist_array_inits(content)
        # Fix 13: Infer vector element type from casts in same method
        content = _fix_void_vector_type_inference(content)
        # Bug 4: Replace array length PORT comments with tracked size constants
        content = _fix_array_length_tracking(content)
        # Bug 5: Remove self-indexing NIO buffer residue (m_proj[m_proj] etc)
        content = _fix_self_indexing(content)
        # Bug 6: Fix ArrayList.remove(Object) -> std::find + erase
        content = _fix_remove_semantics(content)
        # Bug 7: Fix this-pointer passed to ref-param constructor (Vertex::remap)
        content = _fix_this_pointer_to_ref(content)
        # Bug 9: Fix new ClassName() assigned to value array slot
        content = _fix_new_to_value_array(content)
        # Bug 10: Fix try/catch/finally remnants
        content = _fix_try_catch_finally(content)
        # Bug 11: Fix null assigned to value types (HitResult etc)
        content = _fix_null_value_types(content)
        # Bug 8: Fix pointer return assigned to value
        content = _fix_pointer_return_to_value(content)
        # Fix: Stub out functions whose bodies are mostly PORT-commented (>50% PORT lines)
        content = _stub_dead_functions(content)
        # Fix 10: Add missing #includes (must run LAST — after all transforms that add new references)
        content = _fix_missing_static_includes(content)
        # Warning fixes (won't break logic, just suppress MSVC warnings)
        content = _fix_warnings(content)

    if not is_cpp:
        # Fix 9 (BUG 29): Deduplicate field declarations in headers
        content = _dedup_header_fields(content)
        # Bug 11 header: Add bool m_hasField for value-type fields that use NULL patterns
        content = _fix_null_value_type_headers(content)
        # Fix struct/class forward decl mismatch in headers
        content = _fix_struct_forward_decls(content)

    # Profile-specific type name renames in includes/forward-decls/body
    if PROFILE.name == 'indev':
        content = re.sub(r'\bTessellator\b', 'Tesselator', content)
        # Fix #include "Tessellator.h" -> "Tesselator.h"
        content = content.replace('#include "Tessellator.h"', '#include "Tesselator.h"')

    return content


STRUCT_TYPES = None  # set by init_profile_globals — but also includes 'TesselatorVertex'


def _fix_struct_forward_decls(content):
    """Fix 'class X;' forward decls for types that are actually structs."""
    struct_types = PROFILE.struct_classes | {'TesselatorVertex'}
    for st in struct_types:
        content = re.sub(r'\bclass\s+%s\s*;' % re.escape(st), 'struct %s;' % st, content)
    return content


def _fix_warnings(content):
    """Fix MSVC warnings in converter-generated .cpp files."""
    lines = content.split('\n')
    result = []
    for line in lines:
        fl = line
        # W1: signed/unsigned mismatch — int i < vector.size()
        # Change: for(int i = 0; i < vec.size() -> for(size_t i = 0; i < vec.size()
        fl = re.sub(r'for\s*\(\s*int\s+(\w+)\s*=\s*0\s*;\s*\1\s*<\s*(\w+)\.size\(\)',
                    r'for(size_t \1 = 0; \1 < \2.size()', fl)
        # Also: standalone int i; ... i < vec.size() patterns
        # Only convert if this specific variable is used with .size() within the file
        int_decl_match = re.match(r'(\s*)\bint\s+(\w+);\s*$', fl)
        if int_decl_match and '.size()' in content:
            var_name = int_decl_match.group(1 + 1)  # group(2)
            # Check if this variable is actually compared to .size() anywhere in content
            size_compare_pat = r'\b%s\s*[<>=!]+\s*\w+\.size\(\)' % re.escape(var_name)
            if re.search(size_compare_pat, content):
                fl = re.sub(r'\bint\s+(%s);\s*$' % re.escape(var_name), r'size_t \1;', fl)

        # W2: XOR precedence — "br == c1 ^ layer == 1" should be "(br == c1) ^ (layer == 1)"
        # or better: "(br == c1) != (layer == 1)" which is the actual Java semantics
        fl = re.sub(r'(\w+)\s*==\s*(\w+)\s*\^\s*(\w+)\s*==\s*(\w+)',
                    r'(\1 == \2) != (\3 == \4)', fl)

        # W3: (double) cast before sinf/cosf/sqrtf is unnecessary — they take/return float
        fl = re.sub(r'\(float\)\s*\(double\)', '(float)', fl)
        fl = re.sub(r'\(float\)sqrtf\(\(double\)(\w+)\)', r'sqrtf(\1)', fl)
        fl = re.sub(r'\(double\)(\w+)\s*\*\s*3\.14159265358979323846f\s*/\s*180\.0f',
                    r'\1 * (3.14159265358979323846f / 180.0f)', fl)

        # W4: long narrowing from __int64 — use long long for nanosecond vars
        fl = re.sub(r'\blong\s+passedNs\b', 'long long passedNs', fl)
        fl = re.sub(r'\blong\s+now\b', 'long long now', fl)

        result.append(fl)
    return '\n'.join(result)


def _fix_pointer_param_access(content):
    # Known pointer fields from parent classes
    # Include both bare names AND m_ prefixed versions for chained access
    _BASE_PTR_FIELDS = {
        'rand', 'random', 'worldObj', 'mc', 'fontRenderer', 'renderManager',
        'material', 'stepSound', 'boundingBox', 'model',
        'renderEngine', 'theWorld', 'thePlayer', 'currentScreen',
        'entityToAttack', 'pathToEntity', 'playerToAttack',
    }
    KNOWN_POINTER_FIELDS = set(_BASE_PTR_FIELDS)
    for f in _BASE_PTR_FIELDS:
        KNOWN_POINTER_FIELDS.add('m_' + f)
        KNOWN_POINTER_FIELDS.add('s_' + f)
    lines = content.split('\n')
    result = []
    ptr_params = set()
    ptr_locals = set()
    for line in lines:
        sig = re.match(r'\w[\w:*&<> ]*\s+\w+::\w+\((.+)\)\s*\{?', line)
        if sig:
            ptr_params = set()
            ptr_locals = set()
            for p in sig.group(1).split(','):
                p = p.strip()
                if '*' in p:
                    nm = re.search(r'(\w+)\s*$', p)
                    if nm: ptr_params.add(nm.group(1))
        ctor = re.match(r'(\w+)::\1\((.+)\)', line)
        if ctor:
            ptr_params = set()
            ptr_locals = set()
            for p in ctor.group(2).split(','):
                p = p.strip()
                if '*' in p:
                    nm = re.search(r'(\w+)\s*$', p)
                    if nm: ptr_params.add(nm.group(1))
        # Track local pointer variables: Type* varName or Type *varName = ...
        local_ptr = re.match(r'\s+\w[\w:]*\*\s+(\w+)', line)
        if local_ptr:
            ptr_locals.add(local_ptr.group(1))
        # Also: varName = new Type(...)
        new_assign = re.match(r'\s+(?:\w[\w:]*\*?\s+)?(\w+)\s*=\s*new\s+\w+', line)
        if new_assign:
            ptr_locals.add(new_assign.group(1))
        fl = line
        all_ptrs = ptr_params | ptr_locals | KNOWN_POINTER_FIELDS
        for pn in all_ptrs:
            # Replace var.method( and var.field with var->method( and var->field
            # But NOT ClassName.static (uppercase first letter after dot handled elsewhere)
            fl = re.sub(r'\b%s\.(?=[a-z])' % re.escape(pn), '%s->' % pn, fl)
        result.append(fl)
    return '\n'.join(result)


def _fix_parent_field_refs(content):
    all_pf = set()
    for flds in PARENT_FIELDS.values():
        all_pf.update(flds)
    # Don't prefix fields that should stay bare
    all_pf -= BARE_FIELDS
    if not all_pf:
        return content
    lines = content.split('\n')
    result = []
    current_params = set()
    local_vars = set()
    for line in lines:
        s = line.strip()
        if (s.startswith('//') or s.startswith('#') or s.startswith('/*') or
            s.startswith('*') or '::' in s):
            sig = re.match(r'\w[\w:*&<> ]*\s+\w+::\w+\((.+)\)\s*\{?', s)
            if sig:
                current_params = set()
                local_vars = set()
                for p in sig.group(1).split(','):
                    nm = re.search(r'(\w+)\s*$', p.strip())
                    if nm:
                        current_params.add(nm.group(1))
            ctor = re.match(r'(\w+)::\1\((.+)\)', s)
            if ctor:
                current_params = set()
                local_vars = set()
                for p in ctor.group(2).split(','):
                    nm = re.search(r'(\w+)\s*$', p.strip())
                    if nm:
                        current_params.add(nm.group(1))
            result.append(line)
            continue
        fl = line
        for decl_match in re.finditer(r'\b(?:int|float|double|long|short|byte|bool|char|unsigned)\s+(\w+)', fl):
            local_vars.add(decl_match.group(1))
        for_match = re.search(r'for\s*\(\s*(?:int|float|double|long)\s+(\w+)', fl)
        if for_match:
            local_vars.add(for_match.group(1))

        skip_names = current_params | local_vars
        for fld in sorted(all_pf, key=len, reverse=True):
            if fld in skip_names:
                continue
            pat = r'(?<![.\w])(?<!m_)(?<!s_)(?<!::)\b%s\b(?!\s*\()' % re.escape(fld)
            if re.search(pat, fl):
                fl = re.sub(pat, 'm_%s' % fld, fl)
        result.append(fl)
    return '\n'.join(result)


def _fix_new_pointer_chains(content):
    """In C++, `new X(...)` yields X*, so a member access on it must use -> not . — and the builder chain
    it heads (e.g. Block static init `(new Block(...)).setHardness(...).setStepSound(...)`, where each
    setter returns the same pointer) is all ->. Anchored strictly on `new`, so value-returning chains like
    `Tesselator::getInstance().vertexUV(...)` are left untouched. Paren-balanced so args/nesting are safe."""
    res = []
    i, n = 0, len(content)
    while i < n:
        m = re.match(r'new\s+\w+\s*\(', content[i:])
        if not m:
            res.append(content[i]); i += 1; continue
        start = i
        j = i + m.end() - 1            # index of the '(' opening new X(...)
        depth = 0
        while j < n:
            if content[j] == '(':
                depth += 1
            elif content[j] == ')':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        res.append(content[start:j + 1])   # emit `new X(...)` verbatim (inner dots untouched)
        i = j + 1
        while i < n and content[i] in ' \t\n':
            res.append(content[i]); i += 1
        if i < n and content[i] == ')':    # the wrapping ')' of `(new X(...))`
            res.append(')'); i += 1
        while i < n:                       # convert the trailing .member chain to ->
            while i < n and content[i] in ' \t\n':
                res.append(content[i]); i += 1
            mm = re.match(r'\.(\w+)', content[i:])
            if not mm:
                break
            res.append('->' + mm.group(1)); i += mm.end()
            if i < n and content[i] == '(':
                d, k = 0, i
                while k < n:
                    if content[k] == '(':
                        d += 1
                    elif content[k] == ')':
                        d -= 1
                        if d == 0:
                            break
                    k += 1
                res.append(content[i:k + 1]); i = k + 1
    return ''.join(res)


def _fix_reference_casts(content):
    """Java downcast `(RefType)expr` -> C++ `(RefType*)expr` (reference types are pointers here). Only
    KNOWN reference/interface type names are rewritten, so value casts ((int)/(float)/...), sizeof(T),
    and ordinary parenthesised exprs are untouched. Idempotent ((T*) won't re-match \\(\\w+\\))."""
    types = REFERENCE_TYPES | INTERFACE_TYPES

    def repl(m):
        t = m.group(1)
        return '(%s*)' % t if t in types else m.group(0)
    # (Type) immediately followed by a cast target (identifier / '(' / new ...).
    return re.sub(r'\((\w+)\)(?=\s*[A-Za-z_(])', repl, content)


_JAVA_BOXED_CONSTANTS = {
    'Integer.MAX_VALUE': '2147483647', 'Integer.MIN_VALUE': '(-2147483647 - 1)',
    'Long.MAX_VALUE': '9223372036854775807LL', 'Long.MIN_VALUE': '(-9223372036854775807LL - 1)',
    'Short.MAX_VALUE': '32767', 'Short.MIN_VALUE': '(-32768)',
    'Byte.MAX_VALUE': '127', 'Byte.MIN_VALUE': '(-128)',
    'Float.MAX_VALUE': '3.402823466e+38f', 'Float.MIN_VALUE': '1.175494351e-38f',
    'Float.POSITIVE_INFINITY': '(1e30f)', 'Float.NEGATIVE_INFINITY': '(-1e30f)',
    'Double.MAX_VALUE': '1.7976931348623157e308', 'Double.MIN_VALUE': '2.2250738585072014e-308',
}


def _fix_java_boxed_constants(content):
    """java.lang boxed constants (Integer.MAX_VALUE, Long.MIN_VALUE, Float.POSITIVE_INFINITY, ...) ->
    C++ literals. Handles both the `.` and the already-converted `::` form. (Math.PI etc. are in
    MATH_REPLACEMENTS; parseInt/parseX are funcs, left for a platform helper.)"""
    for k, v in _JAVA_BOXED_CONSTANTS.items():
        content = content.replace(k, v).replace(k.replace('.', '::'), v)
    return content


def _fix_cross_object_field_access(content, global_fields):
    """BUG 24: Convert obj.fieldName -> obj.m_fieldName and obj->fieldName -> obj->m_fieldName
    for any field name in the global field set across ALL parsed classes.
    Skips fields in BARE_FIELDS since those keep their Java names.

    PERF: the old version looped over EVERY prefixable field (hundreds, once Alpha has 377 classes)
    for EVERY line -> O(lines x fields), the dominant cost of a conversion run. This version (a) pre-
    filters to only the field names that actually occur in this file, then (b) folds them into ONE
    combined alternation regex per access kind, so each line is scanned twice total instead of
    2*fields times. Behaviour is identical: dot- and arrow-access are disjoint syntactic contexts and
    each '.field'/'->field' token is matched once, so single-pass == the old sequential per-field pass.
    """
    if not global_fields:
        return content
    prefixable = global_fields - BARE_FIELDS
    if not prefixable:
        return content
    # (#2) Pre-filter: only field names that actually appear anywhere in this file.
    present = [f for f in prefixable if f in content]
    if not present:
        return content
    # Longer names first so a longer field can't be shadowed by a shorter prefix (word boundaries
    # already make this safe, but it mirrors the old sorted() order exactly).
    present.sort(key=len, reverse=True)
    alt = '|'.join(re.escape(f) for f in present)
    # (#3) One combined regex per access kind instead of one per field.
    dot_re = re.compile(r'([\w\]])\.(?!m_)(?!s_)\b(%s)\b(?!\s*\()' % alt)
    arrow_re = re.compile(r'->(?!m_)(?!s_)\b(%s)\b(?!\s*\()' % alt)
    lines = content.split('\n')
    result = []
    for line in lines:
        s = line.strip()
        is_signature = (s and '::' in s and (s.endswith('{') or s.endswith(')')) and
                       re.match(r'[\w:*&<> ]+\s+\w+::\w+\(', s))
        if (s.startswith('//') or s.startswith('#') or s.startswith('/*') or
            s.startswith('*') or is_signature):
            result.append(line)
            continue
        fl = dot_re.sub(lambda m: '%s.m_%s' % (m.group(1), m.group(2)), line)
        fl = arrow_re.sub(lambda m: '->m_%s' % m.group(1), fl)
        result.append(fl)
    return '\n'.join(result)


def _fix_nio_buffer_method_calls(content):
    """BUG 28: FloatBuffer/IntBuffer mapped to float*/int* but Java method calls remain.
    Convert .flip()/.clear()/.put()/.limit()/.asIntBuffer() etc to concrete C++ code."""
    NIO_NOOP = {'flip', 'rewind', 'limit', 'remaining', 'capacity', 'asIntBuffer', 'asFloatBuffer'}
    lines = content.split('\n')
    result = []
    for line in lines:
        s = line.strip()
        if s.startswith('//') or s.startswith('#'):
            result.append(line)
            continue
        fl = line
        indent = len(fl) - len(fl.lstrip())
        pad = ' ' * indent

        # .clear() -> memset
        m = re.search(r'(\w+)\.(clear)\(\)', fl)
        if m and '// PORT:' not in fl:
            var = m.group(1)
            fl = pad + 'memset(%s, 0, sizeof(%s));' % (var, var)
            result.append(fl)
            continue

        # .put(val).put(val)... chain -> sequential assignments
        m = re.search(r'(\w+)\.put\((.+?)\)(?:\.put\((.+?)\))?(?:\.put\((.+?)\))?(?:\.put\((.+?)\))?', fl)
        if m and '.put(' in fl and '// PORT:' not in fl:
            var = m.group(1)
            vals = [m.group(i) for i in range(2, 6) if m.group(i)]
            assignments = '; '.join(['%s[%d] = %s' % (var, i, v) for i, v in enumerate(vals)])
            fl = pad + '%s;' % assignments
            result.append(fl)
            continue

        # .position(n) -> /* position = n */ (tracking info)
        m = re.search(r'(\w+)\.position\((\w+)\)', fl)
        if m and '// PORT:' not in fl:
            fl = pad + '// buffer %s position = %s' % (m.group(1), m.group(2))
            result.append(fl)
            continue

        # .get(idx) -> array[idx]  (already handled by earlier regex, but catch stragglers)
        # No-op methods
        for meth in NIO_NOOP:
            if '.%s(' % meth in fl or '->%s(' % meth in fl:
                if '// PORT:' not in fl:
                    fl = pad + '// NIO %s() — no-op for C array' % meth
                    break
        result.append(fl)
    return '\n'.join(result)


def _dedup_header_fields(content):
    """BUG 29: Deduplicate field declarations in headers where _field -> m_field
    and field -> m_field produce the same name."""
    lines = content.split('\n')
    seen_fields = set()
    result = []
    for line in lines:
        # Match field declarations: [tab]type [optional comment] m_name; or s_name;
        fm = re.search(r'\b(m_\w+|s_\w+)\s*;', line)
        if fm and not '(' in line:  # field decl, not method
            field_name = fm.group(1)
            if field_name in seen_fields:
                continue  # skip duplicate
            seen_fields.add(field_name)
        result.append(line)
    return '\n'.join(result)


def _fix_null_value_type_headers(content):
    """Bug 11 header: For value-type fields that had NULL patterns, we need to
    replace the NULL init in the header and add a bool flag field.
    Look for m_hasXxx(false) pattern (added by the cpp-side fix) and ensure
    the header has a bool m_hasXxx declaration."""
    # In the header, the init list won't appear, but if there's a field like
    # HitResult m_hitResult that had NULL, we add bool m_hasHitResult after it.
    for vt in VALUE_TYPES:
        # Find: [type] m_[name]; where type matches a value type
        def _add_has_flag(m):
            full_line = m.group(0)
            field_name = m.group(1)
            base = field_name[2:]  # strip m_
            has_field = 'm_has%s%s' % (base[0].upper(), base[1:])
            return full_line + '\n\tbool %s;' % has_field
        # Only add if not already present
        pattern = r'\t%s\s+(m_\w+)\s*;' % re.escape(vt)
        for fm in re.finditer(pattern, content):
            field_name = fm.group(1)
            base = field_name[2:]
            has_field = 'm_has%s%s' % (base[0].upper(), base[1:])
            if has_field not in content:
                content = content.replace(fm.group(0), fm.group(0) + '\n\tbool %s;' % has_field)
    return content


def _fix_missing_static_includes(content):
    """Fix 10: Scan .cpp for ClassName:: usage and add missing #includes."""
    KNOWN_CLASSES = {
        # RubyDung
        'Tile', 'Textures', 'Frustum', 'Chunk', 'Tesselator', 'Level',
        'LevelRenderer', 'Entity', 'Player', 'Zombie', 'RubyDung',
        'AABB', 'Vec3', 'Vertex', 'Polygon', 'Cube', 'HitResult',
        'Timer', 'LevelListener',
        # Indev — core
        'Block', 'Material', 'Item', 'ItemStack', 'World',
        'EntityLiving', 'EntityPlayer', 'EntityPlayerSP', 'EntityFX',
        'EntityCreature', 'EntityAnimal', 'EntityMob',
        'EntityArrow', 'EntityItem', 'EntitySheep', 'EntityTNT',
        'NBTTagCompound', 'NBTTagList', 'NBTBase', 'NBTTagFloat',
        'AxisAlignedBB', 'Vec3D', 'MovingObjectPosition',
        'RenderBlocks', 'RenderGlobal', 'RenderItem', 'RenderManager',
        'Tessellator', 'Gui', 'GuiScreen', 'FontRenderer',
        'GameSettings', 'Minecraft', 'MathHelper',
        'Shaders', 'Random', 'Common',
        # Indev — additional
        'Slot', 'SlotCrafting', 'WorldRenderer', 'CraftingRecipe',
        'InventoryPlayer', 'InventoryCrafting', 'InventoryLargeChest',
        'ScaledResolution', 'TileEntity', 'TileEntityChest',
        'MobSpawner', 'Pathfinder', 'PathEntity', 'PathPoint',
        'NoiseGenerator', 'NoiseGeneratorDistort', 'NoiseGeneratorOctaves',
        'NoiseGeneratorPerlin', 'LevelGenerator', 'LevelLoader',
        'ModelBase', 'ModelBiped', 'ModelQuadruped', 'ModelRenderer',
        'StepSound', 'CraftingManager', 'PlayerController',
        'EffectRenderer', 'EntityMap', 'Light',
        'SoundManager', 'SoundPool', 'KeyBinding',
        'MovementInput', 'MovementInputFromOptions',
        'TextureFX', 'RenderHelper', 'ItemRenderer',
        'RecipesTools', 'RecipesWeapons', 'RecipesIngots', 'RecipesFood',
        'RecipesCrafting', 'RecipesArmor', 'RecipeSorter',
        'EntityCreeper', 'EntityGiantZombie', 'EntityPig',
        'EntitySkeleton', 'EntitySpider', 'EntityZombie',
        'ItemBlock', 'ItemArmor', 'ItemAxe', 'ItemBow',
        'ItemFlintAndSteel', 'ItemFood', 'ItemHoe', 'ItemPickaxe',
        'ItemSeeds', 'ItemSoup', 'ItemSpade', 'ItemSword', 'ItemTool',
        'NBTTagByte', 'NBTTagByteArray', 'NBTTagDouble', 'NBTTagEnd',
        'NBTTagInt', 'NBTTagLong', 'NBTTagShort', 'NBTTagString',
        'MaterialLiquid', 'MaterialLogic', 'MaterialTransparent',
        'StepSoundGlass', 'StepSoundGravel',
    }
    lines = content.split('\n')
    existing_includes = set()
    needed_system = set()
    for line in lines:
        im = re.match(r'#include\s+"(\w+)\.h"', line)
        if im:
            existing_includes.add(im.group(1))
        im2 = re.match(r'#include\s+<(\w+)>', line)
        if im2:
            existing_includes.add(im2.group(1))

    # Check for std:: functions that need headers
    if 'std::find' in content and 'algorithm' not in existing_includes:
        needed_system.add('<algorithm>')
    if 'std::map' in content and 'map' not in existing_includes:
        needed_system.add('<map>')
    if 'std::string' in content and 'string' not in existing_includes:
        needed_system.add('<string>')

    # Widen the class universe to every generated header (not just the hardcoded list)
    # so body-only concrete uses (e.g. `new WorldChunkManager`, `(BlockLeaves*)x`,
    # `RenderManager*`) get their #include. A .cpp can include freely (no cycle risk).
    # Pre-filter to names actually present in this file -> O(present) not O(all classes).
    class_universe = KNOWN_CLASSES | AVAILABLE_HEADERS
    candidates = [c for c in class_universe if c not in existing_includes and c in content]
    needed = set()
    for line in lines:
        for cls_name in candidates:
            if cls_name in needed:
                continue
            if '%s::' % cls_name in line:                                  # static / scope
                needed.add(cls_name); continue
            if re.search(r'\b%s\s+\w+\s*[=;,)]' % cls_name, line):         # value decl / param
                needed.add(cls_name); continue
            if re.search(r'\bnew\s+%s\b' % cls_name, line):                # new T / new T[]
                needed.add(cls_name); continue
            if re.search(r'\(\s*%s\s*\*?\s*\)' % cls_name, line):          # (T) / (T*) cast
                needed.add(cls_name); continue
            if re.search(r'\b%s\s*\*' % cls_name, line):                   # T* pointer type (decl/cast/field)
                needed.add(cls_name); continue
            if re.search(r'\b%s\s*\(' % cls_name, line):                   # T(...) ctor/call
                if '::' not in line or '%s::' % cls_name not in line:
                    needed.add(cls_name); continue

    if needed or needed_system:
        result = []
        last_include_idx = 0
        for i, line in enumerate(lines):
            if line.startswith('#include'):
                last_include_idx = i
        for i, line in enumerate(lines):
            result.append(line)
            if i == last_include_idx:
                for sh in sorted(needed_system):
                    result.append('#include %s' % sh)
                for cn in sorted(needed):
                    result.append('#include "%s.h"' % cn)
        return '\n'.join(result)
    return content


def _dedup_init_list(content):
    """Fix 11: Remove duplicate entries in constructor init lists."""
    lines = content.split('\n')
    result = []
    in_init_list = False
    seen_members = set()
    for line in lines:
        s = line.strip()
        if s.startswith(': ') or (s.startswith(', ') and in_init_list):
            in_init_list = True
            mm = re.match(r'[\s:,]+(\w+)\(', s)
            if mm:
                member = mm.group(1)
                if member in seen_members:
                    continue  # skip duplicate
                seen_members.add(member)
        elif s == '{':
            in_init_list = False
            seen_members = set()
        elif not s.startswith(',') and not s.startswith(':'):
            in_init_list = False
            seen_members = set()
        result.append(line)
    return '\n'.join(result)


def _fix_port_blocked_var_stubs(content):
    """Fix 12: When a PORT comment killed a variable declaration, emit a stub."""
    lines = content.split('\n')
    result = []
    for i, line in enumerate(lines):
        result.append(line)
        if '// PORT:' in line and '// was:' in line:
            was_part = line.split('// was:')[-1].strip()
            vm = re.match(r'(?:double|float|long|int|short|byte)\s+(\w+)\s*=', was_part)
            if vm:
                var_type = was_part.split()[0]
                var_name = vm.group(1)
                indent = len(line) - len(line.lstrip())
                result.append(' ' * indent + '%s %s = 0; // PORT: stub for blocked declaration' % (var_type, var_name))
    return '\n'.join(result)


def _fix_void_vector_type_inference(content):
    """Fix 13: Infer vector element type from context — casts, method calls, or RHS function return."""
    # Known method-to-type mapping for common patterns
    METHOD_TYPE_MAP = {
        'clipXCollide': 'AABB', 'clipYCollide': 'AABB', 'clipZCollide': 'AABB',
    }
    FUNC_RETURN_TYPE = {
        'getCubes': 'AABB',
    }

    lines = content.split('\n')
    void_vectors = {}  # varName -> inferred type

    for line in lines:
        # Detect: std::vector<void*> varName = something->getCubes(...)
        vm = re.search(r'std::vector<void\*>.*\b(\w+)\s*=\s*\w+->(\w+)\(', line)
        if vm:
            var, func = vm.group(1), vm.group(2)
            if func in FUNC_RETURN_TYPE:
                void_vectors[var] = FUNC_RETURN_TYPE[func]

        # Detect: std::vector<void*> varName (declared but not assigned from function)
        # Then check if push_back is called with a known type
        vm2 = re.search(r'std::vector<void\*>.*\b(\w+)\s*(?:/\*|;)', line)
        if vm2:
            void_vectors.setdefault(vm2.group(1), None)

        # Detect: varName.push_back(AABB(...)) or varName.push_back(someAABBvar)
        pm = re.search(r'(\w+)\.push_back\(\s*(\w+)\s*\(', line)
        if pm:
            var, type_name = pm.group(1), pm.group(2)
            if var in void_vectors and is_value_type(type_name):
                void_vectors[var] = type_name

        # Detect: varName[i]->knownMethod( or varName[i].knownMethod(
        for meth, typ in METHOD_TYPE_MAP.items():
            if re.search(r'(\w+)\[\w+\][\.\->]+%s\(' % meth, line):
                mm = re.search(r'(\w+)\[\w+\]', line)
                if mm:
                    void_vectors[mm.group(1)] = typ

        # Detect: (Type)varName[i] casts
        cm = re.search(r'\((\w+)\)(\w+)\[', line)
        if cm:
            cast_type, var = cm.group(1), cm.group(2)
            if cast_type not in ('int', 'float', 'double', 'long', 'char', 'bool'):
                void_vectors[var] = cast_type

    # Apply replacements
    result = []
    for line in lines:
        fl = line
        for var, inferred in void_vectors.items():
            if inferred and 'std::vector<void*>' in fl and var in fl:
                fl = fl.replace('std::vector<void*> /* PORT: type */', 'std::vector<%s>' % inferred)
                fl = fl.replace('std::vector<void*> /* PORT: determine element type */', 'std::vector<%s>' % inferred)
                fl = fl.replace('std::vector<void*>', 'std::vector<%s>' % inferred)
        # Also fix: aABBs[i]->method for value types should be aABBs[i].method
        for var, inferred in void_vectors.items():
            if inferred and is_value_type(inferred):
                fl = re.sub(r'\b%s\[(\w+)\]->' % re.escape(var), r'%s[\1].' % var, fl)
        result.append(fl)
    return '\n'.join(result)


def _hoist_array_inits(content):
    """Bug 3: Hoist C++03 compound literal array inits to local variable declarations.
    Converts: expr = Polygon((Type[]){a, b, c, d} /*_count=4*/, ...)
    To: Type _arrN[4] = {a, b, c, d}; (before the line) and passes _arrN."""
    lines = content.split('\n')
    result = []
    arr_counter = [0]
    for line in lines:
        # Find all (Type[]){elements} /*_count=N*/ patterns on this line
        pattern = r'\((\w+)\[\]\)\{([^}]+)\}\s*/\*_count=(\d+)\*/'
        matches = list(re.finditer(pattern, line))
        if matches:
            indent = len(line) - len(line.lstrip())
            hoisted_decls = []
            new_line = line
            # Process in reverse so string indices stay valid
            for m in reversed(matches):
                arr_counter[0] += 1
                arr_name = '_tmpVerts%d' % arr_counter[0]
                type_name = m.group(1)
                elements = m.group(2)
                count = m.group(3)
                decl = '%s%s %s[%s] = {%s};' % (' ' * indent, type_name, arr_name, count, elements)
                hoisted_decls.insert(0, decl)
                new_line = new_line[:m.start()] + arr_name + new_line[m.end():]
            for d in hoisted_decls:
                result.append(d)
            result.append(new_line)
        else:
            result.append(line)
    return '\n'.join(result)


def _fix_array_length_tracking(content):
    """Bug 4: Replace 'i < m_arrayPtr /* PORT: array length - use tracked size */'
    with a proper count. Detect the allocation size from 'new Type[N]' in the same file."""
    lines = content.split('\n')
    # First pass: find all allocations to know sizes
    alloc_sizes = {}  # varName -> size expression
    for line in lines:
        # Field allocations: m_X = new Type[N]
        am = re.search(r'(m_\w+|s_\w+)\s*=\s*new\s+\w+\[([^\]]+)\]', line)
        if am:
            alloc_sizes[am.group(1)] = am.group(2)
        # Local variable allocations: Type* name = new Type[N]
        lm = re.search(r'\w+\*\s+(\w+)\s*=\s*new\s+\w+\[([^\]]+)\]', line)
        if lm:
            alloc_sizes[lm.group(1)] = lm.group(2)
    # Known constant sizes for specific fields
    KNOWN_SIZES = {
        'm_polygons': '6',   # Cube always has 6 polygons
        'm_vertices': '8',   # Cube always has 8 vertices
    }
    alloc_sizes.update(KNOWN_SIZES)

    # Also detect hoisted array sizes: Type _tmpVertsN[count] = {...};
    for line in lines:
        hm = re.search(r'(\w+)\[(\d+)\]\s*=\s*\{', line)
        if hm:
            alloc_sizes[hm.group(1)] = hm.group(2)

    result = []
    for line in lines:
        fl = line
        # Pattern: i < m_field /* PORT: array length - use tracked size */
        am = re.search(r'(\w+)\s*<\s*([\w]+)\s*/\*\s*PORT:\s*array length.*?\*/', fl)
        if am:
            field_name = am.group(2)
            size = alloc_sizes.get(field_name)
            if size:
                fl = fl.replace(am.group(0), '%s < %s' % (am.group(1), size))
            else:
                # Fallback: use a PORT marker with clearer intent
                fl = fl.replace(am.group(0), '%s < %s_count /* PORT: set this from allocation */' % (am.group(1), field_name))
        # Pattern: = varName /* PORT: array length */ used in assignment
        am2 = re.search(r'=\s*([\w]+)\s*/\*\s*PORT:\s*array length.*?\*/', fl)
        if am2:
            field_name = am2.group(1)
            size = alloc_sizes.get(field_name)
            if size:
                fl = fl.replace(am2.group(0), '= %s' % size)
            else:
                # For params like 'vertices' in Polygon(Vertex* vertices), use 4 (always quads)
                fl = fl.replace(am2.group(0), '= 4 /* PORT: quad vertex count */')
        result.append(fl)
    return '\n'.join(result)


def _fix_self_indexing(content):
    """Bug 5: Remove lines like m_proj[m_proj]; and m_modl[m_modl]; which are
    residue from Java NIO buffer.get(float[]) calls."""
    lines = content.split('\n')
    result = []
    for line in lines:
        s = line.strip()
        # Match: m_xxx[m_xxx]; (self-indexing)
        if re.match(r'(m_\w+)\[\1\]\s*;', s):
            continue  # skip the line entirely
        result.append(line)
    return '\n'.join(result)


def _fix_remove_semantics(content):
    """Bug 6: Fix .erase(.begin() + ptr) for object removal.
    Java ArrayList.remove(Object) should become find+erase in C++."""
    lines = content.split('\n')
    result = []
    for line in lines:
        m = re.search(r'(\w+)\.erase\(\.begin\(\)\s*\+\s*(\w+)\)\s*/\*\s*PORT:\s*verify remove semantics\s*\*/', line)
        if m:
            vec = m.group(1)
            obj = m.group(2)
            indent = len(line) - len(line.lstrip())
            pad = ' ' * indent
            # If the line ASSIGNS the result, this is Java List.remove(int index), which
            # returns the removed element -> 'LHS = vec[index]; vec.erase(vec.begin()+index);'
            # (the converter otherwise dropped the LHS, leaving the variable undeclared).
            am = re.match(r'\s*(?:([\w:<>*&]+(?:\s+[\w:<>*&]+)*?)\s+)?(\w+)\s*=\s*(?:\([^)]*\)\s*)?'
                          r'[\w.>-]*\.erase\(\.begin', line)
            if am:
                decl_type = am.group(1)
                lhs = am.group(2)
                prefix = (decl_type + ' ') if decl_type else ''
                result.append('%s%s%s = %s[%s];' % (pad, prefix, lhs, vec, obj))
                result.append('%s%s.erase(%s.begin() + %s);' % (pad, vec, vec, obj))
                continue
            # No assignment -> Java List.remove(Object): find the element and erase it.
            if 'listener' in vec.lower() or 'Listener' in vec:
                elem_type = 'LevelListener*'
            else:
                elem_type = 'void*'
            result.append('%sstd::vector<%s>::iterator _removeIt = std::find(%s.begin(), %s.end(), %s);' % (pad, elem_type, vec, vec, obj))
            result.append('%sif (_removeIt != %s.end()) %s.erase(_removeIt);' % (pad, vec, vec))
            continue
        result.append(line)
    return '\n'.join(result)


def _fix_this_pointer_to_ref(content):
    """Bug 7: Fix 'return Vertex(this, u, v)' -> 'return Vertex(*this, u, v)'
    where this (pointer) is passed to a constructor expecting const Vertex&."""
    content = re.sub(r'\b(\w+)\(this,', r'\1(*this,', content)
    return content


def _fix_new_to_value_array(content):
    """Bug 9: Fix 'valueArray[i] = new ClassName(...)' -> 'valueArray[i] = ClassName(...)'
    when the array holds values (not pointers)."""
    lines = content.split('\n')
    result = []
    for line in lines:
        # Pattern: m_arrayField[expr] = new ClassName(
        m = re.search(r'(m_\w+)\[([^\]]+)\]\s*=\s*new\s+(\w+)\(', line)
        if m:
            class_name = m.group(3)
            line = line.replace('= new %s(' % class_name, '= %s(' % class_name)
        result.append(line)
    return '\n'.join(result)


def _fix_try_catch_finally(content):
    """Bug 10: Fix broken try/catch/finally remnants.
    Finally blocks: keep the body (it's cleanup code that should always run).
    Catch blocks: already handled by body converter (DbgPrintF)."""
    lines = content.split('\n')
    result = []
    for line in lines:
        s = line.strip()
        # Remove lines with undeclared Java exception/stream variables
        if re.match(r'(e|var\d+)\.(printStackTrace|readFully|close|write)\b', s):
            indent = len(line) - len(line.lstrip())
            result.append(' ' * indent + '// Removed Java stream/exception call: ' + s)
            continue
        # Fix 'finally' blocks — strip the finally keyword, keep the body
        if s == '} finally {':
            indent = len(line) - len(line.lstrip())
            result.append(' ' * indent + '} { // was finally — cleanup code follows')
            continue
        # Java HashMap calls: idMap.containsKey(), idMap[x]->intValue()
        if re.search(r'\bidMap\b', s) and '// PORT:' not in s:
            indent = len(line) - len(line.lstrip())
            result.append(' ' * indent + '// PORT: Java HashMap -> std::map: ' + s)
            continue
        # Orphaned else blocks after PORT-commented if
        if re.match(r'\}\s*else\s*\{', s):
            # Check if the previous non-empty line is a PORT comment
            prev_idx = len(result) - 1
            while prev_idx >= 0 and not result[prev_idx].strip():
                prev_idx -= 1
            if prev_idx >= 0 and '// PORT:' in result[prev_idx]:
                indent = len(line) - len(line.lstrip())
                result.append(' ' * indent + '// PORT: orphaned else from commented-if {')
                # Track depth to comment out the else body
                in_catch_block = True
                catch_depth = 1
                continue
        # Java BufferedImage method calls on undeclared img var
        if re.search(r'\bimg\.(getWidth|getHeight|getRGB)\b', s) and '// PORT:' not in s:
            indent = len(line) - len(line.lstrip())
            result.append(' ' * indent + '// PORT: Java BufferedImage call: ' + s)
            continue
        # GLU.gluErrorString and similar Java/OpenGL utility calls
        if 'GLU.' in s:
            if '// PORT:' not in s:
                indent = len(line) - len(line.lstrip())
                result.append(' ' * indent + '// PORT: Java GLU call: ' + s)
                continue
            elif '// was:' in line:
                # Has both a PORT marker and GLU — only replace GLU in the CODE part
                # (before the // was: comment)
                was_idx = line.find('// was:')
                if was_idx > 0:
                    code_part = line[:was_idx]
                    comment_part = line[was_idx:]
                    code_part = re.sub(r'GLU\.\w+\([^)]*\)', '"GL_ERROR"', code_part)
                    result.append(code_part + comment_part)
                    continue
                # If GLU is only in the comment, leave as-is
                result.append(line)
                continue
            else:
                # PORT-marked but no // was: — replace inline
                line = re.sub(r'GLU\.\w+\([^)]*\)', '"GL_ERROR"', line)
                result.append(line)
                continue
        # JOptionPane, System.exit, etc. — Java Swing/AWT calls
        if re.search(r'\b(JOptionPane|System\.exit|Component)\b', s) and '// PORT:' not in s:
            indent = len(line) - len(line.lstrip())
            result.append(' ' * indent + '// PORT: Java Swing/AWT: ' + s)
            continue
        result.append(line)
    
    # Second pass: fix orphaned braces after catch block comments
    # Pattern: "// PORT: was catch block" followed by commented-out lines, then a lone "}"
    lines2 = '\n'.join(result).split('\n')
    final = []
    i = 0
    while i < len(lines2):
        s = lines2[i].strip()
        # When we see "// PORT: was catch block", skip until the matching orphaned }
        if '// PORT: was catch block' in s:
            final.append(lines2[i])
            i += 1
            # Consume lines until we hit a lone } that closes the catch block
            depth = 0
            while i < len(lines2):
                inner = lines2[i].strip()
                if inner == '}' and depth == 0:
                    indent = len(lines2[i]) - len(lines2[i].lstrip())
                    final.append(' ' * indent + '// PORT: end catch block')
                    i += 1
                    break
                depth += inner.count('{') - inner.count('}')
                final.append(lines2[i])
                i += 1
            continue
        final.append(lines2[i])
        i += 1
    return '\n'.join(final)


def _fix_pointer_return_to_value(content):
    """Bug 8: Fix 'ClassName var = ClassName::staticMethod()' where the static method
    returns a pointer. E.g. 'Frustum frustum = Frustum::getFrustum()' -> 'Frustum* frustum = ...'
    Also fixes subsequent dot access to arrow access for that variable."""
    PTR_RETURN_METHODS = {
        'Frustum::getFrustum': 'Frustum*',
    }
    lines = content.split('\n')
    result = []
    ptr_local_vars = set()  # track vars we converted to pointers
    for line in lines:
        fl = line
        for method, ret_type in PTR_RETURN_METHODS.items():
            if method in fl:
                m = re.search(r'\b(\w+)\s+(\w+)\s*=\s*%s\(' % re.escape(method), fl)
                if m:
                    declared_type = m.group(1)
                    var_name = m.group(2)
                    if not declared_type.endswith('*'):
                        fl = fl.replace('%s %s' % (declared_type, var_name),
                                       '%s %s' % (ret_type, var_name), 1)
                        ptr_local_vars.add(var_name)
        # Fix dot access for known pointer local vars
        for var in ptr_local_vars:
            fl = re.sub(r'\b%s\.' % re.escape(var), '%s->' % var, fl)
        result.append(fl)
    return '\n'.join(result)


def _stub_dead_functions(content):
    """Stub out function bodies that are mostly PORT-commented Java API code."""
    lines = content.split('\n')
    result = []
    i = 0
    while i < len(lines):
        fm = re.match(r'(\w[\w:*&<> ]*)\s+(\w+)::(\w+)\(([^)]*)\)\s*\{', lines[i])
        if fm:
            ret_type = fm.group(1).strip()
            func_header = lines[i]
            body_start = i + 1
            depth = 1
            j = body_start
            while j < len(lines) and depth > 0:
                depth += lines[j].count('{') - lines[j].count('}')
                j += 1
            body_end = j
            body_lines = lines[body_start:body_end]

            port_count = 0
            code_count = 0
            for bl in body_lines:
                s = bl.strip()
                if not s or s == '}':
                    continue
                if '// PORT:' in s or '// PORT-BLOCKED:' in s:
                    port_count += 1
                else:
                    code_count += 1

            total = port_count + code_count
            if total > 0 and port_count > total * 0.6 and code_count > 0:
                body_text = '\n'.join(body_lines)
                has_undeclared = bool(re.search(r'\b(e\[|img\.|idMap\b|w \* h)', body_text))
                if has_undeclared:
                    result.append(func_header)
                    result.append('\t// PORT: Function body is Java API code — needs manual reimplementation')
                    result.append('\t// Original used: Java ImageIO, BufferedImage, HashMap, NIO Buffers')
                    for bl in body_lines:
                        s = bl.strip()
                        if s and s != '}':
                            indent = len(bl) - len(bl.lstrip())
                            if '// PORT:' not in s and '// PORT-BLOCKED:' not in s:
                                result.append(' ' * indent + '// PORT-ORIGINAL: ' + s)
                            else:
                                result.append(bl)
                    if ret_type == 'int':
                        result.append('\treturn -1; // PORT: stub return')
                    elif ret_type == 'float':
                        result.append('\treturn 0.0f; // PORT: stub return')
                    elif ret_type == 'bool':
                        result.append('\treturn false; // PORT: stub return')
                    elif ret_type != 'void':
                        result.append('\treturn %s(); // PORT: stub return' % ret_type)
                    result.append('}')
                    result.append('')
                    i = body_end
                    continue
            result.append(lines[i])
            i += 1
            continue
        result.append(lines[i])
        i += 1
    return '\n'.join(result)


def _fix_null_value_types(content):
    """Bug 11: Fix NULL assigned to/compared with value types.
    For value types like HitResult, replace null patterns with a boolean flag."""
    # Collect value-type fields that are assigned NULL
    null_value_fields = set()
    for m in re.finditer(r'(m_\w+)\s*(?:!=|==)\s*NULL', content):
        null_value_fields.add(m.group(1))
    for m in re.finditer(r'(m_\w+)\(NULL\)', content):
        null_value_fields.add(m.group(1))

    # Filter: only fix fields whose name suggests a value type
    # Check if any value type name appears in the field name (case-insensitive)
    vt_lower = set(v.lower() for v in VALUE_TYPES)
    filtered = set()
    for field in null_value_fields:
        base = field[2:].lower()  # strip m_
        for vt in vt_lower:
            if vt in base:
                filtered.add(field)
                break
    null_value_fields = filtered

    for field in null_value_fields:
        base = field[2:]  # strip m_
        has_field = 'm_has%s%s' % (base[0].upper(), base[1:])

        # Init list: m_field(NULL) -> m_hasField(false)
        content = content.replace('%s(NULL)' % field, '%s(false)' % has_field)
        # Assignment: m_field = NULL -> m_hasField = false
        content = re.sub(r'\b%s\s*=\s*NULL\b' % re.escape(field),
                        '%s = false' % has_field, content)
        # After value assignment, set flag true
        content = re.sub(
            r'(\b%s\s*=\s*\w+\([^;]*\);)' % re.escape(field),
            r'\1 %s = true;' % has_field,
            content
        )
        # Comparison: m_field != NULL -> m_hasField
        content = re.sub(r'\b%s\s*!=\s*NULL\b' % re.escape(field),
                        has_field, content)
        content = re.sub(r'\b%s\s*==\s*NULL\b' % re.escape(field),
                        '!%s' % has_field, content)

    return content


# ============================================================================
# PORT SUMMARY
# ============================================================================

def generate_port_summary(classes):
    L = []
    L.append('# Port Summary')
    L.append('')
    L.append('## Statistics')
    L.append('- Java files: %d' % len(classes))
    L.append('- Abstract: %d' % sum(1 for c in classes if c.is_abstract))
    L.append('- Interfaces: %d' % sum(1 for c in classes if c.is_interface))
    L.append('- Fields: %d' % sum(len(c.fields) for c in classes))
    L.append('- Methods: %d' % sum(len(c.methods) for c in classes))
    L.append('')
    L.append('## Key Manual Work')
    L.append('1. `// PORT: D3D9` — OpenGL -> Direct3D 9')
    L.append('2. `// PORT: array length` — track sizes manually')
    L.append('3. `// PORT: determine element type` — type bare std::vector')
    L.append('4. `// PORT: delete X if owned` — ownership decisions')
    L.append('5. `// PORT: was throw` — add error returns')
    L.append('6. `// PORT: synchronized` — Xbox 360 threading')
    L.append('7. NBT serialization — full custom implementation')
    L.append('8. Random class — implement Java-compatible LCG')
    L.append('')

    # Inheritance tree
    L.append('## Inheritance Tree')
    pm = {}
    for c in classes:
        if c.parent:
            pm.setdefault(c.parent, []).append(c.name)
    def ptree(n, ind=0):
        L.append('%s- %s' % ('  ' * ind, n))
        for ch in sorted(pm.get(n, [])):
            ptree(ch, ind + 1)
    names = set(c.name for c in classes)
    roots = set()
    for c in classes:
        if c.parent and c.parent not in names: roots.add(c.name)
        elif not c.parent and c.name in pm: roots.add(c.name)
    for r in sorted(roots):
        ptree(r)

    return '\n'.join(L)


# ============================================================================
# STDAFX GENERATION
# ============================================================================

STDAFX_H_CONTENT = r'''// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// C4355: 'this' used in base-member init list. Java ctors routinely pass `this` to
// sub-objects (new LoadingScreenRenderer(this), new EntityRenderer(this), inventory
// back-references). The object is sufficiently constructed for storing the pointer,
// so this pattern is intentional and safe here -- silence the project-wide noise.
#pragma warning(disable: 4355)

#include <xtl.h>
#include <xboxmath.h>

// TODO: reference additional headers your program requires here
'''

STDAFX_CPP_CONTENT = r'''// stdafx.cpp : source file that includes just the standard includes
// RubyDung.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
'''


def _generate_stdafx(input_dir, output_dir):
    """Copy stdafx from input dir if present, otherwise generate default pair."""
    h_src = os.path.join(input_dir, 'stdafx.h')
    cpp_src = os.path.join(input_dir, 'stdafx.cpp')
    h_dst = os.path.join(output_dir, 'stdafx.h')
    cpp_dst = os.path.join(output_dir, 'stdafx.cpp')

    # Check input dir for existing stdafx
    if os.path.exists(h_src):
        with open(h_src, 'r', encoding='utf-8', errors='replace') as f:
            h_content = f.read()
        # Ensure DbgPrint is declared
        if 'DbgPrint' not in h_content:
            h_content = h_content.rstrip() + '\n\n// Debug output\nextern "C" ULONG __cdecl DbgPrint(...);\n'
        with open(h_dst, 'w', encoding='utf-8', errors='replace') as f:
            f.write(h_content)
    else:
        with open(h_dst, 'w', encoding='utf-8', errors='replace') as f:
            f.write(STDAFX_H_CONTENT)

    if os.path.exists(cpp_src):
        with open(cpp_src, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        with open(cpp_dst, 'w', encoding='utf-8', errors='replace') as f:
            f.write(content)
    else:
        with open(cpp_dst, 'w', encoding='utf-8', errors='replace') as f:
            f.write(STDAFX_CPP_CONTENT)


def _generate_main_cpp(classes, input_dir, output_dir):
    """Find the entry point class and append Xbox 360 entry stub to its .cpp.
    No main.cpp is ever created. Entry point lives in Minecraft.cpp or RubyDung.cpp.
    If a stale main.cpp exists from a previous run, delete it."""
    # Delete any stale main.cpp
    stale = os.path.join(output_dir, 'main.cpp')
    if os.path.exists(stale):
        os.remove(stale)
        print('Deleted stale main.cpp')

    # If a hand-written Main.cpp override exists (handwritten/Main.cpp, in PORT_OVERRIDES), it provides
    # the REAL wired entry point and apply_port_overrides copies it in. Do NOT also append a main() stub
    # to the entry class -> that would be a duplicate `main()` (link error). Skip stub generation.
    if (PROFILE.name == 'indev' and 'Main' in PORT_OVERRIDES
            and os.path.isfile(os.path.join(HANDWRITTEN_DIR, 'Main.cpp'))):
        print('Entry point: using hand-written handwritten/Main.cpp override (no stub appended)')
        return

    entry_class = None

    # Strategy 1: class with public static void main()
    for cls in classes:
        for m in cls.methods:
            if m.name == 'main' and m.is_static:
                entry_class = cls.name
                break
        if entry_class:
            break

    # Strategy 2: class named Minecraft or RubyDung
    if not entry_class:
        for cls in classes:
            if cls.name in ('Minecraft', 'RubyDung'):
                entry_class = cls.name
                break

    # Strategy 3: Runnable with run(), not Gui/Loader
    if not entry_class:
        for cls in classes:
            if 'Runnable' in cls.interfaces and not cls.name.startswith('Gui') and not cls.name.startswith('Load'):
                for m in cls.methods:
                    if m.name == 'run' and not m.is_static:
                        entry_class = cls.name
                        break
            if entry_class:
                break

    if not entry_class:
        return

    cpp_path = os.path.join(output_dir, entry_class + '.cpp')
    if not os.path.exists(cpp_path):
        print('WARNING: %s.cpp not found' % entry_class)
        return

    # Append Xbox 360 entry stub to the entry class .cpp
    stub = '\n'
    stub += '// ============================================================================\n'
    stub += '// Xbox 360 Entry Point\n'
    stub += '// ============================================================================\n'
    stub += '\n'
    stub += 'void __cdecl main()\n'
    stub += '{\n'
    stub += '\t// PORT: Initialize Xbox 360 systems here\n'
    stub += '\t// - Create D3D9 device\n'
    stub += '\t// - Init XInput\n'
    stub += '\t// - Set up audio (XACT)\n'
    stub += '\t// - Load content\n'
    stub += '\n'
    stub += '\t%s* pGame = new %s();\n' % (entry_class, entry_class)
    stub += '\t// PORT: call into %s game loop\n' % entry_class
    stub += '\t// pGame->run();\n'
    stub += '\tdelete pGame;\n'
    stub += '}\n'

    with open(cpp_path, 'a', encoding='utf-8', errors='replace') as f:
        f.write(stub)

    print('Entry point appended to %s.cpp' % entry_class)


# ============================================================================
# MAIN
# ============================================================================



# ============================================================================
# EMBEDDED D3D9 TEMPLATES (compressed + base64)
# ============================================================================

import base64, zlib

_PLATFORM_TEMPLATES_B64 = {
    'Chunk.cpp':
        'eNrFV21v2kgQ/s6vmPakyAbSQFP1Q2gikQC5SCipAunpVEXIsZdg1djceqGYa+6338yu12ubTd/U0/kDrOdtZ2bnmVkfHR3+0qdx'
        'dAQXi3X86ZW/WsEhjNmGReATBeYJBzafh37IYgGcxQHjYfwI4XIVsSXSPBEmMVr4xS79FsZ+tA4YvJyIoD/fvlq8LNGUtxXSNIxY'
        'jcK2Ys1ZWqVOFh6GIImNEEOSpk5OOHtYh5GYLsJ0xL0lg1Po9MoC61XgCZYq+pSlKYs8kfCm5qczURBR6PpuPO41GpskDLREGIcC'
        'd+cscNzG3w3AJ5yDU1VUmq7kKhl6asZj9hmMC47bk4JPjafqhgFLBU+y2p4Bi5hgFZNK3xoBmsytyT9H1kYTok3UBsrObNvJF5le'
        '7PRi29Usvdh1VWQnEJEdB80oQhu2HQdtuW3IcJHRYoeLXafgd5HfJT4uMlrscKENtmE5C0IuMkfwNdOhet7DA0bS75+fO848Sjzh'
        'krv5MjPLnVmS01rALHEflaTlbMO4YNvz9XzO+MfOfZEpC7dr514k61go1Y6N0805Jvf/qOTruCb90XB2OxwP+5Ohs+dR7unXhLok'
        'VKsWhYDAoYOKvIzxbxUpfPkCj7NVcBwM2Cb0WbV46eEMARgrd+RPDqJWq0TD5nOLJemlDJIoAOUnPEhHZfsRiEnl0TfjklL3ri2p'
        'imVSrvc+p6DhkSVLJnhGZZqAiXUPGIdnBGMNOflDPsqsbdH6ttPD/3dYrj1otbZ1IBeyGcpmKJuhbCZlM5M3I1/R2aHODnV2qLOT'
        'Oju3IlnV02cnoYZ+p9QinS0iCLHjunuy+9r0PCRJBHQip+Bk8OIUcnMBW4kFNOE1HMFxng7b7i9Q17Vy7fvRQ45iQSb+p8MzNXIq'
        'BdhWPrRVUWBryEOyO/FkpbIoZT/l1SP30vQ/catKMW9PDfOrq/aSCcQF02hBVHkw58myVLrgxQH4nCHgwKvCSsEaK+qvtRdIeODp'
        'Vqv8kYkPBj2Oi6f8ple0g5IeAuorgEdXh56/kBvBA/MRZSmWjOChFz9Gcpy+lb5h/0gLtzS75AEKmk2b8La6x4UKcz/I32+Hk7vx'
        'FBY00crN6vBM6XwotQ+niMPmQBPScMeSuWMmr2K77UJvcDy4m/Qvh7M/bq+mw5vr8Z+GhyOGXL1OYPRh1IZ1StcofX7MjzyurlIl'
        'W+9vbsazwXDUxxCMoQNr0zN86sHypdyk6NRG/avxcOAsuEut22rlO3o4xjBGZMrymodRVK7DUubrWWrCihZpZSLKY7E6cnhGmziY'
        'NBrNNKeaTfdAmUCa27NEVW+23z0p6nFWsXaRxKQq6y8FGg9F9X4OsQf6CefMFyzA1zigU014kGfBT+JUWHKRcl9nw4o7goMeMoSI'
        'IBVXwbY2vYrBEEoG/r0zIMHXVqueERLGrc9p2J6ifFODurCZxzwKeWpweEKH8LoNXXA4tjielmIlJ7BUTcGoM/qoPG61aOTqaD/q'
        'vVvQue/9oMbrH9bo3tuDm2AfwuKtRHeMAf6f0R3/dD5KxfoMku7iiLD09WuRpeVZrohy6O3dEDGnN3GUQX6FJFjKy3iB0fxyXi9H'
        'aiXhMhRaMYUV3fnk9xdlHt/wb+nFvpnVZG7vY+0dvHnu9qSvtZ3aHNb0bo2eu4oZmXt4R7ApmY31Rfa5MX0rEwY+jkAW2GawzI2t'
        'D7/I79gHB/YDO9Nj10RbHW8TPcAHZrY4+ccvfa1uCmYpAXsmJgKn5HKSrLnPqBnbRw8B6Lnh+Kz1Afc+v+d0/OGGOTTtprPp7VX/'
        '+nI8HF9NptKqNXZz4dz75E2ZGMjPQF2a5jjpw5Aq+l9OlOLW',
    'Chunk.h':
        'eNqtUk1PwzAMvfdXWOzCpkEZnKCnfQiGQBxgQ0IITWnrUossmZJ00CH47SRpBy27rlLq5vnZfn1JGB7t9QnCEMZ5Id6OcziCW1wj'
        'h8TtIZMKMMsoIRQGFIoUFYlXW7BnBR3KbO8M7uejp8n87moxns7vbhbToGNREribsCUi4UWKcDCWy6UUx/lBAxsOR6M24v+rDc1Q'
        'a+TMSOXwIOFM68qJ4DNYFTGn5CIA+7huwFgcR37rW/WAu1AhiRTaAFmTPk76UNq1OdnJDGzGrs2gyviXNsxQ4vMK44K4meWkLxVb'
        'YvSfUKxSZlBHQbBStLaflbhYSg7LRUrKlFXN9YQUJuZs8ojK4MeoyDJU5z1LWjeA59OXCMCe/exdAmclKu2r3agtcywLYRyxqeXP'
        'th7ohfndOWEN07yRh02v+r8OuVjWcVNH54/H67gZdKux31WnbsO2taS0vo+Hjuvl1wSf02gmzpBWVa3fE0iQeciZwnRLaWZT1EbJ'
        '8o/Qtrwe784rbc3/ssyOlUWZM3bn0v4AVKIQ6A==',
    'Common.h':
        'eNqtVE1P20AQvftXjMTFpiUfGChRWiSDE0BKAjIfor1Ya3ucrLrejdZrcIT47x3bxBDS9oQPyb43b2bezq7d7e596mN1u3CmskzJ'
        'zgL2XpdgVkvMgckEuIxFkRBIlYagiFZ+IefwEKkS3KMeLJU2VOKTPe3wVCaYQnB3+tO/m52HZ1fT6dUsvLB2iOYS/xKhpMYqfC+N'
        '6CxO3hNkN2NmsckmbjLYYsoPVDnXbLngcb5J5ybhaosSPNrktps+YmyUPrGquY+VfmI6gQRjwTQzXMncomWewwQfUQzfgwBpJBr1'
        'mrwWbPWGRtJws1qjXyqLOK7R2aKQv9fglos2cIt5joKRnzUz1kVuimxo5UYXsQHPOz1twQU3AeaFMC1zj7H7DmiDZQuvlVjNlWxN'
        'FBH1rXZ9LlTEBPiuTxt/5DGCnaOBaAUZ4xLYcil4XA/DsbA0qCVc+lzT3FzfrxMGuzAPl3RYDWzKTmnUENMEDZMmb6/QNLy+bG9N'
        'BcDt9A/6h4P9o0P38HjwbeDuu8cHR6m1QwPmaV3Lx7lGuvNGgWYJZ3Qs6xL+6Dy8vQoDzwe7LteF/nGv00udt7vp+ZWElGA3MRJV'
        'WqcufsNSBI0CWY605VirNvPGG4/CYDQZeTcje+nAM/C0+affvZOgSbKdYYXhB8zuJpMhvMBLXTig91VlkArFaJxonhAl9Oq3uG9x'
        'KaoOTbBRjqu17VjPFtCj0RQ0artWOJoUtkO+X3Hgzfxw6j0Mrf/0yqrjo24ZK//dr2FIes/E19c4JRD64KSRwJdNt7ALdiOnr1Uj'
        'cWpTzfFB5W3r2/AH23+UiA==',
    'Cube.cpp':
        'eNqtVlFvmzAQfudX3DppIitrIbRd1WwP2/oyaVWrtlrTTlPFYmdBcjAikAJV//vONgYHQpa1QwKf78533x1n+/b33/3Xx9rfhy/Z'
        'L7o3iWN4J0mYc0IZhPOY0TmN0iANeYR6/9nx6zCasIxQ2LlKyadpvjfbMXgS0wrnahYQmiwE0xLSkxPxtcMohfya5ufT6cIBMSuq'
        '2cACfE5gfq/ldr4iclCkle2iJcptd+BAIb8lfjX7kqdKoolSEdaj9WRZSx4SUOgWNNW2N2F8lIYbjPCxVh1VoqIRFbVo1VlAyGee'
        '21PGA3TlOqCooqZKVzl+UMNMDUT7rxZ6wrsLu6AsDR5GhrQQ0sKQzkxpKaSlISUjS4pFgSU0SCkcw5ImaTihCyn4jhOaQ+baArCA'
        'KkC6e+5UfQejFTXPzr222nFXbajUPKV23KfmK6cral2nzMDm9WNjBjavHxszsHn92JiBzWthqypC5/GH+xOznrmjNt+TfK/DH0r+'
        'sMP3Jd/v8A8En3XtH0p+1/6R5Hftv5d8v1MSRzANUA4PYToDDD7NEgoTzhMSRihfKRSh6SpEj+jbwfjwHTroD560x5iz4jePqsxc'
        'qJktlzrmLtsFgu+DeQQIniPN9D3d9XJNywa+M/2rDOiehp4hEiZeH+H7a6B7beheC7r7Ethbwh2amRZwBezMWwN32IY7dDa7dP81'
        'y23Ia+D6dXaHIqsqu2sLw2/D9bcoDPcZhbEN7IMatldlWBTFOtgHbdgHf8kyeWmWewrjsC4MUcOiOMQWXFfHh23Ih86WW+g523BD'
        'CJ2L+YIvqrsyzvUNGRc1VeprMccA4lwFVgi6UHQp6LJtOKERdii2XhxOwX71+z4mPjmlSzwGB3VUCcWDLmpOw89ZyAikSRAtpjyZ'
        'y44LcEjCXKqc+qfjs0/Xl1/Hgnst9BxBYesx1sStJu4k8Y1PAjZqVktjciWT5u03jSXMAQZfDtrqaEzq3kllZVq0O72Kt1rxVjVI'
        'vYpjrThWLdXAuBn4/FcY0ROQ4OAtCL9quFXDuGX0LGNpGLNC2pRxO9CY14g6WPqWrVq4e846Cd2I6SpY4rWWJVgh2ILxBH927989'
        'Z+RGanyEquXFkr2Xi5T/TqrU7bml+WoN2RiVVuoEpsFVSWkA0vSmQWgbJtqaWUzwYlfTL3hKpEGULmwjV5dyF0HAmOoNVI/JE5Bt'
        'dIhpcUc4fICjEezuhmpTPVrN4VCfP+HPPb0lFYgnw8ki5dhq8NVUbYinL/LeeJ6sPw7S38o=',
    'Cube.h':
        'eNqtUk1vwjAMvedXWHDppEI5TdO4wb5OG2Jj0oQmFBp3jdQ2VRKgZdp/n9uEqtsOu5CD4zz7PdtJomh01sWiCOa7LY5TGLUO5Epg'
        'BonSEKdc89iihq0SNZRcW0P5Z25gKJNCYALL1eztZvV4v5mvZrebBzYkUBb4BydCEWc7gTCYqzxXxTgd9LBX1Barn9hCZfWHS2zm'
        'vVP6wLUAgXFGE1qpCkaeMfCccoHaTJk/NxfCPlmp5Z5bvGZAyxWAfLMnR8Zo1lfv0zbiy1CodJ5ZX/qQLCzB1QtWT0li+ljdYazc'
        'bTMZuypJpriFKoQ6hOO0Dy2VJbS1R7LEa4JNp0GjeKoRtvon9Qsn0Zq9kgIMWh/6n9USuBAzVQW+iUno26k77zhx5IPbUreJvgYV'
        'XSgTdLN5iU6hn6uRfoUOCPqiEYd0kgnQ4/3+Dt9r19/G',
    'Frustum.cpp':
        'eNrdVm1v2jAQ/p5fYXUSgkKBmGrSmvYDtGWr1o6KoqkSQlUITvGWxFnidEDV/z7nvQm+jm20QssHOJ2fs+95fGe71TrY6qe0Wqjv'
        'BT4P7KbhuugAfaXkJzJjFzICy6LOPaK2axGbOFznlDnIZB4665x9ENFbTucddQwrmBG0d8NnXXPRnO8986WZFpw3c31GPD90Kgkg'
        'pXR05N9Rx+e6YxAtGz06SoxqTXlUkPhsYvuEV+27hHcDtRvIpyvCzNxZq2nKUzbJfr7GPeHlCfNlm4ZuCRV1TjKMFkE8wgPPQZXn'
        'GYrpHxid5VM7zLN1SyRybekOqVKHi7RmJF3GtJjOka3fO5SHWpwg/4fHzWo0GBFLkx+HYZNxe4L2Zc46GKHKItSXIrAsAk8ifMJd'
        'AXJrneRkNAVI5/cgvAmoUwatib++c4nsomc+klB37lGD+KJbmI38qAyRv/Q5sSOY6JDbq+5oeHGLDIu6Wu4MAxdXgcWpay2rlXC0'
        'gSpJIYdF+yCaMEYV/a7HvsX+50qKfM4X3NMNjvicZM3rhjWTZBf6DWZPqUNmceKLNHRI7+c8BpeEGl58/DSajLsTUVhhjs079VCc'
        'EImpanJ4L4fjHI4h+GkO7+TwDgQ/y+GHOfwwgZf6JYopKXVJTDnby/P+Gtk6SDZGF7nWQa4xuki1DlKN0UWm9ZeZhiEloj3GObOl'
        'VHuD0WhwBZLFGoAH6IJ4gDCIByhjKeU4qER6xFwp49HgGizktXQiMFDGcjBQxHIwUMJyliKivK+68T3miKqm7tXKWnZPP4NUO5oU'
        'DXAF0ABZAA2w7cj3VISU6PY95vCUr0PWCfeHgy9w465llcCBUobgQCVDcKCQ5aSjmPhxMWXMyi8gl4kL/8JJr5/4thc3QmwsU2OV'
        'PQfEsyx6I1CxelsTf8fovYbqdRpr9pjd2dRE+cNmTCP19tFCZFrw9kLvsuw9Db2rsldQPharNttmLVsmX/DZi8fULT+5k8PvScl/'
        'EwT3AiKRw3fnxCMb6JEanj6jgb9D6hykGb2OQEYwJX3xYl+uaaRmImXWKrMWOBvNrBXemmyqXDdVLpy6SV3BYgE54B3IAdAB74AO'
        'eAd0APYC70A94B2oB7wD9fAPOWx6fP2fJxeqVAr6vOYJtcFaWzuJ/p4XfkNef97VW98v/Ib7hd9wv/DbvwKLp4TBHJ+jbrfXqyBd'
        'n07T3k/mKIJDQHPRbkTI5jI1VqkRHjbxUGqs1Ohd/guNy19g',
    'Frustum.h':
        'eNq1VNFOwjAUfd9X3EBiwAgIog8aHxg4IAojOEyMIaTrOmjStWTdEDD+ux0bC8IgPmCfbu459/T0treVSumsS6tUwPBDGYReeQYl'
        'eKPkE9w4AThkjPKp4px50zx1uUNcGI7099ao354Yw9GrNepNOlpe5SknWZAq45iFDoFcU3ie4OVZbifXaOh6lNEwQ1JuT6V9afPQ'
        'ZhTfa6CWDFBAMWDBZQCUBzDstjsWPML1Qzb+8mREcPUIrJuWZfYUoXaEYJkDhd4cK280nxVcPwIbQ7Mf7X4b49mkxgn3+gnrzROu'
        'W7Fnbe7TBQpI3DuXCRSAN0mex8fd+KM+/iWQ9BzkhColxDGJNHbanxAKxcMDJdAlTEmQRbOFYDAXyl2Xb/HY0fIqsbbaBuukcFMj'
        '5zPikz8UbQMfOTSUuxI4tImhhmF1oFJNZdJonUbLWoqm0bq2L/y/mvGdRrNxAQjZdnH/VheCOoARU8Oucnud34Bc+B5idE0GDHFS'
        'iN6HpA5RlG8llifcoS6ofyRjZH8Ak55eKg==',
    'LevelRenderer.cpp':
        'eNqtV21v2zYQ/l4g/4HrgEFKbMdah36wGwOp7azdvLSIEwxYEQSyRFlEZMmQqETkkP32HV9kibKcKV2MRKLI5zkej7w73ulp/1V/'
        'R29OT9ECP+DoCsc+TnE68LZb1EfqM0M0xCgS4yjPSLxGXpjH9xkim22ENzimLiVJLKS8tl4/ktiLch+jt0vqnwfFIHxb7zR1Nscu'
        '0jyj+abRe00i3Oj6Grlsj70MXbFy2Xv0xphnNDI+Lfl1rOxjH71B8BuhzZ38tuq9PehVlrMubxaLWi/FWYYjlyZpOfK3GpT0/sT1'
        '/QXJKI5hOhqSzB6rYfXc3BVTtSFnJeGR+DREp2j66eby97vl57/m4xLLmlgfbw9heRMbYrIOaRtYPUlMEU2oG+2IlXLHtcmPK+G7'
        'ybySEuNHJMeOv9Vk3RoTBUmKLDFbAYThGF4fqqnG6OSk0ObVhjQ4THGY5LCKw+wKXOMZXK64XHJ5xeW2SWjwS+MUQ+AXsP49AzaR'
        'TCBZFyQXSN4FWTiAtAp0ghy7kw4SzzrjucTzTvgWCQHo5qCJcYhtpXS9a9xOZTWqPNO20r/edYDKa1R1xG21FqOv2yLADAQiQ1Fa'
        'msnDrk+mXT/5MMhbZJae8E2Kua07hAonPThFPTgfPdh5aDvQhn/u2A1hT9Wnbj6ZYaMKO3qS6yoOCWFPLbHvHzP4VaFK2FFHvX3X'
        '0wP9SYo3yQNuDWaGckqaMsS+uO8PM4YvE+XLBHy5Jk14MzkcCXwcYYpru3Q73rNzhft2u0O2LXMnrNqK0u4PCfFRw/ipbFkqYx2j'
        'rXz3pDlks9oNuRRBWOUkotdg5ovU3WC5YIXQ+fEYBaoBQ7prNFpjqtuW/Urx/QVGr280HAOtX3/i5Sv8OS4Vq20AJEh3tbIP75mB'
        '1UZUBhu3+chB+2+Jd9+wfmXyIEpcioQrvRsMAy34/PzjR7RKRDRQ+P5ktRqs0+TRgo0Tf/sGljlC2MoG4qAoN6wK4GLIkmOOCLUw'
        'm13DMIPO6nRm0lkbnRt0Xqdzk86b9JbUXOjcXDhdkjLTWZk5L0nHXOdj7nRNxFWo6k9Itkwi4ouLoVVAKIVIatv7nBYxzSAKsmJC'
        'LXvcDt3pHLieckTV+oDeC71F225nHphb/ITa4OaJd18e7AuQY9XVghShltWT8x1S76nLAoMoz8LWFT51yD3/Edc+gfG8JM4ogtYV'
        'zvKI/oRCM8P8sL7b+u/8GX4ghrlSTPM0Ns4h1DTz2F1FGLnRNnQRtGJfVC9iI0JI6JG8y+IgwB5VlLrw/mSJqVJxCQUOtmbvZlfL'
        'u/PF10/nHxfzy9n8Urx76PrqZl5apIOA5dVU0nsIvmVLdEmxL5Aymy+vm2K+XM7tpgWmbuTlsHkYbfNI1m7KGH20cakX4gz95j64'
        'IwRDlpensBH0mmzwHySKSAb3fGc4HAzFrWU4+Bm8fTj4ZSdaJBW0Jg8gQ8lM3Xgt7ZsmG4mnCbzeK8Lszy9XM0SJzEG/YpjFu58m'
        'eVz5i4qfQk0BAYUCy5J9tmRpXQKtTKC0CZoLvgE2SMUFnAfso0zWceaWK6yu8EajPMM3O4Lq3elUgbY+GFF9TsUZdWOaWXvmVhsl'
        '62Xp248ESqvS8qtUzB6DO+1fwBqxwxzykghuZNIyPVR/lfBOUSAcFOLBxIOLR9A+m+nj/8MxLs4Xy7n93H0mw3RGUspUuhj2dP7q'
        '6UTU0zlP9+s3XHR3IQFS5enZfp0BebGtm7WjWTuat6N5O7oKUaDTBwQ+I9P4Ln2KAkUPsOYA1wO8OSCqobN69SDzf3Xl6iOnLl+j'
        'WYlmCs1a0VyjeYnmCs330IfT+tnL8vrZ9yX2sxdk9teuvvqT3RntVF4dPOoUXHQaQoTEvrKiPtT6TFdHejdfIbYAMrd6cfWStbvo'
        'PFGdotJ+zsNkwJsmUb6JW2bnpscx5xk1hq16OF0VcaOoVKBlElBA/e2uZbLSr33L8r32rct0MeW/KLaJZg==',
    'LevelRenderer.h':
        'eNqtU01PwkAQvfdXTCQxQATk4gHixVLFSIipYqKXZm0HunG7JbtbBIz+dvej1Vbx5h66u29m3syb6Q4GvX9d3mAAM9wgC5EnKFD0'
        'U+iBu0hQKQIzVigk5SuI04K/SB3zz0W06FInXEK4uHicLOZX0Sx4CGZhMJ8EYRBGU6+lrZTj3w6agsesSBCO/DzLct5Pj2qYlXgA'
        'mlGpkBvVdZNvZDahKVUhyoKpJnyPUiIjKrcMppmXuXglIoEEY0YEUTTnnj5JCbeM7FCMvfLa6DqMYF08MxpDoyzvzVsLuiEKRx7o'
        'JZUmjCHOuVRAuQJ/upjfRHfXTwGcw/BsbL3sx/J0IYvs/JzB6uoa0A3SoYYni7b+L2j3G9rXoW/xhlJ93bREJ2b0XUkltF3WZavq'
        'OJ6PpkenpmKT0wSEtbRd/7qwtvuJrcgeywDru6bxyw/PutlR6WG2XQ+/xnoMaT1v9SqqQZhkKJYkRsdFhSoIc5yKMvRTwleYtE1N'
        'W1fazm37Kn09hNFVqvycFRk/ELkvCU7LfXiIgjBWhXZMw+u/iXWQqCY6YOeYT5uc+3LfDqscJW5yvWu+lm4UXZo+/PnkPgF25Fp7',
    'Main.cpp':
        'eNrNGGlz2lbwO79iQ6cZ4QNj7LiOadKR0cPWhGskHKdpMhohPUC1eNLoSCCd/Pfu04UkZMdO8Uz5AGjf3tfb1dHR4U4/taMjGOgW'
        'axquC4fwYeqs4OSsBZQF3hpcx2IBouxY5i8WM+zQpFBXA1OcrZqLeg7WdZZLhxVhSjhdSyGbF6ETugpCj/pFqLrQTepFQG7dle1M'
        'dRukEwmc6d/UCPyaLFke/jmRXu/BXHP50RsY3vT7nRpypB6DDEOiXyyDxnjmiRk/diLGiXSYRwL8LdLkPKLFEw+9nIA6D+IaC93L'
        'EHfvfNS867AAAwyuHix8jHqQ+BF0NMd20H0mzDxnCXN9SS8+JeifYObY6FnOYWzrBoXEqqbL5qAzE7jm0YPFIFjQTE5MBwzFQODA'
        '2gk9aK7oavfG+YEeWAYYDvODSJ09mBBFEeWhNiEfJjcK0cbi5BrDXY9ty4z7lDOm3qli1L0WlR9ySV1Qf6bQ9XTbnurGHbg8AovY'
        'sUn8wNQDHYTQx/BZM5hZNoaUOdz/ITMbz+xt7BQw7otdcj3qS0TRVPkjQQ8dn2XODJlvzRmN84QX1MYECTX/a4t6DypAp5/RtV8c'
        'y4QryqinB3S84ZPUjVAU5R2UZZcB00btnxrgZ+Z4IHBT1qh7q4M/v28pgdD9/UaEHhMVCFcx4aqScJUSFon5h9NaJqcW1lWW78Oq'
        'wc3vFKh4NXsUnQA6WkKNO+pNHd0zeW3zFlNAnjqOnWJxOcIKjuC0gaxRJP/XgF+hDW+4AQXC7VhxTfeh9RnZpAz/AA8uQPCQU7vx'
        'OPrjIv2c08+fQN8u0k85/fQJ9Cecvv3q1Qb7ey3+/v485SszK7B02/pGIW39Ueak9+7uhV4rRL3pTyLJeNEJaaJvEmdzM9YSX8UX'
        'YqpgjPZaQKimSu+090RR5dEwcTE2GiEliS/RcmV4FEuSAdF6otzv1DZeTvVQaQChCy7eQNhDsVU4DNPXw86KGewX/cNJUNYYrSLD'
        'CbZhRRwQbPEq4O3sujH7j9RzBnTpeGvhZQQ+AB9d7syE6KmRqB59RZDmJbbUy3A2o96tZQYL3rfa50kRlDGuqTVfBIjyW/sejJ7j'
        'LXWOgZr2BhPtw7lyfnV+eV6N3cXuzJGP88eD0A4sVV+6Np2sXRrzGmAgZVUcjPtEG46GJE+gftVdguyMRK56K45Jr0e6E02S1a6o'
        'SHnsRTzI3FrMdL5mw8/mPD7A7vgGemJfLUgiTJ/aVAwDR6JusFDx4jMsGzEnyk0BsYxSdovUPlULPunZ+tyHrPskwNC2VcOjlGkK'
        'nWGOLBRMR5ldf0PMswLqOJdBMl7I3hfdjsWl+SIPMVfei31NHgyIJIsT0imkYlISZuSdCJjWz4J3zDjRD9/GaLEPhSzRWwfZX8SS'
        'yPvJn2OiXYv9DZz7uYDVVQgqgUiKdCviXIHFhSPGWBl1iarKw6sNcpLJm+f8RBpB82nNy5LXG5GEhde4pyQX3n3laNKZjhmIiIwP'
        'GPwCp37aHjKph28RV4lQVI7BW4Siah/JULzsk4MoIRKlHkHWRd8MRhLScb/gg9a9bXS2VEuHHRxvMMIWTptCtKbkALxjuNaK2sjc'
        'xIHWuWtUKxEXWKJFKxLMy0sbyMOe3MdUiUAYkJ42HmHu3GfMvXzEq93wkcdlPrz+895JQqpqo3ed57q9+rgabJaFaEMojvi7lxqN'
        'LFxuuutlF9goDNwwkOg0nKsBD7so1DkiT4CHdGw2m59YvZRYE1x2cTOxYwOjRWCTaJwHH6aTmBV3Od73EmkXF/ZG0R5S9ZBIqNo/'
        'CndnmV/1LVplbn7/yuZ8HG197oPcxLOxNzE3nZxhzvvq0dzT14WFghdQwjwje2DaPjs9gOPX7QM4O83JeYqn+EwmFEa21Lj85FY/'
        'eHiqKx6XJ+iHT7PDRlVPzOcHXxh0I8jtXeUMyW3wP0yPra2ykBsFTo9PjGwR/7ms8O8sdhg4jG4lRWb7Y9Ki3cIWdvyKf7VahcR4'
        'pIMqsoJT/g9SIum229X7Ig4SvHxZMjM5aDxbc+7aVGc4Rqdj++6FREt3Iia/SiQv3C4uTOoHnrMWEn+pYo9oCukTUSVC/q675xx5'
        'Pp97sjebJHqzOX6eN5uRizTNMKlhwxLzIr9vVSx/5Vkt29Iajynzno4dxOSNycqxPpGKlR3nalVXyynkxzHM9HmRBZWzFn5enYRx'
        'UaV8Ej1G0R8PHpnexVnhaWrbeTH/QeMqKTjTeQHvwPyNYeUMkr7HYSZ4IYvwoqP03fdeBMJ+yejXDCiUh36Oc/i2OmjxGTIXtjua'
        'SW3KBwJEKakVW17b9sL32r9Iza4e',
    'Player.cpp':
        'eNqtVm1P20gQ/l6p/2HU6pCdkhdyooXk6CkEjqMFiiDoKF+ijb1OLByvtV6HOBX97TezXid23poPtQTOrmeemXnmZbder/7W5+2b'
        'eh1uA5ZyWXOiCKpmATxUvkrBEaGSIgi4C4MUhmzMI+aCP44CPkYRpnwRaoyH2A+HMOGOErI6YDEqjMVEC0HgP3NQIw58ykgTuogj'
        'GYK7nJR/d0jv/dAJEpfDu3vldrxpbfSuuGnCpU3t+kUeVRglCmIMioMVc0URj5kf6rAhECKy377hU8VlCI+XN7cPvf59r9M7h2Hf'
        'EHNPuu250ECIYPGxK8IQ6eFum+xmTrRa2du64hMeVCCgF1oBfFpwrnNg5Zs/sv1xf8T94Uh98zxy8gQOah+bHoK+FnF/GuBMT3+a'
        'CN+F/LvynWdrAToVCDRtZ4uUFqlZzGgxM4vsv++BtRqWcdsA0mNIuuhcn992zvaASD5ZYqtm2G8v1Ba/MDmnzHmGQaKUCEEJkJxi'
        'jkTsZ5WXS5JLiFJ7OdWiMewtWe+fdrpf7YVCwU16NO6tiC274MjrBp/OOHNnIuTUHVguWOHWz+bhH+Dm+7rgTbEXbGpxiEdCKuj9'
        '+3B9et+77H7tn513zp6+3ZwjN0eNRmMTExcYuBol40FMuYMJCxIew4uvRnPDC3kvEEyBnCKmxQax5ibukfbdow2f15m34W8oikEd'
        '/mx++vip1vCwFhv4aq/gp2vwv++G/30H/GCN/1e7+X+1i//BGv+vdvP/arv/pcx1oggtSZGNS/CkGIOkFgadypJseicU+DGk7AWw'
        '8z1V15L2PkzNl8hXzgisJKq74iVcKS+TGaHuI86p35q1w6Jn2kD1hGqjMhdra9s3fIgd6bYgSuKR8RB7Diffc7ZaoGhnPpxQBWxH'
        'SaI5RBJt4KcbYKdkcZU7Wlv5C6rHxK6dGT0xy/Ya0c9QljzelpUuC5wkoGHvCfnCpAssdE3U2TkWZ6nCVJQUrxOkecwoC3SoOfow'
        'q0fUlK4vUZOS7Bh0+o1HyF0ySM+ScEiHbKuEBqT0SGHhCepZaOyOuTay6ojYszQptNFeVXpCJS20m84/QgKOHh8HhWLB4nS2/GGI'
        '23h6a0W7tdwlGTwdCcRqBazr/u0lVv/BkSZ7Lb+ZpiF2JTpTKELxFoRULv6EQ4x+UK1kzGasbgBcjnxTiu8KycTzgcuIh65PeZFk'
        'yaDtZ62JzXLcwFE6lBznavc/uwR16c3LBHvQonD2yQdszKxkaBfX+xhHsSlNO5LI42avC1IUW4mrDbFd5/mLdZ9XM97Q8y9swvQ8'
        'agLW3lCKJMQQcd04pEpkvly2S6WQTwsRXmgNHHYEYUZb4/AXwy2vplbOUn1AB7c+nYIUSVGSedysp4U54tIUmddJhUZyZeFQweis'
        'KPm0TTLDNJRX6BDZhmhI3yJXCvdLgqNKh9Ex15LyHPrFLaRjw97enOTN95GUctGoHTS91fvIa/EuRhcDySZ4TzR3N92oVA+ltM2F'
        'qWz0oGM6b570ncVNiqK3plgtKf7N5rVnroguVMin44McNM13jvKd2YrM4sa4FPSPUsK01pFXzk55E8N+/R8xynyb',
    'Player.h':
        'eNqtkD0LwkAMhvf7FaFdVLDddfKj6FBEBAenct6lepjelXotFNHf7rVVUHRsliTP+waShOG412BhCFviNRbBGcavElBbZWsQRtvC'
        'EKGEYw0nnmHOpZvoeQVfpVpiCrv9/LDcb1bJNp4dol2yZr7DSuMfxQ1pQaVE8BYmy4wOzt4Hi9oDGsYE8ev1fdgE8vJISkBnYDfW'
        '9RMGLjrTIMYKaQTUpOG0VSpV2JITPF4Wh794ZZQEq8SlUe5O9FFLlYL77u/uT0lPd9o=',
    'Polygon.cpp':
        'eNqtVG1P2zAQ/p5fcWPSlkrpaxgfmoG0NQMxIahogQ+oQlbitNZSu7Od0Bb47zs7bwWqfcJSG/ue585399judtsfOpxuF8Yi3cwF'
        '70SrFbSrFUjKYyoZnwNbrlK6pFwTzQRHjw9O4TPjUZrFFA4mOv6RrDuLgx1bld0r42RBMDlljKaCWyo1XUMi5JJo84FoQSSJNJU7'
        'dbiKLCkQBVOqFE2JFrJwbDlKyyzSMEKvwuQ8OYAjSQUGXHuw8WAbWFN4d3UdQiRSIYMdTuZBHjgvgePkgsVVF4fDYnu3hR5c6TIs'
        'S8D9NH9YxX4c0pxFFJ6fIbcbj0TGNXwHv2WZZkiqM8kxsllgtT8zlsYlHYiUZAOJFEv4m5HYcpoyLEvdH86KVKsII7HaWAi3VsCM'
        '2LhSFITEZCFlf8yUzRknKfwmOQHX92DgQd+DXpGYabLLMFUGx+AH+Dk5hl4A7TYrCE91/pYVrw0PzxcLasC2pBTvC+RIqHK6Z7OG'
        'VtSAEWYdEyTvrITqrPfimxrf7MW3Nb7di1tZkdNbn5ZjLy2zYTLowtFhp5fs5dhy8L8L/qDmvDiNBNyQrWigBehH/ElG+DxFSR6Z'
        'XlSixLjisTnApuehH1YhrlaUn13AaHQH7RMDwOhuCBnK2PMGXh8Ij3Hme4O3h6Le5/6obHNj6c1KGcz0LdhvwME7cNCA/Xeg/7+w'
        'hw3ovwO/vd6zKv4Gy1T2FfiqqrsQ0yjFa29eKXAvr6ZwentaHMbdu9Y+mVBddCJsHNzySRkO1UNeg61gj3soyeNYsiXTLKc3Yxc7'
        'P54+TK/Pf1yeXfy6OJ9M7V2pS/BAsS0VidtI0MLAL84/YYKvlQ==',
    'Polygon.h':
        'eNqtk19vgkAMwN/5FI0kBuYUUJ/G9jKXuIdFzRKXmMUYAse8BO6W4yAuxu++3oHgnH9e7APttf2VliuO072pGI4DM578fHHWW0N3'
        'b0MmRR7KXBCIuYBwHYgglERAyiOSQByEJEP0xr2YNGYRieF9/rx4mU/Gq9n0bTGeTlavhol+ysipEGIsTPKIQGvE01QN0jrwfRAh'
        'yUb5jHKo/YzG1gCUMg4FKopTfQ6XvvZTJrWTbEY8Z9I3tLdiLVuflDwcZlluGdju/qaXL7nTqdm9rh2q/HNlDoLbOkXdhKVQCk/g'
        '+qgeyyrQbuvD0IdOhzY1G1RJPSFdIq87QdOvc3ZG+bzWeO6Wuqh07lVn79w0w+NJ6lbcphV32RMkDb4tK054IG1VtjIL1/b/s17D'
        'esese4XtN2z/AuudYgcNO7jQ856tPmjBaQSC4H4Ly8ZrYxmu1A63ykQfjQH/wxO7/QufsBPa',
    'RubyDung.cpp':
        'eNrdGl1T4zjynV+h3auinCFhAgyzs2ShKiSBoYrZoZLMDcnVFWXHCvHiWC5/QJLdud9+3ZJsS7ICzN7ewx0Pxm51t7pb3a1uKW/f'
        'tv7Sv523b8kw99b9PLrfn8UxaZFPbhCRe3dJySx005QEyzikSxplbhawCAj+Ygn+FkSzMPcp+XGU+d35an/xowIrhdOgY7rK8oSm'
        'BjQIqQ7pLfLoQQddJHma5UsdOFq4Pk04O7TIZcg8NyT9oz7x6WMwoztX/SChs+yo3+ffP78h93exf+SLT3JKfv1yfd0RxGC62PVJ'
        'CgajO7dXv958Gd+Nxt3xAGjuxeAIxzo7HmNhBeyxKII5qA/s5m6YUsGv0LWSQUK4EBlNElgwCSoFseLOFm5iIu4UBj45Kd6cxg6B'
        'vxOyvHsK/GzhHBx+aAtYE2ALGtwvMuenQwU2Z/c9FrLEUWBZsKSJg7NUsJA+0tAGG9IIFqCGH4fuugZcBNmQpnmYGfAkj6IAxOe2'
        'U8Ao7hie9zT56qY3YMqU+iZWSOcvI80TWKlUVRIiJBuDogj7feebas5/Kfb8nRP4NM0StnYaHcTki1+hB1GQlZiw7D03nOUhuAkB'
        '45IZWpcPBVGGX7CAPx+2j47aHQ6dh8zNyDwBcHv/eK4B7znwgw70AHiw35ZA/jifgIvCZEMYcvCj4Tg409kZOXjfILvk8Pi40dFQ'
        'L+uoH+yY5xUmIqoolfsADsRc7/P15+Hd7fDy3EFpmnwi/jxvKOKCjUY0q6wDliGpiGM+LmP65CSl2UXhnw5Xv8G1fIsygAmapIJe'
        'WqHnJVQKYPDu0ygNsrXT3m8fzg0Zr2BdAzcMNpTwiJAq83fQN6JPZMwj5X274m8hxnQSzAg4JsuTGRU6YsYTroNvqSPJedoTcJA0'
        'AUc2+PYSip7FQ08KxN+lQNc8TA+P3zcJf7x/Vy6VFq0qehnBEqekEDEsUW9EQOs4hlQbtvQCqeAc1tVBlw/QiTvw7xdy0IaXvb1A'
        'RKEIGDGVpNyP83Rx57mzBwfnnHJoMWeTQDrj69vmT/HVkKJ8qwSqghumvqTwNnvosTzKnFI1mXFgPEtyqiiTUEiykYRCrD+ywFdi'
        'vUwESriH1I1IHte1T2H17zQDVIriGIhjs4ZPQwrGLHH/EfxTVbHiMYOZE81BSlKxdLWFFDtHHV3zjW0OYxAHc1J6Q209Obh1lrqP'
        'tBCwPmHHJNAm+WaRk8dexwjEbUqVu01BUALqROo6WuNVBqZcfz02RSzLIS2ciwrg5CQBqdyUdsNQW7BR92JwNxxcD7qjgWPWBEXK'
        '0nGUWqBhc1Fw7dI9nxYgDS6T9HdzoUDtL7GPoVtlOMW4rTPXf3SjGeUbpbKOKoObhIGRUuAwe0jLAVv4l1w5quL7ulD4hxjqhN+s'
        'UwvnLCEJ/3Qq4bdIfHEzgo0H8oGmsSgR9vYqmv7Xz8M+meUJMH4unRTRoGKenappaA9TX3ubstK3cr4SKbeXNq4yOuWczHEhu05p'
        'N9lHN/LBJYIozrMSvuDAK4Q5ZTqte5aGViXAGxaGRFbCHDalCftElyxZO7t64dwkmPfY3FGL6yKDC3MnRYTe8onA3pzSaTeJwUyS'
        'Watwp2BzSgbDIRQloy+93mA0ahgJrE5shoiU9LL7aXDT7e8SbBBOjX5gX3YOdncDlCQjXp5lDDYWRugqyDS/Acr9p3M+nkJ9pc+I'
        'NhqOt7mOupPJzuP59Z8ogmB2/g5BJtuEqO0EdrRndwZd4i3hjv0AbM281kcFPCg7HogXstkDcZ6CbAHJ34PIhL6uRVgUruFBYuwI'
        'miRiGVmw0K8k5GW82mPI3gHdBy3hDZUxcob1hhHwNuLdXfLDttYFB9Vt6AexD21f3MJYlFeJjkLbOltpHVXrbG18b6BOsmbQrdKd'
        '2qxh9+lraLrUlYAKA4z+H62E0seZC3FdDVnXwUIqlsHaG37nKuAm5q3IqW7dVaeOtDaR1hakjYm00ZEskaUgzzGjtRuk1fJM5lbU'
        'gwbssq9DPeRcN69BPeJcX4X6jnNdvQb1mHNdPWOOWkB4EAQeOL4Hzn6wxdntPnBq8bftO58oR6otTxZNLmx8f1XVr5b7okJy6p1N'
        'NbUo6bX6vqKqK7Bkj7QHm1Tijpns5MRxgqtodYUVUczCij+JGbTHeHhYHUDEIhaKOVcMqhtH+YaAV0cb5A1x1QOMeK3Rrw36tUa/'
        'ttBvNPqNQb/R6DcqfaHneR6EPnkMoMNcupDEVqL8OOrffuqOh1e3CB0nbgS5Ct6GLLstXibNcmzKX/8OTBTmnAdnyXFCfvDq7Fb8'
        'WvEKH2t8bIq6pySCGTjFhJPI+SpbAACU6Q8u78af74bd/lb624L+VqVfvUhvFXpa9Nvi2WrvH5mHHT303IieEEEAk+Dc4t8E/nGw'
        'MdcnCPwgDtd8IjQj1HeVnQr9ayJuIavecObvpxKCa52ZclCEeIKRI6msQQaYeSyizBJdeOQF3WWcsN+g0AQTb3c+aKl+M1W4AWFi'
        'JHykF+zx+iNXBRGbVY5E7DEbun7gRs5P/FSqWQ7KQzF5NEzeVgBxLlxhAuExnq9Ao2E/OLspdaisgjDLIR9orAQa8VxMvlgLKPmr'
        'nptcgxPsE8DriSUQt1ut9hWHTbNd+dCR4QnfboFh0YfDK1VUNCPtimNK6B6jNHOjzDivFB2c+OwVKPaEHGOqNpzkz55cQI9U5OtE'
        'ZgAlX67dpyHvWZ7JJFp+DbLZwqRY2SkKCS5Y8uQmPvH53QX37TzNcFFmC76+eAuFx7xiffG+6qSg5dsmUt/CjK00iOaOELlRw5gC'
        'xoylNYSu7wuxYXQZswgnw714UiBw8cVe9ohdfMjYAzZN4J1Oirsmvrm+n2Iti8gl6xFDpSaw8CwH5/NoxeZpQSPBOUhLaGUWYU0g'
        'rukFhuRaFJZudHSCCRBw/G0Iph22MLTs6/7/9cY+dNfgcwn4ASzjPIh8eCTghykLIfgyqBiV6aB3BEQoO8u7FAFPMxrze5f2sXrH'
        'gsU7HsOcY4+DS4qnWw0gYQkYfiVNrmFNTKy1DWtqYhV1Ae+KIJdcF5h6j19VnIVfpJmQe94RH78IHeXX3ilXzaw7BXFG0wyVAh/Y'
        'Ez77hlN1LHioFqz1nnDV7XioGKzpnvBYE29H64psRuUyqZ1egTixIE5siFML4tR2KKicYLTOgnSE/iIaDC5YU84r/08b23pF8ME+'
        'pPBkCdUQHr6Cg82xMX5yUwI5vNr+xJi7gszxRMksYdh/1JpFTgtNXEdynru4BYBvZyx+vmlU3ealMxmcycfYl6vQUvy8Y8deF9gT'
        'FXuyBXtTYE9V7Gkdu04OqsBkv2DLqxijPMwM8OaULYnrwR5DHDALR2vUGFEIm4LbmcKt3dEZeTRkT8TxWJbB10u8NppkRwavvSlk'
        'cR/EwvusV/BS5To0eLUKXvAlHeM5ZitNsGNTsFvJjB/3vIKZKtk7U7KCGfbTFl7fnmvn1boGL/w+lj8QsAceNCFNMYfuPOLW7tmz'
        'Ty13e4ZzaynbM3xZy9Se4bpGfq5uE23HCPKCwij70Mg/qD9KaezY9FLvqRLi5fN5cV+u0rbO+Dgel2Oh2OTX8teD7vBu3B1eDsbk'
        'jwoyPf9ycTEYNtX+QbnCPzj80CSH7XdNfuPf5D86qM4ULTOf0/sgGs1oRB17JzDjVT4Hqr2SWe33GcHyGAo0DuGlsokziFwvhMyZ'
        'h2GBpwsDU4rrIXF9AKoNR3c9MMmnz/2BMAt83PW+2hn7NIYmCTeM72A/HfzaPb8G7uPhl0F5PfEy2dfh1XhgoTXMhz+YcGAXRQF9'
        'vvWLWk5eLBk/bRCKFL+EMboZiO1M3CH6sqcxOpmUyjtGX8DMFT3H6kpeVxaslFsV4yLTqDlqNpF46LVbLkG/WTrzre2W3jqiucUP'
        'JiBJqQYzrrhbZ+UFoqgyDWevmP13zvzk7K712E+6JbpAtfAHz607piKz1/1Ty0rce/j6n1ncg9cu7oGZdkRklWqjnbFoW8AuGeJO'
        'ufOSevwawSphnY9xBq7fQtR+SmGqARul80Yhf8aQL2QDXZ9B5GsJXB/Fc3LorR2xs5hPfs7xb8F3NJ8=',
    'RubyDung.h':
        'eNqtU01P3DAUvPtXWOyFIsFWQuphQRy6UQsSdFG0iI8LcpKXxKrjF/kj7bZqf3ufY7IkKL3hQ/Q8M7bfjJ3l8vhdB1sueeqzXeJ1'
        'dVLzY34jpOaVaIDnSljLSzT8IcOf/PTTR96icbTinVtYyFIXUPL07vNjcvft6/O+uGQLIqSGWY4W6lz5AvjBGpsG9Ul9MMK2sgEz'
        'ha6hAzUDpUANmLfqWyV2b7EnbDIJU+xSuhSsV24Cn3eQOzQXjMUgh5TZb9Ya2QkHK8ZpSO148/xDFq4+GwE1yKp2Eek/yWmy3lxv'
        'UuJKrNao0ES293lEsAtFxHpXAVOhGGGD0z03AFETLQey7auIWlesVtHOeQzg6IIkv/rSjnrcJxF2qIdJFGSIilDjtZa6Gi2iF7g1'
        'sqrA8AIy9DoHOpHymSwLabzI7oW9NWAtFJOdFZT/UwznfDH9w6YzHPUwSrsMxIuT5H6TJmE/YV3I9oyx1mdK5vG+hns8/BDlf8fA'
        'az9SSzdIOpQFebPO4G6CURjDfPooetbJ/PtU3t/VYalQOC7GjAXn2zV5MGKObrCDyG4xXvGcqg3nzeC10IWCK9363tEfsrmgTmQZ'
        'Ip37Mf8BA5tZww==',
    'Shaders.cpp':
        'eNrtWnlv28YS/1+fYusHGKQhK74atDaSQhWpWA+yJUi044e2EChxJTGgSJaHItfQd3+zB7nLQ5Qcy0kbVEgIejkzOzvzm53Z482b'
        '473+am/eoOHctHAQNia+j475X2hhuuYML7AbIXvhO/TNjGzPRVMvQA9jb4XO354A+571+Y/tTpzYwuhgGFnN6aoxP5DbuKqZxpa3'
        'WHguaat1NDvAk+hcu8dBhFeM/OejZIiXl+FoKX1B79DtXbd7Jfj69go7ZWy++FDk2q23W8/Aqy/rcgMr61fDE8cMqHPKOiefX8Ca'
        '7bqmnWsPN01j0HmQyT97gWPdmFFgr642kCxt/Lmawg+8TwlFbex5jvxx6s101xw72AJdpqYT4qva1PHMKEekYTe0o0cgOmmcTVlX'
        '93rL6A0ucpQtz/EC5aTx47QOtD/B87Rxwp8qaACRkcAcxSEO0TIcnY9OkOlayGevoPHUdnBIo4iEiT21QcGQ9UMjZWGu7EW8QBNv'
        '4YOdx7YD2lHhzAOcGIVeHEwwUqApigNsqbWQxNsEGN0wQpO5GRyh2WgZDimhwclgnDUEvwNqi4vVBdB8JM64B3P3waDoEgV4ZocR'
        'DpTJiXr1u3sgMwB525v1AQOLMEP6k0SavoRREE8idD8cdW77dwZ6Sr+QB5V4jnyPSOr3hh2j07u9KpJcwJjA9EDU6nV7g5MSkjNE'
        'THOJDP2h1esNNIlmXalX787YoNjFayuW0oDfZxLJabXuQumFabtKalvb9eNIzY1EEHtxBN+zvbO2BhnnO7SIHYWNSqGiSDMFt1rP'
        'Y0QtFcNs8Y4pwv4qpYvo9MCo4L3MIBaACmiEfo2/SkURw71DoQnQNiOs4JWvHEsAbazQEZWl5jQOMHC4BZus4Y0FMp1jnxlqfiHU'
        'ONhMkg+DMw1o+Dc5csJNQdbi2PqicDytCMd+RTh+E0jznime+zk8c0XKdQVFWhx18HqmKamF6wJgamnkTm3XdCRm9nq0Gb32FCky'
        'th7Re0gDP+YjjnaRym4EszHIB/f7inAqaa3nqBKFwXA5hddl0BXMZfAtTxWxu2OyuEsJvzxdfLUc8E3m9+9rYv6y6XArnPwSOOUm'
        'r2t7Nnfgf5TB0dneJ6/9zDrcTpId0ZE8iILpskWp7dqRotaeasl08gPYyDq3NLy0J1itJdNHEuKsaCUt9HE90Id3XQPNA9bYIaXq'
        'r/F0isHazDctD9Y3SeldoMFB4AU34SxDQR/g4rsQi/KVezopVy+LpWzCZswxMJqQ0qI5RgAD1zIDK+XMLv4IjwyRZdhndKDRAevi'
        '4KpA5ctUvkxFHz2KWg2P49kQ1gLurKmQFZ4PhbM7QynylvKU2Gg0wEFq1gKMCW9goVRzEkbEpJyWuVYpltl1pNx1bg0VQOtgt4RA'
        'rafuLvkR99T584AA9KAujAWrj0rmQ4GFOjpMnM7E8SET8LWbna6uKfNAZch7SmWWWbRt2mQpFXl0YZIx0/1Q2DIRnvQqQP2UUbmk'
        'C4V6W004j99/wBFDbt+zXTIvqFIn5CdIB9jBZogViWBdEU5r4XfqUTkMj9+3AgzFpLxEVxTtI1QxR6qwbJl6lV4peimz1ueayx3k'
        'BvUin9EhbXZZpYl2DDBfyhG7xZfMUR1e/rbw8l8cXv7fNbz632V4SZtYrxBdh5nNsK8bW/39xZaosJ6RvjYx7ZbARK22MYUJkn9+'
        'EpOM9W8a20Mao5u/XyXeKjy3n4jbOZ9t4Nkto1XEm7+HePP/vvH2b157aV77JsH28vRGRJJY49nJEkdKYXmcMR2K5HRxmUl24Igw'
        'YbuJYQW5MKPJHBk4DLFjRl6QbIzRvQSyEYtX5iRyHi8TLmk/SiEbLNSFfFtBOT2DFmkPVDl9y8wJEX6vDwz9Qe/qN/qt8TNVMylG'
        'f/sDwCIs/gShSP4RLk1vdY3/9fVRu9trGud1lLbe6MZ1TxtpersJi/560nw3bH7QR8meFchB63pW8ulZPSMZ3uleRn0nyZwUfiWS'
        '39aLOp/tqnOyF5zTmdOM9FtNYcZcX+2c16TjSEW2eF3kJULyClVeEY3PCIutoM7VcCmsy3EmEsR3gLS94kHOroeFQ+pXmR9fCoyO'
        'a0e26dh/YTJ5BcT1pCPbwm5EToK5jR7YwXeHNyuHmTN13l85nThYryQTp+vqlt02vsGJ7FR1C4XxBDQPp7HjPP6Qm9O5CWAOBgus'
        'a7WlZ1til9TCMDt7j+lG6bDZ1kcDAHxzqCulmyY5iuLKr0KEDITNciqoCrPMxu+JkMKIQxx9FL5T2N6nuPJwSICQWCPjZnI4YEbl'
        'Eu9TL28TKPBQJY8cL+AJwfNuUgV8NkqNfQuCZ8grF5dsHkfh7hvkuSrBdCaxQ4IRas2x7QIGqVGPiSGOhfYsqlYC+Oxayeelnw+G'
        'm9iJbN+BYICPdP6QTM/mk82RlGdORBSiSrAYgemGUHhgiUe9ooO77g67UKr4MISQFCPxwj1emJ+8IGOCIY6yS362jU6sSkmy8yZQ'
        'y4vDxAFtcnDV6t0OjRGUPl3tvqN/7A96/4XlCa18jlSm3UWuSiOdk0Nfn5yOYqhs04yVXqeZJmeninzzpo4yl3V+ofdp0CU6oZdr'
        'xJP399xBtHsf+s1B82YoDSBVBHJYyTDkVdw2E0r1vui8L3XO01zat7hKJDp/vtRtQyqL4HZqZIWeS2H2hwjZzJUp/nWjKO49hV/Y'
        'YH9lZImbVfzrRlnsZhWTFPAKG82Sl3FGanJkLwFLAR6gHqdXsYozTZjuDPO1mTzLFCcZkf4r4VaejrZ5tDRFVfQlFzUl+SZFr4Rg'
        'C09NmIDQPD1eVWgmRp5v/hlLM6kcnimxIu615e647TK8EsBedz5cd+G/IQE27U2EwbrUcaKGe0XXyQn+ef7bylntxDz79+PJ/wMj'
        'BB1i',
    'Shaders.h':
        'eNqtVVtv2jAUfs+vOGqliU5Q2EBbgSdGuFVAUMJtTygkDngKNrIdSjXtv8+OA4QRWjE1UqLE5zvnO/cUi4UPvYxiEZy16yPGH9dQ'
        'SN5h4xJ3hTaICAgog/mS7qH8rSTRH0x/jwPiowDsyY+f5mTYWTjdhtmynUXXuJfnmKAskVQjXhj5CO4c4TeC/eP6zlChdEK6dEPg'
        'OgqPEi5cIjgwtMJcyCAVyCsVvHINZpSFfmGK0UthxOgv5AlMiYxcMLyPYZWC9z2BJceQU9kIIhExBBFHDzHuqQZtuoKty9wNUiSQ'
        '8xHhWLzmQfIzkQdEfPVwlyHytVJVK3k0pCx2fYqYQPt/XQceUsGPuZg6i6Y1dMaLmWX3zWmvNRvZ1jNA6QoCjlflEtG2OqOG3Rg4'
        'GvGUiWhafctObFRjR0d4j8J3/BxdsVDKRKS9+HKJ6PY63b68xxrx1TC80OX80LfGb2MbLUPs1Qwllu4I7EHPxEyWtGzqvGps9TPw'
        'xS51UM9UiSNMaWxP3/XbOYZ0jPY3E72hpdlMJLPAXNW0KUp1+n9aKcL4ISvdjBhTKyBufg/xtF2zbM4HjbHdm0srL2pGBvGI1K+C'
        'dnLS3sNs5SCmMQdP1KhwJAQmqzMvlpTKVlwEdNXSw3VmOgipK7TY1AN5wTxtNceWXdGgphrGFHGaBBMscg9n+juKffARF4y+HkQX'
        'cun17JSdXDwvqZg/qeRmmJVq02O+btE6rbIbdaOt7wqkm695WJxvhdU+Jj0XZ+iw3upXwEkJcroqyYa8io5rkWBZPinl6vCyvOaX'
        'XMqyjdV+9nUoWTWToAkRF7A/dfljkZsaB6rlMn47fwEhdEwR',
    'Tesselator.cpp':
        'eNqtVv9P6zYQ/52/4vQmbSmUfqGAphaQeJQiJgaIFpiEKuQmTmq9kPTZTlsY7G/fOYnjOG03tL380Picu8997ny+a7O5+0OfrWYT'
        'RlQIGhIZ84Y7m8EuPFAu6RImie9Tji8WevhmL7OQvtBIEsniCA1/MJOfWOSGiUfhy1B6p/6yMf1S2iuRtPaHU4LkhNrcMjrdrlk7'
        'tS3Apwsvz3MMjLlUONf3V1fZdj3fpsuzOImk0zLbidNqtPySWkXmTtuSg4o8qchTIs7iEAn5JBTU2h/RpUw41V/+TL8ZvnAMEV2U'
        'Dio7oaffT/94fji/G12enQ/Hva0POwV/WTnIMD0aUkmfxiXw1G4eMw/KxixisrByQ0oQo5fT0oEgrZRwaT8PxHxZh53DWWHm+UfD'
        '1noj1HD8MCYSkjpki7mBsFxLnhScEpQTLcxRmG/glJ1Misu1g0AvJpYnHXzZj5K5FgIUAi1MUJisd5oFnntdamevevGmvTIfHDtN'
        'J8dQPvysmDJl9fhhIqb6wD7S3/SnWkE/g0qJqYUny8s4s583lqi01MIrCq9aeEPhrWccZEx1jqq0sNncEvcbpMkGIuD07uJr8bX/'
        'eHPXB5VIJ13WHJXWbdg7OFD3qFdRDCzFYLPixFKcrFOcN9z8TFvLwaCVPvAODoejI2gf1tQ6UOtf1XJSzivFMq+EaaNlz8pRFJnK'
        '67a2ApKkR5OUWean9a/+lalqVlVbs1diYp35zs76Us0ramNBHiM4vL9D8DzzOl6fzrGc1KbptIaf1U7UwylmIFohhuVyLyiItMX/'
        'IiDzhz3MDQlPpxA41zcjGDwMcGrJKZGo5LMl9cBPIlcpZK7LpHZPhlRm1d83QE4+R7pdkYelPtZ6FpnRlELMWcAiEsJvZE6aNzMa'
        'XVxhPeMo8jhZCPieEE/Agskp7mJiKN91w9j9tmAYyYJFHouChgbsd/qQCOzvKyowecU4fZKEEnwsJZ/HmGSf4B0trB8pTgWMVcaY'
        'QCSN1nJqEGKupvZiSiNkEqU3HHczfmgiOSNREGo8DXqj41OKoPtCF1p1aNdhrw4dcFYDYxFkqahpnAGSVuGlmTh7BDcJQ/Rfh0XG'
        'OkVEuDaQyFPrDoql24HRKgZ6KNjF1oT9Ss8xuifQWtN2zjglkhZB61LK/+I4h0WkMEM5jZ1FQlJ8xz7s1wosRUyjPFhzy1DYhkNT'
        '29WWu22Zbxzta3yMDahp9JhmR3Fi6eTE15EhguLOjqFuEqIDEdz9SoQamQxJ7/dWFDwhSwqHtoIlqCNnXJjkmAN28ur0dGnWLEsr'
        'HU/a4w60xvZo0mTVl96nENobEfY+ibC3EaE9/udkDCleOs/Ohirx/5qNzv/Oxv5GhM4nEQ4+lc+P1Sq1+28fG+UtZy9Msjm9v3Us'
        '39gzbkfPo7vL0+uLq/Ory+Gobn0v3zKsLpXqPdPLitu7OR4bTrA3GvtO9fqZM6mtuXTFn2gLeGV8FWPuY+tvmwDW9w==',
    'Tesselator.h':
        'eNqtU8GO2jAQvecrRsslVOzSXfVQgXpgAdFKbbcKLG1PkZM4xJJjr2wngl3Rb+/YTiCk3dv6kEzG7834vXHG4+s3XcF4DBuqNeXE'
        'SHVTwDVsqTJ0D0mV51Thi/EM37nEmJi0oBkoKjDFxA7Zb3ycAcuxdg7R4/3vxeP3VbxZrtfLr7PNQxR/Dga4xQR9ZRfJIuVVRuFq'
        'LstSipviKrACG0UooSTGKTEFBXOSDWFZaQOllQe6IFZv7TkZTTlRxDAphoE2qkpNxy9fOHgJAFfOJVbfj+AwgucpuIXNf0jNLB3C'
        '2ztIDth16OCLnw/RAlLJpWrADr5geV5p6jdgFq3uIfzQ5fk21QjqEw3cEPemUpYnVcYEQTyEH1vi8Yy9WJYoDeETuGuaBAEK1roj'
        'EuU9KVZjxYk7gDZoR4qNBHrGhIFvs1/xdhltvsyXa/gEt+/tmjqse/T9egdlbO1lKdUeZqv4HN3PZSVMh+31ljEqRsj0Iqdsbmcf'
        'id9IpOT4VRA9d8b2ko1J0yB4qhLO0knvfOHQM/5cplyulizDgzLTglwi55Uu2sw5jTrC06B8UHdpbroNQrWIXRskXah3JTzdLh8c'
        '2uC52xvHuaLGUKXdLU9J73dtvd5Rsz2bHQ6bYb4gFP0RvVnA0RE95t9hNsXsNF+r5CaNZS4vkneCU+JMPqLPAzwpy62M///hfwEe'
        'H3WS',
    'Textures.cpp':
        'eNrNV+FO4zgQ/t+nMJzYSyEthd6tOLqLFJqERVugaqkOiUPITdzW2jSJHBeKWN79xnaaOE2hLHDSRYK6zsx45vPMN9Pd3dqHPpXd'
        'XXRJ5nzGSFL34hjVFl9REGGfhmOEQx9NcYjHZEpCjug0DuQKcxqFoP/BDv1GQy+Y+QRt9rlvjeb1yaa2l/kKu5WE+4eHUxx/kYuE'
        'M3DXRKc2ZcTjTTuV/Wv7KAsRpG65Wp/huFWplIU1WYFA+s1l0dSlATG8KEw48iaYbaMRbHQxn1QrjxUEDx0hY2N8G/tN3yZ31CPo'
        '50+0kUuh9GEETIbofNDptOSm/Ac30Z4Q74cwgwNGsP8g74D48vWrgz08pJwwzCOGKEdfkR5xfURD38g8amVug+TGkigByapy+nHZ'
        'dcprRwkBLHxl4ikPYwWiqVHwpRzyICHIbtpXbQiYkyW0nTnikQQBdc9Pdm27v3t81kWEe/WFvhsxwBk8uSMsgYQUCtZB7+Dk4PgA'
        'jSI2xVx8oBgHhHPiQ/5CJidS/VvP6Q86l2jCwLUXnLCMLH79ds1sdwFoviOs3dqOa4F9E+UPePw39fkE6mwGkQvF3xN0L7Zeo/uN'
        '0PGEF5Unci/T3tNVitpnNO6QOxIkYCCM0JTGkE9Jptl4XnOQAGi6g+7Z5e0CZlMTdBXiNYE53EtzvzaE1LJ6J8e6dvfiolOKELS7'
        'URSkwflkhGeBururYTRHzc+NIkLuaefS6d2eX5w7puYADSD7VYAjuRYc5s/kh8ik1xgBoHQ7AFRuK4dr7roN+ZjLcLWjIGLfyQOo'
        'i2wDrkQ4QZzhMIkxEyxqNGRkYRSSnBdEdZirLyDuM+80HEWvku2qXM9kP6UFKDeqWv2Jynet045jGxNWFWyVlaqq1eXyB+udaIwI'
        'Y+C8CMAnw9l4rMMiqBENZ6Pr/T8/37Sy7SQG8PjIgDcm2nQx5K6fFXd66iHaSpCRVuUhALzVOLiq/hNumnmFQbFWc6sXMx7PuC2c'
        '6EtGtMQBmkCJap+KhIu9CUFc/GkQ6TR4vTj5Bjgi3S4y2AnhGWwUrmgFLpBq/UHPtdoOZH2/DW8TTxlJFWtHYEXWpg2vDMioT0Im'
        'DWQFpEU4O7JNFGHc8udbPgA35V+3/GUQhfG6JKJ0rYgl/aKKOD38RYgrGsYZOE+/1lZtzHGhrYZ4qnHr6x9lYxYmdBwCGsqYD9bf'
        'YgzQVbRsyqUi2Wcb/f+ntwvwPrKvv6WnqzaaiShCX6L9MrEXurEObu2o0JfzZpzezyTNXeh8DfP5JrXcd0r0aCrKew8//iL5eQWg'
        'XqA/WRIfS32dCNJRzPbACkGB/gCozkX7u2Pf9pz2JeSp94P4Pbh/ZUbeT8Zbwox4JzkrFzXTBtXQcrE/aLcdx1ZgLiO3XLYk4YIY'
        '4Cij+KqaH1KPjylP8ojzeQxyyxBl+wD6jRZ8fEmzBNY7O3m1PhaYYYUPvegeTGTe7ICpbQ2Repdyb9KqrKWhhHmpqdyMTF/4/KOo'
        'X/iSRTJXkcwhEqkHSz2QcjDpNZ+GkCWIJghGMEsWAbrHIU/Q8UnPQkY2KoME9vgMB8GDekVDNCXTiD1US2YLsSGRDiq867mIBqJr'
        '3LTWKI3LSntrlYZlpf21Sris1FyhVNpIb1+LCQwNW2vl9oTceL3cvpBj6+WaQg4X5Z4q5VW2yEpzEAZZcVZ1CoBfAOR9xCUM/0e0'
        '1bdcB4in41h9x0iPeCWrFeY2cfbqmW3tOKXaTTZPoRFMKbJwtclKD7DQh94zNt1F1NcGJUYCghNiBYGxmDzeNimoUxWXlMaGIYE5'
        '1ai20HOzQgvt7FC+TNiFe8rnhkKiFWx5EAwDaysCHcLksujuz88a+vRVHr4eV/5Grx31CV+YhhZVTKgn8OVfSJ1JCQ==',
    'Textures.h':
        'eNqtk1FLw0AMx9/vUwT3MmVuwkBwymCzcwoqMjeYTyPrpevB9a7cXacgfndjW3XTvSjLSy+95Jfk37TTOd6riU4HpvQSCke+ncLx'
        'pwPaolRmBWgkZGhwRRmZAIl1MF/aF+iennDunptpqMRISmAyGz5Fs/vxYjqaT2eT0ePiWjT4QhnaeceJJtaFJDi4tFlmTTs9+H53'
        'kWHe33B9cDxZX4hYo/df44tXkTu1xkA9AWw+YFAxP2Svx4SL8lDltuAmUo7i0I3q7LOjPvhFqJw7zM+FyIulVnHFYpVvWVBAqEMg'
        'cTZjN1GaIMeQQvPhfgy+yHPrAklYK4SoG80PN3v5XbX8TrV3xcgr5jVja3yAOEV3VBZ4YP7heQna6marF4fPMBkPByAx4N+KRpyx'
        'VdRgRq2q1h+tohTGq5VhFSrcR0f/wyle2WclQ9oqjympVRp+SDEhTegJUOtPRfzm/GurJLgqaKB180f6UJkNJfn3cMQ7/LElvyBL'
        'Dq1la+5QtYYw/43Xp8EYlZQN7tj4dxnEIqs=',
    'Zombie.cpp':
        'eNqtWOtP20gQ/85fsdeTkANJsJOAQjg4UV6KBD2aUB3wBRnvGqxz7NQPaufU+9tvZnZtr21erRpRe3d2Zn6zszsPd2ur90t/a1tb'
        '7DZc3Hui7yyXrKcmTARikcMz8ZKceYulLxYwsRMvDEDkF9vwuxc4fsoF+zBP+KGb9R8/aDRlXo12JbIkjURcp84fbS4iIuK+TrJE'
        'RIHts0Rys0i4IhKBI9YELbHpsRcJJxkeK327G+zhznm0IzXfW1uT6JOJfBvn4kn4G8zHV5e5fmgnbJmVo7wcrTprDH4TdkI+NEhC'
        '0rosChPDLCaJtxB/uW5cUeKlENyw+qarCRzi+r80z9g+gO7ROMdxLscrHK/kmB6FaqDP7ICHi1M0zuiwDWYNhrtjawgYkh8QnuEy'
        'aDedi7vLKcwGOvchsBt1/k1GNgOn2TctVzPkUdgc+APxjR2l98Iwu8zs7JVLvQOb849hZvRGoKDLemP5krOx/Oto+u5Dnuv6rJ0u'
        's3YUBy42NZpS4aBQaA26bKRrtKOFqWscmZpGXKw0DnVd6jWqq5QCsUguw9jobRPLoDSkgWu9hmtVuNa7cK0S9zVYXzyYjQMpUXGt'
        'Qh20/dcAlfzFZiWL9SKs9Qqs9YOw1V6fQ/1eRe9/KnyLCOLCF4mgu7enE/Dq1Ah4kE2CVSPg7psEi8CfQo+zwoLEc/4p4bMQnFAE'
        'MI6LAMaxHsAymWQ2kM0y+CQxrxPLIN7cp+isxSm9MCp3d932wmYzjnv1PIAB3UwMzXkz4Mnk2AtcAyDUgZHFThjrNHp4LjPC4CwK'
        '04Cz9fWG8j+kcpkKpf9IG6f9WwMF+73ptUX4JOaYSYGv1P4nKhu4kJfhbW4rWWSdCR/K25MwMrsLpnYred1UQoVHryVvZBzk4N+q'
        'JpBxtoF27hbuyQvCWBFWTY7SJzk7YJZplnWg2jyUPnnvO63N685sihXGFNA6/Lj0Y/PqQsmEsipLAbOLO4w4vz3cLfmQH4snzxEd'
        'zToonoG2GSjGHz3wPpZW24HSW5TkUlOt7DbN1lF6B3ORKD5MH3XBljcA+UssCjjOYmoRaEl1C5NJGgslzyXN6NRtPxMJc9II/JBQ'
        'SWVuGDE78BayH0Ku88PZ2cnd9NPVydnJjJigE4jEV6nocyqi/FJEILewof84gsMBLxjryNh5nucUpFNoVnJjHRUpLh6m974ggLlw'
        'woBjbTcktYPU/ufU5pd2lLCtko7yJb2mBzdxhTvar6mE9gBuHdZ/pYE6EkwUGhI2Fq1UFXsrIfPS9niIv1E9aWHVVk2F0XPt+9iA'
        'JGGUZmAi2dnZGVDOgfoFcTaActs4jyk6bxlCvAq2DGOvPAUJ4tHyNabYEG3OQEsWokZ7r8V2g/FMbDmw5S+x3WJiJrYVsK10tvKG'
        'p57P2bcwgifcjMjLaOV4eHx9cXg1m14j9Sqyg7iLo7lj+6IanfrekmazMLnplqxy+Dcq1dBIJ0EQk0/30Fiv9CsXFIObYnCrPFkp'
        'QHAveCBhZRKeof58W0Yar/oT+bKqyv+GtTdFmc/zWsNQiYFLSOaGhKSDqNTp/Wnjjhxh8grEhBEGnFZpaDGGt8THcgY6G6AXqZ94'
        'Sz8nTDqALqvgK+tb1r4kqA0J/6cFcRM/I0z26m4qMyAUE+KTqoxCTPFWiXLJIeTk9CgM4sQOkriZLA8pL8pWii0h4cRam5/P6DND'
        'HVsr9MdD+jQpmxoplL0oJDuPonLJpjt7HQKzC1wcujG1TxopvapJt5PTYGgN1IdO3VTZeb8DvIVqvQN1jKi9Nqpsvd+JavVHrt46'
        '/5inKunipGfUFzx/0KpnqH2R1WnS3y2a1aTJLbZoLb7KKrjNb6XhKZf/tdGMooJOUVRMmmHQjpaXON8OmNdam09fzs/pM+Z/a7jx'
        'mw==',
    'Zombie.h':
        'eNqtkrFOwzAQhnc/xalZUkSbsjZTWypAAiohdaBL5cRnYmHHVeJEBATPzqVJJLcw1oN9/3f3n23ZUTS56GBRBDtrEoXTDCZ9CJij'
        'aWh2yjVUceEtAyVzgRJetsvX2+3z3X63eVo+rPf3LCCscvwnQ6Y81ZVAGK2sMTafZiOPrY9HPWWrKsGWsFTzshyuNodDlWiVQmdh'
        'X6zTcwY0Ws8VZMhF7OnEisbXvDCzM33ja41vszPd54+T1JY7KKyLPemUwY2Upc/KA6KITz2LmB1Bd53wEWvU7Q60XPdVH0PQDMHn'
        'uOtSq8JVXMNP7x73zQZeWyXoJOl7OBhaUCA9VxF2rThlvskWEFQS6Pv8faxf0bq1GQ==',
    'stdafx.cpp':
        'eNplkL1uwzAMhHc9xSGZa+/ZAhhFtwBthqy0RNcyFErQD1q/fSTHyJKVvLuPx75Hyoam/06HgBOSL1EzJusYeaYMK9oVwwlLSbmOuOpJDEXzWqm+x3cZ16HIbxf0jD/rHEbe1CHyh/b3UAMNZibDsel3qB+Xp1p7yWTl3ZLXwJU0+XinbL0oddy5OOwh80G1yOtluJwQeeLIUjuQrCBjbHOR29kJqy8Q5nY+fq7D+fPWfTV7LQXxrXA9wqbtBeoBubdjEg==',
    'stdafx.h':
        'eNpVjrFuwzAMRHd/xQFdg3gPik7ds+QHGImyFMiSS1GA/felAgRtFgLkvTvePKOpp7CfIy5IxeXuGSFlG1VMo+JJPNrRlNc3oJ2meYZBm9QHO0Xb2KWQ3DsFjaQgYfTGHkH4p3PRfJxw7zoihuYilcXkVP4A06bpYxNaVkItjm17RX/ums/x6//hXveVNI7rSL1dv68XCAcWNi/I+6SpFsqITJ6l4aj92X4RWjHeJrG+0QzTL3KMYWA=',
    'AABB.h': 'eNrFl99r2zAQx9/9V4gWSkKXxH5t0kDSwfa0h0HH2pfi2nIqcOxgO5l1pf/77iT/jJwQdyEzBCunu6/vdB8ZeTIZnfWyJhO2WCyX4zc2YotcpKNFKFYR99ky3ka+iFY4yFkQJ8yLw1CkIo6YzzPuZTjC6DOncy2CyOcB+/m4fPr6+OPbCyX38t26RqOIuGHHgMgLtz5nVw/xeh1H47cry0qzZOtlqjDr3WJ4BWHsZiy3vzCJP7CnTauDVvyBM7WsTSJ2bsbvGvN8k4owjmhy+xoKT8+R+GDI1JiuO1Qf2EN6gLqBvuWONuob6FuhOLDHdlBIvH9Ytax+7gulWwxlPYR6SJmXDvUQnGE7KxTSeaGMzgxFdG4oobNDAZ0fhe9laCTIeL5xI7/IM3fLR8tqBO4QgYnSTMdW+VSVsXvMbLpvl2SXph3IDqY9d0jHMXXILk07kF31uZwQARvkLpsx3QvK7BYl3em+x7zycLo8ZENDKg1pelQa0unygIYGKA0wPSoNcEqPyiXh2TaJNEOKHsWNIkaxoiihBmvVZkdXSfznn/uJ75DmurSbipPSPdRZnAT3UHvZbaes7jFOdsnqRuNkU/Zz66MlvVBsfj/QO9DnA7UUSueGeeUa5QfWiBrnjTHTOa3CDUaM8T7D3Ot9WnvXEUARUEQARTR3thlV57p2819uODVmWyhrXVzdGbVuaDib4nRp6bLbKn5UvyG7IuihRdSM1qjTqfthdGG+9wcL0l0yLYcqnzUrt9mc6OpfuaMrxx16cuXzS1Xe/e9jfwuU+8lA/Oko4vIo4jkBm9vV8s5aq3s5xGUbcakQl/0Rlxpx2Q9x2bvR8myIyzbiUiEu+yMuNeKyH+IXqvxExOUhxJ+PIg5nRvz4e/+ziEMbcVCIQ3/EQSMO/RCH3o2GsyEObcRBIQ79EQeNOPRD/EKVn4g4tBF/jeOQiSjjSYrfZ2mb7/9PdZF091t/emrNgRumvFX2LhY+W8c7fvz8ule5edA3j+3mIdw8/JtH+fpgrpP8wBP6NceP6YDh1/b+F+xfbnJvHg==',
    'HitResult.h': 'eNqtUMFqwzAMvfsrBLk00Cw7N7ex0QxKD1lz2MmERG4NwYbYATul/15bzRIYPVaHp4elpycrz7OXBstzKKWt0Iy9fbtABg8KWkADQ+MzK3sEqSwOBlsrtQKLxgbdixdJpFAdCqjqj9/P+rjn5fep+vqpDydesiRUpMLnRWbsMLZ2/Qe7MggRlgZXLNSvdFqpLgDCDUY1GuzCC+hBnqVq+qVDPDpE08Y7dOgYlRa3TQo7cJv3dAuecCLUhCIgXG//JXEud1uaz/2cpznrOYuUVDGiAXfkwD1Z8Ik8uCaTv97gdCsYS1B1UsStnx7sDhM3lPE=',
    'Vec3.h': 'eNqtklFrgzAQx9/zKY4KQ7dWC75N9rJ2bE97GGthT6FopIJNRONIUvrdd0lqLW7sqYHA/c/f/S9nkiSLmy6SJLBleRrvYQHp2sZStJDXu67DbzduFlQlL1gJH5vnr/Xm/ZVuX1YpfSMBJivOfuWxgOd1XzCYrcThIHi8nxHSybbPpTs2ORLAVdZiJ0FlV0JfC5MRp2xJGMEjqHAZL8toDnoIjA/geHLkiHsHquZnL6ovkfFWVDkjqp2NzaLJxQAqLlnbiHon2acIc8E7f/Y7kINTE4HLuyI/0tVYEp5AwQOEMlZ4SyqCe2iyCaUtpT2lkdJ/UsZSxlMGKTOhWib7lvvBFR5P4zYy8sB5qG9RFdAx+f+fmYyisC9VYyNttR61sdoMfU54XwHjRVUCvs/pq/gBY/i97Q==',
    'Vertex.h': 'eNqtksFqg0AQhu/7FEOEoMVoobfk1ja0px5ClfYkso4o6K6suzZNyLt3XDWkTaEpZA7yz+x8M7PuhOHiqsbCEGJUGrdBAYtRQquV4doohFwq4EWqUq5RQS0zrFpirjyEU+Yiwxw20f37Y/TylMTrzev6LXlmDoVLgb+cECR4ZTKE2YOsaymCYnYSi5Hf9RE23GW8GdszIOsPoZHtynp5JVMN5tTpVmxM7CnXgyUY99bzoaMv7A/fTgdk64/s5yR2k0jMUXWeJXtb9hO4hBGwo9LGTYztMCX9aMOlaLUdfQ7NJaWbfxTtnTlIXaC6pLRNDEj93QIU1mkz/iSBH9FUlXTsgR1gwI49FNLuiWnEG12UrT+iFhpe6sAO9EoOiqzMgdb4fEW+ALHv2t0=',
}

# ============================================================================
# INDEV PLATFORM TEMPLATES (raw strings — no base64 for readability)
# ============================================================================

_INDEV_TEMPLATES_RAW = {
    'stdafx.h': r'''// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// C4355: 'this' used in base-member init list -- intentional/safe here (see note in
// the other stdafx template). Silence project-wide.
#pragma warning(disable: 4355)

#include <xtl.h>
#include <xboxmath.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <xaudio2.h>
#include <xgraphics.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cassert>
#include <malloc.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>

#pragma warning(disable: 4018) // signed/unsigned mismatch
#pragma warning(disable: 4244) // conversion float/double
#pragma warning(disable: 4305) // truncation double to float

// Xbox 360 equivalents of Java standard library
typedef unsigned char byte;
#define null NULL

// Forward declarations for globals
extern IDirect3D9*       g_pD3D;
extern IDirect3DDevice9* g_pd3dDevice;
''',

    'stdafx.cpp': r'''// stdafx.cpp : source file that includes just the standard includes
#include "stdafx.h"
''',

    'Common.h': r'''#ifndef COMMON_H
#define COMMON_H

#include "stdafx.h"

// --- Globals ---
extern IDirect3D9*              g_pD3D;
extern IDirect3DDevice9*        g_pd3dDevice;
extern IDirect3DTexture9*       g_terrainTexture;
extern IDirect3DTexture9*       g_guiTexture;
extern IDirect3DTexture9*       g_charTexture;
extern IDirect3DTexture9*       g_defaultTexture;

// --- Macros ---
#define DEG_TO_RAD (D3DX_PI / 180.0f)
#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

// --- Random helper (replaces java.util.Random) ---
class Random {
public:
    Random() { seed = GetTickCount(); }
    Random(unsigned long long s) { seed = (unsigned long long)s; }
    void setSeed(long long s) { seed = (unsigned long long)s; }
    int nextInt(int bound) {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return (int)((seed >> 33) % bound);
    }
    int nextInt() { return nextInt(0x7FFFFFFF); }
    float nextFloat() { return (float)nextInt(10000) / 10000.0f; }
    double nextDouble() { return (double)nextFloat(); }
    double nextGaussian() { return (nextFloat() - 0.5f) * 2.0f; }
    bool nextBoolean() { return nextInt(2) == 0; }
    long long nextLong() { seed = seed * 6364136223846793005ULL + 1; return (long long)seed; }
private:
    unsigned long long seed;
};

// --- GetNanoTime ---
inline long long GetNanoTime() {
    LARGE_INTEGER counter, freq;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);
    return (long long)((double)counter.QuadPart * 1000000000.0 / (double)freq.QuadPart);
}

// --- DbgPrint format helper ---
// Xbox 360 kernel debug channel. Nexia360 CAPTURES DbgPrint into nexia.log but IGNORES
// OutputDebugStringA -- so DbgPrintF must call DbgPrint to be visible in the emulator log.
extern "C" unsigned long __cdecl DbgPrint(const char* Format, ...);
inline void DbgPrintF(const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    _vsnprintf(buf, sizeof(buf)-1, fmt, args);
    va_end(args);
    buf[sizeof(buf)-1] = 0;
    OutputDebugStringA(buf);
    DbgPrint("%s", buf);   // <-- the channel Nexia360 logs
}

// --- Math helpers (Java equivalents) ---
inline float signumf(float x) { return (x > 0.0f) ? 1.0f : (x < 0.0f) ? -1.0f : 0.0f; }

// --- Java string concatenation helpers ('"" + x' / 'str + x') ---
// Java implicitly converts numbers to strings in concatenation; C++ does not.
inline std::string toStr(const std::string& s) { return s; }
inline std::string toStr(const char* s)        { return std::string(s ? s : ""); }
inline std::string toStr(int v)                { char b[32]; sprintf(b, "%d", v); return b; }
inline std::string toStr(unsigned int v)       { char b[32]; sprintf(b, "%u", v); return b; }
inline std::string toStr(long long v)          { char b[32]; sprintf(b, "%lld", v); return b; }
inline std::string toStr(unsigned char v)      { char b[32]; sprintf(b, "%d", (int)v); return b; }
inline std::string toStr(char v)               { return std::string(1, v); }
inline std::string toStr(bool v)               { return v ? "true" : "false"; }
inline std::string toStr(float v)              { char b[32]; sprintf(b, "%g", (double)v); return b; }
inline std::string toStr(double v)             { char b[32]; sprintf(b, "%g", v); return b; }

// --- Java String API helpers (no direct std::string equivalent) ---
inline bool javaStartsWith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(0, p.size(), p) == 0;
}
inline bool javaEndsWith(const std::string& s, const std::string& p) {
    return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}
inline int javaIndexOf(const std::string& s, const std::string& x) {
    size_t p = s.find(x); return p == std::string::npos ? -1 : (int)p;
}
inline int javaIndexOf(const std::string& s, char c) {
    size_t p = s.find(c); return p == std::string::npos ? -1 : (int)p;
}
inline std::string javaToLowerCase(std::string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)tolower((unsigned char)s[i]); return s;
}
inline std::string javaToUpperCase(std::string s) {
    for (size_t i = 0; i < s.size(); ++i) s[i] = (char)toupper((unsigned char)s[i]); return s;
}
inline std::string javaTrim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (unsigned char)s[a] <= ' ') ++a;
    while (b > a && (unsigned char)s[b-1] <= ' ') --b;
    return s.substr(a, b - a);
}

// --- Java List.remove() helpers ---
// Java has remove(int index) [removes by position, returns the element] and remove(Object)
// [removes by value, returns bool]. Two distinctly-named helpers keep these unambiguous even
// for vector<int> (where a single overloaded name would be ambiguous). Both work in
// expression context (ternary / assignment), so 'list.remove(i)' -> '_vecRemoveAt(list, i)'.
template<class V> typename V::value_type _vecRemoveAt(V& v, int i) {
    typename V::value_type e = v[i];
    v.erase(v.begin() + i);
    return e;
}
template<class V, class T> bool _vecRemoveObj(V& v, const T& o) {
    typename V::iterator it = std::find(v.begin(), v.end(), o);
    if (it != v.end()) { v.erase(it); return true; }
    return false;
}

#endif // COMMON_H
''',

    'Shaders.h': r'''#ifndef SHADERS_H
#define SHADERS_H

#include "stdafx.h"

struct TesselatorVertex {
    float x, y, z;
    DWORD color;
    float u, v;
};

class Shaders {
public:
    static bool init();
    static void shutdown();

    static void useTexturedShader();
    static void useUntexturedShader();
    static void updateShaderConstants();

    static void setWorldMatrix(const D3DXMATRIX& m);
    static void setViewMatrix(const D3DXMATRIX& m);
    static void setProjectionMatrix(const D3DXMATRIX& m);
    static void setFogColor(float r, float g, float b);
    static void setFogDensity(float d);
    static void setFogEnabled(bool enabled);
    static void setGamma(float g);   // PORT(brightness): PS c3 gamma exponent (1.0 = identity)

    static D3DXMATRIX& getWorldMatrix();
    static D3DXMATRIX& getViewMatrix();
    static D3DXMATRIX& getProjectionMatrix();

    static IDirect3DVertexDeclaration9* s_vertexDecl;

private:
    static IDirect3DVertexShader9*  s_texturedVS;
    static IDirect3DPixelShader9*   s_texturedPS;
    static IDirect3DVertexShader9*  s_untexturedVS;
    static IDirect3DPixelShader9*   s_untexturedPS;
    static D3DXMATRIX s_worldMatrix;
    static D3DXMATRIX s_viewMatrix;
    static D3DXMATRIX s_projMatrix;
    static float s_fogColor[3];
    static float s_fogDensity;
    static bool  s_fogEnabled;
    static float s_gamma;   // PORT(brightness): PS c3 gamma exponent (1.0 = identity)
};

#endif // SHADERS_H
''',

    'Shaders.cpp': r'''#include "stdafx.h"
#include "Shaders.h"
#include "Common.h"

// ---------------------------------------------------------------------------
// Shaders are precompiled OFFLINE to Xenos microcode with the Xbox 360 XDK
// shader compiler (fxc /Tvs_3_0 | /Tps_3_0). We deliberately do NOT call
// D3DXCompileShader at runtime:
//   * The 360 D3DX microcode compiler rejects some HLSL the PC compiler accepts
//     (notably the 'discard' statement -- the textured pixel shader therefore
//     uses clip(), which compiles to kill_gt). Compiling the 'discard' shader at
//     runtime faulted the 360 D3DX compiler; the CRT then tried to unwind via
//     XexPcToFileHeader and the title died with a black screen.
//   * Shipping 360 titles embed precompiled microcode; runtime compilation is a
//     devkit-only convenience.
// The HLSL sources live in shaders\*.hlsl; regenerate via shaders\build_shaders.ps1.
// ---------------------------------------------------------------------------
static const DWORD g_textured_vs_bytecode[] = {
    0x102a1101, 0x00000130, 0x0000010c, 0x00000000, 0x00000024, 0x000000bc, 
    0x000000e4, 0x00000000, 0x00000000, 0x00000094, 0x0000001c, 0x00000087, 
    0xfffe0300, 0x00000002, 0x0000001c, 0x00000000, 0x00000080, 0x00000044, 
    0x00020008, 0x00010000, 0x00000050, 0x00000000, 0x00000060, 0x00020000, 
    0x00040000, 0x00000070, 0x00000000, 0x675f466f, 0x67506172, 0x616d7300, 
    0x00010003, 0x00010004, 0x00010000, 0x00000000, 0x675f576f, 0x726c6456, 
    0x69657750, 0x726f6a00, 0x00030003, 0x00040004, 0x00010000, 0x00000000, 
    0x76735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x00000001, 0x00000000, 0x00000000, 0x00000014, 0x00fc0010, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x000000cc, 0x00210003, 
    0x00000000, 0x00000000, 0x00001c63, 0x00000001, 0x00000003, 0x00000003, 
    0x00000290, 0x00100003, 0x0000a004, 0x00305005, 0x00003050, 0x00011151, 
    0x0002f2a0, 0x0000100e, 0x0000100d, 0x0000100f, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3fb8aa3b, 0x00000000, 
    0x00000000, 0x00000000, 0x70153003, 0x00001200, 0xc2000000, 0x00005006, 
    0x00001200, 0xc4000000, 0x0000500b, 0x00002200, 0x00000000, 0x05f83000, 
    0x00000a88, 0x00000000, 0x05f81000, 0x00000688, 0x00000000, 0x05f80000, 
    0x00000fc8, 0x00000000, 0xc8080002, 0x003e3e00, 0x6f030300, 0xc8040002, 
    0x003e3e00, 0x6f020300, 0xc8020002, 0x003e3e00, 0x6f010300, 0xc8010002, 
    0x003e3e00, 0x6f000300, 0xc80f803e, 0x00000000, 0xe2020200, 0xa8400000, 
    0x00000042, 0xe20000ff, 0xc8040000, 0x02c66c00, 0xa1000800, 0x3a108001, 
    0x000000c6, 0xe2000000, 0xc8038000, 0x00b0b000, 0xe2000000, 0xc80f8002, 
    0x00000000, 0xe2010100, 0x00000000, 0x00000000, 0x00000000
};
static const DWORD g_textured_ps_bytecode[] = {
    0x102a1100, 0x00000124, 0x000000c4, 0x00000000, 0x00000024, 0x000000d0, 
    0x000000f8, 0x00000000, 0x00000000, 0x000000a8, 0x0000001c, 0x0000009b, 
    0xffff0300, 0x00000003, 0x0000001c, 0x00000000, 0x00000094, 0x00000058, 
    0x00020000, 0x00010000, 0x00000064, 0x00000000, 0x00000074, 0x00020001, 
    0x00010000, 0x00000064, 0x00000000, 0x00000080, 0x00030000, 0x00010000, 
    0x00000084, 0x00000000, 0x675f466f, 0x67436f6c, 0x6f7200ab, 0x00010003, 
    0x00010004, 0x00010000, 0x00000000, 0x675f466f, 0x67506172, 0x616d7300, 
    0x733000ab, 0x0004000c, 0x00010001, 0x00010000, 0x00000000, 0x70735f33, 
    0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 0x00000001, 
    0x00000000, 0x00000000, 0x00000014, 0x01fc0010, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000040, 0x00000084, 0x10000200, 0x00000008, 
    0x00000000, 0x00001c63, 0x00030007, 0x00000021, 0x00003050, 0x00001151, 
    0x0000f2a0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x3f000000, 0x00000000, 0x3f800000, 0x00000000, 0x00011002, 
    0x00001200, 0xc4000000, 0x00006003, 0x10091200, 0x22000000, 0x10080001, 
    0x1f1ff688, 0x00004000, 0xc8080202, 0x001b1b00, 0xe1000200, 0xc80e0200, 
    0x01fcfcfc, 0xcb000200, 0xb8110002, 0x036cc643, 0xa001ffff, 0xc8010202, 
    0x006cb1c6, 0x8b0201ff, 0xc8070202, 0x00156cc0, 0xcb000200, 0xc8000202, 
    0x00b16c00, 0x79ff0000, 0xc80f8000, 0x00000000, 0xe2020200, 0x00000000, 
    0x00000000, 0x00000000
};
static const DWORD g_untextured_vs_bytecode[] = {
    0x102a1101, 0x000000c4, 0x00000084, 0x00000000, 0x00000024, 0x00000000, 
    0x0000008c, 0x00000000, 0x00000000, 0x00000064, 0x0000001c, 0x00000057, 
    0xfffe0300, 0x00000001, 0x0000001c, 0x00000000, 0x00000050, 0x00000030, 
    0x00020000, 0x00040000, 0x00000040, 0x00000000, 0x675f576f, 0x726c6456, 
    0x69657750, 0x726f6a00, 0x00030003, 0x00040004, 0x00010000, 0x00000000, 
    0x76735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x00000084, 0x00010001, 0x00000000, 0x00000000, 0x00001021, 0x00000001, 
    0x00000002, 0x00000001, 0x00000290, 0x00100003, 0x0030a004, 0x0000f0a0, 
    0x00001009, 0x30052003, 0x00001200, 0xc2000000, 0x00004005, 0x00001200, 
    0xc4000000, 0x00001009, 0x00002200, 0x00000000, 0x05f81000, 0x00000a88, 
    0x00000000, 0x05f80000, 0x00000688, 0x00000000, 0xc801803e, 0x003e3e00, 
    0x6f000100, 0xc802803e, 0x003e3e00, 0x6f010100, 0xc804803e, 0x003e3e00, 
    0x6f020100, 0xc808803e, 0x003e3e00, 0x6f030100, 0xc80f8000, 0x00000000, 
    0xe2000000, 0x00000000, 0x00000000, 0x00000000
};
static const DWORD g_untextured_ps_bytecode[] = {
    0x102a1100, 0x0000007c, 0x00000024, 0x00000000, 0x00000024, 0x00000000, 
    0x00000058, 0x00000000, 0x00000000, 0x00000030, 0x0000001c, 0x00000023, 
    0xffff0300, 0x00000000, 0x00000000, 0x00000000, 0x0000001c, 0x70735f33, 
    0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 0x00000024, 
    0x10000000, 0x00000000, 0x00000000, 0x00001021, 0x00000001, 0x00000001, 
    0x0000f0a0, 0x00000000, 0x1001c400, 0x22000000, 0xc80f8000, 0x00000000, 
    0xe2000000, 0x00000000, 0x00000000, 0x00000000
};

IDirect3DVertexDeclaration9* Shaders::s_vertexDecl = NULL;
IDirect3DVertexShader9*  Shaders::s_texturedVS = NULL;
IDirect3DPixelShader9*   Shaders::s_texturedPS = NULL;
IDirect3DVertexShader9*  Shaders::s_untexturedVS = NULL;
IDirect3DPixelShader9*   Shaders::s_untexturedPS = NULL;
D3DXMATRIX Shaders::s_worldMatrix;
D3DXMATRIX Shaders::s_viewMatrix;
D3DXMATRIX Shaders::s_projMatrix;
float Shaders::s_fogColor[3] = {0.5f, 0.8f, 1.0f};
float Shaders::s_fogDensity = 0.02f;
bool  Shaders::s_fogEnabled = false;
float Shaders::s_gamma = 1.0f;   // PORT(brightness): PS gamma exponent (1.0 = identity = no change)

bool Shaders::init() {
    D3DVERTEXELEMENT9 decl[] = {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
        { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };
    if (FAILED(g_pd3dDevice->CreateVertexDeclaration(decl, &s_vertexDecl)))
        s_vertexDecl = NULL;

    // Create the four shaders straight from the embedded 360 microcode. No
    // runtime compilation -> no D3DX compiler dependency, no fault path.
    if (FAILED(g_pd3dDevice->CreateVertexShader((const DWORD*)g_textured_vs_bytecode,   &s_texturedVS)))   s_texturedVS   = NULL;
    if (FAILED(g_pd3dDevice->CreatePixelShader ((const DWORD*)g_textured_ps_bytecode,   &s_texturedPS)))   s_texturedPS   = NULL;
    if (FAILED(g_pd3dDevice->CreateVertexShader((const DWORD*)g_untextured_vs_bytecode, &s_untexturedVS))) s_untexturedVS = NULL;
    if (FAILED(g_pd3dDevice->CreatePixelShader ((const DWORD*)g_untextured_ps_bytecode, &s_untexturedPS))) s_untexturedPS = NULL;

    D3DXMatrixIdentity(&s_worldMatrix);
    D3DXMatrixIdentity(&s_viewMatrix);
    D3DXMatrixIdentity(&s_projMatrix);
    return (s_texturedVS && s_texturedPS && s_untexturedVS && s_untexturedPS);
}

void Shaders::shutdown() {
    if (s_vertexDecl)   { s_vertexDecl->Release();   s_vertexDecl = NULL; }
    if (s_texturedVS)   { s_texturedVS->Release();   s_texturedVS = NULL; }
    if (s_texturedPS)   { s_texturedPS->Release();    s_texturedPS = NULL; }
    if (s_untexturedVS) { s_untexturedVS->Release();  s_untexturedVS = NULL; }
    if (s_untexturedPS) { s_untexturedPS->Release();   s_untexturedPS = NULL; }
}

void Shaders::useTexturedShader() {
    g_pd3dDevice->SetVertexShader(s_texturedVS);
    g_pd3dDevice->SetPixelShader(s_texturedPS);
    g_pd3dDevice->SetVertexDeclaration(s_vertexDecl);
}

void Shaders::useUntexturedShader() {
    g_pd3dDevice->SetVertexShader(s_untexturedVS);
    g_pd3dDevice->SetPixelShader(s_untexturedPS);
    g_pd3dDevice->SetVertexDeclaration(s_vertexDecl);
}

void Shaders::updateShaderConstants() {
    D3DXMATRIX wvp;
    D3DXMatrixMultiply(&wvp, &s_worldMatrix, &s_viewMatrix);
    D3DXMatrixMultiply(&wvp, &wvp, &s_projMatrix);
    D3DXMatrixTranspose(&wvp, &wvp);
    g_pd3dDevice->SetVertexShaderConstantF(0, (float*)&wvp, 4);
    float fogParams[4] = { s_fogDensity, s_fogEnabled ? 1.0f : 0.0f, 0, 0 };
    g_pd3dDevice->SetVertexShaderConstantF(8, fogParams, 1);
    float fogCol[4] = { s_fogColor[0], s_fogColor[1], s_fogColor[2], 1.0f };
    g_pd3dDevice->SetPixelShaderConstantF(0, fogCol, 1);
    g_pd3dDevice->SetPixelShaderConstantF(1, fogParams, 1);
    // PORT(★ CRITICAL): both the textured AND untextured pixel shaders read g_Gamma at
    // register c3 (texel.rgb = pow(texel.rgb, g_Gamma.x)). If c3 is never set it is an
    // UNINITIALIZED shader constant -> pow(rgb, garbage) -> corrupted/black output -> the
    // whole scene renders invisible (blue clear shows through). MUST upload c3 every draw.
    // Default 1.0 = identity (no change), matching Java's gamma-less OpenGL.
    float gammaVec[4] = { s_gamma, s_gamma, s_gamma, 1.0f };
    g_pd3dDevice->SetPixelShaderConstantF(3, gammaVec, 1);
}

void Shaders::setWorldMatrix(const D3DXMATRIX& m) { s_worldMatrix = m; }
void Shaders::setViewMatrix(const D3DXMATRIX& m)  { s_viewMatrix = m; }
void Shaders::setProjectionMatrix(const D3DXMATRIX& m) { s_projMatrix = m; }
D3DXMATRIX& Shaders::getWorldMatrix()  { return s_worldMatrix; }
D3DXMATRIX& Shaders::getViewMatrix()   { return s_viewMatrix; }
D3DXMATRIX& Shaders::getProjectionMatrix() { return s_projMatrix; }
void Shaders::setFogColor(float r, float g, float b) { s_fogColor[0]=r; s_fogColor[1]=g; s_fogColor[2]=b; }
void Shaders::setFogDensity(float d) { s_fogDensity = d; }
void Shaders::setFogEnabled(bool e)  { s_fogEnabled = e; }
void Shaders::setGamma(float g)      { s_gamma = g; }   // PORT(brightness): PS gamma exponent for the next draw(s)
''',

    'Tesselator.h': r'''#ifndef TESSELATOR_H
#define TESSELATOR_H

#include "stdafx.h"
#include "Shaders.h"

// Xbox 360 D3D9 Tesselator — replaces Java Tessellator (OpenGL immediate mode)
// Collects quads as triangle pairs into a dynamic vertex buffer and flushes with DrawPrimitiveUP.

class Tesselator {
public:
    static Tesselator& getInstance();

    void init();
    void init(int mode);   // GL primitive-mode variant (mode accepted, draws quads)
    void flush();

    void vertex(float x, float y, float z);
    void vertexUV(float x, float y, float z, float u, float v);
    void tex(float u, float v);
    void color3f(float r, float g, float b);
    void color4f(float r, float g, float b, float a);
    void color3b(int r, int g, int b);
    void color4b(int r, int g, int b, int a);
    void colorI(int rgb);
    void normal(float x, float y, float z);
    void disableColor();

    int getVertexCount() const { return m_vertexCount; }
    const TesselatorVertex* getVertices() const { return m_vertices; }

    static Tesselator* instance;

private:
    Tesselator();
    static const int MAX_VERTICES = 262144;
    TesselatorVertex m_vertices[MAX_VERTICES];
    int m_vertexCount;
    float m_u, m_v;
    DWORD m_color;
    bool m_hasTexture;
    bool m_hasColor;
    bool m_colorDisabled;
    int m_addedVertices;
};

#endif // TESSELATOR_H
''',

    'Tesselator.cpp': r'''#include "stdafx.h"
#include "Tesselator.h"
#include "Common.h"

Tesselator* Tesselator::instance = NULL;

Tesselator::Tesselator()
    : m_vertexCount(0), m_u(0), m_v(0)
    , m_color(0xFFFFFFFF), m_hasTexture(false)
    , m_hasColor(false), m_colorDisabled(false), m_addedVertices(0)
{
}

Tesselator& Tesselator::getInstance() {
    static Tesselator s_instance;
    instance = &s_instance;
    return s_instance;
}

void Tesselator::init() {
    m_vertexCount = 0;
    m_addedVertices = 0;
    m_hasTexture = false;
    m_hasColor = false;
    m_colorDisabled = false;
}

// Java Tessellator.startDrawing(int mode) variant: GL primitive mode (GL_QUADS=7,
// GL_LINE_STRIP=3, GL_LINES=1, ...). The D3D9 Tesselator currently always emits
// quads via flush(); the mode is accepted for source compatibility (PORT: line/
// strip primitive modes draw as quads for now).
void Tesselator::init(int mode) {
    (void)mode;
    init();
}

void Tesselator::flush() {
    if (m_vertexCount == 0) return;

    // Bind a shader for this batch. The Xbox 360 D3D runtime REJECTS any draw call
    // with no vertex shader set ("ERR[D3D]: A vertex shader must be set"). Pick
    // textured vs untextured by whether texcoords were supplied; useTexturedShader/
    // useUntexturedShader also set the vertex declaration. The caller has already
    // bound the texture and the transform matrices; updateShaderConstants() uploads
    // the current WVP so the batch draws with the right transform.
    if (m_hasTexture) {
        Shaders::useTexturedShader();
    } else {
        Shaders::useUntexturedShader();
    }
    Shaders::updateShaderConstants();

    // Convert quads to triangles with reversed winding for D3D CW culling
    int quadCount = m_vertexCount / 4;
    int triVertCount = quadCount * 6;
    TesselatorVertex* triVerts = (TesselatorVertex*)alloca(triVertCount * sizeof(TesselatorVertex));

    int out = 0;
    for (int q = 0; q < quadCount; ++q) {
        int base = q * 4;
        // Triangle 1: 0, 2, 1 (reversed winding)
        triVerts[out++] = m_vertices[base + 0];
        triVerts[out++] = m_vertices[base + 2];
        triVerts[out++] = m_vertices[base + 1];
        // Triangle 2: 0, 3, 2
        triVerts[out++] = m_vertices[base + 0];
        triVerts[out++] = m_vertices[base + 3];
        triVerts[out++] = m_vertices[base + 2];
    }

    // Leftover triangles (if vertex count not multiple of 4)
    int leftover = m_vertexCount - (quadCount * 4);
    if (leftover == 3) {
        triVerts[out++] = m_vertices[m_vertexCount - 3];
        triVerts[out++] = m_vertices[m_vertexCount - 1];
        triVerts[out++] = m_vertices[m_vertexCount - 2];
    }

    if (out > 0) {
        g_pd3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, out / 3,
            triVerts, sizeof(TesselatorVertex));
    }

    m_vertexCount = 0;
    m_addedVertices = 0;
}

void Tesselator::tex(float u, float v) { m_u = u; m_v = v; m_hasTexture = true; }

void Tesselator::color3f(float r, float g, float b) {
    if (m_colorDisabled) return;
    int ri = (int)(r * 255.0f); if (ri > 255) ri = 255; if (ri < 0) ri = 0;
    int gi = (int)(g * 255.0f); if (gi > 255) gi = 255; if (gi < 0) gi = 0;
    int bi = (int)(b * 255.0f); if (bi > 255) bi = 255; if (bi < 0) bi = 0;
    m_color = D3DCOLOR_ARGB(255, ri, gi, bi);
    m_hasColor = true;
}

void Tesselator::color4f(float r, float g, float b, float a) {
    if (m_colorDisabled) return;
    int ri = (int)(r * 255.0f); if (ri > 255) ri = 255; if (ri < 0) ri = 0;
    int gi = (int)(g * 255.0f); if (gi > 255) gi = 255; if (gi < 0) gi = 0;
    int bi = (int)(b * 255.0f); if (bi > 255) bi = 255; if (bi < 0) bi = 0;
    int ai = (int)(a * 255.0f); if (ai > 255) ai = 255; if (ai < 0) ai = 0;
    m_color = D3DCOLOR_ARGB(ai, ri, gi, bi);
    m_hasColor = true;
}

void Tesselator::color3b(int r, int g, int b) { m_color = D3DCOLOR_ARGB(255, r&0xFF, g&0xFF, b&0xFF); m_hasColor = true; }
void Tesselator::color4b(int r, int g, int b, int a) { m_color = D3DCOLOR_ARGB(a&0xFF, r&0xFF, g&0xFF, b&0xFF); m_hasColor = true; }
void Tesselator::colorI(int rgb) { m_color = D3DCOLOR_ARGB(255, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF); m_hasColor = true; }
void Tesselator::normal(float x, float y, float z) { /* normals stored via shader lighting if needed */ }
void Tesselator::disableColor() { m_colorDisabled = true; }

void Tesselator::vertex(float x, float y, float z) {
    if (m_vertexCount >= MAX_VERTICES) { flush(); }
    TesselatorVertex& v = m_vertices[m_vertexCount++];
    v.x = x; v.y = y; v.z = z;
    v.color = m_color;
    v.u = m_u; v.v = m_v;
    ++m_addedVertices;
}

void Tesselator::vertexUV(float x, float y, float z, float u, float v) {
    m_u = u; m_v = v; m_hasTexture = true;
    vertex(x, y, z);
}
''',

    'Textures.h': r'''#ifndef TEXTURES_H
#define TEXTURES_H

#include "stdafx.h"

// Xbox 360 texture manager — loads from game:\Content\ folder
// Replaces Java's RenderEngine texture system

class Textures {
public:
    static void init();
    static void shutdown();

    // Load a texture by Xbox 360 path (e.g. "game:\Content\terrain.png")
    // Returns a texture ID (index into internal array)
    static int load(const char* path);

    // Get the D3D9 texture pointer by ID
    static IDirect3DTexture9* getById(int id);

    // Bind a texture by ID to sampler stage 0
    static void bind(int id);

    // Bind a texture directly
    static void bindDirect(IDirect3DTexture9* tex);

    // Pre-load all known game textures
    static void loadAll();

    static const int MAX_TEXTURES = 128;

private:
    static IDirect3DTexture9* s_textures[MAX_TEXTURES];
    static char s_paths[MAX_TEXTURES][256];
    static int s_count;
};

#endif // TEXTURES_H
''',

    'Textures.cpp': r'''#include "stdafx.h"
#include "Textures.h"
#include "Common.h"

IDirect3DTexture9* Textures::s_textures[MAX_TEXTURES];
char Textures::s_paths[MAX_TEXTURES][256];
int Textures::s_count = 0;

void Textures::init() {
    memset(s_textures, 0, sizeof(s_textures));
    memset(s_paths, 0, sizeof(s_paths));
    s_count = 0;
}

void Textures::shutdown() {
    for (int i = 0; i < s_count; ++i) {
        if (s_textures[i]) {
            s_textures[i]->Release();
            s_textures[i] = NULL;
        }
    }
    s_count = 0;
}

int Textures::load(const char* path) {
    // Check if already loaded
    for (int i = 0; i < s_count; ++i) {
        if (_stricmp(s_paths[i], path) == 0) {
            return i;
        }
    }

    if (s_count >= MAX_TEXTURES) {
        DbgPrintF("ERROR: Texture limit reached (%d), cannot load: %s\n", MAX_TEXTURES, path);
        return -1;
    }

    int id = s_count;
    strncpy(s_paths[id], path, 255);
    s_paths[id][255] = 0;

    HRESULT hr = D3DXCreateTextureFromFileExA(
        g_pd3dDevice, path,
        D3DX_DEFAULT, D3DX_DEFAULT,  // width, height from file
        1,                            // 1 mip level
        0,                            // usage
        D3DFMT_A8R8G8B8,             // force 32-bit ARGB
        D3DPOOL_DEFAULT,
        D3DX_FILTER_NONE,            // no filter on load
        D3DX_FILTER_NONE,            // no mip filter
        0xFF000000,                  // color key (black = transparent)
        NULL, NULL,
        &s_textures[id]);

    if (FAILED(hr)) {
        DbgPrintF("WARNING: Failed to load texture: %s (hr=0x%08X)\n", path, hr);
        s_textures[id] = NULL;
    } else {
        DbgPrintF("Loaded texture [%d]: %s\n", id, path);
    }

    ++s_count;
    return id;
}

IDirect3DTexture9* Textures::getById(int id) {
    if (id < 0 || id >= s_count) return NULL;
    return s_textures[id];
}

void Textures::bind(int id) {
    g_pd3dDevice->SetTexture(0, getById(id));
}

void Textures::bindDirect(IDirect3DTexture9* tex) {
    g_pd3dDevice->SetTexture(0, tex);
}

void Textures::loadAll() {
    // Pre-load all known game textures from Content folder
    static const char* texturePaths[] = {
        "game:\\Content\\terrain.png",
        "game:\\Content\\mob\\char.png",
        "game:\\Content\\default.png",
        "game:\\Content\\particles.png",
        "game:\\Content\\clouds.png",
        "game:\\Content\\rain.png",
        "game:\\Content\\water.png",
        "game:\\Content\\dirt.png",
        "game:\\Content\\grass.png",
        "game:\\Content\\rock.png",
        "game:\\Content\\shadow.png",
        "game:\\Content\\gui\\gui.png",
        "game:\\Content\\gui\\icons.png",
        "game:\\Content\\gui\\items.png",
        "game:\\Content\\gui\\logo.png",
        "game:\\Content\\gui\\inventory.png",
        "game:\\Content\\gui\\container.png",
        "game:\\Content\\gui\\crafting.png",
        "game:\\Content\\item\\arrows.png",
        "game:\\Content\\item\\sign.png",
        "game:\\Content\\misc\\gear.png",
        "game:\\Content\\misc\\gearmiddle.png",
        "game:\\Content\\mob\\creeper.png",
        "game:\\Content\\mob\\pig.png",
        "game:\\Content\\mob\\sheep.png",
        "game:\\Content\\mob\\sheep_fur.png",
        "game:\\Content\\mob\\skeleton.png",
        "game:\\Content\\mob\\spider.png",
        "game:\\Content\\mob\\spider_eyes.png",
        "game:\\Content\\mob\\zombie.png",
        "game:\\Content\\armor\\chain_1.png",
        "game:\\Content\\armor\\chain_2.png",
        "game:\\Content\\armor\\diamond_1.png",
        "game:\\Content\\armor\\diamond_2.png",
        "game:\\Content\\armor\\gold_1.png",
        "game:\\Content\\armor\\gold_2.png",
        "game:\\Content\\armor\\iron_1.png",
        "game:\\Content\\armor\\iron_2.png",
        "game:\\Content\\terrain\\sun.png",
        "game:\\Content\\terrain\\moon.png",
    };
    int count = sizeof(texturePaths) / sizeof(texturePaths[0]);
    DbgPrintF("Loading %d textures from Content folder...\n", count);
    for (int i = 0; i < count; ++i) {
        load(texturePaths[i]);
    }
    DbgPrintF("Texture loading complete: %d/%d loaded\n", s_count, count);
}
''',

    'Main.cpp': r'''// Main.cpp — Xbox 360 entry point for Minecraft Indev
// Creates D3D9 device, initializes subsystems, runs game loop

#include "stdafx.h"
#include "Common.h"
#include "Shaders.h"
#include "Tesselator.h"
#include "Textures.h"

// Globals
IDirect3D9*       g_pD3D       = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
IDirect3DTexture9* g_terrainTexture = NULL;
IDirect3DTexture9* g_guiTexture     = NULL;
IDirect3DTexture9* g_charTexture    = NULL;
IDirect3DTexture9* g_defaultTexture = NULL;

// Forward declarations
class Minecraft;
extern Minecraft* g_minecraft;

static bool InitD3D() {
    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!g_pD3D) return false;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth        = SCREEN_WIDTH;
    d3dpp.BackBufferHeight       = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount        = 1;
    d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow          = NULL;
    d3dpp.Windowed               = FALSE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags                  = 0;
    d3dpp.FullScreen_RefreshRateInHz = 60;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    HRESULT hr = g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
        D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice);
    return SUCCEEDED(hr);
}

void __cdecl main() {
    if (!InitD3D()) {
        OutputDebugStringA("D3D init failed\n");
        return;
    }

    // Render state defaults
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    g_pd3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    if (!Shaders::init()) {
        OutputDebugStringA("Shader compile failed\n");
        return;
    }

    // Load all textures from game:\Content\ folder
    Textures::init();
    Textures::loadAll();

    // Convenience globals for the most-used textures
    g_terrainTexture = Textures::getById(Textures::load("game:\\Content\\terrain.png"));
    g_guiTexture     = Textures::getById(Textures::load("game:\\Content\\gui\\gui.png"));
    g_charTexture    = Textures::getById(Textures::load("game:\\Content\\mob\\char.png"));
    g_defaultTexture = Textures::getById(Textures::load("game:\\Content\\default.png"));

    OutputDebugStringA("Minecraft Indev — Xbox 360 port initialized\n");

    // PORT: Main game loop placeholder
    bool running = true;
    while (running) {
        // PORT: XInput polling
        XINPUT_STATE state;
        if (XInputGetState(0, &state) == ERROR_SUCCESS) {
            if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) {
                running = false;
            }
        }

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
            D3DCOLOR_XRGB(128, 204, 255), 1.0f, 0);
        g_pd3dDevice->BeginScene();

        // PORT: game->render() here

        g_pd3dDevice->EndScene();
        g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }

    // Cleanup
    Shaders::shutdown();
    Textures::shutdown();
    if (g_pd3dDevice)     g_pd3dDevice->Release();
    if (g_pD3D)           g_pD3D->Release();
}
''',
}

# Stub headers emitted alongside templates for Indev
_INDEV_STUB_HEADERS = {
    'Random.h': r'''#ifndef RANDOM_H
#define RANDOM_H
// Random is defined in Common.h
#include "Common.h"
#endif // RANDOM_H
''',
    'SoundSystem.h': r'''#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
// Stub: Java sound system not used on Xbox 360
#include <string>

class SoundSystemConfig {
public:
    static int TYPE_STREAMING;
    static int TYPE_NORMAL;
    static void addLibrary(int lib) {}
    static void setCodec(const std::string& ext, int codec) {}
};

class SoundSystem {
public:
    SoundSystem() {}
    void init() {}
    void stop() {}
    void setListenerPosition(float x, float y, float z) {}
    void setListenerAngle(float yaw) {}
    void setMasterVolume(float v) {}
    void setVolume(const std::string& src, float v) {}
    void play(const std::string& src) {}
    void backgroundMusic(const std::string& name, const std::string& path, bool loop) {}
    void newSource(bool priority, const std::string& name, const std::string& path, float x, float y, float z, int type, float dist) {}
    void newStreamingSource(bool priority, const std::string& name, const std::string& path, float x, float y, float z, int type, float dist) {}
    bool playing(const std::string& src) { return false; }
};

class LibraryLWJGLOpenAL {};
class CodecJOrbis {};

#endif // SOUNDSYSTEM_H
''',
    'Vec3.h': r'''#ifndef VEC3_H
#define VEC3_H
// Vec3 is the RubyDung name; Indev uses Vec3D
#include "Vec3D.h"
typedef Vec3D Vec3;
#endif // VEC3_H
''',
    'Map.h': r'''#ifndef MAP_H
#define MAP_H
// Stub: java.util.Map -> std::map
#include <map>
#endif // MAP_H
''',
    'DataInput.h': r'''#ifndef DATAINPUT_H
#define DATAINPUT_H
// java.io.DataInput — big-endian reader over an in-memory byte buffer.
// Faithful to Java's DataInputStream semantics (used by NBT load).
#include <string>
#include <cstring>

class DataInput {
public:
    const unsigned char* data;
    int length;
    int pos;

    DataInput() : data(0), length(0), pos(0) {}
    DataInput(const unsigned char* d, int len) : data(d), length(len), pos(0) {}

    int readUnsignedByte() { return (pos < length) ? data[pos++] : 0; }
    signed char readByte()  { return (signed char)readUnsignedByte(); }
    bool readBoolean()      { return readUnsignedByte() != 0; }
    short readShort()       { int h = readUnsignedByte(), l = readUnsignedByte(); return (short)((h << 8) | l); }
    int readUnsignedShort() { int h = readUnsignedByte(), l = readUnsignedByte(); return (h << 8) | l; }
    int readInt() {
        int a = readUnsignedByte(), b = readUnsignedByte(), c = readUnsignedByte(), d = readUnsignedByte();
        return (a << 24) | (b << 16) | (c << 8) | d;
    }
    long long readLong() {
        long long v = 0;
        for (int i = 0; i < 8; ++i) v = (v << 8) | (long long)readUnsignedByte();
        return v;
    }
    float readFloat()   { int i = readInt(); float f; memcpy(&f, &i, 4); return f; }
    double readDouble() { long long l = readLong(); double d; memcpy(&d, &l, 8); return d; }
    void readFully(unsigned char* dst, int len) { for (int i = 0; i < len; ++i) dst[i] = (unsigned char)readUnsignedByte(); }
    // Java DataInput.readUTF: 2-byte length prefix, then modified-UTF8 bytes.
    std::string readUTF() {
        int n = readUnsignedShort();
        std::string s;
        s.reserve(n);
        for (int i = 0; i < n; ++i) s.push_back((char)readUnsignedByte());
        return s;
    }
};
#endif // DATAINPUT_H
''',
    'Class.h': r'''#ifndef CLASS_H
#define CLASS_H
// Stub: java.lang.Class — not used on Xbox 360
#endif // CLASS_H
''',
    'Cursor.h': r'''#ifndef CURSOR_H
#define CURSOR_H
// Stub: LWJGL Cursor — no cursor on Xbox 360
#endif // CURSOR_H
''',
    'DecimalFormat.h': r'''#ifndef DECIMALFORMAT_H
#define DECIMALFORMAT_H
// Stub: java.text.DecimalFormat — not used on Xbox 360
#endif // DECIMALFORMAT_H
''',
    'InputStream.h': r'''#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H
// Stub: java.io.InputStream — not used on Xbox 360
#endif // INPUTSTREAM_H
''',
    'URL.h': r'''#ifndef URL_H
#define URL_H
// Stub: java.net.URL — not used on Xbox 360
#endif // URL_H
''',
    'Session.h': r'''#ifndef SESSION_H
#define SESSION_H
// Stub: Minecraft Session — not used on Xbox 360
class Session {
public:
    std::string username;
    Session() : username("Player") {}
};
#endif // SESSION_H
''',
    'MinecraftApplet.h': r'''#ifndef MINECRAFTAPPLET_H
#define MINECRAFTAPPLET_H
// Stub: Java applet — not used on Xbox 360
#endif // MINECRAFTAPPLET_H
''',
    'ImageBufferDownload.h': r'''#ifndef IMAGEBUFFERDOWNLOAD_H
#define IMAGEBUFFERDOWNLOAD_H
// Stub: Image download — not used on Xbox 360
#endif // IMAGEBUFFERDOWNLOAD_H
''',
    'Player.h': r'''#ifndef PLAYER_H
#define PLAYER_H
// Player.h alias — Indev uses EntityPlayer/EntityPlayerSP
#include "EntityPlayerSP.h"
#endif // PLAYER_H
''',
    'Zombie.h': r'''#ifndef ZOMBIE_H
#define ZOMBIE_H
// Zombie.h alias — Indev uses EntityZombie
#include "EntityZombie.h"
#endif // ZOMBIE_H
''',
    'DataOutput.h': r'''#ifndef DATAOUTPUT_H
#define DATAOUTPUT_H
// java.io.DataOutput — big-endian writer into a growable byte buffer.
// Faithful to Java's DataOutputStream semantics (used by NBT save).
#include <string>
#include <vector>

class DataOutput {
public:
    std::vector<unsigned char> buffer;

    void writeByte(int v)    { buffer.push_back((unsigned char)(v & 0xFF)); }
    void writeBoolean(bool v){ writeByte(v ? 1 : 0); }
    void writeShort(int v)   { writeByte(v >> 8); writeByte(v); }
    void writeChar(int v)    { writeShort(v); }
    void writeInt(int v)     { writeByte(v >> 24); writeByte(v >> 16); writeByte(v >> 8); writeByte(v); }
    void writeLong(long long v) { for (int i = 56; i >= 0; i -= 8) writeByte((int)(v >> i)); }
    void writeFloat(float v)    { int i;       memcpy(&i, &v, 4); writeInt(i); }
    void writeDouble(double v)  { long long l; memcpy(&l, &v, 8); writeLong(l); }
    void write(const unsigned char* d, int len) { for (int i = 0; i < len; ++i) buffer.push_back(d[i]); }
    // Java DataOutput.writeUTF: 2-byte length prefix, then modified-UTF8 bytes.
    void writeUTF(const std::string& s) {
        writeShort((int)s.size());
        for (size_t i = 0; i < s.size(); ++i) buffer.push_back((unsigned char)s[i]);
    }
};
#endif // DATAOUTPUT_H
''',
    'OutputStream.h': r'''#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H
// Stub: java.io.OutputStream — not used on Xbox 360
#endif // OUTPUTSTREAM_H
''',
    'Robot.h': r'''#ifndef ROBOT_H
#define ROBOT_H
// Stub: java.awt.Robot — not used on Xbox 360
#endif // ROBOT_H
''',
    'OpenGlCapsChecker.h': r'''#ifndef OPENGLCAPSCHECKER_H
#define OPENGLCAPSCHECKER_H
// Stub: OpenGL caps checker — not used on Xbox 360
#endif // OPENGLCAPSCHECKER_H
''',
    'ThreadDownloadImageData.h': r'''#ifndef THREADDOWNLOADIMAGEDATA_H
#define THREADDOWNLOADIMAGEDATA_H
// Stub: Image download thread — not used on Xbox 360
#endif // THREADDOWNLOADIMAGEDATA_H
''',
    'Vertex.h': r'''#ifndef VERTEX_H
#define VERTEX_H
// Stub: Vertex — Indev uses PositionTextureVertex
#include "PositionTextureVertex.h"
#endif // VERTEX_H
''',
    'ThreadDownloadResources.h': r'''#ifndef THREADDOWNLOADRESOURCES_H
#define THREADDOWNLOADRESOURCES_H
// Stub: Resource download thread — not used on Xbox 360
#endif // THREADDOWNLOADRESOURCES_H
''',
}


def _decode_templates():
    """Decode base64 RubyDung templates."""
    result = {}
    for fname, b64_parts in _PLATFORM_TEMPLATES_B64.items():
        if isinstance(b64_parts, str):
            b64 = b64_parts
        else:
            b64 = ''.join(b64_parts)
        raw = zlib.decompress(base64.b64decode(b64))
        result[fname] = raw.decode('utf-8').replace('\r\n', '\n').replace('\r', '\n')
    return result

_RUBYDUNG_TEMPLATES = _decode_templates()


def get_platform_templates():
    """Return the platform templates for the active profile."""
    if PROFILE.name == 'rubydung':
        return dict(_RUBYDUNG_TEMPLATES)
    # indev AND all newer/derived profiles (alpha, beta, custom — they inherit indev) use the modern
    # indev platform templates: Shaders/Tesselator/Textures expose the getInstance / vertexUV / colorI /
    # color3f|4f / normal / getWorldMatrix / Textures::load / getById API that post-RubyDung rendering
    # code calls, plus the java.io stub headers (Class/Random/Map/DataInput/...). RubyDung's primitive
    # templates were the wrong base for alpha (caused ~421 C2039 'not a member' errors).
    d = dict(_INDEV_TEMPLATES_RAW)
    d.update(_INDEV_STUB_HEADERS)
    return d

# Default — rebuilt when profile changes
PLATFORM_TEMPLATES = get_platform_templates()


# ============================================================================
# POSTFIX ERROR-REDUCTION PASSES
# ============================================================================
# Integrated from postfix.py — runs after conversion to fix systematic C++ errors.
# All constants prefixed with PF_ to avoid collision with converter globals.

PF_VALUE_TYPES = {
    'AxisAlignedBB', 'Vec3D', 'MovingObjectPosition',
    'ItemStack', 'KeyBinding', 'ChatLine', 'NextTickListEntry',
    'MetadataChunkBlock', 'SoundPoolEntry',
    'PositionTextureVertex', 'TexturedQuad', 'ScaledResolution',
}

PF_REFERENCE_TYPES = {
    'World', 'Minecraft', 'Entity', 'EntityLiving', 'EntityPlayer',
    'EntityPlayerSP', 'EntityCreature', 'EntityAnimal', 'EntityMob',
    'EntityArrow', 'EntityItem', 'EntityTNT', 'EntityFX',
    'EntityBubbleFX', 'EntityDiggingFX', 'EntityExplodeFX',
    'EntityFlameFX', 'EntityLavaFX', 'EntityPickupFX', 'EntityRainFX',
    'EntitySplashFX', 'EntityCreeper', 'EntityGiantZombie',
    'EntityPig', 'EntitySheep', 'EntitySkeleton', 'EntitySpider',
    'EntityZombie', 'EntitySorter',
    'Block', 'BlockBookshelf', 'BlockBreakable', 'BlockChest',
    'BlockContainer', 'BlockCrops', 'BlockDirt', 'BlockFarmland',
    'BlockFire', 'BlockFlower', 'BlockFlowing', 'BlockFluid',
    'BlockGears', 'BlockGlass', 'BlockGrass', 'BlockLeaves',
    'BlockLeavesBase', 'BlockLog', 'BlockMushroom', 'BlockOre',
    'BlockOreBlock', 'BlockSand', 'BlockSapling', 'BlockSource',
    'BlockSponge', 'BlockStationary', 'BlockStep', 'BlockStone',
    'BlockTNT', 'BlockTorch', 'BlockWorkbench',
    'Item', 'ItemArmor', 'ItemAxe', 'ItemBlock', 'ItemBow',
    'ItemFlintAndSteel', 'ItemFood', 'ItemHoe', 'ItemPickaxe',
    'ItemSeeds', 'ItemSoup', 'ItemSpade', 'ItemSword', 'ItemTool',
    'RenderEngine', 'RenderGlobal', 'RenderBlocks', 'RenderManager',
    'Render', 'RenderLiving', 'RenderItem', 'RenderArrow',
    'Tessellator', 'Tesselator', 'WorldRenderer', 'EntityRenderer',
    'ItemRenderer', 'FontRenderer',
    'Gui', 'GuiScreen', 'GuiButton', 'GuiSmallButton', 'GuiIngame',
    'GuiContainer', 'GuiChest', 'GuiCrafting', 'GuiInventory',
    'GuiControls', 'GuiErrorScreen', 'GuiGameOver', 'GuiIngameMenu',
    'GuiMainMenu', 'GuiOptions',
    'ModelBase', 'ModelBiped', 'ModelCreeper', 'ModelPig',
    'ModelQuadruped', 'ModelRenderer', 'ModelSheep', 'ModelSheepFur',
    'ModelSkeleton', 'ModelSpider', 'ModelZombie',
    'GameSettings', 'Timer', 'MouseHelper',
    'EffectRenderer', 'EntityMap', 'EntityMapSlot',
    'Light', 'LevelGenerator', 'MobSpawner', 'Pathfinder',
    'PathEntity', 'PlayerController', 'PlayerControllerCreative',
    'PlayerControllerSP', 'PlayerLoader',
    'SoundManager', 'SoundPool',
    'LoadingScreenRenderer', 'TileEntity', 'TileEntityChest',
    'Frustrum', 'ClippingHelperFaces',
    'CraftingManager', 'CraftingRecipe',
    'InventoryPlayer', 'InventoryCrafting', 'InventoryCraftResult',
    'InventoryLargeChest',
    'MovementInput', 'MovementInputFromOptions',
    'LevelLoader',
    'TextureFX', 'TextureFlamesFX', 'TextureGearsFX',
    'TextureLavaFX', 'TextureWaterFX', 'TextureWaterFlowFX',
    'Material', 'MaterialLiquid', 'MaterialLogic', 'MaterialTransparent',
    'StepSound', 'StepSoundGlass', 'StepSoundGravel',
    'NoiseGenerator', 'NoiseGeneratorDistort', 'NoiseGeneratorOctaves',
    'NoiseGeneratorPerlin',
    'RecipeSorter', 'RecipesArmor', 'RecipesCrafting', 'RecipesFood',
    'RecipesIngots', 'RecipesTools', 'RecipesWeapons',
    'NBTTagCompound', 'NBTTagList', 'NBTTagFloat', 'NBTTagShort',
    'NBTBase', 'NBTTagByte', 'NBTTagByteArray', 'NBTTagDouble',
    'NBTTagEnd', 'NBTTagInt', 'NBTTagLong', 'NBTTagString',
    'Random', 'RenderHelper', 'IInventory',
}

PF_RETURNS_POINTER_TYPES = {
    'NBTTagList', 'NBTTagCompound', 'NBTBase', 'NBTTagFloat', 'NBTTagShort',
    'NBTTagByte', 'NBTTagByteArray', 'NBTTagDouble', 'NBTTagEnd',
    'NBTTagInt', 'NBTTagLong', 'NBTTagString',
    'Material', 'StepSound', 'World', 'Entity', 'EntityPlayerSP',
    'EntityMapSlot', 'RenderEngine', 'FontRenderer',
    'TileEntity', 'TileEntityChest', 'PathEntity', 'IWorldAccess',
}

PF_BLOCK_STATICS = {
    'crate', 'clothGray', 'torch', 'stairDouble',
    'fire', 'dirt', 'stone', 'planks', 'grass', 'waterMoving',
    'waterStill', 'lavaMoving', 'lavaStill', 'stairSingle',
    'cobblestone', 'tnt', 'sand', 'gravel', 'oreGold', 'oreIron',
    'oreDiamond', 'oreCoal', 'wood', 'leaves', 'sponge', 'glass',
    'cloth', 'blockGold', 'blockIron', 'blockDiamond',
    'workbench', 'crops', 'tilledField', 'chest', 'cactus',
    'bedrock', 'ice', 'bookShelf', 'mossStone', 'obsidian',
    'torchWood', 'mushroomBrown', 'mushroomRed', 'sapling',
    'flowerYellow', 'flowerRed', 'clothRed',
}

PF_ITEM_STATICS = {
    'helmetLeather', 'helmetChain', 'helmetSteel', 'helmetDiamond', 'helmetGold',
    'plateLeather', 'plateChain', 'plateSteel', 'plateDiamond', 'plateGold',
    'legsLeather', 'legsChain', 'legsSteel', 'legsDiamond', 'legsGold',
    'bootsLeather', 'bootsChain', 'bootsSteel', 'bootsDiamond', 'bootsGold',
}

def fix_vec3_0f(content, fname):
    """Fix: Vec3.0f / Vec3::0f -> Vec3D (broken decompiler float literal in type name)."""
    changed = 0
    old = content
    # Vec3.0f -> Vec3D  and  Vec3::0f -> Vec3D
    content = re.sub(r'\bVec3\.0f\b', 'Vec3D', content)
    content = re.sub(r'\bVec3::0f\b', 'Vec3D', content)
    if content != old:
        changed = content.count('Vec3D') - old.count('Vec3D')
    return content, max(changed, 0)


def fix_bare_zero_port(content, fname):
    """Fix: lines starting with '  0 /* PORT:' -> '// PORT:' (bare expression statement)."""
    changed = 0
    old = content
    content = re.sub(r'^(\s*)0\s*/\*\s*PORT:', r'\1// PORT:', content, flags=re.MULTILINE)
    if content != old:
        # Remove trailing */ from these converted lines
        content = re.sub(r'^(\s*// PORT:[^\n]*?)\s*\*/', r'\1', content, flags=re.MULTILINE)
        changed = 1
    return content, changed


def fix_this_dot(content, fname):
    """Fix: this.member -> this->member in non-comment lines."""
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        if '// ' in line and 'this.' in line.split('// ', 1)[1] if '// ' in line else False:
            result.append(line)
            continue
        old = line
        # Only replace this. that's NOT in a comment
        if 'this.' in line:
            comment_start = line.find('//')
            if comment_start >= 0:
                code_part = line[:comment_start]
                comment_part = line[comment_start:]
            else:
                code_part = line
                comment_part = ''
            new_code = re.sub(r'\bthis\.', 'this->', code_part)
            line = new_code + comment_part
        if line != old:
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_java_casts_to_ptr(content, fname):
    """Fix: (Type)expr -> (Type*)expr for reference types used as C-style casts."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # Pattern: (TypeName)expr where TypeName is a known reference type
    # But NOT (TypeName*) which is already correct
    for t in sorted(PF_REFERENCE_TYPES, key=len, reverse=True):
        # (Type)varname or (Type)expr->method or (Type)NULL
        pat = r'\((%s)\)(?!\*)' % re.escape(t)
        repl = r'(%s*)' % t
        content = re.sub(pat, repl, content)
    if content != old:
        changed = 1
    return content, changed


def fix_nbt_float_access(content, fname):
    """Fix: ((NBTTagFloat)var.tagAt(n)).floatValue -> static_cast<NBTTagFloat*>(var->tagAt(n))->m_floatValue"""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(
        r'\(\(NBTTagFloat\*\)(\w+)\.tagAt\((\d+)\)\)\.floatValue',
        r'static_cast<NBTTagFloat*>(\1->tagAt(\2))->m_floatValue',
        content)
    content = re.sub(
        r'\(\(NBTTagFloat\*\)(\w+)->tagAt\((\d+)\)\)\.floatValue',
        r'static_cast<NBTTagFloat*>(\1->tagAt(\2))->m_floatValue',
        content)
    # NOTE: a global '->m_data' -> '->m_floatValue' replace used to live here as a
    # migration hack. It was removed — NBTTagFloat's field is already 'm_floatValue',
    # and the blanket replace corrupted Light's own 'm_data' field (15 errors).
    if content != old:
        changed = content.count('m_floatValue') - old.count('m_floatValue')
    return content, max(changed, 0)


def fix_ptr_to_value_locals(content, fname):
    """Fix: Type var = ptrExpr -> Type* var = ptrExpr for types that return pointers.
    Catches patterns like: NBTTagList var2 = var1->getTagList(...)
    and: Material var6 = expr->getBlockMaterial(...)
    and: StepSound var = expr->stepSound
    """
    if not fname.endswith('.cpp'):
        return content, 0
    # Platform/value types that look like reference types (uppercase, not in any Java
    # profile set) but are returned/used BY VALUE — never convert these to pointers.
    EXCLUDED = {'D3DXMATRIX', 'D3DXVECTOR2', 'D3DXVECTOR3', 'D3DXVECTOR4',
                'D3DXQUATERNION', 'D3DCOLOR', 'HRESULT', 'DWORD', 'Tesselator'}
    total = 0
    # Iterate to a fixpoint: converting one local to a pointer (e.g. a '(T*)' cast temp)
    # makes it a known pointer var, which can in turn promote a local assigned FROM it
    # ('T v = thatTemp;'). A single pass collects ptr_vars before such promotions, so
    # chained cases need another round.
    for _ in range(6):
        lines = content.split('\n')
        ptr_vars = set()
        for line in lines:
            for mm in re.finditer(r'\b([A-Z]\w+)\*\s+(\w+)\s*[;=]', line):
                ptr_vars.add(mm.group(2))
            for mm in re.finditer(r'\b([A-Z]\w+)\*\s+(\w+)\s*[,)]', line):  # params
                ptr_vars.add(mm.group(2))
        result = []
        changed = 0
        for line in lines:
            old = line
            m = re.match(r'^(\s+)([A-Z]\w+)\s+(\w+)\s*=\s*(.+)$', line)
            if m:
                indent, typename, varname, rhs = m.groups()
                lhs = line.split('=')[0]
                rhs_strip = rhs.strip().rstrip(';').strip()
                if ('*' not in lhs and typename not in EXCLUDED
                        and not typename.startswith('std')):
                    if is_reference_type(typename):
                        # Reference types are ALWAYS pointers in this port — a value local
                        # of one is wrong regardless of the RHS form (field, method, NULL...).
                        # EXCLUDED guards the platform value types (D3DXMATRIX/Tesselator/...).
                        line = '%s%s* %s = %s' % (indent, typename, varname, rhs)
                    elif typename in PF_RETURNS_POINTER_TYPES:
                        rhs_strip2 = rhs_strip
                        if (rhs_strip2 in ptr_vars or '->' in rhs or 'new ' in rhs
                                or '.get' in rhs or '.tagAt' in rhs
                                or bool(re.match(r'^\(\s*\w+\s*\*\s*\)', rhs_strip2))):
                            line = '%s%s* %s = %s' % (indent, typename, varname, rhs)
            else:
                # Bare declaration 'RefType var;' (no initializer) — also a pointer for
                # reference types, else a later 'var = ptr;' assignment mismatches.
                bm = re.match(r'^(\s+)([A-Z]\w+)\s+(\w+)\s*;\s*$', line)
                if bm:
                    indent2, tn, vn = bm.groups()
                    # Use the EXPLICIT reference_types set (not the uppercase fallback) so
                    # bare D3D value decls (D3DPRESENT_PARAMETERS d3dpp;) aren't touched.
                    if tn in PROFILE.reference_types:
                        line = '%s%s* %s = NULL;' % (indent2, tn, vn)
            if line != old:
                changed += 1
            result.append(line)
        content = '\n'.join(result)
        total += changed
        if changed == 0:
            break
    return content, total


def fix_new_without_pointer(content, fname):
    """Fix: Type var = new Type(...) -> Type* var = new Type(...) for reference types."""
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        m = re.match(r'^(\s+)([A-Z]\w+)\s+(\w+)\s*=\s*new\s+([A-Z]\w+)\s*\(', line)
        if m and m.group(2) in PF_REFERENCE_TYPES and '*' not in line.split('=')[0]:
            old = line
            line = re.sub(
                r'^(\s+)(%s)\s+(%s\s*=\s*new\s)' % (re.escape(m.group(2)), re.escape(m.group(3))),
                r'\g<1>\g<2>* \g<3>', line)
            if line != old:
                changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_arrow_on_value_type(content, fname):
    """Fix: var-> on a variable declared as a value type -> var."""
    lines = content.split('\n')
    var_types = {}
    for line in lines:
        m = re.match(r'\s+([A-Z]\w+)\*?\s+(\w+)\s*[;=]', line)
        if m:
            var_types[m.group(2)] = m.group(1)
        m2 = re.match(r'\s+([A-Z]\w+)\*\s+(\w+)\s*=', line)
        if m2:
            var_types[m2.group(2)] = m2.group(1) + '*'

    changed = 0
    result = []
    for line in lines:
        fl = line
        for vname, vtype in var_types.items():
            base_type = vtype.rstrip('*').strip()
            is_ptr = vtype.endswith('*')
            if base_type in PF_VALUE_TYPES and not is_ptr:
                old = fl
                fl = re.sub(r'\b%s->' % re.escape(vname), '%s.' % vname, fl)
                if fl != old:
                    changed += 1
        result.append(fl)
    return '\n'.join(result), changed


def fix_boundingbox_arrow(content, fname):
    """Fix: boundingBox-> to boundingBox. since it's a value type (AxisAlignedBB)."""
    changed = content.count('boundingBox->')
    content = content.replace('boundingBox->', 'boundingBox.')
    return content, changed


def fix_missing_includes(content, fname):
    """Add missing #includes based on known incomplete-type errors."""
    if not fname.endswith('.cpp'):
        return content, 0

    INCLUDE_MAP = {
        'GuiContainer.cpp': {'Minecraft'},
        'GuiControls.cpp': {'Minecraft'},
        'GuiGameOver.cpp': {'Minecraft'},
        'GuiIngameMenu.cpp': {'Minecraft'},
        'GuiMainMenu.cpp': {'Minecraft', 'FontRenderer'},
        'GuiOptions.cpp': {'Minecraft'},
        'GuiIngame.cpp': {'EntityPlayerSP', 'World'},
        'GuiChest.cpp': {'FontRenderer'},
        'GuiButton.cpp': {'RenderEngine'},
        'LoadingScreenRenderer.cpp': {'FontRenderer'},
        'PlayerControllerCreative.cpp': {'EntityPlayerSP'},
        'BlockFlowing.cpp': {'BlockFire'},
        'BlockFluid.cpp': {'BlockFire'},
        'BlockSand.cpp': {'BlockFire'},
        'BlockStationary.cpp': {'BlockFire'},
        'ItemFlintAndSteel.cpp': {'BlockFire'},
        'ItemRenderer.cpp': {'BlockFire', 'World'},
        'Render.cpp': {'BlockFire'},
        'RenderBlocks.cpp': {'BlockFire'},
        'TextureFlamesFX.cpp': {'BlockFire'},
        'BlockFlower.cpp': {'BlockGrass'},
        'EntityAnimal.cpp': {'BlockGrass'},
        'ItemHoe.cpp': {'BlockGrass'},
        'LevelGenerator.cpp': {'BlockGrass'},
        'LevelLoader.cpp': {'BlockGrass'},
        'BlockGears.cpp': {'Material'},
        'BlockSponge.cpp': {'Material'},
        'BlockTorch.cpp': {'Material'},
        'NBTTagCompound.cpp': {'NBTBase', 'NBTTagByte', 'NBTTagShort', 'NBTTagInt',
                               'NBTTagLong', 'NBTTagFloat', 'NBTTagDouble',
                               'NBTTagString', 'NBTTagByteArray', 'NBTTagList', 'NBTTagEnd'},
        'Pathfinder.cpp': {'World'},
        'RenderManager.cpp': {'EntityItem', 'World'},
        'RenderItem.cpp': {'RenderManager'},
        'EntitySheep.cpp': {'Block'},
        'Light.cpp': {'World'},
        'EntityPickupFX.cpp': {'RenderManager'},
        'RenderGlobal.cpp': {'RenderManager', 'Vec3D', 'EffectRenderer', 'GameSettings'},
        'EntityArrow.cpp': {'EntityMap', 'InventoryPlayer'},
        'EntityPlayerSP.cpp': {'EffectRenderer', 'GuiChest', 'GuiCrafting'},
        'GuiChest.cpp': {'FontRenderer', 'Minecraft'},
        'GuiContainer.cpp': {'Minecraft', 'IInventory', 'GameSettings'},
        'GuiIngame.cpp': {'EntityPlayerSP', 'World', 'EffectRenderer', 'EntityRenderer',
                          'GameSettings', 'RenderEngine'},
        'GuiMainMenu.cpp': {'Minecraft', 'FontRenderer', 'RenderEngine', 'GuiOptions'},
        'GuiScreen.cpp': {'RenderEngine'},
        'ItemBow.cpp': {'InventoryPlayer'},
        'ItemRenderer.cpp': {'BlockFire', 'World', 'InventoryPlayer', 'RenderEngine'},
        'LevelLoader.cpp': {'BlockGrass', 'EntityMap'},
        'ModelZombie.cpp': {'ModelRenderer'},
        'PlayerController.cpp': {'EffectRenderer'},
        'PlayerControllerCreative.cpp': {'EntityPlayerSP', 'InventoryPlayer'},
        'PlayerControllerSP.cpp': {'InventoryPlayer'},
        'GuiControls.cpp': {'Minecraft', 'GuiSmallButton'},
        'GuiCrafting.cpp': {'InventoryCraftResult', 'Minecraft', 'FontRenderer'},
        'GuiGameOver.cpp': {'Minecraft', 'GuiOptions'},
        'GuiIngameMenu.cpp': {'Minecraft', 'GuiOptions'},
        'GuiInventory.cpp': {'InventoryCraftResult', 'Minecraft', 'FontRenderer'},
        'Light.cpp': {'World', 'IWorldAccess'},
        # Types reached via inherited fields / static subclass pointers (name not textual
        # in the file, so auto-detect can't see them):
        'EntityItem.cpp': {'InventoryPlayer'},        # var->m_inventory->...
        'ItemBlock.cpp': {'BlockFire'},               # Block::s_fire is a BlockFire*
        'GuiScreen.cpp': {'RenderEngine', 'SoundManager'},  # mc->m_sndManager->...
        'EntityCreature.cpp': {'Pathfinder', 'World'},      # worldObj->pathFinder->...
        'PlayerControllerSP.cpp': {'RenderGlobal', 'InventoryPlayer'},
        'Block.cpp': {'EntityPlayer', 'InventoryPlayer'},
    }

    # Inherited-field-deref C2027s (type name not textual in the file, so the auto-detect
    # below can't see it). Extracted from build_alpha24.log; merged (not overwritten) so
    # existing INCLUDE_MAP entries for the same file keep their types.
    _C2027_FIXES = {
        'BlockFire.cpp': {'BlockLeaves', 'BlockPortal'},
        'BlockFlowing.cpp': {'WorldProvider'},
        'BlockGrass.cpp': {'WorldChunkManager'},
        'BlockLeaves.cpp': {'WorldChunkManager'},
        'ChunkCache.cpp': {'WorldProvider'},
        'EntityCow.cpp': {'InventoryPlayer'},
        'EntityPlayerSP.cpp': {'InventoryPlayer'},
        'EntityRenderer.cpp': {'WorldProvider'},
        'EntitySnowball.cpp': {'InventoryPlayer'},
        'GuiConflictWarning.cpp': {'Minecraft'},
        'GuiConnectFailed.cpp': {'Minecraft'},
        'GuiEditSign.cpp': {'Minecraft'},
        'GuiFurnace.cpp': {'FontRenderer', 'Minecraft'},
        'GuiGameOver.cpp': {'EntityPlayerSP'},
        'GuiIngameMenu.cpp': {'World'},
        'GuiSlider.cpp': {'GameSettings'},
        'ItemSign.cpp': {'Material'},
        'Render.cpp': {'GameSettings'},
        'RenderBiped.cpp': {'ModelRenderer', 'RenderManager'},
        'RenderFireball.cpp': {'RenderManager'},
        'RenderPainting.cpp': {'RenderManager'},
        'RenderSnowball.cpp': {'RenderManager'},
        'Teleporter.cpp': {'BlockPortal'},
        'TexturePortalFX.cpp': {'BlockPortal'},
        'TileEntitySignRenderer.cpp': {'ModelRenderer'},
        'WorldGenTrees.cpp': {'BlockLeaves'},
    }

    needed_types = set(INCLUDE_MAP.get(fname, set())) | _C2027_FIXES.get(fname, set())

    # Auto-detect: any class header whose COMPLETE type is used here needs an include.
    # High-confidence triggers only (pointer decl 'C*', scope 'C::', or 'new C') so we
    # don't pull headers in for incidental textual mentions.
    self_name = fname[:-4]  # strip .cpp
    # strip comments so mentions in comments don't trigger includes
    code = re.sub(r'/\*.*?\*/', ' ', content, flags=re.DOTALL)
    code = re.sub(r'//[^\n]*', ' ', code)
    for cname in AVAILABLE_HEADERS:
        if cname == self_name or cname in needed_types:
            continue
        # High-confidence triggers only (pointer decl 'C*', scope 'C::', or 'new C').
        # NOTE: a broad "any mention" trigger was tried and reverted — it added ~2085
        # includes and RAISED undefined-type errors (pulled in headers referencing other
        # incomplete types) without flipping any file. Cases where the type name isn't
        # textually present (inherited-field derefs) need type inference, not text search.
        if (re.search(r'\b%s\b\s*\*' % re.escape(cname), code)
                or re.search(r'\b%s::' % re.escape(cname), code)
                or re.search(r'\bnew\s+%s\b' % re.escape(cname), code)):
            needed_types.add(cname)

    if not needed_types:
        return content, 0

    existing = set()
    for m in re.finditer(r'#include\s+"(\w+)\.h"', content):
        existing.add(m.group(1))

    # Only add includes for headers that ACTUALLY EXIST. The hardcoded INCLUDE_MAP is
    # shared across profiles, so an indev-only entry (e.g. EntityArrow->EntityMap) would
    # otherwise inject a #include for a class alpha doesn't have -> C1083. AVAILABLE_HEADERS
    # is the set of generated .h, populated before this postfix runs.
    to_add = (needed_types - existing)
    if AVAILABLE_HEADERS:
        to_add = {t for t in to_add if t in AVAILABLE_HEADERS}
    if not to_add:
        return content, 0

    lines = content.split('\n')
    last_inc = 0
    for i, line in enumerate(lines):
        if line.startswith('#include'):
            last_inc = i

    insert_lines = ['#include "%s.h"' % t for t in sorted(to_add)]
    for j, inc_line in enumerate(insert_lines):
        lines.insert(last_inc + 1 + j, inc_line)

    return '\n'.join(lines), len(to_add)


def fix_new_dot_method(content, fname):
    """Fix: (new Type()).method(args) -> (new Type())->method(args) for reference types."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\(new\s+([A-Z]\w+)\s*\(([^)]*)\)\)\.([\w]+)',
                     r'(new \1(\2))->\3', content)
    if content != old:
        changed = content.count(')->') - old.count(')->')
    return content, max(changed, 0)


def fix_java_array_init(content, fname):
    """Fix: new Type[]{val1, val2} -> C++03 compatible array init."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'new\s+void\*\s*\[\]\s*\{([^}]*)\}',
                     r'NULL /* PORT: array init {\1} */', content)
    content = re.sub(r'new\s+String\s*\[\]\s*\{([^}]*)\}',
                     r'NULL /* PORT: string array init {\1} */', content)
    content = re.sub(r'new\s+KeyBinding\s*\[\]\s*\{([^}]*)\}',
                     r'NULL /* PORT: KeyBinding array init {\1} */', content)
    content = re.sub(r'new\s+(\w+)\s*\[\]\s*\{([^}]*)\}',
                     r'NULL /* PORT: \1 array init {\2} */', content)
    if content != old:
        changed = 1
    return content, changed


def fix_cast_ptr_to_value_local(content, fname):
    """'Type var = (X*)expr;' -> 'Type* var = (X*)expr;'.

    The decompiled source declares a value local but immediately assigns a pointer
    cast (common at the top of render/AI methods: 'EntityArrow v = (EntityArrow*)e;').
    Making the local a pointer is the correct fix; later '.'->'->' passes follow.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    pat = re.compile(r'(?m)^(\s*)([A-Z]\w+)(\s+)(\w+)(\s*=\s*\(\s*[A-Za-z_]\w*\s*\*\s*\))')
    content, n = pat.subn(r'\1\2*\3\4\5', content)
    return content, n


def fix_itemstack_array(content, fname):
    """ItemStack arrays are arrays of nullable pointers (ItemStack**).

    'new ItemStack[n]' -> 'new ItemStack*[n]()' (n NULL slots, matching Java's null-filled
    array). Element access 'arr[i].member' -> 'arr[i]->member' for the known ItemStack
    array fields, since the elements are now pointers.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    content, n = re.subn(r'\bnew\s+ItemStack\s*\[([^\]]+)\]', r'new ItemStack*[\1]()', content)
    total += n
    content, n = re.subn(r'\bnew\s+Item\s*\[([^\]]+)\]', r'new Item*[\1]()', content)
    total += n
    content, n = re.subn(r'\bnew\s+Block\s*\[([^\]]+)\]', r'new Block*[\1]()', content)
    total += n
    # array-of-pointer element access: itemsList[i]./blocksList[i]. -> ...->
    content, n = re.subn(r'\b(itemsList|blocksList)\s*\[([^\]]+)\]\.', r'\1[\2]->', content)
    total += n
    # arr[i]. -> arr[i]-> for ItemStack-array fields (m_mainInventory, m_armorInventory,
    # m_stackList, m_chestContents, etc.) — any '<ident>[<idx>].' on a known ItemStack array.
    for arrname in ('m_mainInventory', 'm_armorInventory', 'm_stackList',
                    'm_chestContents', 'm_craftingMatrix', 'm_inventoryContents',
                    'm_currentRecipe', 'mainInventory', 'stackList'):
        content, n = re.subn(r'\b%s\s*\[([^\]]+)\]\.' % re.escape(arrname),
                             r'%s[\1]->' % arrname, content)
        total += n
    return content, total


def fix_string_concat(content, fname):
    """Java string concat: '"" + expr' -> 'toStr(expr)' (C++ can't add char* + number).

    Covers the decompiler's common 'return "" + value;' toString idiom. toStr() is
    defined in Common.h with overloads for the numeric types.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    # '"" + <expr>' where expr is an identifier/member/call chain (stops at ; , ) + )
    content, total = re.subn(r'""\s*\+\s*([A-Za-z_][\w.\->()\[\]:]*)', r'toStr(\1)', content)
    # Chained concat starting with a string literal (e.g. toString:
    #   return "(" + m_xCoord + ", " + m_yCoord + ")";
    # 'const char* + std::string' chains correctly, so wrap each numeric member/var
    # operand in toStr(). Scoped to lines that BEGIN a string concat (return/= "..." +)
    # so arithmetic '+' elsewhere is untouched.
    out = []
    for line in content.split('\n'):
        # A return/assignment that is a string concat (contains a string literal joined by '+').
        if re.search(r'(?:return|=)\s*[^;]*"[^"]*"\s*\+', line) or re.search(r'\+\s*"[^"]*"\s*\+', line):
            # wrap '+ <numeric term>' operands — but NOT when followed by member access
            # ('.'/'->'), which means it's a string field (e.g. '+ var3->username'). The
            # term includes any trailing '* / %' chain (tighter than '+'), so e.g.
            # '+ var16 * 100L / var24' -> '+ toStr(var16 * 100L / var24)' (not just var16).
            line, k = re.subn(
                r'\+\s*((?:m_\w+|var\d+)(?:\s*[*/%]\s*(?:m_\w+|var\d+|\d+[Ll]?|\([^()]*\)))*)'
                r'\b(?!\s*[\w(.>\-\[])',
                r'+ toStr(\1)', line)
            total += k
            # wrap a LEADING member operand: 'return m_x +' / '= m_x +'
            line, k = re.subn(r'(\breturn |= )(m_\w+|var\d+)(\s*\+)', r'\1toStr(\2)\3', line)
            total += k
        out.append(line)
    return '\n'.join(out), total


def fix_raw_container_template(content, fname):
    """Give raw Java generics valid C++ template args: 'std::map name' -> 'std::map<std::string,int> name'.

    Raw 'HashMap'/'HashSet' convert to a bare 'std::map'/'std::set', which is a hard
    C2955 error in any header — and a broken header breaks every file that includes it.
    The value type is a best-effort default (most Indev maps are string-keyed); the goal
    is a valid header that unblocks includers. The lookahead keeps templated forms intact.
    """
    if not (fname.endswith('.h') or fname.endswith('.cpp')):
        return content, 0
    content, n1 = re.subn(r'\bstd::map\s+(?=[A-Za-z_]\w*\s*[;,)=])',
                          'std::map<std::string, int> ', content)
    content, n2 = re.subn(r'\bstd::set\s+(?=[A-Za-z_]\w*\s*[;,)=])',
                          'std::set<int> ', content)
    return content, n1 + n2


def fix_unsigned_shift(content, fname):
    """Java unsigned right shift 'a >>> b' -> '(unsigned int)(a) >> b' (C++ has no >>>).

    Preserves Java's zero-fill semantics, which matters for color/alpha extraction
    like 'argb >>> 24'. Handles a parenthesised group or an identifier chain as the
    left operand (covers the forms that actually occur in the decompiled source).
    """
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    pat = re.compile(
        r'((?:\([^()]*\))|(?:[A-Za-z_]\w*(?:(?:->|\.|::)\w+)*(?:\[[^\]]*\])?))\s*>>>\s*'
    )
    content, n = pat.subn(r'(unsigned int)(\1) >> ', content)
    return content, n


def fix_new_value_struct(content, fname):
    """Drop 'new' on value-type constructor calls: 'new Vec3D(a,b,c)' -> 'Vec3D(a,b,c)'.

    Types in PROFILE.struct_classes are value types (passed/returned by value), so a
    heap 'new T(...)' is always wrong — it yields a T* where a T is expected
    (C2440/C2664). Only the parenthesised constructor form is rewritten; the array
    form 'new T[n]' is left intact (a valid array-of-values allocation).
    """
    if not fname.endswith('.cpp'):
        return content, 0
    types = PROFILE.struct_classes
    if not types:
        return content, 0
    total = 0
    for t in sorted(types, key=len, reverse=True):
        pat = re.compile(r'\bnew\s+(%s)\s*\(' % re.escape(t))
        content, n = pat.subn(r'\1(', content)
        total += n
    return content, total


def fix_java_array_decl(content, fname):
    """Fix Java array declarations 'TYPE[] name' -> 'TYPE* name'.

    Java array locals/params/returns survive conversion as 'unsigned char[] var',
    'int[] var', 'std::string[] var', 'Block[] var', etc. — all illegal in C++.
    Empty brackets '[]' only ever appear in these declaration forms; a real C++
    allocation 'new T[n]' always has a size, so matching '[]' is safe and targeted.
    """
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    pat = re.compile(
        r'\b('
        r'unsigned char|signed char|unsigned int|unsigned short|unsigned long|'
        r'char|int|short|long long|long|float|double|bool|void|std::string|'
        r'[A-Z]\w*'
        r')\s*\[\]\s+(?=[A-Za-z_])'
    )
    content, n = pat.subn(r'\1* ', content)
    return content, n


def fix_broken_port_valueof(content, fname):
    """Fix: Character./* PORT: valueOf('X') */ -> 'X'."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r"(?:Character|char)\.\s*/\*\s*PORT:\s*valueOf\('([^']*)'\)\s*\*/", r"'\1'", content)
    content = re.sub(r"(?:Character|char)\.\s*/\*\s*PORT:\s*valueOf\(([^)]*)\)\s*\*/", r'\1', content)
    content = re.sub(r"(?:Integer|Float|Short)\.\s*/\*\s*PORT:\s*valueOf\(([^)]*)\)\s*\*/", r'\1', content)
    if content != old:
        changed = 1
    return content, changed


def fix_block_dot_static(content, fname):
    """Fix remaining Block.field -> Block::s_field and Item.field -> Item::s_field patterns."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    for field in PF_BLOCK_STATICS:
        pat = r'\bBlock\.%s\b' % re.escape(field)
        repl = 'Block::s_%s' % field
        content = re.sub(pat, repl, content)
    for field in PF_ITEM_STATICS:
        pat = r'\bItem\.%s\b' % re.escape(field)
        repl = 'Item::s_%s' % field
        content = re.sub(pat, repl, content)
    # Material/Block/Item .<field> -> ::s_<field> (statics are s_-prefixed). These class
    # names never name a local, so 'Class.x' is always static access. Lookahead skips '.h'.
    content = re.sub(r'\bMaterial\.(?!h\b)(\w+)', r'Material::s_\1', content)
    content = re.sub(r'\bBlock\.(?!h\b)(\w+)', r'Block::s_\1', content)
    content = re.sub(r'\bItem\.(?!h\b)(\w+)', r'Item::s_\1', content)
    # Static pointers (Block*/Item*/Material* s_X) reached with '.' need '->':
    # 'Block::s_clothDarkGray.blockID' -> 'Block::s_clothDarkGray->blockID'.
    content = re.sub(r'\b(Block|Item|Material)::s_(\w+)\.', r'\1::s_\2->', content)
    if content != old:
        changed = 1
    return content, changed


def fix_character_valueof(content, fname):
    """Fix: Character.valueOf('X') and char.valueOf('X') -> 'X'"""
    changed = 0
    old = content
    content = re.sub(r"Character\.valueOf\(([^)]*)\)", r'\1', content)
    content = re.sub(r"char\.valueOf\(([^)]*)\)", r'\1', content)
    content = re.sub(r"Integer\.valueOf\(([^)]*)\)", r'\1', content)
    content = re.sub(r"Float\.valueOf\(([^)]*)\)", r'\1', content)
    content = re.sub(r"Short\.valueOf\(([^)]*)\)", r'\1', content)
    content = re.sub(r'(?<![/\\])\bchar\.(?!h\b|png\b|class\b|gif\b|jpg\b)', '/* PORT: char. */ ', content)
    if content != old:
        changed = 1
    return content, changed


def fix_string_in_body(content, fname):
    """Fix: String/StringBuilder/Object/List as type names in .cpp body code."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        if line.strip().startswith('//') or line.strip().startswith('#'):
            result.append(line)
            continue
        old = line
        line = re.sub(r'^(\s+)String\s+(\w+)', lambda m: m.group(0).replace('String', 'std::string'), line)
        line = re.sub(r'^(\s+)StringBuilder\s+(\w+)', lambda m: m.group(0).replace('StringBuilder', 'std::string'), line)
        line = re.sub(r'^(\s+)Object\s+(\w+)', lambda m: m.group(0).replace('Object', 'void*'), line)
        line = re.sub(r'^(\s+)List\s+(\w+)', lambda m: m.group(0).replace('List', 'std::vector<void*>'), line)
        if line != old:
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_this_assignment(content, fname):
    """Fix: Type var; ... var = this -> Type* var; ... (use -> for var)."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')

    this_vars = {}
    for i, line in enumerate(lines):
        m = re.match(r'^(\s+)([A-Z]\w+)\s+(\w+)\s*;\s*$', line)
        if m:
            vname = m.group(3)
            for j in range(i + 1, min(i + 20, len(lines))):
                if re.match(r'\s+%s\s*=\s*this\s*;' % re.escape(vname), lines[j]):
                    this_vars[vname] = (i, m.group(2))
                    break

    if not this_vars:
        return content, 0

    result = []
    for i, line in enumerate(lines):
        fl = line
        for vname, (decl_idx, type_name) in this_vars.items():
            if i == decl_idx:
                fl = re.sub(r'^(\s+)%s\s+%s\s*;' % (re.escape(type_name), re.escape(vname)),
                           r'\g<1>%s* %s;' % (type_name, vname), fl)
                if fl != line:
                    changed += 1
            else:
                if '= this' not in fl:
                    old = fl
                    fl = re.sub(r'\b%s\.' % re.escape(vname), '%s->' % vname, fl)
                    if fl != old:
                        changed += 1
        result.append(fl)
    return '\n'.join(result), changed


def fix_math_references(content, fname):
    """Fix: Math.PI -> M_PI, Math.abs -> fabsf, Math.atan -> atanf, etc."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\(float\)Math\.PI\b', '(float)M_PI', content)
    content = re.sub(r'\bMath\.PI\b', 'M_PI', content)
    content = re.sub(r'\bMath\.abs\b', 'fabsf', content)
    content = re.sub(r'\bMath\.atan\b', 'atanf', content)
    content = re.sub(r'\bMath\.atan2\b', 'atan2f', content)
    content = re.sub(r'\bMath\.sin\b', 'sinf', content)
    content = re.sub(r'\bMath\.cos\b', 'cosf', content)
    content = re.sub(r'\bMath\.sqrt\b', 'sqrtf', content)
    content = re.sub(r'\bMath\.floor\b', 'floorf', content)
    content = re.sub(r'\bMath\.ceil\b', 'ceilf', content)
    content = re.sub(r'\bMath\.min\b', 'min', content)
    content = re.sub(r'\bMath\.max\b', 'max', content)
    content = re.sub(r'\bMath\.pow\b', 'powf', content)
    # Java allows '%' on floats (modulo); C++ does not -> fmodf. Target a float-literal
    # right operand; left operand is a simple var/member (NO parens, so 'if(' isn't eaten).
    content = re.sub(r'([\w.>\[\]]+)\s*%\s*(\d+\.\d+[Ff]?)', r'fmodf(\1, \2)', content)
    content = re.sub(r'\bMath\.random\(\)', '((float)::rand() / (float)RAND_MAX)', content)
    # Bare 'rand()' collides with the inherited Entity 'rand' field (a Random), which
    # makes 'rand()' parse as a call on the member (C2064). Force the C library function.
    content = re.sub(r'(?<![:.\w>])rand\(\)', '::rand()', content)
    if content != old:
        changed = 1
    return content, changed


def fix_instanceof(content, fname):
    """Fix: instanceof expressions -> PORT-BLOCKED comments."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        old = line
        if 'instanceof' in line and '// PORT' not in line:
            # Wrap the entire if/while line in a PORT-BLOCKED comment
            stripped = line.lstrip()
            indent = line[:len(line) - len(stripped)]
            line = '%s// PORT-BLOCKED: %s' % (indent, stripped)
        if line != old:
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_rendermanager_dot(content, fname):
    """Fix: RenderManager.instance -> RenderManager::instance, also fix s_instance leftovers."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\bRenderManager\.instance\b', 'RenderManager::instance', content)
    content = re.sub(r'\bRenderManager::s_instance\b', 'RenderManager::instance', content)
    if content != old:
        changed = 1
    return content, changed


def _balanced_paren(s, open_idx):
    """Given s[open_idx] == '(', return (inner_text, index_of_matching_close) or (None, -1)."""
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == '(':
            depth += 1
        elif s[i] == ')':
            depth -= 1
            if depth == 0:
                return s[open_idx + 1:i], i
    return None, -1


def _extract_receiver(s, dot_idx):
    """Walk left from s[dot_idx] (a '.' starting '.remove(') to the start of the receiver
    postfix-expression: an identifier chain with '->' / '.' / '::' and balanced '[...]'
    subscripts. Stops at a leading cast/paren-group or any operator (so '(int*)a->b.remove'
    yields 'a->b', not the cast)."""
    i = dot_idx - 1
    bdepth = 0
    while i >= 0:
        ch = s[i]
        if ch == ']':
            bdepth += 1
        elif ch == '[':
            if bdepth == 0:
                break
            bdepth -= 1
        elif bdepth > 0:
            pass  # inside a subscript: consume anything
        elif ch.isalnum() or ch in '_.':
            pass
        elif ch == '>' and i - 1 >= 0 and s[i - 1] == '-':
            pass
        elif ch == '-' and i + 1 < len(s) and s[i + 1] == '>':
            pass
        elif ch == ':':
            pass
        else:
            break
        i -= 1
    return i + 1


def fix_vector_remove(content, fname):
    """Java List.remove(...) -> _vecRemoveAt/_vecRemoveObj(receiver, expr) (Common.h helpers).

    Substring replacement (NOT whole-line) so a surrounding return/ternary/assignment/cast
    survives — the old whole-line rewrite destroyed e.g. 'return cond ? (int[])l.remove(n) : ...'.
    The receiver is extracted left-balanced (handles 'a->b' and 'm_grid[idx]'); the arg is
    paren-balanced so 'remove(list.size() - 1)' keeps its tail."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    out = []
    for line in content.split('\n'):
        if '.remove(' not in line:
            out.append(line)
            continue
        i = 0
        newline = ''
        while True:
            idx = line.find('.remove(', i)
            if idx == -1:
                newline += line[i:]
                break
            if '//' in line[i:idx] or '/* PORT' in line[i:idx]:
                newline += line[i:]
                break
            rstart = _extract_receiver(line, idx)
            receiver = line[rstart:idx]
            open_idx = idx + len('.remove(') - 1  # position of '('
            inner, close_idx = _balanced_paren(line, open_idx)
            if not receiver or inner is None:
                newline += line[i:idx + len('.remove(')]
                i = idx + len('.remove(')
                continue
            newline += line[i:rstart]
            expr = inner.strip()
            # Surviving '.remove(' calls here are index-removes (object removes were already
            # lowered to find/erase). Pick by shape: numeric / .size()-based / inc-dec / bare
            # loop var -> index; otherwise object.
            index_like = (bool(re.match(r'^-?\d+$', expr)) or '.size()' in expr or
                          bool(re.search(r'(\+\+|--)', expr)) or
                          bool(re.match(r'^var\d+$', expr)))
            helper = '_vecRemoveAt' if index_like else '_vecRemoveObj'
            newline += '%s(%s, %s)' % (helper, receiver, expr)
            changed += 1
            i = close_idx + 1
        out.append(newline)
    return '\n'.join(out), changed


def fix_tochararray(content, fname):
    """Fix: str.toCharArray() -> str.c_str()."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\.toCharArray\(\)', '.c_str()', content)
    if content != old:
        changed = 1
    return content, changed


def fix_buffered_io(content, fname):
    """Fix: BufferedReader/PrintWriter Java I/O -> C++ fstream PORT comments."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        old = line
        # BufferedReader var1 = new BufferedReader(new FileReader(...));
        if 'BufferedReader' in line and 'new BufferedReader' in line and '// PORT' not in line:
            stripped = line.lstrip()
            indent = line[:len(line) - len(stripped)]
            # Extract variable name
            m = re.search(r'(\w+)\s*=\s*new\s+BufferedReader', line)
            if m:
                vname = m.group(1)
                line = '%s// PORT-TODO: replace Java I/O — BufferedReader %s' % (indent, vname)
        elif 'PrintWriter' in line and 'new PrintWriter' in line and '// PORT' not in line:
            stripped = line.lstrip()
            indent = line[:len(line) - len(stripped)]
            m = re.search(r'(\w+)\s*=\s*new\s+PrintWriter', line)
            if m:
                vname = m.group(1)
                line = '%s// PORT-TODO: replace Java I/O — PrintWriter %s' % (indent, vname)
        # var1->readLine() / var1->println(...) / var1->close() on these Java I/O vars
        elif re.search(r'\w+->(readLine|println|close)\b', line) and '// PORT' not in line:
            stripped = line.lstrip()
            indent = line[:len(line) - len(stripped)]
            line = '%s// PORT-TODO: %s' % (indent, stripped)
        if line != old:
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_label_gotos(content, fname):
    """Java labeled break/continue -> C++ goto. The label sits BEFORE the loop (as in the Java source),
    so a bare `goto label` RESTARTS the loop. That is correct ONLY for `continue label` on a
    while(true)/for(;;). It is WRONG -- and causes an INFINITE LOOP / hang -- for:
      * `break label`  : must EXIT the loop (the label must be placed AFTER it). Bug seen: LevelGenerator
                         erode "stuck on Eroding", Frustrum::isBoundingBoxInFrustrum (every frame!).
      * `continue label` on a do{}while(cond): must jump to the CONDITION, not the top -- else it spins
                         forever once the condition would have ended the loop. Bug seen: the shore-water
                         World::fluidFlowCheck FREEZE (frame frozen, audio kept playing).
    A correct generic rewrite needs control-flow analysis the line-based converter doesn't do, so we
    emit the goto (keeps it compiling) AND a loud PORT-WARNING so each site is manually verified.
    See the 'console-boot-crash' note (continue-on-do-while) and the labeled-break notes."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    WARN = ('  // PORT-WARNING(label goto): VERIFY semantics -- `break label` must EXIT (label AFTER the '
            'loop, else infinite loop); `continue label` on a do{}while must jump to the CONDITION.')
    for line in lines:
        old = line
        # break labelNN; -> goto labelNN;
        line = re.sub(r'\bbreak\s+(label\d+)\s*;', r'goto \1;', line)
        # continue labelNN; -> goto labelNN;
        line = re.sub(r'\bcontinue\s+(label\d+)\s*;', r'goto \1;', line)
        if line != old:
            changed += 1
            if 'PORT-WARNING' not in line:
                line = line + WARN
        result.append(line)
    return '\n'.join(result), changed


def fix_dot_on_pointer_members(content, fname):
    """Fix member accesses that use . but the member is a pointer type.
    CONSERVATIVE: only includes members unlikely to collide with local variable names."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content

    # Only members that are unambiguously this->member pointer accesses
    # Excluded: m_inventory, m_random, rand, material, stepSound, currentScreen,
    #           m_slot, m_entityMap (these collide with local vars or are accessed on non-this objects)
    PTR_MEMBERS = [
        # Entity/World - always class members
        'worldObj', 'theWorld',
        # Minecraft core
        'mc', 'm_options', 'm_gameSettings', 'm_entityRenderer',
        'm_effectRenderer', 'm_renderEngine', 'fontRenderer',
        'm_loadingScreen', 'm_guiLoading',
        # Model parts (ModelBiped/ModelQuadruped) - always pointer members
        'm_a', 'm_b', 'm_c', 'm_d', 'm_e', 'm_f', 'm_g',
        'head', 'body', 'leg1', 'leg2', 'leg3', 'leg4',
    ]
    for mem in PTR_MEMBERS:
        content = re.sub(r'\b%s\.(?!cpp|h|class\b)' % re.escape(mem), '%s->' % mem, content)

    # Static pointer: RenderManager::instance.x -> RenderManager::instance->x
    content = re.sub(r'\bRenderManager::instance\.', 'RenderManager::instance->', content)

    if content != old:
        changed = 1
    return content, changed


def fix_3_0f_in_strings(content, fname):
    """Fix: '3.0f' appearing in string literals from decompiler artifact.
    e.g., '3.0f m_anaglyph' -> '3D Anaglyph'."""
    changed = 0
    old = content
    # "3.0f m_anaglyph" in the option string
    content = content.replace('"3.0f m_anaglyph: "', '"3D Anaglyph: "')
    # anaglyph3.0f -> anaglyph3d
    content = re.sub(r'anaglyph3\.0f', 'anaglyph3d', content)
    if content != old:
        changed = 1
    return content, changed


def fix_entity_null_cast(content, fname):
    """Fix: attackEntityFrom((Entity*)NULL, n) -> attackEntityFrom(NULL, n)
    The cast to pointer of NULL is redundant and causes issues."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # '(Type)NULL' / '(Type*)NULL' -> NULL (casting NULL to any class type is just NULL;
    # value casts like '(ItemStack)NULL' are otherwise illegal for nullable reference types)
    content = re.sub(r'\(\s*[A-Z][\w:]*\s*\*?\s*\)\s*NULL\b', 'NULL', content)
    if content != old:
        changed = 1
    return content, changed


def fix_ptr_local_dot_to_arrow(content, fname):
    """Comprehensive fix: scan for Type* varname declarations, convert varname. -> varname->
    This is the critical follow-up to ptr-to-value-locals which added * but didn't fix access.
    Now scope-aware: tracks ptr vars per function body and excludes value-type vars."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')

    # Process function by function
    # Track brace depth to know scope boundaries
    result = []
    ptr_vars = set()  # currently known pointer vars in this scope
    non_ptr_vars = set()  # known non-pointer vars (value types) in this scope
    brace_depth = 0
    func_start_depth = 0
    in_func = False

    for line in lines:
        # Track brace depth
        open_b = line.count('{')
        close_b = line.count('}')

        # Detect function definition start
        if not in_func and re.match(r'[\w:*&\s]+\s+\w+::\w+\s*\(', line):
            in_func = True
            func_start_depth = brace_depth
            ptr_vars = set()
            non_ptr_vars = set()
            # Extract pointer params from signature
            params = re.findall(r'(\w+)\*\s+(\w+)', line)
            for _, pname in params:
                ptr_vars.add(pname)

        if in_func:
            # Detect pointer-typed local declarations: Type* varname
            m = re.match(r'\s+([A-Z]\w+)\*\s+(\w+)\s*[;=]', line)
            if m:
                ptr_vars.add(m.group(2))

            # Detect non-pointer local declarations: Type varname (value types)
            m2 = re.match(r'\s+([A-Z]\w+)\s+(\w+)\s*[;=]', line)
            if m2 and '*' not in line.split('=')[0].split(m2.group(2))[0]:
                non_ptr_vars.add(m2.group(2))

        brace_depth += open_b - close_b

        # End of function
        if in_func and brace_depth <= func_start_depth and close_b > 0:
            in_func = False
            # Don't clear yet - process this line first

        # Apply fixes
        fl = line
        stripped = fl.lstrip()
        if in_func and not stripped.startswith('//') and not stripped.startswith('#') and not stripped.startswith('/*'):
            # Only convert vars that are in ptr_vars AND NOT in non_ptr_vars
            active_ptrs = ptr_vars - non_ptr_vars
            for vname in active_ptrs:
                # NOTE: do not skip lines that already contain 'vname->' — a single
                # line often mixes both (e.g. 'var16.hurtTime > 0 || var16->deathTime'),
                # and the substitution below only rewrites 'vname.' (never 'vname->'),
                # so it is safe to run regardless. The old '-> not in fl' guard left
                # the '.' occurrences on such mixed lines unconverted.
                if vname + '.' in fl:
                    # Don't modify declaration lines
                    if re.match(r'\s+\w+\*?\s+%s\s*[;=]' % re.escape(vname), fl):
                        continue
                    old = fl
                    fl = re.sub(r'\b%s\.(?!cpp|h|py)' % re.escape(vname), '%s->' % vname, fl)
                    if fl != old:
                        changed += 1

        result.append(fl)

    return '\n'.join(result), changed


def fix_ptr_param_dot_to_arrow(content, fname):
    """Fix function parameters that are pointer types accessed with . instead of ->
    e.g., EntityArrow(World* var1, EntityLiving* var2) then var2.posX -> var2->posX
    Also handles constructors with initializer lists."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')

    # Find function/constructor definitions and extract pointer parameters
    brace_depth = 0
    current_params = set()
    in_func = False

    for i, line in enumerate(lines):
        # Detect function/constructor definition (including multi-line with init lists)
        m = re.match(r'(?:[\w:*&\s]+\s+)?(\w+::)+\w+\s*\(([^)]*)\)', line)
        if m:
            params_str = m.group(2)
            new_params = set()
            for pm in re.finditer(r'(\w+)\*\s+(\w+)', params_str):
                new_params.add(pm.group(2))
            if new_params:
                current_params = new_params
                in_func = True
                brace_depth = line.count('{') - line.count('}')
                continue

        if in_func:
            brace_depth += line.count('{') - line.count('}')
            if brace_depth <= 0 and '}' in line:
                in_func = False
                current_params = set()
                continue

            if current_params:
                for vname in current_params:
                    # See fix_ptr_local_dot_to_arrow: don't skip mixed lines that also
                    # contain 'vname->'; the sub only rewrites 'vname.' so it is safe.
                    if vname + '.' in line:
                        stripped = line.lstrip()
                        if stripped.startswith('//') or stripped.startswith('#'):
                            continue
                        old = line
                        line = re.sub(r'\b%s\.(?!cpp|h)' % re.escape(vname), '%s->' % vname, line)
                        if line != old:
                            changed += 1
                lines[i] = line

    return '\n'.join(lines), changed


def fix_positiontexturevertex_this(content, fname):
    """PositionTextureVertex's remap ctor takes a PositionTextureVertex by value, but the
    call passes 'this' (a pointer) — dereference it."""
    if fname != 'PositionTextureVertex.cpp':
        return content, 0
    new, n = re.subn(r'PositionTextureVertex\(this,', 'PositionTextureVertex(*this,', content)
    return new, n


def fix_tesselator_value_arg(content, fname):
    """A Tesselator value (from 'Tesselator v = Tesselator::getInstance()') passed to a
    Tesselator* parameter (renderQuad/renderParticle) — pass its address."""
    if not fname.endswith('.cpp'):
        return content, 0
    tvars = set(re.findall(r'Tesselator\s+(\w+)\s*=\s*Tesselator::getInstance\(\)', content))
    if not tvars:
        return content, 0
    n = 0
    for v in tvars:
        content, c = re.subn(r'(renderQuad|renderParticle)\(\s*%s\s*,' % re.escape(v),
                             r'\1(&%s,' % v, content)
        n += c
    return content, n


def fix_double_suffix(content, fname):
    """Java double-literal suffix: '0.0D' / '100.0D' -> '0.0' (C++ has no 'D' suffix; the
    default for a fractional literal is double anyway)."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'(\b\d+\.\d+)[Dd]\b', r'\1', content)
    return new, n


def fix_effectrenderer_fxlayers(content, fname):
    """EffectRenderer::m_fxLayers is an array of EntityFX* layers (new vector<EntityFX*>[3]),
    not vector<void*>[]."""
    if fname != 'EffectRenderer.h':
        return content, 0
    new = content.replace('std::vector<void*>* /* PORT: determine element type */ m_fxLayers',
                          'std::vector<EntityFX*>* m_fxLayers')
    return new, (1 if new != content else 0)


def fix_guiingame_chatlist(content, fname):
    """GuiIngame::m_chatMessageList holds ChatLine* (m_chatMessageList[i]->m_updateCounter),
    not void*. Retype + forward-declare ChatLine (.h) and include it (.cpp)."""
    if fname == 'GuiIngame.h':
        new = content.replace('std::vector<void*> /* PORT: determine element type */ m_chatMessageList',
                              'std::vector<ChatLine*> m_chatMessageList')
        if new != content and 'class ChatLine;' not in new:
            new = new.replace('class Minecraft;', 'class ChatLine;\nclass Minecraft;', 1)
        return new, (1 if new != content else 0)
    if fname == 'GuiIngame.cpp':
        if 'm_chatMessageList' in content and '#include "ChatLine.h"' not in content and 'ChatLine' in AVAILABLE_HEADERS:
            return content.replace('#include "Common.h"\n',
                                   '#include "Common.h"\n#include "ChatLine.h"\n', 1), 1
        return content, 0
    return content, 0


def fix_guiingame_includes(content, fname):
    """GuiIngame dereferences PlayerController* and RenderGlobal* (->shouldDrawHUD etc.) but
    only forward-declares them; add the headers for the complete types."""
    if fname != 'GuiIngame.cpp':
        return content, 0
    n = 0
    for hdr in ('PlayerController', 'RenderGlobal'):
        if ('#include "%s.h"' % hdr) not in content and hdr in AVAILABLE_HEADERS:
            content = content.replace('#include "Common.h"\n',
                                      '#include "Common.h"\n#include "%s.h"\n' % hdr, 1)
            n += 1
    return content, n


def fix_vector3d_mangle(content, fname):
    """The identifier 'vector3D' was mis-read as 'vector3.0f' (the 'D' suffix taken as a
    Java double literal). Restore it to the m_vector3D field (do the m_-prefixed form first
    so the bare-form replace doesn't double-prefix)."""
    if 'vector3.0f' not in content and 'm_vector3D.' not in content:
        return content, 0
    n = content.count('vector3.0f')
    content = content.replace('m_vector3.0f', 'm_vector3D')
    content = content.replace('vector3.0f', 'm_vector3D')
    # m_vector3D is a Vec3D* field -> member access uses '->' (the dot-arrow passes ran
    # before the unmangle so missed it). 'm_vector3D = ...' has a space, not '.', so safe.
    content, c = re.subn(r'\bm_vector3D\.', 'm_vector3D->', content)
    n += c
    return content, n


def fix_stringbuilder(content, fname):
    """Java StringBuilder -> std::string concat: '(new std::string()).append(a).append(b)'
    -> 'std::string() + toStr(a) + toStr(b)'. toStr has overloads for str/char*/numeric,
    so mixed-type appends just work. (Only chained/expression appends occur — verified no
    standalone in-place '.append(...)' statements that this would turn into no-ops.)"""
    if not fname.endswith('.cpp') or '.append(' not in content:
        return content, 0
    n = 0
    content, c = re.subn(r'\(new std::string\(\)\)', 'std::string()', content); n += c
    content, c = re.subn(r'\.append\(([^()]*(?:\([^()]*\))?[^()]*)\)', r' + toStr(\1)', content); n += c
    return content, n


def fix_nbttaglist_type(content, fname):
    """NBTTagList::m_tagList holds NBTBase* (createTagOfType/push_back), not void*."""
    if fname != 'NBTTagList.h':
        return content, 0
    new = content.replace('std::vector<void*> /* PORT: determine element type */ m_tagList',
                          'std::vector<NBTBase*> m_tagList')
    return new, (1 if new != content else 0)


def fix_isnan(content, fname):
    """Java Float.isNaN(x)/Double.isNaN(x) -> (x != x). NaN is the only value that
    compares unequal to itself, so this is the standard C++03 NaN test (no <cmath>
    isnan needed). Arg is matched without nested parens (simple member/var expr)."""
    if not fname.endswith('.cpp'):
        return content, 0
    count = [0]
    def repl(m):
        count[0] += 1
        e = m.group(1).strip()
        return '((%s) != (%s))' % (e, e)
    content = re.sub(r'\b(?:Float|Double)\.isNaN\(([^()]*)\)', repl, content)
    return content, count[0]


def fix_ptr_local_dot_to_arrow_wholefile(content, fname):
    """Robust supplement to the scope-aware dot->arrow pass (whose per-function brace
    tracking misses some pointer locals, e.g. on lines mixing 'v.x' and 'v->y').

    Decompiled locals reuse the same name with the same type, so a name declared
    'Type* name' and NEVER as a value 'Type name' anywhere in the file is
    unambiguously a pointer file-wide -> rewrite 'name.member' to 'name->member'.
    Primitive (lowercase-typed) locals can't take member access, so are irrelevant;
    value-typed (capital-typed) locals are excluded, so this won't touch them."""
    if not fname.endswith('.cpp'):
        return content, 0
    ptr_names = set(re.findall(r'\b[A-Z]\w+\s*\*\s*(\w+)\s*[;=]', content))
    val_names = set(re.findall(r'\b[A-Z]\w+\s+(\w+)\s*[;=]', content))  # value decls (no '*')
    targets = ptr_names - val_names
    if not targets:
        return content, 0
    count = [0]
    out = []
    for line in content.split('\n'):
        s = line.lstrip()
        if s.startswith(('//', '#', '/*', '*')):
            out.append(line)
            continue
        for nm in targets:
            if nm + '.' in line:
                # skip this name's own declaration line
                if re.match(r'\s*[A-Za-z_][\w:]*\s*\*?\s*%s\s*[;=]' % re.escape(nm), line):
                    continue
                new = re.sub(r'\b%s\.(?!\d)' % re.escape(nm), nm + '->', line)
                if new != line:
                    count[0] += 1
                    line = new
        out.append(line)
    return '\n'.join(out), count[0]


def _strip_for_braces(line):
    """Return line with string/char literals and comments removed, so '{'/'}' counting
    for scope tracking isn't fooled by braces inside e.g. /* PORT: {"###"...} */."""
    out = []
    i, n = 0, len(line)
    while i < n:
        c = line[i]
        if c == '/' and i + 1 < n and line[i + 1] == '/':
            break
        if c == '/' and i + 1 < n and line[i + 1] == '*':
            j = line.find('*/', i + 2)
            if j == -1:
                break
            i = j + 2
            continue
        if c == '"' or c == "'":
            q = c
            j = i + 1
            while j < n and line[j] != q:
                if line[j] == '\\':
                    j += 1
                j += 1
            i = j + 1
            continue
        out.append(c)
        i += 1
    return ''.join(out)


def _iter_functions(lines):
    """Yield (start_idx, end_idx) for each top-level function body (definition header
    line through its matching closing brace). Brace depth is computed on literal-stripped
    lines for accuracy."""
    depth = 0
    func_start = None
    seen_open = False
    for i, line in enumerate(lines):
        if func_start is None and depth == 0:
            if re.search(r'\b\w+::~?\w+\s*\(', line) and not line.rstrip().endswith(';'):
                func_start = i
                seen_open = False
        s = _strip_for_braces(line)
        depth += s.count('{') - s.count('}')
        if depth < 0:
            depth = 0
        if func_start is not None:
            if depth > 0:
                seen_open = True
            elif seen_open and depth == 0:
                yield (func_start, i)
                func_start = None
                seen_open = False


def fix_per_function_value_access(content, fname):
    """Per-function, scope-accurate fixes the whole-file heuristics can't do safely
    (decompiled names like var1 are reused with different types across methods). Types
    are read from each function's OWN signature + local declarations:
      - AxisAlignedBB value params/locals accessed with '->' -> '.'  (C2819 / C2232)
      - std::string value params/locals compared to NULL -> .empty() (C2678)
    Only affects files that already contain the erroring pattern (a passing file can't,
    or it would be failing), so regression risk is minimal."""
    if not fname.endswith('.cpp'):
        return content, 0
    lines = content.split('\n')
    total = 0
    for (a, b) in list(_iter_functions(lines)):
        ptr_names, aabb_vals, str_vals, vec_vals, val_names, aabb_vec = set(), set(), set(), set(), set(), set()
        for li in range(a, b + 1):
            ln = lines[li]
            for nm in re.findall(r'\b[A-Za-z_]\w*\s*\*\s*(\w+)\s*[;=,)]', ln):
                ptr_names.add(nm)
            for nm in re.findall(r'\b[A-Z]\w+\s+(\w+)\s*[;=,)]', ln):  # value decls (no '*')
                val_names.add(nm)
            for nm in re.findall(r'\bstd::vector<AxisAlignedBB>\s+(\w+)', ln):  # value-element AABB list
                aabb_vec.add(nm)
            for nm in re.findall(r'\bAxisAlignedBB\s+(\w+)\s*[;=,)]', ln):
                aabb_vals.add(nm)
            for nm in re.findall(r'\bstd::string\s+(\w+)\s*[;=,)]', ln):
                str_vals.add(nm)
            for nm in re.findall(r'\bstd::vector<[^>]*>\s+(\w+)\s*(?:/\*.*?\*/\s*)?[;=]', ln):
                vec_vals.add(nm)
        aabb_vals -= ptr_names
        str_vals -= ptr_names
        vec_vals -= ptr_names
        # General pointer-locals: a name declared 'Type* nm' and never a value 'Type nm'
        # in this function -> rewrite 'nm.member' to 'nm->member' (the scoped, safe form
        # of dot->arrow that the whole-file pass can't do due to cross-function collisions).
        general_ptrs = ptr_names - val_names
        if not (aabb_vals or str_vals or vec_vals or general_ptrs or aabb_vec):
            continue
        for li in range(a, b + 1):
            ln = lines[li]
            orig = ln
            for nm in aabb_vals:
                # value AABB: '->' -> '.', and Java null-checks (nullable-in-Java) -> const
                ln = re.sub(r'\b%s\s*!=\s*NULL\b' % re.escape(nm), 'true', ln)
                ln = re.sub(r'\b%s\s*==\s*NULL\b' % re.escape(nm), 'false', ln)
                ln = re.sub(r'\b%s->' % re.escape(nm), nm + '.', ln)
            for nm in str_vals:
                ln = re.sub(r'\b%s\s*!=\s*NULL\b' % re.escape(nm), '!%s.empty()' % nm, ln)
                ln = re.sub(r'\b%s\s*==\s*NULL\b' % re.escape(nm), '%s.empty()' % nm, ln)
            for nm in vec_vals:
                # Java 'list != null' / '== null' — a std::vector value is never null;
                # and a value vector accessed with '->' should use '.'.
                ln = re.sub(r'\b%s\s*!=\s*NULL\b' % re.escape(nm), 'true', ln)
                ln = re.sub(r'\b%s\s*==\s*NULL\b' % re.escape(nm), 'false', ln)
                ln = re.sub(r'\b%s->' % re.escape(nm), nm + '.', ln)
            for nm in general_ptrs:
                # 'nm.member' -> 'nm->member' (regex won't touch 'nm =' / 'nm(' decls).
                ln = re.sub(r'\b%s\.(?!cpp\b|h\b|[0-9])' % re.escape(nm), nm + '->', ln)
            for nm in aabb_vec:
                # vector<AxisAlignedBB> holds values -> element access 'v[i]->' is 'v[i].'
                ln = re.sub(r'\b%s(\[[^\]]*\])->' % re.escape(nm), r'%s\1.' % nm, ln)
            if ln != orig:
                total += 1
                lines[li] = ln
    return '\n'.join(lines), total


def fix_integer_parseint(content, fname):
    """Fix: Integer.parseInt(x) -> atoi(x.c_str())"""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'Integer\.parseInt\(([^)]+)\)', r'atoi(\1.c_str())', content)
    content = re.sub(r'Integer\.toHexString\(([^)]+)\)', r'/* PORT: toHexString(\1) */ ""', content)
    if content != old:
        changed = 1
    return content, changed


def fix_m_pi_define(content, fname):
    """Add M_PI define to files that use it. Use direct define for Xbox 360 MSVC compat."""
    if not fname.endswith('.cpp'):
        return content, 0
    if 'M_PI' not in content:
        return content, 0
    if '#define M_PI' in content:
        return content, 0

    # Add direct M_PI define at top, after stdafx include
    lines = content.split('\n')
    for i, line in enumerate(lines):
        if '#include "stdafx.h"' in line:
            lines.insert(i + 1, '#ifndef M_PI')
            lines.insert(i + 2, '#define M_PI 3.14159265358979323846')
            lines.insert(i + 3, '#endif')
            return '\n'.join(lines), 1
    # Fallback: before first #include
    for i, line in enumerate(lines):
        if line.startswith('#include'):
            lines.insert(i, '#ifndef M_PI')
            lines.insert(i + 1, '#define M_PI 3.14159265358979323846')
            lines.insert(i + 2, '#endif')
            return '\n'.join(lines), 1
    return content, 0


def fix_dot_class(content, fname):
    """Fix: TypeName.class -> PORT-BLOCKED (Java reflection, no C++ equivalent)."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        old = line
        if '.class' in line and '// PORT' not in line:
            # Check it's a Java .class reference (TypeName.class)
            if re.search(r'\b[A-Z]\w+\.class\b', line):
                stripped = line.lstrip()
                indent = line[:len(line) - len(stripped)]
                line = '%s// PORT-BLOCKED: %s' % (indent, stripped)
        if line != old:
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_broken_port_line(content, fname):
    """Fix: mc->// PORT: ... -> // PORT-TODO: ... (broken converter output causing cascades)."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # Pattern: someExpr->// PORT: ... or someExpr.// PORT: ...
    content = re.sub(r'\w+->\s*//\s*PORT:', '// PORT-TODO:', content)
    content = re.sub(r'\w+\.\s*//\s*PORT:', '// PORT-TODO:', content)
    if content != old:
        changed = 1
    return content, changed


def fix_nbt_short_access(content, fname):
    """Fix: ((NBTTagShort*)expr).shortValue -> static_cast<NBTTagShort*>(expr)->m_shortValue"""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # ((NBTTagShort*)varN.tagAt(N)).shortValue
    content = re.sub(
        r'\(\(NBTTagShort\*\)(\w+)\.tagAt\((\d+)\)\)\.shortValue',
        r'static_cast<NBTTagShort*>(\1->tagAt(\2))->m_shortValue',
        content)
    content = re.sub(
        r'\(\(NBTTagShort\*\)(\w+)->tagAt\((\d+)\)\)\.shortValue',
        r'static_cast<NBTTagShort*>(\1->tagAt(\2))->m_shortValue',
        content)
    if content != old:
        changed = 1
    return content, changed


def fix_java_string_api(content, fname):
    """Convert Java String methods to std::string equivalents / Common.h helpers.

    Only the unambiguous String-specific methods are touched (substring/charAt/getBytes/
    startsWith/endsWith/toLowerCase/toUpperCase/trim) to avoid colliding with collections.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    # substring(start, end) -> substr(start, (end)-(start)) ; substring(start) -> substr(start)
    content, n = re.subn(r'\.substring\(([^,()]+),\s*([^()]+)\)', r'.substr(\1, (\2) - (\1))', content); total += n
    content, n = re.subn(r'\.substring\(', '.substr(', content); total += n
    content, n = re.subn(r'\.charAt\(', '.at(', content); total += n
    content, n = re.subn(r'\b(\w+)\.startsWith\(', r'javaStartsWith(\1, ', content); total += n
    content, n = re.subn(r'\b(\w+)\.endsWith\(', r'javaEndsWith(\1, ', content); total += n
    content, n = re.subn(r'\b(\w+)\.toLowerCase\(\)', r'javaToLowerCase(\1)', content); total += n
    content, n = re.subn(r'\b(\w+)\.toUpperCase\(\)', r'javaToUpperCase(\1)', content); total += n
    content, n = re.subn(r'\b(\w+)\.trim\(\)', r'javaTrim(\1)', content); total += n
    # getBytes on a simple var OR a chain (e.g. var->getKey().getBytes("UTF-8"))
    content, n = re.subn(r'\b([\w]+(?:(?:->|\.)\w+\([^()]*\))*)\.getBytes\([^()]*\)',
                         r'(const unsigned char*)(\1).c_str()', content); total += n
    return content, total


def fix_array_length(content, fname):
    """Array-length tracking: companion '<arr>_count' set at allocation + used for .length
    and for byte-array stream methods.

    - 'm_X = new T[N];'      -> also 'm_X_count = N;'   (field companion declared in header)
    - 'T* var = new T[N];'   -> also 'int var_count = N;'  (local companion)
    - 'X /* PORT: array length ... */' -> 'X_count'
    - 'stream->write(X)' / 'stream->readFully(X)' (1 arg) -> '..., X_count)'
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    lines = content.split('\n')
    out = []
    for line in lines:
        out.append(line)
        fm = re.match(r'^(\s*)(m_\w+|s_\w+)\s*=\s*new\s+[\w:]+\s*\[([^\]]+)\]\s*;', line)
        if fm:
            out.append('%s%s_count = %s;' % (fm.group(1), fm.group(2), fm.group(3)))
            total += 1
            continue
        lm = re.match(r'^(\s*)[\w:]+\*\s+(\w+)\s*=\s*new\s+[\w:]+\s*\[([^\]]+)\]\s*;', line)
        if lm:
            out.append('%sint %s_count = %s;' % (lm.group(1), lm.group(2), lm.group(3)))
            total += 1
    content = '\n'.join(out)
    # 'X /* PORT: array length ... */' -> 'X_count'
    content, n = re.subn(r'(\w+)\s*/\*\s*PORT:\s*array length[^*]*\*/', r'\1_count', content); total += n
    # byte-array stream methods: pass the companion length as the 2nd arg
    content, n = re.subn(r'->write\((\w+)\)', r'->write(\1, \1_count)', content); total += n
    content, n = re.subn(r'->readFully\((\w+)\)', r'->readFully(\1, \1_count)', content); total += n
    return content, total


def fix_nbt_string_bytes(content, fname):
    """NBT byte[]<->String conversions:
      new std::string(bytes, "UTF-8")  -> std::string((const char*)bytes, bytes_count)
      'unsigned char* v = (const unsigned char*)...' -> 'const unsigned char* v = ...'
      m_key(NULL)/m_key == NULL  -> ""/empty()  (m_key is a std::string, not nullable)
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    content, n = re.subn(r'new std::string\((\w+),\s*"[^"]*"\)',
                         r'std::string((const char*)\1, \1_count)', content); total += n
    content, n = re.subn(r'\bunsigned char\* (\w+) = \(const unsigned char\*\)',
                         r'const unsigned char* \1 = (const unsigned char*)', content); total += n
    if ': m_key(NULL)' in content:
        content = content.replace(': m_key(NULL)', ': m_key("")'); total += 1
    content, n = re.subn(r'\bm_key == NULL', 'm_key.empty()', content); total += n
    return content, total


def fix_vector_element_ptr(content, fname):
    """'std::vector<RefType>' -> 'std::vector<RefType*>' — reference types (incl. abstract
    ones like Entity) are stored as pointers, not values."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    total = 0
    for r in PROFILE.reference_types:
        content, n = re.subn(r'std::vector<\s*%s\s*>' % re.escape(r), 'std::vector<%s*>' % r, content)
        total += n
    return content, total


def fix_cast_result_dot(content, fname):
    """'(Type*)var).member' -> '(Type*)var)->member' — a pointer-cast result reached with '.'.
    Common in the decompiled source: '((EntityPlayer*)var1).inventory'.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    content, n = re.subn(r'\*\)(\w+)\)\.', r'*)\1)->', content)
    return content, n


def fix_java_constants(content, fname):
    """Java boxed-type constants -> C++ literals (Float.POSITIVE_INFINITY, Integer.MAX_VALUE...)."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    repl = {
        # NB: 3.4028235e38 rounds just ABOVE FLT_MAX (3.40282347e38) -> MSVC C2177
        # "constant too big" for a float literal. Use the largest representable value below it.
        'Float.POSITIVE_INFINITY': '3.4028234e38f',
        'Float.NEGATIVE_INFINITY': '-3.4028234e38f',
        'Float.MAX_VALUE': '3.4028234e38f',
        'Float.MIN_VALUE': '1.4e-45f',
        'Double.POSITIVE_INFINITY': '1.7976931348623157e308',
        'Double.NEGATIVE_INFINITY': '-1.7976931348623157e308',
        'Double.MAX_VALUE': '1.7976931348623157e308',
        'Integer.MAX_VALUE': '2147483647',
        'Integer.MIN_VALUE': '(-2147483647 - 1)',
        'Long.MAX_VALUE': '9223372036854775807LL',
        'Byte.MAX_VALUE': '127', 'Short.MAX_VALUE': '32767',
        'Byte.MIN_VALUE': '(-128)', 'Short.MIN_VALUE': '(-32768)',
    }
    total = 0
    for j, c in repl.items():
        cnt = content.count(j)
        if cnt:
            content = content.replace(j, c)
            total += cnt
    return content, total


def fix_array_cast(content, fname):
    """Java array casts '(int[])expr' / '(byte[])expr' are invalid C++ — the cast target is
    a pointer, so rewrite to '(int*)' / '(unsigned char*)' etc. (matches how array locals are
    already lowered to pointers). Java primitive 'byte'->'unsigned char', 'boolean'->'bool'."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    prim = {'byte': 'unsigned char', 'boolean': 'bool'}
    def repl(m):
        t = m.group(1)
        return '(%s*)' % prim.get(t, t)
    new, n = re.subn(r'\(([A-Za-z_]\w*)\[\]\)', repl, content)
    return new, n


def fix_ptr_cast_dot(content, fname):
    """'((Type*)expr).member' -> '((Type*)expr)->member'. A C-style cast to a pointer
    followed by '.' is always wrong (the LHS is a pointer). Paren-balanced so the inner
    expr may contain calls/subscripts. Runs late so it also catches casts other passes
    introduced (e.g. value-cast -> pointer-cast)."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = 0
    out_lines = []
    for line in content.split('\n'):
        dots = []
        for m in re.finditer(r'\(\((?:const\s+)?[A-Za-z_][\w:]*\s*\*\)', line):
            inner, close = _balanced_paren(line, m.start())
            if close == -1:
                continue
            j = close + 1
            while j < len(line) and line[j] == ' ':
                j += 1
            if j < len(line) and line[j] == '.' and line[j:j+2] != '..':
                dots.append(j)
        for j in sorted(set(dots), reverse=True):
            line = line[:j] + '->' + line[j + 1:]
            n += 1
        out_lines.append(line)
    return '\n'.join(out_lines), n


def fix_texture_path_escapes(content, fname):
    """C4129 + corrupted paths: texture literals emitted as "game:\\Content\\x.png" with
    SINGLE backslashes -> MSVC reads \\C,\\t,... as (invalid) escapes and DROPS the
    backslash (\\t even becomes a TAB), so the path string is wrong and the texture
    never loads. Normalize every backslash inside a game:\\ literal to a doubled \\\\."""
    def fix(m):
        inner = m.group(1)
        inner = inner.replace('\\\\', '\\').replace('\\', '\\\\')  # collapse then double -> idempotent
        return '"' + inner + '"'
    new, n = re.subn(r'"(game:\\[^"]*)"', fix, content)
    return new, n


def fix_dangling_string_returns(content, fname):
    """C4172 (returning address of local/temporary): the converter models Java String
    getters as `const std::string&` but the body returns a STRING LITERAL (or `cond ?
    "" : member`), which builds a temporary std::string and returns a dangling ref ->
    UB at runtime. (a) wrap a whole-body single literal return in a function-static;
    (b) `x.empty() ? "" : x` just returns x (an lvalue, no temp)."""
    n = 0
    # (a) const std::string& C::M(...) { return "lit"; }
    def wrap(m):
        return '%sstatic const std::string _ret = %s; return _ret;%s' % (m.group(1), m.group(2), m.group(3))
    content, c = re.subn(
        r'(const std::string&\s+\w+::\w+\([^)]*\)\s*\{\s*)return\s+("(?:[^"\\]|\\.)*");(\s*\})',
        wrap, content, flags=re.S); n += c
    # (b) ternary empty-check that creates a temp from "" -> return the member directly
    content, c = re.subn(r'return\s+(\w+)\.empty\(\)\s*\?\s*""\s*:\s*\1\s*;', r'return \1;', content); n += c
    return content, n


def fix_shift_precedence_parens(content, fname):
    """C4554: the converter faithfully preserves Java precedence (additive binds tighter
    than shift; relational binds tighter than ^) but MSVC wants it explicit.
    Parenthesize the additive LHS of >> / << and the relational RHS of ^.
    Behaviour-preserving (parens match the existing parse)."""
    n = 0
    # operand may itself contain multiplicative ops (var11 * 4)
    OPERAND = r'(?:\w+(?:\s*[*/%]\s*\w+)*|\([^()]*\))'
    pat = re.compile(r'(?<![\w)])(%s(?:\s*[+\-]\s*%s)+)\s*(>>|<<)\s' % (OPERAND, OPERAND))
    content, c = pat.subn(r'(\1) \2 ', content); n += c
    # `expr ^ a <= b` -> `expr ^ (a <= b)` (relational binds tighter than bitwise xor)
    content, c = re.subn(r'\^\s*(\w+\s*(?:<=|>=|<|>|==|!=)\s*\w+)', r'^ (\1)', content); n += c
    return content, n


def fix_string_temp_getters_byvalue(content, fname):
    """C4172: getters declared `const std::string&` that build+return a TEMPORARY
    (toString -> toStr(x); StepSound names -> "step."+toStr(x); a ternary of literals)
    return a dangling ref. Return BY VALUE instead. These are virtual (toString /
    StepSound funcs), so the decl + def must change uniformly across base + overrides --
    done here per-file by matching the method name everywhere it appears."""
    METHODS = ['toString', 'func_1146_a', 'func_1145_d', 'func_1147_b', 'func_1144_c', 'pickMobSpawner']
    n = 0
    for mth in METHODS:
        # definition:  const std::string& Class::method(  -> std::string Class::method(
        content, c = re.subn(r'const std::string& (\w+::%s)\(' % mth, r'std::string \1(', content); n += c
        # declaration (virtual or not):  [virtual ]const std::string& method(  -> by value
        content, c = re.subn(r'(virtual )?const std::string& (%s)\(' % mth, r'\1std::string \2(', content); n += c
    return content, n


def fix_nbtbase_gettagname(content, fname):
    """C4172: NBTBase::getTagName is `const std::string&` with a switch of string-literal
    returns -> each builds + returns a dangling temporary. Return by VALUE (literals copy
    cheaply; Java returned String by value anyway). Touches the .h decl + .cpp def."""
    if fname == 'NBTBase.h':
        new = content.replace('static const std::string& getTagName(', 'static std::string getTagName(')
        return new, (1 if new != content else 0)
    if fname == 'NBTBase.cpp':
        new = content.replace('const std::string& NBTBase::getTagName(', 'std::string NBTBase::getTagName(')
        return new, (1 if new != content else 0)
    return content, 0


def fix_path_addpoint_return(content, fname):
    """C4715: Path::addPoint returns var1 only in the else-branch; the if-branch (which
    was a Java `throw new IllegalStateException` -> converted to a DbgPrintF log under
    /EH off) falls through with no return -> returns garbage. Return var1 on that path."""
    if fname != 'Path.cpp':
        return content, 0
    new = content.replace('\t\t\treturn var1;\n\t\t}\n}',
                          '\t\t\treturn var1;\n\t\t}\n\treturn var1;\n}', 1)
    return new, (1 if new != content else 0)


def fix_enum_forward_decls(content, fname):
    """Java enums are GENERATED as `struct EnumX {...}` but get forward-declared as
    `class EnumX;`. MSVC mangles struct (U) and class (V) DIFFERENTLY, so a virtual
    method taking an `EnumX*` produces two mangled names: the TU that sees the struct
    definition emits ...PAU..., a TU that only sees the `class` forward-decl emits
    ...PAV... -> LNK2001 on every such vtable slot (hit World's light methods +
    MobSpawnerBase::getEntitiesForType hard). Normalize enum forward-decls to `struct`.
    Anchored on the trailing ';' so the `struct EnumX {` definition is never touched."""
    if not ENUM_TYPE_NAMES:
        return content, 0
    present = [e for e in ENUM_TYPE_NAMES if e in content]
    if not present:
        return content, 0
    alt = '|'.join(re.escape(e) for e in present)
    new, n = re.subn(r'\bclass (%s)\s*;' % alt, r'struct \1;', content)
    return new, n


def fix_dangling_commented_ctor(content, fname):
    """A ctor/method whose SIGNATURE was commented out as dead (e.g. `// PORT: Java
    Swing/AWT: Minecraft::Minecraft(...AWT Component...)`) left its initializer list +
    body dangling: the leading ':' is a C2059 syntax error and the orphan init list
    defaults-to-int (a cascade of C4430). Comment out the dangling init-list + body so
    the dead ctor fully disappears. Guard: only fires when a commented member signature
    is immediately followed (past blanks) by a line starting with ':'."""
    if not fname.endswith('.cpp'):
        return content, 0
    lines = content.split('\n')
    out = []
    i = 0
    changed = 0
    sig_re = re.compile(r'\s*//\s*PORT:.*\b\w+::\w+\s*\(')
    while i < len(lines):
        ln = lines[i]
        if sig_re.match(ln):
            j = i + 1
            while j < len(lines) and lines[j].strip() == '':
                j += 1
            if j < len(lines) and lines[j].lstrip().startswith(':'):
                out.append(ln)
                for k in range(i + 1, j):
                    out.append(lines[k])
                depth = 0
                started = False
                while j < len(lines):
                    cur = lines[j]
                    out.append(cur if cur.lstrip().startswith('//') else '// ' + cur)
                    depth += cur.count('{') - cur.count('}')
                    if '{' in cur:
                        started = True
                    j += 1
                    if started and depth <= 0:
                        break
                changed += 1
                i = j
                continue
        out.append(ln)
        i += 1
    return '\n'.join(out), changed


def fix_nbttagcompound(content, fname):
    """NBTTagCompound: the getters left the map indexed as 'm_tagMap[k]' (treating the map*
    as an array) and dropped the per-getter subtype cast; the map was never allocated; and
    Map.put survived. Fix deref + subtype casts + ctor allocation + put()."""
    if fname != 'NBTTagCompound.cpp':
        return content, 0
    n = 0
    new = content.replace('m_tagMap(NULL)', 'm_tagMap(new std::map<std::string, void*>())')
    if new != content:
        n += 1; content = new
    new = content.replace('m_tagMap->put(var2->getKey(), var2);',
                          '(*m_tagMap)[var2->getKey()] = (void*)var2;')
    if new != content:
        n += 1; content = new
    field_type = {
        'm_byteValue': 'NBTTagByte', 'm_shortValue': 'NBTTagShort',
        'm_intValue': 'NBTTagInt', 'm_longValue': 'NBTTagLong',
        'm_floatValue': 'NBTTagFloat', 'm_stringValue': 'NBTTagString',
        'm_byteArray': 'NBTTagByteArray',
    }
    for fld, typ in field_type.items():
        content, c = re.subn(r'm_tagMap\[var1\]->%s' % fld,
                             '((%s*)(*m_tagMap)[var1])->%s' % (typ, fld), content)
        n += c
    content, c = re.subn(r'(\((?:NBTTagCompound|NBTTagList)\*\))m_tagMap\[var1\]',
                         r'\1(*m_tagMap)[var1]', content)
    n += c
    return content, n


def fix_light(content, fname):
    """Light: type the void* member vectors to their real element types (.h), and drop the
    orphan 'new LinkedList();' (a no-op quirk carried over verbatim from the Java source)."""
    if fname == 'Light.h':
        n = 0
        for fld in ('m_skyLightList', 'm_blockLightList', 'm_lightingUpdateList1'):
            new = content.replace(
                'std::vector<void*> /* PORT: determine element type */ %s' % fld,
                'std::vector<MetadataChunkBlock> %s' % fld)
            if new != content:
                n += 1; content = new
        for fld in ('m_lightingUpdateList', 'm_lightingUpdateList2'):
            new = content.replace(
                'std::vector<void*> /* PORT: determine element type */ %s' % fld,
                'std::vector<int*> %s' % fld)
            if new != content:
                n += 1; content = new
        return content, n
    if fname == 'Light.cpp':
        new = re.sub(r'^\s*new LinkedList\(\);\s*\n', '', content, flags=re.M)
        return new, (1 if new != content else 0)
    return content, 0


def fix_entitymap_misc(content, fname):
    """EntityMap: vector.clear() emitted as memset; the overloaded helper 'a' was field-
    prefixed to 'm_a'; and '!=' was mis-lowered to '!x == y'."""
    if fname != 'EntityMap.cpp':
        return content, 0
    n = 0
    content, c = re.subn(r'memset\((m_entitiesExcludingEntity), 0, sizeof\(\1\)\)',
                         r'\1.clear()', content); n += c
    content, c = re.subn(r'\bm_a\(', 'a(', content); n += c
    new = content.replace('if(!var11 == var9) {', 'if(var11 != var9) {')
    if new != content:
        n += 1; content = new
    return content, n


def fix_entitymapslot(content, fname):
    """EntityMapSlot: the no-arg getters a()/b()/c() were mis-modeled with a spurious 'this'
    param (EntityMapSlot* var0) and called statically. Restore instance no-arg form + fix the
    EntityMap call sites. Also fix the by-object removal on the grid cell vector in b()."""
    if fname == 'EntityMapSlot.h':
        n = 0
        for g in ('a', 'b', 'c'):
            new = content.replace('int %s(EntityMapSlot* var0);' % g, 'int %s();' % g)
            if new != content:
                n += 1; content = new
        return content, n
    if fname == 'EntityMapSlot.cpp':
        n = 0
        getter_field = {'a': 'm_xSlot', 'b': 'm_ySlot', 'c': 'm_zSlot'}
        for g, fld in getter_field.items():
            content, c = re.subn(
                r'int EntityMapSlot::%s\(EntityMapSlot\* var0\) \{\s*return var0->%s;\s*\}' % (g, fld),
                'int EntityMapSlot::%s() {\n\t\treturn %s;\n}' % (g, fld), content)
            n += c
        content, c = re.subn(
            r'(m_entityMap->m_entityGrid\[[^\]]*\])\.erase\(\.begin\(\) \+ var1\)\s*'
            r'/\* PORT: verify remove semantics \*/',
            r'_vecRemoveObj(\1, var1)', content)
        n += c
        return content, n
    if fname == 'EntityMap.cpp':
        new = re.sub(r'EntityMapSlot::([abc])\((var\d+)\)', r'\2->\1()', content)
        return new, (1 if new != content else 0)
    return content, 0


def fix_mobspawner(content, fname):
    """MobSpawner: the reflective entity counts (entitiesInLevelList(EntityMob/Animal.class))
    were PORT-blocked, leaving var3/var4 undeclared; stub them to 0 (spawn cap simply not
    enforced). var23 holds Entity subclasses (dynamic_cast + Entity*/EntityLiving* casts) so
    it must be Entity*, not void*."""
    if fname != 'MobSpawner.cpp':
        return content, 0
    n = 0
    new = content.replace(
        '// PORT-BLOCKED: int var3 = worldObj->entitiesInLevelList(EntityMob.class);',
        'int var3 = 0; // PORT: countEntities(EntityMob.class) needs reflection; cap disabled')
    if new != content:
        n += 1; content = new
    new = content.replace(
        '// PORT-BLOCKED: var4 = worldObj->entitiesInLevelList(EntityAnimal.class);',
        'var4 = 0; // PORT: countEntities(EntityAnimal.class) needs reflection; cap disabled')
    if new != content:
        n += 1; content = new
    new = content.replace('void* var23;', 'Entity* var23;')
    if new != content:
        n += 1; content = new
    return content, n


def fix_block_ctor_msg(content, fname):
    """Block ctor: the 'slot already occupied' diagnostic concatenates Block* operands
    (blocksList[i], this) that have no toStr overload. Keep the slot number, drop the
    un-stringable pointers."""
    if fname != 'Block.cpp':
        return content, 0
    new = content.replace(
        '"Slot " + toStr(var1) + " is already occupied by " + blocksList[var1] + " when adding " + this',
        '"Slot " + toStr(var1) + " is already occupied"')
    return new, (1 if new != content else 0)


def fix_playercontrollercreative(content, fname):
    """PlayerControllerCreative::onRespawn fills the creative hotbar from
    Session.registeredBlocksList (a stub here). Use sequential block IDs as the palette —
    compiles and gives a usable creative hotbar without the (deferred) Session palette."""
    if fname != 'PlayerControllerCreative.cpp':
        return content, 0
    new = re.sub(
        r'new ItemStack\([^;]*registeredBlocksList\[var2\][^;]*\)',
        'new ItemStack(var2 + 1) /* PORT: creative palette (Session.registeredBlocksList) TODO */',
        content)
    return new, (1 if new != content else 0)


def fix_guimainmenu_splashes(content, fname):
    """GuiMainMenu: the String[] splashes array collapsed to a scalar std::string and its
    random-pick initializer referenced the dropped 'splashes' symbol. Give a fixed splash
    and make the destructor not delete[] a std::string."""
    if fname != 'GuiMainMenu.cpp':
        return content, 0
    n = 0
    new = re.sub(r'm_splashes\(NULL [^\n]*?\*/\)', 'm_splashes("")', content)
    if new != content:
        n += 1; content = new
    new = re.sub(r'm_currentSplash\(splashes\[.*\]\)',
                 'm_currentSplash("Now with shaders!")', content)
    if new != content:
        n += 1; content = new
    new = content.replace('\tdelete[] m_splashes;\n', '')
    if new != content:
        n += 1; content = new
    new = content.replace('\tm_splashes = NULL;\n', '\tm_splashes.clear();\n')
    if new != content:
        n += 1; content = new
    return content, n


def fix_texturegearsfx(content, fname):
    """TextureGearsFX loads gear pixels from PNG resources via BufferedImage (var3), which is
    PORT-blocked -> var3 undeclared. Drop the getRGB reads (gear FX renders blank until D3D9
    texture loading is wired up)."""
    if fname != 'TextureGearsFX.cpp':
        return content, 0
    new = re.sub(r'(\n[ \t]*)var3\.getRGB\([^;]*\);',
                 r'\1// PORT: gear pixels (var3 BufferedImage) -> D3D9 texture load TODO', content)
    return new, (1 if new != content else 0)


def fix_levelgenerator(content, fname):
    """LevelGenerator: wrap the int operands in the phase-count diagnostic for std::string
    concat, and include Material.h (getBlockMaterial(...)->getCanBlockGrass())."""
    if fname != 'LevelGenerator.cpp':
        return content, 0
    n = 0
    new = content.replace(
        '"Wrong number of phases! Wanted " + this->m_phases + ", got " + this->m_phaseBar',
        '"Wrong number of phases! Wanted " + toStr(this->m_phases) + ", got " + toStr(this->m_phaseBar)')
    if new != content:
        n += 1; content = new
    if '#include "Material.h"' not in content and 'Material' in AVAILABLE_HEADERS:
        content = content.replace('#include "Common.h"\n',
                                  '#include "Common.h"\n#include "Material.h"\n', 1)
        n += 1
    return content, n


def fix_pathfinder(content, fname):
    """Pathfinder::m_pointMap is a HashMap keyed by PathPoint hash (Integer.valueOf), not a
    String map; retype it to int-keyed and allocate it (the ctor left it NULL)."""
    if fname == 'Pathfinder.h':
        new = content.replace('std::map<std::string, void*>* m_pointMap',
                              'std::map<int, void*>* m_pointMap')
        return new, (1 if new != content else 0)
    if fname == 'Pathfinder.cpp':
        new = content.replace('m_pointMap(NULL)', 'm_pointMap(new std::map<int, void*>())')
        return new, (1 if new != content else 0)
    return content, 0


def fix_stream_io_restore(content, fname):
    """The converter PORT-commented the Java DataInputStream/DataOutputStream wrappers
    (file I/O was platform-specific). The JavaIO bridge now provides them STFS-backed, so
    restore the original declaration captured in the '// was:' comment."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, c = re.subn(
        r'[^\n]*// was: (Data(?:Input|Output)Stream\* var\d+ = new [^\n;]+;)',
        r'\t\1', content)
    return new, c


def fix_levelloader_io(content, fname):
    """Wire converted LevelLoader save/load onto the JavaIO bridge + fix the converted
    logic the decompiler mangled: the two diagnostic string concats, the loadEntity ternary
    (incompatible Entity-subclass pointers need a common Entity* type), the tile-entity
    save loop (Java map.keySet() iterator -> std::map iteration over World::m_map), and the
    DataOutputStream close() that flushes the NBT buffer to the file."""
    if fname != 'LevelLoader.cpp':
        return content, 0
    n = 0
    # NB: lambda replacements — a plain re.sub replacement string would interpret the
    # literal '\n'/'\"' as escape sequences (newline/quote) and re-break the string.
    new = re.sub(r'DbgPrintF\("%s\\n", "Skipping unknown entity id[^;]*\);',
                 lambda m: 'DbgPrintF("Skipping unknown entity id \\"%s\\"\\n", var19.c_str());', content)
    if new != content:
        n += 1; content = new
    new = re.sub(r'DbgPrintF\("%s\\n", "Skipping unknown tile entity id[^;]*\);',
                 lambda m: 'DbgPrintF("Skipping unknown tile entity id \\"%s\\"\\n", var14.c_str());', content)
    if new != content:
        n += 1; content = new
    new, c = re.subn(r'new (Entity\w+)\(var1\)', r'(Entity*)new \1(var1)', content)
    n += c; content = new
    new = content.replace(
        'for (size_t _i17 = 0; _i17 < var1->m_map->keySet().size(); ++_i17) {',
        'for (std::map<int, void*>::iterator _it = var1->m_map->begin(); _it != var1->m_map->end(); ++_it) {')
    if new != content:
        n += 1; content = new
    new = content.replace('int var19 = ((int)var17->next()).intValue();', 'int var19 = _it->first;')
    if new != content:
        n += 1; content = new
    new = content.replace('TileEntity* var20 = (TileEntity*)var1->(*m_map).operator[](var19);',
                          'TileEntity* var20 = (TileEntity*)_it->second;')
    if new != content:
        n += 1; content = new
    new = content.replace('   // Removed Java stream/exception call: var14->close();',
                          '\t\tvar14->close();')
    if new != content:
        n += 1; content = new
    return content, n


def fix_gamesettings_io(content, fname):
    """GameSettings options.txt I/O was PORT-stubbed (BufferedReader/PrintWriter). Reimplement
    loadOptions/saveOptions on the JavaIO File/stream bridge (the options file lives on the
    title's storage, same as world saves), preserving the converted key->field parse logic.
    Also include File.h and wrap the setOptionString label literals for std::string concat."""
    if fname == 'GameSettings.h':
        n = 0
        new = content.replace('std::string /* PORT: was java.io.File */* m_optionsFile;',
                              'File* m_optionsFile;')
        if new != content:
            n += 1; content = new
        if 'class File;' not in content:
            content = re.sub(r'(\nclass GameSettings\b)', r'\nclass File;\1', content, count=1)
            n += 1
        return content, n
    if fname != 'GameSettings.cpp':
        return content, 0
    n = 0
    if '#include "File.h"' not in content:
        content = content.replace('#include "Common.h"\n', '#include "Common.h"\n#include "File.h"\n', 1)
        n += 1
    # the game-dir ctor arg var2 is a std::string* — deref for the File(const string&, ...) ctor
    new = content.replace('new File(var2, "options.txt")', 'new File(*var2, "options.txt")')
    if new != content:
        n += 1; content = new
    # setKeyBindingString was a no-return stub (Keyboard.getKeyName gone under XInput); return
    # the binding description (XInput button labels are a later UI concern).
    new = re.sub(r'\n[ \t]*// STUBBED: return m_keyBindings\[var1\][^\n]*',
                 '\n\treturn m_keyBindings[var1].m_keyDescription;', content)
    if new != content:
        n += 1; content = new
    # setOptionString: "Label: " + (...) is const char* + const char* (C2110); make the
    # leading label a std::string so the whole nested ternary concatenates as std::string.
    content, c = re.subn(r'"([^"\n]*: )" \+ ', lambda m: 'std::string("%s") + ' % m.group(1), content)
    n += c
    load_body = (
        'void GameSettings::loadOptions() {\n'
        '\tif (m_optionsFile == NULL || !m_optionsFile->exists()) return;\n'
        '\tFileInputStream _fis(m_optionsFile);\n'
        '\tstd::string _all; { int _c; while ((_c = _fis.read()) >= 0) _all += (char)_c; }\n'
        '\tsize_t _p = 0;\n'
        '\twhile (_p < _all.size()) {\n'
        '\t\tsize_t _nl = _all.find(\'\\n\', _p);\n'
        '\t\tstd::string var2 = _all.substr(_p, (_nl == std::string::npos ? _all.size() : _nl) - _p);\n'
        '\t\t_p = (_nl == std::string::npos) ? _all.size() : _nl + 1;\n'
        '\t\tif (!var2.empty() && var2[var2.size() - 1] == \'\\r\') var2.erase(var2.size() - 1);\n'
        '\t\tsize_t _colon = var2.find(\':\');\n'
        '\t\tif (_colon == std::string::npos) continue;\n'
        '\t\tstd::string var5[2]; var5[0] = var2.substr(0, _colon); var5[1] = var2.substr(_colon + 1);\n'
        '\t\tif (var5[0] == "m_music") m_music = var5[1] == "true";\n'
        '\t\tif (var5[0] == "m_sound") m_sound = var5[1] == "true";\n'
        '\t\tif (var5[0] == "invertYMouse") m_invertMouse = var5[1] == "true";\n'
        '\t\tif (var5[0] == "showFrameRate") m_showFPS = var5[1] == "true";\n'
        '\t\tif (var5[0] == "viewDistance") m_renderDistance = atoi(var5[1].c_str());\n'
        '\t\tif (var5[0] == "bobView") m_fancyGraphics = var5[1] == "true";\n'
        '\t\tif (var5[0] == "anaglyph3d") m_anaglyph = var5[1] == "true";\n'
        '\t\tif (var5[0] == "m_limitFramerate") m_limitFramerate = var5[1] == "true";\n'
        '\t\tif (var5[0] == "m_difficulty") m_difficulty = atoi(var5[1].c_str());\n'
        '\t\tfor (int var3 = 0; var3 < m_keyBindings_count; ++var3)\n'
        '\t\t\tif (var5[0] == "key_" + m_keyBindings[var3].m_keyDescription)\n'
        '\t\t\t\tm_keyBindings[var3].m_keyCode = atoi(var5[1].c_str());\n'
        '\t}\n'
        '}')
    save_body = (
        'void GameSettings::saveOptions() {\n'
        '\tif (m_optionsFile == NULL) return;\n'
        '\tFileOutputStream _fos(m_optionsFile);\n'
        '\tstd::string _o;\n'
        '\t_o += "m_music:" + std::string(m_music ? "true" : "false") + "\\n";\n'
        '\t_o += "m_sound:" + std::string(m_sound ? "true" : "false") + "\\n";\n'
        '\t_o += "invertYMouse:" + std::string(m_invertMouse ? "true" : "false") + "\\n";\n'
        '\t_o += "showFrameRate:" + std::string(m_showFPS ? "true" : "false") + "\\n";\n'
        '\t_o += "viewDistance:" + toStr(m_renderDistance) + "\\n";\n'
        '\t_o += "bobView:" + std::string(m_fancyGraphics ? "true" : "false") + "\\n";\n'
        '\t_o += "anaglyph3d:" + std::string(m_anaglyph ? "true" : "false") + "\\n";\n'
        '\t_o += "m_limitFramerate:" + std::string(m_limitFramerate ? "true" : "false") + "\\n";\n'
        '\t_o += "m_difficulty:" + toStr(m_difficulty) + "\\n";\n'
        '\tfor (int var2 = 0; var2 < m_keyBindings_count; ++var2)\n'
        '\t\t_o += "key_" + m_keyBindings[var2].m_keyDescription + ":" + toStr(m_keyBindings[var2].m_keyCode) + "\\n";\n'
        '\tif (!_o.empty()) _fos.write((const unsigned char*)_o.data(), (int)_o.size());\n'
        '\t_fos.close();\n'
        '}')
    new = re.sub(r'void GameSettings::loadOptions\(\) \{.*?\n\}', lambda m: load_body, content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    new = re.sub(r'void GameSettings::saveOptions\(\) \{.*?\n\}', lambda m: save_body, content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    return content, n


def fix_entityrenderer(content, fname):
    """EntityRenderer: stub the two Java-AWT debug screenshot helpers (BufferedImage/Graphics/
    ImageIO/DataBufferInt — irredeemable on 360), null the (now-unused) DecimalFormat field,
    and fix the instanceof->dynamic_cast mangle (mc->dynamic_cast<T>(field))."""
    if fname != 'EntityRenderer.cpp':
        return content, 0
    n = 0
    c = content.count('m_entityDecimalFormat(new DecimalFormat("0000"))')
    if c:
        content = content.replace('m_entityDecimalFormat(new DecimalFormat("0000"))',
                                  'm_entityDecimalFormat(NULL)')
        n += c
    for hdr in ('GameSettings', 'EntityMap', 'RenderEngine', 'InventoryPlayer',
                'MouseHelper', 'GuiIngame', 'GuiScreen'):
        if ('#include "%s.h"' % hdr) not in content and hdr in AVAILABLE_HEADERS:
            content = content.replace('#include "Common.h"\n',
                                      '#include "Common.h"\n#include "%s.h"\n' % hdr, 1)
            n += 1
    # instanceof mangle + dropped mc->m_ prefix: '[recv->/.]dynamic_cast<PCC*>(playerController)'
    # -> 'dynamic_cast<PlayerControllerCreative*>(mc->m_playerController)'
    content, c = re.subn(
        r'(?:\w+(?:->|\.))?dynamic_cast<PlayerControllerCreative\s*\*>\(playerController\)',
        'dynamic_cast<PlayerControllerCreative*>(mc->m_playerController)', content)
    n += c
    new = re.sub(r'void EntityRenderer::grabLargeScreenshot\(\) \{.*?\n\}',
                 lambda m: ('void EntityRenderer::grabLargeScreenshot() {\n'
                            '\t// PORT: large map screenshot (Java AWT BufferedImage/ImageIO) '
                            'not supported on Xbox 360\n}'),
                 content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    new = re.sub(r'void\* /\* PORT: was BufferedImage \*/ EntityRenderer::screenshotBuffer\(.*?\) \{.*?\n\}',
                 lambda m: ('void* EntityRenderer::screenshotBuffer(unsigned char* var0, int var1, int var2) {\n'
                            '\treturn NULL; // PORT: BufferedImage screenshot not supported on Xbox 360\n}'),
                 content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    return content, n


def fix_instanceof_dynamic_cast(content, fname):
    """Java `expr instanceof Type` was mis-converted to `receiver->dynamic_cast<Type*>(expr)`
    (dynamic_cast is an operator, not a member) -> `dynamic_cast<Type*>(expr)`."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, c = re.subn(r'\b\w+->dynamic_cast<([\w:]+\s*\*)>\(([^()]*)\)',
                     r'dynamic_cast<\1>(\2)', content)
    return new, c


def fix_display_isactive(content, fname):
    """Display.isActive() (LWJGL window focus) is always true on 360. The converter blanket-
    replaced it with `true /* PORT: always active on 360 */`, which broke the enclosing
    constructs: `if(...){` lost its brace (orphaning the block's `}`), and assignments lost
    their LHS. Reconstruct from the `// was:` comment — focus-loss ifs (`!Display.isActive()`)
    become if(false), focus ifs become if(true), `x = Display.isActive()` becomes `x = true;`."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = 0
    out = []
    for line in content.split('\n'):
        m = re.match(r'(\s*)(?:true|false) /\* PORT: [^*]*?\*/\s*// was: (.*)$', line)
        if not m:
            out.append(line); continue
        indent, was = m.group(1), m.group(2).strip()
        im = re.match(r'if\s*\((.*)\)\s*\{$', was)
        if im:
            cond = im.group(1).replace(' ', '')
            # LWJGL window-state on 360: focus is always active; close-request / focus-loss never fire.
            if 'isCloseRequested' in cond or '!Display' in cond:
                val = 'false'
            elif 'Display.isActive()' in cond:
                val = 'true'
            else:
                val = 'false'
            out.append('%sif (%s) { // PORT: LWJGL window state (Display) fixed on 360' % (indent, val))
            n += 1; continue
        am = re.match(r'(?:this\.)?(\w+)\s*=\s*Display\.isActive\(\);$', was)
        if am:
            fld = am.group(1)
            out.append('%s%s = true;' % (indent, fld if fld.startswith('m_') else 'm_' + fld))
            n += 1; continue
        out.append('%s(void)0; // PORT: %s' % (indent, was))
        n += 1
    return '\n'.join(out), n


def fix_renderglobal(content, fname):
    """RenderGlobal compile fixes: WorldRenderer arrays are arrays-of-reference-type
    (WorldRenderer**, not WorldRenderer*); the void* update list is WorldRenderer*; the
    occlusion _count companion is missing; cacheActiveRenderInfo needs EntityPlayerSP complete
    for the upcast; .get()->[]; trailing int in a string concat; network skin download stubbed;
    and the mangled display-list batch-draw block (dropped 'if(var4>0){' + lost 'return var4;').
    The real chunk display-list -> D3D9 vertex-buffer draw stays a runtime stub."""
    if fname == 'RenderGlobal.h':
        n = 0
        for a, b in (
            ('WorldRenderer* m_sortedWorldRenderers;', 'WorldRenderer** m_sortedWorldRenderers;'),
            ('WorldRenderer* m_worldRenderers;', 'WorldRenderer** m_worldRenderers;'),
            ('std::vector<void*> /* PORT: determine element type */ m_worldRenderersToUpdate;',
             'std::vector<WorldRenderer*> m_worldRenderersToUpdate;'),
        ):
            new = content.replace(a, b)
            if new != content:
                n += 1; content = new
        if 'int m_glOcclusionQueryBase_count;' not in content:
            content = content.replace('\tint m_worldRenderers_count;',
                                      '\tint m_worldRenderers_count;\n\tint m_glOcclusionQueryBase_count;')
            n += 1
        return content, n
    if fname != 'RenderGlobal.cpp':
        return content, 0
    n = 0
    if '#include "EntityPlayerSP.h"' not in content and 'EntityPlayerSP' in AVAILABLE_HEADERS:
        content = content.replace('#include "Common.h"\n',
                                  '#include "Common.h"\n#include "EntityPlayerSP.h"\n', 1); n += 1
    content, c = re.subn(r'new WorldRenderer\[', 'new WorldRenderer*[', content); n += c
    content, c = re.subn(r'(m_worldRenderers\[[^\]]*\] = )WorldRenderer\(', r'\1new WorldRenderer(', content); n += c
    content, c = re.subn(r'(m_sortedWorldRenderers|m_worldRenderers)\[([^\]]*)\]\.', r'\1[\2]->', content); n += c
    content, c = re.subn(r'm_worldRenderersToUpdate\.get\(([^)]*)\)', r'm_worldRenderersToUpdate[\1]', content); n += c
    # the object-remove find/erase hardcoded a void* iterator (from _fix_remove_semantics) but
    # the list is now vector<WorldRenderer*> -> match the iterator type.
    new = content.replace(
        'std::vector<void*>::iterator _removeIt = std::find(m_worldRenderersToUpdate.begin()',
        'std::vector<WorldRenderer*>::iterator _removeIt = std::find(m_worldRenderersToUpdate.begin()')
    if new != content:
        n += 1; content = new
    new = content.replace(
        '", I: " + (m_countEntitiesTotal - m_countEntitiesHidden - m_countEntitiesRendered)',
        '", I: " + toStr(m_countEntitiesTotal - m_countEntitiesHidden - m_countEntitiesRendered)')
    if new != content:
        n += 1; content = new
    new = re.sub(r'void RenderGlobal::obtainEntitySkin\(Entity\* var1\) \{.*?\n\}',
                 lambda m: ('void RenderGlobal::obtainEntitySkin(Entity* var1) {\n'
                            '\t// PORT: skin download (network) not used on Xbox 360\n}'),
                 content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    new = re.sub(r'void RenderGlobal::releaseEntitySkin\(Entity\* var1\) \{.*?\n\}',
                 lambda m: ('void RenderGlobal::releaseEntitySkin(Entity* var1) {\n'
                            '\t// PORT: skin release (network) not used on Xbox 360\n}'),
                 content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    new = re.sub(
        r'[ \t]*memset\(m_renderIntBuffer, 0, sizeof\(m_renderIntBuffer\)\);.*?\n\}\n\nvoid RenderGlobal::renderAllRenderLists',
        lambda m: ('\t// PORT: chunk display-list batch draw (renderIntBuffer/glCallLists) -> '
                   'D3D9 VB draw (runtime TODO)\n\treturn var4;\n}\n\nvoid RenderGlobal::renderAllRenderLists'),
        content, flags=re.DOTALL)
    if new != content:
        n += 1; content = new
    return content, n


def fix_minecraft_running(content, fname):
    """Minecraft.cpp uses bare `running`/`isGamePaused` flags the parser dropped from
    Minecraft.h; declare them. `running` is also referenced by LoadingScreenRenderer."""
    if fname != 'Minecraft.h':
        return content, 0
    n = 0
    if 'bool running;' not in content:
        new = content.replace('\tbool m_appletMode;', '\tbool running;\n\tbool m_appletMode;', 1)
        if new != content:
            n += 1; content = new
    if 'bool isGamePaused;' not in content:
        new = content.replace('\tbool m_appletMode;', '\tbool isGamePaused;\n\tbool m_appletMode;', 1)
        if new != content:
            n += 1; content = new
    return content, n


def fix_nbtbase_abstract(content, fname):
    """NBTBase::read/writeTagContents are abstract in Java but were emitted non-virtual, so
    the static read/writeTag calls bind to NBTBase (undefined -> LNK2019). Make them pure
    virtual (every NBT subclass overrides both) so the calls dispatch correctly."""
    if fname != 'NBTBase.h':
        return content, 0
    n = 0
    for decl in ('void writeTagContents(DataOutput* var1);', 'void readTagContents(DataInput* var1);'):
        new = content.replace('\t' + decl, '\tvirtual ' + decl[:-1] + ' = 0;')
        if new != content:
            n += 1; content = new
    return content, n


def fix_minecraft(content, fname):
    """Minecraft.cpp run()/init is LWJGL-bound (Display, IntBuffer, Sys, GL caps, Controllers,
    OS-detection data dir, resource-download threads) — none exist on 360. Stub them to the D3D9
    + fixed-storage equivalents, keeping the game-object setup and the tick/render loop. The
    Display.isActive/CloseRequested mangles are handled by fix_display_isactive."""
    if fname != 'Minecraft.cpp':
        return content, 0
    n = 0
    # needed complete types for ctor news / loop derefs
    for hdr in ('LoadingScreenRenderer', 'EntityRenderer', 'GuiScreen', 'WorldRenderer',
                'GameSettings', 'GuiErrorScreen', 'World', 'ItemRenderer'):
        if ('#include "%s.h"' % hdr) not in content and hdr in AVAILABLE_HEADERS:
            content = content.replace('#include "Common.h"\n',
                                      '#include "Common.h"\n#include "%s.h"\n' % hdr, 1)
            n += 1
    repls = [
        ('m_server = NULL;', 'm_server = "";'),
        ('m_server != NULL', '!m_server.empty()'),
        ('\t\tnew ThreadSleepForever(this, "Timer hack thread");\n',
         '\t\t// PORT: ThreadSleepForever (Java timer hack) n/a on 360\n'),
        ('\t\t\t\tDisplay.setParent(m_mcCanvas);', '\t\t\t\t(void)0;'),
        ('m_displayWidth = Display.getDisplayMode().getWidth();', 'm_displayWidth = SCREEN_WIDTH;'),
        ('m_displayHeight = Display.getDisplayMode().getHeight();', 'm_displayHeight = SCREEN_HEIGHT;'),
        ('IntBuffer var24;', 'int* var24 = NULL;'),
        ('DbgPrintF("%s\\n", "LWJGL version: " + Sys::getVersion());',
         'DbgPrintF("%s\\n", "Xbox 360 D3D9");'),
        ('\t\t\t\tControllers::create();', '\t\t\t\t(void)0; // PORT: XInput needs no init'),
        ('m_glCapabilities = new OpenGlCapsChecker();', 'm_glCapabilities = NULL;'),
        ('m_downloadResourcesThread->closeMinecraft();',
         '/* PORT: resource-download thread n/a on 360 */;'),
        ('"The game broke! [" + var18 + "]"', '"The game broke!"'),
        ('Mouse.setNativeCursor(NULL);', '(void)0; // PORT: no native cursor on 360'),
        # AWT Canvas (m_mcCanvas is void*) sizes -> fixed 360 screen (dead code on 360 anyway)
        ('m_mcCanvas->getWidth()', 'SCREEN_WIDTH'),
        ('m_mcCanvas->getHeight()', 'SCREEN_HEIGHT'),
        # static field access
        ('WorldRenderer.chunksUpdated', 'WorldRenderer::chunksUpdated'),
    ]
    for a, b in repls:
        cnt = content.count(a)
        if cnt:
            content = content.replace(a, b); n += cnt
    # orphan glGetString string-literal expressions (System.out.println(glGetString(...)) remnants)
    content, c = re.subn(r'[ \t]*"Xbox 360" /\* PORT: was glGetString \*/\n', '', content); n += c
    # field-prefix glued onto a comment ('m_// ...') — always a broken dropped-call line
    c = content.count('m_//')
    if c:
        content = content.replace('m_//', '//'); n += c
    # GL caps dump block (var2 ContextCapabilities is undefined) -> drop
    content, c = re.subn(
        r'[ \t]*DbgPrintF\("%s\\n", "OpenGL 3\.0: ".*?"COMPATIBILITY PROFILE: " \+ \(\(var25 & 2\) != 0\)\);\n\s*\}',
        lambda m: '\t\t\t\t// PORT: GL caps dump n/a (no GL context on 360)', content, flags=re.DOTALL)
    n += c
    # OS-detection data dir -> fixed Xbox storage path (m_mcDataDir is std::string*)
    content, c = re.subn(
        r'[ \t]*std::string var3 = "minecraft";.*?m_mcDataDir = var27;',
        lambda m: '\t\t\tm_mcDataDir = new std::string("cache:\\\\.minecraft\\\\");',
        content, flags=re.DOTALL)
    n += c
    # resource-download thread spawn -> skip
    content, c = re.subn(
        r'[ \t]*var1->m_downloadResourcesThread = new ThreadDownloadResources\([^;]*\);\n'
        r'[ \t]*var1->m_downloadResourcesThread->start\(\);',
        lambda m: '\t\t\t\t// PORT: resource-download thread (network) n/a on 360',
        content, flags=re.DOTALL)
    n += c
    # AWT canvas resize check (m_mcCanvas is void*, always NULL on 360)
    content, c = re.subn(
        r'if\(m_mcCanvas != NULL && !m_fullscreen && \(m_mcCanvas->getWidth\(\)[^{]*\) \{',
        lambda m: 'if (false) { // PORT: AWT canvas resize n/a on 360', content)
    n += c
    # int concat in the fps debug line
    new = content.replace('+ WorldRenderer::chunksUpdated +', '+ toStr(WorldRenderer::chunksUpdated) +')
    if new != content:
        n += 1; content = new
    # applet document-base host check (minecraft.net) — web/applet only, n/a on 360
    content, c = re.subn(r'if\(m_mcApplet->getDocumentBase\(\)[^{]*\) \{',
                         'if (true) { // PORT: applet host check n/a on 360', content)
    n += c
    # runTick input-event-loop block (Mouse.next()/Keyboard.next() with labeled breaks) is
    # irredeemable — the converter commented out the loop braces, leaving openings unbalanced.
    # Replace the whole block with a stub (keeping the leftClickCounter logic); XInput poll TBD.
    content, c = re.subn(
        r'if\(currentScreen == NULL \|\| currentScreen->m_allowUserInput\) \{.*?'
        r'(?=\n\t\tif\(currentScreen != NULL\) \{\n\t\t\tm_prevFrameTime)',
        lambda m: ('if (currentScreen == NULL || currentScreen->m_allowUserInput) {\n'
                   '\t\t\tif (m_leftClickCounter > 0) {\n'
                   '\t\t\t\t--m_leftClickCounter;\n'
                   '\t\t\t}\n'
                   '\t\t\t// PORT: Mouse/Keyboard event loops -> XInput poll/dispatch (runtime TODO)\n'
                   '\t\t}'),
        content, flags=re.DOTALL)
    n += c
    return content, n


def fix_pathpoint_arrays(content, fname):
    """PathPoint is a reference type, so its Java arrays are arrays-of-pointers (PathPoint**),
    but the converter left the array FIELDS/locals as PathPoint* (array of values) and
    PathEntity still accessed elements with '.'. Retype the arrays + allocations + element
    access across Path/Pathfinder/PathEntity, fix the lost arraycopy, and thread the real path
    length into PathEntity (was stubbed to '>= 4')."""
    if fname == 'Path.h':
        new = content.replace('PathPoint* m_pathPoints;', 'PathPoint** m_pathPoints;')
        return new, (1 if new != content else 0)
    if fname == 'Pathfinder.h':
        new = content.replace('PathPoint* m_pathOptions;', 'PathPoint** m_pathOptions;')
        return new, (1 if new != content else 0)
    if fname == 'PathEntity.h':
        n = 0
        new = content.replace('PathEntity(PathPoint* var1);',
                              'PathEntity(PathPoint** var1, int var1_count = 0);')
        if new != content:
            n += 1; content = new
        # NB: an 'int m_points_count;' array-length companion already exists — only retype.
        new = content.replace('PathPoint* m_points;', 'PathPoint** m_points;')
        if new != content:
            n += 1; content = new
        return content, n
    if fname in ('Path.cpp', 'Pathfinder.cpp'):
        n = 0
        content, c = re.subn(r'new PathPoint\[', 'new PathPoint*[', content); n += c
        content, c = re.subn(r'PathPoint\* (var\d+) = new PathPoint\*\[',
                             r'PathPoint** \1 = new PathPoint*[', content); n += c
        content, c = re.subn(r'\(PathPoint\)', '(PathPoint*)', content); n += c
        if fname == 'Path.cpp':
            new = content.replace('/* PORT: memcpy */(m_pathPoints, 0, var2, 0, m_count);',
                                  'memcpy(var2, m_pathPoints, m_count * sizeof(PathPoint*));')
            if new != content:
                n += 1; content = new
        if fname == 'Pathfinder.cpp':
            new = content.replace('new PathEntity(var3)', 'new PathEntity(var3, var3_count)')
            if new != content:
                n += 1; content = new
        return content, n
    if fname == 'PathEntity.cpp':
        n = 0
        new = content.replace('PathEntity::PathEntity(PathPoint* var1)',
                              'PathEntity::PathEntity(PathPoint** var1, int var1_count)')
        if new != content:
            n += 1; content = new
        new = content.replace('\t\tm_points = var1;\n',
                              '\t\tm_points = var1;\n\t\tm_points_count = var1_count;\n')
        if new != content:
            n += 1; content = new
        c = content.count(': m_points(NULL)\n\t, m_pathIndex(0)')
        content = content.replace(': m_points(NULL)\n\t, m_pathIndex(0)',
                                  ': m_points(NULL)\n\t, m_points_count(0)\n\t, m_pathIndex(0)')
        n += c
        content, c = re.subn(r'm_points\[([^\]]+)\]\.', r'm_points[\1]->', content); n += c
        new = content.replace('m_pathIndex >= 4 /* PORT: quad vertex count */',
                              'm_pathIndex >= m_points_count')
        if new != content:
            n += 1; content = new
        return content, n
    return content, 0


def fix_redundant_nanotime(content, fname):
    """Remove the injected 'static long long GetNanoTime()' — Common.h now provides an
    inline one, and having both is a redefinition (different linkage)."""
    if not fname.endswith('.cpp'):
        return content, 0
    new = re.sub(
        r'\n?// Xbox 360 equivalent of Java System\.nanoTime\(\)\nstatic long long GetNanoTime\(\)\n\{.*?\n\}\n',
        '\n', content, flags=re.DOTALL)
    return new, (1 if new != content else 0)


def fix_near_passing_misc(content, fname):
    """Targeted per-file fixes for near-passing files (bare static refs, field-name slips)."""
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    if fname == 'TexturedQuad.cpp':
        # delegating ctor referenced non-existent field names; real field is m_vertexPositions
        content, n = re.subn(r'\bm_vertices\b', 'm_vertexPositions', content); total += n
        content, n = re.subn(r'[ \t]*m_vertexCount\s*=\s*[^;]+;[^\n]*\n', '', content); total += n
    if fname.startswith('Item'):
        # Item's static 'rand' (Random*) referenced bare collides with C ::rand(); qualify it.
        content, n = re.subn(r'\brand(?=->|\.)', 'Item::s_rand', content); total += n
    # ItemStack is a reference type (pointer everywhere), so a bare 'ItemStack(...)' value
    # construction is wrong — it's heap-allocated. Skip 'new ItemStack(' (already correct)
    # and '::ItemStack(' / ItemStack.cpp (the class's own ctor definitions).
    if fname != 'ItemStack.cpp':
        content, n = re.subn(r'(?<!new )(?<!:)\bItemStack\s*\(', 'new ItemStack(', content); total += n
    # EntityPlayer's field is m_inventory; bare '->inventory' (e.g. after a cast) misses the prefix
    content, n = re.subn(r'->inventory\b', '->m_inventory', content); total += n
    if fname == 'World.cpp':
        # bare field 'random' (the Random* field is m_random) missed in ctor init list
        content, n = re.subn(r'\brandom(?=\s*->)', 'm_random', content); total += n
    if fname == 'PathPoint.cpp':
        # equals(Object): '((PathPoint)var).field' casts void* to a value — use a pointer cast.
        content, n = re.subn(r'\(\(PathPoint\)(\w+)\)\.', r'((PathPoint*)\1)->', content); total += n
        content, n = re.subn(r'->hash\b', '->m_hash', content); total += n  # field is m_hash
    # bare Block static 'stairSingle' (Block* pointer). Emit '->' for member access since
    # this runs after the static-pointer-dot pass.
    content, n = re.subn(r'(?<![:\w])stairSingle\s*\.', 'Block::s_stairSingle->', content); total += n
    content, n = re.subn(r'(?<![:\w])stairSingle\b', 'Block::s_stairSingle', content); total += n
    return content, total


def fix_tesselator_instance(content, fname):
    """'Tesselator::method(..)' -> 'Tesselator::getInstance().method(..)'.

    Tesselator is a singleton; every method except the static getInstance() is an
    instance method, but the decompiled source calls them with '::' (static) syntax.
    """
    # Skip Tesselator.cpp itself — those are the method DEFINITIONS, not calls.
    if not fname.endswith('.cpp') or fname == 'Tesselator.cpp':
        return content, 0
    content, n = re.subn(r'\bTesselator::(?!getInstance\b)(\w+)\(',
                         r'Tesselator::getInstance().\1(', content)
    return content, n


def fix_undeclared_count(content, fname):
    """Declare 'varN_count' length companions that have no allocation site (array came from a
    method/param/getBytes), so they compile. Inserted before first use as 'int varN_count = 0;'.
    NOTE: 0 is a compile placeholder — runtime length is lost for these (flagged; mostly NBT
    string writes). Only touches local 'var\\d+_count' names, never member m_/s_ companions.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    counts = set(re.findall(r'\b(var\d+_count)\b', content))
    undeclared = [c for c in counts if not re.search(r'\bint\s+%s\b' % re.escape(c), content)]
    if not undeclared:
        return content, 0
    lines = content.split('\n')
    total = 0
    for c in undeclared:
        for i, ln in enumerate(lines):
            if c in ln:
                indent = ln[:len(ln) - len(ln.lstrip())]
                lines.insert(i, '%sint %s = 0;' % (indent, c))
                total += 1
                break
    return '\n'.join(lines), total


def fix_world_length(content, fname):
    """World has a real 'length' FIELD, but the converter turns every '.length' into the
    array-length marker -> 'X_count'. For World vars (recognised by a nearby '->width'),
    'X_count' is really 'X->length'.
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    # World vars: the standard names worldObj/theWorld are always World*, plus any var
    # reached via ->width / ->height in this file.
    world_vars = set(['worldObj', 'theWorld'])
    for var in set(re.findall(r'(\w+)->width\b', content)) | set(re.findall(r'(\w+)->height\b', content)):
        world_vars.add(var)
    for var in world_vars:
        # World.length comes through as the marker, the plain '<var>_count' (post array-length),
        # or '<var>->length'. World's field is m_length (width/height are bare).
        new = content.replace('%s_count /* PORT: set this from allocation */' % var, '%s->m_length' % var)
        new = re.sub(r'\b%s_count\b' % re.escape(var), '%s->m_length' % var, new)
        new = re.sub(r'\b%s->length\b' % re.escape(var), '%s->m_length' % var, new)
        if new != content:
            content = new
            total += 1
    return content, total


def fix_map_iteration(content, fname):
    """Rewrite the generated values()/keySet() indexed loop into a real std::map iterator loop.

    The iterator pass emits, for a map:
        for (size_t _iN = 0; _iN < MAP->values().size(); ++_iN) {
            T* v = (T*)MAP->values()[_iN];
    which is invalid (std::map has no values()/[]). Convert to:
        for (std::map<std::string, void*>::iterator _iN = MAP->begin(); _iN != MAP->end(); ++_iN) {
            T* v = (T*)_iN->second;
    (keySet() -> ->first).
    """
    if not fname.endswith('.cpp'):
        return content, 0
    total = 0
    # for-header
    def hdr(m):
        idx, mapvar = m.group(1), m.group(2)
        return ('for (std::map<std::string, void*>::iterator %s = %s->begin(); '
                '%s != %s->end(); ++%s) {' % (idx, mapvar, idx, mapvar, idx))
    content, n = re.subn(
        r'for \(size_t (\w+) = 0; \1 < (\w+)->values\(\)\.size\(\); \+\+\1\) \{', hdr, content)
    total += n
    content, n = re.subn(
        r'for \(size_t (\w+) = 0; \1 < (\w+)->keySet\(\)\.size\(\); \+\+\1\) \{', hdr, content)
    total += n
    # element access inside such loops: MAP->values()[idx] -> idx->second ; keySet()[idx] -> idx->first
    content, n = re.subn(r'\b\w+->values\(\)\[(\w+)\]', r'\1->second', content); total += n
    content, n = re.subn(r'\b\w+->keySet\(\)\[(\w+)\]', r'\1->first', content); total += n
    return content, total


def fix_map_types(content, fname):
    """Java Map/HashMap stub types -> real std::map<std::string, void*>.

    'Map*'/'HashMap*' (currently empty stub classes) -> 'std::map<std::string, void*>*';
    'new HashMap()' -> 'new std::map<std::string, void*>()'. Safe: no game class is named
    Map/HashMap/TreeMap, so these tokens are always Java collections.
    """
    if not (fname.endswith('.h') or fname.endswith('.cpp')):
        return content, 0
    MAP = 'std::map<std::string, void*>'
    total = 0
    content, n = re.subn(r'\b(?:Map|HashMap|TreeMap|SortedMap|LinkedHashMap|AbstractMap)\s*\*',
                         MAP + '*', content); total += n
    content, n = re.subn(r'\bnew\s+(?:HashMap|TreeMap|LinkedHashMap)\s*(?:<[^>]*>)?\s*\(\s*\)',
                         'new ' + MAP + '()', content); total += n
    return content, total


def _split_top_comma(s):
    """Split 's' at the first top-level comma (ignoring commas inside parens/brackets/angles)."""
    depth = 0
    for i, ch in enumerate(s):
        if ch in '([<':
            depth += 1
        elif ch in ')]>':
            depth -= 1
        elif ch == ',' and depth == 0:
            return s[:i], s[i + 1:]
    return s, None


def fix_hashmap_put(content, fname):
    """Convert std::map operations on known map vars to real C++ std::map syntax.

    Map vars (MAP_VAR_NAMES, from parsed Java Map/HashMap fields) are modeled as
    std::map<std::string, void*>* pointers:
      map->put(k, v)      -> (*map)[k] = (void*)(v);
      map->get(k)         -> (*map)[k]
      map->containsKey(k) -> map->count(k)
      map->remove(k)      -> map->erase(k)
      map->isEmpty()      -> map->empty()
    Scoped to MAP_VAR_NAMES so vector .get/.remove are untouched.
    """
    if not fname.endswith('.cpp') or not MAP_VAR_NAMES:
        return content, 0
    total = 0
    lines = content.split('\n')
    result = []
    for line in lines:
        old = line
        # uncomment a PORT-TODO'd map put so we can convert it
        m_un = re.match(r'^(\s*)//\s*PORT-TODO:\s*(\w+->put\(.*)$', line)
        if m_un and m_un.group(2).split('->')[0] in MAP_VAR_NAMES:
            line = m_un.group(1) + m_un.group(2)
        # map->put(k, v) -> (*map)[k] = (void*)(v);
        pm = re.search(r'\b(\w+)->put\(', line)
        if pm and pm.group(1) in MAP_VAR_NAMES:
            mapvar = pm.group(1)
            start = pm.end()
            depth, i = 1, start
            while i < len(line) and depth > 0:
                if line[i] == '(':
                    depth += 1
                elif line[i] == ')':
                    depth -= 1
                i += 1
            args = line[start:i - 1]
            key, val = _split_top_comma(args)
            if val is not None:
                indent = line[:len(line) - len(line.lstrip())]
                line = '%s(*%s)[%s] = (void*)(%s);' % (indent, mapvar, key.strip(), val.strip())
        # scalar ops on map vars
        for mv in MAP_VAR_NAMES:
            if mv + '->' not in line:
                continue
            line = re.sub(r'\b%s->containsKey\(' % re.escape(mv), '%s->count(' % mv, line)
            line = re.sub(r'\b%s->isEmpty\(\)' % re.escape(mv), '%s->empty()' % mv, line)
            line = re.sub(r'\b%s->remove\(' % re.escape(mv), '%s->erase(' % mv, line)
            line = re.sub(r'\b%s->get\(' % re.escape(mv), '(*%s).operator[](' % mv, line)
        if line != old:
            total += 1
        result.append(line)
    return '\n'.join(result), total


def fix_iterator_pattern(content, fname):
    """Fix: Java Iterator pattern -> C++ indexed for loop.
    Iterator varN = collection.iterator();
    while(varN.hasNext()) {
        Type varM = (Type)varN.next();
    ->
    for (size_t _iN = 0; _iN < collection.size(); ++_iN) {
        Type* varM = (Type*)collection[_iN];
    """
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r'^(\s*)Iterator\s+(\w+)\s*=\s*(.+)\.iterator\(\)\s*;', line)
        if m and '// PORT' not in line:
            indent = m.group(1)
            iter_var = m.group(2)
            collection = m.group(3)
            # Scan forward past blank lines to find while(iterVar.hasNext())
            j = i + 1
            while j < len(lines) and lines[j].strip() == '':
                j += 1
            if j < len(lines):
                next_stripped = lines[j].strip()
                wm = re.match(r'while\s*\(\s*%s\.hasNext\(\)\s*\)\s*\{' % re.escape(iter_var), next_stripped)
                if wm:
                    # Scan forward past blank lines for cast line
                    k = j + 1
                    while k < len(lines) and lines[k].strip() == '':
                        k += 1
                    if k < len(lines):
                        next2 = lines[k].strip()
                        cm = re.match(r'(\w+)\s+(\w+)\s*=\s*\((\w+)\)\s*%s\.next\(\)\s*;' % re.escape(iter_var), next2)
                        if cm:
                            cast_type = cm.group(1)
                            cast_var = cm.group(2)
                            loop_idx = '_i%s' % iter_var.replace('var', '')
                            result.append('%sfor (size_t %s = 0; %s < %s.size(); ++%s) {' % (
                                indent, loop_idx, loop_idx, collection, loop_idx))
                            if cast_type in PF_REFERENCE_TYPES:
                                result.append('%s\t%s* %s = (%s*)%s[%s];' % (
                                    indent, cast_type, cast_var, cast_type, collection, loop_idx))
                            else:
                                result.append('%s\t%s %s = (%s)%s[%s];' % (
                                    indent, cast_type, cast_var, cast_type, collection, loop_idx))
                            changed += 1
                            i = k + 1  # Skip past all consumed lines
                            continue
                    # No cast — simpler loop
                    loop_idx = '_i%s' % iter_var.replace('var', '')
                    result.append('%sfor (size_t %s = 0; %s < %s.size(); ++%s) {' % (
                        indent, loop_idx, loop_idx, collection, loop_idx))
                    changed += 1
                    i = j + 1
                    continue
        result.append(line)
        i += 1
    return '\n'.join(result), changed


def fix_string_literal_arrow(content, fname):
    """Fix: string literals corrupted by prior dot-on-pointer-members.
    e.g., "random->splash" -> "random.splash" inside string literals."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # Fix "random->X" back to "random.X" inside string literals
    content = re.sub(r'"random->(\w+)"', r'"random.\1"', content)
    # Also fix return "random->hurt" etc
    content = re.sub(r'"random->(\w+)"', r'"random.\1"', content)
    if content != old:
        changed = 1
    return content, changed


def fix_deref_this_to_this(content, fname):
    """Fix: *this -> this in function calls expecting pointer args.
    worldObj->a(*this, ...) -> worldObj->a(this, ...)"""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    # *this in function argument position: func(*this, or func(*this)
    # But NOT: *this-> or *this. (those are member access)
    content = re.sub(r'\(\*this([,)])', r'(this\1', content)
    # Also mid-argument: , *this, or , *this)
    content = re.sub(r',\s*\*this([,)])', r', this\1', content)
    if content != old:
        changed = 1
    return content, changed


def fix_private_to_public(content, fname):
    """Make all class members public.

    Decompiled Minecraft routinely accesses 'private' fields across classes (the
    obfuscated originals were package-private, and subclasses read parent fields
    directly). Enforcing C++ access control only produces C2248 errors with no
    upside for a faithful port, so every 'private:' section becomes 'public:'.
    """
    if not fname.endswith('.h'):
        return content, 0
    if 'private:' not in content and 'protected:' not in content:
        return content, 0
    content, n1 = re.subn(r'(?m)^\tprivate:\s*$|^private:\s*$',
                          'public: // was private', content)
    # protected members are also accessed cross-class in the decompiled source
    # (e.g. Slot reads GuiContainer fields), so expose them too.
    content, n2 = re.subn(r'(?m)^\tprotected:\s*$|^protected:\s*$',
                          'public: // was protected', content)
    return content, n1 + n2


def fix_aabb_null_return(content, fname):
    """Fix: return NULL in functions returning AxisAlignedBB -> return AxisAlignedBB()."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    lines = content.split('\n')
    result = []
    in_aabb_func = False
    for line in lines:
        old = line
        # Detect AxisAlignedBB-returning function
        if re.match(r'AxisAlignedBB\s+\w+::\w+\s*\(', line):
            in_aabb_func = True
        elif re.match(r'\w', line) and not line.startswith('\t') and not line.startswith(' '):
            in_aabb_func = False

        if in_aabb_func and line.strip() == 'return NULL;':
            indent = line[:len(line) - len(line.lstrip())]
            line = indent + 'return AxisAlignedBB();'
            changed += 1
        result.append(line)
    return '\n'.join(result), changed


def fix_iworld_cast(content, fname):
    """Fix: (IWorldAccess)expr -> (IWorldAccess*)expr — missed by java-casts-to-ptr."""
    if not fname.endswith('.cpp'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\(IWorldAccess\)(?!\*)', '(IWorldAccess*)', content)
    content = re.sub(r'\(Slot\)(?!\*)', '(Slot*)', content)
    if content != old:
        changed = 1
    return content, changed


def fix_stub_header(output_dir):
    """Create any remaining missing stub headers."""
    # Stub classes get a default ctor, a '(void*)' ctor (callers do 'new GuiX(this)'
    # passing the parent screen), and a virtual dtor.
    stub_template = ('#ifndef %s_H\n#define %s_H\n\n// Stub — not present in decompiled source\n'
                     '// TODO: implement or remove references\n\nclass %s {\npublic:\n'
                     '\t%s() {}\n\t%s(void*) {}\n\tvirtual ~%s() {}\n};\n\n#endif\n')
    # Gui dialog stubs must derive from GuiScreen (they're passed to displayGuiScreen).
    gui_stub_template = ('#ifndef %s_H\n#define %s_H\n#include "GuiScreen.h"\n\n'
                         '// Stub — level-dialog .java skipped (network save/load, not on 360)\n'
                         'class %s : public GuiScreen {\npublic:\n'
                         '\t%s() {}\n\t%s(void*) {}\n\tvirtual ~%s() {}\n};\n\n#endif\n')
    stubs = {
        'ThreadDownloadResources.h': '#ifndef THREADDOWNLOADRESOURCES_H\n#define THREADDOWNLOADRESOURCES_H\n// Stub\n#endif\n',
        'GuiNewLevel.h': gui_stub_template % ('GUINEWLEVEL', 'GUINEWLEVEL', 'GuiNewLevel', 'GuiNewLevel', 'GuiNewLevel', 'GuiNewLevel'),
        'GuiLoadLevel.h': gui_stub_template % ('GUILOADLEVEL', 'GUILOADLEVEL', 'GuiLoadLevel', 'GuiLoadLevel', 'GuiLoadLevel', 'GuiLoadLevel'),
        'GuiSaveLevel.h': gui_stub_template % ('GUISAVELEVEL', 'GUISAVELEVEL', 'GuiSaveLevel', 'GuiSaveLevel', 'GuiSaveLevel', 'GuiSaveLevel'),
        'MinecraftError.h': stub_template % ('MINECRAFTERROR', 'MINECRAFTERROR', 'MinecraftError', 'MinecraftError', 'MinecraftError', 'MinecraftError'),
    }
    created = 0
    for name, content in stubs.items():
        path = os.path.join(output_dir, name)
        # Always (re)write so template changes take effect on a postfix-only cycle.
        with open(path, 'w', encoding='utf-8', errors='replace') as f:
            f.write(content)
        created += 1
    return created


def fix_object_in_headers(content, fname):
    """Fix: Object* -> void* in header files (Java Object has no C++ equivalent)."""
    if not fname.endswith('.h'):
        return content, 0
    changed = 0
    old = content
    content = re.sub(r'\bObject\*', 'void*', content)
    content = re.sub(r'\bObject\b(?!\s*\*)', 'void*', content)
    if content != old:
        changed = 1
    return content, changed


def fix_system_time(content, fname):
    """Postfix net: map any leftover System.currentTimeMillis() -> GetTickCount().
    The glColor handler emits its args verbatim, bypassing the conversion-phase
    System.* mapping, so the time call can survive inside a color() argument."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = content.count('System.currentTimeMillis()')
    if n:
        content = content.replace('System.currentTimeMillis()', 'GetTickCount()')
    return content, n


def fix_allowuserinput_prefix(content, fname):
    """GuiScreen's field is m_allowUserInput, but some subclasses use it bare
    ('allowUserInput = false') — the field-prefix pass missed the usage. (\\b won't
    match inside 'm_allowUserInput', so this won't double-prefix.)"""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'\ballowUserInput\b', 'm_allowUserInput', content)
    return new, n


def fix_craftingmanager_getinstance(content, fname):
    """CraftingManager::getInstance() returns a CraftingManager* (singleton pointer),
    so '.method' must be '->method'."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'CraftingManager::getInstance\(\)\.', 'CraftingManager::getInstance()->', content)
    return new, n


def fix_entityplayersp_include(content, fname):
    """'mc->thePlayer->dropPlayerItem(...)' needs EntityPlayerSP's complete type
    (thePlayer is EntityPlayerSP*); the auto-include doesn't trigger on chained access."""
    if not fname.endswith('.cpp') or '->dropPlayerItem' not in content:
        return content, 0
    if '#include "EntityPlayerSP.h"' in content or 'EntityPlayerSP' not in AVAILABLE_HEADERS:
        return content, 0
    for anchor in ['#include "Common.h"\n', '#include "stdafx.h"\n']:
        idx = content.find(anchor)
        if idx != -1:
            pos = idx + len(anchor)
            return content[:pos] + '#include "EntityPlayerSP.h"\n' + content[pos:], 1
    return content, 0


def fix_getitem_include(content, fname):
    """Add Item.h where '->getItem()->' is used. ItemStack::getItem() returns Item*,
    and calling a method through it needs Item's complete type, else MSVC reports
    C2027 'use of undefined type Item' + C2227. The converter's auto-include only
    triggers on pointer-decl/::/new, not on a chained method-return deref."""
    if not fname.endswith('.cpp'):
        return content, 0
    if '->getItem()->' not in content:
        return content, 0
    if '#include "Item.h"' in content:
        return content, 0
    if 'Item' not in AVAILABLE_HEADERS:
        return content, 0
    for anchor in ['#include "Common.h"\n', '#include "stdafx.h"\n']:
        idx = content.find(anchor)
        if idx != -1:
            pos = idx + len(anchor)
            return content[:pos] + '#include "Item.h"\n' + content[pos:], 1
    return content, 0


def fix_comparator_operator(content, fname):
    """Java Comparator classes (RecipeSorter/EntitySorter/RenderSorter) have int compare();
    std::sort needs a functor. Add an inline operator() that delegates to compare()."""
    if fname not in ('RecipeSorter.h', 'EntitySorter.h', 'RenderSorter.h'):
        return content, 0
    if 'operator()' in content:
        return content, 0
    m = re.search(r'(virtual int compare\([^;]*\);)', content)
    if not m:
        return content, 0
    inject = m.group(1) + '\n\tbool operator()(void* a, void* b) { return compare(a, b) < 0; }'
    return content.replace(m.group(1), inject, 1), 1


def fix_sort_calls(content, fname):
    """Java Collections.sort / Arrays.sort -> std::sort. Comparators become functors
    (see fix_comparator_operator); 'new Comp(args)' -> a 'Comp(args)' temporary."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = 0
    content, c = re.subn(
        r'Collections::sort\(\s*([^,()]+?)\s*,\s*new\s+(\w+)\(([^)]*)\)\s*\)',
        r'std::sort(\1.begin(), \1.end(), \2(\3))', content); n += c
    content, c = re.subn(
        r'Collections::sort\(\s*([^,()]+?)\s*\)',
        r'std::sort(\1.begin(), \1.end())', content); n += c
    content, c = re.subn(
        r'Arrays::sort\(\s*([^,()]+?)\s*,\s*new\s+(\w+)\(([^)]*)\)\s*\)',
        r'std::sort(\1, \1 + \1_count, \2(\3))', content); n += c
    return content, n


def fix_addrecipe_default_count(content, fname):
    """CraftingManager::addRecipe(ItemStack*, void*, int count) is called everywhere with
    only 2 args (the varargs recipe pattern is PORT-stubbed to NULL), so give the count
    a default. Compile-level fix; the recipe grid data itself remains a runtime PORT stub."""
    if fname != 'CraftingManager.h':
        return content, 0
    new = content.replace(
        'void addRecipe(ItemStack* var1, void* var2, int var2_count);',
        'void addRecipe(ItemStack* var1, void* var2, int var2_count = 0);')
    return new, (1 if new != content else 0)


def fix_entity_doublenbtlist(content, fname):
    """Entity.newDoubleNBTList(double...) is varargs. The converter hoists each call's
    args into a fixed-size local `float _tmpVertsN[count] = {...}` but the call passes only
    the array (1 arg) while the signature takes (float* var0, int var0_count), and the body
    hardcodes the element count to 4. Pass the real array length at each call site and have
    the body iterate var0_count instead of 4 (fixes both the arg-count compile error and the
    out-of-bounds read on the size-2 Rotation array)."""
    if fname != 'Entity.cpp':
        return content, 0
    count = 0
    sizes = {}
    for m in re.finditer(r'\bfloat\s+(_tmpVerts\d+)\[(\d+)\]\s*=', content):
        sizes[m.group(1)] = m.group(2)
    def repl_call(mm):
        arr = mm.group(1)
        n = sizes.get(arr)
        return mm.group(0) if not n else 'newDoubleNBTList(%s, %s)' % (arr, n)
    new = re.sub(r'newDoubleNBTList\((_tmpVerts\d+)\)', repl_call, content)
    if new != content:
        count += 1
        content = new
    new = content.replace(
        'int var2 = 4 /* PORT: quad vertex count */;',
        'int var2 = var0_count;')
    if new != content:
        count += 1
        content = new
    return content, count


def fix_savedmatrix_redef(content, fname):
    """Two glPushMatrix in the same scope both emit 'D3DXMATRIX _savedMatrix = ...;'
    -> C2374 redefinition. Keep the first decl per scope; convert later SAME-scope decls
    to a plain assignment (sequential push/pop pairs safely reuse the saved matrix).
    Nested-scope re-decls are legal C++ shadowing and are left alone (brace-tracked)."""
    if not fname.endswith('.cpp'):
        return content, 0
    scope_decl = [set()]
    count = 0
    out = []
    for line in content.split('\n'):
        m = re.match(r'(\s*)D3DXMATRIX _savedMatrix = (.*)$', line)
        if m:
            if '_savedMatrix' in scope_decl[-1]:
                line = '%s_savedMatrix = %s' % (m.group(1), m.group(2))
                count += 1
            else:
                scope_decl[-1].add('_savedMatrix')
        for ch in line:
            if ch == '{':
                scope_decl.append(set())
            elif ch == '}' and len(scope_decl) > 1:
                scope_decl.pop()
        out.append(line)
    return '\n'.join(out), count


def fix_textures_scope_prefix(content, fname):
    """'obj->Textures::load(...)' / 'obj.Textures::load(...)' — Textures is a static
    registry class, so an instance prefix is always wrong (C2039 'Textures is not a
    member of Minecraft/RenderEngine'). Strip the prefix to leave 'Textures::load(...)'."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'\b\w+(?:->|\.)Textures::', 'Textures::', content)
    return new, n


def fix_memset_vector_clear(content, fname):
    """Java list.clear() was mis-ported to memset() on a std::vector (C2664). Detect names
    used with vector methods in this file and convert memset(name,0,sizeof(name))->name.clear().
    A real C array used with memset is never also used with .push_back/.begin/etc., so this
    only rewrites genuine vectors."""
    if not fname.endswith('.cpp'):
        return content, 0
    veclike = set(re.findall(r'\b(\w+)\.(?:push_back|begin|end|size|clear|empty|erase)\s*\(', content))
    if not veclike:
        return content, 0
    def repl(m):
        return (m.group(1) + '.clear()') if m.group(1) in veclike else m.group(0)
    new, n = re.subn(r'memset\(\s*(\w+)\s*,\s*0\s*,\s*sizeof\(\s*\1\s*\)\s*\)', repl, content)
    return new, n


def fix_default_vector_init(content, fname):
    """'X = /* default vector */;' (Java 'X = new ArrayList()') leaves an empty RHS
    (C2059). X is a default-constructed std::vector member -> 'X.clear();' re-inits it."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'([\w.>()\[\]:_-]+?)\s*=\s*/\* default vector \*/',
                     r'\1.clear()', content)
    return new, n


def fix_arrays_fill(content, fname):
    """Java Arrays.fill(arr, value) -> std::fill over the tracked array + its _count companion."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'Arrays::fill\(\s*(\w+)\s*,\s*([^)]+)\)',
                     r'std::fill(\1, \1 + \1_count, \2)', content)
    return new, n


def fix_system_gc(content, fname):
    """Java System.gc() (manual GC hint) has no Xbox equivalent -> no-op."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'\bSystem\.gc\(\)', '(void)0', content)
    return new, n


def fix_tracked_size_marker(content, fname):
    """A mis-ported Java 'arr.length' / 'arr[i].length' left as
    'expr /* PORT: array length - use tracked size */' (compared to an int -> C2446).
    Replace with the base array's tracked '<base>_count' companion."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(
        r'(\w+)(?:\[[^\]]*\])?\s*/\* PORT: array length - use tracked size \*/',
        r'\1_count', content)
    return new, n


def fix_array_mlength_marker(content, fname):
    """'new T[<wrong> /* PORT: array m_length - use tracked size */]' — the converter put a
    bogus size expr (a data pointer) for a width*length level array. Use width * m_length."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'\[\s*\w+\s*/\* PORT: array m_length - use tracked size \*/\s*\]',
                     '[width * m_length]', content)
    return new, n


def fix_2d_array_new(content, fname):
    """'new void*[][]' (empty-dim 2D new from Java Object[][] init) is invalid C++.
    The recipe-pattern data is PORT-stubbed, so initialize the pointer to NULL."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'new\s+\w[\w:]*\s*\*?\s*(?:/\*.*?\*/\s*)?\[\]\[\]', 'NULL', content)
    return new, n


def fix_value_arrow_to_dot_wholefile(content, fname):
    """Inverse of the pointer dot->arrow supplement, for value types accessed with '->'
    (MSVC C2819 'type X does not have operator ->'). A name declared 'AxisAlignedBB name'
    (value) and never 'AxisAlignedBB* name' (pointer) anywhere in the file is a value
    file-wide -> rewrite 'name->member' to 'name.member'. Only AxisAlignedBB for now
    (its by-value params/locals are the dominant C2819 cluster); nullable value-or-null
    types (Vec3D, ItemStack, ...) are pointers and excluded."""
    if not fname.endswith('.cpp'):
        return content, 0
    count = 0
    for vt in ('AxisAlignedBB',):
        val_names = set(re.findall(r'\b%s\s+(\w+)\s*[;=,)]' % vt, content))
        ptr_names = set(re.findall(r'\b%s\s*\*\s*(\w+)' % vt, content))
        for nm in (val_names - ptr_names):
            new, n = re.subn(r'\b%s->' % re.escape(nm), nm + '.', content)
            if n:
                count += n
                content = new
    return content, count


def fix_entityplayer_camerapitch(content, fname):
    """The parser captured EntityPlayer's m_cameraYaw but dropped the adjacent cameraPitch
    field (used in the cloak-swing update). Declare it and zero-init it in the ctors."""
    if fname == 'EntityPlayer.h':
        new = content.replace('float m_cameraYaw;',
                              'float m_cameraYaw;\n\tfloat cameraPitch;', 1)
        return new, (1 if new != content else 0)
    if fname == 'EntityPlayer.cpp':
        new = content.replace(', m_cameraYaw(0)', ', m_cameraYaw(0)\n\t, cameraPitch(0)')
        return new, (1 if new != content else 0)
    return content, 0


def fix_world_ticklist_type(content, fname):
    """World::m_tickList holds NextTickListEntry (values) but was modeled as vector<void*>.
    Retype it so the remove-by-index reconstruction ('NextTickListEntry v = m_tickList[i]')
    and var8.member accesses compile. NextTickListEntry is a small value struct (no cycle)."""
    if fname == 'World.h':
        new = content.replace(
            'std::vector<void*> /* PORT: determine element type */ m_tickList',
            'std::vector<NextTickListEntry> m_tickList')
        if new != content and '#include "NextTickListEntry.h"' not in new:
            new = new.replace('#define WORLD_H',
                              '#define WORLD_H\n#include "NextTickListEntry.h"', 1)
        return new, (1 if new != content else 0)
    if fname == 'World.cpp':
        if 'm_tickList' in content and '#include "NextTickListEntry.h"' not in content:
            for anchor in ['#include "Common.h"\n', '#include "stdafx.h"\n']:
                idx = content.find(anchor)
                if idx != -1:
                    pos = idx + len(anchor)
                    return content[:pos] + '#include "NextTickListEntry.h"\n' + content[pos:], 1
        return content, 0
    return content, 0


def fix_treeset(content, fname):
    """Java TreeSet<Integer> (sorted set of packed positions, e.g. createExplosion) ->
    std::set<int>: 'new TreeSet()' -> 'new std::set<int>()', set-var push_back -> insert,
    'list.addAll(set)' -> range insert, and the receiving vector + Integer.intValue() ->
    plain int element access."""
    if not fname.endswith('.cpp') or 'TreeSet' not in content:
        return content, 0
    n = 0
    content, c = re.subn(r'\bTreeSet\b', 'std::set<int>', content); n += c
    for v in set(re.findall(r'std::set<int>\s*\*\s*(\w+)', content)):
        content = content.replace('%s->push_back(' % v, '%s->insert(' % v)
    # vectors that receive addAll(set) hold ints -> retype + drop Integer.intValue()
    for v in set(re.findall(r'(\w+)\.addAll\(\w+\)', content)):
        content = re.sub(r'std::vector<void\*>(\s+%s\b)' % re.escape(v),
                         r'std::vector<int>\1', content)
    content, c = re.subn(r'(\w+)\.addAll\((\w+)\)',
                         r'\1.insert(\1.end(), \2->begin(), \2->end())', content); n += c
    content, c = re.subn(r'->intValue\(\)', '', content); n += c
    return content, n


def fix_aabb_vector_element(content, fname):
    """For vars typed std::vector<AxisAlignedBB> (value elements, e.g. from
    getCollidingBoundingBoxes), element access 'v[i]->member' must be 'v[i].member'.
    Runs AFTER the colliding-bb retype so the vars are already vector<AxisAlignedBB>."""
    if not fname.endswith('.cpp') or 'std::vector<AxisAlignedBB>' not in content:
        return content, 0
    n = 0
    for nm in set(re.findall(r'\bstd::vector<AxisAlignedBB>\s+(\w+)', content)):
        content, c = re.subn(r'\b%s(\[[^\]]*\])->' % re.escape(nm), r'%s\1.' % nm, content)
        n += c
    return content, n


def fix_colliding_bb_type(content, fname):
    """World::getCollidingBoundingBoxes builds/returns a list of AxisAlignedBB values, but
    was modeled as vector<void*> (so 'var2.push_back(aabb)' fails). Retype the method, its
    local accumulator, and the caller locals to vector<AxisAlignedBB>."""
    n = 0
    if fname in ('World.h', 'World.cpp'):
        content, c = re.subn(
            r'std::vector<void\*> /\* PORT: determine element type \*/\s*(World::)?getCollidingBoundingBoxes',
            r'std::vector<AxisAlignedBB> \1getCollidingBoundingBoxes', content)
        n += c
    if fname == 'World.cpp':
        # the local accumulator var2 (comment trails the name) inside that method
        content, c = re.subn(r'std::vector<void\*>\s+var2\s*/\* PORT: determine element type \*/',
                             'std::vector<AxisAlignedBB> var2', content)
        n += c
    if fname == 'Entity.cpp':
        content, c = re.subn(
            r'std::vector<void\*> /\* PORT: type \*/ (\w+) = (\w[\w>-]*->getCollidingBoundingBoxes)',
            r'std::vector<AxisAlignedBB> \1 = \2', content)
        n += c
    return content, n


def fix_entitymap_remove(content, fname):
    """EntityMap has a remove(Entity*) method; the generic vector-remove pass mis-generated
    'm_entityMap->erase(.begin() + e)' on it. Use the real remove()."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(
        r'm_entityMap->erase\(\.begin\(\)\s*\+\s*(\w+)\)\s*/\*\s*PORT:\s*verify remove semantics\s*\*/',
        r'm_entityMap->remove(\1)', content)
    return new, n


def fix_worldaccess_remove_iterator(content, fname):
    """m_worldAccesses was retyped to vector<IWorldAccess*>, but its remove-by-value
    find/erase still declares a hardcoded vector<void*>::iterator. Match the container."""
    if not fname.endswith('.cpp'):
        return content, 0
    new, n = re.subn(r'std::vector<void\*>::iterator (_removeIt = std::find\(m_worldAccesses)',
                     r'std::vector<IWorldAccess*>::iterator \1', content)
    return new, n


def fix_world_tileentity_map(content, fname):
    """World::m_map (TileEntity by packed-int position) was keyed by std::string, but it's
    used with int keys and Java Map.put(). Retype to map<int,void*> and put()->[]=."""
    if fname == 'World.h':
        new = content.replace('std::map<std::string, void*>* m_map',
                              'std::map<int, void*>* m_map')
        return new, (1 if new != content else 0)
    if fname == 'World.cpp':
        new, n = re.subn(r'm_map->put\(([^,]+),\s*([^)]+)\)', r'(*m_map)[\1] = \2', content)
        return new, n
    return content, 0


def fix_world_worldaccess_type(content, fname):
    """World::m_worldAccesses holds IWorldAccess* (RenderGlobal etc.) but was modeled as
    vector<void*>, so m_worldAccesses[i]->loadRenderers() fails. Retype to vector<IWorldAccess*>
    (World.h already forward-declares IWorldAccess; World.cpp already includes it)."""
    if fname != 'World.h':
        return content, 0
    new = content.replace(
        'std::vector<void*> /* PORT: determine element type */ m_worldAccesses',
        'std::vector<IWorldAccess*> m_worldAccesses')
    return new, (1 if new != content else 0)


_BLOCK_STATIC_HEADERS = {'s_grass': 'BlockGrass', 's_fire': 'BlockFire'}


def fix_blockgrass_include(content, fname):
    """Block::s_grass / s_fire are typed BlockGrass* / BlockFire*; '->member' through them
    needs the subclass's complete type. Add the header where the static is dereferenced."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = 0
    for stat, hdr in _BLOCK_STATIC_HEADERS.items():
        if ('Block::%s->' % stat) not in content:
            continue
        if ('#include "%s.h"' % hdr) in content or hdr not in AVAILABLE_HEADERS:
            continue
        for anchor in ['#include "Common.h"\n', '#include "stdafx.h"\n']:
            idx = content.find(anchor)
            if idx != -1:
                pos = idx + len(anchor)
                content = content[:pos] + ('#include "%s.h"\n' % hdr) + content[pos:]
                n += 1
                break
    return content, n


def fix_reflection_stub(content, fname):
    """Java reflection (Class / getClass / isAssignableFrom) has no C++03 equivalent.
    Stub: '<class>->isAssignableFrom(<entity>->getClass())' -> 'true', and Class* -> void*.
    NOTE: this makes entitiesInLevelList count ALL entities for any class query (a Phase-3
    mob-cap nicety), but it compiles and links."""
    if not (fname.endswith('.cpp') or fname.endswith('.h')):
        return content, 0
    n = 0
    if fname.endswith('.cpp'):
        content, c = re.subn(r'\w+->isAssignableFrom\([^()]*->getClass\(\)\)', 'true', content)
        n += c
    content, c = re.subn(r'\bClass\s*\*', 'void*', content)
    n += c
    return content, n


def fix_world_entity_query(content, fname):
    """World::a(Entity*, AABB) delegates to EntityMap::a (now vector<Entity*>) but was
    typed vector<void*>, so callers' 'std::vector<Entity*> v = worldObj->a(...)' mismatch
    (C2440). Retype ONLY that method (getCollidingBoundingBoxes is an AABB list, left alone)."""
    if fname not in ('World.h', 'World.cpp'):
        return content, 0
    n = 0
    out = []
    for line in content.split('\n'):
        if 'a(Entity*' in line and 'std::vector<void*> /* PORT: determine element type */' in line:
            line = line.replace('std::vector<void*> /* PORT: determine element type */',
                                'std::vector<Entity*>')
            n += 1
        out.append(line)
    return '\n'.join(out), n


def fix_entitymap_element_type(content, fname):
    """EntityMap's grid/entity lists and a() overloads hold Entity* but were modeled as
    std::vector<void*> (element type undetermined). Scoped to EntityMap.{h,cpp}, retype
    to vector<Entity*> so the entity-in-AABB queries assign cleanly to callers' declared
    std::vector<Entity*> locals (EntityArrow, RenderGlobal)."""
    if fname not in ('EntityMap.h', 'EntityMap.cpp'):
        return content, 0
    n = content.count('std::vector<void*>')
    if n:
        content = content.replace('std::vector<void*>', 'std::vector<Entity*>')
    return content, n


def fix_controllist_element_type(content, fname):
    """GuiScreen::controlList only ever holds GuiButton*/GuiSmallButton* (push_back of
    new GuiButton/GuiSmallButton; GuiSmallButton derives from GuiButton), but the
    converter modeled it as std::vector<void*>, so controlList[i]->member fails. Type
    it as vector<GuiButton*>. (All call sites already include GuiButton.h; the only
    element cast present is (GuiButton*)controlList[i], which stays valid.)"""
    if not fname.endswith('.h'):
        return content, 0
    new = content.replace(
        'std::vector<void*> /* PORT: determine element type */ controlList;',
        'std::vector<GuiButton*> controlList;')
    return new, (1 if new != content else 0)


def fix_tesselator_getinstance_ptr(content, fname):
    """Postfix: 'Tesselator* x = Tesselator::getInstance();' assigns a reference to a
    pointer (C2440). getInstance() returns Tesselator&, so take its address. Only the
    pointer-init form is touched — the 'Tesselator var = getInstance();' value form in
    other files is left alone (changing it to a reference would break reassignments)."""
    if not fname.endswith('.cpp'):
        return content, 0
    new = re.sub(r'(Tesselator\s*\*\s*[\w:]+\s*=\s*)Tesselator::getInstance\(\)',
                 r'\1&Tesselator::getInstance()', content)
    return new, (1 if new != content else 0)


def fix_m_textures(content, fname):
    """Postfix: stray 'm_' prefix on the Textures registry class
    (m_Textures::load -> Textures::load). 'Textures' is a static class, never a
    field, so this rewrite is unambiguously safe."""
    if not fname.endswith('.cpp'):
        return content, 0
    n = content.count('m_Textures::')
    if n:
        content = content.replace('m_Textures::', 'Textures::')
    return content, n


def fix_sound_manager_include(content, fname):
    """Postfix: ensure SoundManager.h is included where a SoundManager pointer is
    dereferenced (e.g. mc->m_sndManager->playSound(...)), which needs the complete
    type. The converter only auto-includes on pointer-decl/::/new triggers, not on
    chained member access, so this fills that gap."""
    if not fname.endswith('.cpp'):
        return content, 0
    if 'm_sndManager->' not in content:
        return content, 0
    if '#include "SoundManager.h"' in content:
        return content, 0
    if 'SoundManager' not in AVAILABLE_HEADERS:
        return content, 0
    for anchor in ['#include "Common.h"\n', '#include "stdafx.h"\n']:
        idx = content.find(anchor)
        if idx != -1:
            pos = idx + len(anchor)
            content = content[:pos] + '#include "SoundManager.h"\n' + content[pos:]
            return content, 1
    return content, 0


# Derived from the parsed Java classes during run_conversion (see populate call).
# PTR_FIELD_NAMES:  m_/s_-prefixed field names whose Java type maps to a pointer.
# PTR_METHOD_NAMES: method names whose Java return type maps to a pointer.
# Used by the type-aware dot->arrow passes to fix 'ptrExpr.member' -> 'ptrExpr->member'.
PTR_FIELD_NAMES = set()
PTR_METHOD_NAMES = set()

# Basenames of all .h files present in the output dir (set at the start of run_postfix).
# Used by fix_missing_includes to auto-add headers for types whose complete definition
# is used in a .cpp (pointer decl, '::' scope, or 'new').
AVAILABLE_HEADERS = set()
ENUM_TYPE_NAMES = set()   # names of Java-enum classes (generated as `struct`, see fix_enum_forward_decls)

# ---- Hand-written PORT overrides ----------------------------------------------
# A few subsystems are irredeemably Java-specific platform glue (AWT/NIO texture
# managers, web skin download, etc.) that cannot be transpiled into working C++03.
# For these, a hand-written file lives in handwritten/ and is copied over the
# converter's output as the LAST step (after postfix, before line-ending fixup),
# so it survives regeneration and is never mangled by postfix passes. Each override
# mirrors the converter-generated interface so all converted callers keep working.
# Put ONLY the .cpp in handwritten/ when the generated .h is fine to keep; put both
# when the header also needs hand authoring.
HANDWRITTEN_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'handwritten')
PORT_OVERRIDES = {
    'RenderEngine',   # AWT/NIO texture manager -> thin shim over the Textures registry
    'XactAudio',      # poured-in XACT3 audio engine (reference port), renamed to avoid
                      # clashing with the converted `class SoundManager`
    'SoundManager',   # bridge: converted SoundManager.h interface -> XACT3 free functions
    'SoundPool',      # vestigial under XACT3 (Paul Lamb pool) -> no-op stub
    'FontRenderer',   # AWT font-pixel scan + GL display lists -> DrawPrimitiveUP quads
    'RenderManager',  # Java reflection entity->renderer Class map -> dynamic_cast dispatch
    'CraftingManager',# irredeemable Java varargs Object[] recipe parser -> stub (recipe
                      # data is itself PORT-stubbed); crafting is a Phase-3 runtime stub
    # --- Platform I/O: STFS virtual filesystem + java.io bridge (World save/load) ---
    'JavaIO',         # InputStream/OutputStream/File/{File,GZIP,Data}{Input,Output}Stream
    'InputStream',    # -> include JavaIO.h (overrides the generated stub)
    'OutputStream',   # -> include JavaIO.h (overrides the generated stub)
    'File',           # -> include JavaIO.h (java.io.File on the mounted drive)
    'Save360',        # STFS save-container mount (pulled from reference): mounts "Save:\\"
    'MouseHelper',    # AWT/LWJGL mouse capture -> trivial look-delta holder (XInput right stick
                      # writes m_deltaX/m_deltaY from the input layer)
    'Main',           # the WIRED Xbox 360 entry point (D3D9 init + ApplyDeviceDefaults + shaders +
                      # textures + MathHelper/World/Block/Item::initStatics in dep order + new
                      # Minecraft(...) + run loop). Replaces _generate_main_cpp's unwired stub; that
                      # stub is suppressed when this override exists (see _generate_main_cpp).
}


def apply_port_overrides(output_dir):
    """Copy hand-written PORT-override files from handwritten/ into output_dir.

    Only files whose basename is in PORT_OVERRIDES are copied, and only if present
    in handwritten/. Returns the number of files copied. Run this AFTER postfix so
    the hand-written content is authoritative.
    """
    if not os.path.isdir(HANDWRITTEN_DIR):
        return 0
    n = 0
    for fn in sorted(os.listdir(HANDWRITTEN_DIR)):
        if not (fn.endswith('.cpp') or fn.endswith('.h')):
            continue
        if os.path.splitext(fn)[0] not in PORT_OVERRIDES:
            continue
        with open(os.path.join(HANDWRITTEN_DIR, fn), 'r', encoding='utf-8', errors='replace') as f:
            data = f.read()
        with open(os.path.join(output_dir, fn), 'w', encoding='utf-8') as f:
            f.write(data)
        log.info('  PORT override: %s', fn)
        n += 1
    return n


def apply_profile_overrides(output_dir, profile_name):
    """Copy every .cpp/.h from handwritten_<profile_name>/ over the output. These are per-version,
    hand-ported platform/render classes the converter can't transpile (e.g. handwritten_alpha/
    RenderEngine.cpp -- a shim over the Textures registry). The directory's presence is the gate (no
    PORT_OVERRIDES list); copied last so they win over generation + postfix. Returns the count."""
    d = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'handwritten_' + profile_name)
    if not os.path.isdir(d):
        return 0
    n = 0
    for fn in sorted(os.listdir(d)):
        if not (fn.endswith('.cpp') or fn.endswith('.h')):
            continue
        with open(os.path.join(d, fn), 'r', encoding='utf-8', errors='replace') as f:
            data = f.read()
        with open(os.path.join(output_dir, fn), 'w', encoding='utf-8') as f:
            f.write(data)
        log.info('  %s override: %s', profile_name, fn)
        n += 1
    return n

# Field/var names whose Java type is a Map (HashMap/TreeMap/etc.), so their ->get/->put/
# ->remove convert to std::map ops rather than the vector forms. Populated from parsed classes.
MAP_VAR_NAMES = set()
_JAVA_MAP_TYPES = {'Map', 'HashMap', 'TreeMap', 'SortedMap', 'LinkedHashMap', 'AbstractMap'}

# Method names that must never be treated as pointer-returning even if some class
# declares them so — they are value getters/singletons (often provided by templated
# types like Tesselator that the parser never sees) and rewriting '.' to '->' breaks them.
PTR_METHOD_BLACKLIST = {'toString', 'getKey', 'clone', 'getInstance'}


def populate_pointer_name_sets(classes):
    """Fill PTR_FIELD_NAMES / PTR_METHOD_NAMES from parsed classes for the dot->arrow passes.

    A name is only included if it is UNAMBIGUOUS — pointer-typed everywhere it occurs and
    never value-typed in any class. A name used with both pointer and value meaning is
    excluded, since a global text rewrite cannot tell the two uses apart.
    """
    PTR_FIELD_NAMES.clear()
    PTR_METHOD_NAMES.clear()
    MAP_VAR_NAMES.clear()
    for cls in classes:
        for f in cls.fields:
            base = re.sub(r'<.*>', '', f.type_name).strip()
            if base in _JAVA_MAP_TYPES:
                MAP_VAR_NAMES.add(field_cpp_name(f.name, f.is_static))
    ptr_fields, val_fields = set(), set()
    ptr_methods, val_methods = set(), set()
    for cls in classes:
        for f in cls.fields:
            if f.is_array:
                continue
            cpp = field_cpp_name(f.name, f.is_static)
            if not (cpp.startswith('m_') or cpp.startswith('s_')):
                continue  # bare public-API names risk colliding with locals
            (ptr_fields if is_reference_type(f.type_name) else val_fields).add(cpp)
        for m in cls.methods:
            if m.is_constructor or not m.return_type:
                continue
            rt = m.return_type.strip()
            if rt.endswith('[]') or m.name in PTR_METHOD_BLACKLIST:
                continue
            (ptr_methods if is_reference_type(rt) else val_methods).add(m.name)
    PTR_FIELD_NAMES.update(ptr_fields - val_fields)
    PTR_METHOD_NAMES.update(ptr_methods - val_methods)


def fix_ptr_field_dot(content, fname):
    """Convert '.<m_field>' / '->m_field' followed by '.' to '->' when m_field is a pointer field.

    Catches 'worldObj->m_playerEntity.method()' (a pointer field reached with '.').
    Only m_/s_-prefixed names from PTR_FIELD_NAMES are touched, so locals are safe.
    """
    if not fname.endswith('.cpp') or not PTR_FIELD_NAMES:
        return content, 0
    total = 0
    for fld in PTR_FIELD_NAMES:
        # the field, then a dot that is member access (not a float, not ->, not .cpp/.h)
        pat = re.compile(r'\b%s\.(?!cpp\b|h\b|\d)' % re.escape(fld))
        content, n = pat.subn('%s->' % fld, content)
        total += n
    return content, total


def fix_ptr_method_dot(content, fname):
    """Convert 'expr.method(args).' / 'expr->method(args).' to '...->' when method returns a pointer.

    Catches 'var9.getItem().getItemStackLimit()' (pointer-returning getter reached, then '.').
    Args must contain no nested parens (covers the common getter forms).
    """
    if not fname.endswith('.cpp') or not PTR_METHOD_NAMES:
        return content, 0
    total = 0
    for meth in PTR_METHOD_NAMES:
        pat = re.compile(r'(\b%s\s*\([^()]*\))\.(?!\d)' % re.escape(meth))
        content, n = pat.subn(r'\1->', content)
        total += n
    return content, total


def fix_tesselator_fwddecl(content, fname):
    """HEADER fix: the converter renames Tessellator->Tesselator in code + includes, but missed forward
    declarations -> headers carry `class Tessellator;` (two L) while using `Tesselator*` (one L) -> C2061;
    and some (WorldRenderer.h) use `Tesselator*` in a member with NO decl at all -> C4430. Rename the
    stale forward decl, and inject `class Tesselator;` when the type is used but undeclared/unincluded."""
    if not fname.endswith('.h'):
        return content, 0
    changed = 0
    new = content.replace('class Tessellator;', 'class Tesselator;')
    if new != content:
        changed += 1
    if (re.search(r'\bTesselator\b', new) and 'class Tesselator;' not in new
            and '#include "Tesselator.h"' not in new):
        m = re.search(r'#define \w+_H[^\n]*\n', new)
        if m:
            new = (new[:m.end()] + '\nclass Tesselator;  // PORT: fwd decl (used as Tesselator* member)\n'
                   + new[m.end():])
            changed += 1
    return new, changed


def fix_tessellator_include(content, fname):
    """`#include "Tessellator.h"` -> `"Tesselator.h"` (the class is renamed Tessellator->Tesselator and the
    platform template is Tesselator.h). The postprocess rename only reaches pure-logic files; structural
    platform files (the *FX particles) bypass it, so catch them universally here."""
    new = content.replace('#include "Tessellator.h"', '#include "Tesselator.h"')
    return new, (1 if new != content else 0)


def run_postfix(output_dir):
    """Run all postfix error-reduction passes on the output directory."""
    log = logging.getLogger('java2xbox360')

    fixes = [
        ('vec3-0f', fix_vec3_0f),
        ('bare-zero-port', fix_bare_zero_port),
        ('3.0f-strings', fix_3_0f_in_strings),
        ('iterator-pattern', fix_iterator_pattern),  # Must run before java-casts-to-ptr
        ('instanceof', fix_instanceof),
        ('label-gotos', fix_label_gotos),
        ('buffered-io', fix_buffered_io),
        ('broken-port-line', fix_broken_port_line),
        ('dot-class', fix_dot_class),
        ('java-casts-to-ptr', fix_java_casts_to_ptr),
        ('new-without-pointer', fix_new_without_pointer),
        ('ptr-to-value-locals', fix_ptr_to_value_locals),
        ('cast-ptr-to-value-local', fix_cast_ptr_to_value_local),
        ('arrow-on-value-type', fix_arrow_on_value_type),
        ('boundingbox-arrow', fix_boundingbox_arrow),
        ('this-assignment', fix_this_assignment),
        ('new-dot-method', fix_new_dot_method),
        ('entity-null-cast', fix_entity_null_cast),
        ('nbt-float-access', fix_nbt_float_access),
        ('nbt-short-access', fix_nbt_short_access),
        ('ptr-local-dot-to-arrow', fix_ptr_local_dot_to_arrow),
        ('ptr-local-dot-to-arrow-wholefile', fix_ptr_local_dot_to_arrow_wholefile),
        ('per-function-value-access', fix_per_function_value_access),
        ('ptr-param-dot-to-arrow', fix_ptr_param_dot_to_arrow),
        ('this-dot', fix_this_dot),
        ('rendermanager-dot', fix_rendermanager_dot),
        ('dot-on-pointer-members', fix_dot_on_pointer_members),
        ('ptr-field-dot', fix_ptr_field_dot),
        ('ptr-method-dot', fix_ptr_method_dot),
        ('java-array-init', fix_java_array_init),
        ('java-array-decl', fix_java_array_decl),
        ('array-length', fix_array_length),
        ('new-value-struct', fix_new_value_struct),
        ('raw-container-template', fix_raw_container_template),
        ('itemstack-array', fix_itemstack_array),
        ('string-concat', fix_string_concat),
        ('unsigned-shift', fix_unsigned_shift),
        ('broken-port-valueof', fix_broken_port_valueof),
        ('block-dot-static', fix_block_dot_static),
        ('missing-includes', fix_missing_includes),
        ('character-valueof', fix_character_valueof),
        ('string-in-body', fix_string_in_body),
        ('math-references', fix_math_references),
        ('integer-parseint', fix_integer_parseint),
        ('m-pi-define', fix_m_pi_define),
        ('vector-remove', fix_vector_remove),
        ('tochararray', fix_tochararray),
        ('map-types', fix_map_types),
        ('world-length', fix_world_length),
        ('undeclared-count', fix_undeclared_count),
        ('java-string-api', fix_java_string_api),
        ('java-constants', fix_java_constants),
        ('nbt-string-bytes', fix_nbt_string_bytes),
        ('vector-element-ptr', fix_vector_element_ptr),
        ('cast-result-dot', fix_cast_result_dot),
        ('redundant-nanotime', fix_redundant_nanotime),
        ('near-passing-misc', fix_near_passing_misc),
        ('tesselator-instance', fix_tesselator_instance),
        ('map-iteration', fix_map_iteration),
        ('hashmap-put', fix_hashmap_put),
        ('string-literal-arrow', fix_string_literal_arrow),
        ('deref-this-to-this', fix_deref_this_to_this),
        ('private-to-public', fix_private_to_public),
        ('aabb-null-return', fix_aabb_null_return),
        ('iworld-cast', fix_iworld_cast),
        ('object-in-headers', fix_object_in_headers),
        ('system-time', fix_system_time),
        ('tesselator-getinstance-ptr', fix_tesselator_getinstance_ptr),
        ('tesselator-value-arg', fix_tesselator_value_arg),
        ('double-suffix', fix_double_suffix),
        ('effectrenderer-fxlayers', fix_effectrenderer_fxlayers),
        ('guiingame-includes', fix_guiingame_includes),
        ('guiingame-chatlist', fix_guiingame_chatlist),
        ('vector3d-mangle', fix_vector3d_mangle),
        ('positiontexturevertex-this', fix_positiontexturevertex_this),
        ('stringbuilder', fix_stringbuilder),
        ('nbttaglist-type', fix_nbttaglist_type),
        ('isnan', fix_isnan),
        ('allowuserinput-prefix', fix_allowuserinput_prefix),
        ('craftingmanager-getinstance', fix_craftingmanager_getinstance),
        ('entityplayersp-include', fix_entityplayersp_include),
        ('getitem-include', fix_getitem_include),
        ('controllist-element-type', fix_controllist_element_type),
        ('entitymap-element-type', fix_entitymap_element_type),
        ('entityplayer-camerapitch', fix_entityplayer_camerapitch),
        ('world-entity-query', fix_world_entity_query),
        ('treeset', fix_treeset),
        ('colliding-bb-type', fix_colliding_bb_type),
        ('aabb-vector-element', fix_aabb_vector_element),
        ('entitymap-remove', fix_entitymap_remove),
        ('worldaccess-remove-iterator', fix_worldaccess_remove_iterator),
        ('world-tileentity-map', fix_world_tileentity_map),
        ('world-worldaccess-type', fix_world_worldaccess_type),
        ('world-ticklist-type', fix_world_ticklist_type),
        ('blockgrass-include', fix_blockgrass_include),
        ('reflection-stub', fix_reflection_stub),
        ('addrecipe-default-count', fix_addrecipe_default_count),
        ('entity-doublenbtlist', fix_entity_doublenbtlist),
        ('comparator-operator', fix_comparator_operator),
        ('sort-calls', fix_sort_calls),
        ('savedmatrix-redef', fix_savedmatrix_redef),
        ('textures-scope-prefix', fix_textures_scope_prefix),
        ('memset-vector-clear', fix_memset_vector_clear),
        ('default-vector-init', fix_default_vector_init),
        ('arrays-fill', fix_arrays_fill),
        ('system-gc', fix_system_gc),
        ('tracked-size-marker', fix_tracked_size_marker),
        ('array-mlength-marker', fix_array_mlength_marker),
        ('2d-array-new', fix_2d_array_new),
        # NOTE: 'value-arrow-to-dot' (whole-file AxisAlignedBB '->'->'.' ) was tried and
        # REVERTED — decompiler reuses a name (var1, var2) as an AABB value in one method
        # and a pointer of another type in another method, so a file-wide rewrite breaks
        # the pointer usage (regressed AxisAlignedBB.cpp + Frustrum.cpp). Needs a proper
        # per-function scope pass; the whole-file heuristic is unsafe for this direction.
        ('m-textures', fix_m_textures),
        ('sound-manager-include', fix_sound_manager_include),
        # --- per-file logic sweep (Entity newDoubleNBTList already above) ---
        ('enum-forward-decls', fix_enum_forward_decls),
        ('path-addpoint-return', fix_path_addpoint_return),
        ('texture-path-escapes', fix_texture_path_escapes),
        ('dangling-string-returns', fix_dangling_string_returns),
        ('string-temp-getters-byvalue', fix_string_temp_getters_byvalue),
        ('nbtbase-gettagname', fix_nbtbase_gettagname),
        ('shift-precedence-parens', fix_shift_precedence_parens),
        ('dangling-commented-ctor', fix_dangling_commented_ctor),
        ('nbttagcompound', fix_nbttagcompound),
        ('light', fix_light),
        ('entitymap-misc', fix_entitymap_misc),
        ('entitymapslot', fix_entitymapslot),
        ('mobspawner', fix_mobspawner),
        ('block-ctor-msg', fix_block_ctor_msg),
        ('playercontrollercreative', fix_playercontrollercreative),
        ('guimainmenu-splashes', fix_guimainmenu_splashes),
        ('texturegearsfx', fix_texturegearsfx),
        ('levelgenerator', fix_levelgenerator),
        ('pathfinder', fix_pathfinder),       # after map-types (int-keyed pointMap)
        ('pathpoint-arrays', fix_pathpoint_arrays),
        ('stream-io-restore', fix_stream_io_restore),
        ('levelloader-io', fix_levelloader_io),
        ('gamesettings-io', fix_gamesettings_io),
        ('display-isactive', fix_display_isactive),
        ('instanceof-dynamic-cast', fix_instanceof_dynamic_cast),
        ('entityrenderer', fix_entityrenderer),
        ('renderglobal', fix_renderglobal),
        ('nbtbase-abstract', fix_nbtbase_abstract),
        ('minecraft-running', fix_minecraft_running),
        ('minecraft', fix_minecraft),
        ('array-cast', fix_array_cast),       # after vector-remove ((int[])_vecRemoveAt(...))
        ('tessellator-include', fix_tessellator_include),  # Tessellator.h -> Tesselator.h (all files)
        ('tesselator-fwddecl', fix_tesselator_fwddecl),    # header: class Tessellator; -> Tesselator; + add fwd decl
        ('ptr-cast-dot', fix_ptr_cast_dot),   # LAST: catches casts other passes introduced
    ]

    fix_stub_header(output_dir)

    total_changes = 0
    files_changed = 0
    per_fix_counts = {}

    all_files = sorted([f for f in os.listdir(output_dir) if f.endswith('.cpp') or f.endswith('.h')])
    AVAILABLE_HEADERS.clear()
    AVAILABLE_HEADERS.update(f[:-2] for f in all_files if f.endswith('.h'))
    for fname in all_files:
        fpath = os.path.join(output_dir, fname)
        with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()

        original = content
        file_changes = 0

        for fix_name, fix_func in fixes:
            content, n = fix_func(content, fname)
            if n > 0:
                file_changes += n
                per_fix_counts[fix_name] = per_fix_counts.get(fix_name, 0) + n

        if content != original:
            files_changed += 1
            total_changes += file_changes
            with open(fpath, 'w', encoding='utf-8') as f:
                f.write(content)

    for fix_name, count in sorted(per_fix_counts.items(), key=lambda x: -x[1])[:10]:
        log.debug('    %-25s %d', fix_name, count)

    return total_changes, files_changed

# ============================================================================
# PIPELINE
# ============================================================================

# These are set from PROFILE by init_profile_globals(), but PLATFORM_TEMPLATES
# only contains RubyDung templates for now. Indev templates will be added in
# increment 3. For Indev, platform files that lack templates are converted
# structurally (same as pure-logic) with PORT flags.


# (#1) Parallel per-file conversion. Each pure-logic class is independent (generate + postprocess +
# write), so we fan them out across processes. Enabled ONLY for direct CLI runs (main sets _MP_ALLOWED):
# when the converter is imported and driven in-process (the GUI's QThread, the updater), a Windows
# 'spawn' worker would re-import the host module (e.g. PySide6) — so those paths run sequentially, which
# is already fast thanks to the #2/#3 algorithmic fix. Also off when frozen.
USE_MULTIPROCESSING = True
_MP_ALLOWED = False        # set True only by main() (CLI invocation)
_MP_GLOBAL_FIELDS = None
_MP_OUTDIR = None


def _mp_init(version, enum_reg, global_fields, output_dir):
    """Pool initializer: Windows 'spawn' re-imports this module fresh in each worker, so re-apply the
    version profile + enum registration and stash the shared read-only conversion state."""
    global _MP_GLOBAL_FIELDS, _MP_OUTDIR
    set_active_profile(version)
    init_profile_globals()
    for name, consts in enum_reg:
        REFERENCE_TYPES.add(name)
        STATIC_CLASS_FIELDS.setdefault(name, set()).update(consts)
    _MP_GLOBAL_FIELDS = global_fields
    _MP_OUTDIR = output_dir


def _mp_convert(cls):
    """Worker task: generate + postprocess + write one class. Returns (name, error_str_or_None)."""
    h_path = os.path.join(_MP_OUTDIR, cls.name + '.h')
    cpp_path = os.path.join(_MP_OUTDIR, cls.name + '.cpp')
    try:
        h = postprocess(generate_header(cls), False, _MP_GLOBAL_FIELDS)
        with open(h_path, 'w', encoding='utf-8', errors='replace') as f:
            f.write(h)
        if not cls.is_interface:
            cpp = postprocess(generate_source(cls), True, _MP_GLOBAL_FIELDS)
            with open(cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                f.write(cpp)
        return (cls.name, None)
    except Exception as e:                       # noqa: BLE001
        for p in (h_path, cpp_path):
            try:
                if os.path.exists(p):
                    os.remove(p)
            except OSError:
                pass
        return (cls.name, str(e))


def run_conversion(java_dir, output_dir):
    # Read from active PROFILE
    pure_logic = PROFILE.pure_logic_files
    platform_classes = PROFILE.platform_file_classes
    required = PROFILE.required_output_files

    os.makedirs(output_dir, exist_ok=True)

    # ---- Phase 1: Parse Java ----
    log.info('Phase 1: Parsing Java source (%s profile)', PROFILE.name)
    # Recursive discovery: the indev baseline is flat, but newer versions (alpha/beta) keep their
    # sources nested under net/minecraft/{src,client,isom}/... -- walk the tree and collect .java by
    # basename (output is keyed by class name). os.walk over a flat dir yields the same files, so this
    # is safe for indev/rubydung too.
    java_paths = {}
    for _dp, _dirs, _fs in os.walk(java_dir):
        for _f in _fs:
            if _f.endswith('.java'):
                if _f not in java_paths:
                    java_paths[_f] = os.path.join(_dp, _f)
                else:
                    log.warning('  Duplicate class file name (keeping first): %s', _f)
    java_files = sorted(java_paths.keys())
    log.info('  Found %d .java files', len(java_files))

    classes = []
    errors = []
    for jf in java_files:
        if jf in SKIP_FILES:
            log.debug('  Skipped: %s', jf)
            continue
        try:
            cls = parse_java_file(java_paths[jf])
            if not cls.name:
                errors.append((jf, 'No class name'))
                continue
            classes.append(cls)
            log.debug('  Parsed: %s -> %s (fields=%d methods=%d)',
                       jf, cls.name, len(cls.fields), len(cls.methods))
        except Exception as e:
            errors.append((jf, str(e)))
            log.error('  Parse error: %s: %s', jf, e)

    log.info('  Parsed %d classes, %d errors', len(classes), len(errors))

    # Register Java enums so other files treat EnumX as a pointer singleton type and EnumX.CONST
    # rewrites to EnumX::CONST (constants modeled as static members, like static class fields).
    enum_classes = [c for c in classes if getattr(c, 'is_enum', False)]
    ENUM_TYPE_NAMES.clear()
    ENUM_TYPE_NAMES.update(c.name for c in enum_classes)
    for ec in enum_classes:
        REFERENCE_TYPES.add(ec.name)
        STATIC_CLASS_FIELDS.setdefault(ec.name, set()).update(cn for cn, _a in ec.enum_constants)
    if enum_classes:
        log.info('  Registered %d enum(s): %s', len(enum_classes),
                 ', '.join(c.name for c in enum_classes))

    # Resolve each class's INHERITED instance fields by walking the parsed parent chain, so subclass
    # method bodies prefix bare references to parent fields (posX, blockMaterial, ...) -> m_posX instead
    # of leaving them undeclared (C2065). Stored on the class so it travels to mp workers via pickling.
    by_name = {c.name: c for c in classes}
    for cls in classes:
        own = set(f.name for f in cls.fields)
        seen_names = set(own)
        inh = []
        p = cls.parent
        guard = set([cls.name])
        while p and p in by_name and p not in guard:
            guard.add(p)
            pc = by_name[p]
            for f in pc.fields:
                # Skip private parent fields: a subclass can't legally reference them (C++ would C2248),
                # and Java wouldn't either. public/protected/package are subclass-accessible.
                if not f.is_static and f.access != 'private' and f.name not in seen_names:
                    inh.append((f.name, f.type_name))
                    seen_names.add(f.name)
            p = pc.parent
        cls.inherited_fields = inh
    _ninh = sum(1 for c in classes if c.inherited_fields)
    log.info('  Resolved inherited fields for %d subclasses', _ninh)

    # ---- Phase 2: Build global field set ----
    global_fields = set()
    for cls in classes:
        for f in cls.fields:
            if not f.is_static:
                global_fields.add(f.name)
    for flds in PARENT_FIELDS.values():
        global_fields.update(flds)

    # ---- Phase 3: Generate pure-logic files via converter ----
    log.info('Phase 2: Converting pure-logic files')
    converted_count = 0
    skipped_count = 0
    # Build the work list (skip files already converted from a previous run).
    # Interfaces are ALWAYS emitted (header-only): they may be classified into interface_types without
    # being listed in pure_logic (e.g. scaffolded profiles), but every implementor #includes them, so a
    # missing IFoo.h cascades C1083 into hundreds of files. (indev lists them in pure_logic already, so
    # this is a no-op there.)
    to_convert = []
    for cls in classes:
        if cls.name not in pure_logic and not cls.is_interface:
            continue
        h_path = os.path.join(output_dir, cls.name + '.h')
        cpp_path = os.path.join(output_dir, cls.name + '.cpp')
        if os.path.exists(h_path) and (cls.is_interface or os.path.exists(cpp_path)):
            skipped_count += 1
            continue
        to_convert.append(cls)

    nproc = max(1, (os.cpu_count() or 2) - 1)
    use_mp = (USE_MULTIPROCESSING and _MP_ALLOWED and not getattr(sys, 'frozen', False)
              and nproc > 1 and len(to_convert) >= 8)
    if use_mp:
        import multiprocessing as mp
        enum_reg = [(e.name, [cn for cn, _a in e.enum_constants]) for e in enum_classes]
        log.info('  Converting %d files across %d worker processes...', len(to_convert), nproc)
        done = 0
        pool = mp.Pool(nproc, initializer=_mp_init,
                       initargs=(PROFILE.name, enum_reg, global_fields, output_dir))
        try:
            for name, err in pool.imap_unordered(_mp_convert, to_convert, chunksize=1):
                done += 1
                converted_count += 1
                if err:
                    errors.append((name, err))
                    log.error('  [%d/%d] FAILED: %s: %s', done, len(to_convert), name, err)
                else:
                    log.info('  [%d/%d] Converting: %s', done, len(to_convert), name)
        finally:
            pool.close()
            pool.join()
    else:
        for cls in to_convert:
            h_path = os.path.join(output_dir, cls.name + '.h')
            cpp_path = os.path.join(output_dir, cls.name + '.cpp')
            try:
                converted_count += 1
                log.info('  [%d/%d] Converting: %s', converted_count, len(to_convert), cls.name)
                h = postprocess(generate_header(cls), False, global_fields)
                with open(h_path, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(h)
                if not cls.is_interface:
                    cpp = postprocess(generate_source(cls), True, global_fields)
                    with open(cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                        f.write(cpp)
            except Exception as e:
                errors.append((cls.name, str(e)))
                log.error('  FAILED: %s: %s', cls.name, e)
                if os.path.exists(h_path):
                    os.remove(h_path)
                    log.debug('  Deleted broken: %s', h_path)
                if os.path.exists(cpp_path):
                    os.remove(cpp_path)
                    log.debug('  Deleted broken: %s', cpp_path)
    if skipped_count:
        log.info('  Skipped %d already-converted files', skipped_count)
    log.info('  Converted %d new files, %d total (%d errors)',
             converted_count - len([e for e in errors]), converted_count + skipped_count, len(errors))

    # ---- Phase 2.5: Java enums -> struct of static singletons ----
    for cls in enum_classes:
        h_path = os.path.join(output_dir, cls.name + '.h')
        cpp_path = os.path.join(output_dir, cls.name + '.cpp')
        try:
            with open(h_path, 'w', encoding='utf-8', errors='replace') as f:
                f.write(generate_enum_header(cls))
            with open(cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                f.write(generate_enum_source(cls))
            converted_count += 1
            log.info('  [enum] %s (%d constants)', cls.name, len(cls.enum_constants))
        except Exception as e:                       # noqa: BLE001
            errors.append((cls.name, str(e)))
            log.error('  FAILED enum: %s: %s', cls.name, e)

    # ---- Phase 2b: Post-fix converter output for platform-specific patterns ----
    if PROFILE.name == 'rubydung':
        # Tile: Add initTiles/destroyTiles (Java uses static init, Xbox 360 needs explicit lifecycle)
        tile_h_path = os.path.join(output_dir, 'Tile.h')
        if os.path.exists(tile_h_path):
            with open(tile_h_path, 'r', encoding='utf-8', errors='replace') as f:
                tile_h = f.read()
            if 'initTiles' not in tile_h:
                tile_h = tile_h.replace(
                    'static Tile* grass;',
                    'static Tile* grass;\n\n\tstatic void initTiles();\n\tstatic void destroyTiles();'
                )
                with open(tile_h_path, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(tile_h)
                log.debug('  Post-fix: Added initTiles/destroyTiles to Tile.h')

        tile_cpp_path = os.path.join(output_dir, 'Tile.cpp')
        if os.path.exists(tile_cpp_path):
            with open(tile_cpp_path, 'r', encoding='utf-8', errors='replace') as f:
                tile_cpp = f.read()
            if 'initTiles' not in tile_cpp:
                tile_cpp = tile_cpp.replace('Tile* Tile::rock = new Tile(0);', 'Tile* Tile::rock = NULL;')
                tile_cpp = tile_cpp.replace('Tile* Tile::grass = new Tile(1);', 'Tile* Tile::grass = NULL;')
                init_code = ('\nvoid Tile::initTiles()\n{\n\trock = new Tile(0);\n\tgrass = new Tile(1);\n}\n'
                             '\nvoid Tile::destroyTiles()\n{\n\tdelete rock;\n\tdelete grass;\n'
                             '\trock = NULL;\n\tgrass = NULL;\n}\n\n')
                insert_point = tile_cpp.find('Tile* Tile::grass = NULL;\n')
                if insert_point >= 0:
                    insert_point = tile_cpp.find('\n', insert_point) + 1
                    tile_cpp = tile_cpp[:insert_point] + init_code + tile_cpp[insert_point:]
                with open(tile_cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(tile_cpp)
                log.debug('  Post-fix: Added initTiles/destroyTiles to Tile.cpp')

    # Convert class -> struct for value types (shared across all profiles)
    for struct_name in PROFILE.struct_classes:
        h_path = os.path.join(output_dir, struct_name + '.h')
        if os.path.exists(h_path):
            with open(h_path, 'r', encoding='utf-8', errors='replace') as f:
                h_content = f.read()
            if 'class %s' % struct_name in h_content:
                h_content = h_content.replace('class %s' % struct_name, 'struct %s' % struct_name)
                with open(h_path, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(h_content)
                log.debug('  Post-fix: %s class -> struct', struct_name)

    # Remove stale .cpp files for header-only structs (profile-specific)
    for header_only in PROFILE.header_only_structs:
        stale_cpp = os.path.join(output_dir, header_only + '.cpp')
        if os.path.exists(stale_cpp):
            os.remove(stale_cpp)
            log.debug('  Post-fix: Removed stale %s.cpp (header-only struct)', header_only)

    # Indev-specific post-fixes
    if PROFILE.name == 'indev':
        # Rename Tessellator output files to Tesselator
        for ext in ['.h', '.cpp']:
            old_name = os.path.join(output_dir, 'Tessellator' + ext)
            new_name = os.path.join(output_dir, 'Tesselator' + ext)
            if os.path.exists(old_name):
                if os.path.exists(new_name):
                    os.remove(new_name)
                os.rename(old_name, new_name)
                log.debug('  Post-fix: Renamed Tessellator%s -> Tesselator%s', ext, ext)

    # ---- Phase 4: Emit platform files from templates ----
    log.info('Phase 3: Emitting D3D9 platform files')
    template_count = 0
    structural_platform_count = 0
    template_total = len(PLATFORM_TEMPLATES)
    for fname, content in sorted(PLATFORM_TEMPLATES.items()):
        path = os.path.join(output_dir, fname)
        with open(path, 'w', encoding='utf-8', errors='replace') as f:
            f.write(content)
        template_count += 1
        log.info('  [%d/%d] Template: %s (%d bytes)', template_count, template_total, fname, len(content))

    # For platform classes that DON'T have templates (e.g. Indev classes),
    # fall back to structural conversion with PORT flags
    # Build set of template names including alternate spellings (Tessellator->Tesselator)
    _template_names = set(PLATFORM_TEMPLATES.keys())
    if PROFILE.name == 'indev':
        # Tesselator.h/cpp templates cover the Tessellator Java class
        _template_names.add('Tessellator.h')
        _template_names.add('Tessellator.cpp')
    platform_to_convert = [c for c in classes
                           if c.name in platform_classes
                           and c.name + '.h' not in _template_names
                           and c.name + '.cpp' not in _template_names
                           and c.name not in pure_logic]
    platform_total = len(platform_to_convert)
    platform_skipped = 0
    for idx, cls in enumerate(platform_to_convert, 1):
        h_name = cls.name + '.h'
        cpp_name = cls.name + '.cpp'
        h_path = os.path.join(output_dir, h_name)
        cpp_path = os.path.join(output_dir, cpp_name)
        # Skip if already converted from a previous run
        if os.path.exists(h_path) and (cls.is_interface or os.path.exists(cpp_path)):
            platform_skipped += 1
            continue
        try:
            log.info('  [%d/%d] Platform fallback: %s', idx, platform_total, cls.name)
            h = postprocess(generate_header(cls), False, global_fields)
            with open(h_path, 'w', encoding='utf-8', errors='replace') as f:
                f.write(h)
            if not cls.is_interface:
                cpp = postprocess(generate_source(cls), True, global_fields)
                with open(cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                    f.write(cpp)
            structural_platform_count += 1
        except Exception as e:
            errors.append((cls.name, str(e)))
            log.error('  FAILED: %s: %s', cls.name, e)
            if os.path.exists(h_path):
                os.remove(h_path)
                log.debug('  Deleted broken: %s', h_path)
            if os.path.exists(cpp_path):
                os.remove(cpp_path)
                log.debug('  Deleted broken: %s', cpp_path)
    if platform_skipped:
        log.info('  Skipped %d already-converted platform files', platform_skipped)

    log.info('  Emitted %d platform files (%d templates, %d structural fallback)',
             template_count + structural_platform_count, template_count, structural_platform_count)

    # ---- Phase 4b: Post-fix generated and template files ----
    # Inject GetNanoTime() helper into any .cpp that calls it
    GET_NANO_TIME_HELPER = (
        '// Xbox 360 equivalent of Java System.nanoTime()\n'
        'static long long GetNanoTime()\n'
        '{\n'
        '\tLARGE_INTEGER counter, freq;\n'
        '\tQueryPerformanceCounter(&counter);\n'
        '\tQueryPerformanceFrequency(&freq);\n'
        '\treturn (long long)((double)counter.QuadPart * 1000000000.0 / (double)freq.QuadPart);\n'
        '}\n\n'
    )
    for fname in os.listdir(output_dir):
        if fname.endswith('.cpp'):
            fpath = os.path.join(output_dir, fname)
            with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                cpp_content = f.read()
            if 'GetNanoTime()' in cpp_content and 'static long long GetNanoTime' not in cpp_content:
                # Insert helper after the last #include line
                lines = cpp_content.split('\n')
                insert_idx = 0
                for li, ln in enumerate(lines):
                    if ln.strip().startswith('#include'):
                        insert_idx = li + 1
                lines.insert(insert_idx, '\n' + GET_NANO_TIME_HELPER)
                with open(fpath, 'w', encoding='utf-8', errors='replace') as f:
                    f.write('\n'.join(lines))
                log.debug('  Post-fix: Injected GetNanoTime() into %s', fname)

    # Fix RubyDung.cpp game loop: move handleInput() before ticks
    rubydung_cpp_path = os.path.join(output_dir, 'RubyDung.cpp')
    if os.path.exists(rubydung_cpp_path):
        with open(rubydung_cpp_path, 'r', encoding='utf-8', errors='replace') as f:
            rd_content = f.read()
        # Pattern: timer/tick/render/fps/handleInput -> handleInput/timer/tick/render/fps
        old_loop = (
            '        // Handle input\n'
            '        handleInput();\n'
            '    }'
        )
        if old_loop in rd_content and '// Poll input BEFORE' not in rd_content:
            # Remove handleInput from after render
            rd_content = rd_content.replace(
                '        // Handle input\n'
                '        handleInput();\n'
                '    }',
                '    }'
            )
            # Add handleInput before timer update
            rd_content = rd_content.replace(
                '    while (m_running)\n'
                '    {\n'
                '        // Update timer',
                '    while (m_running)\n'
                '    {\n'
                '        // Poll input BEFORE ticks so Player::tick() has fresh gamepad state\n'
                '        handleInput();\n'
                '        \n'
                '        // Update timer'
            )
            with open(rubydung_cpp_path, 'w', encoding='utf-8', errors='replace') as f:
                f.write(rd_content)
            log.debug('  Post-fix: Moved handleInput() before tick loop in RubyDung.cpp')

    # ---- Phase 5: Build helpers ----
    log.info('Phase 4: Build helpers')
    with open(os.path.join(output_dir, 'PORT_SUMMARY.md'), 'w', encoding='utf-8', errors='replace') as f:
        f.write(generate_port_summary(classes))
    with open(os.path.join(output_dir, 'BUILD.bat'), 'w', encoding='utf-8', errors='replace') as f:
        f.write('@echo off\nREM RubyDung Xbox 360\n')
        f.write('REM VS2010 Xbox 360 Game project, add all .cpp/.h\n')
        f.write('REM PCH: stdafx.h, Link: d3d9 d3dx9 xgraphics xinput\n')
        f.write('REM Content\\terrain.png + char.png\necho See comments. & pause\n')
    h_files = sorted([f for f in os.listdir(output_dir) if f.endswith('.h')])
    cpp_files = sorted([f for f in os.listdir(output_dir) if f.endswith('.cpp')])
    with open(os.path.join(output_dir, 'FILE_MANIFEST.md'), 'w', encoding='utf-8', errors='replace') as f:
        f.write('# RubyDung Xbox 360 File Manifest\nGenerated: %s\n\n' % datetime.now().strftime('%Y-%m-%d %H:%M:%S'))
        f.write('## Headers (%d)\n' % len(h_files))
        for h in h_files:
            f.write('- %s\n' % h)
        f.write('\n## Sources (%d)\n' % len(cpp_files))
        for c in cpp_files:
            f.write('- %s\n' % c)

    # ---- Phase 6: Verify ----
    log.info('Phase 5: Verification')
    missing = []
    total_lines = 0
    for req in required:
        path = os.path.join(output_dir, req)
        if not os.path.exists(path):
            missing.append(req)
            log.warning('  MISSING: %s', req)
        else:
            with open(path, 'r', errors='replace') as fh:
                lines = sum(1 for _ in fh)
            total_lines += lines
            log.debug('  OK: %s (%d bytes, %d lines)', req, os.path.getsize(path), lines)
    if not missing:
        log.info('  All %d required files present', len(required))
    else:
        log.warning('  Missing %d files!', len(missing))
    log.info('  Total source: %d lines', total_lines)

    # ---- Phase 5: Run postfix passes (error-reduction fixes) ----
    # --no-postfix produces a PRISTINE (pre-postfix) conversion for caching: restore it
    # and re-run normally (conversion skips existing files) to apply postfix in ~30s clean.
    # indev AND all indev-derived profiles (alpha/beta/custom) run the postfix error-reduction passes
    # (ptr-cast-dot, instanceof->dynamic_cast, sort/comparator, array-cast, value/pointer name tracking,
    # etc.). Most are generic; file-specific indev fixes are fname/pattern-guarded and no-op on other
    # versions' content. Only rubydung (its own primitive pipeline) opts out.
    if PROFILE.name != 'rubydung' and not SKIP_POSTFIX:
        log.info('Phase 5: Running postfix error-reduction passes')
        populate_pointer_name_sets(classes)
        pf_changes, pf_files = run_postfix(output_dir)
        log.info('  Postfix: %d changes across %d files', pf_changes, pf_files)
    elif SKIP_POSTFIX:
        log.info('Phase 5: SKIPPED (--no-postfix; pristine output for caching)')

    return len(classes), converted_count, template_count + structural_platform_count, errors, missing


# ============================================================================
# MAIN
# ============================================================================

def convert_tree(java_dir, output_dir, version='indev', log_path=None, log_handler=None):
    """Full conversion pipeline (the reusable body of main(), minus CLI arg parsing / sys.exit):
    set the version profile, run the conversion, apply hand-written PORT overrides, fix line endings.
    Returns (parsed, converted, templated, errors, missing). Used by main() AND by the GUI / updater so
    a frozen .exe can convert IN-PROCESS (no spawned python). Pass an optional logging.Handler in
    log_handler to receive the log stream live (added AFTER setup_logging, which clears handlers)."""
    set_active_profile(version)
    init_profile_globals()
    os.makedirs(output_dir, exist_ok=True)
    if log_path is None:
        log_path = os.path.join(output_dir, 'build.log')
    setup_logging(log_path)
    if log_handler is not None:
        log.addHandler(log_handler)
    log.info('=' * 60)
    log.info('java2xbox360.py — Java to Xbox 360 C++')
    log.info('Version profile: %s', PROFILE.name)
    log.info('=' * 60)
    log.info('Input:  %s', os.path.abspath(java_dir))
    log.info('Output: %s', os.path.abspath(output_dir))
    log.info('')

    parsed, converted, templated, errors, missing = run_conversion(java_dir, output_dir)

    # Hand-written PORT overrides (indev) -- copied last so they win over generation + postfix.
    if PROFILE.name == 'indev':
        n_over = apply_port_overrides(output_dir)
        if n_over:
            log.info('Applied %d hand-written PORT override file(s)', n_over)

    # Per-version hand-ported platform/render overrides from handwritten_<profile>/ (e.g.
    # handwritten_alpha/RenderEngine.cpp). The dir's presence is the gate; any profile may have one.
    n_pov = apply_profile_overrides(output_dir, PROFILE.name)
    if n_pov:
        log.info('Applied %d %s override file(s)', n_pov, PROFILE.name)

    # DOS line endings (MSVC requires \r\n)
    for fname in os.listdir(output_dir):
        if fname.endswith(('.cpp', '.h', '.bat', '.md')):
            p = os.path.join(output_dir, fname)
            with open(p, 'rb') as f:
                data = f.read()
            data = data.replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')
            with open(p, 'wb') as f:
                f.write(data)
    log.debug('Converted all output files to DOS line endings')
    return parsed, converted, templated, errors, missing


def main():
    if len(sys.argv) < 3:
        print('Usage: python3 java2xbox360.py <java_dir> <output_dir> [--version <name>] [--log <logfile>]')
        print('Versions: %s' % ', '.join(sorted(VERSION_PROFILES.keys())))
        sys.exit(1)

    java_dir = sys.argv[1]
    output_dir = sys.argv[2]

    global SKIP_POSTFIX, _MP_ALLOWED
    _MP_ALLOWED = True   # CLI run -> safe to fan out across worker processes
    if '--no-postfix' in sys.argv:
        SKIP_POSTFIX = True

    # Parse --version flag (default: rubydung)
    version_name = 'rubydung'
    if '--version' in sys.argv:
        idx = sys.argv.index('--version')
        if idx + 1 < len(sys.argv):
            version_name = sys.argv[idx + 1].lower()

    log_path = os.path.join(output_dir, 'build.log')
    if '--log' in sys.argv:
        idx = sys.argv.index('--log')
        if idx + 1 < len(sys.argv):
            log_path = sys.argv[idx + 1]

    if not os.path.isdir(java_dir):
        print('ERROR: Java source directory not found: %s' % java_dir)
        sys.exit(1)

    start = time.time()
    # Full pipeline (profile + convert + PORT overrides + DOS line endings) -- shared with the
    # GUI / update_version so a frozen .exe converts in-process. set_active_profile is inside.
    parsed, converted, templated, errors, missing = convert_tree(
        java_dir, output_dir, version_name, log_path)

    elapsed = time.time() - start
    h_count = len([f for f in os.listdir(output_dir) if f.endswith('.h')])
    cpp_count = len([f for f in os.listdir(output_dir) if f.endswith('.cpp')])

    log.info('')
    log.info('=' * 60)
    log.info('DONE (%.2fs)', elapsed)
    log.info('=' * 60)
    log.info('Parsed:    %d Java classes', parsed)
    log.info('Converted: %d pure-logic files', converted)
    log.info('Templated: %d D3D9 platform files', templated)
    log.info('Output:    %d .h + %d .cpp = %d files', h_count, cpp_count, h_count + cpp_count)
    if errors:
        log.warning('Errors: %d', len(errors))
        for n, e in errors:
            log.warning('  %s: %s', n, e)
    if missing:
        log.error('Missing files: %s', ', '.join(missing))
        sys.exit(1)


if __name__ == '__main__':
    main()
