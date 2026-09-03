; vpmtd_lcode_01.asm -- VPMTD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

VPMTD_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        clc
        ret
VPMTD_PM_API:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_1_0025
        movzx  eax, word ptr [ebp + 4]
        cmp    eax, 3
        ja     SHORT loc_1_0025
        db  0FFh, 024h, 085h, 038h, 002h, 000h, 000h
loc_1_0025:
        or     dword ptr [ebp + 2ch], 1
        ret
loc_1_002A:
        and    dword ptr [ebp + 2ch], 0fffffffeh
        ret
VPMTD_SYS_CRITICAL_INIT:
        mov    ax, word ptr [ebp + 1ch]
        db  066h, 0A3h, 020h, 002h, 000h, 000h
        mov    ax, word ptr [ebp + 10h]
        db  066h, 0A3h, 022h, 002h, 000h, 000h
        mov    ax, word ptr [ebp + 18h]
        db  066h, 0A3h, 024h, 002h, 000h, 000h
        xor    eax, eax
        db  066h, 0A1h, 024h, 002h, 000h, 000h
        db  0BEh, 096h, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        or     esi, esi
        jne    SHORT loc_1_006C
        mov    word ptr [ebp + 1ch], 0
        jmp    SHORT loc_1_0025
loc_1_006C:
        db  0C6h, 005h, 026h, 002h, 000h, 000h, 000h
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_1_002A
VPMTD_DEVICE_INIT:
        db  0C6h, 005h, 026h, 002h, 000h, 000h, 002h
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_1_002A
VPMTD_INIT_COMPLETE:
        mov    ax, word ptr [ebp + 18h]
        db  066h, 0A3h, 024h, 002h, 000h, 000h
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_1_002A
VPMTD_SYS_VM_INIT:
        mov    edi, ebx
        xor    eax, eax
        mov    ah, 62h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        db  066h, 089h, 01Dh, 032h, 002h, 000h, 000h
        xor    eax, eax
        mov    ah, 50h
        movzx  ebx, word ptr [ebp + 38h]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ebx, edi
        jb     SHORT loc_1_0111
        movzx  edx, word ptr [ebp + 14h]
        movzx  edi, word ptr [ebp + 3ch]
        or     edi, edi
        je     SHORT loc_1_00E9
        push   0
        push   edi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, -1
        je     SHORT loc_1_0111
        movzx  edx, word ptr [ebp + 14h]
        add    edx, eax
loc_1_00E9:
        movzx  eax, word ptr [ebp + 1ch]
        movzx  ebx, word ptr [ebp + 10h]
        movzx  ecx, word ptr [ebp + 18h]
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_0111
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 14h], dx
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 10h], bx
        jmp    SHORT loc_1_011D
loc_1_0111:
        mov    word ptr [ebp + 1ch], 0ffffh
        mov    word ptr [ebp + 14h], 0ffffh
loc_1_011D:
        xor    eax, eax
        mov    ah, 50h
        db  00Fh, 0B7h, 01Dh, 032h, 002h, 000h, 000h
        push   21h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        jb     SHORT loc_1_0137
        jmp    NEAR PTR loc_1_002A
loc_1_0137:
        jmp    NEAR PTR loc_1_0025
loc_1_013C:
        db  080h, 03Dh, 026h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_0150
        db  080h, 03Dh, 026h, 002h, 000h, 000h, 002h
        je     SHORT loc_1_0195
        jmp    SHORT loc_1_018E
loc_1_0150:
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  066h, 08Bh, 00Dh, 020h, 002h, 000h, 000h
        db  00Fh, 0B7h, 015h, 022h, 002h, 000h, 000h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        db  00Fh, 0B7h, 005h, 024h, 002h, 000h, 000h
        db  0BEh, 096h, 001h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_1_018E:
        db  0C6h, 005h, 026h, 002h, 000h, 000h, 000h
loc_1_0195:
        ret
loc_1_0196:
        db  080h, 03Dh, 026h, 002h, 000h, 000h, 000h
        je     SHORT loc_1_01AA
        db  080h, 03Dh, 026h, 002h, 000h, 000h, 002h
        je     SHORT loc_1_01CE
        jmp    SHORT loc_1_01CE
loc_1_01AA:
        xor    eax, eax
        mov    ecx, dword ptr [ebx]
        test   byte ptr [ecx], 1
        jne    SHORT loc_1_01B8
        mov    eax, 1000h
loc_1_01B8:
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    ecx, 0bh
        db  0BEh, 03Ch, 001h, 000h, 000h
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
loc_1_01CE:
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 03Ah, 000h, 001h, 000h, 000h, 000h, 056h, 050h, 04Dh, 054h
        db  044h, 020h, 020h, 020h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_0226:
        add    ah, byte ptr [ebx + 3ah]
        pop    esp
        inc    esi
        dec    edi
        dec    edi
        push   esp
        pop    eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 02Fh
        db  000h, 000h, 000h, 07Bh, 000h, 000h, 000h, 08Ah, 000h, 000h, 000h, 09Ch, 000h, 000h, 000h
_LTEXT ends
        end
