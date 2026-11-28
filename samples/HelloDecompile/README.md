# HelloDecompile

Decompiles one function to C with `cb_function_decompile`, from C++ through `cb.hpp`: `cb::sdk::Session` starts the SDK, `cb::sdk::Binary`
closes itself, a failed call throws `cb::sdk::Error` with the library's message. `--module` decompiles every function (`cb_module_decompile`).

```
HelloDecompile lz32.dll LZCopy          by name
HelloDecompile lz32.dll 0xbfea11aa      by address
HelloDecompile lzexpand.dll LZCOPY      a 16-bit function, cut out of its module's C
HelloDecompile lz32.dll --module
```

The first run of a module takes as long as the decompiler takes; what it made is kept in the state folder (`--state`), so a second run is quick.
