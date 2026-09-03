; t20n3_pcode_05_02.asm -- T20N3 object 5 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_5087:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_5_50AE
loc_5_50AE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 13ah]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_50E4:
        xor    ax, ax
        jmp    NEAR PTR loc_5_50EC
loc_5_50EC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 13ah]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_5122:
        xor    ax, ax
        jmp    NEAR PTR loc_5_512A
loc_5_512A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_5131:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_514C:
        xor    ax, ax
        jmp    NEAR PTR loc_5_5154
loc_5_5154:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_515B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_5170:
        xor    ax, ax
        jmp    NEAR PTR loc_5_5178
loc_5_5178:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    3108h
_PTEXT ends
        end
