; t30n3_pcode_05_02.asm -- T30N3 object 5 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_505C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        xor    ax, ax
        jmp    NEAR PTR loc_5_5080
loc_5_5080:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_5087:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 191h]
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0ffffh
        out    dx, ax
        xor    ax, ax
        jmp    NEAR PTR loc_5_50B1
loc_5_50B1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_5_50B8:
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
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        jmp    NEAR PTR loc_5_50DC
loc_5_50DC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_50E3:
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
        jmp    NEAR PTR loc_5_510A
loc_5_510A:
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
loc_5_5140:
        xor    ax, ax
        jmp    NEAR PTR loc_5_5148
loc_5_5148:
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
loc_5_517E:
        xor    ax, ax
        jmp    NEAR PTR loc_5_5186
loc_5_5186:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_518D:
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
loc_5_51A8:
        xor    ax, ax
        jmp    NEAR PTR loc_5_51B0
loc_5_51B0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_51B7:
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
loc_5_51CC:
        xor    ax, ax
        jmp    NEAR PTR loc_5_51D4
loc_5_51D4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    3108h
_PTEXT ends
        end
