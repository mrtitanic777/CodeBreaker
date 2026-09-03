; vmm_pcode_20.asm -- VMM object 20 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_20_0000:
        pushad
        clc
        mov    ecx, dword ptr [ebx - 38h]
        jecxz  loc_20_001A
        push   0
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        sub    eax, 1
loc_20_001A:
        popad
        ret
sub_20_001C:
        pushad
        mov    esi, dword ptr [ebx - 20h]
        push   100h
        push   0
        push   1
        push   dword ptr [esi + 50h]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_20_003C
        popad
        clc
        ret
loc_20_003C:
        popad
        stc
        ret
        db  000h
sub_20_0040:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 28h]
        cmp    esi, ebx
        je     SHORT loc_20_006F
        movzx  ebp, word ptr [edi - 34h]
        push   ebx
        push   ebp
        mov    eax, esi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        cmp    eax, ebx
        je     SHORT loc_20_00B9
        jmp    SHORT loc_20_0073
loc_20_006F:
        mov    ebp, dword ptr [esp + 10h]
loc_20_0073:
        mov    eax, 100h
        test   byte ptr [edi + 1], 20h
        je     SHORT loc_20_0080
        sub    eax, eax
loc_20_0080:
        push   eax
        push   110h
        mov    eax, dword ptr [edi + 4]
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_20_00B0
        or     esi, esi
        je     SHORT loc_20_00B9
        push   0
        push   ebp
        shr    esi, 0ch
        push   esi
        db  0FFh, 015h, 0B8h, 0D4h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_20_00B9
loc_20_00B0:
        or     dword ptr [edi + 24h], 4
        mov    ebx, 0ffffffffh
loc_20_00B9:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  000h, 000h, 000h
_PTEXT ends
        end
