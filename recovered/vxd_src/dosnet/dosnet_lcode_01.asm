; dosnet_lcode_01.asm -- DOSNET object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

DOSNET_Control:
        sub    eax, 1
        cmp    eax, 5
        jae    SHORT loc_1_000F
        db  0FFh, 024h, 085h, 011h, 000h, 000h, 000h
loc_1_000F:
        clc
        ret
        db  000h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 02Eh, 000h, 000h, 000h
        db  00Ch, 000h, 000h, 000h, 02Eh, 08Bh, 0C0h
loc_1_0028:
        cmp    byte ptr [ebp + 1dh], 5fh
        jne    NEAR PTR loc_1_0126
        mov    al, byte ptr [ebp + 1ch]
        cmp    al, 47h
        jae    NEAR PTR loc_1_0128
        cmp    byte ptr [ebp + 1ch], 4
        je     SHORT loc_1_00BF
        cmp    byte ptr [ebp + 1ch], 3
        jne    NEAR PTR loc_1_0126
        cmp    byte ptr [ebp + 10h], 84h
        je     SHORT loc_1_005D
        cmp    byte ptr [ebp + 10h], 4
        jne    NEAR PTR loc_1_0126
loc_1_005D:
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  eax, word ptr [ebp + 4]
        add    esi, eax
        movzx  edi, word ptr [ebp + 38h]
        shl    edi, 4
        movzx  eax, word ptr [ebp]
        add    edi, eax
loc_1_0077:
        add    esi, dword ptr [ebx + 4]
        add    edi, dword ptr [ebx + 4]
        cmp    byte ptr [esi], 0
        je     NEAR PTR loc_1_0126
        cmp    word ptr [esi + 1], 3ah
        jne    NEAR PTR loc_1_0126
        movzx  eax, byte ptr [esi]
        or     al, 20h
        sub    al, 61h
        jb     NEAR PTR loc_1_0126
        cmp    al, 1ah
        jae    NEAR PTR loc_1_0126
        push   eax
        mov    ecx, 1
        db  0E8h, 0ADh, 0FFh, 0FEh, 0FFh
        pop    eax
        jb     SHORT loc_1_0126
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], 0
        jmp    SHORT loc_1_0116
loc_1_00BF:
        movzx  esi, word ptr [ebp + 3ch]
        shl    esi, 4
        movzx  eax, word ptr [ebp + 4]
        add    esi, eax
loc_1_00CC:
        add    esi, dword ptr [ebx + 4]
        cmp    word ptr [esi], 2f2fh
        je     SHORT loc_1_0126
        cmp    word ptr [esi], 5c5ch
        je     SHORT loc_1_0126
        cmp    word ptr [esi], 5c2fh
        je     SHORT loc_1_0126
        cmp    word ptr [esi], 2f5ch
        je     SHORT loc_1_0126
        cmp    word ptr [esi + 1], 3ah
        jne    SHORT loc_1_0126
        movzx  eax, byte ptr [esi]
        or     al, 20h
        sub    al, 61h
        jb     SHORT loc_1_0126
        cmp    al, 1ah
        jae    SHORT loc_1_0126
        push   eax
        mov    ecx, 1
        db  0E8h, 054h, 0FFh, 0FEh, 0FFh
        pop    eax
        jb     SHORT loc_1_0126
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], 1
loc_1_0116:
        mov    eax, 4e20h
        db  0BEh, 0A1h, 000h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_1_0126:
        stc
        ret
loc_1_0128:
        je     SHORT loc_1_017B
        cmp    al, 48h
        je     SHORT loc_1_019F
        cmp    al, 55h
        jne    SHORT loc_1_0126
        db  08Bh, 00Dh, 020h, 002h, 000h, 000h
        db  0E8h, 021h, 0FFh, 0FEh, 0FFh
        jb     SHORT loc_1_0126
        mov    dword ptr [edx], 0ffffffffh
        mov    dword ptr [edx + 8], 1
        mov    dword ptr [edx + 4], 0
        db  08Bh, 00Dh, 020h, 002h, 000h, 000h
        db  08Bh, 035h, 018h, 002h, 000h, 000h
        add    esi, dword ptr [ebx + 4]
        xor    eax, eax
loc_1_0164:
        test   word ptr [esi + 43h], 8000h
        je     SHORT loc_1_0170
        bts    dword ptr [edx + 4], eax
loc_1_0170:
        inc    eax
        db  003h, 035h, 01Ch, 002h, 000h, 000h
        loop   loc_1_0164
        jmp    SHORT loc_1_0116
loc_1_017B:
        movzx  esi, word ptr [ebp + 38h]
        shl    esi, 4
        movzx  eax, word ptr [ebp]
        add    esi, eax
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, edi
        movzx  eax, ax
        shr    edi, 10h
        shl    edi, 4
        add    edi, eax
        lea    esi, [esi]
        jmp    NEAR PTR loc_1_0077
loc_1_019F:
        movzx  esi, word ptr [ebp + 38h]
        shl    esi, 4
        movzx  eax, word ptr [ebp]
        add    esi, eax
        jmp    NEAR PTR loc_1_00CC
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 007h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 01Ah, 000h, 001h, 000h, 000h, 000h, 044h, 04Fh, 053h, 04Eh
        db  045h, 054h, 020h, 020h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0B4h, 001h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_021C:
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
_LTEXT ends
        end
