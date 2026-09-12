import struct, os, glob
for f in sorted(glob.glob('files/WF/*')):
    if os.path.isdir(f): continue
    d = open(f,'rb').read()
    name = os.path.basename(f)
    sig = d[:2]
    tag = ''
    if sig == b'MZ':
        # look at e_lfanew (0x3C) for NE/PE
        if len(d) >= 0x40:
            lfa = struct.unpack('<H', d[0x3C:0x3E])[0]
            if lfa and lfa+2 <= len(d):
                sub = d[lfa:lfa+2]
                tag = f"MZ->{sub.decode('latin1') if sub.isalpha() else sub.hex()} @0x{lfa:x}"
            else:
                tag = "MZ (plain DOS)"
    elif sig[:1] == b'\xe9' or sig[:1]==b'\xeb':
        tag = "COM/boot (jmp)"
    else:
        tag = sig.hex()
    print(f"{name:16} {len(d):>8}  {tag}")
