CUSTOM VERSION PROFILES
=======================

Drop a <name>.json file in this folder to add a new convertible Minecraft version. The file name is the
profile name (infdev.json -> use it with --version infdev, or pick "infdev" in the GUI combo). They load
automatically at startup (CLI and GUI). No need to edit java2xbox360.py.

A custom profile INHERITS a base profile and MERGES in your deltas, so you only specify what's DIFFERENT
from the base (a new version is usually "indev + a few new types / skipped files").

KEYS
----
  "base": "indev"            base profile to inherit ("indev" | "rubydung" | another loaded profile).
                             Default: "indev".

  SET fields  -> your list is ADDED to the base set; "<field>_remove" removes entries:
     value_types, reference_types, interface_types, skip_files, pure_logic_files,
     platform_file_classes, bare_fields, bare_static_fields, header_only_structs, struct_classes
     e.g.  "value_types": ["NewVec2"],  "skip_files_remove": ["EnumOS.java"]

  LIST fields -> EXTENDS the base list:
     required_output_files

  DICT-of-list fields -> merged per key (your lists union into the base's):
     static_class_fields, parent_fields
     e.g.  "static_class_fields": { "NewBlock": ["s_grass", "s_dirt"] }

  DICT-of-string fields -> update (override / add keys):
     extra_type_map, extra_gl_map, extra_math_map
     e.g.  "extra_type_map": { "BlockPos": "Vec3i" }

EXAMPLE  (profiles/infdev.json)
-------------------------------
{
  "base": "indev",
  "value_types": ["ChunkCoordIntPair"],
  "reference_types": ["ChunkProvider", "ChunkCache"],
  "skip_files": ["ThreadedFileIOBase.java"],
  "skip_files_remove": [],
  "static_class_fields": { "Blocks": ["s_bedrock"] },
  "extra_type_map": { "NextTickListEntry": "TickEntry" }
}

NOTES
-----
- Reference types = nullable/polymorphic Java classes -> become C++ pointers (arrays-of-pointers).
- Value types = small structs passed by value (Vec3, AABB, ...).
- skip_files are NOT converted (platform/AWT/network files). pure_logic_files convert as-is;
  platform_file_classes use D3D9 templates.
- The GUI "New profile..." button writes a template here, then opens it for editing.
