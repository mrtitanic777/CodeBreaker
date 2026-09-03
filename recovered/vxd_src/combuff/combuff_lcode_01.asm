; combuff_lcode_01.asm -- COMBUFF object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

COMBUFF_Control:
        cmp    eax, 1
        db  00Fh, 084h, 0F7h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0
        db  00Fh, 084h, 0EEh, 0FFh, 0FEh, 0FFh
        clc
        ret
loc_1_0014:
        mov    edx, dword ptr [esi + 18h]
        add    dx, 2
        in     al, dx
        test   al, 1
        je     SHORT loc_1_002E
        stc
        ret
loc_1_0022:
        mov    edx, dword ptr [esi + 18h]
        add    dx, 2
        in     al, dx
        test   al, 1
        jne    SHORT loc_1_0062
loc_1_002E:
        and    al, 7
        movzx  eax, al
        db  0FFh, 024h, 045h, 054h, 002h, 000h, 000h
loc_1_003A:
        add    dx, 4
        in     al, dx
        and    al, 0fh
        mov    byte ptr [esi + 7ah], al
        or     byte ptr [esi + 7bh], 8
        jmp    SHORT loc_1_0022
loc_1_004A:
        or     byte ptr [esi + 7bh], 2
        jmp    SHORT loc_1_0022
loc_1_0050:
        add    dx, 3
loc_1_0054:
        in     al, dx
        and    al, 1fh
        je     SHORT loc_1_0022
        push   edx
        call   sub_1_00BB
        pop    edx
        jmp    SHORT loc_1_0054
loc_1_0062:
        mov    eax, dword ptr [esi + 20h]
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        call   sub_1_0134
        call   sub_1_007E
        clc
        ret
        db  090h
loc_1_0078:
        db  0CDh, 020h, 003h, 080h, 003h, 000h
sub_1_007E:
        push   eax
        push   ebx
        push   ecx
        mov    ebx, dword ptr [esi + 1ch]
        or     ebx, ebx
        je     SHORT loc_1_00B7
        mov    cl, byte ptr [esi + 7bh]
        and    cl, byte ptr [esi + 74h]
        je     SHORT loc_1_00B7
        test   byte ptr [esi + 75h], 80h
        jne    SHORT loc_1_00B7
        test   byte ptr [esi + 76h], 8
        je     SHORT loc_1_00B7
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     SHORT loc_1_00B7
        db  0CDh, 020h, 006h, 000h, 003h, 000h
        test   ecx, 1
        jne    SHORT loc_1_00B7
        db  0CDh, 020h, 002h, 000h, 003h, 000h
loc_1_00B7:
        pop    ecx
        pop    ebx
        pop    eax
        ret
sub_1_00BB:
        mov    ah, al
        test   ah, 1
        je     SHORT loc_1_00C6
        add    edx, -5
        in     al, dx
loc_1_00C6:
        mov    bl, byte ptr [esi + 78h]
        test   bl, 1
        jne    SHORT loc_1_00F0
        cmp    byte ptr [esi + 7ch], 0ffh
        je     SHORT loc_1_00F0
        or     bl, ah
        mov    byte ptr [esi + 78h], bl
        test   ah, 1
        je     SHORT loc_1_00EA
        mov    byte ptr [esi + 79h], al
        or     byte ptr [esi + 7bh], 1
        test   bl, 1eh
        jne    SHORT loc_1_00EB
loc_1_00EA:
        ret
loc_1_00EB:
        or     byte ptr [esi + 7bh], 4
        ret
loc_1_00F0:
        mov    bx, word ptr [esi + 85h]
        cmp    word ptr [esi + 83h], bx
        je     SHORT loc_1_0124
        movzx  ebx, word ptr [esi + 7fh]
        mov    word ptr [esi + ebx*2 + 87h], ax
        inc    word ptr [esi + 83h]
        inc    ebx
        cmp    bx, word ptr [esi + 85h]
        jb     SHORT loc_1_011F
        xor    ebx, ebx
loc_1_011F:
        mov    word ptr [esi + 7fh], bx
        ret
loc_1_0124:
        movzx  ebx, word ptr [esi + 7fh]
        or     ah, 2
        mov    word ptr [esi + ebx*2 + 87h], ax
        ret
sub_1_0134:
        cmp    word ptr [esi + 83h], 0
        je     SHORT loc_1_01A3
        cmp    byte ptr [esi + 7ch], 0ffh
        je     SHORT loc_1_01A3
loc_1_0144:
        test   byte ptr [esi + 78h], 1
        jne    SHORT loc_1_01A3
        dec    word ptr [esi + 83h]
        movzx  ebx, word ptr [esi + 81h]
        mov    dx, word ptr [esi + ebx*2 + 87h]
        inc    ebx
        cmp    bx, word ptr [esi + 85h]
        jb     SHORT loc_1_016C
        xor    ebx, ebx
loc_1_016C:
        mov    word ptr [esi + 81h], bx
        or     dh, byte ptr [esi + 78h]
        mov    byte ptr [esi + 78h], dh
        test   dh, 1
        je     SHORT loc_1_0190
        mov    byte ptr [esi + 79h], dl
        or     byte ptr [esi + 7bh], 1
        test   dh, 1eh
        je     SHORT loc_1_01A3
        or     byte ptr [esi + 7bh], 4
        jmp    SHORT loc_1_01A3
loc_1_0190:
        cmp    word ptr [esi + 83h], 0
        jne    SHORT loc_1_0144
        test   dh, 1eh
        je     SHORT loc_1_01A3
        or     byte ptr [esi + 7bh], 4
loc_1_01A3:
        ret
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 042h
        db  055h, 046h, 046h, 020h, 0FFh, 0FFh, 0FFh, 02Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 078h, 000h, 000h, 000h
        db  005h, 002h, 000h, 000h, 07Eh, 000h, 000h, 000h, 0CCh, 000h, 000h, 000h, 0E8h, 000h, 000h, 000h
        db  024h, 001h, 000h, 000h, 030h, 001h, 000h, 000h, 045h, 001h, 000h, 000h, 06Dh, 001h, 000h, 000h
        db  086h, 001h, 000h, 000h, 08Ch, 001h, 000h, 000h, 093h, 001h, 000h, 000h, 099h, 001h, 000h, 000h
        db  0B0h, 001h, 000h, 000h, 0D2h, 001h, 000h, 000h, 0EEh, 001h, 000h, 000h, 0FDh, 001h, 000h, 000h
loc_1_0244:
        add    dword ptr [edx + eax], eax
        add    al, 6
        push   es
        push   es
        push   es
        add    byte ptr [edx + eax], al
        add    al, 6
        push   es
        push   es
        push   es
loc_1_0254:
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        dec    edx
        add    byte ptr [eax], al
        db  000h, 050h, 000h
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
