; vfd_lcode_01.asm -- VFD object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h
VFD_Control:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_001F
        cmp    eax, 1
        db  00Fh, 084h, 0EAh, 0FFh, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 0E1h, 0FFh, 0FEh, 0FFh
loc_1_001F:
        cmp    eax, 6
        je     SHORT loc_1_0026
        clc
        ret
loc_1_0026:
        db  080h, 03Dh, 0C8h, 001h, 000h, 000h, 0FFh
        jne    SHORT loc_1_0041
        in     al, 86h
        mov    ah, al
        and    ah, 0c0h
        cmp    ah, 40h
        jne    SHORT loc_1_0041
        or     al, 0c0h
        jmp    SHORT loc_1_003F
loc_1_003F:
        out    86h, al
loc_1_0041:
        clc
        ret
        db  0B8h, 000h, 004h, 000h, 000h, 0F8h, 0C3h, 08Bh, 0C0h
loc_1_004C:
        test   byte ptr [ebp + 14h], 80h
        jne    SHORT loc_1_0092
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0CDh, 020h, 005h, 000h, 005h, 000h
        movzx  eax, word ptr [ebp + 1ch]
        db  08Bh, 035h, 0BCh, 001h, 000h, 000h
        mov    word ptr [ebx + esi], ax
        cmp    ah, 2
        jne    SHORT loc_1_0085
        db  083h, 03Dh, 0C0h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_0085
        mov    word ptr [ebx + esi], 0
loc_1_0085:
        xor    eax, eax
        db  0BEh, 094h, 000h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_1_0092:
        stc
        ret
loc_1_0094:
        db  0CDh, 020h, 006h, 000h, 005h, 000h
        db  0A1h, 0BCh, 001h, 000h, 000h
        mov    word ptr [ebx + eax], 0
        test   byte ptr [ebp + 2ch], 1
        je     SHORT loc_1_00CA
        db  0A1h, 0C4h, 001h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        test   dl, 1
        jne    SHORT loc_1_00CA
        or     dl, 1
        db  0CDh, 020h, 005h, 000h, 004h, 000h
        db  0CDh, 020h, 012h, 000h, 004h, 000h
loc_1_00CA:
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_1_00D0:
        pushad
        db  08Bh, 03Dh, 0BCh, 001h, 000h, 000h
        add    edi, ebx
        movzx  ecx, word ptr [edi]
        jecxz  loc_1_012B
        movzx  edi, ch
        db  0CDh, 020h, 004h, 000h, 004h, 000h
        cmp    edi, 5
        je     SHORT loc_1_010D
        cmp    edi, 4
        je     SHORT loc_1_0132
        cmp    edi, 2
        jne    SHORT loc_1_012B
        test   dl, 0ch
        jne    SHORT loc_1_012B
        mov    edi, esi
        sub    edi, dword ptr [ebx + 4]
        cmp    edi, 0e0000h
        jb     SHORT loc_1_012B
        or     dl, 4
        jmp    SHORT loc_1_0125
loc_1_010D:
        mov    edi, dword ptr [78h]
        movzx  ecx, di
        shr    edi, 10h
        shl    edi, 4
        add    edi, ecx
        movzx  ecx, byte ptr [edi + 4]
        shl    ecx, 2
loc_1_0125:
        db  0CDh, 020h, 005h, 000h, 004h, 000h
loc_1_012B:
        popad
        db  0CDh, 020h, 012h, 080h, 004h, 000h
loc_1_0132:
        test   dl, 1
        jne    SHORT loc_1_0155
        push   ebx
        push   edx
        xor    bl, bl
        mov    bh, 0ffh
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        pop    edx
        pop    ebx
        db  0CDh, 020h, 006h, 000h, 004h, 000h
        db  0CDh, 020h, 008h, 000h, 004h, 000h
        jmp    SHORT loc_1_0169
loc_1_0155:
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 003h, 000h, 004h, 000h
        db  0CDh, 020h, 007h, 000h, 004h, 000h
loc_1_0169:
        popad
        ret
        db  000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 01Bh, 000h, 002h, 000h, 000h, 000h, 056h, 046h, 044h, 020h
        db  020h, 020h, 020h, 020h, 000h, 000h, 006h, 0A0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_01CC:
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    esi
        dec    esp
        dec    edi
        push   eax
        push   eax
        pop    ecx
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        push   ebx
        db  000h
loc_1_01DC:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        inc    esi
        inc    ecx
        push   ebx
        push   esp
        inc    esi
        dec    esp
        dec    edi
        push   eax
        push   eax
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
