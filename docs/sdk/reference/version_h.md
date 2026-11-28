# version.h

[The reference](README.md)

```
cb/version.h -- the CodeBreaker SDK's version, as macros only (the resource compiler reads this file too).

The version follows semantic versioning: a minor release only adds; a major release may remove, and the reference lists what
moved. Before 1.0 a minor release may still change what an earlier 0.x had.
```

## CB_SDK_VERSION_MAJOR

`version.h` · macro · since 0.1

The major version of the headers this program is compiled against.

```c
#define CB_SDK_VERSION_MAJOR 0
```

## CB_SDK_VERSION_MINOR

`version.h` · macro · since 0.1

The minor version of the headers.

```c
#define CB_SDK_VERSION_MINOR 1
```

## CB_SDK_VERSION_PATCH

`version.h` · macro · since 0.1

The patch version of the headers.

```c
#define CB_SDK_VERSION_PATCH 0
```

## CB_SDK_VERSION_STRING

`version.h` · macro · since 0.1

The version of the headers as text, "major.minor.patch".

```c
#define CB_SDK_VERSION_STRING "0.1.0"
```

## CB_SDK_MAKE_VERSION

`version.h` · macro · since 0.1

A version as one number, (major << 16) | (minor << 8) | patch, which compares as versions do.

```c
#define CB_SDK_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
```

## CB_SDK_VERSION

`version.h` · macro · since 0.1

The version of the headers as one number; compare it with cb_version(), the library's.

```c
#define CB_SDK_VERSION CB_SDK_MAKE_VERSION(CB_SDK_VERSION_MAJOR, CB_SDK_VERSION_MINOR, CB_SDK_VERSION_PATCH)
```

