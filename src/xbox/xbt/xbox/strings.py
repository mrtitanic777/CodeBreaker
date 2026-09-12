"""String scanner — extract readable strings from memory or binary files."""

from typing import Optional


def scan_strings(data: bytes, base_addr: int = 0, min_length: int = 4,
                 encoding: str = "ascii") -> list:
    """Scan binary data for readable strings.
    
    Returns list of (address, string) tuples.
    """
    results = []
    
    if encoding == "ascii":
        current = []
        start = 0
        for i, b in enumerate(data):
            if 0x20 <= b <= 0x7E:
                if not current:
                    start = i
                current.append(chr(b))
            else:
                if len(current) >= min_length:
                    results.append((base_addr + start, "".join(current)))
                current = []
        if len(current) >= min_length:
            results.append((base_addr + start, "".join(current)))
    
    elif encoding == "utf16be":
        i = 0
        current = []
        start = 0
        while i + 1 < len(data):
            ch = (data[i] << 8) | data[i + 1]
            if 0x20 <= ch <= 0x7E or ch in (0x0A, 0x0D, 0x09):
                if not current:
                    start = i
                current.append(chr(ch))
            else:
                if len(current) >= min_length:
                    results.append((base_addr + start, "".join(current)))
                current = []
            i += 2
        if len(current) >= min_length:
            results.append((base_addr + start, "".join(current)))
    
    elif encoding == "utf16le":
        i = 0
        current = []
        start = 0
        while i + 1 < len(data):
            ch = data[i] | (data[i + 1] << 8)
            if 0x20 <= ch <= 0x7E or ch in (0x0A, 0x0D, 0x09):
                if not current:
                    start = i
                current.append(chr(ch))
            else:
                if len(current) >= min_length:
                    results.append((base_addr + start, "".join(current)))
                current = []
            i += 2
        if len(current) >= min_length:
            results.append((base_addr + start, "".join(current)))
    
    return results


def categorize_strings(strings: list) -> dict:
    """Categorize found strings by likely type."""
    categories = {
        "file_paths": [],
        "error_messages": [],
        "function_names": [],
        "debug_strings": [],
        "urls": [],
        "registry_keys": [],
        "other": [],
    }
    
    for addr, s in strings:
        sl = s.lower()
        if "\\" in s or "/" in s or s.endswith(".exe") or s.endswith(".xex") or s.endswith(".dll"):
            categories["file_paths"].append((addr, s))
        elif "error" in sl or "fail" in sl or "invalid" in sl or "cannot" in sl:
            categories["error_messages"].append((addr, s))
        elif s.startswith("Xe") or s.startswith("Nt") or s.startswith("Ke") or s.startswith("Mm"):
            categories["function_names"].append((addr, s))
        elif "http" in sl or "xbox" in sl or ".com" in sl:
            categories["urls"].append((addr, s))
        elif s.startswith("HKEY") or s.startswith("Software\\"):
            categories["registry_keys"].append((addr, s))
        elif "debug" in sl or "assert" in sl or "trace" in sl or "%s" in s or "%d" in s:
            categories["debug_strings"].append((addr, s))
        else:
            categories["other"].append((addr, s))
    
    return categories


DESCRIPTION = "scan an Xbox 360 binary for strings (categorized, ASCII+UTF-16)"


def main():
    import sys
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print("usage: xstrings <file> [min_len]"); return
    data = open(a[0], "rb").read()
    mn = int(a[1]) if len(a) > 1 else 4
    found = scan_strings(data, 0, mn)
    cats = categorize_strings(found)

    def _text(s):
        if isinstance(s, (tuple, list)):
            return s[1] if len(s) > 1 else s[0]         # (address, string)
        return getattr(s, "text", s)
    for cat, items in cats.items():
        if items:
            print("[%s] %d" % (cat, len(items)))
            for s in items[:10]:
                print("   %s" % _text(s))
    print("total: %d strings" % len(found))


if __name__ == "__main__":
    main()
