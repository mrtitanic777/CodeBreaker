# cb.hpp

[The reference](README.md)

```
cb/cb.hpp -- the CodeBreaker SDK for C++: the C interface (cb.h, binary.h, disasm.h, decompile.h) with RAII handles,
std::string_view and std::span, and errors as exceptions. Header only, C++20; it adds nothing to a call but the check of its status.

    cb::sdk::Session session;                                     // cb_init ... cb_shutdown
    cb::sdk::Binary binary = cb::sdk::Binary::open("lz32.dll");
    for (const cb_section& s : binary.sections()) std::cout << s.name << "\n";
    std::cout << binary.function("LZCopy").decompile().view();
    for (const cb_instruction& i : cb::sdk::disasm("z80", bytes, 0x0000)) ...

A call that fails throws cb::sdk::Error, with the status and cb_last_error()'s message.
```

