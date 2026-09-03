; vflatd_pcode_02.asm -- VFLATD object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

VFLATD_Service_00:
        mov    eax, 201h
        xor    ecx, ecx
        ret
        db  040h, 000h, 000h, 000h, 06Dh, 000h, 000h, 000h, 07Ch, 004h, 000h, 000h, 0FDh, 000h, 000h, 000h
        db  098h, 000h, 000h, 000h, 086h, 004h, 000h, 000h, 022h, 005h, 000h, 000h
VFLATD_PM_API:
        movzx  eax, byte ptr [ebp + 14h]
        cmp    eax, 6
        ja     SHORT loc_2_0036
        db  0FFh, 024h, 085h, 008h, 000h, 000h, 000h
loc_2_0036:
        or     dword ptr [ebp + 2ch], 1
        ret
loc_2_003B:
        and    dword ptr [ebp + 2ch], 0fffffffeh
        ret
loc_2_0040:
        mov    dword ptr [ebp + 1ch], 201h
        db  00Fh, 0B7h, 005h, 002h, 003h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        xor    eax, eax
        db  039h, 005h, 024h, 003h, 000h, 000h
        je     SHORT loc_2_0060
        db  0A1h, 0F8h, 002h, 000h, 000h
loc_2_0060:
        mov    dword ptr [ebp + 14h], eax
        db  0A1h, 0FCh, 002h, 000h, 000h
        mov    dword ptr [ebp + 18h], eax
        jmp    SHORT loc_2_003B
loc_2_006D:
        mov    edx, 0
        mov    ebx, 10000h
        mov    esi, 0a0000h
        movzx  eax, word ptr [ebp + 1ch]
        movzx  ecx, word ptr [ebp + 18h]
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
        shl    eax, 0ch
        jmp    NEAR PTR loc_2_011A
loc_2_0098:
        movzx  edx, byte ptr [ebp + 15h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp + 18h]
        add    ecx, 0fffh
        and    ecx, 0fffff000h
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp + 14h], eax
        mov    ecx, dword ptr [ebp + 18h]
        add    ecx, 0fffh
        shr    ecx, 0ch
        dec    ecx
        push   0
        push   80h
        push   92h
        push   ecx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        mov    dword ptr [ebp + 1ch], eax
        db  066h, 0A3h, 002h, 003h, 000h, 000h
        jmp    NEAR PTR loc_2_003B
loc_2_00FD:
        movzx  edx, byte ptr [ebp + 15h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 4]
        movzx  ecx, word ptr [ebp + 18h]
        xchg   edi, eax
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   edi, eax
loc_2_011A:
        call   sub_2_012A
        mov    dword ptr [ebp + 1ch], eax
        mov    dword ptr [ebp + 14h], edx
        jmp    NEAR PTR loc_2_003B
sub_2_012A:
        push   eax
        push   ebx
        push   esi
        push   ecx
        push   edi
        push   edx
        cmp    dword ptr [esp + 8], 64h
        ja     NEAR PTR loc_2_03D4
        db  066h, 083h, 03Dh, 002h, 003h, 000h, 000h, 000h
        jne    NEAR PTR loc_2_036A
        mov    eax, dword ptr [esp]
        test   eax, eax
        jne    NEAR PTR loc_2_03D4
        db  066h, 0A3h, 000h, 003h, 000h, 000h
        mov    eax, cr3
        db  0A3h, 093h, 002h, 000h, 000h
        add    dword ptr [esp + 14h], 0fffh
        shr    dword ptr [esp + 14h], 0ch
        mov    eax, dword ptr [esp + 10h]
        test   eax, 0fffh
        jne    NEAR PTR loc_2_03D4
        db  0A3h, 004h, 003h, 000h, 000h
        bsf    ecx, eax
        shr    eax, 0ch
        mov    dword ptr [esp + 10h], eax
        db  088h, 00Dh, 04Ah, 001h, 000h, 000h
        sub    cl, 8
        db  088h, 00Dh, 06Ah, 001h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 008h, 003h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        add    eax, 0fh
        and    al, 0f0h
        cmp    eax, 400h
        jae    SHORT loc_2_01BA
        mov    eax, 400h
loc_2_01BA:
        mov    dword ptr [esp + 14h], eax
        db  0A3h, 0FCh, 002h, 000h, 000h
        db  0C1h, 025h, 0FCh, 002h, 000h, 000h, 00Ch
        lea    eax, [eax + eax]
        push   eax
        push   9
        push   eax
        push   80080000h
        db  0CDh, 020h, 01Dh, 001h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_2_03D4
        mov    esi, eax
        db  0A3h, 0F8h, 002h, 000h, 000h
        call   sub_2_03DC
        je     NEAR PTR loc_2_03D4
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        db  003h, 035h, 0FCh, 002h, 000h, 000h
        db  001h, 035h, 033h, 001h, 000h, 000h
        db  08Bh, 01Dh, 004h, 003h, 000h, 000h
        shr    esi, 0ch
        shr    ebx, 0ch
        push   0
        push   ebx
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_03D4
        db  0A1h, 0F8h, 002h, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        dec    ecx
        push   0
        push   80h
        push   92h
        push   ecx
        push   eax
        db  0CDh, 020h, 07Ah, 000h, 001h, 000h
        add    esp, 14h
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 076h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     NEAR PTR loc_2_03D4
        db  066h, 0A3h, 002h, 003h, 000h, 000h
        db  08Bh, 015h, 0F8h, 002h, 000h, 000h
        db  08Ah, 00Dh, 04Ah, 001h, 000h, 000h
        shr    edx, cl
        db  089h, 015h, 04Ch, 001h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        shl    esi, 0ch
        shr    esi, cl
        db  089h, 035h, 053h, 001h, 000h, 000h
        add    edx, esi
        db  089h, 015h, 05Bh, 001h, 000h, 000h
        db  00Fh, 001h, 005h, 00Eh, 003h, 000h, 000h
        db  08Bh, 015h, 010h, 003h, 000h, 000h
        and    eax, 0fff8h
        add    edx, eax
        add    edx, 3
        db  089h, 015h, 06Eh, 001h, 000h, 000h
        add    edx, 1
        db  089h, 015h, 078h, 001h, 000h, 000h
        add    edx, 3
        db  089h, 015h, 07Fh, 001h, 000h, 000h
        pushfd
        cli
        sub    esp, 8
        sidt   [esp + 2]
        pop    eax
        pop    eax
        add    eax, 70h
        mov    bx, cs
        db  0BEh, 044h, 001h, 000h, 000h
        xchg   word ptr [eax + 2], bx
        xchg   word ptr [eax], si
        ror    esi, 10h
        xchg   word ptr [eax + 6], si
        ror    esi, 10h
        mov    ax, cs
        cmp    bx, ax
        jne    SHORT loc_2_0300
        db  0C6h, 005h, 03Ch, 001h, 000h, 000h, 0E9h
        db  081h, 0EEh, 041h, 001h, 000h, 000h
loc_2_0300:
        db  089h, 035h, 03Dh, 001h, 000h, 000h
        db  066h, 089h, 01Dh, 041h, 001h, 000h, 000h
        popfd
        mov    eax, 10076h
        db  0BEh, 09Bh, 005h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 014h, 003h, 000h, 000h
        mov    eax, 10098h
        db  0BEh, 0BFh, 000h, 000h, 000h
        db  0CDh, 020h, 090h, 000h, 001h, 000h
        db  089h, 035h, 018h, 003h, 000h, 000h
        db  066h, 0C7h, 005h, 00Eh, 003h, 000h, 000h, 000h, 000h
        db  081h, 03Dh, 004h, 003h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_036A
        db  0BFh, 068h, 001h, 000h, 000h
        db  0BEh, 075h, 001h, 000h, 000h
        mov    ecx, 7
        rep movsd
        db  081h, 02Dh, 09Ch, 002h, 000h, 000h, 00Dh, 000h, 000h, 000h
loc_2_036A:
        xor    edx, edx
        db  081h, 03Dh, 004h, 003h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_2_037D
        mov    edx, 0fffffff3h
loc_2_037D:
        db  0BEh, 090h, 001h, 000h, 000h
        add    esi, edx
        mov    edi, dword ptr [esp + 8]
        db  00Fh, 0B7h, 00Dh, 00Eh, 003h, 000h, 000h
        db  066h, 089h, 03Dh, 00Eh, 003h, 000h, 000h
        add    esi, ecx
        sub    edi, ecx
        je     SHORT loc_2_03A9
        jl     SHORT loc_2_03C9
        add    edi, esi
        std
loc_2_03A1:
        mov    ecx, 6
        rep movsb
        cld
loc_2_03A9:
        mov    esi, dword ptr [esp + 4]
        db  0BFh, 08Bh, 001h, 000h, 000h
        add    edi, edx
        mov    ecx, dword ptr [esp + 8]
        rep movsb
        db  00Fh, 0B7h, 005h, 002h, 003h, 000h, 000h
        db  08Bh, 015h, 0F8h, 002h, 000h, 000h
        jmp    SHORT loc_2_03D8
loc_2_03C9:
        sub    esi, 5
        add    edi, esi
        cld
        jmp    SHORT loc_2_03A1
loc_2_03D4:
        xor    eax, eax
        mov    edx, eax
loc_2_03D8:
        add    esp, 18h
        ret
sub_2_03DC:
        pushad
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        mov    edi, esi
        db  003h, 035h, 0FCh, 002h, 000h, 000h
        db  08Bh, 01Dh, 004h, 003h, 000h, 000h
        shr    esi, 0ch
        shr    edi, 0ch
        shr    ebx, 0ch
loc_2_03FA:
        db  0A1h, 008h, 003h, 000h, 000h
        shr    eax, 0ch
        push   40020000h
        push   eax
        push   ebx
        push   edi
        db  0CDh, 020h, 028h, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_2_0438
        add    edi, ebx
        cmp    edi, esi
        jb     SHORT loc_2_03FA
        db  0A1h, 008h, 003h, 000h, 000h
        shr    eax, 0ch
        push   40060000h
        push   eax
        push   ebx
        push   esi
        db  0CDh, 020h, 028h, 001h, 001h, 000h
        add    esp, 10h
        or     eax, eax
loc_2_0438:
        popad
        ret
sub_2_043A:
        mov    eax, cr3
        push   0
        push   1000h
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        mov    edi, eax
        mov    eax, esi
        shr    eax, 0ah
        mov    edi, dword ptr [edi + eax*4]
        and    edi, 0fffff000h
        push   0
        push   1000h
        push   edi
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        mov    edi, eax
        mov    eax, esi
        and    eax, 3ffh
        lea    edi, [edi + eax*4]
        ret
loc_2_047C:
        db  0E8h, 081h, 0FBh, 0FDh, 0FFh
        jmp    NEAR PTR loc_2_003B
loc_2_0486:
        db  066h, 083h, 03Dh, 002h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_2_0036
        db  081h, 03Dh, 0FCh, 002h, 000h, 000h, 000h, 000h, 040h, 000h
        ja     NEAR PTR loc_2_0036
        db  081h, 03Dh, 004h, 003h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    NEAR PTR loc_2_0036
        db  0FFh, 005h, 024h, 003h, 000h, 000h
        db  0A1h, 024h, 003h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        cmp    eax, 1
        ja     NEAR PTR loc_2_003B
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        db  0A1h, 0FCh, 002h, 000h, 000h
        shr    esi, 0ch
        shr    eax, 0ch
        push   0
        push   eax
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        call   sub_2_043A
        db  089h, 03Dh, 049h, 002h, 000h, 000h
        db  089h, 03Dh, 0FFh, 001h, 000h, 000h
        db  0E8h, 003h, 0FBh, 0FDh, 0FFh
        db  0BEh, 061h, 001h, 000h, 000h
        db  0B8h, 0F3h, 001h, 000h, 000h
        sub    eax, esi
        mov    dl, 0e9h
        xchg   byte ptr [esi], dl
        xchg   dword ptr [esi + 1], eax
        db  088h, 015h, 028h, 003h, 000h, 000h
        db  0A3h, 029h, 003h, 000h, 000h
        jmp    NEAR PTR loc_2_003B
loc_2_0522:
        db  066h, 083h, 03Dh, 002h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_2_0036
        db  083h, 03Dh, 024h, 003h, 000h, 000h, 000h
        je     NEAR PTR loc_2_0036
        db  0FFh, 00Dh, 024h, 003h, 000h, 000h
        db  0A1h, 024h, 003h, 000h, 000h
        mov    dword ptr [ebp + 1ch], eax
        jne    NEAR PTR loc_2_003B
        db  0FFh, 005h, 024h, 003h, 000h, 000h
        db  0E8h, 0A6h, 0FAh, 0FDh, 0FFh
        db  0FFh, 00Dh, 024h, 003h, 000h, 000h
        db  08Bh, 035h, 0F8h, 002h, 000h, 000h
        db  0A1h, 0FCh, 002h, 000h, 000h
        shr    esi, 0ch
        shr    eax, 0ch
        push   0
        push   eax
        push   esi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        db  0A1h, 029h, 003h, 000h, 000h
        db  08Ah, 015h, 028h, 003h, 000h, 000h
        db  088h, 015h, 061h, 001h, 000h, 000h
        db  0A3h, 062h, 001h, 000h, 000h
        jmp    NEAR PTR loc_2_003B
loc_2_059B:
        pop    eax
        pop    ecx
        pop    edx
        xchg   dword ptr [esp], eax
        push   eax
        push   edx
        push   ecx
        db  0FFh, 015h, 014h, 003h, 000h, 000h
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [esp], ecx
        or     eax, eax
        je     SHORT loc_2_0606
        push   eax
        sub    esp, 8
        sgdt   [esp + 2]
        pop    eax
        pop    eax
        db  03Bh, 005h, 010h, 003h, 000h, 000h
        je     SHORT loc_2_0605
        db  0A3h, 010h, 003h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 002h, 003h, 000h, 000h
        and    cl, 0f8h
        lea    eax, [ecx + eax + 3]
        mov    ecx, 0fffffff3h
        db  081h, 03Dh, 004h, 003h, 000h, 000h, 000h, 000h, 001h, 000h
        je     SHORT loc_2_05F3
        db  0A3h, 06Eh, 001h, 000h, 000h
        xor    ecx, ecx
loc_2_05F3:
        add    eax, 1
        db  089h, 081h, 078h, 001h, 000h, 000h
        add    eax, 3
        db  089h, 081h, 07Fh, 001h, 000h, 000h
loc_2_0605:
        pop    eax
loc_2_0606:
        ret
_PTEXT ends
        end
