; ebios_icode_02.asm -- EBIOS object 2 segment _ITEXT
.386p
_ITEXT segment dword public use32 'ICODE'

loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        mov    edi, esp
        push   0
        push   edi
        push   0
        db  0CDh, 020h, 074h, 000h, 001h, 000h
        add    esp, 0ch
        db  08Bh, 015h, 054h, 000h, 000h, 000h
        movzx  edx, dx
        mov    ecx, 1
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        xchg   edx, eax
        cmp    eax, edx
        ja     SHORT loc_2_004F
        db  089h, 00Dh, 008h, 000h, 000h, 000h
        db  0FEh, 005h, 00Ch, 000h, 000h, 000h
        add    eax, ecx
        dec    eax
        cmp    eax, edx
        jbe    NEAR PTR loc_2_00D6
        sub    eax, edx
        mov    ecx, eax
        mov    eax, edx
        inc    eax
        jmp    SHORT loc_2_00AE
loc_2_004F:
        cmp    eax, 0a0h
        jb     SHORT loc_2_008A
        db  089h, 00Dh, 008h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 000h, 000h, 006h, 000h
        cmp    eax, 30ah
        jb     SHORT loc_2_00AD
        pop    ecx
        xor    ebx, ebx
        db  0CDh, 020h, 010h, 000h, 006h, 000h
        xchg   ecx, eax
        test   ecx, 1000h
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        je     SHORT loc_2_00AE
        db  0FEh, 005h, 00Ch, 000h, 000h, 000h
        jmp    SHORT loc_2_00D6
loc_2_008A:
        mov    edx, eax
        db  0CDh, 020h, 0D5h, 000h, 001h, 000h
        xchg   edx, eax
        cmp    eax, edx
        jbe    SHORT loc_2_0111
        cmp    eax, 96h
        jb     SHORT loc_2_00A5
        mov    ecx, 0a0h
        sub    ecx, eax
loc_2_00A5:
        db  089h, 00Dh, 008h, 000h, 000h, 000h
        jmp    SHORT loc_2_00AE
loc_2_00AD:
        pop    eax
loc_2_00AE:
        bt     dword ptr [edi], eax
        jb     SHORT loc_2_0111
        inc    eax
        loop   loc_2_00AE
        db  0A1h, 054h, 000h, 000h, 000h
        movzx  eax, ax
        push   0
        push   0
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        push   eax
        db  0CDh, 020h, 072h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_0111
loc_2_00D6:
        db  08Bh, 015h, 054h, 000h, 000h, 000h
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        cmp    dx, ax
        jb     SHORT loc_2_010C
        cmp    dx, 0a0h
        jae    SHORT loc_2_010C
        test   edx, 0ffff0000h
        je     SHORT loc_2_010C
        mov    eax, edx
        shr    eax, 10h
        movzx  edx, dx
        shl    edx, 8
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0A3h, 004h, 000h, 000h, 000h
loc_2_010C:
        mov    esp, ebp
        pop    ebp
        clc
        ret
loc_2_0111:
        pushad
        xor    esi, esi
        db  0CDh, 020h, 0BEh, 000h, 001h, 000h
        add    byte ptr [eax], al
_ITEXT ends
        end
