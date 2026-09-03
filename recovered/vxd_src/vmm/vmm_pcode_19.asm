; vmm_pcode_19.asm -- VMM object 19 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

sub_19_0000:
        pushad
        mov    ecx, dword ptr [ebx - 38h]
        jecxz  loc_19_0016
        push   0
        push   0
        push   1
        push   ecx
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_19_0016:
        popad
        ret
sub_19_0018:
        pushad
        mov    esi, dword ptr [ebx - 20h]
        push   100h
        push   0
        push   1
        push   dword ptr [esi + 50h]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        popad
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_19_0040:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 14h]
        sub    esi, esi
        mov    eax, dword ptr [ebx + 28h]
        cmp    eax, esi
        je     SHORT loc_19_006C
        movzx  edi, word ptr [ebx - 34h]
        push   esi
        push   edi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B8h, 0D4h, 000h, 000h
        add    esp, 0ch
        cmp    eax, esi
        je     SHORT loc_19_00BF
        jmp    SHORT loc_19_0070
loc_19_006C:
        mov    edi, dword ptr [esp + 0ch]
loc_19_0070:
        push   ebx
        db  0E8h, 096h, 001h, 0EDh, 0FFh
        mov    eax, 100h
        test   byte ptr [ebx + 1], 20h
        je     SHORT loc_19_0083
        sub    eax, eax
loc_19_0083:
        push   eax
        push   110h
        mov    eax, dword ptr [ebx + 4]
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B8h, 0D4h, 000h, 000h
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_19_00B6
        mov    eax, dword ptr [ebx + 28h]
        or     eax, eax
        je     SHORT loc_19_00BF
        push   0
        push   edi
        shr    eax, 0ch
        push   eax
        db  0FFh, 015h, 0B4h, 0D4h, 000h, 000h
        add    esp, 0ch
        jmp    SHORT loc_19_00BF
loc_19_00B6:
        and    dword ptr [ebx + 24h], 0fffffffbh
        mov    esi, 0ffffffffh
loc_19_00BF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
        db  000h, 000h
_PTEXT ends
        end
