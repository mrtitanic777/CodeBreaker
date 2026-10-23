# Xbox 360 avatar assets (the asset pack, STRB records, XamAvatarGetAssets' result)

What a native avatar service needs, established from the owner's 17559 system update (the console's own XAM running
recompiled, MC_RECOMP_XAM=1, its inputs and outputs compared) and from xam.xex's code (read, not copied). Addresses
are 17559's xam.xex (base 0x815F0000).

## Where the assets are

- **AvatarAssetPack.toc** (14.6 MB) in package FFFE07DF00000002 of the system extended partition
  (`\Device\Harddisk0\SystemExtPartition\Content\0000000000000000\FFFE07DF\00008000\`), found through XAM's package
  manager (`XamPackageManagerFindPackageContainingIndexedXEX("AvatarAssetPack.toc")`). AvatarAssetPackLegacyV1.toc
  is beside it.
- A profile's own assets (awards, purchases): its package's `AvatarAssets\<asset GUID as 32 hex digits>`, each a LIVE
  package holding `asset_v2.bin` (an STRB record) and `icon.png`. XAM mounts the folder as `ProfileAssetCache:`.
- The dash's own carryables (the sign-in screen's sleeping props): FFFE07DF00000006 ("Sleepies"), handed to XAM with
  XamAvatarSetCustomAsset(size, STRB record).

## Asset IDs and the manifest

An asset is a GUID `{MMMMMMMM-NNNN-VVVV-C1C8F109A19CB2E0}`: M the component mask it fills, N the asset number (its
slot in the pack), V 1 or 3, C1C8F109A19CB2E0 Microsoft's namespace. Other namespaces are titles' awards (e.g.
`{00001000-70BE-6243-C98E-F042584111F7}`, a carryable of title 584111F7).

The manifest is the profile's setting 0x63E80044 (FFFE07D1.gpd; 0x3E8 bytes after the setting's 0x18-byte header):
+0x0C an array of 16-byte GUIDs (worn assets), colours (ARGB) from +0xFC, the body's parts (body, head, hair, shoes,
glasses ...) from +0x120 as 32-byte entries {GUID, mask}, the carryable at +0x1E0, the owner's XUID and console ID at
+0x380. The dash passes it to XamAvatarGetAssets as is (with a custom asset's GUID replaced by its pointer).

Component masks (xavatar.h XAVATAR_COMPONENT_MASK): 1 head, 2 body, 4 hair, 8 shirt, 0x10 trousers, 0x20 shoes,
0x40 hat, 0x80 gloves, 0x100 glasses, 0x200 wristwear, 0x400 earrings, 0x800 ring, 0x1000 carryable. Pack masks above
0x1FFF: 0x2000 ... 0x40000 face features (eyebrows, eyes, mouth ...), 0x80000 / 0x100000 / 0x200000 more of them,
0x400000 animations, 0x800000 an outfit flag (0x800FF8: a whole-body outfit), 0x1000000 small extras.

## AvatarAssetPack.toc

- 0x30-byte header, then 1282 index slots of 0x10C bytes at 0x30 + number * 0x10C (1103 hold an asset), big-endian:
  +0 the component mask, +4 flags (body types ...), +0x104 the record's offset in the file, +0x108 its size.
- Each asset is an STRB record.

Index entries (0x10C bytes, big-endian), more fields: +4 four flag bytes (body types ...), +0x94 a companion asset's
GUID in big-endian form (Data1 first; zero when none), which XAM loads with the asset: a clothing item's body patch
(outfit 0xAC -> 0x3EB, shoes 0x3C -> 0x3E8, default outfit 0x447 -> 0x448), hair's variant (0x1D0 -> 0x1D3), eyes'
pair (0x29E -> 0x29F). Category 0x1000000 holds the body patches (231: 116 keyed to the male body, 115 the female).

## STRB records ("Structured Binary", xam 0x8196DBE8)

- 6 bytes: "STRB", a version byte (0 or 1; higher: "Structured Binary version too high"), an endianness byte (0:
  little-endian fields).
- Version 1: 0x18 bytes from offset 6: the format GUID 59BDE08B-4909-4EE4-B991-ADA87B7C0B6B (16 bytes; XAM checks it),
  the width of a chunk's type field (+0x10, 1), of its size fields (+0x11, 4), two zero bytes, the alignment (+0x14,
  4). The first chunk is at the alignment rounded up from 0x1D (version 0: 0x14 bytes, from 0x19): 0x20.
- A chunk: a header of type width + 2 * size width bytes, aligned (12 bytes: u8 kind, 4-byte size, 4-byte value;
  the sizes read big-endian here), then size bytes, the next chunk aligned after it.
- Kinds: 8 (or 6) the asset's metadata, always first (0x8196EE50); 3 a model (LZX-compressed); 1 an animation;
  4 an uncompressed asset (face feature textures ...); 2 a compressed texture asset; 5 a carryable's extra; 7 colour
  data inside a model's asset (0x81966450).
- Metadata (kind 8, 15 bytes): a sub-version byte (1 or 2), then bit fields: body types (8 bits: 1 male, 2 female, 3
  both), asset type (32: 1 a component, 2 / 3 textures ...), component mask (32), a value (32), and with sub-version 2
  the skeleton version (8; sub-version 1 means version 1). Every 17559 pack asset seen is skeleton version 2. A model
  whose skeleton version differs from XAM's (2 - (0x81B4B238 < 3)) is re-bound to the current skeleton
  (0x81975B58, 0x819713A8: vertices moved by the joints' bind-pose change).

A compressed payload is a sequence of blocks {u32 compressed size, u32 0, u32 uncompressed size, LZX data}, each
at most 32 KB out, decoded with LDIDecompress (window 32 KB? the context XAM creates) and a reset after each block.

## The model (kind 3, decompressed)

Every structure is a **little-endian bit stream**: bytes are read as little-endian u32 words, fields taken LSB-first
at bit offsets (XAM's 0x8197A388(stream, bit offset, bit count, out)). Layout (0x81974A90):

- Header, 0x30 bytes: 12 fields of 32 bits: CpuMemoryBufferSize, GpuMemoryBufferSize, TotalTextureDataSize,
  GlobalVertexBufferSize, GlobalIndexBufferSize, BatchCount, TextureCount, then the vertex buffer's and index
  buffer's offsets in the GPU buffer, the batches' and textures' offsets in the CPU buffer, and a size XAM checks
  against the space it has. (The outfit 0x447: 0xB10, 0x35FF6, 0x8000, 0x26880, 0x7776, 5, 6, 0x8000, 0x2E880, 0x110,
  0, ...)
- Then BatchCount batches (0x819747D8), each a 0x21-byte header of bit fields: 32 shader, 5 parameter count, then
  32-bit TriangleCount, VertexCount, VertexUvCount, VertexStride, IndexStride, vertex offset, index offset (both
  relative to the GPU buffer); then the shader parameters (0x81971408, 0x18 bytes each: XAVATAR_SHADER_PARAM); then
  the vertices: VertexUvCount 7 means a compressed form (0x819746E8), otherwise 0x81974760; for a left-handed caller
  (XamAvatarInitialize's flags) each vertex's Z is negated and its packed normal's top field negated; then
  TriangleCount * 3 indices of IndexStride bytes (0x81971C30).
- Then TextureCount textures (0x81978680), each: 8 bytes {data offset in the GPU buffer, size}, a 0x21-byte header
  of bit fields (format, width, height, total base size, base size, layer count, two flags, a pitch and a row count),
  then the texture data, laid out with the caller's texture functions (XamAvatarInitialize's table; XAM keeps them
  at 0x81A975CC / 0x81A975D4: the dash's own XG-style layout code).
- Last (0x81971CA8): vertex copies (pairs of vertex offsets, a vertex's first 0x18 bytes copied).

## XamAvatarGetAssets' result (XAVATAR_ASSETS, xavatar.h)

CPU buffer: XAVATAR_ASSETS at +0 {pSkeleton, pCarryable, ComponentCount, pComponentInfo, pComponentModels}, the
skeleton (71 joints of 96 bytes: hierarchy, bind pose) from +0x20, the component infos (32 bytes: GUID, mask) and
models (52 bytes: XAVATAR_MODEL) after it, then each model's own CPU block (its textures (44 bytes each) and batches
(512 bytes each)). GPU buffer: per model its texture data, then vertices (40-byte XAVATAR_VERTEX; 36 without colour,
52 for the head's 6 UVs), then 16-bit indices. Result sizes (XamAvatarGetAssetsResultSize, 0x81965EC0): CPU 0x1F80
(+0x3147C with the carryable) plus, per component in the mask, a fixed maximum per component slot (XAM's table at
0x81970970(index): +4 mask, +8 CPU size, +0xC GPU size), each rounded (CPU to 16, GPU to 4 KB).

Textures: formats 0x1A200052 (DXT1), 0x1A200054 (DXT4_5), 0x1A200152 / 0x1A200154 (the same, tiled); the head has
layered textures (5 and 14 layers: eyes, eyebrows, mouth animations).

## Verified against the console's XAM (outfit 0x447: every vertex, index and texture byte identical)

- **Vertices, formats 1 ... 6** (the batch's VertexUvCount field; 7 is another, compressed form): a 0x37-byte header,
  then bit-packed vertices from bit 434 (= byte 0x36, bit 2). Header bit fields: 0 count (32); 32 scale s (float);
  64 / 96 / 128 the X / Y / Z offsets (floats); 160 / 166 / 172 the X / Y / Z widths (6 bits each); 178 normal bias,
  210 normal width; 242 weights bias, 274 weights width; 306 bindings bias, 338 bindings width; 370 colour bias, 402
  colour width (32 bits each). A vertex: qx, qy, qz, then normal, weights, bindings, colour (each width bits + its
  bias, as u32), then format * 2 half-float UVs (16 bits each). Positions are on a hexagonal close-packed lattice:
  with (sx, sy, sz) = s * (2, 1.632993, 1.7320508) (xam's 0x81A975C0, single precision):
  y = sy * qy + oy; z = qz * sz + oz, plus sz / 3 when qy is odd; x = qx * sx + ox, plus sx / 2 when qy ^ qz is odd,
  each a single-precision fused multiply-add (fmadds: exact product and sum, one rounding). The output vertex is
  {float x, y, z; u32 normal, weights, bindings, colour; u16 UVs}, big-endian: stride 0x20 + 4 per UV pair (format 3:
  40 bytes, 6: 52). A left-handed caller's vertices get Z negated and the normal's top 10 bits negated.
- **Indices**: 8 bytes (u32 count = TriangleCount * 3, u16 bias, u16 width), then count values of width bits; each
  index = value + bias, a big-endian u16.
- **Textures**: per texture 8 bytes (u32 offset in the GPU buffer, u32 size), then 0x21 bytes of bit fields: 0 format,
  32 width, 64 height, 96 TotalBaseSize, 128 BaseSize, 160 LayerCount (32 each), 192 "zero" (1 bit: no data, the
  texture is cleared), 193 tiled (1 bit), 194 the source's row pitch in bytes, 226 its rows of 4x4 blocks (32 each);
  then pitch * rows * layers bytes of DXT blocks (8 bytes DXT1 / format 0x52, 16 DXT4_5 / 0x54), already big-endian.
  Linear textures: row r at offset + r * (blocks wide rounded up to 32) * block size. Tiled ones (0x152, 0x154): each
  block at the GPU's standard 2D tiled offset for a surface of pitch / block size blocks.

## The GetAssets pipeline (17559's xam.xex)

XamAvatarGetAssets (0x81674AD8) posts message 0x600006 to app 0xF2 (avatarcore); its request object (vtable
0x81632D78 -> 0x81964D30) runs 0x81969848: the manifest's console ID compared with this console's (then the owner is
looked up, XamProfileFindAccount), 0x81969350 (state, components), then 0x81969490 until it stops answering pending:
per asset 0x81966FC8 (metadata, then a component model 0x819669A0, or a carryable 0x81966C98), 0x819672F8 (texture
assets: kind 2 into a model's layered textures, 0x819671A0; kind 4, 0x81966138), then 0x819660A0 (skeleton 0x81975C00
and the carryable) and 0x81966348 (components without a model dropped). Open: the body's vertices at the wrists and
ankles end up moved by small rotations (outfit 0xAC wears wristwear, shoes are worn); the step that does it is in the
vector code (0x819660A0 or the skinning helpers), not yet read.

## Model patches (STRB kind 4; verified: willywonka2003's body and head identical to the console's XAM)

A patch is applied to the loaded model whose buffer sizes it names (xam 0x81966138 -> 0x81975D80):
1. **Hidden triangles** (0x8197A078): 0x20 bytes of bit fields: 0 count, 32 the target's GlobalIndexBufferSize, 64 the
   target asset's GUID (128 bits), 192 bias, 224 width; then count values of width bits. Triangle t = value + bias is
   hidden: its first index is copied over its other two (index buffer bytes t*6 .. t*6+5).
2. **Replaced vertices** (0x8197A710), right after the hidden triangles' bits: 0x53 bytes of bit fields: 0 count, 32 the
   target's GlobalVertexBufferSize, 64 GUID, 192 bias, 224 width, then at bit 256 the lattice descriptor of vertex
   headers (scale, offsets, widths, normal / weights / bindings biases and widths) and at 594 / 626 the colour bias and
   width; entries from bit 658: a byte offset into the vertex buffer (width bits + bias), then position, normal,
   weights, bindings, colour as in vertices (no UVs); the vertex's first 0x1C bytes are replaced (left-handed callers:
   Z and the normal negated as for vertices).

Clothing's companion patches reshape the body under it (hiding covered triangles, tucking wrists and ankles); face
shape assets (masks 0x80000, 0x100000, 0x200000: 0x31F, 0x32B, 0x33A ...) patch the head.

## Face textures and the head (verified)

Face assets (asset type 2, STRB kind 2: an LZX-compressed texture record, the 0x21-byte texture header and its data)
go into the head's textures by the shader parameters that use them (xam 0x819671A0): the asset's slot (from its mask:
0x8000 0, 0x2000 1, 0x4000 2, 0x10000 3, 0x40000 4, 0x20000 5) names shader usages (xam's table 0x81632F08: slot 0
mouth 12; 1 eyes 9, 10; 2 eyebrows 7, 8; 3 facial hair 6; 4 eye shadow 11; 5 skin features 5); every texture a
batch's parameter of those usages points at gets the asset's header and data (layer L at L * BaseSize). The head
model's own textures carry the "zero" flag: they are cleared (willywonka2003: textures 0, 1 and 5 stay zero; 2, 3,
4 get the eyebrows (5 layers), eyes and mouth (14 layers each)).

## The CPU side (in progress)

XAVATAR_TEXTURE = {Format, Width, Height, TotalBaseSize, 0, BaseSize, 0, MipLevels 1, LayerCount, pBaseData (GPU
buffer + the texture's offset), 0}. Batches (XAVATAR_TRIANGLE_BATCH, 512 bytes) as decoded, with pVertices /
pIndices in the GPU buffer. Shader parameters of the colour usages are set from the request's colours (up to 10
entries per component, 0x20 bytes each, from +0x170 of XAM's component record: usage, colour as float4; usages 22 ...
24 are the item's custom colours), then a model's optional colour chunk (kind 7, a palette of ARGB triplets) maps the
custom colours onto its nearest palette entry (vector code after 0x81966558; distances with vmsum3fp). Rim light
(usage 27) and the default custom colours of an item without any (0x447: (29, 44, 145, 147) / 256) are still to be
traced to their source.
