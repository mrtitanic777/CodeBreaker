# XDK API Documentation Index

## What this is

A searchable tab-separated index of 11,906 Xbox 360 XDK documentation pages
extracted from the official XDK CHM help files.

## Source location

The original .htm files live at:
```
C:\Users\mrtit\Desktop\XDK Documentation
```

## Index file

```
docs/platform/xdk-api-index.tsv
```

Format: `filename<TAB>title` (one line per file, sorted alphabetically).

## How to search

Use grep/findstr/Select-String to find APIs by keyword:

```bash
grep -i "D3DDevice" xdk-api-index.tsv
grep -i "XInput" xdk-api-index.tsv
grep -i "XACT" xdk-api-index.tsv
grep -i "XContent" xdk-api-index.tsv
grep -i "XNet\|Winsock" xdk-api-index.tsv
grep -i "texture" xdk-api-index.tsv
```

Or in PowerShell:
```powershell
Select-String -Pattern "XMemAlloc" xdk-api-index.tsv
```

## API categories

Approximate file counts by API area (based on filename prefixes):

| Category | Files |
|----------|------:|
| Other | 6,853 |
| D3D / Graphics | 1,926 |
| Math / Intrinsics | 1,334 |
| XUI | 999 |
| XACT / Audio | 246 |
| Debug / Profiling | 220 |
| XContent / Storage | 97 |
| XNet / Networking | 79 |
| XInput / Controllers | 78 |
| XMemory / Alloc | 37 |
| XAM / System | 18 |
| Compression | 10 |
| Winsock | 7 |
| DVD / File Cache | 2 |
| **Total** | **11,906** |
