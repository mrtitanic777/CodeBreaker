; vkd_pcode_10.asm -- VKD object 10 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VKD_PM_API:
        mov    eax, dword ptr [ebp + 1ch]
        cmp    eax, 2
        jae    SHORT loc_10_0012
        shl    eax, 2
        db  02Eh, 0FFh, 0A0h, 018h, 000h, 000h, 000h
loc_10_0012:
        or     word ptr [ebp + 2ch], 1
        ret
loc_10_0018:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sub    al, 0
        add    byte ptr [eax], al
loc_10_0020:
        mov    dword ptr [ebp + 1ch], 201h
        and    byte ptr [ebp + 2ch], 0feh
        ret
loc_10_002C:
        pushad
        and    byte ptr [ebp + 2ch], 0feh
        mov    ebx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        db  080h, 03Dh, 028h, 012h, 000h, 000h, 000h
        jne    NEAR PTR loc_10_0123
        or     ebx, ebx
        je     SHORT loc_10_0064
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     NEAR PTR loc_10_011E
        db  03Bh, 01Dh, 014h, 012h, 000h, 000h
        je     SHORT loc_10_006A
        jmp    NEAR PTR loc_10_011E
loc_10_0064:
        db  08Bh, 01Dh, 014h, 012h, 000h, 000h
loc_10_006A:
        mov    esi, ebx
        db  003h, 035h, 010h, 012h, 000h, 000h
        cmp    edx, -1
        je     SHORT loc_10_00B5
        and    edx, 707eh
        db  0A1h, 02Dh, 012h, 000h, 000h
        push   eax
        db  0FFh, 035h, 035h, 012h, 000h, 000h
        db  0C7h, 005h, 035h, 012h, 000h, 000h, 079h, 00Fh, 000h, 000h
        movzx  eax, word ptr [esi + 36h]
        push   ebx
        mov    ebx, edx
        mov    byte ptr [esi + 3ah], 0
        db  0E8h, 0B2h, 005h, 0F6h, 0FFh
        pop    ebx
        db  08Fh, 005h, 035h, 012h, 000h, 000h
        push   ecx
        push   edx
        mov    edx, esi
        db  0E8h, 0FDh, 000h, 0F6h, 0FFh
        pop    edx
        pop    ecx
loc_10_00B5:
        mov    al, ch
        movzx  ecx, cl
loc_10_00BA:
        cmp    edx, -1
        je     SHORT loc_10_00CE
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_10_00CE
        push   eax
        mov    al, 0e0h
        db  0E8h, 0ACh, 00Eh, 0F6h, 0FFh
        pop    eax
loc_10_00CE:
        db  0E8h, 0A6h, 00Eh, 0F6h, 0FFh
        jae    SHORT loc_10_00DE
        push   edx
        mov    edx, esi
        db  0E8h, 0D3h, 000h, 0F6h, 0FFh
        pop    edx
loc_10_00DE:
        loop   loc_10_00BA
        cmp    edx, -1
        je     SHORT loc_10_0115
        push   edx
        mov    edx, esi
        db  0E8h, 0C3h, 000h, 0F6h, 0FFh
        pop    edx
        xchg   dword ptr [esp], ebx
        movzx  eax, word ptr [esi + 36h]
        db  0FFh, 035h, 035h, 012h, 000h, 000h
        db  0C7h, 005h, 035h, 012h, 000h, 000h, 079h, 00Fh, 000h, 000h
        mov    byte ptr [esi + 3ah], 0
        db  0E8h, 047h, 005h, 0F6h, 0FFh
        db  08Fh, 005h, 035h, 012h, 000h, 000h
        pop    ebx
loc_10_0115:
        mov    edx, esi
        db  0E8h, 094h, 000h, 0F6h, 0FFh
        jmp    SHORT loc_10_0123
loc_10_011E:
        or     word ptr [ebp + 2ch], 1
loc_10_0123:
        popad
        ret
        db  020h, 056h, 04Bh
_PTEXT ends
        end
