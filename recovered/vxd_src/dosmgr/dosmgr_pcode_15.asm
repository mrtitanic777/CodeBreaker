; dosmgr_pcode_15.asm -- DOSMGR object 15 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

DOSMGR_DESTROY_VM:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        btr    word ptr [edx + ebx], 7
        jae    SHORT loc_15_0018
        mov    esi, dword ptr [edx + ebx + 44h]
        db  0CDh, 020h, 012h, 000h, 001h, 000h
loc_15_0018:
        jmp    SHORT loc_15_0031
DOSMGR_VM_NOT_EXECUTEABLE:
        call   sub_15_0190
        db  03Bh, 01Dh, 050h, 000h, 000h, 000h
        jne    SHORT loc_15_0031
        db  0C7h, 005h, 050h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_15_0031:
        db  003h, 01Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        xchg   dword ptr [ebx + 48h], eax
        or     eax, eax
        je     SHORT loc_15_004C
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_15_004C:
        xor    eax, eax
        xchg   dword ptr [ebx + 20h], eax
        or     eax, eax
        je     SHORT loc_15_0061
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_15_0061:
        clc
        ret
DOSMGR_CLOSE_VM_NOTIFY:
        sub    esp, 4
        mov    dword ptr [esp], 1
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        db  08Bh, 015h, 010h, 000h, 000h, 000h
        shl    edx, 0ch
        shr    dx, 0ch
        add    dx, 174h
        mov    word ptr [ebp + 30h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 34h], dx
loc_15_0090:
        mov    byte ptr [ebp + 1dh], 51h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  eax, word ptr [ebp + 10h]
        db  066h, 03Bh, 005h, 034h, 000h, 000h, 000h
        je     NEAR PTR loc_15_013A
        shl    eax, 4
        mov    edi, eax
        push   0
        push   100h
        push   eax
        db  0CDh, 020h, 05Fh, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_15_0150
        mov    ecx, dword ptr [ebx + 4]
        mov    ax, word ptr [ebp + 28h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 24h]
        mov    dword ptr [ecx + edi + 0ah], eax
        movzx  esi, word ptr [ecx + edi + 16h]
        shl    esi, 4
        cmp    esi, edi
        jne    SHORT loc_15_010C
        db  0CDh, 020h, 06Bh, 000h, 001h, 000h
        shr    esi, 0ch
        cmp    eax, esi
        je     SHORT loc_15_00FD
        and    dword ptr [esp], 0
loc_15_00FD:
        db  00Fh, 0B7h, 035h, 034h, 000h, 000h, 000h
        mov    word ptr [ecx + edi + 16h], si
        shl    esi, 4
loc_15_010C:
        mov    ax, word ptr [ebp + 34h]
        shl    eax, 10h
        mov    ax, word ptr [ebp + 30h]
        sub    ax, 18h
        xchg   dword ptr [ecx + esi + 2eh], eax
        mov    word ptr [ebp + 1ch], 4cffh
        push   eax
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        pop    dword ptr [ecx + esi + 2eh]
        jmp    NEAR PTR loc_15_0090
loc_15_013A:
        cmp    dword ptr [esp], 0
        je     SHORT loc_15_0150
        mov    ecx, 1
        mov    eax, 80000000h
        db  0CDh, 020h, 0E0h, 000h, 001h, 000h
loc_15_0150:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        add    esp, 4
        clc
        ret
        db  000h
loc_15_015C:
        mov    word ptr [ebp + 30h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 34h], dx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        bts    word ptr [ecx + ebx], 1
        db  00Fh, 083h, 0D5h, 000h, 0F1h, 0FFh
        btr    word ptr [ecx + ebx], 5
        cmp    dword ptr [ecx + ebx + 18h], 0
        je     SHORT loc_15_018E
        mov    edx, dword ptr [ecx + ebx + 1ch]
        call   dword ptr [ecx + ebx + 18h]
loc_15_018E:
        ret
        db  000h
sub_15_0190:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        sub    ecx, ecx
        xchg   dword ptr [edi + ebx + 5ch], ecx
        jecxz  loc_15_01AA
        push   0
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_15_01AA:
        and    byte ptr [edi + ebx + 1], 0efh
        db  083h, 03Dh, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_15_01DF
        mov    ecx, dword ptr [edi + ebx + 54h]
        jecxz  loc_15_022C
        or     eax, 0ffffffffh
        db  0E8h, 0DBh, 000h, 0F1h, 0FFh
        sub    eax, eax
        mov    dword ptr [edi + ebx + 54h], eax
        push   eax
        push   ecx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        db  0FFh, 00Dh, 000h, 001h, 000h, 000h
        jne    SHORT loc_15_022C
loc_15_01DF:
        db  08Bh, 00Dh, 004h, 001h, 000h, 000h
        jecxz  loc_15_022C
        push   ebx
        push   esi
        push   edi
        mov    eax, 80000000h
        sub    edx, edx
        db  08Bh, 01Dh, 008h, 001h, 000h, 000h
        db  0BEh, 044h, 003h, 000h, 000h
loc_15_01FC:
        mov    edi, dword ptr [ebx + edx*4]
        cmp    edi, esi
        je     SHORT loc_15_020E
        db  0CDh, 020h, 03Dh, 001h, 001h, 000h
        jb     SHORT loc_15_0229
        mov    dword ptr [ebx + edx*4], esi
loc_15_020E:
        inc    eax
        inc    edx
        loop   loc_15_01FC
        db  089h, 00Dh, 004h, 001h, 000h, 000h
        db  089h, 00Dh, 008h, 001h, 000h, 000h
        push   ecx
        push   ebx
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
loc_15_0229:
        pop    edi
        pop    esi
        pop    ebx
loc_15_022C:
        ret
        db  000h, 000h, 000h
DOSMGR_END_PM_APP:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, ebx
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        call   sub_15_0284
        call   sub_15_02AB
        push   ebx
        mov    ebx, edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        xchg   edi, ebx
        mov    ax, word ptr [edi + 16h]
        mov    word ptr [ebx + 2ah], ax
        pop    ebx
        mov    ax, word ptr [edi + 14h]
        mov    word ptr [esi + 36h], ax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 38h], eax
        xor    eax, eax
        mov    dword ptr [esi + 3ch], eax
loc_15_026F:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     SHORT loc_15_0282
        push   edi
        movzx  edi, word ptr [eax + 8]
        db  0E8h, 04Dh, 00Ch, 0F1h, 0FFh
        pop    edi
        jmp    SHORT loc_15_026F
loc_15_0282:
        clc
        ret
sub_15_0284:
        pushad
        mov    cx, word ptr [edi + 8]
        mov    edx, dword ptr [edi + 4]
        mov    eax, 23h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        mov    cx, word ptr [edi + 0eh]
        mov    edx, dword ptr [edi + 0ah]
        mov    eax, 24h
        db  0CDh, 020h, 045h, 000h, 001h, 000h
        popad
        ret
sub_15_02AB:
        push   edi
        push   fs
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    fs, word ptr [edi + 2ah]
        movzx  eax, word ptr fs:[2ch]
        push   0
        push   eax
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        cmp    eax, dword ptr [ebx + 4]
        jb     SHORT loc_15_02D5
        sub    eax, dword ptr [ebx + 4]
loc_15_02D5:
        shr    eax, 4
        test   eax, 0ffff0000h
        je     SHORT loc_15_02E1
        xor    eax, eax
loc_15_02E1:
        xchg   word ptr fs:[2ch], ax
        pop    fs
        pop    edi
        ret
loc_15_02ED:
        pushad
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        db  003h, 03Dh, 004h, 000h, 000h, 000h
        call   sub_15_0284
        call   sub_15_02AB
        mov    eax, 21h
        db  0CDh, 020h, 046h, 000h, 001h, 000h
        popad
        ret
loc_15_0311:
        pushad
        db  0CDh, 020h, 088h, 000h, 001h, 000h
        mov    eax, 16h
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        mov    eax, 21h
        db  0CDh, 020h, 046h, 000h, 001h, 000h
        popad
        ret
_PTEXT ends
        end
