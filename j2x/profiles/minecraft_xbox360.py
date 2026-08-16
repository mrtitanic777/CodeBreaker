# -*- coding: utf-8 -*-
"""The bundled Minecraft / Xbox-360 target profile.

This is DATA, not logic: the game- and platform-specific facts (class names, package roles, nullable
fields/getters, static-init seed, project-filter rules) that the generic transpiler core reads through
the `profiles` interface. Names and APIs are referenced as facts so the tool can recognize and
translate them; no third-party source is embedded here.
"""

from . import Profile, register

# Proven, ctor-MEDIATED static-init dependencies used to SEED the topological order (e.g. Block's
# ctor reads Material statics, so Material must init first).
STATIC_INIT_ORDER = ['MathHelper', 'Material', 'Block', 'Item', 'CraftingManager',
                     'EntityList', 'TileEntity', 'MobSpawnerBase', 'OsMap', 'Packet']

# Package -> one-line role, for the signatures.md codebase map.
PACKAGE_ROLES = {
    'net.minecraft.game': 'Top-level game wiring and shared game-wide types.',
    'net.minecraft.game.entity': 'Entity base classes and the entity update/movement core.',
    'net.minecraft.game.entity.animal': 'Passive/animal mobs.',
    'net.minecraft.game.entity.monster': 'Hostile mobs.',
    'net.minecraft.game.entity.misc': 'Non-living entities (items, projectiles, TNT, particles).',
    'net.minecraft.game.entity.player': 'The player entity and player inventory.',
    'net.minecraft.game.item': 'Items and item stacks (the inventory-side counterpart of blocks).',
    'net.minecraft.game.level': 'The world/level model: world container, chunks, generation, save/load.',
    'net.minecraft.game.level.block': 'Block definitions and per-block behaviour.',
    'net.minecraft.game.level.material': 'Block material properties (solidity, liquid, replaceable).',
    'net.minecraft.game.physics': 'Math/collision primitives: vectors, bounding boxes, ray hits.',
    'net.minecraft.game.tile': 'Tile entities (blocks with extra per-instance state, e.g. chests).',
    'net.minecraft.client': 'Client-side game shell, input and main loop.',
    'net.minecraft.client.render': 'Rendering: world/entity renderers and render helpers.',
    'net.minecraft.client.gui': 'GUI screens and HUD.',
    'net.minecraft.client.sound': 'Sound engine and sound playback.',
    'util': 'Standalone utility helpers (math, noise, etc.).',
}

# Types heavy enough that per-frame heap allocation of them is a hotpath-rule flag.
HEAVY_TYPES = ('Tesselator', 'Tessellator', 'NBTTagCompound', 'NBTTagList')

# Registry arrays whose slots are NULL until an id is registered (deref of an unregistered slot is a
# virtual call on NULL = crash on the console).
NULLABLE = {'blocksList', 'itemsList'}

# Fields that are legitimately NULL at points in the lifecycle -- any 360 deref without a null check
# is a hard crash, so the converter guards them.
NULLABLE_FIELDS = {
    'worldObj', 'mc', 'thePlayer', 'theWorld', 'currentScreen',
    'playerController', 'riddenByEntity', 'ridingEntity',
    'renderEngine', 'fontRenderer', 'ingameGUI', 'loadingScreen',
    'sndManager', 'effectRenderer', 'renderGlobal', 'entityRenderer',
    'craftingInventory', 'inventorySlots',
}

# Getters that can return NULL (sparse-registry lookups / optional references).
NULLABLE_GETTERS = {
    'getChunkFromChunkCoords', 'getChunkFromBlockCoords',
    'getItem', 'getSmeltingResultItem',
    'getCompoundTag', 'getNetClientHandler',
    'getDocumentBase', 'getParentFile',
    'getReadThread', 'getWriteThread',
    'getWorldFromRenderManager',
}

# Return (pointer) type of each pointer-returning nullable getter, so the null-guard hoist can declare
# a typed C++03 temp. getSmeltingResultItem is intentionally ABSENT (returns int, never derefs).
NULLABLE_GETTER_RETURN_TYPE = {
    'getChunkFromChunkCoords': 'Chunk', 'getChunkFromBlockCoords': 'Chunk',
    'getItem': 'Item', 'getCompoundTag': 'NBTTagCompound',
    'getNetClientHandler': 'NetClientHandler', 'getWorldFromRenderManager': 'World',
    'getDocumentBase': 'URL', 'getParentFile': 'File',
    'getReadThread': 'Thread', 'getWriteThread': 'Thread',
}

# Visual Studio .vcxproj/.filters classification: (basename regex, filter path). First match wins;
# EVERY file must resolve to a filter (fallback 'Game (misc)'), never the project root.
VCXPROJ_FILTERS = [
    # ---- j2x runtime & build glue -------------------------------------------------------------
    (r'^Debug$', 'Debug'),
    (r'^(JavaRT|JavaString|JArray|JList|JObject|JMap|JSet|JCat|Runnable|Iterator|Class|j2x_\w*|stdafx|Common|ReflectionFactory)$', 'Runtime'),

    # ---- Platform layer: hand-written 360 backends + Java-stdlib bridges -----------------------
    (r'^(Kernel|KernelConfig|ControllerConfig|SDKConfig)$', 'Platform\\Config'),
    (r'^(Save360|ContentStorage|STFS\w*)$', 'Platform\\Storage (STFS)'),   # XContent/STFS durable-save mount
    (r'^(GL\d*|GLU|GLContext|GLAllocation|ARB\w+|NV\w+|BufferUtils|Display|DisplayMode|Sys|Project|Frustrum)$', 'Platform\\Video (GL->D3D9)'),
    (r'^(Keyboard|Mouse|Controllers?|Input|InputConfig|OSK|MouseHelper)$', 'Platform\\Input'),
    (r'^(Socket\w*|InetAddress|SocketAddress|UdpSocket|URL|URI|URLConnection|HttpURLConnection|BindException|ConnectException|UnknownHostException|MalformedURLException)$', 'Platform\\Networking (XNet)'),
    (r'^(Inflate|Inflater|InflaterInputStream|Deflater|DeflaterOutputStream|GZIP\w*|Zip\w*|DataFormatException)$', 'Platform\\Compression (zlib)'),
    (r'^(File\w*|Buffered(Input|Output|Reader|Writer)\w*|Data(Input|Output)\w*|(Input|Output)Stream\w*|ByteArray\w*Stream|Print\w*|StringReader|StringWriter|PushbackReader|InputStreamReader|Reader|Writer|RandomAccessFile|FileIO|EOFException|IOException|FileNotFoundException)$', 'Platform\\Java IO'),
    (r'^(Pattern|Matcher|Charset|Locale|Calendar|Date|GregorianCalendar|SimpleDateFormat|DateFormat|DecimalFormat|NumberFormat|Properties|Logger|BigInteger|MessageDigest|MD5\w*|ByteOrder|Reference|SoftReference)$', 'Platform\\Java util'),
    (r'^(Frame|Panel|Canvas|Component\w*|Graphics\w*|Applet|MinecraftApplet|Cursor|Robot|Toolkit|Clipboard|Transferable|DataFlavor|\w*Layout|\w*Event|\w*Listener|\w*Adapter|Window\w*|Image\w*|BufferedImage|Raster|WritableRaster|DataBuffer\w*|Font|FontMetrics|Rectangle|Dimension|Color|Focus\w*|BufferStrategy|Document|DocumentBuilder\w*|Node|NodeList|Element)$', 'Platform\\AWT-Swing (dead)'),
    (r'^(\w*Exception|\w*Error|Throwable)$', 'Platform\\Java exceptions'),

    # ---- Game networking ----------------------------------------------------------------------
    (r'^Packet\w*', 'Network\\Packets'),
    (r'^(NetClientHandler|NetServerHandler|NetHandler|NetLoginHandler)$', 'Network\\Handlers'),
    (r'^(NetworkManager|Network\w*Thread|ThreadConnectToServer|ThreadCloseConnection|TcpConnection|MemoryConnection)$', 'Network\\Connection'),

    # ---- Audio (game engine + platform bridge) ------------------------------------------------
    (r'^(SoundManager|SoundPool\w*|MusicTicker)$', 'Audio\\Engine'),
    (r'^(Sound\w*|Audio\w*|XactAudio|Codec\w*|Library\w*OpenAL)$', 'Audio\\Platform'),

    # ---- GUI ----------------------------------------------------------------------------------
    (r'^(GuiIngame\w*|GuiParticle|GuiAchievement)$', 'GUI\\In-game HUD'),
    (r'^(GuiButton|GuiSlot\w*|GuiSlider|GuiTextField|GuiSmall\w*|GuiScreenButtonList)$', 'GUI\\Widgets'),
    (r'^Gui\w*', 'GUI\\Screens'),
    (r'^(FontRenderer|ScaledResolution|LoadingScreen\w*)$', 'GUI\\Text & Layout'),

    # ---- Rendering (before Entities/Items/Blocks so RenderX/ModelX win over the *X* fallbacks) -
    (r'^(Render\w*|IRender\w*)$', 'Rendering\\Renderers'),
    (r'^Model\w*', 'Rendering\\Models'),
    (r'^(Texture\w*|TexturePack\w*|\w+FX|IconCoord|Colorizer\w*|\w*Colorizer)$', 'Rendering\\Textures'),
    (r'^(Tessellator|EffectRenderer|\w*Particle\w*)$', 'Rendering\\Tessellator & FX'),

    # ---- TileEntities (before Entities: 'TileEntity*' also matches the Entity fallback) --------
    (r'^TileEntity\w*', 'TileEntities'),

    # ---- Entities -----------------------------------------------------------------------------
    (r'^(EntityPlayer\w*|EntityClientPlayerMP|InventoryPlayer)$', 'Entities\\Player'),
    (r'^(Path\w*|EntityAI\w*)$', 'Entities\\AI & Pathfinding'),
    (r'^(Entity|EntityLiving|EntityCreature|DataWatcher|WatchableObject)$', 'Entities\\Base'),
    (r'^(Entity\w*|\w+Entity|MobSpawner\w*|SpawnListEntry)$', 'Entities\\Types'),

    # ---- Blocks -------------------------------------------------------------------------------
    (r'^(Block|BlockContainer|Material|StepSound)$', 'Blocks\\Base'),
    (r'^Block.+', 'Blocks\\Types'),

    # ---- Items & Inventory --------------------------------------------------------------------
    (r'^(CraftingManager|\w*Recipe\w*|RecipesIngredients|FurnaceRecipes)$', 'Items & Inventory\\Recipes'),
    (r'^(Container\w*|Slot\w*|Inventory\w*|IInventory)$', 'Items & Inventory\\Containers'),
    (r'^(Item|Item.+|ItemStack)$', 'Items & Inventory\\Items'),

    # ---- World & Generation -------------------------------------------------------------------
    (r'^(Chunk\w*|IChunk\w*)$', 'World & Generation\\Chunk'),
    (r'^BiomeGen\w*', 'World & Generation\\Biomes'),
    (r'^(MapGen\w*|WorldGen\w*|NoiseGen\w*|\w*Generator|ChunkProvider\w*|StructureBoundingBox|Component\w*Structure)$', 'World & Generation\\Generation'),
    (r'^(Spawner\w*|SpawnerAnimals)$', 'World & Generation\\Spawning'),
    (r'^(World\w*|IWorld\w*|IBlockAccess|NextTickListEntry|WorldInfo|WorldProvider\w*)$', 'World & Generation\\World'),

    # ---- IO & NBT (game-level save/load; the platform stream/zlib bridges are under Platform) --
    (r'^NBT\w*', 'IO & NBT\\NBT'),
    (r'^RegionFile\w*', 'IO & NBT\\Region'),
    (r'^(Save\w*|\w*ChunkLoader|ChunkLoader|IChunkLoader|AnvilSave\w*|\w*Storage|MapStorage|ISaveFormat|ISaveHandler|CompressedStreamTools)$', 'IO & NBT\\Save'),
    (r'^(Compressed\w*|\w*Cache|MapData\w*|MapColor)$', 'IO & NBT\\Misc'),

    # ---- Java library glue / dead code that would otherwise fall to 'Game (misc)' --------------
    (r'^J_\w*', 'Platform\\JSON (argo)'),                       # argo JSON parser (stats/achievements payloads)
    (r'^(ByteBuffer|ShortBuffer|IntBuffer|FloatBuffer|DoubleBuffer|LongBuffer|CharBuffer|NIOBuffers)$', 'Platform\\Java NIO'),
    (r'^(Canvas\w*|Iso\w*|IsomPreview\w*)$', 'Platform\\AWT-Swing (dead)'),   # crash-report/isometric-preview canvases (dead)
    (r'^(Collection|Collections|Comparable|Comparator|Entry|Enumeration|JIterator|HashMap\w*|HashSet\w*|TreeMap\w*|TreeSet\w*|ArrayList\w*|LinkedList\w*|LinkedHashMap\w*|StringBuilder|StringBuffer)$', 'Runtime'),
    (r'^Chat\w*', 'GUI\\Screens'),                              # ChatLine, ChatAllowedCharacters
    (r'^(ClippingHelper\w*|Frustrum|ICamera)$', 'Rendering\\Renderers'),   # frustum culling
    (r'^Explosion$', 'World & Generation\\World'),

    # ---- Math & Util --------------------------------------------------------------------------
    (r'^(MathHelper|Vec3D\w*|AxisAlignedBB|MovingObjectPosition|Direction|Facing|ChunkCoordinates\w*|ChunkPosition|WeightedRandom\w*|Tuple|LongHashMap\w*|MCHash\w*|Random)$', 'Math & Util'),

    # ---- Enums --------------------------------------------------------------------------------
    (r'^Enum\w*', 'Enums'),

    # ---- Core ---------------------------------------------------------------------------------
    (r'^(StatBase|StatList|Stat\w*|IStat\w*|Achievement\w*|StatFileWriter|StatsSyncher|StatCollector|StatStringFormat\w*)$', 'Core\\Stats & Achievements'),
    (r'^(Minecraft|MinecraftApplet|Session|Timer|Main|GameSettings|EnumOptions\w*|Profiler|MovementInput\w*)$', 'Core'),
]

PROFILE = Profile(
    'minecraft_xbox360',
    static_init_order=STATIC_INIT_ORDER,
    package_roles=PACKAGE_ROLES,
    heavy_types=HEAVY_TYPES,
    vcxproj_filters=VCXPROJ_FILTERS,
    nullable=NULLABLE,
    nullable_fields=NULLABLE_FIELDS,
    nullable_getters=NULLABLE_GETTERS,
    nullable_getter_return_type=NULLABLE_GETTER_RETURN_TYPE,
    unsigned_byte_scalar_fields=set(),
    default_value_types=set(),
)
register(PROFILE)
