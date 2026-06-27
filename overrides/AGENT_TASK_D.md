# Task D: make a class-keyed REGISTRY override actually functional (RTTI identity + factories)

One override class is a Java registry that maps `Class` objects <-> ids/names and creates instances
by reflection (`class.newInstance()` / `getConstructor(...).newInstance(...)`). The port stubbed
all of that out (`X.class` -> `Class()` with no identity, maps empty, creation returns NULL), so the
registry is non-functional. Make it WORK using C++ RTTI for identity and template factories for
construction. The original Java registration list is already pasted in the file as an
`/* ---- ORIGINAL JAVA (stubbed below) ---- */` comment -- use it as your source of truth.

## Paths
- Override you edit: `C:/Users/mrtit/Documents/Visual Studio 2010/Projects/MinecraftAlpha/ConversionTool/overrides/<Class>.cpp` (and `.h` only if strictly needed)
- Original Java: `C:/Users/mrtit/Desktop/MinecraftAlpha/minecraft/src/net/minecraft/src/<Class>.java`
- Reference headers (read-only, has every game class header): `/c/Users/mrtit/AppData/Local/Temp/j2xref`
- Compiler: `C:/msys64/mingw64/bin/g++.exe`

## The mechanism (verified to compile in this toolchain)
- **Class identity** = `typeid(X).name()` (a stable `std::string`, unique per class). For an object's
  RUNTIME class use `typeid(*ptr).name()` (the Minecraft classes are polymorphic -- virtual dtors --
  so this returns the DERIVED type, exactly like Java getClass()).
- **Construction** = one template factory per ctor signature, declared at the top of the .cpp:
  ```cpp
  #include <typeinfo>
  template<class T> static void* j2x_new0() { return new T(); }              // no-arg ctors
  template<class T> static void* j2x_newWorld(void* w) { return new T((World*)w); }  // World-arg ctors (entities)
  ```
  Register a class with `&j2x_new0<X>` (type `void*(*)()`) or `&j2x_newWorld<X>` (type `void*(*)(void*)`).
- **Maps**: key the class-side maps by the `std::string` identity. Keep/repair the existing maps in the
  override; store factories in a `std::map<std::string, void*(*)()>` (or `(void*)(void*)` for entities).

## Steps
1. Read `overrides/<Class>.cpp` and the `/* ORIGINAL JAVA */` block(s) listing the registrations
   (`addMapping(EntityArrow.class, "Arrow", 10)` etc.). Read `<Class>.java` to confirm signatures.
2. At the top of the .cpp add `#include <typeinfo>` and the factory template(s) you need.
3. **#include the header of EVERY class that gets registered** (e.g. `#include "EntityArrow.h"` ... for
   all ~25-30 of them). They all exist under j2xref. This is the bulk of the edit -- don't skip any.
4. Rewrite the registration (the static-init / map-fill) to populate the maps with the real data:
   `typeid(X).name()` for the class key, the id/name from the Java, and `&j2x_new0<X>` / `&j2x_newWorld<X>`
   as the factory. If the file has a converter `_j2x_clinit`/`_ClinitRun` trigger use it; otherwise add a
   file-local `namespace { struct _Init { _Init(){ ...register all... } } _init_inst; }` AFTER the static
   field definitions.
5. Rewrite the lookup/create methods to use the maps:
   - create-by-id / create-by-name -> find the factory, call it (pass the World* for entities), cast result.
   - id/name-by-object -> `typeid(*obj).name()` -> map lookup.
   Keep the PUBLIC method signatures EXACTLY as the header declares them (other files call them) -- only
   change the bodies. Touch the `.h` only if you must add a private map/helper member (keep public API stable).
6. Compile until 0 errors:
   `"C:/msys64/mingw64/bin/g++.exe" -std=c++03 -fsyntax-only -I"<overrides>" -I/c/Users/mrtit/AppData/Local/Temp/j2xref "<overrides>/<Class>.cpp"`
7. Sanity: confirm the registration actually populates (not empty) and create/lookup use the factories/typeid.

## Report
- Class, # of entries registered, which methods you made functional, and confirm 0 compile errors.
- Note anything you couldn't make functional (and why).
