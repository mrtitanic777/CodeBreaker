; vmm_pcode_22.asm -- VMM object 22 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

Get_Config_Directory:
        db  0BAh, 0A8h, 001h, 000h, 000h
        ret
        db  000h, 000h, 0EBh, 006h, 0FFh, 025h, 021h, 000h, 000h, 000h
loc_22_0010:
        cmp    word ptr [ebx - 66h], 0
        db  00Fh, 085h, 0FBh, 014h, 0EAh, 0FFh
        db  0FFh, 025h, 021h, 000h, 000h, 000h
loc_22_0021:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        db  0A3h, 015h, 004h, 09Fh, 000h
        add    byte ptr [edx + 39h], dh
        cmp    ecx, 4
        ja     SHORT loc_22_003C
        jb     SHORT loc_22_003A
        out    dx, al
        ret
loc_22_003A:
        in     al, dx
        ret
loc_22_003C:
        push   edx
        push   ecx
        shr    ecx, 3
        or     ecx, 1
loc_22_0044:
        inc    edx
        db  00Fh, 0A3h, 015h, 004h, 09Fh, 000h, 000h
        jb     SHORT loc_22_0068
        loop   loc_22_0044
        pop    ecx
        pop    edx
        cmp    ecx, 0ch
        ja     SHORT loc_22_005F
        jb     SHORT loc_22_005C
        out    dx, ax
        ret
loc_22_005C:
        in     ax, dx
        ret
loc_22_005F:
        cmp    ecx, 14h
        jb     SHORT loc_22_0066
        out    dx, eax
        ret
loc_22_0066:
        in     eax, dx
        ret
loc_22_0068:
        pop    ecx
        pop    edx
        pushad
        db  0E8h, 0D0h, 02Ch, 0EAh, 0FFh
        db  08Bh, 01Dh, 00Ch, 0F7h, 000h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0E8h, 096h, 02Eh, 0EAh, 0FFh
        test   byte ptr [esp + 18h], 4
        jne    SHORT loc_22_0089
        mov    dword ptr [esp + 1ch], eax
loc_22_0089:
        popad
        ret
        db  000h
loc_22_008C:
        movzx  eax, word ptr [ebp + 10h]
        or     eax, eax
        jne    SHORT loc_22_00A0
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
loc_22_00A0:
        db  0CDh, 020h, 046h, 001h, 001h, 000h
        jecxz  loc_22_00FE
        mov    esi, 1ch
        mov    edi, 24h
        test   byte ptr [ebx], 20h
        je     SHORT loc_22_00C1
        mov    esi, 20h
        mov    edi, 28h
loc_22_00C1:
        lea    edx, [esi + ecx]
        cmp    dword ptr [edx], 0
        je     SHORT loc_22_00FE
        mov    eax, dword ptr [edi + ecx]
        test   eax, eax
        jne    SHORT loc_22_00F1
        db  0BEh, 011h, 036h, 000h, 000h
        test   byte ptr [ebx], 20h
        je     SHORT loc_22_00E2
        db  0CDh, 020h, 00Ch, 000h, 001h, 000h
        jmp    SHORT loc_22_00E8
loc_22_00E2:
        db  0CDh, 020h, 00Bh, 000h, 001h, 000h
loc_22_00E8:
        cmp    eax, -1
        sbb    eax, -1
        mov    dword ptr [edi + ecx], eax
loc_22_00F1:
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        clc
        ret
loc_22_00FE:
        xor    eax, eax
        jmp    SHORT loc_22_00F1
loc_22_0102:
        test   byte ptr [ebx], 20h
        jne    SHORT loc_22_016F
        movzx  eax, word ptr [ebp + 10h]
        mov    ecx, ebx
loc_22_010D:
        cmp    eax, dword ptr [ebx + 0ch]
        je     SHORT loc_22_0124
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, ecx
        jne    SHORT loc_22_010D
        mov    word ptr [ebp + 1ch], 1
        jmp    SHORT loc_22_016F
loc_22_0124:
        mov    ax, word ptr [ebp + 14h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 4]
        cmp    eax, 40000000h
        jb     SHORT loc_22_013E
        mov    word ptr [ebp + 1ch], 2
        jmp    SHORT loc_22_016F
loc_22_013E:
        movzx  ecx, word ptr [ebp + 18h]
        test   ecx, 0fffffffch
        je     SHORT loc_22_0152
        mov    word ptr [ebp + 1ch], 3
        jmp    SHORT loc_22_016F
loc_22_0152:
        mov    dx, word ptr [ebp + 38h]
        shl    edx, 10h
        mov    dx, word ptr [ebp]
        db  0BEh, 076h, 001h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        and    word ptr [ebp + 2ch], 0fffeh
        clc
        ret
loc_22_016F:
        or     word ptr [ebp + 2ch], 1
        clc
        ret
loc_22_0176:
        mov    ecx, edx
        shr    ecx, 10h
        movzx  edx, dx
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 080h, 001h, 000h
loc_22_0196:
        push   ebx
        push   edx
        xor    ebx, ebx
        xor    edx, edx
        or     eax, eax
        jne    SHORT loc_22_01A5
        mov    ebx, dword ptr [edi]
        mov    edx, dword ptr [edi + 4]
loc_22_01A5:
        db  08Bh, 00Dh, 040h, 0C3h, 000h, 000h
        jmp    SHORT loc_22_01AF
loc_22_01AD:
        mov    ecx, dword ptr [ecx]
loc_22_01AF:
        jecxz  loc_22_01C5
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_22_01BB
        cmp    dword ptr [ecx + 10h], edx
        je     SHORT loc_22_01C5
loc_22_01BB:
        cmp    word ptr [ecx + 6], ax
        jne    SHORT loc_22_01AD
        or     eax, eax
        je     SHORT loc_22_01AD
loc_22_01C5:
        pop    edx
        pop    ebx
        ret
loc_22_01C8:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0ch]
        add    ecx, edx
        dec    ecx
        xor    eax, eax
        db  08Bh, 03Dh, 0F9h, 0F1h, 000h, 000h
        jmp    SHORT loc_22_01E0
loc_22_01DE:
        mov    edi, dword ptr [edi]
loc_22_01E0:
        test   edi, edi
        je     SHORT loc_22_01FA
        cmp    ecx, dword ptr [edi + 4]
        jb     SHORT loc_22_01FA
        cmp    edx, dword ptr [edi + 8]
        ja     SHORT loc_22_01DE
        inc    eax
        cmp    edx, dword ptr [edi + 4]
        jb     SHORT loc_22_01FA
        cmp    ecx, dword ptr [edi + 8]
        ja     SHORT loc_22_01FA
        inc    eax
loc_22_01FA:
        pop    edi
        ret
_PTEXT ends
        end
