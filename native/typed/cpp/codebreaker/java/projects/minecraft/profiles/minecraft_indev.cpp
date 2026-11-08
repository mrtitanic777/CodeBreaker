// codebreaker/java/projects/minecraft/profiles/minecraft_indev.cpp -- typed C++ port of
// codebreaker.java.projects.minecraft.profiles.minecraft_indev (see the header).

#include "codebreaker/java/projects/minecraft/profiles/minecraft_indev.hpp"

#include "codebreaker/java/projects/minecraft/profiles/minecraft_base.hpp"

namespace codebreaker::java::projects::minecraft::profiles::minecraft_indev {

namespace _base = codebreaker::java::projects::minecraft::profiles::minecraft_base;
namespace _canon = codebreaker::java::projects::minecraft::profiles::minecraft_xbox360;

const py::dict<py::str, py::str>& PACKAGE_ROLES() {
    static const py::dict<py::str, py::str> roles = [] {
        py::dict<py::str, py::str> d = _base::SHARED_PACKAGE_ROLES.copy();
        d.set(py::str(R"rx(com.mojang.nbt)rx"), py::str(R"rx(NBT tag type system (NBTBase + tag classes) -- level/entity serialization.)rx"));
        d.set(py::str(R"rx(util)rx"), py::str(R"rx(Misc helpers: MathHelper, IProgressUpdate (progress callback).)rx"));
        d.set(py::str(R"rx(net.minecraft.game)rx"), py::str(R"rx(Top-level game interfaces/containers (IInventory, InventoryLargeChest).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity)rx"), py::str(R"rx(Entity base hierarchy: Entity, EntityLiving, EntityCreature.)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity.animal)rx"), py::str(R"rx(Passive mobs (pig, sheep, ...).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity.monster)rx"), py::str(R"rx(Hostile mobs (zombie, skeleton, creeper, spider, ...).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity.misc)rx"), py::str(R"rx(Non-living entities (dropped item, primed TNT).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity.projectile)rx"), py::str(R"rx(Projectiles (arrow).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.entity.player)rx"), py::str(R"rx(Player entity + inventory (EntityPlayer, InventoryPlayer).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.item)rx"), py::str(R"rx(Item registry + item classes (Item, ItemStack, ItemBlock, tools/food).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.item.recipe)rx"), py::str(R"rx(Crafting system (CraftingManager, recipes, RecipeSorter).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level)rx"), py::str(R"rx(The finite world + level services: World, LevelLoader, MobSpawner, Light, EntityMap, pathfinding hooks.)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.block)rx"), py::str(R"rx(Block registry + block subclasses, step sounds.)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.block.tileentity)rx"), py::str(R"rx(Tile entities (TileEntity, TileEntityChest).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.generator)rx"), py::str(R"rx(Whole-level terrain generation (LevelGenerator).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.generator.noise)rx"), py::str(R"rx(Perlin/octave noise generators.)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.material)rx"), py::str(R"rx(Block material types (Material + liquid/logic/transparent).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.level.path)rx"), py::str(R"rx(Mob A* pathfinding (Path, PathEntity, PathPoint, Pathfinder).)rx"));
        d.set(py::str(R"rx(net.minecraft.game.physics)rx"), py::str(R"rx(Math/collision primitives (AxisAlignedBB, Vec3D, MovingObjectPosition).)rx"));
        d.set(py::str(R"rx(net.minecraft.client)rx"), py::str(R"rx(Client core/bootstrap: Minecraft, applets, GameSettings, Session, Timer, loading/threads, main menu.)rx"));
        d.set(py::str(R"rx(net.minecraft.client.controller)rx"), py::str(R"rx(Player interaction modes (survival/creative controllers).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.effect)rx"), py::str(R"rx(Particles (EffectRenderer, Entity*FX).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.gui)rx"), py::str(R"rx(GUI screens/widgets (GuiScreen, FontRenderer, level dialogs, buttons).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.gui.container)rx"), py::str(R"rx(Inventory/crafting/chest container GUIs + slots.)rx"));
        d.set(py::str(R"rx(net.minecraft.client.model)rx"), py::str(R"rx(3D entity models (ModelBase, ModelBiped, mob models).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.player)rx"), py::str(R"rx(Client-side player + input (EntityPlayerSP, MovementInput).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.render)rx"), py::str(R"rx(World/entity rendering (RenderGlobal, WorldRenderer, Tessellator, RenderEngine, ItemRenderer).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.render.camera)rx"), py::str(R"rx(Frustum/camera (Frustrum, ICamera, isometric camera).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.render.entity)rx"), py::str(R"rx(Entity renderers (Render, RenderManager, RenderPlayer, ...).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.render.texture)rx"), py::str(R"rx(Animated textures (TextureFX: lava/water/gears/flames).)rx"));
        d.set(py::str(R"rx(net.minecraft.client.sound)rx"), py::str(R"rx(Audio (SoundManager, SoundPool, SoundPoolEntry).)rx"));
        return d;
    }();
    return roles;
}

// Material statics feed Block ctors; Block statics feed Item; CraftingManager reads both. Material's
// subclasses (MaterialTransparent/Liquid/Logic) load with Material. (Derived from static-init reads.)
const py::list<py::str> STATIC_INIT_ORDER = {
    py::str(R"rx(Material)rx"),
    py::str(R"rx(Block)rx"),
    py::str(R"rx(Item)rx"),
    py::str(R"rx(CraftingManager)rx"),
};

// Reference registries with NULL slots for unregistered ids -> deref of an empty slot is a virtual call
// on NULL. Block.blocksList[256] (ids 0,61-255 NULL) and Item.itemsList[1024] (unregistered NULL).
const py::set<py::str> NULLABLE = {
    py::str(R"rx(blocksList)rx"),
    py::str(R"rx(itemsList)rx"),
};

// Fields legitimately NULL at points in the lifecycle (set after ctor / cleared on world unload), with
// unguarded derefs on the console. Names verified against Minecraft.java / World.java / Entity.java.
const py::set<py::str> NULLABLE_FIELDS = {
    py::str(R"rx(currentScreen)rx"),
    py::str(R"rx(effectRenderer)rx"),
    py::str(R"rx(entityMap)rx"),
    py::str(R"rx(fontRenderer)rx"),
    py::str(R"rx(mouseHelper)rx"),
    py::str(R"rx(objectMouseOver)rx"),
    py::str(R"rx(playerEntity)rx"),
    py::str(R"rx(renderEngine)rx"),
    py::str(R"rx(renderGlobal)rx"),
    py::str(R"rx(sndManager)rx"),
    py::str(R"rx(thePlayer)rx"),
    py::str(R"rx(theWorld)rx"),
    py::str(R"rx(worldObj)rx"),
};

// Getters that can return NULL (sparse lookups / optional refs) whose pointer result is dereferenced.
const py::set<py::str> NULLABLE_GETTERS = {
    py::str(R"rx(collisionRayTrace)rx"),
    py::str(R"rx(decrStackSize)rx"),
    py::str(R"rx(findMatchingRecipe)rx"),
    py::str(R"rx(findPlayerToAttack)rx"),
    py::str(R"rx(findSubclassOf)rx"),
    py::str(R"rx(getBlockTileEntity)rx"),
    py::str(R"rx(getCollisionBoundingBoxFromPool)rx"),
    py::str(R"rx(getIntermediateWithXValue)rx"),
    py::str(R"rx(getIntermediateWithYValue)rx"),
    py::str(R"rx(getIntermediateWithZValue)rx"),
    py::str(R"rx(getStackInSlot)rx"),
    py::str(R"rx(rayTraceBlocks)rx"),
};

// Pointer return type of each nullable getter, so the C++03 null-guard hoist can declare a typed temp.
const py::dict<py::str, py::str> NULLABLE_GETTER_RETURN_TYPE = {
    {py::str(R"rx(getBlockTileEntity)rx"), py::str(R"rx(TileEntity)rx")},
    {py::str(R"rx(rayTraceBlocks)rx"), py::str(R"rx(MovingObjectPosition)rx")},
    {py::str(R"rx(findSubclassOf)rx"), py::str(R"rx(Entity)rx")},
    {py::str(R"rx(findMatchingRecipe)rx"), py::str(R"rx(ItemStack)rx")},
    {py::str(R"rx(collisionRayTrace)rx"), py::str(R"rx(MovingObjectPosition)rx")},
    {py::str(R"rx(getCollisionBoundingBoxFromPool)rx"), py::str(R"rx(AxisAlignedBB)rx")},
    {py::str(R"rx(getStackInSlot)rx"), py::str(R"rx(ItemStack)rx")},
    {py::str(R"rx(decrStackSize)rx"), py::str(R"rx(ItemStack)rx")},
    {py::str(R"rx(findPlayerToAttack)rx"), py::str(R"rx(Entity)rx")},
    {py::str(R"rx(getIntermediateWithXValue)rx"), py::str(R"rx(Vec3D)rx")},
    {py::str(R"rx(getIntermediateWithYValue)rx"), py::str(R"rx(Vec3D)rx")},
    {py::str(R"rx(getIntermediateWithZValue)rx"), py::str(R"rx(Vec3D)rx")},
};

const Profile& PROFILE() {
    static const Profile p = Profile{
        py::str(R"rx(minecraft_indev)rx"),
        STATIC_INIT_ORDER.copy(),
        PACKAGE_ROLES().copy(),
        _base::SHARED_HEAVY_TYPES.copy(),  // tuple(heavy_types)
        _base::build_filters(),
        _base::SHARED_DEAD_TYPES,
        std::nullopt,                      // optional_types
        py::set<py::str>::from(NULLABLE),
        py::set<py::str>::from(NULLABLE_FIELDS),
        py::set<py::str>::from(NULLABLE_GETTERS),
        NULLABLE_GETTER_RETURN_TYPE.copy(),
        py::set<py::str>(),                // unsigned_byte_scalar_fields=set()
        py::set<py::str>(),                // default_value_types=set()
        py::list<py::str>(),               // heavy_types_high = ()
    };
    return p;
}

}  // namespace codebreaker::java::projects::minecraft::profiles::minecraft_indev
