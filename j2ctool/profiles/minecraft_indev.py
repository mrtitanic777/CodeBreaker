# -*- coding: utf-8 -*-
"""Indev edition profile -- SOURCE-DERIVED from a RetroMCP Indev decompile (net.minecraft.game.*,
com.mojang.nbt; 230 classes).

Indev has the Block/Item/Material registries and mobs/items/inventory/crafting, but the world is a
single FINITE ``World`` (flat ``byte[]`` block array indexed ``(y*length + z)*width + x`` -- fields
width/length/height) with NO chunk streaming. Saves are one GZIP+NBT stream via ``LevelLoader``
(``com.mojang.nbt``). No ``Chunk``/``ChunkProvider``, no ``BiomeGenBase``/``RegionFile`` -- those
distinguish it from Alpha/Beta.

The flat ``World.d[]`` walks into the (engine-level) OOB fixed-array crash class; bounds-guarding that
access is a rules concern, not a profile field.
"""

from . import Profile, register
from . import minecraft_base as _base

# net.minecraft.game.* / net.minecraft.client.* / com.mojang.nbt / util -- from the real package tree.
PACKAGE_ROLES = dict(_base.SHARED_PACKAGE_ROLES)
PACKAGE_ROLES.update({
    'com.mojang.nbt': 'NBT tag type system (NBTBase + tag classes) -- level/entity serialization.',
    'util': 'Misc helpers: MathHelper, IProgressUpdate (progress callback).',
    'net.minecraft.game': 'Top-level game interfaces/containers (IInventory, InventoryLargeChest).',
    'net.minecraft.game.entity': 'Entity base hierarchy: Entity, EntityLiving, EntityCreature.',
    'net.minecraft.game.entity.animal': 'Passive mobs (pig, sheep, ...).',
    'net.minecraft.game.entity.monster': 'Hostile mobs (zombie, skeleton, creeper, spider, ...).',
    'net.minecraft.game.entity.misc': 'Non-living entities (dropped item, primed TNT).',
    'net.minecraft.game.entity.projectile': 'Projectiles (arrow).',
    'net.minecraft.game.entity.player': 'Player entity + inventory (EntityPlayer, InventoryPlayer).',
    'net.minecraft.game.item': 'Item registry + item classes (Item, ItemStack, ItemBlock, tools/food).',
    'net.minecraft.game.item.recipe': 'Crafting system (CraftingManager, recipes, RecipeSorter).',
    'net.minecraft.game.level': 'The finite world + level services: World, LevelLoader, MobSpawner, '
                                'Light, EntityMap, pathfinding hooks.',
    'net.minecraft.game.level.block': 'Block registry + block subclasses, step sounds.',
    'net.minecraft.game.level.block.tileentity': 'Tile entities (TileEntity, TileEntityChest).',
    'net.minecraft.game.level.generator': 'Whole-level terrain generation (LevelGenerator).',
    'net.minecraft.game.level.generator.noise': 'Perlin/octave noise generators.',
    'net.minecraft.game.level.material': 'Block material types (Material + liquid/logic/transparent).',
    'net.minecraft.game.level.path': 'Mob A* pathfinding (Path, PathEntity, PathPoint, Pathfinder).',
    'net.minecraft.game.physics': 'Math/collision primitives (AxisAlignedBB, Vec3D, MovingObjectPosition).',
    'net.minecraft.client': 'Client core/bootstrap: Minecraft, applets, GameSettings, Session, Timer, '
                            'loading/threads, main menu.',
    'net.minecraft.client.controller': 'Player interaction modes (survival/creative controllers).',
    'net.minecraft.client.effect': 'Particles (EffectRenderer, Entity*FX).',
    'net.minecraft.client.gui': 'GUI screens/widgets (GuiScreen, FontRenderer, level dialogs, buttons).',
    'net.minecraft.client.gui.container': 'Inventory/crafting/chest container GUIs + slots.',
    'net.minecraft.client.model': '3D entity models (ModelBase, ModelBiped, mob models).',
    'net.minecraft.client.player': 'Client-side player + input (EntityPlayerSP, MovementInput).',
    'net.minecraft.client.render': 'World/entity rendering (RenderGlobal, WorldRenderer, Tessellator, '
                                   'RenderEngine, ItemRenderer).',
    'net.minecraft.client.render.camera': 'Frustum/camera (Frustrum, ICamera, isometric camera).',
    'net.minecraft.client.render.entity': 'Entity renderers (Render, RenderManager, RenderPlayer, ...).',
    'net.minecraft.client.render.texture': 'Animated textures (TextureFX: lava/water/gears/flames).',
    'net.minecraft.client.sound': 'Audio (SoundManager, SoundPool, SoundPoolEntry).',
})

# Material statics feed Block ctors; Block statics feed Item; CraftingManager reads both. Material's
# subclasses (MaterialTransparent/Liquid/Logic) load with Material. (Derived from static-init reads.)
STATIC_INIT_ORDER = ['Material', 'Block', 'Item', 'CraftingManager']

# Reference registries with NULL slots for unregistered ids -> deref of an empty slot is a virtual call
# on NULL. Block.blocksList[256] (ids 0,61-255 NULL) and Item.itemsList[1024] (unregistered NULL).
NULLABLE = {'blocksList', 'itemsList'}

# Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on world unload), with
# unguarded derefs on the console. Names verified against Minecraft.java / World.java / Entity.java.
NULLABLE_FIELDS = {
    'theWorld', 'thePlayer', 'renderGlobal', 'effectRenderer', 'renderEngine', 'fontRenderer',
    'currentScreen', 'objectMouseOver', 'sndManager', 'mouseHelper',
    'playerEntity', 'entityMap', 'worldObj',
}

# Getters that can return NULL (sparse lookups / optional refs) whose pointer result is dereferenced.
NULLABLE_GETTERS = {
    'getBlockTileEntity', 'rayTraceBlocks', 'findSubclassOf', 'findMatchingRecipe',
    'collisionRayTrace', 'getCollisionBoundingBoxFromPool', 'getStackInSlot', 'decrStackSize',
    'findPlayerToAttack',
    'getIntermediateWithXValue', 'getIntermediateWithYValue', 'getIntermediateWithZValue',
}

# Pointer return type of each nullable getter, so the C++03 null-guard hoist can declare a typed temp.
NULLABLE_GETTER_RETURN_TYPE = {
    'getBlockTileEntity': 'TileEntity',
    'rayTraceBlocks': 'MovingObjectPosition',
    'findSubclassOf': 'Entity',
    'findMatchingRecipe': 'ItemStack',
    'collisionRayTrace': 'MovingObjectPosition',
    'getCollisionBoundingBoxFromPool': 'AxisAlignedBB',
    'getStackInSlot': 'ItemStack',
    'decrStackSize': 'ItemStack',
    'findPlayerToAttack': 'Entity',
    'getIntermediateWithXValue': 'Vec3D',
    'getIntermediateWithYValue': 'Vec3D',
    'getIntermediateWithZValue': 'Vec3D',
}

register(Profile(
    'minecraft_indev',
    static_init_order=STATIC_INIT_ORDER,
    package_roles=PACKAGE_ROLES,
    heavy_types=_base.SHARED_HEAVY_TYPES,
    vcxproj_filters=_base.build_filters(),
    dead_types=_base.SHARED_DEAD_TYPES,
    nullable=NULLABLE,
    nullable_fields=NULLABLE_FIELDS,
    nullable_getters=NULLABLE_GETTERS,
    nullable_getter_return_type=NULLABLE_GETTER_RETURN_TYPE,
    unsigned_byte_scalar_fields=set(),
    default_value_types=set(),
))
