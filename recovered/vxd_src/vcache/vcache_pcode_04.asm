; vcache_pcode_04.asm -- VCACHE object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VCache_Register:
        push   ecx
        db  0A1h, 0A0h, 009h, 000h, 000h
        cmp    esi, -1
        je     NEAR PTR loc_4_00D3
        cmp    al, 0ah
        jae    NEAR PTR loc_4_00D3
        shl    eax, 4
        db  005h, 0C8h, 009h, 000h, 000h
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 4], ecx
        db  003h, 00Dh, 024h, 009h, 000h, 000h
        jb     NEAR PTR loc_4_00D3
        db  039h, 00Dh, 010h, 009h, 000h, 000h
        jb     NEAR PTR loc_4_00D3
        db  089h, 00Dh, 024h, 009h, 000h, 000h
        mov    dword ptr [eax], 0
        db  0FFh, 005h, 0A0h, 009h, 000h, 000h
        mov    ecx, dword ptr [eax + 4]
        jecxz  loc_4_00A9
        push   ebx
        push   edx
        push   edi
        push   esi
loc_4_0058:
        push   eax
        push   ecx
        db  08Ah, 025h, 0A0h, 009h, 000h, 000h
        add    ah, 63h
        mov    al, 13h
        mov    ebx, ecx
        mov    edi, ecx
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    ecx
        pop    eax
        or     esi, esi
        je     SHORT loc_4_00B3
        loop   loc_4_0058
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
loc_4_0080:
        db  08Ah, 025h, 0A0h, 009h, 000h, 000h
        add    ah, 63h
        xor    al, al
        mov    ebx, ecx
        mov    edi, ecx
        push   ecx
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        pop    ecx
        loop   loc_4_0080
        pop    esi
        pop    edi
        pop    edx
        pop    ebx
loc_4_00A9:
        db  0A0h, 0A0h, 009h, 000h, 000h
        add    al, 63h
        clc
loc_4_00B1:
        pop    ecx
        ret
loc_4_00B3:
        mov    ebx, eax
        db  0BAh, 0D6h, 000h, 000h, 000h
        db  08Ah, 025h, 0A0h, 009h, 000h, 000h
        add    ah, 63h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        pop    esi
        pop    edi
        pop    edx
        pop    ebx
        db  0FFh, 00Dh, 0A0h, 009h, 000h, 000h
loc_4_00D3:
        stc
        jmp    SHORT loc_4_00B1
loc_4_00D6:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        ret
VCache_Deregister:
        push   eax
        xor    ecx, ecx
        db  0CDh, 020h, 010h, 000h, 08Bh, 004h
        pop    eax
        jb     SHORT loc_4_0111
        movzx  esi, ah
        sub    esi, 64h
        shl    esi, 4
        db  081h, 0C6h, 0C8h, 009h, 000h, 000h
        or     ebx, 0ffffffffh
        xchg   dword ptr [esi + 8], ebx
        db  0BAh, 012h, 001h, 000h, 000h
        push   esi
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        pop    esi
loc_4_0111:
        ret
loc_4_0112:
        mov    word ptr [esi + 30h], 0
        mov    byte ptr [esi + 32h], 0
        pushad
        call   ebx
        popad
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        ret
VCache_AdjustMinimum:
        cmp    ecx, 40h
        ja     NEAR PTR loc_4_0249
        movzx  esi, ah
        sub    esi, 64h
        jb     NEAR PTR loc_4_0249
        db  03Bh, 035h, 0A0h, 009h, 000h, 000h
        jae    NEAR PTR loc_4_0248
        shl    esi, 4
        db  081h, 0C6h, 0C8h, 009h, 000h, 000h
        cmp    dword ptr [esi + 4], ecx
        jb     SHORT loc_4_0180
        xchg   dword ptr [esi + 4], ecx
        sub    ecx, dword ptr [esi + 4]
        mov    ebx, dword ptr [esi]
        add    dword ptr [esi], ecx
        db  029h, 00Dh, 024h, 009h, 000h, 000h
        cmp    ebx, 0
        jge    SHORT loc_4_017F
        neg    ebx
        cmp    ebx, ecx
        ja     SHORT loc_4_0173
        mov    ecx, ebx
loc_4_0173:
        db  001h, 00Dh, 0A8h, 009h, 000h, 000h
        db  001h, 00Dh, 0ACh, 009h, 000h, 000h
loc_4_017F:
        ret
loc_4_0180:
        sub    ecx, dword ptr [esi + 4]
        sub    eax, eax
        mov    edx, dword ptr [esi]
        cmp    edx, 0
        jle    SHORT loc_4_01A5
        mov    eax, edx
        sub    eax, ecx
        sbb    edx, edx
        and    eax, edx
        add    eax, ecx
        add    dword ptr [esi + 4], eax
        sub    dword ptr [esi], eax
        db  001h, 005h, 024h, 009h, 000h, 000h
        sub    ecx, eax
        je     SHORT loc_4_0219
loc_4_01A5:
        push   eax
        mov    eax, esi
        db  02Dh, 0C8h, 009h, 000h, 000h
        shr    eax, 4
        mov    ah, al
        add    ah, 64h
        pushad
        db  0BAh, 0E0h, 006h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        popad
        push   eax
        mov    al, 13h
        mov    ebx, esi
        mov    edi, esi
        sub    ecx, dword ptr [esi]
        push   0
        jmp    SHORT loc_4_01D5
loc_4_01CF:
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
loc_4_01D5:
        inc    edi
        push   eax
        push   ecx
        db  0CDh, 020h, 004h, 000h, 08Bh, 004h
        pop    ecx
        pop    eax
        jae    SHORT loc_4_01CF
        or     esi, esi
        je     SHORT loc_4_021B
        push   esi
        loop   loc_4_01D5
        xor    esi, esi
        xchg   dword ptr [ebx], esi
        add    dword ptr [ebx + 4], esi
        db  001h, 035h, 024h, 009h, 000h, 000h
loc_4_01F6:
        pop    esi
        cmp    esi, 0
        je     SHORT loc_4_020A
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        jmp    SHORT loc_4_01F6
loc_4_020A:
        pop    eax
        pushad
        db  0BAh, 0E8h, 006h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        popad
        pop    edx
loc_4_0219:
        clc
        ret
loc_4_021B:
        pop    esi
        cmp    esi, 0
        je     SHORT loc_4_022F
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h
        db  0CDh, 020h, 005h, 000h, 08Bh, 004h
        jmp    SHORT loc_4_021B
loc_4_022F:
        pop    eax
        pushad
        db  0BAh, 0E8h, 006h, 000h, 000h
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h
        popad
        pop    ecx
        neg    ecx
        add    ecx, dword ptr [esi + 4]
        call   VCache_AdjustMinimum
loc_4_0248:
        stc
loc_4_0249:
        ret
        db  023h, 056h
_PTEXT ends
        end
